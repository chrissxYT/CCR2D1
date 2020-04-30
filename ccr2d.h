#pragma once

#ifdef _MSC_VER
//disable the warning for spectre mitigations
#pragma warning(disable :5040)
//enable strcpy and other "unsafe" functions
#define _CRT_SECURE_NO_WARNINGS
#endif

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__)
#define WIN 1
#define win(x) x
#define UNIX 0
#define unix(x)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
typedef HANDLE thread;
typedef LPTHREAD_START_ROUTINE tstart;
#ifdef CCR2D1_DLL
#define CCR2D1_API __declspec(dllexport)
#else
#define CCR2D1_API __declspec(dllimport)
#endif
#define TFUNC DWORD WINAPI
#define _TSETUP_CANCEL_TYPE()
#define thread_cancel(t) TerminateThread((t), 0)
#define sleep_ms(MS) Sleep((MS))
#define _PASSIVE_READ _getch
#define M_0_0() COORD c; \
	c.X = c.Y = 0; \
	SetConsoleCursorPosition(GetConsoleWindow(), c)
#else
#define WIN 0
#define win(x)
#define UNIX 1
#define unix(x) x
#include <unistd.h>
#include <pthread.h>
typedef pthread_t thread;
typedef void *(*tstart) (void *);
#define CCR2D1_API
#define TFUNC void *
#define _TSETUP_CANCEL_TYPE() pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0)
#define thread_cancel(t) pthread_cancel(t)
#define sleep_ms(MS) usleep((MS) * 1000)
#define _PASSIVE_READ getchar
//move the cursor to (0;0)
#define M_0_0() const char *c = "\e[0;0f"; \
	while (*c) putc(*c++, stdout)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long ulong;
typedef char *str;
typedef uint error;
typedef int key;
typedef void(*kel)(key);
typedef void(*errhdl)(error);

#ifndef bool
typedef int bool;
#endif

#define LESHIFTIN16(x) ((x)[0] >> 8 | (x)[1])
#define LESHIFTIN32(x) ((x)[0] >> 24 | (x)[1] >> 16 | (x)[2] >> 8 | (x)[3])
#define BESHIFTIN16(x) ((x)[0] | (x)[1] >> 8)
#define BESHIFTIN32(x) ((x)[0] | (x)[1] >> 8 | (x)[2] >> 16 | (x)[3] >> 24)

#define color(p, _r, _g, _b) (p).r = (_r); (p).g = (_g); (p).b = (_b)

#define D_E "E"
#define D_0 " "
#define D_1 "░"
#define D_2 "▒"
#define D_3 "▓"
#define D_4 "█"

//a system() call failed
#define ERR_SYSTEM_FAIL      0x00000001
//a SIGILL was caused
#define ERR_SIGILL           0x00000002
//a SIGFPE (mett ellol) was caused
#define ERR_SIGFPE           0x00000003
//an unknown SIG occured
#define ERR_ILLSIG           0x00000004
//a header is not what it should be (usually CCR2D1P)
#define ERR_INCORRECT_HEADER 0x00000005
//a fread failed
#define ERR_FREAD_FAIL       0x00000006

errhdl error_handler = 0;

//a single pixel in an image
typedef struct
{
	//the char printed (not a single char for unicode support)
	char dnsty[250];
	unsigned short r, g, b;

	//yes, using a big dnsty buffer has pretty bad
	//effects to memory efficiency, but at our typical
	//resolution we don't have to care.
} pixel;

#define palloc(n) malloc((n) * sizeof(pixel))

//a sprite displayed on the screen
typedef struct
{
	//priority (0 is lowest, 0xffffffff highest)
	uint pri;
	//width in console pixels
	uint wid;
	//height in console pixels
	uint hei;
	//x and y positions
	uint x, y;
	//array of pixels aka. the actual image; NULL = invalid
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
	uint wid;
	//the render height
	uint hei;
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

typedef enum
{
	NONE = 0x0,
	TL1 = 0x1,
	TR1 = 0x2,
	BL1 = 0x4,
	BR1 = 0x8,
	TL2 = 0x10,
	TR2 = 0x20,
	BL2 = 0x40,
	BR2 = 0x80,
} colpos;

//mallocs a new CCR2D1 object,
//sets width and height,
//mallocs space for the workers,
//mallocs a wid * hei buffer,
//mallocs space for max_spr sprites,
//mallocs bck and copies it
CCR2D1_API ccr2d1 *c2dnew(pixel *bck, uint wid, uint hei,
		uint max_spr, uint slp, uint max_kel);

//starts the worker threads,
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
		uint wid, uint hei, pixel *pxl);

//moves the sprite with the given id relative to its current position
CCR2D1_API void c2dsprmvr(ccr2d1 *obj, uint sid, uint x, uint y);

//moves the sprite with the given id to the given absolute position
#define c2dsprmva(obj, sid, new_x, new_y) \
	(obj)->spr[(sid)].x = (new_x); \
	(obj)->spr[(sid)].y = (new_y);

//check if the two sprites collide
CCR2D1_API colpos c2dchkcol(ccr2d1 *obj, uint sid1, uint sid2);

//adds a new key event listener to the obj's kel
CCR2D1_API void c2dkeladd(ccr2d1 *obj, kel ltr);

//a memset for pixel arrays
CCR2D1_API void pxlset(pixel *ptr, str dty, ulong num);

//a version of memcpy respecting the type of dest and src
#define typcpy(dest, src, n) \
	for (ulong i = 0; i < n; i++) dest[i] = src[i]

//a version of memcpy specifically and only for pixel arrays
#define pxlcpy(dest, src, n) typcpy(dest, src, n)

//a version of memcpy specifically and only for sprite arrays
#define sprcpy(dest, src, n) typcpy(dest, src, n)

//creates a new thread that runs func with arg and returns it
CCR2D1_API thread thread_create(tstart func, void *arg);

//usually the first function called by a new thread,
//sets canceltype for the thread if not on windows,
//casts arg to a ccr2d1 ptr and returns it
#define setup_thread(arg) _TSETUP_CANCEL_TYPE(); ccr2d1 *obj = (ccr2d1*)arg

//mallocs a new pixel array that's indexed [x][y]
CCR2D1_API pixel **pxlarr2dmallocxy(uint wid, uint hei);

//frees the given pixel array that's indexed [x][y]
CCR2D1_API void pxlarr2dfreexy(pixel **pxl, uint wid);

//load picture - reads a picture from stream into buffer and sets
//*width to the width and *height to the height
CCR2D1_API void c2dldp(FILE *stream, pixel *buffer, uint *width, uint *height);

CCR2D1_API void c2ddefaultalpha2dnsty(unsigned short alpha, char *dnsty);

typedef void (*alpha2dnstyfunc) (unsigned short, pixel *);

alpha2dnstyfunc alpha2dnsty = c2ddefaultalpha2dnsty;
