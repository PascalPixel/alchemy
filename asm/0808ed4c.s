@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808ed4c
	.thumb_func
Func_0808ed4c:
	push	{lr}
	bl	Func_0808ec14
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldr	r3, [pc, #20]
	ldr	r3, [r3, #0]
	lsls	r2, r0, #3
	adds	r3, r3, r2
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
.L1:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
