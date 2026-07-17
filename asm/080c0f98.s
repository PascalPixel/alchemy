@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c0f98
	.thumb_func
Func_080c0f98:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	Func_080b7dd0
	cmp	r0, #0
	beq.n	.L0
	ldr	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L1
	cmp	r2, #2
	beq.n	.L2
	b.n	.L0
.L1:
	ldr	r4, [r0, #80]
	movs	r2, #13
	movs	r3, #3
	ldrb	r1, [r4, #5]
	negs	r2, r2
	ands	r5, r3
	adds	r3, r2, #0
	lsls	r0, r5, #2
	ands	r3, r1
	orrs	r3, r0
	strb	r3, [r4, #5]
	ldrb	r3, [r4, #17]
	ands	r2, r3
	orrs	r2, r0
	strb	r2, [r4, #17]
	b.n	.L0
.L2:
	movs	r3, #3
	ands	r5, r3
	ldr	r1, [r0, #80]
	lsls	r0, r5, #2
	movs	r5, #13
	movs	r6, #0
	negs	r5, r5
.L3:
	ldmia	r1!, {r4}
	cmp	r4, #0
	beq.n	.L0
	ldrb	r2, [r4, #5]
	adds	r3, r5, #0
	ands	r3, r2
	orrs	r3, r0
	ldrb	r2, [r4, #17]
	strb	r3, [r4, #5]
	adds	r3, r5, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r6, #1
	strb	r3, [r4, #17]
	cmp	r6, #3
	ble.n	.L3
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
