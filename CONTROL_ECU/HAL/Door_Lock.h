/******************************************************************************
 * Module: Door Lock (Solenoid)
 * File Name: Door_Lock.h
 * Description: Header file for the Door Lock HAL driver.
 * Controls a solenoid via a relay using Open Drain/Input logic.
 *******************************************************************************/

#ifndef DOOR_LOCK_H_
#define DOOR_LOCK_H_

#include <stdint.h>

/* Function Prototypes */

/**
 * @brief Initialize the Door Lock GPIO pin.
 * State: Sets pin to Input (High Impedance) to ensure the lock starts CLOSED.
 */
void DoorLock_Init(void);

/**
 * @brief Opens the door (Energizes Solenoid).
 * Logic: Sets Pin to OUTPUT and drives LOW (0V).
 * Current flows through relay coil -> Door Unlocks.
 */
void DoorLock_Unlock(void);

/**
 * @brief Closes the door (De-energizes Solenoid).
 * Logic: Sets Pin to INPUT (Hi-Z).
 * Relay internal 5V pulls line high -> Relay OFF -> Door Locks.
 */
void DoorLock_Lock(void);

#endif /* DOOR_LOCK_H_ */