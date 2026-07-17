@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080be070
	.thumb_func
Func_080be070:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	sub	sp, #16
	movs	r0, #1
	cmp	r6, #7
	bls.n	.L0
	movs	r0, #2
.L0:
	mov	r5, sp
	adds	r1, r5, #0
	bl	Func_080b6c08
	movs	r2, #0
	cmp	r2, r0
	bge.n	.L1
	ldrh	r3, [r5, #0]
	cmp	r3, r6
	beq.n	.L1
	adds	r1, r5, #0
.L2:
	adds	r2, #1
	cmp	r2, r0
	bge.n	.L1
	adds	r1, #2
	ldrh	r3, [r1, #0]
	cmp	r3, r6
	bne.n	.L2
.L1:
	adds	r3, r2, #0
	eors	r3, r0
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	add	sp, #16
	pop	{r5, r6}
	pop	{r1}
	bx	r1
