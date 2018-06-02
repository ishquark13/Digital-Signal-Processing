/*
 * timer2.h
 *
 *  Created on: Oct 20, 2016
 *      Author: Ish's Master Race PC
 */

#ifndef TIMER2_H_
#define TIMER2_H_



void timer_init2(float a, float b);

/*private functions*/
void enable_timer_interrupt2();
__interrupt void frequency10Khz();


#endif /* TIMER2_H_ */
