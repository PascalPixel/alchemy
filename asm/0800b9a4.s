@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800b9a4
	.thumb_func
Func_0800b9a4:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r1, #0
	cmp	r5, #0
	beq.n	.L0
	cmp	r7, #3
	bhi.n	.L0
	lsls	r3, r7, #2
	adds	r6, r3, #0
	adds	r6, #40
	ldr	r0, [r5, r6]
	cmp	r0, #0
	beq.n	.L0
	bl	Func_0800bc48
	movs	r3, #0
	adds	r2, r7, #1
	str	r3, [r5, r6]
	movs	r0, #0
	cmp	r2, #3
	bhi.n	.L1
	lsls	r3, r2, #2
	adds	r3, r3, r5
	adds	r1, r3, #0
	adds	r1, #40
.L3:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L2
	adds	r0, #1
.L2:
	adds	r2, #1
	cmp	r2, #3
	bls.n	.L3
.L1:
	cmp	r0, #0
	bne.n	.L0
	adds	r3, r5, #0
	adds	r3, #39
	strb	r7, [r3, #0]
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
