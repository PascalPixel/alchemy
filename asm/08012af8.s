@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08012af8
	.thumb_func
Func_08012af8:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
.L1:
	adds	r5, r5, r6
	cmp	r5, #0
	bge.n	.L0
	movs	r5, #128
	lsls	r5, r5, #2
	b.n	.L1
.L0:
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r5, r3
	blt.n	.L2
	movs	r5, #1
	negs	r5, r5
	b.n	.L1
.L2:
	adds	r0, r5, #0
	bl	Func_08185000
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L1
	adds	r0, r5, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
