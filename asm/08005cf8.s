@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08005cf8
	.thumb_func
Func_08005cf8:
	push	{lr}
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	movs	r0, #51
	bl	Func_08002dd8
	pop	{r1}
	bx	r1
