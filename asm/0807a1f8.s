@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807a1f8
	.thumb_func
Func_0807a1f8:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r7, r0, #0
	bl	Func_08077394
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r2, r5, r3
	ldrb	r3, [r0, r2]
	cmp	r3, #9
	bls.n	.L1
	movs	r3, #10
	strb	r3, [r0, r2]
	b.n	.L0
.L1:
	lsls	r1, r5, #2
	adds	r3, r1, #0
	adds	r3, #248
	movs	r2, #1
	ldr	r3, [r0, r3]
	lsls	r2, r6
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r4, #132
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldr	r3, [r0, r3]
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L2
	cmp	r7, #7
	bls.n	.L3
	movs	r0, #1
.L3:
	bl	Func_08077330
	movs	r2, #132
	adds	r3, r0, #0
	lsls	r2, r2, #1
	adds	r1, r3, #0
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r4, #0
	adds	r1, #8
	cmp	r4, r3
	bge.n	.L4
	ldrb	r3, [r1, #0]
	cmp	r5, r3
	bne.n	.L5
	ldrb	r3, [r1, #1]
	cmp	r6, r3
	beq.n	.L4
.L5:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	adds	r4, #1
	cmp	r4, r3
	bge.n	.L4
	lsls	r2, r4, #2
	ldrb	r3, [r1, r2]
	cmp	r5, r3
	bne.n	.L5
	adds	r3, r1, r2
	ldrb	r3, [r3, #1]
	cmp	r6, r3
	bne.n	.L5
.L4:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	cmp	r4, r3
	beq.n	.L6
	lsls	r3, r4, #2
	adds	r3, r1, r3
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bgt.n	.L0
	movs	r4, #2
	negs	r4, r4
	cmp	r3, r4
	beq.n	.L0
.L6:
	movs	r0, #1
	b.n	.L2
.L0:
	movs	r0, #0
.L2:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
