/*
 * timer.c
 * This is the timer config file for the Solar Soaker PoC testing MCU board.
 * This will configure a timer that will allow us to blink an LED at 5Hz, indicating
 * a sucessful flash.
 */

#include "msp.h"
#include "timer.h"
#include <stdint.h>

// These are used for the systic timer. Ignore them unless needed.
#define STRVR *(uint32_t *)0xE000E014
#define STCVR *(uint32_t *)0xE000E018
#define STCSR *(uint32_t *)0xE000E010

// function definition for timer A configuration
void timer_a0_config(){
    TIMER_A0->R = 0;         // Reset count to zero when we configure timer
    TIMER_A0->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_IE;   // SMCLK, Up mode, this bitmask sets upmode
    TIMER_A0->CCR[0] = 37500;    // Value to count to
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;   // TACCR0 interrupt enabled
    //TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_7; // set clock divider to divide an additional X2 for X16 total - expansion reg
    //Enable Interrupts in the NVIC
    NVIC_EnableIRQ(TA0_0_IRQn);
}
// function definition for the timer interrupt
void TA0_0_IRQHandler(){
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);
        // toggle the LED on P1.0
        P1->OUT ^= BIT0;
    }
}

void systic_config() {
    STRVR |= 0x00FFFFFF; //program in max val to this register
    STCVR = 0; //clear this reg by writing any value to it
    STCSR |= 0x05; //config stcsr register for the required operation, run
    // instructions to set this up are on page 72 of the tech ref
}
