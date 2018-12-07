
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

#define I2C_MODULE              EUSCI_B2


/* I2C Master Configuration Parameter */
const I2C_Config i2cConfig =
{
     EUSCI_B_CTLW0_SSEL__SMCLK,              // SMCLK Clock Source
     3000,                                   // SMCLK = 3kHz
     EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
     0,                                      // No byte counter threshold
     EUSCI_B_CTLW1_ASTP_0                    // No Autostop
};

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	I2C_init(I2C_MODULE, &i2cConfig);               // Initialize I2C
    I2C_setSlaveAddress(I2C_MODULE, INA219_ADDR);   // Set slave address
    I2C_enable(I2C_MODULE);                         // Enable Module



    //THIS SECTION IS FOR TESTING I2C FUNCTIONALITY WITH THE INA219

    uint32_t NUM_TX_BYTES = 3;
    uint32_t NUM_RX_BYTES = 2;

    uint8_t configReg = 0x00;
    uint8_t calibReg = 0x05;
    uint8_t resetData[3] = {configReg, 0x80, 0x00};
    uint8_t calibData[3] = {calibReg, 0xAA, 0xAA};

    uint8_t RXData[2] = {0, 0};


    // Reset all INA219 Registers
    I2C_send(I2C_MODULE, resetData, NUM_TX_BYTES);

    // Read calibration register to verify it has been reset
    I2C_send(I2C_MODULE, &calibReg, 1);
    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);

    // Set calibration register to 0xAAAA
    I2C_send(I2C_MODULE, calibData, NUM_TX_BYTES);
    // Read calibration register to verify it has been set to 0xAAAA
    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);



	while(1);

}
