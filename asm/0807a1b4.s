@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807a1b4
	.thumb_func
Func_0807a1b4:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r2, #0
	bl	Func_08077394
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r6, r5, r3
	ldrb	r4, [r0, r6]
	adds	r3, r4, #0
	cmp	r3, #9
	bhi.n	.L0
	lsls	r3, r5, #2
	adds	r1, r3, #0
	adds	r1, #248
	movs	r2, #1
	ldr	r3, [r0, r1]
	lsls	r2, r7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
.L0:
	movs	r0, #1
	negs	r0, r0
	b.n	.L2
.L1:
	adds	r3, r4, #1
	strb	r3, [r0, r6]
	ldr	r3, [r0, r1]
	orrs	r3, r2
	str	r3, [r0, r1]
	movs	r0, #0
.L2:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
