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
#include "timer3.h"


#define MAX_SRAM_SIZE 0x100000
Uint32 switchCombination;
extern Uint16 select;
Uint16 circularbuffer[];
//extern int i = 0;
Uint32 l = 0;
Uint32 t = 0;

void decimation (int n) {
	do {
		if (l % n) {
			l++;
			continue;
		}
		bufferForInterpolation[t] = circularBuffer[l];
		l++;
		t++;
	}while(l < 64000);

}

void interpolation() {
	Uint16 interp;
	l = 0;
	while (l<256000) {
	bufferForInterpolation[2*l] = circularBuffer[l];
	interp = (circularBuffer[l]>>1)+(circularBuffer[l+1]>>1);
	bufferForInterpolation[(2*l)+1] = interp;
	l++;
	}
	if (l == 256000) {
		l =0;
	}
}




int main(void) {

	DisableDog();
	InitPll(10,3);



	timer_init3(150,14.2215);
	Sram_init();
	useGpio();
	digitalToAnalogConverter_init();
	analogToDigitalConverter_init();
	//enable_timer_interrupt3();
	while(1) {

		switchCombination = GpioDataRegs.GPADAT.all & 0x00000F00;
		switchCombination = switchCombination >> 8;

			if (switchCombination == 4) {
					//timer_init3(150,50.94);
				//playback at 20Khz
				select = 4;
			}

//					enable_timer_interrupt3();
//				}
			else if (switchCombination == 1) {
					timer_init3(150,74.98);
					//playback at 15Khz
					select = 1;
					enable_timer_interrupt3();
				}

//				}
//
				else if(switchCombination == 2) {
					timer_init3(150,50.94);
					//playback at 20Khz
					select = 2;
					enable_timer_interrupt3();
				}
////
////				else if(switchCombination == 3) {
////					timer_init3(150,41.21);
////					//playback at 25Khz
////					select = 3;
////					enable_timer_interrupt3();
////				}
////
////				else if (switchCombination == 4) {
////					timer_init3(150,33);
////					//playback at 30Khz
////					select = 4;
////					enable_timer_interrupt3();
////
////
////				}
////
////				else if(switchCombination == 5) {
////					timer_init3(150,18.79);
////					//playback at 35Khz
////					select = 5;
////					enable_timer_interrupt3();
////				}
////
////				else if(switchCombination == 6) {
////					timer_init3(150,10.22);
////					//playback at 40Khz
////					select = 6;
////					enable_timer_interrupt3();
////				}
////
				    else if(switchCombination == 7) {
				    CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
					timer_init3(150,24.12);
					//interpolate and send 20khz
					interpolation();
					select = 7;
					enable_timer_interrupt3();
				}
//
				else if(switchCombination == 8) {
					CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
					interpolation();
					timer_init3(150,13.77);
					//interpolate and send 40khz
					select = 8;
					enable_timer_interrupt3();

					}


				else if (switchCombination == 9) {
					CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
					timer_init3(150,43.14);
					decimation(2);
					//decimate and send 20khz
					select = 9;
					enable_timer_interrupt3();
				}
					//decimation(2);

//				}

//				else if(switchCombination == 10) {
//					timer_init3(150,75.04);
//					decimation(3);
//
//					//decimate and send 10Khz
//					select = 10;
//					enable_timer_interrupt3();
//				}

////				else if(switchCombination == 11) {
////					timer_init3(150,10.22);
////					//decimate by 5 and send at 10khz
////					select = 11;
////					enable_timer_interrupt3();
////				}
//				//else {
//					//CpuTimer1Regs.TCR.bit.TSS = 1;
//				//}


	}

	return 0;
}
