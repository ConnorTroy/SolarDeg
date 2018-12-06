/*
 * gpio.c
 *
 *  Created on: Dec 4, 2018
 *      Author: ktbow
 */
#include "gpio.h"
#include "msp.h"

void gpio_config(void) {
    // configure port 1, pin 0 (p1.0) as an output pin.
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;
}

