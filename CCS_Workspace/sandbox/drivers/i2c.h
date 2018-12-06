/*
 * i2c.h
 *
 *  Created on: Dec 2, 2018
 *      Author: ctroy
 */

#ifndef DRIVERS_I2C_H_
#define DRIVERS_I2C_H_

#include <stdint.h>
#include <msp.h>

#define EUSCI_B_I2C_NO_AUTO_STOP                           EUSCI_B_CTLW1_ASTP_0
#define EUSCI_B_I2C_SET_BYTECOUNT_THRESHOLD_FLAG           EUSCI_B_CTLW1_ASTP_1
#define EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD  \
                                                           EUSCI_B_CTLW1_ASTP_2

#define EUSCI_B_I2C_SET_DATA_RATE_1MBPS                                 1000000
#define EUSCI_B_I2C_SET_DATA_RATE_400KBPS                                400000
#define EUSCI_B_I2C_SET_DATA_RATE_100KBPS                                100000

#define EUSCI_B_I2C_CLOCKSOURCE_ACLK                   EUSCI_B_CTLW0_SSEL__ACLK
#define EUSCI_B_I2C_CLOCKSOURCE_SMCLK                 EUSCI_B_CTLW0_SSEL__SMCLK

#define EUSCI_B_I2C_OWN_ADDRESS_OFFSET0                                    0x00
#define EUSCI_B_I2C_OWN_ADDRESS_OFFSET1                                    0x02
#define EUSCI_B_I2C_OWN_ADDRESS_OFFSET2                                    0x04
#define EUSCI_B_I2C_OWN_ADDRESS_OFFSET3                                    0x06

#define EUSCI_B_I2C_OWN_ADDRESS_DISABLE                                    0x00
#define EUSCI_B_I2C_OWN_ADDRESS_ENABLE                      EUSCI_B_I2COA0_OAEN

#define EUSCI_B_I2C_TRANSMIT_MODE                              EUSCI_B_CTLW0_TR
#define EUSCI_B_I2C_RECEIVE_MODE                                           0x00

#define EUSCI_B_I2C_TIMEOUT_DISABLE                        EUSCI_B_CTLW1_CLTO_0
#define EUSCI_B_I2C_TIMEOUT_28_MS                          EUSCI_B_CTLW1_CLTO_1
#define EUSCI_B_I2C_TIMEOUT_31_MS                          EUSCI_B_CTLW1_CLTO_2
#define EUSCI_B_I2C_TIMEOUT_34_MS                          EUSCI_B_CTLW1_CLTO_3

#define EUSCI_B_I2C_NAK_INTERRUPT                             EUSCI_B_IE_NACKIE
#define EUSCI_B_I2C_ARBITRATIONLOST_INTERRUPT                   EUSCI_B_IE_ALIE
#define EUSCI_B_I2C_STOP_INTERRUPT                             EUSCI_B_IE_STPIE
#define EUSCI_B_I2C_START_INTERRUPT                            EUSCI_B_IE_STTIE
#define EUSCI_B_I2C_TRANSMIT_INTERRUPT0                        EUSCI_B_IE_TXIE0
#define EUSCI_B_I2C_TRANSMIT_INTERRUPT1                        EUSCI_B_IE_TXIE1
#define EUSCI_B_I2C_TRANSMIT_INTERRUPT2                        EUSCI_B_IE_TXIE2
#define EUSCI_B_I2C_TRANSMIT_INTERRUPT3                        EUSCI_B_IE_TXIE3
#define EUSCI_B_I2C_RECEIVE_INTERRUPT0                         EUSCI_B_IE_RXIE0
#define EUSCI_B_I2C_RECEIVE_INTERRUPT1                         EUSCI_B_IE_RXIE1
#define EUSCI_B_I2C_RECEIVE_INTERRUPT2                         EUSCI_B_IE_RXIE2
#define EUSCI_B_I2C_RECEIVE_INTERRUPT3                         EUSCI_B_IE_RXIE3
#define EUSCI_B_I2C_BIT9_POSITION_INTERRUPT                   EUSCI_B_IE_BIT9IE
#define EUSCI_B_I2C_CLOCK_LOW_TIMEOUT_INTERRUPT               EUSCI_B_IE_CLTOIE
#define EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT                    EUSCI_B_IE_BCNTIE

#define EUSCI_B_I2C_BUS_BUSY                                EUSCI_B_STATW_BBUSY
#define EUSCI_B_I2C_BUS_NOT_BUSY                                           0x00

#define EUSCI_B_I2C_STOP_SEND_COMPLETE                                     0x00
#define EUSCI_B_I2C_SENDING_STOP                            EUSCI_B_CTLW0_TXSTP

#define EUSCI_B_I2C_START_SEND_COMPLETE                                    0x00
#define EUSCI_B_I2C_SENDING_START                           EUSCI_B_CTLW0_TXSTT

typedef struct
{
    uint_fast8_t selectClockSource;     //clock source
    uint32_t i2cClk;                    //rate of the clock supplied to the I2C module
    uint32_t dataRate;                  //set up for selecting data transfer rate
    uint_fast8_t byteCounterThreshold;  //sets threshold for automatic STOP or UCSTPIFG
    uint_fast8_t autoSTOPGeneration;    //sets up the STOP condition generation
} I2C_Config;

#define EUSCI_A_CMSIS(x) ((EUSCI_A_Type *) x)
#define EUSCI_B_CMSIS(x) ((EUSCI_B_Type *) x)

//Initializes I2C as master and with parameters set in I2C_Config
void I2C_init(EUSCI_B_Type * module, const I2C_Config *config);

//Sets new I2C address for use with given module
void I2C_setSlaveAddress(EUSCI_B_Type * module, uint8_t address);

//Writes data to i2c line given data location and number of bytes
void I2C_send(EUSCI_B_Type * module, uint8_t *tx_data, uint32_t num_bytes);

//Reads data from i2c line given a place to store it and number of bytes to receive
void I2C_receive(EUSCI_B_Type * module, uint8_t *rx_data, uint32_t num_bytes);

#endif /* DRIVERS_I2C_H_ */
