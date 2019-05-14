#include "ccr2d.h"
#include <signal.h>

#define WID 20
#define HEI 10
#define FPS 60
#define SLP 1000 / FPS
#define PXL WID * HEI

ccr2d1 *obj;
uint sid;

void keyh(key k)
{
	if(k == 'q') raise(SIGINT);
	(*obj->spr[sid].pxl[0].dnsty)++;
	(*obj->spr[sid].pxl[3].dnsty) = k;
}

void err(error e)
{
	c2dstop(obj);
	while(1) printf("error: 0x%8x\n", e);
}

int main()
{
	error_handler = err;
	pixel *bck = palloc(PXL);
	pxlset(bck, D_1, PXL);
	obj = c2dnew(bck, WID, HEI, 10, SLP, 1);
	free(bck);
	c2dstart(obj);
	pixel spr[9];
	pxlset(spr, D_0, 9);
	color(spr[1], 0, 0, 0xff);
	strcpy(spr[1].dnsty, D_2);
	color(spr[4], 0, 0xff, 0);
	strcpy(spr[4].dnsty, D_1);
	sid = c2dspradd(obj, 2, 2, 1, 3, 3, spr);
	c2dkeladd(obj, keyh);
	while(1) sleep_ms(0);
}
