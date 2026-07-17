@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808ece0
	.thumb_func
Func_0808ece0:
	push	{lr}
	bl	Func_0808ec14
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L0
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	lsls	r2, r0, #3
	adds	r3, r3, r2
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L1
	movs	r1, #4
	bl	Func_08009080
.L1:
	movs	r0, #124
	bl	Func_080f9010
	movs	r0, #12
	bl	Func_080030f8
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
