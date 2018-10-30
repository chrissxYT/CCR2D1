#include "ccr2d.h"

#define wid 20
#define hei 10
#define fps 60

ccr2d1 *obj;

void kel(int i)
{
	obj->spr[0].pxl[0].dnsty = i;
}

int main()
{
	pixel bck[wid * hei];
	pxlarr(wid, hei, bck);
	obj = c2dnew(bck, 20, 10, 10, 1000 / fps, 10);
	c2dstart(obj);
	pixel spr[9];
	pxlarr(3, 3, spr);
	spr[1].color = C_BLUE;
	spr[1].dnsty = D_2;
	spr[4].color = C_GREEN;
	spr[4].dnsty = D_1;
	c2dspradd(obj, 2, 2, 1, 3, 3, spr);
	c2dkeladd(obj, kel);
	while(1) sleep_ms(1000000);
}
