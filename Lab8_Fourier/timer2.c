/*
 * timer2.c
 *
 *  Created on: Nov 14, 2016
 *      Author: Ish's Master Race PC
 */
#include "timer2.h"
#include <DSP28x_Project.h>
#include <DSP2833x_XIntrupt.h>
#include <DSP2833x_CpuTimers.h>
#include <DSP2833x_SysCtrl.h>
#include <DSP2833x_XIntrupt.h>
#include <DSP2833x_GlobalPrototypes.h>
#include "analogToDigitalConverter.h"
#include "digitalToAnalogConverter.h"
#include "math.h"

#define PI 3.14159265359

float a = 100;
int y = 0;
float f = 0;
int state_dir = 0;

__interrupt void modulate() {

	// do stuff: sample the ADC depending on the parameter of the init_timer func

	/*
	 * this will modulate the wah-wah effect in a triangle wave from 100hz-3000hz 5 times a second.
	 * the first and second for loop will give the fc used in the main c-file
	 *
	 */


		if (state_dir == 1) {
			f = 2*sinf((PI*a)/44100);
					a--;
					if (a == 100.0) {
						state_dir = 0;
					}
			}

		else{
		f = 2*sinf((PI*a)/((float)44100));
		a++;

			if(a == 3000.0) {
				state_dir = 1;
			}
		}




	CpuTimer2Regs.TCR.bit.TIF |= 0x0001;

}
void enable_timer_interrupt2() {

		EALLOW;
		PieVectTable.TINT2 = modulate;
		IER |=  M_INT14;
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EINT;
		CpuTimer2Regs.TCR.bit.TSS = 0;	// start the timer
		EDIS;
}

void timer_init2(float a, float b) {
		EALLOW;
		//InitCpuTimers();
		ConfigCpuTimer(&CpuTimer2,a,b);
		EDIS;
		// end of timers
}
