/*
 * FileName: ring_byte_buf.hpp
 * Author:   xinlin-z
 * Desc:     Head-only Auto-resizable Thread-safe Ring Byte Buffer template!
 * Blog:     https://cs.pynote.net
 * Github:   https://github.com/xinlin-z/ringbb
 */
#ifndef _RING_BYTE_BUF_HPP
#define _RING_BYTE_BUF_HPP 1
#include <cstring>
#include <cmath>
#include <mutex>
#include <cassert>


#define STATIC_TYPE_ASSERTS(X)  \
    static_assert(sizeof(X)==1,           "sizeof(T) != 1."); \
    static_assert(!std::is_same_v<X,bool>,"bool type is not supported."); \
    static_assert(!std::is_class_v<X>,    "class type is not supported.");\
    static_assert(!std::is_array_v<X>,    "array type is not supported.");\
    /* sizeof(void)==1, and you can only get a warning */ \
    static_assert(!std::is_void_v<X>,     "void type is not supported."); \
    /* new operator cannot be applied to a reference type */ \
    static_assert(!std::is_reference_v<X>,"ref type is not supported.");\


template<typename T=std::byte>
struct ring_byte_buf{
    STATIC_TYPE_ASSERTS(T);

    using value_type = T;
    using type = ring_byte_buf<T>;

    explicit ring_byte_buf(size_t init_size=4096);
    ~ring_byte_buf();

    template<typename U>
    ring_byte_buf(const ring_byte_buf<U>&) = delete;
    template<typename U>
    ring_byte_buf(ring_byte_buf<U>&&) = delete;
    template<typename U>
    ring_byte_buf<U>& operator=(const ring_byte_buf<U>&) = delete;
    template<typename U>
    ring_byte_buf<U>& operator=(ring_byte_buf<U>&&) = delete;

    void reset() noexcept;
    void clear() noexcept;
    bool empty() noexcept;
    void shrink();
    size_t capacity() noexcept;
    size_t size() noexcept;

    // push_back
    template<typename U>
    void push_back(const U *cont, size_t len);
    template<typename U>
    void push_back(const U &cont);
    template<size_t N>
    void push_back(const char (&a)[N]);
    void push_back(const char *cont);

    // push_front
    template<typename U>
    void push_front(const U *cont, size_t len);
    template<typename U>
    void push_front(const U &cont);
    template<size_t N>
    void push_front(const char (&a)[N]);
    void push_front(const char *cont);

    // pop_back
    template<typename U>
    size_t pop_back(U *cont, size_t len) noexcept;

    // pop_front
    template<typename U>
    size_t pop_front(U *cont, size_t len) noexcept;

private:
    T *p;
    size_t cap;
    size_t ocsize{};
    size_t wp{};
    size_t rp{};
    std::mutex mutex{};

    size_t _get_newsize(size_t needed_size) noexcept;
    template<typename U>
    void _recap(const U *cont, size_t len, bool is_push_back=true);
    template<typename U>
    void _pop_all(U *cont) noexcept;
};


template<typename T>
ring_byte_buf<T>::ring_byte_buf(size_t init_size):
    p{ new T[init_size] },
    cap{ init_size } {}


template<typename T>
ring_byte_buf<T>::~ring_byte_buf() {
    delete[] p;
}


template<typename T>
void ring_byte_buf<T>::reset() noexcept {
    mutex.lock();
    delete[] p;
    p = nullptr;
    cap = ocsize = wp = rp = 0;
    mutex.unlock();
}


template<typename T>
void ring_byte_buf<T>::clear() noexcept {
    mutex.lock();
    ocsize = wp = rp = 0;
    mutex.unlock();
}


template<typename T>
bool ring_byte_buf<T>::empty() noexcept {
    mutex.lock();
    bool retval { ocsize==0 };
    mutex.unlock();
    return retval;
}


template<typename T>
size_t ring_byte_buf<T>::_get_newsize(size_t needed_size) noexcept{
    return pow(2, (int)ceil(log2(needed_size)));
}


template<typename T>
template<typename U>
void ring_byte_buf<T>::_recap(const U *cont, size_t len,
                                  bool is_push_back){
    size_t newsize { _get_newsize(ocsize+len) };
    T *t { new T[newsize] };
    size_t front_len{};

    if(!is_push_back){
        memcpy(t, cont, len);
        front_len = len;
    }
    _pop_all(t+front_len);
    if(is_push_back)
        memcpy(t+ocsize, cont, len);

    delete[] p;
    p = t;
    cap = newsize;
    ocsize += len;
    wp = ocsize;
    rp = 0;
}


template<typename T>
template<typename U>
void ring_byte_buf<T>::push_back(const U *cont, size_t len) {
    STATIC_TYPE_ASSERTS(U);
    if (len == 0)
        return;
    mutex.lock();
    if (len <= (cap - ocsize)) {
        if (wp + len <= cap){
            memcpy(p+wp, cont, len);
            wp += len;
        }
        else {
            size_t fwlen = cap - wp;
            memcpy(p+wp, cont, fwlen);
            memcpy(p, cont+fwlen, len-fwlen);
            wp = len - fwlen;
        }
        ocsize += len;
    } else
        _recap(cont, len);
    mutex.unlock();
}


/**
 * If there is no this overload,
 * push_back("123") will be taken by the array overload with length 4.
 * I'd like to take this form as a C-style string.
**/
template<typename T>
void ring_byte_buf<T>::push_back(const char *cont){  // string literal
    push_back(cont, strlen(cont));
}


template<typename T>
template<typename U>
void ring_byte_buf<T>::push_back(const U &cont){
    if constexpr(std::is_same_v<U,std::string>)       // std::string
        push_back(cont.c_str(), cont.size());
    else{                                             // single char like 'a'
        static_assert(std::is_same_v<U,char>,
                      "push_back(const U&) type not support.");
        push_back(&cont, 1);
    }
}


/**
 * reference parameter does not decay!
 * if define: char a[] {"abcd"};
 * N is 5, the last char is '\0' for sure.
 * if define: char b[] {'a','b','c','d'};
 * N is only 4.
 * if define: char c[] {'a','b','c','\0'};
 * the last \0 will be write into ring buffer!!
**/
template<typename T>
template<size_t N>
void ring_byte_buf<T>::push_back(const char (&a)[N]){  // char array
    push_back(a, N);
}


template<typename T>
template<typename U>
void ring_byte_buf<T>::push_front(const U *cont, size_t len) {
    STATIC_TYPE_ASSERTS(U);
    if (len == 0)
        return;
    mutex.lock();
    if (len <= (cap - ocsize)){
        if(rp >= len){
            memcpy(p+rp-len, cont, len);
            rp -= len;
        } else{
            size_t frlen = len - rp;
            memcpy(p+cap-frlen, cont, frlen);
            memcpy(p, cont+frlen, len-frlen);
            rp = cap - frlen;
        }
        ocsize += len;
    } else
        _recap(cont, len, false);
    mutex.unlock();
}


template<typename T>
void ring_byte_buf<T>::push_front(const char *cont){  // string literal
    push_front(cont, strlen(cont));
}


template<typename T>
template<typename U>
void ring_byte_buf<T>::push_front(const U &cont){
    if constexpr(std::is_same_v<U,std::string>)       // std::string
        push_front(cont.c_str(), cont.size());
    else{                                             // single char like 'a'
        static_assert(std::is_same_v<U,char>,
                      "push_back(const U&) type not support.");
        push_front(&cont, 1);
    }
}


template<typename T>
template<size_t N>
void ring_byte_buf<T>::push_front(const char (&a)[N]){  // char array
    push_front(a, N);
}


template<typename T>
template<typename U>
void ring_byte_buf<T>::_pop_all(U *cont) noexcept{
    if (wp > rp)
        memcpy(cont, p+rp, ocsize);
    else {
        size_t frlen = cap - rp;
        memcpy(cont, p+rp, frlen);
        memcpy(cont+frlen, p, wp);
    }
}


template<typename T>
template<typename U>
size_t ring_byte_buf<T>::pop_front(U *cont, size_t len) noexcept{
    STATIC_TYPE_ASSERTS(U);
    size_t retval;
    mutex.lock();
    if (ocsize == 0)
        retval = 0;
    else {
        if (ocsize >= len) {
            if((cap-rp) >= len){
                memcpy(cont, p+rp, len);
                rp += len;
            } else {
                size_t frlen = cap - rp;
                memcpy(cont, p+rp, frlen);
                memcpy(cont+frlen, p, len-frlen);
                rp = len - frlen;
            }
            ocsize -= len;
            retval = len;
        } else {
            _pop_all(cont);
            rp = wp;
            retval = ocsize;
            ocsize = 0;
        }
    }
    mutex.unlock();
    return retval;
}


template<typename T>
template<typename U>
size_t ring_byte_buf<T>::pop_back(U *cont, size_t len) noexcept{
    STATIC_TYPE_ASSERTS(U);
    size_t retval;
    mutex.lock();
    if (ocsize == 0)
        retval = 0;
    else {
        if(ocsize >= len){
            if(wp >= len){
                memcpy(cont, p+wp-len, len);
                wp -= len;
            } else {
                size_t frlen = len - wp;
                memcpy(cont, p+cap-frlen, frlen);
                memcpy(cont+frlen, p, wp);
                wp = cap - frlen;
            }
            ocsize -= len;
            retval = len;
        } else {
            _pop_all(cont);
            rp = wp;
            retval = ocsize;
            ocsize = 0;
        }
    }
    mutex.unlock();
    return retval;
}


template<typename T>
size_t ring_byte_buf<T>::capacity() noexcept {
    mutex.lock();
    size_t retval { cap };
    mutex.unlock();
    return retval;
}


template<typename T>
size_t ring_byte_buf<T>::size() noexcept {
    mutex.lock();
    size_t retval { ocsize };
    mutex.unlock();
    return retval;
}


template<typename T>
void ring_byte_buf<T>::shrink(){
    mutex.lock();
    if(ocsize == 0){
        mutex.unlock();
        reset();
        return;
    }

    size_t newsize { _get_newsize(ocsize) };
    if(cap == newsize){
        mutex.unlock();
        return;
    }

    assert(cap > newsize);
    T *t { new T[newsize] };
    _pop_all(t);
    delete[] p;
    p = t;
    cap = newsize;
    wp = ocsize;
    rp = 0;
    mutex.unlock();
}


#endif
