#ifndef _P_UART0_H
#define _P_UART0_H

#include <stdint.h>

uint32_t get_UART0_DR();
uint32_t get_UART0_FR();
uint32_t get_UART0_IBRD();
uint32_t get_UART0_FBRD();
uint32_t get_UART0_LCRH();
uint32_t get_UART0_CR();
uint32_t get_UART0_IMSC();
uint32_t get_UART0_ICR();
#endif /* _P_UART0_H */