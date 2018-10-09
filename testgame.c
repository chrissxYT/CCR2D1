#include "ccr2d.h"

#define wid 20
#define hei 10

int main(int argc, char **argv)
{
	struct pixel bck[wid * hei];
	pxlarr(wid, hei, bck);
	struct ccr2d1 *obj = c2dnew(bck, 20, 10, 10);
	c2dstart(obj);
	while(1)
		//if is_amd64 || is_x86
		//	run pause instruction to save power
		//	jmp to while statement
		//else
		//	jmp to while statement
#if defined(__amd64__) || defined(__amd64) || defined(_M_X64) || \
		defined(_M_AMD64) || defined(i386) || defined(\
				__i386) || defined(_M_I86) || \
		defined(_M_IX86) || defined(_X86_) || defined(\
				__X86__) || defined(__ia64__) || \
		defined(_IA64) || defined(__IA64__) || defined(\
				__ia64) || defined(_M_IA64)
		__asm__("pause");
#else
		;
}
