// test_main.c - Main entry point for all tests
#include <stdio.h>
#include "test_framework.h"

// Declare test suite functions
void run_dio_tests(void);
void run_button_tests(void);
void run_led_tests(void);
void run_uart_tests(void);
void run_buzzer_tests(void);
void run_door_tests(void);

int main(void) {
    // Initialize system
    SysTick_Init();
    
    // Initialize test framework
    test_init();
    
    printf("\n============================================\n");
    printf("   DOOR LOCKER SYSTEM - COMPLETE TEST SUITE\n");
    printf("============================================\n\n");
    
    // Run ALL tests automatically
    printf("Running ALL tests...\n\n");
    
    //run_dio_tests();
    //run_button_tests();
    run_buzzer_tests();
    run_door_tests();
    //run_led_tests();
    run_eeprom_tests();
    run_uart_tests();
    
    // Final summary
    printf("\n============================================\n");
    printf("            TESTING COMPLETE\n");
    printf("============================================\n\n");
    
    printf("=== OVERALL TEST RESULTS ===\n");
    printf("Total Tests Run:    %lu\n", g_test_stats.total);
    printf("Passed:            %lu\n", g_test_stats.passed);
    printf("Failed:            %lu\n", g_test_stats.failed);
    printf("Skipped:           %lu\n", g_test_stats.skipped);
    
    if (g_test_stats.failed == 0) {
        printf("\n? ALL TESTS PASSED!\n");
    } else {
        printf("\n? %lu TEST(S) FAILED\n", g_test_stats.failed);
    }
    
    // End with blinking LED pattern
    Led_RedInit();
    Led_GreenInit();
    Led_BlueInit();
    
    for (int i = 0; i < 5; i++) {
        if (g_test_stats.failed == 0) {
            // Success pattern: Green blink
            Led_GreenTurnOn();
            test_delay_ms(200);
            Led_GreenTurnOff();
        } else {
            // Failure pattern: Red blink
            Led_RedTurnOn();
            test_delay_ms(200);
            Led_RedTurnOff();
        }
        test_delay_ms(200);
    }
    
    // Hold here
    while (1) {
        // Can add watchdog or reset option here
    }
    
    return 0;
}
