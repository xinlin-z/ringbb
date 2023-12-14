#include "ring_byte_buf.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>  // -lm


static inline void _read_all2buf(ringbb *rb, void *buf){
    if(rb->wp >= rb->rp)
        memcpy(buf, rb->buf+rb->rp, rb->size);
    else{
        size_t rrlen = rb->capacity - rb->rp;
        memcpy(buf, rb->buf+rb->rp, rrlen);
        memcpy(buf+rrlen, rb->buf, rb->wp);
    }
}


static inline bool _recap(ringbb *rb,
                          const void *mem, size_t len,
                          bool is_push_back){
    size_t need_size = pow(2.0, (int)ceil(log2(rb->size+len)));
    unsigned char *buf = (unsigned char*)malloc(need_size);
    if(!buf)
        return false;

    if(!is_push_back)
        memcpy(buf, mem, len);
    _read_all2buf(rb, buf+(is_push_back?0:len));
    free(rb->buf);
    if(is_push_back)
        memcpy(buf+rb->size, mem, len);

    rb->buf = buf;
    rb->capacity = need_size;
    rb->size += len;
    rb->wp = rb->size;
    rb->rp = 0;
    return true;
}


bool rbb_init(ringbb *rb, size_t len){
    size_t need_size = pow(2.0, (int)ceil(log2(len)));
    need_size = need_size<=64 ? 64 : need_size;  // min 64 at init
    rb->buf = (unsigned char*)malloc(need_size);
    if(!rb->buf)
        return false;
    rb->capacity = need_size;
    rb->size = rb->wp = rb->rp = 0;
    return true;
}


void rbb_free(ringbb *rb){
    free(rb->buf);
    rb->buf = NULL;
    rb->capacity = rb->size = rb->wp = rb->rp = 0;
}


bool rbb_push_back(ringbb *rb, const void *mem, size_t len){
    if(len <= rb->capacity - rb->size){
        if(rb->wp + len <= rb->capacity){
            memcpy(rb->buf+rb->wp, mem, len);
            rb->wp += len;
        }else{
            size_t rwlen = rb->capacity - rb->wp;
            memcpy(rb->buf+rb->wp, mem, rwlen);
            memcpy(rb->buf, mem+rwlen, len-rwlen);
            rb->wp = len - rwlen;
        }
        rb->size += len;
        return true;
    }else
        return _recap(rb, mem, len, true);
}


bool rbb_push_front(ringbb *rb, const void *mem, size_t len){
    if(len <= rb->capacity-rb->size){
        if(rb->rp >= len){
            memcpy(rb->buf+(rb->rp-len), mem, len);
            rb->rp -= len;
        }else{
            size_t rwlen = len - rb->rp;
            rb->rp = rb->capacity - rwlen;
            memcpy(rb->buf+rb->rp, mem, rwlen);
            memcpy(rb->buf, mem+rwlen, len-rwlen);
        }
        rb->size += len;
        return true;
    }else
        return _recap(rb, mem, len, false);
}


size_t rbb_pop_front(ringbb *rb, void *buf, size_t len){
    size_t rt = len;
    if(rb->size >= len){
        if(rb->capacity-rb->rp >= len){
            memcpy(buf, rb->buf+rb->rp, len);
            rb->rp += len;
        }else{
            size_t rrlen = rb->capacity - rb->rp;
            memcpy(buf, rb->buf+rb->rp, rrlen);
            memcpy(buf+rrlen, rb->buf, len-rrlen);
            rb->rp = len - rrlen;
        }
        rb->size -= len;
    }else{
        _read_all2buf(rb, buf);
        rb->rp = rb->wp;
        rt = rb->size;
        rb->size = 0;
    }
    return rt;
}


size_t rbb_pop_back(ringbb *rb, void *buf, size_t len){
    size_t rt = len;
    if(rb->size >= len){
        if(rb->wp >= len){
            memcpy(buf, rb->buf+rb->wp-len, len);
            rb->wp -= len;
        }else{
            size_t rrlen = len - rb->wp;
            rb->wp = rb->capacity - rrlen;
            memcpy(buf, rb->buf+rb->wp, rrlen);
            memcpy(buf+rrlen, rb->buf, len-rrlen);
        }
        rb->size -= len;
    }else{
        _read_all2buf(rb, buf);
        rb->rp = rb->wp;
        rt = rb->size;
        rb->size = 0;
    }
    return rt;
}


