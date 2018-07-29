/*
 * File:   MCP3421.c
 * Author: Achu
 *
 * Created on 7 July, 2018, 10:36 PM
 */


#include <xc.h>

#include "I2C_bitbang.h"

#include "MC3421.h"
#include "USART.h"

float lsb = MCP3421_LSB_12BITS;
char gain = MCP3421_GAIN_1_VALUE;
MCP3421ConfigurationRegister configuration;

/**
 * Sends a command to the MCP3421 to initiate a new convertion. One-Shot convertion
 * must be configured. In continuous mode this funtion as no effect on the ADC.
 */
void MCP3421InitiateConvertion(void)
{
    configuration.ReadyBit = MCP3421_INITIATE_CONVERTION;

    I2CDeviceSetDeviceAddress(MCP3421_DEVICE_ADDRESS);
    I2CDeviceWriteBytes(configuration.byte, 0, 0x00);
}

/**
 * Sets the MCP3421 convertion mode to Continuous convertion mode.
 */
void MCP3421SetConvertionModeContinuous(void)
{
    configuration.ConvertionModeBit = MCP3421_MODE_CONTINUOUS;

    I2CDeviceSetDeviceAddress(MCP3421_DEVICE_ADDRESS);
    I2CDeviceWriteBytes(configuration.byte, 0, 0x00);
}

/**
 * Sets the MCP3421 convertion mode to One-Shot mode.
 */
void MCP3421SetConvertionModeOneShot(void)
{
    configuration.ConvertionModeBit = MCP3421_MODE_ONE_SHOT;

    I2CDeviceSetDeviceAddress(MCP3421_DEVICE_ADDRESS);
    I2CDeviceWriteBytes(configuration.byte, 0, 0x00);
}

/**
 * Sets the convertion rate on the MCP3421.
 * @param value Value to select the convertion rate.
 * @warning Must select one of the following values:
 * #MCP3421_SAMPLE_RATE_240_SPS ,
 * #MCP3421_SAMPLE_RATE_60_SPS , 
 * #MCP3421_SAMPLE_RATE_15_SPS and
 * #MCP3421_SAMPLE_RATE_3_75_SPS.
 */
void MCP3421SetConvertionRate(unsigned char value)
{
    configuration.SampleRateSelectionBits = value;
    I2CDeviceSetDeviceAddress(MCP3421_DEVICE_ADDRESS);
    I2CDeviceWriteBytes(configuration.byte, 0, 0x00);

    switch (value)
    {
    case MCP3421_SAMPLE_RATE_240_SPS:
        lsb = MCP3421_LSB_12BITS;
        break;

    case MCP3421_SAMPLE_RATE_60_SPS:
        lsb = MCP3421_LSB_14BITS;
        break;

    case MCP3421_SAMPLE_RATE_15_SPS:
        lsb = MCP3421_LSB_16BITS;
        break;

    case MCP3421_SAMPLE_RATE_3_75_SPS:
        lsb = MCP3421_LSB_18BITS;
        break;

    default:
        lsb = MCP3421_LSB_12BITS;
        break;
    }
}

/**
 * Sets the gain of the internal PGA.
 * @param value Value to select the gain internally.
 * @warning Must select one of the following values:
 * #MCP3421_GAIN_1,
 * #MCP3421_GAIN_2,
 * #MCP3421_GAIN_4 and
 * #MCP3421_GAIN_8.
 */
void MCP3421SetPGAGain(unsigned char value)
{
    configuration.PGAGainSelectionBits = value;
    I2CDeviceSetDeviceAddress(MCP3421_DEVICE_ADDRESS);
    I2CDeviceWriteBytes(configuration.byte, 0, 0x00);

    switch (value)
    {
    case MCP3421_GAIN_1:
        gain = MCP3421_GAIN_1_VALUE;
        break;

    case MCP3421_GAIN_2:
        gain = MCP3421_GAIN_2_VALUE;
        break;

    case MCP3421_GAIN_4:
        gain = MCP3421_GAIN_4_VALUE;
        break;

    case MCP3421_GAIN_8:
        gain = MCP3421_GAIN_8_VALUE;
        break;

    default:
        gain = MCP3421_GAIN_1_VALUE;
        break;
    }
}

/**
 * This funtion gets the value from the ADC. The funtion as blocking properties,
 *  if the convertion is not completed the device is continuous asked if the
 * convertion is completed. When is completed the value in volts is returned.
 * @return The voltage on the terminals of the ADC.
 */
long  MCP3421GetValue(void)
{
    long voltage,voltage1 = 0;
    long stat=0;
    MCP3421Data data;

    I2CDeviceSetDeviceAddress(MCP3421_DEVICE_ADDRESS);

        I2CStart();
        
        I2CWrite(MCP3421_DEVICE_ADDRESS);
        char buf[10];
        delay(1);
        voltage = I2CRead(ACK);
        voltage = (voltage<<8);
        voltage |= I2CRead(ACK);
        stat = I2CRead(NACK);
        I2CStop();
        
        return voltage;
}