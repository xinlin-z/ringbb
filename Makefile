.RECIPEPREFIX = $
.PHONY: all lib clean

lib: libringbb.so
all: lib test clean

libringbb.so: ring_byte_buf.c ring_byte_buf.h
$ gcc -std=c99 -Wall -Wextra -O3 -fsanitize=address -shared -fPIC $< -o $@

test: test_ringbb.c libringbb.so
$ gcc -std=c99 -Wall -Wextra -O3 -fsanitize=address -Xlinker -rpath . $< -o $@ -L. -lringbb
$ ./test

clean:
$ rm -f test
$ rm -f libringbb.so
