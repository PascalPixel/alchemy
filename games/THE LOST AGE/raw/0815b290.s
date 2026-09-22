.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_0801521c, 0x0801521c
	.set sub_08020010, 0x08020010
	.set sub_08020030, 0x08020030
	.set sub_08020048, 0x08020048
	.set sub_08118040, 0x08118040
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08144aac, 0x08144aac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815b24c, 0x0815b24c
	.set sub_0815bb7c, 0x0815bb7c
	.set sub_0815be50, 0x0815be50
	.set sub_0815c2b0, 0x0815c2b0
	.set sub_0815c300, 0x0815c300
	.set sub_0815c970, 0x0815c970
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0815b290
Overlay_0815b290:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	str	r1, [sp, #0]
	movs	r1, #0
	mov	sl, r2
	adds	r7, r3, #0
	mov	fp, r0
	mov	r8, r1
	bl	sub_080143ac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #16]
	adds	r5, r0, #0
	ldrb	r3, [r6, #20]
	movs	r2, #0
	b.n	.L_0815b2c8
.L_0815b2be:
	adds	r2, #1
	adds	r6, #56
	cmp	r2, #63
	bgt.n	.L_0815b2ce
	ldrb	r3, [r6, #20]
.L_0815b2c8:
	cmp	r3, #0
	bne.n	.L_0815b2be
	mov	r8, r6
.L_0815b2ce:
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080142d4
	mov	r2, r8
	strb	r5, [r2, #16]
	ldr	r5, [pc, #60]
	mov	r3, r8
	ldrb	r2, [r3, #17]
	movs	r1, #0
	strh	r1, [r3, #18]
	strb	r5, [r3, #26]
	movs	r3, #2
	negs	r3, r3
	ands	r3, r2
	mov	r2, r8
	strb	r3, [r2, #17]
	mov	r3, r8
	stmia	r3!, {r1}
	mov	r2, sl
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #4
	orrs	r7, r0
	orrs	r7, r2
	str	r7, [r3, #0]
	mov	r3, r8
	adds	r3, #28
	str	r1, [r3, #0]
	movs	r3, #192
	mov	r1, r8
	lsls	r3, r3, #7
	str	r3, [r1, #32]
	ldr	r3, [pc, #12]
	mov	r9, r3
	movs	r3, #187
	b.n	.L_0815b324
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x36e0
	.2byte 0x0200
.L_0815b324:
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	lsrs	r3, r3, #5
	orrs	r3, r2
	str	r3, [r1, #36]
	bl	sub_0815b24c
	mov	r2, sp
	ldrb	r2, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r1, fp
	str	r3, [r6, #12]
	movs	r3, #1
	str	r0, [r6, #40]
	strb	r1, [r6, #20]
	strb	r2, [r6, #21]
	strb	r5, [r6, #23]
	strb	r5, [r6, #22]
	strb	r3, [r6, #27]
	strb	r5, [r6, #26]
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_08020030
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #24]
	ldrb	r2, [r6, #20]
	ldrb	r3, [r6, #21]
	mov	r1, sl
	adds	r5, r3, #0
	muls	r5, r2
	movs	r3, #128
	lsls	r3, r3, #6
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_0815b374
	lsrs	r5, r5, #1
.L_0815b374:
	adds	r1, r5, #0
	ldrb	r0, [r6, #16]
	movs	r2, #0
	bl	sub_080142d4
	ldrh	r3, [r7, #0]
	ldrh	r1, [r6, #8]
	adds	r3, r3, r5
	strh	r3, [r7, #0]
	ldrb	r3, [r6, #16]
	mov	r0, r8
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r2, [r3, #2]
	ldr	r3, [pc, #24]
	lsls	r2, r2, #17
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, #8]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #16]
	adds	r7, r0, #0
	ldrb	r3, [r5, #20]
	mov	r8, r1
	movs	r6, #0
	movs	r2, #0
	b.n	.L_0815b3d2
.L_0815b3c8:
	adds	r2, #1
	adds	r5, #56
	cmp	r2, #63
	bgt.n	.L_0815b3d8
	ldrb	r3, [r5, #20]
.L_0815b3d2:
	cmp	r3, #0
	bne.n	.L_0815b3c8
	adds	r6, r5, #0
.L_0815b3d8:
	bl	sub_0815b24c
	mov	r3, r8
	strb	r3, [r5, #21]
	movs	r3, #128
	ldrb	r2, [r5, #17]
	lsls	r3, r3, #9
	str	r3, [r5, #12]
	movs	r3, #2
	negs	r3, r3
	ands	r3, r2
	movs	r1, #0
	strb	r3, [r5, #17]
	movs	r3, #1
	str	r0, [r5, #40]
	strb	r7, [r5, #20]
	strb	r1, [r5, #23]
	strb	r1, [r5, #22]
	strb	r3, [r5, #27]
	strb	r1, [r5, #26]
	strh	r1, [r5, #18]
	adds	r0, r5, #0
	bl	sub_08020030
	adds	r0, r6, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r4, #160
	lsls	r4, r4, #19
	movs	r0, #0
	adds	r4, #2
.L_0815b41a:
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	lsls	r2, r3, #10
	lsls	r1, r3, #5
	orrs	r2, r1
	orrs	r2, r3
	adds	r0, #1
	strh	r2, [r4, #0]
	adds	r4, #2
	cmp	r0, #63
	bne.n	.L_0815b41a
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	mov	sl, r3
	mov	r9, r0
	mov	r3, sl
	muls	r3, r1
	movs	r0, #1
	sub	sp, #8
	negs	r0, r0
	str	r2, [sp, #4]
	add	r0, sl
	subs	r2, r1, #1
	mov	r8, r2
	adds	r1, r3, #0
	mov	fp, r0
	ldr	r3, [pc, #168]
	mov	r0, r9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4640
	movs	r4, #0
	cmp	r0, #0
	beq.n	.L_0815b4fe
.L_0815b472:
	lsls	r0, r4, #14
	mov	r1, r8
	adds	r5, r4, #0
	adds	r6, r4, #0
	str	r4, [sp, #0]
	bl	sub_08002054
	bl	sub_08002090
	lsls	r3, r0, #6
	ldr	r2, [sp, #4]
	subs	r3, r3, r0
	asrs	r1, r3, #16
	adds	r3, r2, #0
	muls	r3, r1
	movs	r7, #0
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bge.n	.L_0815b4a0
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r3, r0
.L_0815b4a0:
	asrs	r1, r3, #16
	cmp	r1, #63
	ble.n	.L_0815b4a8
	movs	r1, #63
.L_0815b4a8:
	cmp	r4, #0
	blt.n	.L_0815b4f8
.L_0815b4ac:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	mov	r0, fp
	subs	r2, r0, r3
	mov	r0, r8
	subs	r3, r0, r7
	mov	r0, sl
	muls	r0, r3
	adds	r3, r0, #0
	adds	r3, r3, r2
	mov	r2, r9
	strb	r1, [r2, r3]
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	mov	r0, fp
	subs	r2, r0, r3
	mov	r0, r8
	subs	r3, r0, r5
	mov	r0, sl
	muls	r0, r3
	adds	r3, r0, #0
	adds	r3, r3, r2
	mov	r2, r9
	strb	r1, [r2, r3]
	lsls	r3, r7, #1
	subs	r3, r6, r3
	subs	r6, r3, #1
	cmp	r6, #0
	bge.n	.L_0815b4f2
	lsls	r3, r5, #1
	adds	r3, r6, r3
	subs	r6, r3, #2
	subs	r5, #1
.L_0815b4f2:
	adds	r7, #1
	cmp	r5, r7
	bge.n	.L_0815b4ac
.L_0815b4f8:
	adds	r4, #1
	cmp	r4, r8
	bne.n	.L_0815b472
.L_0815b4fe:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0258
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	subs	r1, r6, #1
	lsrs	r3, r6, #31
	mov	r9, r1
	adds	r3, r6, r3
	adds	r1, r6, #0
	muls	r1, r6
	asrs	r3, r3, #1
	sub	sp, #8
	subs	r3, #1
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	lsls	r1, r1, #1
	ldr	r3, [pc, #244]
	adds	r7, r0, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	mov	r3, r9
	mov	fp, r2
	cmp	r3, #0
	beq.n	.L_0815b622
.L_0815b54a:
	movs	r1, #0
	mov	r5, fp
	mov	sl, r1
	lsls	r0, r5, #14
	mov	r1, r9
	bl	sub_08002054
	bl	sub_08002090
	lsls	r3, r0, #6
	ldr	r2, [sp, #4]
	subs	r3, r3, r0
	asrs	r4, r3, #16
	adds	r3, r2, #0
	muls	r3, r4
	mov	r8, fp
	cmp	r3, #0
	bge.n	.L_0815b576
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
.L_0815b576:
	asrs	r4, r3, #16
	cmp	r4, #63
	ble.n	.L_0815b57e
	movs	r4, #63
.L_0815b57e:
	mov	r2, fp
	cmp	r2, #0
	blt.n	.L_0815b61a
	lsls	r3, r6, #1
	mov	ip, r3
.L_0815b588:
	ldr	r1, [sp, #0]
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r0, r1, r3
	mov	r3, r9
	mov	r1, sl
	subs	r2, r3, r1
	adds	r1, r6, #0
	muls	r1, r2
	adds	r3, r1, r0
	strb	r4, [r7, r3]
	mov	r3, ip
	subs	r2, r3, r2
	subs	r3, r2, #1
	adds	r2, r6, #0
	muls	r2, r3
	adds	r3, r2, r0
	strb	r4, [r7, r3]
	subs	r3, r6, r0
	adds	r1, r1, r3
	adds	r2, r2, r3
	adds	r1, r7, r1
	adds	r2, r7, r2
	subs	r1, #1
	subs	r2, #1
	strb	r4, [r1, #0]
	strb	r4, [r2, #0]
	mov	r1, sl
	ldr	r2, [sp, #0]
	lsrs	r3, r1, #31
	add	r3, sl
	asrs	r3, r3, #1
	subs	r0, r2, r3
	mov	r3, r9
	subs	r2, r3, r5
	adds	r1, r6, #0
	muls	r1, r2
	adds	r3, r1, r0
	strb	r4, [r7, r3]
	mov	r3, ip
	subs	r2, r3, r2
	subs	r3, r2, #1
	adds	r2, r6, #0
	muls	r2, r3
	adds	r3, r2, r0
	strb	r4, [r7, r3]
	subs	r3, r6, r0
	adds	r1, r1, r3
	adds	r2, r2, r3
	adds	r1, r7, r1
	subs	r1, #1
	adds	r2, r7, r2
	strb	r4, [r1, #0]
	subs	r2, #1
	mov	r1, sl
	strb	r4, [r2, #0]
	lsls	r3, r1, #1
	mov	r2, r8
	subs	r3, r2, r3
	subs	r3, #1
	mov	r8, r3
	cmp	r3, #0
	bge.n	.L_0815b612
	lsls	r3, r5, #1
	add	r3, r8
	subs	r3, #2
	mov	r8, r3
	subs	r5, #1
.L_0815b612:
	movs	r3, #1
	add	sl, r3
	cmp	r5, sl
	bge.n	.L_0815b588
.L_0815b61a:
	movs	r1, #1
	add	fp, r1
	cmp	fp, r9
	bne.n	.L_0815b54a
.L_0815b622:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0258
	.2byte 0x0300
	push	{lr}
	sub	sp, #256
	movs	r1, #160
	lsls	r1, r1, #19
	movs	r2, #128
	ldr	r3, [pc, #72]
	add	r0, sp, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4669
	mov	r2, sp
	movs	r0, #1
	add	r4, sp, #128
	adds	r1, #2
	adds	r2, #254
.L_0815b652:
	ldrh	r3, [r2, #0]
	adds	r0, #1
	strh	r3, [r1, #0]
	subs	r2, #2
	adds	r1, #2
	cmp	r0, #64
	bne.n	.L_0815b652
	ldrh	r3, [r4, #0]
	mov	r2, sp
	movs	r0, #160
	strh	r3, [r2, #0]
	mov	r1, sp
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b03
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	add	sp, #256
	pop	{pc}
	movs	r0, r0
	.4byte 0x00000810
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #92]
	ldr	r3, [pc, #196]
	sub	sp, #24
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	add	r1, sp, #8
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	movs	r2, #192
	movs	r3, #0
	str	r3, [r1, #12]
	str	r3, [r1, #4]
	lsls	r2, r2, #3
	adds	r2, #228
	adds	r5, r7, r2
	ldr	r3, [r5, #0]
	movs	r2, #238
	str	r3, [r1, #0]
	movs	r3, #221
	lsls	r3, r3, #3
	adds	r6, r7, r3
	ldr	r3, [r6, #0]
	lsls	r2, r2, #7
	adds	r2, #248
	str	r3, [r1, #8]
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	mov	r2, sp
	movs	r3, #0
	bl	sub_08020010
	movs	r3, #222
	lsls	r3, r3, #3
	adds	r2, r7, r3
	ldr	r2, [r2, #0]
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	movs	r2, #192
	str	r3, [r5, #0]
	lsls	r2, r2, #3
	adds	r2, #244
	adds	r1, r7, r2
	ldr	r2, [r1, #0]
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	movs	r2, #238
	str	r3, [r6, #0]
	lsls	r2, r2, #7
	adds	r2, #140
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	cmp	r3, #19
	bgt.n	.L_0815b704
	ldr	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	b.n	.L_0815b708
.L_0815b704:
	ldr	r3, [r1, #0]
	ldr	r2, [pc, #88]
.L_0815b708:
	adds	r3, r3, r2
	str	r3, [r1, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #140
	adds	r5, r7, r3
	ldr	r0, [r5, #0]
	cmp	r0, #11
	ble.n	.L_0815b72c
	movs	r1, #222
	lsls	r1, r1, #3
	adds	r2, r7, r1
	ldr	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	str	r3, [r2, #0]
	ldr	r0, [r5, #0]
.L_0815b72c:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #248
	adds	r3, r7, r2
	movs	r1, #7
	ldr	r6, [r3, #0]
	bl	sub_08002054
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L_0815b744
	adds	r1, r0, #7
.L_0815b744:
	asrs	r1, r1, #3
	lsls	r1, r1, #3
	subs	r1, r0, r1
	adds	r1, #7
	adds	r0, r6, #0
	bl	sub_08020030
	ldr	r3, [r5, #0]
	add	sp, #24
	adds	r3, #1
	str	r3, [r5, #0]
	pop	{r5, r6, r7, pc}
	.4byte 0x08196e4c
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #152
	str	r0, [sp, #100]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	adds	r2, r3, #0
	str	r0, [sp, #96]
	adds	r2, #176
	ldr	r2, [r2, #0]
	ldr	r1, [r3, #92]
	str	r2, [sp, #92]
	movs	r2, #0
	ldr	r3, [r3, #100]
	str	r2, [sp, #80]
	str	r3, [sp, #84]
	movs	r3, #240
	str	r2, [sp, #76]
	lsls	r3, r3, #7
	mov	fp, r1
	adds	r3, #240
	add	r3, fp
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r3, #0
	str	r3, [sp, #88]
	ldr	r5, [pc, #48]
	movs	r6, #31
.L_0815b7aa:
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r4, #0
	adds	r0, #192
	mov	r9, r4
.L_0815b7b4:
	ldrh	r3, [r0, #0]
	adds	r1, r6, #0
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	ands	r2, r5
	lsrs	r3, r3, #26
	ands	r3, r5
	cmp	r2, #0
	bne.n	.L_0815b7dc
	cmp	r3, #0
	ble.n	.L_0815b7ce
	subs	r3, #1
.L_0815b7ce:
	cmp	r1, #0
	ble.n	.L_0815b7dc
	subs	r1, #1
	b.n	.L_0815b7dc
	movs	r0, r0
	.2byte 0x001f
	.2byte 0x0000
.L_0815b7dc:
	cmp	r2, #0
	ble.n	.L_0815b7e2
	subs	r2, #1
.L_0815b7e2:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	movs	r7, #1
	orrs	r3, r2
	add	r9, r7
	orrs	r3, r1
	mov	r1, r9
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r1, #128
	bne.n	.L_0815b7b4
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #88]
	adds	r2, #1
	str	r2, [sp, #88]
	cmp	r2, #32
	bne.n	.L_0815b7aa
	movs	r0, #0
	bl	sub_081435e0
	ldr	r1, [pc, #220]
	ldr	r0, [pc, #220]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0815b840
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #136
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0815b840:
	strh	r4, [r0, #0]
	movs	r0, #1
	ldr	r6, [pc, #172]
	bl	sub_08013560
	movs	r1, #128
	movs	r2, #1
	ldr	r5, [pc, #168]
	adds	r0, r6, #0
	lsls	r1, r1, #1
	negs	r2, r2
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2380
	lsls	r3, r3, #1
	adds	r6, r6, r3
	adds	r0, r6, #0
	movs	r1, #128
	ldr	r2, [pc, #148]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2480
	ldr	r1, [pc, #144]
	lsls	r4, r4, #10
	adds	r6, #128
	movs	r0, #0
	adds	r4, #2
.L_0815b876:
	movs	r3, #0
.L_0815b878:
	adds	r3, #1
	stmia	r6!, {r1}
	adds	r1, r1, r4
	cmp	r3, #8
	bne.n	.L_0815b878
	ldr	r2, [pc, #120]
	movs	r3, #0
.L_0815b886:
	adds	r3, #1
	stmia	r6!, {r2}
	cmp	r3, #8
	bne.n	.L_0815b886
	adds	r0, #1
	cmp	r0, #16
	bne.n	.L_0815b876
	movs	r1, #200
	lsls	r1, r1, #1
	adds	r1, #255
	movs	r0, #8
	movs	r2, #3
	bl	sub_08152404
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	movs	r0, #1
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	subs	r2, #70
	strh	r3, [r2, #0]
	bl	sub_08013560
	mov	r4, sp
	adds	r4, #128
	adds	r1, r4, #0
	movs	r0, #0
	str	r4, [sp, #72]
	bl	sub_08144aac
	b.n	.L_0815b904
	.4byte 0x00000080
	.4byte 0x00000100
	.4byte 0x00001010
	.4byte 0x00003f46
	.4byte 0x00001f83
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x0600f800
	.4byte 0x03000260
	.4byte 0x03ff03ff
	.2byte 0x0200
	.2byte 0x0201
.L_0815b904:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	movs	r5, #240
	adds	r2, #132
	lsls	r5, r5, #5
	add	r2, fp
	movs	r3, #0
	movs	r1, #200
	adds	r5, #129
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #228]
	add	r5, fp
	bl	sub_080145a8
	adds	r1, r5, #0
	ldr	r0, [pc, #224]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r7, #0
	movs	r1, #140
	mov	r9, r7
	lsls	r1, r1, #6
.L_0815b940:
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0815b94c
	adds	r3, #128
	strb	r3, [r5, #0]
.L_0815b94c:
	movs	r0, #1
	add	r9, r0
	adds	r5, #1
	cmp	r9, r1
	bne.n	.L_0815b940
	movs	r5, #240
	lsls	r5, r5, #5
	movs	r3, #80
	adds	r5, #129
	str	r3, [sp, #0]
	add	r5, fp
	movs	r3, #112
	str	r3, [sp, #4]
	ldr	r4, [sp, #128]
	ldr	r0, [sp, #96]
	adds	r1, r5, #0
	movs	r2, #24
	movs	r3, #8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x23f0
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, fp
	movs	r6, #1
	str	r6, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [pc, #132]
	bl	sub_08014644
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #128]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #120]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #112]
	ldr	r1, [sp, #84]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #92]
	movs	r1, #0
	movs	r2, #200
	movs	r3, #168
	str	r1, [sp, #60]
	lsls	r2, r2, #16
	str	r6, [r4, #16]
	lsls	r3, r3, #15
	movs	r1, #240
	str	r2, [sp, #64]
	str	r3, [sp, #68]
	lsls	r1, r1, #6
	ldr	r3, [pc, #80]
	ldr	r0, [sp, #96]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0c
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r0, #1
	strh	r3, [r2, #0]
	bl	sub_08013560
	movs	r0, #184
	movs	r1, #92
	movs	r7, #0
	negs	r0, r0
	negs	r1, r1
	str	r7, [sp, #88]
	str	r0, [sp, #16]
	str	r1, [sp, #12]
.L_0815b9f2:
	movs	r2, #1
	str	r2, [sp, #56]
	ldr	r3, [pc, #44]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0815ba28
	movs	r3, #1
	str	r3, [sp, #76]
	b.n	.L_0815ba28
	.4byte 0x00007141
	.4byte 0x08143001
	.4byte 0x000000b1
	.4byte 0x00000178
	.4byte 0x000000b3
	.4byte 0x00000137
	.4byte 0x03000258
	.2byte 0x1150
	.2byte 0x0300
.L_0815ba28:
	ldr	r3, [sp, #88]
	ldr	r4, [sp, #56]
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0815ba3c
	ldr	r7, [sp, #76]
	cmp	r7, #1
	bne.n	.L_0815ba3c
	movs	r0, #0
	str	r0, [sp, #56]
.L_0815ba3c:
	ldr	r1, [sp, #88]
	cmp	r1, #0
	bne.n	.L_0815bb1c
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne.n	.L_0815ba76
	movs	r1, #185
	lsls	r1, r1, #4
	adds	r1, #255
	ldr	r0, [pc, #120]
	bl	sub_080145a8
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #255
	ldr	r0, [pc, #112]
	bl	sub_080145a8
	movs	r1, #184
	lsls	r1, r1, #6
	adds	r1, #129
	movs	r3, #0
	ldr	r0, [pc, #100]
	add	r1, fp
	movs	r2, #1
	bl	sub_08157cf4
	movs	r3, #1
	str	r3, [sp, #60]
.L_0815ba76:
	ldr	r2, [pc, #92]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #144
	strh	r3, [r2, #4]
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r0, [pc, #36]
	b.n	.L_0815badc
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00007741
	.4byte 0x00000784
	.4byte 0x00001f80
	.4byte 0x08143265
	.4byte 0x08143425
	.4byte 0x000000c7
	.4byte 0x03001120
	.2byte 0x0150
	.2byte 0x0000
.L_0815badc:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #320]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2400
	mov	r9, r4
	movs	r7, #15
	movs	r6, #0
	mov	r5, fp
.L_0815baf8:
	str	r6, [r5, #24]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #60
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #44
	str	r0, [r5, #4]
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	subs	r6, #2
	adds	r5, #28
	cmp	r1, #9
	bne.n	.L_0815baf8
.L_0815bb1c:
	ldr	r2, [sp, #88]
	cmp	r2, #75
	bgt.n	sub_0815bb7c
	adds	r0, r2, #0
	cmp	r2, #0
	bge.n	.L_0815bb2a
	adds	r0, #3
.L_0815bb2a:
	movs	r1, #18
	asrs	r0, r0, #2
	bl	sub_08002064
	ldr	r4, [sp, #88]
	movs	r3, #215
	lsls	r3, r3, #2
	adds	r6, r0, #0
	adds	r0, r4, #0
	muls	r0, r3
	bl	sub_08002096
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	movs	r2, #60
	subs	r2, r2, r0
	ldr	r0, [pc, #224]
	lsls	r1, r6, #1
	ldrh	r1, [r0, r1]
	ldr	r0, [pc, #220]
	ldr	r3, [sp, #88]
	ldrb	r5, [r0, r6]
	subs	r3, #8
	lsrs	r0, r5, #1
	subs	r2, r2, r0
	ldr	r0, [pc, #212]
	movs	r7, #240
	ldrb	r4, [r0, r6]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #72]
	lsls	r7, r7, #5
	add	r1, fp
	adds	r7, #129
	ldr	r4, [r0, #4]
	adds	r1, r1, r7
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b16
	subs	r3, #76
	cmp	r3, #31
	bhi.n	.L_0815bbe4
	movs	r1, #0
	mov	r9, r1
	mov	r7, fp
.L_0815bb8a:
	ldr	r0, [r7, #24]
	cmp	r0, #17
	bhi.n	.L_0815bbd4
	movs	r1, #3
	bl	sub_08002054
	mov	r2, r9
	movs	r5, #1
	ands	r5, r2
	ldr	r2, [pc, #152]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [pc, #144]
	ldr	r2, [r7, #0]
	ldrb	r6, [r3, r0]
	lsls	r5, r5, #2
	lsrs	r3, r6, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #136]
	ldrb	r4, [r3, r0]
	ldr	r3, [r7, #4]
	str	r6, [sp, #0]
	adds	r3, r3, r4
	ldr	r4, [pc, #128]
	subs	r3, #32
	ldrb	r0, [r4, r0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b8
.L_0815bbd4:
	movs	r1, #1
	add	r9, r1
	adds	r3, r0, #1
	mov	r2, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #4
	bne.n	.L_0815bb8a
.L_0815bbe4:
	ldr	r3, [sp, #88]
	cmp	r3, #108
	bne.n	.L_0815bbf4
	movs	r2, #128
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
.L_0815bbf4:
	ldr	r4, [sp, #88]
	subs	r4, #108
	str	r4, [sp, #52]
	cmp	r4, #8
	bhi.n	.L_0815bc12
	ldr	r7, [sp, #88]
	ldr	r0, [pc, #68]
	ldr	r1, [pc, #32]
	movs	r3, #128
	lsls	r2, r7, #9
	lsls	r3, r3, #19
	adds	r2, r2, r0
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_0815bc12:
	ldr	r1, [sp, #12]
	cmp	r1, #39
	bls.n	.L_0815bc1a
	b.n	.L_0815bd42
.L_0815bc1a:
	ldr	r2, [sp, #16]
	ldr	r7, [pc, #44]
	adds	r0, r2, r1
	b.n	.L_0815bc50
	movs	r0, r0
	.4byte 0x00000010
	.4byte 0x03000730
	.4byte 0x08198686
	.4byte 0x08198662
	.4byte 0x08198674
	.4byte 0x0819747a
	.4byte 0x08197467
	.4byte 0x08197473
	.4byte 0x0819746d
	.4byte 0xffff2800
	.2byte 0x07ff
	.2byte 0x0001
.L_0815bc50:
	lsls	r0, r0, #3
	adds	r0, r0, r1
	lsls	r0, r0, #5
	bl	sub_08002090
	movs	r1, #3
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #9
	movs	r4, #1
	negs	r4, r4
	adds	r6, r0, r3
	mov	r8, r4
	cmp	r6, r7
	bgt.n	.L_0815bc74
	movs	r6, #132
	lsls	r6, r6, #9
.L_0815bc74:
	movs	r0, #128
	adds	r1, r6, #0
	lsls	r0, r0, #17
	bl	sub_08002054
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #216
	add	r3, fp
	ldrh	r2, [r3, #0]
	strh	r0, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #218
	add	r3, fp
	ldrh	r2, [r3, #0]
	strh	r0, [r3, #0]
	movs	r0, #184
	adds	r1, r6, #0
	lsls	r0, r0, #16
	bl	sub_08002054
	movs	r5, #238
	movs	r7, #64
	lsls	r5, r5, #7
	adds	r5, #208
	subs	r0, r7, r0
	add	r5, fp
	lsls	r0, r0, #8
	str	r0, [r5, #0]
	movs	r0, #152
	lsls	r0, r0, #15
	adds	r1, r6, #0
	bl	sub_08002054
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #212
	subs	r0, r7, r0
	lsls	r0, r0, #8
	add	r5, fp
	str	r0, [r5, #0]
	ldr	r0, [sp, #12]
	cmp	r0, #15
	bgt.n	.L_0815bcd2
	mov	r8, r0
	b.n	.L_0815bce0
.L_0815bcd2:
	ldr	r3, [sp, #88]
	subs	r3, #116
	cmp	r3, #8
	bhi.n	.L_0815bce0
	ldr	r1, [sp, #16]
	subs	r1, r7, r1
	mov	r8, r1
.L_0815bce0:
	movs	r2, #1
	negs	r2, r2
	cmp	r8, r2
	beq.n	.L_0815bd34
	mov	r4, r8
	movs	r3, #0
	ldr	r1, [pc, #112]
	mov	r9, r3
	mov	r7, r8
	lsls	r3, r4, #3
	adds	r6, r3, r4
	movs	r0, #0
	movs	r4, #0
	lsls	r5, r7, #1
.L_0815bcfc:
	lsls	r3, r6, #2
	cmp	r3, #0
	bge.n	.L_0815bd04
	adds	r3, #15
.L_0815bd04:
	asrs	r3, r3, #4
	mov	r2, r9
	muls	r2, r3
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_0815bd12
	adds	r3, #63
.L_0815bd12:
	asrs	r2, r3, #6
	adds	r3, r4, r0
	cmp	r3, #0
	bge.n	.L_0815bd1c
	adds	r3, #63
.L_0815bd1c:
	asrs	r3, r3, #6
	lsls	r3, r3, #10
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #1
	add	r9, r3
	mov	r7, r9
	adds	r1, #2
	adds	r4, r4, r5
	add	r0, r8
	cmp	r7, #32
	bne.n	.L_0815bcfc
.L_0815bd34:
	ldr	r0, [sp, #88]
	cmp	r0, #116
	bne.n	.L_0815bd42
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
.L_0815bd42:
	ldr	r3, [sp, #88]
	subs	r3, #124
	cmp	r3, #7
	bhi.n	.L_0815bda2
	ldr	r4, [pc, #16]
	movs	r0, #160
	movs	r1, #0
	lsls	r0, r0, #19
	mov	r9, r1
	b.n	.L_0815bd64
	movs	r0, r0
	.4byte 0x00007341
	.4byte 0x0000001f
	.2byte 0x0100
	.2byte 0x0500
.L_0815bd64:
	ldrh	r3, [r0, #0]
	movs	r2, #31
	ands	r2, r3
	lsls	r3, r3, #16
	subs	r1, r2, #3
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r4
	ands	r3, r4
	subs	r2, #3
	subs	r3, #3
	cmp	r1, #0
	bge.n	.L_0815bd80
	movs	r1, #0
.L_0815bd80:
	cmp	r2, #0
	bge.n	.L_0815bd86
	movs	r2, #0
.L_0815bd86:
	cmp	r3, #0
	bge.n	.L_0815bd8c
	movs	r3, #0
.L_0815bd8c:
	lsls	r2, r2, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	movs	r2, #1
	orrs	r3, r1
	add	r9, r2
	strh	r3, [r0, #0]
	mov	r3, r9
	adds	r0, #2
	cmp	r3, #64
	bne.n	.L_0815bd64
.L_0815bda2:
	ldr	r4, [sp, #88]
	cmp	r4, #132
	bne.n	sub_0815be50
	ldr	r7, [sp, #60]
	cmp	r7, #1
	bne.n	.L_0815bddc
	ldr	r0, [pc, #120]
	bl	sub_08014644
	ldr	r0, [pc, #116]
	bl	sub_08014644
	movs	r1, #185
	lsls	r1, r1, #4
	adds	r1, #255
	ldr	r0, [pc, #108]
	bl	sub_080145a8
	movs	r1, #148
	lsls	r1, r1, #6
	adds	r1, #151
	ldr	r0, [pc, #100]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #0
	str	r0, [sp, #60]
.L_0815bddc:
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #80]
	adds	r2, #40
	str	r3, [r2, #0]
	ldr	r3, [pc, #80]
	adds	r2, #4
	str	r3, [r2, #0]
	ldr	r3, [pc, #36]
	subs	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #6
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	subs	r2, #26
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r0, [pc, #52]
	bl	sub_08013300
	adds	r1, r0, #0
	b.n	.L_0815be44
	movs	r0, r0
	.4byte 0x00007141
	.4byte 0x000000cc
	.4byte 0x00000100
	.4byte 0x00000784
	.4byte 0x00001010
	.4byte 0x08143265
	.4byte 0x08143425
	.4byte 0x08143001
	.4byte 0x0000015c
	.4byte 0xfffff800
	.4byte 0xfffff000
	.2byte 0x013a
	.2byte 0x0000
.L_0815be44:
	movs	r0, #160
	ldr	r3, [pc, #72]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9916
	cmp	r1, #133
	bne.n	.L_0815bf08
	movs	r1, #240
	movs	r2, #0
	ldr	r3, [pc, #56]
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r3, [pc, #48]
	lsls	r1, r1, #6
	ldr	r0, [pc, #48]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b06
	movs	r2, #128
	movs	r4, #164
	lsls	r2, r2, #19
	lsls	r4, r4, #7
	ldr	r1, [pc, #36]
	strh	r3, [r2, #0]
	adds	r4, #152
	movs	r2, #0
	mov	r9, r2
	add	r4, fp
	movs	r0, #0
	mov	r2, fp
	b.n	.L_0815bea4
	.4byte 0x00007541
	.4byte 0x03000730
	.4byte 0x03000260
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x86aa
	.2byte 0x0819
.L_0815bea4:
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	str	r0, [r2, #24]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	subs	r0, #8
	lsls	r3, r3, #16
	str	r3, [r2, #4]
	movs	r3, #2
	ldrsb	r3, [r1, r3]
	lsls	r3, r3, #6
	str	r3, [r2, #8]
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	lsls	r3, r3, #15
	str	r3, [r2, #12]
	movs	r3, #4
	ldrsb	r3, [r1, r3]
	adds	r1, #5
	lsls	r3, r3, #15
	str	r3, [r2, #16]
	movs	r3, #0
	str	r3, [r2, #20]
	movs	r3, #1
	add	r9, r3
	mov	r7, r9
	adds	r2, #28
	cmp	r7, #6
	bne.n	.L_0815bea4
	movs	r0, #0
	movs	r2, #1
	adds	r3, r4, #0
	mov	r9, r0
	negs	r2, r2
	adds	r3, #24
.L_0815beee:
	movs	r1, #1
	add	r9, r1
	mov	r4, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r4, #96
	bne.n	.L_0815beee
	ldr	r0, [pc, #476]
	ldr	r1, [pc, #480]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_0815bf08:
	ldr	r7, [sp, #88]
	cmp	r7, #76
	bne.n	.L_0815bf14
	movs	r0, #138
	bl	sub_081c0010
.L_0815bf14:
	ldr	r0, [sp, #88]
	movs	r1, #133
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_0815bf24
	movs	r0, #138
	bl	sub_081c0010
.L_0815bf24:
	ldr	r3, [sp, #88]
	subs	r3, #133
	cmp	r3, #132
	bls.n	.L_0815bf2e
	b.n	.L_0815c32a
.L_0815bf2e:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #48]
	movs	r0, #1
	bl	sub_081969f8
	movs	r2, #164
	lsls	r2, r2, #7
	ldr	r3, [sp, #88]
	adds	r2, #152
	add	r2, fp
	str	r0, [sp, #44]
	movs	r5, #0
	str	r2, [sp, #40]
	cmp	r3, #196
	ble.n	.L_0815bf56
	ldr	r0, [pc, #400]
	bl	sub_0815f0a0
.L_0815bf56:
	ldr	r2, [pc, #400]
	ldr	r3, [sp, #120]
	ldr	r4, [sp, #44]
	ands	r3, r2
	movs	r2, #6
	orrs	r3, r2
	ldr	r2, [pc, #392]
	mov	r7, sp
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #120]
	movs	r3, #7
	str	r3, [r4, #0]
	ldr	r3, [pc, #376]
	adds	r7, #120
	str	r5, [r4, #20]
	str	r7, [sp, #36]
	str	r7, [r4, #16]
	str	r3, [r4, #8]
	ldr	r0, [sp, #48]
	mov	r2, fp
	adds	r2, #224
	str	r0, [r4, #12]
	str	r2, [sp, #20]
	movs	r1, #0
	mov	r9, r1
	mov	r6, fp
.L_0815bf90:
	mov	r3, r9
	lsls	r3, r3, #3
	str	r3, [sp, #32]
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #0
	bne.n	.L_0815bfa6
	movs	r0, #138
	bl	sub_081c0010
.L_0815bfa6:
	ldr	r5, [r6, #24]
	cmp	r5, #60
	bne.n	.L_0815bfb4
	movs	r0, #212
	bl	sub_081c0010
	ldr	r5, [r6, #24]
.L_0815bfb4:
	cmp	r5, #64
	bne.n	.L_0815bfc0
	movs	r0, #134
	bl	sub_081c0010
	ldr	r5, [r6, #24]
.L_0815bfc0:
	cmp	r5, #35
	bhi.n	.L_0815c00a
	adds	r0, r5, #0
	movs	r1, #6
	bl	sub_08002054
	movs	r1, #6
	bl	sub_08002064
	ldr	r2, [pc, #288]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #284]
	movs	r7, #2
	ldrsh	r2, [r6, r7]
	ldrb	r5, [r3, r0]
	movs	r4, #224
	lsls	r4, r4, #3
	lsrs	r3, r5, #1
	add	r1, fp
	adds	r1, r1, r4
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r6, r4]
	ldr	r4, [pc, #264]
	ldrb	r4, [r4, r0]
	str	r5, [sp, #0]
	adds	r3, r3, r4
	ldr	r4, [pc, #260]
	subs	r3, #36
	ldrb	r0, [r4, r0]
	ldr	r4, [sp, #128]
	str	r0, [sp, #4]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b5
.L_0815c00a:
	adds	r0, r5, #0
	subs	r0, #12
	cmp	r0, #31
	bhi.n	.L_0815c058
	cmp	r0, #0
	bge.n	.L_0815c018
	adds	r0, #3
.L_0815c018:
	movs	r1, #18
	asrs	r0, r0, #2
	bl	sub_08002064
	ldr	r2, [pc, #224]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #220]
	movs	r7, #240
	ldrb	r5, [r3, r0]
	lsls	r7, r7, #5
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r6, r4]
	ldr	r4, [pc, #208]
	adds	r7, #129
	ldrb	r4, [r4, r0]
	add	r1, fp
	adds	r1, r1, r7
	ldr	r7, [sp, #72]
	lsrs	r0, r4, #1
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	subs	r3, r3, r0
	ldr	r4, [r7, #4]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b5
.L_0815c058:
	adds	r0, r5, #0
	subs	r0, #44
	cmp	r0, #21
	bls.n	.L_0815c062
	b.n	.L_0815c182
.L_0815c062:
	lsls	r0, r0, #10
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r0, r3, #4
	cmp	r0, #0
	bge.n	.L_0815c074
	adds	r0, #63
.L_0815c074:
	ldr	r3, [pc, #152]
	ldr	r2, [r6, #0]
	asrs	r0, r0, #6
	mov	sl, r0
	adds	r0, r2, r3
	mov	r8, r0
	ldr	r0, [r6, #4]
	subs	r5, #59
	adds	r7, r0, r3
	cmp	r5, #0
	bge.n	.L_0815c08c
	movs	r5, #0
.L_0815c08c:
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #12]
	adds	r3, r0, r3
	str	r3, [r6, #4]
	ldr	r3, [pc, #124]
	adds	r2, r2, r1
	str	r2, [r6, #0]
	cmp	r2, r3
	bgt.n	.L_0815c0a6
	movs	r4, #128
	lsls	r4, r4, #8
	adds	r3, r1, r4
	str	r3, [r6, #12]
.L_0815c0a6:
	ldr	r3, [r6, #4]
	ldr	r0, [pc, #108]
	cmp	r3, r0
	bgt.n	.L_0815c0b8
	ldr	r3, [r6, #16]
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r3, r3, r1
	b.n	.L_0815c0be
.L_0815c0b8:
	ldr	r3, [r6, #16]
	ldr	r2, [pc, #96]
	adds	r3, r3, r2
.L_0815c0be:
	str	r3, [r6, #16]
	ldr	r3, [r6, #8]
	cmp	r3, #0
	blt.n	.L_0815c120
	ldr	r0, [r6, #20]
	bl	sub_08002096
	lsls	r3, r0, #1
	ldr	r2, [r6, #8]
	adds	r3, r3, r0
	lsls	r3, r3, #8
	asrs	r3, r3, #16
	adds	r2, r2, r3
	b.n	.L_0815c132
	movs	r0, r0
	.4byte 0x000000c8
	.4byte 0x02010000
	.4byte 0x00000150
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x0819747a
	.4byte 0x08197467
	.4byte 0x08197473
	.4byte 0x0819746d
	.4byte 0x08198686
	.4byte 0x08198662
	.4byte 0x08198674
	.4byte 0xffc00000
	.4byte 0x003fffff
	.4byte 0x003bffff
	.2byte 0x8000
	.2byte 0xffff
.L_0815c120:
	.2byte 0x6970
	bl	sub_08002096
	lsls	r3, r0, #1
	ldr	r2, [r6, #8]
	adds	r3, r3, r0
	lsls	r3, r3, #8
	asrs	r3, r3, #16
	subs	r2, r2, r3
.L_0815c132:
	str	r2, [r6, #8]
	ldr	r3, [r6, #20]
	movs	r4, #128
	lsls	r4, r4, #3
	adds	r3, r3, r4
	str	r3, [r6, #20]
	ldr	r0, [sp, #56]
	cmp	r0, #0
	beq.n	.L_0815c180
	ldr	r1, [pc, #720]
	ldr	r2, [sp, #36]
	lsls	r3, r5, #12
	adds	r3, r3, r1
	str	r3, [r2, #4]
	bl	sub_08014de4
	adds	r1, r7, #0
	movs	r2, #0
	mov	r0, r8
	bl	sub_08015160
	movs	r0, #192
	lsls	r0, r0, #8
	bl	sub_08015024
	ldr	r0, [r6, #8]
	bl	sub_08015068
	mov	r0, sl
	bl	sub_0801521c
	ldr	r0, [pc, #680]
	ldr	r1, [sp, #48]
	movs	r2, #4
	bl	sub_08196958
	ldr	r0, [sp, #44]
	bl	sub_08196a7c
.L_0815c180:
	ldr	r5, [r6, #24]
.L_0815c182:
	cmp	r5, #64
	bne.n	.L_0815c26a
	movs	r4, #2
	ldrsh	r3, [r6, r4]
	ldr	r7, [sp, #20]
	movs	r1, #0
	str	r3, [r7, #0]
	mov	sl, r1
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	str	r3, [r7, #4]
	mov	r3, r9
	lsls	r2, r3, #4
	ldr	r4, [sp, #40]
	lsls	r3, r3, #7
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r5, r3, r4
.L_0815c1a6:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r7, r0, #0
	ands	r7, r3
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	mov	r8, r3
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #8
	str	r3, [r5, #24]
	adds	r0, r7, #0
	ldr	r3, [r6, #0]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	bl	sub_08002096
	mov	r3, r8
	muls	r3, r0
	cmp	r3, #0
	bge.n	.L_0815c1e4
	adds	r3, #3
.L_0815c1e4:
	asrs	r3, r3, #2
	str	r3, [r5, #12]
	adds	r0, r7, #0
	bl	sub_08002090
	mov	r3, r8
	muls	r3, r0
	cmp	r3, #0
	bge.n	.L_0815c1f8
	adds	r3, #3
.L_0815c1f8:
	movs	r7, #1
	add	sl, r7
	asrs	r3, r3, #2
	mov	r0, sl
	str	r3, [r5, #16]
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_0815c1a6
	ldr	r2, [sp, #100]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L_0815c26a
	movs	r7, #36
.L_0815c216:
	bl	sub_08014878
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #9
	lsls	r5, r5, #2
	adds	r5, r5, r3
	bl	sub_08014878
	ldr	r4, [sp, #32]
	movs	r3, #31
	ldr	r1, [sp, #100]
	ands	r3, r0
	adds	r3, r4, r3
	adds	r3, #52
	ldrsh	r0, [r7, r1]
	str	r3, [sp, #4]
	movs	r2, #128
	movs	r3, #128
	movs	r1, #1
	lsls	r2, r2, #10
	lsls	r3, r3, #12
	str	r5, [sp, #0]
	bl	sub_0815f000
	ldr	r3, [sp, #100]
	movs	r2, #1
	ldrsh	r0, [r7, r3]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, sl
	negs	r2, r2
	bl	sub_0814cd48
	ldr	r1, [sp, #100]
	movs	r0, #1
	ldr	r3, [r1, #20]
	add	sl, r0
	adds	r7, #2
	cmp	sl, r3
	bne.n	.L_0815c216
.L_0815c26a:
	ldr	r2, [sp, #56]
	cmp	r2, #0
	beq.n	sub_0815c2b0
	ldr	r3, [r6, #24]
	subs	r3, #64
	cmp	r3, #11
	bhi.n	sub_0815c2b0
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	movs	r1, #6
	asrs	r0, r0, #1
	bl	sub_08002064
	lsls	r1, r0, #4
	ldr	r4, [sp, #20]
	subs	r1, r1, r0
	movs	r3, #148
	lsls	r1, r1, #7
	lsls	r3, r3, #6
	adds	r3, #151
	add	r1, fp
	ldr	r2, [r4, #0]
	movs	r0, #40
	adds	r1, r1, r3
	ldr	r7, [sp, #72]
	ldr	r3, [r4, #4]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	subs	r2, #20
	subs	r3, #24
	ldr	r4, [r7, #4]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9805
	movs	r1, #1
	add	r9, r1
	adds	r0, #28
	mov	r2, r9
	str	r0, [sp, #20]
	adds	r6, #28
	cmp	r2, #6
	beq.n	.L_0815c2c4
	b.n	.L_0815bf90
.L_0815c2c4:
	ldr	r6, [pc, #344]
	ldr	r5, [sp, #40]
	movs	r3, #0
	mov	r9, r3
.L_0815c2cc:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0815c312
	ldr	r7, [sp, #56]
	asrs	r3, r3, #2
	adds	r4, r3, #2
	cmp	r7, #0
	beq.n	sub_0815c300
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #84]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #72]
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #6
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0815c312:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r5, #28
	cmp	r2, #96
	bne.n	.L_0815c2cc
	ldr	r0, [sp, #44]
	bl	sub_08013164
	ldr	r0, [sp, #48]
	bl	sub_08013164
.L_0815c32a:
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L_0815c332
	b.n	.L_0815c4a6
.L_0815c332:
	ldr	r4, [sp, #88]
	cmp	r4, #131
	ble.n	.L_0815c33a
	b.n	.L_0815c4a6
.L_0815c33a:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #28]
	movs	r0, #1
	bl	sub_081969f8
	movs	r2, #239
	adds	r6, r0, #0
	movs	r3, #0
	lsls	r2, r2, #7
	ldr	r7, [sp, #88]
	add	r2, fp
	str	r3, [r6, #20]
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r3, [sp, #112]
	ldr	r2, [pc, #184]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #184]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #112]
	movs	r3, #184
	lsls	r3, r3, #6
	adds	r3, #129
	add	r2, sp, #112
	add	r3, fp
	str	r3, [r2, #4]
	ldr	r3, [pc, #160]
	str	r2, [r6, #16]
	str	r3, [r6, #8]
	str	r1, [r6, #0]
	ldr	r1, [sp, #28]
	ldr	r3, [pc, #156]
	movs	r2, #0
	str	r1, [r6, #12]
	str	r2, [sp, #24]
	lsls	r2, r7, #9
	adds	r3, r3, r2
	lsls	r0, r7, #1
	ldr	r4, [pc, #144]
	adds	r0, r0, r7
	mov	r9, r3
	ldr	r1, [pc, #144]
	lsls	r3, r7, #11
	ldr	r5, [sp, #88]
	movs	r7, #152
	negs	r2, r2
	lsls	r7, r7, #8
	adds	r7, r7, r2
	adds	r4, r4, r3
	lsls	r0, r0, #10
	mov	r8, r7
	mov	sl, r4
	adds	r7, r0, r1
	subs	r5, #76
.L_0815c3c0:
	cmp	r5, #0
	blt.n	.L_0815c47c
	bl	sub_08014de4
	movs	r0, #128
	movs	r2, #0
	lsls	r0, r0, #12
	movs	r1, #0
	bl	sub_08015160
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_0815c404
	ldr	r3, [pc, #88]
	adds	r0, r7, r3
	cmp	r5, #39
	bgt.n	.L_0815c3f8
	movs	r4, #192
	lsls	r0, r5, #11
	lsls	r4, r4, #8
	cmp	r0, r4
	ble.n	.L_0815c3f8
	movs	r0, #192
	lsls	r0, r0, #8
.L_0815c3f8:
	bl	sub_0801521c
	mov	r0, r8
	bl	sub_08015068
	b.n	.L_0815c46c
.L_0815c404:
	cmp	r5, #23
	ble.n	.L_0815c440
	movs	r3, #224
	lsls	r3, r3, #9
	mov	r1, sl
	subs	r0, r3, r1
	bl	sub_0801521c
	b.n	.L_0815c466
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x08199210
	.4byte 0x08197424
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0xffff6800
	.4byte 0xfffda000
	.4byte 0xfffc7000
	.2byte 0xe000
	.2byte 0xfffe
.L_0815c440:
	.2byte 0x2380
	lsls	r3, r3, #2
	adds	r3, #170
	muls	r3, r5
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #240
	subs	r0, r0, r3
	bl	sub_08015024
	movs	r3, #169
	adds	r0, r5, #0
	muls	r0, r3
	movs	r2, #240
	lsls	r2, r2, #8
	adds	r2, #40
	adds	r0, r0, r2
	bl	sub_080150e4
.L_0815c466:
	mov	r0, r9
	bl	sub_08015068
.L_0815c46c:
	ldr	r0, [pc, #256]
	ldr	r1, [sp, #28]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0815c47c:
	ldr	r2, [sp, #24]
	ldr	r3, [pc, #244]
	ldr	r4, [pc, #244]
	ldr	r1, [pc, #248]
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r2, #1
	add	r9, r3
	add	sl, r4
	add	r8, r0
	adds	r7, r7, r1
	subs	r5, #16
	str	r2, [sp, #24]
	cmp	r2, #2
	bne.n	.L_0815c3c0
	adds	r0, r6, #0
	bl	sub_08013164
	ldr	r0, [sp, #28]
	bl	sub_08013164
.L_0815c4a6:
	ldr	r3, [sp, #52]
	cmp	r3, #179
	bhi.n	.L_0815c538
	ldr	r3, [pc, #208]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #104]
	str	r4, [sp, #108]
	ldr	r4, [sp, #88]
	cmp	r4, #131
	ble.n	.L_0815c4e2
	adds	r5, r4, #0
	subs	r5, #132
	lsls	r0, r5, #11
	bl	sub_08002096
	movs	r7, #200
	lsls	r0, r0, #1
	lsls	r7, r7, #16
	lsls	r5, r5, #10
	adds	r7, r0, r7
	adds	r0, r5, #0
	str	r7, [sp, #64]
	bl	sub_08002096
	movs	r1, #168
	lsls	r0, r0, #1
	lsls	r1, r1, #15
	adds	r1, r0, r1
	str	r1, [sp, #68]
.L_0815c4e2:
	ldr	r2, [sp, #56]
	cmp	r2, #0
	beq.n	.L_0815c538
	add	r2, sp, #136
	movs	r3, #0
	movs	r5, #238
	lsls	r5, r5, #7
	str	r3, [r2, #12]
	str	r3, [r2, #4]
	ldr	r7, [pc, #140]
	adds	r5, #220
	mov	r9, r3
	add	r4, sp, #104
	adds	r6, r2, #0
	add	r5, fp
.L_0815c500:
	ldr	r3, [pc, #132]
	mov	r0, r9
	ldrb	r3, [r3, r0]
	ldr	r1, [sp, #64]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r3, r3, r7
	str	r3, [r6, #0]
	ldr	r3, [pc, #120]
	ldr	r2, [sp, #68]
	ldrb	r3, [r3, r0]
	adds	r1, r6, #0
	lsls	r3, r3, #16
	adds	r3, r3, r2
	adds	r3, r3, r7
	str	r3, [r6, #8]
	ldmia	r5!, {r0}
	adds	r2, r4, #0
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	ldr	r4, [sp, #8]
	cmp	r0, #7
	bne.n	.L_0815c500
.L_0815c538:
	ldr	r1, [sp, #88]
	movs	r2, #10
	adds	r2, #255
	cmp	r1, r2
	ble.n	.L_0815c62e
	movs	r3, #133
	lsls	r3, r3, #1
	cmp	r1, r3
	bne.n	.L_0815c5c8
	ldr	r3, [pc, #68]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	str	r3, [r2, #0]
	ldr	r2, [pc, #20]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #32
	strh	r2, [r3, #0]
	adds	r3, #6
	strh	r2, [r3, #0]
	movs	r4, #0
	mov	r9, r4
	mov	r5, fp
	b.n	.L_0815c594
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x08199210
	.4byte 0xffffe000
	.4byte 0xffff8000
	.4byte 0xffff4000
	.4byte 0x08196e54
	.4byte 0xffe00000
	.4byte 0x08198654
	.4byte 0x0819865b
	.2byte 0x9000
	.2byte 0xffff
.L_0815c594:
	.2byte 0xf6b8
	.2byte 0xf970
	.2byte 0x2130
	bl	sub_0800206c
	adds	r0, #40
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r1, #96
	bl	sub_0800206c
	mov	r7, r9
	adds	r0, #16
	negs	r3, r7
	str	r0, [r5, #4]
	lsrs	r2, r3, #31
	movs	r0, #1
	adds	r3, r3, r2
	add	r9, r0
	asrs	r3, r3, #1
	mov	r1, r9
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L_0815c594
.L_0815c5c8:
	movs	r2, #0
	mov	r9, r2
	mov	r6, fp
.L_0815c5ce:
	ldr	r3, [r6, #24]
	cmp	r3, #23
	bhi.n	.L_0815c61e
	ldr	r4, [sp, #56]
	cmp	r4, #0
	beq.n	.L_0815c61e
	adds	r0, r3, #0
	cmp	r3, #0
	bge.n	.L_0815c5e2
	adds	r0, r3, #3
.L_0815c5e2:
	movs	r1, #6
	asrs	r0, r0, #2
	bl	sub_08002064
	ldr	r2, [pc, #276]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #272]
	ldr	r2, [r6, #0]
	ldrb	r5, [r3, r0]
	movs	r7, #224
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #264]
	add	r1, fp
	ldrb	r4, [r3, r0]
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r4
	ldr	r4, [pc, #256]
	lsls	r7, r7, #3
	ldrb	r0, [r4, r0]
	subs	r3, #48
	str	r0, [sp, #4]
	adds	r1, r1, r7
	ldr	r4, [sp, #128]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
.L_0815c61e:
	movs	r0, #1
	add	r9, r0
	adds	r3, #1
	mov	r1, r9
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r1, #32
	bne.n	.L_0815c5ce
.L_0815c62e:
	ldr	r2, [sp, #88]
	movs	r3, #144
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_0815c67e
	movs	r1, #144
	ldr	r0, [pc, #212]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #228
	movs	r3, #188
	add	r2, fp
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r2, #221
	lsls	r2, r2, #3
	movs	r3, #192
	add	r2, fp
	lsls	r3, r3, #15
	str	r3, [r2, #0]
	movs	r3, #222
	lsls	r3, r3, #3
	movs	r2, #0
	add	r3, fp
	str	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #244
	add	r3, fp
	str	r2, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #140
	add	r3, fp
	movs	r4, #1
	str	r2, [r3, #0]
	str	r4, [sp, #80]
.L_0815c67e:
	ldr	r7, [sp, #56]
	cmp	r7, #0
	beq.n	.L_0815c69a
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
.L_0815c69a:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #88]
	movs	r3, #163
	adds	r0, #2
	adds	r1, #1
	adds	r2, #1
	lsls	r3, r3, #1
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #88]
	cmp	r2, r3
	beq.n	.L_0815c6b8
	bl	.L_0815b9f2
.L_0815c6b8:
	movs	r0, #134
	bl	sub_081180e8
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r0, [r6, #96]
	ldr	r5, [pc, #60]
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x480d
	movs	r1, #240
	lsls	r1, r1, #6
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x480b
	movs	r1, #240
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2001
	b.n	.L_0815c720
	.4byte 0x00001f83
	.4byte 0x0819747a
	.4byte 0x08197467
	.4byte 0x08197473
	.4byte 0x0819746d
	.4byte 0x0815b68d
	.4byte 0x03000260
	.4byte 0x06004000
	.2byte 0x8000
	.2byte 0x0600
.L_0815c720:
	bl	sub_08013560
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r4, #128
	adds	r2, #82
	lsls	r4, r4, #19
	adds	r4, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	mov	r9, r4
	mov	r7, r9
	movs	r0, #128
	strh	r3, [r7, #0]
	lsls	r0, r0, #19
	ldr	r3, [pc, #36]
	mov	r8, r0
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r2, [sp, #92]
	b.n	.L_0815c770
	movs	r0, r0
	.4byte 0x0000100e
	.4byte 0x00003f46
	.4byte 0x00007741
	.2byte 0x3001
	.2byte 0x0814
.L_0815c770:
	movs	r7, #0
	movs	r0, #195
	str	r7, [r2, #16]
	lsls	r0, r0, #1
	ldr	r6, [r6, #36]
	bl	sub_081c0010
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #160
	add	r3, fp
	ldr	r5, [pc, #120]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #120]
	strh	r3, [r5, #4]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #164
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r0, #160
	strh	r3, [r5, #6]
	lsls	r0, r0, #19
	movs	r3, #120
	movs	r1, #128
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	mov	sl, r6
	lsls	r1, r1, #1
	ldr	r6, [pc, #92]
	adds	r0, #192
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2001
	bl	sub_08013560
	movs	r3, #206
	lsls	r3, r3, #3
	add	r3, sl
	movs	r2, #1
	ldrh	r1, [r3, #0]
	negs	r2, r2
	movs	r0, #2
	bl	sub_08118040
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	movs	r1, #240
	strh	r3, [r2, #0]
	lsls	r1, r1, #6
	ldr	r0, [pc, #48]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x480c
	bl	sub_08014644
	ldr	r1, [pc, #44]
	movs	r3, #32
	strh	r3, [r5, #6]
	ldr	r0, [pc, #44]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0815c83c
	b.n	.L_0815c81c
	movs	r0, r0
	.4byte 0x00000787
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x08143489
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_0815c81c:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	strh	r2, [r1, #0]
	lsls	r3, r3, #2
	movs	r2, #230
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	mov	r1, r8
	stmia	r3!, {r1}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0815c83c:
	strh	r4, [r0, #0]
	mov	r2, r9
	strh	r7, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #0
	ldr	r4, [pc, #24]
	str	r3, [sp, #88]
	mov	r8, r4
.L_0815c850:
	movs	r6, #160
	lsls	r6, r6, #3
	movs	r7, #160
	adds	r6, #108
	lsls	r7, r7, #19
	movs	r0, #0
	add	r6, sl
	adds	r7, #192
	mov	r9, r0
	b.n	.L_0815c868
	.2byte 0x001f
	.2byte 0x0000
.L_0815c868:
	ldrh	r3, [r6, #0]
	movs	r5, #31
	ands	r5, r3
	lsls	r3, r3, #16
	mov	r2, r8
	lsrs	r1, r3, #21
	lsrs	r0, r3, #26
	ldr	r4, [sp, #88]
	ands	r1, r2
	ands	r0, r2
	ldr	r2, [sp, #88]
	adds	r3, r5, r4
	adds	r4, r3, #0
	adds	r3, r1, r2
	adds	r2, r3, #0
	ldr	r3, [sp, #88]
	subs	r4, #30
	adds	r3, r3, r0
	subs	r2, #40
	subs	r3, #30
	cmp	r4, #0
	bge.n	.L_0815c898
	movs	r4, #0
	b.n	.L_0815c89e
.L_0815c898:
	cmp	r4, r5
	ble.n	.L_0815c89e
	adds	r4, r5, #0
.L_0815c89e:
	cmp	r2, #0
	bge.n	.L_0815c8a6
	movs	r2, #0
	b.n	.L_0815c8ac
.L_0815c8a6:
	cmp	r2, r1
	ble.n	.L_0815c8ac
	adds	r2, r1, #0
.L_0815c8ac:
	cmp	r3, #0
	bge.n	.L_0815c8b4
	movs	r3, #0
	b.n	.L_0815c8ba
.L_0815c8b4:
	cmp	r3, r0
	ble.n	.L_0815c8ba
	adds	r3, r0, #0
.L_0815c8ba:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r4
	movs	r4, #1
	add	r9, r4
	mov	r0, r9
	strh	r3, [r7, #0]
	adds	r6, #2
	adds	r7, #2
	cmp	r0, #128
	bne.n	.L_0815c868
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #88]
	adds	r1, #1
	str	r1, [sp, #88]
	cmp	r1, #40
	bne.n	.L_0815c850
	ldr	r1, [pc, #104]
	ldr	r0, [pc, #104]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0815c914
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #234
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0815c914:
	strh	r4, [r0, #0]
	ldr	r2, [sp, #80]
	cmp	r2, #1
	bne.n	.L_0815c922
	ldr	r0, [pc, #52]
	bl	sub_08014644
.L_0815c922:
	movs	r5, #238
	lsls	r5, r5, #7
	movs	r3, #0
	adds	r5, #220
	mov	r9, r3
	add	r5, fp
.L_0815c92e:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r4, #1
	add	r9, r4
	mov	r7, r9
	cmp	r7, #8
	bne.n	.L_0815c92e
	add	sp, #152
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0xb68d
	.2byte 0x0815
	push	{lr}
	movs	r1, #0
	bl	sub_0815c970
	pop	{pc}
