
//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include <stdlib.h>
#include "LCD1602A.h"


#define BTEST 0        // Test the binary clutch positions on/off
#define LCD 1          // display LCD


//***PROGRAM TUNING*********************************************************************************************
#define TOLERANCE 150    // Maximum difference between position counts before the program engages the clutch
#define MAXENCPOS 4600   // Maximum encoder position (assumes all encoders are the same)
#define ENCTIMER 30      // How often the encoder pulses are read (ms)
#define BINARYAWD 258    // maximum position of the binary clutch (AWD)
#define BINARYRWD 265    // minimum position of the binary clutch (RWD)
#define DYNAMICAWD 0     // Maximum position of the dynamic clutch (AWD)
#define DYNAMICRWD 12375 // Minimum position of the dynamic clutch (RWD)
#define DYNAMICINC 25    // Dcount Increment amount
#define SWEEPDELAY 1000  // how much the delay variable is delaying by
//**************************************************************************************************************

//***FRONT LEFT ENCODER*****************************************************************************************
int32_t oldCountFL = 0;                  // stores the previous position counter value
int32_t newCountFL = 0;                  // stores the new position counter value
int32_t currentEncoderPositionFL = 0;    // stores the current encoder position
int32_t FLPos = 0;                       // Valid Front Left Encoder position
//**************************************************************************************************************

//***FRONT RIGHT ENCODER****************************************************************************************
int32_t oldCountFR = 0;                  // stores the previous position counter value
int32_t newCountFR = 0;                  // stores the new position counter value
int32_t currentEncoderPositionFR = 0;    // stores the current encoder position
int32_t FRPos = 0;                       // Valid Front Right Encoder position
//**************************************************************************************************************

//***BACK AXLE ENCODER******************************************************************************************
int32_t oldCountB = 0;                   // stores the previous position counter value
int32_t newCountB = 0;                   // stores the new position counter value
int32_t currentEncoderPositionB = 0;     // stores the current encoder position
int32_t BPos = 0;                        // Valid Rear Encoder position
//**************************************************************************************************************

//***PROGRAM CONTROL********************************************************************************************
int16_t TurnDiff = 0;            // difference between FL and FR pulses
int16_t SlipDiff = 0;            // difference between the average FL and FR pulses and B
int32_t Dcount = DYNAMICRWD;     // sweeps the dynamic clutch
int32_t Delay = 0;               // delays the clock allowing the dynamic PWM to sweep slower if needed
//**************************************************************************************************************
uint16_t awd_ratio = 0;
uint16_t rwd_ratio = 0;

//***Function Declaration**************************************************************************************
int32_t MakeValidPosition(int32_t Pos); // makes sure the position does not roll over and cause the positions to be completely different



//
// Main
//
void main(void)

{
    // Configure system clock and PLL, enable peripherals, and configure flash if used.
    Device_init();
    Device_initGPIO();

    // Initialize the PIE module and vector table.
    Interrupt_initModule();
    Interrupt_initVectorTable();

    Board_init();    // Initialize settings

    LCD_GPIO_Init();
    LCD_init();

    // set encoders max position count (assume all encoders are the same)
    EQEP_setPositionCounterConfig(myEQEP1_BASE,EQEP_POSITION_RESET_UNIT_TIME_OUT, MAXENCPOS);
    EQEP_setPositionCounterConfig(myEQEP2_BASE,EQEP_POSITION_RESET_UNIT_TIME_OUT, MAXENCPOS);
    EQEP_setPositionCounterConfig(myEQEP3_BASE,EQEP_POSITION_RESET_UNIT_TIME_OUT, MAXENCPOS);

    // set up eQEP unit timer (how often encoder pulses are checked)
    EQEP_enableUnitTimer(myEQEP1_BASE,((ENCTIMER*1000000)/5));
    EQEP_enableUnitTimer(myEQEP2_BASE,((ENCTIMER*1000000)/5));
    EQEP_enableUnitTimer(myEQEP3_BASE,((ENCTIMER*1000000)/5));

    EINT;     // Enable global interrupts.
    ERTM;     // Enable real-time debug.


    while(BTEST){ // Servo position test
        DEVICE_DELAY_US(1500000); // delay
        EPWM_setCounterCompareValue(myEPWM0_BASE, EPWM_COUNTER_COMPARE_A, BINARYRWD); // RWD position
        DEVICE_DELAY_US(1500000); // delay
        EPWM_setCounterCompareValue(myEPWM0_BASE, EPWM_COUNTER_COMPARE_A, BINARYAWD); // AWD position
    }

    while(1){
      TurnDiff = abs(FRPos - FLPos);            // calculate the difference between the front encoders
      SlipDiff = abs(BPos - ((FRPos+FLPos)/2)); // calculate the difference between the average of the front encoders and the rear encoders
       awd_ratio = 50 - (Dcount * 50) / DYNAMICRWD;
       rwd_ratio = 100 - awd_ratio;

      if(DYNAMICRWD < Dcount < DYNAMICAWD)      // makes sure that the Dynamic clutch cant go out of range
      {
          Dcount = DYNAMICRWD;
      }
      // All Wheel Drive
      if((TurnDiff > TOLERANCE) | ((SlipDiff > TOLERANCE) && (BPos > ((FRPos+FLPos)/2)))){ // checks if any of the wheels are spinning faster than the others (wheel slipping)
          if((Dcount > DYNAMICAWD) && (Delay >= SWEEPDELAY)){ // delays Dcount if it is sweeping the dynamic PWM too fast

              Dcount -= DYNAMICINC; // decrement Dcount
              Delay = 0;            // reset the delay
          }
          EPWM_setCounterCompareValue(myEPWM0_BASE, EPWM_COUNTER_COMPARE_A, BINARYAWD); // Engages the Binary clutch to its full AWD position
          EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, Dcount);    // Sweeps the Dynamic clutch towards its AWD position
      }
      // Rear Wheel Drive
      else{ // if all the wheels are turning around the same speed within a set tolerance
          if((Dcount < DYNAMICRWD) && (Delay >= SWEEPDELAY)){ // delays Dcount if it is sweeping the dynamic PWM too fast

              Dcount += DYNAMICINC; // increment Dcount
              Delay = 0;            // reset the delay
          }
          EPWM_setCounterCompareValue(myEPWM0_BASE, EPWM_COUNTER_COMPARE_A, BINARYRWD); // Disengages the Binary clutch to its full RWD position
          EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, Dcount);    // Sweeps the Dynamic clutch back towards its RWD position
      }
      Delay++; // increment delay

    }

}



// EQEP1 ISR- Front Left encoder monitoring
__interrupt void
INT_myEQEP1_ISR(void)
{
    currentEncoderPositionFL = EQEP_getPosition(myEQEP1_BASE);     // Save current encoder position value for monitoring
    newCountFL = EQEP_getPositionLatch(myEQEP1_BASE); // gets the position of the Front Left encoder when the ISR goes off

    FLPos = MakeValidPosition(newCountFL); // makes sure if the encoder is spinning in the negative direction the position is correct

    EQEP_clearInterruptStatus(myEQEP1_BASE,EQEP_INT_UNIT_TIME_OUT|EQEP_INT_GLOBAL); // Clear EQEP1 interrupt flag
    Interrupt_clearACKGroup(INT_myEQEP1_INTERRUPT_ACK_GROUP); // issue an ACK
 }

// EQEP2 ISR- Front Right encoder monitoring
__interrupt void
INT_myEQEP2_ISR(void)
{
    currentEncoderPositionFR = EQEP_getPosition(myEQEP2_BASE); // Save current encoder position value for monitoring
    newCountFR = EQEP_getPositionLatch(myEQEP2_BASE); // gets the position of the Front Right encoder when the ISR goes off

    FRPos = MakeValidPosition(newCountFR); // makes sure if the encoder is spinning in the negative direction the position is correct

    EQEP_clearInterruptStatus(myEQEP2_BASE,EQEP_INT_UNIT_TIME_OUT|EQEP_INT_GLOBAL); // Clear EQEP2 interrupt flag
    Interrupt_clearACKGroup(INT_myEQEP2_INTERRUPT_ACK_GROUP); // issue an ACK
 }

// EQEP3 ISR- Rear Axle encoder monitoring
__interrupt void
INT_myEQEP3_ISR(void)
{
    currentEncoderPositionB = EQEP_getPosition(myEQEP3_BASE);    // Save current encoder position value for monitoring
    newCountB = EQEP_getPositionLatch(myEQEP3_BASE); // gets the position of the Rear encoder when the ISR goes off

    BPos = MakeValidPosition(newCountB); // makes sure if the encoder is spinning in the negative direction the position is correct
    if(LCD){ // updates lcd screen every 30 ms
        displayWheelSpeeds(FLPos, FRPos, BPos);
        displayDynamicRatio(awd_ratio,rwd_ratio);
    }
    EQEP_clearInterruptStatus(myEQEP3_BASE,EQEP_INT_UNIT_TIME_OUT|EQEP_INT_GLOBAL); // Clear EQEP3 interrupt flag
    Interrupt_clearACKGroup(INT_myEQEP3_INTERRUPT_ACK_GROUP); // issue an ACK
 }

int32_t MakeValidPosition(int32_t Pos){ // makes sure the position does not roll over and cause the positions to be completely different

    if((MAXENCPOS - Pos) < Pos){  // checks if the current position is in the positive or negative direction
        return (MAXENCPOS - Pos); // returns a valid position when the position is negative
    }
    else{
        return Pos; // returns the original position if it was already in the right direction
    }

}



//
// End of File
//
