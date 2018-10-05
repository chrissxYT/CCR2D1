#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//using constexpr for many string constants
//because this only allocs once in the binary
//and not once in every pixel

//All the color codes for the pixels
constexpr char *C_RESET = "\e[0m";
constexpr char *C_BOLD = "\e[1m";
constexpr char *C_DIM = "\e[2m";
constexpr char *C_BLINK = "\e[5m";
constexpr char *C_HIDDEN = "\e[8m";
constexpr char *C_BLACK = "\e[30m";
constexpr char *C_RED = "\e[31m";
constexpr char *C_GREEN = "\e[32m";
constexpr char *C_YELLOW = "\e[33m";
constexpr char *C_BLUE = "\e[34m";
constexpr char *C_MAGENTA = "\e[35m";
constexpr char *C_CYAN = "\e[36m";
constexpr char *C_LIGHT_GRAY = "\e[37m";
constexpr char *C_DARK_GRAY = "\e[90m";
constexpr char *C_LIGHT_RED = "\e[91m";
constexpr char *C_LIGHT_GREEN = "\e[92m";
constexpr char *C_LIGHT_YELLOW = "\e[93m";
constexpr char *C_LIGHT_BLUE = "\e[94m";
constexpr char *C_LIGHT_MAGENTA = "\e[95m";
constexpr char *C_LIGHT_CYAN = "\e[96m";
constexpr char *C_WHITE = "\e[97m";

//move the cursor to 0;0
constexpr char *M_0_0 = "\e[0;0f";

//no sprite
#define NOSPR -1

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
	//priority (0 is lowest, int_max highest, <0 means invalid)
	int pri;
	//width in console pixels, <0 means invalid sprite
	int wid;
	//height in console pixels, <0 means invalid sprite
	int hei;
	//x position
	int x;
	//y position
	int y;
	//array of pixels / the actual image, null means invalid
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
	//all the sprites to be drawn
	sprite *spr;
	//sprite count
	int spc;
	//the background
	pixel *bck;
} ccr2d1;

//mallocs a new CCR2D1 object,
//sets width and height,
//mallocs space for 2 workers,
//mallocs a wid * hei buffer,
//mallocs space for max_spr sprites,
//mallocs bck and copies it
struct ccr2d1 *c2dnew(pixel *bck, int wid, int hei, int max_spr);

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

void c2dspradd(struct ccr2d1 *obj, int x, int y,
	int pri, int wid, int hei, pixel *pxl);

//sleeps the thread for the given milliseconds
void sleep_ms(int ms);
