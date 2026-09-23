# ringbb

Ring Byte Buffer in C, which is auto-resizable, not thread-safe, and with
the capability to push (write) the pop (read) data in both ends (deque).
It could be complied with C++.

```shell
# compile libringbb.so
$ make [lib]
```

## Interface

```c
bool   rbb_init(ringbb*, size_t);
void   rbb_free(ringbb*);
bool   rbb_push_back(ringbb*, const void*, size_t);
bool   rbb_push_front(ringbb*, const void*, size_t);
size_t rbb_pop_back(ringbb*, void*, size_t);
size_t rbb_pop_front(ringbb*, void*, size_t);
bool   rbb_shrink(ringbb*);

bool   rbb_empty(const ringbb*);
size_t rbb_size(const ringbb*);
size_t rbb_capacity(const ringbb*);
void   rbb_clear(ringbb*);
```

## Test

```shell
# make lib and test, run test and then clean all
$ make all
gcc -std=c99 -Wall -Wextra -O3 -fsanitize=address -shared -fPIC ring_byte_buf.c -o libringbb.so
gcc -std=c99 -Wall -Wextra -O3 -fsanitize=address -Xlinker -rpath . test_ringbb.c -o test -L. -lringbb
./test
All Pass!
rm -f test
rm -f libringbb.so
```

## Memory Growth Strategy

```c
#define RBB_BUF_LEN_UNIT     ((size_t)1 << 12)  /* 4K */

#if SIZE_MAX > 0xFFFFFFFFU
#define RBB_BUF_MAX_SIZE     ((size_t)1 << 32)  /* 4G @ 64bit */
#else
#define RBB_BUF_MAX_SIZE     ((size_t)1 << 30)  /* 1G @ 32bit */
#endif
```

The buffer size is always multiples of `RBB_BUF_LEN_UNIT`.

Initially, the buffer size is just enough according to the parameter passed to `rbb_init`. As pushing bytes into the ring buffer, the buffer size is increased by doubling as long as the total capacity is less than `RBB_BUF_MAX_SIZE` and more than the actual needed size. If the size of doubling is exceeded the max limit, buffer size fall back to the actual size. `rbb_shrink` is used to try to free extra memory if necessary. Otherwise, there is no automatic mechanism to decrease the buffer size.
