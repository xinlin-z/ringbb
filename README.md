# ringbb

Ring Byte Buffer in pure C, auto resizable, no thread safe, and with
the capability to push(write) the pop(read) data buffer in both sides.
It could then be used flexibly in different scenarios and
with C and C++ respectively.

Interfaces:

```c
bool rbb_init(ringbb*, size_t);
void rbb_free(ringbb*);
bool rbb_shrink(ringbb*);
bool rbb_push_back(ringbb*, const void*, size_t);
bool rbb_push_front(ringbb*, const void*, size_t);
size_t rbb_pop_front(ringbb*, void*, size_t);
size_t rbb_pop_back(ringbb*, void*, size_t);
```

Test on Linux:

```shell
$ make
$ ./testc
$ ./cpptest
```

If no FAILED literal, it's passed!

