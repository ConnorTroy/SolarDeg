#include "msp.h"
#include "stdio.h"

/**
 * main.c
 */

#define TEMP_SENSE_ADDR                     0x48



int get_temp()
{
    return 0;
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	while(1){
	    int temp = get_temp();

	}
}
