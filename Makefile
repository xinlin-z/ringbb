.RECIPEPREFIX = $
.PHONY: all lib clean

all: run clean
lib: libringbb.so

run: test_ringbb.c libringbb.so
$ gcc -std=c99 -Wall -Wextra -O3 -fsanitize=address -Xlinker -rpath . $^ -o $@ -L. -lringbb
$ ./run

libringbb.so: ring_byte_buf.c ring_byte_buf.h
$ gcc -std=c99 -Wall -Wextra -O3 -fsanitize=address -shared -fPIC $< -o $@

clean:
$ rm -f run
$ rm -f libringbb.so

