#include <xc.h>
#include "delay.h"
#include "LCD_16x2.h"
#define RS LATD0                    /*PIN 0 of PORTB is assigned for register select Pin of LCD*/
#define EN LATD2                    /*PIN 1 of PORTB is assigned for enable Pin of LCD */
#define ldata LATA                  /*PORTB(PB4-PB7) is assigned for LCD Data Output*/ 
#define LCD_Port TRISA
#define testPort LATB


void LCD_Init()
{
    LCD_Port = 0;                   /*PORT as Output Port*/
    TRISD0 =0;
    TRISD2 =0;
    MSdelay(20);                    /*15ms,16x2 LCD Power on delay*/
    //testPort =0b00000001;
    LCD_Command(0x02);              /*send for initialization of LCD 
                                     *for nibble (4-bit) mode */
    //testPort =0b00000010;
    LCD_Command(0x28);              /*use 2 line and 
                                     *initialize 5*8 matrix in (4-bit mode)*/
   // testPort =0b00000011;
	LCD_Command(0x01);              /*clear display screen*/
    //testPort =0b00000100;
    LCD_Command(0x0c);  
   // testPort =0b00000101;/*display on cursor off*/
	LCD_Command(0x06);              /*increment cursor (shift cursor to right)*/	   
   // testPort =0b00000110;
}


void LCD_print(char str[])
{
	int sndstr_cnt =0;
	
	while (str[sndstr_cnt] != 0x00)
	{
		LCD_Char(str[sndstr_cnt]);
		sndstr_cnt++;
	}
}
void LCD_Command(unsigned char cmd )
{
    char data=0;
    data = (data & 0x0f) | (cmd>>4);
	//ldata = (ldata & 0x0f) |(0xF0 & cmd);   /*Send higher nibble of command first to PORT*/ 
	ldata =  (cmd>>4);               /*Send lower nibble of data to PORT*/
    RS = 0;                                 /*Command Register is selected i.e.RS=0*/ 
	EN = 1;                                 /*High-to-low pulse on Enable pin to latch data*/ 
	NOP();
	EN = 0;
	MSdelay(3);
     data = (data & 0xf0) | (0x0f & cmd);
    ldata = (0x0f & cmd);
    //ldata = (ldata & 0x0f) | (cmd<<4);      /*Send lower nibble of command to PORT */
	EN = 1;
	NOP();
	EN = 0;
	MSdelay(3);
}



void LCD_Char(unsigned char dat)
{
	//ldata = (ldata & 0x0f) | (0xF0 & dat);   /*Send higher nibble of data first to PORT*/
    ldata =  (dat>>4);               /*Send lower nibble of data to PORT*/
    //ldata = (ldata & 0x0f) | (dat>>4);               /*Send lower nibble of data to PORT*/
	RS = 1;                                  /*Data Register is selected*/
	EN = 1;                                  /*High-to-low pulse on Enable pin to latch data*/
	NOP();
	EN = 0;
	MSdelay(3);
    ldata = (0x0f & dat);
    //ldata = (ldata & 0x0f) | (0x0f & dat);
    //ldata = (ldata & 0x0f) | (dat<<4);               /*Send lower nibble of data to PORT*/
	EN = 1;                         /*High-to-low pulse on Enable pin to latch data*/
	NOP();
	EN = 0;
	MSdelay(3);
    RS = 0;
}
void LCD_Clear()
{
   	LCD_Command(0x01);     /*clear display screen*/
    MSdelay(3);
}

