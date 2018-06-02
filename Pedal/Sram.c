/*
 * Sram.c
 *
 *  Created on: Oct 14, 2016
 *      Author: Ish's Master Race PC
 */
#include <DSP28x_Project.h>
//float circularBuffer[32627];
void Sram_init() {
	EALLOW;

	GpioCtrlRegs.GPAMUX2.all = 0xFF000000;
	GpioCtrlRegs.GPBMUX1.all = 0xFFFFF0A0;
	GpioCtrlRegs.GPCMUX1.all = 0xFFFFFFFF;
	GpioCtrlRegs.GPCMUX2.all = 0x0000FFFF;
	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;

//		XintfRegs.XINTCNF2.bit.CLKMODE = 0;
//		XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
//		XintfRegs.XTIMING6.bit.XWRTRAIL = 1;
//		XintfRegs.XTIMING6.bit.XWRACTIVE = 1;
//		XintfRegs.XTIMING6.bit.XWRLEAD = 1;
//		XintfRegs.XTIMING6.bit.XRDTRAIL = 1;
//		XintfRegs.XTIMING6.bit.XRDACTIVE = 1;
//		XintfRegs.XTIMING6.bit.XRDLEAD = 1;

//	EALLOW;
//	// All Zones---------------------------------
//	// Timing for all zones based on XTIMCLK = SYSCLKOUT
//	XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
//	// Buffer up to 3 writes
//	XintfRegs.XINTCNF2.bit.WRBUFF = 0;
//	// XCLKOUT is enabled
//	XintfRegs.XINTCNF2.bit.CLKOFF = 0;
//	// XCLKOUT = XTIMCLK
//	XintfRegs.XINTCNF2.bit.CLKMODE = 0;
//	// Disable XHOLD to prevent XINTF bus from going into high impedance state
//	// whenever TZ3 signal goes low. This occurs because TZ3 on GPIO14 is
//	// shared with HOLD of XINTF
//	XintfRegs.XINTCNF2.bit.HOLD = 1;
//
//	// Zone 7------------------------------------
//	// When using ready, ACTIVE must be 1 or greater
//	// Lead must always be 1 or greater
//	// Zone write timing
//	XintfRegs.XTIMING7.bit.XWRLEAD = 1;
//	XintfRegs.XTIMING7.bit.XWRACTIVE = 1; //2
//	XintfRegs.XTIMING7.bit.XWRTRAIL = 1;
//	// Zone read timing
//	XintfRegs.XTIMING7.bit.XRDLEAD = 1;
//	XintfRegs.XTIMING7.bit.XRDACTIVE = 1; //3
//	XintfRegs.XTIMING7.bit.XRDTRAIL = 0;
//
//	// don't double all Zone read/write lead/active/trail timing
//	XintfRegs.XTIMING7.bit.X2TIMING = 0;
//
//	// Zone will not sample XREADY signal
//	XintfRegs.XTIMING7.bit.USEREADY = 0;
//	XintfRegs.XTIMING7.bit.READYMODE = 0;
//
//	// 1,1 = x16 data bus
//	// 0,1 = x32 data bus
//	// other values are reserved
//	XintfRegs.XTIMING7.bit.XSIZE = 3;
//
//   //Force a pipeline flush to ensure that the write to
//   //the last register configured occurs before returning.
//   __asm(" RPT #7 || NOP");

	EALLOW;
	    // All Zones---------------------------------
	    // Timing for all zones based on XTIMCLK = SYSCLKOUT
	    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
	    // Buffer up to 3 writes
	    XintfRegs.XINTCNF2.bit.WRBUFF = 3;
	    // XCLKOUT is enabled
	    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
	    // XCLKOUT = XTIMCLK
	    XintfRegs.XINTCNF2.bit.CLKMODE = 0;
	    // Disable XHOLD to prevent XINTF bus from going into high impedance state
	    // whenever TZ3 signal goes low. This occurs because TZ3 on GPIO14 is
	    // shared with HOLD of XINTF
	    XintfRegs.XINTCNF2.bit.HOLD = 1;

	    // Zone 7------------------------------------
	    // When using ready, ACTIVE must be 1 or greater
	    // Lead must always be 1 or greater
	    // Zone write timing
	    XintfRegs.XTIMING7.bit.XWRLEAD = 1;
	    XintfRegs.XTIMING7.bit.XWRACTIVE = 2;
	    XintfRegs.XTIMING7.bit.XWRTRAIL = 1;
	    // Zone read timing
	    XintfRegs.XTIMING7.bit.XRDLEAD = 1;
	    XintfRegs.XTIMING7.bit.XRDACTIVE = 3;
	    XintfRegs.XTIMING7.bit.XRDTRAIL = 0;

	    // don't double all Zone read/write lead/active/trail timing
	    XintfRegs.XTIMING7.bit.X2TIMING = 0;

	    // Zone will not sample XREADY signal
	    XintfRegs.XTIMING7.bit.USEREADY = 0;
	    XintfRegs.XTIMING7.bit.READYMODE = 0;

	    // 1,1 = x16 data bus
	    // 0,1 = x32 data bus
	    // other values are reserved
	    XintfRegs.XTIMING7.bit.XSIZE = 3;
	    EDIS;

	   //Force a pipeline flush to ensure that the write to
	   //the last register configured occurs before returning.
	   __asm(" RPT #7 || NOP");

}
