

.RECIPEPREFIX = >
test_rbb: test_ring_byte_buf.cpp
> g++ -Wall -Wextra -O3 $< -o $@
> ./$@

