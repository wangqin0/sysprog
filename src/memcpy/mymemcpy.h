#include <stddef.h>

void* mymemcpy1(void* dst, const void* src, size_t sz) {
    char *d = dst;
    const char *s = src;
    while (sz != 0) {
        *d = *s;

        d++;
        s++;
        sz--;
    }
    return dst;
}

void* mymemcpy(void* dst, const void* src, size_t sz) {
    return dst;
}
