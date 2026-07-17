@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800e6ac
	.thumb_func
Func_0800e6ac:
	push	{lr}
	adds	r4, r2, #0
	cmp	r1, #0
	bne.n	.L0
	adds	r3, r0, #0
	adds	r3, #87
	strb	r4, [r3, #0]
	b.n	.L1
.L0:
	cmp	r1, #1
	bne.n	.L2
	adds	r2, r0, #0
	adds	r2, #87
	ldrb	r3, [r2, #0]
	adds	r3, r3, r4
	strb	r3, [r2, #0]
	b.n	.L1
.L2:
	adds	r0, #87
	lsls	r3, r4, #24
	ldrb	r2, [r0, #0]
	lsrs	r3, r3, #24
	movs	r1, #0
	cmp	r2, r3
	bne.n	.L3
	movs	r1, #1
.L3:
	strb	r1, [r0, #0]
.L1:
	pop	{r0}
	bx	r0
