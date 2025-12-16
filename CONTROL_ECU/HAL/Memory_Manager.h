#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../UTILS/std_types.h"
#include <stdbool.h>
#include <stdint.h>

/* ================================================================= */
/* MEMORY MAP CONFIGURATION                                          */
/* ================================================================= */
#define PASSWORD_START_ADDR    0x0000
#define TIMEOUT_START_ADDR     0x0010
#define Check_First_Time_Start_ADDR 0x0020

/* The logic uses 5 digits, BUT we store 8 bytes to align with EEPROM words */
#define PASSWORD_REAL_LEN      5       
#define PASSWORD_STORAGE_LEN   8
#define TIMEOUT_STORAGE_LEN    4      
#define Check_First_Time_LEN   4

/* Default values*/
#define DEFAULT_PASSWORD       "00000"
#define DEFAULT_TIMEOUT        5  // in seconds
#define DEFAULT_CHECK_FIRST_TIME 0x1

/* ================================================================= */
/* FUNCTION PROTOTYPES                                               */
/* ================================================================= */
void Memory_Init(void);
void Memory_SavePassword(char* password);
void Memory_GetPassword(char* buffer);
bool Memory_CheckPassword(char* inputPassword);
void Memory_SaveTimeout(uint32_t timeout);
void Memory_GetTimeout(uint32_t* timeout);
void HardReset(void);

#endif