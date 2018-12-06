
#include "msp.h"
#include "drivers/i2c.h"

#include <stdint.h>
#include <string.h>

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

#define TMP_ADDR                0x48
#define ACC_ADDR_1              0x68
#define ACC_ADDR_2              0x69
#define INA219_ADDR             0x40


#define OFS_LIB_PADIR   ((uint32_t)&P1->DIR - (uint32_t)P1)
#define OFS_LIB_PASEL0  ((uint32_t)&P1->SEL0 - (uint32_t)P1)
#define OFS_LIB_PASEL1  ((uint32_t)&P1->SEL1 - (uint32_t)P1)



/* I2C Master Configuration Parameter */
const I2C_Config i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        300000,                                // SMCLK = 3kHz
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // Initialize USCI_B1 and I2C Master to communicate with slave devices
    //Reset all registers/disable, master, I2C mode, SMCLK
    EUSCI_B2->CTLW0 = (EUSCI_B_CTLW0_SWRST | EUSCI_B_CTLW0_MST | EUSCI_B_CTLW0_MODE_3 | EUSCI_B_CTLW0_SSEL__SMCLK);
    //no autostop to manually control when stop bit is sent
    EUSCI_B2->CTLW1 = EUSCI_B_CTLW1_ASTP_0;
    //set clock divider for SMCLK at 3MHz for 400KBPS data rate
    EUSCI_B2->BRW = (uint16_t) (3000 / 100000);
    //Disable TX IE. Polling interrupt flags manually made more sense to me and the communication is fast
    //so the inefficiencies of polling the flags is minimal
    EUSCI_B2->IE &= ~EUSCI_B_IE_TXIE0;



    uint32_t baseAddress = (uint32_t) P3;
    HWREG16(baseAddress + OFS_LIB_PADIR) &= ~(BIT6 + BIT7);

    HWREG16(baseAddress + OFS_LIB_PASEL0) |= BIT6 + BIT7;
    HWREG16(baseAddress + OFS_LIB_PASEL1) &= ~(BIT6 + BIT7);

    I2C_setSlaveAddress(EUSCI_B2, TMP_ADDR);

    /* Enable I2C Module to start operations */
    EUSCI_B2->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;

    EUSCI_B_CMSIS(EUSCI_B2_BASE)->IFG = 0;

    //Enable the interrupt masked bit
//    EUSCI_B_CMSIS(EUSCI_B2_BASE)->IE |= EUSCI_B_I2C_RECEIVE_INTERRUPT0 + EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_NAK_INTERRUPT;





    uint32_t NUM_TX_BYTES = 3;
    uint32_t NUM_RX_BYTES = 2;

    uint8_t TXData[3] = {0x01, 0x60, 0x90};
    uint8_t RXData[2] = {0, 0};

    uint8_t REG_1 = 0x01;
    uint8_t REG_2 = 0x00;

//    memset(RXData, 0x00, NUM_RX_BYTES);

//    I2C_send(EUSCI_B2, &REG_1, 1);
//    I2C_send(EUSCI_B2, TXData, NUM_TX_BYTES);


//    I2C_send(EUSCI_B2, &REG_2, 1);
    I2C_receive(EUSCI_B2, RXData, NUM_RX_BYTES);


	while(1);

}
