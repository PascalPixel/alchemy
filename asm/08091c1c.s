@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08091c1c
	.thumb_func
Func_08091c1c:
	push	{r5, lr}
	adds	r5, r2, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_08077028
	cmp	r0, #0
	blt.n	.L0
	adds	r0, r5, #0
	b.n	.L1
.L0:
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	bx	lr
