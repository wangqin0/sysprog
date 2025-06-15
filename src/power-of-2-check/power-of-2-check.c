#include <stdint.h>

// check power of 2, return 1 of true
int check_power_of_2(uint32_t n)
{
    return ((n - 1) & n) == 0;
}
