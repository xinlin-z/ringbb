#include "ring_byte_buf.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>  // -lm


static inline void _read_all2buf(ringbb *rbb, void *buf){
    if(rbb->wp > rbb->rp)
        memcpy(buf, rbb->buf+rbb->rp, rbb->size);
    else{
        size_t rrlen = rbb->capacity - rbb->rp;
        memcpy(buf, rbb->buf+rbb->rp, rrlen);
        memcpy(buf+rrlen, rbb->buf, rbb->wp);
    }
}


static inline bool _recap(ringbb *rbb,
                          const void *mem, size_t len,
                          bool is_push_back){
    size_t need_size = pow(2.0, (int)ceil(log2(rbb->size+len)));
    unsigned char *buf = (unsigned char*)malloc(need_size);
    if(!buf)
        return false;

    if(!is_push_back)
        memcpy(buf, mem, len);

    _read_all2buf(rbb, buf+(is_push_back?0:len));
    free(rbb->buf);

    if(is_push_back)
        memcpy(buf+rbb->size, mem, len);

    rbb->buf = buf;
    rbb->capacity = need_size;
    rbb->size += len;
    rbb->wp = rbb->size;
    rbb->rp = 0;
    return true;
}


bool rbb_init(ringbb *rbb, size_t len){
    size_t need_size = pow(2.0, (int)ceil(log2(len)));
    need_size = need_size<=64 ? 64 : need_size;  // min 64 at init
    rbb->buf = (unsigned char*)malloc(need_size);
    if(!rbb->buf)
        return false;
    rbb->capacity = need_size;
    rbb->size = rbb->wp = rbb->rp = 0;
    return true;
}


void rbb_free(ringbb *rbb){
    free(rbb->buf);
    rbb->buf = NULL;
    rbb->capacity = rbb->size = rbb->wp = rbb->rp = 0;
}


bool rbb_push_back(ringbb *rbb, const void *mem, size_t len){
    if(len <= rbb->capacity - rbb->size){
        if(rbb->wp + len <= rbb->capacity){
            memcpy(rbb->buf+rbb->wp, mem, len);
            rbb->wp += len;
        }else{
            size_t rwlen = rbb->capacity - rbb->wp;
            memcpy(rbb->buf+rbb->wp, mem, rwlen);
            memcpy(rbb->buf, mem+rwlen, len-rwlen);
            rbb->wp = len - rwlen;
        }
        rbb->size += len;
        return true;
    }else
        return _recap(rbb, mem, len, true);
}


size_t rbb_pop_front(ringbb *rbb, void *buf, size_t len){
    if(rbb->size == 0)
        return 0;
    size_t rt = len;
    if(rbb->size >= len){
        if(rbb->capacity-rbb->rp >= len){
            memcpy(buf, rbb->buf+rbb->rp, len);
            rbb->rp += len;
        }else{
            size_t rrlen = rbb->capacity - rbb->rp;
            memcpy(buf, rbb->buf+rbb->rp, rrlen);
            memcpy(buf+rrlen, rbb->buf, len-rrlen);
            rbb->rp = len - rrlen;
        }
        rbb->size -= len;
    }else{
        _read_all2buf(rbb, buf);
        rbb->rp = rbb->wp;
        rt = rbb->size;
        rbb->size = 0;
    }
    return rt;
}


