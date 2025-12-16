#include "Memory_Manager.h"
#include "../MCAL/EEPROM_Driver.h"
#include <string.h> // For strcmp/strcpy if needed
#include "std_types.h"
#include <stdint.h>   
#include <stdio.h> 
#include <stdbool.h>

void Memory_Init(void)
{
    Eeprom_Init();
}

void Memory_SavePassword(char* password)
{
    /* 1. Create a temporary aligned buffer (8 bytes) */
    /* Initialize with 0 to ensure clean padding */
    u8 tempBuffer[PASSWORD_STORAGE_LEN] = {0}; 
    
    /* 2. Copy the actual password (5 bytes) into it */
    u8 i;
    for(i = 0; i < PASSWORD_REAL_LEN; i++)
    {
        tempBuffer[i] = password[i];
    }
    
    /* 3. Write the full 8-byte block (Two 32-bit Words) */
    /* This forces the EEPROM to commit the second word correctly */
    Eeprom_WriteBlock(PASSWORD_START_ADDR, tempBuffer, PASSWORD_STORAGE_LEN);
}

void Memory_GetPassword(char* buffer)
{
    /* 1. Read the full 8-byte block */
    Eeprom_ReadBlock(PASSWORD_START_ADDR, (u8*)buffer, PASSWORD_STORAGE_LEN);
    
    /* 2. Force Null-Termination at the 5th character */
    /* This ensures "12345\0\0" is treated as just "12345" */
    buffer[PASSWORD_REAL_LEN] = '\0'; 
}

bool Memory_CheckPassword(char* inputPassword)
{
    char storedPassword[8]; // 
    
    Memory_GetPassword(storedPassword);
    
    /* Compare only the first 5 characters */
    if(strncmp(inputPassword, storedPassword, PASSWORD_REAL_LEN) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

void Memory_SaveTimeout(uint32_t timeout)
{
    /* Write the 4-byte timeout value directly */
    Eeprom_WriteBlock(TIMEOUT_START_ADDR, (u8*)&timeout, TIMEOUT_STORAGE_LEN);
}

void Memory_GetTimeout(uint32_t* timeout)
{
    /* 1. Read the full 8-byte block */
    Eeprom_ReadBlock(TIMEOUT_START_ADDR, (u8*)timeout, TIMEOUT_STORAGE_LEN);
}

void Memory_SaveCheckFirstTime(uint32_t Check_First_Time)
{
    /* Write the 4-byte Check_First_Time value directly */
    Eeprom_WriteBlock(Check_First_Time_Start_ADDR, (u8*)&Check_First_Time, Check_First_Time_LEN);
}

void Memory_GetCheckFirstTime(uint32_t* Check_First_Time)
{
    /* 1. Read the full 8-byte block */
    Eeprom_ReadBlock(Check_First_Time_Start_ADDR, (u8*)Check_First_Time, Check_First_Time_LEN);
}

void HardReset()
{
    Memory_SavePassword(DEFAULT_PASSWORD);
    Memory_SaveTimeout(DEFAULT_TIMEOUT);
    Memory_SaveCheckFirstTime(DEFAULT_CHECK_FIRST_TIME);
}