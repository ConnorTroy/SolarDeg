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

    NVIC_EnableIRQ(EUSCIA0_IRQn); // Enable eUSCIA0 interrupt in NVIC

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
//        uart_send_multple(EUSCI_A0, data, (i++) + 1);
//        uart_send_byte ( UART_MODULE, '\n');
        i = i % sizeof(data);

        // toggle the LED on P1.0
        P1->OUT ^= BIT0;
        __delay_cycles(100000);
    }


}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA0_IRQHandler(void)
{
    uint32_t status =  EUSCI_A0 -> IFG;

    if(status & EUSCI_A_IFG_RXIFG)
    {
        EUSCI_A0 -> IFG &= ~(EUSCI_A_IFG_RXIFG);
        uart_receive(EUSCI_A0, UART_RX_BUF_MULTIBYTE, UART_RX_BUF_LEN);
    }

}
