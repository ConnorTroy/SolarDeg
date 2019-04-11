/*
 * i2c.h
 *
 *  Created on: Dec 2, 2018
 *      Author: Connor Troy
 *
 *  This driver handles basic i2c sends and receives
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <msp.h>

#define EUSCI_B_I2C_SET_DATA_RATE_1MBPS                             1000000
#define EUSCI_B_I2C_SET_DATA_RATE_400KBPS                           400000
#define EUSCI_B_I2C_SET_DATA_RATE_100KBPS                           100000

#define EUSCI_B_I2C_TIMEOUT_DISABLE                                 EUSCI_B_CTLW1_CLTO_0
#define EUSCI_B_I2C_TIMEOUT_28_MS                                   EUSCI_B_CTLW1_CLTO_1
#define EUSCI_B_I2C_TIMEOUT_31_MS                                   EUSCI_B_CTLW1_CLTO_2
#define EUSCI_B_I2C_TIMEOUT_34_MS                                   EUSCI_B_CTLW1_CLTO_3

typedef struct
{
    uint_fast8_t selectClockSource;     //clock source
    uint32_t i2cClk;                    //rate of the clock supplied to the I2C module
    uint32_t dataRate;                  //set up for selecting data transfer rate
    uint_fast8_t byteCounterThreshold;  //sets threshold for automatic STOP or UCSTPIFG
    uint_fast8_t autoSTOPGeneration;    //sets up the STOP condition generation
} I2C_Config;

#define EUSCI_B_CMSIS(x) ((EUSCI_B_Type *) x)

static uint8_t I2C_TXData[3] = {0,0,0};
static uint8_t I2C_RXData[2] = {0,0};
static int I2C_TX_CNT, I2C_RX_CNT;

// Initializes I2C as master and with parameters set in I2C_Config
void I2C_init(EUSCI_B_Type * module, const I2C_Config *config);

// Enables the EUSCI Module
void I2C_enable(EUSCI_B_Type * module);

// Sets new I2C address for use with given module
void I2C_setSlaveAddress(EUSCI_B_Type * module, uint8_t address);

// Writes data to i2c line given data location and number of bytes
void I2C_send(EUSCI_B_Type * module, uint8_t *tx_data, uint32_t num_bytes);

// Reads data from i2c line given a place to store it and number of bytes to receive
void I2C_receive(EUSCI_B_Type * module, uint8_t *rx_data, uint32_t num_bytes);

#endif /* I2C_H_ */
