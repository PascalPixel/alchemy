@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801be80
	.thumb_func
Func_0801be80:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r1, #231
	adds	r5, r0, #0
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #356]
	adds	r3, r3, r5
	mov	sl, r3
	ldrh	r3, [r3, #0]
	movs	r0, #0
	adds	r2, r2, r3
	mov	r8, r0
	adds	r0, r5, #0
	mov	r9, r2
	bl	Func_0801ba34
	mov	r0, sl
	ldrh	r1, [r0, #0]
	adds	r0, r5, #0
	bl	Func_0801b9a8
	ldr	r1, [pc, #328]
	movs	r3, #33
	adds	r2, r5, r1
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #320]
	ldr	r0, [pc, #320]
	movs	r7, #0
	adds	r3, r5, r2
	strh	r7, [r5, #10]
	strh	r7, [r5, #62]
	strh	r7, [r3, #0]
	adds	r3, r5, r0
	strh	r7, [r3, #0]
	bl	Func_0801c21c
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r6, [r3, #0]
	cmp	r6, #0
	beq.n	.L0
	mov	r2, sl
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L0
.L1:
	ldr	r6, [r6, #4]
	movs	r3, #1
	add	r8, r3
	cmp	r6, #0
	beq.n	.L0
	ldrh	r3, [r2, #0]
	cmp	r3, r8
	bne.n	.L1
.L0:
	ldrh	r3, [r6, #16]
	strh	r3, [r6, #28]
	ldrh	r3, [r6, #18]
	movs	r0, #210
	strh	r3, [r6, #30]
	lsls	r0, r0, #2
	adds	r3, r5, r0
	ldr	r7, [r3, #0]
	cmp	r7, #0
	beq.n	.L2
.L4:
	cmp	r7, r6
	beq.n	.L3
	ldrh	r3, [r6, #16]
	strh	r3, [r7, #24]
	movs	r0, #16
	ldrsh	r2, [r7, r0]
	movs	r1, #16
	ldrsh	r3, [r6, r1]
	subs	r3, r3, r2
	asrs	r3, r3, #1
	strh	r3, [r7, #20]
.L3:
	ldr	r7, [r7, #4]
	cmp	r7, #0
	bne.n	.L4
.L2:
	movs	r0, #2
	bl	Func_080030f8
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r7, [r3, #0]
	cmp	r7, #0
	beq.n	.L5
	movs	r2, #0
	mov	r8, r2
.L7:
	cmp	r7, r6
	beq.n	.L6
	ldrh	r0, [r7, #12]
	bl	Func_08003f3c
	mov	r3, r8
	strh	r3, [r7, #10]
.L6:
	ldr	r7, [r7, #4]
	cmp	r7, #0
	bne.n	.L7
.L5:
	movs	r0, #210
	lsls	r0, r0, #2
	adds	r3, r5, r0
	str	r6, [r3, #0]
	movs	r3, #0
	str	r3, [r6, #0]
	str	r3, [r6, #4]
	movs	r1, #211
	movs	r3, #4
	strh	r3, [r6, #24]
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r7, [r3, #0]
	movs	r2, #0
	mov	r8, r2
	cmp	r7, #0
	beq.n	.L8
.L9:
	ldrh	r3, [r6, #24]
	adds	r3, #16
	strh	r3, [r6, #24]
	ldr	r7, [r7, #4]
	movs	r3, #1
	add	r8, r3
	cmp	r7, #0
	bne.n	.L9
.L8:
	mov	r0, r8
	lsls	r1, r0, #1
	movs	r3, #233
	movs	r0, #231
	lsls	r3, r3, #2
	lsls	r0, r0, #2
	adds	r2, r1, r3
	adds	r3, r5, r0
	ldrh	r3, [r3, #0]
	strh	r3, [r5, r2]
	ldr	r3, [pc, #92]
	movs	r2, #235
	lsls	r2, r2, #2
	adds	r0, r1, r2
	adds	r1, r5, r3
	ldrh	r3, [r1, #0]
	adds	r2, r5, #2
	strh	r3, [r2, r0]
	movs	r0, #16
	ldrsh	r2, [r6, r0]
	movs	r0, #24
	ldrsh	r3, [r6, r0]
	ldr	r0, [pc, #84]
	subs	r3, r3, r2
	movs	r2, #0
	mov	r8, r2
	asrs	r3, r3, #1
	strh	r3, [r6, #20]
	mov	r2, r8
	adds	r3, r5, r0
	strh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	ldrh	r2, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #2
	bl	Func_080030f8
	movs	r0, #1
	bl	Func_0801a910
	ldrh	r3, [r6, #10]
	adds	r7, r0, #0
	strh	r3, [r7, #10]
	ldrh	r3, [r6, #32]
	strh	r3, [r7, #32]
	ldrh	r3, [r6, #8]
	strh	r3, [r7, #8]
	ldrh	r3, [r6, #12]
	strh	r3, [r7, #12]
	ldrh	r3, [r6, #14]
	strh	r3, [r7, #14]
	ldrh	r2, [r6, #16]
	strh	r2, [r7, #16]
	b.n	.L10
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x0000039e
	.4byte 0x000003a2
	.4byte 0x000002e2
	.4byte 0x000002fa
	.4byte 0x0000039a
.L10:
	ldrh	r3, [r6, #18]
	strh	r2, [r7, #24]
	strh	r3, [r7, #18]
	strh	r3, [r7, #26]
	ldrh	r3, [r6, #28]
	strh	r3, [r7, #28]
	ldrh	r3, [r6, #30]
	strh	r3, [r7, #30]
	mov	r3, r8
	strh	r3, [r7, #20]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r0, r8
	strh	r0, [r7, #22]
	strh	r3, [r7, #34]
	strh	r3, [r7, #38]
	adds	r0, r7, #0
	adds	r0, #40
	ldrb	r3, [r0, #5]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #33
	negs	r3, r3
	ldrb	r1, [r0, #7]
	ands	r2, r3
	movs	r4, #63
	adds	r3, #16
	ands	r2, r3
	adds	r3, r4, #0
	ands	r3, r1
	ands	r2, r4
	movs	r1, #64
	strb	r2, [r0, #5]
	orrs	r3, r1
	ldrb	r2, [r0, #9]
	strb	r3, [r0, #7]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r0, #9]
	ldrh	r3, [r7, #14]
	ldr	r2, [pc, #88]
	ldrh	r1, [r0, #8]
	ands	r2, r3
	ldr	r3, [pc, #84]
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #210
	mov	r1, r8
	lsls	r2, r2, #2
	strh	r3, [r0, #8]
	strh	r1, [r6, #10]
	adds	r3, r5, r2
	mov	r0, r8
	movs	r1, #211
	str	r0, [r3, #0]
	lsls	r1, r1, #2
	adds	r0, r5, r1
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L11
	adds	r6, r3, #0
	ldr	r2, [r6, #4]
	cmp	r2, #0
	beq.n	.L12
.L13:
	adds	r6, r2, #0
	ldr	r3, [r6, #4]
	adds	r2, r3, #0
	cmp	r3, #0
	bne.n	.L13
.L12:
	movs	r3, #0
	str	r7, [r6, #4]
	str	r6, [r7, #0]
	b.n	.L14
.L11:
	str	r7, [r0, #0]
	str	r3, [r7, #0]
.L14:
	str	r3, [r7, #4]
	mov	r0, r9
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	bx	lr
