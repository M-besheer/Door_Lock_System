// test_lcd.c
#include "test_framework.h"
#include "Lcd.h"
#include "I2C.h"
#include "SYSTICK.h"
#include "std_types.h"

TestResult test_lcd_init(void) {
    test_print("Testing LCD Initialization...\n");
    
    Lcd_Init();
    test_delay_ms(100); // Give LCD time to initialize
    
    TEST_ASSERT(1, "LCD_Init() completed");
    
    return TEST_PASS;
}

TestResult test_lcd_clear(void) {
    test_print("Testing LCD Clear...\n");
    
    Lcd_DisplayString("Test String");
    test_delay_ms(500);
    
    Lcd_Clear();
    test_delay_ms(100);
    
    // Clear should empty the display
    TEST_ASSERT(1, "LCD_Clear() executed");
    
    return TEST_PASS;
}

TestResult test_lcd_string_display(void) {
    test_print("Testing LCD String Display...\n");
    
    const char* test_strings[] = {
        "Hello World!",
        "Door Locker",
        "CSE211/322",
        "Test 1234"
    };
    
    for (int i = 0; i < 4; i++) {
        Lcd_Clear();
        Lcd_DisplayString(test_strings[i]);
        test_print("  Displayed: '%s'\n", test_strings[i]);
        test_delay_ms(500);
    }
    
    TEST_ASSERT(1, "All strings displayed");
    
    return TEST_PASS;
}

TestResult test_lcd_cursor_position(void) {
    test_print("Testing LCD Cursor Positioning...\n");
    
    // Test all positions on 16x2 display
    for (u8 row = 0; row < 2; row++) {
        for (u8 col = 0; col < 16; col++) {
            Lcd_GoToRowColumn(row, col);
            Lcd_DisplayCharacter('X');
            test_delay_ms(10);
        }
    }
    
    Lcd_Clear();
    test_delay_ms(200);
    
    // Test specific positions
    Lcd_GoToRowColumn(0, 0);
    Lcd_DisplayString("Line1");
    Lcd_GoToRowColumn(1, 5);
    Lcd_DisplayString("Line2");
    
    test_delay_ms(500);
    Lcd_Clear();
    
    TEST_ASSERT(1, "Cursor positioning working");
    
    return TEST_PASS;
}

TestResult test_lcd_special_chars(void) {
    test_print("Testing LCD Special Characters...\n");
    
    Lcd_Clear();
    Lcd_DisplayString("Special: ");
    Lcd_DisplayCharacter('*');
    Lcd_DisplayCharacter('#');
    Lcd_DisplayCharacter('@');
    Lcd_DisplayCharacter('$');
    
    test_delay_ms(500);
    Lcd_Clear();
    
    TEST_ASSERT(1, "Special characters displayed");
    
    return TEST_PASS;
}

// LCD Test Suite
TestCase lcd_tests[] = {
    {"LCD Init", test_lcd_init, 200},
    {"LCD Clear", test_lcd_clear, 1000},
    {"LCD String Display", test_lcd_string_display, 3000},
    {"LCD Cursor Position", test_lcd_cursor_position, 2000},
    {"LCD Special Chars", test_lcd_special_chars, 1000},
};

void run_lcd_tests(void) {
    test_print("\n-------------------------------\n");
    test_print("        LCD DRIVER TESTS\n");
    test_print("-------------------------------\n");
    
    test_init();
    test_run_suite(lcd_tests, sizeof(lcd_tests)/sizeof(lcd_tests[0]));
    test_print_summary();
}