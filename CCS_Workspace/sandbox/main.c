#include "driverConfig.h"

/**
 * main.c
 *
 * Connor Troy
 * Power Haus - Solar Soaker - Solar Degradation Unit
 * 12/2/2018
 *
 * This project is a testing environment for the various drivers we will create.
 *
 */

#define TIMER_MODULE    TIMER_A0
#define PWM_FREQ        1           // Hz
#define PWM_DUTY        50          // %


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // configure port 1, pin 0 (p1.0) as an output pin.
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    // configure port 1, pin 1 (p1.4) as an input pin.
    P1->SEL0 &= ~BIT4;
    P1->SEL1 &= ~BIT4;
    P1->REN  |=  BIT4;
    P1->OUT  |=  BIT4;

    // Handle interrupts for 1.4
    P1->IES = BIT4;
    P1->IFG = 0;
    P1->IE  = BIT4;
    NVIC_EnableIRQ(PORT1_IRQn);     // Enable interrupts for button press

    // Timer A0 Config
    TIMER_A0->R = 0; // Reset Count

    float duty_cycle = (float) PWM_DUTY / 100;
    uint16_t period_long = 0x8000;
    uint16_t period_short = period_long * duty_cycle;

    TIMER_A0->CCR[0] = period_long;
    TIMER_A0->CCR[1] = period_short;

    TIMER_A0->EX0 = 0b011;
//    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIS__VCC | TIMER_A_CCTLN_SCCI;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP | TIMER_A_CTL_ID_3;

    TIMER
    TIMER_A1->

//    NVIC_EnableIRQ(TA0_N_IRQn);

	while(1)
	{
//        uint16_t out = (BIT0 & ((TIMER_A0->CCTL[1] & TIMER_A_CCTLN_OUT) != 0));
////        P1->OUT |= out;
//        if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_OUT)
//        {
//            int i;
//            i = 0;
//        }
//        else
//        {
//            int i;
//            i = 0;
//        }
	}

}

void PORT1_IRQHandler(void)
{
    uint8_t IFG = P1->IFG;
    uint8_t IE_store = P1->IE;
    P1->IFG = 0;
    P1->IE = 0;

    if (IFG & BIT4)
    {
        P1->OUT ^= BIT0;
    }


    P1->IE = IE_store;
}

void TA0_N_IRQHandler(void)
{
    TIMER_A0->CTL &= ~(TIMER_A_CTL_IFG);
    P1->OUT ^= BIT0;
    return;
}
