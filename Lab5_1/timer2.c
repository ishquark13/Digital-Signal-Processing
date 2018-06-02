/*
 * timer2.c
 *
 *  Created on: Oct 20, 2016
 *      Author: Ish's Master Race PC
 */


#include <DSP28x_Project.h>
#include <DSP2833x_XIntrupt.h>
#include <DSP2833x_CpuTimers.h>
#include <DSP2833x_SysCtrl.h>
#include <DSP2833x_XIntrupt.h>
#include "analogToDigitalConverter.h"
#include "digitalToAnalogConverter.h"
#include "audioCntrl.h"
#include "timer.h"
#define MAX_SRAM_SIZE 0x100000

/*takes in two parameters, the first specifies the system clock, the next is the duration of the clock in us (approx)*/
__interrupt void frequency10Khz() {

	// do stuff: sample the ADC depending on the parameter of the init_timer func
	/*
	 * Once the buffer is filled, stop recording and light up LED[0]
	Recording cannot happen again until switch[0] has been toggled off and back on
	 */

	digitalToAnalogConverter_send(analogToDigitalConverter_send());


	GpioDataRegs.GPATOGGLE.bit.GPIO5 = 1;

	CpuTimer1Regs.TCR.bit.TIF |= 0x0001;

}
void timer_init2(float a, float b) {
		EALLOW;
		InitCpuTimers();
		ConfigCpuTimer(&CpuTimer1,a,b);
		EDIS;
		// end of timers
}

void enable_timer_interrupt2() {

		EALLOW;
		PieVectTable.XINT13 = frequency10Khz;
		IER |=  M_INT13;
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EINT;
		CpuTimer1Regs.TCR.bit.TSS = 0;	// start the timer
		EDIS;
}
