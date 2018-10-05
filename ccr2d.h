#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

//move the cursor to 0;0
#define M_0_0 "\e[0;0f"

//a single pixel in an image
struct pixel
{
	//the char printed
	char dnsty;
	//ascii color code
	char *color;
} pixel;

//a sprite displayed on the screen
struct sprite
{
	//width in console pixels
	int wid;
	//height in console pixels
	int hei;
	//array of pixels / the actual image
	struct pixel *pxl;
} sprite;

//a char[][] the renderer renders into and the drawer displays
struct buffer
{
	//the char array the chars are saved to
	char **c;
} buffer;

//the main object that holds all the information of CCR2D v1
struct ccr2d1
{
	//the currently rendered chars
	struct buffer bfr;
	//true if started false if not
	int run;
	//an array of pthreads: the workers rendering, drawing, etc.
	pthread_t *wkr;
	//the render width
	int wid;
	//the render height
	int hei;
} ccr2d1;

//mallocs a new CCR2D1 object,
//sets width and height,
//mallocs space for 2 workers,
//mallocs a wid * hei buffer
struct ccr2d1 *c2dnew(int wid, int hei);

//starts the 2 worker threads,
//sets run to true
void c2dstart(struct ccr2d1 *obj);

//a combination of a stop and a delete
//sets run to false,
//stops the 2 worker threads,
//frees the space for the 2 workers,
//frees the whole buffer,
//frees the CCR2D1 object
void c2dstop(struct ccr2d1 *obj);

//sleeps the thread for the given milliseconds
void sleep_ms(int ms);
