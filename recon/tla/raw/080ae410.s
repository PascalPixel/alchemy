.syntax unified
	.thumb
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08038348, 0x08038348
	.set sub_080ad3f8, 0x080ad3f8
	.set sub_080ae16c, 0x080ae16c
	.set sub_080ae358, 0x080ae358
	.set sub_080ae3fc, 0x080ae3fc
	.set sub_080aec04, 0x080aec04
	.set sub_080aed6c, 0x080aed6c
	.set sub_080aeec8, 0x080aeec8
	.set sub_080af148, 0x080af148
	.set sub_080af298, 0x080af298
	.set sub_080afdd8, 0x080afdd8
	.set sub_080b1004, 0x080b1004
	.global Func_080ae410
	.thumb_func
Func_080ae410:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #164
	lsls	r3, r3, #3
	sub	sp, #24
	movs	r2, #144
	adds	r3, r0, r3
	str	r1, [sp, #4]
	lsls	r2, r2, #2
	movs	r1, #64
	str	r3, [sp, #0]
	adds	r1, r1, r0
	adds	r2, r2, r0
	movs	r4, #0
	mov	r9, r1
	mov	fp, r2
	movs	r7, #0
	mov	r8, r4
.L_080ae43e:
	lsls	r3, r7, #2
	movs	r5, #0
	adds	r3, r3, r7
	mov	sl, r5
	lsls	r3, r3, #2
	movs	r5, #128
	adds	r6, r3, #0
	lsls	r5, r5, #4
	adds	r6, #48
	add	r5, r8
.L_080ae452:
	mov	r0, r9
	adds	r1, r6, #0
	bl	sub_080ae3fc
	cmp	r0, #0
	beq.n	.L_080ae466
	adds	r0, r5, #0
	bl	sub_08016cfc
	b.n	.L_080ae46c
.L_080ae466:
	adds	r0, r5, #0
	bl	sub_08016d18
.L_080ae46c:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r6, #1
	adds	r5, #1
	cmp	r1, #6
	ble.n	.L_080ae452
	movs	r2, #7
	adds	r7, #1
	add	r8, r2
	cmp	r7, #3
	ble.n	.L_080ae43e
	ldr	r5, [pc, #356]
	movs	r7, #0
.L_080ae488:
	ldrh	r1, [r5, #0]
	mov	r0, r9
	adds	r5, #2
	bl	sub_080ae3fc
	cmp	r0, #0
	beq.n	.L_080ae4a2
	movs	r3, #130
	lsls	r3, r3, #4
	adds	r0, r7, r3
	bl	sub_08016cfc
	b.n	.L_080ae4ac
.L_080ae4a2:
	movs	r4, #130
	lsls	r4, r4, #4
	adds	r0, r7, r4
	bl	sub_08016d18
.L_080ae4ac:
	adds	r7, #1
	cmp	r7, #5
	ble.n	.L_080ae488
	movs	r5, #128
	lsls	r5, r5, #4
	adds	r5, #34
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ae4ca
	adds	r0, r5, #0
	bl	sub_08016d18
	b.n	.L_080ae4d0
.L_080ae4ca:
	adds	r0, r5, #0
	bl	sub_08016cfc
.L_080ae4d0:
	ldr	r4, [pc, #284]
	mov	r0, fp
	movs	r5, #163
	ldr	r3, [r0, #16]
	lsls	r5, r5, #2
	adds	r2, r4, r5
	str	r3, [r2, #0]
	ldr	r1, [sp, #4]
	cmp	r1, #0
	bne.n	.L_080ae51c
	movs	r0, #147
	lsls	r0, r0, #1
	mov	r2, fp
	adds	r0, #255
	ldrb	r3, [r2, r0]
	adds	r0, r4, r0
	strb	r3, [r0, #0]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #38
	ldrb	r3, [r2, r1]
	adds	r1, r4, r1
	strb	r3, [r1, #0]
	movs	r2, #139
	lsls	r2, r2, #2
	mov	r5, fp
	ldrb	r3, [r5, r2]
	strb	r3, [r4, r2]
	subs	r2, #2
	ldrb	r3, [r5, r2]
	strb	r3, [r4, r2]
	adds	r2, #32
	ldrb	r3, [r5, r2]
	strb	r3, [r4, r2]
	ldrb	r0, [r0, #0]
	ldrb	r1, [r1, #0]
	bl	sub_08038348
.L_080ae51c:
	ldr	r2, [sp, #0]
	ldr	r3, [pc, #212]
	movs	r1, #8
	movs	r0, #4
	add	r1, sp
	mov	sl, r0
	movs	r7, #0
	mov	r9, r1
	mov	r8, r2
	mov	fp, r3
.L_080ae530:
	adds	r0, r7, #0
	bl	sub_08016ca4
	mov	r2, r9
	mov	r3, r8
	adds	r5, r0, #0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldrh	r1, [r3, #0]
	adds	r0, r5, #0
	strh	r1, [r2, #0]
	mov	r1, r8
	ldrb	r3, [r3, #2]
	strb	r3, [r2, #2]
	movs	r2, #166
	lsls	r2, r2, #1
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c2a
	mov	r3, r9
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldrh	r1, [r3, #0]
	strh	r1, [r2, #0]
	ldrb	r3, [r3, #2]
	strb	r3, [r2, #2]
	ldr	r2, [sp, #4]
	cmp	r2, #0
	bne.n	.L_080ae57a
	movs	r4, #148
	lsls	r4, r4, #1
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r2, r5, r0
	strh	r3, [r2, #0]
.L_080ae57a:
	movs	r6, #14
	adds	r5, #244
.L_080ae57e:
	ldrh	r0, [r5, #0]
	subs	r5, #2
	cmp	r0, #0
	beq.n	.L_080ae5a0
	bl	sub_080aec04
	ldrb	r2, [r0, #3]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ae5a0
.L_080ae594:
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_080af148
	cmp	r0, #1
	beq.n	.L_080ae594
.L_080ae5a0:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_080ae57e
	adds	r0, r7, #0
	bl	sub_080ad3f8
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r7, #1
	add	r8, r1
	cmp	r7, sl
	blt.n	.L_080ae530
	bl	sub_080b1004
	movs	r0, #1
	bl	sub_080ae16c
	ldr	r2, [sp, #4]
	movs	r0, #1
	ands	r0, r2
	adds	r0, #44
	bl	sub_08016cfc
	movs	r0, #47
	bl	sub_08016cfc
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #32]
	ldr	r3, [r3, #4]
	add	sp, #24
	str	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080b1284
	.4byte 0x02000240
	.4byte 0x03000730
	.2byte 0x117c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #144
	movs	r7, #128
	lsls	r1, r1, #2
	movs	r3, #164
	lsls	r7, r7, #3
	adds	r1, r1, r0
	lsls	r3, r3, #3
	adds	r7, #101
	ldr	r2, [pc, #176]
	adds	r6, r0, r3
	mov	r8, r1
	adds	r3, r0, r7
	movs	r1, #147
	ldrb	r3, [r3, #0]
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r5, r2, r1
	strb	r3, [r5, #0]
	adds	r7, #1
	adds	r3, r0, r7
	ldrb	r3, [r3, #0]
	adds	r1, #1
	adds	r4, r2, r1
	strb	r3, [r4, #0]
	adds	r7, #6
	adds	r3, r0, r7
	ldrb	r1, [r3, #0]
	movs	r7, #139
	lsls	r7, r7, #2
	adds	r3, r2, r7
	strb	r1, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #106
	adds	r3, r0, r1
	ldrb	r1, [r3, #0]
	subs	r7, #2
	adds	r3, r2, r7
	strb	r1, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #138
	adds	r0, r0, r1
	ldrb	r3, [r0, #0]
	adds	r7, #32
	adds	r2, r2, r7
	strb	r3, [r2, #0]
	sub	sp, #16
	ldrb	r0, [r5, #0]
	ldrb	r1, [r4, #0]
	bl	sub_08038348
	movs	r7, #8
	movs	r5, #0
.L_080ae66e:
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r1, r5, #0
	muls	r1, r3
	adds	r1, r6, r1
	ldrh	r2, [r1, #16]
	strh	r2, [r0, #16]
	ldrh	r3, [r1, #18]
	strh	r2, [r0, #20]
	strh	r3, [r0, #18]
	strh	r3, [r0, #22]
	movs	r2, #146
	ldrh	r3, [r1, #24]
	lsls	r2, r2, #1
	strh	r3, [r0, #24]
	ldrh	r3, [r1, #26]
	strh	r3, [r0, #26]
	ldrh	r3, [r1, #28]
	strh	r3, [r0, #28]
	ldrb	r3, [r1, #30]
	strb	r3, [r0, #30]
	ldrb	r3, [r1, #15]
	strb	r3, [r0, #15]
	ldr	r3, [r1, r2]
	str	r3, [r0, r2]
	adds	r0, r5, #0
	adds	r5, #1
	bl	sub_080ad3f8
	cmp	r5, r7
	blt.n	.L_080ae66e
	mov	r1, r8
	ldr	r2, [pc, #12]
	ldr	r3, [r1, #16]
	add	sp, #16
	str	r3, [r2, #16]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
.L_080ae6c8:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	sub_080aeec8
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080ae6e2
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080aed6c
.L_080ae6e2:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #0
	sub	sp, #16
	mov	r8, r2
.L_080ae6f2:
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r5, #128
	adds	r6, r3, #0
	lsls	r5, r5, #4
	movs	r7, #0
	adds	r6, #48
	add	r5, r8
.L_080ae704:
	adds	r0, r5, #0
	str	r2, [sp, #0]
	bl	sub_08016ce4
	ldr	r2, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080ae71a
	adds	r0, r6, #0
	bl	sub_08016cfc
	ldr	r2, [sp, #0]
.L_080ae71a:
	adds	r7, #1
	adds	r6, #1
	adds	r5, #1
	cmp	r7, #6
	ble.n	.L_080ae704
	movs	r1, #7
	adds	r2, #1
	add	r8, r1
	cmp	r2, #3
	ble.n	.L_080ae6f2
	movs	r0, #222
	bl	sub_080af298
	movs	r0, #0
	bl	sub_080afdd8
	movs	r0, #1
	bl	sub_080afdd8
	movs	r0, #2
	bl	sub_080afdd8
	movs	r0, #3
	bl	sub_080afdd8
	movs	r0, #0
	bl	sub_080ae358
	bl	sub_080b1004
	movs	r2, #0
.L_080ae758:
	adds	r0, r2, #0
	str	r2, [sp, #0]
	bl	sub_080ad3f8
	ldr	r2, [sp, #0]
	adds	r2, #1
	cmp	r2, #7
	ble.n	.L_080ae758
	movs	r0, #1
	bl	sub_080ae16c
	movs	r0, #34
	bl	sub_08016cfc
	add	r3, sp, #8
	mov	r8, r3
	movs	r1, #200
	ldr	r3, [pc, #172]
	lsls	r1, r1, #5
	adds	r1, #80
	adds	r1, r1, r3
	movs	r2, #0
	mov	sl, r1
.L_080ae786:
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r6, r3, #0
	movs	r1, #0
	movs	r7, #0
	adds	r6, #48
	mov	r5, r8
.L_080ae796:
	adds	r0, r6, #0
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	sub_08016ce4
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080ae7ae
	strb	r7, [r5, #0]
	adds	r1, #1
	adds	r5, #1
.L_080ae7ae:
	adds	r7, #1
	adds	r6, #1
	cmp	r7, #6
	ble.n	.L_080ae796
	cmp	r1, #0
	beq.n	.L_080ae7d4
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	sub_08014878
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #0]
	adds	r3, r1, #0
	muls	r3, r0
	mov	r1, r8
	lsrs	r3, r3, #16
	ldrsb	r3, [r1, r3]
	adds	r7, r3, #1
	b.n	.L_080ae7d6
.L_080ae7d4:
	movs	r7, #0
.L_080ae7d6:
	mov	r3, sl
	movs	r1, #1
	adds	r2, #1
	strb	r7, [r3, #0]
	add	sl, r1
	cmp	r2, #3
	ble.n	.L_080ae786
	movs	r0, #202
	movs	r1, #3
	bl	.L_080ae6c8
	movs	r0, #201
	movs	r1, #3
	bl	.L_080ae6c8
	movs	r0, #203
	movs	r1, #1
	bl	.L_080ae6c8
	movs	r0, #206
	movs	r1, #0
	bl	.L_080ae6c8
	movs	r0, #207
	movs	r1, #2
	bl	.L_080ae6c8
	ldr	r1, [pc, #28]
	movs	r3, #163
	lsls	r3, r3, #2
	adds	r2, r1, r3
	ldr	r2, [r2, #0]
	ldr	r3, [r1, #16]
	add	sp, #16
	adds	r3, r3, r2
	str	r3, [r1, #16]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02001000
	.4byte 0x02000240
