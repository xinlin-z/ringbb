#ifndef RING_BYTE_BUF_H
#define RING_BYTE_BUF_H
#include <stddef.h>
#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif


/* both C&C++ support this syntax,
   but in C++, typedef is redundant */
typedef struct ringbb{
    size_t capacity;
    size_t size;
    size_t rp;
    size_t wp;
    unsigned char *buf;
} ringbb;


/* it also serve as the minimum buffer length */
#define RBB_BUF_LEN_UNIT     ((size_t)1 << 12)  // 4K


#if SIZE_MAX > 0xFFFFFFFFu
#define RBB_BUF_MAX_SIZE     ((size_t)1 << 32)  // 4G @ 64bit
#else
#define RBB_BUF_MAX_SIZE     ((size_t)1 << 30)  // 1G @ 32bit
#endif


#ifdef __cplusplus
extern "C" {
#endif
bool   rbb_init(ringbb*, size_t);
void   rbb_free(ringbb*);
bool   rbb_push_back(ringbb*, const void*, size_t);
bool   rbb_push_front(ringbb*, const void*, size_t);
size_t rbb_pop_back(ringbb*, void*, size_t);
size_t rbb_pop_front(ringbb*, void*, size_t);
bool   rbb_shrink(ringbb*);
#ifdef __cplusplus
}
#endif
#endif
