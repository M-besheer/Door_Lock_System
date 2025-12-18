// test_main.c - Main entry point for all tests
#include "test_framework.h"

// Declare test suite functions
void run_dio_tests(void);
void run_button_tests(void);
void run_keypad_tests(void);
void run_lcd_tests(void);
void run_led_tests(void);
void run_adc_tests(void);
void run_uart_tests(void);
void run_i2c_tests(void);
void run_integration_tests(void);

int main(void) {
    // Initialize system
    SysTick_Init();
    
    // Initialize test framework (and UART)
    test_init();
    
    test_print("\n--------------------------------------------\n");
    test_print("   DOOR LOCKER SYSTEM - COMPLETE TEST SUITE\n");
    test_print("--------------------------------------------\n\n");
    
    // Run individual test suites
    #ifdef RUN_ALL_TESTS
    run_dio_tests();
    run_button_tests();
    run_keypad_tests();
    run_lcd_tests();
    run_led_tests();
    run_adc_tests();
    run_uart_tests();
    run_i2c_tests();
    run_integration_tests();
    
    #elif defined(RUN_UNIT_TESTS)
    // Run only unit tests
    run_dio_tests();
    run_button_tests();
    run_keypad_tests();
    run_lcd_tests();
    run_led_tests();
    run_adc_tests();
    run_uart_tests();
    run_i2c_tests();
    
    #elif defined(RUN_SMOKE_TESTS)
    // Quick smoke tests
    test_print("Running SMOKE TESTS (quick verification)...\n\n");
    run_dio_tests();
    run_lcd_tests();
    run_led_tests();
    
    #else
    // Interactive menu (via UART)
    test_print("Select test suite:\n");
    test_print("1. DIO Tests\n");
    test_print("2. Button Tests\n");
    test_print("3. Keypad Tests\n");
    test_print("4. LCD Tests\n");
    test_print("5. LED Tests\n");
    test_print("7. ADC/Pot Tests\n");
    test_print("A. UART Tests\n");
    test_print("B. I2C Tests\n");
    test_print("C. Integration Tests\n");
    test_print("D. ALL Tests\n");
    test_print("\nEnter choice: ");
    
    // Wait for input
    while (!UART0_IsDataAvailable()) {
        // Wait for user input
    }
    
    char choice = UART0_ReceiveChar();
    UART0_SendChar(choice); // Echo
    UART0_SendChar('\n');
    
    switch (choice) {
        case '1': run_dio_tests(); break;
        case '2': run_button_tests(); break;
        case '3': run_keypad_tests(); break;
        case '4': run_lcd_tests(); break;
        case '5': run_led_tests(); break;
        case '7': run_adc_tests(); break;
        case 'A': case 'a': run_uart_tests(); break;
        case 'B': case 'b': run_i2c_tests(); break;
        case 'C': case 'c': run_integration_tests(); break;
        case 'D': case 'd':
            run_dio_tests();
            run_button_tests();
            run_keypad_tests();
            run_lcd_tests();
            run_led_tests();
            run_adc_tests();
            run_uart_tests();
            run_i2c_tests();
            run_integration_tests();
            break;
        default:
            test_print("Invalid choice!\n");
    }
    #endif
    
    // Final summary
    test_print("\n--------------------------------------------\n");
    test_print("            TESTING COMPLETE\n");
    test_print("--------------------------------------------\n\n");
    
    // Print overall statistics
    test_print("=== OVERALL TEST RESULTS ===\n");
    test_print("Total Tests Run:    %lu\n", g_test_stats.total);
    test_print("Passed:            %lu\n", g_test_stats.passed);
    test_print("Failed:            %lu\n", g_test_stats.failed);
    test_print("Skipped:           %lu\n", g_test_stats.skipped);
    
    if (g_test_stats.failed == 0) {
        test_print("\n?? SYSTEM READY FOR PRODUCTION!\n");
    } else {
        test_print("\n??  SYSTEM HAS FAILING TESTS - CHECK DRIVERS\n");
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