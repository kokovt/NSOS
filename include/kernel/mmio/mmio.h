#pragma once

#include <stdint.h>
#include <peripherals/base.h>

extern uint32_t PBASE;

void mmio_init(void);
void mmio_write(uint32_t reg, uint32_t value);
uint32_t mmio_read(uint32_t reg);
