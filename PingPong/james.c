/*
 * main.c
 */

#include <DSP28x_Project.h>

int main(void) {
    EALLOW;//write to protected registers
    DisableDog();//disable watchdog
    InitPll(10, 3);//150 mhz: div by 4 -> 0,1   2 -> 2,  1-> 3

    GpioCtrlRegs.GPAMUX1.all = 0;
    GpioCtrlRegs.GPAPUD.all = 0xFFFFFFFF;//pull up disabled

    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;//init in gpio 0 - 7 LEDS, out
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 0;//init out gpio , 8-11 switches, in

    while(1){
        if(GpioDataRegs.GPADAT.bit.GPIO8 == 1)
            GpioDataRegs.GPADAT.bit.GPIO0 = 1;
    }
}
