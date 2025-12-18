// test_framework.c
#include "test_framework.h"
#include "UART.h"  // Using your UART0 driver for output
#include "SYSTICK.h"
#include <stdarg.h>
#include <stdio.h>

TestStats g_test_stats = {0};

// Implement test_print using your UART0
void test_print(const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    UART0_SendString(buffer);
}

void test_print_string(const char* str) {
    UART0_SendString(str);
}

void test_print_hex(uint32_t value) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "0x%08lX", value);
    UART0_SendString(buffer);
}

void test_print_dec(uint32_t value) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%lu", value);
    UART0_SendString(buffer);
}

void test_init(void) {
    // Initialize UART for test output
    UART0_Init();  // Your UART5 driver (named UART0)
    memset(&g_test_stats, 0, sizeof(g_test_stats));
    
    test_print("\n\n=== TEST FRAMEWORK INITIALIZED ===\n");
    test_print("UART Output: 115200 baud\n\n");
}

void test_run_suite(TestCase* tests, uint32_t count) {
    test_print("Running %lu test(s)...\n\n", count);
    
    for (uint32_t i = 0; i < count; i++) {
        test_print("Test %lu: %s\n", i + 1, tests[i].name);
        test_print("  -> ");
        
        // Run test
        TestResult result = tests[i].function();
        
        // Print result
        switch (result) {
            case TEST_PASS: test_print("PASS\n\n"); break;
            case TEST_FAIL: test_print("FAIL\n\n"); break;
            case TEST_SKIP: test_print("SKIP\n\n"); break;
            case TEST_TIMEOUT: test_print("TIMEOUT\n\n"); break;
        }
    }
}

void test_print_summary(void) {
    test_print("\n=== TEST SUMMARY ===\n");
    test_print("Total:  %lu\n", g_test_stats.total);
    test_print("Passed: %lu\n", g_test_stats.passed);
    test_print("Failed: %lu\n", g_test_stats.failed);
    test_print("Skipped: %lu\n", g_test_stats.skipped);
    
    if (g_test_stats.failed == 0) {
        test_print("\n? ALL TESTS PASSED!\n");
    } else {
        test_print("\n? %lu TEST(S) FAILED\n", g_test_stats.failed);
    }
}

void test_delay_ms(uint32_t ms) {
    // Using your SysTick for delays
    SysTick_Wait(ms);
}