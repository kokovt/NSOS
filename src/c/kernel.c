#include <peripherals/act_light_controls.h>
#include <uart.h>
#include <utils.h>
#include <peripherals/base.h>
#include <graphics/init_framebuffer.h>
#include <graphics/fill_color.h>

void animated_fill_loop(void);

void kernel_main(void)
{
	uint32_t PBASE = get_PBASE();


	// uart_init(PBASE);
	uart_send_string("Initializing framebuffer...\n", PBASE);

	int framebuffer_result = init_framebuffer();
	if (framebuffer_result != 0) {
		uart_send_string("Framebuffer init failed\n", PBASE);
	}

	if(framebuffer_result == 0) {
		uart_send_string("Framebuffer initialized\n", PBASE);
	}

	while (1) {
		animated_fill_loop();
		// uart_send(uart_recv(PBASE), PBASE);
	}
}

void animated_fill_loop(void) {
	uint32_t PBASE = get_PBASE();
	uart_send_string("Animated fill loop\n", PBASE);
	while (1) {
		fill_color(0xFFFFFF);

		while(1) {}
	}
}