@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080c1a14
	.thumb_func
Func_080c1a14:
	push	{lr}
	movs	r0, #0
	movs	r1, #0
	bl	Func_080c0700
	pop	{r0}
	bx	r0
