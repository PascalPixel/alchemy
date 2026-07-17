@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800c548
	.thumb_func
Func_0800c548:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r0, [r0, #80]
	movs	r3, #3
	ands	r1, r3
	ldrb	r2, [r0, #5]
	movs	r3, #13
	negs	r3, r3
	lsls	r1, r1, #2
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #5]
.L0:
	pop	{r0}
	bx	r0
