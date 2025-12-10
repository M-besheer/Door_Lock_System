/*****************************************************************************
 * File: main.c
 * Description: Control ECU EEPROM Persistence Test (With Self-Repair)
 * Author: Ahmedhh
 * Date: December 2025
 *****************************************************************************/

#include "std_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>    
#include <string.h>
/* Drivers */
#include "Memory_Manager.h" 
#include "SYSTICK.h"        

/* * Helper: Check if EEPROM contains something that looks like a password.
 */
/* Helper Update */
bool Is_First_Run(void)
{
    char currentPass[10];
    Memory_GetPassword(currentPass);
    // If it starts with a number, assume we have data
    if(currentPass[0] >= '0' && currentPass[0] <= '9') return FALSE;
    return TRUE;
}
int main()
{
    /* 1. Initialization */
    SysTick_Init();
    
    // TURN OFF BUFFERING: Make printf instant
    
    printf("\n========================================\n");
    printf("[Control ECU] System Starting...\n");
    
    Memory_Init();       
    printf("[Control ECU] EEPROM Initialized.\n");

    /* 2. Persistence Logic Test */
    char myBuffer[10]; 
    
    if (Is_First_Run()) 
    {
        printf("[Status] No existing password found (First Run).\n");
        printf("[Action] Saving default password '12345'...\n");
        
        Memory_SavePassword("12345");
        
        SysTick_Wait(500); 
        printf("[Status] Save Complete. Unplug board to test persistence.\n");
    }
    else 
    {
        printf("[Status] Existing Password Found in Storage!\n");
        
        /* Read from EEPROM */
        Memory_GetPassword(myBuffer);
        
        printf("[RESULT] Password Read: %s\n", myBuffer);
        
        /* === FIX: CHECK AND REPAIR === */
        if (strcmp(myBuffer, "12345") == 0) {
             printf("[TEST] SUCCESS: Data matches exactly.\n");
        } 
        else {
             printf("[TEST] WARNING: Mismatch detected! (Expected: 12345, Got: %s)\n", myBuffer);
             printf("[FIXING] Overwriting corrupted data with '12345' now...\n");
             
             // Force overwrite the correct password
             Memory_SavePassword("12345");
             
             SysTick_Wait(500);
             printf("[FIXING] Repair Complete. Please Restart/Reset the board.\n");
        }
    }
    
    printf("========================================\n");

    /* 3. Infinite Loop */
    while (1)
    {
        // Keep CPU alive
    }
}