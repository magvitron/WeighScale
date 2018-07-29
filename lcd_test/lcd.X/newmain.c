/*
   File:   lcd.main
   Author: Achu

   Created on 7 June, 2018, 11:11 PM
*/


/*
   PIC18F4550 Configuration Bit Settings
   http://www.electronicwings.com
*/


/* PIC18F4550 Configuration Bit Settings*/

/* 'C' source line config statements*/

#include <xc.h>
#include "I2C_bitbang.h"
#include "delay.h"
#include "USART.h"
#include "EEPROM.h"
#include "MC3421.h"
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#define ADC_TIMES 20
#pragma config PLLDIV = 5       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 2       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HSPLL_HS // Oscillator Selection bits (Internal oscillator, CLKO function on RA6, EC used by USB (INTCKO))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)


//#define F_OSC 8000000                       /* define F_OSC for finding bit rate
// here oscillator frequency is 8 MHz so define as 800000 */
//#define I2C_CLOCK 100000                    /* I2C clock frequency is 100 kHz*/
//#define BITRATE ((F_OSC/(4*I2C_CLOCK))-1)  /* find bit rate to assign this value to SSPADD register*/

/*******************************initialization******************************************/
#define CLOCK_FREQ 48000000


#define SW4   PORTBbits.RB2
#define DSW4  TRISBbits.TRISB2

#define SW2   PORTBbits.RB1
#define DSW2  TRISBbits.TRISB1

#define SW3   PORTBbits.RB0
#define DSW3  TRISBbits.TRISB0

//#define ADD0 PORTAbits.RA0// to define here
//#define ADD0_LAT LATAbits.LA0
//#define DATA_TRISA TRISAbits.RA0  //data
//
//#define ADSK LATAbits.LA1 // to define here
//#define CLOCK_TRISA TRISAbits.RA1 // clock

#define ADD0 PORTAbits.RA5// to define here
#define ADD0_LAT LATAbits.LA5
#define DATA_TRISA TRISAbits.RA5  //data

#define ADSK LATAbits.LA4 // to define here
#define CLOCK_TRISA TRISAbits.RA4 // clock

#define KEY_IN PORTBbits.RB0


//#define  DLEDG  TRISDbits.TRISD6
//#define  DLEDR  TRISDbits.TRISD7
//#define  DLEDB  TRISDbits.TRISD3

//#define DSDA  TRISAbits.TRISA5
//  #define DSCL    TRISAbits.TRISA4

//  #define RSDA    PORTAbits.RA5

/**************************UART******************************************************/
unsigned char b, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, tx_buf[10], rdy, ack1, ack2, page, charr[4], gainst, cct, cci, adc;
signed long rez1, rez2, rez3, rez4, rez, rezz, calib, rezult2, rezult3, constt3;
unsigned long int i, thousand, hundred, ten, ones, adc_val;
long sample = 0;
float val = 0;
long count = 0;
/************************i2c functions**************************************/
unsigned long ReadCount(void);
void init(void);
unsigned char compute(void);
void calibrate();
/*********************Proto-Type Declaration*****************************/
void main(void) {
  char buff[10];
  int weight = 0;
  TRISB = 0;
  OSCCON = 0x72;
  init();
  MSdelay(3);
  USART_Init(9600);
  USART_newline();
  send_string(__TIME__);
  USART_newline();
  send_string (__DATE__);
  USART_newline();
  int cnt = 0;
  char buff1[10];
  USART_newline();
  calibrate();

  while (1)
  {
    if (KEY_IN == 0)
    {
      val = 0;
      sample = 0;
      weight = 0;
      count = 0;
      calibrate();
    }
    count = (long)ReadCount();
    weight = (((count - sample) / val) - 2 * ((count - sample) / val));
    intToAscci(weight, buff);
    send_string(buff);
    send_string("  gm");
    USART_newline();
    MSdelay(1000);
  }
}
unsigned long ReadCount(void) {
  
  unsigned long Count;
  unsigned char i;
  DATA_TRISA = 0 ;
  ADD0_LAT = 1;
  ADSK = 0;
  DATA_TRISA = 1;
  Count = 0;
  while (ADD0);
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
void calibrate()
{
      char buff[10];
  send_string("Calibrating...");
  for (int i = 0; i < 100; i++)
  {
    count = ReadCount();
    sample += count;
    MSdelay(20);
    intToAscci(count, buff);
    send_string(buff);
    
  }
  sample /= 100;

  count = 0;
  send_string("Put the weight & wait");
  while (count < 2000)//wait till the weight is in the scales
  {
    count = ReadCount();
    count = count - sample ;
    MSdelay(10);
    intToAscci(count, buff);
    send_string(buff);
  }
  MSdelay(2000);
  send_string("Re-caliberating..");
  for (int i = 0; i < 100; i++)
  {
    count = ReadCount();
    val += sample - count;
    MSdelay(20);
    intToAscci(count, buff);
    send_string(buff);
  }
  val = val / 100.0;
  val = val / 300.0;    // put here your calibrating weight
}
/**********************************I2C FUN****************************************************/

void init(void)
{
  ADCON1 = 0b00001111;
  CMCON = 0b00000111;
  CVRCON = 0;
  UCON = 0;
  CCP1CON = 0;
  CCP2CON = 0;

  SCL = 1;
  DSW2 = 1;
  DSW3 = 1;
  DSW4 = 1;
  calib = 0;
  page = 1;
  MSdelay(100);
  SDA_TRIS = 1;                   // SDA1 (RC4) = input
  SCL_TRIS = 0;                   // SCL1 (RC3) = output
  SCL = 1;                        // SCL is high
  USART_Init(9600);
  TRISA0 = 1;
  DATA_TRISA = 1;
  CLOCK_TRISA = 0;

}
unsigned char compute(void)
{
  b1 = b1 * 2;
  b2 = b2 * 4;
  b3 = b3 * 8;
  b4 = b4 * 16;
  b5 = b5 * 32;
  b6 = b6 * 64;
  b7 = b7 * 128;
  rez1 = b0 + b1 + b2 + b3 + b4 + b5 + b6 + b7;
  b9 = b9 * 2;
  b10 = b10 * 4;
  b11 = b11 * 8;
  b12 = b12 * 16;
  b13 = b13 * 32;
  b14 = b14 * 64;
  b15 = b15 * 128;
  rez2 = b8 + b9 + b10 + b11 + b12 + b13 + b14 + b15;
  b17 = b17 * 2;
  rez3 = b16 + b17;
  rez = rez3 * 65536 + rez2 * 256 + rez1;
  //rez=999999999;
  rez4 = rez & 0b00000000000000100000000000000000;
  char buf[20];
  intToAscci(rez, buf); /* ultoi is not working have to implement this function
  insitu*/
  send_string(buf);
  txr(0x0a);
  txr(0x0D);
  //send_string();
}
