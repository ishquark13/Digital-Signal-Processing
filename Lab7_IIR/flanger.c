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
#include "delay.h"
#include "math.h"

#define MAX_BUF_SIZE 256000
extern double d_buffer[MAX_BUF_SIZE];

static short samp_freq;
static double var_delay;
static short counter;
static short counter_limit;
static short control;
static short max_delay;
static short min_delay;
static double mix_vol;
static double delay_step;

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

	void main(void) {


			DisableDog();
			InitPll(10,3);
		//	I2C_O2O_Master_Init(0x3F, 150, 7);
		//	I2C_O2O_SendBytes(initLCD, sizeof(initLCD));
			InitCpuTimers();
			ConfigCpuTimer(&CpuTimer1,150,22.6757); // handles inputs
			ConfigCpuTimer(&CpuTimer2,150,200);// handles delays
			Sram_init();
			useGpio();
			digitalToAnalogConverter_init();
			analogToDigitalConverter_init();

	    double xin2;
	    double yout2;

	    Flanger_init(700,16000,70,2,0.3,1,0.3);

	    while(1) {
	    	if(GpioDataRegs.GPADAT.bit.GPIO8) {

	    		            /*Apply the Flanger_process function to the sample*/
	            /*When there's new sample at your ADC or CODEC input*/
	            /*Read the sample*/
	            xin2 = (((float)analogToDigitalConverter_send())- 0x7FFF);
	            /*Apply the Delay_task function to the sample*/
	            yout2 = Flanger_process(1.1*xin2);

	            /*Send the output value to your ADC or codec output*/
	            digitalToAnalogConverter_send((Uint16)(yout2 + 0x9FFF));

	            Flanger_sweep();
	    	}
	      else {
	            		digitalToAnalogConverter_send(analogToDigitalConverter_send());

	            	}	// increment in the interrupt

	    }
	}
