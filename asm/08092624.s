@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092624
	.thumb_func
Func_08092624:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	mov	r8, r1
	ldr	r2, [r7, #12]
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	movs	r0, #222
	bl	Func_080090c8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	ldr	r5, [r6, #80]
	bl	Func_08004458
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #1
	bne.n	.L1
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08009080
	ldr	r1, [pc, #8]
	adds	r0, r6, #0
	bl	Func_08009098
	b.n	.L2
	.4byte 0x0809fbec
.L1:
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08009080
	ldr	r1, [pc, #128]
	adds	r0, r6, #0
	bl	Func_08009098
.L2:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L3
	adds	r0, r6, #0
	bl	Func_080929d8
.L3:
	movs	r2, #0
	mov	r8, r2
	adds	r3, r6, #0
	adds	r3, #85
	mov	r1, r8
	strb	r1, [r3, #0]
	bl	Func_08004458
	movs	r1, #10
	bl	Func_08002304
	adds	r0, #5
	lsls	r2, r0, #1
	adds	r2, r2, r0
	lsls	r2, r2, #2
	adds	r2, r2, r0
	lsls	r3, r2, #6
	subs	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, r3, r0
	negs	r3, r3
	str	r3, [r6, #52]
	bl	Func_08004458
	movs	r1, #15
	bl	Func_08002304
	ldr	r3, [pc, #60]
	subs	r0, #7
	lsls	r0, r0, #1
	muls	r3, r0
	str	r3, [r6, #48]
	adds	r3, r6, #0
	adds	r3, #100
	mov	r1, r8
	strh	r1, [r3, #0]
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #32]
	str	r3, [r6, #108]
	adds	r3, r5, #0
	adds	r3, #38
	strb	r2, [r3, #0]
	ldr	r3, [r7, #80]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	ldrb	r1, [r5, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r5, #9]
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0809fc04
	.4byte 0x00001999
	.4byte 0x080925e1
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
