/*
 * IIR.h
 *
 *  Created on: Nov 10, 2016
 *      Author: Ish's Master Race PC
 */

#ifndef LAB7_IIR_H_
#define LAB7_IIR_H_

float x_n = 0;
float x_n_minusOne = 0;
float x_n_minusTwo = 0;
float y_n = 0;
float y_n_minusOne = 0;
float y_n_minusTwo = 0;

float x_n_2 = 0;
float x_n_minusOne_2 = 0;
float x_n_minusTwo_2 = 0;
float y_n_2 = 0;
float y_n_minusOne_2 = 0;
float y_n_minusTwo_2 = 0;

float firstSystem[] = {0.0059,    0.0118,    0.0059 ,  -1.8828,    0.9006};
float secondSystem[] = { 0.2501,    0.5002,    0.2501, -1.7592,    0.7759};
// yh[n] = x[n] - yl[n-1] - Qyb[n-1]

float y_h_n =  0;
float y_l_n_minus1 = 0;
float y_b_n_minus1 = 0;
float y_b_n = 0;
float y_l_n = 0;



#endif /* LAB7_IIR_H_ */
/*
 * read adc
 *
 * read adc[] * a0 +
 *
 */
