all:
	@gcc -O3 ccr2d.c -shared -pthread -fPIC -Wno-unused-variable -s -o bld/libccr2d.so
test:
	@gcc -O0 ccr2d.c -pthread -Wextra -Wall -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wcast-qual -Wconversion -fsanitize=address -fPIC -shared -o bld/libccr2d.so
	@gcc -O0 testgame.c bld/libccr2d.so -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wcast-qual -Wconversion -Wall -fsanitize=address -o bld/testgame
	@LD_LIBRARY_PATH=bld bld/testgame
