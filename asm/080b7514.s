@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b7514
	.thumb_func
Func_080b7514:
	push	{r5, r6, lr}
	movs	r5, #0
	b.n	.L0
.L2:
	adds	r5, #1
.L0:
	cmp	r5, #5
	bgt.n	.L1
	adds	r6, r5, #0
	adds	r6, #128
	adds	r0, r6, #0
	bl	Func_08077008
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L2
.L1:
	cmp	r5, #6
	bne.n	.L3
	movs	r0, #1
	negs	r0, r0
	b.n	.L4
.L3:
	adds	r0, r6, #0
.L4:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
