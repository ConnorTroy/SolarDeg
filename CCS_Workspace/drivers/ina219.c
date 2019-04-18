/*
 * ina219.c
 *
 *  Created on: Dec 9, 2018
 *      Author: Connor Troy
 *
 *  This driver handles communication with the INA219 voltage/current/power sensor
 */

#include "ina219.h"

void INA219_generalInit(INA219** sensorList, uint8_t numSensors)
{
    /*
     * Initializes INA219 Devices with specified eUSCI module, addresses, and settings
     */

    int i;
    for (i = 0; i < numSensors; i++){

        INA219* sensor = sensorList[i];

        // Update eUSCI slave address
        I2C_setSlaveAddress(sensor->module, sensor->address);

        // Separate config data into 2 bytes
        uint16_t configuration = sensor->busVoltageRange + sensor->productGain + sensor->busADCResolution + sensor->shuntADCResolution + sensor->operatingMode;

        uint8_t MSB = (uint8_t) (configuration >> 8);
        uint8_t LSB = (uint8_t) configuration;

        // Create transmit array
        uint8_t TX_Data[3] = {INA219_CONFIG_ADDR, MSB, LSB};

        // Send transmit array
        I2C_send(sensor->module, TX_Data, 3);
    }
}


void INA219_reset(INA219* sensor)
{
    /*
     * Reset all configurations of an INA219 module
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Separate data into 2 bytes
    uint8_t MSB = (uint8_t) (INA219_CONFIG_RST >> 8);
    uint8_t LSB = (uint8_t) INA219_CONFIG_RST;

    // Create transmit array
    uint8_t TX_Data[3] = {INA219_CONFIG_ADDR, MSB, LSB};

    // Send transmit array
    I2C_send(sensor->module, TX_Data, 3);

    // Update local sensor configuration
    INA219_decodeConfiguration(sensor, INA219_CONFIG_DEFAULT);
}


void INA219_updateConfig(INA219* sensor, uint16_t configParameters)
{
    /*
     * Change Settings in the INA219 Configuration Register
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Separate new config data into 2 bytes
    uint8_t MSB = (uint8_t) (configParameters >> 8);
    uint8_t LSB = (uint8_t) configParameters;

    // Create transmit array
    uint8_t TX_Data[3] = {INA219_CONFIG_ADDR, MSB, LSB};

    // Send transmit array
    I2C_send(sensor->module, TX_Data, 3);

    // Update local sensor configuration
    INA219_decodeConfiguration(sensor, configParameters);
}


void INA219_writeCalibrationReg(INA219* sensor, uint16_t value)
{
    /*
     * Writes a value to the INA219 Calibration Register
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Separate new config data into 2 bytes
//    value = value << 1;
    uint8_t MSB = (uint8_t) (value >> 8);
    uint8_t LSB = (uint8_t) value;

    // Create transmit array
    uint8_t TX_Data[3] = {INA219_CALIBRATION_ADDR, MSB, LSB};

    // Send transmit array
    I2C_send(sensor->module, TX_Data, 3);
}


uint16_t INA219_readConfigReg(INA219* sensor)
{
    /*
     * Reads Configuration of the Configuration Register
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Create transmit data
    uint8_t TX_Data = INA219_CONFIG_ADDR;

    // Send register data
    I2C_send(sensor->module, &TX_Data, 1);

    // Recieve data in calibration register
    I2C_receive(sensor->module, RX_Data, 2);

    return (RX_Data[0] << 8) + RX_Data[1];
}


float INA219_readShuntVoltage(INA219* sensor)
{
    /*
     * Returns INA219 Shunt Voltage in mV
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Create transmit data
    uint8_t TX_Data = INA219_SHUNT_VOLT_ADDR;

    // Send register data
    I2C_send(sensor->module, &TX_Data, 1);

    // Recieve data in calibration register
    I2C_receive(sensor->module, RX_Data, 2);

    // Convert register data to human readable format
    uint16_t regData = (RX_Data[0] << 8) + RX_Data[1];

    // Clear sign bits
    uint16_t num_sign_bits = 4 - (INA219_CONFIG_PGA_320 >> 11);
    uint16_t tmp_val = (uint16_t) (regData << num_sign_bits);
    tmp_val = tmp_val >> num_sign_bits;

    //Remove two's complement
    tmp_val --;

    //Remove complement
    tmp_val |= 0x8000;
    tmp_val = ~tmp_val;

    //Replace Sign
    int sign = (-2 * ((regData >> 15) & 1)) + 1;
    float shuntVoltage = (sign * tmp_val) / 100 ;

    return shuntVoltage;
}


float INA219_readBusVoltage(INA219* sensor)
{
    /*
     * Returns INA219 Bus Voltage in V
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Create transmit data
    uint8_t TX_Data = INA219_BUS_VOLT_ADDR;

    // Send register data
    I2C_send(sensor->module, &TX_Data, 1);

    // Recieve data in register
    I2C_receive(sensor->module, RX_Data, 2);

    return ((RX_Data[0] << 8) + RX_Data[1]) >> 3;
}


float INA219_readPower(INA219* sensor)
{
    /*
     * Returns INA219 Power in W
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Create transmit data
    uint8_t TX_Data = INA219_POWER_ADDR;

    // Send register data
    I2C_send(sensor->module, &TX_Data, 1);

    // Recieve data in register
    I2C_receive(sensor->module, RX_Data, 2);

    return (RX_Data[0] << 8) + RX_Data[1];
}


float INA219_readCurrent(INA219* sensor)
{
    /*
     * Returns INA219 Current in A
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Create transmit data
    uint8_t TX_Data = INA219_CURRENT_ADDR;

    // Send register data
    I2C_send(sensor->module, &TX_Data, 1);

    // Recieve data in register
    I2C_receive(sensor->module, RX_Data, 2);

    int val = (RX_Data[0] << 8) + RX_Data[1];
    float ret = (float) val * (sensor -> max_current / 32768) * 1000;

    return ret;
}


uint16_t INA219_readCalibrationReg(INA219* sensor)
{
    /*
     * Returns Configuration of the INA219 Calibration Register
     */

    // Update eUSCI slave address
    I2C_setSlaveAddress(sensor->module, sensor->address);

    // Create transmit data
    uint8_t TX_Data = INA219_CALIBRATION_ADDR;

    // Send register data
    I2C_send(sensor->module, &TX_Data, 1);

    // Recieve data in calibration register
    I2C_receive(sensor->module, RX_Data, 2);

    return (RX_Data[0] << 8) + RX_Data[1];
}


void INA219_decodeConfiguration(INA219* sensor, uint16_t configParameters)
{
    /*
     * Decodes Register Configuration and stores values in sensor struct
     */

    sensor->busVoltageRange = configParameters & INA219_CONFIG_BRNG_MASK;
    sensor->productGain = configParameters & INA219_CONFIG_PGA_MASK;
    sensor->busADCResolution = configParameters & INA219_CONFIG_BADC_MASK;
    sensor->shuntADCResolution = configParameters & INA219_CONFIG_SADC_MASK;
    sensor->operatingMode = configParameters & INA219_CONFIG_OPERATING_MODE_MASK;
}

uint16_t INA219_calculate_calibration(INA219* sensor)
{
    float numer = 0.04096;
    float denom = ((sensor -> max_current / 32768) * 100);

    float calib = numer / denom;

    return (uint16_t) calib;
}

