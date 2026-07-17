@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08011e88
	.thumb_func
Func_08011e88:
	push	{lr}
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r0, #1
	lsls	r2, r3, #19
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r0, #1
	lsls	r4, r3, #19
	cmp	r1, #7
	bhi.n	.L0
	subs	r3, r4, r2
	adds	r0, r1, #0
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L1
	adds	r0, #7
.L1:
	asrs	r0, r0, #3
	adds	r0, r2, r0
	b.n	.L2
.L0:
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	lsls	r2, r3, #19
	adds	r3, r1, #0
	subs	r2, r2, r4
	subs	r3, #8
	adds	r0, r3, #0
	muls	r0, r2
	cmp	r0, #0
	bge.n	.L3
	adds	r0, #7
.L3:
	asrs	r0, r0, #3
	adds	r0, r4, r0
.L2:
	pop	{r1}
	bx	r1
