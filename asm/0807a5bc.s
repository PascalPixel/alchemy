@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807a5bc
	.thumb_func
Func_0807a5bc:
	push	{r5, r6, r7, lr}
	sub	sp, #36
	add	r6, sp, #4
	adds	r7, r0, #0
	adds	r0, r6, #0
	movs	r5, #0
	bl	Func_080796c4
	cmp	r5, r0
	bge.n	.L0
	adds	r1, r6, #0
	adds	r6, r0, #0
.L3:
	ldrh	r0, [r1, #0]
	adds	r1, #2
	str	r1, [sp, #0]
	bl	Func_08077394
	adds	r2, r0, #0
	movs	r0, #1
	negs	r0, r0
	ldr	r1, [sp, #0]
	cmp	r7, r0
	bne.n	.L1
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	adds	r0, #1
	adds	r5, r5, r3
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	adds	r0, #1
	adds	r5, r5, r3
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	adds	r0, #1
	adds	r5, r5, r3
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	b.n	.L2
.L1:
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrb	r3, [r2, r3]
.L2:
	adds	r5, r5, r3
	subs	r6, #1
	cmp	r6, #0
	bne.n	.L3
.L0:
	adds	r0, r5, #0
	add	sp, #36
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
