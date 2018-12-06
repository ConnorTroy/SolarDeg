/*
 * i2c.c
 *
 *  Created on: Dec 2, 2018
 *      Author: ctroy
 */



#include <drivers/i2c.h>
#include <stdint.h>
#include <msp.h>



void I2C_init(EUSCI_B_Type * module, const I2C_Config *config)
{
    /*
     * Initializes I2C as master and with parameters set in I2C_Config
     */

    //Disable the USCI module and clear the other bits of control register
    EUSCI_B_CMSIS(module)->CTLW0 = EUSCI_B_CTLW0_SWRST;

    //Configure Automatic STOP condition generation
    EUSCI_B_CMSIS(module)->CTLW1 &= ~EUSCI_B_CTLW1_ASTP_MASK | config->autoSTOPGeneration;

    //Byte Count Threshold
    EUSCI_B_CMSIS(module)->TBCNT = config->byteCounterThreshold;

    /*
     * Compute the clock divider that achieves the fastest speed less than or
     * equal to the desired speed.  The numerator is biased to favor a larger
     * clock divider so that the resulting clock is always less than or equal
     * to the desired clock, never greater.
     */
    EUSCI_B_CMSIS(module)->BRW = (uint16_t) (config->i2cClk / config->dataRate);

    /*
     * Configure as I2C master mode.
     * UCMST = Master mode
     * UCMODE_3 = I2C mode
     * UCSYNC = Synchronous mode
     */
    EUSCI_B_CMSIS(module)->CTLW0 &=  ~EUSCI_B_CTLW0_SSEL_MASK | (config->selectClockSource | EUSCI_B_CTLW0_MST | EUSCI_B_CTLW0_MODE_3 | EUSCI_B_CTLW0_SYNC | EUSCI_B_CTLW0_SWRST);

}


void I2C_setSlaveAddress(EUSCI_B_Type * module, uint8_t address)
{
    /*
     * Sets new I2C address for use with given module
     */
    EUSCI_B_CMSIS(module)->I2CSA = address;
}


void I2C_send(EUSCI_B_Type * module, uint8_t *tx_data, uint32_t num_bytes)
{
    /*
     * Writes data to i2c line given data location and number of bytes
     */

//    EUSCI_B_CMSIS(module)->CTLW0 |= ;

    // Clear any existing interrupt flag
    EUSCI_B_CMSIS(module)->IFG &= ~EUSCI_B_IFG_TXIFG0;

    // Wait until ready to write
    while (EUSCI_B_CMSIS(module)->STATW & EUSCI_B_STATW_BBUSY);

    EUSCI_B_CMSIS(module)->CTLW0 |= EUSCI_B_CTLW0_TR;

    // Set to transmit mode and initiate start condition
    EUSCI_B_CMSIS(module)->CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    //Poll for transmit interrupt flag and start condition flag.
    while ((EUSCI_B_CMSIS(module)->CTLW0 & EUSCI_B_CTLW0_TXSTT) || !(EUSCI_B_CMSIS(module)->IFG & EUSCI_B_IFG_TXIFG0));

    uint32_t alltime = 0;
    uint32_t counter = 0;
    //iterate through data and write to TXBUF
    while (counter < num_bytes)
    {
        EUSCI_B_CMSIS(module)->TXBUF = (uint8_t) *tx_data;
        alltime++;

        //Poll for transmit or NACK interrupt flag.
        while (!(EUSCI_B_CMSIS(module)->IFG & EUSCI_B_IFG_TXIFG0) && !(EUSCI_B_CMSIS(module)->IFG & EUSCI_B_IFG_NACKIFG));

        //If a ACK is received, iterate to next byte
        int ackcheck = !(EUSCI_B_CMSIS(module)->IFG & EUSCI_B_IFG_NACKIFG);
        if (ackcheck)
        {

            tx_data++;
            counter++;
        }
        //Otherwise clear NACK interrupt and try again
        else
        {
            EUSCI_B_CMSIS(module)->IFG &= ~(EUSCI_B_IFG_NACKIFG);
            EUSCI_B_CMSIS(module)->CTLW0 |= EUSCI_B_CTLW0_TR + EUSCI_B_CTLW0_TXSTT;
            tx_data -= counter;
            counter = 0;
        }
//        EUSCI_B_CMSIS(module)->IFG &= ~(EUSCI_B_IFG_TXIFG0);
    }

    //Make sure transmit buffer is empty and send stop command
//    while (!(EUSCI_B_CMSIS(module)->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B_CMSIS(module)->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
}


void I2C_receive(EUSCI_B_Type * module, uint8_t *rx_data, uint32_t num_bytes)
{
    /*
     * Reads data from i2c line given a place to store it and number of bytes to receive
     */


    // Clear any existing interrupt flag
    EUSCI_B_CMSIS(module)->IFG &= ~EUSCI_B_IFG_RXIFG0;

    //Set to receive mode
    EUSCI_B_CMSIS(module)->CTLW0 &= ~EUSCI_B_CTLW0_TR;

    // Wait until ready to initiate read
    while (EUSCI_B_CMSIS(module)->STATW & EUSCI_B_STATW_BBUSY);

    //Send start
    EUSCI_B_CMSIS(module)->CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    uint32_t counter = 0;
    while (counter < num_bytes)
    {
        //Poll for receive interrupt flag.
//        while (!(EUSCI_B_CMSIS(module)->IFG & EUSCI_B_IFG_RXIFG0));

        //Store data in rx_data
        *rx_data = EUSCI_B_CMSIS(module)->RXBUF & EUSCI_B_RXBUF_RXBUF_MASK;

        rx_data++;
        counter++;
    }
    //Send stop
    EUSCI_B_CMSIS(module)->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
    EUSCI_B_CMSIS(module)->CTLW0 |= EUSCI_B_CTLW0_SWRST;
}





