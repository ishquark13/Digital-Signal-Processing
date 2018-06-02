/*
 * main.c
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

#pragma DATA_SECTION(iir, "iirfilt");
IIR5BIQ32 iir = IIR5BIQ32_DEFAULTS;

#pragma DATA_SECTION(dbuffer,"iirfilt");
long dbuffer[2*IIR32_NBIQ];

const long coeff[5*IIR32_NBIQ] = IIR32_COEFF;
void init_IIR();

int main(void) {

		DisableDog();
		InitPll(10,3);
		analogToDigitalConverter_init();
		//Sram_init();
		digitalToAnalogConverter_init();
		timer_init(150.0,22.6757);
		enable_timer_interrupt();
		EALLOW;
		useGpio();

		init_IIR();

	while(1) {
		EALLOW;
		while(!interruptTimerFlag);

		iir.input = (((float)analogToDigitalConverter_send())- 0x7FFF);
		iir.calc(&iir);
		digitalToAnalogConverter_send((Uint16)(iir.output32*2 + 0x7FFF));
	
		interruptTimerFlag = 0;
	}
	//return 0;
}

void init_IIR() {

	/* IIR Filter Initialisation */
	iir.dbuffer_ptr = dbuffer;
	iir.coeff_ptr=(long *)coeff;
	iir.qfmat=IIR32_QFMAT;
	iir.nbiq=IIR32_NBIQ;
	iir.isf=IIR32_ISF;
	iir.init(&iir);

}
