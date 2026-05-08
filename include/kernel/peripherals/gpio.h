#ifndef	_P_GPIO_H
#define	_P_GPIO_H

#include <stdint.h>
#include <mmio/mmio.h>

#define GPIO_BASE_OFFSET 0x200000
#define GPFSEL0 (PBASE + GPIO_BASE_OFFSET)
#define GPFSET0 (PBASE + GPIO_BASE_OFFSET + 0x1C)
#define GPFCLR0 (PBASE + GPIO_BASE_OFFSET + 0x28)
#define GPPUPPDN0 (PBASE + GPIO_BASE_OFFSET + 0xE4)

unsigned int gpio_call(unsigned int pin_number, unsigned int value, unsigned int base, unsigned int field_size, unsigned int field_max);

unsigned int gpio_set(unsigned int pin_number, unsigned int value);
unsigned int gpio_clear(unsigned int pin_number, unsigned int value);
unsigned int gpio_pull(unsigned int pin_number, unsigned int value);
unsigned int gpio_function(unsigned int pin_number, unsigned int value);

void gpio_useAsAlt3(unsigned int pin_number);
void gpio_useAsAlt5(unsigned int pin_number);

void gpio_initOutputPinWithPullNone(unsigned int pin_number);
void gpio_setPinOutputBool(unsigned int pin_number, unsigned int onOrOff);
#endif  /*_P_GPIO_H */