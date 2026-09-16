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


int main(){
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    printf("All Done! (No FAILED means pass)\n");
    return 0;
}

