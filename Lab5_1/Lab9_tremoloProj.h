/*
 * Lab9_tremoloProj.h
 *
 *  Created on: Dec 1, 2016
 *      Author: Ish's Master Race PC
 */

#ifndef LAB9_TREMOLOPROJ_H_
#define LAB9_TREMOLOPROJ_H_

/*
 * public functions
 */
extern void Tremolo_init(float effect_rate,float depth);
extern float Tremolo_process(float xin);
extern void Tremolo_LFO(void);
/*
 * public variables
 */
static float dep;
static float counter_limit;
static float control;
static float mod;
static float offset;

/*
 * usages of the functions
 */
void Tremolo_init(float effect_rate,float depth) {
	/*
	 * private variables BOIS
	 */
    dep     = depth;
    control = 1;
    mod     = 0;
    counter_limit = effect_rate;
    offset  = 1 - dep;
}

float Tremolo_process(float xin) {
	/*
	 * private variables
	 */
    float yout;
    float m;

    m = (float)mod*dep/counter_limit;
    yout = (m + offset)*xin;
    return yout;
}

void Tremolo_LFO(void) {

	    mod += control;

	    if (mod > counter_limit) {
	        control = -1;
            }
            else if(!mod) {
	        control = 1;
            }
}

#endif /* LAB9_TREMOLOPROJ_H_ */
