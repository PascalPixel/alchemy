.syntax unified
	.thumb
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_080eb2d8, 0x080eb2d8
	.global Func_080eb3d0
	.thumb_func
Func_080eb3d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r2
	mov	sl, r9
	sub	sp, #8
	adds	r7, r3, #0
	movs	r2, #0
	mov	r3, sl
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	mov	fp, r2
	cmp	r3, #0
	blt.n	.L_080eb492
.L_080eb3f4:
	ldr	r2, [sp, #0]
	ldr	r5, [sp, #4]
	add	r2, fp
	mov	r8, r2
	add	r5, sl
	adds	r0, r5, #0
	mov	r1, r8
	adds	r2, r7, #0
	bl	sub_080eb2d8
	ldr	r3, [sp, #0]
	mov	r2, fp
	subs	r6, r3, r2
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080eb2d8
	ldr	r3, [sp, #4]
	mov	r2, sl
	subs	r5, r3, r2
	adds	r0, r5, #0
	mov	r1, r8
	adds	r2, r7, #0
	bl	sub_080eb2d8
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080eb2d8
	ldr	r3, [sp, #0]
	ldr	r5, [sp, #4]
	add	r3, sl
	mov	r8, r3
	add	r5, fp
	adds	r0, r5, #0
	mov	r1, r8
	adds	r2, r7, #0
	bl	sub_080eb2d8
	ldr	r2, [sp, #0]
	mov	r3, sl
	subs	r6, r2, r3
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080eb2d8
	ldr	r2, [sp, #4]
	mov	r3, fp
	subs	r5, r2, r3
	adds	r0, r5, #0
	mov	r1, r8
	adds	r2, r7, #0
	bl	sub_080eb2d8
	adds	r2, r7, #0
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080eb2d8
	mov	r2, fp
	lsls	r3, r2, #1
	mov	r2, r9
	subs	r3, r2, r3
	adds	r3, #1
	mov	r9, r3
	cmp	r3, #0
	bge.n	.L_080eb48a
	mov	r2, sl
	subs	r2, #1
	lsls	r3, r2, #1
	add	r9, r3
	mov	sl, r2
.L_080eb48a:
	movs	r3, #1
	add	fp, r3
	cmp	sl, fp
	bge.n	.L_080eb3f4
.L_080eb492:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r4, r2, #0
	ldr	r2, [sp, #28]
	mov	r9, r3
	mov	lr, r2
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #180
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #96]
	mov	r8, r3
	mov	ip, r2
	cmp	r1, #0
	blt.n	.L_080eb512
	mov	r7, r9
	adds	r3, r1, r7
	cmp	r3, #63
	bgt.n	.L_080eb512
	cmp	r4, #0
	blt.n	.L_080eb512
	mov	r2, lr
	adds	r3, r4, r2
	cmp	r3, #63
	bgt.n	.L_080eb512
	movs	r6, #0
	cmp	r6, lr
	bge.n	.L_080eb512
	lsls	r3, r4, #6
	adds	r3, r3, r1
	lsls	r5, r3, #1
.L_080eb4e6:
	mov	r3, r9
	cmp	r3, #0
	ble.n	.L_080eb50a
	mov	r7, r8
	adds	r1, r5, r7
	mov	r2, r9
.L_080eb4f2:
	ldrb	r4, [r0, #0]
	adds	r3, r4, #0
	cmp	r3, #0
	beq.n	.L_080eb500
	ldrh	r3, [r1, #0]
	mov	r7, ip
	strb	r4, [r7, r3]
.L_080eb500:
	subs	r2, #1
	adds	r0, #1
	adds	r1, #2
	cmp	r2, #0
	bne.n	.L_080eb4f2
.L_080eb50a:
	adds	r6, #1
	adds	r5, #128
	cmp	r6, lr
	blt.n	.L_080eb4e6
.L_080eb512:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
.L_080eb51c:
	push	{r5, lr}
	cmp	r1, #0
	ble.n	.L_080eb53e
	ldr	r5, [pc, #28]
	movs	r4, #31
.L_080eb526:
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080eb536
	adds	r3, r4, #0
	ands	r3, r2
	ldrb	r3, [r5, r3]
	strb	r3, [r0, #0]
.L_080eb536:
	subs	r1, #1
	adds	r0, #1
	cmp	r1, #0
	bne.n	.L_080eb526
.L_080eb53e:
	pop	{r5, pc}
	.2byte 0x3bdc
	.2byte 0x080f
.L_080eb544:
	push	{r5, lr}
	cmp	r1, #0
	ble.n	.L_080eb566
	ldr	r5, [pc, #28]
	movs	r4, #31
.L_080eb54e:
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080eb55e
	adds	r3, r4, #0
	ands	r3, r2
	ldrb	r3, [r5, r3]
	strb	r3, [r0, #0]
.L_080eb55e:
	subs	r1, #1
	adds	r0, #1
	cmp	r1, #0
	bne.n	.L_080eb54e
.L_080eb566:
	pop	{r5, pc}
	.2byte 0x3bfc
	.2byte 0x080f
.L_080eb56c:
	push	{r5, lr}
	cmp	r1, #0
	ble.n	.L_080eb58e
	ldr	r5, [pc, #28]
	movs	r4, #31
.L_080eb576:
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080eb586
	adds	r3, r4, #0
	ands	r3, r2
	ldrb	r3, [r5, r3]
	strb	r3, [r0, #0]
.L_080eb586:
	subs	r1, #1
	adds	r0, #1
	cmp	r1, #0
	bne.n	.L_080eb576
.L_080eb58e:
	pop	{r5, pc}
	.2byte 0x3c1c
	.2byte 0x080f
	push	{r5, lr}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #240
	ldr	r3, [r3, #0]
	ldr	r4, [r2, #96]
	mov	ip, r3
	adds	r3, #188
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #224]
	lsls	r3, r3, #5
	adds	r1, r3, r2
	mov	r2, ip
	adds	r2, #164
	ldr	r3, [r2, #0]
	mov	r5, ip
	adds	r3, #1
	str	r3, [r2, #0]
	adds	r5, #193
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r3, #1
	cmp	r3, #10
	bhi.n	.L_080eb686
	ldr	r2, [pc, #200]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080eb608
	.4byte 0x080eb5fc
	.4byte 0x080eb626
	.4byte 0x080eb640
	.4byte 0x080eb65a
	.4byte 0x080eb686
	.4byte 0x080eb686
	.4byte 0x080eb686
	.4byte 0x080eb686
	.4byte 0x080eb686
	.2byte 0xb674
	.2byte 0x080e
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r2, [pc, #140]
	b.n	.L_080eb682
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	ldr	r3, [pc, #124]
	adds	r0, r4, #0
	lsls	r1, r1, #5
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe02f
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r0, r4, #0
	bl	.L_080eb51c
	b.n	.L_080eb686
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r2, [pc, #72]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r0, r4, #0
	bl	.L_080eb544
	b.n	.L_080eb686
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r0, r4, #0
	bl	.L_080eb56c
	b.n	.L_080eb686
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	adds	r0, r4, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #24]
.L_080eb682:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080eb686:
	movs	r3, #0
	strb	r3, [r5, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0601
	push	{r4, r6, r7, lr}
	lsrs	r6, r1, #32
	lsls	r0, r0, #16
	strh	r0, [r0, #32]
	lsls	r0, r4, #9
	lsls	r0, r0, #12
	lsrs	r0, r0, #32
	strh	r0, [r0, #32]
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #240
	ldr	r5, [r3, #0]
	sub	sp, #4
	adds	r3, r5, #0
	adds	r3, #168
	ldr	r3, [r3, #0]
	lsls	r3, r3, #16
	mov	r9, r3
	adds	r3, r5, #0
	adds	r3, #172
	ldr	r3, [r3, #0]
	lsls	r3, r3, #16
	mov	sl, r3
	adds	r3, r5, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	mov	r8, r3
	adds	r3, r5, #0
	adds	r3, #180
	mov	r1, r8
	ldr	r7, [r3, #0]
	cmp	r1, #0
	bne.n	.L_080eb6de
	b.n	.L_080eb80c
.L_080eb6de:
	cmp	r7, #0
	bne.n	.L_080eb6e4
	b.n	.L_080eb80c
.L_080eb6e4:
	adds	r6, r5, #0
	adds	r6, #192
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	ldrb	r2, [r6, #0]
	cmp	r3, #1
	bne.n	.L_080eb75a
	adds	r2, r5, #0
	adds	r2, #188
	ldrh	r2, [r2, #0]
	movs	r3, #192
	str	r2, [sp, #0]
	lsls	r3, r3, #24
	adds	r0, r5, #0
	movs	r1, #32
	movs	r2, #32
	bl	sub_080eaf98
	ldrb	r2, [r5, #5]
	movs	r3, #33
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r2
	strb	r3, [r5, #9]
	adds	r2, r5, #0
	adds	r2, #191
	ldrb	r2, [r2, #0]
	movs	r1, #3
	ands	r2, r1
	movs	r1, #13
	negs	r1, r1
	lsls	r2, r2, #2
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r5, #9]
	adds	r3, r5, #0
	adds	r3, #190
	ldrb	r3, [r3, #0]
	mov	r2, r9
	strh	r3, [r5, #30]
	mov	r1, r8
	mov	r3, sl
	str	r2, [r5, #12]
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	str	r7, [r5, #24]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	adds	r0, r5, #0
	strh	r3, [r5, #28]
	bl	sub_080eb01c
	ldrb	r2, [r6, #0]
.L_080eb75a:
	movs	r1, #128
	lsls	r3, r2, #24
	lsls	r1, r1, #18
	cmp	r3, r1
	bne.n	.L_080eb7c6
	adds	r2, r5, #0
	adds	r2, #188
	ldrh	r2, [r2, #0]
	movs	r3, #192
	str	r2, [sp, #0]
	lsls	r3, r3, #24
	adds	r0, r5, #0
	movs	r1, #32
	movs	r2, #32
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r5, #5]
	ldrb	r3, [r5, #9]
	movs	r1, #15
	ands	r1, r3
	strb	r1, [r5, #9]
	adds	r2, r5, #0
	adds	r2, #191
	ldrb	r2, [r2, #0]
	movs	r3, #3
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	lsls	r2, r2, #2
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r5, #9]
	adds	r3, r5, #0
	adds	r3, #190
	ldrb	r3, [r3, #0]
	mov	r2, r9
	strh	r3, [r5, #30]
	mov	r1, r8
	mov	r3, sl
	str	r2, [r5, #12]
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	str	r7, [r5, #24]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	adds	r0, r5, #0
	strh	r3, [r5, #28]
	bl	sub_080eb01c
	ldrb	r2, [r6, #0]
.L_080eb7c6:
	lsls	r3, r2, #24
	movs	r2, #192
	lsls	r2, r2, #18
	cmp	r3, r2
	bne.n	.L_080eb80c
	adds	r2, r5, #0
	adds	r2, #191
	ldrb	r2, [r2, #0]
	ldrb	r1, [r5, #9]
	movs	r3, #3
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	lsls	r2, r2, #2
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r5, #9]
	adds	r3, r5, #0
	adds	r3, #190
	ldrb	r3, [r3, #0]
	mov	r1, sl
	strh	r3, [r5, #30]
	mov	r2, r8
	mov	r3, r9
	str	r3, [r5, #12]
	str	r1, [r5, #16]
	str	r2, [r5, #20]
	str	r7, [r5, #24]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	adds	r0, r5, #0
	strh	r3, [r5, #28]
	bl	sub_080eb01c
.L_080eb80c:
	adds	r2, r5, #0
	adds	r2, #160
	ldr	r3, [r2, #0]
	add	sp, #4
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.align 2, 0
