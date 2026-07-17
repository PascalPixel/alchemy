@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808d590
	.thumb_func
Func_0808d590:
	push	{lr}
	ldr	r3, [pc, #12]
	ldr	r0, [r3, #4]
	bl	Func_080072e4
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02008000
