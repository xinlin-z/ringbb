#ifndef RING_BYTE_BUF_H
#define RING_BYTE_BUF_H 1

#ifndef __cplusplus
#include <stdio.h>
#include <stdbool.h>
#else
#include <cstdio>
#include <cstdbool>
#endif


struct ringbb{
    size_t capacity;
    size_t size;
    size_t _rp;
    size_t _wp;
    unsigned char *_buf;
};


#ifndef __cplusplus
typedef struct ringbb ringbb;
#endif


#ifdef __cplusplus
extern "C" {
#endif
bool rbb_init(ringbb*, size_t);
void rbb_free(ringbb*);
bool rbb_shrink(ringbb*);
bool rbb_push_back(ringbb*, const void*, size_t);
bool rbb_push_front(ringbb*, const void*, size_t);
size_t rbb_pop_front(ringbb*, void*, size_t);
size_t rbb_pop_back(ringbb*, void*, size_t);
#ifdef __cplusplus
}
#endif


#endif
