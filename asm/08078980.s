@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078980
	.thumb_func
Func_08078980:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	lsls	r5, r5, #1
	adds	r6, r5, #0
	adds	r7, r0, #0
	adds	r6, #216
	ldrh	r3, [r7, r6]
	ldr	r5, [pc, #68]
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08078414
	cmp	r5, #0
	bne.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldrb	r0, [r0, #3]
	movs	r3, #8
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L2
	movs	r0, #4
	negs	r0, r0
	b.n	.L1
.L2:
	ldrh	r2, [r7, r6]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #3
	negs	r0, r0
	b.n	.L1
.L3:
	movs	r0, #0
.L1:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
