/*
 * Dma_int.c
 *
 *  Created on: Nov 20, 2016
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
#include <DSP2833x_GlobalPrototypes.h>
#include <math.h>
#include <stdio.h>

//__interrupt void dma2_swap() {
//	EALLOW;
//	PieCtrlRegs.PIEACK.bit.ACK7 = 1;
//
//	// need to be able to swap the pingData with the pongData
//	// and the pingCalc with pongCalc
//
//	if (swap) {
//		swap = false;
//
//		pingData = pongBufferIn;
//		pongData = pingBufferIn;
//		pingCalc = pongBufferOut;
//		pongCalc = pingBufferOut;
//	}
//	else {
//		swap = true;
//
//		pingData = pingBufferIn;
//		pongData = pongBufferIn;
//		pingCalc = pingBufferOut;
//		pongCalc = pongBufferOut;
//	}
//
//	 DmaRegs.CH2.DST_ADDR_SHADOW = (Uint32) &pingData[1];
//	 DmaRegs.CH3.SRC_ADDR_SHADOW = (Uint32) &pingCalc[0];
//	 selectFlag =1;
//}

