/******************************************************************************
 * Module: Door Lock (Solenoid)
 * File Name: Door_Lock.c
 * Description: Source file for the Door Lock HAL driver.
 *******************************************************************************/

#include "Door_Lock.h"
#include "tm4c123gh6pm.h"

/* * Hardware Definitions 
 * Port: PORT B
 * Pin:  PB0 (Bit 0)
 */
#define PORTB_CLOCK_MASK    (0x02U) /* Bit 1 for Port B */
#define LOCK_PIN_MASK       (0x10U) /* Bit 0 for PB4 */

void DoorLock_Init(void)
{
    volatile uint32_t delay_loops;

    /* 1. Enable Clock for Port B */
    SYSCTL_RCGCGPIO_R |= PORTB_CLOCK_MASK;
    
    /* Dummy read to ensure clock is stable before accessing registers */
    delay_loops = SYSCTL_RCGCGPIO_R;
    /* (MISRA: Cast to void to indicate variable is intentionally unused after read) */
    (void)delay_loops; 

    /* 2. Configure PB0 direction to INPUT (Locked State) */
    GPIO_PORTB_DIR_R &= ~(LOCK_PIN_MASK);

    /* 3. Enable Digital Functionality on PB0 */
    GPIO_PORTB_DEN_R |= LOCK_PIN_MASK;
    
    /* Ensure Data Register bit is cleared (Low) so when we switch to Output, it sinks current immediately */
    GPIO_PORTB_DATA_R &= ~(LOCK_PIN_MASK);
}

void DoorLock_Unlock(void)
{
    /* * To Unlock: Turn Relay ON (Active Low).
     * 1. Ensure Data is Low (0V).
     * 2. Set Direction to OUTPUT.
     */
    GPIO_PORTB_DATA_R &= ~(LOCK_PIN_MASK);
    GPIO_PORTB_DIR_R  |= LOCK_PIN_MASK;
}

void DoorLock_Lock(void)
{
    /* * To Lock: Turn Relay OFF.
     * Tiva cannot output 5V to match relay VCC.
     * Solution: Set Direction to INPUT (Hi-Z) to disconnect Tiva.
     */
    GPIO_PORTB_DIR_R &= ~(LOCK_PIN_MASK);
}