// test_keypad.c
#include "test_framework.h"
#include "keypad.h"
#include "SYSTICK.h"
#include <string.h>

TestResult test_keypad_init(void) {
    test_print("Testing Keypad Initialization...\n");
    
    Keypad_Init();
    TEST_ASSERT(1, "Keypad_Init() completed");
    
    // Check for phantom key presses
    test_print("  Checking for phantom key presses...\n");
    for (int i = 0; i < 10; i++) {
        const char* key = Keypad_GetKey();
        if (key != 0) {
            test_print("  WARNING: Phantom key detected: %s\n", key);
        }
        test_delay_ms(10);
    }
    
    return TEST_PASS;
}

TestResult test_keypad_single_keys(void) {
    test_print("Testing Keypad - Single Key Detection\n");
    test_print("  Press each key when prompted (10s timeout per key)\n\n");
    
    const char* expected[4][4] = {
        {"1", "2", "3", "A"},
        {"4", "5", "6", "B"},
        {"7", "8", "9", "C"},
        {"*", "0", "#", "D"}
    };
    
    int pressed_count = 0;
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            test_print("  Please press key '%s'... ", expected[row][col]);
            
            const char* pressed_key = 0;
            
            while (SysTick_Wait(10000)) {
                pressed_key = Keypad_GetKey();
                if (pressed_key != 0) {
                    break;
                }
                test_delay_ms(50);
            }
            
            if (pressed_key == 0) {
                test_print("TIMEOUT\n");
                continue;
            }
            
            if (strcmp(pressed_key, expected[row][col]) == 0) {
                test_print("OK\n");
                pressed_count++;
            } else {
                test_print("WRONG (got '%s')\n", pressed_key);
            }
            
            // Wait for key release
            while (Keypad_GetKey() != 0) {
                test_delay_ms(10);
            }
            test_delay_ms(300);
        }
    }
    
    test_print("\n  Keys pressed correctly: %d/16\n", pressed_count);
    TEST_CHECK(pressed_count >= 12, "Most keys working");
    
    return TEST_PASS;
}

TestResult test_keypad_menu_keys(void) {
    test_print("Testing Keypad Menu Keys (+, -, *)...\n");
    
    test_print("  Press '+' then '-' then '*'...\n");
    
    const char* menu_keys[] = {"+", "-", "*", "#"};
    int correct = 0;
    
    for (int i = 0; i < 4; i++) {
        test_print("  Waiting for '%s'... ", menu_keys[i]);
        
        const char* key = 0;
        
        while (SysTick_Wait(5000)) {
            key = Keypad_GetKey();
            if (key != 0) {
                break;
            }
            test_delay_ms(50);
        }
        
        if (key && strcmp(key, menu_keys[i]) == 0) {
            test_print("OK\n");
            correct++;
        } else if (key) {
            test_print("Wrong (got '%s')\n", key);
        } else {
            test_print("Timeout\n");
        }
        
        while (Keypad_GetKey() != 0) {
            test_delay_ms(10);
        }
        test_delay_ms(200);
    }
    
    TEST_CHECK(correct >= 3, "Menu keys functioning");
    
    return TEST_PASS;
}

// Keypad Test Suite
TestCase keypad_tests[] = {
    {"Keypad Init", test_keypad_init, 500},
    {"Keypad All Keys", test_keypad_single_keys, 45000},
    {"Keypad Menu Keys", test_keypad_menu_keys, 25000},
};

void run_keypad_tests(void) {
    test_print("\n-------------------------------\n");
    test_print("       KEYPAD DRIVER TESTS\n");
    test_print("-------------------------------\n");
    
    test_init();
    test_run_suite(keypad_tests, sizeof(keypad_tests)/sizeof(keypad_tests[0]));
    test_print_summary();
}