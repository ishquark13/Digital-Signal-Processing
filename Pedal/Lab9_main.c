 /*
 * Quiz7.c
 *
 *  Created on: Nov 14, 2016
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
//#pragma DATA_SECTION(circularBuffer, ".johns")
float q; // was 0.6-0.8
extern float f;
extern float c;
extern float d;
//float circularBuffer[100000];
//float x_nn[11025];
float y1_n = 0;
float y_nn = 0;
float echo = 0;
//Uint32 j = 0;
Uint32 offset = 0;
int alpha = 7;




void internalADC_init() {
	InitAdc();
	EALLOW;
	SysCtrlRegs.HISPCP.all = 0x03; // make high speed clock equal to 25Mh for ADC
	AdcRegs.ADCTRL1.bit.CONT_RUN = 1; // no freerun mode off
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0;  // prescaler is 1 for bits 8-0 all 0 no multiplier
	AdcRegs.ADCTRL1.bit.CPS = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 0;
	//AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ = 0; // turn this off //
	//AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0; // this off too //
	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0;
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // conversion begin until after

	// new stuff from e2e T//
	 AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	 AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;   // Enable software trigger for SEQ1( In separate instructions)

	 AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)

	  // AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 1;// Enable SOCB from ePWM to start SEQ2
	 AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;         // Reset SEQ1
	 //AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1; //  // Enable software trigger for SEQ2 ( in separate instructions)

	 AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;  // Enable SEQ2 interrupt (every EO
}
int main () {

			DisableDog();
			InitPll(10,3); // (10,4) 1/4 of fper
			analogToDigitalConverter_init();
			//Sram_init();
			digitalToAnalogConverter_init();

			timer_init(150.0,22.6757);
			enable_timer_interrupt();
			timer_init2(150,135); // be faster was 166.667
			enable_timer_interrupt2();
			EALLOW;
			useGpio();
			internalADC_init();

	while(1) {
		EALLOW;
		//GpioDataRegs.GPATOGGLE.bit.GPIO1 = 0;
		while(!interruptTimerFlag);
		q = (AdcRegs.ADCRESULT0)/(60000.0)+ 0.1;
		//GpioDataRegs.GPADAT.bit.GPIO2 = 1;
				//x_nn[u] = (((float)analogToDigitalConverter_send())- 0x7FFF);
				//circularBuffer[j] =
				x_n = (((float)analogToDigitalConverter_send())- 0x7FFF);
				//circularBuffer[j] = x_n;
				y_n = (-1*c*x_n+d*(1-c)*x_n_minusOne+x_n_minusTwo-d*(1-c)*y_n_minusOne+c*y_n_minusTwo);

						x_n_minusTwo = x_n_minusOne;
						x_n_minusOne = x_n;
						y_n_minusTwo = y_n_minusOne;
						y_n_minusOne = y_n;

				 /*
				  * the wave
				  *
				  */
						y_h_n = x_n - y_l_n_minus1 - q*y_b_n_minus1;

						// set new recursive values
						//y_l_n_minus1 = y_l_n;
						//y_b_n_minus1 = y_b_n;
						// end

						y_b_n = f * y_h_n + y_b_n_minus1;

						// set new recursive values
						//y_b_n_minus1 = y_b_n;
						//end
						y_l_n = f*y_b_n + y_l_n_minus1;

						// set new recursive values
						//y_l_n_minus1 = y_l_n;
						//end
						y_l_n_minus1 = y_l_n;
						y_b_n_minus1 = y_b_n;



				/*
				 * control logic for the effects of cascade echo or just phaser, or none at all
				 */

				if (GpioDataRegs.GPADAT.bit.GPIO11)	{
					digitalToAnalogConverter_send((Uint16)((0.7*x_n + 0.6*y_b_n) + 0x7FFF));
					GpioDataRegs.GPADAT.bit.GPIO2 = 1;
					//timer_init2(166.667,166.1234);
				}
				else if (GpioDataRegs.GPADAT.bit.GPIO8) {
					digitalToAnalogConverter_send((Uint16)(x_n + 0x7FFF));
					GpioDataRegs.GPADAT.bit.GPIO1 = 1;
				}

				else if (GpioDataRegs.GPADAT.bit.GPIO10) {
					digitalToAnalogConverter_send(0.1 * (x_n) + (0.9)*(32767.00 * ((float)x_n-32767.00)/abs((float)x_n-32767.00)));
					}

				else
				//digitalToAnalogConverter_send((Uint16)((0.9*y_l_n + 0.1*y_h_n + 0.3*x_n) + 0x7FFF));
				digitalToAnalogConverter_send((Uint16)((0.6*x_n + 0.5*y_n ) + 0x7FFF)); // add remnants of delayed signals
				//timer_init2(166.667,110.1234);
				//digitalToAnalogConverter_send((Uint16)((0.5*x_n + 0.5*y_b_n) + 0x7FFF));

				GpioDataRegs.GPADAT.bit.GPIO2 = 0;
				interruptTimerFlag = 0;

	}
}
