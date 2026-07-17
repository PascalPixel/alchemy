@ BIOS-call wrappers: software interrupts are not expressible in C.
	.thumb

	swi 0xb
	bx lr

	mov r0, #0
	swi 0x19
	bx lr
