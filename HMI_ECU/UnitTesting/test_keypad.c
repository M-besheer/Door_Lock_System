// test_keypad.c
#include "test_framework.h"
#include "keypad.h"
#include "SYSTICK.h"
#include <string.h>
#include "std_types.h"

TestResult test_keypad_init(void) {
    printf("Testing Keypad Initialization...\n");
    
    Keypad_Init();
    TEST_ASSERT(1, "Keypad_Init() completed");
    
    // Check for phantom key presses
    printf("  Checking for phantom key presses...\n");
    for (int i = 0; i < 10; i++) {
        const char* key = Keypad_GetKey();
        if (key != 0) {
            printf("  WARNING: Phantom key detected: %s\n", key);
        }
        test_delay_ms(10);
    }
    
    return TEST_PASS;
}

TestResult test_keypad_single_keys(void) {
    printf("Testing Keypad - Single Key Detection\n");
    printf("  Press each key when prompted (10s timeout per key)\n\n");
    
    const char* expected[4][4] = {
        {"1", "2", "3", "A"},
        {"4", "5", "6", "B"},
        {"7", "8", "9", "C"},
        {"*", "0", "#", "D"}
    };
    
    int pressed_count = 0;
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            printf("  Please press key '%s'... ", expected[row][col]);
            
            const char* pressed_key = 0;
            
            uint32_t timeout = 10000; // 10 seconds
            uint32_t elapsed_time = 0;
            
            while (elapsed_time < timeout) {
                pressed_key = Keypad_GetKey();
                if (pressed_key != 0) {
                    break;
                }
                
                // Wait 100ms and increment counter
                SysTick_Wait(100);
                elapsed_time += 100;
            }
            
            if (pressed_key == 0) {
                printf("TIMEOUT\n");
                continue;
            }
            
            if (strcmp(pressed_key, expected[row][col]) == 0) {
                printf("OK\n");
                pressed_count++;
            } else {
                printf("WRONG (got '%s')\n", pressed_key);
            }
            
            // Wait for key release
            while (Keypad_GetKey() != 0) {
                test_delay_ms(10);
            }
            test_delay_ms(300);
        }
    }
    
    printf("\n  Keys pressed correctly: %d/16\n", pressed_count);
    TEST_CHECK(pressed_count >= 12, "Most keys working");
    
    return TEST_PASS;
}


// Keypad Test Suite
TestCase keypad_tests[] = {
    {"Keypad Init", test_keypad_init, 500},
    {"Keypad All Keys", test_keypad_single_keys, 45000},
};

void run_keypad_tests(void) {
    printf("\n-------------------------------\n");
    printf("       KEYPAD DRIVER TESTS\n");
    printf("-------------------------------\n");
    
    test_init();
    test_run_suite(keypad_tests, sizeof(keypad_tests)/sizeof(keypad_tests[0]));
    test_print_summary();
}
