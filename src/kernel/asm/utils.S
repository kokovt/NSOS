#if defined(__aarch64__)
.globl put32
put32:
	str w1,[x0]
	ret

.globl get32
get32:
	ldr w0,[x0]
	ret

.globl delay
delay:
	subs x0, x0, #1
	bne delay
	ret
#elif defined(__arm__)

.globl put32
put32:
    str r1, [r0]
    bx lr

.globl get32
get32:
    ldr r0, [r0]
    bx lr

.globl delay
delay:
    subs r0, r0, #1
    bne delay
    bx lr

#elif defined(_USE_LIMINE_)
#else
	#error "Unsupported Architecture"

#endif

