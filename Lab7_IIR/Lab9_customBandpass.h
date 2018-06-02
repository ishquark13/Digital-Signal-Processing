/*
 * Lab9_customBandpass.h
 *
 *  Created on: Dec 1, 2016
 *      Author: Ish's Master Race PC
 */

#ifndef LAB9_CUSTOMBANDPASS_H_
#define LAB9_CUSTOMBANDPASS_H_

 struct bp_coeffs{
	double e;
	double p;
	double d[3];
};

struct bp_filter{
	double e;
	double p;
	double d[3];
	double x[3];
	double y[3];
};

extern void bp_iir_init(double fsfilt,double gb,double Q,short fstep, short fmin);
extern void bp_iir_setup(struct bp_filter * H,int index);
extern double bp_iir_filter(double yin,struct bp_filter * H);

/*
 * function usage below:
 */



#endif /* LAB9_CUSTOMBANDPASS_H_ */
