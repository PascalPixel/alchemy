@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800c3ec
	.thumb_func
Func_0800c3ec:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L0
	ldr	r3, [r0, #80]
	strh	r1, [r3, #30]
.L0:
	pop	{r0}
	bx	r0
