@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b7f70
	.thumb_func
Func_080b7f70:
	push	{lr}
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r4, #15
	ands	r4, r3
	cmp	r4, #1
	bne.n	.L0
	cmp	r1, #0
	bne.n	.L1
	ldr	r0, [r0, #80]
	b.n	.L2
.L0:
	cmp	r4, #2
	bne.n	.L1
	ldr	r3, [r0, #80]
	lsls	r2, r1, #2
	ldr	r0, [r2, r3]
	b.n	.L2
.L1:
	movs	r0, #0
.L2:
	pop	{r1}
	bx	r1
