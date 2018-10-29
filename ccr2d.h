#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//using const for many string constants
//because this only allocs once in the binary
//and not once in every pixel

//All the color codes for the pixels
const char *C_RESET = "\e[0m";
const char *C_BOLD = "\e[1m";
const char *C_DIM = "\e[2m";
const char *C_BLINK = "\e[5m";
const char *C_HIDDEN = "\e[8m";
const char *C_BLACK = "\e[30m";
const char *C_RED = "\e[31m";
const char *C_GREEN = "\e[32m";
const char *C_YELLOW = "\e[33m";
const char *C_BLUE = "\e[34m";
const char *C_MAGENTA = "\e[35m";
const char *C_CYAN = "\e[36m";
const char *C_LIGHT_GRAY = "\e[37m";
const char *C_DARK_GRAY = "\e[90m";
const char *C_LIGHT_RED = "\e[91m";
const char *C_LIGHT_GREEN = "\e[92m";
const char *C_LIGHT_YELLOW = "\e[93m";
const char *C_LIGHT_BLUE = "\e[94m";
const char *C_LIGHT_MAGENTA = "\e[95m";
const char *C_LIGHT_CYAN = "\e[96m";
const char *C_WHITE = "\e[97m";

//move the cursor to (0;0)
const char *M_0_0 = "\e[0;0f";

#define D_0 32
#define D_1 176
#define D_2 177
#define D_3 178

//a single pixel in an image
typedef struct
{
	//the char printed
	char dnsty;
	//ascii color code
	const char *color;
} pixel;

//a sprite displayed on the screen
typedef struct
{
	//priority (0 is lowest, 0xffffffff highest)
	unsigned pri;
	//width in console pixels
	long unsigned wid;
	//height in console pixels
	long unsigned hei;
	//x position
	int x;
	//y position
	int y;
	//array of pixels / the actual image, null means invalid
	pixel *pxl;
} sprite;

//a char[][] the renderer renders into and the drawer displays
typedef struct
{
	//the char array the chars are saved to
	char **c;
} buffer;

//the main object that holds all the information of CCR2D v1
typedef struct
{
	//the currently rendered chars
	buffer bfr;
	//0 if not running
	int run;
	//an array of pthreads: the workers rendering, drawing, etc.
	pthread_t *wkr;
	//the render width
	long unsigned wid;
	//the render height
	long unsigned hei;
	//all the sprites to be drawn
	sprite *spr;
	//sprite count
	unsigned spc;
	//the background
	pixel *bck;
} ccr2d1;

//mallocs a new CCR2D1 object,
//sets width and height,
//mallocs space for 2 workers,
//mallocs a wid * hei buffer,
//mallocs space for max_spr sprites,
//mallocs bck and copies it
ccr2d1 *c2dnew(pixel *bck, long unsigned wid, long unsigned hei,
		unsigned max_spr);

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
void c2dspradd(ccr2d1 *obj, int x, int y, unsigned pri,
		long unsigned wid, long unsigned hei, pixel *pxl);

//sleeps the thread for the given milliseconds
void sleep_ms(unsigned ms);

//initialize a new pixel array at the buffer
void pxlarr(long unsigned wid, long unsigned hei, pixel *bfr);
