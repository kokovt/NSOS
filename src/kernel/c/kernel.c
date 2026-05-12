#include "graphics/basic_graphics.h"
#include <kernel.h>
#include <uart.h>

void animated_fill_loop(void);

void kernel_main(void) {
  mmio_init();
  uart_init(9600);
  init_framebuffer();

  set_active_font_by_index(KERNEL_FONT_PIXEL_MPLUS);

  ScreenSize size = get_monitor_resolution();

  uint32_t color = 0xA9A7AA;
  drawString("Needy Streamer Overload\nCopyright (C) 2020-\n\nNGO BIOS "
             "Rev1.0\n\nMain Processor : raincandy\nMemory Testing: OK",
             0, 0, color);
  drawString("Booting Windose20", 0, size.height - 10, color);

  drawString("NSOS - Copyright (C) 2026 Kokovt/Onyx", size.width - 320,
             size.height - 40, color);
  drawString("Based upon NSO", size.width - 320, size.height - 30, color);
  drawString("This project is not affiliated with WSS", size.width - 320,
             size.height - 20, color);
  drawString("Playground or Microsoft", size.width - 320, size.height - 10,
             color);

  uart_writeText("Testing the UART output!");
  while (1) {
    uart_update();
    //? Check for input
    if(uart_isReadByteReady()) {
      char ch = uart_readByte();
      if(ch == 'K') {
        uart_readByte(); // Ignore the comma.
        char next_char = uart_readByte();
        if(!uart_isReadByteReady()) {
          drawChar(next_char, 500, 500, 0xFFFFFF);
        }
      }
    }
  }
}