#include <mmio/mmio.h>

uint32_t PBASE;

void mmio_init(void) {
    PBASE = get_PBASE();
}
uint32_t mmio_read(uint32_t reg) {
    return *(volatile uint32_t*)(uintptr_t)(reg);
}

void mmio_write(uint32_t reg, uint32_t value) {
    *(volatile uint32_t *)(uintptr_t)(reg) = value;
}   