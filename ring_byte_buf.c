#include "ring_byte_buf.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>  // -lm


static inline void _read_all2buf(ringbb *rb, void *buf){
    if(rb->_wp >= rb->_rp)  // _wp==_rp when size==0
        memcpy(buf, rb->_buf+rb->_rp, rb->size);
    else{
        size_t rrlen = rb->capacity - rb->_rp;
        memcpy(buf, rb->_buf+rb->_rp, rrlen);
        memcpy(buf+rrlen, rb->_buf, rb->_wp);
    }
}


static inline bool _recap(ringbb *rb, const void *mem,
                          size_t len, bool is_push_back){
    size_t need_size = pow(2.0, (int)ceil(log2(rb->size+len)));
    unsigned char *buf = (unsigned char*)malloc(need_size);
    if(!buf)
        return false;

    if(!is_push_back)
        memcpy(buf, mem, len);
    _read_all2buf(rb, buf+(is_push_back?0:len));
    free(rb->_buf);
    if(is_push_back)
        memcpy(buf+rb->size, mem, len);

    rb->_buf = buf;
    rb->capacity = need_size;
    rb->size += len;
    rb->_wp = rb->size;
    rb->_rp = 0;
    return true;
}


bool rbb_init(ringbb *rb, size_t len){
    size_t need_size = pow(2.0, (int)ceil(log2(len)));
    need_size = need_size<=64 ? 64 : need_size;  // min 64 at init
    rb->_buf = (unsigned char*)malloc(need_size);
    if(!rb->_buf)
        return false;
    rb->capacity = need_size;
    rb->size = rb->_wp = rb->_rp = 0;
    return true;
}


void rbb_free(ringbb *rb){
    free(rb->_buf);
    rb->_buf = NULL;
    rb->capacity = rb->size = rb->_wp = rb->_rp = 0;
}


bool rbb_shrink(ringbb *rb){
    size_t need_size = pow(2.0, (int)ceil(log2(rb->size)));
    if(need_size == rb->capacity)
        return true;
    unsigned char *buf = (unsigned char*)malloc(need_size);  // might lt 64
    if(!buf)
        return false;
    _read_all2buf(rb, buf);
    free(rb->_buf);
    rb->_buf = buf;
    rb->capacity = need_size;
    rb->_wp = rb->size;
    rb->_rp = 0;
    return true;
}


bool rbb_push_back(ringbb *rb, const void *mem, size_t len){
    if(len <= rb->capacity - rb->size){
        if(rb->_wp + len <= rb->capacity){
            memcpy(rb->_buf+rb->_wp, mem, len);
            rb->_wp += len;
        }else{
            size_t rwlen = rb->capacity - rb->_wp;
            memcpy(rb->_buf+rb->_wp, mem, rwlen);
            memcpy(rb->_buf, mem+rwlen, len-rwlen);
            rb->_wp = len - rwlen;
        }
        rb->size += len;
        return true;
    }else
        return _recap(rb, mem, len, true);
}


bool rbb_push_front(ringbb *rb, const void *mem, size_t len){
    if(len <= rb->capacity-rb->size){
        if(rb->_rp >= len){
            memcpy(rb->_buf+(rb->_rp-len), mem, len);
            rb->_rp -= len;
        }else{
            size_t rwlen = len - rb->_rp;
            rb->_rp = rb->capacity - rwlen;
            memcpy(rb->_buf+rb->_rp, mem, rwlen);
            memcpy(rb->_buf, mem+rwlen, len-rwlen);
        }
        rb->size += len;
        return true;
    }else
        return _recap(rb, mem, len, false);
}


size_t rbb_pop_front(ringbb *rb, void *buf, size_t len){
    size_t rt = len;
    if(rb->size >= len){
        if(rb->capacity-rb->_rp >= len){
            memcpy(buf, rb->_buf+rb->_rp, len);
            rb->_rp += len;
        }else{
            size_t rrlen = rb->capacity - rb->_rp;
            memcpy(buf, rb->_buf+rb->_rp, rrlen);
            memcpy(buf+rrlen, rb->_buf, len-rrlen);
            rb->_rp = len - rrlen;
        }
        rb->size -= len;
    }else{
        _read_all2buf(rb, buf);
        rb->_rp = rb->_wp;
        rt = rb->size;
        rb->size = 0;
    }
    return rt;
}


size_t rbb_pop_back(ringbb *rb, void *buf, size_t len){
    size_t rt = len;
    if(rb->size >= len){
        if(rb->_wp >= len){
            memcpy(buf, rb->_buf+rb->_wp-len, len);
            rb->_wp -= len;
        }else{
            size_t rrlen = len - rb->_wp;
            rb->_wp = rb->capacity - rrlen;
            memcpy(buf, rb->_buf+rb->_wp, rrlen);
            memcpy(buf+rrlen, rb->_buf, len-rrlen);
        }
        rb->size -= len;
    }else{
        _read_all2buf(rb, buf);
        rb->_rp = rb->_wp;
        rt = rb->size;
        rb->size = 0;
    }
    return rt;
}


