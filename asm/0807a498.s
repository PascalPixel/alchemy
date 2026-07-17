@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807a498
	.thumb_func
Func_0807a498:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r2, #0
	adds	r5, r1, #0
	mov	fp, r3
	sub	sp, #4
	mov	sl, r0
	bl	Func_08077394
	movs	r2, #1
	lsls	r3, r5, #2
	mov	r9, r2
	adds	r3, #248
	mov	r8, r3
	mov	r3, r9
	lsls	r3, r6
	mov	r2, r8
	adds	r7, r0, #0
	mov	r9, r3
	ldr	r3, [r7, r2]
	mov	r2, r9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	r0, sl
	bl	Func_0807a2bc
	adds	r1, r5, #0
	str	r0, [sp, #0]
	adds	r2, r6, #0
	mov	r0, fp
	bl	Func_0807a1b4
	cmp	r0, #0
	bne.n	.L0
	adds	r2, r6, #0
	mov	r0, sl
	adds	r1, r5, #0
	bl	Func_0807a350
	mov	r2, r8
	ldr	r3, [r7, r2]
	mov	r2, r9
	bics	r3, r2
	mov	r2, r8
	str	r3, [r7, r2]
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r2, r5, r3
	ldrb	r3, [r7, r2]
	adds	r3, #255
	strb	r3, [r7, r2]
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L1
	mov	r0, fp
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	Func_0807a2e4
	b.n	.L2
.L1:
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	r0, sl
	bl	Func_0807a3a8
	mov	r0, fp
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	Func_0807a458
.L2:
	movs	r0, #0
	b.n	.L3
.L0:
	movs	r0, #1
	negs	r0, r0
.L3:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
