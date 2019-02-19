/*
 * uart.c
 *
 *  Created on: Feb 12, 2019
 *      Author: Connor Troy
 * 
 * This driver handles UART communication
 */

#include "uart.h"

/*
Initializes UART with parameters set in UART_Config
*/
void uart_init(EUSCI_A_Type * module, const UART_Config *config)
{
    
    // Disable the USCI module and clear the other bits of control register
    EUSCI_A_CMSIS(module) -> CTLW0 = UCSWRST;

    /* Clock source select */
    EUSCI_A_CMSIS(module) -> CTLW0 = (EUSCI_A_CMSIS(module) -> CTLW0 & ~UCSSEL_3) | config->selectClockSource;

    // MSB, LSB select
    if (config->msborLsbFirst)
        EUSCI_A_CMSIS(module) -> CTLW0 |= UCMSB;
    else
        EUSCI_A_CMSIS(module) -> CTLW0 &= ~UCMSB;

    // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    if (config->numberofStopBits)
        EUSCI_A_CMSIS(module) -> CTLW0 |= UCSPB;
    else
        EUSCI_A_CMSIS(module) -> CTLW0 &= ~UCSPB;

    // Parity
    switch (config->parity)
    {
    case UART_NO_PARITY:
        EUSCI_A_CMSIS(module) -> CTLW0 &= ~UCPEN;
        break;
    case UART_ODD_PARITY:
        EUSCI_A_CMSIS(module) -> CTLW0 |=  UCPEN;
        EUSCI_A_CMSIS(module) -> CTLW0 &= ~UCPAR;
        break;
    case UART_EVEN_PARITY:
        EUSCI_A_CMSIS(module) -> CTLW0 |= UCPEN;
        EUSCI_A_CMSIS(module) -> CTLW0 |= UCPAR;
        break;
    }

    // BaudRate Control Register 
    EUSCI_A_CMSIS(module) -> BRW = config -> clockPrescalar;
    EUSCI_A_CMSIS(module) -> MCTLW = ((config -> secondModReg << 8) + (config -> firstModReg << 4) + config -> overSampling);

    // Asynchronous mode & 8 bit character select & clear mode 
    EUSCI_A_CMSIS(module) -> CTLW0 &= ~(UCSYNC | UC7BIT | UCMODE_3 | UCRXEIE | UCBRKIE | UCDORM | UCTXADDR | UCTXBRK) | config->uartMode;

    // Enable pins specific to each eUSCI module
    switch((int)module)
    {
    case (int)EUSCI_A0:
        // Pins 1.2 (Rx) and 1.3 (Tx)
        P1 -> SEL0 |=   BIT2 + BIT3;
        P1 -> SEL1 &= ~(BIT2 + BIT3);
        break;
    case (int)EUSCI_A1:
        // Pins 2.2 (Rx) and 2.3 (Tx)
        P2 -> SEL0 |=   BIT2 + BIT3;
        P2 -> SEL1 &= ~(BIT2 + BIT3);
        break;
    case (int)EUSCI_A2:
        // Pins 3.2 (Rx) and 3.3 (Tx)
        P3 -> SEL0 |=   BIT2 + BIT3;
        P3 -> SEL1 &= ~(BIT2 + BIT3);
        break;
    case (int)EUSCI_A3:
        // Pins 9.6 (Rx) and 9.7 (Tx)
        P9 -> SEL0 |=   BIT6 + BIT7;
        P9 -> SEL1 &= ~(BIT6 + BIT7);
        break;
    }

}


/*
Enables the EUSCI Module
*/
void uart_enable(EUSCI_A_Type * module)
{
    EUSCI_A_CMSIS(module) -> CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
    EUSCI_A_CMSIS(module) -> IFG &= ~(EUSCI_A_IFG_RXIFG); // Clear Receive flag
    EUSCI_A_CMSIS(module) -> IE |= EUSCI_A_IE_RXIE; // Enable USCI_A0 Receive Interrupt
}


/*
Transmit Single Byte over UART
*/
void uart_send_byte(EUSCI_A_Type * module, uint8_t data)
{
    while(!(EUSCI_A_CMSIS(module) -> IFG & EUSCI_A_IE_TXIE));   // Wait until ready to transmit

    EUSCI_A_CMSIS(module) -> TXBUF = data;                      // Load byte into transmit buffer
}


/*
Transmit String (or similar) over UART
*/
void uart_send_multiple(EUSCI_A_Type * module, uint8_t * data, uint32_t num_bytes)
{
    uint32_t count = 0;
    while (count < num_bytes)
    {
        if (data[count] != '\0')
        {
            uart_send_byte( module, data[count++] );
        }
        else
        {
//            uart_send_byte( module, ' ' );
            count++;
        }
    }
}


/*
 * Receive single byte and place in multibyte receive buffer
 */
void uart_receive(EUSCI_A_Type * module, uint8_t * buffer, uint32_t bufferSize)
{
    int i;
    for (i = 0; i < bufferSize - 1; i++){
        buffer[i] = buffer [i+1];
    }
    buffer[bufferSize-1] = EUSCI_A_CMSIS(module) -> RXBUF;
}


