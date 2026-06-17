#include "mm.h"
#if defined(__aarch64__)

.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // We're not on the main core, so hang in an infinite wait loop
1:  wfe
    b       1b
2:  // We're on the main core!

    // Set stack to start below our code
    ldr     x1, =_start
    mov     sp, x1

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
3:  cbz     w2, 4f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b               // Loop if non-zero

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      kernel_main
    // In case it does return, halt the master core too
    b       1b

#elif defined(__arm__)


.section ".text.boot"

.globl _start
_start:
    /* ARM1176 (Pi 0 / Pi 1): single-core; QEMU MPIDR low bits are not Aff0, so
       do not gate on MPIDR. Cortex SMP (Pi 2+) uses MPIDR Aff0 == 0 on boot CPU. */
    mrc p15, 0, r0, c0, c0, 0
    mov r1, r0, lsr #4
    ldr r2, =0xfff
    and r1, r1, r2
    ldr r2, =0xb76
    cmp r1, r2
    beq master

    mrc p15, 0, r0, c0, c0, 5
    and r0, r0, #0xFF
    cmp r0, #0
    beq master
    b   proc_hang

proc_hang:
    b   proc_hang

master:
    // Load the 32-bit absolute addresses from the pointers below
    ldr r0, ptr_bss_begin
    ldr r1, ptr_bss_end
    sub r1, r1, r0
    bl  memzero

    ldr sp, =LOW_MEMORY
    bl  kernel_main
    b   proc_hang

// Guarded absolute address pointers placed safely in the read-only section
.align 2
ptr_bss_begin: .word bss_begin
ptr_bss_end:   .word bss_end
#elif defined(_USE_LIMINE_)
#else
	#error "Unsupported Architecture"

#endif
