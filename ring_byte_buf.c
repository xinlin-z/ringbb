#include "ring_byte_buf.h"
#include <stdlib.h>
#include <string.h>


static inline size_t _get_needed_size(size_t len)
{
    /* never return 0, avoid malloc(0) */
    if(len == 0)
        return RBB_BUF_LEN_UNIT;

    /* just cover len bytes with multiples of unit */
    return len % RBB_BUF_LEN_UNIT == 0 ? len
           : ((len/RBB_BUF_LEN_UNIT)+1) * RBB_BUF_LEN_UNIT;
}


static inline void _read_all2buf(ringbb *rb, void *buf)
{
    if(rb->size == 0)
        return;

    if(rb->wp > rb->rp)
        memcpy(buf, rb->buf + rb->rp, rb->size);
    else
    {
        size_t rlen = rb->capacity - rb->rp;
        memcpy(buf, rb->buf + rb->rp, rlen);
        memcpy(buf + rlen, rb->buf, rb->wp);
    }
}


static inline size_t _pop_all(ringbb *rb, void *buf)
{
    size_t rt = rb->size;
    _read_all2buf(rb, buf);
    rb->size = rb->rp = rb->wp = 0;
    return rt;
}


static bool _recap(ringbb *rb,
                   const void *mem,
                   size_t len,
                   bool is_push_back)
{
    /* rb->size+len might be wrapped, - is safe! */
    if(len > RBB_BUF_MAX_SIZE - rb->size)
        return false;
    size_t needed_size = _get_needed_size(rb->size+len);

    /* len equals zero when shrink */
    if(len != 0)
    {
        /* geometric memory growth until half max size,
           then grow with fitted unit */
        size_t doubled_size = rb->capacity * 2;
        if((doubled_size > needed_size)
                && (doubled_size <= RBB_BUF_MAX_SIZE))
            needed_size = doubled_size;
    }

    unsigned char *buf = (unsigned char*)malloc(needed_size);
    if(!buf)
        return false;

    /* copy all bytes to buf */
    _read_all2buf(rb, buf + (is_push_back ? 0 : len));

    if(len != 0)
    {
        if(is_push_back)
            memcpy(buf + rb->size, mem, len);
        else
            memcpy(buf, mem, len);
    }

    free(rb->buf);
    rb->buf = buf;
    rb->capacity = needed_size;
    rb->size += len;
    rb->wp = rb->size;
    rb->rp = 0;
    return true;
}


bool rbb_init(ringbb *rb, size_t len)
{
    if(len > RBB_BUF_MAX_SIZE)
        return false;

    size_t needed_size = _get_needed_size(len);
    rb->buf = (unsigned char*)malloc(needed_size);
    if(!rb->buf)
        return false;

    rb->capacity = needed_size;
    rb->size = rb->wp = rb->rp = 0;
    return true;
}


void rbb_free(ringbb *rb)
{
    free(rb->buf);
    rb->buf = NULL;
    rb->capacity = rb->size = rb->wp = rb->rp = 0;
}


bool rbb_shrink(ringbb *rb)
{
    size_t needed_size = _get_needed_size(rb->size);
    if(needed_size == rb->capacity)
        return true;

    return _recap(rb, NULL, 0, true);
}


bool rbb_push_back(ringbb *rb, const void *mem, size_t len)
{
    if(len == 0)
        return true;

    if(len <= rb->capacity - rb->size)
    {
        if(rb->wp + len <= rb->capacity)
        {
            memcpy(rb->buf + rb->wp, mem, len);
            rb->wp += len;
        } else
        {
            size_t rlen = rb->capacity - rb->wp;
            memcpy(rb->buf + rb->wp, mem, rlen);
            memcpy(rb->buf, mem+rlen, len-rlen);
            rb->wp = len - rlen;
        }

        rb->size += len;
        return true;
    } else
        return _recap(rb, mem, len, true);
}


bool rbb_push_front(ringbb *rb, const void *mem, size_t len)
{
    if(len == 0)
        return true;

    if(len <= rb->capacity - rb->size)
    {
        if(rb->rp >= len)
        {
            memcpy(rb->buf + (rb->rp-len), mem, len);
            rb->rp -= len;
        } else
        {
            size_t rlen = len - rb->rp;
            rb->rp = rb->capacity - rlen;
            memcpy(rb->buf + rb->rp, mem, rlen);
            memcpy(rb->buf, mem+rlen, len-rlen);
        }

        rb->size += len;
        return true;
    } else
        return _recap(rb, mem, len, false);
}


size_t rbb_pop_front(ringbb *rb, void *buf, size_t len)
{
    if(len == 0)
        return 0;

    if(rb->size >= len)
    {
        if(rb->capacity - rb->rp >= len)
        {
            memcpy(buf, rb->buf + rb->rp, len);
            rb->rp += len;
        } else
        {
            size_t rlen = rb->capacity - rb->rp;
            memcpy(buf, rb->buf + rb->rp, rlen);
            memcpy(buf+rlen, rb->buf, len-rlen);
            rb->rp = len - rlen;
        }

        rb->size -= len;
        return len;
    } else
        return _pop_all(rb, buf);
}


size_t rbb_pop_back(ringbb *rb, void *buf, size_t len)
{
    if(len == 0)
        return 0;

    if(rb->size >= len)
    {
        if(rb->wp >= len)
        {
            memcpy(buf, rb->buf + rb->wp-len, len);
            rb->wp -= len;
        } else
        {
            size_t rlen = len - rb->wp;
            rb->wp = rb->capacity - rlen;
            memcpy(buf, rb->buf + rb->wp, rlen);
            memcpy(buf+rlen, rb->buf, len-rlen);
        }

        rb->size -= len;
        return len;
    } else
        return _pop_all(rb, buf);
}

