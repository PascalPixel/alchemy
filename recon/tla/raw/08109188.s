.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0800205c, 0x0800205c
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014d00, 0x08014d00
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020030, 0x08020030
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_08038060, 0x08038060
	.set sub_08038070, 0x08038070
	.set sub_08038078, 0x08038078
	.set sub_08038080, 0x08038080
	.set sub_080380b0, 0x080380b0
	.set sub_080380c8, 0x080380c8
	.set sub_08038120, 0x08038120
	.set sub_08038140, 0x08038140
	.set sub_08038260, 0x08038260
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad018, 0x080ad018
	.set sub_080ad020, 0x080ad020
	.set sub_080ad030, 0x080ad030
	.set sub_080ad048, 0x080ad048
	.set sub_080ad1c0, 0x080ad1c0
	.set sub_080ad1c8, 0x080ad1c8
	.set sub_080ad1d0, 0x080ad1d0
	.set sub_080ad1d8, 0x080ad1d8
	.set sub_080ad1e8, 0x080ad1e8
	.set sub_080ad1f0, 0x080ad1f0
	.set sub_080ad258, 0x080ad258
	.set sub_080ad278, 0x080ad278
	.set sub_080ad288, 0x080ad288
	.set sub_080f8028, 0x080f8028
	.set sub_080f8038, 0x080f8038
	.set sub_080f8058, 0x080f8058
	.set sub_080f8060, 0x080f8060
	.set sub_080f8068, 0x080f8068
	.set sub_081084f4, 0x081084f4
	.set sub_0810857c, 0x0810857c
	.set sub_08108630, 0x08108630
	.set sub_08108690, 0x08108690
	.set sub_0810875c, 0x0810875c
	.set sub_08108af0, 0x08108af0
	.set sub_0810a2d0, 0x0810a2d0
	.set sub_0810a2d8, 0x0810a2d8
	.set sub_0810a748, 0x0810a748
	.set sub_0810b6c4, 0x0810b6c4
	.set sub_0810b6cc, 0x0810b6cc
	.set sub_0810bea8, 0x0810bea8
	.set sub_0810bf98, 0x0810bf98
	.set sub_081c0010, 0x081c0010
	.global Func_08109188
	.thumb_func
Func_08109188:
.L_08109188:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	ldr	r5, [r3, #12]
	cmp	r5, #0
	beq.n	.L_081091be
	adds	r0, r5, #0
	bl	sub_08038060
	ldr	r0, [pc, #32]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	ldr	r3, [pc, #24]
	movs	r1, #6
	ldr	r0, [r3, #16]
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r2, r5, #0
	movs	r3, #32
	bl	sub_080380b0
.L_081091be:
	add	sp, #4
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x00001237
	.2byte 0x0240
	.2byte 0x0200
.L_081091cc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	sub	sp, #4
	mov	r8, r1
	adds	r7, r2, #0
	adds	r5, r3, #0
	cmp	r6, #0
	bne.n	.L_08109220
	b.n	.L_08109252
.L_081091e2:
	ldr	r0, [pc, #120]
	adds	r1, r6, #0
	movs	r2, #0
	b.n	.L_08109218
.L_081091ea:
	ldr	r0, [pc, #116]
	adds	r1, r6, #0
	movs	r2, #0
	b.n	.L_08109218
.L_081091f2:
	ldr	r5, [pc, #112]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #5
	adds	r2, r6, #0
	movs	r3, #32
	subs	r5, #3
	bl	sub_080380b0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #72
.L_08109218:
	movs	r3, #8
	bl	sub_08038080
	b.n	.L_08109252
.L_08109220:
	adds	r0, r6, #0
	bl	sub_08038260
	ldr	r0, [pc, #64]
	adds	r1, r6, #0
	add	r0, r8
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	cmp	r7, #0
	bne.n	.L_08109240
	cmp	r5, #1
	beq.n	.L_081091e2
	cmp	r5, #2
	beq.n	.L_081091ea
.L_08109240:
	cmp	r5, #3
	bne.n	.L_081091f2
	ldr	r0, [pc, #36]
	adds	r1, r6, #0
	adds	r0, r7, r0
	movs	r2, #8
	movs	r3, #8
	bl	sub_08038080
.L_08109252:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000123f
	.4byte 0x00001240
	.4byte 0x00001238
	.4byte 0x0000025f
	.2byte 0x1249
	.2byte 0x0000
.L_08109270:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	cmp	r5, #0
	beq.n	.L_0810928a
	bl	sub_08038260
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_0810928a:
	pop	{r5, r6, pc}
.L_0810928c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r2, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	r9, r1
	mov	sl, r3
	mov	r3, r9
	adds	r6, r0, #0
	cmp	r3, #0
	bge.n	.L_081092b4
	adds	r3, #3
.L_081092b4:
	asrs	r3, r3, #2
	str	r3, [sp, #12]
	lsls	r3, r3, #2
	mov	fp, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #4
	add	r3, sl
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	cmp	r6, #0
	beq.n	.L_08109396
	adds	r0, r6, #0
	bl	sub_08038060
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_081092fe
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #238
	add	r3, sl
	ldrh	r0, [r3, #0]
	movs	r3, #12
	negs	r3, r3
	movs	r1, #128
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #88
	lsls	r1, r1, #23
	bl	sub_080380c8
	movs	r2, #0
	movs	r3, #17
	strb	r2, [r0, #4]
	strb	r3, [r0, #5]
	strh	r2, [r0, #12]
.L_081092fe:
	mov	r3, fp
	adds	r3, #4
	cmp	r3, r5
	bge.n	.L_08109326
	movs	r3, #158
	lsls	r3, r3, #3
	add	r3, sl
	movs	r1, #128
	ldrh	r0, [r3, #0]
	movs	r5, #0
	movs	r3, #88
	lsls	r1, r1, #23
	adds	r2, r6, #0
	str	r5, [sp, #0]
	bl	sub_080380c8
	movs	r3, #15
	strb	r5, [r0, #4]
	strb	r3, [r0, #5]
	strh	r5, [r0, #12]
.L_08109326:
	movs	r2, #0
	mov	r8, r2
	mov	r1, sl
	ldr	r2, [sp, #12]
	adds	r1, #248
	mov	r3, sl
	adds	r3, #2
	str	r1, [sp, #4]
	movs	r1, #153
	str	r3, [sp, #8]
	movs	r7, #156
	lsls	r3, r2, #3
	lsls	r1, r1, #3
	lsls	r7, r7, #1
	adds	r6, r3, r1
.L_08109344:
	ldr	r2, [sp, #8]
	mov	r3, fp
	ldrsh	r5, [r2, r6]
	ldr	r1, [sp, #4]
	add	r3, r8
	ldmia	r1!, {r0}
	adds	r2, r1, #0
	str	r2, [sp, #4]
	cmp	r0, #0
	beq.n	.L_08109388
	cmp	r3, r9
	bne.n	.L_08109364
	movs	r1, #30
	bl	sub_08020030
	b.n	.L_0810936a
.L_08109364:
	movs	r1, #1
	bl	sub_08020030
.L_0810936a:
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r2, sl
	str	r3, [r7, r2]
	adds	r0, r5, #0
	ldr	r1, [sp, #16]
	bl	sub_080ad1c8
	cmp	r0, #0
	bne.n	.L_08109388
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	mov	r1, sl
	str	r3, [r7, r1]
.L_08109388:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #4
	adds	r6, #2
	cmp	r3, #3
	ble.n	.L_08109344
.L_08109396:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	sl, r0
	adds	r0, r1, #0
	mov	r9, r2
	str	r3, [sp, #16]
	bl	sub_08016ca4
	adds	r7, r0, #0
	mov	r0, r9
	bl	sub_080ad010
	adds	r5, r0, #0
	movs	r0, #1
	negs	r0, r0
	mov	r1, sl
	str	r0, [sp, #12]
	cmp	r1, #0
	bne.n	.L_081093e2
	b.n	.L_0810959a
.L_081093e2:
	mov	r0, sl
	bl	sub_08038060
	mov	r1, r9
	ldr	r0, [sp, #20]
	bl	sub_080ad1c0
	cmp	r0, #0
	bne.n	.L_08109402
	ldr	r0, [pc, #108]
	mov	r1, sl
	movs	r2, #8
	movs	r3, #24
	bl	sub_08038078
	b.n	.L_0810959a
.L_08109402:
	ldrb	r1, [r5, #2]
	ldr	r0, [sp, #20]
	bl	sub_080ad1d0
	ldr	r2, [sp, #12]
	cmp	r0, r2
	bne.n	.L_08109468
	movs	r3, #216
	ldrh	r2, [r7, r3]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r3, r1, #0
	ands	r3, r2
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0810943a
	mov	ip, r1
	adds	r1, r7, #0
	adds	r1, #216
.L_08109428:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L_0810943a
	adds	r1, #2
	ldrh	r2, [r1, #0]
	mov	r3, ip
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08109428
.L_0810943a:
	cmp	r5, #15
	bne.n	.L_08109460
	adds	r6, r7, #0
	movs	r5, #0
	adds	r6, #216
	b.n	.L_08109448
.L_08109446:
	adds	r5, #1
.L_08109448:
	cmp	r5, #14
	bgt.n	.L_0810945a
	ldrh	r0, [r6, #0]
	bl	sub_080ad010
	ldrb	r3, [r0, #2]
	adds	r6, #2
	cmp	r3, #6
	bne.n	.L_08109446
.L_0810945a:
	cmp	r5, #15
	bne.n	.L_08109460
	movs	r5, #0
.L_08109460:
	lsls	r0, r5, #1
	b.n	.L_0810947a
	.2byte 0x123b
	.2byte 0x0000
.L_08109468:
	lsls	r0, r0, #1
	adds	r3, r0, #0
	adds	r3, #216
	ldrh	r3, [r7, r3]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r1, #255
	ands	r1, r3
	str	r1, [sp, #12]
.L_0810947a:
	ldr	r3, [pc, #52]
	adds	r5, r0, #0
	mov	r0, r9
	orrs	r0, r3
	adds	r5, #216
	mov	r9, r0
	ldrh	r2, [r7, r5]
	mov	r1, r9
	strh	r1, [r7, r5]
	ldr	r0, [sp, #20]
	mov	r8, r2
	bl	sub_080ad008
	ldrh	r3, [r7, #60]
	add	r2, sp, #24
	str	r3, [r2, #0]
	adds	r6, r7, #0
	ldrh	r3, [r7, #62]
	adds	r6, #64
	str	r3, [r2, #4]
	mov	r0, r8
	ldrh	r3, [r6, #0]
	mov	fp, r2
	str	r3, [r2, #8]
	adds	r3, r7, #0
	b.n	.L_081094b4
	movs	r0, r0
	.2byte 0x0200
	.2byte 0x0000
.L_081094b4:
	adds	r3, #66
	str	r3, [sp, #8]
	ldrb	r3, [r3, #0]
	str	r3, [r2, #12]
	strh	r0, [r7, r5]
	ldr	r0, [sp, #20]
	bl	sub_080ad008
	ldrh	r3, [r7, #60]
	add	r1, sp, #40
	str	r3, [r1, #0]
	movs	r5, #0
	ldrh	r3, [r7, #62]
	mov	r9, r1
	str	r3, [r1, #4]
	mov	r8, r5
	ldrh	r3, [r6, #0]
	movs	r7, #0
	str	r3, [r1, #8]
	ldr	r2, [sp, #8]
	ldrb	r3, [r2, #0]
	str	r3, [r1, #12]
	movs	r3, #2
	str	r3, [sp, #4]
.L_081094e4:
	mov	r0, r8
	mov	r1, r9
	ldr	r2, [r0, r1]
	mov	r1, fp
	ldr	r3, [r0, r1]
	cmp	r2, r3
	ble.n	.L_081094fe
	ldr	r2, [sp, #16]
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #246
	adds	r3, r2, r0
	b.n	.L_0810950c
.L_081094fe:
	cmp	r2, r3
	bge.n	.L_08109526
	ldr	r1, [sp, #16]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #244
	adds	r3, r1, r2
.L_0810950c:
	ldrh	r0, [r3, #0]
	movs	r1, #128
	subs	r3, r7, #4
	str	r3, [sp, #0]
	lsls	r1, r1, #23
	movs	r3, #56
	mov	r2, sl
	bl	sub_080380c8
	movs	r3, #0
	adds	r6, r7, #0
	strb	r3, [r0, #4]
	b.n	.L_08109528
.L_08109526:
	lsls	r6, r5, #4
.L_08109528:
	add	r1, sp, #40
	mov	r3, r8
	ldr	r0, [r3, r1]
	mov	r2, sl
	movs	r1, #3
	movs	r3, #32
	str	r7, [sp, #0]
	bl	sub_080380b0
	mov	r2, r8
	add	r0, sp, #40
	mov	r1, fp
	ldr	r3, [r2, r0]
	ldr	r0, [r2, r1]
	cmp	r3, r0
	beq.n	.L_08109554
	movs	r1, #3
	mov	r2, sl
	movs	r3, #72
	str	r6, [sp, #0]
	bl	sub_080380b0
.L_08109554:
	ldr	r0, [pc, #80]
	mov	r1, sl
	adds	r0, r5, r0
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_08038080
	ldr	r2, [sp, #4]
	mov	r0, sl
	movs	r3, #13
	movs	r1, #0
	str	r2, [sp, #0]
	bl	sub_08038070
	ldr	r3, [sp, #4]
	movs	r0, #4
	adds	r3, #2
	adds	r5, #1
	str	r3, [sp, #4]
	add	r8, r0
	adds	r7, #16
	cmp	r5, #2
	ble.n	.L_081094e4
	ldr	r1, [sp, #12]
	movs	r2, #1
	negs	r2, r2
	cmp	r1, r2
	beq.n	.L_0810959a
	ldr	r0, [pc, #28]
	movs	r2, #0
	adds	r0, r1, r0
	movs	r3, #48
	mov	r1, sl
	bl	sub_08038080
.L_0810959a:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001245
	.2byte 0x025f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	adds	r5, r0, #0
	adds	r0, r7, #0
	adds	r6, r2, #0
	bl	sub_08016ca4
	mov	r8, r0
	cmp	r5, #0
	beq.n	.L_08109616
	adds	r0, r5, #0
	bl	sub_08038060
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_080ad030
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_081095fe
	lsls	r3, r0, #1
	adds	r3, #216
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r1, #5
	lsrs	r0, r0, #11
	adds	r0, #1
	bl	sub_08038120
	ldr	r0, [pc, #40]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	b.n	.L_0810960a
.L_081095fe:
	ldr	r0, [pc, #32]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_0810960a:
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	adds	r3, r7, #0
	bl	sub_0810bf98
.L_08109616:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0000123d
	.2byte 0x123c
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r6, r1, #0
	mov	sl, r3
	adds	r5, r0, #0
	bl	sub_08016ca4
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080ad010
	mov	r8, r0
	mov	r3, r8
	ldrb	r2, [r3, #3]
	movs	r3, #16
	ands	r3, r2
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_081096e6
	ldr	r0, [pc, #152]
	bl	sub_081084f4
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080ad030
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_08109678
	lsls	r3, r0, #1
	adds	r3, #216
	ldrh	r3, [r7, r3]
	lsrs	r3, r3, #11
	adds	r7, r3, #1
	b.n	.L_0810967a
.L_08109678:
	movs	r7, #0
.L_0810967a:
	mov	r2, r8
	ldrh	r3, [r2, #0]
	movs	r5, #30
	cmp	r3, #0
	beq.n	.L_08109690
	ldr	r3, [pc, #108]
	ldrh	r1, [r2, #0]
	ldr	r0, [r3, #16]
	bl	sub_0800205c
	adds	r5, r0, #0
.L_08109690:
	movs	r3, #129
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_081096bc
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_080ad1e8
	cmp	r5, r0
	ble.n	.L_081096b8
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_080ad1e8
	b.n	.L_081096ba
.L_081096b8:
	adds	r0, r5, #0
.L_081096ba:
	adds	r5, r0, #0
.L_081096bc:
	adds	r5, r5, r7
	cmp	r5, #30
	ble.n	.L_081096c4
	movs	r5, #30
.L_081096c4:
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #5
	add	r3, sl
	movs	r2, #12
	strb	r2, [r3, #0]
	movs	r0, #0
	movs	r1, #128
	movs	r2, #48
	bl	sub_08108af0
	mov	r3, r8
	ldrh	r2, [r3, #0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	.L_081096f8
.L_081096e6:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00001251
	.2byte 0x0240
	.2byte 0x0200
.L_081096f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r1
	movs	r1, #128
	sub	sp, #16
	mov	fp, r0
	lsls	r1, r1, #3
	movs	r0, #56
	str	r2, [sp, #12]
	bl	sub_08014d00
	movs	r2, #1
	str	r2, [sp, #8]
	mov	r3, r9
	mov	r2, fp
	subs	r3, r3, r2
	mov	r9, r3
	movs	r3, #2
	str	r3, [sp, #0]
	mov	r8, r0
	movs	r1, #4
	movs	r0, #7
	movs	r2, #23
	movs	r3, #3
	bl	sub_08038010
	movs	r5, #1
	negs	r5, r5
	movs	r7, #0
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_08109744
	b.n	.L_0810989e
.L_08109744:
	bl	sub_080143ac
	str	r0, [sp, #4]
	cmp	r0, #96
	bne.n	.L_08109750
	b.n	.L_0810989e
.L_08109750:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	bl	sub_080142d4
	ldr	r5, [pc, #56]
	ldr	r0, [sp, #4]
	adds	r1, r5, #0
	mov	r2, sl
	movs	r3, #0
	str	r7, [sp, #0]
	bl	sub_080380c8
	adds	r1, r5, #0
	mov	r2, sl
	movs	r3, #32
	ldr	r0, [sp, #4]
	str	r7, [sp, #0]
	bl	sub_080380c8
	ldrh	r1, [r0, #24]
	ldr	r3, [pc, #20]
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	adds	r2, #4
	ands	r2, r3
	ldr	r3, [pc, #16]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #24]
	b.n	.L_08109864
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x40004000
	.2byte 0xfc00
	.2byte 0xffff
.L_0810979c:
	ldr	r2, [pc, #276]
	ldr	r3, [r2, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081097b4
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	str	r3, [sp, #8]
	subs	r7, #1
.L_081097b4:
	ldr	r2, [pc, #252]
	ldr	r3, [r2, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081097cc
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	str	r3, [sp, #8]
	adds	r7, #1
.L_081097cc:
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	.L_0810985e
	mov	r2, r9
	movs	r3, #0
	adds	r0, r7, r2
	mov	r1, r9
	str	r3, [sp, #8]
	bl	sub_08002064
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r7, r0, #0
	adds	r3, #212
	ldr	r0, [pc, #200]
	mov	r1, r8
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	movs	r0, #30
	movs	r1, #14
	bl	sub_0810875c
	mov	r0, fp
	add	r0, r9
	movs	r1, #0
	mov	r2, r8
	bl	sub_0810875c
	mov	r3, fp
	adds	r0, r3, r7
	adds	r0, #1
	movs	r1, #10
	mov	r2, r8
	bl	sub_0810875c
	mov	r0, fp
	movs	r1, #2
	mov	r2, r8
	bl	sub_0810875c
	movs	r1, #128
	ldr	r0, [sp, #4]
	lsls	r1, r1, #1
	mov	r2, r8
	bl	sub_080142d4
	adds	r5, r7, #1
	adds	r0, r5, #0
	movs	r1, #2
	mov	r2, sl
	movs	r3, #72
	str	r6, [sp, #0]
	bl	sub_080380b0
	ldr	r2, [sp, #12]
	movs	r1, #6
	adds	r0, r5, #0
	muls	r0, r2
	movs	r3, #88
	mov	r2, sl
	str	r6, [sp, #0]
	bl	sub_080380b0
	ldr	r0, [pc, #104]
	mov	r1, sl
	movs	r2, #136
	movs	r3, #0
	bl	sub_08038080
.L_0810985e:
	movs	r0, #1
	bl	sub_08013560
.L_08109864:
	ldr	r2, [pc, #76]
	ldr	r3, [r2, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810987a
	movs	r0, #112
	bl	sub_081c0010
	adds	r5, r7, #1
	b.n	.L_08109890
.L_0810987a:
	ldr	r3, [pc, #56]
	ldr	r6, [r3, #4]
	movs	r3, #2
	ands	r6, r3
	cmp	r6, #0
	beq.n	.L_0810979c
	movs	r0, #113
	bl	sub_081c0010
	movs	r5, #1
	negs	r5, r5
.L_08109890:
	movs	r0, #1
	bl	sub_08013560
	mov	r0, sl
	movs	r1, #2
	bl	sub_08038018
.L_0810989e:
	movs	r0, #56
	bl	sub_0801314c
	adds	r0, r5, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03001150
	.4byte 0x0810c248
	.2byte 0x1235
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	adds	r7, r0, #0
	mov	r0, r8
	adds	r5, r2, #0
	bl	sub_080ad010
	adds	r6, r0, #0
	movs	r3, #0
	ldrb	r1, [r6, #2]
	adds	r0, r7, #0
	mov	sl, r3
	bl	sub_080ad1d0
	mov	r9, r0
	movs	r0, #101
	bl	sub_081c0010
	cmp	sl, r5
	bge.n	.L_08109912
.L_081098f0:
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_080ad020
	mov	sl, r0
	ldrh	r0, [r6, #0]
	subs	r5, #1
	negs	r0, r0
	bl	sub_080ad1d8
	ldrh	r0, [r6, #0]
	bl	sub_080ad258
	bl	.L_08109188
	cmp	r5, #0
	bne.n	.L_081098f0
.L_08109912:
	ldr	r0, [pc, #36]
	bl	sub_0810857c
	adds	r0, r7, #0
	mov	r1, sl
	bl	.L_0810993c
	cmp	r0, #0
	beq.n	.L_0810992c
	adds	r0, r7, #0
	mov	r1, r9
	bl	.L_08109a3c
.L_0810992c:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1252
	.2byte 0x0000
.L_0810993c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	r9, r1
	mov	fp, r3
	mov	r8, r0
	bl	sub_08016ca4
	mov	r3, r9
	lsls	r5, r3, #1
	adds	r7, r0, #0
	adds	r5, #216
	ldrh	r3, [r7, r5]
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r6, r3
	adds	r0, r6, #0
	bl	sub_080ad010
	ldrh	r2, [r7, r5]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	mov	sl, r0
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08109a24
	mov	r0, r8
	adds	r1, r6, #0
	bl	sub_080ad1c0
	cmp	r0, #0
	beq.n	.L_081099d0
	mov	r3, sl
	ldrb	r1, [r3, #2]
	mov	r0, r8
	bl	sub_080ad1d0
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_081099b8
	lsls	r3, r0, #1
	adds	r3, #216
	ldrh	r0, [r7, r3]
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08109a24
.L_081099b8:
	mov	r0, r8
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #108]
	bl	sub_081084f4
	movs	r0, #0
	bl	sub_08108630
	cmp	r0, #0
	beq.n	.L_081099d4
.L_081099d0:
	movs	r0, #0
	b.n	.L_08109a24
.L_081099d4:
	mov	r0, r8
	mov	r1, r9
	bl	sub_080ad048
	mov	r3, fp
	ldr	r0, [r3, #36]
	cmp	r0, #0
	beq.n	.L_081099ea
	mov	r1, r8
	bl	.L_0810a004
.L_081099ea:
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08109a1c
	movs	r0, #103
	bl	sub_081c0010
	bl	sub_08038140
	movs	r1, #8
	movs	r2, #4
	movs	r3, #2
	ldr	r0, [pc, #44]
	bl	sub_08038038
	b.n	.L_08109a14
.L_08109a0e:
	movs	r0, #1
	bl	sub_08013560
.L_08109a14:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_08109a0e
.L_08109a1c:
	ldr	r0, [pc, #24]
	bl	sub_0810857c
	movs	r0, #1
.L_08109a24:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001253
	.4byte 0x00000fff
	.2byte 0x1254
	.2byte 0x0000
.L_08109a3c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	bl	sub_08016ca4
	movs	r3, #1
	negs	r3, r3
	mov	r8, r3
	cmp	r6, r8
	bne.n	.L_08109a58
	movs	r0, #0
	b.n	.L_08109a92
.L_08109a58:
	lsls	r3, r6, #1
	adds	r3, #216
	ldrh	r3, [r0, r3]
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_080ad010
	ldrb	r3, [r0, #2]
	movs	r0, #0
	cmp	r3, #6
	beq.n	.L_08109a92
	adds	r0, r5, #0
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08109a92
	adds	r0, r7, #0
	adds	r1, r6, #0
	mov	r2, r8
	bl	.L_0810a108
	movs	r0, #1
.L_08109a92:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_08109a98:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_080ad010
	ldrh	r5, [r0, #0]
	adds	r0, r6, #0
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08109ab6
	movs	r5, #0
	b.n	.L_08109ad4
.L_08109ab6:
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08109ac8
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_08109ad4
.L_08109ac8:
	lsls	r3, r5, #1
	adds	r0, r3, r5
	cmp	r0, #0
	bge.n	.L_08109ad2
	adds	r0, #3
.L_08109ad2:
	asrs	r5, r0, #2
.L_08109ad4:
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	sub	sp, #8
	ldr	r6, [r3, #0]
	movs	r0, #0
	movs	r5, #2
	movs	r1, #9
	movs	r2, #12
	movs	r3, #4
	str	r0, [sp, #4]
	str	r5, [sp, #0]
	mov	r8, r0
	bl	sub_08038010
	str	r0, [r6, #12]
	bl	.L_08109188
	movs	r1, #12
	movs	r2, #14
	movs	r3, #8
	movs	r0, #16
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r1, #14
	str	r0, [r6, #36]
	movs	r2, #13
	movs	r3, #3
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r1, #192
	lsls	r1, r1, #4
	movs	r2, #128
	adds	r1, #232
	lsls	r2, r2, #3
	adds	r3, r6, r1
	adds	r2, #220
	str	r0, [r3, #0]
	adds	r3, r6, r2
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #5
	adds	r2, r6, r3
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r1, #2
	movs	r2, #0
	mov	sl, r0
	bl	sub_080f8060
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #5
	adds	r0, r0, r6
	movs	r7, #1
	mov	r9, r0
.L_08109b5e:
	cmp	r7, #0
	beq.n	.L_08109bc4
	ldr	r4, [sp, #4]
	movs	r1, #153
	lsls	r1, r1, #3
	lsls	r2, r4, #1
	adds	r3, r6, #2
	adds	r2, r2, r1
	ldrsh	r0, [r3, r2]
	adds	r3, r4, #0
	mov	r8, r0
	cmp	r4, #0
	bge.n	.L_08109b7a
	adds	r3, r4, #3
.L_08109b7a:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r4, r3
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	movs	r2, #0
	subs	r1, #12
	mov	r0, sl
	bl	sub_08108af0
	movs	r3, #3
	mov	r2, r9
	strb	r3, [r2, #0]
	cmp	r7, #2
	bne.n	.L_08109bb0
	ldr	r0, [sp, #4]
	cmp	r0, #0
	bge.n	.L_08109ba2
	adds	r0, #3
.L_08109ba2:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_080f8058
	movs	r0, #1
	bl	sub_08013560
.L_08109bb0:
	ldr	r1, [sp, #4]
	mov	r0, sl
	movs	r2, #0
	bl	.L_0810928c
	ldr	r0, [r6, #36]
	mov	r1, r8
	bl	.L_0810a004
	movs	r7, #0
.L_08109bc4:
	ldr	r1, [pc, #224]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08109c3e
	movs	r0, #1
	bl	sub_08013560
	mov	r0, r8
	bl	sub_080ad1f0
	cmp	r0, #0
	bne.n	.L_08109be8
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_08109b5e
.L_08109be8:
	mov	r0, sl
	bl	sub_08038060
	movs	r0, #112
	bl	sub_081c0010
	movs	r0, #129
	lsls	r0, r0, #3
	adds	r0, #255
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L_08109c0e
	mov	r0, r8
	bl	.L_08109cac
	b.n	.L_08109c26
.L_08109c0e:
	cmp	r3, #3
	bne.n	.L_08109c1a
	mov	r0, r8
	bl	sub_0810a2d0
	b.n	.L_08109c26
.L_08109c1a:
	mov	r0, r8
	bl	sub_0810b6c4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08109c72
.L_08109c26:
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #220
	adds	r3, r6, r1
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r3, #12
	mov	r2, r9
	strb	r3, [r2, #0]
	movs	r7, #2
	b.n	.L_08109b5e
.L_08109c3e:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08109c54
	movs	r0, #113
	movs	r5, #1
	bl	sub_081c0010
	negs	r5, r5
	b.n	.L_08109c72
.L_08109c54:
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r3, r6, r0
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	add	r0, sp, #4
	movs	r2, #4
	bl	sub_08108690
	adds	r7, r0, #0
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08109b5e
.L_08109c72:
	movs	r0, #0
	bl	sub_0810bea8
	bl	sub_080f8068
	mov	r0, sl
	movs	r1, #2
	bl	sub_08038018
	ldr	r0, [r6, #36]
	movs	r1, #2
	bl	sub_08038018
	ldr	r0, [r6, #12]
	movs	r1, #2
	bl	sub_08038018
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r5, #0
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
.L_08109cac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r0, [sp, #24]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	fp, r3
	bl	sub_08016ca4
	movs	r1, #0
	movs	r2, #1
	movs	r3, #2
	str	r0, [sp, #12]
	str	r2, [sp, #8]
	str	r3, [sp, #0]
	mov	sl, r1
	movs	r0, #15
	movs	r1, #8
	movs	r2, #15
	movs	r3, #4
	bl	sub_08038010
	str	r0, [sp, #16]
.L_08109ce8:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r3, #3
	movs	r0, #0
	movs	r1, #5
	bl	sub_08038010
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #3
	adds	r3, #220
	add	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #18
	strb	r3, [r2, #5]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	movs	r3, #12
	add	r2, fp
	strb	r3, [r2, #0]
	movs	r3, #1
	mov	r9, r3
.L_08109d18:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_08109dda
	movs	r2, #0
	ldr	r0, [sp, #24]
	mov	r9, r2
	bl	sub_080ad1f0
	adds	r3, r0, #0
	subs	r3, #1
	str	r0, [sp, #8]
	cmp	sl, r3
	ble.n	.L_08109d34
	mov	sl, r3
.L_08109d34:
	mov	r1, sl
	lsls	r3, r1, #1
	adds	r7, r3, #0
	ldr	r3, [sp, #12]
	adds	r7, #216
	ldrh	r2, [r3, r7]
	ldr	r3, [pc, #60]
	mov	r0, sl
	adds	r1, r3, #0
	ands	r1, r2
	mov	r8, r1
	mov	r2, fp
	movs	r1, #5
	ldr	r6, [r2, #36]
	bl	sub_08002064
	movs	r1, #5
	adds	r5, r0, #0
	mov	r0, sl
	bl	sub_08002054
	adds	r2, r0, #0
	lsls	r5, r5, #4
	lsls	r2, r2, #4
	adds	r2, #8
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_08108af0
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	movs	r3, #3
	add	r2, fp
	strb	r3, [r2, #0]
	movs	r3, #129
	b.n	.L_08109d84
	movs	r0, r0
	.2byte 0x01ff
	.2byte 0x0000
.L_08109d84:
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L_08109da4
	ldr	r3, [sp, #12]
	ldrh	r0, [r3, r7]
	bl	.L_08109a98
	mov	r1, r8
	adds	r2, r0, #0
	movs	r3, #1
	b.n	.L_08109db6
.L_08109da4:
	cmp	r3, #3
	bne.n	.L_08109dbe
	ldr	r1, [sp, #12]
	ldrh	r0, [r1, r7]
	bl	.L_0810a2ac
	mov	r1, r8
	adds	r2, r0, #0
	movs	r3, #2
.L_08109db6:
	ldr	r0, [sp, #16]
	bl	.L_081091cc
	b.n	.L_08109dd0
.L_08109dbe:
	mov	r0, r8
	bl	sub_0810a748
	mov	r1, r8
	adds	r2, r0, #0
	movs	r3, #3
	ldr	r0, [sp, #16]
	bl	.L_081091cc
.L_08109dd0:
	ldr	r1, [pc, #540]
	ldr	r0, [sp, #20]
	add	r1, r8
	bl	.L_08109270
.L_08109dda:
	ldr	r4, [pc, #536]
	movs	r3, #1
	ldr	r2, [r4, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08109de8
	b.n	.L_08109f9a
.L_08109de8:
	ldr	r6, [r4, #4]
	movs	r7, #2
	ands	r6, r7
	cmp	r6, #0
	beq.n	.L_08109df4
	b.n	.L_08109f8e
.L_08109df4:
	ldr	r3, [r4, #0]
	movs	r2, #4
	ands	r3, r2
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_08109e82
	ldr	r2, [sp, #12]
	mov	r1, sl
	lsls	r3, r1, #1
	adds	r3, #216
	movs	r0, #126
	ldrh	r5, [r2, r3]
	str	r4, [sp, #4]
	bl	sub_081c0010
	movs	r3, #10
	movs	r1, #9
	movs	r2, #16
	movs	r0, #0
	str	r7, [sp, #0]
	bl	sub_08038010
	adds	r1, r5, #0
	adds	r7, r0, #0
	bl	sub_080f8038
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #9
	add	r3, fp
	strb	r6, [r3, #0]
	ldr	r4, [sp, #4]
	mov	r1, r8
	ldr	r3, [r4, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08109e50
	adds	r6, r4, #0
	movs	r5, #4
.L_08109e42:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_08109e42
.L_08109e50:
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #9
	add	r2, fp
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r1, #2
	adds	r0, r7, #0
	bl	sub_08038018
	bl	.L_08109188
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #232
	add	r3, fp
	ldr	r0, [r3, #0]
	bl	sub_08038060
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	mov	r9, r2
	b.n	.L_08109d18
.L_08109e82:
	ldr	r3, [r4, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08109eac
	movs	r0, #111
	str	r4, [sp, #4]
	bl	sub_081c0010
	movs	r3, #1
	ldr	r0, [sp, #8]
	negs	r3, r3
	add	sl, r3
	add	r0, sl
	ldr	r1, [sp, #8]
	bl	sub_08002064
	ldr	r4, [sp, #4]
	movs	r1, #1
	mov	sl, r0
	mov	r9, r1
.L_08109eac:
	ldr	r3, [r4, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08109ed4
	movs	r0, #111
	str	r4, [sp, #4]
	bl	sub_081c0010
	ldr	r0, [sp, #8]
	movs	r2, #1
	add	sl, r2
	add	r0, sl
	ldr	r1, [sp, #8]
	bl	sub_08002064
	ldr	r4, [sp, #4]
	movs	r3, #1
	mov	sl, r0
	mov	r9, r3
.L_08109ed4:
	ldr	r3, [r4, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08109f0a
	movs	r1, #5
	negs	r1, r1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #0
	bge.n	.L_08109eee
	movs	r3, #15
	add	sl, r3
.L_08109eee:
	ldr	r1, [sp, #8]
	cmp	sl, r1
	blt.n	.L_08109f00
.L_08109ef4:
	movs	r2, #5
	ldr	r3, [sp, #8]
	negs	r2, r2
	add	sl, r2
	cmp	sl, r3
	bge.n	.L_08109ef4
.L_08109f00:
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #1
	mov	r9, r1
.L_08109f0a:
	ldr	r3, [pc, #232]
	movs	r2, #128
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08109f40
	ldr	r3, [sp, #8]
	movs	r2, #5
	add	sl, r2
	cmp	sl, r3
	blt.n	.L_08109f26
	movs	r1, #15
	negs	r1, r1
	add	sl, r1
.L_08109f26:
	mov	r2, sl
	cmp	r2, #0
	bge.n	.L_08109f36
.L_08109f2c:
	movs	r3, #5
	add	sl, r3
	mov	r1, sl
	cmp	r1, #0
	blt.n	.L_08109f2c
.L_08109f36:
	movs	r0, #111
	bl	sub_081c0010
	movs	r2, #1
	mov	r9, r2
.L_08109f40:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08109d18
.L_08109f48:
	ldr	r0, [sp, #20]
	movs	r1, #2
	bl	sub_08038018
	movs	r0, #1
	bl	sub_08013560
	cmp	r5, #0
	bne.n	.L_08109fd8
	movs	r3, #129
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L_08109fa4
	ldr	r0, [sp, #24]
	mov	r1, sl
	bl	.L_0810a03c
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_08109f86
	ldr	r0, [sp, #24]
	mov	r1, sl
	bl	.L_0810a108
.L_08109f86:
	ldr	r0, [pc, #112]
	bl	sub_081084f4
	b.n	.L_08109fcc
.L_08109f8e:
	movs	r0, #113
	movs	r5, #1
	bl	sub_081c0010
	negs	r5, r5
	b.n	.L_08109f48
.L_08109f9a:
	movs	r0, #112
	bl	sub_081c0010
	movs	r5, #0
	b.n	.L_08109f48
.L_08109fa4:
	cmp	r3, #3
	bne.n	.L_08109fb8
	ldr	r0, [sp, #24]
	mov	r1, sl
	bl	sub_0810a2d8
	ldr	r0, [pc, #72]
	bl	sub_081084f4
	b.n	.L_08109fcc
.L_08109fb8:
	ldr	r0, [sp, #24]
	mov	r1, sl
	bl	sub_0810b6cc
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08109fd8
	ldr	r0, [pc, #56]
	bl	sub_081084f4
.L_08109fcc:
	ldr	r0, [sp, #24]
	bl	sub_080ad1f0
	cmp	r0, #0
	beq.n	.L_08109fd8
	b.n	.L_08109ce8
.L_08109fd8:
	ldr	r0, [sp, #16]
	movs	r1, #2
	bl	sub_08038018
	adds	r0, r5, #0
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000092
	.4byte 0x03001150
	.4byte 0x0000125b
	.4byte 0x00001273
	.2byte 0x12fe
	.2byte 0x0000
.L_0810a004:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	cmp	r5, #0
	beq.n	.L_0810a034
	bl	sub_08038060
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_0810bf98
	adds	r0, r6, #0
	bl	sub_080ad1f0
	cmp	r0, #0
	bne.n	.L_0810a034
	ldr	r0, [pc, #12]
	adds	r1, r5, #0
	movs	r2, #8
	movs	r3, #20
	bl	sub_08038078
.L_0810a034:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x123e
	.2byte 0x0000
.L_0810a03c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	sub	sp, #4
	ldr	r7, [r3, #0]
	mov	sl, r0
	mov	r9, r1
	bl	sub_08016ca4
	mov	r1, r9
	lsls	r5, r1, #1
	adds	r6, r0, #0
	adds	r5, #216
	ldrh	r0, [r6, r5]
	bl	sub_080ad010
	movs	r2, #1
	str	r2, [sp, #0]
	mov	r8, r0
	ldrh	r0, [r6, r5]
	bl	.L_08109a98
	mov	r1, r9
	mov	fp, r0
	mov	r0, sl
	bl	sub_080ad018
	mov	r3, r8
	ldrb	r2, [r3, #3]
	movs	r3, #16
	ands	r3, r2
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_0810a0f4
	cmp	r0, #1
	ble.n	.L_0810a0f4
	ldr	r0, [pc, #112]
	bl	sub_081084f4
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #228
	adds	r3, r7, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r0, #0
	mov	r8, r2
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #230
	adds	r3, r7, r2
	subs	r2, #10
	adds	r5, r7, r2
	ldr	r2, [r5, #0]
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #5
	adds	r2, r7, r3
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r1, #128
	movs	r2, #48
	bl	sub_08108af0
	mov	r1, sl
	mov	r2, fp
	movs	r0, #0
	bl	.L_081096f8
	str	r0, [sp, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [r5, #0]
	bl	sub_080f8028
	movs	r0, #0
	mov	r1, r8
	adds	r2, r6, #0
	bl	sub_08108af0
.L_0810a0f4:
	ldr	r0, [sp, #0]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x125e
	.2byte 0x0000
.L_0810a108:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r1, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r7, r2, #0
	str	r3, [sp, #12]
	mov	r8, r0
	bl	sub_08016ca4
	ldr	r2, [sp, #16]
	adds	r6, r0, #0
	lsls	r2, r2, #1
	str	r2, [sp, #8]
	adds	r5, r2, #0
	adds	r5, #216
	ldrh	r3, [r6, r5]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r2, r3
	mov	r9, r2
	mov	r0, r9
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #4
	ands	r3, r2
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	movs	r2, #1
	str	r3, [sp, #4]
	negs	r2, r2
	movs	r3, #0
	mov	fp, r0
	str	r3, [sp, #0]
	cmp	r7, r2
	bne.n	.L_0810a16a
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r7, #1
.L_0810a16a:
	ldrh	r0, [r6, r5]
	bl	.L_08109a98
	adds	r2, r7, #0
	muls	r2, r0
	mov	sl, r2
	cmp	r2, #0
	bne.n	.L_0810a18a
	mov	r0, r9
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #252]
	bl	sub_0810857c
	b.n	.L_0810a270
.L_0810a18a:
	ldrh	r2, [r6, r5]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810a1b2
	mov	r3, fp
	ldrb	r2, [r3, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810a1b2
	mov	r0, r9
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #216]
	bl	sub_0810857c
	b.n	.L_0810a270
.L_0810a1b2:
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_0810a1bc
	ldr	r5, [pc, #204]
	b.n	.L_0810a1e4
.L_0810a1bc:
	ldr	r3, [sp, #8]
	adds	r3, #216
	ldrh	r2, [r6, r3]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810a1d0
	ldr	r5, [pc, #188]
	b.n	.L_0810a1e4
.L_0810a1d0:
	cmp	r7, #1
	ble.n	.L_0810a1d8
	ldr	r5, [pc, #184]
	b.n	.L_0810a1e4
.L_0810a1d8:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_0810a1e2
	ldr	r5, [pc, #180]
	b.n	.L_0810a1e4
.L_0810a1e2:
	ldr	r5, [pc, #180]
.L_0810a1e4:
	mov	r0, r9
	movs	r1, #2
	bl	sub_08038120
	mov	r0, sl
	movs	r1, #5
	bl	sub_08038120
	adds	r0, r5, #0
	bl	sub_0810857c
	movs	r0, #0
	bl	sub_08108630
	cmp	r0, #0
	beq.n	.L_0810a21e
	ldr	r2, [sp, #4]
	cmp	r2, #0
	bne.n	.L_0810a210
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L_0810a214
.L_0810a210:
	ldr	r5, [pc, #136]
	b.n	.L_0810a216
.L_0810a214:
	ldr	r5, [pc, #136]
.L_0810a216:
	adds	r0, r5, #0
	bl	sub_0810857c
	b.n	.L_0810a270
.L_0810a21e:
	movs	r0, #102
	bl	sub_081c0010
	cmp	r7, #0
	ble.n	.L_0810a238
	adds	r5, r7, #0
.L_0810a22a:
	mov	r0, r8
	ldr	r1, [sp, #16]
	subs	r5, #1
	bl	sub_080ad278
	cmp	r5, #0
	bne.n	.L_0810a22a
.L_0810a238:
	mov	r0, r8
	bl	sub_080ad288
	mov	r0, r8
	bl	sub_080ad008
	mov	r0, sl
	bl	sub_080ad1d8
	bl	.L_08109188
	ldr	r2, [sp, #12]
	mov	r1, r8
	ldr	r0, [r2, #36]
	bl	.L_0810a004
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L_0810a264
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_0810a268
.L_0810a264:
	ldr	r5, [pc, #60]
	b.n	.L_0810a26a
.L_0810a268:
	ldr	r5, [pc, #60]
.L_0810a26a:
	adds	r0, r5, #0
	bl	sub_0810857c
.L_0810a270:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000125d
	.4byte 0x0000125c
	.4byte 0x00001263
	.4byte 0x00001262
	.4byte 0x00001261
	.4byte 0x00001260
	.4byte 0x0000125f
	.4byte 0x00001267
	.4byte 0x00001265
	.4byte 0x00001266
	.2byte 0x1264
	.2byte 0x0000
.L_0810a2ac:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_080ad010
	ldrh	r3, [r0, #0]
	lsrs	r0, r3, #2
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_0810a2c4
	movs	r0, #0
.L_0810a2c4:
	pop	{r5, pc}
	.align 2, 0
