/*****************************************************************************
 * File: main.c
 * Description: Control ECU MAIN Application
 *****************************************************************************/

#include "std_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>    

/* Drivers */
#include "../HAL/Memory_Manager.h" 
#include "../MCAL/SYSTICK.h"
#include "../HAL/Door_Lock.h"
#include "../HAL/Buzzer.h"
#include "../MCAL/GPTM.h"
#include "../MCAL/uart.h"
#include "../HAL/Button.h"
#include "../HAL/Led.h"


/* Definitions */
#define CMD_CHECK_PASS     '1'  // Simplified for keyboard input
#define CMD_SAVE_PASS      '2'
#define CMD_OPEN_DOOR      '3'
#define CMD_ALARM          '4'
#define CMD_SET_TIMEOUT    '5'
#define CMD_Check_First_Time '6'

#define PASSWORD_LEN       5


#define ALARM_DURATION_SEC  5
#define F_CPU               16000000UL 
#define ALARM_CYCLES        (ALARM_DURATION_SEC * F_CPU)

/* Global Variables */
uint32_t g_isfirstime = 0;
uint8_t g_doorTimeout = 5; 
int counter = 0;
/* ========================================================================== */
/* FUNCTION PROTOTYPES                                                        */
/* ========================================================================== */

void Control_SystemInit(void);
void Control_CheckPassword(char* password);
void Control_SavePassword(char* password);
void Control_OpenDoorSequence(void);
void Control_ActivateAlarm(void);
void Control_UpdateTimeout(uint32_t timeout);
void UART5_Handler(void);


/* ========================================================================== */
/* MAIN FUNCTION                                                              */
/* ========================================================================== */

int main(void) {
    
    /* 1. Initialization */
    Control_SystemInit();
    Memory_GetCheckFirstTime(&g_isfirstime);

    /* Load saved timeout from EEPROM on boot */
    
    uint32_t temp_timeout; 
    Memory_GetTimeout(&temp_timeout); 
    g_doorTimeout = (uint8_t)temp_timeout;

    /* 2. Main Loop */
    while(1){
        // CPU enters sleep mode here. 
        // Clock stops until a UART or GPIO interrupt occurs.
        __asm(" WFI ");
    }
    
        
}

/* ========================================================================== */
/* INTERRUPT SERVICE ROUTINES (ISRs)                                          */
/* ========================================================================== */

void UART5_Handler(void){
        uint32_t status = UART5_GetInterruptStatus();

        if (UART5_CheckRxInterrupt(status)) {
            char command = UART5_ReceiveChar();

            char Data[7] = {"00000"};
        
        
            switch(command) {
                case CMD_CHECK_PASS:
                    for(int i=0; i<5; i++){
                    Data[i] = UART5_ReceiveChar();
                    }
                    Data[5] = '\0'; 

                    Control_CheckPassword(Data);
                    break;
                
                case CMD_SAVE_PASS:
                    for(int i=0; i<5; i++){
                        Data[i] = UART5_ReceiveChar();
                        }
                    Data[5] = '\0'; 

                    Control_SavePassword(Data);
                    break;
                
                case CMD_OPEN_DOOR:
                    Control_OpenDoorSequence();
                    break;
                
                case CMD_ALARM:
                    Control_ActivateAlarm();
                    break;

                case CMD_SET_TIMEOUT:
                    for(int i=0; i<5; i++){
                     Data[i] = UART5_ReceiveChar();
                    }
                    Data[5] = '\0'; 

                    Control_UpdateTimeout((uint32_t)strtoul(Data, NULL, 10));
                    break;
                
                case CMD_Check_First_Time:
                    if(g_isfirstime == 1){
                        UART5_SendChar('1');
                    } else {
                        UART5_SendChar('0');
                    }
                    break;
                default:
                    UART5_SendChar('9');
                    UART5_Flush();
                break;
            }
        }
        UART5_ClearInterruptStatus(status);
}

void Timer0A_Handler_CloseDoor(void)
{
    GPTM_DoorTimer_ClearInt(); // Clear Flag
    
    DoorLock_Lock();
    
    SysTick_Wait(500);

    
    /* Stop Timer */
    GPTM_DoorTimer_Stop();
}

void Timer1A_Handler_StopAlarm(void)
{
    GPTM_AlarmTimer_ClearInt(); // Clear Flag
    
    Buzzer_Stop();
    
    /* Stop Timer */
    GPTM_AlarmTimer_Stop();     
}

void GPIOF_Handler(void)
{
    /* 1. Clear the Interrupt Flag immediately */
    Button_ClearInterrupt(); 
    /* 3. Execute the Memory Reset Logic */
    HardReset();
    

    /* 4. Reset RAM Variables to match Memory */
    g_doorTimeout = 5; 
    g_isfirstime  = 1; 

}

/* ========================================================================== */
/* FUNCTION IMPLEMENTATIONS                                                   */
/* ========================================================================== */


void Control_SystemInit(void)
{
  __asm("CPSIE I"); // Enable Interrupts
  UART5_Init();
  DoorLock_Init();
  Buzzer_Init();
  Memory_Init();
  GPTM_Init();

  //NEW: HARD RESET BUTTON CONFIGURATION (PF4)
  Button_HardReset_Init();   
  
  Led_RedInit();
  Led_BlueInit();
  Led_GreenInit();
  
}

void Control_CheckPassword(char* password)
{
   bool Check = Memory_CheckPassword(password);
   SysTick_Wait(200);

   
   if(Check) {
       counter = 0;
       UART5_SendChar('1');
   } else {
       if(counter<3){
       Buzzer_SmallBuzz();
       counter++;
       UART5_SendChar('0');
       Led_RedTurnOn();
       SysTick_Wait(200);
       Led_RedTurnOff();
       } 
       else{
        UART5_SendChar('2');
        Control_ActivateAlarm();
        }
   }
}

void Control_SavePassword(char* password)
{    
    if(g_isfirstime == 1){
        g_isfirstime = 0;
        Memory_SaveCheckFirstTime(g_isfirstime);
    }

    Memory_SavePassword(password);
    Led_BlueTurnOn();
    SysTick_Wait(200);
    Led_BlueTurnOff();
}

void Control_OpenDoorSequence(void)
{   

    DoorLock_Unlock();

    Led_GreenTurnOn();
    SysTick_Wait(200);
    Led_GreenTurnOff();
    
    /* Start Timer0 */
    GPTM_DoorTimer_Start(g_doorTimeout);
    
}

void Control_ActivateAlarm(void)
{   
    Buzzer_Start();

    Led_RedTurnOn();
    SysTick_Wait(200);
    Led_RedTurnOff();

    /* Start Timer1 */
    GPTM_AlarmTimer_Start(ALARM_DURATION_SEC);
    
}

void Control_UpdateTimeout(uint32_t timeout)
{  
  if(timeout < 5 || timeout > 30) {
    UART5_SendChar('8');
      return;
  }

  g_doorTimeout = (uint8_t)timeout;
  Memory_SaveTimeout(timeout);
    Led_BlueTurnOn();
    SysTick_Wait(200);
    Led_BlueTurnOff();
}
