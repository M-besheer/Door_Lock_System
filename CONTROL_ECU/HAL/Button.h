#ifndef BUTTON_H
#define BUTTON_H

#include "../UTILS/std_types.h"

// Define the Button Location (SW2 is on PF0)
#define BUTTON_PORT   PORT_F
#define BUTTON_PIN    PIN_0

// Function Prototypes
void Button_Init(void);
u8   Button_Read(void); 
void Buzzer_Lalaland(void);

#endif