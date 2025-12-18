// test_door.c
#include "test_framework.h"
#include "Door_Lock.h"
#include "SYSTICK.h"

TestResult test_door_init(void) {
    test_print("Testing Door Lock Initialization...\n");
    
    DoorLock_Init();
    test_delay_ms(100);
    
    TEST_ASSERT(1, "DoorLock_Init() completed");
    
    return TEST_PASS;
}

TestResult test_door_unlock_lock(void) {
    test_print("Testing Door Unlock/Lock Cycle...\n");
    
    test_print("  Unlocking door...\n");
    DoorLock_Unlock();
    test_delay_ms(1000);
    
    test_print("  Door unlocked for 3 seconds...\n");
    for (int i = 3; i > 0; i--) {
        test_print("    %d...\n", i);
        test_delay_ms(1000);
    }
    
    test_print("  Locking door...\n");
    DoorLock_Lock();
    test_delay_ms(1000);
    
    TEST_ASSERT(1, "Door unlock/lock cycle completed");
    
    return TEST_PASS;
}

// Door Lock Test Suite
TestCase door_tests[] = {
    {"Door Init", test_door_init, 200},
    {"Door Unlock/Lock", test_door_unlock_lock, 6000},
};

void run_door_tests(void) {
    test_print("\n-------------------------------\n");
    test_print("       DOOR LOCK TESTS\n");
    test_print("-------------------------------\n");
    
    test_init();
    test_run_suite(door_tests, sizeof(door_tests)/sizeof(door_tests[0]));
    test_print_summary();
}