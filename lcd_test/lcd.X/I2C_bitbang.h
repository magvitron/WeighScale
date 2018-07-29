/* 
 * File:   I2C_bitbang.h
 * Author: Achu
 *
 * Created on 7 July, 2018, 10:12 PM
 */

#ifndef I2C_BITBANG_H
#define	I2C_BITBANG_H

#ifdef	__cplusplus
extern "C" {
#endif

/********************************************************************
*
*                     Software License Agreement
*
*     ©2007 Microchip Technology Inc
*     Mirochip Technology Inc. ("Microchip") licenses this software to
*     you solely for the use with Microchip Products. The software is
*     owned by Microchip and is protected under applicable copyright
*     laws. All rights reserved.
*
*     SOFTWARE IS PROVIDED "AS IS." MICROCHIP EXPRESSLY DISCLAIMS ANY
*     WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
*     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
*     PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
*     BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
*     DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
*     PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
*     BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
*     ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
*
*******************************************************************
*
*  Filename:           PIC16_I2C_BITBANG.h
*  Date:               February 21, 2012
*  File Version:       1.0
*  Compiled using:     MPLAB X IDE v1.41
*                      XC8 v1.10
*
*  Author:             Eugen Ionescu
*  Company:            Microchip Technology Inc.
*
*******************************************************************/


unsigned char deviceAddressRead;
unsigned char deviceAddressWrite;

#define  I2C_Direction_Transmitter      0x00
#define  I2C_Direction_Receiver         0x01
#define SYSTEM_OSCILATOR    16000000UL
/**Selects the desired I2C clock speed.*/
#define I2C_SPEED           400000
/**Calculates the value to put on the baudrate register based on the desired speed.
 SYSTEM_OSCILATOR must contain the value of the oscilator. Check the formula for
 different microcontrollers.*/
#define I2CBAUDVALUE        (((SYSTEM_OSCILATOR/4)/I2C_SPEED)-1)
void bit_out(unsigned char data);               // outputs a bit to I2C bus
unsigned char bit_in(unsigned char data);
unsigned char i2c_wr(unsigned char i2c_data);   // writes a byte to the I2C bus
unsigned char i2c_rd(unsigned char ack);	// reads a byte from the I2C bus
void ack_poll(unsigned char control);
void I2CDeviceWriteBytes(unsigned char address,
                         unsigned char length,
                         unsigned char *data);

//#define DSDA	
//	#define DSCL    
//	#define SDA		
//	#define SCL     

#define SDA_TRIS  TRISAbits.TRISA5
#define SCL_TRIS  TRISAbits.TRISA4
#define SDA_IN     PORTAbits.RA5
#define SDA       LATAbits.LATA5
#define SCL       LATAbits.LATA4

#define ACK       0x00
#define NACK      0x80


#define PAGE_SIZE     64                     // EEPROM Page Size
#define EEPROM_WR     0xA0                   // WRITE instruction
#define EEPROM_RD     0xA1                   // READ instruction



#ifdef	__cplusplus
}
#endif








void I2CInit(void);
void I2CStart(void);
void I2CRestart(void);
void I2CStop(void);
char I2CAck (unsigned char control);
void I2CNotAck(void);
void I2CSendAddress(unsigned char Address, unsigned char I2C_Direction);
unsigned char I2CWrite(unsigned char data_out);
unsigned char I2CRead(unsigned char ack);

void I2CDeviceSetDeviceAddress(unsigned char address);


#endif /* _I2CDEV_H_ */