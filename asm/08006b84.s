@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08006b84
	.thumb_func
Func_08006b84:
	push	{r4, lr}
	adds	r4, r0, #0
	subs	r3, r2, #1
	cmp	r2, #0
	beq.n	.L0
	movs	r2, #1
	negs	r2, r2
.L1:
	ldrb	r0, [r4, #0]
	strb	r0, [r1, #0]
	adds	r4, #1
	adds	r1, #1
	subs	r3, #1
	cmp	r3, r2
	bne.n	.L1
.L0:
	pop	{r4}
	pop	{r0}
	bx	r0
