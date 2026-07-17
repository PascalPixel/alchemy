@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080920fc
	.thumb_func
Func_080920fc:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	bl	Func_0808ba1c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r1, r6, #0
	bl	Func_08093a6c
	adds	r0, r5, #0
	bl	Func_08009148
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
