# ringbb

Ring Byte Buffer in pure C, auto resizable, no thread safe, and with
the capability to push(write) the pop(read) data buffer in both sides.
It could then be used flexibly in different scenarios and
with C and C++ respectively.

**Interface**

```c
bool rbb_init(ringbb*, size_t);
void rbb_free(ringbb*);
bool rbb_shrink(ringbb*);
bool rbb_push_back(ringbb*, const void*, size_t);
bool rbb_push_front(ringbb*, const void*, size_t);
size_t rbb_pop_front(ringbb*, void*, size_t);
size_t rbb_pop_back(ringbb*, void*, size_t);
```

To access ring byte buffer's capacity and size, just visit the member
variable. Suppose `rb` is a pointer to ringbb struct (check
ring_byte_buf.h), capacity is `rb->capacity`, size is `rb->size`.
You can only read them, do not modify them in any case!!

**Test on Linux**

```shell
$ make
$ ./testc
$ ./cpptest
```

If no FAILED literal, it's passed!

**Example**

`test_ringbb.c` and `cpptest.cpp` can be taken as examples.

