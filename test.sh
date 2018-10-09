#!/bin/sh
gcc -O0 -x c ccr2d.c -pthread -fPIC -shared -o libccr2d.so
gcc -O0 testgame.c libccr2d.so -o testgame
LD_LIBRARY_PATH=. ./testgame
