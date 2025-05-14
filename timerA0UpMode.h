#include <msp430.h>
/*
 * timers.h
 *
 *  Created on: Dec. 4, 2024
 *      Author: apan18
 */

#ifndef TIMERA0UPMODE_H_
#define TIMERA0UPMODE_H_



#define BLINK_RATE 62499 // 0.25s or 62499 cycles b/c CCR0 + 1 and clock divided by 2
#define PWM_FREQ 0x3E7 // set frequency to 1kHz
#define mod0 OUTMOD_0
#define mod1 OUTMOD_1
#define mod2 OUTMOD_2
#define mod3 OUTMOD_3
#define mod4 OUTMOD_4
#define mod5 OUTMOD_5
#define mod6 OUTMOD_6
#define mod7 OUTMOD_7
#define TOGGLING 1
#define WAITING 2

#define DUTY0 0
#define DUTY10 100
#define DUTY20 200
#define DUTY30 300
#define DUTY40 400
#define DUTY50 500
#define DUTY60 600
#define DUTY70 700
#define DUTY80 800
#define DUTY90 900



void blinkInit();
void timerA0UpModeInit (unsigned int ta0ccr0Val, int divide, int clockSelect, int countMode);
void hwPWMInit();
void timerA0UpModeChannelConfig (unsigned char channel, unsigned char outmode, unsigned char intEn, unsigned char EIS);


void sweep();


extern long int dutyCycle[];
extern int state;

#endif /* TIMERA0UPMODE_H_ */
