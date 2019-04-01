#!/bin/sh
gcc -O3 ccr2d.c -shared -pthread -fPIC -Wno-unused-variable -s -o build/libccr2d.so
