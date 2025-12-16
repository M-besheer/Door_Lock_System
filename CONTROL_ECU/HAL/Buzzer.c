#include "Buzzer.h"
#include "../CONFIG/Buzzer_Cfg.h"
#include "../MCAL/Dio.h"
#include "../MCAL/SYSTICK.h"

/* Initialize the buzzer pin as output */
void Buzzer_Init(void)
{
    Dio_Init(BUZZER_PORT, BUZZER_PIN, OUTPUT);
}

/* Start buzzer (steady ON) */
void Buzzer_Start(void)
{
    Dio_WriteChannel(BUZZER_PORT, BUZZER_PIN, HIGH);
}

/* Stop buzzer (OFF) */
void Buzzer_Stop(void)
{
    Dio_WriteChannel(BUZZER_PORT, BUZZER_PIN, LOW);
}

/* Short beep: ON for 200 ms then OFF */
void Buzzer_SmallBuzz(void)
{
    Buzzer_Start();
    SysTick_Wait(200);
    Buzzer_Stop();
}
