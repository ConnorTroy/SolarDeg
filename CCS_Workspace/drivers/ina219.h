/*
 * ina219.h
 *
 *  Created on: Dec 9, 2018
 *      Author: Connor Troy
 *
 *  This driver handles communication with the INA219 voltage/current/power sensor
 */

#ifndef _INA219_H_
#define _INA219_H_

#include <stdint.h>
#include <msp.h>

#include "i2c.h"

// Device Addressing
#define INA219_ADDR_1                                                      0x40
#define INA219_ADDR_2                                                      0x41
#define INA219_ADDR_3                                                      0x42
#define INA219_ADDR_4                                                      0x43
#define INA219_ADDR_5                                                      0x44
#define INA219_ADDR_6                                                      0x45
#define INA219_ADDR_7                                                      0x46
#define INA219_ADDR_8                                                      0x47
#define INA219_ADDR_9                                                      0x48
#define INA219_ADDR_10                                                     0x49
#define INA219_ADDR_11                                                     0x4A
#define INA219_ADDR_12                                                     0x4B
#define INA219_ADDR_13                                                     0x4C
#define INA219_ADDR_14                                                     0x4D
#define INA219_ADDR_15                                                     0x4E
#define INA219_ADDR_16                                                     0x4F

// Device Register Addressing
#define INA219_CONFIG_ADDR                                                 0x00
#define INA219_SHUNT_VOLT_ADDR                                             0x01
#define INA219_BUS_VOLT_ADDR                                               0x02
#define INA219_POWER_ADDR                                                  0x03
#define INA219_CURRENT_ADDR                                                0x04
#define INA219_CALIBRATION_ADDR                                            0x05

// Configuration Register
#define INA219_CONFIG_DEFAULT                                              0x399F

#define INA219_CONFIG_RST                                                  0x8000

#define INA219_CONFIG_BRNG_MASK                                            0x2000
#define INA219_CONFIG_BRNG_16V                                             0x0000
#define INA219_CONFIG_BRNG_32V                                             0x2000

#define INA219_CONFIG_PGA_MASK                                             0x1800
#define INA219_CONFIG_PGA_40                                               0x0000
#define INA219_CONFIG_PGA_80                                               0x0800
#define INA219_CONFIG_PGA_160                                              0x1000
#define INA219_CONFIG_PGA_320                                              0x1800

#define INA219_CONFIG_BADC_MASK                                            0x0780
#define INA219_CONFIG_BADC_9_BIT                                           0x0000
#define INA219_CONFIG_BADC_10_BIT                                          0x0080
#define INA219_CONFIG_BADC_11_BIT                                          0x0100
#define INA219_CONFIG_BADC_12_BIT                                          0x0180
#define INA219_CONFIG_BADC_2_SAMPLES                                       0x0480
#define INA219_CONFIG_BADC_4_SAMPLES                                       0x0400
#define INA219_CONFIG_BADC_8_SAMPLES                                       0x0580
#define INA219_CONFIG_BADC_16_SAMPLES                                      0x0600
#define INA219_CONFIG_BADC_32_SAMPLES                                      0x0680
#define INA219_CONFIG_BADC_64_SAMPLES                                      0x0700
#define INA219_CONFIG_BADC_128_SAMPLES                                     0x0780

#define INA219_CONFIG_SADC_MASK                                            0x0078
#define INA219_CONFIG_SADC_9_BIT                                           0x0000
#define INA219_CONFIG_SADC_10_BIT                                          0x0008
#define INA219_CONFIG_SADC_11_BIT                                          0x0010
#define INA219_CONFIG_SADC_12_BIT                                          0x0018
#define INA219_CONFIG_SADC_2_SAMPLES                                       0x0048
#define INA219_CONFIG_SADC_4_SAMPLES                                       0x0040
#define INA219_CONFIG_SADC_8_SAMPLES                                       0x0058
#define INA219_CONFIG_SADC_16_SAMPLES                                      0x0060
#define INA219_CONFIG_SADC_32_SAMPLES                                      0x0068
#define INA219_CONFIG_SADC_64_SAMPLES                                      0x0070
#define INA219_CONFIG_SADC_128_SAMPLES                                     0x0078

#define INA219_CONFIG_OPERATING_MODE_MASK                                  0x0007
#define INA219_CONFIG_POWER_DOWN                                           0x0000
#define INA219_CONFIG_SHUNT_VOLTAGE_TRIGGERED                              0x0001
#define INA219_CONFIG_BUS_VOLTAGE_TRIGGERED                                0x0002
#define INA219_CONFIG_SHUNT_BUS_TRIGGERED                                  0x0003
#define INA219_CONFIG_ADC_DISABLE                                          0x0004
#define INA219_CONFIG_SHUNT_VOLTAGE_CONTINUOUS                             0x0005
#define INA219_CONFIG_BUS_VOLTAGE_CONTINUOUS                               0x0006
#define INA219_CONFIG_SHUNT_BUS_CONTINUOUS                                 0x0007


typedef struct
{
    EUSCI_B_Type * module;
    uint16_t address;
    uint16_t busVoltageRange;
    uint16_t productGain;
    uint16_t busADCResolution;
    uint16_t shuntADCResolution;
    uint16_t operatingMode;
} INA219;

//static uint8_t Tx_Data[3] = {0,0,0};
static uint8_t RX_Data[2] = {0,0};

// Initializes INA219 Devices with specified eUSCI module, addresses, and settings
void INA219_generalInit(INA219** sensor, uint8_t numSensors);

// Reset all configurations of an INA219 module
void INA219_reset(INA219* sensor);

// Change Settings in the INA219 Configuration Register
void INA219_updateConfig(INA219* sensor, uint16_t configParameters);

// Writes a value to the INA219 Calibration Register
void INA219_writeCalibrationReg(INA219* sensor, uint16_t value);

// Reads Configuration of the Configuration Register
uint16_t INA219_readConfigReg(INA219* sensor);

// Returns INA219 Shunt Voltage in mV
float INA219_readShuntVoltage(INA219* sensor);

// Returns INA219 Bus Voltage in V
int INA219_readBusVoltage(INA219* sensor);

// Returns INA219 Power in W
int INA219_readPower(INA219* sensor);

// Returns INA219 Current in A
int INA219_readCurrent(INA219* sensor);

// Returns Configuration of the INA219 Calibration Register
uint16_t INA219_readCalibrationReg(INA219* sensor);

// Decodes Register Configuration and stores values in sensor struct
void INA219_decodeConfiguration(INA219* sensor, uint16_t configParameters);

#endif /* _INA219_H_ */
