#ifndef _MAILBOX_H
#define _MAILBOX_H

#include <stdint.h>
#include <mmio/mmio.h>
#include <utils.h>

#define MBOX_BASE 0xB880
// #define MBOX_BASE 0xFE00B880
#define MBOX_READ (MBOX_BASE + 0x00)
#define MBOX_STATUS (MBOX_BASE + 0x18)
#define MBOX_WRITE (MBOX_BASE + 0x20)

#define FULL 0x80000000
#define EMPTY 0x40000000

enum {
    MBOX_REQUEST  = 0
};

enum mailbox_channel {
    MBOX_CH_POWER = 0,
    MBOX_CH_FB = 1,
    MBOX_CH_VUART = 2,
    MBOX_CH_VCHIQ = 3,
    MBOX_CH_LEDS = 4,
    MBOX_CH_BTNS = 5,
    MBOX_CH_TOUCH = 6,
    MBOX_CH_COUNT = 7,
    MBOX_CH_PROP = 8
};

enum {
    MBOX_TAG_SETPOWER   = 0x28001,
    MBOX_TAG_SETCLKRATE = 0x38002,

    MBOX_TAG_SETPHYWH   = 0x48003,
    MBOX_TAG_SETVIRTWH  = 0x48004,
    MBOX_TAG_SETVIRTOFF = 0x48009,
    MBOX_TAG_SETDEPTH   = 0x48005,
    MBOX_TAG_SETPXLORDR = 0x48006,
    MBOX_TAG_GETFB      = 0x40001,
    MBOX_TAG_GETPITCH   = 0x40008,

    MBOX_TAG_LAST       = 0
};


void mailbox_write(unsigned long buffer_addr, uint32_t channel);
uint32_t mailbox_read(uint32_t channel);

#endif /* _MAILBOX_H */