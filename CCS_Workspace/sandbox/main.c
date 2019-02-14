#include "driverConfig.h"
//#include "printf.h"

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
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // configure port 1, pin 0 (p1.0) as an output pin.
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    uart_init(UART_MODULE, &uartConfig);
    uart_enable(UART_MODULE);

    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG); // Clear receive flag
    NVIC_EnableIRQ(EUSCIA0_IRQn); // Enable eUSCIA0 interrupt in NVIC
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE; // Enable USCI_A0 Receive character Interrupt


//    char data[5] = {'h', 'e', 'l', 'l', 'o'};
    uint8_t data[] = {'\n', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',};
    int count = 0;
    int i = 0;

    while(1)
    {
//        MAP_PCM_gotoLPM0();
//        if (i < 6){ uart_send_byte( UART_MODULE, data[i++] ); }
//        else {
//            char count_char = (count++ ) + 48;
//            uart_send_byte( UART_MODULE, count_char );
//            uart_send_byte( UART_MODULE, '\n');
//            i = 0;
//        }
//        count = count % 10;
        uart_send_multple(EUSCI_A0, data, (i++) + 1);
//        uart_send_byte ( UART_MODULE, '\n');
        i = i % sizeof(data);

        // toggle the LED on P1.0
        P1->OUT ^= BIT0;
        __delay_cycles(100000);
    }


//    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
//
//    I2C_init(I2C_MODULE, &i2cConfig);               // Initialize I2C
//    I2C_setSlaveAddress(I2C_MODULE, INA219_ADDR);   // Set slave address
//    I2C_enable(I2C_MODULE);                         // Enable Module
//
//
//    //THIS SECTION IS FOR TESTING I2C FUNCTIONALITY WITH THE INA219
//
//    uint32_t NUM_TX_BYTES = 3;
//    uint32_t NUM_RX_BYTES = 2;
//
//    uint8_t configReg = 0x00;
//    uint8_t calibReg = 0x05;
////    uint8_t resetData[3] = {configReg, 0x39, 0x9F};
//    uint8_t resetData[3] = {configReg, 0x80, 0x00};
//    uint8_t calibData[3] = {calibReg, 0xAA, 0x00};
//
//    uint8_t RXData[2] = {0, 0};
//
//
//    I2C_send(I2C_MODULE, &configReg, 1);
////    __delay_cycles(800000);
//    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);
////    __delay_cycles(800000);
//
//
//    // Reset all INA219 Registers
//    I2C_send(I2C_MODULE, resetData, NUM_TX_BYTES);
////    __delay_cycles(800000);
//    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);
////    __delay_cycles(800000);
//
//    // Read calibration register to verify it has been reset
//    I2C_send(I2C_MODULE, &calibReg, 1);
////    __delay_cycles(800000);
//    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);
////    __delay_cycles(800000);
//
//    // Set calibration register to 0xAAAA
//    I2C_send(I2C_MODULE, calibData, NUM_TX_BYTES);
////    __delay_cycles(800000);
//    // Read calibration register to verify it has been set to 0xAAAA
//    I2C_receive(I2C_MODULE, RXData, NUM_RX_BYTES);

//    while(1);

}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA0_IRQHandler(void)
{
//    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
//
//    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);
//
//    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
//    {
//        MAP_UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE));
//    }

}
