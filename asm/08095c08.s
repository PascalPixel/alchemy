@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08095c08
	.thumb_func
Func_08095c08:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #436]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	Func_08092054
	movs	r2, #64
	adds	r2, r2, r7
	movs	r6, #0
	ldrsb	r6, [r2, r6]
	mov	r9, r0
	mov	sl, r2
	cmp	r6, #0
	bne.n	.L0
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #24]
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	mov	r8, sp
	str	r2, [sp, #0]
	str	r3, [sp, #8]
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	lsls	r1, r5, #1
	lsls	r3, r0, #1
	adds	r3, r3, r0
	adds	r1, r1, r5
	lsls	r1, r1, #11
	lsls	r3, r3, #11
	lsrs	r3, r3, #16
	lsrs	r1, r1, #16
	subs	r1, r1, r3
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r0, #240
	adds	r1, r1, r3
	lsls	r0, r0, #15
	mov	r2, r8
	bl	Func_0800447c
	mov	r1, r8
	ldr	r3, [r1, #0]
	str	r3, [r7, #12]
	ldr	r3, [r1, #8]
	str	r3, [r7, #16]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #36]
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r6, [r3, #0]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	mov	r1, r9
	adds	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [r1, #80]
	ldr	r0, [r7, #0]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	ldrb	r1, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	adds	r3, r7, #0
	adds	r3, #71
	strb	r6, [r3, #0]
	strh	r6, [r7, #56]
	ldr	r3, [pc, #284]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r0, #134
	bl	Func_080f9010
	b.n	.L1
.L0:
	cmp	r6, #1
	bne.n	.L2
	movs	r2, #56
	ldrsh	r3, [r7, r2]
	cmp	r3, #3
	bne.n	.L3
	ldr	r1, [r7, #0]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	adds	r2, r7, #0
	strb	r3, [r1, #9]
	adds	r2, #71
	movs	r3, #4
	strb	r3, [r2, #0]
	b.n	.L3
.L2:
	cmp	r6, #2
	bne.n	.L4
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L1
	ldr	r3, [r7, #4]
	str	r3, [r7, #20]
	ldr	r3, [r7, #8]
	ldr	r1, [r7, #0]
	str	r3, [r7, #24]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	adds	r2, r7, #0
	adds	r2, #71
	strb	r3, [r1, #9]
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #68
	strb	r0, [r3, #0]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #40
	strh	r3, [r7, #58]
	b.n	.L1
.L4:
	cmp	r6, #3
	bne.n	.L5
	movs	r1, #1
	mov	r8, r1
	adds	r3, r7, #0
	adds	r3, #68
	mov	r2, r8
	strb	r2, [r3, #0]
	ldr	r3, [r7, #20]
	str	r3, [r7, #4]
	ldr	r3, [r7, #24]
	str	r3, [r7, #8]
	mov	r1, r9
	ldr	r3, [r1, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r2, #160
	ldr	r3, [r1, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r1, #16]
	adds	r0, r5, #0
	str	r3, [r5, #8]
	bl	Func_080974d8
	bl	Func_08004458
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #11
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	mov	r1, sl
	ldrb	r3, [r1, #0]
	adds	r3, #1
	strb	r3, [r1, #0]
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r0, #145
	bl	Func_080f9010
	b.n	.L1
.L5:
	cmp	r6, #4
	bne.n	.L6
.L3:
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L1
	mov	r1, sl
	ldrb	r3, [r1, #0]
	subs	r3, #1
	strb	r3, [r1, #0]
	b.n	.L1
.L6:
	cmp	r6, #5
	bne.n	.L1
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L1
	adds	r0, r7, #0
	bl	Func_0809bb34
.L1:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001800
