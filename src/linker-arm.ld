/* QEMU / firmware load ARM Raspberry Pi kernels at 0x8000; literals must match. */
SECTIONS
{
	. = 0x8000;
	.text.boot : { *(.text.boot) }
	.text : { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(CONSTANT(MAXPAGESIZE));
	bss_begin = .;
	.bss : { *(.bss*) }
	bss_end = .;
}
