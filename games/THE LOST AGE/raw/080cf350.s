.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080142d4, 0x080142d4
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c0, 0x080200c0
	.set sub_080200c8, 0x080200c8
	.set sub_08020148, 0x08020148
	.set sub_080201c0, 0x080201c0
	.set sub_08020218, 0x08020218
	.set sub_08020290, 0x08020290
	.set sub_08038248, 0x08038248
	.set sub_08038258, 0x08038258
	.set sub_080cad84, 0x080cad84
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080ceba8, 0x080ceba8
	.set sub_080cec24, 0x080cec24
	.set sub_080ceffc, 0x080ceffc
	.set sub_080dc10c, 0x080dc10c
	.global Overlay_080cf350
Overlay_080cf350:
.L_080cf350:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080cf3ac
	bl	sub_080cdf5c
	bl	sub_080cad84
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #52]
	movs	r3, #128
	lsls	r3, r3, #10
	adds	r2, r6, #0
	str	r3, [r6, #48]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r5, r0, #0
	ldr	r2, [r5, #12]
	movs	r3, #144
	lsls	r3, r3, #14
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	bl	sub_08020148
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_08020290
	movs	r0, #3
	bl	sub_08013560
	adds	r0, r5, #0
	movs	r1, #28
	bl	sub_08020090
	ldr	r1, [pc, #16]
	adds	r0, r6, #0
	bl	sub_080200a8
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
.L_080cf3ac:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0074
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	cmp	r5, #0
	beq.n	.L_080cf41a
	bl	sub_080cdf5c
	bl	sub_080cad84
	movs	r3, #1
	ands	r3, r6
	adds	r7, r0, #0
	cmp	r3, #0
	beq.n	.L_080cf3f0
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020218
	ldr	r1, [pc, #64]
	adds	r0, r5, #0
	bl	sub_080200a8
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #40]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #72]
	ldr	r3, [pc, #48]
	str	r3, [r5, #108]
.L_080cf3f0:
	cmp	r6, #3
	bne.n	.L_080cf3fa
	movs	r0, #60
	bl	sub_08013560
.L_080cf3fa:
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080cf408
	adds	r0, r5, #0
	bl	.L_080cf350
.L_080cf408:
	cmp	r6, #3
	bne.n	.L_080cf412
	movs	r0, #80
	bl	sub_08013560
.L_080cf412:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08020090
.L_080cf41a:
	pop	{r5, r6, r7, pc}
	.4byte 0x080effd8
	.2byte 0xf0d1
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r1
	mov	r9, r0
	bl	sub_080cdf5c
	bl	sub_080cad84
	movs	r1, #193
	adds	r7, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	bl	sub_08014cc0
	ldr	r2, [r7, #12]
	movs	r3, #144
	lsls	r3, r3, #14
	mov	r8, r0
	movs	r0, #234
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	bl	sub_080200c0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080cf4d0
	ldr	r5, [r6, #80]
	movs	r3, #0
	ldrb	r2, [r5, #5]
	strb	r3, [r5, #26]
	strb	r3, [r5, #27]
	subs	r3, #33
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #9]
	mov	r0, r9
	bl	sub_08038248
	movs	r2, #128
	lsls	r2, r2, #3
	add	r2, r8
	movs	r1, #128
	ldrb	r0, [r5, #16]
	bl	sub_080142d4
	movs	r0, #68
	bl	sub_0801314c
	movs	r3, #1
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cf4ac
	ldr	r3, [pc, #48]
	str	r3, [r6, #108]
.L_080cf4ac:
	movs	r3, #2
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cf4bc
	adds	r0, r6, #0
	bl	.L_080cf350
.L_080cf4bc:
	movs	r0, #80
	bl	sub_08013560
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08020090
	adds	r0, r6, #0
	bl	sub_080200c8
.L_080cf4d0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xf0d1
	.2byte 0x080c
	push	{r5, r6, lr}
	sub	sp, #12
	adds	r5, r0, #0
	bl	sub_08014878
	movs	r3, #100
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, #9
	bhi.n	.L_080cf54c
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r5, r5, #4
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r0, #209
	lsls	r0, r0, #1
	adds	r0, #255
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080cf54c
	ldr	r1, [pc, #32]
	bl	sub_080200a8
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_08020090
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_080cf54c:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0194
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	bl	sub_080ceba8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	mov	r8, r0
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #376]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [r1, #0]
	mov	fp, r0
	ldr	r3, [r3, #4]
	mov	r9, r2
	str	r3, [sp, #4]
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_080cf590
	b.n	.L_080cf6dc
.L_080cf590:
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	movs	r0, #0
	movs	r2, #2
	mov	sl, r0
	add	r8, r2
	cmp	r6, #0
	bne.n	.L_080cf5a2
	b.n	.L_080cf6dc
.L_080cf5a2:
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080cf5ae
	b.n	.L_080cf6dc
.L_080cf5ae:
	mov	r0, r8
	movs	r2, #6
	movs	r3, #0
	ldrsh	r5, [r0, r3]
	movs	r1, #2
	ldrsh	r7, [r0, r1]
	add	r8, r2
	cmp	r6, #128
	beq.n	.L_080cf5d2
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	beq.n	.L_080cf5d2
	adds	r0, r7, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080cf6b8
.L_080cf5d2:
	adds	r0, r5, #0
	add	r1, sp, #8
	bl	sub_080cec24
	cmp	r0, #0
	bne.n	.L_080cf6b8
	mov	r0, sl
	adds	r0, #64
	bl	sub_080ceffc
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080cf6b8
	ldr	r3, [r5, #76]
	ldr	r0, [pc, #260]
	cmp	r3, r0
	bne.n	.L_080cf61a
	adds	r3, r5, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r0, #0
	lsls	r4, r3, #16
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r1, r4, #0
	lsls	r5, r3, #16
	adds	r2, r5, #0
	str	r4, [sp, #0]
	bl	sub_080201c0
	adds	r6, r0, #0
	ldr	r4, [sp, #0]
	b.n	.L_080cf63e
.L_080cf61a:
	adds	r3, r6, #0
	subs	r3, #128
	movs	r0, #128
	lsls	r3, r3, #16
	lsls	r0, r0, #9
	cmp	r3, r0
	bls.n	.L_080cf6b8
	cmp	r6, #130
	beq.n	.L_080cf6b8
	cmp	r6, #132
	beq.n	.L_080cf6b8
	cmp	r6, #133
	beq.n	.L_080cf6b8
	cmp	r6, #131
	beq.n	.L_080cf6b8
	ldr	r4, [r5, #8]
	ldr	r6, [r5, #12]
	ldr	r5, [r5, #16]
.L_080cf63e:
	mov	r1, fp
	subs	r2, r4, r1
	mov	r0, r9
	subs	r3, r5, r0
	asrs	r2, r2, #16
	movs	r1, #135
	subs	r3, r3, r6
	adds	r2, #15
	lsls	r1, r1, #1
	asrs	r3, r3, #16
	cmp	r2, r1
	bhi.n	.L_080cf6b8
	adds	r3, #47
	cmp	r3, #238
	bhi.n	.L_080cf6b8
	movs	r2, #1
	negs	r2, r2
	cmp	r7, r2
	beq.n	.L_080cf6b8
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_08016ce4
	adds	r7, r0, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bne.n	.L_080cf6b8
	movs	r0, #209
	lsls	r0, r0, #1
	adds	r3, r5, #0
	adds	r0, #255
	adds	r1, r4, #0
	adds	r2, r6, #0
	bl	sub_080200c0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080cf6b8
	adds	r3, r5, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	ldr	r1, [pc, #100]
	bl	sub_080200a8
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020218
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020090
	ldr	r1, [r5, #80]
	movs	r0, #13
	ldrb	r2, [r1, #9]
	negs	r0, r0
	adds	r3, r0, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r1, #9]
.L_080cf6b8:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #15
	bgt.n	.L_080cf6dc
	mov	r0, r8
	movs	r3, #0
	ldrsh	r6, [r0, r3]
	movs	r1, #2
	add	r8, r1
	cmp	r6, #0
	beq.n	.L_080cf6dc
	ldr	r2, [pc, #24]
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080cf6dc
	b.n	.L_080cf5ae
.L_080cf6dc:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00004000
	.4byte 0xffff0000
	.4byte 0x31415927
	.2byte 0x01b8
	.2byte 0x080f
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #124]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #57
	adds	r3, r0, r2
	ldrb	r2, [r3, #0]
	movs	r4, #128
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	lsls	r4, r4, #19
	ldrh	r2, [r3, #10]
	ldr	r1, [pc, #48]
	ldrh	r2, [r4, #0]
	orrs	r2, r1
	strh	r2, [r4, #0]
	ldrh	r1, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r1, [r2, #0]
	adds	r0, #2
	ldrh	r1, [r0, #0]
	adds	r2, #2
	strh	r1, [r2, #0]
	adds	r0, #2
	ldrh	r2, [r0, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #64
	strh	r2, [r1, #0]
	adds	r0, #2
	ldrh	r4, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #66
	strh	r4, [r2, #0]
	b.n	.L_080cf774
	.2byte 0x6000
	.2byte 0x0000
.L_080cf774:
	movs	r4, #160
	adds	r2, #2
	strh	r4, [r2, #0]
	adds	r2, #2
	strh	r4, [r2, #0]
	adds	r0, #2
	ldr	r2, [pc, #4]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.2byte 0x0001
	.2byte 0xa660
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #32]
	ldr	r3, [r3, #124]
	sub	sp, #24
	movs	r0, #160
	str	r3, [sp, #20]
	lsls	r0, r0, #3
	adds	r0, #60
	adds	r4, r3, r0
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	cmp	r2, #0
	bne.n	.L_080cf7b8
	.2byte 0xe07f
.L_080cf7b8:
	movs	r5, #160
	lsls	r5, r5, #3
	adds	r5, #61
	adds	r1, r3, r5
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r0, [r1, #0]
	cmp	r3, r2
	.2byte 0xdb54
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r7, [sp, #20]
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #62
	adds	r3, r7, r0
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	cmp	r2, #0
	.2byte 0xd143
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #59
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #64
	bne.n	.L_080cf7fc
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #129
	b.n	.L_080cf804
.L_080cf7fc:
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #159
.L_080cf804:
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cf82a
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #74
	ldrh	r2, [r3, #0]
	ldr	r1, [pc, #52]
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_080cf82a:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #176
	ldrh	r1, [r2, #10]
	movs	r3, #197
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	movs	r3, #254
	ldrh	r1, [r2, #10]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	.2byte 0xf000
	.2byte 0xfc44
	movs	r0, r0
	movs	r7, r3
	movs	r0, r0
	.2byte 0xf6fd
	.2byte 0x080c
	.2byte 0xf78d
	.2byte 0x080c
.L_080cf868:
	ldr	r5, [sp, #20]
	movs	r7, #165
	lsls	r7, r7, #3
	adds	r3, r5, r7
	strh	r2, [r3, #0]
	b.n	.L_080cf8b8
.L_080cf874:
	ldr	r2, [sp, #20]
	movs	r5, #160
	lsls	r5, r5, #3
	adds	r5, #59
	adds	r3, r2, r5
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r7, [sp, #20]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #58
	adds	r5, r7, r3
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r2, r2, r3
	adds	r3, r0, #1
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r0, r3, #0
	muls	r0, r2
	movs	r1, #0
	ldrsb	r1, [r4, r1]
	ldr	r3, [pc, #448]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	ldrsb	r3, [r5, r3]
	movs	r5, #160
	lsls	r5, r5, #3
	adds	r5, #42
	adds	r3, r3, r0
	adds	r2, r7, r5
	strh	r3, [r2, #0]
.L_080cf8b8:
	ldr	r7, [sp, #20]
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #57
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r4, r7, r3
	adds	r0, r4, #4
	str	r4, [sp, #0]
	bl	sub_08038258
	movs	r1, #165
	lsls	r1, r1, #3
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	ldr	r4, [sp, #0]
	cmp	r3, #77
	bls.n	.L_080cf8ee
	bl	.L_080d00d0
.L_080cf8ee:
	ldr	r2, [pc, #376]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080cfa30
	.4byte 0x080cfaa0
	.4byte 0x080cfb72
	.4byte 0x080cfbe4
	.4byte 0x080cfc76
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080cfcde
	.4byte 0x080cfdd2
	.4byte 0x080cfef4
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.4byte 0x080d00d0
	.2byte 0xffec
	.2byte 0x080c
	ldr	r3, [pc, #40]
	movs	r5, #160
	strh	r3, [r4, #0]
	ldr	r3, [pc, #40]
	adds	r4, #2
	strh	r3, [r4, #0]
	ldr	r2, [sp, #20]
	lsls	r5, r5, #3
	adds	r5, #42
	adds	r3, r2, r5
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_080cfa6c
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_080cfa70
	movs	r0, r0
	.4byte 0x00007f7f
	.4byte 0x00000001
	.4byte 0x03000528
	.2byte 0xf8f8
	.2byte 0x080c
.L_080cfa6c:
	movs	r3, #31
	ands	r5, r3
.L_080cfa70:
	ldr	r3, [pc, #292]
	movs	r7, #0
	ldrb	r5, [r3, r5]
	movs	r3, #241
	mov	r8, r7
	subs	r6, r3, r5
.L_080cfa7c:
	str	r4, [sp, #0]
	bl	sub_08014878
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r4, [sp, #0]
	lsrs	r3, r3, #16
	movs	r0, #1
	lsls	r2, r3, #8
	add	r8, r0
	adds	r3, r3, r5
	orrs	r2, r3
	mov	r1, r8
	strh	r2, [r4, #0]
	adds	r4, #4
	cmp	r1, #159
	bls.n	.L_080cfa7c
	b.n	.L_080d00d0
	ldr	r2, [sp, #20]
	movs	r5, #160
	lsls	r5, r5, #3
	adds	r5, #42
	adds	r3, r2, r5
	ldrh	r5, [r3, #0]
	movs	r3, #31
	ands	r3, r5
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r3, [pc, #228]
	lsls	r2, r2, #2
	adds	r2, r2, r3
	movs	r3, #32
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080cfacc
	ldrh	r3, [r2, #0]
	strh	r3, [r4, #0]
	adds	r4, #2
	ldrh	r3, [r2, #2]
	b.n	.L_080cfad4
.L_080cfacc:
	ldrh	r3, [r2, #2]
	strh	r3, [r4, #0]
	adds	r4, #2
	ldrh	r3, [r2, #0]
.L_080cfad4:
	strh	r3, [r4, #0]
	adds	r4, #2
	movs	r7, #0
	adds	r2, #4
	mov	r8, r7
	mov	sl, r2
.L_080cfae0:
	mov	r2, sl
	ldrh	r7, [r2, #0]
	ldrh	r0, [r2, #2]
	cmp	r7, #0
	beq.n	.L_080cfb62
	cmp	r0, #0
	beq.n	.L_080cfb04
	movs	r1, #0
	mov	r9, r1
	cmp	r9, r7
	bge.n	.L_080cfb62
.L_080cfaf6:
	movs	r2, #1
	add	r9, r2
	strh	r0, [r4, #0]
	adds	r4, #4
	cmp	r9, r7
	blt.n	.L_080cfaf6
	b.n	.L_080cfb62
.L_080cfb04:
	ldrb	r3, [r2, #4]
	ldrb	r6, [r2, #6]
	mov	fp, r3
	ldrb	r0, [r2, #7]
	ldrb	r3, [r2, #5]
	cmp	r7, #0
	beq.n	.L_080cfb62
	mov	r5, fp
	subs	r5, r3, r5
	subs	r0, r0, r6
	str	r5, [sp, #16]
	str	r0, [sp, #12]
	movs	r2, #0
	movs	r3, #0
	mov	r9, r7
.L_080cfb22:
	adds	r0, r3, #0
	adds	r1, r7, #0
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_08002054
	ldr	r2, [sp, #8]
	adds	r5, r0, #0
	adds	r1, r7, #0
	adds	r0, r2, #0
	bl	sub_08002054
	add	r5, fp
	ldr	r4, [sp, #0]
	adds	r0, r6, r0
	lsls	r5, r5, #8
	adds	r5, r5, r0
	strh	r5, [r4, #0]
	ldr	r0, [sp, #12]
	ldr	r2, [sp, #8]
	movs	r5, #1
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #16]
	negs	r5, r5
	add	r9, r5
	adds	r2, r2, r0
	mov	r0, r9
	adds	r4, #4
	adds	r3, r3, r1
	cmp	r0, #0
	bne.n	.L_080cfb22
.L_080cfb62:
	movs	r2, #1
	add	r8, r2
	movs	r1, #8
	mov	r3, r8
	add	sl, r1
	cmp	r3, #2
	bls.n	.L_080cfae0
	b.n	.L_080d00d0
	ldr	r5, [sp, #20]
	movs	r7, #160
	lsls	r7, r7, #3
	adds	r7, #42
	adds	r3, r5, r7
	ldrh	r3, [r3, #0]
	subs	r5, r3, #1
	movs	r3, #32
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080cfba0
	ldr	r3, [pc, #4]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #4]
	b.n	.L_080cfba6
	.4byte 0x00000001
	.4byte 0x00007f7f
	.4byte 0x080f01c4
	.2byte 0x2e9c
	.2byte 0x080f
.L_080cfba0:
	ldr	r3, [pc, #40]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #40]
.L_080cfba6:
	adds	r4, #2
	strh	r3, [r4, #0]
	adds	r4, #2
	movs	r3, #31
	ands	r5, r3
	movs	r0, #0
	mov	r8, r0
	lsls	r5, r5, #4
.L_080cfbb6:
	str	r4, [sp, #0]
	bl	sub_08014878
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	adds	r0, r5, r0
	ldr	r4, [sp, #0]
	cmp	r0, #255
	bls.n	.L_080cfbd4
	movs	r0, #255
	b.n	.L_080cfbd4
	.4byte 0x00007f7f
	.2byte 0x0001
	.2byte 0x0000
.L_080cfbd4:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	strh	r0, [r4, #0]
	adds	r4, #4
	cmp	r2, #159
	bls.n	.L_080cfbb6
	b.n	.L_080d00d0
	ldr	r5, [sp, #20]
	movs	r7, #160
	lsls	r7, r7, #3
	adds	r7, #42
	adds	r3, r5, r7
	ldr	r2, [pc, #48]
	ldrh	r5, [r3, #0]
	ldr	r3, [pc, #48]
	strh	r2, [r4, #0]
	adds	r4, #2
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r5, #32
	bls.n	.L_080cfc0c
	strh	r3, [r4, #0]
	adds	r4, #2
	movs	r3, #64
	strh	r2, [r4, #0]
	subs	r5, r3, r5
	adds	r4, #2
.L_080cfc0c:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	ldr	r7, [pc, #16]
	lsls	r3, r3, #16
	movs	r0, #0
	mov	fp, r3
	mov	r8, r0
	b.n	.L_080cfc2c
	.4byte 0x00007f7f
	.4byte 0x00000001
	.2byte 0x02d4
	.2byte 0x0300
.L_080cfc2c:
	mov	r5, r8
	subs	r5, #80
	adds	r0, r5, #0
	muls	r0, r5
	mov	r1, fp
	lsls	r0, r0, #16
	str	r4, [sp, #0]
	subs	r0, r1, r0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2378
	asrs	r0, r0, #8
	subs	r6, r3, r0
	ldr	r4, [sp, #0]
	adds	r0, #120
	cmp	r6, #0
	bge.n	.L_080cfc50
	movs	r6, #0
.L_080cfc50:
	cmp	r0, #0
	bge.n	.L_080cfc56
	movs	r0, #0
.L_080cfc56:
	cmp	r6, #240
	ble.n	.L_080cfc5c
	movs	r6, #240
.L_080cfc5c:
	cmp	r0, #240
	ble.n	.L_080cfc62
	movs	r0, #240
.L_080cfc62:
	lsls	r3, r6, #8
	movs	r2, #1
	adds	r3, r3, r0
	add	r8, r2
	strh	r3, [r4, #0]
	mov	r3, r8
	adds	r4, #4
	cmp	r3, #159
	bls.n	.L_080cfc2c
	b.n	.L_080d00d0
	ldr	r5, [sp, #20]
	movs	r7, #160
	lsls	r7, r7, #3
	adds	r7, #42
	adds	r3, r5, r7
	ldrh	r5, [r3, #0]
	movs	r3, #32
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080cfc9c
	ldr	r3, [pc, #8]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #8]
	b.n	.L_080cfca2
	movs	r0, r0
	.4byte 0x00000001
	.2byte 0x7f7f
	.2byte 0x0000
.L_080cfc9c:
	ldr	r3, [pc, #36]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #36]
.L_080cfca2:
	adds	r4, #2
	strh	r3, [r4, #0]
	adds	r4, #2
	movs	r3, #31
	ands	r3, r5
	lsls	r2, r3, #4
	subs	r2, r2, r3
	lsls	r2, r2, #4
	lsrs	r5, r2, #5
	movs	r3, #240
	subs	r3, r3, r5
	movs	r0, #0
	lsls	r3, r3, #8
	mov	r8, r0
	adds	r3, #240
	b.n	.L_080cfccc
	movs	r0, r0
	.4byte 0x00007f7f
	.2byte 0x0001
	.2byte 0x0000
.L_080cfccc:
	movs	r1, #2
	add	r8, r1
	mov	r2, r8
	strh	r5, [r4, #0]
	strh	r3, [r4, #4]
	adds	r4, #8
	cmp	r2, #159
	bls.n	.L_080cfccc
	b.n	.L_080d00d0
	adds	r2, r6, #0
	adds	r2, #228
	ldr	r3, [pc, #760]
	ldr	r5, [r2, #0]
	ldr	r6, [r2, #4]
	ands	r5, r3
	ands	r6, r3
	str	r4, [sp, #0]
	bl	sub_080cdf5c
	bl	sub_080cad84
	ldr	r3, [r0, #8]
	ldr	r4, [sp, #0]
	subs	r3, r3, r5
	cmp	r3, #0
	bge.n	.L_080cfd08
	movs	r5, #255
	lsls	r5, r5, #8
	adds	r5, #255
	adds	r3, r3, r5
.L_080cfd08:
	ldr	r2, [r0, #12]
	asrs	r7, r3, #16
	ldr	r3, [r0, #16]
	subs	r3, r3, r2
	subs	r0, r3, r6
	cmp	r0, #0
	bge.n	.L_080cfd1e
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_080cfd1e:
	ldr	r2, [sp, #20]
	movs	r5, #160
	asrs	r3, r0, #16
	lsls	r5, r5, #3
	subs	r3, #16
	adds	r5, #54
	mov	sl, r3
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	movs	r0, #160
	strh	r3, [r4, #0]
	lsls	r0, r0, #3
	adds	r0, #52
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r4, #2
	movs	r1, #160
	strh	r3, [r4, #0]
	lsls	r1, r1, #3
	adds	r1, #42
	adds	r3, r2, r1
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	bne.n	.L_080cfd5e
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_080cfd62
.L_080cfd5e:
	movs	r3, #31
	ands	r5, r3
.L_080cfd62:
	ldr	r3, [pc, #636]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cfd70
	movs	r5, #0
.L_080cfd70:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r3, r3, #16
	mov	fp, r3
	ldr	r3, [pc, #612]
	movs	r2, #0
	mov	r8, r2
	mov	r9, r3
.L_080cfd84:
	mov	r0, r8
	mov	r1, sl
	subs	r5, r0, r1
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r0, r3, #1
	adds	r0, r0, r3
	mov	r2, fp
	lsls	r0, r0, #15
	str	r4, [sp, #0]
	subs	r0, r2, r0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x1200
	subs	r6, r7, r0
	ldr	r4, [sp, #0]
	adds	r0, r7, r0
	cmp	r6, #0
	bge.n	.L_080cfdac
	movs	r6, #0
.L_080cfdac:
	cmp	r0, #0
	bge.n	.L_080cfdb2
	movs	r0, #0
.L_080cfdb2:
	cmp	r6, #240
	ble.n	.L_080cfdb8
	movs	r6, #240
.L_080cfdb8:
	cmp	r0, #240
	ble.n	.L_080cfdbe
	movs	r0, #240
.L_080cfdbe:
	lsls	r3, r6, #8
	adds	r3, r3, r0
	strh	r3, [r4, #0]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	adds	r4, #4
	cmp	r5, #159
	bls.n	.L_080cfd84
	b.n	.L_080d00d0
	adds	r1, r6, #0
	adds	r1, #228
	ldr	r3, [pc, #516]
	ldr	r2, [r1, #0]
	ldr	r7, [sp, #20]
	ldr	r1, [r1, #4]
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #44
	ands	r2, r3
	ands	r1, r3
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_080cfdfa
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_080cfdfa:
	ldr	r5, [sp, #20]
	movs	r0, #166
	lsls	r0, r0, #3
	asrs	r7, r3, #16
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	subs	r1, r3, r1
	cmp	r1, #0
	bge.n	.L_080cfe14
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r1, r1, r2
.L_080cfe14:
	asrs	r3, r1, #16
	subs	r3, #16
	lsls	r2, r3, #1
	mov	sl, r3
	ldr	r3, [pc, #448]
	ldr	r5, [sp, #20]
	ldr	r3, [r3, #0]
	movs	r0, #160
	lsls	r0, r0, #3
	subs	r3, r3, r2
	adds	r0, #52
	mov	r9, r3
	adds	r3, r5, r0
	ldrh	r3, [r3, #0]
	movs	r1, #160
	strh	r3, [r4, #0]
	lsls	r1, r1, #3
	adds	r1, #54
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r4, #2
	movs	r2, #160
	strh	r3, [r4, #0]
	lsls	r2, r2, #3
	adds	r2, #42
	adds	r3, r5, r2
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_080cfe5e
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_080cfe62
.L_080cfe5e:
	movs	r3, #31
	ands	r5, r3
.L_080cfe62:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r3, r3, #16
	mov	fp, r3
	movs	r3, #0
	mov	r8, r3
.L_080cfe72:
	mov	r5, r8
	mov	r0, sl
	movs	r1, #120
	subs	r3, r5, r0
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_080cfe84
	movs	r3, #120
	negs	r3, r3
.L_080cfe84:
	cmp	r3, #120
	ble.n	.L_080cfe8a
	movs	r3, #120
.L_080cfe8a:
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #15
	mov	r3, fp
	subs	r0, r3, r0
	str	r4, [sp, #0]
	ldr	r3, [pc, #324]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1200
	subs	r6, r7, r0
	adds	r0, r7, r0
	ldr	r4, [sp, #0]
	cmp	r6, r0
	bge.n	.L_080cfec4
	ldr	r2, [pc, #312]
	movs	r3, #31
	mov	r5, r9
	ands	r3, r5
	ldrsb	r3, [r2, r3]
	subs	r6, r6, r3
	adds	r0, r0, r3
	cmp	r6, r0
	blt.n	.L_080cfec4
	movs	r6, #240
	movs	r0, #240
.L_080cfec4:
	cmp	r6, #0
	bge.n	.L_080cfeca
	movs	r6, #0
.L_080cfeca:
	cmp	r0, #0
	bge.n	.L_080cfed0
	movs	r0, #0
.L_080cfed0:
	cmp	r6, #240
	ble.n	.L_080cfed6
	movs	r6, #240
.L_080cfed6:
	cmp	r0, #240
	ble.n	.L_080cfedc
	movs	r0, #240
.L_080cfedc:
	movs	r1, #1
	lsls	r3, r6, #8
	add	r8, r1
	adds	r3, r3, r0
	mov	r2, r8
	movs	r0, #2
	strh	r3, [r4, #0]
	add	r9, r0
	adds	r4, #4
	cmp	r2, #159
	bls.n	.L_080cfe72
	b.n	.L_080d00d0
	adds	r2, r6, #0
	adds	r2, #228
	ldr	r3, [pc, #224]
	ldr	r5, [r2, #0]
	ldr	r6, [r2, #4]
	ands	r5, r3
	ands	r6, r3
	str	r4, [sp, #0]
	bl	sub_080cdf5c
	bl	sub_080cad84
	ldr	r3, [r0, #8]
	ldr	r4, [sp, #0]
	subs	r3, r3, r5
	cmp	r3, #0
	bge.n	.L_080cff1e
	movs	r5, #255
	lsls	r5, r5, #8
	adds	r5, #255
	adds	r3, r3, r5
.L_080cff1e:
	ldr	r2, [r0, #12]
	asrs	r7, r3, #16
	ldr	r3, [r0, #16]
	subs	r3, r3, r2
	subs	r0, r3, r6
	cmp	r0, #0
	bge.n	.L_080cff34
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_080cff34:
	ldr	r2, [sp, #20]
	movs	r5, #160
	asrs	r3, r0, #16
	lsls	r5, r5, #3
	subs	r3, #16
	adds	r5, #52
	mov	sl, r3
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	movs	r0, #160
	strh	r3, [r4, #0]
	lsls	r0, r0, #3
	adds	r0, #54
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r4, #2
	movs	r1, #160
	strh	r3, [r4, #0]
	lsls	r1, r1, #3
	adds	r1, #42
	adds	r3, r2, r1
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_080cff74
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_080cff78
.L_080cff74:
	movs	r3, #31
	ands	r5, r3
.L_080cff78:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r3, r3, #16
	mov	fp, r3
	ldr	r3, [pc, #92]
	movs	r2, #0
	mov	r8, r2
	mov	r9, r3
.L_080cff8c:
	mov	r0, r8
	mov	r1, sl
	subs	r5, r0, r1
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r0, r3, #1
	adds	r0, r0, r3
	mov	r2, fp
	lsls	r0, r0, #15
	str	r4, [sp, #0]
	subs	r0, r2, r0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x1200
	subs	r6, r7, r0
	ldr	r4, [sp, #0]
	adds	r0, r7, r0
	cmp	r6, #0
	bge.n	.L_080cffb4
	movs	r6, #0
.L_080cffb4:
	cmp	r0, #0
	bge.n	.L_080cffba
	movs	r0, #0
.L_080cffba:
	cmp	r6, #240
	ble.n	.L_080cffc0
	movs	r6, #240
.L_080cffc0:
	cmp	r0, #240
	ble.n	.L_080cffc6
	movs	r0, #240
.L_080cffc6:
	lsls	r3, r6, #8
	adds	r3, r3, r0
	strh	r3, [r4, #0]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	adds	r4, #4
	cmp	r5, #159
	bls.n	.L_080cff8c
	b.n	.L_080d00d0
	movs	r0, r0
	.4byte 0xffff0000
	.4byte 0x0300122c
	.4byte 0x030002d4
	.2byte 0x01e6
	.2byte 0x080f
	adds	r2, r6, #0
	adds	r2, #228
	ldr	r3, [pc, #252]
	ldr	r5, [r2, #0]
	ldr	r6, [r2, #4]
	ands	r5, r3
	ands	r6, r3
	str	r4, [sp, #0]
	bl	sub_080cdf5c
	bl	sub_080cad84
	ldr	r3, [r0, #8]
	ldr	r4, [sp, #0]
	subs	r3, r3, r5
	cmp	r3, #0
	bge.n	.L_080d0016
	movs	r7, #255
	lsls	r7, r7, #8
	adds	r7, #255
	adds	r3, r3, r7
.L_080d0016:
	ldr	r2, [r0, #12]
	asrs	r7, r3, #16
	ldr	r3, [r0, #16]
	subs	r3, r3, r2
	subs	r0, r3, r6
	cmp	r0, #0
	bge.n	.L_080d002c
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_080d002c:
	ldr	r2, [sp, #20]
	movs	r5, #160
	asrs	r3, r0, #16
	lsls	r5, r5, #3
	subs	r3, #8
	adds	r5, #52
	mov	sl, r3
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	movs	r0, #160
	strh	r3, [r4, #0]
	lsls	r0, r0, #3
	adds	r0, #54
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r4, #2
	movs	r1, #160
	strh	r3, [r4, #0]
	lsls	r1, r1, #3
	adds	r1, #42
	adds	r3, r2, r1
	ldrh	r5, [r3, #0]
	movs	r2, #32
	adds	r3, r5, #0
	ands	r3, r2
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_080d006c
	movs	r3, #31
	ands	r3, r5
	subs	r5, r2, r3
	b.n	.L_080d0070
.L_080d006c:
	movs	r3, #31
	ands	r5, r3
.L_080d0070:
	lsls	r3, r5, #2
	adds	r5, r3, r5
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r3, r3, #16
	mov	fp, r3
	ldr	r3, [pc, #116]
	movs	r2, #0
	mov	r8, r2
	mov	r9, r3
.L_080d0084:
	mov	r0, r8
	mov	r1, sl
	subs	r5, r0, r1
	adds	r3, r5, #0
	muls	r3, r5
	lsls	r0, r3, #1
	adds	r0, r0, r3
	mov	r2, fp
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	subs	r0, r2, r0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x1200
	subs	r6, r7, r0
	ldr	r4, [sp, #0]
	adds	r0, r7, r0
	cmp	r6, #0
	bge.n	.L_080d00ac
	movs	r6, #0
.L_080d00ac:
	cmp	r0, #0
	bge.n	.L_080d00b2
	movs	r0, #0
.L_080d00b2:
	cmp	r6, #240
	ble.n	.L_080d00b8
	movs	r6, #240
.L_080d00b8:
	cmp	r0, #240
	ble.n	.L_080d00be
	movs	r0, #240
.L_080d00be:
	lsls	r3, r6, #8
	adds	r3, r3, r0
	strh	r3, [r4, #0]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	adds	r4, #4
	cmp	r5, #159
	bls.n	.L_080d0084
.L_080d00d0:
	ldr	r7, [sp, #20]
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #57
	adds	r3, r7, r0
	ldrb	r2, [r3, #0]
	movs	r1, #1
	eors	r2, r1
	strb	r2, [r3, #0]
.L_080d00e2:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
