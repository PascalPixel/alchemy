@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080f94e0
	.thumb_func
Func_080f94e0:
	push	{lr}
	adds	r2, r0, #0
	lsls	r2, r2, #16
	ldr	r0, [pc, #12]
	asrs	r2, r2, #16
	movs	r1, #255
	bl	Func_080fb334
	pop	{r0}
	bx	r0
	.4byte 0x02004290
