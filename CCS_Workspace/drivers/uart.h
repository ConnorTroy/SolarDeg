/*
 * uart.h
 *
 *  Created on: Feb 12, 2019
 *      Author: Connor Troy
 */

#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include "msp.h"
#include <stdint.h>

#define UART_NO_PARITY                                  0x00
#define UART_ODD_PARITY                                 0x01
#define UART_EVEN_PARITY                                0x02

#define UART_MSB_FIRST                                  EUSCI_A_CTLW0_MSB
#define UART_LSB_FIRST                                  0x00

#define UART_ONE_STOP_BIT                               0x00
#define UART_TWO_STOP_BITS                              EUSCI_A_CTLW0_SPB

#define UART_OVERSAMPLING                               0x01
#define UART_LOW_FREQUENCY                              0x00

typedef struct
{
    uint_fast8_t    selectClockSource;      // SMCLK Clock Source
    uint_fast16_t   clockPrescalar;         // Clock Prescalar
    uint_fast8_t    firstModReg;            // UCxBRF
    uint_fast8_t    secondModReg;           // UCxBRS
    uint_fast8_t    parity;                 // Parity Bit Type
    uint_fast16_t   msborLsbFirst;          // MSB / LSB First
    uint_fast16_t   numberofStopBits;       // Num Stop Bits
    uint_fast16_t   uartMode;               // EUSCI A to UART Mode
    uint_fast8_t    overSampling;           // UART Sampling Type
} UART_Config;

#define EUSCI_A_CMSIS(x) ((EUSCI_A_Type *) x)

// Initializes UART with parameters set in UART_Config
void uart_init(EUSCI_A_Type * module, const UART_Config *config);

// Enables the EUSCI Module
void uart_enable(EUSCI_A_Type * module);

// Transmit Single Byte over UART
void uart_send_byte(EUSCI_A_Type * module, uint8_t data);

// Transmit String (or similar) over UART
void uart_send_multple(EUSCI_A_Type * module, uint8_t * data, uint32_t num_bytes);

#endif /* DRIVERS_UART_H_ */
