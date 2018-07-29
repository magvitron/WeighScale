/**
 *	\file main.c
 *	\brief main file
 *//*
   File:   lcd.main
   Author: ManuKrishnan

   Created on 29 June, 2018, 10:26 PM
*/
/* PIC18F4550 Configuration Bit Settings*/

/* 'C' source line config statements*/

#include <xc.h>
#include "delay.h"
#include "USART.h"
#include "EEPROM.h"
#include "LCD_16x2.h"
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#define ADC_TIMES 20
#pragma config PLLDIV = 5		// PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 2		// USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HSPLL_HS // Oscillator Selection bits (Internal oscillator, CLKO function on RA6, EC used by USB (INTCKO))
#pragma config FCMEN = OFF		// Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF		// Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF		// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON			// Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3			// Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF		// USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF		// Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768	// Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON		// CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF		 // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF	// Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON	   // MCLR Pin Enable bit (RE3 input pin enabled; MCLR pin disabled)

// CONFIG4L
#pragma config STVREN = ON		// Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF		 // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF		// Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF		// Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF		// Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF		// Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF		// Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF		// Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF		// Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF		// Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF		// Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF		// Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF		// Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF		// Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF		// Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF		// Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF		// Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF		// Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF		// Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF		// Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF		// Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF		// Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)


/*******************************initialization******************************************/
#define CLOCK_FREQ 48000000


#define SW4	  PORTBbits.RB2
#define DSW4  TRISBbits.TRISB2

#define SW2	  PORTBbits.RB1
#define DSW2  TRISBbits.TRISB1

#define SW3	  PORTBbits.RB0
#define DSW3  TRISBbits.TRISB0

#define ADD0 PORTAbits.RA5// to define here
#define ADD0_LAT LATAbits.LA5
#define DATA_TRISA TRISAbits.RA5  //data

#define ADSK LATAbits.LA4 // to define here
#define CLOCK_TRISA TRISAbits.RA4 // clock

#define KEY_IN PORTBbits.RB1


/**************************UART******************************************************/
unsigned char b, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, tx_buf[10], rdy, ack1, ack2, page, charr[4], gainst, cct, cci, adc;
signed long rez1, rez2, rez3, rez4, rez, rezz, calib, rezult2, rezult3, constt3;
unsigned long int i, thousand, hundred, ten, ones, adc_val;
long sample = 0;
long temp_val = 0;

float val = 0;
long count = 0;
/******************************************************************************/
unsigned long ReadCount(void);
void init(void);
unsigned char compute(void);
void calibrate();
/******************************************************************************/
/**
 *	\brief main Function, the execution starts here
 *
 *	\return Nothing
 *
 *	\details Nothing more.
 */
void main(void) {
  char buff[10];
  long address = 0;
  int weight = 0;
	int cnt = 0;
  char buff1[10];
  /******************************************************************************/
  TRISB = 0;
  OSCCON = 0x72;

  /****************************intialize*************************************/
  init();
  MSdelay(3);
  USART_Init(9600);
  USART_newline();
  send_string(__TIME__);
  USART_newline();
  send_string (__DATE__);
  USART_newline();
  /****************************************************************************/
  address = 0;
  sample = EEPROMReadlong(address);
  address += 4;
  temp_val = EEPROMReadlong(address);
  val = -1 * (temp_val);
  val = val / 300.0;	// put here your calibrating weight
/******************************************************************************/
  send_string("sample: ");
  intToAscci(sample, buff);
  send_string(buff);
  USART_newline();
/******************************************************************************/
  send_string("val: ");
  intToAscci(temp_val, buff);
  send_string(buff);
  USART_newline();
  /*check whether its the first run */
  if (sample == 0)
  {
	/*if it is the first run, then run caliberate*/
	calibrate();
  }
  /*Clear the lcd duh!*/
  LCD_Clear();
  /*Loop for ever!*/
  while (1)
  {
	  /*check for any caliberation request*/
	if (KEY_IN == 0)
	{
	  /*clear all the calib values first*/
	  val = 0;
	  sample = 0;
	  weight = 0;
	  count = 0;
	  /*run caliberatiob*/
	  calibrate();
	}
/******************************************************************************/
	/*Reading the weight*/
	count = (long)ReadCount();
	weight = (((count - sample) / val) - 2 * ((count - sample) / val));
 /******************************************************************************/
	/*print the weight to serial and LCD*/
	intToAscci(weight, buff);
	send_string(buff);
	send_string("  gm");
	USART_newline();
	/*cursor home */
	LCD_Command(0x02);
	LCD_print(buff);
	LCD_print(" gm");
	MSdelay(10);
  }
}
/**
 *	\brief Read the value from HX711 module
 *
 *	\return nothing
 *
 *	\details nothing more!
 */
unsigned long ReadCount(void) {
/*local variables*/
  unsigned long Count;
  unsigned char i;
  /*make the data pin output*/
  DATA_TRISA = 0 ;
  /*make the data pin high*/
  ADD0_LAT = 1;
  /*turn clck low*/
  ADSK = 0;
  DATA_TRISA = 1;
  Count = 0;
  /*wait till conversion is finished*/
  while (ADD0);
  /*take ADC data from hx711 by bitbanging refer datasheet for more*/
  for (i = 0; i < 24; i++) {
	ADSK = 1;
	Count = Count << 1;
	ADSK = 0;
	if (ADD0) Count++;
  }
  ADSK = 1;
  Count = Count ^ 0x800000;
  ADSK = 0;
  return (Count);
}
/**
 *	\brief Caiberate the scale and save in EEPROM
 *
 *	\return nothing
 *
 *	\details nothing more!
 */
void calibrate()
{
  char buff[10];
  long address = 0;
  long temp_val_local = 0;
  send_string("Calibrating...");
  LCD_Clear();
  LCD_print("Calibrating");
  /*taking 100 samples*/
  for (int i = 0; i < 100; i++)
  {
	count = ReadCount();
	sample += count;
	MSdelay(20);
	intToAscci(count, buff);
	send_string(buff);
	USART_newline();
  }
  /*take the average*/
  sample /= 100;
  /*clear the count*/
  count = 0;
  LCD_Clear();
  LCD_print("Put 300gm weight");
  send_string("Put the weight & wait");
  USART_newline();
  /*wait for the user to put the weight*/
  while (count < 2000)//wait till the weight is in the scales
  {
	count = ReadCount();
	count = count - sample ;
	MSdelay(20);
	send_string('.');
  }
  LCD_Clear();
  LCD_print("Detected weight..");
  /* wait for a bit...*/
  MSdelay(2000);
  LCD_Clear();
  LCD_print("Recalibrating..");
  send_string("Recalibrating..");
  /* read the value for 300 gm weight*/
  for (int i = 0; i < 100; i++)
  {
	count = ReadCount();
	/*here we are taking the negetive value in purposes*/
	temp_val += sample - count;
	/*delay for a bit*/
	MSdelay(20);
	send_string('.');
  }
 /* Write the data to EEPROM*/
  address = 0;
  EEPROMWritelong(address, sample);
  address += 4;
  /*take the average reading*/
  temp_val = temp_val / 100;
  /*multiply with -1 to make it positive*/
  temp_val_local = -1 * temp_val;
 /* write this to EEPROM*/
  EEPROMWritelong(address, temp_val_local);

  intToAscci(temp_val_local, buff);
  send_string(buff);

  address = 0;
 /* read this value from EEPROM for further things*/
  sample = EEPROMReadlong(address);
  address += 4;
  temp_val = EEPROMReadlong(address);
  val = -1 * (temp_val);
/*put here your calibrating weight*/
  val = val / 300.0;
}

/**
 *	\brief initialize all the modules
 *
 *	\return nothing
 *
 *	\details Nothing
 */
void init(void)
{
	/*controller configurations*/
  ADCON1 = 0b00001111;
  CMCON = 0b00000111;
  CVRCON = 0;
  UCON = 0;
  CCP1CON = 0;
  CCP2CON = 0;
 /*switches as inputs*/
  DSW2 = 1;
  DSW3 = 1;
  DSW4 = 1;
  calib = 0;
  page = 1;
  MSdelay(100);
/*init UART*/
  USART_Init(9600);
  TRISA0 = 1;
  DATA_TRISA = 1;
  CLOCK_TRISA = 0;
/*LCD*/
  LCD_Init();
  LCD_print("Weigh Scale");
  /*init is success!*/
}
