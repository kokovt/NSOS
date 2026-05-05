#include <mailbox/mailbox.h>

void mailbox_write(unsigned long buffer_addr, uint32_t channel, uint32_t PBASE) {
    // 1. Ensure the buffer address is strictly within 32-bit space.
    // The GPU can only read the bottom 4GB of physical RAM.
    static uint32_t __attribute__((aligned(16))) value;
    value = (uint32_t)(buffer_addr & 0xFFFFFFFF);

    // 2. Clear out the bottom 4 bits of the address so they don't corrupt the channel
    value &= ~0xF;

    // 3. Wait for the mailbox to be ready to accept a write
    while(mmio_read(MBOX_STATUS, PBASE) & FULL) {
        delay(1);
    }

    // 4. Combine value and channel and write it directly to the hardware register
    // This bypasses creating local stack variables that the GPU cannot read.
    mmio_write(MBOX_WRITE, value | (channel & 0xF), PBASE);
}


uint32_t mailbox_read(uint32_t channel, uint32_t PBASE) {
    while(1) {
        while(mmio_read(MBOX_STATUS, PBASE) & EMPTY) {
            delay(1); // Wait for 1 cycle
        }
        uint32_t r = mmio_read(MBOX_READ, PBASE);
        
        if((r & 0xF) == channel) {
            return r;
        }
    }
}
