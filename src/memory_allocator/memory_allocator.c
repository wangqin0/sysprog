/*
Memory Allocator Simulation
  Simulate a simple memory manager that allocates and frees blocks from a fixed-size memory array. 
  You might implement first-fit allocation: find the first contiguous free segment of required size and mark it as used. 
  Also implement freeing of a given block id, coalescing freed segments if needed
*/

#include <_locale.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define MEM_SZ (1 << 30)

typedef struct block
{
    size_t size;
    int free;
    struct block *next;
} block;

static struct
{
    uint8_t mem[MEM_SZ];
    block *block_list;
} allocator;

void mem_init()
{
    allocator.block_list = (block *)(allocator.mem);
    allocator.block_list->size = MEM_SZ - sizeof(block);
    allocator.block_list->free = 1;
    allocator.block_list->next = NULL;
}

void *mem_alloc(size_t size)
{
    block *curr = allocator.block_list;
    while (curr)
    {
        if (curr->free && curr->size >= size)
        {
            if (curr->size >= size + sizeof(block))
            {
                block *split = (block *)((uint8_t *)curr + sizeof(block) + size);
                split->size = curr->size - size - sizeof(block);
                split->free = 1;
                split->next = curr->next;

                curr->next = split;
            }
            curr->free = 0;
            return (uint8_t *)curr + sizeof(block);
        }
        curr = curr->next;
    }
    return NULL;
}

void mem_free(void *ptr)
{
    if (!ptr) return;
    block *blk = ((block *)ptr) - 1;
    blk->free = 1;

    // connect free blocks from the start
    block *curr = allocator.block_list;
    while (curr && curr->next)
    {
        if (curr->free && curr->next->free)
        {
            curr->size += curr->next->size + sizeof(block);
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}