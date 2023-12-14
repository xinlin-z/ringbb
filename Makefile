CC := gcc

.RECIPEPREFIX = >
.PHONY: all lib clean

all: lib testc
lib: libringbb.so

libringbb.so: ring_byte_buf.c ring_byte_buf.h
> $(CC) -Wall -Wextra -O3 -fsanitize=address -shared -fPIC $< -o $@ -lm

testc: test_ringbb.c
> $(CC) -Wall -Wextra -O3 -fsanitize=address -Xlinker -rpath . $^ -o $@ -L. -lringbb

clean:
> rm -f testc
> rm -f libringbb.so

