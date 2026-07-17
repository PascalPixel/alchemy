@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078e28
	.thumb_func
Func_08078e28:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r0, #0
	bl	Func_08077394
	ldr	r3, [pc, #148]
	adds	r5, r0, #0
	movs	r0, #88
	mov	ip, r3
	ands	r6, r3
	ldrh	r3, [r0, r5]
	mov	r2, ip
	movs	r4, #1
	ands	r2, r3
	negs	r4, r4
	movs	r1, #0
	cmp	r2, r6
	bne.n	.L0
	strh	r2, [r0, r5]
	movs	r4, #0
	b.n	.L1
.L0:
	adds	r1, #1
	adds	r0, #4
	cmp	r1, #30
	bgt.n	.L1
	ldrh	r3, [r0, r5]
	mov	r2, ip
	ands	r2, r3
	cmp	r2, r6
	bne.n	.L0
	strh	r2, [r0, r5]
	adds	r4, r1, #0
.L1:
	cmp	r4, #0
	bge.n	.L2
	movs	r2, #88
	ldrh	r3, [r5, r2]
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L3
	strh	r6, [r5, r2]
	movs	r4, #0
	b.n	.L4
.L3:
	adds	r1, #1
	cmp	r1, #30
	bgt.n	.L4
	lsls	r3, r1, #2
	adds	r2, r3, #0
	adds	r2, #88
	ldrh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L3
	strh	r6, [r5, r2]
	adds	r4, r1, #0
.L4:
	cmp	r4, #0
	bge.n	.L2
	movs	r0, #1
	negs	r0, r0
	b.n	.L5
.L2:
	adds	r0, r7, #0
	bl	Func_08078bf0
	movs	r3, #88
	ldrh	r3, [r5, r3]
	movs	r1, #0
	cmp	r3, r6
	beq.n	.L6
	adds	r0, r5, #0
	adds	r0, #88
.L7:
	adds	r1, #1
	cmp	r1, #31
	bgt.n	.L6
	adds	r0, #4
	ldrh	r3, [r0, #0]
	cmp	r3, r6
	bne.n	.L7
.L6:
	adds	r0, r1, #0
.L5:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00003fff
