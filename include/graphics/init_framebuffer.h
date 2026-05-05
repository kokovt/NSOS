#ifndef _INIT_FRAMEBUFFER_H
#define _INIT_FRAMEBUFFER_H

#include <stdint.h>
#define PBASE_PI4 0xFE000000u

extern uint32_t *fb_pointer;
extern uint32_t fb_width, fb_height, fb_pitch;


int init_framebuffer(void);
void draw_pixel(int x, int y, int color);
#endif /* _INIT_FRAMEBUFFER_H */
