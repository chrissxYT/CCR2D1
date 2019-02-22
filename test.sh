#!/bin/sh
gcc -O0 ccr2d.c -pthread -Wextra -Wall -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wcast-qual -Wconversion -fsanitize=address -fPIC -shared -o build/libccr2d.so
gcc -O0 testgame.c libccr2d.so -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wcast-qual -Wconversion -Wall -fsanitize=address -o build/testgame
LD_LIBRARY_PATH=build build/testgame
stty sane
