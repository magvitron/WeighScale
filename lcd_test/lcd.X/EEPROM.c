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

void EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 unsigned char s by using bitshift.
      //One = Most significant -> Four = Least significant unsigned char 
      unsigned char  four = (value & 0xFF);
      unsigned char  three = ((value >> 8) & 0xFF);
      unsigned char  two = ((value >> 16) & 0xFF);
      unsigned char  one = ((value >> 24) & 0xFF);

      //Write the 4 unsigned char s into the eeprom memory.
      EEPROM_Write(address, four);
      EEPROM_Write(address + 1, three);
      EEPROM_Write(address + 2, two);
      EEPROM_Write(address + 3, one);
      }

long EEPROMReadlong(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM_Read(address);
      long three = EEPROM_Read(address + 1);
      long two = EEPROM_Read(address + 2);
      long one = EEPROM_Read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }

