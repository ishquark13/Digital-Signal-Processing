/*
 * audioCntrl.h
 *
 *  Created on: Oct 15, 2016
 *      Author: Ish's Master Race PC
 */

#ifndef AUDIOCNTRL_H_
#define AUDIOCNTRL_H_
extern Uint16 circularBuffer[64000];
extern Uint16 bufferForInterpolation[256000];
extern int stateFlag;
void useGpio();
void checkSwitches();


#endif /* AUDIOCNTRL_H_ */
