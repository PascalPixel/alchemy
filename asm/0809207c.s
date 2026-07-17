@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0809207c
	.thumb_func
Func_0809207c:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	adds	r1, r0, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r1, r5, #0
	bl	Func_08093a6c
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
