#include "ccr2d.h"

#define wid 20
#define hei 10

//void c2dspradd(ccr2d1 *obj, int x, int y, unsigned pri,
//                long unsigned wid, long unsigned hei, pixel *pxl);

int main()
{
	pixel bck[wid * hei];
	pxlarr(wid, hei, bck);
	ccr2d1 *obj = c2dnew(bck, 20, 10, 10);
	c2dstart(obj);
	pixel spr[9];
	pxlarr(3, 3, spr);
	spr[1].color = C_BLUE;
	spr[1].dnsty = D_2;
	spr[4].color = C_GREEN;
	spr[4].dnsty = D_1;
	c2dspradd(obj, 2, 2, 1, 3, 3, spr);
	while(1);
}
