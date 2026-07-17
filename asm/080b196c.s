@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b196c
	.thumb_func
Func_080b196c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	bl	Func_08077008
	movs	r3, #1
	negs	r3, r3
	mov	r8, r3
	cmp	r6, r8
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	lsls	r3, r6, #1
	adds	r3, #216
	ldrh	r3, [r0, r3]
	ldr	r5, [pc, #56]
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08077018
	ldrb	r3, [r0, #2]
	movs	r0, #0
	cmp	r3, #6
	beq.n	.L1
	adds	r0, r5, #0
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L1
	adds	r0, r7, #0
	adds	r1, r6, #0
	mov	r2, r8
	bl	Func_080b1f4c
	movs	r0, #1
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
