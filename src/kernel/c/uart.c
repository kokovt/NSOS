#ifndef _USE_LIMINE_
// TODO Rewrite to use the mini-uart instead of the full UART
//? The reason for this is that the full UART disables the bluetooth support.
//? I would, in fact, like to have bluetooth in the future.

//? This heavily references
// https://github.com/sypstraw/rpi4-osdev/blob/master/part4-miniuart/io.c ? Its
// a cool reference, and you should check it out.

#include <cutils.h>
#include <graphics/basic_graphics.h>
#include <uart.h>

unsigned char uart_output_queue[UART_MAX_QUEUE];
unsigned int uart_output_queue_write = 0;
unsigned int uart_output_queue_read = 0;

void uart_init(unsigned int baud) {

  gpio_useAsAlt5(14);
  gpio_useAsAlt5(15);
  mmio_write(AUX_ENABLES, 1); // enable UART1
  mmio_write(AUX_MU_IER_REG, 0);
  mmio_write(AUX_MU_CNTL_REG, 0);
  mmio_write(AUX_MU_LCR_REG, 3); // 8 bits
  mmio_write(AUX_MU_MCR_REG, 0);
  mmio_write(AUX_MU_IER_REG, 0);
  mmio_write(AUX_MU_IIR_REG, 0xC6); // disable interrupts
  mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(baud));
  mmio_write(AUX_MU_CNTL_REG, 3); // enable RX/TX
}

unsigned int uartIsOutputQueueEmpty() {
  return (uart_output_queue_read == uart_output_queue_write);
}

unsigned int uart_isReadByteReady() { return mmio_read(AUX_MU_LSR_REG) & 0x01; }

unsigned int uart_isWriteByteReady() {
  return mmio_read(AUX_MU_LSR_REG) & 0x20;
}

unsigned char uart_readByte() {
  while (!uart_isReadByteReady())
    ;
  return (unsigned char)mmio_read(AUX_MU_IO_REG);
}

void uart_writeByteBlockingActual(unsigned char ch) {
  while (!uart_isWriteByteReady())
    ;
  mmio_write(AUX_MU_IO_REG, ch);
}

void uart_loadOutputFifo() {
  while (!uartIsOutputQueueEmpty() && uart_isWriteByteReady()) {
    uart_writeByteBlockingActual(uart_output_queue[uart_output_queue_read]);
    uart_output_queue_read =
        (uart_output_queue_read + 1) & (UART_MAX_QUEUE - 1);
  }
}

void uart_writeByteBlocking(unsigned char ch) {
  unsigned int next = (uart_output_queue_write + 1) & (UART_MAX_QUEUE - 1);

  while (next == uart_output_queue_read)
    uart_loadOutputFifo();

  uart_output_queue[uart_output_queue_write] = ch;
  uart_output_queue_write = next;
}

void uart_writeText(char *buffer) {
  while (*buffer) {
    if (*buffer == '\n')
      uart_writeByteBlocking('\r');
    uart_writeByteBlocking(*buffer++);
  }
}

void uart_drainOutputQueue() {
  while (!uartIsOutputQueueEmpty())
    uart_loadOutputFifo();
}

//? Keypress format:
//? K,[KeyID],[1 for Down / 0 for Up]
//? Mouse format:
//? M,[Left Button],[Right button],[X movement],[Y movement]
void uart_update() {
  if (!uartIsOutputQueueEmpty() && uart_isWriteByteReady()) {
    uart_loadOutputFifo();
  }
}
#endif
