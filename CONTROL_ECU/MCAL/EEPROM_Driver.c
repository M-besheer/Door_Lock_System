#include "EEPROM_Driver.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"

bool Eeprom_Init(void)
{
    /* 1. Enable the EEPROM Peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);

    /* 2. Wait for the EEPROM module to be ready */
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0))
    {
        // Wait loop
    }

    /* 3. Perform TivaWare EEPROM Init (Handles recovery and block setting) */
    uint32_t result = EEPROMInit();

    /* EEPROMInit returns EEPROM_INIT_OK (0) on success */
    if(result == EEPROM_INIT_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE; // Initialization failed
    }
}

void Eeprom_WriteBlock(u32 address, u8* buffer, u32 length)
{
    /* TivaWare's EEPROMProgram writes data into the block.
     * Arguments: (uint32_t *pui32Data, uint32_t ui32Address, uint32_t ui32Count)
     * Note: TivaWare handles unaligned bytes automatically in this function.
     */
    EEPROMProgram((uint32_t *)buffer, address, length);
}

void Eeprom_ReadBlock(u32 address, u8* buffer, u32 length)
{
    /* TivaWare's EEPROMRead reads data.
     * Arguments: (uint32_t *pui32Data, uint32_t ui32Address, uint32_t ui32Count)
     */
    EEPROMRead((uint32_t *)buffer, address, length);
}

void Eeprom_WriteByte(u32 address, u8 data)
{
    /* Wrap single byte in array to use the Block function */
    u8 tempBuffer[4] = {0}; // Padding for safety, though 1 byte works
    tempBuffer[0] = data;
    EEPROMProgram((uint32_t *)tempBuffer, address, 1);
}

u8 Eeprom_ReadByte(u32 address)
{
    u8 data = 0;
    u32 tempRead = 0;
    
    /* Read 1 byte. TivaWare handles the masking. */
    EEPROMRead(&tempRead, address, 1);
    data = (u8)tempRead;
    
    return data;
}