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
#include "analogToDigitalConverter.h"
#include "digitalToAnalogConverter.h"
#include "audioCntrl.h"
#include "timer.h"
//#pragma DATA_SECTION(circularBuffer, ".johns")
//#pragma DATA_SECTION(bufferForInterpolation, ".johns2")
#define MAX_SRAM_SIZE 0x100000

Uint32 i = 0;
Uint32 l = 0;
Uint32 p = 0;
Uint32 t = 0;
int temp = 0;
int startAgain = 1;
Uint32 mixedSample = 0;

//Uint16 circularBuffer[128000];
extern Uint16 bufferForInterpolation[];
/*takes in two parameters, the first specifies the system clock, the next is the duration of the clock in us (approx)*/


void decimation (int n) {
	do {
		if ( p % n) {
			p++;
			continue;
		}
		bufferForInterpolation[t] = circularBuffer[p];
		p++;
		t++;
	}while(p <128000 );

	if (p <= 128000) {
		GpioDataRegs.GPADAT.bit.GPIO0 = 1;
		p = 0;
	}
}


void interpolation() {
	Uint16 interp;
	l = 0;
	while (l<256000) {
	bufferForInterpolation[2*l] = circularBuffer[l];
	interp = (circularBuffer[l]/2)+(circularBuffer[l+1]/2);
	bufferForInterpolation[(2*l)+1] = interp;
	l++;
	}
//	if (l == 256000) {
//		l =0;
//	}
}


__interrupt void sample_time() {

	// do stuff: sample the ADC depending on the parameter of the init_timer func
	/*
	 * Once the buffer is filled, stop recording and light up LED[0]
	Recording cannot happen again until switch[0] has been toggled off and back on
	 */

	if(stateFlag == 1) {

		circularBuffer[i] = analogToDigitalConverter_send();
		i++;
		temp++;

		if (i >= 128000) {
				i = 0;
				CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
				GpioDataRegs.GPADAT.bit.GPIO0 = 1;
				/*
				 *  once buffer is full clear the flag to
				 *  keep the information stored
				 */
				startAgain = 0;
				CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
				ConfigCpuTimer(&CpuTimer1,150,32.2215);
			}

	}

	else if (stateFlag == 2) {

		mixedSample = analogToDigitalConverter_send();
		mixedSample >>= 1;
		mixedSample += circularBuffer[i]/2;
		circularBuffer[i] = mixedSample;
		i++;

		if (i >= 128000) {
						i = 0;
						CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
						GpioDataRegs.GPADAT.bit.GPIO0 = 1;
						/*
						 *  once buffer is full clear the flag to
						 *  keep the information stored
						 */
						startAgain = 0;
						CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
						ConfigCpuTimer(&CpuTimer1,150,32.2215);
					}

			}

	else if (stateFlag == 4) {
		//interpolation();
		digitalToAnalogConverter_send(circularBuffer[i]);
				i++;

				if (i >= 128000) {
								i = 0;
								CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
								GpioDataRegs.GPADAT.bit.GPIO0 = 0;
								/*
								 *  once buffer is full clear the flag to
								 *  keep the information stored
								 */
								startAgain = 0;
								CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
								ConfigCpuTimer(&CpuTimer1,150,32.2215);
							}
	}

	else if (stateFlag == 3) {

			digitalToAnalogConverter_send(bufferForInterpolation[i]);
					i++;

					if (i >= 256000) {
									i = 0;
									CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
									GpioDataRegs.GPADAT.bit.GPIO0 = 0;
									/*
									 *  once buffer is full clear the flag to
									 *  keep the information stored
									 */
									startAgain = 0;
									CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
									ConfigCpuTimer(&CpuTimer1,150,32.2215);
								}
	}
	else if (stateFlag == 9) {
		//decimation(2);
		digitalToAnalogConverter_send(bufferForInterpolation[i]);
						i++;

						if (i >= 64000) {
										i = 0;
										CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
										GpioDataRegs.GPADAT.bit.GPIO0 = 0;
										/*
										 *  once buffer is full clear the flag to
										 *  keep the information stored
										 */
										startAgain = 0;
										CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
										ConfigCpuTimer(&CpuTimer1,150,32.2215);
									}
	}

						else if (stateFlag == 11) {
					//decimation(2);
					digitalToAnalogConverter_send(circularBuffer[i]);
									i++;

						if (i >= 128000) {
										i = 0;
										CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
										GpioDataRegs.GPADAT.bit.GPIO0 = 0;
										/*
										 *  once buffer is full clear the flag to
										 *  keep the information stored
										 */
										startAgain = 0;
										CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
										//ConfigCpuTimer(&CpuTimer1,150,32.2215);
									}
						}
						else if (stateFlag == 10) {
											//decimation(2);
											digitalToAnalogConverter_send(circularBuffer[i]);
															i++;

												if (i >= 128000) {
																i = 0;
																CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
																GpioDataRegs.GPADAT.bit.GPIO0 = 0;
																/*
																 *  once buffer is full clear the flag to
																 *  keep the information stored
																 */
																startAgain = 0;
																CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
																//ConfigCpuTimer(&CpuTimer1,150,32.2215);
															}


	}
	CpuTimer1Regs.TCR.bit.TIF |= 0x0001;

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
		PieVectTable.XINT13 = sample_time;
		IER |=  M_INT13;
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EINT;
		CpuTimer1Regs.TCR.bit.TSS = 0;	// start the timer
		EDIS;
}


