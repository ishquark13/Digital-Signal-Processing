/*
 * Chorus.c
 *
 *  Created on: Jun 22, 2017
 *      Author: Ish's Master Race PC
 */


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
#include "iir.h"
#include "Lab7_IIR.h"
#include <DSP2833x_GlobalPrototypes.h>
#include "timer2.h"
#define N 32768
#define LP 42
#pragma DATA_SECTION(y, ".johns");
#pragma DATA_SECTION(chorus, ".johns");
float y[N];
float chorus[N];
extern float f;
float debug;
float outputOfFIR;
Uint16 voltage1;
Uint16 voltage2;

Uint32 j = 0;
Uint16 LFO_value = 0;
Uint32 lfo = 0;

float filter_taps_LP_72[] = {0.006960,0.004870,0.006450,0.008260,0.010291,0.012532,0.014961,0.017540,0.020242,0.023029,0.025833,0.028627,
0.031341,0.033929,0.036335,0.038508,0.040398,0.041964,0.043171,0.043988,0.044403,0.044403,0.043988,0.043171,0.041964,0.040398,0.038508,
0.036335,0.033929,0.031341,0.028627,0.025833,0.023029,0.020242,0.017540,0.014961,0.012532,0.010291,0.008260,0.006450,0.004870,
0.006960};


__interrupt void readADC() {

// do stuff: sample the ADC after a single sequence and call results in main
// Reinitialize for next ADC sequence

	voltage1 = AdcRegs.ADCRESULT0 >>4;
	voltage2 = AdcRegs.ADCRESULT1 >>4;

	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
}

void ADC(void) {

	InitAdc(); // TI function for ADC cal
	EALLOW; // make sure the correct clocks are enabled
	PieVectTable.ADCINT = &readADC;
	SysCtrlRegs.HISPCP.all = 0x03; // make high speed clock equal to 25Mhz for ADC

	AdcRegs.ADCTRL1.bit.SUSMOD = 00;  // Emulation Suspend mode ignored
	AdcRegs.ADCTRL1.bit.ACQ_PS = 5;  //  5 ADCCLK periods
	AdcRegs.ADCTRL1.bit.CPS = 0;	      //  divide by 1
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;  // Start Stop mode
	AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0;  //Sequencer Override Disabled
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;   // Cascaded mode enabled

	AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;  //Core clock divided by 18
	// Found this by trial and error.  Could need to be changed // given different sensor inputs

	AdcRegs.ADCTRL3.bit.SMODE_SEL = 1;  //Set to Simultaneous sampling mode

	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 8;  // set so that A0 BO, A1 B1, A2 B2, A3 B3, A4 B4 are
	//  sampled

	AdcRegs.ADCREFSEL.bit.REF_SEL = 0;  // Internal reference used

	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0;  // set these 5 fields so that the order of ADC channels
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1;  // sampled is A0,B0,A1,B1,A2,B2,A3,B3
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2;
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3;
	//AdcRegs.ADCCHSELSEQ2.bit.CONV04 = ??;
	//AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;      // Reset SEQ1
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;    // Clear INT SEQ1 Good practice to clear possible
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1; //start of conv begin
	// interrupt before enabling the interrupt.

	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  //Enable SEQ1’s interrupt
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0;  //Set Interrupt to occur every SEQ1 completion


	EALLOW;
	PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
	IER |= M_INT1; // Enable CPU Interrupt 1
	EINT;          // Enable Global interrupt INTM
	ERTM;		   // Enable Global realtime interrupt DBGM
	EDIS;
	 // example to grab data: q = (AdcRegs.ADCRESULT0)
}


int main () {

			DisableDog();
			InitPll(10,3); // (10,4) 1/4 of fper
			//Sram_init();
			analogToDigitalConverter_init();
			digitalToAnalogConverter_init();

			timer_init(150.0,22.6757);
			enable_timer_interrupt();
			timer_init2(150,20000); // be faster was 10000
			enable_timer_interrupt2();
			EALLOW;
			//useGpio();
			DINT;
			InitPieCtrl();
			IER = 0x0000;
			IFR = 0x0000;
			InitPieVectTable();

			ADC();




	while(1) {

		/*EALLOW;
		//GpioDataRegs.GPATOGGLE.bit.GPIO1 = 0;
		while(!interruptTimerFlag);
				// keep track of index below

				j   %= N;
				LFO_value = (Uint16) (f * 40000); //(123811) normalize to 20-40 ms LFO rate on a sine wave
				// sample input

				x_n = (((float)analogToDigitalConverter_send()));
				y[j] = x_n;
				debug = 0.6 * y[j] + 0.4* y[(j - LFO_value)& (0x7FFF)];
				if (GpioDataRegs.GPADAT.bit.GPIO8) {
				// add a lowpass filter here
//					for (int a = 0; a < 42; a++) {
//						outputOfFIR += filter_taps_LP_72[a] * chorus[(j-a) & (0x00FF)];
//					}
				digitalToAnalogConverter_send((Uint16)(debug)); // assign debug variable to output chorus to debug sound-out
				}
				else {
					digitalToAnalogConverter_send(analogToDigitalConverter_send());
				}
				j++;
				// check for max length
				if(j >= 65536) {
					j = 0;
				}
				// toggle flag
				interruptTimerFlag = 0;
				// turn on LED*/
	}
}


