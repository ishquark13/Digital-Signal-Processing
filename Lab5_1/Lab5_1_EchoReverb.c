/*
 * Lab5_1_EchoReverb.c
 *
 *  Created on: Oct 21, 2016
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
//#pragma DATA_SECTION(circularBuffer, ".johns")
//#pragma DATA_SECTION(bufferForInterpolation, ".johns2")

#define MAX_SRAM_SIZE 0x100000

Uint16 initLCD[] = {0x3C, 0x38, 0x3C, 0x38,0x3C, 0x38, 0x2C, 0x28,0x2C, 0x28, 0x8C, 0x88,0x0C, 0x08, 0xFC,0xF8,0x0C, 0x08, 0x1C, 0x18};
Uint16 reverb[]              = {'R','e','v','e','r','b','\0'};
Uint16 echo[]                = {'E','c','h','o','\0'};
Uint32 offset;
int alphaMultiplier = 14;
Uint32 j = 0;
Uint16 reverbOutput;
Uint16 echoOutput;
Uint32 switchCombination;
extern Uint16 circularBuffer[64000];
extern Uint16 bufferForInterpolation[256000];
__interrupt void interrupt1() {

		switchCombination 		 = GpioDataRegs.GPADAT.all & 0x00000F00;
		switchCombination        = switchCombination >> 8;
	// fix for the variables switches
	// do stuff
		alphaMultiplier = ((switchCombination & 0xE)>>1);
	if (switchCombination & 0x01) {
		//sendStringData(reverb);
		circularBuffer[j] = analogToDigitalConverter_send();

		// change the 441*2 is a DEBUG move

		if (j < alphaMultiplier*441*2) {
		offset = 128000-((alphaMultiplier * 441*2)-j);
		}
		else  {
			offset = j - 441*alphaMultiplier*2;
		}

		reverbOutput = (Uint16)(((0.7) * (float)(circularBuffer[j])))+((0.3) * (float)(circularBuffer[offset]));
		digitalToAnalogConverter_send(reverbOutput);
				j++;
		}


	//else for echo
	else {
		circularBuffer[j] = analogToDigitalConverter_send();

	// change the 441*2 is a DEBUG move

		if (j < alphaMultiplier*11025) {
		offset = 128000-((alphaMultiplier * 11025)-j);
		}
		else  {
			offset = j - 11025*alphaMultiplier;
		}

		echoOutput = (Uint16)(((0.6) * (float)(circularBuffer[j])))+((0.4) * (float)(circularBuffer[offset]));
		circularBuffer[j] = echoOutput;
		digitalToAnalogConverter_send(echoOutput);
				j++;
		}

			if (j == 128000) {
					j = 0;
					CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
					GpioDataRegs.GPADAT.bit.GPIO0 = 1;
					/*
					 *  once buffer is full clear the flag to
					 *  keep the information stored
					 */
					//CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
				}
			CpuTimer1Regs.TCR.bit.TIF |= 0x0001;

		}



void enable_timer1() {

		EALLOW;
		PieVectTable.XINT13 = interrupt1;
		IER |=  M_INT13;
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EINT;
		CpuTimer1Regs.TCR.bit.TSS = 0;	// start the timer
		EDIS;
}


int main(void) {

	DisableDog();
	InitPll(10,3);
	I2C_O2O_Master_Init(0x3F, 150, 7);
	I2C_O2O_SendBytes(initLCD, sizeof(initLCD));
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer1,150,12.54);
	Sram_init();
	useGpio();
	digitalToAnalogConverter_init();
	analogToDigitalConverter_init();
	enable_timer1();

	//Uint32 GpioSwitches;


	while(1)	{ //start of the program


	}

}
