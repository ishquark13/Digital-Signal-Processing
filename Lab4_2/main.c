/*
 * main.c
 * will run the SPI as a master device alongside the McBSP to obtain
 * slave device information from the ADC
 */

#include <DSP2833x_Device.h>
#include <DSP28x_Project.h>
#include "OneToOneI2CDriver.h"
#include <DSP2833x_Xintf.h>

//Uint16 temp = 0;
Uint16 clearLCD[]       = {0x0C, 0x08, 0x1C, 0x18};
void voltmeter();

interrupt void ADC_ISR()
{
	// all vars in here must be declared volatile
	// read adc in here
	// clear intr flag
	//temp++;
	I2C_O2O_SendBytes(clearLCD,sizeof(clearLCD));
	//GpioCtrlRegs.GPAMUX1.all |= 0x0000;
	//GpioCtrlRegs.GPADIR.all  = 0x00FF;
	//GpioDataRegs.GPADAT.all  = 0x00FF;
	//GpioDataRegs.GPATOGGLE.all |= 0x00FF;
	voltmeter();

	CpuTimer1Regs.TCR.bit.TIF |= 0x0001;
}


void init_ADC()
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



}

void init_DAC()
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

void timer_INIT() {

		InitCpuTimers();
		ConfigCpuTimer(&CpuTimer1, 37.5, 0.897E5);
		EALLOW;
		PieVectTable.XINT13 = ADC_ISR;
		IER |=  M_INT13;
		PieCtrlRegs.PIECTRL.all |= 0x01;
		EnableInterrupts();
		EINT;
		CpuTimer1.RegsAddr->TCR.bit.TSS = 0;	// start the timer

		// end of timers

}

Uint16 ADC_send() {
	
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

void DAC_send(Uint16 digitalValue) {

	EALLOW;
	McbspaRegs.DXR1.all = digitalValue;
	EDIS;
}


void sendStringData(Uint16* johns){

	Uint16 output[4] = {};

	while(*johns !=0 ) {
		int i = 0;
		output[i] = (*johns & 0xF0)|(0x0D);
		i++;
		output[i] = (*johns & 0xF0)|(0x09);
		i++;
		output[i] = ((*johns & 0x0F)<<4)|(0x0D);
		i++;
		output[i] = ((*johns & 0x0F)<<4)|(0x09);
		johns++;
		I2C_O2O_SendBytes(output,4);
	}


}
void voltmeter() {

	Uint16 sndv [6] = {};
	float ADC_value = ADC_send();
	float voltage   = (ADC_value/0xFFFF)*(3.3);
	int v1          =  (int)voltage;
	float voltage2    = 10*(voltage-v1);
	int v2          =  (int)(voltage2);
	int v3			=  (int)10*(voltage2-v2);


	 sndv[0]      	=  v1 + 0x30;
	 sndv[1]      	= '.';
	 sndv[2]		=  v2 + 0x30;
	 sndv[3]		=  v3 + 0x30;
	 sndv[4]       	= 'V';

	 //TODO: consider fixing this bit comment or decrease buffer size

	 //sndv[5]        =  0;

	 //TODO:
	 //import the sendStringData function

	 sendStringData(sndv);



}
int main(void) {
	int temp;
	Uint16 arrayBytesSend[] = {0x3C, 0x38, 0x3C, 0x38,0x3C, 0x38, 0x2C, 0x28,0x2C, 0x28, 0x8C, 0x88,0x0C, 0x08, 0xFC,0xF8,0x0C, 0x08, 0x1C, 0x18};

	Uint16 debug[]              = {'F','u','c','k',' ', 'M', 'c', 'b', 's','p','\0'};
	DisableDog();
	InitPll(10,1);								// 150MHz/4?



    init_ADC();
    init_DAC();
   // timer_INIT();
    // init the LCD with cursor blinking at the start position
    I2C_O2O_Master_Init(0x3F, 37.5, 7);
    I2C_O2O_SendBytes(arrayBytesSend, sizeof(arrayBytesSend));
    sendStringData(debug);
    while(1) {
    	 // put stuff here

    	//DAC_send(ADC_send());

    	//ADC_send();
    	//voltmeter();
    	//sendStringData(debug);
    	//I2C_O2O_SendBytes(clearLCD,sizeof(clearLCD));


    	DAC_send(ADC_send());

    }

	return 0;
}
