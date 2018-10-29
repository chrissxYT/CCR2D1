# CCR2D
CConsoleRetro2D - C library for NES/GB-like games in the console
## Example code
```c
#include "ccr2d.h"

int main()
{
    //temporaryly allocate the background on the stack 
    struct pixel bck[70 * 28];
    //initialize the background empty
    pxlarr(70, 28, bck);
    //create new CCR2D v1 render object
    struct ccr2d1 *obj = c2dnew(bck, 70, 28, 10);
    //start rendering and drawing
    c2dstart(obj);
    //don't do anything else until the user kills the process
    while(1);
}
```
