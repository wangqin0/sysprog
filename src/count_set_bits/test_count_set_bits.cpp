#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    int count1(uint32_t n);
}

class CountSetBitsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CountSetBitsTest, ZeroAndOne) {
    EXPECT_EQ(0, count1(0));
    EXPECT_EQ(1, count1(1));
}

TEST_F(CountSetBitsTest, PowersOfTwo) {
    EXPECT_EQ(1, count1(2));       // 10
    EXPECT_EQ(1, count1(4));       // 100
    EXPECT_EQ(1, count1(8));       // 1000
    EXPECT_EQ(1, count1(16));      // 10000
    EXPECT_EQ(1, count1(32));      // 100000
    EXPECT_EQ(1, count1(64));      // 1000000
    EXPECT_EQ(1, count1(128));     // 10000000
    EXPECT_EQ(1, count1(256));     // 100000000
    EXPECT_EQ(1, count1(512));     // 1000000000
    EXPECT_EQ(1, count1(1024));    // 10000000000
    EXPECT_EQ(1, count1(2048));    // 100000000000
    EXPECT_EQ(1, count1(4096));    // 1000000000000
    EXPECT_EQ(1, count1(8192));    // 10000000000000
    EXPECT_EQ(1, count1(16384));   // 100000000000000
    EXPECT_EQ(1, count1(32768));   // 1000000000000000
    EXPECT_EQ(1, count1(65536));   // 10000000000000000
    EXPECT_EQ(1, count1(131072));  // 100000000000000000
    EXPECT_EQ(1, count1(262144));  // 1000000000000000000
    EXPECT_EQ(1, count1(524288));  // 10000000000000000000
    EXPECT_EQ(1, count1(1048576)); // 100000000000000000000
    EXPECT_EQ(1, count1(2097152)); // 1000000000000000000000
    EXPECT_EQ(1, count1(16777216));// 1000000000000000000000000
    EXPECT_EQ(1, count1(2147483648U)); // 10000000000000000000000000000000
}

TEST_F(CountSetBitsTest, ConsecutiveBits) {
    EXPECT_EQ(2, count1(3));       // 11
    EXPECT_EQ(3, count1(7));       // 111
    EXPECT_EQ(4, count1(15));      // 1111
    EXPECT_EQ(5, count1(31));      // 11111
    EXPECT_EQ(6, count1(63));      // 111111
    EXPECT_EQ(7, count1(127));     // 1111111
    EXPECT_EQ(8, count1(255));     // 11111111
    EXPECT_EQ(16, count1(65535));  // 1111111111111111
}

TEST_F(CountSetBitsTest, AlternatingBits) {
    EXPECT_EQ(16, count1(0xAAAAAAAAU)); // 10101010...
    EXPECT_EQ(16, count1(0x55555555U)); // 01010101...
}

TEST_F(CountSetBitsTest, SpecificPatterns) {
    EXPECT_EQ(2, count1(5));       // 101
    EXPECT_EQ(2, count1(6));       // 110
    EXPECT_EQ(3, count1(11));      // 1011
    EXPECT_EQ(3, count1(13));      // 1101
    EXPECT_EQ(3, count1(14));      // 1110
    EXPECT_EQ(4, count1(23));      // 10111
    EXPECT_EQ(4, count1(27));      // 11011
    EXPECT_EQ(4, count1(29));      // 11101
    EXPECT_EQ(4, count1(30));      // 11110
}

TEST_F(CountSetBitsTest, LargeNumbers) {
    EXPECT_EQ(1, count1(0x80000000U)); // 10000000000000000000000000000000
    EXPECT_EQ(2, count1(0x80000001U)); // 10000000000000000000000000000001
    EXPECT_EQ(3, count1(0x80000003U)); // 10000000000000000000000000000011
    EXPECT_EQ(31, count1(0x7FFFFFFFU)); // 01111111111111111111111111111111
    EXPECT_EQ(32, count1(0xFFFFFFFFU)); // 11111111111111111111111111111111
}

TEST_F(CountSetBitsTest, ByteBoundaries) {
    EXPECT_EQ(8, count1(0x000000FFU)); // 00000000000000000000000011111111
    EXPECT_EQ(8, count1(0x0000FF00U)); // 00000000000000001111111100000000
    EXPECT_EQ(8, count1(0x00FF0000U)); // 00000000111111110000000000000000
    EXPECT_EQ(8, count1(0xFF000000U)); // 11111111000000000000000000000000
}

TEST_F(CountSetBitsTest, MixedPatterns) {
    EXPECT_EQ(4, count1(0x0000000FU)); // 00000000000000000000000000001111
    EXPECT_EQ(4, count1(0x000000F0U)); // 00000000000000000000000011110000
    EXPECT_EQ(4, count1(0x00000F00U)); // 00000000000000000000111100000000
    EXPECT_EQ(4, count1(0x0000F000U)); // 00000000000000001111000000000000
    EXPECT_EQ(4, count1(0x000F0000U)); // 00000000000011110000000000000000
    EXPECT_EQ(4, count1(0x00F00000U)); // 00000000111100000000000000000000
    EXPECT_EQ(4, count1(0x0F000000U)); // 00001111000000000000000000000000
    EXPECT_EQ(4, count1(0xF0000000U)); // 11110000000000000000000000000000
}

TEST_F(CountSetBitsTest, CornerCases) {
    EXPECT_EQ(16, count1(0x0000FFFFU)); // Lower 16 bits set
    EXPECT_EQ(16, count1(0xFFFF0000U)); // Upper 16 bits set
    EXPECT_EQ(16, count1(0x00FF00FFU)); // Alternating bytes
    EXPECT_EQ(16, count1(0xFF00FF00U)); // Alternating bytes
}