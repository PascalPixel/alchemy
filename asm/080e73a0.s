@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080e73a0
	.thumb_func
Func_080e73a0:
	push	{r5, r6, lr}
	mov	r6, r9
	push	{r6}
	sub	sp, #4
	mov	r2, r9
	mov	r3, sp
	str	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r5, r1, #0
	adds	r1, r3, #0
	subs	r1, #136
	ldr	r3, [r1, #0]
	movs	r6, #225
	lsls	r6, r6, #7
	adds	r2, r3, r6
	ldr	r3, [r2, #24]
	movs	r6, #1
	negs	r6, r6
	movs	r4, #0
	cmp	r3, r6
	bne.n	.L0
	str	r4, [r2, #24]
	b.n	.L1
.L0:
	adds	r4, #1
	cmp	r4, #32
	beq.n	.L2
	lsls	r3, r4, #3
	subs	r3, r3, r4
	ldr	r2, [r1, #0]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	movs	r3, #225
	lsls	r3, r3, #7
	adds	r2, r2, r3
	movs	r6, #1
	ldr	r3, [r2, #24]
	negs	r6, r6
	cmp	r3, r6
	bne.n	.L0
	movs	r3, #0
	str	r3, [r2, #24]
.L1:
	str	r0, [r2, #0]
	str	r5, [r2, #4]
.L2:
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
