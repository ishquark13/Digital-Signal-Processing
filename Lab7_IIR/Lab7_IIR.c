/*
 * IIR.c
 *
 *  Created on: Nov 10, 2016
 *      Author: Ish's Master Race PC
 */
#include <DSP28x_Project.h>
#include "OneToOneI2CDriver.h"
#include <DSP2833x_Xintf.h>
#include<DSP2833x_XIntrupt.h>
#include "timer.h"
#include "analogToDigitalConverter.h"
#include "DSP2833x_CpuTimers.h"
#include <DSP2833x_SysCtrl.h>
#include "audioCntrl.h"
#include "Sram.h"
#include "digitalToAnalogConverter.h"
#include "hammingWindow.h"

#include "Lab7_IIR.h"

/*
 * while(!intflags);
 * iir.input = analogToDigitalConverter_send();
 * iir.calc = (&iir);
 * digitalToAnalogConverter_send((Uint16)iir.output32);
 *
 */
int main () {

			DisableDog();
			InitPll(10,3);
			analogToDigitalConverter_init();
			//Sram_init();
			digitalToAnalogConverter_init();
			timer_init(150.0,22.6757);
			enable_timer_interrupt();
			EALLOW;
			useGpio();

	while(1) {
		EALLOW;

		while(!interruptTimerFlag);
		x_n = (((float)analogToDigitalConverter_send())- 0x7FFF);

		y_n = (firstSystem[0]*x_n + firstSystem[1]*x_n_minusOne + firstSystem[2]*x_n_minusTwo - firstSystem[3]*y_n_minusOne - firstSystem[4]*y_n_minusTwo);

		x_n_minusTwo = x_n_minusOne;
		x_n_minusOne = x_n;
		y_n_minusTwo = y_n_minusOne;
		y_n_minusOne = y_n;

		// the _2 subscript denotes the second system that is to be fed the first
		// system's output. Ex. x_n_2 == x[n] of system 2

		x_n_2 = y_n;

		y_n_2 = (secondSystem[0]*x_n_2 + secondSystem[1]*x_n_minusOne_2 + secondSystem[2]*x_n_minusTwo_2 - secondSystem[3]*y_n_minusOne_2 - secondSystem[4]*y_n_minusTwo_2);

		x_n_minusTwo_2 = x_n_minusOne_2;
		x_n_minusOne_2 = x_n_2;
		y_n_minusTwo_2 = y_n_minusOne_2;
		y_n_minusOne_2 = y_n_2;

	digitalToAnalogConverter_send((Uint16)(y_n_2 + 0x7FFF));
	interruptTimerFlag = 0;
	}

}
