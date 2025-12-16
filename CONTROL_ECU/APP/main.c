/*****************************************************************************
 * File: main_test.c
 * Description: Control ECU Logic Test (Terminal I/O Version)
 * Note: Requires Semihosting/Debug Terminal enabled in IDE.
 *****************************************************************************/

#include "std_types.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>      // Added for printf/scanf
#include <string.h>

/* Drivers */
#include "../HAL/Memory_Manager.h" 
#include "../MCAL/SYSTICK.h"
#include "../HAL/Door_Lock.h"
#include "../HAL/Buzzer.h"

/* Definitions */
#define CMD_CHECK_PASS     1  // Simplified for keyboard input
#define CMD_SAVE_PASS      2
#define CMD_OPEN_DOOR      3
#define CMD_ALARM          4
#define CMD_SET_TIMEOUT    5
#define CMD_Check_First_Time 6

#define PASSWORD_LEN       5
#define EEPROM_PASS_ADDR   0x00
#define EEPROM_TIME_ADDR   0x10

#define ALARM_DURATION_SEC  2
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
void Control_CheckPassword(void);
void Control_SavePassword(void);
void Control_OpenDoorSequence(void);
void Control_ActivateAlarm(void);
void Control_UpdateTimeout(void);

/* ========================================================================== */
/* MAIN FUNCTION                                                              */
/* ========================================================================== */

int main(void) {
    int input_command;
    
    /* 1. Initialization */
    Control_SystemInit();
    Memory_GetCheckFirstTime(&g_isfirstime);
    printf("========================================\n");
    printf("   Door Locker System - Logic Test      \n");
    printf("========================================\n");

    /* Load saved timeout from EEPROM on boot */
    
    uint32_t temp_timeout; 
    Memory_GetTimeout(&temp_timeout); 
    g_doorTimeout = (uint8_t)temp_timeout;

    /* 2. Main Loop */
    while(1) {
        /* Terminal Menu */
        printf("\n--- Main Menu ---\n");
        printf("1. Check Password\n");
        printf("2. Save New Password\n");
        printf("3. Open Door\n");
        printf("4. Test Alarm\n");
        printf("5. Set Auto-Lock Timeout\n");
        printf("Enter Command Number: ");
        
        scanf("%d", &input_command); // Wait for user input
        printf("\n");

        switch(input_command) {
            case CMD_CHECK_PASS:
                Control_CheckPassword();
                break;
                
            case CMD_SAVE_PASS:
                Control_SavePassword();
                break;
                
            case CMD_OPEN_DOOR:
                Control_OpenDoorSequence();
                break;
                
            case CMD_ALARM:
                Control_ActivateAlarm();
                break;

            case CMD_SET_TIMEOUT:
                Control_UpdateTimeout();
                break;
                
            case CMD_Check_First_Time:
                if(g_isfirstime == 1){
                    printf("First Time Running the System. Setting Up Defaults...\n");
                } else {
                    printf("System has been initialized before.\n");
                }
            default:
                printf("Invalid Command. Try 1-5.\n");
                break;
        }
    }
}

/* ========================================================================== */
/* FUNCTION IMPLEMENTATIONS                                                   */
/* ========================================================================== */

void Control_SystemInit(void)
{
  __asm("CPSIE I"); // Enable Interrupts
  // UART Removed
  DoorLock_Init();
  Buzzer_Init();
  Memory_Init();
  /* ======================================================= */
  /* NEW: HARD RESET BUTTON CONFIGURATION (PF4)              */
  /* ======================================================= */
  SYSCTL_RCGCGPIO_R |= 0x20;             // 1. Enable Clock for Port F
  while((SYSCTL_PRGPIO_R & 0x20) == 0);  // 2. Wait for Ready

  GPIO_PORTF_LOCK_R = 0x4C4F434B;        // 3. Unlock Port F (Required for PF0, good practice)
  GPIO_PORTF_CR_R |= 0x10;               // 4. Commit PF4
  
  GPIO_PORTF_DIR_R &= ~0x10;             // 5. PF4 is Input
  GPIO_PORTF_PUR_R |= 0x10;              // 6. Enable Pull-Up Resistor
  GPIO_PORTF_DEN_R |= 0x10;              // 7. Digital Enable

  /* Interrupt Configuration */
  GPIO_PORTF_IS_R &= ~0x10;              // Edge Sensitive
  GPIO_PORTF_IBE_R &= ~0x10;             // Not both edges
  GPIO_PORTF_IEV_R &= ~0x10;             // Falling Edge (Press)
  GPIO_PORTF_ICR_R = 0x10;               // Clear any prior flags
  GPIO_PORTF_IM_R |= 0x10;               // Unmask (Enable) Interrupt for PF4

  /* NVIC Configuration (IRQ 30 for Port F) */
  NVIC_EN0_R |= (1 << 30);               
  
  printf("[Init] Hard Reset Button (PF4) Enabled.\n");
}

void Control_CheckPassword(void)
{
   uint8_t Entered_Password[PASSWORD_LEN + 1]; // +1 for null terminator in scanf
   
   printf("Enter 5-digit Password to Check: ");
   scanf("%5s", Entered_Password); // Read string from console

   /* Convert string to raw bytes if needed, but array matches structure */
   bool Check = Memory_CheckPassword(Entered_Password);
   
   if(Check == true) {
       printf("[Result] Password MATCH! (Access Granted)\n");
       counter = 0;
       Control_OpenDoorSequence();
   } else {
       printf("[Result] Password MISMATCH! (Access Denied)\n");
       if(counter<3){
       Buzzer_SmallBuzz();
       counter++;
       printf("*Buzzer Beeped*\n");}
       else{Control_ActivateAlarm();}
   }
}

void Control_SavePassword(void)
{
    uint8_t New_Password[PASSWORD_LEN + 1];
    
    if(g_isfirstime == 1){
        g_isfirstime = 0;
        Memory_SaveCheckFirstTime(g_isfirstime);
    }

    printf("Enter NEW 5-digit Password to Save: ");
    scanf("%5s", New_Password);
    
    Memory_SavePassword(New_Password);
    printf("[Result] Password Saved to EEPROM.\n");
}

void Control_OpenDoorSequence(void)
{
    printf("[Action] Unlocking Door...\n");
    DoorLock_Unlock();
    printf("[Hardware] Solenoid ACTIVE (Unlocked)\n");
    
    /* Timer Setup */
    uint32_t delay_cycles = g_doorTimeout * 16000000;
     
    SYSCTL_RCGCTIMER_R |= 0x01;       // Enable Timer0
    volatile uint32_t d = SYSCTL_RCGCTIMER_R; // Delay
    
    TIMER0_CTL_R = 0x00;              // Disable
    TIMER0_CFG_R = 0x00;              // 32-bit
    TIMER0_TAMR_R = 0x01;             // One-Shot
    TIMER0_TAILR_R = delay_cycles;    // Load delay
    TIMER0_IMR_R = 0x01;              // Enable Interrupt
    NVIC_EN0_R = 1 << 19;             // NVIC Enable
    TIMER0_CTL_R |= 0x01;             // Enable Timer
    
    printf("[Timer] Timer started for %d seconds. Waiting for ISR...\n", g_doorTimeout);
}

void Control_ActivateAlarm(void)
{
    printf("[Action] SECURITY ALERT! Activating Alarm...\n");
    Buzzer_Start();
    printf("[Hardware] Buzzer ON\n");

    /* Timer 1 Setup */
    SYSCTL_RCGCTIMER_R |= 0x02;      
    volatile uint32_t delay = SYSCTL_RCGCTIMER_R;
    (void)delay;

    TIMER1_CTL_R = 0x00000000;       
    TIMER1_CFG_R = 0x00000000;       
    TIMER1_TAMR_R = 0x00000001;      
    TIMER1_TAILR_R = ALARM_CYCLES;   
    TIMER1_ICR_R = 0x00000001;       
    TIMER1_IMR_R = 0x00000001;       
    NVIC_EN0_R = (1 << 21);
    TIMER1_CTL_R |= 0x00000001;
    
    printf("[Timer] Alarm Timer started for 3 seconds...\n");
}

void Control_UpdateTimeout(void)
{
  uint32_t new_timeout;
  printf("Enter New Timeout (5-30): ");
  scanf("%u", &new_timeout);
  
  if(new_timeout < 5 || new_timeout > 30) {
      printf("[Error] Invalid value. Must be 5-30.\n");
      return;
  }
  
  g_doorTimeout = (uint8_t)new_timeout;
  Memory_SaveTimeout(new_timeout);
  printf("[Result] Timeout updated to %d seconds.\n", g_doorTimeout);
}

/* ========================================================================== */
/* INTERRUPT SERVICE ROUTINES (ISRs)                                          */
/* ========================================================================== */

void Timer0A_Handler_CloseDoor(void)
{
    TIMER0_ICR_R = 0x01; // Clear Flag
    
    DoorLock_Lock();
    
    /* Stop Timer */
  //  TIMER0_CTL_R &= ~0x01;
    
    /* Visual Feedback for Terminal */
    printf("\n[ISR] -> Timer0 Expired: Door Automatically LOCKED.\n");
    printf("Enter Command Number: "); // Reprompt for menu clarity
}

void Timer1A_Handler_StopAlarm(void)
{
    TIMER1_ICR_R = 0x00000001; // Clear Flag
    
    Buzzer_Stop();
    
    /* Stop Timer */
    TIMER1_CTL_R &= ~0x00000001;     
    
    /* Visual Feedback for Terminal */
    printf("\n[ISR] -> Timer1 Expired: Alarm SILENCED.\n");
    printf("Enter Command Number: "); // Reprompt for menu clarity
}

/* ========================================================================== */
/* PORT F INTERRUPT HANDLER                                                   */
/* ========================================================================== */
void GPIOF_Handler(void)
{
    /* 1. Clear the Interrupt Flag immediately */
    GPIO_PORTF_ICR_R = 0x10; 

    /* 2. Visual Feedback */
    printf("\n\n[Interrupt] !!! HARD RESET BUTTON PRESSED !!!\n");
    
    /* 3. Execute the Memory Reset Logic */
    HardReset();

    /* 4. Reset RAM Variables to match Memory */
    g_doorTimeout = 5; 
    g_isfirstime  = 1; 

    printf("[System] Factory Defaults Restored. \n");
    printf("Please Enter Command Number: "); 
}