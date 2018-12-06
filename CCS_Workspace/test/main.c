#include "msp.h"
#include <stdint.h>


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	uint32_t i = 0;

	P1->SEL0 &= ~BIT0;
	P1->SEL1 &= ~BIT0;
	P1->DIR |= BIT0;
	P1->OUT |= BIT0;


	while(1)
	{
	    P1->OUT ^= BIT0;
	    for (i=0; i < 30000; i++);

	}

}
