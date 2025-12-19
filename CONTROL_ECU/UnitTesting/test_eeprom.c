// test_eeprom.c
#include "test_framework.h"
#include "EEPROM_Driver.h"
#include "SYSTICK.h"
#include <string.h>

TestResult test_eeprom_init(void) {
    printf("Testing EEPROM Initialization...\n");
    
    bool init_result = Eeprom_Init();
    TEST_ASSERT(init_result == TRUE, "EEPROM Initialization");
    
    return TEST_PASS;
}

TestResult test_eeprom_byte_rw(void) {
    printf("Testing EEPROM Byte Read/Write...\n");
    
    u32 test_address = 0x100;
    u8 write_data = 0x5A;
    u8 read_data;
    
    // Write byte
    Eeprom_WriteByte(test_address, write_data);
    test_delay_ms(10);
    
    // Read byte
    read_data = Eeprom_ReadByte(test_address);
    
    printf("  Wrote: 0x%02X, Read: 0x%02X\n", write_data, read_data);
    TEST_ASSERT(read_data == write_data, "Byte R/W match");
    
    // Test different value
    write_data = 0xA5;
    Eeprom_WriteByte(test_address, write_data);
    test_delay_ms(10);
    
    read_data = Eeprom_ReadByte(test_address);
    TEST_ASSERT(read_data == write_data, "Byte overwrite");
    
    return TEST_PASS;
}

TestResult test_eeprom_block_rw(void) {
    printf("Testing EEPROM Block Read/Write...\n");
    
    u32 base_address = 0x200;
    u8 write_buffer[32];
    u8 read_buffer[32];
    
    // Fill buffer with pattern
    for (int i = 0; i < 32; i++) {
        write_buffer[i] = 0x20 + i;
    }
    
    // Write block
    Eeprom_WriteBlock(base_address, write_buffer, 32);
    test_delay_ms(20);
    
    // Read block
    Eeprom_ReadBlock(base_address, read_buffer, 32);
    
    // Compare
    bool match = true;
    for (int i = 0; i < 32; i++) {
        if (write_buffer[i] != read_buffer[i]) {
            printf("  Mismatch at offset %d\n", i);
            match = false;
            break;
        }
    }
    
    TEST_ASSERT(match, "Block R/W match");
    printf("  32-byte block verified\n");
    
    return TEST_PASS;
}

TestResult test_eeprom_password_storage(void) {
    printf("Testing EEPROM Password Storage...\n");
    
    // Simulate password storage
    u32 pass_address = 0x300;
    char password[] = "12345";
    char read_pass[6];
    
    // Write password (5 chars + null terminator)
    Eeprom_WriteBlock(pass_address, (u8*)password, 6);
    test_delay_ms(10);
    
    // Read password
    Eeprom_ReadBlock(pass_address, (u8*)read_pass, 6);
    
    printf("  Stored: '%s', Read: '%s'\n", password, read_pass);
    TEST_ASSERT(strcmp(password, read_pass) == 0, "Password storage");
    
    return TEST_PASS;
}

// EEPROM Test Suite
TestCase eeprom_tests[] = {
    {"EEPROM Init", test_eeprom_init, 200},
    {"EEPROM Byte R/W", test_eeprom_byte_rw, 500},
    {"EEPROM Block R/W", test_eeprom_block_rw, 500},
    {"EEPROM Password", test_eeprom_password_storage, 500},
};

void run_eeprom_tests(void) {
    printf("\n-------------------------------\n");
    printf("        EEPROM TESTS\n");
    printf("-------------------------------\n");
    
    test_init();
    test_run_suite(eeprom_tests, sizeof(eeprom_tests)/sizeof(eeprom_tests[0]));
    test_print_summary();
}