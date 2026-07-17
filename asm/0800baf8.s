@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800baf8
	.thumb_func
Func_0800baf8:
	push	{lr}
	adds	r3, r0, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	adds	r0, #40
	adds	r4, r3, #0
.L2:
	ldmia	r0!, {r2}
	cmp	r2, #0
	beq.n	.L1
	ldr	r3, [r2, #12]
	cmp	r3, #0
	beq.n	.L1
	strb	r1, [r2, #21]
.L1:
	subs	r4, #1
	cmp	r4, #0
	bne.n	.L2
.L0:
	pop	{r0}
	bx	r0
