#include "ring_byte_buf.h"
#include <stdio.h>
#include <string.h>


#define EQ(a,b)  \
    do {  \
        if((a) != (b))  \
            fprintf(stderr, "EQ FAILED %s:%d\n", __FILE__, __LINE__);  \
    } while(0)


void test_1(){
    ringbb rb;
    rbb_init(&rb, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 0);

    char *m1 = "0123456789";
    rbb_push_back(&rb, m1, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 10);

    char p1[16] = {};
    size_t r = rbb_pop_front(&rb, p1, 16);
    EQ(r, 10);
    EQ(memcmp(m1,p1,10), 0);
    r = rbb_pop_front(&rb, p1, 1);
    EQ(r, 0);
    r = rbb_pop_front(&rb, p1, 0);
    EQ(r, 0);

    rbb_push_front(&rb, m1, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 10);
    r = rbb_pop_back(&rb, p1, 16);
    EQ(r, 10);
    EQ(memcmp(m1,p1,10), 0);
    r = rbb_pop_back(&rb, p1, 1);
    EQ(r, 0);
    r = rbb_pop_back(&rb, p1, 0);
    EQ(r, 0);

    rbb_free(&rb);
}


void test_2(){
    char *m1 = "0123456789";
    ringbb rb;

    rbb_init(&rb, 0);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 0);
    rbb_push_back(&rb, m1, 0);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 0);
    rbb_push_back(&rb, m1, 1);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 1);
    rbb_push_back(&rb, m1, 1);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 2);
    rbb_push_back(&rb, m1, 1);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 3);
    rbb_free(&rb);

    rbb_init(&rb, 0);
    rbb_push_front(&rb, m1, 0);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 0);
    rbb_push_front(&rb, m1, 1);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 1);
    rbb_push_front(&rb, m1, 1);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 2);
    rbb_push_front(&rb, m1, 1);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 3);
    rbb_free(&rb);

    rbb_init(&rb, 0);
    for(int i=0; i<10; ++i)
        rbb_push_back(&rb, m1, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 100);
    for(int i=0; i<10; ++i)
        rbb_push_back(&rb, m1, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 200);
    for(int i=0; i<10; ++i)
        rbb_push_back(&rb, m1, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 300);
    rbb_free(&rb);

    rbb_init(&rb, 0);
    for(int i=0; i<10; ++i)
        rbb_push_front(&rb, m1, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 100);
    for(int i=0; i<10; ++i)
        rbb_push_front(&rb, m1, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 200);
    for(int i=0; i<10; ++i)
        rbb_push_front(&rb, m1, 10);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 300);
    rbb_free(&rb);
}


void test_3(){
    ringbb rb;
    rbb_init(&rb, 100);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    char c;
    for(size_t i=0; i<128; ++i){
        c = (char)i;
        rbb_push_back(&rb, &c, 1);
        EQ(rb.size, i+1);
    }
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    size_t r;
    for(size_t i=0; i<128; ++i){
        r = rbb_pop_front(&rb, &c, 1);
        EQ(r, 1);
        EQ(c, (char)i);
        EQ(rb.size, 128-i-1);
    }
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    r = rbb_pop_front(&rb, &c, 1);
    EQ(r, 0);
    r = rbb_pop_front(&rb, &c, 0);
    EQ(r, 0);
    rbb_free(&rb);

    rbb_init(&rb, 100);
    for(size_t i=0; i<128; ++i){
        c = (char)i;
        rbb_push_front(&rb, &c, 1);
        EQ(rb.size, i+1);
    }
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    for(size_t i=0; i<128; ++i){
        r = rbb_pop_back(&rb, &c, 1);
        EQ(r, 1);
        EQ(c, (char)i);
        EQ(rb.size, 128-i-1);
    }
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    r = rbb_pop_back(&rb, &c, 1);
    EQ(r, 0);
    r = rbb_pop_back(&rb, &c, 0);
    EQ(r, 0);
    rbb_free(&rb);
}


void test_4(){
    char *m1 = "0123456789";
    char c10[10] = {};

    ringbb rb;
    rbb_init(&rb, RBB_BUF_LEN_UNIT);
    for(int i=0; i<1000; ++i)
    {
        rbb_push_back(&rb, m1, 10);
        EQ(rbb_pop_front(&rb,&c10,10), 10);
        EQ(memcmp(m1,c10,10), 0);
        EQ(rbb_pop_front(&rb,&c10,10), 0);
        rbb_push_front(&rb, m1, 10);
        EQ(rbb_pop_back(&rb,&c10,10), 10);
        EQ(memcmp(m1,c10,10), 0);
        EQ(rbb_pop_back(&rb,&c10,10), 0);
        rbb_push_back(&rb, m1, 10);
        EQ(rbb_pop_back(&rb,&c10,10), 10);
        EQ(memcmp(m1,c10,10), 0);
        EQ(rbb_pop_back(&rb,&c10,10), 0);
        rbb_push_front(&rb, m1, 10);
        EQ(rbb_pop_front(&rb,&c10,10), 10);
        EQ(memcmp(m1,c10,10), 0);
        EQ(rbb_pop_front(&rb,&c10,10), 0);
    }
    rbb_free(&rb);

    rbb_init(&rb, RBB_BUF_LEN_UNIT);
    for(int i=0; i<1000; ++i)
        rbb_push_back(&rb, m1, 10);
    for(int i=0; i<1000; ++i)
    {
        EQ(rbb_pop_front(&rb,&c10,10), 10);
        EQ(memcmp(m1,c10,10), 0);
    }
    EQ(rb.size, 0);
    for(int i=0; i<1000; ++i)
        rbb_push_front(&rb, m1, 10);
    for(int i=0; i<1000; ++i)
    {
        EQ(rbb_pop_back(&rb,&c10,10), 10);
        EQ(memcmp(m1,c10,10), 0);
    }
    EQ(rb.size, 0);
    rbb_free(&rb);

    rbb_init(&rb, RBB_BUF_LEN_UNIT);
    char *m2 = "0123456";
    for(int i=0; i<1000; ++i)
    {
        rbb_push_back(&rb, m2, 7);
        rbb_shrink(&rb);
    }
    for(int i=0; i<500; ++i)
        EQ(rbb_pop_back(&rb, c10, 10), 10);
    EQ(rb.size, 2000);
    rbb_shrink(&rb);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    rbb_free(&rb);
}


void test_5(){
    char *m1 = "0123456789";
    ringbb rb;
    rbb_init(&rb, RBB_BUF_LEN_UNIT);
    char c10[10] = {};
    for(int i=0; i<1000; ++i)
        rbb_push_back(&rb, m1, 10);
    EQ(rb.size, 10000);
    for(int i=0; i<1000; ++i)
    {
        rbb_pop_front(&rb, c10, 10);
        EQ(memcmp(m1,c10,10), 0);
    }
    EQ(rb.size, 0);
    for(int i=0; i<1000; ++i)
        rbb_push_front(&rb, m1, 10);
    EQ(rb.size, 10000);
    for(int i=0; i<1000; ++i){
        rbb_pop_back(&rb, c10, 10);
        EQ(memcmp(m1,c10,10), 0);
    }
    EQ(rb.size, 0);
    rbb_free(&rb);
}


void test_6(){
    ringbb rb;
    rbb_init(&rb, 0);
    rbb_shrink(&rb);
    EQ(rb.capacity, RBB_BUF_LEN_UNIT);
    EQ(rb.size, 0);
    rbb_free(&rb);
}


/* test_7 is created by Claude Code!

   ---- helpers for test_7 (macros only, no extra functions) ----
   The ring is driven side by side with a plain flat-array deque `ref`
   holding the same bytes, so every pop is compared byte-for-byte and
   every op re-checks the struct invariants. Pushed bytes come from a
   running counter, so any ordering / offset mistake shows up. */
#define T7_REF_MAX   (64 * RBB_BUF_LEN_UNIT)
#define T7_NEEDED(n) ((n) == 0 ? RBB_BUF_LEN_UNIT  \
        : (((n) + RBB_BUF_LEN_UNIT - 1) / RBB_BUF_LEN_UNIT) * RBB_BUF_LEN_UNIT)

/* This is a macro implementing a linear congruential generator (LCG)
   — a classic fast pseudo-random number generator technique.

   comma operator:
   The whole thing is (expr1, expr2). In C, the comma operator
   evaluates expr1, discards its value, then evaluates and returns expr2.

   unsigned is always unsigned int! */
#define T7_RAND(s)   ((s) = (s) * 6364136223846793005ULL + 1442695040888963407ULL,  \
                      (unsigned)((s) >> 33))

/* check rb invariants */
#define T7_INV()  \
    do {  \
        EQ(rb.capacity % RBB_BUF_LEN_UNIT, 0);  \
        EQ(rb.capacity >= RBB_BUF_LEN_UNIT, true);  \
        EQ(rb.size <= rb.capacity, true);  \
        EQ(rb.rp <= rb.capacity, true);  \
        EQ(rb.wp <= rb.capacity, true);  \
        EQ(rb.size, reflen);  \
        EQ(rbb_size(&rb), rb.size);  \
        EQ(rbb_capacity(&rb), rb.capacity);  \
        EQ(rbb_empty(&rb), rb.size == 0);  \
    } while(0)

#define T7_FILL(len)  \
    do {  \
        for(size_t i_=0; i_<(len); ++i_)  \
            src[i_] = (unsigned char)seq++;  \
    } while(0)

#define T7_PUSH_BACK(len)  \
    do {  \
        size_t l_ = (len);  \
        T7_FILL(l_);  \
        EQ(rbb_push_back(&rb, src, l_), true);  \
        memcpy(ref + reflen, src, l_);  \
        reflen += l_;  \
        T7_INV();  \
    } while(0)

#define T7_PUSH_FRONT(len)  \
    do {  \
        size_t l_ = (len);  \
        T7_FILL(l_);  \
        EQ(rbb_push_front(&rb, src, l_), true);  \
        memmove(ref + l_, ref, reflen);  \
        memcpy(ref, src, l_);  \
        reflen += l_;  \
        T7_INV();  \
    } while(0)

/* over-pop (len > size) must drain everything and reset rp/wp to 0 */
#define T7_POP_FRONT(len)  \
    do {  \
        size_t l_ = (len), e_ = l_ < reflen ? l_ : reflen;  \
        EQ(rbb_pop_front(&rb, dst, l_), e_);  \
        EQ(memcmp(dst, ref, e_), 0);  \
        memmove(ref, ref + e_, reflen - e_);  \
        reflen -= e_;  \
        if(l_ > e_) { EQ(rb.rp, 0); EQ(rb.wp, 0); }  \
        T7_INV();  \
    } while(0)

#define T7_POP_BACK(len)  \
    do {  \
        size_t l_ = (len), e_ = l_ < reflen ? l_ : reflen;  \
        EQ(rbb_pop_back(&rb, dst, l_), e_);  \
        EQ(memcmp(dst, ref + reflen - e_, e_), 0);  \
        reflen -= e_;  \
        if(l_ > e_) { EQ(rb.rp, 0); EQ(rb.wp, 0); }  \
        T7_INV();  \
    } while(0)


void test_7(){
    static unsigned char ref[T7_REF_MAX];
    static unsigned char src[T7_REF_MAX];
    static unsigned char dst[T7_REF_MAX];
    size_t reflen = 0;
    unsigned seq = 0;  /* unsigned int */
    const size_t U = RBB_BUF_LEN_UNIT;
    unsigned char sentinel = 0;
    unsigned char *oldbuf;
    ringbb rb;

    /* ---- init: capacity rounds up to unit multiples, not powers of two ---- */
    EQ(rbb_init(&rb, U), true);     EQ(rb.capacity, U);   rbb_free(&rb);
    EQ(rbb_init(&rb, U+1), true);   EQ(rb.capacity, 2*U); rbb_free(&rb);
    EQ(rbb_init(&rb, 3*U-1), true); EQ(rb.capacity, 3*U); rbb_free(&rb);
    EQ(rbb_init(&rb, 3*U), true);   EQ(rb.capacity, 3*U); rbb_free(&rb);
    EQ(rbb_init(&rb, 3*U+1), true); EQ(rb.capacity, 4*U); rbb_free(&rb);

    /* ---- init: over-max is rejected before touching the struct ---- */
    rb.buf = &sentinel;
    rb.capacity = rb.size = rb.rp = rb.wp = 77;
    EQ(rbb_init(&rb, RBB_BUF_MAX_SIZE + 1), false);
    EQ(rbb_init(&rb, SIZE_MAX), false);
    EQ(rb.buf == &sentinel, true);
    EQ(rb.capacity, 77); EQ(rb.size, 77); EQ(rb.rp, 77); EQ(rb.wp, 77);

    /* ---- free: zeroes everything, double free is harmless ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(1);
    rbb_free(&rb);
    EQ(rb.buf == NULL, true);
    EQ(rb.capacity, 0); EQ(rb.size, 0); EQ(rb.rp, 0); EQ(rb.wp, 0);
    rbb_free(&rb);
    reflen = 0;

    /* ---- zero-length ops: no-ops, never dereference the pointer ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(3);
    oldbuf = rb.buf;
    EQ(rbb_push_back(&rb, NULL, 0), true);
    EQ(rbb_push_front(&rb, NULL, 0), true);
    EQ(rbb_pop_back(&rb, NULL, 0), 0);
    EQ(rbb_pop_front(&rb, NULL, 0), 0);
    EQ(rb.buf == oldbuf, true);
    EQ(rb.size, 3); EQ(rb.rp, 0); EQ(rb.wp, 3);
    EQ(rbb_empty(&rb), false);
    T7_INV();

    /* ---- clear: resets indices, keeps the allocation ---- */
    rbb_clear(&rb);
    reflen = 0;
    EQ(rb.buf == oldbuf, true);
    EQ(rb.capacity, U); EQ(rb.size, 0); EQ(rb.rp, 0); EQ(rb.wp, 0);
    EQ(rbb_empty(&rb), true);
    T7_POP_FRONT(1);                /* pop from empty */
    T7_POP_BACK(1);
    T7_PUSH_FRONT(5);               /* still usable after clear */
    T7_POP_BACK(5);
    rbb_free(&rb);

    /* ---- exact fit (len == capacity-size) then +1 byte doubles ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(U);
    EQ(rb.capacity, U); EQ(rb.rp, 0); EQ(rb.wp, U);
    oldbuf = rb.buf;
    T7_PUSH_BACK(1);
    EQ(rb.capacity, 2*U); EQ(rb.rp, 0); EQ(rb.wp, U+1);
    EQ(rb.buf != oldbuf, true);
    T7_POP_FRONT(U+1);              /* len == size: normal path, not pop_all */
    EQ(rb.size, 0); EQ(rb.rp, U+1); EQ(rb.wp, U+1);
    rbb_free(&rb);

    /* ---- full & wrapped with rp == wp != 0, then grow from it ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(10);
    T7_POP_FRONT(10);
    EQ(rb.rp, 10); EQ(rb.wp, 10);
    T7_PUSH_BACK(U);                /* split write fills it exactly */
    EQ(rb.capacity, U); EQ(rb.size, U); EQ(rb.rp, 10); EQ(rb.wp, 10);
    T7_PUSH_FRONT(1);               /* recap must linearize wrapped data */
    EQ(rb.capacity, 2*U); EQ(rb.rp, 0); EQ(rb.wp, U+1);
    T7_POP_BACK(U+2);               /* over-pop of everything */
    rbb_free(&rb);

    /* ---- rp == wp == capacity (empty): both pointers sit one past end ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(U);
    T7_POP_FRONT(U);
    EQ(rb.rp, U); EQ(rb.wp, U);
    T7_PUSH_FRONT(5);               /* rp == cap: plain backward write */
    EQ(rb.rp, U-5); EQ(rb.wp, U);
    T7_POP_BACK(5);                 /* wp == cap: plain backward read */
    EQ(rb.rp, U-5); EQ(rb.wp, U-5);
    rbb_free(&rb);

    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(U);
    T7_POP_FRONT(U);
    T7_PUSH_BACK(3);                /* wp == cap: zero-length first chunk */
    EQ(rb.rp, U); EQ(rb.wp, 3);
    T7_POP_FRONT(3);                /* rp == cap: zero-length first chunk */
    EQ(rb.rp, 3); EQ(rb.wp, 3);
    rbb_free(&rb);

    /* ---- rp == wp == capacity (full), then grow from it ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(U);
    T7_POP_FRONT(U);
    T7_PUSH_BACK(U);
    EQ(rb.capacity, U); EQ(rb.size, U); EQ(rb.rp, U); EQ(rb.wp, U);
    T7_PUSH_BACK(1);
    EQ(rb.capacity, 2*U); EQ(rb.rp, 0); EQ(rb.wp, U+1);
    T7_POP_FRONT(U+1);
    rbb_free(&rb);

    /* ---- wp == 0 while size > 0 ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(10);
    T7_PUSH_FRONT(5);               /* rp == 0: whole chunk lands at the end */
    EQ(rb.rp, U-5); EQ(rb.wp, 10);
    T7_POP_BACK(10);
    EQ(rb.wp, 0); EQ(rb.size, 5);
    oldbuf = rb.buf;
    EQ(rbb_shrink(&rb), true);      /* already minimal: no realloc, no relayout */
    EQ(rb.buf == oldbuf, true); EQ(rb.rp, U-5); EQ(rb.wp, 0);
    T7_PUSH_BACK(U);                /* recap from the wp == 0 state */
    EQ(rb.capacity, 2*U); EQ(rb.rp, 0); EQ(rb.wp, U+5);
    T7_POP_FRONT(U+5);
    rbb_free(&rb);

    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(10);
    T7_PUSH_FRONT(5);
    T7_POP_BACK(10);
    T7_POP_BACK(5);                 /* wp == 0 < len: backward wrap read */
    EQ(rb.wp, U-5); EQ(rb.rp, U-5);
    rbb_free(&rb);

    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(10);
    T7_PUSH_FRONT(5);
    T7_POP_BACK(10);
    T7_POP_FRONT(100);              /* over-pop from the wp == 0 state */
    rbb_free(&rb);

    /* ---- split (two-chunk) reads and writes in all four directions ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(3);
    T7_POP_FRONT(3);                /* rp = wp = 3 */
    T7_PUSH_FRONT(10);              /* 0 < rp < len: 7 at the end, 3 at the front */
    EQ(rb.rp, U-7); EQ(rb.wp, 3);
    T7_POP_FRONT(4);                /* stays within the tail chunk */
    EQ(rb.rp, U-3);
    T7_POP_FRONT(6);                /* crosses the end */
    EQ(rb.rp, 3); EQ(rb.size, 0);
    T7_PUSH_FRONT(10);
    T7_POP_BACK(5);                 /* 0 < wp < len: crosses the start backwards */
    EQ(rb.wp, U-2);
    T7_POP_BACK(5);
    EQ(rb.wp, U-7); EQ(rb.rp, U-7);
    T7_PUSH_BACK(10);               /* wp + len > cap: split forward write */
    EQ(rb.wp, 3);
    T7_POP_FRONT(10);               /* split forward read */
    EQ(rb.rp, 3); EQ(rb.size, 0);
    T7_PUSH_FRONT(3);               /* rp == len: lands exactly at 0, no wrap */
    EQ(rb.rp, 0); EQ(rb.wp, 3);
    T7_POP_FRONT(3);
    EQ(rb.rp, 3); EQ(rb.size, 0);

    /* pops into exactly-sized buffers: not one byte more may be written */
    T7_PUSH_FRONT(10);
    {
        unsigned char t6[6], t4[4];
        EQ(rbb_pop_back(&rb, t6, 6), 6);
        EQ(memcmp(t6, ref + reflen - 6, 6), 0);
        reflen -= 6;
        T7_INV();
        EQ(rbb_pop_front(&rb, t4, 4), 4);
        EQ(memcmp(t4, ref, 4), 0);
        reflen -= 4;
        T7_INV();
        EQ(rb.size, 0);
    }
    rbb_free(&rb);

    /* ---- over-max pushes are rejected and leave everything untouched ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(3);
    T7_POP_FRONT(3);
    T7_PUSH_FRONT(10);              /* wrapped state */
    oldbuf = rb.buf;
    EQ(rbb_push_back(&rb, src, SIZE_MAX), false);
    EQ(rbb_push_back(&rb, src, RBB_BUF_MAX_SIZE + 1), false);
    EQ(rbb_push_back(&rb, src, RBB_BUF_MAX_SIZE - rb.size + 1), false);
    EQ(rbb_push_front(&rb, src, SIZE_MAX), false);
    EQ(rbb_push_front(&rb, src, RBB_BUF_MAX_SIZE + 1), false);
    EQ(rbb_push_front(&rb, src, RBB_BUF_MAX_SIZE - rb.size + 1), false);
    EQ(rb.buf == oldbuf, true);
    EQ(rb.capacity, U); EQ(rb.size, 10); EQ(rb.rp, U-7); EQ(rb.wp, 3);
    T7_INV();
    T7_POP_FRONT(10);               /* data still intact */
    rbb_free(&rb);

    /* ---- growth policy: fit-to-unit when big, doubling otherwise ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_BACK(10*U);             /* needed(10U) > 2U: grows to fit */
    EQ(rb.capacity, 10*U); EQ(rb.rp, 0); EQ(rb.wp, 10*U);
    T7_PUSH_BACK(1);                /* 2*10U > needed(10U+1): doubles */
    EQ(rb.capacity, 20*U);
    T7_PUSH_BACK(20*U);             /* needed 31U, doubled 40U wins */
    EQ(rb.capacity, 40*U);

    /* ---- shrink: to unit multiple, idempotent, linearizes wrapped data ---- */
    EQ(rbb_shrink(&rb), true);
    EQ(rb.capacity, 31*U); EQ(rb.rp, 0); EQ(rb.wp, rb.size);
    T7_INV();
    oldbuf = rb.buf;
    EQ(rbb_shrink(&rb), true);      /* nothing to do: no realloc */
    EQ(rb.capacity, 31*U); EQ(rb.buf == oldbuf, true);
    T7_POP_FRONT(1);                /* size is now exactly 30U */
    EQ(rbb_shrink(&rb), true);
    EQ(rb.capacity, 30*U); EQ(rb.rp, 0); EQ(rb.wp, 30*U);
    T7_POP_FRONT(29*U);
    T7_POP_BACK(1);
    EQ(rb.rp, 29*U); EQ(rb.wp, 30*U-1);
    T7_PUSH_BACK(U);                /* wraps: 1 byte at the end, rest at the front */
    EQ(rb.rp, 29*U); EQ(rb.wp, U-1); EQ(rb.size, 2*U-1);
    EQ(rbb_shrink(&rb), true);
    EQ(rb.capacity, 2*U); EQ(rb.rp, 0); EQ(rb.wp, 2*U-1);
    T7_POP_FRONT(2*U-1);
    EQ(rb.size, 0);
    EQ(rbb_shrink(&rb), true);      /* empty: back to one unit */
    EQ(rb.capacity, U); EQ(rb.rp, 0); EQ(rb.wp, 0);
    rbb_free(&rb);

    /* ---- growth via push_front, then clear + shrink ---- */
    EQ(rbb_init(&rb, 0), true);
    T7_PUSH_FRONT(U+1);
    EQ(rb.capacity, 2*U); EQ(rb.rp, 0); EQ(rb.wp, U+1);
    T7_PUSH_FRONT(U);               /* needed 3U, doubled 4U wins */
    EQ(rb.capacity, 4*U); EQ(rb.rp, 0); EQ(rb.wp, 2*U+1);
    rbb_clear(&rb);
    reflen = 0;
    EQ(rb.capacity, 4*U);
    EQ(rbb_shrink(&rb), true);
    EQ(rb.capacity, U);
    rbb_free(&rb);

    /* ---- randomized differential test against the reference deque ---- */
    EQ(rbb_init(&rb, 0), true);
    {
        unsigned long long s = 0x9E3779B97F4A7C15ULL;
        for(int it=0; it<40000; ++it)
        {
            unsigned op = T7_RAND(s) % 16;
            unsigned r1 = T7_RAND(s);
            size_t len = (r1 % 4 == 0) ? (size_t)(T7_RAND(s) % 16)
                                       : (size_t)(T7_RAND(s) % (3*U + 1));
            if(op < 7)
            {
                if(len > T7_REF_MAX - reflen)
                    len = T7_REF_MAX - reflen;
                if(op % 2)
                    T7_PUSH_BACK(len);
                else
                    T7_PUSH_FRONT(len);
            } else if(op < 14)
            {
                if(op % 2)
                    T7_POP_FRONT(len);
                else
                    T7_POP_BACK(len);
            } else if(op == 14)
            {
                size_t oldcap = rb.capacity;
                oldbuf = rb.buf;
                EQ(rbb_shrink(&rb), true);
                EQ(rb.capacity, T7_NEEDED(reflen));
                if(rb.capacity != oldcap)
                {   /* reallocated: data was linearized */
                    EQ(rb.rp, 0); EQ(rb.wp, rb.size);
                } else
                    EQ(rb.buf == oldbuf, true);   /* no needless realloc */
                T7_INV();
            } else if((r1 & 0x70) == 0)
            {
                rbb_clear(&rb);
                reflen = 0;
                T7_INV();
            }
        }
    }
    T7_POP_FRONT(reflen);
    EQ(rb.size, 0);
    EQ(rbb_shrink(&rb), true);
    EQ(rb.capacity, U);
    rbb_free(&rb);
}


int main(){
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    printf("All Pass!\n");
    return 0;
}

