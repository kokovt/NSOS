#include <peripherals/UART0.h>

uint32_t get_UART0_DR(uint32_t PBASE) {
    return (PBASE + 0x00201000);
}

uint32_t get_UART0_FR(uint32_t PBASE) {
    return (PBASE + 0x00201018);
}

uint32_t get_UART0_IBRD(uint32_t PBASE) {
    return (PBASE + 0x00201024);
}

uint32_t get_UART0_FBRD(uint32_t PBASE) {
    return (PBASE + 0x00201028);
}

uint32_t get_UART0_LCRH(uint32_t PBASE) {
    return (PBASE + 0x0020102C);
}

uint32_t get_UART0_CR(uint32_t PBASE) {
    return (PBASE + 0x00201030);
}

uint32_t get_UART0_IMSC(uint32_t PBASE) {
    return (PBASE + 0x00201038);
}

uint32_t get_UART0_ICR(uint32_t PBASE) {
    return (PBASE + 0x00201044);
}