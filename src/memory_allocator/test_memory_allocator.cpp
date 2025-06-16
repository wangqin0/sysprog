#include <gtest/gtest.h>
#include <stdint.h>
#include <cstring>

extern "C" {
    void mem_init();
    void *mem_alloc(size_t size);
    void mem_free(void *ptr);
}

class MemoryAllocatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        mem_init();
    }
    void TearDown() override {}
};

TEST_F(MemoryAllocatorTest, InitializationTest) {
    // Test that initialization works
    void *ptr = mem_alloc(100);
    EXPECT_NE(nullptr, ptr);
    mem_free(ptr);
}

TEST_F(MemoryAllocatorTest, BasicAllocation) {
    void *ptr = mem_alloc(100);
    EXPECT_NE(nullptr, ptr);
    
    // Test that we can write to allocated memory
    memset(ptr, 0xAA, 100);
    uint8_t *bytes = (uint8_t*)ptr;
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(0xAA, bytes[i]);
    }
    
    mem_free(ptr);
}

TEST_F(MemoryAllocatorTest, ZeroSizeAllocation) {
    void *ptr = mem_alloc(0);
    EXPECT_NE(nullptr, ptr);
    mem_free(ptr);
}

TEST_F(MemoryAllocatorTest, MultipleAllocations) {
    void *ptr1 = mem_alloc(100);
    void *ptr2 = mem_alloc(200);
    void *ptr3 = mem_alloc(50);
    
    EXPECT_NE(nullptr, ptr1);
    EXPECT_NE(nullptr, ptr2);
    EXPECT_NE(nullptr, ptr3);
    
    // Pointers should be different
    EXPECT_NE(ptr1, ptr2);
    EXPECT_NE(ptr2, ptr3);
    EXPECT_NE(ptr1, ptr3);
    
    // Test writing to each allocation
    memset(ptr1, 0x11, 100);
    memset(ptr2, 0x22, 200);
    memset(ptr3, 0x33, 50);
    
    // Verify data integrity
    uint8_t *bytes1 = (uint8_t*)ptr1;
    uint8_t *bytes2 = (uint8_t*)ptr2;
    uint8_t *bytes3 = (uint8_t*)ptr3;
    
    for (int i = 0; i < 100; i++) EXPECT_EQ(0x11, bytes1[i]);
    for (int i = 0; i < 200; i++) EXPECT_EQ(0x22, bytes2[i]);
    for (int i = 0; i < 50; i++) EXPECT_EQ(0x33, bytes3[i]);
    
    mem_free(ptr1);
    mem_free(ptr2);
    mem_free(ptr3);
}

TEST_F(MemoryAllocatorTest, FreeAndRealloc) {
    void *ptr1 = mem_alloc(100);
    EXPECT_NE(nullptr, ptr1);
    
    mem_free(ptr1);
    
    // Should be able to allocate again
    void *ptr2 = mem_alloc(80);
    EXPECT_NE(nullptr, ptr2);
    
    mem_free(ptr2);
}

TEST_F(MemoryAllocatorTest, FreeNullPointer) {
    // Should not crash
    mem_free(nullptr);
    
    // Should still be able to allocate after freeing null
    void *ptr = mem_alloc(100);
    EXPECT_NE(nullptr, ptr);
    mem_free(ptr);
}

TEST_F(MemoryAllocatorTest, CoalescingAdjacentBlocks) {
    // Allocate three blocks
    void *ptr1 = mem_alloc(100);
    void *ptr2 = mem_alloc(100);
    void *ptr3 = mem_alloc(100);
    
    EXPECT_NE(nullptr, ptr1);
    EXPECT_NE(nullptr, ptr2);
    EXPECT_NE(nullptr, ptr3);
    
    // Free middle block first
    mem_free(ptr2);
    
    // Free first block - should coalesce with middle
    mem_free(ptr1);
    
    // Now free third block - should coalesce with the combined first+middle
    mem_free(ptr3);
    
    // Should be able to allocate a large block that uses the coalesced space
    void *large_ptr = mem_alloc(280); // Should fit in coalesced space
    EXPECT_NE(nullptr, large_ptr);
    
    mem_free(large_ptr);
}

TEST_F(MemoryAllocatorTest, FragmentationAndCoalescing) {
    // Create a fragmented memory pattern
    void *ptrs[10];
    
    // Allocate 10 blocks
    for (int i = 0; i < 10; i++) {
        ptrs[i] = mem_alloc(50);
        EXPECT_NE(nullptr, ptrs[i]);
    }
    
    // Free every other block to create fragmentation
    for (int i = 1; i < 10; i += 2) {
        mem_free(ptrs[i]);
    }
    
    // Free remaining blocks
    for (int i = 0; i < 10; i += 2) {
        mem_free(ptrs[i]);
    }
    
    // Should be able to allocate a large block after coalescing
    void *large_ptr = mem_alloc(400);
    EXPECT_NE(nullptr, large_ptr);
    
    mem_free(large_ptr);
}

TEST_F(MemoryAllocatorTest, BlockSplitting) {
    // Allocate a small block from large memory
    void *small_ptr = mem_alloc(100);
    EXPECT_NE(nullptr, small_ptr);
    
    // Should still be able to allocate more (block splitting should work)
    void *another_ptr = mem_alloc(200);
    EXPECT_NE(nullptr, another_ptr);
    
    mem_free(small_ptr);
    mem_free(another_ptr);
}

TEST_F(MemoryAllocatorTest, LargeAllocation) {
    // Try to allocate a reasonably large chunk
    size_t large_size = 1024 * 1024; // 1MB
    void *ptr = mem_alloc(large_size);
    EXPECT_NE(nullptr, ptr);
    
    // Test writing to large allocation
    memset(ptr, 0xCC, large_size);
    uint8_t *bytes = (uint8_t*)ptr;
    
    // Check first, middle, and last bytes
    EXPECT_EQ(0xCC, bytes[0]);
    EXPECT_EQ(0xCC, bytes[large_size/2]);
    EXPECT_EQ(0xCC, bytes[large_size-1]);
    
    mem_free(ptr);
}

TEST_F(MemoryAllocatorTest, MemoryExhaustion) {
    const size_t chunk_size = 1024 * 1024; // 1MB chunks
    std::vector<void*> ptrs;
    
    // Keep allocating until we run out of memory
    void *ptr;
    while ((ptr = mem_alloc(chunk_size)) != nullptr) {
        ptrs.push_back(ptr);
        // Prevent infinite loop in case of bug
        if (ptrs.size() > 2000) break; // More than 2GB would be unexpected
    }
    
    // Should have allocated at least some blocks
    EXPECT_GT(ptrs.size(), 0);
    
    // Free all allocated blocks
    for (void *p : ptrs) {
        mem_free(p);
    }
    
    // Should be able to allocate again after freeing
    ptr = mem_alloc(chunk_size);
    EXPECT_NE(nullptr, ptr);
    mem_free(ptr);
}

TEST_F(MemoryAllocatorTest, AlignmentTest) {
    void *ptr = mem_alloc(100);
    EXPECT_NE(nullptr, ptr);
    
    // Check that pointer is reasonably aligned
    uintptr_t addr = (uintptr_t)ptr;
    EXPECT_EQ(0, addr % sizeof(void*)); // Should be at least pointer-aligned
    
    mem_free(ptr);
}

TEST_F(MemoryAllocatorTest, DoubleFreePrevention) {
    void *ptr = mem_alloc(100);
    EXPECT_NE(nullptr, ptr);
    
    mem_free(ptr);
    
    // Double free should not crash (though behavior is undefined)
    // This test mainly ensures the program doesn't crash
    mem_free(ptr);
    
    // Should still be able to allocate after double free
    void *new_ptr = mem_alloc(50);
    EXPECT_NE(nullptr, new_ptr);
    mem_free(new_ptr);
}

TEST_F(MemoryAllocatorTest, StressTest) {
    const int iterations = 1000;
    const int max_size = 1024;
    
    for (int i = 0; i < iterations; i++) {
        size_t size = (rand() % max_size) + 1;
        void *ptr = mem_alloc(size);
        
        if (ptr != nullptr) {
            // Write pattern to verify integrity
            uint8_t pattern = i % 256;
            memset(ptr, pattern, size);
            
            // Verify pattern
            uint8_t *bytes = (uint8_t*)ptr;
            for (size_t j = 0; j < size; j++) {
                EXPECT_EQ(pattern, bytes[j]);
            }
            
            // Free immediately or later
            if (i % 3 == 0) {
                mem_free(ptr);
            }
        }
    }
}

TEST_F(MemoryAllocatorTest, MinimumAllocationSize) {
    void *ptr1 = mem_alloc(1);
    EXPECT_NE(nullptr, ptr1);
    
    void *ptr2 = mem_alloc(1);
    EXPECT_NE(nullptr, ptr2);
    
    // Pointers should be different even for tiny allocations
    EXPECT_NE(ptr1, ptr2);
    
    mem_free(ptr1);
    mem_free(ptr2);
}