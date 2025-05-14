/*************************************************************************************************
 * usciUart.h
 * - - C interface file for MSP430 USCI UART A1, A0
 *
 *  Author: Greg Scutt
 *  Created on: March 1, 2017
 *  Modified: Feb 19, 2018
 **************************************************************************************************/

#ifndef USCIUART_H_
#define USCIUART_H_

#define     TXD_A1          BIT4            //Transmit Data on P4.4
#define     RXD_A1          BIT5            //Recieve Data on P4.5
#define     ENTER_KEY       0x0D
#define     BUF_SZ          100

extern char rxBuffer[];
extern const char errorMessage[];

void usciA1UartInit();
void usciA1UartTxChar(unsigned char txChar);
void usciA1UartTxString(unsigned char * txChar);
char * usciA1UartGets(char * rxString);
int usciA1UartTXBuffer(char * buffer, int bufLen);

#endif /* USCIUART_H_ */
