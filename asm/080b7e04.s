@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b7e04
	.thumb_func
Func_080b7e04:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #0
	adds	r0, #40
	movs	r2, #3
.L2:
	ldmia	r0!, {r3}
	cmp	r3, #0
	beq.n	.L1
	str	r1, [r3, #16]
.L1:
	subs	r2, #1
	cmp	r2, #0
	bge.n	.L2
.L0:
	pop	{r0}
	bx	r0
