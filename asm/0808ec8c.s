@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808ec8c
	.thumb_func
Func_0808ec8c:
	push	{r5, lr}
	bl	Func_0808ec14
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L0
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	lsls	r2, r0, #3
	adds	r3, r3, r2
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [r3, #0]
	movs	r0, #18
	bl	Func_080030f8
	cmp	r5, #0
	beq.n	.L1
	adds	r0, r5, #0
	movs	r1, #7
	bl	Func_08009080
.L1:
	movs	r0, #146
	bl	Func_080f9010
	cmp	r5, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #40]
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080091e0
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
