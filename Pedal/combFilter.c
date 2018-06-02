/*
 * combFilter.c
 *
 *  Created on: Jun 13, 2017
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
#pragma DATA_SECTION(y, ".johns")

#define M 4000
float y[128000];
float y1_n = 0;
float y_nn = 0;
float feed = 0.4;
float q = 1;
Uint32 itr = 0;
Uint32 j = 0;
Uint32 k = 0;
Uint32 m = 0;
Uint32 x = 0;
Uint32 delay = 64000 - M;
Uint16 output;
int alphaMultiplier = 10;
float reverbOutput = 0;

//void internalADC_init() {
//	InitAdc();
//	EALLOW;
//	SysCtrlRegs.HISPCP.all = 0x03; // make high speed clock equal to 25Mh for ADC
//	AdcRegs.ADCTRL1.bit.CONT_RUN = 1; // no freerun mode off
//	AdcRegs.ADCTRL1.bit.ACQ_PS = 0;  // prescaler is 1 for bits 8-0 all 0 no multiplier
//	AdcRegs.ADCTRL1.bit.CPS = 0;
//	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0;
//	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = 0;
//	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 0;
//	//AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ = 0; // turn this off //
//	//AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0; // this off too //
//	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0;
//	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // conversion begin until after
//
//	// new stuff from e2e T//
//	 AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
//	 AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;   // Enable software trigger for SEQ1( In separate instructions)
//
//	 AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)
//
//	  // AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 1;// Enable SOCB from ePWM to start SEQ2
//	 AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;         // Reset SEQ1
//	 //AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1; //  // Enable software trigger for SEQ2 ( in separate instructions)
//
//	 AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;  // Enable SEQ2 interrupt (every EO
//}
int main () {

	DisableDog();
	InitPll(10,3); // (10,4) 1/4 of fper
	Sram_init();
	analogToDigitalConverter_init();
	digitalToAnalogConverter_init();
	//Sram_init();
	timer_init(150.0,22.6757);
	enable_timer_interrupt();
	//timer_init2(150,200); // be faster was 166.667
	//enable_timer_interrupt2();
	EALLOW;
	useGpio();
	//internalADC_init();

	while(1) {
		EALLOW;
		//GpioDataRegs.GPATOGGLE.bit.GPIO1 = 0;
		while(!interruptTimerFlag);
		//q = (AdcRegs.ADCRESULT0)/(60000.0)+ 0.1; uncomment for adc usage
		y[j] = analogToDigitalConverter_send();

			if (GpioDataRegs.GPADAT.bit.GPIO8) {

				if (j < 441*1) {
						x = 128000-(( 441*1)-j);
						}
						else  {
							x = j - 441*1;
							}

				if (j < 441*6) {
						k = 128000-((441*6)-j);
						}
						else  {
							k = j - 441*6;
							}

				if (j < 441*9) {
						m = 128000-((441*9)-j);
						}
						else  {
							m = j - 441*9;
							}

				reverbOutput = (0.1 * y[j] + 0.2 * y[x] + 0.3 * y[k] + 0.4 * y[m]);
				digitalToAnalogConverter_send((Uint16)reverbOutput);
						j++;
			}

			else {
				digitalToAnalogConverter_send(analogToDigitalConverter_send());
			}
			if (j == 128000) {
					j = 0;
		}

		interruptTimerFlag = 0;

	}
}
