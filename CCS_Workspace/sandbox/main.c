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
//    uint8_t resetData[3] = {configReg, 0x39, 0x9F};
    uint8_t resetData[3] = {configReg, 0x80, 0x00};
    uint8_t calibData[3] = {calibReg, 0xAA, 0x00};

    uint8_t RXData[2] = {0, 0};


    I2C_send(I2C_MODULE, &configReg, 1);
//    __delay_cycles(800000);
    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);
//    __delay_cycles(800000);


    // Reset all INA219 Registers
    I2C_send(I2C_MODULE, resetData, NUM_TX_BYTES);
//    __delay_cycles(800000);
    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);
//    __delay_cycles(800000);

    // Read calibration register to verify it has been reset
    I2C_send(I2C_MODULE, &calibReg, 1);
//    __delay_cycles(800000);
    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);
//    __delay_cycles(800000);

    // Set calibration register to 0xAAAA
    I2C_send(I2C_MODULE, calibData, NUM_TX_BYTES);
//    __delay_cycles(800000);
    // Read calibration register to verify it has been set to 0xAAAA
    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);



	while(1);

}
