#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../UTILS/std_types.h"
#include <stdbool.h>

/* ================================================================= */
/* MEMORY MAP CONFIGURATION                                          */
/* ================================================================= */
#define PASSWORD_START_ADDR    0x0000 

/* The logic uses 5 digits, BUT we store 8 bytes to align with EEPROM words */
#define PASSWORD_REAL_LEN      5       
#define PASSWORD_STORAGE_LEN   8       

/* ================================================================= */
/* FUNCTION PROTOTYPES                                               */
/* ================================================================= */
void Memory_Init(void);
void Memory_SavePassword(char* password);
void Memory_GetPassword(char* buffer);
bool Memory_CheckPassword(char* inputPassword);

#endif