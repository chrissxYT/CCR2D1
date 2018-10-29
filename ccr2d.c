#include "ccr2d.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void quicksort(sprite *spr, unsigned first, unsigned last)
{
	if (first < last)
	{
		sprite temp;
		unsigned pivot = first;
		unsigned i = first;
		unsigned j = last;
		while (i < j)
		{
			while (spr[i].pri<=spr[pivot].pri && i < last)
			{
				i++;
			}
			while (spr[j].pri > spr[pivot].pri)
			{
				j--;
			}
			if (i < j)
			{
				temp = spr[i];
				spr[i] = spr[j];
				spr[j] = temp;
			}
		}
		temp = spr[pivot];
		spr[pivot] = spr[j];
		spr[j] = temp;
		quicksort(spr, first, j - 1);
		quicksort(spr, j + 1, last);
	}
}

void *render(void *vargp)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	ccr2d1 *obj = (ccr2d1*)vargp;
	while(1)
	{
		pixel pxl[obj->wid][obj->hei];
		for(long unsigned y = 0; y < obj->hei; y++)
		{
			long unsigned i = y * obj->wid;
			for(long unsigned x = 0; x < obj->wid; x++)
			{
				pxl[x][y] = obj->bck[i + x];
			}
		}
		unsigned spc = obj->spc;
		long unsigned spc_sizeof_sprite = spc * sizeof(sprite);
		sprite *spr = malloc(spc_sizeof_sprite);
		memcpy(spr, obj->spr, spc_sizeof_sprite);
		quicksort(spr, 0, spc - 1);
		for(unsigned i = 0; i < spc; i++)
		{
			sprite s = spr[i];
			for(long unsigned j = 0; j < s.hei; j++)
			{
				for(long unsigned k = 0; k < s.wid; k++)
				{
					pixel p =
						s.pxl[j * s.wid + k];
					//null color ^= transparent
					if(p.color != 0)
					{
						pxl[(long long)
							k + s.x][
							(long long)j
								+ s.y]
							= p;
					}
				}
			}
		}
		free(spr);
		sleep_ms(1);
	}
}

void *draw(void *vargp)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	ccr2d1 *obj = (ccr2d1*)vargp;
	while(1)
	{
		puts(M_0_0);
		for(long unsigned i = 0; i < obj->hei; i++)
		{
			puts(obj->bfr.c[i]);
			putc('\n', stdout);
		}
		sleep_ms(1);
	}
}

void c2dspradd(ccr2d1 *obj, int x, int y, unsigned pri,
		long unsigned wid, long unsigned hei, pixel *pxl)
{
	obj->spr[obj->spc].pri = pri;
	obj->spr[obj->spc].wid = wid;
	obj->spr[obj->spc].hei = hei;
	obj->spr[obj->spc].pxl = pxl;
	obj->spr[obj->spc].x = x;
	obj->spr[obj->spc].y = y;
	obj->spc++;
}

ccr2d1 *c2dnew(pixel *bck, long unsigned wid, long unsigned hei,
		unsigned max_spr)
{
	ccr2d1 *obj = malloc(sizeof(ccr2d1));
	obj->wid = wid;
	obj->hei = hei;
	obj->run = 0;
	obj->wkr = malloc(sizeof(pthread_t) * 2);
	obj->bfr.c = malloc(hei * sizeof(char*));
	for(long unsigned i = 0; i < hei; i++)
	{
		obj->bfr.c[i] = malloc(wid * 10);
	}
	obj->spr = malloc(sizeof(sprite) * max_spr);
	obj->spc = 0;
	long unsigned sz = wid * hei * sizeof(pixel);
	pixel *bck_a = malloc(sz);
	memcpy(bck_a, bck, sz);
	obj->bck = bck_a;
	return obj;
}

void c2dstart(ccr2d1 *obj)
{
	obj->run = 1;
	pthread_create(&obj->wkr[0], 0, render, obj);
	pthread_create(&obj->wkr[1], 0, draw, obj);
}

void c2dstop(ccr2d1 *obj)
{
	obj->run = 0;
	for(unsigned i = 0; i < sizeof(obj->wkr) / sizeof(pthread_t); i++)
	{
		pthread_cancel(obj->wkr[i]);
	}
	free(obj->wkr);
	for(unsigned i = 0; i < obj->hei; i++)
	{
		free(obj->bfr.c[i]);
	}
	free(obj->bfr.c);
	free(obj);
}

void pxlarr(long unsigned wid, long unsigned hei, pixel *bfr)
{
	long unsigned i = wid * hei;
	for(unsigned j = 0; j < i; j++)
	{
		bfr[j].dnsty = D_0;
		bfr[j].color = C_RESET;
	}
}

void sleep_ms(unsigned ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}
