@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b7e24
	.thumb_func
Func_080b7e24:
	push	{r5, r6, lr}
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
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
	ldr	r0, [r0, #80]
	bl	Func_080b7e04
	b.n	.L0
.L2:
	ldr	r5, [r0, #80]
	movs	r6, #3
.L4:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L3
	bl	Func_080b7e04
.L3:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L4
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
