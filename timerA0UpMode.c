/*
 * timers.c
 *
 *  Created on: Dec. 4, 2024
 *      Author: apan18
 */
#include <msp430.h>
#include <timerA0UpMode.h>

long int dutyCycle[] = {1,100,200,300,400,500,600,700,800,900,1000,900,800,700,600,500,400,300,200,100};
int state = 0;




//-----------------------------------------------------------------------------------------------------------------------------
// timerA0UpModeInit
// Arguments: ta0ccr0Val value to put into CCR0 (+1), divide to select the clock divisor,
//             clockSelect to select which clock to use (TASSEL_<3:0>)
// Returns: none

void timerA0UpModeInit(unsigned int ta0ccr0Val, int divide, int clockSelect, int countMode)
{
    TA0CTL = MC_0; // stops the clock
    TA0CTL &= ~TAIE;


    TA0CTL = clockSelect | countMode | divide; // to control the clock parameters used for CCRx
    TA0CCR0 = ta0ccr0Val; // loads value into CCR0 register (# + 1)

}


//-----------------------------------------------------------------------------------------------------------------------------
// timerA0UpModeChannelConfig
// Arguments: outMode to select the output mode of the channel connected to the clock
//            channel to select the channel(s) used for output, intEn to enable/disable interrupts
// Returns: none
void timerA0UpModeChannelConfig (unsigned char channel, unsigned char outmode, unsigned char intEn, unsigned char EIS)
{

    // sets channels used to desired output mode (mod<7:0>)

    if(channel == 1)
    {
        TA0CCTL1 = outmode;
        P1DIR |= BIT2; // sets P1 BIT2 to output
        P1SEL |= BIT2; // sets P1 to peripheral so it is always running and outputs the clock signal

    }
    else if(channel == 2)
    {
        TA0CCTL2 = outmode;
        P1DIR |= BIT3; // sets P1 BIT3 to output
        P1SEL |= BIT3; // sets P1 to peripheral so it is always running and outputs the clock signal
        P1DIR |= BIT5;
        P2DIR |= BIT7;

    }

    else if(channel == 3)
       {
           TA0CCTL2 = outmode;
           P1DIR |= BIT3; // sets P1 BIT3 to output
           P1SEL |= BIT3; // sets P1 to peripheral so it is always running and outputs the clock signal
           P1DIR |= BIT5;
           P2DIR |= BIT7;

           TA0CCTL1 = outmode;
           P1DIR |= BIT2; // sets P1 BIT2 to output
           P1SEL |= BIT2; // sets P1 to peripheral so it is always running and outputs the clock signal
           P1DIR |= BIT4;
           P2DIR |= BIT2;

       }


    if(intEn)
    {


       switch(EIS)
       {
       case 0:
       {
           TA0CCTL0 |= CCIE;
           break;
       }
       case 1:
       {
           TA0CCTL1 |= CCIE;
           break;
       }
       case 2:
       {
           TA0CCTL2 |= CCIE;
           break;
       }
       case 3:
       {
           TA0CCTL3 |= CCIE;
           break;
       }
       case 4:
       {
           TA0CCTL4 |= CCIE;
           break;
       }
       }
       __enable_interrupt();
    }

}

void sweep()
{
    int i;
    for (i = 0; i < 20; i++)
    {
        TA0CCR3 = dutyCycle[i];
        _delay_cycles(250000);
    }
}
