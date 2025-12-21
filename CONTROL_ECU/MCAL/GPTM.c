/******************************************************************************
 * File: GPTM.c
 * Module: General Purpose Timer Module (MCAL)
 * Description: Implementation for Timer0 One-Shot Logic
 ******************************************************************************/
#include "../UTILS/std_types.h"
#include <stdio.h>  // For printf
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "GPTM.h"

#define SYSTEM_CLOCK 16000000UL


/* TivaWare Includes */


/* Global Callback Pointer */
static void (*g_Timer0ACallback)(void) = 0;

/* ================================================================= */
/* INITIALIZATION                                                    */
/* ================================================================= */
/* ================================================================= */
/* GLOBAL INIT - Call this ONCE in Control_SystemInit                */
/* ================================================================= */
void GPTM_Init(void)
{
    /* 1. Enable Clocks for Timer0 and Timer1 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    /* 2. Wait for Ready */
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));

    /* 3. Configure both as 32-bit One-Shot Timers */
    TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);

    /* 4. Enable Timeout Interrupt source inside the Timers */
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    /* 5. Enable Interrupts in NVIC */
    IntEnable(INT_TIMER0A); // For Door
    IntEnable(INT_TIMER1A); // For Alarm
}

/* ================================================================= */
/* START TIMER                                                       */
/* ================================================================= */
void GPTM_StartOneShot(u32 seconds)
{
    /* 1. Get System Clock Frequency (e.g., 16,000,000 or 80,000,000) */
    u32 clockFreq = SysCtlClockGet();

    /* 2. Calculate Load Value: (Seconds * Clock_Hz) - 1 */
    /* Example: 5s * 16MHz = 80,000,000 ticks */
    u32 loadValue = (seconds * clockFreq);

    /* 3. Load the value into Timer0A */
    TimerLoadSet(TIMER0_BASE, TIMER_A, loadValue);

    /* 4. Start the Timer */
    TimerEnable(TIMER0_BASE, TIMER_A);
}

/* ================================================================= */
/* STOP TIMER                                                        */
/* ================================================================= */
void GPTM_Stop(void)
{
    TimerDisable(TIMER0_BASE, TIMER_A);
}

/* ================================================================= */
/* CALLBACK REGISTRATION                                             */
/* ================================================================= */
void GPTM_SetCallback(void (*ptr)(void))
{
    g_Timer0ACallback = ptr;
}

/* ================================================================= */
/* INTERRUPT SERVICE ROUTINE (ISR)                                   */
/* This function MUST be linked in startup_ewarm.c                   */
/* ================================================================= */
void Timer0A_Handler(void)
{
    /* 1. Clear the Interrupt Flag (Crucial! Otherwise, it loops forever) */
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    /* 2. Execute the registered Application function (e.g., CloseDoor) */
    if(g_Timer0ACallback != 0)
    {
        g_Timer0ACallback();
    }
}

/* ================================================================= */
/* TIMER 0 (DOOR) HELPERS                                            */
/* ================================================================= */
void GPTM_DoorTimer_Start(uint32_t seconds)
{
    uint32_t loadValue = (seconds * SYSTEM_CLOCK) - 1;
    TimerLoadSet(TIMER0_BASE, TIMER_A, loadValue);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void GPTM_DoorTimer_Stop(void)
{
    TimerDisable(TIMER0_BASE, TIMER_A);
}

void GPTM_DoorTimer_ClearInt(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

/* ================================================================= */
/* TIMER 1 (ALARM) HELPERS                                           */
/* ================================================================= */
void GPTM_AlarmTimer_Start(uint32_t seconds)
{
    uint32_t loadValue = (seconds * SYSTEM_CLOCK) - 1;
    TimerLoadSet(TIMER1_BASE, TIMER_A, loadValue);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void GPTM_AlarmTimer_Stop(void)
{
    TimerDisable(TIMER1_BASE, TIMER_A);
}

void GPTM_AlarmTimer_ClearInt(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}