#include "ccr2d.h"
#ifdef _WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE > 199308
#include <time.h>
#else
#include <unistd.h>
#endif

void *render(void *vargp)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	struct ccr2d1 *obj = (struct ccr2d1*)vargp;
	while(1)
	{
		//draw sprites to buffer
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

struct ccr2d1 *c2dnew(int wid, int hei)
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
