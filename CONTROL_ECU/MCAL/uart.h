/**************************
 * File: uart.h
 * Module: UART (Universal Asynchronous Receiver/Transmitter)
 * Description: Source file for TM4C123GH6PM UART5 Driver (TivaWare)
 * Date: December 21, 2025
 * * Configuration:
 * - UART5 (PE4: RX, PE5: TX)
 **************************/

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 *                          Function Prototypes                                *
 ******************************************************************************/

/*
 * UART0_Init
 * Initializes UART0 with 115200 baud rate, 8N1 configuration.
 * Uses PA0 (RX) and PA1 (TX).
 * System clock is assumed to be 16 MHz.
 */
void UART5_Init(void);

/*
 * UART0_SendChar
 * Transmits a single character through UART0.
 * Blocks until the transmit FIFO is ready.
 * 
 * Parameters:
 *   data - Character to transmit
 */
void UART5_SendChar(char data);

/*
 * UART0_ReceiveChar
 * Receives a single character from UART0.
 * Blocks until a character is available in the receive FIFO.
 * 
 * Returns:
 *   Received character
 */
char UART5_ReceiveChar(void);

/*
 * UART0_SendString
 * Transmits a null-terminated string through UART0.
 * 
 * Parameters:
 *   str - Pointer to null-terminated string to transmit
 */
void UART5_SendString(const char *str);

/*
 * UART0_IsDataAvailable
 * Checks if data is available in the receive FIFO.
 * 
 * Returns:
 *   1 if data is available, 0 otherwise
 */
uint8_t UART5_IsDataAvailable(void);

void UART5_Flush(void);

uint32_t UART5_GetInterruptStatus(void);

bool     UART5_CheckRxInterrupt(uint32_t status);

void     UART5_ClearInterruptStatus(uint32_t status);
void UART5_ClearInterruptFlag(void);
#endif /* UART_H_ */
