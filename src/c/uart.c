#include "peripherals/UART0.h"
#include "peripherals/gpio.h"
#include "utils.h"

int uart_initialized = 0;

void uart_send(char c, uint32_t PBASE) {
  
  if(!uart_initialized) {
    return;
  }

  while (1) {
    if (!(get32(get_UART0_FR(PBASE)) & (1 << 5)))
      break;
  }
  put32(get_UART0_DR(PBASE), c);
}

char uart_recv(uint32_t PBASE) {
  if(!uart_initialized) {
    return 0;
  }

  while (1) {
    if (!(get32(get_UART0_FR(PBASE)) & (1 << 4)))
      break;
  }
  return (get32(get_UART0_DR(PBASE)) & 0xFF);
}

void uart_send_string(char *str, uint32_t PBASE) {
  if(!uart_initialized) {
    return;
  }

  
  for (int i = 0; str[i] != '\0'; i++) {
    uart_send((char)str[i], PBASE);
  }
}

void uart_init(uint32_t PBASE) {
  unsigned int selector;

  selector = get32(get_GPFSEL1(PBASE));
  selector &= ~(7 << 12);
  selector |= 4 << 12;
  selector &= ~(7 << 15);
  selector |= 4 << 15;
  put32(get_GPFSEL1(PBASE), selector);

  put32(get_GPPUD(PBASE), 0);
  delay(150);
  put32(get_GPPUDCLK0(PBASE), (1 << 14) | (1 << 15));
  delay(150);
  put32(get_GPPUDCLK0(PBASE), 0);
  put32(get_UART0_CR(PBASE), 0);      // Disable UART during config
  put32(get_UART0_ICR(PBASE), 0x7FF); // Clear pending interrupts
  put32(get_UART0_IBRD(PBASE), 26);   // 115200 baud @ 48MHz clock
  put32(get_UART0_FBRD(PBASE), 3);
  put32(get_UART0_LCRH(PBASE),
        (1 << 4) | (1 << 5) | (1 << 6)); // Enable FIFO + 8-bit words
  put32(get_UART0_IMSC(PBASE), 0);       // Mask all interrupts
  put32(get_UART0_CR(PBASE),
        (1 << 0) | (1 << 8) | (1 << 9)); // Enable UART, RX, TX

  uart_initialized = 1;
}

void uart_send_int(int num, uint32_t PBASE) {
  if(!uart_initialized) {
    return;
  }
  
  char buf[12];
  int i = 0;

  if(num == 0) {
    uart_send('0', PBASE);
    return;
  }

  if(num < 0) {
    uart_send('-', PBASE);
    num = -num;
  }

  while(num > 0) {
    buf[i++] = (num % 10) + '0';
    num /= 10;
  }

  while(i > 0) {
    uart_send(buf[--i], PBASE);
  }
}