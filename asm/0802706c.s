@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0802706c
	.thumb_func
Func_0802706c:
	push	{r5, lr}
	ldrb	r3, [r0, #1]
	movs	r2, #15
	ands	r2, r3
	movs	r5, #0
	cmp	r2, #1
	bne.n	.L0
	movs	r5, #1
.L0:
	cmp	r2, #11
	bne.n	.L1
	movs	r5, #2
.L1:
	ldrb	r3, [r0, #3]
	cmp	r3, #3
	bne.n	.L2
	movs	r5, #3
.L2:
	cmp	r3, #4
	bne.n	.L3
	movs	r5, #4
.L3:
	cmp	r3, #64
	bne.n	.L4
	movs	r5, #6
.L4:
	ldrb	r0, [r0, #3]
	bl	Func_080772b8
	cmp	r0, #0
	beq.n	.L5
	movs	r5, #5
.L5:
	adds	r0, r5, #0
	pop	{r5}
	pop	{r1}
	bx	r1
