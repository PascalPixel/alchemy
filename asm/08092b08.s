@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092b08
	.thumb_func
Func_08092b08:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	Func_0808ba1c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L0
	ldr	r1, [r6, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r5, r3
	adds	r3, r2, #0
	lsls	r4, r5, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	ldrb	r3, [r1, #21]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #21]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
