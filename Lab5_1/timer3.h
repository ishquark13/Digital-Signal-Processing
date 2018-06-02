/*
 * timer3.h
 *
 *  Created on: Oct 21, 2016
 *      Author: Ish's Master Race PC
 */

#ifndef TIMER3_H_
#define TIMER3_H_


void timer_init3(float a, float b);

/*private functions*/
void enable_timer_interrupt3();
__interrupt void changeFrequency();



#endif /* TIMER3_H_ */
