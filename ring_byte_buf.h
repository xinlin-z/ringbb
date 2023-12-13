#ifndef RING_BYTE_BUF_H
#define RING_BYTE_BUF_H 1
#include <stdio.h>


struct ringbb{
    size_t capacity;
    size_t size;
    size_t rp;
    size_t wp;
    unsigned char *buf;
};


#ifndef __cplusplus
typedef struct ringbb ringbb;
#endif


bool rbb_init(ringbb*, size_t);
void rbb_free(ringbb*);
bool rbb_push_back(ringbb*, const void*, size_t);
size_t rbb_pop_front(ringbb*, void*, size_t);


#endif