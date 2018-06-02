/*
 * Lab9_AutoWahWah.h
 *
 *  Created on: Dec 1, 2016
 *      Author: Ish's Master Race PC
 */
#include <math.h>
#include "Lab9_customBandpass.h"
#ifndef LAB9_AUTOWAHWAH_H_
#define LAB9_AUTOWAHWAH_H_
/*
 * variables to be used
 */




/* Public Functions in lab9 part 3:
 *
 * These functions relate the equations of the state variable filters' highpass,lowpass, and separate bandpass
 * given the set of parameters the previously mentioned filters are generated and manipulated accordingly, the last function is a
 * controlled modulation of a enveloped signal sine wave
 */

extern void AutoWah_init(short effect_rate,short sampling,short maxf,short minf,short Q,double gainfactor,short freq_step);
extern double AutoWah_process(double xin);
extern void AutoWah_sweep(double xin);

/*
 * uses of the functions
 */




#endif /* LAB9_AUTOWAHWAH_H_ */
