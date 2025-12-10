#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H

#include "../UTILS/std_types.h"
#include <stdbool.h>

/* ================================================================= */
/* FUNCTION PROTOTYPES                                               */
/* ================================================================= */

/* * Function: Eeprom_Init
 * Description: Enables EEPROM clock, waits for ready, and handles recovery.
 * Returns: TRUE if successful, FALSE if hardware failed.
 */
bool Eeprom_Init(void);

/* * Function: Eeprom_WriteByte
 * Description: Writes a single byte to a specific address.
 * Note: Since TivaC uses 32-bit words, this involves a Read-Modify-Write operation.
 */
void Eeprom_WriteByte(u32 address, u8 data);

/* * Function: Eeprom_ReadByte
 * Description: Reads a single byte from a specific address.
 */
u8 Eeprom_ReadByte(u32 address);

/* * Function: Eeprom_WriteBlock
 * Description: Writes an array of bytes (efficient for strings/passwords).
 */
void Eeprom_WriteBlock(u32 address, u8* buffer, u32 length);

/* * Function: Eeprom_ReadBlock
 * Description: Reads an array of bytes.
 */
void Eeprom_ReadBlock(u32 address, u8* buffer, u32 length);

#endif /* EEPROM_DRIVER_H */