@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b08b8
	.thumb_func
Func_080b08b8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L0
	movs	r1, #13
	ldrsb	r1, [r7, r1]
	mov	sl, r1
	cmp	r1, #0
	beq.n	.L0
	ldr	r2, [r7, #0]
	ldrb	r6, [r7, #12]
	mov	r8, r2
	adds	r6, #1
	movs	r1, #4
	ldrsh	r2, [r7, r1]
	movs	r1, #8
	ldrsh	r3, [r7, r1]
	strb	r6, [r7, #12]
	lsls	r6, r6, #24
	subs	r3, r3, r2
	asrs	r6, r6, #24
	adds	r0, r6, #0
	muls	r0, r3
	mov	r1, sl
	bl	Func_080022ec
	ldrh	r5, [r7, #4]
	mov	r3, r8
	adds	r5, r5, r0
	strh	r5, [r3, #6]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #56]
	mov	r1, r8
	ands	r5, r3
	mov	r9, r2
	ldr	r3, [pc, #52]
	ldrh	r2, [r1, #22]
	ands	r3, r2
	orrs	r3, r5
	mov	r2, r8
	strh	r3, [r2, #22]
	movs	r1, #6
	ldrsh	r2, [r7, r1]
	movs	r1, #10
	ldrsh	r3, [r7, r1]
	subs	r3, r3, r2
	adds	r0, r6, #0
	muls	r0, r3
	mov	r1, sl
	bl	Func_080022ec
	ldrh	r5, [r7, #6]
	mov	r2, r8
	adds	r5, r5, r0
	strh	r5, [r2, #8]
	strb	r5, [r2, #20]
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0xfffffe00
.L1:
	cmp	r6, sl
	bne.n	.L0
	mov	r3, r9
	strb	r3, [r7, #13]
	strb	r3, [r7, #12]
.L0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
