#pragma once

#include <graphics/init_framebuffer.h>
#include <stdint.h>
#include <fonts/font_info.h>
#include <fonts/basic_font.h>
#include <fonts/PixelMPlus.h>

enum {
    KERNEL_FONT_BASIC = 0,
    KERNEL_FONT_PIXEL_MPLUS = 1,
    KERNEL_FONT_COUNT = 2,
};

extern const font_info_t *const kernel_fonts[KERNEL_FONT_COUNT];

int createColor(int red, int green, int blue);
void drawRect(int x1, int y1, int x2, int y2, uint32_t color, int fill);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
void drawCircle(int x1, int y1, int radius, uint32_t color, int fill);
void drawChar(unsigned char ch, int x, int y, uint32_t color);
void drawString(char *s, int x, int y, uint32_t color);

void set_active_font(const font_info_t *f);
void set_active_font_by_index(unsigned i);
