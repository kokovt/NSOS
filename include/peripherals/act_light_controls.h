#ifndef ACT_LIGHT_CONTROLS_H
#define ACT_LIGHT_CONTROLS_H

#include <stdint.h>

/*
 * Raspberry Pi 400: single LED on GPIO 42 (Linux DTS: GPIO_ACTIVE_HIGH).
 * Pi 4 Model B: often the same pin but sometimes wired active-low — see
 * ACT_LED_ACTIVE_LOW in act_light_controls.c if on/off are inverted.
 */
void init_act_led(uint32_t PBASE);
void act_led_on(uint32_t PBASE);
void act_led_off(uint32_t PBASE);

#endif