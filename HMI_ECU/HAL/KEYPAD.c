
/*****************************************************************************
 * File: keypad.c
 * Description: 4x4 Keypad Driver for TM4C123GH6PM
 * Author: Ahmedhh
 * Date: November 27, 2025
 ******************************************************************************/

#include "keypad.h"
#include "dio.h"

/*
 * Keypad mapping array.
 * Each element is a null-terminated string (some keys may map to multi-char strings).
 * The array is organized as [row][column], matching the physical keypad layout.
 */
const char *keypad_codes[4][4] = {
    {"1", "2", "3", "A"},
    {"4", "5", "6", "B"},
    {"7", "8", "9", "C"},
    {"*", "0", "#", "D"}
};

/*
 * Pin configuration for keypad interface.
 */
#define KEYPAD_COL_PORT PORT_C
#define KEYPAD_COL_PINS {PIN_4, PIN_5, PIN_6, PIN_7} // PC4-PC7

#define KEYPAD_ROW_PORT PORT_A
#define KEYPAD_ROW_PINS {PIN_2, PIN_3, PIN_4, PIN_5} // PA2-PA6


/*
 * Keypad_Init
 * Initializes the GPIO pins for keypad operation.
 * - Rows are set as inputs with internal pull-up resistors (PortA).
 * - Columns are set as outputs and driven HIGH (PortB).
 * This function must be called before using Keypad_GetKey.
 */
void Keypad_Init(void) {
    uint8_t row_pins[4] = KEYPAD_ROW_PINS;
    uint8_t col_pins[4] = KEYPAD_COL_PINS;
    // Configure rows (PortA) as input with pull-up
    for (uint8_t i = 0; i < 4; i++) {
        Dio_Init(KEYPAD_ROW_PORT, row_pins[i], INPUT);
        Dio_SetPUR(KEYPAD_ROW_PORT, row_pins[i], ENABLE);
    }
    // Configure columns (PortB) as output and set HIGH
    for (uint8_t i = 0; i < 4; i++) {
        Dio_Init(KEYPAD_COL_PORT, col_pins[i], OUTPUT);
        Dio_WriteChannel(KEYPAD_COL_PORT, col_pins[i], HIGH);
    }
}


/*
 * Keypad_GetKey
 * Scans the keypad and returns the character of the pressed key.
 * Returns 0 if no key is pressed.
 *
 * Scanning logic:
 *   1. Set each column LOW one at a time, others HIGH.
 *   2. Read all row inputs; if any row reads LOW, a key is pressed.
 *   3. Wait for key release (debounce).
 *   4. Return the mapped character from keypad_codes.
 */
const char *Keypad_GetKey(void) {
    uint8_t row_pins[4] = KEYPAD_ROW_PINS;
    uint8_t col_pins[4] = KEYPAD_COL_PINS;
    for (uint8_t col = 0; col < 4; col++) {
        // Set all columns HIGH (inactive)
        for (uint8_t c = 0; c < 4; c++) {
            Dio_WriteChannel(KEYPAD_COL_PORT, col_pins[c], HIGH);
        }
        // Set current column LOW (active)
        Dio_WriteChannel(KEYPAD_COL_PORT, col_pins[col], LOW);
        // Small delay for signal to settle
        for (volatile uint32_t d = 0U; d < 100U; d++) 
        {
            /* Wait for signal to settle */
        }

        // Scan rows for key press
        for (uint8_t row = 0; row < 4; row++) {
            uint8_t pin_val = Dio_ReadChannel(KEYPAD_ROW_PORT, row_pins[row]);
            if (pin_val == LOW) {
                // Key detected at (col, row)
                // Wait for key release (debounce)
                while (Dio_ReadChannel(KEYPAD_ROW_PORT, row_pins[row]) == LOW);
                // Return the mapped character from keypad_codes
                return keypad_codes[row][col];
            }
        }
    }
    return 0; // No key pressed
}
