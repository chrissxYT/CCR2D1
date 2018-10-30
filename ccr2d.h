#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>
typedef HANDLE thread;
typedef LPTHREAD_START_ROUTINE tstart;
#else
#define WIN 0
#include <unistd.h>
#include <pthread.h>
typedef pthread_t thread;
typedef void *(*tstart) (void *);
#endif

typedef unsigned uint;
typedef unsigned long ulong;
typedef char *str;
typedef void(*kel) (int);

//using const for many string constants
//because this only allocs once in the binary
//and not once for every pixel

//All the color codes for the pixels
const str C_RESET = "\e[0m";
const str C_BOLD = "\e[1m";
const str C_DIM = "\e[2m";
const str C_BLINK = "\e[5m";
const str C_HIDDEN = "\e[8m";
const str C_BLACK = "\e[30m";
const str C_RED = "\e[31m";
const str C_GREEN = "\e[32m";
const str C_YELLOW = "\e[33m";
const str C_BLUE = "\e[34m";
const str C_MAGENTA = "\e[35m";
const str C_CYAN = "\e[36m";
const str C_LIGHT_GRAY = "\e[37m";
const str C_DARK_GRAY = "\e[90m";
const str C_LIGHT_RED = "\e[91m";
const str C_LIGHT_GREEN = "\e[92m";
const str C_LIGHT_YELLOW = "\e[93m";
const str C_LIGHT_BLUE = "\e[94m";
const str C_LIGHT_MAGENTA = "\e[95m";
const str C_LIGHT_CYAN = "\e[96m";
const str C_WHITE = "\e[97m";
const str C_NULL = "";

//move the cursor to (0;0)
const str M_0_0 = "\e[0;0f";

#define D_0 ' '
#define D_1 '+'
#define D_2 '#'
#define D_3 '@'

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
	int x;
	//y position
	int y;
	//array of pixels / the actual image, null means invalid
	pixel *pxl;
} sprite;

//a few rendering and drawing buffers mashed into one struct
typedef struct
{
	//the char array the chars are in the end rendered to
	int **c;
	//the pixel array the pixels are prerendered into
	pixel **p;
} buffer;

//the main object that holds all the information of CCR2D v1
typedef struct
{
	//the currently rendered chars
	buffer bfr;
	//0 if not running
	int run;
	//an array of pthreads: the workers rendering, drawing, etc.
	thread *wkr;
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
ccr2d1 *c2dnew(pixel *bck, ulong wid, ulong hei,
		uint max_spr, uint slp, uint max_kel);

//starts the 2 worker threads,
//sets run to true
void c2dstart(ccr2d1 *obj);

//a combination of a stop and a delete
//sets run to false,
//stops the 2 worker threads,
//frees the space for the 2 workers,
//frees the whole buffer,
//frees the CCR2D1 object
void c2dstop(ccr2d1 *obj);

//adds a new sprite to the obj's spr
void c2dspradd(ccr2d1 *obj, int x, int y, uint pri,
		ulong wid, ulong hei, pixel *pxl);

//adds a new key event listener to the obj's kel
void c2dkeladd(ccr2d1 *obj, kel kel);

//sleeps the thread for the given milliseconds
void sleep_ms(uint ms);

//initialize a new pixel array at the buffer
void pxlarr(ulong len, pixel *bfr);

thread thread_create(tstart tstart, void *arg);

void thread_cancel(thread t);

ccr2d1 *setup_thread(void *arg);

pixel **pxlarr2dmallocxy(ulong wid, ulong hei);

void pxlarr2dfreexy(pixel **pxl, ulong wid);
