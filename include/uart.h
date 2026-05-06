#ifndef _UART_H
#define _UART_H

#include <stdint.h>

void uart_init();
char uart_recv();
void uart_send(char c);
int uart_send_string(char *str);
void uart_send_int(int num);

#endif /*_UART_H */