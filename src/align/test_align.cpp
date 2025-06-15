#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    uintptr_t align(uintptr_t p, int n);
}

class AlignTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(AlignTest, AlignBy1) {
    // n=0 means divide by 2^0 = 1 (no alignment)
    EXPECT_EQ(0, align(0, 0));
    EXPECT_EQ(1, align(1, 0));
    EXPECT_EQ(7, align(7, 0));
    EXPECT_EQ(15, align(15, 0));
    EXPECT_EQ(16, align(16, 0));
    EXPECT_EQ(1023, align(1023, 0));
    EXPECT_EQ(1024, align(1024, 0));
}

TEST_F(AlignTest, AlignBy2) {
    // n=1 means divide by 2^1 = 2
    EXPECT_EQ(0, align(0, 1));
    EXPECT_EQ(0, align(1, 1));
    EXPECT_EQ(1, align(2, 1));
    EXPECT_EQ(1, align(3, 1));
    EXPECT_EQ(2, align(4, 1));
    EXPECT_EQ(2, align(5, 1));
    EXPECT_EQ(3, align(6, 1));
    EXPECT_EQ(3, align(7, 1));
    EXPECT_EQ(4, align(8, 1));
    EXPECT_EQ(7, align(15, 1));
    EXPECT_EQ(8, align(16, 1));
}

TEST_F(AlignTest, AlignBy4) {
    // n=2 means divide by 2^2 = 4
    EXPECT_EQ(0, align(0, 2));
    EXPECT_EQ(0, align(1, 2));
    EXPECT_EQ(0, align(2, 2));
    EXPECT_EQ(0, align(3, 2));
    EXPECT_EQ(1, align(4, 2));
    EXPECT_EQ(1, align(5, 2));
    EXPECT_EQ(1, align(6, 2));
    EXPECT_EQ(1, align(7, 2));
    EXPECT_EQ(2, align(8, 2));
    EXPECT_EQ(3, align(15, 2));
    EXPECT_EQ(4, align(16, 2));
    EXPECT_EQ(255, align(1023, 2));
    EXPECT_EQ(256, align(1024, 2));
}

TEST_F(AlignTest, AlignBy8) {
    // n=3 means divide by 2^3 = 8
    EXPECT_EQ(0, align(0, 3));
    EXPECT_EQ(0, align(7, 3));
    EXPECT_EQ(1, align(8, 3));
    EXPECT_EQ(1, align(15, 3));
    EXPECT_EQ(2, align(16, 3));
    EXPECT_EQ(127, align(1023, 3));
    EXPECT_EQ(128, align(1024, 3));
}

TEST_F(AlignTest, AlignBy16) {
    // n=4 means divide by 2^4 = 16
    EXPECT_EQ(0, align(0, 4));
    EXPECT_EQ(0, align(15, 4));
    EXPECT_EQ(1, align(16, 4));
    EXPECT_EQ(1, align(31, 4));
    EXPECT_EQ(2, align(32, 4));
    EXPECT_EQ(63, align(1023, 4));
    EXPECT_EQ(64, align(1024, 4));
}

TEST_F(AlignTest, LargerAlignments) {
    // n=5 means divide by 2^5 = 32
    EXPECT_EQ(0, align(31, 5));
    EXPECT_EQ(1, align(32, 5));
    EXPECT_EQ(31, align(1023, 5));
    EXPECT_EQ(32, align(1024, 5));
    
    // n=6 means divide by 2^6 = 64
    EXPECT_EQ(0, align(63, 6));
    EXPECT_EQ(1, align(64, 6));
    EXPECT_EQ(15, align(1023, 6));
    EXPECT_EQ(16, align(1024, 6));
    
    // n=10 means divide by 2^10 = 1024
    EXPECT_EQ(0, align(1023, 10));
    EXPECT_EQ(1, align(1024, 10));
    EXPECT_EQ(1, align(2047, 10));
    EXPECT_EQ(2, align(2048, 10));
}

TEST_F(AlignTest, LargePointers) {
    uintptr_t large_ptr = 0x100000000ULL;  // 4GB
    EXPECT_EQ(large_ptr >> 1, align(large_ptr, 1));
    EXPECT_EQ(large_ptr >> 2, align(large_ptr, 2));
    EXPECT_EQ(large_ptr >> 3, align(large_ptr, 3));
    EXPECT_EQ(large_ptr >> 4, align(large_ptr, 4));
    EXPECT_EQ(large_ptr >> 10, align(large_ptr, 10));
    EXPECT_EQ(large_ptr >> 20, align(large_ptr, 20));
}

TEST_F(AlignTest, MaxValues) {
    uintptr_t max_ptr = UINTPTR_MAX;
    EXPECT_EQ(max_ptr >> 1, align(max_ptr, 1));
    EXPECT_EQ(max_ptr >> 2, align(max_ptr, 2));
    EXPECT_EQ(max_ptr >> 3, align(max_ptr, 3));
    EXPECT_EQ(max_ptr >> 10, align(max_ptr, 10));
}