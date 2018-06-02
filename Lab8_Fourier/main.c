/*
 * main.c
 * Contains part 1 of lab8 which is the 256 point DFT implementation in c
 * on sample based processing
 * Author: Ishmael
 * Nov-16-2016
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
#include <DSP2833x_GlobalPrototypes.h>
#include <math.h>
#include <stdio.h>
#include "Dma_int.h"
#include "fpu_rfft.h"

#pragma DATA_SECTION(dftValues, "DMARAML4")
#pragma DATA_SECTION(x, "DMARAML5")
#pragma DATA_SECTION(Xreal, "DMARAML5")
#pragma DATA_SECTION(Ximaginary, "DMARAML5")
#pragma DATA_SECTION(Magnitude, "DMARAML4")

#define N 256	// size of dftValues discrete time array
#define PI2 6.28318530718
int sortVar = 0;
float magnitudeLargestValue = 0;
float frequencyLargestValue;
float dftValues[256] = {0};
int z = 0;
/* variables for the DFT
 * n is the discrete time index of the input signal
 * k is the frequency domain index
 *
 */
int n = 0;
int k = 0;
float x[N];
float Xreal[N];
float Ximaginary[N];
float Magnitude[N];

Uint16 arrayBytesSend[] = {0x3C, 0x38, 0x3C, 0x38,0x3C, 0x38, 0x2C, 0x28,0x2C, 0x28, 0x8C, 0x88,0x0C, 0x08, 0xFC,0xF8,0x0C, 0x08, 0x1C, 0x18};
char debug[]            = {'M','a','x',' ','F', ' ', '=',' ','\0'};
char gain[]				= {'M','a','x',' ','G', ' ', '=',' ','\0'};
char Hz[]				= {'H', 'z', '\0'};
char dB[]				= {'d', 'B', '\0'};
Uint16 twoLineMode[]    = {0xCC,0xC8,0x0C,0x08};
Uint16 goHome[]    		= {0x0C,0x08,0x3C,0x38};
Uint16 clearScreen[]    = {0x0C,0x08,0x1C,0x18};
char maxFreq[20];
char maxMag[20];
/*
 * X[k] = sum from n = 0 to N-1 of dftValues[n]*e^(-jkn2pi)/N
 * using euler's theorem and the de moivre's thm.:
 * X[k] = Xreal[k] + j*Ximaginary[k]
 * Xreal[k] = sum from n = 0 to N-1 of x[n]*cos(jkn2pi/N)
 * Ximaginary[k] = -sum from n = 0 to N-1 of x[n]*sin(jkn2pi/N)
 */

//RFFT_32 ->

int main(void) {
	// initializations
	DisableDog();
	InitPll(10,3);
	analogToDigitalConverter_init();
	//Sram_init();
	digitalToAnalogConverter_init();
	timer_init(150.0,22.6757);
	enable_timer_interrupt_DFT();
	EALLOW;
	useGpio();
	I2C_O2O_Master_Init(0x3F, 150, 7);
	I2C_O2O_SendBytes(arrayBytesSend, sizeof(arrayBytesSend));
	//I2C_O2O_SendBytes(twoLineMode, sizeof(twoLineMode));
	//sendStringData(debug);
	//I2C_O2O_SendBytes(twoLineMode, sizeof(twoLineMode));

	
	while(1) {
			EALLOW;
			while(z < 256) {
				while(!interruptTimerFlag);
				// put into array
				 dftValues[z] = (((float)analogToDigitalConverter_send())- 0x7FFF);
				 z++;
				 interruptTimerFlag = 0;
				}

			//calculate stuff... turn off timer
			CpuTimer1Regs.TCR.bit.TSS = 1;

			// credits to:
			// algorithm implementation of nested for-loops from: https://batchloaf.wordpress.com/2013/12/07/simple-dft-in-c/

			 for (k=0 ; k<N ; ++k)
			    {
			        // Real part of X[k]
			        Xreal[k] = 0;
			        for (n=0 ; n<N ; ++n) Xreal[k] += dftValues[n] * cosf(n * k * PI2 / N);

			        // Imaginary part of X[k]
			        Ximaginary[k] = 0;
			        for (n=0 ; n<N ; ++n) Ximaginary[k] -= dftValues[n] * sinf(n * k * PI2 / N);

			        // Power at kth frequency bin take the sqrt to get magnitude
			        Magnitude[k] = sqrtf(Xreal[k]*Xreal[k] + Ximaginary[k]*Ximaginary[k]);
			    }

			 magnitudeLargestValue = 0;

			 // TODO:
			 // ask about aliasing and N/2

			 for (sortVar = 0; sortVar < N/2 ; sortVar++) {
				 if(magnitudeLargestValue < Magnitude[sortVar]) {
				            magnitudeLargestValue = Magnitude[sortVar];
				            frequencyLargestValue = (ceil(((float)(44100/N)*((float)sortVar))));
				 }
			 }

			 magnitudeLargestValue = ceil(10*log10(magnitudeLargestValue));

			 sendStringData(debug);
			 sprintf(maxFreq, "%u", (Uint16)frequencyLargestValue);
			 sendStringData(maxFreq);
			 sendStringData(Hz);

			 I2C_O2O_SendBytes(twoLineMode, sizeof(twoLineMode));
			 sendStringData(gain);
			 sprintf(maxMag, "%u", (Uint16)magnitudeLargestValue);
			 sendStringData(maxMag);
			 sendStringData(dB);

			//I2C_O2O_SendBytes(clearScreen, sizeof(clearScreen));	// go to home
			I2C_O2O_SendBytes(goHome, sizeof(goHome));
			 CpuTimer1Regs.TCR.bit.TSS = 0;

			//digitalToAnalogConverter_send((Uint16)(iir.output32*2 + 0x7FFF));
			//clear flags and also clear the reset the buffer to be calculated again

			z = 0;
			//interruptTimerFlag = 0;
		}
	//return 0;
}
