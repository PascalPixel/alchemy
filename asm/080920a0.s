@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080920a0
	.thumb_func
Func_080920a0:
	push	{lr}
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	adds	r1, r0, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	bl	Func_08009140
.L0:
	pop	{r0}
	bx	r0
