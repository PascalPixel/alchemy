.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014dac, 0x08014dac
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038010, 0x08038010
	.set sub_08038070, 0x08038070
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038290, 0x08038290
	.set sub_080383c0, 0x080383c0
	.set sub_080383e8, 0x080383e8
	.set sub_080383f0, 0x080383f0
	.set sub_080383f8, 0x080383f8
	.set sub_080ad100, 0x080ad100
	.set sub_080ad250, 0x080ad250
	.set sub_080f80a8, 0x080f80a8
	.set sub_080f80c4, 0x080f80c4
	.set sub_080f80e0, 0x080f80e0
	.set sub_080f8888, 0x080f8888
	.set sub_080f8ab4, 0x080f8ab4
	.set sub_080f9108, 0x080f9108
	.set sub_080f9448, 0x080f9448
	.set sub_080f9464, 0x080f9464
	.set sub_080fa478, 0x080fa478
	.set sub_080fe894, 0x080fe894
	.set sub_080feb58, 0x080feb58
	.set sub_080fee04, 0x080fee04
	.set sub_080feec0, 0x080feec0
	.set sub_080ffcd4, 0x080ffcd4
	.set sub_0810021c, 0x0810021c
	.set sub_081051a8, 0x081051a8
	.set sub_0810526c, 0x0810526c
	.set sub_081053a8, 0x081053a8
	.set sub_08105468, 0x08105468
	.set sub_081054cc, 0x081054cc
	.global Func_080fe274
	.thumb_func
Func_080fe274:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #236
	movs	r0, #220
	sub	sp, #12
	bl	sub_08014cc0
	adds	r6, r0, #0
	movs	r0, #64
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #160
	lsls	r0, r0, #6
	bl	sub_08014dac
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #24]
	ldr	r5, [r3, #124]
	mov	fp, r3
	movs	r3, #1
	movs	r1, #0
	adds	r7, r0, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_080383e8
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #54
	adds	r5, r5, r2
	ldrh	r3, [r5, #0]
	str	r3, [sp, #4]
	movs	r3, #17
	strh	r3, [r5, #0]
	bl	sub_080f80c4
	movs	r0, #0
	bl	sub_080f80e0
	ldr	r3, [pc, #60]
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r2, r2, r6
	strh	r3, [r2, #0]
	movs	r3, #129
	lsls	r3, r3, #2
	adds	r0, r6, r3
	mov	r9, r2
	bl	sub_080ad100
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r6, r2
	strb	r0, [r3, #0]
	movs	r1, #3
	movs	r0, #0
	movs	r2, #0
	movs	r3, #7
	bl	sub_080fee04
	ldr	r3, [pc, #20]
	movs	r1, #160
	mov	sl, r3
	mov	r0, r8
	lsls	r1, r1, #19
	movs	r2, #64
	mov	lr, sl
	.2byte 0xf800
	.2byte 0xe004
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x0730
	.2byte 0x0300
	movs	r2, #160
	ldr	r1, [pc, #200]
	lsls	r2, r2, #6
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x200e
	bl	sub_080f9108
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #176]
	lsls	r1, r1, #19
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #164]
	adds	r1, #28
	adds	r2, #1
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #148]
	adds	r1, #4
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #140]
	adds	r1, #28
	adds	r2, #1
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_080383c0
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #13
	movs	r3, #5
	movs	r1, #0
	movs	r2, #17
	bl	sub_08038010
	adds	r3, r6, #0
	adds	r3, #240
	str	r0, [r3, #0]
	ldr	r0, [sp, #8]
	bl	sub_080ad250
	cmp	r0, #0
	beq.n	.L_080fe3a8
	bl	sub_081051a8
.L_080fe3a8:
	ldr	r0, [pc, #92]
	bl	sub_080383f8
	bl	sub_081053a8
	bl	sub_080f9448
	ldr	r2, [pc, #60]
	mov	r3, r9
	strh	r2, [r3, #0]
	bl	.L_080fe49c
	str	r0, [sp, #8]
	bl	sub_080f9464
	bl	sub_08105468
	ldr	r0, [r6, #40]
	bl	sub_08038268
	bl	sub_0810526c
	bl	sub_080f80a8
	movs	r3, #20
	movs	r1, #0
	movs	r2, #30
	movs	r0, #0
	bl	sub_080383e8
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08038290
	movs	r0, #0
	b.n	.L_080fe40c
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x06004000
	.4byte 0x05000200
	.4byte 0x050001c8
	.4byte 0x050001e8
	.2byte 0x2500
	.2byte 0x0600
.L_080fe40c:
	bl	sub_080383c0
	movs	r0, #160
	mov	r1, r8
	movs	r2, #64
	lsls	r0, r0, #19
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x22a0
	adds	r1, r7, #0
	lsls	r2, r2, #6
	ldr	r0, [pc, #84]
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1c38
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	mov	r3, fp
	ldr	r2, [r3, #60]
	ldr	r3, [pc, #52]
	strb	r3, [r2, #6]
	bl	sub_080fa478
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_080383e8
	movs	r0, #220
	bl	sub_0801314c
	mov	r2, fp
	ldr	r3, [r2, #24]
	ldr	r2, [pc, #28]
	movs	r0, #1
	strh	r2, [r3, #4]
	add	r3, sp, #4
	ldrh	r3, [r3, #0]
	strh	r3, [r5, #0]
	bl	sub_08013560
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	b.n	.L_080fe47c
	.4byte 0x00000001
	.4byte 0x00000000
	.2byte 0x4000
	.2byte 0x0600
.L_080fe47c:
	bl	sub_080383f0
	mov	r3, fp
	ldr	r2, [r3, #60]
	ldr	r3, [pc, #16]
	strb	r3, [r2, #6]
	ldr	r0, [sp, #8]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
.L_080fe49c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r5, #0
	mov	sl, r3
	ldr	r3, [pc, #200]
	movs	r7, #0
	movs	r6, #0
	mov	r8, r3
	b.n	.L_080fe552
.L_080fe4ba:
	cmp	r5, #1
	beq.n	.L_080fe508
	cmp	r5, #1
	bgt.n	.L_080fe4c8
	cmp	r5, #0
	beq.n	.L_080fe4d2
	b.n	.L_080fe550
.L_080fe4c8:
	cmp	r5, #2
	beq.n	.L_080fe520
	cmp	r5, #3
	beq.n	.L_080fe53a
	b.n	.L_080fe550
.L_080fe4d2:
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r1, #144
	strh	r7, [r3, #0]
	lsls	r1, r1, #3
	mov	r0, r8
	bl	sub_080145a8
	movs	r0, #1
	bl	sub_081054cc
	movs	r0, #0
	bl	.L_080fe580
	adds	r5, r0, #0
	mov	r0, r8
	bl	sub_08014644
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_080fe504
	adds	r6, r5, #0
	movs	r7, #1
.L_080fe504:
	movs	r5, #1
	b.n	.L_080fe552
.L_080fe508:
	movs	r0, #1
	bl	sub_081054cc
	bl	sub_080feec0
	adds	r6, r0, #0
	mvns	r2, r6
	negs	r3, r2
	orrs	r3, r2
	lsrs	r5, r3, #31
	lsls	r5, r5, #1
	b.n	.L_080fe552
.L_080fe520:
	movs	r0, #0
	bl	sub_081054cc
	bl	sub_080ffcd4
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	movs	r5, #0
	cmp	r6, r3
	beq.n	.L_080fe552
	movs	r5, #3
	b.n	.L_080fe552
.L_080fe53a:
	movs	r0, #0
	bl	sub_081054cc
	bl	sub_0810021c
	adds	r6, r0, #0
	mvns	r2, r6
	negs	r3, r2
	orrs	r3, r2
	lsrs	r5, r3, #31
	b.n	.L_080fe552
.L_080fe550:
	movs	r7, #1
.L_080fe552:
	cmp	r7, #0
	bne.n	.L_080fe562
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fe4ba
.L_080fe562:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fe572
	movs	r6, #1
	negs	r6, r6
.L_080fe572:
	adds	r0, r6, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x4da9
	.2byte 0x0810
.L_080fe580:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r2, #28
	adds	r2, r2, r0
	adds	r3, #220
	lsls	r0, r0, #2
	ldr	r5, [r3, #0]
	mov	sl, r0
	mov	r3, sl
	adds	r3, #20
	ldr	r0, [r5, r3]
	movs	r6, #0
	movs	r3, #1
	strb	r3, [r0, #5]
	strh	r6, [r0, #12]
	ldr	r0, [r5, #16]
	sub	sp, #4
	mov	r8, r2
	ldrsb	r7, [r5, r2]
	bl	sub_08038260
	movs	r0, #185
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fe5ce
	movs	r3, #3
	ldr	r0, [r5, #16]
	movs	r1, #9
	str	r3, [sp, #0]
	movs	r2, #1
	movs	r3, #9
	bl	sub_08038070
.L_080fe5ce:
	movs	r3, #3
	ldr	r0, [r5, #16]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #3
	movs	r3, #12
	bl	sub_08038070
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L_080fe5ee
	ldr	r3, [pc, #40]
	mov	r2, r8
	strb	r3, [r5, r2]
	b.n	.L_080fe5fc
.L_080fe5ee:
	lsls	r0, r7, #1
	adds	r0, r0, r7
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	sub_080f8ab4
.L_080fe5fc:
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L_080fe614
	bl	sub_080feb58
	b.n	.L_080fe618
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080fe614:
	bl	sub_080fe894
.L_080fe618:
	adds	r6, r0, #0
	mov	r3, sl
	adds	r3, #20
	ldr	r0, [r5, r3]
	bl	sub_080f8888
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
