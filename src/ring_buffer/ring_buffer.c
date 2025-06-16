#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define BUF_SZ 256

static struct {
    uint8_t buf[BUF_SZ];
    size_t h;
    size_t t;
    int full;
} ringbuf = {
    .h = 0,
    .t = 0,
    .full = 0
};

int empty() {
    return (!ringbuf.full && (ringbuf.h == ringbuf.t));
}

int full() {
    return ringbuf.full;
}

// return number of bytes read, -1 on error
int ringbuf_read(uint8_t *dst, size_t sz)
{
    if (!dst) return -1;

    size_t i = 0;
    while (i < sz)
    {
        if (empty()) break;

        dst[i] = ringbuf.buf[ringbuf.h];
        ringbuf.h = (ringbuf.h + 1) & (BUF_SZ - 1);
        ringbuf.full = 0;
        i++;
    }
    return i;
}

// return number of bytes written, -1 on error
int ringbuf_write(uint8_t *src, size_t sz)
{
    if (!src) return -1;
    size_t i = 0;
    while (i < sz) {
        if (full()) break;

        ringbuf.buf[ringbuf.t] = src[i];
        ringbuf.t = (ringbuf.t + 1) & (BUF_SZ - 1);
        if (ringbuf.t == ringbuf.h) ringbuf.full = 1;
        i++;
    }
    return i;
}