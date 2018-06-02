//#include <DSP2833x_Device.h>
#include <DSP28x_Project.h>
/*Public Functions*/
void digitalToAnalogConverter_init()
{
	SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 1;
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 2;
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;

	McbspaRegs.SPCR2.all = 0;					// reset in order to begin using Mcbsp
	McbspaRegs.SPCR1.all = 0;					// resets the rx and also feeds word to the right and sample-rate set generator
	McbspaRegs.SPCR1.bit.CLKSTP = 2;			// set to clock stop mode

	McbspaRegs.PCR.bit.CLKRP = 0;				// clock-stop mode disables the per clock when not data transmission is active
	McbspaRegs.PCR.bit.CLKXP = 1;				// we want this mode because it samples a rising edge like the ADC diagram						// this also holds the transmit value frame that essentially enables the SPI interface
	McbspaRegs.PCR.bit.CLKXM = 1;				//
	McbspaRegs.PCR.bit.SCLKME= 0;
	McbspaRegs.SRGR2.bit.CLKSM = 1;
	McbspaRegs.SRGR1.bit.CLKGDV = 8;
	McbspaRegs.PCR.bit.FSXM    = 1;
	McbspaRegs.SRGR2.bit.FSGM = 0;
	McbspaRegs.PCR.bit.FSXP   = 1;
	McbspaRegs.XCR2.bit.XPHASE = 0;
	McbspaRegs.RCR2.bit.RPHASE = 0;
	McbspaRegs.XCR1.bit.XFRLEN1 = 0;
	McbspaRegs.XCR1.bit.XWDLEN1 = 2;

	McbspaRegs.SPCR2.bit.GRST   = 1;
	DELAY_US(100);
	McbspaRegs.SPCR2.bit.XRST   = 1;
	McbspaRegs.SPCR1.bit.RRST   = 1;

	DELAY_US(100);							// need to wait at least 2*(1/100mHz)

	McbspaRegs.SPCR2.bit.FRST = 1; 			// frame set is 1



}
void digitalToAnalogConverter_send(Uint16 digitalValue) {

	EALLOW;
	McbspaRegs.DXR1.all = digitalValue;
	EDIS;
}
