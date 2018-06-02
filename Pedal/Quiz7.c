/*
 * Quiz7.c
 *
 *  Created on: Nov 14, 2016
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



float q = 0.5;
extern float f;

int main () {

			DisableDog();
			InitPll(10,3);
			analogToDigitalConverter_init();
			//Sram_init();
			digitalToAnalogConverter_init();

			timer_init(150.0,22.6757);
			enable_timer_interrupt();
			timer_init2(150,90.667); // be faster
			enable_timer_interrupt2();
			EALLOW;
			useGpio();

	while(1) {
		EALLOW;

		while(!interruptTimerFlag);

				x_n = (((float)analogToDigitalConverter_send())- 0x7FFF);

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
				//digitalToAnalogConverter_send((Uint16)((0.9*y_l_n + 0.1*y_h_n + 0.3*x_n) + 0x7FFF));
				digitalToAnalogConverter_send((Uint16)((0.5*x_n + 0.5*y_b_n) + 0x7FFF));

				interruptTimerFlag = 0;

	}
}
