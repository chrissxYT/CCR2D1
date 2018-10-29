#!/bin/sh
gcc -O3 -x c ccr2d.c -shared -pthread -fPIC -s -o libccr2d.so
