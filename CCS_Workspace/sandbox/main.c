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

    // configure port 1, pin 1 (p1.1) as an input pin.
    P1->SEL0 &= ~BIT1;
    P1->SEL1 &= ~BIT1;
    P1->REN  |=  BIT1;
    P1->OUT  |=  BIT1;

    // configure port 1, pin 1 (p1.4) as an input pin.
    P1->SEL0 &= ~BIT4;
    P1->SEL1 &= ~BIT4;
    P1->REN  |=  BIT4;
    P1->OUT  |=  BIT4;


    uart_init(UART_MODULE, &uartConfig);
    uart_enable(UART_MODULE);

    NVIC_EnableIRQ(EUSCIA0_IRQn);   // Enable eUSCIA0 interrupt in NVIC


    memset(UART_RX_BUF_MULTIBYTE, '-', UART_RX_BUF_LEN);

    poll_for_ext_connect( UART_MODULE, UART_RX_BUF_MULTIBYTE, UART_RX_BUF_LEN );
    P1->OUT &= ~BIT0;

    // Handle interrupts for 1.1 and 1.4
    P1->IES = BIT1 + BIT4;
    P1->IFG = 0;
    P1->IE  = BIT1 + BIT4;
    NVIC_EnableIRQ(PORT1_IRQn);     // Enable interrupts for button press

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
//        uart_send_multiple(EUSCI_A0, data, (i++) + 1);
//        uart_send_byte ( UART_MODULE, '\n');
//        i = i % sizeof(data);

        // toggle the LED on P1.0
//        P1->OUT ^= BIT0;
//        __delay_cycles(100000);
    }


}

// Inputs UART Receive data to UART Buffer
void EUSCIA0_IRQHandler(void)
{
    uint32_t status = EUSCI_A0 -> IFG;

    if(status & EUSCI_A_IFG_RXIFG)
    {
        EUSCI_A0 -> IFG &= ~(EUSCI_A_IFG_RXIFG);
        uart_receive(EUSCI_A0, UART_RX_BUF_MULTIBYTE, UART_RX_BUF_LEN);
    }

}

void PORT1_IRQHandler(void)
{
    uint8_t IFG = P1->IFG;
    uint8_t IE_store = P1->IE;

    P1->IE = 0;
    P1->IFG = 0;
    if (IFG & BIT4)
    {
        float sample_vData[] =  {-0.2, -0.19, -0.18, -0.17, -0.16, -0.15, -0.14, -0.13, -0.12, -0.11,
                                 -0.1, -0.09, -0.08, -0.07, -0.06, -0.05, -0.04, -0.03, -0.02, -0.01,
                                 0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11,
                                 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23,
                                 0.24, 0.25, 0.26, 0.27, 0.28, 0.29, 0.3, 0.31, 0.32, 0.33, 0.34, 0.35,
                                 0.36, 0.37, 0.38, 0.39, 0.4, 0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47,
                                 0.48, 0.49, 0.5, 0.51, 0.52, 0.53, 0.54, 0.55, 0.56, 0.57, 0.58, 0.59,
                                 0.6, 0.61, 0.62, 0.63, 0.64, 0.65, 0.66, 0.67, 0.68, 0.69, 0.7, 0.71,
                                 0.72, 0.73, 0.74, 0.75, 0.76, 0.77, 0.78, 0.79, 0.8, 0.81, 0.82, 0.83,
                                 0.84, 0.85, 0.86, 0.87, 0.88, 0.89, 0.9, 0.91, 0.92, 0.93, 0.94, 0.95,
                                 0.96, 0.97, 0.98, 0.99, 1.0, 1.01, 1.02, 1.03, 1.04, 1.05, 1.06, 1.07,
                                 1.08,  1.09, 1.1, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 1.17, 1.18, 1.19,
                                 1.2, 1.21, 1.22, 1.23, 1.24, 1.25, 1.26, 1.27, 1.28, 1.29, 1.3};

        float sample_iData[] =  {21.55, 21.584, 21.571, 21.555, 21.546, 21.575, 21.571, 21.574, 21.55,
                                 21.572, 21.568, 21.578, 21.551, 21.552, 21.575, 21.572, 21.57, 21.549,
                                 21.549, 21.557, 21.552, 21.521, 21.523, 21.552, 21.533, 21.533, 21.525,
                                 21.531, 21.545, 21.512, 21.51, 21.539, 21.517, 21.525, 21.499, 21.506,
                                 21.501, 21.519, 21.508, 21.514, 21.511, 21.52, 21.517, 21.5, 21.533,
                                 21.533, 21.506, 21.53, 21.524, 21.521, 21.52, 21.485, 21.511, 21.495,
                                 21.512, 21.506, 21.463, 21.463, 21.472, 21.467, 21.459, 21.437, 21.421,
                                 21.436, 21.423, 21.421, 21.412, 21.396, 21.369, 21.351, 21.333, 21.321,
                                 21.304, 21.268, 21.251, 21.216, 21.163, 21.132, 21.088, 21.071, 21.017,
                                 20.946, 20.897, 20.831, 20.781, 20.714, 20.618, 20.554, 20.484, 20.38,
                                 20.307, 20.218, 20.138, 20.028, 19.928, 19.827, 19.723, 19.608, 19.497,
                                 19.369, 19.24, 19.114, 18.986, 18.82, 18.688, 18.516, 18.357, 18.172,
                                 17.971, 17.77, 17.561, 17.309, 17.057, 16.745, 16.4, 16.055, 15.635,
                                 15.154, 14.633, 13.997, 13.281, 12.446, 11.465, 10.335, 8.9922, 7.4173,
                                 5.6127, 3.5084, 1.0466, -1.7668, -4.9379, -8.5696, -12.602, -17.041,
                                 -21.942, -27.3, -32.952, -39.051, -45.541, -52.368, -59.491, -66.621,
                                 -74.036, -81.314, -88.65, -95.567, -102.05, -107.18, -110.31, -109.91,
                                 -98.614};

        ext_send_start( UART_MODULE );

        // Simulate Cell 0
        uart_send_byte( UART_MODULE, '0');
        uart_send_byte( UART_MODULE, '\n');
        // Simulate Active Area 0
        uart_send_byte( UART_MODULE, '0');
        uart_send_byte( UART_MODULE, '\n');

        int i;
        for (i = 0; i < sizeof(sample_vData) / sizeof(float); i++)
        {
         char vLine[6];
         memset(vLine, ' ', 6);
         ftoa(sample_vData[i], vLine, 2);
         uart_send_multiple( UART_MODULE, (uint8_t*)vLine, sizeof(vLine));
         uart_send_byte( UART_MODULE, ' ');

         char iLine[8];
         memset(iLine, ' ', 8);
         ftoa(sample_iData[i], iLine, 3);
         uart_send_multiple( UART_MODULE, (uint8_t*)iLine, sizeof(iLine));
         uart_send_byte( UART_MODULE, '\n');

         P1->OUT ^= BIT0;
         __delay_cycles(100000);
        }
        ext_send_end( UART_MODULE );
        P1->OUT &= ~BIT0;

    }

    if (IFG & BIT1)
    {
        float sample_vData[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

        float sample_iData[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

        ext_send_start( UART_MODULE );

        // Simulate Cell 0
        uart_send_byte( UART_MODULE, '0');
        uart_send_byte( UART_MODULE, '\n');
        // Simulate Active Area 0
        uart_send_byte( UART_MODULE, '0');
        uart_send_byte( UART_MODULE, '\n');

        int i;
        for (i = 0; i < sizeof(sample_vData) / sizeof(float); i++)
        {
         char vLine[6];
         memset(vLine, ' ', 6);
         ftoa(sample_vData[i], vLine, 2);
         uart_send_multiple( UART_MODULE, (uint8_t*)vLine, sizeof(vLine));
         uart_send_byte( UART_MODULE, ' ');

         char iLine[8];
         memset(iLine, ' ', 8);
         ftoa(sample_iData[i], iLine, 3);
         uart_send_multiple( UART_MODULE, (uint8_t*)iLine, sizeof(iLine));
         uart_send_byte( UART_MODULE, '\n');

         P1->OUT ^= BIT0;
         __delay_cycles(100000);
        }
        ext_send_end( UART_MODULE );
        P1->OUT &= ~BIT0;
    }

    P1->IE = IE_store;
}
