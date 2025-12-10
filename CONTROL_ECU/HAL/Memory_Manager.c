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
    char storedPassword[10]; // Big enough for 8 bytes + null
    
    Memory_GetPassword(storedPassword);
    
    /* Compare only the first 5 characters */
    if(strncmp(inputPassword, storedPassword, PASSWORD_REAL_LEN) == 0)
    {
        return TRUE;
    }
    return FALSE;
}