#include "ring_byte_buf.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <cmath>
#include <cstring>
#include <string>
using namespace std;


#define HOPE_EQ(a,b) do{\
      if(a != b)\
          std::cerr<<"HOPE_EQ FAILED "<<__FILE__<<": "<<__LINE__<<std::endl;\
    }while(0)


void test_empty(){
    ring_byte_buf rb;
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


void test_push_back(){
    ring_byte_buf rb;
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
    ring_byte_buf rb;
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
    ring_byte_buf rb;
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


TEST(ring_byte_buf, 0002){
    ring_byte_buf<char> rb{8};
    EXPECT_EQ(rb.empty(), true);
    EXPECT_EQ(rb.capacity(), 8);
    EXPECT_EQ(rb.size(), 0);
    string a {"12345678"};
    rb.push_back(a);
    rb.push_back(a);
    EXPECT_EQ(rb.capacity(), 16);
    EXPECT_EQ(rb.size(), 16);
    EXPECT_EQ(rb.empty(), false);
    byte b[9] {};
    rb.pop_front(b, 9);
    EXPECT_EQ(rb.capacity(), 16);
    EXPECT_EQ(rb.size(), 7);
    EXPECT_EQ(memcmp(b,"123456781",9), 0);
}


TEST(ring_byte_buf, 0003){
    ring_byte_buf<uint8_t> u8rb;
    for(int i=0; i<1024*1024+1; ++i)
        u8rb.push_back("9");
    EXPECT_EQ(u8rb.capacity(), 1024*1024*2);
    EXPECT_EQ(u8rb.size(), 1024*1024+1);
    uint8_t b[4] {};
    for(int i=0; i<1024*1024+1; ++i){
        u8rb.pop_front(b, 1);
        EXPECT_EQ(b[0], '9');
    }
    EXPECT_EQ(u8rb.capacity(), 1024*1024*2);
    EXPECT_EQ(u8rb.size(), 0);
    string a {"90"};
    for(int i=0; i<1024*1024+1; ++i)
        u8rb.push_back(a);
    EXPECT_EQ(u8rb.capacity(), 1024*1024*4);
    EXPECT_EQ(u8rb.size(), (1024*1024+1)*2);
    for(int i=0; i<1024*1024+1; ++i){
        EXPECT_EQ(u8rb.pop_front(b, 2), 2);
        EXPECT_STREQ((char*)b, "90");
    }
    EXPECT_EQ(u8rb.capacity(), 1024*1024*4);
    EXPECT_EQ(u8rb.size(), 0);
    u8rb.reset();
    EXPECT_EQ(u8rb.capacity(), 0);
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


TEST(ring_byte_buf, 0004){
    ring_byte_buf<char> crb;
    thread wt1(write_rb<decltype(crb)::value_type>, ref(crb));
    thread wt2(write_rb<decltype(crb)::value_type>, ref(crb));
    wt1.join();
    wt2.join();
    EXPECT_EQ(crb.capacity(), (size_t)pow(2,23));
    crb.reset();

    ring_byte_buf rb;
    thread wt3(write_rb<decltype(rb)::value_type>, ref(rb));
    thread rt1(read_rb<decltype(rb)::value_type>,  ref(rb));
    thread wt4(write_rb<decltype(rb)::value_type>, ref(rb));
    thread rt2(read_rb<decltype(rb)::value_type>,  ref(rb));
    wt3.join();
    rt1.join();
    wt4.join();
    rt2.join();
    EXPECT_EQ(rb.size(), 0);
    EXPECT_EQ(rb.empty(), true);
    EXPECT_LE(rb.capacity(), (size_t)pow(2,23));
}


TEST(ring_byte_buf, 0005){
    ring_byte_buf rb;

    char cp[] {'a','b','c','d'};
    rb.push_back(cp);
    EXPECT_EQ(rb.capacity(), 4);
    EXPECT_EQ(rb.size(), 4);
    rb.reset();

    char cc[] {'a','b','c','\0'};
    rb.push_back(cc);
    EXPECT_EQ(rb.capacity(), 4);
    EXPECT_EQ(rb.size(), 4);
}


TEST(ring_byte_buf, 0006){
    std::byte a[32];
    ring_byte_buf rb;
    rb.push_front("12345");
    EXPECT_EQ(rb.capacity(), 8);
    EXPECT_EQ(rb.size(), 5);
    rb.push_front('a');
    rb.push_front('b');
    rb.push_front('c');
    EXPECT_EQ(rb.capacity(), 8);
    EXPECT_EQ(rb.size(), 8);
    rb.push_front('d');
    EXPECT_EQ(rb.capacity(), 16);
    EXPECT_EQ(rb.size(), 9);
    rb.pop_front(a, 9);
    EXPECT_EQ(memcmp(a,"dcba12345",9), 0);
    std::string s {"12345678"};
    rb.push_front(s+s+s);
    EXPECT_EQ(rb.capacity(), 32);
    EXPECT_EQ(rb.size(), 24);
    rb.pop_front(a, 7);
    rb.pop_front(a, 8);
    rb.pop_front(a, 9);
    EXPECT_EQ(memcmp(a,"812345678",9), 0);
    EXPECT_EQ(rb.capacity(), 32);
    EXPECT_EQ(rb.size(), 0);
}

TEST(ring_byte_buf, 0007){
    std::byte a[9];
    ring_byte_buf rb;
    for(int i=0; i<1000; ++i){
        rb.push_front("345");
        rb.push_back("6789");
        rb.push_front("12");
        rb.pop_front(a, 9);
        EXPECT_EQ(memcmp(a,"123456789",9), 0);
        EXPECT_EQ(rb.capacity(), 16);
        EXPECT_EQ(rb.size(), 0);
        rb.reset();
    }
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


TEST(ring_byte_buf, 0008){
    ring_byte_buf<char> crb;
    thread wt1(write_rb_front<decltype(crb)::value_type>, ref(crb));
    thread wt2(write_rb_front<decltype(crb)::value_type>, ref(crb));
    wt1.join();
    wt2.join();
    EXPECT_EQ(crb.capacity(), (size_t)pow(2,23));
    crb.reset();

    ring_byte_buf rb;
    thread wt3(write_rb_front<decltype(rb)::value_type>, ref(rb));
    thread rt1(read_rb_back<decltype(rb)::value_type>,  ref(rb));
    thread wt4(write_rb_front<decltype(rb)::value_type>, ref(rb));
    thread rt2(read_rb_back<decltype(rb)::value_type>,  ref(rb));
    wt3.join();
    rt1.join();
    wt4.join();
    rt2.join();
    EXPECT_EQ(rb.size(), 0);
    EXPECT_EQ(rb.empty(), true);
    EXPECT_LE(rb.capacity(), (size_t)pow(2,23));
}


TEST(ring_byte_buf, 0009){
    ring_byte_buf rb(1024);
    EXPECT_EQ(rb.empty(), true);
    EXPECT_EQ(rb.capacity(), 1024);
    EXPECT_EQ(rb.size(), 0);
    rb.shrink();
    EXPECT_EQ(rb.empty(), true);
    EXPECT_EQ(rb.capacity(), 0);
    EXPECT_EQ(rb.size(), 0);
    rb.shrink();

    char a[8];
    rb.push_back("12345");
    rb.shrink();
    EXPECT_EQ(rb.capacity(), 8);
    rb.pop_back(a, 2);
    EXPECT_EQ(memcmp(a,"45",2), 0);
    EXPECT_EQ(rb.capacity(), 8);
    rb.shrink();
    EXPECT_EQ(rb.capacity(), 4);
    rb.pop_front(a, 1);
    EXPECT_EQ(memcmp(a,"1",1), 0);
    rb.shrink();
    EXPECT_EQ(rb.capacity(), 2);
    rb.pop_front(a, 1);
    EXPECT_EQ(memcmp(a,"2",1), 0);
    rb.shrink();
    EXPECT_EQ(rb.capacity(), 1);
    rb.pop_front(a, 1);
    EXPECT_EQ(memcmp(a,"3",1), 0);
    rb.shrink();
    EXPECT_EQ(rb.capacity(), 0);
}

// copy from gtest_main.cc
int main(int argc, char **argv) {
    test_empty();
    test_push_back();
    test_push_back_2();
    test_push_back_3();
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

