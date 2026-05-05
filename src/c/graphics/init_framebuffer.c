#include "peripherals/base.h"
#include <graphics/init_framebuffer.h>
#include <mailbox/mailbox.h>
#include <stddef.h>
#include <stdint.h>
#include <uart.h>

volatile unsigned int __attribute__((aligned(16))) fb_mailbox[36];

uint32_t *fb_pointer;
uint32_t fb_width, fb_height, fb_pitch;

unsigned int isrgb;
uint32_t fb_isrgb = 0;
unsigned char *fb;


int init_framebuffer()
{
  fb_mailbox[0] = 35*4; // Length of message in bytes
  fb_mailbox[1] = MBOX_REQUEST;

  fb_mailbox[2] = MBOX_TAG_SETPHYWH; // Tag identifier
  fb_mailbox[3] = 8; // Value size in bytes
  fb_mailbox[4] = 0;
  fb_mailbox[5] = 1920; // Value(width)
  fb_mailbox[6] = 1080; // Value(height)
  
  fb_mailbox[7] = MBOX_TAG_SETVIRTWH;
  fb_mailbox[8] = 8;
  fb_mailbox[9] = 8;
  fb_mailbox[10] = 1920;
  fb_mailbox[11] = 1080;
  
  fb_mailbox[12] = MBOX_TAG_SETVIRTOFF;
  fb_mailbox[13] = 8;
  fb_mailbox[14] = 8;
  fb_mailbox[15] = 0; // Value(x)
  fb_mailbox[16] = 0; // Value(y)
  
  fb_mailbox[17] = MBOX_TAG_SETDEPTH;
  fb_mailbox[18] = 4;
  fb_mailbox[19] = 4;
  fb_mailbox[20] = 32; // Bits per pixel
  
  fb_mailbox[21] = MBOX_TAG_SETPXLORDR;
  fb_mailbox[22] = 4;
  fb_mailbox[23] = 4;
  fb_mailbox[24] = 1; // RGB
  
  fb_mailbox[25] = MBOX_TAG_GETFB;
  fb_mailbox[26] = 8;
  fb_mailbox[27] = 8;
  fb_mailbox[28] = 4096; // FrameBufferInfo.pointer
  fb_mailbox[29] = 0;    // FrameBufferInfo.size
  
  fb_mailbox[30] = MBOX_TAG_GETPITCH;
  fb_mailbox[31] = 4;
  fb_mailbox[32] = 4;
  fb_mailbox[33] = 0; // Bytes per line
  
  fb_mailbox[34] = MBOX_TAG_LAST;
  
  uint32_t PBASE = get_PBASE();
  
  mailbox_write((uintptr_t)fb_mailbox, MBOX_CH_PROP, PBASE);
  // #ifdef __aarch64__
  // Check call is successful and we have a pointer with depth 32
  if (mailbox_read(MBOX_CH_PROP, PBASE) && fb_mailbox[20] == 32 && fb_mailbox[28] != 0) {
      fb_mailbox[28] &= 0x3FFFFFFF; // Convert GPU address to ARM address
      fb_width = fb_mailbox[10];       // Actual physical width
      fb_height = fb_mailbox[11];      // Actual physical height
      fb_pitch = fb_mailbox[33];       // Number of bytes per line
      isrgb = fb_mailbox[24];       // Pixel order
      fb = (unsigned char *)((long)fb_mailbox[28]);

      return 0;
  }

  return -1;
}

void draw_pixel(int x, int y, int color)
{
    int offs = (y * fb_pitch) + (x * 4);
    *((unsigned int*)(fb + offs)) = color;
}
