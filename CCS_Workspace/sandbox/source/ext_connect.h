/*
 * ext_connect.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Connor Troy
 */

#ifndef SOURCE_EXT_CONNECT_H_
#define SOURCE_EXT_CONNECT_H_


#include <string.h>

#include "msp.h"
#include "uart.h"

#define POLL_STR        "SOLAR_SOAKER_FW\n"
#define EXP_STR         "SS_EXTERNAL"
#define SEND_START      "START\n"
#define SEND_END        "END\n"

// Check if external machine is connected
void poll_for_ext_connect(EUSCI_A_Type * module, uint8_t * buffer, uint32_t bufferSize);

// Send Start string to initialize data transfer
void ext_send_start(EUSCI_A_Type * module);

// Send line to external machine
void ext_send_data_line(EUSCI_A_Type * module, char* line, uint32_t len);

// Send End string to end data transfer
void ext_send_end(EUSCI_A_Type * module);

#endif /* SOURCE_EXT_CONNECT_H_ */
