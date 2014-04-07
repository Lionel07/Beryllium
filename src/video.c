#include <log.h>
#include <mutex.h>
#ifdef X86
#include <x86/drivers/textmode.h>
#include <x86/drivers/bga.h>
#endif
#ifdef ARM
#include <arm/intergrator-cp/drivers/qemu-PL110.h>
#endif

int video_device = 0; //Textmode x86
int video_graphics_init() //returns 0 if failed, 1 if sucessfull
{
	//bga_init();
	return 0;
}

int video_graphics_capable()
{
	#ifdef X86
	return !bga_isavalable();
	#else
	return 1;
	#endif
	return 0;
}

void video_printchar(int x,int y, unsigned char c)
{
	#ifdef X86
	textmode_write(x,y,c);
	#else
	gterminal_draw_char(x, y, c);
	#endif
}

void video_printcoloredchar(int x,int y, unsigned char c, unsigned char attribute)
{
	#ifdef X86
	textmode_write_color(x,y,c, attribute);
	#endif
}
void video_scroll(int from,int to)
{
	#ifdef X86
	textmode_scroll(from,to);
	#endif
}

void video_setcursor(int x,int y)
{
	#ifdef X86
	textmode_setcursor(x,y);
	#endif
}

void video_setattributetext(unsigned char back, unsigned char fore)
{
	#ifdef X86
	textmode_setcolor(back,fore);
	#endif
}
void video_resetattributetext()
{
	#ifdef X86
	textmode_resetcolor();
	#endif
}

void video_draw_pixel(int x, int y, uint8_t r,uint8_t g,uint8_t b)
{
	#ifdef X86

	#else
	qemu_pl110_write(x,y,r,g,b);

	#endif
}

void video_drawrect(int x,int y, int xx, int yy, uint8_t r,uint8_t g,uint8_t b)
{

}