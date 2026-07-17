@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080a1bcc
	.thumb_func
Func_080a1bcc:
	push	{lr}
	movs	r0, #108
	movs	r1, #40
	movs	r2, #8
	bl	Func_080a1bdc
	pop	{r0}
	bx	r0
