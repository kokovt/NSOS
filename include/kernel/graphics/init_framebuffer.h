#pragma once

#include <cutils.h>
#include <stddef.h>
#include <stdint.h>
#ifdef _USE_LIMINE_
#include <limine.h>
#else
#include <mailbox/mailbox.h>
#include <uart.h>
#endif
extern volatile uint32_t *fb_pointer;
extern uint32_t fb_width, fb_height, fb_pitch;

int init_framebuffer(void);
void draw_pixel(int x, int y, int color);

typedef struct {
  unsigned int width;
  unsigned int height;
} ScreenSize;

ScreenSize get_monitor_resolution(void);
