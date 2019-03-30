#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__)
#define WIN 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>
typedef HANDLE thread;
typedef LPTHREAD_START_ROUTINE tstart;
#ifdef CCR2D1_DLL
#define CCR2D1_API __declspec(dllexport)
#else
#define CCR2D1_API __declspec(dllimport)
#endif
#define TFUNC DWORD WINAPI
#define _TSETUP_CANCEL_TYPE
#define _CANCEL_THREAD(t) TerminateThread(t, 0)
#define _MS_SLEEP(MS) Sleep(MS)
#define _PASSIVE_READ _getch
#else
#define WIN 0
#include <unistd.h>
#include <pthread.h>
typedef pthread_t thread;
typedef void *(*tstart) (void *);
#define CCR2D1_API
#define TFUNC void *
#define _TSETUP_CANCEL_TYPE pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0)
#define _CANCEL_THREAD(t) pthread_cancel(t)
#define _MS_SLEEP(MS) usleep(MS * 1000)
#define _PASSIVE_READ getchar
#endif

typedef unsigned uint;
typedef unsigned long ulong;
typedef char *str;
typedef uint error;
typedef int key;
typedef void(*kel)(key);
typedef void(*errhdl)(error);

#ifndef __cplusplus
typedef int bool;
#endif

#define SHIFTIN(x) (x[0] >> 24 | x[1] >> 16 | x[2] >> 8 | x[3])

//using define for many string constants
//because const-correctness would be a nightmare

//All the color codes for the pixels
#define C_RESET "\e[0m"
#define C_BOLD "\e[1m"
#define C_DIM "\e[2m"
#define C_BLINK "\e[5m"
#define C_HIDDEN "\e[8m"
#define C_BLACK "\e[30m"
#define C_RED "\e[31m"
#define C_GREEN "\e[32m"
#define C_YELLOW "\e[33m"
#define C_BLUE "\e[34m"
#define C_MAGENTA "\e[35m"
#define C_CYAN "\e[36m"
#define C_LIGHT_GRAY "\e[37m"
#define C_DARK_GRAY "\e[90m"
#define C_LIGHT_RED "\e[91m"
#define C_LIGHT_GREEN "\e[92m"
#define C_LIGHT_YELLOW "\e[93m"
#define C_LIGHT_BLUE "\e[94m"
#define C_LIGHT_MAGENTA "\e[95m"
#define C_LIGHT_CYAN "\e[96m"
#define C_WHITE "\e[97m"
#define C_NULL ""

#if !WIN
//move the cursor to (0;0)
#define M_0_0 "\e[0;0f"
#endif

#define D_0 ' '
#define D_1 '+'
#define D_2 '#'
#define D_3 '@'

//a system() call failed
#define ERR_SYSTEM_FAIL      0x00000001
//a SIGILL was caused
#define ERR_SIGILL           0x00000002
//a SIGFPE was caused
#define ERR_SIGFPE           0x00000003
//an unknown SIG occured
#define ERR_ILLSIG           0x00000004
//a header is not what it should be (usually CCR2D1P)
#define ERR_INCORRECT_HEADER 0x00000005

errhdl error_handler = 0;

//a single pixel in an image
typedef struct
{
	//the char printed
	int dnsty;
	//ascii color code
	str color;
} pixel;

//a sprite displayed on the screen
typedef struct
{
	//priority (0 is lowest, 0xffffffff highest)
	uint pri;
	//width in console pixels
	ulong wid;
	//height in console pixels
	ulong hei;
	//x position
	uint x;
	//y position
	uint y;
	//array of pixels / the actual image, null means invalid
	pixel *pxl;
} sprite;

//a few rendering and drawing buffers mashed into one struct
typedef struct
{
	//the pixel array the pixels are rendered into in phase 1
	pixel **p;
	//the char array the chars are rendered into in phase 2
	char **c;
	//the int array the finished chars are aligned into in phase 3
	int *i;
} buffer;

//the main object that holds all the information of CCR2D v1
typedef struct
{
	//the currently rendered chars
	buffer bfr;
	//0 if not running
	int run;
	//the workers for rendering, drawing, etc.
	thread wkr[5];
	//the render width
	ulong wid;
	//the render height
	ulong hei;
	//all the sprites to be drawn
	sprite *spr;
	//sprite count
	uint spc;
	//the background
	pixel *bck;
	//the sleep time between render/draw cycles
	uint slp;
	//key event listeners
	kel *kel;
	//key [event] listener count
	uint klc;
} ccr2d1;

//mallocs a new CCR2D1 object,
//sets width and height,
//mallocs space for 2 workers,
//mallocs a wid * hei buffer,
//mallocs space for max_spr sprites,
//mallocs bck and copies it
CCR2D1_API ccr2d1 *c2dnew(pixel *bck, ulong wid, ulong hei,
		uint max_spr, uint slp, uint max_kel);

//starts the 2 worker threads,
//sets run to true
CCR2D1_API void c2dstart(ccr2d1 *obj);

//a combination of a stop and a delete
//sets run to false,
//stops the 2 worker threads,
//frees the space for the 2 workers,
//frees the whole buffer,
//frees the CCR2D1 object
CCR2D1_API void c2dstop(ccr2d1 *obj);

//adds a new sprite to the obj's spr
//returns its id
CCR2D1_API uint c2dspradd(ccr2d1 *obj, uint x, uint y, uint pri,
		ulong wid, ulong hei, pixel *pxl);

//moves the sprite with the given id relative to its current position
CCR2D1_API void c2dsprmvr(ccr2d1 *obj, uint sid, uint x, uint y);

//moves the sprite with the given id to the given absolute position
#define c2dsprmva(obj, sid, new_x, new_y) \
	obj->spr[sid].x = new_x; \
	obj->spr[sid].y = new_y;

//check if the two sprites collide
CCR2D1_API bool c2dchkcol(ccr2d1 *obj, uint sid1, uint sid2);

//adds a new key event listener to the obj's kel
CCR2D1_API void c2dkeladd(ccr2d1 *obj, kel ltr);

//pauses the thread for the given milliseconds
#define sleep_ms(ms) _MS_SLEEP(ms)

//a memset for pixel arrays
CCR2D1_API void pxlset(pixel *ptr, int dty, ulong num);

//a version of memcpy specifically and only for pixel arrays
#define pxlcpy(dest, src, n) \
	for (ulong i = 0; i < n; i++) dest[i] = src[i];

//a version of memcpy specifically and only for sprite arrays
#define sprcpy(dest, src, n) \
	for (ulong i = 0; i < n; i++) dest[i] = src[i];

//creates a new thread that runs func with arg and returns it
CCR2D1_API thread thread_create(tstart func, void *arg);

//just crashes the given thread t
#define thread_cancel(t) _CANCEL_THREAD(t)

//usually the first function called by a new thread,
//sets canceltype for the thread if not on windows,
//casts arg to a ccr2d1 ptr and returns it
#define setup_thread(arg) _TSETUP_CANCEL_TYPE; ccr2d1 *obj = (ccr2d1*)arg

//mallocs a new pixel array that's indexed [x][y]
CCR2D1_API pixel **pxlarr2dmallocxy(ulong wid, ulong hei);

//frees the given pixel array that's indexed [x][y]
CCR2D1_API void pxlarr2dfreexy(pixel **pxl, ulong wid);

//load picture - reads a CCR2D1P from stream into buffer and sets
//*width to the width and *height to the height
CCR2D1_API void c2dldp(FILE *stream, pixel *buffer, ulong *width, ulong *height);
