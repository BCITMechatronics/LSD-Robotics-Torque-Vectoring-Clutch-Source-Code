#include <msp430.h> 
#include <timerA0UpMode.h>
#include <usciUart.h>
#include <CmdInterpreter.h>
#include <string.h>
#include <stdlib.h>


/**
 * main.c
 */
int main(void)
{
    volatile int i = 0;
    char rxString[50];
    int cmdIndex = -1;

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    timerA0UpModeChannelConfig (3, OUTMOD_7, 0, 3);
    timerA0UpModeInit(PWM_FREQ, ID_0, TASSEL_2, MC_1 );


     usciA1UartInit();
     cmdsinit();       // Initialize commands


     while (1) {
         //get input from user via UART
         if (usciA1UartGets(rxString) != NULL) {
             //parse the command
             cmdIndex = parseCmd(rxString);
             usciA1UartTxString("\n\r");

             if (cmdIndex != -1) {
                 //command is valid, execute it
                 executeCMD(cmdIndex);
             }
         }
     }



	return 0;
}
