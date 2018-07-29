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
void EEPROMWriteDouble(int p_address, long p_value);
float EEPROMReadDouble(int p_address);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

