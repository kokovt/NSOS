#include "peripherals/gpio.h"

uint32_t get_GPFSEL1() {
    return (PBASE + GPIO_BASE_OFFSET + 0x04);
}

uint32_t get_GPFSEL4() {
    return (PBASE + GPIO_BASE_OFFSET + 0x10);
}

uint32_t get_GPSET0() {
    return (PBASE + GPIO_BASE_OFFSET + 0x1C);
}

uint32_t get_GPSET1() {
    return (PBASE + GPIO_BASE_OFFSET + 0x20);
}

uint32_t get_GPCLR0() {
    return (PBASE + GPIO_BASE_OFFSET + 0x28);
}

uint32_t get_GPCLR1() {
    return (PBASE + GPIO_BASE_OFFSET + 0x2C);
}

uint32_t get_GPPUD() {
    return (PBASE + GPIO_BASE_OFFSET + 0x94);
}

uint32_t get_GPPUDCLK0() {
    return (PBASE + GPIO_BASE_OFFSET + 0x98);
}

uint32_t get_GPPUPPDN0() {
    return (PBASE + GPIO_BASE_OFFSET + 0xE4);
}