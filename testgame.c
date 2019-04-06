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
	//this will cause memory leaks but on our
	//test machines we got enough ram
	if(obj->spr[sid].pxl[0].dnsty == D_0)
		obj->spr[sid].pxl[0].dnsty = malloc(1),
		*obj->spr[sid].pxl[0].dnsty = *D_0;
	(*obj->spr[sid].pxl[0].dnsty)++;
	if(obj->spr[sid].pxl[3].dnsty == D_0)
		obj->spr[sid].pxl[3].dnsty = malloc(1);
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
	pixel bck[PXL];
	pxlset(bck, D_1, PXL);
	obj = c2dnew(bck, WID, HEI, 10, SLP, 10);
	c2dstart(obj);
	pixel spr[9];
	pxlset(spr, D_0, 9);
	spr[1].color = C_BLUE;
	spr[1].dnsty = D_2;
	spr[4].color = C_GREEN;
	spr[4].dnsty = D_1;
	sid = c2dspradd(obj, 2, 2, 1, 3, 3, spr);
	c2dkeladd(obj, keyh);
	while(1) sleep_ms(0);
}
