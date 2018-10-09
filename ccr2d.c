#include "ccr2d.h"
#ifdef _WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE > 199308
#include <time.h>
#else
#include <unistd.h>
#endif

void quicksort(struct sprite *spr, int first, int last)
{
	if (first < last)
	{
		struct sprite temp;
		int pivot;
		int i = first;
		int j = last;
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
	struct ccr2d1 *obj = (struct ccr2d1*)vargp;
	while(1)
	{
		struct pixel pxl[obj->wid][obj->hei];
		memcpy(pxl, obj->bck,
				obj->wid * obj->hei *sizeof(sprite));
		int spc = obj->spc;
		struct sprite *spr = malloc(spc * sizeof(sprite));
		memcpy(spr, obj->spr, spc * sizeof(sprite));
		quicksort(spr, 0, spc - 1);
		for(int i = 0; i < spc; i++)
		{
			struct sprite s = spr[i];
			for(int j = 0; j < s.hei; j++)
			{
				for(int k = 0; k < s.wid; k++)
				{
					struct pixel p =
						s.pxl[j * s.wid + k];
					//null color ^= transparent
					if(p.color != 0)
					{
						pxl[k + s.x][j + s.y]
							= p;
					}
				}
			}
		}
		free(spr);
	}
}

void *draw(void *vargp)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	struct ccr2d1 *obj = (struct ccr2d1*)vargp;
	while(1)
	{
		puts(M_0_0);
		for(int i = 0; i < obj->hei; i++)
		{
			puts(obj->bfr.c[i]);
			putc('\n', stdout);
		}
		sleep_ms(1);
	}
}

void c2dspradd(struct ccr2d1 *obj, int x, int y,
	int pri, int wid, int hei, struct pixel *pxl)
{
	obj->spr[obj->spc].pri = pri;
	obj->spr[obj->spc].wid = wid;
	obj->spr[obj->spc].hei = hei;
	obj->spr[obj->spc].pxl = pxl;
	obj->spr[obj->spc].x = x;
	obj->spr[obj->spc].y = y;
	obj->spc++;
	obj->spr[obj->spc].pri = NOSPR;
	obj->spr[obj->spc].wid = NOSPR;
	obj->spr[obj->spc].hei = NOSPR;
	obj->spr[obj->spc].x = NOSPR;
	obj->spr[obj->spc].y = NOSPR;
	obj->spr[obj->spc].pxl = 0;
}

struct ccr2d1 *c2dnew(struct pixel *bck,int wid, int hei, int max_spr)
{
	struct ccr2d1 *obj = malloc(sizeof(ccr2d1));
	obj->wid = wid;
	obj->hei = hei;
	obj->run = 0;
	obj->wkr = malloc(sizeof(pthread_t) * 2);
	obj->bfr.c = malloc(hei);
	for(int i = 0; i < hei; i++)
	{
		obj->bfr.c[i] = malloc(wid * 10);
	}
	obj->spr = malloc(sizeof(sprite) * max_spr);
	obj->spr[0].pri = NOSPR;
	obj->spr[0].wid = NOSPR;
	obj->spr[0].hei = NOSPR;
	obj->spr[0].x = NOSPR;
	obj->spr[0].y = NOSPR;
	obj->spr[0].pxl = 0;
	obj->spc = 0;
	struct pixel *bck_a = malloc(sizeof(bck));
	memcpy(bck_a, bck, sizeof(bck));
	obj->bck = bck_a;
}

void c2dstart(struct ccr2d1 *obj)
{
	obj->run = 1;
	pthread_create(&obj->wkr[0], 0, render, obj);
	pthread_create(&obj->wkr[1], 0, draw, obj);
}

void c2dstop(struct ccr2d1 *obj)
{
	obj->run = 0;
	for(int i = 0; i < sizeof(obj->wkr) / sizeof(pthread_t); i++)
	{
		pthread_cancel(obj->wkr[i]);
	}
	free(obj->wkr);
	for(int i = 0; i < obj->hei; i++)
	{
		free(obj->bfr.c[i]);
	}
	free(obj->bfr.c);
	free(obj);
}

void pxlarr(int wid, int hei, struct pixel *bfr)
{
	int i = wid * hei;
	for(int j = 0; j < i; j++)
	{
		bfr[j].dnsty = D_0;
		bfr[j].color = C_RESET;
	}
}

void sleep_ms(int ms)
{
#ifdef WIN32
    Sleep(ms);
#elif _POSIX_C_SOURCE > 199308
    struct timespec ts;
    ts.tv_sec = ms / 1000;
   ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, 0);
#else
    usleep(ms * 1000);
#endif
}
