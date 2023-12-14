#include "ring_byte_buf.h"
#include <stdio.h>


#define HOPE_EQ(a,b) \
    do{\
        if(a != b)\
            fprintf(stderr, "HOPE_EQ FAILED %s:%d\n", __FILE__, __LINE__);\
    }while(0)


int main(){
    ringbb rb;
    rbb_init(&rb, 16);
    HOPE_EQ(rb.capacity, 16);
    HOPE_EQ(rb.size, 0);
    rbb_free(&rb);

    printf("Done!\n");
    return 0;
}
