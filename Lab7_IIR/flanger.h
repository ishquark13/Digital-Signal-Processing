/*
 * flanger.h
 *
 *  Created on: May 9, 2017
 *      Author: Ish's Master Race PC
 */

#ifndef FLANGER_H_
#define FLANGER_H_

extern void Flanger_init(short effect_rate,short sampling,short maxd,short mind,double fwv,double stepd,double fbv);
double Flanger_process(double xin);
 void Flanger_sweep(void);

#endif /* FLANGER_H_ */
