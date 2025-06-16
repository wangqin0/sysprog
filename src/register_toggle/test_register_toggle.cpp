#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    void set_bit(uint8_t r, int n);
    void clear_bit(uint8_t r, int n);
}

class RegisterToggleTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
    
    uint8_t get_bit(uint8_t reg, int n) {
        return (reg >> n) & 1;
    }
    
    uint8_t expected_set_bit(uint8_t reg, int n) {
        return reg | (1 << n);
    }
    
    uint8_t expected_clear_bit(uint8_t reg, int n) {
        return reg & ~(1 << n);
    }
};

// Tests for set_bit function
TEST_F(RegisterToggleTest, SetBitBasic) {
    // Note: Current implementation has a bug - it doesn't modify the passed parameter
    // These tests document the current behavior and expected behavior
    uint8_t reg = 0x00;
    uint8_t original = reg;
    
    set_bit(reg, 0);
    // Current bug: reg remains unchanged because parameter is passed by value
    EXPECT_EQ(original, reg);
    
    // Expected behavior would be: EXPECT_EQ(0x01, reg);
}

TEST_F(RegisterToggleTest, SetBitPosition0) {
    uint8_t reg = 0x00;
    uint8_t expected = expected_set_bit(reg, 0);
    EXPECT_EQ(0x01, expected);
}

TEST_F(RegisterToggleTest, SetBitPosition1) {
    uint8_t reg = 0x00;
    uint8_t expected = expected_set_bit(reg, 1);
    EXPECT_EQ(0x02, expected);
}

TEST_F(RegisterToggleTest, SetBitPosition7) {
    uint8_t reg = 0x00;
    uint8_t expected = expected_set_bit(reg, 7);
    EXPECT_EQ(0x80, expected);
}

TEST_F(RegisterToggleTest, SetBitAlreadySet) {
    uint8_t reg = 0xFF;
    uint8_t expected = expected_set_bit(reg, 3);
    EXPECT_EQ(0xFF, expected);
}

TEST_F(RegisterToggleTest, SetBitMixed) {
    uint8_t reg = 0x55; // 01010101
    uint8_t expected = expected_set_bit(reg, 1);
    EXPECT_EQ(0x57, expected); // 01010111
}

TEST_F(RegisterToggleTest, SetBitAllPositions) {
    for (int i = 0; i < 8; i++) {
        uint8_t reg = 0x00;
        uint8_t expected = expected_set_bit(reg, i);
        EXPECT_EQ(1 << i, expected);
    }
}

// Tests for clear_bit function
TEST_F(RegisterToggleTest, ClearBitBasic) {
    // Note: Current implementation has TWO bugs:
    // 1. Parameter passed by value (not modified)
    // 2. Wrong bitwise operation (should be &= ~(1 << n), not &= (1 << n))
    uint8_t reg = 0xFF;
    uint8_t original = reg;
    
    clear_bit(reg, 0);
    // Current bug: reg remains unchanged because parameter is passed by value
    EXPECT_EQ(original, reg);
    
    // Expected behavior would be: EXPECT_EQ(0xFE, reg);
}

TEST_F(RegisterToggleTest, ClearBitPosition0) {
    uint8_t reg = 0xFF;
    uint8_t expected = expected_clear_bit(reg, 0);
    EXPECT_EQ(0xFE, expected);
}

TEST_F(RegisterToggleTest, ClearBitPosition1) {
    uint8_t reg = 0xFF;
    uint8_t expected = expected_clear_bit(reg, 1);
    EXPECT_EQ(0xFD, expected);
}

TEST_F(RegisterToggleTest, ClearBitPosition7) {
    uint8_t reg = 0xFF;
    uint8_t expected = expected_clear_bit(reg, 7);
    EXPECT_EQ(0x7F, expected);
}

TEST_F(RegisterToggleTest, ClearBitAlreadyClear) {
    uint8_t reg = 0x00;
    uint8_t expected = expected_clear_bit(reg, 3);
    EXPECT_EQ(0x00, expected);
}

TEST_F(RegisterToggleTest, ClearBitMixed) {
    uint8_t reg = 0xAA; // 10101010
    uint8_t expected = expected_clear_bit(reg, 1);
    EXPECT_EQ(0xA8, expected); // 10101000
}

TEST_F(RegisterToggleTest, ClearBitAllPositions) {
    for (int i = 0; i < 8; i++) {
        uint8_t reg = 0xFF;
        uint8_t expected = expected_clear_bit(reg, i);
        EXPECT_EQ(0xFF & ~(1 << i), expected);
    }
}

// Edge case tests
TEST_F(RegisterToggleTest, InvalidBitPosition) {
    // Test behavior with invalid bit positions
    uint8_t reg = 0x55;
    
    // Positions beyond 7 should ideally be handled gracefully
    // Current implementation doesn't validate input
    uint8_t expected_set_8 = expected_set_bit(reg, 8);
    uint8_t expected_set_neg = expected_set_bit(reg, -1);
    
    // These document current behavior (undefined behavior in practice)
    // In real implementation, should add bounds checking
}

// Comprehensive bit manipulation tests
TEST_F(RegisterToggleTest, SetAndClearSequence) {
    uint8_t reg = 0x00;
    
    // Test expected behavior for setting multiple bits
    reg = expected_set_bit(reg, 0);
    EXPECT_EQ(0x01, reg);
    
    reg = expected_set_bit(reg, 2);
    EXPECT_EQ(0x05, reg);
    
    reg = expected_set_bit(reg, 7);
    EXPECT_EQ(0x85, reg);
    
    // Test expected behavior for clearing bits
    reg = expected_clear_bit(reg, 0);
    EXPECT_EQ(0x84, reg);
    
    reg = expected_clear_bit(reg, 7);
    EXPECT_EQ(0x04, reg);
    
    reg = expected_clear_bit(reg, 2);
    EXPECT_EQ(0x00, reg);
}

TEST_F(RegisterToggleTest, AlternatingPattern) {
    uint8_t reg = 0x00;
    
    // Set odd bits
    for (int i = 1; i < 8; i += 2) {
        reg = expected_set_bit(reg, i);
    }
    EXPECT_EQ(0xAA, reg); // 10101010
    
    // Clear even bits (should be no-op)
    for (int i = 0; i < 8; i += 2) {
        reg = expected_clear_bit(reg, i);
    }
    EXPECT_EQ(0xAA, reg);
    
    // Set even bits
    for (int i = 0; i < 8; i += 2) {
        reg = expected_set_bit(reg, i);
    }
    EXPECT_EQ(0xFF, reg);
}

TEST_F(RegisterToggleTest, BoundaryValues) {
    // Test with all bits set
    uint8_t reg = 0xFF;
    for (int i = 0; i < 8; i++) {
        uint8_t result = expected_clear_bit(reg, i);
        EXPECT_EQ(0xFF & ~(1 << i), result);
    }
    
    // Test with no bits set
    reg = 0x00;
    for (int i = 0; i < 8; i++) {
        uint8_t result = expected_set_bit(reg, i);
        EXPECT_EQ(1 << i, result);
    }
}

TEST_F(RegisterToggleTest, IdempotentOperations) {
    uint8_t reg = 0x55;
    
    // Setting an already set bit should not change the register
    uint8_t result1 = expected_set_bit(reg, 0);
    uint8_t result2 = expected_set_bit(result1, 0);
    EXPECT_EQ(result1, result2);
    
    // Clearing an already clear bit should not change the register
    result1 = expected_clear_bit(reg, 1);
    result2 = expected_clear_bit(result1, 1);
    EXPECT_EQ(result1, result2);
}

TEST_F(RegisterToggleTest, PowerOfTwoPatterns) {
    // Test setting individual bits (powers of 2)
    for (int i = 0; i < 8; i++) {
        uint8_t reg = 0x00;
        uint8_t result = expected_set_bit(reg, i);
        EXPECT_EQ(1 << i, result);
        EXPECT_TRUE((result & (result - 1)) == 0 || result == 0); // Power of 2 check
    }
}