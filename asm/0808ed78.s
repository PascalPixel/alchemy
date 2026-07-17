@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808ed78
	.thumb_func
Func_0808ed78:
	push	{lr}
	bl	Func_0808ec14
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L0
	ldr	r3, [pc, #32]
	ldr	r3, [r3, #0]
	lsls	r2, r0, #3
	adds	r3, r3, r2
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	adds	r2, r0, #0
	adds	r2, #84
	movs	r3, #0
	strb	r3, [r2, #0]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
