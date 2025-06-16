#include <stdint.h>

uint32_t reverse_bits(uint32_t n)
{
    uint32_t res = 0;
    int bits = sizeof(n) * 8;
    
    for (int i = 0; i < bits; i++)
    {
        res = (res << 1) | (n & 1);
        n >>= 1;
    }
    
    return res;
}
