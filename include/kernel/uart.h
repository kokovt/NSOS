#ifndef _UART_H
#define _UART_H

#include <stdint.h>
#include <mmio/mmio.h>
#include <peripherals/base.h>
#include <peripherals/gpio.h>
enum { AUX_UART_CLOCK = 500000000, UART_MAX_QUEUE = 16 * 1024 };

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK / (baud * 8)) - 1)

#define PBASE             0xFE000000

#define AUX_BASE          (PBASE + 0x00215000)
#define AUX_IRQ           (AUX_BASE)
#define AUX_ENABLES       (AUX_BASE + 0x04)
#define AUX_MU_IO_REG     (AUX_BASE + 0x40)
#define AUX_MU_IER_REG    (AUX_BASE + 0x44)
#define AUX_MU_IIR_REG    (AUX_BASE + 0x48)
#define AUX_MU_LCR_REG    (AUX_BASE + 0x4C)
#define AUX_MU_MCR_REG    (AUX_BASE + 0x50)
#define AUX_MU_LSR_REG    (AUX_BASE + 0x54)
#define AUX_MU_MSR_REG    (AUX_BASE + 0x60)
#define AUX_MU_SCRATCH    (AUX_BASE + 0x68)
#define AUX_MU_CNTL_REG   (AUX_BASE + 0x60) 
#define AUX_MU_STAT_REG   (AUX_BASE + 0x64) 
#define AUX_MU_BAUD_REG   (AUX_BASE + 0x68) 


void uart_init(unsigned int baud);

unsigned int uartIsOutputQueueEmpty(void);
unsigned int uart_isReadByteReady(void);
unsigned int uart_isWriteByteReady(void);

unsigned char uart_readByte(void);

void uart_writeByteBlocking(unsigned char ch);
void uart_writeByteBlocking(unsigned char ch);

void uart_loadOutputFifo();
void uart_writeByteBlocking(unsigned char ch);
void uart_writeText(char *buffer);
void uart_drainOutputQueue();
void uart_update();


#endif /*_UART_H */