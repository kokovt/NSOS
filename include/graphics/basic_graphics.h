#ifndef BASIC_GRAPHICS_H
#define BASIC_GRAPHICS_H

#include <graphics/init_framebuffer.h>
#include <stdint.h>
#include <fonts/basic_font.h>

int createColor(int red, int green, int blue);
void drawRect(int x1, int y1, int x2, int y2, uint32_t color, int fill);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
void drawCircle(int x1, int y1, int radius, uint32_t color, int fill);
void drawChar(unsigned char ch, int x, int y, uint32_t color);
void drawString(char *s, int x, int y, uint32_t color);

#endif