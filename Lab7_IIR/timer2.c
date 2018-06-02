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

float x1 = 100;
int y1 = 0;
float f = 0;
int state_dir = 0;
float d = 0;
float c = 0;

__interrupt void modulate() {

	// do stuff: sample the ADC depending on the parameter of the init_timer func

	/*
	 * this will modulate the wah-wah effect in a triangle wave from 100hz-3000hz 5 times a second.
	 * the first and second for loop will give the fc used in the main c-file
	 *
	 */


//		if (state_dir == 1) {
//			f = 2*sinf((PI*x1)/44100);
//					x1--;
//					if (x1 == 100.0) {
//						state_dir = 0;
//					}
//			}
//
//		else{
//		f = 2*sinf((PI*x1)/((float)44100));
//		x1++;
//
//			if(x1 == 3000.0) {	//3000
//				state_dir = 1;
//			}
//		}

	if (state_dir == 1) {
				f = 2*sinf(PI*x1/44100);
				d = -1*cosf((2*PI*x1)/44100);
				c = tanf(2*PI*x1/44100) - 1/(tanf(2*PI*x1/44100) + 1);
						x1--;
						if (x1 == 200.0) {
							state_dir = 0;
						}
				}

			else{
				f = 2*sinf(PI*x1/44100);
				d = -1*cosf((2*PI*x1)/44100);
				c = tanf(2*PI*x1/44100) - 1/(tanf(2*PI*x1/44100) + 1);
			x1++;

				if(x1 == 2000.0) {	//3000
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
