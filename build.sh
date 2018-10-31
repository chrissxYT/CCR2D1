#!/bin/sh
gcc -O3 ccr2d.c -shared -pthread -fPIC -s -o libccr2d.so
