#ifndef _MMIO_H
#define _MMIO_H

#include <stdint.h>


void mmio_write(uint32_t reg, uint32_t value, uint32_t PBASE);

uint32_t mmio_read(uint32_t reg, uint32_t PBASE);

#endif /* _MMIO_H */