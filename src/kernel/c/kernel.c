#include <kernel.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__aarch64__) || defined(__arm__)
#include <uart.h>

void animated_fill_loop(void);
#elif defined(_USE_LIMINE_)
#include <limine.h>
// ----- ATTRIBUTES -----
__attribute__((
    used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(6);

__attribute__((used,
               section(".limine_requests_"
                       "start"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used,
               section(".limine_requests_"
                       "end"))) static volatile LIMINE_REQUESTS_END_MARKER;

static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}
#endif

void kernel_main(void) {
#ifdef _USE_LIMINE_
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    hcf();
  }
#endif
#ifndef _USE_LIMINE_
  mmio_init();
  uart_init(9600);

#endif
  init_framebuffer();

  set_active_font_by_index(KERNEL_FONT_PIXEL_MPLUS);

  ScreenSize size = get_monitor_resolution();

  uint32_t color = 0xA9A7AA;
  drawString("Needy Streamer Overload\nCopyright (C) 2020-\n\nNGO BIOS "
             "Rev1.0\n\nMain Processor : raincandy\nMemory Testing: OK",
             0, 0, color);
  drawString("Booting Windose20", 0, fb_height - 10, color);

  drawString("NSOS - Copyright (C) 2026 Kokovt/Onyx", fb_width - 320,
             size.height - 40, color);
  drawString("Based upon NSO", fb_width - 320, fb_height - 30, color);
  drawString("This project is not affiliated with WSS", fb_width - 320,
             size.height - 20, color);
  drawString("Playground or Microsoft", fb_width - 320, fb_height - 10, color);

#if defined(__aarch64__) || defined(__arm__)
  uart_writeText("Testing the UART output!");
  while (1) {
    uart_update();
    //? Check for input
    if (uart_isReadByteReady()) {
      char ch = uart_readByte();
      if (ch == 'K') {
        uart_readByte(); // Ignore the comma.
        char next_char = uart_readByte();
        if (!uart_isReadByteReady()) {
          drawChar(next_char, 500, 500, 0xFFFFFF);
        }
      }
    }
  }

  // _USE_LIMINE_ is a compile define I am going to use for x86-64
#elif defined(_USE_LIMINE_)
  hcf();
#endif
}
