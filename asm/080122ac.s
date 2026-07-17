@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080122ac
	.thumb_func
Func_080122ac:
	push	{lr}
	adds	r0, r1, #0
	bl	Func_08012204
	subs	r0, #5
	cmp	r0, #7
	bhi.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r1}
	bx	r1
