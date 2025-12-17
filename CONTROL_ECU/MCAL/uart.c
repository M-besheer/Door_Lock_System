/**************************
 * File: uart.c
 * Module: UART (Universal Asynchronous Receiver/Transmitter)
 * Description: Source file for TM4C123GH6PM UART5 Driver (TivaWare)
 * Author: Ahmedhh
 * Date: December 10, 2025
 * * Configuration:
 * - UART5 (PE4: RX, PE5: TX)
 * - Baud Rate: 115200
 * - Data: 8 bits
 * - Parity: None
 * - Stop: 1 bit
 * - System Clock: 16 MHz
 **************************/

#include "uart.h"
#include <stdint.h>
#include <stdbool.h>

/* TivaWare includes */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

/**************************
 * Definitions                                *
 **************************/

#define SYSTEM_CLOCK    16000000    /* 16 MHz system clock */
#define BAUD_RATE       115200      /* Target baud rate */

/**************************
 * Function Implementations                          *
 **************************/

/*
 * UART5_Init
 * Initializes UART5 with 115200 baud rate, 8N1 configuration using TivaWare.
 */
void UART0_Init(void)
{
    /* 1. Enable peripheral clocks for UART5 and GPIOE */
    /* CHANGED: UART0 -> UART5, GPIOA -> GPIOE */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    
    /* Wait for peripherals to be ready */
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART5));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    
    /* 2. Configure GPIO pins for UART functionality */
    /* PE4: U5RX (UART5 Receive) */
    /* PE5: U5TX (UART5 Transmit) */
    /* CHANGED: PA0/PA1 -> PE4/PE5 mappings */
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    
    /* Set pin type to UART */
    /* CHANGED: PORTA -> PORTE, PIN_0/1 -> PIN_4/5 */
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    
    /* 3. Configure UART parameters */
    /* CHANGED: UART0_BASE -> UART5_BASE */
    UARTConfigSetExpClk(UART5_BASE, SYSTEM_CLOCK, BAUD_RATE,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | 
                         UART_CONFIG_PAR_NONE));
    
    /* 4. Enable UART5 */
    /* CHANGED: UART0_BASE -> UART5_BASE */
    UARTEnable(UART5_BASE);
}

/*
 * UART5_SendChar
 * Transmits a single character through UART5 using TivaWare.
 */
void UART0_SendChar(char data)
{
    /* CHANGED: UART0_BASE -> UART5_BASE */
    UARTCharPut(UART5_BASE, data);
}

/*
 * UART5_ReceiveChar
 * Receives a single character from UART5 using TivaWare.
 */
char UART0_ReceiveChar(void)
{
    /* CHANGED: UART0_BASE -> UART5_BASE */
    return (char)UARTCharGet(UART5_BASE);
}

/*
 * UART5_SendString
 * Transmits a null-terminated string through UART5.
 */
void UART0_SendString(const char *str)
{
    while (*str != '\0')
    {
        UART0_SendChar(*str);
        str++;
    }
}

/*
 * UART5_IsDataAvailable
 * Checks if data is available in the receive FIFO using TivaWare.
 */
uint8_t UART0_IsDataAvailable(void)
{
    /* CHANGED: UART0_BASE -> UART5_BASE */
    return (UARTCharsAvail(UART5_BASE)) ? 1 : 0;
}