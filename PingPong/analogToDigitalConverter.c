/*
 * analogToDigitalConverter.c
 *
 *  Created on: Oct 14, 2016
 *      Author: Ish's Master Race PC
 */
#include <DSP28x_Project.h>
#include <DSP2833x_Device.h>
#include "OneToOneI2CDriver.h"
#include <DSP2833x_Xintf.h>
#include<DSP2833x_XIntrupt.h>

/*Public functions*/

void  analogToDigitalConverter_init()
{
EALLOW;

	SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 1;	// enable the McBSP per
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 3;
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;

	McbspbRegs.SPCR2.all = 0;					// reset in order to begin using Mcbsp
	McbspbRegs.SPCR1.all = 0;					// resets the rx and also feeds word to the right
	McbspbRegs.SPCR1.bit.CLKSTP = 2;			// set to clock stop mode

	McbspbRegs.PCR.bit.CLKRP = 1;				// clock-stop mode disables the per clock when not data transmission is active
						//.clkxp = 						// we want this mode because it samples a rising edge like the ADC diagram
						//.clkrp = 0						// this also holds the transmit value frame that essentially enables the SPI interface
	McbspbRegs.PCR.bit.CLKXM = 1;				//
	McbspbRegs.PCR.bit.SCLKME= 0;
	McbspbRegs.SRGR2.bit.CLKSM = 1;
	McbspbRegs.SRGR1.bit.CLKGDV = 8;
	McbspbRegs.PCR.bit.FSXM    = 1;
	McbspbRegs.SRGR2.bit.FSGM = 0;
	McbspbRegs.PCR.bit.FSXP   = 1;
	McbspbRegs.XCR2.bit.XDATDLY = 1;
	McbspbRegs.RCR2.bit.RDATDLY = 1;
	McbspbRegs.XCR1.bit.XFRLEN1 = 0;
	McbspbRegs.XCR1.bit.XWDLEN1 = 4;
	McbspbRegs.RCR1.bit.RFRLEN1 = 0;
	McbspbRegs.RCR1.bit.RWDLEN1 = 4;
	McbspbRegs.SPCR2.bit.GRST   = 1;
	DELAY_US(100);
	McbspbRegs.SPCR2.bit.XRST   = 1;
	McbspbRegs.SPCR1.bit.RRST   = 1;

	DELAY_US(100);							// need to wait at least 2*(1/100mHz)

	McbspbRegs.SPCR2.bit.FRST = 1; 			// frame set is 1


	DELAY_US(10);
}
Uint16  analogToDigitalConverter_send() {

	EALLOW;
	McbspbRegs.DXR2.all = 0xFFFF;
	McbspbRegs.DXR1.all = 0xFFFF;


	while(McbspbRegs.SPCR1.bit.RRDY != 1);    // wait until ready flag is set

		Uint16 highByteAdc = McbspbRegs.DRR2.all;	// higher 16 bits first
		Uint16 lowByteAdc  = McbspbRegs.DRR1.all;	// lower 16 bits next
		Uint16	data_bois;

		// grab the useful 16-bits the word length is 24-bits but the first 6 are trash and also the last 2
		//TODO: write the necessary code to accomplish the above

		/*
		 * make the top 6 bits zero and the bottom 2 also zero
		 *
		 */

		highByteAdc = highByteAdc << 14 & 0xC000;
		lowByteAdc  = lowByteAdc >> 2   & 0xFFFC;
		data_bois   = highByteAdc | lowByteAdc;

		return data_bois;
}
