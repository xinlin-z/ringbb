/*
 * Show that libringbb.so can also be used in C++.
 * */
#include "ring_byte_buf.h"
#include <cstring>


#define HOPE_EQ(a,b) \
    do{\
        if((a) != (b))\
            fprintf(stderr, "HOPE_EQ FAILED %s:%d\n", __FILE__, __LINE__);\
    }while(0)


int main(){
    // the same with test_1() in test_ringbb.c
    ringbb rb;
    rbb_init(&rb, 10);
    HOPE_EQ(rb.capacity, 64);
    HOPE_EQ(rb.size, 0);
    const char *m1 = "0123456789";
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
    printf("Done!\n");
}
