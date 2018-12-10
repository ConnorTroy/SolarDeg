/*
 * ina219.c
 *
 *  Created on: Dec 9, 2018
 *      Author: Connor Troy
 *
 *  This driver handles communication with the INA219 voltage/current/power sensor
 */

#include "ina219.h"

void Init(const INA219_Config *config)
{
    /*
     * Initializes INA219 Devices with specified eUSCI module, addresses, and settings
     */
}


void reset(uint16_t address)
{
    /*
     * Reset all configurations of an INA219 module
     */
}


void updateConfig(uint16_t config)
{
    /*
     * Change Settings in the INA219 Configuration Register
     */
}


void writeCalibrationReg(uint16_t address, uint16_t value)
{
    /*
     * Writes a value to the INA219 Calibration Register
     */
}


uint16_t readConfigReg(uint16_t address)
{
    /*
     * Reads Configuration of the Configuration Register
     */
}


int readShuntVoltage(uint16_t address)
{
    /*
     * Returns INA219 Shunt Voltage in mV
     */
}


int readBusVoltage(uint16_t address)
{
    /*
     * Returns INA219 Bus Voltage in V
     */
}


int readPower(uint16_t address)
{
    /*
     * Returns INA219 Power in W
     */
}


int readCurrent(uint16_t address)
{
    /*
     * Returns INA219 Current in A
     */
}


uint16_t readCalibrationReg(uint16_t address)
{
    /*
     * Returns Configuration of the INA219 Calibration Register
     */
}
