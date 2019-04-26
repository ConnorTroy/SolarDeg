/*
 * ina219.h
 *
 *  Created on: Apr 22, 2019
 *      Author: Connor Troy
 *
 *  This driver handles communication with the PCA PWM IC module
 */

#ifndef PCA_H_
#define PCA_H_


#include <msp.h>

#include "i2c.h"

// Register Definitions
#define     PCA_MODE_1_REG          0x00
#define     PCA_MODE_2_REG          0x01
#define     PCA_GRP_PWM_REG         0x12
//#define     PCA_GRP_FRQ_REG         0x13
#define     PCA_LED_OUT_0_REG       0x14
#define     PCA_LED_OUT_1_REG       0x15
#define     PCA_LED_OUT_2_REG       0x16
#define     PCA_LED_OUT_3_REG       0x17

const static uint8_t PCA_PWM_DUTY[] = {
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11
};

// Register Configurations
#define     PCA_MODE_1_SLEEP        0x10
#define     PCA_MODE_1_SUB_1        0x08
#define     PCA_MODE_1_SUB_2        0x04
#define     PCA_MODE_1_SUB_3        0x02
#define     PCA_MODE_1_ALLCALL      0x01

#define     PCA_MODE_2_DMBLNK       0x20
#define     PCA_MODE_2_INVRT        0x10
#define     PCA_MODE_2_OCH          0x08
#define     PCA_MODE_2_OUTDRV       0x04

#define     PCA_LED_OUT_OFF         0x00
#define     PCA_LED_OUT_ON_FULL_CTL 0xFF

// Address Declaration
#define     PCA_ADDR            0x50

// Function Declarations

void PCA_enable_output(EUSCI_B_Type * module);

void PCA_reset(EUSCI_B_Type * module);

void PCA_change_duty(EUSCI_B_Type * module, uint8_t pin_num, uint8_t val);


#endif /* PCA_H_ */
