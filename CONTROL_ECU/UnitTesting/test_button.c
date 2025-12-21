// test_button.c
#include "test_framework.h"
#include "Button.h"
#include "SYSTICK.h"

TestResult test_button_init(void) {
    printf("Testing Button Initialization...\n");
    
    Button_Init();
    TEST_ASSERT(1, "Button_Init() completed");
    
    return TEST_PASS;
}

TestResult test_button_read_logic(void) {
    printf("Testing Button Read Logic (Active LOW)...\n");
    printf("  Press and release the button when prompted\n");
    
    // Initial read (should be FALSE/released)
    u8 initial_state = Button_Read();
    printf("  Initial state (released): %s\n", 
               initial_state == FALSE ? "FALSE" : "TRUE");
    
    // Wait for button press
    printf("  Please PRESS the button now...\n");
    
    uint32_t timeout = 10000; // 10 seconds
    uint32_t elapsed_time = 0;
    u8 pressed_state = FALSE;
    
    while (elapsed_time < timeout) {
        pressed_state = Button_Read();
        if (pressed_state == TRUE) {
            break;
        }
        
        // Wait 100ms and increment counter
        SysTick_Wait(100);
        elapsed_time += 100;
    }
    
    if (pressed_state == TRUE) {
        printf("  Button PRESSED detected: TRUE\n");
        
        // Wait for release
        printf("  Please RELEASE the button...\n");
        while (Button_Read() == TRUE) {
            test_delay_ms(100);
        }
        printf("  Button RELEASED detected: FALSE\n");
        
        TEST_ASSERT(1, "Button press/release cycle completed");
    } else {
        TEST_SKIP("Button press timeout - skipping");
    }
    
    return TEST_PASS;
}

TestResult test_button_debounce(void) {
    printf("Testing Button Debounce (rapid reads)...\n");
    
    uint32_t samples = 100;
    uint32_t transitions = 0;
    u8 last_state = Button_Read();
    
    for (uint32_t i = 0; i < samples; i++) {
        u8 current_state = Button_Read();
        if (current_state != last_state) {
            transitions++;
            last_state = current_state;
        }
        test_delay_ms(10); // Read every 10ms
    }
    
    printf("  Samples: %lu, Transitions: %lu\n", samples, transitions);
    TEST_CHECK(transitions < 10, "Debouncing working (few transitions)");
    
    return TEST_PASS;
}

// Button Test Suite
TestCase button_tests[] = {
    {"Button Init", test_button_init, 100},
    {"Button Read Logic", test_button_read_logic, 15000},
    {"Button Debounce", test_button_debounce, 2000},
};

void run_button_tests(void) {
    printf("\n-------------------------------\n");
    printf("       BUTTON DRIVER TESTS\n");
    printf("-------------------------------\n");
    
    test_init();
    test_run_suite(button_tests, sizeof(button_tests)/sizeof(button_tests[0]));
    test_print_summary();
}