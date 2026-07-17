@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0801edcc
	.thumb_func
Func_0801edcc:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #0
	strb	r1, [r0, #5]
	strh	r3, [r0, #12]
.L0:
	pop	{r0}
	bx	r0
