#include <graphics/fill_color.h>
#include <uart.h>
#include <peripherals/base.h>

void fill_color(uint32_t color)
{
	for (uint32_t y = 0; y < fb_height; y++) {
		for (uint32_t x = 0; x < fb_width; x++) {
			draw_pixel((int)x, (int)y, color);
		}
	}
}
