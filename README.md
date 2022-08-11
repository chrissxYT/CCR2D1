# CCR2D1 [![](https://tokei.rs/b1/github/pixelcmtd/CCR2D1)](https://github.com/pixelcmtd/CCR2D1)

CConsoleRetro2D version 1 - A C library for NES/GB-like games in the console

## Disclaimer

This library is not production ready, it's still full of bugs.

## Sample code

```c
#include "ccr2d.h"

int main()
{
    //temporaryly allocate the background on the stack
    pixel bck[70 * 28];
    //initialize the background empty
    pxlarr(70, 28, bck);
    //create new CCR2D v1 object
    ccr2d1 *obj = c2dnew(bck, 70, 28, 10);
    //start rendering and drawing
    c2dstart(obj);
    //don't do anything else until the user kills the process
    while(1) sleep_ms(1000);
}
```
