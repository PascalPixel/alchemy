@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078708
	.thumb_func
Func_08078708:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	mov	sl, r0
	bl	Func_08077394
	lsls	r5, r5, #1
	mov	fp, r5
	mov	r3, fp
	adds	r7, r0, #0
	adds	r3, #216
	ldrh	r5, [r7, r3]
	mov	r0, sl
	adds	r1, r5, #0
	bl	Func_0807842c
	cmp	r0, #0
	bne.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r3, r5, #0
	ands	r3, r2
	mov	r9, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L1
	adds	r0, r5, #0
	bl	Func_08078414
	ldrb	r0, [r0, #2]
	mov	r8, r0
	cmp	r0, #6
	beq.n	.L2
	movs	r6, #0
	movs	r5, #216
	b.n	.L3
.L5:
	adds	r5, #2
	adds	r6, #1
.L3:
	cmp	r6, #14
	bgt.n	.L4
	ldrh	r3, [r5, r7]
	mov	r2, r9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	ldrh	r0, [r5, r7]
	bl	Func_08078414
	ldrb	r3, [r0, #2]
	cmp	r3, r8
	bne.n	.L5
.L4:
	cmp	r6, #15
	beq.n	.L2
	lsls	r3, r6, #1
	adds	r6, r3, #0
	adds	r6, #216
	ldrh	r0, [r7, r6]
	bl	Func_08078414
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	movs	r0, #2
	negs	r0, r0
	b.n	.L1
.L6:
	ldrh	r2, [r7, r6]
	ldr	r3, [pc, #36]
	ands	r3, r2
	strh	r3, [r7, r6]
.L2:
	mov	r3, fp
	adds	r3, #216
	ldrh	r2, [r7, r3]
	ldr	r1, [pc, #20]
	orrs	r2, r1
	strh	r2, [r7, r3]
	mov	r0, sl
	bl	Func_08078bf0
	mov	r0, sl
	bl	Func_08077428
	movs	r0, #0
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000200
	.4byte 0x0000fdff
.L1:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
