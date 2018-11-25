#include "msp.h"
#include "stdio.h"

/**
 * main.c
 */

#define TEMP_SENSE_ADDR                     0x48

static const uint32_t GPIO_PORT_TO_BASE[] =
{   0x00,
    (uint32_t)P1,
    (uint32_t)P1+1,
    (uint32_t)P3,
    (uint32_t)P3+1,
    (uint32_t)P5,
    (uint32_t)P5+1,
    (uint32_t)P7,
    (uint32_t)P7+1,
    (uint32_t)P9,
    (uint32_t)P9+1,
    (uint32_t)PJ
};


int get_temp()
{
    return 0;
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	int selectedPort = 1;
	int selectedPins = 0x0001;

    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + ((uint32_t)&P1->SEL0 - (uint32_t)P1)) &= ~selectedPins;
    HWREG16(baseAddress + ((uint32_t)&P1->SEL1 - (uint32_t)P1)) &= ~selectedPins;
    HWREG16(baseAddress + ((uint32_t)&P1->DIR - (uint32_t)P1)) |= selectedPins;


    HWREG16(baseAddress + ((uint32_t)&P1->DIR - (uint32_t)P1)) |= selectedPins;
    HWREG16(baseAddress + ((uint32_t)&P1->SEL0 - (uint32_t)P1)) |= selectedPins;
    HWREG16(baseAddress + ((uint32_t)&P1->SEL1 - (uint32_t)P1)) &= ~selectedPins;

	while(1){
	    HWREG16(baseAddress + OFS_PAOUT) ^= selectedPins;
        __delay_cycles(300000);

	}
}
