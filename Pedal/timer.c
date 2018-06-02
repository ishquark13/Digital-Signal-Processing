/*
 * timer.c
 *
 *  Created on: Oct 14, 2016
 *      Author: Ish's Master Race PC
 */
#include <DSP28x_Project.h>
#include <DSP2833x_XIntrupt.h>
#include <DSP2833x_CpuTimers.h>
#include <DSP2833x_SysCtrl.h>
#include <DSP2833x_XIntrupt.h>
#include <DSP2833x_GlobalPrototypes.h>
#include "analogToDigitalConverter.h"
#include "digitalToAnalogConverter.h"
#include "audioCntrl.h"
#include "timer.h"
#include <stdbool.h>
#define MAX_SRAM_SIZE 0x100000

Uint32 interruptTimerFlag = 0;
Uint32 i = 0;
Uint32 l = 0;
Uint32 p = 0;
Uint32 t = 0;
int temp = 0;
int startAgain = 1;
Uint32 mixedSample = 0;
/*takes in two parameters, the first specifies the system clock, the next is the duration of the clock in us (approx)*/


//void decimation (int n) {
//	do {
//		if ( p % n) {
//			p++;
//			continue;
//		}
//		bufferForInterpolation[t] = circularBuffer[p];
//		p++;
//		t++;
//	}while(p <128000 );
//
//	if (p <= 128000) {
//		GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//		p = 0;
//	}
//}


//void interpolation() {
//	Uint16 interp;
//	l = 0;
//	while (l<256000) {
//	bufferForInterpolation[2*l] = circularBuffer[l];
//	interp = (circularBuffer[l]>>1)+(circularBuffer[l+1]>>1);
//	bufferForInterpolation[(2*l)+1] = interp;
//	l++;
//	}
////	if (l == 256000) {
////		l =0;
////	}
//}


__interrupt void setFlag() {

	// do stuff: sample the ADC depending on the parameter of the init_timer func
	/*

 * This interrupt minimizes processing and leaves it all in main in order to do this the only thing done is the setting of the timerflag
	 * and clearing the interrupt flag.
	 */
	GpioDataRegs.GPADAT.bit.GPIO1 = 1;
	interruptTimerFlag = 1;
	GpioDataRegs.GPADAT.bit.GPIO1 = 0;
//	GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;
}

void timer_init(float a, float b) {
		EALLOW;
		InitCpuTimers();
		ConfigCpuTimer(&CpuTimer1,a,b);
		EDIS;
		// end of timers
}

void enable_timer_interrupt() {

		EALLOW;
		PieVectTable.XINT13 = setFlag;
		IER |=  M_INT13;
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EINT;
		CpuTimer1Regs.TCR.bit.TSS = 0;	// start the timer
		EDIS;
}


