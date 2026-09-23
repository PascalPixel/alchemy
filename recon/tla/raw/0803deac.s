.syntax unified
	.thumb
	.set sub_0801401c, 0x0801401c
	.set sub_08014128, 0x08014128
	.set sub_080142d4, 0x080142d4
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08016ce4, 0x08016ce4
	.set sub_0803e8ec, 0x0803e8ec
	.set sub_081180b0, 0x081180b0
	.global Func_0803deac
	.thumb_func
Func_0803deac:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #72]
	cmp	r0, #0
	beq.n	.L_0803dede
	movs	r3, #239
	lsls	r3, r3, #1
	movs	r1, #0
	adds	r2, r4, r3
	movs	r0, #0
.L_0803dec2:
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_0803ded2
	movs	r3, #234
	adds	r0, r4, r0
	lsls	r3, r3, #1
	adds	r0, r0, r3
	b.n	.L_0803defe
.L_0803ded2:
	adds	r1, #1
	adds	r2, #52
	adds	r0, #52
	cmp	r1, #5
	bne.n	.L_0803dec2
	b.n	.L_0803defc
.L_0803dede:
	adds	r2, r4, #0
	adds	r0, r4, #0
	movs	r1, #0
	adds	r2, #104
	adds	r0, #114
.L_0803dee8:
	ldrh	r3, [r0, #0]
	adds	r0, #52
	cmp	r3, #0
	bne.n	.L_0803def4
	adds	r0, r2, #0
	b.n	.L_0803defe
.L_0803def4:
	adds	r1, #1
	adds	r2, #52
	cmp	r1, #7
	bne.n	.L_0803dee8
.L_0803defc:
	movs	r0, #0
.L_0803defe:
	pop	{pc}
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl	sub_080145a8
	pop	{pc}
	movs	r0, r0
	.2byte 0xdf25
	.2byte 0x0803
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_08014644
	pop	{pc}
	movs	r0, r0
	.2byte 0xdf25
	.2byte 0x0803
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #72]
	movs	r0, #192
	mov	sl, r3
	lsls	r0, r0, #2
	movs	r3, #0
	sub	sp, #24
	add	r0, sl
	str	r0, [sp, #8]
	mov	fp, r3
	movs	r3, #210
	lsls	r3, r3, #2
	add	r3, sl
	movs	r1, #182
	movs	r2, #195
	ldr	r7, [r3, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	add	r1, sl
	add	r2, sl
	mov	r8, r1
	mov	r9, r2
	cmp	r7, #0
	bne.n	.L_0803df66
	b.n	.L_0803e0ba
.L_0803df66:
	adds	r6, r7, #0
	adds	r6, #40
	ldrb	r3, [r6, #5]
	movs	r4, #4
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	strb	r3, [r6, #5]
	movs	r5, #63
	ldrb	r3, [r6, #7]
	negs	r5, r5
	adds	r2, r5, #0
	ands	r3, r2
	strb	r3, [r6, #7]
	ldrh	r1, [r7, #16]
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #44]
	ands	r1, r3
	ldrh	r3, [r6, #6]
	movs	r0, #240
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #6]
	ldrh	r3, [r7, #18]
	strb	r3, [r6, #4]
	adds	r1, r3, #0
	str	r0, [sp, #4]
	movs	r3, #232
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r2, [r3, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803dfb8
	adds	r3, r2, r1
	strb	r3, [r6, #4]
	b.n	.L_0803e058
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_0803dfb8:
	movs	r1, #16
	ldrsh	r2, [r7, r1]
	movs	r3, #24
	ldrsh	r1, [r7, r3]
	ldrh	r5, [r7, #16]
	ldrh	r4, [r7, #24]
	cmp	r2, r1
	beq.n	.L_0803e00c
	ldrh	r0, [r7, #20]
	mov	ip, r0
	movs	r0, #20
	ldrsh	r3, [r7, r0]
	cmp	r3, #0
	ble.n	.L_0803dfe0
	adds	r3, r2, r3
	cmp	r3, r1
	bgt.n	.L_0803dfe6
	mov	r1, ip
	adds	r3, r5, r1
	b.n	.L_0803dfee
.L_0803dfe0:
	adds	r3, r2, r3
	cmp	r3, r1
	bge.n	.L_0803dfea
.L_0803dfe6:
	strh	r4, [r7, #16]
	b.n	.L_0803dff0
.L_0803dfea:
	mov	r2, ip
	adds	r3, r5, r2
.L_0803dfee:
	strh	r3, [r7, #16]
.L_0803dff0:
	ldrh	r1, [r7, #16]
	ldr	r3, [pc, #16]
	ldr	r2, [pc, #16]
	ands	r1, r3
	ldrh	r3, [r6, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #6]
	b.n	.L_0803e058
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_0803e00c:
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #158
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	fp, r3
	bne.n	.L_0803e058
	movs	r3, #241
	str	r3, [sp, #4]
	mov	r4, r8
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	beq.n	.L_0803e058
	add	r5, sp, #12
	adds	r1, r5, #0
	ldrh	r0, [r7, #8]
	bl	sub_081180b0
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_0803e058
	ldr	r2, [r5, #0]
	mov	r3, r8
	strh	r2, [r3, #24]
	mov	r4, r8
	ldr	r1, [r5, #4]
	movs	r5, #34
	ldrsh	r3, [r4, r5]
	strh	r1, [r4, #26]
	cmp	r3, #0
	bne.n	.L_0803e058
	mov	r0, r8
	strh	r2, [r0, #16]
	movs	r3, #1
	mov	r2, r8
	strh	r1, [r2, #18]
	strh	r3, [r4, #34]
.L_0803e058:
	movs	r5, #34
	ldrsh	r3, [r7, r5]
	cmp	r3, #0
	beq.n	.L_0803e0ae
	movs	r0, #4
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0803e0a6
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #226
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0803e088
	ldrb	r3, [r6, #5]
	movs	r0, #13
	negs	r0, r0
	ands	r3, r0
	movs	r2, #4
	orrs	r3, r2
	b.n	.L_0803e090
.L_0803e088:
	ldrb	r3, [r6, #5]
	movs	r1, #13
	negs	r1, r1
	ands	r3, r1
.L_0803e090:
	strb	r3, [r6, #5]
	ldrh	r3, [r7, #10]
	cmp	r3, #1
	bne.n	.L_0803e0a6
	ldrb	r3, [r6, #5]
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #5]
.L_0803e0a6:
	adds	r0, r6, #0
	ldr	r1, [sp, #4]
	bl	sub_08014128
.L_0803e0ae:
	ldr	r7, [r7, #4]
	movs	r3, #1
	add	fp, r3
	cmp	r7, #0
	beq.n	.L_0803e0ba
	b.n	.L_0803df66
.L_0803e0ba:
	mov	r4, r9
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_0803e0c4
	b.n	.L_0803e1ea
.L_0803e0c4:
	mov	r0, sl
	bl	sub_0803e8ec
	mov	r5, r9
	adds	r5, #40
	ldrb	r3, [r5, #5]
	movs	r6, #13
	negs	r6, r6
	adds	r2, r6, #0
	ldrb	r1, [r5, #7]
	ands	r2, r3
	movs	r3, #4
	negs	r3, r3
	ands	r2, r3
	subs	r3, #59
	ands	r3, r1
	movs	r1, #17
	negs	r1, r1
	ands	r2, r1
	movs	r1, #32
	adds	r7, r0, #0
	orrs	r2, r1
	movs	r0, #63
	ands	r2, r0
	strb	r2, [r5, #5]
	ands	r3, r0
	ldrb	r2, [r5, #9]
	movs	r1, #128
	orrs	r3, r1
	strb	r3, [r5, #7]
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	mov	r0, r9
	ldrh	r3, [r0, #14]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #255
	ands	r2, r3
	ldrh	r1, [r5, #8]
	ldr	r3, [pc, #508]
	ldr	r0, [pc, #512]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldrh	r2, [r7, #16]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r1, #255
	subs	r2, #4
	ands	r2, r1
	ldr	r3, [pc, #496]
	ldrh	r1, [r5, #6]
	mov	fp, r3
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r3, [pc, #488]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldrb	r1, [r7, #18]
	lsrs	r3, r3, #1
	ands	r3, r2
	ldrb	r3, [r0, r3]
	mov	r4, r9
	lsls	r3, r3, #24
	asrs	r3, r3, #25
	adds	r1, r1, r3
	subs	r1, #4
	strb	r1, [r5, #4]
	movs	r0, #34
	ldrsh	r2, [r4, r0]
	movs	r0, #38
	ldrsh	r3, [r4, r0]
	ldrh	r1, [r4, #34]
	cmp	r2, r3
	beq.n	.L_0803e1ca
	movs	r0, #208
	lsls	r0, r0, #2
	add	r0, sl
	strh	r1, [r0, #0]
	movs	r3, #192
	ldrh	r2, [r4, #34]
	lsls	r3, r3, #2
	adds	r3, #66
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r2, #209
	lsls	r2, r2, #2
	add	r2, sl
	movs	r3, #0
	strh	r3, [r2, #0]
	bl	sub_0801401c
	ldrb	r2, [r5, #7]
	movs	r3, #31
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	lsls	r0, r0, #1
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r5, #7]
	ldrb	r3, [r5, #5]
	ldrh	r1, [r5, #6]
	movs	r2, #3
	orrs	r3, r2
	movs	r2, #255
	strb	r3, [r5, #5]
	lsls	r2, r2, #8
	lsls	r3, r1, #23
	movs	r4, #128
	adds	r2, #240
	lsrs	r3, r3, #23
	lsls	r4, r4, #1
	adds	r3, r3, r2
	adds	r4, #255
	mov	r2, fp
	ands	r3, r4
	ands	r2, r1
	orrs	r2, r3
	ldrb	r3, [r5, #4]
	strh	r2, [r5, #6]
	adds	r3, #240
	strb	r3, [r5, #4]
	mov	r0, r9
	ldrh	r3, [r0, #34]
	ldrh	r2, [r0, #36]
	mov	r1, r9
	adds	r3, r3, r2
	strh	r3, [r1, #34]
.L_0803e1ca:
	movs	r0, #4
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0803e1e2
	ldrb	r3, [r5, #5]
	adds	r2, r6, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r5, #5]
.L_0803e1e2:
	adds	r0, r5, #0
	movs	r1, #248
	bl	sub_08014128
.L_0803e1ea:
	mov	r0, sl
	movs	r1, #0
	bl	.L_0803e488
	mov	r0, sl
	movs	r1, #1
	bl	.L_0803e488
	movs	r3, #211
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r7, [r3, #0]
	cmp	r7, #0
	bne.n	.L_0803e208
	b.n	.L_0803e34e
.L_0803e208:
	movs	r2, #208
	lsls	r2, r2, #2
	movs	r3, #13
	add	r2, sl
	negs	r3, r3
	mov	r9, r2
	mov	fp, r3
.L_0803e216:
	movs	r4, #16
	ldrsh	r2, [r7, r4]
	movs	r5, #24
	ldrsh	r3, [r7, r5]
	adds	r6, r7, #0
	adds	r6, #40
	ldrh	r1, [r7, #16]
	cmp	r2, r3
	beq.n	.L_0803e22e
	ldrh	r3, [r7, #20]
	adds	r3, r1, r3
	strh	r3, [r7, #16]
.L_0803e22e:
	movs	r0, #18
	ldrsh	r2, [r7, r0]
	movs	r4, #26
	ldrsh	r3, [r7, r4]
	ldrh	r1, [r7, #18]
	cmp	r2, r3
	beq.n	.L_0803e242
	ldrh	r3, [r7, #22]
	adds	r3, r1, r3
	strh	r3, [r7, #18]
.L_0803e242:
	movs	r4, #128
	ldrh	r3, [r7, #16]
	lsls	r4, r4, #1
	ldr	r5, [pc, #208]
	ldrh	r1, [r6, #6]
	adds	r4, #255
	adds	r2, r4, #0
	ands	r2, r3
	adds	r3, r5, #0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, #6]
	ldrh	r3, [r7, #18]
	movs	r0, #0
	strb	r3, [r6, #4]
	ldrh	r1, [r7, #34]
	movs	r3, #34
	ldrsh	r2, [r7, r3]
	mov	lr, r2
	movs	r3, #38
	ldrsh	r2, [r7, r3]
	mov	ip, r2
	cmp	lr, ip
	beq.n	.L_0803e2d6
	ldrh	r3, [r7, #36]
	str	r4, [sp, #0]
	adds	r3, r1, r3
	mov	r1, r9
	strh	r3, [r7, #34]
	strh	r3, [r1, #0]
	movs	r3, #192
	ldrh	r2, [r7, #34]
	lsls	r3, r3, #2
	adds	r3, #66
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r3, #209
	lsls	r3, r3, #2
	add	r3, sl
	strh	r0, [r3, #0]
	mov	r0, r9
	bl	sub_0801401c
	movs	r3, #31
	ands	r0, r3
	movs	r1, #63
	ldrb	r3, [r6, #7]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	lsls	r0, r0, #1
	orrs	r3, r0
	strb	r3, [r6, #7]
	ldrb	r3, [r6, #5]
	ldrh	r1, [r6, #6]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r6, #5]
	movs	r3, #255
	lsls	r2, r1, #23
	lsls	r3, r3, #8
	ldr	r4, [sp, #0]
	adds	r3, #248
	lsrs	r2, r2, #23
	adds	r2, r2, r3
	adds	r3, r5, #0
	ands	r2, r4
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, #6]
	ldrb	r3, [r6, #4]
	adds	r3, #248
	strb	r3, [r6, #4]
	b.n	.L_0803e2ee
.L_0803e2d6:
	ldrb	r3, [r6, #5]
	movs	r4, #4
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	strb	r3, [r6, #5]
	movs	r5, #63
	ldrb	r3, [r6, #7]
	negs	r5, r5
	adds	r2, r5, #0
	ands	r3, r2
	strb	r3, [r6, #7]
.L_0803e2ee:
	movs	r0, #4
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0803e33e
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #226
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0803e324
	ldrb	r3, [r6, #5]
	mov	r0, fp
	ands	r3, r0
	movs	r2, #4
	orrs	r3, r2
	b.n	.L_0803e32a
	.4byte 0xfffffc00
	.4byte 0x0805ea0c
	.4byte 0xfffffe00
	.2byte 0x1100
	.2byte 0x0300
.L_0803e324:
	ldrb	r3, [r6, #5]
	mov	r1, fp
	ands	r3, r1
.L_0803e32a:
	strb	r3, [r6, #5]
	ldrh	r3, [r7, #10]
	cmp	r3, #1
	bne.n	.L_0803e33e
	ldrb	r3, [r6, #5]
	mov	r2, fp
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #5]
.L_0803e33e:
	adds	r0, r6, #0
	movs	r1, #240
	bl	sub_08014128
	ldr	r7, [r7, #4]
	cmp	r7, #0
	beq.n	.L_0803e34e
	b.n	.L_0803e216
.L_0803e34e:
	mov	r4, r8
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_0803e358
	b.n	.L_0803e46c
.L_0803e358:
	ldr	r3, [pc, #76]
	ldr	r5, [pc, #80]
	ldr	r2, [r3, #0]
	movs	r3, #15
	lsrs	r2, r2, #2
	ands	r2, r3
	mov	fp, r5
	lsls	r2, r2, #8
	movs	r1, #128
	add	r2, fp
	ldrh	r0, [r4, #12]
	lsls	r1, r1, #1
	bl	sub_080142d4
	ldr	r3, [pc, #44]
	ldr	r1, [sp, #8]
	ands	r0, r3
	ldrh	r2, [r1, #8]
	ldr	r3, [pc, #48]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	mov	r3, r8
	ldrh	r0, [r3, #24]
	movs	r4, #24
	ldrsh	r2, [r3, r4]
	ldrh	r1, [r3, #16]
	movs	r5, #16
	ldrsh	r3, [r3, r5]
	cmp	r2, r3
	beq.n	.L_0803e3c0
	subs	r3, r2, r3
	asrs	r3, r3, #1
	cmp	r3, #0
	beq.n	.L_0803e3bc
	b.n	.L_0803e3b4
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x03001100
	.4byte 0x0805c9c4
	.2byte 0xfc00
	.2byte 0xffff
.L_0803e3b4:
	adds	r3, r1, r3
	mov	r0, r8
	strh	r3, [r0, #16]
	b.n	.L_0803e3c0
.L_0803e3bc:
	mov	r1, r8
	strh	r0, [r1, #16]
.L_0803e3c0:
	mov	r2, r8
	mov	r5, r8
	ldrh	r1, [r2, #26]
	movs	r3, #26
	ldrsh	r2, [r2, r3]
	movs	r4, #18
	ldrsh	r3, [r5, r4]
	adds	r0, r3, #0
	cmp	r2, r3
	beq.n	.L_0803e3ec
	subs	r3, r2, r3
	asrs	r3, r3, #1
	cmp	r3, #0
	beq.n	.L_0803e3e6
	adds	r3, r0, r3
	mov	r0, r8
	strh	r3, [r0, #18]
	adds	r0, r3, #0
	b.n	.L_0803e3ec
.L_0803e3e6:
	mov	r2, r8
	strh	r1, [r2, #18]
	adds	r0, r1, #0
.L_0803e3ec:
	ldr	r3, [pc, #72]
	ldr	r1, [pc, #76]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #2
	ands	r3, r2
	ldrb	r3, [r1, r3]
	ldr	r4, [sp, #8]
	adds	r3, r3, r0
	subs	r3, #32
	strb	r3, [r4, #4]
	mov	r5, r8
	ldrh	r2, [r5, #16]
	ldr	r3, [pc, #44]
	subs	r2, #4
	ands	r2, r3
	ldrh	r1, [r4, #6]
	ldr	r3, [pc, #48]
	ldr	r0, [sp, #8]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #6]
	movs	r0, #4
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0803e464
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #226
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0803e456
	b.n	.L_0803e444
	.4byte 0x000001ff
	.4byte 0x03001100
	.4byte 0x0805ea0c
	.2byte 0xfe00
	.2byte 0xffff
.L_0803e444:
	ldr	r1, [sp, #8]
	movs	r2, #13
	ldrb	r3, [r1, #5]
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r1, #5]
	b.n	.L_0803e464
.L_0803e456:
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #8]
	ldrb	r2, [r3, #5]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r4, #5]
.L_0803e464:
	ldr	r0, [sp, #8]
	movs	r1, #248
	bl	sub_08014128
.L_0803e46c:
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #162
	add	r2, sl
	ldrh	r3, [r2, #0]
	add	sp, #24
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_0803e488:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	mov	r8, r1
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	lsrs	r4, r3, #2
	movs	r3, #7
	ands	r4, r3
	movs	r3, #52
	mov	r2, r8
	muls	r2, r3
	adds	r3, r2, r6
	ldrh	r3, [r3, #10]
	cmp	r3, #0
	bne.n	.L_0803e4ac
	b.n	.L_0803e5a0
.L_0803e4ac:
	movs	r0, #128
	adds	r3, r6, r2
	lsls	r0, r0, #1
	adds	r2, #16
	ldrh	r1, [r6, r2]
	adds	r0, #255
	adds	r5, r3, #0
	mov	ip, r0
	adds	r5, #40
	mov	r3, ip
	ldr	r7, [pc, #44]
	ands	r3, r1
	ldrh	r1, [r5, #6]
	adds	r0, r7, #0
	ands	r0, r1
	orrs	r0, r3
	strh	r0, [r5, #6]
	adds	r2, r6, r2
	ldrh	r3, [r2, #2]
	mov	r1, r8
	strb	r3, [r5, #4]
	cmp	r1, #0
	beq.n	.L_0803e4f8
	ldrh	r2, [r6, #60]
	ldr	r1, [pc, #20]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803e512
	lsls	r3, r0, #23
	lsrs	r3, r3, #23
	adds	r3, r3, r2
	b.n	.L_0803e508
	.4byte 0x03001100
	.4byte 0xfffffe00
	.2byte 0xc5c4
	.2byte 0x0805
.L_0803e4f8:
	ldrh	r2, [r6, #8]
	ldr	r1, [pc, #92]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803e512
	lsls	r3, r0, #23
	lsrs	r3, r3, #23
	subs	r3, r3, r2
.L_0803e508:
	mov	r2, ip
	ands	r3, r2
	ands	r0, r7
	orrs	r0, r3
	strh	r0, [r5, #6]
.L_0803e512:
	movs	r3, #52
	mov	r0, r8
	muls	r0, r3
	adds	r3, r0, #0
	adds	r3, #12
	lsls	r2, r4, #7
	adds	r2, r1, r2
	ldrh	r0, [r6, r3]
	movs	r1, #128
	bl	sub_080142d4
	ldr	r3, [pc, #40]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #44]
	ands	r3, r2
	orrs	r3, r0
	movs	r0, #4
	strh	r3, [r5, #8]
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0803e57a
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #226
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0803e570
	b.n	.L_0803e560
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x0805c1c4
	.2byte 0xfc00
	.2byte 0xffff
.L_0803e560:
	ldrb	r3, [r5, #5]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r5, #5]
	b.n	.L_0803e57a
.L_0803e570:
	ldrb	r2, [r5, #5]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r5, #5]
.L_0803e57a:
	movs	r1, #238
	adds	r0, r5, #0
	bl	sub_08014128
	movs	r3, #52
	mov	r2, r8
	muls	r2, r3
	adds	r3, r2, #0
	adds	r1, r3, #0
	adds	r1, #8
	ldrh	r2, [r6, r1]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803e5a0
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r2, r0
	strh	r3, [r6, r1]
.L_0803e5a0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
