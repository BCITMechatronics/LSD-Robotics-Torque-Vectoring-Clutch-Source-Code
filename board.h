/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// EPWM1 -> myEPWM0 Pinmux
//
//
// EPWM1A - GPIO Settings
//
#define GPIO_PIN_EPWM1A 0
#define myEPWM0_EPWMA_GPIO 0
#define myEPWM0_EPWMA_PIN_CONFIG GPIO_0_EPWM1A
//
// EPWM1B - GPIO Settings
//
#define GPIO_PIN_EPWM1B 1
#define myEPWM0_EPWMB_GPIO 1
#define myEPWM0_EPWMB_PIN_CONFIG GPIO_1_EPWM1B

//
// EPWM2 -> myEPWM1 Pinmux
//
//
// EPWM2A - GPIO Settings
//
#define GPIO_PIN_EPWM2A 2
#define myEPWM1_EPWMA_GPIO 2
#define myEPWM1_EPWMA_PIN_CONFIG GPIO_2_EPWM2A
//
// EPWM2B - GPIO Settings
//
#define GPIO_PIN_EPWM2B 3
#define myEPWM1_EPWMB_GPIO 3
#define myEPWM1_EPWMB_PIN_CONFIG GPIO_3_EPWM2B

//
// EQEP1 -> myEQEP1 Pinmux
//
//
// EQEP1A - GPIO Settings
//
#define GPIO_PIN_EQEP1A 20
#define myEQEP1_EQEPA_GPIO 20
#define myEQEP1_EQEPA_PIN_CONFIG GPIO_20_EQEP1A
//
// EQEP1B - GPIO Settings
//
#define GPIO_PIN_EQEP1B 21
#define myEQEP1_EQEPB_GPIO 21
#define myEQEP1_EQEPB_PIN_CONFIG GPIO_21_EQEP1B
//
// EQEP1I - GPIO Settings
//
#define GPIO_PIN_EQEP1I 99
#define myEQEP1_EQEPINDEX_GPIO 99
#define myEQEP1_EQEPINDEX_PIN_CONFIG GPIO_99_EQEP1I

//
// EQEP2 -> myEQEP2 Pinmux
//
//
// EQEP2A - GPIO Settings
//
#define GPIO_PIN_EQEP2A 54
#define myEQEP2_EQEPA_GPIO 54
#define myEQEP2_EQEPA_PIN_CONFIG GPIO_54_EQEP2A
//
// EQEP2B - GPIO Settings
//
#define GPIO_PIN_EQEP2B 55
#define myEQEP2_EQEPB_GPIO 55
#define myEQEP2_EQEPB_PIN_CONFIG GPIO_55_EQEP2B
//
// EQEP2I - GPIO Settings
//
#define GPIO_PIN_EQEP2I 57
#define myEQEP2_EQEPINDEX_GPIO 57
#define myEQEP2_EQEPINDEX_PIN_CONFIG GPIO_57_EQEP2I

//
// EQEP3 -> myEQEP3 Pinmux
//
//
// EQEP3A - GPIO Settings
//
#define GPIO_PIN_EQEP3A 6
#define myEQEP3_EQEPA_GPIO 6
#define myEQEP3_EQEPA_PIN_CONFIG GPIO_6_EQEP3A
//
// EQEP3B - GPIO Settings
//
#define GPIO_PIN_EQEP3B 7
#define myEQEP3_EQEPB_GPIO 7
#define myEQEP3_EQEPB_PIN_CONFIG GPIO_7_EQEP3B
//
// EQEP3I - GPIO Settings
//
#define GPIO_PIN_EQEP3I 9
#define myEQEP3_EQEPINDEX_GPIO 9
#define myEQEP3_EQEPINDEX_PIN_CONFIG GPIO_9_EQEP3I

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
#define myEPWM0_BASE EPWM1_BASE
#define myEPWM0_TBPRD 279
#define myEPWM0_COUNTER_MODE EPWM_COUNTER_MODE_UP_DOWN
#define myEPWM0_TBPHS 0
#define myEPWM0_CMPA 140
#define myEPWM0_CMPB 0
#define myEPWM0_CMPC 0
#define myEPWM0_CMPD 0
#define myEPWM0_DBRED 0
#define myEPWM0_DBFED 0
#define myEPWM0_TZA_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM0_TZB_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM0_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED
#define myEPWM1_BASE EPWM2_BASE
#define myEPWM1_TBPRD 12500
#define myEPWM1_COUNTER_MODE EPWM_COUNTER_MODE_UP_DOWN
#define myEPWM1_TBPHS 0
#define myEPWM1_CMPA 12375
#define myEPWM1_CMPB 0
#define myEPWM1_CMPC 0
#define myEPWM1_CMPD 0
#define myEPWM1_DBRED 0
#define myEPWM1_DBFED 0
#define myEPWM1_TZA_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM1_TZB_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM1_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED

//*****************************************************************************
//
// EQEP Configurations
//
//*****************************************************************************
#define myEQEP1_BASE EQEP1_BASE
void myEQEP1_init();
#define myEQEP2_BASE EQEP2_BASE
void myEQEP2_init();
#define myEQEP3_BASE EQEP3_BASE
void myEQEP3_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myEQEP1
// ISR need to be defined for the registered interrupts
#define INT_myEQEP1 INT_EQEP1
#define INT_myEQEP1_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_myEQEP1_ISR(void);

// Interrupt Settings for INT_myEQEP2
// ISR need to be defined for the registered interrupts
#define INT_myEQEP2 INT_EQEP2
#define INT_myEQEP2_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_myEQEP2_ISR(void);

// Interrupt Settings for INT_myEQEP3
// ISR need to be defined for the registered interrupts
#define INT_myEQEP3 INT_EQEP3
#define INT_myEQEP3_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_myEQEP3_ISR(void);

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	EPWM_init();
void	EQEP_init();
void	INTERRUPT_init();
void	SYNC_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
