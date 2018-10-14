#!/bin/sh
gcc -O0 -x c ccr2d.c -pthread -Wextra -Wall -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wcast-qual -Wconversion -fPIC -shared -o libccr2d.so
gcc -O0 testgame.c libccr2d.so -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wcast-qual -Wconversion -Wall -o testgame
LD_LIBRARY_PATH=. ./testgame
