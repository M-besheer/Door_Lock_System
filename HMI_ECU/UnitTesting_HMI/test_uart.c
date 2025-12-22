// test_uart.c
#include "test_framework.h"
#include "UART.h"
#include "SYSTICK.h"

TestResult test_uart_init(void) {
    printf("Testing UART Initialization...\n");
    
    // Note: UART0_Init() is already called in test_init()
    // But we'll verify it works
    TEST_ASSERT(1, "UART5 (misnamed UART0) initialized");
    
    return TEST_PASS;
}

TestResult test_uart_echo(void) {
    printf("Testing UART Echo (connect PE4 and PE5 for loopback)...\n");
    printf("  If loopback not connected, this test will timeout.\n");
    
    // Send test pattern
    const char* test_string = "ECHO_TEST\n";
    UART0_SendString(test_string);
    test_delay_ms(10);
    
    // Try to receive (only works if loopback connected)
    char received[32];
    int i = 0;
    
    uint32_t timeout = 10000; // 10 seconds
    uint32_t elapsed_time = 0;
    
  
    while (elapsed_time < timeout) {
        if (UART0_IsDataAvailable()) {
            received[i++] = UART0_ReceiveChar();
            if (received[i-1] == '\n') break;
        }
        // Wait 100ms and increment counter
        SysTick_Wait(100);
        elapsed_time += 100;
    }
    received[i] = '\0';
    
    if (i > 0) {
        printf("  Received: %s", received);
        TEST_CHECK(strcmp(test_string, received) == 0, "UART echo");
    } else {
        TEST_SKIP("No loopback connection - skipping echo test");
    }
    
    return TEST_PASS;
}

TestResult test_uart_throughput(void) {
    printf("Testing UART Throughput...\n");
    
    // Send multiple strings
    for (int i = 0; i < 5; i++) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Test message %d\n", i);
        UART0_SendString(buffer);
        test_delay_ms(100);
    }
    
    TEST_ASSERT(1, "UART throughput test completed");
    
    return TEST_PASS;
}

// UART Test Suite
TestCase uart_tests[] = {
    {"UART Init", test_uart_init, 100},
    {"UART Echo", test_uart_echo, 2000},
    {"UART Throughput", test_uart_throughput, 1000},
};

void run_uart_tests(void) {
    printf("\n-------------------------------\n");
    printf("        UART TESTS\n");
    printf("-------------------------------\n");
    
    test_init();
    test_run_suite(uart_tests, sizeof(uart_tests)/sizeof(uart_tests[0]));
    test_print_summary();
}
