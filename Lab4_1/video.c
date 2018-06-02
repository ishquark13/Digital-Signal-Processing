/*
 * video.c
 */
#include<DSP28x_Project.h>

interrupt void myISR() {

	//TODO: STUFF

}
int16  main(void) {
	// disable the watchdog


	EALLOW;
	SysCtrlRegs.WDCR = 0x68;

	// OR
	DisableDog();


	// pll clock
	InitPll(10,3);
	// configure the cpu timer doesnt start it. set /tss bit to 0
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 150,1E6);



	CpuTimer0.RegsAddr->TCR.bit.TSS = 0;
	// mess with GPIO

	//GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
	// calling the whole union

	GpioCtrlRegs.GPADIR.all = 0;
	
	PieVectTable.XINT13 = myISR;
	// pointer of this function is at the pieVect table now
	IER |= M_INT13;
	EINT;
	return 0;
}
