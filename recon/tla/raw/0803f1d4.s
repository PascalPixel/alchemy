.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080143ac, 0x080143ac
	.set sub_0803d4e4, 0x0803d4e4
	.set sub_0803d5c4, 0x0803d5c4
	.set sub_0803d8ac, 0x0803d8ac
	.set sub_0803d8f0, 0x0803d8f0
	.set sub_0803d98c, 0x0803d98c
	.set sub_0803deac, 0x0803deac
	.set sub_0803ef48, 0x0803ef48
	.set sub_0803efd4, 0x0803efd4
	.set sub_0803f758, 0x0803f758
	.global Func_0803f1d4
	.thumb_func
Func_0803f1d4:
	push	{r5, r6, lr}
	movs	r6, #1
	sub	sp, #12
	negs	r6, r6
	adds	r5, r1, #0
	str	r6, [sp, #8]
	cmp	r0, #2
	beq.n	.L_0803f206
	cmp	r0, #2
	bhi.n	.L_0803f1ee
	cmp	r0, #1
	beq.n	.L_0803f1f6
	b.n	.L_0803f22e
.L_0803f1ee:
	cmp	r0, #4
	beq.n	.L_0803f21e
	cmp	r0, #6
	bne.n	.L_0803f22e
.L_0803f1f6:
	movs	r1, #0
	add	r2, sp, #8
	add	r3, sp, #4
	adds	r0, r5, #0
	str	r1, [sp, #0]
	bl	sub_0803d4e4
	b.n	.L_0803f22e
.L_0803f206:
	bl	sub_080143ac
	adds	r2, r0, #0
	str	r2, [sp, #8]
	adds	r0, r6, #0
	cmp	r2, #96
	beq.n	.L_0803f230
	adds	r0, r5, #0
	movs	r1, #26
	bl	sub_0803d8ac
	b.n	.L_0803f22e
.L_0803f21e:
	movs	r1, #0
	str	r1, [sp, #0]
	add	r2, sp, #8
	add	r3, sp, #4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_0803d98c
.L_0803f22e:
	ldr	r0, [sp, #8]
.L_0803f230:
	add	sp, #12
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r3
	movs	r3, #1
	sub	sp, #12
	adds	r4, r2, #0
	negs	r3, r3
	adds	r7, r0, #0
	adds	r5, r1, #0
	str	r2, [sp, #8]
	adds	r6, r4, #0
	cmp	r4, r3
	bne.n	.L_0803f25e
	bl	sub_080143ac
	adds	r4, r0, #0
	str	r0, [sp, #8]
	adds	r0, r6, #0
	cmp	r4, #96
	beq.n	.L_0803f2d6
.L_0803f25e:
	subs	r0, r7, #1
	cmp	r0, #7
	bhi.n	.L_0803f2d4
	ldr	r2, [pc, #120]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0803f28c
	.4byte 0x0803f29e
	.4byte 0x0803f2d4
	.4byte 0x0803f2b6
	.4byte 0x0803f2d4
	.4byte 0x0803f28c
	.4byte 0x0803f2aa
	.2byte 0xf2c8
	.2byte 0x0803
	movs	r1, #1
	str	r1, [sp, #0]
	add	r2, sp, #8
	add	r3, sp, #4
	adds	r0, r5, #0
	mov	r1, r8
	bl	sub_0803d4e4
	b.n	.L_0803f2d2
	adds	r2, r4, #0
	adds	r0, r5, #0
	movs	r1, #58
	bl	sub_0803d8ac
	b.n	.L_0803f2d2
	adds	r2, r4, #0
	adds	r0, r5, #0
	movs	r1, #42
	bl	sub_0803d8ac
	b.n	.L_0803f2d2
	movs	r1, #1
	str	r1, [sp, #0]
	add	r2, sp, #8
	add	r3, sp, #4
	adds	r0, r5, #0
	mov	r1, r8
	bl	sub_0803d98c
	b.n	.L_0803f2d2
	adds	r2, r4, #0
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0803d8f0
.L_0803f2d2:
	ldr	r4, [sp, #8]
.L_0803f2d4:
	adds	r0, r4, #0
.L_0803f2d6:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xf26c
	.2byte 0x0803
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r6, r1, #0
	sub	sp, #12
	adds	r5, r2, #0
	adds	r1, r3, #0
	cmp	r7, #2
	beq.n	.L_0803f320
	cmp	r7, #2
	bhi.n	.L_0803f2fe
	cmp	r7, #1
	beq.n	.L_0803f306
	b.n	.L_0803f356
.L_0803f2fe:
	cmp	r7, #4
	beq.n	.L_0803f33a
	cmp	r7, #6
	bne.n	.L_0803f356
.L_0803f306:
	cmp	r1, #0
	beq.n	.L_0803f30e
	ldrh	r3, [r5, #12]
	str	r3, [sp, #8]
.L_0803f30e:
	add	r3, sp, #4
	str	r1, [sp, #0]
	add	r2, sp, #8
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_0803d4e4
	ldr	r3, [pc, #152]
	b.n	.L_0803f352
.L_0803f320:
	cmp	r1, #0
	beq.n	.L_0803f328
	ldrh	r3, [r5, #12]
	str	r3, [sp, #8]
.L_0803f328:
	add	r3, sp, #4
	str	r1, [sp, #0]
	add	r2, sp, #8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_0803d5c4
	ldr	r3, [pc, #132]
	b.n	.L_0803f352
.L_0803f33a:
	cmp	r1, #0
	beq.n	.L_0803f342
	ldrh	r3, [r5, #12]
	str	r3, [sp, #8]
.L_0803f342:
	add	r3, sp, #4
	str	r1, [sp, #0]
	add	r2, sp, #8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_0803d98c
	ldr	r3, [pc, #108]
.L_0803f352:
	adds	r3, r6, r3
	strh	r3, [r5, #32]
.L_0803f356:
	ldr	r3, [sp, #8]
	strh	r6, [r5, #8]
	ldr	r6, [sp, #4]
	strh	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r6, [r5, #14]
	strh	r7, [r5, #10]
	strh	r3, [r5, #34]
	strh	r3, [r5, #38]
	adds	r0, r5, #0
	adds	r0, #40
	ldrb	r3, [r0, #5]
	movs	r5, #13
	negs	r5, r5
	adds	r2, r5, #0
	ands	r2, r3
	movs	r3, #33
	negs	r3, r3
	ldrb	r1, [r0, #7]
	ands	r2, r3
	adds	r3, #16
	movs	r4, #63
	ands	r2, r3
	ands	r2, r4
	adds	r3, r4, #0
	ands	r3, r1
	strb	r2, [r0, #5]
	movs	r1, #64
	ldrb	r2, [r0, #9]
	orrs	r3, r1
	strb	r3, [r0, #7]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r0, #9]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ldrh	r2, [r0, #8]
	ands	r6, r3
	ldr	r3, [pc, #28]
	add	sp, #12
	ands	r3, r2
	orrs	r3, r6
	strh	r3, [r0, #8]
	ldrb	r3, [r0, #9]
	ands	r5, r3
	strb	r5, [r0, #9]
	pop	{r5, r6, r7, pc}
	.4byte 0x0000003a
	.4byte 0x0000025f
	.4byte 0x000005a7
	.2byte 0xfc00
	.2byte 0xffff
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
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #158
	adds	r3, r3, r5
	mov	sl, r3
	ldrh	r3, [r3, #0]
	movs	r0, #0
	adds	r2, r2, r3
	mov	r8, r0
	adds	r0, r5, #0
	mov	r9, r2
	bl	sub_0803efd4
	mov	r0, sl
	ldrh	r1, [r0, #0]
	adds	r0, r5, #0
	bl	sub_0803ef48
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #162
	adds	r2, r5, r1
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #128
	lsls	r2, r2, #2
	movs	r0, #128
	adds	r2, #226
	lsls	r0, r0, #2
	movs	r7, #0
	adds	r3, r5, r2
	adds	r0, #250
	strh	r7, [r5, #10]
	strh	r7, [r5, #62]
	strh	r7, [r3, #0]
	adds	r3, r5, r0
	strh	r7, [r3, #0]
	bl	sub_0803f758
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r6, [r3, #0]
	cmp	r6, #0
	beq.n	.L_0803f454
	mov	r2, sl
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0803f454
.L_0803f444:
	ldr	r6, [r6, #4]
	movs	r3, #1
	add	r8, r3
	cmp	r6, #0
	beq.n	.L_0803f454
	ldrh	r3, [r2, #0]
	cmp	r3, r8
	bne.n	.L_0803f444
.L_0803f454:
	ldrh	r3, [r6, #16]
	movs	r0, #210
	strh	r3, [r6, #28]
	ldrh	r3, [r6, #18]
	lsls	r0, r0, #2
	strh	r3, [r6, #30]
	adds	r3, r5, r0
	ldr	r7, [r3, #0]
	cmp	r7, #0
	beq.n	.L_0803f484
.L_0803f468:
	cmp	r7, r6
	beq.n	.L_0803f47e
	ldrh	r3, [r6, #16]
	movs	r0, #16
	ldrsh	r2, [r7, r0]
	strh	r3, [r7, #24]
	movs	r1, #16
	ldrsh	r3, [r6, r1]
	subs	r3, r3, r2
	asrs	r3, r3, #1
	strh	r3, [r7, #20]
.L_0803f47e:
	ldr	r7, [r7, #4]
	cmp	r7, #0
	bne.n	.L_0803f468
.L_0803f484:
	movs	r0, #2
	bl	sub_08013560
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r7, [r3, #0]
	cmp	r7, #0
	beq.n	.L_0803f4ae
	movs	r2, #0
	mov	r8, r2
.L_0803f49a:
	cmp	r7, r6
	beq.n	.L_0803f4a8
	ldrh	r0, [r7, #12]
	bl	sub_08014274
	mov	r3, r8
	strh	r3, [r7, #10]
.L_0803f4a8:
	ldr	r7, [r7, #4]
	cmp	r7, #0
	bne.n	.L_0803f49a
.L_0803f4ae:
	movs	r0, #210
	lsls	r0, r0, #2
	adds	r3, r5, r0
	str	r6, [r3, #0]
	movs	r3, #0
	str	r3, [r6, #0]
	str	r3, [r6, #4]
	movs	r3, #4
	strh	r3, [r6, #24]
	movs	r1, #211
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r7, [r3, #0]
	movs	r2, #0
	mov	r8, r2
	cmp	r7, #0
	beq.n	.L_0803f4e0
.L_0803f4d0:
	ldrh	r3, [r6, #24]
	adds	r3, #16
	strh	r3, [r6, #24]
	ldr	r7, [r7, #4]
	movs	r3, #1
	add	r8, r3
	cmp	r7, #0
	bne.n	.L_0803f4d0
.L_0803f4e0:
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
	movs	r3, #192
	movs	r2, #235
	lsls	r3, r3, #2
	lsls	r2, r2, #2
	adds	r3, #158
	adds	r0, r1, r2
	adds	r1, r5, r3
	ldrh	r3, [r1, #0]
	adds	r2, r5, #2
	strh	r3, [r2, r0]
	movs	r0, #16
	ldrsh	r2, [r6, r0]
	movs	r0, #24
	ldrsh	r3, [r6, r0]
	movs	r0, #192
	subs	r3, r3, r2
	lsls	r0, r0, #2
	movs	r2, #0
	mov	r8, r2
	asrs	r3, r3, #1
	adds	r0, #154
	strh	r3, [r6, #20]
	mov	r2, r8
	adds	r3, r5, r0
	strh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	ldrh	r2, [r1, #0]
	movs	r0, #2
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl	sub_08013560
	movs	r0, #1
	bl	sub_0803deac
	ldrh	r3, [r6, #10]
	adds	r7, r0, #0
	strh	r3, [r7, #10]
	mov	r0, r8
	ldrh	r3, [r6, #32]
	movs	r4, #63
	strh	r3, [r7, #32]
	ldrh	r3, [r6, #8]
	strh	r3, [r7, #8]
	ldrh	r3, [r6, #12]
	strh	r3, [r7, #12]
	ldrh	r3, [r6, #14]
	strh	r3, [r7, #14]
	b.n	.L_0803f55c
	movs	r0, r0
	.2byte 0x0080
	.2byte 0x0000
.L_0803f55c:
	ldrh	r2, [r6, #16]
	strh	r2, [r7, #16]
	ldrh	r3, [r6, #18]
	strh	r2, [r7, #24]
	strh	r3, [r7, #18]
	strh	r3, [r7, #26]
	movs	r2, #13
	ldrh	r3, [r6, #28]
	negs	r2, r2
	strh	r3, [r7, #28]
	ldrh	r3, [r6, #30]
	strh	r0, [r7, #22]
	strh	r3, [r7, #30]
	mov	r3, r8
	strh	r3, [r7, #20]
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r3, [r7, #34]
	strh	r3, [r7, #38]
	adds	r0, r7, #0
	adds	r0, #40
	ldrb	r3, [r0, #5]
	ldrb	r1, [r0, #7]
	ands	r2, r3
	movs	r3, #33
	negs	r3, r3
	ands	r2, r3
	adds	r3, #16
	ands	r2, r3
	ands	r2, r4
	adds	r3, r4, #0
	strb	r2, [r0, #5]
	ands	r3, r1
	ldrb	r2, [r0, #9]
	movs	r1, #64
	orrs	r3, r1
	strb	r3, [r0, #7]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r0, #9]
	ldrh	r3, [r7, #14]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #255
	ldrh	r1, [r0, #8]
	ands	r2, r3
	ldr	r3, [pc, #76]
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #210
	lsls	r2, r2, #2
	strh	r3, [r0, #8]
	mov	r1, r8
	adds	r3, r5, r2
	mov	r0, r8
	strh	r1, [r6, #10]
	str	r0, [r3, #0]
	movs	r1, #211
	lsls	r1, r1, #2
	adds	r0, r5, r1
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_0803f5f4
	adds	r6, r3, #0
	ldr	r2, [r6, #4]
	cmp	r2, #0
	beq.n	.L_0803f5ec
.L_0803f5e2:
	adds	r6, r2, #0
	ldr	r3, [r6, #4]
	adds	r2, r3, #0
	cmp	r3, #0
	bne.n	.L_0803f5e2
.L_0803f5ec:
	movs	r3, #0
	str	r7, [r6, #4]
	str	r6, [r7, #0]
	b.n	.L_0803f5f8
.L_0803f5f4:
	str	r7, [r0, #0]
	str	r3, [r7, #0]
.L_0803f5f8:
	str	r3, [r7, #4]
	mov	r0, r9
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffffc00
