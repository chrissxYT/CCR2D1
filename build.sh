#!/bin/sh
gcc -x c ccr2d.c -shared -pthread -fPIC -s -o libccr2d.so
