/* test_mymemcpy.c
 *
 * Complete unit tests for a Linux-compatible mymemcpy implementation.
 * Verifies:
 *  - byte-for-byte correctness
 *  - dest return value
 *  - zero-length no-op
 *  - arbitrary alignment
 *  - struct copying
 *
 * Overlapping regions are deliberately NOT tested, as mymemcpy
 * behavior is undefined in that case (use memmove instead).
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "mymemcpy.h"

#define TEST_EQ(ptr, expected) assert(memcmp((ptr), (expected), sizeof(expected) - 1) == 0)
#define TEST_MEM(ptr, buf, len) assert(memcmp((ptr), (buf), (len)) == 0)

/* Test 1: simple string copy */
static void test_string_copy(void) {
    char src[]  = "Hello, mymemcpy!";
    char dest[sizeof(src)];
    void *ret = mymemcpy(dest, src, sizeof(src));
    assert(ret == dest);
    TEST_EQ(dest, "Hello, mymemcpy!");
}

/* Test 2: zero-length copy is no-op, return dest */
static void test_zero_length(void) {
    char src[]  = "unchanged";
    char dest[] = "DESTROYED";
    void *ret = mymemcpy(dest, src, 0);
    assert(ret == dest);
    /* dest must remain exactly as before */
    assert(strcmp(dest, "DESTROYED") == 0);
}

/* Test 3: arbitrary alignment and binary data */
static void test_unaligned_binary(void) {
    /* allocate extra bytes for misalignment */
    uint8_t *big = malloc(64 + 3);
    uint8_t *src = big + 1;
    uint8_t *dst = big + 17;
    for (size_t i = 0; i < 32; i++)
        src[i] = (uint8_t)(i * 7 + 13);
    void *ret = mymemcpy(dst, src, 32);
    assert(ret == dst);
    TEST_MEM(dst, src, 32);
    free(big);
}

/* Test 4: large buffer copy */
static void test_large_copy(void) {
    size_t N = 1024 * 1024;  /* 1 MiB */
    uint8_t *src = malloc(N);
    uint8_t *dst = malloc(N);
    for (size_t i = 0; i < N; i++)
        src[i] = (uint8_t)(i ^ (i >> 8));
    void *ret = mymemcpy(dst, src, N);
    assert(ret == dst);
    assert(memcmp(dst, src, N) == 0);
    free(src);
    free(dst);
}

/* A sample struct for Test 5 */
struct point { double x, y; int label; };

/* Test 5: copying an array of structs */
static void test_struct_array(void) {
    struct point src[5] = {
        {1.0, 2.0, 10},
        {3.14, 1.59, 20},
        {2.71, 8.28, 30},
        {0.0, 0.0, 40},
        {-1.0, -1.0, 50}
    }, dst[5];
    void *ret = mymemcpy(dst, src, sizeof(src));
    assert(ret == dst);
    /* verify field-by-field */
    for (int i = 0; i < 5; i++) {
        assert(dst[i].x     == src[i].x);
        assert(dst[i].y     == src[i].y);
        assert(dst[i].label == src[i].label);
    }
}

int main(void) {
    test_string_copy();
    test_zero_length();
    test_unaligned_binary();
    test_large_copy();
    test_struct_array();
    printf("All mymemcpy tests passed.\n");
    return 0;
}
