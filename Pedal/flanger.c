/*
 * flanger.c
 *
 *  Created on: May 9, 2017
 *      Author: Ish's Master Race PC
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
#include "iir.h"
#include "Lab7_IIR.h"
#include <DSP2833x_GlobalPrototypes.h>
#include "timer2.h"
#include "flanger.h"
#include "Delay.h"
#include "math.h"

#define MAX_BUF_SIZE 64000
extern double d_buffer[MAX_BUF_SIZE];

// change the below variables to static
static short samp_freq;
static double var_delay;
static short counter;
static short counter_limit;
static short control;
static short max_delay;
static short min_delay;
static double mix_vol;
static double delay_step;
short q = 0;

/*
This is the initialization function, basically
it passes the initialization parameters to the delay block
and initializes the flanger control variables.
*/
void Flanger_init(short effect_rate,short sampling,short maxd,short mind,double fwv,double stepd,double fbv) {
	Delay_Init(2,fbv,fwv,1);

	samp_freq = sampling;
	counter = effect_rate;
	control = 1;
	var_delay = mind;

	//User Parameters
	counter_limit = effect_rate;
	max_delay =  maxd;
	min_delay = mind;
	mix_vol = 1;
	delay_step = stepd;
}

/*This is the flanging process task
that uses the delay task inside*/
double Flanger_process(double xin) {
	double yout;

	yout = Delay_task(xin);
	return yout;
}

/*
This sweep function creates a slow frequency
ramp that will go up and down changing the
delay value at the same time. The counter
variable is a counter of amount of samples that
the function waits before it can change the delay.
*/
void Flanger_sweep(void) {
	if (!--counter) {
	    var_delay+=control*delay_step;

	    if (var_delay > max_delay) {
	        control = -1;
            }

            if (var_delay < min_delay) {
	        control = 1;
            }

	    Delay_set_delay(var_delay);
	    counter = counter_limit;
	}
}
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
	void main(void) {

		DisableDog();
		InitPll(10,3); // (10,4) 1/4 of fper
		Sram_init();
		analogToDigitalConverter_init();
		digitalToAnalogConverter_init();
		//Sram_init();
		timer_init(150.0,22.6757);// maybe dont need
		enable_timer_interrupt();// remove
		//timer_init2(150,200); // be faster was 166.667
		//enable_timer_interrupt2();
		EALLOW;
		useGpio();

	    double xin2;
	    double yout2;

	    //Flanger_init(80,30000,70,3,0.35,1,0.35); // change to 32k
	    //Flanger_init(100,30000,300,2,0.3,1,0.3);
	    Flanger_init(800,16000,70,2,0.3,1,0.3);
	   // Delay_Init(50,0.3,1,0.3);

	    while(1) {

	    	if(GpioDataRegs.GPADAT.bit.GPIO8) {
	            xin2 = (((double)analogToDigitalConverter_send())); // bias the signal
	            /*Apply the Delay_task function to the sample*/
	           yout2 = Flanger_process(0.7*xin2);
	           // yout2 = Delay_task(xin2);
	            /*Send the output value to your ADC or codec output*/
	            digitalToAnalogConverter_send((Uint16)(yout2));

	            Flanger_sweep();
	    	}
	      else {
	            		digitalToAnalogConverter_send(analogToDigitalConverter_send());

	            	}	// increment in the interrupt

	    }
	}
