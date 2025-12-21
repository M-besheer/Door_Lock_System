#include "Button.h"
#include "../CONFIG/Led_Cfg.h" // To use PORT_F/PIN_0 defs if you want, or use local defines
#include "../MCAL/Dio.h"
#include "tm4c123gh6pm.h" 
#include <stdint.h>

void Button_Init(void)
{
    // Initialize PF0 as Input. 
    // Your Dio_Init MUST handle the "Unlock" and "Pull-Up" for this to work!
    Dio_Init(BUTTON_PORT, BUTTON_PIN, INPUT);
}

u8 Button_Read(void)
{
    u8 returnValue = FALSE;
    u8 Val = Dio_ReadChannel(BUTTON_PORT, BUTTON_PIN);
    if (Val == LOW) 
    {
        returnValue = TRUE;  // Pressed
    }
    else 
    {
        returnValue = FALSE; // Released
    }
    return returnValue;
}
void Button_HardReset_Init(void)
{
    /* 1. Enable Clock for Port F */
    SYSCTL_RCGCGPIO_R |= 0x20;            
    while((SYSCTL_PRGPIO_R & 0x20) == 0){};

    /* 2. Unlock Port F (PF0/PF4 protection) */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;        
    GPIO_PORTF_CR_R |= 0x10;
    
    /* 3. Configure PF4 as Input with Pull-Up */
    GPIO_PORTF_DIR_R &= ~0x10;             // PF4 Input
    GPIO_PORTF_PUR_R |= 0x10;              // Enable Pull-Up
    GPIO_PORTF_DEN_R |= 0x10;              // Digital Enable

    /* 4. Interrupt Configuration for PF4 */
    GPIO_PORTF_IS_R &= ~0x10;              // Edge Sensitive
    GPIO_PORTF_IBE_R &= ~0x10;             // Not both edges
    GPIO_PORTF_IEV_R &= ~0x10;             // Falling Edge (Press)
    GPIO_PORTF_ICR_R = 0x10;               // Clear any prior flags
    GPIO_PORTF_IM_R |= 0x10;               //(Enable) Interrupt for PF4

    /* 5. NVIC Configuration (IRQ 30 for Port F) */
    NVIC_EN0_R |= (1 << 30);               
}

void Button_ClearInterrupt(void)
{
    /* Clear the interrupt flag for PF4 (Bit 4) */
    GPIO_PORTF_ICR_R = 0x10; 
}