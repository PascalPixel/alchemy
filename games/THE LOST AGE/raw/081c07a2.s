.syntax unified
	.thumb
	.set sub_081c05f0, 0x081c05f0
	.set sub_081c0738, 0x081c0738
	.set sub_081c075a, 0x081c075a
	.set sub_081c077a, 0x081c077a
	.set sub_081c28e0, 0x081c28e0
	.global Overlay_081c07a2
Overlay_081c07a2:
	push	{r4, r5, r6, r7, lr}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	sub	sp, #24
	str	r1, [sp, #0]
	adds	r5, r2, #0
	ldr	r1, [pc, #528]
	ldr	r1, [r1, #0]
	str	r1, [sp, #4]
	ldr	r1, [pc, #528]
	adds	r0, r0, r1
	ldrb	r0, [r0, #0]
	strb	r0, [r5, #4]
	ldr	r3, [r5, #64]
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c07e8
	strb	r0, [r5, #5]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c07e6
	strb	r0, [r5, #6]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c07e6
	ldrb	r1, [r5, #4]
	adds	r1, r1, r0
	strb	r1, [r5, #4]
	adds	r3, #1
.L_081c07e6:
	str	r3, [r5, #64]
.L_081c07e8:
	movs	r0, #0
	str	r0, [sp, #20]
	adds	r4, r5, #0
	adds	r4, #36
	ldrb	r2, [r4, #0]
	movs	r0, #192
	tst	r0, r2
	beq.n	.L_081c0838
	ldrb	r3, [r5, #5]
	movs	r0, #64
	tst	r0, r2
	beq.n	.L_081c0808
	ldr	r1, [r5, #44]
	adds	r1, r1, r3
	ldrb	r0, [r1, #0]
	b.n	.L_081c080a
.L_081c0808:
	adds	r0, r3, #0
.L_081c080a:
	movs	r1, #12
	muls	r1, r0
	ldr	r0, [r5, #40]
	adds	r1, r1, r0
	mov	r9, r1
	mov	r6, r9
	ldrb	r1, [r6, #0]
	movs	r0, #192
	tst	r0, r1
	beq.n	.L_081c0820
	b.n	.L_081c09b8
.L_081c0820:
	movs	r0, #128
	tst	r0, r2
	beq.n	.L_081c083c
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_081c0834
	subs	r1, #192
	lsls	r1, r1, #1
	str	r1, [sp, #20]
.L_081c0834:
	ldrb	r3, [r6, #1]
	b.n	.L_081c083c
.L_081c0838:
	mov	r9, r4
	ldrb	r3, [r5, #5]
.L_081c083c:
	str	r3, [sp, #8]
	ldr	r6, [sp, #0]
	ldrb	r1, [r6, #9]
	ldrb	r0, [r5, #29]
	adds	r0, r0, r1
	cmp	r0, #255
	bls.n	.L_081c084c
	movs	r0, #255
.L_081c084c:
	str	r0, [sp, #16]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	movs	r6, #7
	ands	r6, r0
	str	r6, [sp, #12]
	beq.n	.L_081c088c
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
	cmp	r4, #0
	bne.n	.L_081c0864
	b.n	.L_081c09b8
.L_081c0864:
	subs	r6, #1
	lsls	r0, r6, #6
	adds	r4, r4, r0
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c0912
	movs	r0, #64
	tst	r0, r1
	bne.n	.L_081c0912
	ldrb	r1, [r4, #19]
	ldr	r0, [sp, #16]
	cmp	r1, r0
	bcc.n	.L_081c0912
	beq.n	.L_081c0884
	b.n	.L_081c09b8
.L_081c0884:
	ldr	r0, [r4, #44]
	cmp	r0, r5
	bcs.n	.L_081c0912
	b.n	.L_081c09b8
.L_081c088c:
	ldr	r4, [sp, #4]
	movs	r3, #10
	adds	r4, #80
	movs	r7, #199
	movs	r2, #0
	movs	r6, #0
	movs	r0, #3
.L_081c089a:
	ldrb	r1, [r4, #0]
	tst	r1, r7
	bne.n	.L_081c08a6
	subs	r0, #1
	beq.n	.L_081c0912
	adds	r6, r4, #0
.L_081c08a6:
	lsrs	r1, r1, #7
	adcs	r2, r2
	adds	r4, #64
	subs	r3, #1
	bgt.n	.L_081c089a
	cmp	r6, #0
	beq.n	.L_081c08d6
	cmp	r2, #0
	bne.n	.L_081c08bc
	adds	r4, r6, #0
	b.n	.L_081c0912
.L_081c08bc:
	mov	r8, r6
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	b.n	.L_081c08ca
.L_081c08c4:
	ldr	r0, [r4, #44]
	cmp	r0, r7
	beq.n	.L_081c0912
.L_081c08ca:
	subs	r4, #64
	lsrs	r2, r2, #1
	bcs.n	.L_081c08c4
	bne.n	.L_081c08ca
	mov	r4, r8
	b.n	.L_081c0912
.L_081c08d6:
	lsls	r6, r7, #4
	cmp	r2, #0
	bne.n	.L_081c0904
	mov	r8, r2
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	movs	r2, #1
	lsls	r2, r2, #10
	subs	r2, #1
	b.n	.L_081c0904
.L_081c08ea:
	ldrb	r0, [r4, #19]
	cmp	r0, r6
	bne.n	.L_081c08fc
	ldr	r0, [r4, #44]
	cmp	r0, r7
	bcc.n	.L_081c0904
	adds	r7, r0, #0
	mov	r8, r4
	b.n	.L_081c0904
.L_081c08fc:
	bhi.n	.L_081c0904
	ldr	r7, [r4, #44]
	adds	r6, r0, #0
	mov	r8, r4
.L_081c0904:
	subs	r4, #64
	lsrs	r2, r2, #1
	bcs.n	.L_081c08ea
	bne.n	.L_081c0904
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L_081c09b8
.L_081c0912:
	adds	r0, r4, #0
	bl	sub_081c0738
	movs	r1, #0
	str	r1, [r4, #48]
	ldr	r3, [r5, #32]
	str	r3, [r4, #52]
	cmp	r3, #0
	beq.n	.L_081c0926
	str	r4, [r3, #48]
.L_081c0926:
	str	r4, [r5, #32]
	str	r5, [r4, #44]
	ldrb	r0, [r5, #27]
	strb	r0, [r5, #28]
	cmp	r0, r1
	beq.n	.L_081c0938
	adds	r1, r5, #0
	bl	sub_081c075a
.L_081c0938:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	sub_081c28e0
	ldr	r0, [r5, #4]
	str	r0, [r4, #16]
	ldr	r0, [sp, #16]
	strb	r0, [r4, #19]
	ldr	r0, [sp, #8]
	strb	r0, [r4, #8]
	ldr	r0, [sp, #20]
	strb	r0, [r4, #20]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	strb	r0, [r4, #1]
	ldr	r7, [r6, #4]
	str	r7, [r4, #36]
	ldr	r0, [r6, #8]
	str	r0, [r4, #4]
	ldrh	r0, [r5, #30]
	strh	r0, [r4, #12]
	bl	sub_081c077a
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r3, r1, r0
	bpl.n	.L_081c0972
	movs	r3, #0
.L_081c0972:
	ldr	r6, [sp, #12]
	cmp	r6, #0
	beq.n	.L_081c09a0
	mov	r6, r9
	ldrb	r0, [r6, #2]
	strb	r0, [r4, #30]
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	bne.n	.L_081c098c
	movs	r0, #112
	tst	r0, r1
	bne.n	.L_081c098e
.L_081c098c:
	movs	r1, #8
.L_081c098e:
	strb	r1, [r4, #31]
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe004
.L_081c09a0:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	sub_081c05f0
	str	r0, [r4, #32]
	movs	r0, #128
	strb	r0, [r4, #0]
	ldrb	r1, [r5, #0]
	movs	r0, #240
	ands	r0, r1
	strb	r0, [r5, #0]
.L_081c09b8:
	add	sp, #24
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{pc}
	movs	r0, r0
	.4byte 0x03007ff0
	.4byte 0x081c36b0
