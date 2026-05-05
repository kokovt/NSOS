#include <mmio/mmio.h>

uint32_t mmio_read(uint32_t reg, uint32_t PBASE) {
    return *(volatile uint32_t*)(PBASE + reg);
}

void mmio_write(uint32_t reg, uint32_t value, uint32_t PBASE) {
    *(volatile uint32_t *)(PBASE + reg) = value;
}