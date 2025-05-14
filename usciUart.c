/*************************************************************************************************
 * usciUart.c
 * - C implementation or source file for MSP430 UCSI UART A1
 *
 *  Author: Greg Scutt
 *  Created on: March 1, 2017
 *  Modified: February 26th, 2018
 **************************************************************************************************/

#include <msp430.h>
#include "usciUart.h"
#include <stdio.h>
#include <strings.h>

char rxBuffer[BUF_SZ];
const char errorMessage[] = "\n\rERROR FOUND\n\r";

/************************************************************************************
* Function: usciA1UartInit
* - configures UCA1 UART to use SMCLK, no parity, 8 bit data, LSB first, one stop bit
*  BAUD rate = 19.2Kbps with 16xoversampling.
*  assumes SMCLK = 2^20 Hz.
* argument:
* Arguments: none, but baud rate would be useful
*
* return: none
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void usciA1UartInit(){

	// READ THIS --> You must use UCSI Control BITS defined in msp430.h !!
	// In fact you must use pre-defined control bits whenever possible. no obscure HEX codes allowed anymore.

	P4SEL |= TXD_A1 | RXD_A1;
	UCA1CTL1 |= UCSWRST; 			// USCI A1  Software Reset Enabled
	//********************

	UCA1CTL1 	|= 	UCSSEL_2; 		// select SMCLK. User is responsible for setting this rate.

	UCA1CTL0	 = 	0; 				// RESET UCA1CTL0 before new configuration
	UCA1CTL0	&=	~UCPEN			// No Parity
				&	~UCMSB			// LSB First
				&	~UC7BIT			// 8 bits of data
				&	~UCSPB			// 1 stop bit
				&	~UCSYNC;		// UART Mode


	UCA1BR1 = 0;	// calculate pre-scaler
	UCA1BR0 = 3;	// calculate pre-scaler
	UCA1MCTL = (UCBRF_6 | UCOS16 | UCBRS_1);	// calculate first and second stage modulation

	UCA1CTL1 	&= ~UCSWRST; 		//  configured. take state machine out of reset.
	}


/************************************************************************************
* Function: usciA1UartTxChar
* - writes a single character to UCA1TXBUF if it is ready
* argument:
* Arguments: txChar - byte to be transmitted
*
* return: none
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void usciA1UartTxChar(unsigned char txChar){

	while (!(UCA1IFG & UCTXIFG)); // is this efficient ?
		UCA1TXBUF = txChar;	 // if TXBUF ready then transmit a byte by writing to it
}


/************************************************************************************
* Function: usciA1UartTxString
* - writes a C string of characters, one char at a time to UCA1TXBUF by calling
*   usciA1UartTxChar. Stops when it encounters  the NULL character in the string
*   does NOT transmit the NULL character
* argument:
* Arguments: txChar - pointer to char (string) to be transmitted
*
* return: number of characters transmitted
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void usciA1UartTxString(unsigned char *txChar){

    int i = 0;
    while(txChar[i] != '\0'){
        usciA1UartTxChar(txChar[i]);
        i++;
    }


	}

/************************************************************************************
* Function: usciA1UartTXBuffer
************************************************************************************/
int usciA1UartTXBuffer(char * buffer, int bufLen){
    int i = 0;
    int byteTrans = 0;
    char transData[225];
        while((i < bufLen) & (buffer[i] != '\0')){
            usciA1UartTxChar(buffer[i]);
            i++;
            byteTrans++;
        }
    sprintf(transData, "\n\r%d Characters Transmitted\n\r", byteTrans);
    usciA1UartTxString(transData);
    return byteTrans;
 }

/************************************************************************************
* Function: usciA1UartGets
************************************************************************************/
char * usciA1UartGets(char * rxString){
    int bufIndex = 0;
    do{
        while(!(UCA1IFG & UCRXIFG)){

        }
        if(UCRXIFG == 1){
        rxBuffer[bufIndex++] = UCA1RXBUF;
        usciA1UartTxChar(UCA1RXBUF);
        }
    }while((UCA1RXBUF != ENTER_KEY) & (bufIndex < (BUF_SZ-1)));

    if(rxString[0] == '\0')return NULL;

    rxBuffer[bufIndex] = '\0';
    memcpy(rxString, rxBuffer, bufIndex + 1);



    return * rxString;
}


#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;
  case 2:
      usciA1UartTxChar(UCA1RXBUF);
    break;
  case 4:break;
  default: break;
  }
}
