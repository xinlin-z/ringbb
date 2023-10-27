

.PHONY: test_rbb


.RECIPEPREFIX = >
test_rbb: test.cpp
> g++ -Wall -Wextra -O3 -fsanitize=address $< -o $@
> ./$@

