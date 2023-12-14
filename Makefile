CC := gcc

.RECIPEPREFIX = >
.PHONY: all lib clean

all: lib testc
lib: libringbb.so

libringbb.so: ring_byte_buf.c
> $(CC) -Wall -Wextra -O3 -shared -fPIC $^ -o $@ -lm

testc: test_ringbb.c
> $(CC) -Wall -Wextra -O3 -Xlinker -rpath . $^ -o $@ -L. -lringbb

clean:
> rm -f testc
> rm -f libringbb.so

