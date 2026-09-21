# ringbb

Ring Byte Buffer in C, auto resizable, no thread-safe, and with
the capability to push (write) the pop (read) data in both ends (deque).

**Interface**

```c
#ifdef __cplusplus
extern "C" {
#endif
bool   rbb_init(ringbb*, size_t);
void   rbb_free(ringbb*);
bool   rbb_push_back(ringbb*, const void*, size_t);
bool   rbb_push_front(ringbb*, const void*, size_t);
size_t rbb_pop_back(ringbb*, void*, size_t);
size_t rbb_pop_front(ringbb*, void*, size_t);
bool   rbb_shrink(ringbb*);

static inline bool   rbb_empty(const ringbb *rb) { return rb->size == 0; }
static inline size_t rbb_size(const ringbb *rb) { return rb->size; }
static inline size_t rbb_capacity(const ringbb *rb) { return rb->capacity; }
static inline void   rbb_clear(ringbb *rb) { rb->size = rb->wp = rb->rp = 0; }
#ifdef __cplusplus
}
#endif
```

**Test on Linux**

```shell
$ make all
gcc -std=c99 -Wall -Wextra -O3 -fsanitize=address -shared -fPIC ring_byte_buf.c -o libringbb.so
gcc -std=c99 -Wall -Wextra -O3 -fsanitize=address -Xlinker -rpath . test_ringbb.c -o test -L. -lringbb
./test
All Pass!
rm -f test
rm -f libringbb.so
```
