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

#include <stdint.h>
#include <stdio.h>

/* Protocol Drivers */
#include "i2c.h"
#include "uart.h"

/* Device Drivers */
#include "ina219.h"

/* Source Files */
#include "source/ext_connect.h"
#include "source/helpers.h"


#define TMP_ADDR                0x48
#define ACC_ADDR_1              0x68
#define ACC_ADDR_2              0x69
#define INA219_ADDR             0x40

#define I2C_MODULE              EUSCI_B1
#define UART_MODULE             EUSCI_A0


#define UART_RX_BUF_LEN         15
uint8_t UART_RX_BUF_MULTIBYTE[UART_RX_BUF_LEN];

/* I2C Master Configuration Parameter */
const I2C_Config i2cConfig =
{
    EUSCI_B_CTLW0_SSEL__SMCLK,                    // SMCLK Clock Source
    32,                                      // SMCLK = 1MHz
    1,           // Desired I2C Clock of 100khz
    0,                                           // No byte counter threshold
    EUSCI_B_CTLW1_ASTP_0                         // No Autostop
};

/* UART Configuration Parameter */
const UART_Config uartConfig =
{
     EUSCI_A_CTLW0_SSEL__SMCLK,                   // SMCLK Clock Source
     19,                                          // Clock Prescalar
     8,                                           // UCxBRF = 8
     0,                                           // UCxBRS = 0
     UART_NO_PARITY,                              // Parity Bit Type
     UART_LSB_FIRST,                              // MSB / LSB First
     UART_ONE_STOP_BIT,                           // Num Stop Bits
     EUSCI_A_CTLW0_MODE_0,                        // eUSCI_A UART Mode
     UART_OVERSAMPLING                            // UART Sampling Type
};

/* INA219 Configuration Parameter */
INA219 powerSensorConfig =
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
