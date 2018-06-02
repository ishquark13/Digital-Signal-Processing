/*
 * Lab5_1_Aliasing.c
 *
 *  Created on: Oct 20, 2016
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
#include "timer2.h"


int main(void) {

	DisableDog();
	InitPll(10,3);
	timer_init2(150,2);
	Sram_init();
	useGpio();
	digitalToAnalogConverter_init();
	analogToDigitalConverter_init();
	enable_timer_interrupt2();

	while(1) {

		//checkSwitches();
		GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1;
	}

	return 0;
}
