/*
 * Lab9_tremoloProj.c
 *
 *  Created on: Dec 1, 2016
 *      Author: Ish's Master Race PC
 */
#include <DSP28x_Project.h>
#include "OneToOneI2CDriver.h"
#include <DSP2833x_Xintf.h>
#include<DSP2833x_XIntrupt.h>
#include "analogToDigitalConverter.h"
#include "digitalToAnalogConverter.h"
#include "DSP2833x_CpuTimers.h"
#include <DSP2833x_SysCtrl.h>
#include "audioCntrl.h"
#include "Sram.h"
#include "digitalToAnalogConverter.h"
#include "hammingWindow.h"
#include "Lab9_tremoloProj.h"
//extern Uint16 circularBuffer[];
float circularBuffer1[500];
Uint16 delay1 = 0;
Uint16 delay2 = 512;
Uint32 x = 0;
Uint16 Output;
Uint32 offset1 = 0;
Uint32 offset2 = 0;
float yout = 0;

__interrupt void delayAdjust() {

		delay1++;
		if(delay1 == 1024) {
			delay1 = 0;
		}

		delay2++;

		if (delay2 == 1024) {
			delay2 = 0;
		}
		CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
}


__interrupt void equation() {


	if (x < delay1) {
			offset1 = 128000-((delay1)-x);
			}
			else  {
				offset1 = x - delay1;
			}


	if (x < delay2) {
				offset2 = 128000-((delay2)-x);
				}
				else  {
					offset2 = x - delay2;
				}


	//TODO: try using the offset in case index errors persist, use the substitution
	// delay1 == offset1
	// delay2 == offset2


	circularBuffer1[x] =(((float)analogToDigitalConverter_send())- 0x7FFF);
	            /*use the tremolo on adc sample in real-time*/
	if(GpioDataRegs.GPADAT.bit.GPIO8) {
	            yout = Tremolo_process(0.7*circularBuffer1[x]);
	            /*now send the modulation to the dac*/
	            digitalToAnalogConverter_send((Uint16)(yout + 0x7FFF));
	            	            /*Makes LFO vary to reduce noise and carrier the frequency*/
	            	            Tremolo_LFO();
	            	            x++;
	}
	if(!GpioDataRegs.GPADAT.bit.GPIO8) {
		digitalToAnalogConverter_send(analogToDigitalConverter_send());
//	}
//	            /*Send modulated output to the DAC*/
//	            digitalToAnalogConverter_send((Uint16)yout);
//	            /*Makes LFO vary*/
//	            Tremolo_LFO();
//	            x++;

//		Output = (Uint16)(((hamming[delay1]) * (float)(circularBuffer[offset1]))) + ((hamming[delay2]) * (float)(circularBuffer[offset2]));
//				digitalToAnalogConverter_send(Output);
//				x++;
	}	// increment in the interrupt
		if (x == 8000) {
							x = 0;
							GpioDataRegs.GPADAT.bit.GPIO0 = 1;
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


void main(void) {
	DisableDog();
	InitPll(10,3);
//	I2C_O2O_Master_Init(0x3F, 150, 7);
//	I2C_O2O_SendBytes(initLCD, sizeof(initLCD));
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer1,150,22.6757); // handles inputs
	ConfigCpuTimer(&CpuTimer2,150,200);// handles delays
	Sram_init();
	useGpio();
	digitalToAnalogConverter_init();
	analogToDigitalConverter_init();
	enable_timer12();



    Tremolo_init(4000,1);

    while(1) {
    	 // stuff in interrupt
        }
    }

