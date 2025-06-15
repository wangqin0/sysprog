#include <stdint.h>

uintptr_t align(uintptr_t p, int n)
{
    return p / (1 << n);
}
