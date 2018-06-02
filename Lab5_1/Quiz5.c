/*
 * Quiz5.c
 *
 *  Created on: Oct 21, 2016
 *      Author: Ish's Master Race PC
 */
#include <DSP28x_Project.h>
#include "OneToOneI2CDriver.h"
#include <DSP2833x_Xintf.h>
#include<DSP2833x_XIntrupt.h>
//#include "timer.h"
//#include "timer2.h"
//#include "timer3.h"
#include "analogToDigitalConverter.h"
#include "digitalToAnalogConverter.h"
#include "DSP2833x_CpuTimers.h"
#include <DSP2833x_SysCtrl.h>
#include "audioCntrl.h"
#include "Sram.h"
#include "digitalToAnalogConverter.h"
#include "hammingWindow.h"
//#pragma DATA_SECTION(circularBuffer, ".johns")
//#pragma DATA_SECTION(bufferForInterpolation, ".johns2")
extern Uint16 circularBuffer[];
Uint16 initLCD[] = {0x3C, 0x38, 0x3C, 0x38,0x3C, 0x38, 0x2C, 0x28,0x2C, 0x28, 0x8C, 0x88,0x0C, 0x08, 0xFC,0xF8,0x0C, 0x08, 0x1C, 0x18};
Uint16 cp[] = {'C','O','R','N',' ','P','U','F','F','S'};
Uint16 twoLine[] = {0xCC, 0xC8, 0x0C, 0x08};
volatile Uint16 delay1 = 0;
volatile Uint16 delay2 = 512;
volatile Uint32 x = 0;
volatile Uint16 Output = 0;
volatile Uint32 offset1 = 0;
volatile Uint32 offset2 = 0;

__interrupt void delayAdjust() {


		if(delay1 >= 1024) {
			delay1 = 0;
		}
		else delay1++;



		if (delay2 >= 1024) {
			delay2 = 0;
		}
		else delay2++;

		CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
}

__interrupt void equation() {

	circularBuffer[x] = analogToDigitalConverter_send();

	if (x < delay1) {
			offset1 = (64000-(delay1-x)) - 1;
			}
			else  {
				offset1 = x - delay1;
			}


	if (x < delay2) {
				offset2 = (64000-(delay2-x)) - 1;
				}
				else  {
					offset2 = x - delay2;
				}

	//TODO: try using the offset in case index errors persist, use the substitution
	// delay1 == offset1
	// delay2 == offset2




		if(GpioDataRegs.GPADAT.bit.GPIO8) {
		//Output = (Uint16)(((hamming[delay1]) * (float)(circularBuffer[offset1]))) + ((hamming[delay2]) * (float)(circularBuffer[offset2]));
			Output = (Uint16) ( ( ( ( hamming[ delay1 ]) * (float) circularBuffer[offset1]) ) + ( hamming[ delay2 ])*( (float) circularBuffer[ offset2 ] ) );
				digitalToAnalogConverter_send(Output);
				x++;
				GpioDataRegs.GPADAT.bit.GPIO0 = 1;
		}


		if(!GpioDataRegs.GPADAT.bit.GPIO8) {
				digitalToAnalogConverter_send(analogToDigitalConverter_send());
				x++;
				GpioDataRegs.GPADAT.bit.GPIO0 = 0;
				}

		// increment in the interrupt
		if (x >= 64000) {
							x = 0;
							//GpioDataRegs.GPADAT.bit.GPIO0 = 1;
							/*
							 *  once buffer is full clear the flag to
							 *  keep the information stored
							 */
							//CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
						}
		CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
				}




void enable_timer12() {

		EALLOW;
		PieVectTable.XINT13 = equation;
		IER |=  M_INT13;
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EINT;
		CpuTimer1Regs.TCR.bit.TSS = 0;	// start the timer

		PieVectTable.TINT2 = delayAdjust;
		IER |=  M_INT14;
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EINT;
		CpuTimer2Regs.TCR.bit.TSS = 0;	// start the timer
		EDIS;
}


int main(void) {

	DisableDog();
	InitPll(10,3);
//	I2C_O2O_Master_Init(0x3F, 150, 7);
//	I2C_O2O_SendBytes(initLCD, sizeof(initLCD));
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer1,150,22.6757); // handles inputs
	ConfigCpuTimer(&CpuTimer2,150,200);// handles delays // 200 for one key

	Sram_init();
	useGpio();
	digitalToAnalogConverter_init();
	analogToDigitalConverter_init();
	enable_timer12();
//	sendStringData(cp);
//	I2C_O2O_SendBytes(twoLine, sizeof(twoLine));
//	sendStringData(lit);
//	//Uint32 GpioSwitches;


	while(1)	{ //start of the program



		//digitalToAnalogConverter_send(analogToDigitalConverter_send());
	}
}
