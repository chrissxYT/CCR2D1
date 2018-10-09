#include "ccr2d.h"

#define wid 20
#define hei 10

int main(int argc, char **argv)
{
	struct pixel bck[wid * hei];
	pxlarr(wid, hei, bck);
	struct ccr2d1 *obj = c2dnew(bck, 20, 10, 10);
	c2dstart(obj);
	while(1);
}
