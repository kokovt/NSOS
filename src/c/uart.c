#include "peripherals/UART0.h"
#include "peripherals/gpio.h"
#include "utils.h"

int uart_initialized = 0;

void uart_send(char c) {

  if (!uart_initialized) {
    return;
  }

  while (get32(get_UART0_FR()) & (1 << 5)) {
  }
  put32(get_UART0_DR(), c);
}
char uart_recv() {
  if (!uart_initialized) {
    return 0;
  }

  while (get32(get_UART0_FR()) & (1 << 4)) {
  }
  return (get32(get_UART0_DR()) & 0xFF);
}

int uart_send_string(char *str) {
  if (!uart_initialized) {
    return 0;
  }

  for (int i = 0; str[i] != '\0'; i++) {
    uart_send((char)str[i]);
  }

  return uart_initialized;
}
void uart_init() {
  unsigned int selector;

  selector = get32(get_GPFSEL1());
  selector &= ~(7 << 12);
  selector |= 4 << 12;
  selector &= ~(7 << 15);
  selector |= 4 << 15;
  put32(get_GPFSEL1(), selector);

  put32(get_GPPUD(), 0);
  delay(150);
  put32(get_GPPUDCLK0(), (1 << 14) | (1 << 15));
  delay(150);
  put32(get_GPPUDCLK0(), 0);
  put32(get_UART0_CR(), 0);      // Disable UART during config
  put32(get_UART0_ICR(), 0x7FF); // Clear pending interrupts
  put32(get_UART0_IBRD(), 26);   // 115200 baud @ 48MHz clock
  put32(get_UART0_FBRD(), 3);
  put32(get_UART0_LCRH(),
        (1 << 4) | (1 << 5) | (1 << 6)); // Enable FIFO + 8-bit words
  put32(get_UART0_IMSC(), 0);            // Mask all interrupts
  put32(get_UART0_CR(),
        (1 << 0) | (1 << 8) | (1 << 9)); // Enable UART, RX, TX

  uart_initialized = 1;
}

void uart_send_int(int num) {
  if (!uart_initialized) {
    return;
  }

  char buf[12];
  int i = 0;

  if (num == 0) {
    uart_send('0');
    return;
  }

  if (num < 0) {
    uart_send('-');
    num = -num;
  }

  while (num > 0) {
    buf[i++] = (num % 10) + '0';
    num /= 10;
  }

  while (i > 0) {
    uart_send(buf[--i]);
  }
}