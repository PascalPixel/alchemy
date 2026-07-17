@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08017dd4
	.thumb_func
Func_08017dd4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #0
	adds	r6, r1, #0
	adds	r7, r0, #0
	mov	r8, r2
	mov	r9, r3
	cmp	r6, #0
	bge.n	.L0
	cmp	r2, #0
	bne.n	.L1
	movs	r3, #1
	mov	r9, r3
.L1:
	negs	r6, r6
.L0:
	movs	r3, #32
	adds	r5, r7, #0
	strb	r3, [r7, #0]
	adds	r5, #12
	mov	sl, r7
.L2:
	adds	r0, r6, #0
	movs	r1, #10
	bl	Func_080022fc
	adds	r0, #48
	strb	r0, [r5, #0]
	movs	r1, #10
	adds	r0, r6, #0
	bl	Func_080022ec
	subs	r5, #1
	adds	r6, r0, #0
	cmp	r5, sl
	bne.n	.L2
	movs	r0, #0
	strb	r0, [r7, #13]
	movs	r1, #32
	movs	r0, #1
	movs	r4, #45
	adds	r2, r7, #0
	b.n	.L3
.L6:
	adds	r2, #1
	adds	r0, #1
.L3:
	cmp	r0, #13
	beq.n	.L4
	ldrb	r3, [r2, #1]
	cmp	r3, #48
	bne.n	.L5
	cmp	r0, #12
	beq.n	.L6
	strb	r1, [r2, #1]
	b.n	.L6
.L5:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L4
	strb	r4, [r2, #0]
.L4:
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L7
	ldrb	r3, [r7, #0]
	movs	r0, #0
	cmp	r3, #32
	bne.n	.L8
	adds	r2, r7, #0
.L9:
	adds	r0, #1
	cmp	r0, #12
	beq.n	.L8
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, #32
	beq.n	.L9
.L8:
	adds	r0, r7, r0
	b.n	.L10
.L7:
	mov	r3, r8
	cmp	r3, #12
	bls.n	.L11
	movs	r3, #12
	mov	r8, r3
.L11:
	mov	r3, r8
	subs	r0, r7, r3
	adds	r0, #13
.L10:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
