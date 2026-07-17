@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b7aac
	.thumb_func
Func_080b7aac:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	Func_08077008
	adds	r2, r0, #0
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	movs	r5, #1
	cmp	r3, #0
	beq.n	.L0
	movs	r1, #158
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L1
	subs	r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L1
	adds	r1, #10
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L2
.L1:
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r3, r2
	negs	r2, r3
	orrs	r2, r3
	lsrs	r5, r2, #31
	lsls	r5, r5, #2
	b.n	.L2
.L0:
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	eors	r3, r5
	negs	r2, r3
	orrs	r2, r3
	lsrs	r5, r2, #31
	movs	r3, #5
	subs	r5, r3, r5
.L2:
	adds	r0, r6, #0
	bl	Func_080b7dd0
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	Func_08009080
	adds	r0, r6, #0
	bl	Func_080b7dd0
	movs	r1, #3
	ands	r1, r6
	ldr	r0, [r0, #0]
	adds	r1, #14
	bl	Func_08009088
	pop	{r5, r6}
	pop	{r1}
	bx	r1
