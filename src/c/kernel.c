#include <kernel.h>

void animated_fill_loop(void);

void kernel_main(void) {
  mmio_init();
  init_framebuffer();

  drawString("The internet is truly the festering cesspool of hell born from a modern society\nbut even so, i have nowhere else to turn to\ncoz I have trouble getting out of bed in the morning! i dont want to go out!\n i dont want to get a proper job! im scared of other people!\n adults piss me off! and im stupid!\n but I still want everyone to fawn over me!!\n and streamers get fawned over the most rn! so thats why I want to be one!", 00, 300, createColor(255, 255, 255));

  ScreenSize size = get_monitor_resolution();
  char buffer[12];
  itoa(size.width, buffer);
  drawString(buffer, 0, 0, createColor(255, 255, 255));
  itoa(size.height, buffer);
  drawString(buffer, 0, 10, createColor(255, 255, 255));

  while (1) {
  }
}

