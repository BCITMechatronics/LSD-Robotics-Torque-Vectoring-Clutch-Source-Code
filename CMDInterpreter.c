/*
 * UsciCmdInterpreter.c
 *
 *  Created on: Jan. 9, 2022
 *      Author: Autumn Friesen
 */

#include <CmdInterpreter.h>
#include <msp430.h>
#include <string.h>
#include <usciUart.h>
#include <stdlib.h>
#include <timerA0UpMode.h>



#define ARRAYSIZE 50
#define MODE_FLAT 0
#define MODE_BUMPY 1
#define MODE_ROUGH 2
#define MODE_POTHOLE 3
#define MODE_BACKWHEEL 4
#define MODE_FRONTWHEEL 5

//Flat Ground
int Flat[ARRAYSIZE] = {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
//Bumpy Ground
int RBump[ARRAYSIZE] = {2,8,4,7,3,6,5,2,8,5,7,3,6,5,2,8,5,7,3,6,5,2,8,4,7,3,6,5,2,8,5,7,3,6,5,2,8,4,7,3,6,5,2,8,4,7,3,6,5,2};
int LBump[ARRAYSIZE] = {3,7,5,8,2,6,4,3,7,5,8,2,6,4,3,7,5,8,2,6,4,3,7,5,8,2,6,4,3,7,5,8,2,6,4,3,7,5,8,2,6,4,3,7,5,8,2,6,4,3};
//Rough Ground
int RRough[ARRAYSIZE] =  {9,1,8,2,9,1,8,2,5,5,8,2,9,1,8,2,5,5,9,1,8,2,9,1,8,2,5,5,9,1,8,2,9,1,8,2,5,5,9,1,8,2,9,1,8,2,5,5,5,5};
int LRough[ARRAYSIZE] =  {1,9,2,8,1,9,2,8,5,5,2,8,1,9,2,8,5,5,1,9,2,8,1,9,2,8,5,5,1,9,2,8,1,9,2,8,5,5,1,9,2,8,1,9,2,8,5,5,5,5};
//Potholes
int RPot[ARRAYSIZE] = {5,5,5,5,5,2,5,5,5,9,2,5,5,5,9,2,5,5,5,9,5,5,5,9,5,2,5,5,5,5,2,5,9,5,5,2,5,9,5,5,2,5,5,9,5,5,5,5,5,5};
int LPot[ARRAYSIZE] = {5,5,5,5,5,9,5,5,5,2,9,5,5,5,2,9,5,5,5,2,5,5,5,2,5,9,5,5,5,5,9,5,2,5,5,9,5,2,5,5,9,5,5,2,5,5,5,5,5,5};
//Back Wheel
int Back[ARRAYSIZE] =  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
//Front Wheel
int Front[ARRAYSIZE] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};


CMD cmds[MAX_CMDS]={0};
CMD *cmd;
char rxbuffer2[BUF_SZ];

const char invalidCmd[] = "\n\rERROR, INVALID COMMAND\n\r";
const char checkArgs[] = "\n\rERROR, WRONG NUMBER OF ARGUMENTS\n\r";

void randomDelay(int TerrainMode);

/*
 * Name: cmdsinit
 *
 * Function:
 * Initializes the list of commands for the command interpreter
 *
 * Inputs: none
 * Returns: none
 */
void cmdsinit()
{
    //Load command list elements here

    cmds[0] = (CMD){"FLAT", 0, {0}};  //Flat Command
    cmds[1] = (CMD){"BUMPY", 0, {0}}; // Bumpy Command
    cmds[2] = (CMD){"ROUGH", 0, {0}}; // Rough Command
    cmds[3] = (CMD){"POT", 0, {0}};  // Pothole Command
    cmds[4] = (CMD){"BSLIP", 0, {0}}; // BackSlip Command
    cmds[5] = (CMD){"FSLIP", 0, {0}}; // FrontSlip Command

    P6DIR |= 0xFF;
    P6OUT &= ~0xFF;//set Port 6 as outputs


    //This must change whenever you add, modify, or remove commands



}

/*
 * Name: parseCmd
 *
 * Function:
 * Parses string for command name and arguments. Validates command by calling "validateCmd". If the command is valid
 * the function will parse the remaining args in the string into the args array of the CMD struct. If the number of args
 * is not the same as nArgs for the command the function will return an error as the command is invalid.
 *
 * Input: [char *] String containing command name and comma delimited arguments.
 * Returns: [int] The command number. (It's element in the array of commands.) If the command is invalid return -1.
 */
int parseCmd(char * cmdLine)
{
    char * tok;
    char * delim = " "; //delimiters
    int cmdIndex = -1;   //index for executing cmds

    //Process command string into tokens
    tok = strtok(cmdLine, delim);
    if (tok == NULL) {
            usciA1UartTxString(invalidCmd);
            return -1; //no input/invalid command
        }
    //Validate the command name
    cmdIndex = validateCmd(tok);
    if (cmdIndex == -1) {
        usciA1UartTxString(invalidCmd);
        return -1; //invalid command name
    }

    cmd = &cmds[cmdIndex];

//
//    //Parse args
//    int argCount = 0;
//    while ((tok = strtok(NULL, delim)) != NULL) {
//        if (argCount < MAX_ARGS) {
//            cmd->args[argCount++] = atoi(tok); //convert string to int and store
//        } else {
//            usciA1UartTxString(checkArgs);
//            return -1; //too many args
//        }
//    }
//
//    //Check arg number
//    if (argCount != cmd->nArgs) {
//        usciA1UartTxString(checkArgs);
//        return -1; //arg count wrong
//    }

    return cmdIndex;

    //This function should be written in a way that it can be used for ANY number of commands or arguments
    //and should not need to be modified if you use the command interpreter for other commands.
}

/*
 * Name: validateCmd
 *
 * Function:
 * Called by "parseCmd" to compare the "command name" portion of the string being parsed against the list of commands.
 *
 * Input: [char *] The "command name" portion of a command string
 * Returns: [int] Returns the index of the command in the command list array for valid commands. Returns -1 for invalid commands.
 */
int validateCmd(char * cmdName)
{
    //Compare the name string token to the list of command names in your cmds array.
    int i = 0;
    int idx = -1;
    int invalidCmd = 1;

    while (invalidCmd && i < MAX_CMDS) {
        invalidCmd = strcmp(cmdName, cmds[i++].name);
    }
    if (!invalidCmd)
        idx = i - 1;
    return idx;
    //This function should be written in a way that it can be used for ANY number of commands or arguments
    //and should not need to be modified if you use the command interpreter for other commands.
}

/*
 * Name: executeCMD
 *
 * Function:
 * Executes the selected command.
 *
 * Input: [int] The command number. (It's element in the array of commands.)
 * Return: [int] Return 0 if the command executes correctly. Return -1 for an invalid command.
 */
int executeCMD(int cmdIndex)
{
    cmd = &cmds[cmdIndex];

    //All code to execute the commands go in here.
    switch (cmdIndex){
    case -1:{
        usciA1UartTxString(invalidCmd);
        return -1;
    }
    case 0:{ //FLAT
        TA0CCR2 = DUTY50;
        TA0CCR1 = DUTY50;
        break;
    }
    case 1:{ //bumpy
        int bufIndex = 0;
        usciA1UartTxString("\n\rActive\n\r");
        do{
            while(!(UCA1IFG & UCRXIFG)){


        int r = (rand() % (ARRAYSIZE+1));
        switch(RBump[r]){
                   case(0):{
                       TA0CCR1 = DUTY0;
                       break;
                   }
                   case(1):{
                       TA0CCR1 = DUTY10;
                       break;
                   }
                   case(2):{
                       TA0CCR1 = DUTY20;
                       break;
                   }
                   case(3):{
                       TA0CCR1 = DUTY30;
                       break;
                   }
                   case(4):{
                       TA0CCR1 = DUTY40;
                       break;
                   }
                   case(5):{
                       TA0CCR1 = DUTY50;
                       break;
                   }
                   case(6):{
                       TA0CCR1 = DUTY60;
                       break;
                   }
                   case(7):{
                       TA0CCR1 = DUTY70;
                       break;
                   }
                   case(8):{
                       TA0CCR1 = DUTY80;
                       break;
                   }
                   case(9):{
                       TA0CCR1 = DUTY90;
                       break;
                   }
        }

             switch(LBump[r]){
             case(0):{
                 TA0CCR2 = DUTY0;
                  break;
             }
             case(1):{
                 TA0CCR2 = DUTY10;
                 break;
             }
             case(2):{
                 TA0CCR2 = DUTY20;
                 break;
             }
             case(3):{
                 TA0CCR2 = DUTY30;
                 break;
             }
             case(4):{
                 TA0CCR2 = DUTY40;
                 break;
             }
             case(5):{
                 TA0CCR2 = DUTY50;
                 break;
             }
             case(6):{
                 TA0CCR2 = DUTY60;
                 break;
             }
             case(7):{
                 TA0CCR2 = DUTY70;
                 break;
             }
             case(8):{
                 TA0CCR2 = DUTY80;
                 break;
             }
             case(9):{
                 TA0CCR2 = DUTY90;
                 break;
             }
             }
             randomDelay(MODE_BUMPY);
            }
            if(UCRXIFG == 1){
            rxbuffer2[bufIndex++] = UCA1RXBUF;
            usciA1UartTxChar(UCA1RXBUF);
            }
        }while(UCA1RXBUF != ENTER_KEY);
        TA0CCR2 = DUTY50;
        TA0CCR1 = DUTY50;
        usciA1UartTxString("\n\rDisabled\n\r");
        break;

    }
    case 2:{ //rough
        int bufIndex = 0;
                usciA1UartTxString("\n\rActive\n\r");
                do{
                    while(!(UCA1IFG & UCRXIFG)){


                int r = (rand() % (ARRAYSIZE+1));
                switch(RRough[r]){
                           case(0):{
                               TA0CCR1 = DUTY0;
                               break;
                           }
                           case(1):{
                               TA0CCR1 = DUTY10;
                               break;
                           }
                           case(2):{
                               TA0CCR1 = DUTY20;
                               break;
                           }
                           case(3):{
                               TA0CCR1 = DUTY30;
                               break;
                           }
                           case(4):{
                               TA0CCR1 = DUTY40;
                               break;
                           }
                           case(5):{
                               TA0CCR1 = DUTY50;
                               break;
                           }
                           case(6):{
                               TA0CCR1 = DUTY60;
                               break;
                           }
                           case(7):{
                               TA0CCR1 = DUTY70;
                               break;
                           }
                           case(8):{
                               TA0CCR1 = DUTY80;
                               break;
                           }
                           case(9):{
                               TA0CCR1 = DUTY90;
                               break;
                           }
                }

                     switch(LRough[r]){
                     case(0):{
                         TA0CCR2 = DUTY0;
                          break;
                     }
                     case(1):{
                         TA0CCR2 = DUTY10;
                         break;
                     }
                     case(2):{
                         TA0CCR2 = DUTY20;
                         break;
                     }
                     case(3):{
                         TA0CCR2 = DUTY30;
                         break;
                     }
                     case(4):{
                         TA0CCR2 = DUTY40;
                         break;
                     }
                     case(5):{
                         TA0CCR2 = DUTY50;
                         break;
                     }
                     case(6):{
                         TA0CCR2 = DUTY60;
                         break;
                     }
                     case(7):{
                         TA0CCR2 = DUTY70;
                         break;
                     }
                     case(8):{
                         TA0CCR2 = DUTY80;
                         break;
                     }
                     case(9):{
                         TA0CCR2 = DUTY90;
                         break;
                     }
                     }
                     randomDelay(MODE_ROUGH);
                    }
                    if(UCRXIFG == 1){
                    rxbuffer2[bufIndex++] = UCA1RXBUF;
                    usciA1UartTxChar(UCA1RXBUF);
                    }
                }while(UCA1RXBUF != ENTER_KEY);
                TA0CCR2 = DUTY50;
                TA0CCR1 = DUTY50;
                usciA1UartTxString("\n\rDisabled\n\r");
                break;
    }

    case 3:{ //potholes
        int bufIndex = 0;
                usciA1UartTxString("\n\rActive\n\r");
                do{
                    while(!(UCA1IFG & UCRXIFG)){


                int r = (rand() % (ARRAYSIZE+1));
                switch(RPot[r]){
                           case(0):{
                               TA0CCR1 = DUTY0;
                               break;
                           }
                           case(1):{
                               TA0CCR1 = DUTY10;
                               break;
                           }
                           case(2):{
                               TA0CCR1 = DUTY20;
                               break;
                           }
                           case(3):{
                               TA0CCR1 = DUTY30;
                               break;
                           }
                           case(4):{
                               TA0CCR1 = DUTY40;
                               break;
                           }
                           case(5):{
                               TA0CCR1 = DUTY50;
                               break;
                           }
                           case(6):{
                               TA0CCR1 = DUTY60;
                               break;
                           }
                           case(7):{
                               TA0CCR1 = DUTY70;
                               break;
                           }
                           case(8):{
                               TA0CCR1 = DUTY80;
                               break;
                           }
                           case(9):{
                               TA0CCR1 = DUTY90;
                               break;
                           }
                }

                     switch(LPot[r]){
                     case(0):{
                         TA0CCR2 = DUTY0;
                          break;
                     }
                     case(1):{
                         TA0CCR2 = DUTY10;
                         break;
                     }
                     case(2):{
                         TA0CCR2 = DUTY20;
                         break;
                     }
                     case(3):{
                         TA0CCR2 = DUTY30;
                         break;
                     }
                     case(4):{
                         TA0CCR2 = DUTY40;
                         break;
                     }
                     case(5):{
                         TA0CCR2 = DUTY50;
                         break;
                     }
                     case(6):{
                         TA0CCR2 = DUTY60;
                         break;
                     }
                     case(7):{
                         TA0CCR2 = DUTY70;
                         break;
                     }
                     case(8):{
                         TA0CCR2 = DUTY80;
                         break;
                     }
                     case(9):{
                         TA0CCR2 = DUTY90;
                         break;
                     }
                     }
                     randomDelay(MODE_POTHOLE);
                    }
                    if(UCRXIFG == 1){
                    rxbuffer2[bufIndex++] = UCA1RXBUF;
                    usciA1UartTxChar(UCA1RXBUF);
                    }
                }while(UCA1RXBUF != ENTER_KEY);
                TA0CCR2 = DUTY50;
                TA0CCR1 = DUTY50;
                usciA1UartTxString("\n\rDisabled\n\r");
                break;
    }
    case 4:{ //back Slip
        TA0CCR2 = DUTY20;
        TA0CCR1 = DUTY20;
        break;
    }
    case 5:{ //front Slip
        TA0CCR2 = DUTY90;
        TA0CCR1 = DUTY90;
        break;
    }



    }
    //This must change whenever you add, modify, or remove commands
}


void randomDelay(int TerrainMode) {

    unsigned int minDelayMs = 200;
    unsigned int maxDelayMs = 600;

    switch (TerrainMode) {
            case MODE_FLAT:
                minDelayMs = 600;
                maxDelayMs = 1000;
                break;
            case MODE_BUMPY:
                minDelayMs = 300;
                maxDelayMs = 600;
                break;
            case MODE_ROUGH:
                minDelayMs = 150;
                maxDelayMs = 400;
                break;
            case MODE_POTHOLE:
                minDelayMs = 100;
                maxDelayMs = 400;
                break;
            case MODE_BACKWHEEL:
                minDelayMs = 600;
                maxDelayMs = 1000;
                break;
            case MODE_FRONTWHEEL:
                minDelayMs = 600;
                maxDelayMs = 1000;
                break;
        }

    unsigned int delayMs = minDelayMs + (rand() % (maxDelayMs - minDelayMs + 1));

      volatile unsigned int i;
      for (i = 0; i < delayMs; i++) {
          __delay_cycles(1000);  // 1ms delay per loop (for 1MHz clock)
      }

}
