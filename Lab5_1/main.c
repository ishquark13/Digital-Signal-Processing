/*
 * main.c
 */
//TODO: fix the GPIO function and switch logic to do the lab


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


#define MAX_SRAM_SIZE 0x100000

//extern int i = 0;

int main(void) {

	DisableDog();
	InitPll(10,3);
	timer_init(150,32.2215);
	Sram_init();
	useGpio();
	digitalToAnalogConverter_init();
	analogToDigitalConverter_init();
	while(1) {
		//digitalToAnalogConverter_send(analogToDigitalConverter_send());
		//timer_init(150,32.2215);
		checkSwitches();

	}

	return 0;
}
