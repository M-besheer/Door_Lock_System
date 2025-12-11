#include <stdint.h>
#include "tm4c123gh6pm.h"

// --- Configuration ---
// Assuming 16 MHz Clock
// 3 seconds  = 48,000,000 cycles
// 15 seconds = 240,000,000 cycles
#define DELAY_3_SEC   47999999
#define DELAY_15_SEC  239999999

// Global State
#define STATE_CLOSED 0
#define STATE_OPEN   1
volatile uint8_t g_DoorState = STATE_CLOSED;

// Prototypes
void Timer0A_Init(void);
void Timer0AIntHandler(void);

int main() {
    // 1. Setup Clock for Port B (Bit 1 = 0x02)
    SYSCTL_RCGCGPIO_R |= 0x02U; 
    
    // Delay for clock stabilization
    volatile uint32_t delay = SYSCTL_RCGCGPIO_R; 
    
    // 2. Configure PB0 - STARTUP STATE (CLOSED)
    // To ensure the door starts CLOSED (Relay OFF), we need the pin to be an INPUT.
    // This allows the relay's internal 5V to pull the line High.
    GPIO_PORTB_DIR_R &= ~0x01U;    // PB0 as INPUT (Hi-Z)
    
    // Enable Digital Function
    GPIO_PORTB_DEN_R |= 0x01U;     
    
    g_DoorState = STATE_CLOSED;

    // 3. Setup Interrupts
    __asm("CPSIE  I"); // Enable global interrupts

    // 4. Configure Timer
    Timer0A_Init();

    while (1) {
        // Sleep until interrupt
        __asm("      wfi\n");
    }
}

void Timer0A_Init(void){ 
    SYSCTL_RCGCTIMER_R |= 0x01;     // Activate TIMER0
    volatile uint32_t delay = SYSCTL_RCGCTIMER_R;

    TIMER0_CTL_R = 0x00000000;      // Disable TIMER0A
    TIMER0_CFG_R = 0x00000000;      // 32-bit mode
    
    // 0x02 = Periodic Mode
    TIMER0_TAMR_R = 0x00000002;     
    
    // Initial Load: Wait 15 seconds before first opening
    TIMER0_TAILR_R = DELAY_15_SEC;  
    
    TIMER0_ICR_R = 0x00000001;      // Clear flags
    TIMER0_IMR_R = 0x00000001;      // Arm interrupt
    
    NVIC_EN0_R = 1 << 19;           // Enable IRQ 19
    
    TIMER0_CTL_R = 0x00000003;      // Enable TIMER0A (with Stall bit)
}

void Timer0AIntHandler(void)
{
    // Acknowledge interrupt
    TIMER0_ICR_R = 0x00000001; 

    // Disable timer to update load value safely
    TIMER0_CTL_R &= ~0x01; 

    if (g_DoorState == STATE_CLOSED) {
        // --- OPEN THE DOOR (Turn Relay ON) ---
        // Logic: Relay is Active Low. We need to sink current to Ground.
        
        // 1. Make sure Data register is 0 (Low)
        GPIO_PORTB_DATA_R &= ~0x01; 
        
        // 2. Set Pin to OUTPUT (This connects the 0V to the pin)
        GPIO_PORTB_DIR_R |= 0x01; 
        
        g_DoorState = STATE_OPEN;
        TIMER0_TAILR_R = DELAY_3_SEC; // Wait 3 seconds
        
    } else {
        // --- CLOSE THE DOOR (Turn Relay OFF) ---
        // Logic: We need 5V to turn it off. Tiva can't output 5V.
        // Solution: Disconnect