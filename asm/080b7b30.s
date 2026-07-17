@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b7b30
	.thumb_func
Func_080b7b30:
	push	{r5, r6, lr}
	bl	Func_080b7dd0
	cmp	r0, #0
	beq.n	.L0
	ldr	r5, [r0, #0]
	cmp	r5, #0
	beq.n	.L0
	movs	r3, #0
	movs	r6, #0
	str	r3, [r0, #32]
	str	r3, [r0, #36]
	b.n	.L1
.L2:
	bl	Func_08009038
	adds	r6, #1
.L1:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_080b7f70
	cmp	r0, #0
	bne.n	.L2
	adds	r3, r5, #0
	adds	r3, #84
	strb	r0, [r3, #0]
	str	r0, [r5, #80]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
