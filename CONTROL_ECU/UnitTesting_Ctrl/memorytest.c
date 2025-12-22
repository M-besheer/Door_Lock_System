#include "../HAL/Memory_Manager.h"
#include "../MCAL/EEPROM_Driver.h"
#include "std_types.h"
#include <string.h> // For strcmp/strcpy if needed
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


bool testPasswordManagement(void)
{
    const char* testPassword = "12345";
    char retrievedPassword[PASSWORD_STORAGE_LEN]; // 8 bytes to accommodate storage format
    Memory_SavePassword((char*)testPassword);
    Memory_GetPassword(retrievedPassword);
    return (strncmp(testPassword, retrievedPassword, PASSWORD_REAL_LEN) == 0);
}

bool testPasswordCheck(void)
{
    const char* correctPassword = "54321";
    const char* wrongPassword = "11111";
    Memory_SavePassword((char*)correctPassword);
    return Memory_CheckPassword((char*)correctPassword) && 
           !Memory_CheckPassword((char*)wrongPassword);
}

bool testDoublePasswordWrite(void)
{
    const char* firstPassword = "22222";
    const char* secondPassword = "33333";
    char retrievedPassword[PASSWORD_STORAGE_LEN];
    Memory_SavePassword((char*)firstPassword);
    Memory_SavePassword((char*)secondPassword);
    Memory_GetPassword(retrievedPassword);
    return (strncmp(secondPassword, retrievedPassword, PASSWORD_REAL_LEN) == 0);
}

bool testTimeoutManagement(void)
{
    uint32_t testTimeout = 10; // seconds
    uint32_t retrievedTimeout = 0;
    Memory_SaveTimeout(testTimeout);
    Memory_GetTimeout(&retrievedTimeout);
    return (testTimeout == retrievedTimeout);
}

bool testDoubleTimeoutWrite(void)
{
    uint32_t firstTimeout = 15;
    uint32_t secondTimeout = 20;
    uint32_t retrievedTimeout = 0;
    Memory_SaveTimeout(firstTimeout);
    Memory_SaveTimeout(secondTimeout);
    Memory_GetTimeout(&retrievedTimeout);
    return (secondTimeout == retrievedTimeout);
}

/* Combined tests: ensure password and timeout storage operate together correctly */
bool testPasswordAndTimeout_PersistTogether(void)
{
    const char* pass = "77777";
    uint32_t timeout = 30;
    uint32_t retrievedTimeout = 0;

    Memory_SavePassword((char*)pass);
    Memory_SaveTimeout(timeout);

    /* Use check API for password and getter for timeout */
    bool passOk = Memory_CheckPassword((char*)pass);
    Memory_GetTimeout(&retrievedTimeout);

    return passOk && (retrievedTimeout == timeout);
}

bool testPasswordThenTimeoutOverwriteVerify(void)
{
    const char* firstPass = "24680";
    const char* secondPass = "13579";
    uint32_t timeout = 45;
    uint32_t retrievedTimeout = 0;

    Memory_SavePassword((char*)firstPass);
    Memory_SaveTimeout(timeout);
    /* Overwrite password but keep timeout same */
    Memory_SavePassword((char*)secondPass);

    bool passOk = Memory_CheckPassword((char*)secondPass);
    Memory_GetTimeout(&retrievedTimeout);

    return passOk && (retrievedTimeout == timeout);
}

bool testInterleavedWrites(void)
{
    const char* pass1 = "00001";
    const char* pass2 = "99999";
    uint32_t t1 = 5;
    uint32_t t2 = 60;
    uint32_t retrievedTimeout = 0;

    Memory_SavePassword((char*)pass1);
    Memory_SaveTimeout(t1);
    Memory_SavePassword((char*)pass2);
    Memory_SaveTimeout(t2);

    bool passOk = Memory_CheckPassword((char*)pass2);
    Memory_GetTimeout(&retrievedTimeout);

    return passOk && (retrievedTimeout == t2);
}

/* HardReset tests */
bool testHardResetRestoresDefaults(void)
{
    /* Change values then hard reset and verify defaults */
    Memory_SavePassword((char*)"88888");
    Memory_SaveTimeout(99);
    HardReset();

    bool passOk = Memory_CheckPassword((char*)DEFAULT_PASSWORD);
    uint32_t retrievedTimeout = 0;
    Memory_GetTimeout(&retrievedTimeout);

    return passOk && (retrievedTimeout == DEFAULT_TIMEOUT);
}

bool testHardResetAfterWrites(void)
{
    /* Write some values, hard reset, then ensure defaults are back */
    Memory_SavePassword((char*)"12345");
    Memory_SaveTimeout(12);
    Memory_SavePassword((char*)"54321");
    Memory_SaveTimeout(34);
    HardReset();

    bool passOk = Memory_CheckPassword((char*)DEFAULT_PASSWORD);
    uint32_t retrievedTimeout = 0;
    Memory_GetTimeout(&retrievedTimeout);

    return passOk && (retrievedTimeout == DEFAULT_TIMEOUT);
}

void runAllTests(void)
{
    testPasswordManagement() ? printf("Password Management Test: PASSED\n") : printf("Password Management Test: FAILED\n");
    testPasswordCheck() ? printf("Password Check Test: PASSED\n") : printf("Password Check Test: FAILED\n");
    testDoublePasswordWrite() ? printf("Double Password Write Test: PASSED\n") : printf("Double Password Write Test: FAILED\n");
    testTimeoutManagement() ? printf("Timeout Management Test: PASSED\n") : printf("Timeout Management Test: FAILED\n");
    testDoubleTimeoutWrite() ? printf("Double Timeout Write Test: PASSED\n") : printf("Double Timeout Write Test: FAILED\n");

    /* Combined tests */
    testPasswordAndTimeout_PersistTogether() ? printf("Password+Timeout Persist Test: PASSED\n") : printf("Password+Timeout Persist Test: FAILED\n");
    testPasswordThenTimeoutOverwriteVerify() ? printf("Password Then Timeout Overwrite Test: PASSED\n") : printf("Password Then Timeout Overwrite Test: FAILED\n");
    testInterleavedWrites() ? printf("Interleaved Writes Test: PASSED\n") : printf("Interleaved Writes Test: FAILED\n");
    testHardResetRestoresDefaults() ? printf("HardReset Restores Defaults Test: PASSED\n") : printf("HardReset Restores Defaults Test: FAILED\n");
    testHardResetAfterWrites() ? printf("HardReset After Writes Test: PASSED\n") : printf("HardReset After Writes Test: FAILED\n");
}

int main(void)
{
    Memory_Init();
    runAllTests();
    return 0;
}