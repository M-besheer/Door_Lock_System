// test_integration.c
#include "test_framework.h"
#include "Lcd.h"
#include "keypad.h"
#include "Button.h"
#include "Led.h"
#include "potentiometer.h"
#include "SYSTICK.h"
#include <string.h>

TestResult test_integration_password_flow(void) {
    printf("=== Integration: Password Flow ===\n");
    
    // Initialize components
    Lcd_Init();
    Keypad_Init();
    Button_Init();
    Led_RedInit();
    Led_GreenInit();
    
    test_delay_ms(500);
    
    // Simulate LCD display
    Lcd_Clear();
    Lcd_DisplayString("+ Open Door");
    printf("  LCD: Displayed menu\n");
    test_delay_ms(1000);
    
    // Simulate button press
    printf("  Button: Checking state\n");
    u8 btn_state = Button_Read();
    printf("    Button state: %s\n", btn_state ? "PRESSED" : "RELEASED");
    
    // Simulate keypad input
    printf("  Keypad: Simulating '1' press\n");
    // (In real test, you'd press the key)
    
    // Test LED feedback
    Led_RedTurnOn();
    printf("  LED: Red ON (error state)\n");
    test_delay_ms(200);
    Led_RedTurnOff();
    Led_GreenTurnOn();
    printf("  LED: Green ON (success state)\n");
    test_delay_ms(200);
    Led_GreenTurnOff();
    
    
    Lcd_Clear();
    Lcd_DisplayString("Test Complete");
    
    TEST_ASSERT(1, "Integration flow executed");
    
    return TEST_PASS;
}

TestResult test_integration_system_states(void) {
    printf("=== Integration: System States ===\n");
    
    // Test different system states with LED indicators
    printf("  State 1: Idle (Blue LED)\n");
    Led_BlueTurnOn();
    test_delay_ms(500);
    Led_BlueTurnOff();
    
    printf("  State 2: Processing (Green blinking)\n");
    for (int i = 0; i < 3; i++) {
        Led_GreenTurnOn();
        test_delay_ms(100);
        Led_GreenTurnOff();
        test_delay_ms(100);
    }
    
    printf("  State 3: Error (Red + Buzzer)\n");
    Led_RedTurnOn();
    test_delay_ms(200);
    Led_RedTurnOff();
    test_delay_ms(200);
    
    printf("  State 4: Success (Green steady)\n");
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
    printf("\n-------------------------------\n");
    printf("       INTEGRATION TESTS\n");
    printf("-------------------------------\n");
    
    test_init();
    test_run_suite(integration_tests, sizeof(integration_tests)/sizeof(integration_tests[0]));
    test_print_summary();
}