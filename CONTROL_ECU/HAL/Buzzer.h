#ifndef BUZZER_H
#define BUZZER_H

#include "../UTILS/std_types.h"

/* Initialize the buzzer hardware */
void Buzzer_Init(void);

/* Short single beep (used for single wrong password) */
void Buzzer_SmallBuzz(void);

/* Start continuous buzzing (used when password wrong 3 times) */
void Buzzer_Start(void);

/* Stop buzzing */
void Buzzer_Stop(void);

void Buzzer_Lalaland(void);

#endif
