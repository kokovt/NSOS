#if defined(__aarch64__)

.globl memzero
memzero:
    // Check if size (x1) is 0 or negative before doing anything
    cmp     x1, #0
    b.le    memzero_done

memzero_loop:
    str     xzr, [x0], #8       // Store 0 and increment address by 8
    subs    x1, x1, #8          // Subtract 8 from size
    b.gt    memzero_loop        // Loop only if strictly greater than 0

memzero_done:
    ret
#elif defined(__arm__)

.globl memzero
memzero:
	mov r2, #0          // Create a zero value in r2
memzero_loop:
	str r2, [r0], #4    // Store 0 at address in r0, then increment r0 by 4
	subs r1, r1, #4     // Subtract 4 from the size counter (r1)
	bgt memzero_loop    // If r1 > 0,z loop again
	bx lr               // Return to caller


#else 
	#error "Invalid Archetecture"
#endif