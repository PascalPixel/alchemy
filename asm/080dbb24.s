@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080dbb24
	.thumb_func
Func_080dbb24:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	mov	r8, r0
	mov	r9, r1
	ldr	r7, [r3, #0]
	movs	r6, #0
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #3
	ands	r2, r3
	movs	r1, #13
	lsls	r2, r2, #2
	negs	r1, r1
	ldr	r5, [pc, #72]
	mov	sl, r2
	mov	fp, r1
.L2:
	mov	r0, r9
	bl	Func_08009030
	str	r0, [r5, r7]
	cmp	r0, #0
	beq.n	.L1
	adds	r2, r0, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	bl	Func_08009020
	ldr	r2, [r5, r7]
	ldrb	r3, [r2, #9]
	mov	r1, fp
	ands	r3, r1
	mov	r1, sl
	orrs	r3, r1
	strb	r3, [r2, #9]
.L1:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, r8
	bne.n	.L2
.L0:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x000077d8
