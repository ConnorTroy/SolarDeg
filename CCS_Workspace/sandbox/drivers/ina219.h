/*
 * ina219.h
 *
 *  Created on: Dec 9, 2018
 *      Author: Connor Troy
 *
 *  This driver handles communication with the INA219 voltage/current/power sensor
 */

#ifndef DRIVERS_INA219_H_
#define DRIVERS_INA219_H_

#include <stdint.h>
#include <msp.h>

// Device Addressing
 #define ADDR_1                                                     0x40
 #define ADDR_2                                                     0x41
 #define ADDR_3                                                     0x42
 #define ADDR_4                                                     0x43
 #define ADDR_5                                                     0x44
 #define ADDR_6                                                     0x45
 #define ADDR_7                                                     0x46
 #define ADDR_8                                                     0x47
 #define ADDR_9                                                     0x48
 #define ADDR_10                                                    0x49
 #define ADDR_11                                                    0x4A
 #define ADDR_12                                                    0x4B
 #define ADDR_13                                                    0x4C
 #define ADDR_14                                                    0x4D
 #define ADDR_15                                                    0x4E
 #define ADDR_16                                                    0x4F

// Device Register Addressing
 #define CONFIG_ADDR                                                0x00
 #define SHUNT_VOLT_ADDR                                            0x01
 #define BUS_VOLT_ADDR                                              0x02
 #define POWER_ADDR                                                 0x03
 #define CURRENT_ADDR                                               0x04
 #define CALIBRATION_ADDR                                           0x05

// Configuration Register
 #define CONFIG_DEFAULT                                             0x399F

 #define CONFIG_RST                                                 0x8000
 #define CONFIG_BRNG                                                0x2000

 #define CONFIG_PGA_40                                              0x0000
 #define CONFIG_PGA_80                                              0x0800
 #define CONFIG_PGA_160                                             0x1000
 #define CONFIG_PGA_320                                             0x1800

 #define CONFIG_BADC_9_BIT                                          0x0000
 #define CONFIG_BADC_10_BIT                                         0x0080
 #define CONFIG_BADC_11_BIT                                         0x0100
 #define CONFIG_BADC_12_BIT                                         0x0180
 #define CONFIG_BADC_2_SAMPLES                                      0x0480
 #define CONFIG_BADC_4_SAMPLES                                      0x0400
 #define CONFIG_BADC_8_SAMPLES                                      0x0580
 #define CONFIG_BADC_16_SAMPLES                                     0x0600
 #define CONFIG_BADC_32_SAMPLES                                     0x0680
 #define CONFIG_BADC_64_SAMPLES                                     0x0700
 #define CONFIG_BADC_128_SAMPLES                                    0x0780

 #define CONFIG_SADC_9_BIT                                          0x0000
 #define CONFIG_SADC_10_BIT                                         0x0008
 #define CONFIG_SADC_11_BIT                                         0x0010
 #define CONFIG_SADC_12_BIT                                         0x0018
 #define CONFIG_SADC_2_SAMPLES                                      0x0048
 #define CONFIG_SADC_4_SAMPLES                                      0x0040
 #define CONFIG_SADC_8_SAMPLES                                      0x0058
 #define CONFIG_SADC_16_SAMPLES                                     0x0060
 #define CONFIG_SADC_32_SAMPLES                                     0x0068
 #define CONFIG_SADC_64_SAMPLES                                     0x0070
 #define CONFIG_SADC_128_SAMPLES                                    0x0078

 #define CONFIG_POWER_DOWN                                          0x0000
 #define CONFIG_SHUNT_VOLTAGE_TRIGGERED                             0x0001
 #define CONFIG_BUS_VOLTAGE_TRIGGERED                               0x0002
 #define CONFIG_SHUNT_BUS_TRIGGERED                                 0x0003
 #define CONFIG_ADC_DISABLE                                         0x0004
 #define CONFIG_SHUNT_VOLTAGE_CONTINUOUS                            0x0005
 #define CONFIG_BUS_VOLTAGE_CONTINUOUS                              0x0006
 #define CONFIG_SHUNT_BUS_CONTINUOUS                                0x0007


typedef struct
{
    EUSCI_B_Type * module;
    uint16_t configParameters;
    uint16_t addresses;
} INA219_Config;

/*
 * Constructor/ Destructor
 */

// Initializes INA219 Devices with specified eUSCI module, addresses, and settings
void generalInit(const INA219_Config *config);

// Reset all configurations of an INA219 module
void reset(uint16_t address);

// Change Settings in the INA219 Configuration Register
void updateConfig(uint16_t config);

// Writes a value to the INA219 Calibration Register
void writeCalibrationReg(uint16_t address, uint16_t value);

// Reads Configuration of the Configuration Register
uint16_t readConfigReg(uint16_t address);

// Returns INA219 Shunt Voltage in mV
int readShuntVoltage(uint16_t address);

// Returns INA219 Bus Voltage in V
int readBusVoltage(uint16_t address);

// Returns INA219 Power in W
int readPower(uint16_t address);

// Returns INA219 Current in A
int readCurrent(uint16_t address);

// Returns Configuration of the INA219 Calibration Register
uint16_t readCalibrationReg(uint16_t address);


#endif /* DRIVERS_INA219_H_ */
