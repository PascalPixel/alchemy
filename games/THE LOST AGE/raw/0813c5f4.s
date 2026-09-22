.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_0813cb34, 0x0813cb34
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_0813c5f4
Overlay_0813c5f4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #88
	str	r0, [sp, #72]
	str	r1, [sp, #68]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	str	r0, [sp, #64]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #60]
	ldr	r2, [r5, #48]
	str	r2, [sp, #44]
	bl	sub_081435e0
	ldr	r4, [sp, #72]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0813c638
	movs	r1, #27
	movs	r0, #104
	bl	sub_081963ec
	ldr	r0, [r5, #104]
	movs	r1, #19
	str	r0, [sp, #48]
	movs	r0, #188
	b.n	.L_0813c648
.L_0813c638:
	movs	r1, #31
	movs	r0, #104
	bl	sub_081963ec
	ldr	r1, [r5, #104]
	movs	r0, #188
	str	r1, [sp, #48]
	movs	r1, #23
.L_0813c648:
	bl	sub_081963ec
	adds	r3, r5, #0
	adds	r3, #188
	ldr	r3, [r3, #0]
	str	r3, [sp, #52]
	ldr	r2, [sp, #64]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #56]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #68]
	cmp	r4, #4
	bhi.n	.L_0813c6b0
	ldr	r3, [pc, #44]
	lsls	r0, r4, #2
	ldr	r3, [r0, r3]
	mov	pc, r3
	.4byte 0x0813c688
	.4byte 0x0813c68c
	.4byte 0x0813c690
	.4byte 0x0813c694
	.2byte 0xc6b0
	.2byte 0x0813
	ldr	r0, [pc, #20]
	b.n	.L_0813c6b2
	ldr	r0, [pc, #20]
	b.n	.L_0813c6b2
	ldr	r0, [pc, #20]
	b.n	.L_0813c6b2
	ldr	r0, [pc, #20]
	b.n	.L_0813c6b2
	.4byte 0x0000011a
	.4byte 0x0813c674
	.4byte 0x00000178
	.4byte 0x00000163
	.4byte 0x0000018f
	.2byte 0x0147
	.2byte 0x0000
.L_0813c6b0:
	ldr	r0, [pc, #376]
.L_0813c6b2:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #372]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9910
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r1, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #336]
	bl	sub_080145a8
	ldr	r1, [sp, #72]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r2, [sp, #72]
	movs	r3, #3
	ldr	r1, [r2, #20]
	ldr	r0, [r0, #0]
	adds	r2, r1, #0
	muls	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, #48
	movs	r4, #0
	mov	sl, r0
	str	r3, [sp, #40]
	str	r4, [sp, #56]
	cmp	r1, #0
	beq.n	.L_0813c79e
	movs	r0, #36
	str	r0, [sp, #12]
	mov	r9, r4
.L_0813c714:
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #72]
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r1, [sp, #64]
	mov	r8, r0
	mov	r0, r9
	movs	r4, #0
	lsls	r3, r0, #2
	mov	fp, r4
	adds	r7, r3, r1
.L_0813c72e:
	mov	r2, sl
	ldr	r3, [r2, #8]
	movs	r4, #160
	str	r3, [r7, #0]
	lsls	r4, r4, #13
	ldr	r5, [r2, #12]
	mov	r1, r8
	adds	r5, r5, r4
	str	r5, [r7, #4]
	ldr	r6, [r2, #16]
	str	r6, [r7, #8]
	ldr	r0, [r1, #8]
	movs	r1, #24
	subs	r0, r0, r3
	bl	sub_08002054
	str	r0, [r7, #12]
	mov	r2, r8
	ldr	r0, [r2, #12]
	movs	r3, #160
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r1, #24
	subs	r0, r0, r5
	bl	sub_08002054
	str	r0, [r7, #16]
	mov	r4, r8
	ldr	r0, [r4, #16]
	movs	r1, #24
	subs	r0, r0, r6
	bl	sub_08002054
	str	r0, [r7, #20]
	movs	r0, #1
	add	fp, r0
	movs	r3, #0
	mov	r1, fp
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #3
	bne.n	.L_0813c72e
	movs	r2, #3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r4, [sp, #56]
	add	r9, r3
	ldr	r3, [sp, #12]
	adds	r4, #1
	adds	r3, #2
	str	r3, [sp, #12]
	str	r4, [sp, #56]
	ldr	r0, [sp, #72]
	ldr	r3, [r0, #20]
	cmp	r4, r3
	bne.n	.L_0813c714
.L_0813c79e:
	ldr	r2, [sp, #40]
	movs	r1, #0
	mov	sl, r1
	cmp	r2, #0
	bne.n	.L_0813c7aa
	b.n	.L_0813cadc
.L_0813c7aa:
	ldr	r3, [sp, #44]
	subs	r2, #16
	adds	r3, #12
	str	r2, [sp, #28]
	str	r3, [sp, #32]
.L_0813c7b4:
	ldr	r4, [sp, #64]
	movs	r1, #0
	movs	r0, #225
	str	r1, [sp, #56]
	lsls	r0, r0, #7
	movs	r7, #128
	mov	r2, sl
	adds	r6, r4, r0
	lsls	r7, r7, #12
	lsls	r5, r2, #12
.L_0813c7c8:
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r0, r0, #1
	subs	r0, r7, r0
	asrs	r0, r0, #10
	stmia	r6!, {r0}
	ldr	r4, [sp, #56]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r4, #1
	adds	r5, r5, r3
	str	r4, [sp, #56]
	cmp	r4, #160
	bne.n	.L_0813c7c8
	ldr	r0, [sp, #28]
	cmp	sl, r0
	ble.n	.L_0813c7fe
	ldr	r1, [sp, #40]
	mov	r4, sl
	subs	r2, r1, r4
	ldr	r1, [pc, #52]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_0813c7fe:
	bl	sub_08014de4
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #32]
	bl	sub_080156e8
	movs	r0, #0
	str	r0, [sp, #56]
	ldr	r1, [sp, #72]
	ldr	r3, [r1, #20]
	cmp	r3, #0
	bne.n	.L_0813c818
	b.n	.L_0813cab0
.L_0813c818:
	mov	r3, sl
	movs	r2, #36
	subs	r3, #30
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	str	r0, [sp, #16]
	str	r0, [sp, #8]
	b.n	.L_0813c838
	.4byte 0x00001000
	.4byte 0x00000166
	.4byte 0x03000730
	.2byte 0x3001
	.2byte 0x0814
.L_0813c838:
	ldr	r4, [sp, #16]
	cmp	sl, r4
	bne.n	.L_0813c86e
	ldr	r0, [sp, #68]
	cmp	r0, #1
	bne.n	.L_0813c84a
	movs	r0, #192
	bl	sub_081c0010
.L_0813c84a:
	ldr	r1, [sp, #68]
	cmp	r1, #2
	bne.n	.L_0813c856
	movs	r0, #189
	bl	sub_081c0010
.L_0813c856:
	ldr	r2, [sp, #68]
	cmp	r2, #3
	bne.n	.L_0813c862
	movs	r0, #148
	bl	sub_081c0010
.L_0813c862:
	ldr	r3, [sp, #68]
	cmp	r3, #4
	bne.n	.L_0813c86e
	movs	r0, #138
	bl	sub_081c0010
.L_0813c86e:
	ldr	r4, [sp, #16]
	cmp	sl, r4
	bge.n	.L_0813c876
	b.n	.L_0813ca88
.L_0813c876:
	ldr	r1, [sp, #68]
	movs	r0, #0
	lsls	r1, r1, #2
	str	r1, [sp, #36]
	mov	fp, r0
	mov	r9, r4
.L_0813c882:
	cmp	sl, r9
	blt.n	.L_0813c97c
	ldr	r3, [sp, #8]
	add	r6, sp, #76
	add	r3, fp
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r3, [sp, #64]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	mov	r0, r8
	adds	r1, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	mov	r4, r8
	asrs	r7, r3, #1
	str	r7, [r6, #0]
	ldr	r5, [r4, #24]
	cmp	r5, #0
	bge.n	.L_0813c8b0
	adds	r5, #7
.L_0813c8b0:
	asrs	r2, r5, #3
	cmp	r2, #5
	ble.n	.L_0813c8b8
	movs	r2, #5
.L_0813c8b8:
	ldr	r3, [pc, #580]
	ldr	r0, [sp, #36]
	ldrsb	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_0813c924
	mov	r1, sl
	lsls	r5, r2, #1
	lsrs	r0, r1, #31
	adds	r5, r5, r2
	add	r0, sl
	movs	r1, #3
	lsls	r5, r5, #3
	asrs	r0, r0, #1
	adds	r5, r5, r2
	bl	sub_08002064
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsls	r3, r2, #4
	subs	r3, r3, r2
	ldr	r2, [sp, #64]
	lsls	r3, r3, #6
	lsls	r5, r5, #5
	adds	r5, r5, r3
	movs	r3, #224
	adds	r5, r2, r5
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldr	r3, [r6, #4]
	movs	r4, #20
	movs	r0, #40
	adds	r2, r7, #0
	subs	r2, #10
	subs	r3, #40
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r5, #0
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6832
	movs	r0, #20
	movs	r1, #40
	ldr	r3, [r6, #4]
	subs	r2, #10
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #60]
	adds	r1, r5, #0
	ldr	r4, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe022
.L_0813c924:
	lsls	r5, r2, #1
	adds	r5, r5, r2
	ldr	r0, [sp, #64]
	lsls	r5, r5, #3
	adds	r5, r5, r2
	ldr	r3, [r6, #4]
	lsls	r5, r5, #5
	movs	r1, #178
	adds	r5, r0, r5
	lsls	r1, r1, #6
	adds	r5, r5, r1
	movs	r4, #20
	movs	r0, #40
	adds	r2, r7, #0
	subs	r2, #10
	subs	r3, #40
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r5, #0
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6832
	movs	r0, #20
	movs	r1, #40
	ldr	r3, [r6, #4]
	subs	r2, #10
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #60]
	adds	r1, r5, #0
	ldr	r4, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4640
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138058
	mov	r0, r8
	ldr	r3, [r0, #24]
	adds	r3, #1
	str	r3, [r0, #24]
.L_0813c97c:
	movs	r2, #1
	add	fp, r2
	movs	r1, #6
	mov	r3, fp
	add	r9, r1
	cmp	r3, #3
	beq.n	.L_0813c98c
	b.n	.L_0813c882
.L_0813c98c:
	ldr	r3, [sp, #36]
	ldr	r1, [pc, #368]
	adds	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_0813ca02
	ldr	r3, [sp, #16]
	adds	r3, #30
	cmp	sl, r3
	blt.n	.L_0813ca02
	ldr	r3, [sp, #16]
	adds	r3, #62
	cmp	sl, r3
	bge.n	.L_0813ca02
	ldr	r4, [sp, #24]
	ldr	r2, [sp, #72]
	ldrsh	r0, [r4, r2]
	bl	sub_08118098
	ldr	r4, [sp, #16]
	mov	r3, sl
	subs	r2, r3, r4
	ldr	r3, [sp, #20]
	ldr	r0, [r0, #0]
	ldr	r1, [pc, #324]
	cmp	r3, #0
	bge.n	.L_0813c9c6
	adds	r3, r2, #0
	subs	r3, #23
.L_0813c9c6:
	ldr	r2, [sp, #20]
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r3, r2, r3
	ldrsb	r3, [r1, r3]
	ldr	r2, [r0, #8]
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r0, #8]
	cmp	r2, #0
	ble.n	.L_0813c9e4
	movs	r4, #128
	lsls	r4, r4, #8
	adds	r3, r2, r4
	b.n	.L_0813c9e8
.L_0813c9e4:
	ldr	r1, [pc, #288]
	adds	r3, r2, r1
.L_0813c9e8:
	str	r3, [r0, #8]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #72]
	movs	r1, #1
	ldrsh	r0, [r2, r4]
	movs	r3, #0
	str	r3, [sp, #0]
	negs	r1, r1
	movs	r2, #5
	subs	r3, #1
	bl	sub_0814cd48
	ldr	r1, [pc, #252]
.L_0813ca02:
	ldr	r3, [sp, #36]
	adds	r3, #1
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_0813ca5a
	ldr	r3, [sp, #16]
	adds	r3, #24
	cmp	sl, r3
	bne.n	.L_0813ca3c
	movs	r0, #133
	bl	sub_081c0010
	ldr	r0, [sp, #56]
	cmp	r0, #0
	bne.n	.L_0813ca28
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
.L_0813ca28:
	ldr	r1, [sp, #24]
	ldr	r3, [sp, #72]
	ldrsh	r0, [r1, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #56]
	bl	sub_0814cd48
.L_0813ca3c:
	ldr	r3, [sp, #16]
	adds	r3, #40
	cmp	sl, r3
	bne.n	.L_0813ca58
	ldr	r2, [sp, #72]
	ldr	r4, [sp, #24]
	movs	r3, #8
	ldrsh	r0, [r4, r2]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #56]
	bl	sub_0814cd48
.L_0813ca58:
	ldr	r1, [pc, #164]
.L_0813ca5a:
	ldr	r3, [sp, #36]
	adds	r3, #2
	ldrsb	r1, [r1, r3]
	movs	r3, #1
	negs	r3, r3
	cmp	r1, r3
	beq.n	.L_0813ca88
	ldr	r3, [sp, #16]
	adds	r3, #24
	cmp	sl, r3
	bne.n	.L_0813ca88
	ldr	r4, [sp, #64]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r3, r4, r0
	movs	r2, #4
	str	r2, [r3, #0]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #72]
	ldrsh	r0, [r2, r4]
	bl	sub_08118088
.L_0813ca88:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #56]
	adds	r0, #2
	adds	r3, #3
	subs	r1, #32
	adds	r2, #32
	adds	r4, #1
	str	r0, [sp, #24]
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	str	r3, [sp, #8]
	str	r4, [sp, #56]
	ldr	r0, [sp, #72]
	ldr	r3, [r0, #20]
	cmp	r4, r3
	beq.n	.L_0813cab0
	b.n	.L_0813c838
.L_0813cab0:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r3, #240
	ldr	r1, [sp, #64]
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #40]
	movs	r4, #1
	add	sl, r4
	cmp	sl, r0
	beq.n	.L_0813cadc
	b.n	.L_0813c7b4
.L_0813cadc:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08197533
	.4byte 0x08197547
	.4byte 0xffff8000
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #1
	bl	sub_0813cb34
	pop	{pc}
