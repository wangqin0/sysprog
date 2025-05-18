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
    char *d = dst;
    const char *s = src;
    size_t i;
    const size_t step = sizeof(uint64_t);
    for (i = 0; i < sz; i += step) {
        *((uint64_t*)d) = *((uint64_t*)s);
        d += step;
        s += step;
    }
    while (i < sz) {
        *d = *s;
        d++;
        s++;
        i++;
    }
    return dst;
}
