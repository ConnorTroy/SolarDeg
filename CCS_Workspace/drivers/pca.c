/*
 * ina219.h
 *
 *  Created on: Apr 22, 2019
 *      Author: Connor Troy
 *
 *  This driver handles communication with the PCA PWM IC module
 */

#include "pca.h"

void PCA_enable_output(EUSCI_B_Type * module)
{
    I2C_setSlaveAddress(module, PCA_ADDR);

    uint8_t message = 0x01;
    uint8_t TX_Data[] = {PCA_MODE_1_REG, message};
    I2C_send(module, TX_Data, 2);

    TX_Data[0] = PCA_MODE_2_REG;
    TX_Data[1] = PCA_MODE_2_INVRT + PCA_MODE_2_OUTDRV + 1;
    I2C_send(module, TX_Data, 2);

    TX_Data[0] = PCA_LED_OUT_0_REG;
    TX_Data[1] = PCA_LED_OUT_ON_FULL_CTL;
    I2C_send(module, TX_Data, 2);

    TX_Data[0] = PCA_LED_OUT_1_REG;
    I2C_send(module, TX_Data, 2);

    TX_Data[0] = PCA_LED_OUT_2_REG;
    I2C_send(module, TX_Data, 2);

    TX_Data[0] = PCA_LED_OUT_3_REG;
    I2C_send(module, TX_Data, 2);
}

void PCA_reset(EUSCI_B_Type * module)
{
    uint8_t TX_Data[] = {PCA_GRP_PWM_REG, 0};
    I2C_send(module, TX_Data, 2);
}

void PCA_change_duty(EUSCI_B_Type * module, uint8_t pin_num, uint8_t val)
{
    I2C_setSlaveAddress(module, PCA_ADDR);

    uint8_t TX_Data[] = {PCA_PWM_DUTY[pin_num], val};
    I2C_send(module, TX_Data, 2);
}
