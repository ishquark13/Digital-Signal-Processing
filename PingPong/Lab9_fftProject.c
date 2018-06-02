 // ========================================================================
//  This software is licensed for use with Texas Instruments C28x
//  family DSCs.  This license was provided to you prior to installing
//  the software.  You may review this license by consulting a copy of
//  the agreement in the doc directory of this library.
// ------------------------------------------------------------------------
//          Copyright (C) 2014-2015 Texas Instruments, Incorporated.
//                          All Rights Reserved.
// ========================================================================
//
//  File: Main.c -- Real-time RFFT with 12-bit ADC Input Test Example
//
//	TITLE:   DSP2833x Device Real-time Real FFT Test Program.
//
//=========================================================================
//  DESCRIPTION:
//
//    This program shows how to compute a real FFT with 12-bit real-time ADC
//    input and associated spectrum magnitude, phase. The input data is
//    collected by interrupt service routine function adc_isr(). The signal
//    input (can be any signal from signal generator or sensor) is excerted
//    in ADC input A0. The signal is sampled by epwm module and sampling
//    frequency is defined in Lab.h. Here is 48kHz. The dynamic range of the
//    input signal should be less than 0~3V. Real time real FFT is conducted
//    frame by frame. As long as one frame of data is collected by adc_isr,
//    it will set the FFTStartFlag. Whenever the main program detect the
//    FFTStartFlag set, the FFT calculate starts. The FFTStartFlag is cleared
//    after the calculation finished.
//
//    The input buffer must be aligned to a multiple of the FFT size if using
//    RFFT_adc_f32.  If you do not wish to align the input buffer then use the
//    RFFT_adc_f32u function. In this case, the section alignment #pragma can
//    be commented.  However, using this function will reduce cycle performance
//    of the algorithm.
//
//
// FUNCTIONS:
//
//    void RFFT_adc_f32 (RFFT_ADC_F32_STRUCT *)
//    void RFFT_f32_mag (RFFT_F32_STRUCT *)
//    void RFFT_f32_phase (RFFT_F32_STRUCT *)
//
//    Where RFFT_ADC_F32_STRUCT is a structure defined as:
//
//	 typedef struct {
//	  	uint16_t   *InBuf;
//  	void	   *Tail;
//	 } RFFT_ADC_F32_STRUCT;
//
//    Where RFFT_F32_STRUCT is a structure defined as:
//
//    typedef struct {
//      float  *InBuf;
//      float  *OutBuf;
//      float  *CosSinBuf;
//      float  *MagBuf;
//      float  *PhaseBuf;
//      uint16_t FFTSize;
//      uint16_t FFTStages;
//    } RFFT_F32_STRUCT;
//
//  ASSUMPTIONS:
//
//	   * OutBuf of RFFT_F32_STRUCT has to be passed to Tail of
//       RFFT_ADC_F32_STRUCT
//	   * Input signal is stored in Signal.asm
//     * FFTSize must be a power of 2 (32, 64, 128, etc)
//     * FFTSize must be greater or equal to 32
//     * FFTStages must be log2(FFTSize)
//     * InBuf, OutBuf, CosSinBuf are FFTSize in length
//     * MagBuf and PhaseBuf are FFTSize/2+1 in length
//     * MagBuf and PhaseBuf are not used by this function.
//       They are only used by the magitude and phase calculation functions.
//
//  Watch Variables:
//
//      InBuf(RFFT_ADC_F32_STRUCT)    Input buffer
//      InBuf(RFFT_F32_STRUCT)		  Not used
//		Tail						  Stored the address of OutBuf
//      OutBuf						  Output buffer
//      CosSinBuf        			  Twiddle factor buffer
//      MagBuf       				  Magnitude buffer
//      PhaseBuf                      Phase buffer
//      j							  Index of normalized digital frequency component
//      freq						  Real analog frequency of raw signal
//
//###########################################################################
// $TI Release: C28x Floating Point Unit Library V1.40.00.00 $
// $Release Date: Mar 10, 2014 $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "math.h"
#include "fpu_rfft.h"
#include <DSP28x_Project.h>
#include "OneToOneI2CDriver.h"
#include <DSP2833x_Xintf.h>
#include <DSP2833x_XIntrupt.h>
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

#define RFFT_STAGES     9					// 2^exponent size i.e. 512
#define RFFT_SIZE       (1 << RFFT_STAGES)

#define ADC_BUF_LEN         RFFT_SIZE   // ADC buffer length
#define ADC_SAMPLE_PERIOD   3124            // 3124 = (3125-1) = 48 KHz sampling w/ 150 MHz SYSCLKOUT

#define F_PER_SAMPLE        48000.0L/(float)RFFT_SIZE  //Internal sampling rate is 48kHz

#define N  256
#define PI2 6.28318530718
#define OFFSET 0.118258 // to get the right frequency use: N/(PI2*OFFSET)

RFFT_ADC_F32_STRUCT rfft_adc;
RFFT_F32_STRUCT rfft;

float RFFToutBuff[RFFT_SIZE];                //Calculated FFT result
float RFFTF32Coef[RFFT_SIZE];        //Coefficient table buffer
float RFFTmagBuff[RFFT_SIZE/2+1];      //Magnitude of frequency spectrum

//--- Global Variables
uint16_t AdcBuf[ADC_BUF_LEN];					// ADC buffer allocation

volatile uint16_t FFTStartFlag = 0;			// One frame data ready flag// was changed to selectFlag in dmach2 interrupt

uint16_t DEBUG_TOGGLE = 1;					// Used in realtime mode investigation

// Prototype statements for functions found within this file.
//interrupt void adc_isr(void);

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

/*********************************************************************
*Dma_inits
*
*
*
*********************************************************************/
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




/**********************************************************************
* Function: main()
*
* Description: Main function for C2833x Real-time RFFT
**********************************************************************/
void main(void)
{
	EALLOW;
	PieVectTable.DINTCH1 = isr1;
	PieVectTable.DINTCH3 = isr3;
	uint16_t i,j;
   	float freq;                       // Frequency of single-frequency-component signal

//--- CPU Initialization replace with own inits
	// InitSysCtrl();						// Initialize the CPU (FILE: SysCtrl.c)
	// InitGpio();							// Initialize the shared GPIO pins (FILE: Gpio.c)
	// InitPieCtrl();						// Initialize and enable the PIE (FILE: PieCtrl.c)

//--- Peripheral Initialization
	//InitAdc();							// Initialize the ADC (FILE: Adc.c)

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   	EALLOW;  // This is needed to write to EALLOW protected register
   	//PieVectTable.ADCINT = &adc_isr;
   	//EDIS;    // This is needed to disable write to EALLOW protected registers

    rfft_adc.Tail = &rfft.OutBuf;				//Link the RFFT_ADC_F32_STRUCT to
   												//RFFT_F32_STRUCT. Tail pointer of
   												//RFFT_ADC_F32_STRUCT is passed to
   												//the OutBuf pointer of RFFT_F32_STRUCT
    rfft.FFTSize  = RFFT_SIZE;				//Real FFT size
    rfft.FFTStages = RFFT_STAGES;			//Real FFT stages
    rfft_adc.InBuf = &pongCalc[0];				//Input buffer																	//pongCalc
    rfft.OutBuf = &RFFToutBuff[0];   			//Output buffer
    rfft.CosSinBuf = &RFFTF32Coef[0];	//Twiddle factor
    rfft.MagBuf = &RFFTmagBuff[0];		//Magnitude output buffer

    RFFT_f32_sincostable(&rfft);  				//Calculate twiddle factor

 	//Clean up output buffer
    for (i=0; i < RFFT_SIZE; i++)
    {
    	 RFFToutBuff[i] = 0;
    }

   	//Clean up magnitude buffer
    for (i=0; i < RFFT_SIZE/2; i++)
    {
   		 RFFTmagBuff[i] = 0;
    }

/************************************************************************************
*hardware inits
*
*
*
*
**********************************************************************************/
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


//--- Enable global interrupts
	//asm(" CLRC INTM, DBGM");			// Enable global interrupts and realtime debug

//--- Main Loop
	while(1)							// endless loop - wait for an interrupt
	{
		if(selectFlag)				// If one frame data ready, then do FFT
		{	/////**********************************************pongCalc references ADC********************************************************
			RFFT_adc_f32u(&rfft_adc);   // This version of FFT doesn't need buffer alignment
			RFFT_f32_mag(&rfft);		// Calculate spectrum amplitude

   			j = 1;
   			freq = RFFTmagBuff[1];
   			for(i=2;i<RFFT_SIZE/2+1;i++)
   			{
   				//Looking for the maximum valude of spectrum magnitude
      			if(RFFTmagBuff[i] > freq)
      			{
          			j = i;
          			freq = RFFTmagBuff[i];
      			}
   			}

   			freq = F_PER_SAMPLE * (float)j;	//Convert normalized digital frequency to analog frequency

			selectFlag = 0;			   	//Start collecting the next frame of data // set in DMAcH2 to 1
		}
		asm(" NOP");
	}
} //end of main()

