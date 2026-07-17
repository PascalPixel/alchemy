@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08005c08
	.thumb_func
Func_08005c08:
	push	{lr}
	adds	r4, r2, #0
	movs	r2, #0
	b.n	.L0
.L2:
	subs	r4, #1
	adds	r0, #1
	adds	r1, #1
.L0:
	cmp	r4, #0
	beq.n	.L1
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	beq.n	.L2
.L1:
	adds	r0, r2, #0
	pop	{r1}
	bx	r1
