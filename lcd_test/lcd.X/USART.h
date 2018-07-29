/* 
 * File:   USART.h
 * Author: Achu
 *
 * Created on 7 July, 2018, 10:30 PM
 */

#ifndef USART_H
#define	USART_H

#ifdef	__cplusplus
extern "C" {
#endif
#define F_CPU 8000000/64

void txr(unsigned char temp);
void delay(int d);
void USART_Init(int baud_rate);
void send_string(char str[]);
char* intToAscci(long num, char* str);
void reverse(char str[], int length);
void swap(int *xp, int *yp);
void USART_newline();

#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

