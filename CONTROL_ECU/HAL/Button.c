#include "Button.h"
#include "../CONFIG/Led_Cfg.h" // To use PORT_F/PIN_0 defs if you want, or use local defines
#include "../MCAL/Dio.h"

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