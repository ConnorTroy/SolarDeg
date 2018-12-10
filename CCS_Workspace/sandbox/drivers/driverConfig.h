/*
 * driverConfig.h
 *
 *  Created on: Dec 9, 2018
 *      Author: Connor Troy
 *
 *  This Header allows us to specify includes, defines, etc and keep code organized
 */


#ifndef DRIVERS_DRIVERCONFIG_H_
#define DRIVERS_DRIVERCONFIG_H_



#include "msp.h"
#include "drivers/i2c.h"
#include "drivers/ina219.h"

#include <stdint.h>
#include <string.h>

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

/* INA219 Configuration Parameter */
const INA219_Config powerSensorConfig =
{
     I2C_MODULE,
     CONFIG_DEFAULT,
     0xF000
};


#endif /* DRIVERS_DRIVERCONFIG_H_ */
