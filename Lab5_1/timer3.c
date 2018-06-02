
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
Uint32 k = 0;	// iterator to fill the circular buffer array
Uint32 n = 0; // iterator for part 8 buffer interpolation
Uint32 z = 0; // iterator for part 7 buffer interp
Uint32 a = 0;


Uint16 select;
/*takes in two parameters, the first specifies the system clock, the next is the duration of the clock in us (approx)*/
__interrupt void changeFrequency() {



	if (select == 1) {
		circularBuffer[k] = analogToDigitalConverter_send();
							k++;


							if (k == 128000) {
									k = 0;
									CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
									GpioDataRegs.GPADAT.bit.GPIO0 = 1;
									/*
									 *  once buffer is full clear the flag to
									 *  keep the information stored
									 */
									//startAgain = 0;
									CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
								}
		//digitalToAnalogConverter_send(analogToDigitalConverter_send());
	}
	if (select == 2) {
		digitalToAnalogConverter_send(analogToDigitalConverter_send());
	}
//	if (select == 3) {
//			digitalToAnalogConverter_send(analogToDigitalConverter_send());
//	}
//	if (select == 4) {
//		digitalToAnalogConverter_send(analogToDigitalConverter_send());
//	}
//	if (select == 5) {
//			digitalToAnalogConverter_send(analogToDigitalConverter_send());
//		}
//	if (select == 6) {
//		digitalToAnalogConverter_send(analogToDigitalConverter_send());
//	}
//
//	if (select == 7) {
//		digitalToAnalogConverter_send(bufferForInterpolation[z]);
//		z++;
//		if (z == 256000) {
//			z = 0;
//		}
//	}
//
	if (select == 8) {
		digitalToAnalogConverter_send(bufferForInterpolation[n]);
		n++;
		if (n == 256000) {
			n =0;
		}
	}

	if (select == 9) {
		digitalToAnalogConverter_send(circularBuffer[a]);
		a++;
		if (a ==128000) {
			a = 0;
		}
	}

	if (select == 10) {
		digitalToAnalogConverter_send(circularBuffer[a]);
		a++;
		if (a ==128000) {
			a = 0;
		}
	}

//
//
//
//
//	circularBuffer[k] = analogToDigitalConverter_send();
//			k++;
//
//
//			if (k == 128000) {
//					k = 0;
//					CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
//					GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//					/*
//					 *  once buffer is full clear the flag to
//					 *  keep the information stored
//					 */
//					//startAgain = 0;
//					CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
//				}
//
//
//	//digitalToAnalogConverter_send(analogToDigitalConverter_send());
	if (select == 4){
	circularBuffer[k] = analogToDigitalConverter_send();
					k++;


					if (k == 128000) {
							k = 0;
							CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
							GpioDataRegs.GPADAT.bit.GPIO0 = 1;
							/*
							 *  once buffer is full clear the flag to
							 *  keep the information stored
							 */
							//startAgain = 0;
							CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
						}
	}


	CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
	}

void timer_init3(float a, float b) {
		EALLOW;
		InitCpuTimers();
		ConfigCpuTimer(&CpuTimer1,a,b);
		EDIS;
		// end of timers
}

void enable_timer_interrupt3() {

		EALLOW;
		PieVectTable.XINT13 = changeFrequency;
		IER |=  M_INT13;	// tint2
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EINT;
		CpuTimer1Regs.TCR.bit.TSS = 0;	// start the timer
		EDIS;
}



