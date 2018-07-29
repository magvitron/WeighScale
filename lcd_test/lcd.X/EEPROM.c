#include <xc.h>
#include "EEPROM.h"

void EEPROM_Write (int address, char data)
{
    /* Write Operation*/
    EEADR=address;		/* Write address to the EEADR register */
    EEDATA=data;		/* Copy data to the EEDATA register for
				write to EEPROM location */
    EECON1bits.EEPGD=0;		/* Access data EEPROM memory */
    EECON1bits.CFGS=0;		/* Access flash program or data memory */
    EECON1bits.WREN=1;		/* Allow write to the memory */
    INTCONbits.GIE=0;		/* Disable global interrupt */
    
    /* Assign below sequence to EECON2 Register is necessary
       to write data to EEPROM memory */

    EECON2=0x55;
    EECON2=0xaa;
    
    EECON1bits.WR=1;		/* Start writing data to EEPROM memory */
    INTCONbits.GIE=1;		/* Enable interrupt*/
    
    while(PIR2bits.EEIF==0);	/* Wait for write operation complete */
    PIR2bits.EEIF=0;		/* Reset EEIF for further write operation */
}
char EEPROM_Read(int address)
{
    /*Read operation*/
    EEADR=address;	/* Read data at location 0x00*/
    EECON1bits.WREN=0;	/* WREN bit is clear for Read operation*/  
    EECON1bits.EEPGD=0;	/* Access data EEPROM memory*/
    EECON1bits.RD=1;	/* To Read data of EEPROM memory set RD=1*/
    return(EEDATA);
}

void EEPROMWriteDouble(int p_address, long p_value)
{
    unsigned char   Byte1 = (char)((p_value >> 0) & 0xFF);
 unsigned char Byte2 = (int)((p_value >> 8) & 0xFF);
 unsigned char Byte3 = (int)((p_value >> 16) & 0xFF);
 unsigned char Byte4 = (int)((p_value >> 24) & 0xFF);

 EEPROM_Write(p_address, Byte1);
 EEPROM_Write(p_address + 1, Byte2);
 EEPROM_Write(p_address + 2, Byte3);
 EEPROM_Write(p_address + 3, Byte4);
}

float EEPROMReadDouble(int p_address)
{
 unsigned char Byte1 = EEPROM_Read(p_address);
 unsigned char Byte2 = EEPROM_Read(p_address + 1);
 unsigned char Byte3 = EEPROM_Read(p_address + 2);
 unsigned char Byte4 = EEPROM_Read(p_address + 3);

 long firstTwoBytes = ((Byte1 << 0) & 0xFF) + ((Byte2 << 8) & 0xFF00);
 long secondTwoBytes = (((Byte3 << 0) & 0xFF) + ((Byte4 << 8) & 0xFF00));
 secondTwoBytes *= 65536; // multiply by 2 to power 16 - bit shift 24 to the left

 return (firstTwoBytes + secondTwoBytes);
}
