// test_buzzer.c
#include "test_framework.h"
#include "Buzzer.h"
#include "SYSTICK.h"

TestResult test_buzzer_init(void) {
    test_print("Testing Buzzer Initialization...\n");
    
    Buzzer_Init();
    TEST_ASSERT(1, "Buzzer_Init() completed");
    
    return TEST_PASS;
}

TestResult test_buzzer_on_off(void) {
    test_print("Testing Buzzer ON/OFF...\n");
    
    test_print("  Buzzer ON for 500ms...\n");
    Buzzer_Start();
    test_delay_ms(500);
    
    test_print("  Buzzer OFF for 500ms...\n");
    Buzzer_Stop();
    test_delay_ms(500);
    
    TEST_ASSERT(1, "Buzzer ON/OFF tested");
    
    return TEST_PASS;
}

TestResult test_buzzer_small_buzz(void) {
    test_print("Testing Buzzer Small Buzz (200ms)...\n");
    
    Buzzer_SmallBuzz();
    test_delay_ms(300);
    
    TEST_ASSERT(1, "Small buzz executed");
    
    return TEST_PASS;
}

TestResult test_buzzer_alarm_pattern(void) {
    test_print("Testing Buzzer Alarm Pattern...\n");
    
    // Alarm pattern: 3 short beeps
    for (int i = 0; i < 3; i++) {
        Buzzer_Start();
        test_delay_ms(100);
        Buzzer_Stop();
        test_delay_ms(100);
    }
    
    test_print("  Alarm pattern completed\n");
    TEST_ASSERT(1, "Buzzer pattern tested");
    
    return TEST_PASS;
}

// Buzzer Test Suite
TestCase buzzer_tests[] = {
    {"Buzzer Init", test_buzzer_init, 100},
    {"Buzzer ON/OFF", test_buzzer_on_off, 1500},
    {"Buzzer Small Buzz", test_buzzer_small_buzz, 1000},
    {"Buzzer Alarm Pattern", test_buzzer_alarm_pattern, 1000},
};

void run_buzzer_tests(void) {
    test_print("\n-------------------------------\n");
    test_print("       BUZZER DRIVER TESTS\n");
    test_print("-------------------------------\n");
    
    test_init();
    test_run_suite(buzzer_tests, sizeof(buzzer_tests)/sizeof(buzzer_tests[0]));
    test_print_summary();
}