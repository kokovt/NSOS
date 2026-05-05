#ifndef	_UART_H
#define	_UART_H

#include <stdint.h>

void uart_init ( uint32_t PBASE );
char uart_recv ( uint32_t PBASE );
void uart_send ( char c, uint32_t PBASE );
void uart_send_string(char* str, uint32_t PBASE);
void uart_send_int(int num, uint32_t PBASE);

#endif  /*_UART_H */