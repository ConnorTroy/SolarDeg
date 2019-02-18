/*
 * helpers.h
 *
 *  Created on: Feb 18, 2019
 *      Author: Connor Troy
 */

#ifndef SOURCE_HELPERS_H_
#define SOURCE_HELPERS_H_


#include <math.h>

#include "msp.h"

// reverses a string 'str' of length 'len' 
void reverse(char *str, int len);

 // Converts a given integer x to string str[].  d is the number 
 // of digits required in output. If d is more than the number 
 // of digits in x, then 0s are added at the beginning. 
int intToStr(int x, char str[], int d);
  
// Converts a floating point number to string. 
void ftoa(float n, char *res, int afterpoint);

#endif /* SOURCE_HELPERS_H_ */
