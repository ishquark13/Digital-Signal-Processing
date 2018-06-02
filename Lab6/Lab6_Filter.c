/*
 * Lab6_FilterAssembly.c
 *
 *  Created on: Oct 31, 2016
 *      Author: Ish's Master Race PC
 */

#include <DSP28x_Project.h>
//#include "OneToOneI2CDriver.h"
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
#include "Lab6_FilterAssembly.h"

//Uint32 iterator;

// put the whole structure in sram if want to use the sram.


int16 iterator = CIRCULARBUFFERMAX -1 ;
float output;
int main(void) {

	DisableDog();
	InitPll(10,3);
	analogToDigitalConverter_init();
	//Sram_init();
	digitalToAnalogConverter_init();
	timer_init(150.0,24.6757);
	enable_timer_interrupt();
	EALLOW;
	useGpio();
	//TODO:
	//the interrupt flag triggering at the right times.


	/*
	 * def struct
	 */
	//FIRFilter LPFilter = {0,64,38,{0},&LPFArray[0]};
	while(1) {
	/*
	 * The function below is going to send the struct FIRFILTER to the .asm so we can perform the calculations in assembly.
	 * in the following order:
	 * pointer to the array of TAPS, INDEX of all the TAPS array,
	 */
	//analogToDigitalConverter_send();
	//FirFilter(&LPFilter.circularBuffer[0],&LPFilter.circularBuffer[iterator],LPFilter.TapsMax, LPFilter.circularBufferMax);

	while(!interruptTimerFlag);
	circularBuffer[iterator] = ((float)analogToDigitalConverter_send() - 0x7FFF);
	EALLOW;

	if (iterator == 0) {
		iterator = CIRCULARBUFFERMAX - 1;
	}

	output = 0;
	GpioDataRegs.GPADAT.bit.GPIO0 = 0;
	output = FirFilter(&circularBuffer[0], &BPFArray[0], iterator, TAPSBPF );
	GpioDataRegs.GPADAT.bit.GPIO0 = 1;
	digitalToAnalogConverter_send((Uint16)(output + 0x7FFF));
	iterator--;
	interruptTimerFlag = 0;
}
}
