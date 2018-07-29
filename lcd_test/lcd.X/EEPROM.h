/* 
 * File:   EEPROM.h
 * Author: Achu
 *
 * Created on 28 July, 2018, 9:59 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

void EEPROM_Write (int address, char data);
char EEPROM_Read(int address);
void EEPROMWritelong(int address, long value);
long EEPROMReadlong(long address);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

