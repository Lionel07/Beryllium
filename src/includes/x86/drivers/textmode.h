//Low Level Display Drivers
#ifndef LOW_TEXTMODE_H
#define LOW_TEXTMODE_H
#include <types.h>
///Write a char to the screen
void textmode_write(int x,int y,uint8_t data);
///Write a colored char to the screen
void textmode_write_color(int x,int y,uint8_t data, uint8_t attr);
///Read a char from the screen
uint8_t textmode_read(int x,int y);
///Scroll
void textmode_scroll(int from,int to);
///Set cursor position
void textmode_setcursor(int x,int y);
///Set attribute byte
void textmode_setcolor(uint8_t back,uint8_t fore);
///Reset attribute byte
void textmode_resetcolor();
#endif
