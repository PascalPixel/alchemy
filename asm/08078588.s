@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078588
	.thumb_func
Func_08078588:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_08078414
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #216
	ldrh	r3, [r6, r3]
	ldr	r2, [pc, #104]
	eors	r3, r5
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L1
	adds	r1, r2, #0
	adds	r2, r6, #0
	adds	r2, #216
.L2:
	adds	r0, #1
	cmp	r0, #14
	bgt.n	.L1
	adds	r2, #2
	ldrh	r3, [r2, #0]
	eors	r3, r5
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L2
.L1:
	cmp	r0, #15
	beq.n	.L0
	lsls	r3, r0, #1
	adds	r4, r3, #0
	adds	r4, #216
	ldrh	r1, [r6, r4]
	lsrs	r3, r1, #11
	adds	r2, r3, #1
	cmp	r2, #29
	bhi.n	.L3
	ldr	r3, [pc, #52]
	lsls	r2, r2, #11
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, r4]
	b.n	.L4
.L5:
	strh	r5, [r6, r1]
	b.n	.L4
.L0:
	adds	r2, r6, #0
	movs	r0, #0
	adds	r2, #216
	movs	r1, #216
.L6:
	ldrh	r3, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	beq.n	.L5
	adds	r0, #1
	adds	r1, #2
	cmp	r0, #14
	ble.n	.L6
.L3:
	movs	r0, #1
	negs	r0, r0
.L4:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
	.4byte 0x000007ff
