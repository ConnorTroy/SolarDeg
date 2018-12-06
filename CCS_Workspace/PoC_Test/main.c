/*
 * This is the main file for the Solar Soaker PoC test program. This firmware seeks to
 * toggle an LED at 5Hz in order to demonstrate the prototype microcontroller board
 * that was developed for proof of concept is capable of being flashed, indicating a
 * sucessful design. 04DEC2018
 */

#include "msp.h"
#include <stdint.h>
#include "timer.h"
#include "gpio.h"


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    // set up uP
    gpio_config();
    timer_a0_config();
    __enable_irq();
    while(1);
}
