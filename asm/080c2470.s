@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c2470
	.thumb_func
Func_080c2470:
	push	{r5, r6, lr}
	ldr	r3, [pc, #56]
	adds	r6, r0, #0
	ands	r6, r3
	movs	r5, #0
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L0
	adds	r0, r6, #0
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r5, #1
.L1:
	movs	r3, #4
	ands	r3, r2
	lsls	r5, r5, #1
	cmp	r3, #0
	beq.n	.L2
	adds	r5, #1
.L2:
	lsls	r5, r5, #9
	adds	r5, r5, r6
	adds	r0, r5, #0
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
