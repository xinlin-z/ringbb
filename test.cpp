#include "ring_byte_buf.hpp"
#include <iostream>
#include <thread>
#include <cmath>
#include <cstring>
#include <string>
using namespace std;


#define HOPE_EQ(a,b) do{\
      if(a != b)\
          std::cerr<<"HOPE_EQ FAILED "<<__FILE__<<": "<<__LINE__<<std::endl;\
    }while(0)


#define HOPE_LE(a,b) do{\
      if(a > b)\
          std::cerr<<"HOPE_LE FAILED "<<__FILE__<<": "<<__LINE__<<std::endl;\
    }while(0)


void test_empty(){
    ring_byte_buf rb{0};
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
    rb.shrink();
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
    rb.reset();
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
    rb.clear();
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
    std::byte buf[16] {};
    HOPE_EQ(rb.pop_front(buf,16), 0);
    HOPE_EQ(rb.pop_back(buf,16), 0);
    rb.push_back(buf, 0);
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
}


void test_push_back_1(){
    ring_byte_buf rb{0};
    rb.push_back("", 0);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
    HOPE_EQ(rb.capacity(), 0);
    rb.push_back("1234", 4);
    HOPE_EQ(rb.size(), 4);
    HOPE_EQ(rb.empty(), false);
    HOPE_EQ(rb.capacity(), 4);
    rb.push_back('5');
    HOPE_EQ(rb.size(), 5);
    HOPE_EQ(rb.capacity(), 8);
    rb.push_back("67890", 3);
    HOPE_EQ(rb.size(), 8);
    HOPE_EQ(rb.capacity(), 8);
    rb.push_back("90", 0);
    HOPE_EQ(rb.size(), 8);
    HOPE_EQ(rb.capacity(), 8);
    rb.push_back('9');
    HOPE_EQ(rb.size(), 9);
    HOPE_EQ(rb.capacity(), 16);
    rb.push_back("0", 1);
    HOPE_EQ(rb.size(), 10);
    HOPE_EQ(rb.capacity(), 16);
    rb.shrink();
    HOPE_EQ(rb.size(), 10);
    HOPE_EQ(rb.capacity(), 16);
    rb.clear();
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.capacity(), 16);
    HOPE_EQ(rb.empty(), true);
    rb.reset();
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.empty(), true);
}


void test_push_back_2(){
    ring_byte_buf rb{0};
    string s1 {};
    rb.push_back(s1);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.empty(), true);
    string s2 {""};
    rb.push_back(s2);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.empty(), true);
    string s3 {"12345"};
    rb.push_back(s3);
    HOPE_EQ(rb.size(), 5);
    HOPE_EQ(rb.capacity(), 8);
    HOPE_EQ(rb.empty(), false);
}


void test_push_back_3(){
    ring_byte_buf rb{0};
    const char *s1 {""};
    rb.push_back(s1);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.empty(), true);
    char s2[] {""};
    rb.push_back(s2);
    HOPE_EQ(rb.size(), 1);
    HOPE_EQ(rb.capacity(), 1);
    HOPE_EQ(rb.empty(), false);
    char s3[] {"123"};
    rb.push_back(s3);
    HOPE_EQ(rb.size(), 5);
    HOPE_EQ(rb.capacity(), 8);
    char s4[] {'a','\0'};
    rb.push_back(s4);
    HOPE_EQ(rb.size(), 7);
    HOPE_EQ(rb.capacity(), 8);
}


void test_pop_front_1(){
    ring_byte_buf rb{0};
    rb.push_back("12345", 5);
    HOPE_EQ(rb.size(), 5);
    HOPE_EQ(rb.capacity(), 8);
    int n;
    char x;
    n = rb.pop_front(&x, 1);
    HOPE_EQ(n, 1);
    HOPE_EQ(x, '1');
    n = rb.pop_front(&x, 1);
    HOPE_EQ(n, 1);
    HOPE_EQ(x, '2');
    n = rb.pop_front(&x, 1);
    HOPE_EQ(n, 1);
    HOPE_EQ(x, '3');
    rb.shrink();
    HOPE_EQ(rb.size(), 2);
    HOPE_EQ(rb.capacity(), 2);
    n = rb.pop_front(&x, 1);
    HOPE_EQ(n, 1);
    HOPE_EQ(x, '4');
    n = rb.pop_front(&x, 1);
    HOPE_EQ(n, 1);
    HOPE_EQ(x, '5');
    n = rb.pop_front(&x, 1);
    HOPE_EQ(n, 0);
    HOPE_EQ(x, '5');
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.capacity(), 2);
    rb.shrink();
    HOPE_EQ(rb.capacity(), 0);
}


void test_pop_front_2(){
    ring_byte_buf rb;
    rb.push_back("abcdefg", 7);
    int n;
    char c2[2] {};
    n = rb.pop_front(c2, 2);
    HOPE_EQ(n, 2);
    HOPE_EQ(memcmp(c2,"ab",2), 0);
    n = rb.pop_front(c2, 2);
    HOPE_EQ(n, 2);
    HOPE_EQ(memcmp(c2,"cd",2), 0);
    n = rb.pop_front(c2, 2);
    HOPE_EQ(n, 2);
    HOPE_EQ(memcmp(c2,"ef",2), 0);
    n = rb.pop_front(c2, 2);
    HOPE_EQ(n, 1);
    HOPE_EQ(memcmp(c2,"gf",2), 0);
}


void test_push_front_pop_back(){
    ring_byte_buf rb;
    rb.push_front("1234567", 7);
    int n;
    char c2[2] {};
    n = rb.pop_back(c2, 2);
    HOPE_EQ(n, 2);
    HOPE_EQ(memcmp(c2,"67",2), 0);
    n = rb.pop_back(c2, 2);
    HOPE_EQ(n, 2);
    HOPE_EQ(memcmp(c2,"45",2), 0);
    n = rb.pop_back(c2, 2);
    HOPE_EQ(n, 2);
    HOPE_EQ(memcmp(c2,"23",2), 0);
    n = rb.pop_back(c2, 2);
    HOPE_EQ(n, 1);
    HOPE_EQ(memcmp(c2,"13",2), 0);
}


void test_stress_1(){
    ring_byte_buf rb;
    uint8_t b[4] {};

    for(int i=0; i<1024*1024+1; ++i)
        rb.push_back('9');
    HOPE_EQ(rb.capacity(), 1024*1024*2);
    HOPE_EQ(rb.size(), 1024*1024+1);
    for(int i=0; i<1024*1024+1; ++i){
        rb.pop_front(b, 1);
        HOPE_EQ(b[0], '9');
    }
    HOPE_EQ(rb.capacity(), 1024*1024*2);
    HOPE_EQ(rb.size(), 0);

    string a {"90"};
    for(int i=0; i<1024*1024+1; ++i)
        rb.push_back(a);
    HOPE_EQ(rb.capacity(), 1024*1024*4);
    HOPE_EQ(rb.size(), (1024*1024+1)*2);
    for(int i=0; i<1024*1024+1; ++i){
        HOPE_EQ(rb.pop_front(b, 2), 2);
        HOPE_EQ(memcmp(b,"90",2), 0);
    }
    HOPE_EQ(rb.capacity(), 1024*1024*4);
    HOPE_EQ(rb.size(), 0);

    rb.reset();
    HOPE_EQ(rb.capacity(), 0);
}


void test_stress_2(){
    ring_byte_buf rb;
    uint8_t b[4] {};

    for(int i=0; i<1024*1024+1; ++i)
        rb.push_front('9');
    HOPE_EQ(rb.capacity(), 1024*1024*2);
    HOPE_EQ(rb.size(), 1024*1024+1);
    for(int i=0; i<1024*1024+1; ++i){
        rb.pop_back(b, 1);
        HOPE_EQ(b[0], '9');
    }
    HOPE_EQ(rb.capacity(), 1024*1024*2);
    HOPE_EQ(rb.size(), 0);

    string a {"90"};
    for(int i=0; i<1024*1024+1; ++i)
        rb.push_front(a);
    HOPE_EQ(rb.capacity(), 1024*1024*4);
    HOPE_EQ(rb.size(), (1024*1024+1)*2);
    for(int i=0; i<1024*1024+1; ++i){
        HOPE_EQ(rb.pop_back(b, 2), 2);
        HOPE_EQ(memcmp(b,"90",2), 0);
    }
    HOPE_EQ(rb.capacity(), 1024*1024*4);
    HOPE_EQ(rb.size(), 0);

    rb.reset();
    HOPE_EQ(rb.capacity(), 0);
}


void test_stress_3(){
    ring_byte_buf rb{0};
    std::byte a[10] {};
    int n;

    for(int i=0; i<10000; ++i){
        rb.push_front("345");
        rb.push_back("67890");
        rb.push_front("12");
        n = rb.pop_front(a, 4);
        HOPE_EQ(n, 4);
        n = rb.pop_back(a+4, 6);
        HOPE_EQ(n, 6);
        HOPE_EQ(memcmp(a,"1234567890",10), 0);
        HOPE_EQ(rb.capacity(), 16);
        HOPE_EQ(rb.size(), 0);
        rb.reset();
    }
}


template<typename T>
void write_rb(ring_byte_buf<T> &rb){
    for(int i=0; i<1024*1024; ++i)
        rb.push_back("0123");
}


template<typename T>
void read_rb(ring_byte_buf<T> &rb){
    unsigned char cont[4];
    for(int i=0; i<1024*1024; ++i)
        while(rb.pop_front(cont, 4) == 0);
}


void test_thread_safe_1(){
    ring_byte_buf rb;
    thread wt1(write_rb<decltype(rb)::value_type>, ref(rb));
    thread wt2(write_rb<decltype(rb)::value_type>, ref(rb));
    wt1.join();
    wt2.join();
    HOPE_EQ(rb.capacity(), (size_t)pow(2,23));
    rb.reset();
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
}


void test_thread_safe_2(){
    ring_byte_buf rb;
    thread wt1(write_rb<decltype(rb)::value_type>, ref(rb));
    thread rt1(read_rb<decltype(rb)::value_type>,  ref(rb));
    thread wt2(write_rb<decltype(rb)::value_type>, ref(rb));
    thread rt2(read_rb<decltype(rb)::value_type>,  ref(rb));
    wt1.join();
    rt1.join();
    wt2.join();
    rt2.join();
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
    HOPE_LE(rb.capacity(), (size_t)pow(2,23));
}


template<typename T>
void write_rb_front(ring_byte_buf<T> &rb){
    for(int i=0; i<1024*1024; ++i)
        rb.push_front("0123");
}


template<typename T>
void read_rb_back(ring_byte_buf<T> &rb){
    unsigned char cont[4];
    for(int i=0; i<1024*1024; ++i)
        while(rb.pop_back(cont, 4) == 0);
}


void test_thread_safe_3(){
    ring_byte_buf rb;
    thread wt1(write_rb_front<decltype(rb)::value_type>, ref(rb));
    thread wt2(write_rb_front<decltype(rb)::value_type>, ref(rb));
    wt1.join();
    wt2.join();
    HOPE_EQ(rb.capacity(), (size_t)pow(2,23));
    rb.reset();
    HOPE_EQ(rb.capacity(), 0);
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
}


void test_thread_safe_4(){
    ring_byte_buf rb;
    thread wt1(write_rb_front<decltype(rb)::value_type>, ref(rb));
    thread rt1(read_rb_back<decltype(rb)::value_type>,  ref(rb));
    thread wt2(write_rb_front<decltype(rb)::value_type>, ref(rb));
    thread rt2(read_rb_back<decltype(rb)::value_type>,  ref(rb));
    wt1.join();
    rt1.join();
    wt2.join();
    rt2.join();
    HOPE_EQ(rb.size(), 0);
    HOPE_EQ(rb.empty(), true);
    HOPE_LE(rb.capacity(), (size_t)pow(2,23));
}


int main() {
    test_empty();
    test_push_back_1();
    test_push_back_2();
    test_push_back_3();
    test_pop_front_1();
    test_pop_front_2();
    test_push_front_pop_back();
    test_stress_1();
    test_stress_2();
    test_stress_3();
    test_thread_safe_1();
    test_thread_safe_2();
    test_thread_safe_3();
    test_thread_safe_4();
    std::cout << "Test Done!" << std::endl;
    return 0;
}

