#include "Buzzer.h"
#include "../CONFIG/Buzzer_Cfg.h"
#include "../MCAL/Dio.h"
#include "../MCAL/SYSTICK.h"
#include "std_types.h"

/* --- NOTE FREQUENCIES (Hz) --- */
#define NOTE_D3  147
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294

/* * Helper function to create a Microsecond delay.
 * NOTE: This is an approximate loop for Tiva C @ 16MHz or 80MHz. 
 * For perfect tuning, use a Timer peripheral, but this works for simple songs.
 */
static void Delay_us(u32 microseconds)
{
    /* Simple loop for delay. Adjust '3' based on your clock speed */
    /* If 80MHz clock, try multiplying by roughly 26 */
    /* If 16MHz clock, try multiplying by roughly 5 */
    volatile u32 cycles = (microseconds * 16) / 3; 
    while(cycles > 0)
    {
        cycles--;
    }
}

/* * Internal Helper: Generates a square wave at a specific frequency 
 * frequency: Note pitch in Hz
 * duration_ms: How long to play the note in milliseconds
 */
static void Buzzer_PlayTone(u32 frequency, u32 duration_ms)
{
    u32 period_us = 1000000 / frequency; // Calculate period in microseconds
    u32 half_period = period_us / 2;     // High time and Low time
    u32 cycles = (duration_ms * 1000) / period_us; // How many times to toggle
    u32 i;

    for (i = 0; i < cycles; i++)
    {
        /* Turn Passive Buzzer ON */
        Dio_WriteChannel(PASSIVE_BUZZER_PORT, PASSIVE_BUZZER_PIN, HIGH);
        Delay_us(half_period);

        /* Turn Passive Buzzer OFF */
        Dio_WriteChannel(PASSIVE_BUZZER_PORT, PASSIVE_BUZZER_PIN, LOW);
        Delay_us(half_period);
    }
}

/* Existing Init */
void Buzzer_Init(void)
{
    Dio_Init(BUZZER_PORT, BUZZER_PIN, OUTPUT);
    /* Make sure to Init the Passive buzzer pin too if it's different! */
    /* Dio_Init(PASSIVE_BUZZER_PORT, PASSIVE_BUZZER_PIN, OUTPUT); */
}

/* Existing Start/Stop functions ... */
void Buzzer_Start(void) { Dio_WriteChannel(BUZZER_PORT, BUZZER_PIN, HIGH); }
void Buzzer_Stop(void)  { Dio_WriteChannel(BUZZER_PORT, BUZZER_PIN, LOW); }

void Buzzer_SmallBuzz(void)
{
    Buzzer_Start();
    SysTick_Wait(200);
    Buzzer_Stop();
}

void Buzzer_Alarm(void)
{
    u8 i;
    for (i = 0; i < 9; i++)
    {
        Buzzer_Start();
        SysTick_Wait(200);
        Buzzer_Stop();
        SysTick_Wait(200);
    }
}

/* --- NEW FUNCTION: La La Land (City of Stars) --- */
void Buzzer_Lalaland(void)
{
    /* Phrase 1: "City of stars..." (F# A C#) */
    Buzzer_PlayTone(NOTE_FS3, 400);
    SysTick_Wait(50); // Small gap between notes
    Buzzer_PlayTone(NOTE_A3,  400);
    SysTick_Wait(50);
    Buzzer_PlayTone(NOTE_CS4, 1200); // Long note
    SysTick_Wait(200);

    /* "...are you shining just for me?" (B A) */
    Buzzer_PlayTone(NOTE_B3, 400);
    SysTick_Wait(50);
    Buzzer_PlayTone(NOTE_A3, 800);
    SysTick_Wait(300);

    /* Phrase 2: "City of stars..." (E G# B) */
    Buzzer_PlayTone(NOTE_E3,  400);
    SysTick_Wait(50);
    Buzzer_PlayTone(NOTE_GS3, 400);
    SysTick_Wait(50);
    Buzzer_PlayTone(NOTE_B3,  1200);
    SysTick_Wait(200);

    /* "...there's so much that I can't see" (A G#) */
    Buzzer_PlayTone(NOTE_A3,  400);
    SysTick_Wait(50);
    Buzzer_PlayTone(NOTE_GS3, 800);
    SysTick_Wait(300);

    /* Phrase 3: "Who knows..." (D F# A) */
    Buzzer_PlayTone(NOTE_D3,  400);
    SysTick_Wait(50);
    Buzzer_PlayTone(NOTE_FS3, 400);
    SysTick_Wait(50);
    Buzzer_PlayTone(NOTE_A3,  1200);
    SysTick_Wait(200);
    
    /* "...is this the start of something wonderful" (G# F#) */
    Buzzer_PlayTone(NOTE_GS3, 400);
    SysTick_Wait(50);
    Buzzer_PlayTone(NOTE_FS3, 800);
}