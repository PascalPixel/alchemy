@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080c1a24
	.thumb_func
Func_080c1a24:
	push	{lr}
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	pop	{r0}
	bx	r0
