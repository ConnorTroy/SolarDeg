/*
 * driverConfig.h
 *
 *  Created on: Dec 9, 2018
 *      Author: Connor Troy
 *
 *  This Header allows us to specify includes, defines, etc and keep code organized
 */


#ifndef DRIVERCONFIG_H_
#define DRIVERCONFIG_H_



#include "msp.h"
#include "i2c.h"
#include "ina219.h"

#include <stdint.h>
#include <string.h>

#define TMP_ADDR                0x48
#define ACC_ADDR_1              0x68
#define ACC_ADDR_2              0x69
#define INA219_ADDR             0x40

#define I2C_MODULE              EUSCI_B1

/* I2C Master Configuration Parameter */
const I2C_Config i2cConfig =
{
     EUSCI_B_CTLW0_SSEL__SMCLK,              // SMCLK Clock Source
     100000,                                   // SMCLK = 1MHz
     EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
     0,                                      // No byte counter threshold
     EUSCI_B_CTLW1_ASTP_0                    // No Autostop
};

/* INA219 Configuration Parameter */
const INA219 powerSensorConfig =
{
     I2C_MODULE,
     INA219_ADDR_1,
     INA219_CONFIG_BRNG_32V,
     INA219_CONFIG_PGA_320,
     INA219_CONFIG_BADC_12_BIT,
     INA219_CONFIG_SADC_12_BIT,
     INA219_CONFIG_SHUNT_BUS_CONTINUOUS
};


#endif /* DRIVERCONFIG_H_ */
