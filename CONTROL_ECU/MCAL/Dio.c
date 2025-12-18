#include "Dio.h"
#include <stdint.h>   // Defines uint32_t, uint8_t
#include <stdbool.h>  // Defines bool
// TIVAWARE INCLUDES (Hidden from everyone else)
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "std_types.h"
/* Add this to your Includes if not already there */
#include "driverlib/sysctl.h" 
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

void Dio_Init(u8 PortId, u8 PinId, u8 Direction)
{
  /* ============================================================ */
    /* SAFETY CHECK: FORBIDDEN PINS (JTAG & DEBUG UART)             */
    /* ============================================================ */
    // TEMPORARY TEST: Treat PORT_F, PIN_1 (Red LED) as "Dangerous"
    /*if(PortId == PORT_F && PinId == PIN_1)
    {
        printf("\n[TEST] SUCCESS! Caught forbidden access to PF1.\n");
        printf("[TEST] System Halted. The LED should NOT turn on.\n");
        
        while(1); // Trap the CPU here
    }*/
    // 1. Check for JTAG Pins (PC0 - PC3) -> CRITICAL: Bricks Debugger
    if(PortId == PORT_C && (PinId <= PIN_3))
    {
        // HALT SYSTEM (Infinite Loop) - Code stops here.
        while(1);
    }

    // 2. Check for UART0 Pins (PA0 - PA1) -> WARNING: Kills Console
    // (Optional: You might want to allow this later, but for now, block it)
    if(PortId == PORT_A && (PinId <= PIN_1))
    {
        while(1);
    }
    
    u32 Tiva_Port_Base = 0;
    u32 Tiva_Peripheral_ID = 0;
    u8  Tiva_Pin_Mask = (1 << PinId);

    /* 1. Map Generic PORT to TivaWare ID & Base */
    switch(PortId)
    {
        case PORT_A: Tiva_Peripheral_ID = SYSCTL_PERIPH_GPIOA; Tiva_Port_Base = GPIO_PORTA_BASE; break;
        case PORT_B: Tiva_Peripheral_ID = SYSCTL_PERIPH_GPIOB; Tiva_Port_Base = GPIO_PORTB_BASE; break;
        case PORT_C: Tiva_Peripheral_ID = SYSCTL_PERIPH_GPIOC; Tiva_Port_Base = GPIO_PORTC_BASE; break;
        case PORT_D: Tiva_Peripheral_ID = SYSCTL_PERIPH_GPIOD; Tiva_Port_Base = GPIO_PORTD_BASE; break;
        case PORT_E: Tiva_Peripheral_ID = SYSCTL_PERIPH_GPIOE; Tiva_Port_Base = GPIO_PORTE_BASE; break;
        case PORT_F: Tiva_Peripheral_ID = SYSCTL_PERIPH_GPIOF; Tiva_Port_Base = GPIO_PORTF_BASE; break;
        default: return;
    }

    /* 2. ENABLE CLOCK & WAIT */
    SysCtlPeripheralEnable(Tiva_Peripheral_ID);
    while(!SysCtlPeripheralReady(Tiva_Peripheral_ID));

    /* 3. UNLOCK SPECIAL PINS (PF0 & PD7) */
    if( (PortId == PORT_F && PinId == PIN_0) || (PortId == PORT_D && PinId == PIN_7) )
    {
        HWREG(Tiva_Port_Base + GPIO_O_LOCK) = GPIO_LOCK_KEY; // Unlock
        HWREG(Tiva_Port_Base + GPIO_O_CR)  |= Tiva_Pin_Mask; // Commit
        HWREG(Tiva_Port_Base + GPIO_O_LOCK) = 0;             // Re-Lock
    }

    /* 4. SET DIRECTION & PULL-UPS */
    if(Direction == OUTPUT)
    {
        GPIOPinTypeGPIOOutput(Tiva_Port_Base, Tiva_Pin_Mask);
    }
    else
    {
        GPIOPinTypeGPIOInput(Tiva_Port_Base, Tiva_Pin_Mask);
    }
}



void Dio_WriteChannel(u8 PortId, u8 PinId, u8 Level)
{
    u32 Tiva_Port_Base = 0; // Initialize to 0 for safety
    u8  Tiva_Pin_Mask = (1 << PinId);

    /* 1. Translate Generic PORT to Address */
    switch(PortId) {
        case PORT_A: Tiva_Port_Base = GPIO_PORTA_BASE; break; // Added
        case PORT_B: Tiva_Port_Base = GPIO_PORTB_BASE; break; // Added
        case PORT_C: Tiva_Port_Base = GPIO_PORTC_BASE; break; // Added (Safe for PC4-PC7)
        case PORT_D: Tiva_Port_Base = GPIO_PORTD_BASE; break; // Added
        case PORT_E: Tiva_Port_Base = GPIO_PORTE_BASE; break; // Added
        case PORT_F: Tiva_Port_Base = GPIO_PORTF_BASE; break;
        default: return; // SAFETY EXIT: If port is wrong, do nothing!
    }

    /* 2. CALL TIVAWARE */
    if(Level == HIGH) {
        GPIOPinWrite(Tiva_Port_Base, Tiva_Pin_Mask, Tiva_Pin_Mask);
    } else {
        GPIOPinWrite(Tiva_Port_Base, Tiva_Pin_Mask, 0);
    }
}

u8 Dio_ReadChannel(u8 PortId, u8 PinId)
{
    u32 Tiva_Port_Base = 0;
    u8  Tiva_Pin_Mask = (1 << PinId);
    int32_t Raw_Value;

    switch(PortId) {
        case PORT_A: Tiva_Port_Base = GPIO_PORTA_BASE; break;
        case PORT_B: Tiva_Port_Base = GPIO_PORTB_BASE; break;
        case PORT_C: Tiva_Port_Base = GPIO_PORTC_BASE; break;
        case PORT_D: Tiva_Port_Base = GPIO_PORTD_BASE; break;
        case PORT_E: Tiva_Port_Base = GPIO_PORTE_BASE; break;
        case PORT_F: Tiva_Port_Base = GPIO_PORTF_BASE; break;
        default: return 0;
    }

    // TivaWare Read returns 0 or the BitMask (e.g., 0x10 for Pin 4)
    Raw_Value = GPIOPinRead(Tiva_Port_Base, Tiva_Pin_Mask);

    // Normalize to 1 or 0
    if(Raw_Value == 0) {
        return LOW;
    } else {
        return HIGH;
    }
}

/* ================================================================= */
/* ENABLE/DISABLE PULL-UP RESISTOR                                   */
/* ================================================================= */
void Dio_SetPUR(uint8_t port, uint8_t pin, uint8_t enable)
{
    u32 Tiva_Port_Base = 0;
    u8  Tiva_Pin_Mask = (1 << pin);

    /* 1. Map Generic PORT to TivaWare Base Address */
    switch(port)
    {
        case PORT_A: Tiva_Port_Base = GPIO_PORTA_BASE; break;
        case PORT_B: Tiva_Port_Base = GPIO_PORTB_BASE; break;
        case PORT_C: Tiva_Port_Base = GPIO_PORTC_BASE; break;
        case PORT_D: Tiva_Port_Base = GPIO_PORTD_BASE; break;
        case PORT_E: Tiva_Port_Base = GPIO_PORTE_BASE; break;
        case PORT_F: Tiva_Port_Base = GPIO_PORTF_BASE; break;
        default: return;
    }

    /* 2. UNLOCK SPECIAL PINS (PF0 & PD7) */
    /* Necessary because we might have re-locked them in Dio_Init */
    if( (port == PORT_F && pin == PIN_0) || (port == PORT_D && pin == PIN_7) )
    {
        HWREG(Tiva_Port_Base + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
        HWREG(Tiva_Port_Base + GPIO_O_CR)  |= Tiva_Pin_Mask; 
    }

    /* 3. Configure Pad */
    if(enable)
    {
        /* Enable Weak Pull-Up (Standard 2mA strength) */
        GPIOPadConfigSet(Tiva_Port_Base, Tiva_Pin_Mask, 
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    }
    else
    {
        /* Disable Resistor (Set to Standard Floating) */
        GPIOPadConfigSet(Tiva_Port_Base, Tiva_Pin_Mask, 
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    }
}

/* ================================================================= */
/* ENABLE/DISABLE PULL-DOWN RESISTOR                                 */
/* ================================================================= */
void Dio_SetPDR(uint8_t port, uint8_t pin, uint8_t enable)
{
    u32 Tiva_Port_Base = 0;
    u8  Tiva_Pin_Mask = (1 << pin);

    /* 1. Map Generic PORT to TivaWare Base Address */
    switch(port)
    {
        case PORT_A: Tiva_Port_Base = GPIO_PORTA_BASE; break;
        case PORT_B: Tiva_Port_Base = GPIO_PORTB_BASE; break;
        case PORT_C: Tiva_Port_Base = GPIO_PORTC_BASE; break;
        case PORT_D: Tiva_Port_Base = GPIO_PORTD_BASE; break;
        case PORT_E: Tiva_Port_Base = GPIO_PORTE_BASE; break;
        case PORT_F: Tiva_Port_Base = GPIO_PORTF_BASE; break;
        default: return;
    }

    /* 2. UNLOCK SPECIAL PINS (PF0 & PD7) */
    if( (port == PORT_F && pin == PIN_0) || (port == PORT_D && pin == PIN_7) )
    {
        HWREG(Tiva_Port_Base + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
        HWREG(Tiva_Port_Base + GPIO_O_CR)  |= Tiva_Pin_Mask; 
    }

    /* 3. Configure Pad */
    if(enable)
    {
        /* Enable Weak Pull-Down (Standard 2mA strength) */
        GPIOPadConfigSet(Tiva_Port_Base, Tiva_Pin_Mask, 
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    }
    else
    {
        /* Disable Resistor (Set to Standard Floating) */
        GPIOPadConfigSet(Tiva_Port_Base, Tiva_Pin_Mask, 
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    }
}