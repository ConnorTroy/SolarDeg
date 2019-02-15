/*
 * ext_connect.c
 *
 *  Created on: Feb 14, 2019
 *      Author: ctroy
 */


#include "ext_connect.h"


/*
 * Check if external machine is connected
 */
void poll_for_ext_connect(EUSCI_A_Type * module, uint8_t * buffer, uint32_t bufferSize)
{
    while(strstr( (char*)buffer, EXP_STR ) == NULL)
    {
        uart_send_multiple(module, POLL_STR, sizeof(POLL_STR));
        __delay_cycles(100000);
    }
}


/*
 * Send Start string to initialize data transfer
 */
void ext_send_start(EUSCI_A_Type * module)
{
    uart_send_multiple(module, SEND_START, sizeof(SEND_START));
}


/*
 * Send line to external machine
 */
void ext_send_data_line(EUSCI_A_Type * module, char* line, uint32_t len)
{

}


/*
 * Send End string to end data transfer
 */
void ext_send_end(EUSCI_A_Type * module)
{
    uart_send_multiple(module, SEND_END, sizeof(SEND_END));
}
