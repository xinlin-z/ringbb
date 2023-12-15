#include "ring_byte_buf.h"
#include <stdio.h>
#include <string.h>


#define HOPE_EQ(a,b) \
    do{\
        if((a) != (b))\
            fprintf(stderr, "HOPE_EQ FAILED %s:%d\n", __FILE__, __LINE__);\
    }while(0)


void test_1(){
    ringbb rb;
    rbb_init(&rb, 10);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 0);
    char *m1 = "0123456789";
    rbb_push_back(&rb, m1, 10);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 10);
    char p1[16] = {};
    size_t r = rbb_pop_front(&rb, p1, 16);
    HOPE_EQ(r, 10);
    HOPE_EQ(memcmp(m1,p1,10), 0);
    r = rbb_pop_front(&rb, p1, 16);
    HOPE_EQ(r, 0);
    r = rbb_pop_front(&rb, p1, 0);
    HOPE_EQ(r, 0);
    rbb_free(&rb);
}


void test_11(){
    ringbb rb;
    rbb_init(&rb, 10);
    char *m1 = "0123456789";
    rbb_push_front(&rb, m1, 10);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 10);
    char p1[16] = {};
    size_t r = rbb_pop_back(&rb, p1, 16);
    HOPE_EQ(r, 10);
    HOPE_EQ(memcmp(m1,p1,10), 0);
    r = rbb_pop_back(&rb, p1, 16);
    HOPE_EQ(r, 0);
    r = rbb_pop_back(&rb, p1, 0);
    HOPE_EQ(r, 0);
    rbb_free(&rb);
}

void test_2(){
    ringbb rb;
    rbb_init(&rb, 0);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 0);
    char *m1 = "0123456789";
    rbb_push_back(&rb, m1, 0);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 0);
    rbb_push_back(&rb, m1, 1);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 1);
    rbb_push_back(&rb, m1, 1);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 2);
    rbb_push_back(&rb, m1, 1);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 3);
    rbb_free(&rb);

    rbb_init(&rb, 0);
    for(int i=0; i<10; ++i)
        rbb_push_back(&rb, m1, 10);
    HOPE_EQ(rb.capacity, 128);
    HOPE_EQ(rb.size, 100);
    for(int i=0; i<10; ++i)
        rbb_push_back(&rb, m1, 10);
    HOPE_EQ(rb.capacity, 256);
    HOPE_EQ(rb.size, 200);
    for(int i=0; i<10; ++i)
        rbb_push_back(&rb, m1, 10);
    HOPE_EQ(rb.capacity, 512);
    HOPE_EQ(rb.size, 300);
    rbb_free(&rb);
}


void test_21(){
    ringbb rb;
    rbb_init(&rb, 0);
    char *m1 = "0123456789";
    rbb_push_front(&rb, m1, 0);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 0);
    rbb_push_front(&rb, m1, 1);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 1);
    rbb_push_front(&rb, m1, 1);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 2);
    rbb_push_front(&rb, m1, 1);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 3);
    rbb_free(&rb);

    rbb_init(&rb, 0);
    for(int i=0; i<10; ++i)
        rbb_push_front(&rb, m1, 10);
    HOPE_EQ(rb.capacity, 128);
    HOPE_EQ(rb.size, 100);
    for(int i=0; i<10; ++i)
        rbb_push_front(&rb, m1, 10);
    HOPE_EQ(rb.capacity, 256);
    HOPE_EQ(rb.size, 200);
    for(int i=0; i<10; ++i)
        rbb_push_front(&rb, m1, 10);
    HOPE_EQ(rb.capacity, 512);
    HOPE_EQ(rb.size, 300);
    rbb_free(&rb);
}


void test_3(){
    ringbb rb;
    rbb_init(&rb, 100);
    HOPE_EQ(rb.capacity, 128);
    char c;
    for(size_t i=0; i<128; ++i){
        c = (char)i;
        rbb_push_back(&rb, &c, 1);
        HOPE_EQ(rb.size, i+1);
    }
    HOPE_EQ(rb.capacity, 128);
    size_t r;
    for(size_t i=0; i<128; ++i){
        r = rbb_pop_front(&rb, &c, 1);
        HOPE_EQ(r, 1);
        HOPE_EQ(c, (char)i);
        HOPE_EQ(rb.size, 128-i-1);
    }
    HOPE_EQ(rb.capacity, 128);
    r = rbb_pop_front(&rb, &c, 1);
    HOPE_EQ(r, 0);
    r = rbb_pop_front(&rb, &c, 0);
    HOPE_EQ(r, 0);
    rbb_free(&rb);
}


void test_31(){
    ringbb rb;
    rbb_init(&rb, 100);
    char c;
    for(size_t i=0; i<128; ++i){
        c = (char)i;
        rbb_push_front(&rb, &c, 1);
        HOPE_EQ(rb.size, i+1);
    }
    HOPE_EQ(rb.capacity, 128);
    size_t r;
    for(size_t i=0; i<128; ++i){
        r = rbb_pop_back(&rb, &c, 1);
        HOPE_EQ(r, 1);
        HOPE_EQ(c, (char)i);
        HOPE_EQ(rb.size, 128-i-1);
    }
    HOPE_EQ(rb.capacity, 128);
    r = rbb_pop_back(&rb, &c, 1);
    HOPE_EQ(r, 0);
    r = rbb_pop_back(&rb, &c, 0);
    HOPE_EQ(r, 0);
    rbb_free(&rb);
}


void test_4(){
    ringbb rb;
    rbb_init(&rb, 64);
    char *m1 = "0123456789";
    char c10[10] = {};
    for(int i=0; i<1000; ++i){
        rbb_push_back(&rb, m1, 10);
        HOPE_EQ(rbb_pop_front(&rb,&c10,10), 10);
        HOPE_EQ(memcmp(m1,c10,10), 0);
        HOPE_EQ(rbb_pop_front(&rb,&c10,10), 0);
        rbb_push_front(&rb, m1, 10);
        HOPE_EQ(rbb_pop_back(&rb,&c10,10), 10);
        HOPE_EQ(memcmp(m1,c10,10), 0);
        HOPE_EQ(rbb_pop_back(&rb,&c10,10), 0);
        rbb_push_back(&rb, m1, 10);
        HOPE_EQ(rbb_pop_back(&rb,&c10,10), 10);
        HOPE_EQ(memcmp(m1,c10,10), 0);
        HOPE_EQ(rbb_pop_back(&rb,&c10,10), 0);
        rbb_push_front(&rb, m1, 10);
        HOPE_EQ(rbb_pop_front(&rb,&c10,10), 10);
        HOPE_EQ(memcmp(m1,c10,10), 0);
        HOPE_EQ(rbb_pop_front(&rb,&c10,10), 0);
    }
    rbb_free(&rb);
}


void test_41(){
    ringbb rb;
    rbb_init(&rb, 64);
    char *m1 = "0123456789";
    char c10[10] = {};
    for(int i=0; i<1000; ++i)
        rbb_push_back(&rb, m1, 10);
    for(int i=0; i<1000; ++i){
        HOPE_EQ(rbb_pop_front(&rb,&c10,10), 10);
        HOPE_EQ(memcmp(m1,c10,10), 0);
    }
    HOPE_EQ(rb.size, 0);
    for(int i=0; i<1000; ++i)
        rbb_push_front(&rb, m1, 10);
    for(int i=0; i<1000; ++i){
        HOPE_EQ(rbb_pop_back(&rb,&c10,10), 10);
        HOPE_EQ(memcmp(m1,c10,10), 0);
    }
    HOPE_EQ(rb.size, 0);
    rbb_free(&rb);
}


void test_42(){
    ringbb rb;
    rbb_init(&rb, 64);
    char *m1 = "0123456";
    char c10[10] = {};
    for(int i=0; i<1000; ++i){
        rbb_push_back(&rb, m1, 7);
        rbb_shrink(&rb);
    }
    for(int i=0; i<500; ++i)
        HOPE_EQ(rbb_pop_back(&rb, c10, 10), 10);
    HOPE_EQ(rb.size, 2000);
    HOPE_EQ(rb.capacity, 8192);
    rbb_shrink(&rb);
    HOPE_EQ(rb.capacity, 2048);
    rbb_free(&rb);
}


void test_5(){
    ringbb rb;
    rbb_init(&rb, 64);
    char *m1 = "0123456789";
    char c10[10] = {};
    for(int i=0; i<1000; ++i)
        rbb_push_back(&rb, m1, 10);
    HOPE_EQ(rb.size, 10000);
    for(int i=0; i<1000; ++i){
        rbb_pop_front(&rb, c10, 10);
        HOPE_EQ(memcmp(m1,c10,10), 0);
    }
    HOPE_EQ(rb.size, 0);
    for(int i=0; i<1000; ++i)
        rbb_push_front(&rb, m1, 10);
    HOPE_EQ(rb.size, 10000);
    for(int i=0; i<1000; ++i){
        rbb_pop_back(&rb, c10, 10);
        HOPE_EQ(memcmp(m1,c10,10), 0);
    }
    HOPE_EQ(rb.size, 0);
    rbb_free(&rb);
}


void test_6(){
    ringbb rb;
    rbb_init(&rb, 0);
    rbb_shrink(&rb);
    HOPE_EQ(rb.capacity, 0);
    HOPE_EQ(rb.size, 0);
    HOPE_EQ(rbb_empty(&rb), true);
    rbb_free(&rb);
}


int main(){
    test_1();
    test_11();
    test_2();
    test_21();
    test_3();
    test_31();
    test_4();
    test_41();
    test_42();
    test_5();
    test_6();
    printf("Done! (No FAILED means pass)\n");
    return 0;
}

