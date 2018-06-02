/*
 * audioCntrl.c
 *
 *  Created on: Oct 15, 2016
 *      Author: Ish's Master Race PC
 */
#include <DSP28x_Project.h>
#include <DSP2833x_SysCtrl.h>
#include "timer.h"
#include "digitalToAnalogConverter.h"
#include "audioCntrl.h"
#include "Sram.h"
//#pragma DATA_SECTION(bufferForFilters, ".johns")
//#pragma DATA_SECTION(circularBuffer, ".johns")
//#pragma DATA_SECTION(bufferForInterpolation, ".johns2")
//Uint16 bufferForInterpolation[256000];
//Uint16 circularBuffer[128000];
//extern float bufferForFilters[256];
int stateFlag = 0;
int reset = 1;
// this function enables the switches and LEDS
void useGpio() {

	GpioCtrlRegs.GPAMUX1.all = 0x0000;
	GpioCtrlRegs.GPADIR.all  = 0xFF;
	GpioCtrlRegs.GPAPUD.all = 0xFF;
}

//void checkSwitches() {
//
//	//useGpio();
//	Uint32 switchCombination = GpioDataRegs.GPADAT.all & 0x00000F00;
//	switchCombination = switchCombination>>8;
//	//bit-mask the values of Gpio 8-11 which are the switches
//	//CpuTimer1Regs.TCR.bit.TSS = 0;
//
//	 if (startAgain == 1) {
//		switch (switchCombination) {
//
//		case 1:
//
//			enable_timer_interrupt();
//			//ConfigCpuTimer(&CpuTimer1,150,32.2215);
//			stateFlag = 1;
//			//GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//			break;
//
//		case 3:
//			enable_timer_interrupt();
//			//GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//			stateFlag = 2;
//			break;
//
//		case 4:
//			enable_timer_interrupt();
//			//GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//			stateFlag = 4;
//			break;
//
//		case 7:
//					enable_timer_interrupt();
//					//GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//					stateFlag = 5;
//					break;
//
//		case 8:
//		case 9:
//		case 10:
//		case 11:
//		case 12:
//
//			stateFlag =4 ;
//		}
//
//
//		}
//
//	 else if(startAgain == 0) {
//
//		 if(switchCombination == 8 || switchCombination == 9 || switchCombination == 10 ){
//			 enable_timer_interrupt();
//			 			stateFlag = 4;
//
//		 }
//		 else if (switchCombination == 3) {
//			 interpolation();
//			 enable_timer_interrupt();
//			 			//GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//			 			stateFlag = 3;
//		 }
//		 else if (switchCombination == 0) {
//			 				GpioDataRegs.GPADAT.bit.GPIO0 = 0;
//			 				startAgain =1;
//
//			 			}
//		 else if (switchCombination == 7) {
//		 			 //interpolation();
//			// ConfigCpuTimer(&CpuTimer1,150,15.69);
//		 			 enable_timer_interrupt();
//		 		//ConfigCpuTimer(&CpuTimer1,150,15.69);
//		 			 			//switch comments for the config cpu timer
//		 			 			stateFlag = 10;
//			 		}
//
//		 else if (switchCombination == 5) {
//				 decimation(2);
//				 enable_timer_interrupt();
//				//GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//				stateFlag = 9;
//		 }
//
//		 else if (switchCombination == 13) {
//				 //decimation();
//				 enable_timer_interrupt();
//				// ConfigCpuTimer(&CpuTimer1,150,28.69);
//				//GpioDataRegs.GPADAT.bit.GPIO0 = 1;
//				stateFlag = 11;
//				 }
//	 }
//}





