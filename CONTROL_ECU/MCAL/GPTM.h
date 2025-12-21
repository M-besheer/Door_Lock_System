#ifndef GPTM_H
#define GPTM_H

#include <stdint.h>

/* ================================================================= */
/* FUNCTION PROTOTYPES                                               */
/* ================================================================= */

/*
 * Function: GPTM_Init
 * Description: Initializes Timer0 (Door) and Timer1 (Alarm) as 
 * 32-bit One-Shot timers. Enables interrupts in NVIC.
 */
void GPTM_Init(void);

/* ------------------------- TIMER 0 (DOOR) ------------------------ */

/*
 * Function: GPTM_DoorTimer_Start
 * Description: Loads Timer0 with 'seconds' and starts counting.
 * Parameters:  seconds - Duration in seconds.
 */
void GPTM_DoorTimer_Start(uint32_t seconds);

/*
 * Function: GPTM_DoorTimer_Stop
 * Description: Stops Timer0 immediately.
 */
void GPTM_DoorTimer_Stop(void);

/*
 * Function: GPTM_DoorTimer_ClearInt
 * Description: Clears the Timer0 Timeout Interrupt flag.
 * Must be called inside the ISR.
 */
void GPTM_DoorTimer_ClearInt(void);

/* ------------------------- TIMER 1 (ALARM) ----------------------- */

/*
 * Function: GPTM_AlarmTimer_Start
 * Description: Loads Timer1 with 'seconds' and starts counting.
 * Parameters:  seconds - Duration in seconds.
 */
void GPTM_AlarmTimer_Start(uint32_t seconds);

/*
 * Function: GPTM_AlarmTimer_Stop
 * Description: Stops Timer1 immediately.
 */
void GPTM_AlarmTimer_Stop(void);

/*
 * Function: GPTM_AlarmTimer_ClearInt
 * Description: Clears the Timer1 Timeout Interrupt flag.
 * Must be called inside the ISR.
 */
void GPTM_AlarmTimer_ClearInt(void);

#endif /* GPTM_H */