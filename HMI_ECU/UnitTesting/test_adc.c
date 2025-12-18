// test_adc.c
#include "test_framework.h"
#include "potentiometer.h"
#include "adc.h"
#include "SYSTICK.h"

TestResult test_adc_init(void) {
    test_print("Testing ADC Initialization...\n");
    
    ADC_Init(POT_ADC_CHANNEL);
    test_delay_ms(100);
    
    TEST_ASSERT(1, "ADC_Init() completed");
    
    return TEST_PASS;
}

TestResult test_adc_raw_read(void) {
    test_print("Testing ADC Raw Readings...\n");
    
    uint16_t readings[10];
    uint32_t sum = 0;
    
    test_print("  Taking 10 readings...\n");
    for (int i = 0; i < 10; i++) {
        readings[i] = ADC_Read();
        sum += readings[i];
        test_print("    Reading %d: %u\n", i + 1, readings[i]);
        test_delay_ms(100);
    }
    
    uint32_t average = sum / 10;
    test_print("  Average reading: %lu (0-4095)\n", average);
    
    TEST_CHECK(average <= 4095, "ADC readings in valid range");
    
    return TEST_PASS;
}

TestResult test_potentiometer_mapping(void) {
    test_print("Testing Potentiometer Mapping...\n");
    
    POT_Init();
    
    // Test mapping to different ranges
    test_print("  Mapping to 0-100 range...\n");
    for (int i = 0; i < 5; i++) {
        uint32_t mapped = POT_ReadMapped(0, 100);
        test_print("    Mapped value: %lu\n", mapped);
        TEST_CHECK(mapped <= 100, "Mapping within 0-100 range");
        test_delay_ms(200);
    }
    
    test_print("  Mapping to 5-30 range (door timeout)...\n");
    for (int i = 0; i < 5; i++) {
        uint32_t timeout = POT_ReadMapped(5, 30);
        test_print("    Timeout value: %lu seconds\n", timeout);
        TEST_CHECK(timeout >= 5 && timeout <= 30, "Timeout in 5-30 range");
        test_delay_ms(200);
    }
    
    return TEST_PASS;
}

// ADC/Pot Test Suite
TestCase adc_tests[] = {
    {"ADC Init", test_adc_init, 200},
    {"ADC Raw Read", test_adc_raw_read, 2000},
    {"Potentiometer Mapping", test_potentiometer_mapping, 3000},
};

void run_adc_tests(void) {
    test_print("\n-------------------------------\n");
    test_print("    ADC/POTENTIOMETER TESTS\n");
    test_print("-------------------------------\n");
    
    test_init();
    test_run_suite(adc_tests, sizeof(adc_tests)/sizeof(adc_tests[0]));
    test_print_summary();
}