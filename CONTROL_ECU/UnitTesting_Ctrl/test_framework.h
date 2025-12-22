// test_framework.h
#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Test result types
typedef enum {
    TEST_PASS = 0,
    TEST_FAIL = 1,
    TEST_SKIP = 2,
    TEST_TIMEOUT = 3
} TestResult;

// Test statistics
typedef struct {
    uint32_t total;
    uint32_t passed;
    uint32_t failed;
    uint32_t skipped;
} TestStats;

// Test case structure
typedef struct {
    const char* name;
    TestResult (*function)(void);
    uint32_t timeout_ms;
} TestCase;

// Global test stats
extern TestStats g_test_stats;

// Assertion macros
#define TEST_ASSERT(condition, message, ...) \
    do { \
        g_test_stats.total++; \
        if (condition) { \
            g_test_stats.passed++; \
            printf("[PASS] " message "\n", ##__VA_ARGS__); \
        } else { \
            g_test_stats.failed++; \
            printf("[FAIL] " message " (File: %s, Line: %d)\n", \
                      ##__VA_ARGS__, __FILE__, __LINE__); \
            return TEST_FAIL; \
        } \
    } while(0)

#define TEST_CHECK(condition, message, ...) \
    do { \
        g_test_stats.total++; \
        if (condition) { \
            g_test_stats.passed++; \
            printf("[PASS] " message "\n", ##__VA_ARGS__); \
        } else { \
            g_test_stats.failed++; \
            printf("[FAIL] " message "\n", ##__VA_ARGS__); \
        } \
    } while(0)

#define TEST_SKIP(message, ...) \
    do { \
        g_test_stats.skipped++; \
        printf("[SKIP] " message "\n", ##__VA_ARGS__); \
        return TEST_SKIP; \
    } while(0)

// Test management
void test_init(void);
void test_run_suite(TestCase* tests, uint32_t count);
void test_print_summary(void);
void test_delay_ms(uint32_t ms);

#endif // TEST_FRAMEWORK_H