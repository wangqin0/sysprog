#include <gtest/gtest.h>
#include <stdint.h>
#include <climits>

extern "C" {
    uint32_t reverse_bits(uint32_t n);
}

class ReverseBitsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ReverseBitsTest, Zero) {
    EXPECT_EQ(0U, reverse_bits(0U));
}

TEST_F(ReverseBitsTest, One) {
    EXPECT_EQ(0x80000000U, reverse_bits(1U));
}

TEST_F(ReverseBitsTest, MaxUint32) {
    EXPECT_EQ(UINT32_MAX, reverse_bits(UINT32_MAX));
}

TEST_F(ReverseBitsTest, PowerOfTwo) {
    EXPECT_EQ(0x40000000U, reverse_bits(2U));
    EXPECT_EQ(0x20000000U, reverse_bits(4U));
    EXPECT_EQ(0x10000000U, reverse_bits(8U));
    EXPECT_EQ(0x08000000U, reverse_bits(16U));
}

TEST_F(ReverseBitsTest, HighBit) {
    EXPECT_EQ(1U, reverse_bits(0x80000000U));
}

TEST_F(ReverseBitsTest, AlternatingBits) {
    // 0xAAAAAAAA = 10101010101010101010101010101010
    // reversed = 01010101010101010101010101010101 = 0x55555555
    EXPECT_EQ(0x55555555U, reverse_bits(0xAAAAAAAAU));
    
    // 0x55555555 = 01010101010101010101010101010101
    // reversed = 10101010101010101010101010101010 = 0xAAAAAAAA
    EXPECT_EQ(0xAAAAAAAAU, reverse_bits(0x55555555U));
}

TEST_F(ReverseBitsTest, FirstFourBits) {
    // 0xF = 00000000000000000000000000001111
    // reversed = 11110000000000000000000000000000 = 0xF0000000
    EXPECT_EQ(0xF0000000U, reverse_bits(0xFU));
}

TEST_F(ReverseBitsTest, LastFourBits) {
    // 0xF0000000 = 11110000000000000000000000000000
    // reversed = 00000000000000000000000000001111 = 0xF
    EXPECT_EQ(0xFU, reverse_bits(0xF0000000U));
}

TEST_F(ReverseBitsTest, FirstEightBits) {
    // 0xFF = 00000000000000000000000011111111
    // reversed = 11111111000000000000000000000000 = 0xFF000000
    EXPECT_EQ(0xFF000000U, reverse_bits(0xFFU));
}

TEST_F(ReverseBitsTest, LastEightBits) {
    // 0xFF000000 = 11111111000000000000000000000000
    // reversed = 00000000000000000000000011111111 = 0xFF
    EXPECT_EQ(0xFFU, reverse_bits(0xFF000000U));
}

TEST_F(ReverseBitsTest, FirstSixteenBits) {
    // 0xFFFF = 00000000000000001111111111111111
    // reversed = 11111111111111110000000000000000 = 0xFFFF0000
    EXPECT_EQ(0xFFFF0000U, reverse_bits(0xFFFFU));
}

TEST_F(ReverseBitsTest, LastSixteenBits) {
    // 0xFFFF0000 = 11111111111111110000000000000000
    // reversed = 00000000000000001111111111111111 = 0xFFFF
    EXPECT_EQ(0xFFFFU, reverse_bits(0xFFFF0000U));
}

TEST_F(ReverseBitsTest, SingleBitPositions) {
    // Test each bit position
    for (int i = 0; i < 32; i++) {
        uint32_t input = 1U << i;
        uint32_t expected = 1U << (31 - i);
        EXPECT_EQ(expected, reverse_bits(input)) << "Failed for bit position " << i;
    }
}

TEST_F(ReverseBitsTest, TwoBitsSet) {
    // 0x81 = 10000001 (in first 8 bits)
    // Full 32-bit: 00000000000000000000000010000001
    // Reversed:    10000001000000000000000000000000 = 0x81000000
    EXPECT_EQ(0x81000000U, reverse_bits(0x81U));
}

TEST_F(ReverseBitsTest, CheckerboardPattern) {
    // 0xCC = 11001100 (in first 8 bits)
    // Full 32-bit: 00000000000000000000000011001100
    // Reversed:    00110011000000000000000000000000 = 0x33000000
    EXPECT_EQ(0x33000000U, reverse_bits(0xCCU));
}

TEST_F(ReverseBitsTest, HexPatterns) {
    // 0x12345678 = 00010010001101000101011001111000
    // Reversed:    00011110011010100010110001001000 = 0x1E6A2C48
    EXPECT_EQ(0x1E6A2C48U, reverse_bits(0x12345678U));
    
    // 0xABCDEF00 = 10101011110011011110111100000000
    // Reversed:    00000000111101111011001111010101 = 0x00F7B3D5
    EXPECT_EQ(0x00F7B3D5U, reverse_bits(0xABCDEF00U));
}

TEST_F(ReverseBitsTest, SymmetricNumbers) {
    // Numbers that when reversed should equal themselves or have known patterns
    // 0x9669 = 1001011001101001 (16 bits)
    // When extended to 32 bits: 00000000000000001001011001101001
    // Reversed:                 10010110011010010000000000000000 = 0x96690000
    EXPECT_EQ(0x96690000U, reverse_bits(0x9669U));
}

TEST_F(ReverseBitsTest, BoundaryValues) {
    // Test values around powers of 2
    EXPECT_EQ(0xC0000000U, reverse_bits(3U));  // 11 -> 11000000000000000000000000000000
    EXPECT_EQ(0xE0000000U, reverse_bits(7U));  // 111 -> 11100000000000000000000000000000
    EXPECT_EQ(0xF8000000U, reverse_bits(31U)); // 11111 -> 11111000000000000000000000000000
}

TEST_F(ReverseBitsTest, LargeNumbers) {
    // Test with larger values
    EXPECT_EQ(0x0000FFFFU, reverse_bits(0xFFFF0000U));
    EXPECT_EQ(0xFFFF0000U, reverse_bits(0x0000FFFFU));
}

TEST_F(ReverseBitsTest, InverseProperty) {
    // Test that reverse(reverse(x)) == x for various values
    uint32_t test_values[] = {
        0, 1, 2, 3, 4, 5, 15, 16, 17, 31, 32, 33, 63, 64, 65,
        127, 128, 129, 255, 256, 257, 511, 512, 513, 1023, 1024, 1025,
        0x12345678U, 0xABCDEF00U, 0xDEADBEEFU, 0xCAFEBABEU,
        0x80000000U, 0x7FFFFFFFU, 0xFFFFFFFFU
    };
    
    for (uint32_t val : test_values) {
        EXPECT_EQ(val, reverse_bits(reverse_bits(val))) 
            << "Inverse property failed for 0x" << std::hex << val;
    }
}

TEST_F(ReverseBitsTest, SpecificKnownValues) {
    // Test specific values with known reverses
    struct TestCase {
        uint32_t input;
        uint32_t expected;
    };
    
    TestCase cases[] = {
        {0x00000001U, 0x80000000U},  // 1 -> MSB
        {0x80000000U, 0x00000001U},  // MSB -> 1
        {0x00000003U, 0xC0000000U},  // 11 -> 11 at MSB
        {0xC0000000U, 0x00000003U},  // 11 at MSB -> 11
        {0x0000000FU, 0xF0000000U},  // 1111 -> 1111 at MSB
        {0xF0000000U, 0x0000000FU},  // 1111 at MSB -> 1111
        {0x000000FFU, 0xFF000000U},  // byte -> reversed byte
        {0xFF000000U, 0x000000FFU},  // reversed byte -> byte
    };
    
    for (const auto& test_case : cases) {
        EXPECT_EQ(test_case.expected, reverse_bits(test_case.input))
            << "Failed for input 0x" << std::hex << test_case.input
            << ", expected 0x" << test_case.expected 
            << ", got 0x" << reverse_bits(test_case.input);
    }
}

TEST_F(ReverseBitsTest, RandomPatterns) {
    // Test some carefully chosen bit patterns
    
    // Sparse patterns
    EXPECT_EQ(0x88888888U, reverse_bits(0x11111111U));  // Every 4th bit
    
    // Dense patterns  
    EXPECT_EQ(0x77777777U, reverse_bits(0xEEEEEEEEU));  // 3 of every 4 bits
}

TEST_F(ReverseBitsTest, ConsistencyCheck) {
    // Additional consistency checks for edge cases
    
    // Check that setting the nth bit results in setting the (31-n)th bit in result
    for (int n = 0; n < 32; n++) {
        uint32_t input = 1U << n;
        uint32_t result = reverse_bits(input);
        uint32_t expected_bit_pos = 31 - n;
        
        // Check that only the expected bit is set
        EXPECT_EQ(1U << expected_bit_pos, result) 
            << "Bit " << n << " should map to bit " << expected_bit_pos;
        
        // Check that the expected bit is actually set
        EXPECT_TRUE((result >> expected_bit_pos) & 1)
            << "Expected bit " << expected_bit_pos << " not set for input bit " << n;
    }
}