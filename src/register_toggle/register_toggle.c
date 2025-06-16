#include <stdint.h>

void set_bit(uint8_t r, int n)
{
    r |= 1 << n;
}

void clear_bit(uint8_t r, int n)
{
    r &= 1 << n;
}
