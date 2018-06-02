/*
 * Lab9_vibratoProj.c
 *
 *  Created on: Nov 30, 2016
 *      Author: Ish's Master Race PC
 */


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
#include <math.h>
//#pragma DATA_SECTION(circularBuffer, ".johns")
//#pragma DATA_SECTION(bufferForInterpolation, ".johns2")
float circularBuffer2[];
Uint16 initLCD[] = {0x3C, 0x38, 0x3C, 0x38,0x3C, 0x38, 0x2C, 0x28,0x2C, 0x28, 0x8C, 0x88,0x0C, 0x08, 0xFC,0xF8,0x0C, 0x08, 0x1C, 0x18};
Uint16 cp[] = {'C','O','R','N',' ','P','U','F','F','S'};
Uint16 twoLine[] = {0xCC, 0xC8, 0x0C, 0x08};
Uint16 delay1 = 0;
Uint16 delay2 = 512;
volatile Uint32 x = 0;
float Output;
Uint32 offset1 = 0;
Uint32 offset2 = 0;

int changeDir = 0;

float carrier;
float carrier2;
float carrier3;

float vib_delay;
float b_delay;
float b_width;
float modfreq;
int m;
float mod;
double zeiger;
double indexNum;
float frac;

float width     = 440;

Uint16 adcval = 0;


void internalADC_init() {
	InitAdc();
	EALLOW;
	SysCtrlRegs.HISPCP.all = 0x03; // make high speed clock equal to 25Mh for ADC
	AdcRegs.ADCTRL1.bit.CONT_RUN = 1; // no freerun mode off
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0;  // prescaler is 1 for bits 8-0 all 0 no multiplier
	AdcRegs.ADCTRL1.bit.CPS = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 0;
	//AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ = 0; // turn this off //
	//AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0; // this off too //
	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0;
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // conversion begin until after

	// new stuff from e2e T//
	 AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	 AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;   // Enable software trigger for SEQ1( In separate instructions)

	 AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)

	  // AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 1;// Enable SOCB from ePWM to start SEQ2
	 AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;         // Reset SEQ1
	 //AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1; //  // Enable software trigger for SEQ2 ( in separate instructions)

	 AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;  // Enable SEQ2 interrupt (every EO
}
__interrupt void equation() {

	if (x < delay1) {
				offset1 = 8000+x-delay1;
				}
				else  {
					offset1 = x - delay1 ;
				}

//	vib_delay 	= width;
//	 b_delay     = round(vib_delay*44100);
//	 b_width     = round(width*44100);
//	 modfreq   = 15000.0/44100.0;
//	 m         = modfreq;
//	 mod		= sinf(modfreq*2*3.141659*x);
//	zeiger			= (float)1+(float)delay1+width*mod;
//	indexNum		= floor(zeiger);
//	frac		= zeiger - indexNum;

//	if (x < delay1) {
//			offset1 = 128000-((delay1)-x);
//			}
//			else  {
//				offset1 = x - delay1;
//			}
//
//
//	if (x < delay2) {
//				offset2 = 128000-((delay2)-x);
//				}
//				else  {
//					offset2 = x - delay2;
//				}

	//TODO: try using the offset in case index errors persist, use the substitution
	// delay1 == offset1
	// delay2 == offset2

	/*
	 * have logic to control adc input
	 */
		EALLOW;
		adcval = (AdcRegs.ADCRESULT0)*3.3/4096;	// 12-bit conversion

		circularBuffer2[x] = (((float)analogToDigitalConverter_send())- 0x7FFF);
		if(GpioDataRegs.GPADAT.bit.GPIO9) {
			// this is the Uint16 buffer
			circularBuffer[x] = (((float)analogToDigitalConverter_send())- 0x7FFF);
			Output = (Uint16) (((float)(circularBuffer[x-(offset1+1)]) * frac)) + (float)(circularBuffer[x-offset1] * (1-frac));
			digitalToAnalogConverter_send((Uint16)(Output + 0x7FFF));;
			x++;
		}
		else if(GpioDataRegs.GPADAT.bit.GPIO8) {
			carrier= sinf(2.0*3.141659*x*31.5385*(float)adcval/44100.0);
			carrier2 = cosf(2.0*3.141659*x*500.0/44100.0);
			carrier3 = sinf(2.0*3.141659*x*800.0/44100.0);
					Output = (float)circularBuffer2[x]*carrier;
					//digitalToAnalogConverter_send(Output);
					digitalToAnalogConverter_send((Uint16)(Output + 0x7FFF));


							x++;

//		else if(!GpioDataRegs.GPADAT.bit.GPIO8) {
//		digitalToAnalogConverter_send(analogToDigitalConverter_send());
//		x++;
		}
		else
			digitalToAnalogConverter_send(analogToDigitalConverter_send());
					//x++;

						// increment in the interrupt
		if (x == 8000) {
							x = 0;
							GpioDataRegs.GPADAT.bit.GPIO0 = 1;
							/*
							 *  once buffer is full clear the flag to
							 *  keep the information stored
							 */
							//CpuTimer1Regs.TCR.bit.TSS = 1;	// stop the timer
						}

		AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
		AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;         // Reset SEQ2
		AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
		AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;       // Clear INT SEQ2 bit
		CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
				}



__interrupt void delayAdjust() {

		if (changeDir == 1) {
			delay1--;
			if (delay1 == 0) {
				changeDir = 0;
						}
				}

				else{

				delay1++;

					if(delay1 == 1024) {
						changeDir = 1;
					}
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
//	int m = 0;
//	for( m = 0; m < 8000; m++){
//		circularBuffer[m]=0;
//	}

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
	internalADC_init();
//	sendStringData(cp);
//	I2C_O2O_SendBytes(twoLine, sizeof(twoLine));
//	sendStringData(lit);
//	//Uint32 GpioSwitches;
/*
 *
 * *******************************Vibrato constants below::
 * ******equation is:
 * y(n) = x(n − (M + 1)).frac + x(n − M).(1 − frac);
 */
//	vib_delay = width;
//	 b_delay     = round(vib_delay*44100);
//	 b_width     = round(width*44100);
//	 modfreq   = 10/44100;
//	 m         = modfreq;
//	 mod		= sinf(m*2*3.141659*x);
//	 zeiger			= 1+b_delay+(b_width*mod);
//	 indexNum			= floor(zeiger);
//	 frac		= zeiger - indexNum;



	while(1)	{ //start of the program

		EALLOW;
				adcval = (AdcRegs.ADCRESULT0)*3.3/4096;	// 12-bit conversion

		//digitalToAnalogConverter_send(analogToDigitalConverter_send());
	}
}
