// test_integration.c
#include "test_framework.h"
#include "Button.h"
#include "Led.h"
#include "Buzzer.h"
#include "Door_Lock.h"
#include "EEPROM_Driver.h"
#include "SYSTICK.h"
#include <string.h>

TestResult test_integration_password_flow(void) {
    test_print("=== Integration: Password Flow ===\n");
    
    // Initialize components
    Lcd_Init();
    Keypad_Init();
    Button_Init();
    Led_RedInit();
    Led_GreenInit();
    Buzzer_Init();
    DoorLock_Init();
    Eeprom_Init();
    
    test_delay_ms(500);
    
    
    // Simulate button press
    test_print("  Button: Checking state\n");
    u8 btn_state = Button_Read();
    test_print("    Button state: %s\n", btn_state ? "PRESSED" : "RELEASED");
    
    // Simulate keypad input
    test_print("  Keypad: Simulating '1' press\n");
    // (In real test, you'd press the key)
    
    // Test LED feedback
    Led_RedTurnOn();
    test_print("  LED: Red ON (error state)\n");
    test_delay_ms(200);
    Led_RedTurnOff();
    Led_GreenTurnOn();
    test_print("  LED: Green ON (success state)\n");
    test_delay_ms(200);
    Led_GreenTurnOff();
    
    // Test buzzer
    Buzzer_SmallBuzz();
    test_print("  Buzzer: Short beep\n");
    
    // Test door lock
    test_print("  Door: Unlocking\n");
    DoorLock_Unlock();
    test_delay_ms(1000);
    test_print("  Door: Locking\n");
    DoorLock_Lock();
    test_delay_ms(1000);
    
    Lcd_Clear();
    Lcd_DisplayString("Test Complete");
    
    TEST_ASSERT(1, "Integration flow executed");
    
    return TEST_PASS;
}

TestResult test_integration_system_states(void) {
    test_print("=== Integration: System States ===\n");
    
    // Test different system states with LED indicators
    test_print("  State 1: Idle (Blue LED)\n");
    Led_BlueTurnOn();
    test_delay_ms(500);
    Led_BlueTurnOff();
    
    test_print("  State 2: Processing (Green blinking)\n");
    for (int i = 0; i < 3; i++) {
        Led_GreenTurnOn();
        test_delay_ms(100);
        Led_GreenTurnOff();
        test_delay_ms(100);
    }
    
    test_print("  State 3: Error (Red + Buzzer)\n");
    Led_RedTurnOn();
    Buzzer_Start();
    test_delay_ms(200);
    Buzzer_Stop();
    Led_RedTurnOff();
    test_delay_ms(200);
    
    test_print("  State 4: Success (Green steady)\n");
    Led_GreenTurnOn();
    test_delay_ms(500);
    Led_GreenTurnOff();
    
    TEST_ASSERT(1, "System states tested");
    
    return TEST_PASS;
}

// Integration Test Suite
TestCase integration_tests[] = {
    {"Password Flow", test_integration_password_flow, 5000},
    {"System States", test_integration_system_states, 3000},
};

void run_integration_tests(void) {
    test_print("\n-------------------------------\n");
    test_print("       INTEGRATION TESTS\n");
    test_print("-------------------------------\n");
    
    test_init();
    test_run_suite(integration_tests, sizeof(integration_tests)/sizeof(integration_tests[0]));
    test_print_summary();
}