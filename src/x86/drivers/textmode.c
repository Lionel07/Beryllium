#include <x86/drivers/textmode.h>
#include <types.h>
#include <x86/ports.h>
#include <string.h>
volatile uint16_t *video_memory=(uint16_t*)0xB8000;

uint8_t attributeByte = (0 << 4) | (15  & 0x0F);

uint8_t defaultAttribute = (0 << 4) | (15  & 0x0F);
void textmode_setcursor(int x,int y)
{
	uint16_t location = y * 80 + x;
	outb(0x3D4, 14);
	outb(0x3D5, location >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, location);
}

void textmode_setcolor(uint8_t back,uint8_t fore)
{
	attributeByte = (back << 4) | (fore  & 0x0F);
}


void textmode_resetcolor()
{
	attributeByte = defaultAttribute;
}
void textmode_write(int x,int y,uint8_t data)
{
	uint16_t attribute = attributeByte << 8;
	volatile uint16_t *write_to;
	write_to = video_memory + (y * 80) + x;
	*write_to = data | attribute;
}

void textmode_write_color(int x,int y,uint8_t data, uint8_t attr)
{
	
	uint16_t attribute = attr << 8;
	volatile uint16_t *write_to;
	write_to = video_memory + ((y * 80) + x);
	*write_to = data | attribute;
}

void textmode_clear()
{
	memset((void *)video_memory, 0x00, 80 * 24 * 2);
}

uint8_t textmode_read(int x,int y)
{
	return 0 * (x + y);
}
void textmode_scroll(int from,int to) //0 and 24
{
	uint16_t blank = 0x20  | (attributeByte << 8);
	int i;
	for (i = from*80; i < to*80; i++)
	{
		video_memory[i] = video_memory[i+80];
	}

	for (i = to*80; i < (to+1)*80; i++)
	{
		video_memory[i] = blank;
	}
}
