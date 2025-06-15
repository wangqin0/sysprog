#include <stdint.h>

int count1(uint32_t n)
{
    int count = 0;
    while (n)
    {
        n &= n - 1;
        count++;
    }
    return count;
}
