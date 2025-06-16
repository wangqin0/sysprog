#include <gtest/gtest.h>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <algorithm>

extern "C" {
    int empty();
    int full();
    int ringbuf_read(uint8_t *dst, size_t sz);
    int ringbuf_write(uint8_t *src, size_t sz);
}

class RingBufferTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset ring buffer state by reading all data
        uint8_t dummy[256];
        while (!empty()) {
            ringbuf_read(dummy, sizeof(dummy));
        }
    }
    
    void TearDown() override {
        // Clean up by reading all remaining data
        uint8_t dummy[256];
        while (!empty()) {
            ringbuf_read(dummy, sizeof(dummy));
        }
    }
    
    void fillBuffer() {
        uint8_t data[256];
        for (int i = 0; i < 256; i++) {
            data[i] = i;
        }
        ringbuf_write(data, 256);
    }
    
    bool isBufferEmpty() {
        return empty() != 0;
    }
    
    bool isBufferFull() {
        return full() != 0;
    }
};

TEST_F(RingBufferTest, InitialStateEmpty) {
    EXPECT_TRUE(isBufferEmpty());
    EXPECT_FALSE(isBufferFull());
}

TEST_F(RingBufferTest, SingleByteWriteRead) {
    uint8_t ringbuf_write_data = 0x42;
    uint8_t ringbuf_read_data = 0;
    
    int written = ringbuf_write(&ringbuf_write_data, 1);
    EXPECT_EQ(1, written);
    EXPECT_FALSE(isBufferEmpty());
    EXPECT_FALSE(isBufferFull());
    
    int ringbuf_read_count = ringbuf_read(&ringbuf_read_data, 1);
    EXPECT_EQ(1, ringbuf_read_count);
    EXPECT_EQ(0x42, ringbuf_read_data);
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, MultipleByteWriteRead) {
    uint8_t ringbuf_write_data[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t ringbuf_read_data[5] = {0};
    
    int written = ringbuf_write(ringbuf_write_data, 5);
    EXPECT_EQ(5, written);
    EXPECT_FALSE(isBufferEmpty());
    
    int ringbuf_read_count = ringbuf_read(ringbuf_read_data, 5);
    EXPECT_EQ(5, ringbuf_read_count);
    EXPECT_EQ(0, memcmp(ringbuf_write_data, ringbuf_read_data, 5));
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, FillBufferCompletely) {
    uint8_t data[256];
    for (int i = 0; i < 256; i++) {
        data[i] = i & 0xFF;
    }
    
    int written = ringbuf_write(data, 256);
    EXPECT_EQ(256, written);
    EXPECT_TRUE(isBufferFull());
    EXPECT_FALSE(isBufferEmpty());
}

TEST_F(RingBufferTest, OverflowWrite) {
    // Fill buffer completely
    fillBuffer();
    EXPECT_TRUE(isBufferFull());
    
    // Try to ringbuf_write one more byte
    uint8_t extra_byte = 0xFF;
    int written = ringbuf_write(&extra_byte, 1);
    EXPECT_EQ(0, written);  // Should not ringbuf_write anything
    EXPECT_TRUE(isBufferFull());
}

TEST_F(RingBufferTest, UnderflowRead) {
    EXPECT_TRUE(isBufferEmpty());
    
    uint8_t dummy;
    int ringbuf_read_count = ringbuf_read(&dummy, 1);
    EXPECT_EQ(0, ringbuf_read_count);  // Should not ringbuf_read anything
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, PartialRead) {
    uint8_t ringbuf_write_data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint8_t ringbuf_read_data[5] = {0};
    
    ringbuf_write(ringbuf_write_data, 10);
    
    // Read only 5 bytes
    int ringbuf_read_count = ringbuf_read(ringbuf_read_data, 5);
    EXPECT_EQ(5, ringbuf_read_count);
    EXPECT_EQ(0, memcmp(ringbuf_write_data, ringbuf_read_data, 5));
    EXPECT_FALSE(isBufferEmpty());
    
    // Read remaining 5 bytes
    memset(ringbuf_read_data, 0, 5);
    ringbuf_read_count = ringbuf_read(ringbuf_read_data, 5);
    EXPECT_EQ(5, ringbuf_read_count);
    EXPECT_EQ(0, memcmp(ringbuf_write_data + 5, ringbuf_read_data, 5));
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, PartialWrite) {
    // Fill buffer almost completely (255 bytes)
    uint8_t data[255];
    for (int i = 0; i < 255; i++) {
        data[i] = i;
    }
    ringbuf_write(data, 255);
    
    // Try to ringbuf_write 5 more bytes, only 1 should succeed
    uint8_t extra_data[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
    int written = ringbuf_write(extra_data, 5);
    EXPECT_EQ(1, written);
    EXPECT_TRUE(isBufferFull());
}

TEST_F(RingBufferTest, WrapAroundRead) {
    uint8_t ringbuf_write_data[256];
    for (int i = 0; i < 256; i++) {
        ringbuf_write_data[i] = i;
    }
    
    // Fill buffer
    ringbuf_write(ringbuf_write_data, 256);
    
    // Read half the buffer
    uint8_t ringbuf_read_data[128];
    ringbuf_read(ringbuf_read_data, 128);
    
    // Write new data (should wrap around)
    uint8_t new_data[128];
    for (int i = 0; i < 128; i++) {
        new_data[i] = 0x80 + i;
    }
    ringbuf_write(new_data, 128);
    
    // Read remaining original data
    uint8_t remaining_original[128];
    ringbuf_read(remaining_original, 128);
    EXPECT_EQ(0, memcmp(ringbuf_write_data + 128, remaining_original, 128));
    
    // Read the new wrapped data
    uint8_t wrapped_data[128];
    ringbuf_read(wrapped_data, 128);
    EXPECT_EQ(0, memcmp(new_data, wrapped_data, 128));
    
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, AlternatingWriteRead) {
    for (int cycle = 0; cycle < 10; cycle++) {
        uint8_t ringbuf_write_val = cycle * 10;
        uint8_t ringbuf_read_val = 0;
        
        ringbuf_write(&ringbuf_write_val, 1);
        EXPECT_FALSE(isBufferEmpty());
        
        ringbuf_read(&ringbuf_read_val, 1);
        EXPECT_EQ(ringbuf_write_val, ringbuf_read_val);
        EXPECT_TRUE(isBufferEmpty());
    }
}

TEST_F(RingBufferTest, StreamingDataTransfer) {
    // Test streaming data through the buffer (write some, read some, repeat)
    const int chunk_size = 64;
    const int num_chunks = 8;
    
    for (int chunk = 0; chunk < num_chunks; chunk++) {
        // Create chunk data
        uint8_t write_data[chunk_size];
        for (int i = 0; i < chunk_size; i++) {
            write_data[i] = (chunk * chunk_size + i) & 0xFF;
        }
        
        // Write the chunk
        int written = ringbuf_write(write_data, chunk_size);
        EXPECT_EQ(chunk_size, written);
        
        // Read the chunk back
        uint8_t read_data[chunk_size];
        int read_count = ringbuf_read(read_data, chunk_size);
        EXPECT_EQ(chunk_size, read_count);
        
        // Verify data integrity
        EXPECT_EQ(0, memcmp(write_data, read_data, chunk_size));
    }
    
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, BoundaryConditions) {
    // Test reading/writing exactly at buffer boundaries
    
    // Fill buffer to exactly full
    fillBuffer();
    EXPECT_TRUE(isBufferFull());
    
    // Read exactly one byte
    uint8_t byte;
    ringbuf_read(&byte, 1);
    EXPECT_EQ(0, byte);  // First byte should be 0
    EXPECT_FALSE(isBufferFull());
    EXPECT_FALSE(isBufferEmpty());
    
    // Write exactly one byte back
    byte = 0xFF;
    ringbuf_write(&byte, 1);
    EXPECT_TRUE(isBufferFull());
    
    // Read entire buffer
    uint8_t full_buffer[256];
    int ringbuf_read_count = ringbuf_read(full_buffer, 256);
    EXPECT_EQ(256, ringbuf_read_count);
    EXPECT_TRUE(isBufferEmpty());
    
    // Verify the data pattern (1-255, then 0xFF)
    for (int i = 0; i < 255; i++) {
        EXPECT_EQ(i + 1, full_buffer[i]);
    }
    EXPECT_EQ(0xFF, full_buffer[255]);
}

TEST_F(RingBufferTest, NullPointerHandling) {
    // Test null pointer handling
    uint8_t dummy = 0;
    
    EXPECT_EQ(-1, ringbuf_write(nullptr, 1));
    EXPECT_EQ(-1, ringbuf_read(nullptr, 1));
    
    // Buffer state should remain unchanged
    EXPECT_TRUE(isBufferEmpty());
    
    // Normal operations should still work
    ringbuf_write(&dummy, 1);
    EXPECT_FALSE(isBufferEmpty());
    ringbuf_read(&dummy, 1);
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, ZeroSizeOperations) {
    uint8_t dummy = 0;
    
    // Zero-size operations should succeed but do nothing
    EXPECT_EQ(0, ringbuf_write(&dummy, 0));
    EXPECT_EQ(0, ringbuf_read(&dummy, 0));
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, DataIntegrity) {
    // Test that data maintains integrity through multiple wrap-arounds
    
    const int test_cycles = 5;
    const int chunk_size = 100;
    
    for (int cycle = 0; cycle < test_cycles; cycle++) {
        // Create test pattern
        uint8_t ringbuf_write_pattern[chunk_size];
        for (int i = 0; i < chunk_size; i++) {
            ringbuf_write_pattern[i] = (cycle * chunk_size + i) & 0xFF;
        }
        
        // Write the pattern
        int written = ringbuf_write(ringbuf_write_pattern, chunk_size);
        EXPECT_EQ(chunk_size, written);
        
        // Read it back
        uint8_t ringbuf_read_pattern[chunk_size];
        int ringbuf_read_count = ringbuf_read(ringbuf_read_pattern, chunk_size);
        EXPECT_EQ(chunk_size, ringbuf_read_count);
        
        // Verify integrity
        EXPECT_EQ(0, memcmp(ringbuf_write_pattern, ringbuf_read_pattern, chunk_size));
    }
    
    EXPECT_TRUE(isBufferEmpty());
}

TEST_F(RingBufferTest, PowerOfTwoBoundary) {
    // Test that the buffer size is correctly handled as power of 2
    // BUF_SZ is 256, so valid indices are 0-255
    
    // Fill exactly to capacity
    uint8_t pattern[256];
    for (int i = 0; i < 256; i++) {
        pattern[i] = i;
    }
    
    ringbuf_write(pattern, 256);
    EXPECT_TRUE(isBufferFull());
    
    // Read back and verify the masking works correctly
    uint8_t ringbuf_readback[256];
    ringbuf_read(ringbuf_readback, 256);
    
    for (int i = 0; i < 256; i++) {
        EXPECT_EQ(i, ringbuf_readback[i]) << "Mismatch at index " << i;
    }
}

TEST_F(RingBufferTest, ConsecutiveOperations) {
    // Test many consecutive small operations
    
    for (int i = 0; i < 1000; i++) {
        uint8_t data = i & 0xFF;
        uint8_t ringbuf_readback = 0;
        
        ringbuf_write(&data, 1);
        EXPECT_FALSE(isBufferEmpty());
        
        ringbuf_read(&ringbuf_readback, 1);
        EXPECT_EQ(data, ringbuf_readback);
        EXPECT_TRUE(isBufferEmpty());
    }
}

TEST_F(RingBufferTest, StateTransitions) {
    // Test all possible state transitions
    
    // empty -> not empty
    EXPECT_TRUE(isBufferEmpty());
    uint8_t byte = 1;
    ringbuf_write(&byte, 1);
    EXPECT_FALSE(isBufferEmpty());
    EXPECT_FALSE(isBufferFull());
    
    // not empty -> full
    uint8_t fill_data[255];
    memset(fill_data, 2, 255);
    ringbuf_write(fill_data, 255);
    EXPECT_TRUE(isBufferFull());
    EXPECT_FALSE(isBufferEmpty());
    
    // full -> not full
    ringbuf_read(&byte, 1);
    EXPECT_FALSE(isBufferFull());
    EXPECT_FALSE(isBufferEmpty());
    
    // not full -> empty
    uint8_t drain_data[255];
    ringbuf_read(drain_data, 255);
    EXPECT_TRUE(isBufferEmpty());
    EXPECT_FALSE(isBufferFull());
}

TEST_F(RingBufferTest, SequentialPattern) {
    // Write a known sequential pattern and verify it ringbuf_reads back correctly
    
    const int pattern_size = 200;
    uint8_t ringbuf_write_data[pattern_size];
    uint8_t ringbuf_read_data[pattern_size];
    
    // Create ascending pattern
    for (int i = 0; i < pattern_size; i++) {
        ringbuf_write_data[i] = i % 256;
    }
    
    // Write all data
    int written = ringbuf_write(ringbuf_write_data, pattern_size);
    EXPECT_EQ(pattern_size, written);
    
    // Read it back in different chunk sizes
    int ringbuf_read_offset = 0;
    int chunk_sizes[] = {1, 5, 10, 25, 50, 109};  // Last chunk size gets remaining
    
    for (int chunk_size : chunk_sizes) {
        if (ringbuf_read_offset >= pattern_size) break;
        
        int to_ringbuf_read = std::min(chunk_size, pattern_size - ringbuf_read_offset);
        int ringbuf_read_count = ringbuf_read(ringbuf_read_data + ringbuf_read_offset, to_ringbuf_read);
        
        EXPECT_EQ(to_ringbuf_read, ringbuf_read_count);
        ringbuf_read_offset += ringbuf_read_count;
    }
    
    // Verify the entire pattern
    EXPECT_EQ(0, memcmp(ringbuf_write_data, ringbuf_read_data, pattern_size));
    EXPECT_TRUE(isBufferEmpty());
}