#if defined(__aarch64__) || defined(__arm__)
#include "graphics/basic_graphics.h"
#include <kernel.h>
#include <uart.h>

void animated_fill_loop(void);
#elif defined(_USE_LIMINE_)
#include <limine.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
// ----- ATTRIBUTES -----
__attribute__((
    used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

__attribute__((
    used,
    section(".limine_requests"))) static volatile struct limine_hhdm_request
    hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};

__attribute((used, section(".limine_requests"))) struct limine_memmap_request
    memmap_request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

__attribute((used,
             section(".limine_requests"))) struct limine_kernel_address_request
    kernel_address_request = {.id = LIMINE_KERNEL_ADDRESS_REQUEST,
                              .revision = 0};

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
#if defined(__aarch64__) || defined(__arm__)
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
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    hcf();
  }

  // Ensure we got a framebuffer.
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    hcf();
  }

  // Fetch the first framebuffer.
  struct limine_framebuffer *framebuffer =
      framebuffer_request.response->framebuffers[0];

  // Print a nice pattern to screen as an example.
  // Note: we assume the framebuffer model is RGB with 32-bit pixels.
  volatile uint32_t *fb_ptr = framebuffer->address;
  for (size_t y = 0; y < framebuffer->height; y++) {
    for (size_t x = 0; x < framebuffer->width; x++) {
      uint32_t nX = x * 255 / framebuffer->width;
      uint32_t nY = y * 255 / framebuffer->height;
      fb_ptr[y * (framebuffer->pitch / 4) + x] = (nY << 8) | nX;
    }
  }

  // We're done, just hang...
  hcf();
#endif
}
