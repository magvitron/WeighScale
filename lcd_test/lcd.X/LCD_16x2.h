/* 
 * File:   LCD_16x2.h
 * Author: Achu
 *
 * Created on 29 July, 2018, 5:11 PM
 */

#ifndef LCD_16X2_H
#define	LCD_16X2_H

#ifdef	__cplusplus
extern "C" {
#endif
void LCD_Init();                    /*Initialize LCD*/
void LCD_Command(unsigned char );   /*Send command to LCD*/
void LCD_Char(unsigned char x);     /*Send data to LCD*/
void LCD_print(char str[]);
void LCD_Clear();                   /*Clear LCD Screen*/



#ifdef	__cplusplus
}
#endif

#endif	/* LCD_16X2_H */

