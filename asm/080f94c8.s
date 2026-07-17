@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080f94c8
	.thumb_func
Func_080f94c8:
	push	{lr}
	adds	r1, r0, #0
	lsls	r1, r1, #16
	ldr	r0, [pc, #12]
	lsrs	r1, r1, #16
	bl	Func_080fb2a4
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02004290
