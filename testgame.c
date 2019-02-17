#include "ccr2d.h"

#define WID 20
#define HEI 10
#define FPS 60
#define PXL WID * HEI

ccr2d1 *obj;

void keyh(key k)
{
	obj->spr[0].pxl[0].dnsty++;
}

void err(error e)
{
	if(e == ERR_SYSTEM_FAIL)
	{
		puts("AAAAAHHHHH, WE BROKE SYSTEM");
	}
}

int main()
{
	error_handler = err;
	pixel bck[PXL];
	pxlset(bck, D_1, PXL);
	obj = c2dnew(bck, WID, HEI, 10, 1000 / FPS, 10);
	c2dstart(obj);
	pixel spr[9];
	pxlset(spr, D_0, 9);
	spr[1].color = C_BLUE;
	spr[1].dnsty = D_2;
	spr[4].color = C_GREEN;
	spr[4].dnsty = D_1;
	c2dspradd(obj, 2, 2, 1, 3, 3, spr);
	c2dkeladd(obj, keyh);
	while(1) sleep_ms(0);
}
