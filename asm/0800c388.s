@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800c388
	.thumb_func
Func_0800c388:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	mov	r8, r1
	mov	sl, r2
	cmp	r5, #0
	beq.n	.L0
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L1
	cmp	r2, #2
	beq.n	.L2
	b.n	.L0
.L1:
	mov	r1, r8
	ldr	r0, [r5, #80]
	bl	Func_0800ba30
	ldr	r0, [r5, #80]
	mov	r1, sl
	bl	Func_0800baf8
	b.n	.L0
.L2:
	ldr	r6, [r5, #80]
	movs	r7, #3
.L4:
	ldmia	r6!, {r5}
	cmp	r5, #0
	beq.n	.L3
	mov	r1, r8
	adds	r0, r5, #0
	bl	Func_0800ba30
	adds	r0, r5, #0
	mov	r1, sl
	bl	Func_0800baf8
.L3:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L4
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
