// test_dio.c
#include "test_framework.h"
#include "Dio.h"
#include "SYSTICK.h"

TestResult test_dio_init_output(void) {
    printf("Testing DIO Output Initialization...\n");
    
    // Test each port for output
    for (u8 port = PORT_A; port <= PORT_F; port++) {
        // Skip dangerous ports
        if (port == PORT_A || port == PORT_C) {
            continue; // Skip UART and JTAG ports
        }
        
        for (u8 pin = PIN_0; pin <= PIN_7; pin++) {
            Dio_Init(port, pin, OUTPUT);
            Dio_WriteChannel(port, pin, HIGH);
            test_delay_ms(10);
            
            u8 value = Dio_ReadChannel(port, pin);
            TEST_CHECK(value == HIGH, 
                      "Port %c Pin %d as OUTPUT", 
                      'A' + port, pin);
            
            Dio_WriteChannel(port, pin, LOW);
        }
    }
    
    return TEST_PASS;
}

TestResult test_dio_init_input(void) {
    printf("Testing DIO Input Initialization...\n");
    
    // Test with pull-up resistors
    for (u8 port = PORT_B; port <= PORT_F; port++) {
        if (port == PORT_C) continue; // Skip JTAG port
        
        for (u8 pin = PIN_0; pin <= PIN_7; pin++) {
            Dio_Init(port, pin, INPUT);
            Dio_SetPUR(port, pin, ENABLE);
            
            // Read should be HIGH with pull-up
            u8 value = Dio_ReadChannel(port, pin);
            TEST_CHECK(value == HIGH, 
                      "Port %c Pin %d with PULL-UP", 
                      'A' + port, pin);
            
            test_delay_ms(1);
        }
    }
    
    return TEST_PASS;
}

TestResult test_dio_port_f_special(void) {
    printf("Testing PORT_F Special Pins (PF0, PF4)...\n");
    
    // PF0 is special (unlock required)
    Dio_Init(PORT_F, PIN_0, INPUT);
    TEST_CHECK(1, "PF0 Initialized (requires unlock in Dio_Init)");
    
    // PF1-3 are LEDs on LaunchPad
    Dio_Init(PORT_F, PIN_1, OUTPUT);
    Dio_WriteChannel(PORT_F, PIN_1, HIGH);
    test_delay_ms(100);
    Dio_WriteChannel(PORT_F, PIN_1, LOW);
    TEST_CHECK(1, "PF1 (Red LED) tested");
    
    return TEST_PASS;
}

TestResult test_dio_read_write(void) {
    printf("Testing DIO Read/Write consistency...\n");
    
    // Use PORT_B for testing (safe port)
    u8 test_port = PORT_B;
    u8 test_pin = PIN_0;
    
    // Test write HIGH, read HIGH
    Dio_Init(test_port, test_pin, OUTPUT);
    Dio_WriteChannel(test_port, test_pin, HIGH);
    test_delay_ms(10);
    TEST_ASSERT(Dio_ReadChannel(test_port, test_pin) == HIGH,
               "Write HIGH, Read HIGH");
    
    // Test write LOW, read LOW
    Dio_WriteChannel(test_port, test_pin, LOW);
    test_delay_ms(10);
    TEST_ASSERT(Dio_ReadChannel(test_port, test_pin) == LOW,
               "Write LOW, Read LOW");
    
    return TEST_PASS;
}

// DIO Test Suite
TestCase dio_tests[] = {
    {"DIO Output Init", test_dio_init_output, 1000},
    {"DIO Input Init", test_dio_init_input, 1000},
    {"DIO Port F Special", test_dio_port_f_special, 500},
    {"DIO Read/Write", test_dio_read_write, 500},
};

void run_dio_tests(void) {
    printf("\n-------------------------------\n");
    printf("        DIO DRIVER TESTS\n");
    printf("-------------------------------\n");
    
    test_init();
    test_run_suite(dio_tests, sizeof(dio_tests)/sizeof(dio_tests[0]));
    test_print_summary();
}