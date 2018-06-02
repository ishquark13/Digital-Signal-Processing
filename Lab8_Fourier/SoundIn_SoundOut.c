/*
 * SoundIn_SoundOut.c
 *
 *  Created on: Nov 19, 2016
 *      Author: Ish's Master Race PC
 *      TA: Raz Aloni
 *      Lab8 part b
 *      Function: to have block based sound-in and sound-out with the 3 DMA
 *      channels in conjunction with the McBSP.
 */
#include <DSP28x_Project.h>
#include "OneToOneI2CDriver.h"
#include <DSP2833x_Xintf.h>
#include<DSP2833x_XIntrupt.h>
#include "timer.h"
#include "analogToDigitalConverter.h"
#include "digitalToAnalogConverter.h"
#include "DSP2833x_CpuTimers.h"
#include <DSP2833x_SysCtrl.h>
#include "audioCntrl.h"
#include "Sram.h"
#include "digitalToAnalogConverter.h"
#include "hammingWindow.h"
#include <DSP2833x_GlobalPrototypes.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define N  256
#define PI2 6.28318530718
#define OFFSET 0.118258 // to get the right frequency use: N/(PI2*OFFSET)
// function prototypes
// variables
void DFT();


/*
 * variable for functions and main loop
 */

float angle = 0;
int16 dacOutput = 0; // change this to be an int so -7FFF can bias out
Uint16 k = 0;
Uint16 n = 0;
/*
 * variable for Real-Time-DFT:
 */

float maxFrequency = 0;
float maxGain      = 0;
float Xreal[N/2] = {0};
float Ximaginary[N/2] = {0};
float Xmagnitude[N/2] = {0};
bool swap = true;
int selectFlag = 0;

Uint16 dataForMcBSP[2] = {0x6969,0x0420};
/*
 * make two ping-pong buffers that are used for data in data out
 * and intermediate processing...
 */

Uint16 pingBufferIn[N] = {0};
Uint16 pongBufferIn[N] = {0};
Uint16 pingBufferOut[N/2] = {0};	// this needs to be N/2 for part 3 for and in last for-loop
Uint16 pongBufferOut[N/2] = {0};	// for arguments of symmetry

/*
 * define special pointer to the above arrays as instructed in the
 * TMS320x2833x manual for better use between the available data pointers
 * and the ones to be used for calculations
 * (in part c)
 */

//TODO: figure out how to do the above comment with current arrays

Uint16* pingData = pingBufferIn;
Uint16* pingCalc = pingBufferOut;
Uint16* pongData = pongBufferIn;
Uint16* pongCalc = pongBufferOut;

void align() {
	digitalToAnalogConverter_send(analogToDigitalConverter_send());
}
__interrupt void isr1() {
	EALLOW;
	PieCtrlRegs.PIEACK.bit.ACK7 = 1;
}

__interrupt void isr3() {
	EALLOW;
	PieCtrlRegs.PIEACK.bit.ACK7 = 1;
}

__interrupt void dma2_swap() {
	EALLOW;
	PieCtrlRegs.PIEACK.bit.ACK7 = 1;

	// need to be able to swap the pingData with the pongData
	// and the pingCalc with pongCalc

	if (swap) {
		swap = false;

		pingData = pongBufferIn;
		pongData = pingBufferIn;
		pingCalc = pongBufferOut;
		pongCalc = pingBufferOut;
	}
	else {
		swap = true;

		pingData = pingBufferIn;
		pongData = pongBufferIn;
		pingCalc = pingBufferOut;
		pongCalc = pongBufferOut;
	}

	 DmaRegs.CH2.DST_ADDR_SHADOW = (Uint32) &pingData[1];
	 DmaRegs.CH3.SRC_ADDR_SHADOW = (Uint32) &pingCalc[0];
	 selectFlag =1;
}



void directMemoryAccess_init_ch1() {

    // this inits the DMa to a default wait-state that can be configured
    DMAInitialize();

    EALLOW;

    // need two words per burst to transfer a float
    DmaRegs.CH1.BURST_SIZE.all = 1;
    DmaRegs.CH1.SRC_BURST_STEP = 1;	// Burst_step increases the pointer value at the address of the src and dest by 1
    DmaRegs.CH1.DST_BURST_STEP = 1;
    //TODO: fix these comments and order
    DmaRegs.CH1.DST_TRANSFER_STEP = -1;
    DmaRegs.CH1.SRC_TRANSFER_STEP = -1;// pointer -- for each successful transfer

    // one burst per transfer
    DmaRegs.CH1.TRANSFER_SIZE = 0;

    // destination address is McBSPB Transmit register 2
    DmaRegs.CH1.DST_ADDR_SHADOW = (Uint32) &McbspbRegs.DXR2.all;

    // source address is the garbage data array that starts transactions
    DmaRegs.CH1.SRC_ADDR_SHADOW = (Uint32) &dataForMcBSP[0];

    DmaRegs.CH1.MODE.bit.CHINTE = 0;
    // no interrupts? for part 3
    DmaRegs.CH1.MODE.bit.OVRINTE = 0;
    // no overflow interrupts either
    DmaRegs.CH1.MODE.bit.DATASIZE = 0;
    // "0" in this register sets data for 16 bits which is necessary for DAC
    DmaRegs.CH1.MODE.bit.PERINTSEL = 12;
    // Timer 1 of the CPU needs to trigger this first channel as well as the third
    DmaRegs.CH1.MODE.bit.SYNCE = 0;
    // synchronous off
    DmaRegs.CH1.MODE.bit.CONTINUOUS = 1;
    // always repeat
    DmaRegs.CH1.MODE.bit.ONESHOT = 0;
    // disable this to have continuous
    DmaRegs.CH1.MODE.bit.PERINTE = 1;
    // need this to be able to be interrupted by external timer interrupts
    EINT;
    // or EINT


}

void directMemoryAccess_init_ch2() {

    EALLOW;


    DmaRegs.CH2.BURST_SIZE.all = 1;
    DmaRegs.CH2.SRC_BURST_STEP = 1;
    DmaRegs.CH2.DST_BURST_STEP = -1;
    DmaRegs.CH2.SRC_TRANSFER_STEP = -1;
    DmaRegs.CH2.DST_TRANSFER_STEP = 3;
    DmaRegs.CH2.TRANSFER_SIZE = 127;	// transfer size to N/2 for part 3
    DmaRegs.CH2.SRC_ADDR_SHADOW = (Uint32) &McbspbRegs.DRR2.all;
    // make sure the shadow register is pointing to the data
    DmaRegs.CH2.DST_ADDR_SHADOW = (Uint32) &pingData[1];

    DmaRegs.CH2.MODE.bit.CHINTE = 1;
    DmaRegs.CH2.MODE.bit.DATASIZE = 0;
    DmaRegs.CH2.MODE.bit.SYNCE = 0;
    DmaRegs.CH2.MODE.bit.CONTINUOUS = 1;
    DmaRegs.CH2.MODE.bit.ONESHOT = 0;
    DmaRegs.CH2.MODE.bit.CHINTMODE = 0;
    DmaRegs.CH2.MODE.bit.PERINTE = 1;
    DmaRegs.CH2.MODE.bit.OVRINTE = 0;
    DmaRegs.CH2.MODE.bit.PERINTSEL = 17;
    // McBSPB Receive will trigger the DMA transfer

    InitPieVectTable();                 // initialize pie vector generically
    EALLOW;
    PieCtrlRegs.PIEIER7.bit.INTx2 = 1;      // make sure the DMA channel 2
    IER |= M_INT7;
    PieVectTable.DINTCH2 = &dma2_swap;    // make sure this points to
    EnableInterrupts();						// the interrupt for the
    										// swapping of the DMA

    										// can be utilized with interrupts

}

void directMemoryAccess_init_ch3() {

    EALLOW;


    DmaRegs.CH3.BURST_SIZE.all = 0;
    DmaRegs.CH3.SRC_BURST_STEP = 0;
    DmaRegs.CH3.DST_BURST_STEP = 0;
    DmaRegs.CH3.SRC_TRANSFER_STEP = 1;
    DmaRegs.CH3.DST_TRANSFER_STEP = 0;
    DmaRegs.CH3.TRANSFER_SIZE = 127;	// make n/2 for part 3
    DmaRegs.CH3.SRC_ADDR_SHADOW = (Uint32) &pingCalc[0];
    //// McBSPA Receive will trigger the DMA transfer
    DmaRegs.CH3.DST_ADDR_SHADOW = (Uint32) &McbspaRegs.DXR1.all;

    DmaRegs.CH3.MODE.bit.CHINTE = 1;
    DmaRegs.CH3.MODE.bit.DATASIZE = 0;
    DmaRegs.CH3.MODE.bit.SYNCE = 0;
    DmaRegs.CH3.MODE.bit.CONTINUOUS = 1;
    DmaRegs.CH3.MODE.bit.ONESHOT = 0;
    DmaRegs.CH3.MODE.bit.CHINTMODE = 0;
    DmaRegs.CH3.MODE.bit.PERINTE = 1;
    DmaRegs.CH3.MODE.bit.OVRINTE = 0;
    DmaRegs.CH3.MODE.bit.PERINTSEL = 12;
    EINT;

}




int main() {
	EALLOW;
	PieVectTable.DINTCH1 = isr1;
	PieVectTable.DINTCH3 = isr3;
	// local variables to main:

	Uint16 i = 0;
	Uint16 currentPlace = 0;
	Uint16 k = 0;
	Uint16 n = 0;


	DisableDog();
	InitPll(10,3);
	analogToDigitalConverter_init();
	//Sram_init();
	digitalToAnalogConverter_init();
	timer_init(150.0,22.6757);
	InitPieCtrl();

	EALLOW;
	useGpio();
	SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;
	directMemoryAccess_init_ch1();
	directMemoryAccess_init_ch2();
	directMemoryAccess_init_ch3();
	// start using the DMA channels with these instructions
	DmaRegs.CH1.CONTROL.bit.RUN = 1;
	DmaRegs.CH2.CONTROL.bit.RUN = 1;
	DmaRegs.CH3.CONTROL.bit.RUN = 1;
	// the DMA channels are running...

	enable_timer_interrupt_DMA();
	/*
	 * logic to send the correct buffer out to the DAC
	 */

	while(1) {
		//align();
		if (selectFlag == 1) {
			currentPlace = 0;
			for(i = 0; i < N; i+=2) {
				// account for the +7FFF with less shifts to the pongData when grabbing ADC from McBSPB
				pongCalc[currentPlace++] = (((pongData[i+1]) << 14) | ((pongData[i]) >> 2));
			}
			//maxF, and maxM == 0; for both

			//DFT() or FFT(pongCalc);


			for(k = 0; k <N/2; k++) {

				//Xreal[k] = {0};
				//,Ximaginary[k] = {0};
				//,Xmagnitude[k]= {0};

				for(n = 0; n < N/2; n++) {			//change to n/2 for part 3 with the array sized down
					//angle = ((-2*PI2*k*n)/(N/2));	// change the dma setting for transfer size two
					dacOutput = (pongCalc[n] - 0x7FFF);

					//Xreal[k] +=dacOutput * cosf(angle);
					//Ximaginary[k] +=dacOutput * sinf(angle);

					DFT(n,k);
				}

				Xmagnitude[k] = sqrt((Xreal[k]* Xreal[k]+ Ximaginary[k]* Ximaginary[k]));

			if (Xmagnitude[k] > maxGain && (k <= 64)) {
				maxGain = Xmagnitude[k];
					maxFrequency = k* (N/(PI2*OFFSET));
				}
			}

			maxGain = 10*log10f(maxGain);
			selectFlag = 0;
		}

	}
}

void DFT(int n , int k) {
	angle = ((-2*PI2*k*n)/(N/2));
	Xreal[k] += dacOutput * cosf(angle);
	Ximaginary[k] += dacOutput * sinf(angle);
}

void FFT(float*pongCalc, Uint16 currentIndex) {

	/*
	 * initialize all the variables that will be used
	 */
	int tempx = 0;
	int tempy = 0;


	Uint16 n = 0;
	Uint16 maxIndex = 0;
	Uint16 index = 0;
	Uint16 j     = 0;
	Uint16 i_n   = 0;
	Uint16 i     = 0;

	float omega_hold = 0;
	float omega_wrap = 0;
	float omega_loop = 0;
	float omega_pi   = 0;
	float omega_start = 0;
	float theta      = 0;
	float temp_wrap  = 0;
	float omega_temp = 0;

	/*
	 * the indexing will be changed every time so this bottom snippet will
	 * find the proper indexing for all the conditions
	 * normally the FFT will be only correct to (N/2)-1
	 */

	n = currentIndex *2;
	j = 1;
	for(i = 1; i<n; i+=2) {
		if(j>1) {
			tempx = pongCalc[j-1];
			pongCalc[j-1] = pongCalc[i-1];
			pongCalc[i-1] = tempx;

			tempy = pongCalc[j];
			pongCalc[j] = pongCalc[i];
			pongCalc[i] = tempy;
		}

		index = currentIndex;

		while(index>=2 && j>index) {
			j -= index;
			index >>= 1;
		}
		j+= index;
	}

	// after indexing everything correctly we can now do the butterfly and
	// use the Daniel-Lanzcos lemma explained in the lab report

	maxIndex = 2;

	while(n > maxIndex) {
		i_n = maxIndex << 1;
		theta = -(2*(3.141659)/maxIndex);
		omega_hold = sinf(0.5*theta);
		omega_loop = -2.0*omega_hold * omega_hold;
		omega_pi  = sinf(theta);
		omega_wrap = 1;
		omega_start = 0;

		for(index = 1; index < maxIndex; index+= 2) {
			for(i = index; i <= n; i+= i_n ) {
				j = i + maxIndex;
				// swapping occurs here if the pong buffer is switched at any point
				// external to the FFT

				temp_wrap = omega_wrap*pongCalc[j-1] - omega_start *pongCalc[j];
				omega_temp = omega_wrap*pongCalc[j] - omega_start *pongCalc[j-1];
				pongCalc[j-1] = pongCalc[i-1] - temp_wrap;
				pongCalc[j]   = pongCalc[i] - omega_temp;
				pongCalc[i-1] += temp_wrap;
				pongCalc[i] += omega_temp;



			}
		}


	}
}
