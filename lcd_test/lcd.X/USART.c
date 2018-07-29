#include <xc.h>
#include "USART.h"
void send_string(char str[])
{
	int sndstr_cnt =0;
	
	while (str[sndstr_cnt] != 0x00)
	{
		txr(str[sndstr_cnt]);
		sndstr_cnt++;
	}
}

/**********************************UART******************************************************/

void USART_Init(int baud_rate)
{
    float temp;
    TRISCbits.TRISC6 = 0; // TX as output
    TRISCbits.TRISC7 = 1; // RX as input
    temp=(((float)(F_CPU)/(float)baud_rate)-1);     
    SPBRG=(int)temp;                /*baud rate=9600 SPBRG=(F_CPU /(64*9600))-1*/
    TXSTA=0x20;                     /*TX enable*/ 
    RCSTA=0x90;                     /*RX enable and serial port enable */
}
  void USART_newline()
  {
      
      txr(0x0a);
      txr(0x0d);
  }
void delay(int d)
{
    int i;
    for(i=0;i<=d;i++)
    {
    
    }
}

void txr(unsigned char temp)
{
    while (TXSTAbits.TRMT == 0) {};
    TXREG=temp;
    delay(1000);
    
}

// Implementation of itoa()
char* intToAscci(long num, char* str)
{
    long temp_num = num;
    int counter=0;
    int tempCount=0;
     while(temp_num != 0)
    {
        // n = n/10
        temp_num /= 10;
        counter++;
    }
    if(num<=0)
    {
        str[0] ='0';
        str[1] =0;
        return str;
    }
    tempCount = counter;
    while (num != 0)
    {
        int rem = num % 10;
        str[--counter] = rem+'0';
        num = num/10;
    }
    str[tempCount] = '\0'; // Append string terminator

    // Reverse the string
    //reverse(str, i);
 
    return str;
}
