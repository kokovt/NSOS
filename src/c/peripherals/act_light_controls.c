#include <peripherals/act_light_controls.h>
#include "peripherals/gpio.h"
#include "utils.h"

/*
 * Pi 400 (upstream Linux DTS): GPIO 42, GPIO_ACTIVE_HIGH → GPSET = LED on.
 * Many Pi 4 Model B boards use the same pin but the LED is wired active-low;
 * if your Pi 4B inverts behaviour, set ACT_LED_ACTIVE_LOW to 1 below.
 */
#ifndef ACT_LED_ACTIVE_LOW
#define ACT_LED_ACTIVE_LOW 0
#endif

/* GPIO 42: FSEL4 bits [8:6], GPSET1/GPCLR1 bit (42 - 32) = 10 */
#define ACT_PIN_MASK (1u << 10)

void init_act_led(uint32_t PBASE) {
	uint32_t r = get32(get_GPFSEL4(PBASE));
	r &= ~(7u << 6);
	r |= (1u << 6); /* 001 = output */
	put32(get_GPFSEL4(PBASE), r);
#if ACT_LED_ACTIVE_LOW
	put32(get_GPSET1(PBASE), ACT_PIN_MASK); /* off: pin high */
#else
	put32(get_GPCLR1(PBASE), ACT_PIN_MASK); /* off: pin low */
#endif
}

void act_led_on(uint32_t PBASE) {
#if ACT_LED_ACTIVE_LOW
	put32(get_GPCLR1(PBASE), ACT_PIN_MASK);
#else
	put32(get_GPSET1(PBASE), ACT_PIN_MASK);
#endif
}

void act_led_off(uint32_t PBASE) {
#if ACT_LED_ACTIVE_LOW
	put32(get_GPSET1(PBASE), ACT_PIN_MASK);
#else
	put32(get_GPCLR1(PBASE), ACT_PIN_MASK);
#endif
}
