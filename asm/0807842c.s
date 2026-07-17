@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807842c
	.thumb_func
Func_0807842c:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_08078414
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r6, r6, r3
	ldrb	r3, [r6, #0]
	ldrh	r0, [r0, #4]
	cmp	r3, #7
	bls.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldrb	r3, [r6, #0]
	asrs	r0, r3
	movs	r3, #1
	ands	r0, r3
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
