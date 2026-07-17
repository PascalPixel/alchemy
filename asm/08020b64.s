@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08020b64
	.thumb_func
Func_08020b64:
	push	{r5, r6, lr}
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	sub	sp, #20
	adds	r6, r0, #0
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L0
	mov	r0, sp
	adds	r5, r0, #0
.L1:
	strb	r2, [r5, #0]
	adds	r1, #1
	ldrb	r3, [r1, #0]
	adds	r2, r3, #0
	adds	r3, r2, #0
	adds	r5, #1
	adds	r4, #1
	cmp	r3, #0
	bne.n	.L1
	b.n	.L2
.L0:
	mov	r0, sp
.L2:
	movs	r3, #8
	strb	r3, [r0, r4]
	adds	r4, #1
	movs	r3, #2
	strb	r3, [r0, r4]
	adds	r4, #1
	cmp	r4, #6
	bgt.n	.L3
	movs	r3, #7
	adds	r2, r4, r0
	movs	r1, #95
	subs	r4, r3, r4
.L4:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L4
	movs	r4, #7
.L3:
	movs	r3, #8
	strb	r3, [r0, r4]
	adds	r4, #1
	movs	r3, #15
	strb	r3, [r0, r4]
	adds	r4, #1
	movs	r3, #0
	strb	r3, [r0, r4]
	movs	r3, #2
	negs	r3, r3
	adds	r1, r6, #0
	movs	r2, #0
	bl	Func_0801e858
	add	sp, #20
	pop	{r5, r6}
	pop	{r0}
	bx	r0
