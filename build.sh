#!/bin/sh
gcc -O3 ccr2d.c -shared -pthread -fPIC -s -o build/libccr2d.so
