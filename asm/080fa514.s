@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080fa514
	.thumb_func
Func_080fa514:
	push	{r4, r5, r6, r7, lr}
	ldrb	r5, [r0, #8]
	ldr	r4, [r0, #44]
	cmp	r5, #0
	ble.n	.L0
	movs	r7, #128
.L2:
	ldrb	r1, [r4, #0]
	adds	r0, r7, #0
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L1
	movs	r6, #64
	adds	r0, r6, #0
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L1
	adds	r0, r4, #0
	bl	Func_080fa68c
	strb	r7, [r4, #0]
	movs	r0, #2
	strb	r0, [r4, #15]
	strb	r6, [r4, #19]
	movs	r0, #22
	strb	r0, [r4, #25]
	adds	r1, r4, #0
	adds	r1, #36
	movs	r0, #1
	strb	r0, [r1, #0]
.L1:
	subs	r5, #1
	adds	r4, #80
	cmp	r5, #0
	bgt.n	.L2
.L0:
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
