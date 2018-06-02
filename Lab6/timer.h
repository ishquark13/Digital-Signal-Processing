/*
 * timer.h
 *
 *  Created on: Oct 14, 2016
 *      Author: Ish's Master Race PC
 */

#ifndef TIMER_H_
#define TIMER_H_

//#pragma DATA_SECTION(circularBuffer, ".johns")
#define MAX_SRAM_SIZE 0x100000
extern int startAgain;
extern Uint32 interruptTimerFlag;

/*public functions*/
void timer_init(float a, float b);
void interpolation();
void decimation (int n);
/*private functions*/
void enable_timer_interrupt();
__interrupt void sample_time();

#endif /* TIMER_H_ */
