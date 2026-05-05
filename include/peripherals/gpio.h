#ifndef	_P_GPIO_H
#define	_P_GPIO_H

#include <stdint.h>

#define GPIO_BASE_OFFSET 0x200000
uint32_t get_GPFSEL1(uint32_t PBASE);
uint32_t get_GPSET0(uint32_t PBASE);
uint32_t get_GPSET1(uint32_t PBASE);
uint32_t get_GPCLR0(uint32_t PBASE);
uint32_t get_GPCLR1(uint32_t PBASE);
uint32_t get_GPPUD(uint32_t PBASE);
uint32_t get_GPPUDCLK0(uint32_t PBASE);
uint32_t get_GPPUPPDN0(uint32_t PBASE);
uint32_t get_GPFSEL4(uint32_t PBASE);

#endif  /*_P_GPIO_H */