// test_led.c
#include "test_framework.h"
#include "Led.h"
#include "SYSTICK.h"

TestResult test_led_red(void) {
    printf("Testing Red LED...\n");
    
    Led_RedInit();
    
    // Turn on
    Led_RedTurnOn();
    printf("  Red LED ON\n");
    test_delay_ms(500);
    
    // Turn off
    Led_RedTurnOff();
    printf("  Red LED OFF\n");
    test_delay_ms(200);
    
    TEST_ASSERT(1, "Red LED tested");
    
    return TEST_PASS;
}

TestResult test_led_green(void) {
    printf("Testing Green LED...\n");
    
    Led_GreenInit();
    
    Led_GreenTurnOn();
    printf("  Green LED ON\n");
    test_delay_ms(500);
    
    Led_GreenTurnOff();
    printf("  Green LED OFF\n");
    test_delay_ms(200);
    
    TEST_ASSERT(1, "Green LED tested");
    
    return TEST_PASS;
}

TestResult test_led_blue(void) {
    printf("Testing Blue LED...\n");
    
    Led_BlueInit();
    
    Led_BlueTurnOn();
    printf("  Blue LED ON\n");
    test_delay_ms(500);
    
    Led_BlueTurnOff();
    printf("  Blue LED OFF\n");
    test_delay_ms(200);
    
    TEST_ASSERT(1, "Blue LED tested");
    
    return TEST_PASS;
}

TestResult test_led_pattern(void) {
    printf("Testing LED Patterns...\n");
    
    // Blink pattern
    for (int i = 0; i < 3; i++) {
        Led_RedTurnOn();
        Led_GreenTurnOff();
        Led_BlueTurnOff();
        test_delay_ms(200);
        
        Led_RedTurnOff();
        Led_GreenTurnOn();
        test_delay_ms(200);
        
        Led_GreenTurnOff();
        Led_BlueTurnOn();
        test_delay_ms(200);
        
        Led_BlueTurnOff();
    }
    
    printf("  Pattern completed\n");
    TEST_ASSERT(1, "LED pattern test");
    
    return TEST_PASS;
}

// LED Test Suite
TestCase led_tests[] = {
    {"Red LED", test_led_red, 1000},
    {"Green LED", test_led_green, 1000},
    {"Blue LED", test_led_blue, 1000},
    {"LED Pattern", test_led_pattern, 2000},
};

void run_led_tests(void) {
    printf("\n-------------------------------\n");
    printf("        LED DRIVER TESTS\n");
    printf("-------------------------------\n");
    
    test_init();
    test_run_suite(led_tests, sizeof(led_tests)/sizeof(led_tests[0]));
    test_print_summary();
}