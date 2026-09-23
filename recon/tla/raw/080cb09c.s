.syntax unified
	.thumb
	.set sub_0801489c, 0x0801489c
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080ca164, 0x080ca164
	.set sub_080ca17c, 0x080ca17c
	.set sub_080cb6c8, 0x080cb6c8
	.set sub_080cb6f4, 0x080cb6f4
	.set sub_080cb788, 0x080cb788
	.set sub_080cb8e8, 0x080cb8e8
	.set sub_080ccd48, 0x080ccd48
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2260, 0x080d2260
	.set sub_080d2454, 0x080d2454
	.set sub_080d489c, 0x080d489c
	.set sub_081180f8, 0x081180f8
	.set sub_081c0010, 0x081c0010
	.global Func_080cb09c
	.thumb_func
Func_080cb09c:
	push	{r5, r6, r7, lr}
	movs	r5, #192
	lsls	r5, r5, #18
	sub	sp, #12
	ldr	r6, [r5, #108]
	bl	sub_080cb8e8
	adds	r2, r0, #0
	ldr	r7, [r5, #32]
	movs	r0, #0
	cmp	r2, #0
	beq.n	.L_080cb132
	ldr	r3, [r2, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r0, #128
	ldr	r3, [r2, #12]
	lsls	r0, r0, #13
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	ldrh	r1, [r2, #6]
	adds	r2, r5, #0
	bl	sub_0801489c
	movs	r0, #197
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cb108
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r2, #0
	bge.n	.L_080cb0ea
	ldr	r1, [pc, #80]
	adds	r2, r2, r1
.L_080cb0ea:
	asrs	r2, r2, #21
	movs	r1, #31
	ands	r2, r1
	cmp	r3, #0
	bge.n	.L_080cb0f8
	ldr	r0, [pc, #64]
	adds	r3, r3, r0
.L_080cb0f8:
	asrs	r3, r3, #21
	ands	r3, r1
	lsls	r3, r3, #5
	adds	r3, r2, r3
	ldr	r2, [pc, #56]
	lsls	r3, r3, #2
	adds	r1, r3, r2
	b.n	.L_080cb130
.L_080cb108:
	movs	r0, #156
	ldr	r2, [r5, #0]
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #8]
	cmp	r2, #0
	bge.n	.L_080cb11c
	ldr	r0, [pc, #36]
	adds	r2, r2, r0
.L_080cb11c:
	asrs	r2, r2, #20
	cmp	r3, #0
	bge.n	.L_080cb126
	ldr	r0, [pc, #28]
	adds	r3, r3, r0
.L_080cb126:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r1, r1, r3
.L_080cb130:
	ldrb	r0, [r1, #2]
.L_080cb132:
	add	sp, #12
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x001fffff
	.4byte 0x02020000
	.2byte 0xffff
	.2byte 0x000f
	push	{r5, r6, lr}
	movs	r5, #192
	lsls	r5, r5, #18
	sub	sp, #12
	ldr	r6, [r5, #108]
	bl	sub_080cb8e8
	ldr	r4, [r5, #32]
	cmp	r0, #0
	bne.n	.L_080cb15c
	movs	r0, #0
	b.n	.L_080cb1cc
.L_080cb15c:
	ldr	r1, [r0, #8]
	mov	r3, sp
	str	r1, [r3, #0]
	ldr	r2, [r0, #12]
	str	r2, [r3, #4]
	movs	r2, #197
	ldr	r0, [r0, #16]
	lsls	r2, r2, #1
	str	r0, [r3, #8]
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cb1a4
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_080cb184
	ldr	r4, [pc, #76]
	adds	r3, r1, r4
.L_080cb184:
	asrs	r2, r3, #21
	movs	r1, #31
	ands	r2, r1
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_080cb194
	ldr	r4, [pc, #60]
	adds	r3, r0, r4
.L_080cb194:
	asrs	r3, r3, #21
	ands	r3, r1
	lsls	r3, r3, #5
	ldr	r1, [pc, #56]
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r2, r3, r1
	b.n	.L_080cb1ca
.L_080cb1a4:
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r4, r2
	ldr	r2, [r3, #0]
	adds	r3, r1, #0
	cmp	r3, #0
	bge.n	.L_080cb1b6
	ldr	r4, [pc, #36]
	adds	r3, r3, r4
.L_080cb1b6:
	asrs	r1, r3, #20
	cmp	r0, #0
	bge.n	.L_080cb1c0
	ldr	r3, [pc, #24]
	adds	r0, r0, r3
.L_080cb1c0:
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
.L_080cb1ca:
	ldrb	r0, [r2, #2]
.L_080cb1cc:
	add	sp, #12
	pop	{r5, r6, pc}
	.4byte 0x001fffff
	.4byte 0x02020000
	.2byte 0xffff
	.2byte 0x000f
.L_080cb1dc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	ldr	r3, [pc, #192]
	ldr	r0, [r3, #44]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0x1c05
	cmp	r5, #0
	beq.n	.L_080cb2a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #1
	str	r3, [sp, #0]
	negs	r2, r2
	movs	r1, #0
	ldrsh	r6, [r5, r1]
	cmp	r6, r2
	beq.n	.L_080cb2a4
.L_080cb214:
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	mov	r8, r3
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	movs	r3, #12
	ldrsh	r0, [r5, r3]
	mov	fp, r2
	movs	r2, #6
	ldrsh	r1, [r5, r2]
	movs	r3, #8
	ldrsh	r7, [r5, r3]
	mov	sl, r1
	movs	r2, #10
	ldrsh	r1, [r5, r2]
	mov	r9, r1
	movs	r2, #14
	ldrsh	r1, [r5, r2]
	str	r1, [sp, #4]
	bl	sub_080ccd48
	cmp	r0, #0
	beq.n	.L_080cb28e
	ldr	r2, [sp, #12]
	mov	r1, r8
	lsls	r3, r1, #16
	cmp	r2, r3
	blt.n	.L_080cb28e
	lsls	r3, r7, #16
	cmp	r2, r3
	bge.n	.L_080cb28e
	ldr	r1, [sp, #16]
	lsls	r3, r6, #16
	cmp	r1, r3
	blt.n	.L_080cb28e
	mov	r2, sl
	lsls	r3, r2, #16
	cmp	r1, r3
	bge.n	.L_080cb28e
	ldr	r2, [sp, #8]
	mov	r1, fp
	lsls	r3, r1, #16
	cmp	r2, r3
	blt.n	.L_080cb28e
	mov	r1, r9
	lsls	r3, r1, #16
	cmp	r2, r3
	bge.n	.L_080cb28e
	ldr	r2, [sp, #0]
	movs	r1, #172
	lsls	r1, r1, #1
	adds	r3, r2, r1
	add	r2, sp, #4
	ldrh	r2, [r2, #0]
	movs	r0, #123
	strh	r2, [r3, #0]
	bl	sub_081c0010
	bl	sub_080d2260
	b.n	.L_080cb2a4
.L_080cb28e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r5, #16
	str	r3, [sp, #0]
	movs	r1, #1
	movs	r3, #0
	ldrsh	r6, [r5, r3]
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L_080cb214
.L_080cb2a4:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x8000
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r2, [sp, #24]
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #18
	mov	fp, r0
	ldr	r0, [r3, #108]
	ldr	r3, [r3, #32]
	mov	r8, r1
	mov	r9, r0
	str	r3, [sp, #20]
	bl	sub_080cdf5c
	str	r0, [sp, #16]
	bl	sub_080cb8e8
	movs	r1, #0
	mov	r2, r9
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	cmp	r2, #0
	bne.n	.L_080cb2f4
	b.n	.L_080cb6b8
.L_080cb2f4:
	bl	sub_080ad0f0
	movs	r6, #0
	str	r0, [sp, #4]
	cmp	r6, r0
	bcs.n	.L_080cb320
	ldr	r3, [pc, #512]
	movs	r4, #134
	lsls	r4, r4, #2
	add	r5, sp, #28
	adds	r7, r3, r4
.L_080cb30a:
	ldrb	r0, [r7, #0]
	bl	sub_08016ca4
	ldrh	r3, [r0, #56]
	adds	r6, #1
	strh	r3, [r5, #0]
	ldr	r0, [sp, #4]
	adds	r7, #1
	adds	r5, #2
	cmp	r6, r0
	bcc.n	.L_080cb30a
.L_080cb320:
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cb35e
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080cb33a
	ldr	r3, [pc, #464]
	add	r3, r8
.L_080cb33a:
	asrs	r2, r3, #21
	movs	r1, #31
	mov	r3, sl
	ands	r2, r1
	cmp	r3, #0
	bge.n	.L_080cb34a
	ldr	r3, [pc, #448]
	add	r3, sl
.L_080cb34a:
	asrs	r3, r3, #21
	ands	r3, r1
	lsls	r3, r3, #5
	ldr	r1, [pc, #440]
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r0, r3, r1
	ldrb	r5, [r0, #2]
	movs	r7, #0
	b.n	.L_080cb39c
.L_080cb35e:
	mov	r2, fp
	cmp	r2, #2
	bhi.n	.L_080cb376
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r4, #156
	ldr	r1, [sp, #20]
	lsls	r3, r3, #3
	lsls	r4, r4, #1
	adds	r3, r3, r4
	ldr	r0, [r1, r3]
	b.n	.L_080cb378
.L_080cb376:
	ldr	r0, [pc, #408]
.L_080cb378:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080cb382
	ldr	r3, [pc, #404]
	add	r3, r8
.L_080cb382:
	asrs	r2, r3, #20
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080cb38e
	ldr	r3, [pc, #392]
	add	r3, sl
.L_080cb38e:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r0, r0, r3
	ldrb	r5, [r0, #2]
	ldrb	r7, [r0, #3]
.L_080cb39c:
	movs	r2, #210
	lsls	r2, r2, #1
	add	r2, r9
	ldr	r3, [r2, #0]
	movs	r1, #212
	lsls	r1, r1, #1
	add	r1, r9
	str	r3, [r1, #0]
	str	r0, [r2, #0]
	cmp	r7, #0
	beq.n	.L_080cb3bc
	mov	r0, r8
	ldr	r1, [sp, #24]
	mov	r2, sl
	bl	.L_080cb1dc
.L_080cb3bc:
	subs	r3, r5, #1
	cmp	r3, #229
	bhi.n	.L_080cb3ca
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	strh	r5, [r3, #0]
.L_080cb3ca:
	movs	r3, #226
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cb3da
	movs	r3, #171
	lsls	r3, r3, #1
	add	r3, r9
	strh	r7, [r3, #0]
.L_080cb3da:
	ldr	r4, [pc, #296]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bls.n	.L_080cb3f4
	cmp	r3, #6
	beq.n	.L_080cb3f4
	cmp	r3, #7
	beq.n	.L_080cb3f4
	b.n	.L_080cb57a
.L_080cb3f4:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne.n	.L_080cb3fc
	b.n	.L_080cb57a
.L_080cb3fc:
	ldr	r3, [r3, #56]
	movs	r0, #128
	lsls	r0, r0, #24
	cmp	r3, r0
	bne.n	.L_080cb408
	b.n	.L_080cb57a
.L_080cb408:
	ldr	r1, [sp, #12]
	movs	r0, #104
	adds	r0, #255
	movs	r6, #0
	ldr	r5, [r1, #48]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cb41c
	lsls	r5, r5, #1
.L_080cb41c:
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cb438
	ldr	r0, [sp, #12]
	adds	r1, r5, #0
	adds	r0, #8
	bl	sub_080ca17c
	b.n	.L_080cb44a
.L_080cb438:
	movs	r3, #4
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cb442
	movs	r6, #1
.L_080cb442:
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080ca164
.L_080cb44a:
	movs	r3, #178
	lsls	r3, r3, #1
	add	r3, r9
	strh	r0, [r3, #0]
	movs	r3, #206
	ldr	r2, [sp, #12]
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	ldr	r1, [r2, #48]
	ldr	r3, [pc, #184]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2e00
	bne.n	.L_080cb46e
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r0, r3, #1
.L_080cb46e:
	movs	r2, #208
	lsls	r2, r2, #1
	add	r2, r9
	ldr	r3, [r2, #0]
	adds	r0, r3, r0
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	str	r0, [r2, #0]
	cmp	r0, r3
	ble.n	.L_080cb4b2
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L_080cb48c
	adds	r1, r0, r3
.L_080cb48c:
	ands	r0, r3
	asrs	r1, r1, #16
	str	r0, [r2, #0]
	adds	r0, r1, #0
	movs	r1, #0
	bl	sub_080cb6c8
	bl	sub_081180f8
	cmp	r0, #0
	beq.n	.L_080cb4a8
	movs	r0, #139
	bl	sub_081c0010
.L_080cb4a8:
	bl	sub_080d2454
	bl	sub_080cb788
	str	r0, [sp, #8]
.L_080cb4b2:
	ldr	r4, [pc, #80]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #78
	adds	r3, r4, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_080cb538
	movs	r1, #8
	adds	r3, r1, #0
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cb538
	movs	r3, #212
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	ldrb	r2, [r3, #3]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cb51c
	movs	r2, #128
	ldr	r3, [sp, #12]
	lsls	r2, r2, #2
	adds	r2, #82
	adds	r1, r4, r2
	ldr	r2, [r3, #48]
	cmp	r2, #0
	bge.n	.L_080cb4f8
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r2, r2, r0
.L_080cb4f8:
	ldrh	r3, [r1, #0]
	asrs	r2, r2, #16
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	b.n	.L_080cb538
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x02010000
	.4byte 0x000fffff
	.2byte 0x021c
	.2byte 0x0300
.L_080cb51c:
	movs	r1, #147
	lsls	r1, r1, #2
	adds	r3, r4, r1
	ldrh	r3, [r3, #0]
	movs	r0, #128
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsls	r0, r0, #2
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	adds	r0, #82
	asrs	r2, r2, #1
	adds	r3, r4, r0
	strh	r2, [r3, #0]
.L_080cb538:
	movs	r2, #153
	lsls	r2, r2, #2
	adds	r1, r4, r2
	ldr	r2, [r1, #0]
	cmp	r2, #0
	beq.n	.L_080cb57a
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #94
	adds	r3, r4, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #2
	beq.n	.L_080cb57a
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #48]
	subs	r3, r2, r3
	str	r3, [r1, #0]
	cmp	r3, #0
	bgt.n	.L_080cb57a
	movs	r3, #1
	str	r3, [r1, #0]
	movs	r2, #179
	lsls	r2, r2, #1
	add	r2, r9
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L_080cb57a
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r3, #150
	strh	r3, [r2, #0]
.L_080cb57a:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #78
	adds	r3, r4, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #1
	bne.n	.L_080cb5da
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #82
	adds	r5, r4, r1
	ldrh	r3, [r5, #0]
	subs	r2, #2
	adds	r3, #1
	strh	r3, [r5, #0]
	adds	r6, r4, r2
	ldrh	r0, [r6, #0]
	lsls	r3, r3, #16
	lsls	r2, r0, #16
	asrs	r1, r2, #16
	lsrs	r2, r2, #31
	adds	r1, r1, r2
	asrs	r3, r3, #16
	asrs	r1, r1, #1
	cmp	r3, r1
	bne.n	.L_080cb5c0
	movs	r1, #2
	ldr	r0, [sp, #16]
	adds	r1, #255
	str	r4, [sp, #0]
	bl	sub_080d489c
	ldrh	r0, [r6, #0]
	ldr	r4, [sp, #0]
.L_080cb5c0:
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	lsls	r3, r0, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	bne.n	.L_080cb5da
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r0, [sp, #16]
	str	r4, [sp, #0]
	bl	sub_080d489c
	ldr	r4, [sp, #0]
.L_080cb5da:
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #147
	adds	r1, #82
	lsls	r2, r2, #2
	adds	r0, r4, r1
	adds	r3, r4, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r1, #0
	ldrsh	r2, [r0, r1]
	cmp	r2, r3
	blt.n	.L_080cb61c
	movs	r2, #148
	lsls	r2, r2, #2
	adds	r3, r4, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #0
	strh	r3, [r0, #0]
	movs	r0, #255
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r0, r1
	negs	r0, r0
	ands	r1, r3
	str	r4, [sp, #0]
	bl	sub_080cb6f4
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #0]
	adds	r3, #1
	str	r3, [sp, #8]
.L_080cb61c:
	ldr	r0, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080cb6b8
	movs	r3, #182
	lsls	r3, r3, #1
	movs	r2, #0
	add	r3, r9
	strh	r2, [r3, #0]
	movs	r3, #183
	lsls	r3, r3, #1
	add	r3, r9
	strh	r2, [r3, #0]
	movs	r1, #129
	lsls	r1, r1, #1
	ldr	r0, [sp, #16]
	str	r4, [sp, #0]
	bl	sub_080d489c
	ldr	r1, [sp, #4]
	movs	r6, #0
	ldr	r4, [sp, #0]
	cmp	r6, r1
	bcs.n	.L_080cb6b8
	movs	r2, #181
	lsls	r2, r2, #1
	movs	r7, #183
	movs	r3, #134
	add	r2, r9
	lsls	r7, r7, #1
	lsls	r3, r3, #2
	mov	r8, r2
	add	r7, r9
	adds	r5, r4, r3
.L_080cb65e:
	ldrb	r0, [r5, #0]
	bl	sub_08016ca4
	movs	r4, #56
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	ble.n	.L_080cb674
	ldrh	r3, [r7, #0]
	adds	r3, #1
	strh	r3, [r7, #0]
	b.n	.L_080cb6ae
.L_080cb674:
	add	r3, sp, #28
	lsls	r2, r6, #1
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080cb6ae
	movs	r1, #182
	lsls	r1, r1, #1
	add	r1, r9
	ldrh	r3, [r1, #0]
	mov	r4, r9
	adds	r2, r3, #1
	strh	r2, [r1, #0]
	lsls	r3, r3, #16
	movs	r2, #184
	lsls	r2, r2, #1
	asrs	r3, r3, #15
	adds	r3, r3, r2
	ldrb	r2, [r5, #0]
	mov	r1, r8
	strh	r2, [r4, r3]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r1, #0]
	movs	r3, #50
	adds	r3, #255
	adds	r2, r0, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_080cb6ae:
	ldr	r4, [sp, #4]
	adds	r6, #1
	adds	r5, #1
	cmp	r6, r4
	bcc.n	.L_080cb65e
.L_080cb6b8:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
