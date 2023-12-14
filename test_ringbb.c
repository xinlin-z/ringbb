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
    rbb_push_back(&rb, m1, strlen(m1));
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, strlen(m1));
    char p1[16] = {};
    size_t r = rbb_pop_front(&rb, p1, 16);
    HOPE_EQ(r, 10);
    HOPE_EQ(memcmp(m1,p1,10), 0);
    r = rbb_pop_front(&rb, p1, 16);
    HOPE_EQ(r, 0);
    r = rbb_pop_front(&rb, p1, 16);
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


int main(){
    test_1();
    test_2();
    test_3();
    printf("Done! (No FAILED means pass)\n");
    return 0;
}

