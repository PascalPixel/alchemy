.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.global UiText_MeasureStringVariant
	.global Func_08018a50
	.thumb_func
UiText_MeasureStringVariant:
Func_08018a50:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	str	r1, [sp, #8]
	mov	r8, r3
	ldr	r3, [pc, #492]
	add	r1, sp, #28
	ldr	r5, [r3, #0]
	movs	r3, #15
	str	r3, [r1, #0]
	str	r3, [r1, #4]
	str	r3, [r1, #8]
	str	r3, [r1, #12]
	str	r3, [r1, #16]
	str	r3, [r1, #20]
	str	r3, [r1, #24]
	str	r3, [r1, #28]
	str	r3, [r1, #32]
	str	r3, [r1, #36]
	str	r3, [r1, #40]
	str	r3, [r1, #44]
	str	r3, [r1, #48]
	str	r3, [r1, #52]
	str	r3, [r1, #56]
	str	r3, [r1, #60]
	add	r3, sp, #20
	mov	lr, r2
	mov	r9, r3
	movs	r2, #0
	add	r3, sp, #12
	str	r2, [sp, #4]
	mov	sl, r2
	movs	r4, #0
	movs	r6, #0
	movs	r7, #0
	mov	ip, r2
	mov	fp, r3
.L_08018aa4:
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r5, r3]
	ldr	r3, [pc, #424]
	adds	r0, #1
	ands	r0, r3
	cmp	r2, #31
	bls.n	.L_08018ae0
	cmp	r2, #32
	bne.n	.L_08018ac2
	adds	r4, #5
	adds	r6, #1
	b.n	.L_08018aa4
.L_08018ac2:
	ldr	r3, [pc, #408]
	subs	r2, #32
	lsls	r2, r2, #5
	ldrh	r2, [r3, r2]
	ldr	r3, [pc, #404]
	adds	r3, r5, r3
	ldrh	r3, [r3, #0]
	str	r3, [sp, #0]
	cmp	r3, #1
	beq.n	.L_08018ada
	cmp	r3, #5
	bne.n	.L_08018adc
.L_08018ada:
	adds	r2, #1
.L_08018adc:
	adds	r4, r4, r2
	b.n	.L_08018aa4
.L_08018ae0:
	cmp	r2, #28
	bhi.n	.L_08018aa4
	lsls	r3, r2, #2
	ldr	r2, [pc, #380]
	ldr	r3, [r3, r2]
	mov	pc, r3
	ldrh	r2, [r2, #28]
	lsrs	r1, r0, #32
	ldrh	r4, [r5, #28]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r0, [r4, #26]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r6, [r3, #30]
	lsrs	r1, r0, #32
	ldrh	r6, [r1, #30]
	lsrs	r1, r0, #32
	ldrh	r6, [r3, #30]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r6, [r0, #30]
	lsrs	r1, r0, #32
	ldrh	r6, [r3, #30]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r6, [r3, #30]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r6, [r0, #30]
	lsrs	r1, r0, #32
	mov	r3, r9
	mov	r2, ip
	adds	r6, #1
	strh	r6, [r3, r2]
	mov	r3, fp
	strh	r4, [r3, r2]
	cmp	r7, #0
	bne.n	.L_08018b76
	cmp	sl, r4
	bcs.n	.L_08018b76
	mov	sl, r4
.L_08018b76:
	ldr	r3, [sp, #4]
	cmp	r3, #2
	bhi.n	.L_08018b84
	adds	r3, #1
	str	r3, [sp, #4]
	lsls	r3, r3, #1
	mov	ip, r3
.L_08018b84:
	lsls	r2, r7, #2
	ldr	r3, [r1, r2]
	adds	r3, #15
	movs	r6, #0
	movs	r4, #0
	str	r3, [r1, r2]
	b.n	.L_08018aa4
	mov	r2, r9
	mov	r3, ip
	adds	r6, #1
	strh	r6, [r2, r3]
	mov	r2, fp
	strh	r4, [r2, r3]
	cmp	r7, #0
	bne.n	.L_08018ba8
	cmp	sl, r4
	bcs.n	.L_08018ba8
	mov	sl, r4
.L_08018ba8:
	adds	r7, #1
	b.n	.L_08018be6
	mov	r2, r9
	mov	r3, ip
	adds	r6, #1
	strh	r6, [r2, r3]
	mov	r2, fp
	strh	r4, [r2, r3]
	cmp	r7, #0
	bne.n	.L_08018bc2
	cmp	sl, r4
	bcs.n	.L_08018bc2
	mov	sl, r4
.L_08018bc2:
	adds	r7, #1
	b.n	.L_08018aa4
	ldr	r3, [pc, #144]
	adds	r0, #1
	ands	r0, r3
	b.n	.L_08018bde
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r5, r3]
	ldr	r3, [pc, #132]
	adds	r3, r5, r3
	strh	r2, [r3, #0]
.L_08018bde:
	ldr	r3, [pc, #120]
	adds	r0, #1
	ands	r0, r3
	b.n	.L_08018aa4
.L_08018be6:
	ldr	r2, [pc, #128]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08018bf4
	movs	r3, #2
	add	sl, r3
.L_08018bf4:
	movs	r4, #0
	cmp	r4, r7
	bcs.n	.L_08018c20
	adds	r0, r1, #0
	adds	r5, r0, #0
.L_08018bfe:
	cmp	r4, #0
	bne.n	.L_08018c0a
	ldr	r3, [r5, #0]
	mov	r2, lr
	str	r3, [r2, #0]
	b.n	.L_08018c18
.L_08018c0a:
	mov	r2, lr
	ldr	r3, [r2, #0]
	ldr	r2, [r0, #0]
	cmp	r3, r2
	bcs.n	.L_08018c18
	mov	r3, lr
	str	r2, [r3, #0]
.L_08018c18:
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r7
	bcc.n	.L_08018bfe
.L_08018c20:
	ldr	r3, [sp, #8]
	mov	r2, sl
	str	r2, [r3, #0]
	mov	r3, sl
	adds	r3, #19
	lsrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r3, #16
	mov	r2, r8
	mov	sl, r3
	cmp	r2, #0
	beq.n	.L_08018c9a
	movs	r6, #0
	movs	r5, #0
.L_08018c3c:
	mov	r2, r9
	ldrh	r3, [r5, r2]
	cmp	r3, #1
	bhi.n	.L_08018c6c
	ldr	r3, [pc, #8]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	b.n	.L_08018c8e
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e8c
	.4byte 0x000001ff
	.4byte 0x08032224
	.4byte 0x00000eac
	.4byte 0x08018aec
	.2byte 0x0ea4
	.2byte 0x0000
.L_08018c6c:
	mov	r2, fp
	ldrh	r3, [r5, r2]
	mov	r2, sl
	subs	r0, r2, r3
	subs	r0, #4
	cmp	r0, #0
	bge.n	.L_08018c7c
	movs	r0, #0
.L_08018c7c:
	mov	r3, r9
	ldrh	r1, [r5, r3]
	lsls	r0, r0, #8
	subs	r1, #1
	bl	sub_080022ec
	mov	r2, r8
	movs	r3, #2
	strh	r0, [r2, #0]
.L_08018c8e:
	add	r8, r3
	ldr	r2, [sp, #4]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r2
	bls.n	.L_08018c3c
.L_08018c9a:
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
