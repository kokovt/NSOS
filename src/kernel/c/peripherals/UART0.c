#include <peripherals/UART0.h>
#include <mmio/mmio.h>


uint32_t get_UART0_DR() {
    return (PBASE + 0x00201000);
}

uint32_t get_UART0_FR() {
    return (PBASE + 0x00201018);
}

uint32_t get_UART0_IBRD() {
    return (PBASE + 0x00201024);
}

uint32_t get_UART0_FBRD() {
    return (PBASE + 0x00201028);
}

uint32_t get_UART0_LCRH() {
    return (PBASE + 0x0020102C);
}

uint32_t get_UART0_CR() {
    return (PBASE + 0x00201030);
}

uint32_t get_UART0_IMSC() {
    return (PBASE + 0x00201038);
}

uint32_t get_UART0_ICR() {
    return (PBASE + 0x00201044);
}