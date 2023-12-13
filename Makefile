.RECIPEPREFIX = >
.PHONY: clean

CC = gcc

testc: test_ringbb.c ring_byte_buf.c
> $(CC) -Wall -Wextra -O3 $^ -o $@ -lm

clean:
> rm -f testc

