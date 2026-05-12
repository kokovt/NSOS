#pragma once

#include <stdint.h>
#define PBASE_PI4 0xFE000000u

#include <cutils.h>
#include <mailbox/mailbox.h>
#include <stddef.h>
#include <stdint.h>
#include <uart.h>

extern uint32_t *fb_pointer;
extern uint32_t fb_width, fb_height, fb_pitch;

typedef struct {
  unsigned int width;
  unsigned int height;
} ScreenSize;

int init_framebuffer(void);
void draw_pixel(int x, int y, int color);
ScreenSize get_monitor_resolution(void);
