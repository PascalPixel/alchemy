.syntax unified
	.thumb
	.global Func_08109188
	.thumb_func
Func_08109188:
	.global Func_08109188
	.thumb_func
Func_08109188:
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
	bl	0x08038060
	ldr	r0, [pc, #32]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	0x08038080
	ldr	r3, [pc, #24]
	movs	r1, #6
	ldr	r0, [r3, #16]
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r2, r5, #0
	movs	r3, #32
	bl	0x080380b0
.L_081091be:
	add	sp, #4
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x00001237
	.2byte 0x0240
	.2byte 0x0200
	.global Func_081091cc
	.thumb_func
Func_081091cc:
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
	bl	0x08038080
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #5
	adds	r2, r6, #0
	movs	r3, #32
	subs	r5, #3
	bl	0x080380b0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #72
.L_08109218:
	movs	r3, #8
	bl	0x08038080
	b.n	.L_08109252
.L_08109220:
	adds	r0, r6, #0
	bl	RenderOutput_RedrawSavedRectFar
	ldr	r0, [pc, #64]
	adds	r1, r6, #0
	add	r0, r8
	movs	r2, #0
	movs	r3, #0
	bl	0x08038080
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
	bl	0x08038080
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
	.global Func_08109270
	.thumb_func
Func_08109270:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	cmp	r5, #0
	beq.n	.L_0810928a
	bl	RenderOutput_RedrawSavedRectFar
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	0x08038080
.L_0810928a:
	pop	{r5, r6, pc}
	.global Func_0810928c
	.thumb_func
Func_0810928c:
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
	bl	0x08038060
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
	bl	RenderOutput_CreateFar
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
	bl	RenderOutput_CreateFar
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
	bl	Animation_ApplyChildArgumentFar
	b.n	.L_0810936a
.L_08109364:
	movs	r1, #1
	bl	Animation_ApplyChildArgumentFar
.L_0810936a:
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r2, sl
	str	r3, [r7, r2]
	adds	r0, r5, #0
	ldr	r1, [sp, #16]
	bl	0x080ad1c8
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
	bl	Owner_GetState
	adds	r7, r0, #0
	mov	r0, r9
	bl	Item_Get
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
	bl	0x08038060
	mov	r1, r9
	ldr	r0, [sp, #20]
	bl	0x080ad1c0
	cmp	r0, #0
	bne.n	.L_08109402
	ldr	r0, [pc, #108]
	mov	r1, sl
	movs	r2, #8
	movs	r3, #24
	bl	0x08038078
	b.n	.L_0810959a
.L_08109402:
	ldrb	r1, [r5, #2]
	ldr	r0, [sp, #20]
	bl	0x080ad1d0
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
	bl	Item_Get
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
	bl	0x080ad008
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
	bl	0x080ad008
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
	bl	RenderOutput_CreateFar
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
	bl	0x080380b0
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
	bl	0x080380b0
.L_08109554:
	ldr	r0, [pc, #80]
	mov	r1, sl
	adds	r0, r5, r0
	movs	r2, #0
	adds	r3, r6, #0
	bl	0x08038080
	ldr	r2, [sp, #4]
	mov	r0, sl
	movs	r3, #13
	movs	r1, #0
	str	r2, [sp, #0]
	bl	0x08038070
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
	bl	0x08038080
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
	bl	Owner_GetState
	mov	r8, r0
	cmp	r5, #0
	beq.n	.L_08109616
	adds	r0, r5, #0
	bl	0x08038060
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	0x080ad030
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
	bl	UiText_DrawQuantity
	ldr	r0, [pc, #40]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	0x08038080
	b.n	.L_0810960a
.L_081095fe:
	ldr	r0, [pc, #32]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	0x08038080
.L_0810960a:
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	adds	r3, r7, #0
	bl	Func_0810bf98
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
	bl	Owner_GetState
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	Item_Get
	mov	r8, r0
	mov	r3, r8
	ldrb	r2, [r3, #3]
	movs	r3, #16
	ands	r3, r2
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_081096e6
	ldr	r0, [pc, #152]
	bl	0x081084f4
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	0x080ad030
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
	bl	0x0800205c
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
	bl	0x080ad1e8
	cmp	r5, r0
	ble.n	.L_081096b8
	adds	r0, r6, #0
	movs	r1, #0
	bl	0x080ad1e8
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
	bl	Func_08108af0
	mov	r3, r8
	ldrh	r2, [r3, #0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_081096f8
.L_081096e6:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00001251
	.2byte 0x0240
	.2byte 0x0200
	.global Func_081096f8
	.thumb_func
Func_081096f8:
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
	bl	Runtime_AllocateBlock
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
	bl	UiWindow_CreateFar
	movs	r5, #1
	negs	r5, r5
	movs	r7, #0
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_08109744
	b.n	.L_0810989e
.L_08109744:
	bl	Resource_FindFreeEntry
	str	r0, [sp, #4]
	cmp	r0, #96
	bne.n	.L_08109750
	b.n	.L_0810989e
.L_08109750:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	bl	VramBlock_LoadCached
	ldr	r5, [pc, #56]
	ldr	r0, [sp, #4]
	adds	r1, r5, #0
	mov	r2, sl
	movs	r3, #0
	str	r7, [sp, #0]
	bl	RenderOutput_CreateFar
	adds	r1, r5, #0
	mov	r2, sl
	movs	r3, #32
	ldr	r0, [sp, #4]
	str	r7, [sp, #0]
	bl	RenderOutput_CreateFar
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
	bl	Audio_PlayCue
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
	bl	Audio_PlayCue
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
	bl	Math_Mod
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
	bl	Func_0810875c
	mov	r0, fp
	add	r0, r9
	movs	r1, #0
	mov	r2, r8
	bl	Func_0810875c
	mov	r3, fp
	adds	r0, r3, r7
	adds	r0, #1
	movs	r1, #10
	mov	r2, r8
	bl	Func_0810875c
	mov	r0, fp
	movs	r1, #2
	mov	r2, r8
	bl	Func_0810875c
	movs	r1, #128
	ldr	r0, [sp, #4]
	lsls	r1, r1, #1
	mov	r2, r8
	bl	VramBlock_LoadCached
	adds	r5, r7, #1
	adds	r0, r5, #0
	movs	r1, #2
	mov	r2, sl
	movs	r3, #72
	str	r6, [sp, #0]
	bl	0x080380b0
	ldr	r2, [sp, #12]
	movs	r1, #6
	adds	r0, r5, #0
	muls	r0, r2
	movs	r3, #88
	mov	r2, sl
	str	r6, [sp, #0]
	bl	0x080380b0
	ldr	r0, [pc, #104]
	mov	r1, sl
	movs	r2, #136
	movs	r3, #0
	bl	0x08038080
.L_0810985e:
	movs	r0, #1
	bl	WaitFrames
.L_08109864:
	ldr	r2, [pc, #76]
	ldr	r3, [r2, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810987a
	movs	r0, #112
	bl	Audio_PlayCue
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
	bl	Audio_PlayCue
	movs	r5, #1
	negs	r5, r5
.L_08109890:
	movs	r0, #1
	bl	WaitFrames
	mov	r0, sl
	movs	r1, #2
	bl	UiWork_FinalizeFar
.L_0810989e:
	movs	r0, #56
	bl	Runtime_ReleaseHeapBlock
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
	bl	Item_Get
	adds	r6, r0, #0
	movs	r3, #0
	ldrb	r1, [r6, #2]
	adds	r0, r7, #0
	mov	sl, r3
	bl	0x080ad1d0
	mov	r9, r0
	movs	r0, #101
	bl	Audio_PlayCue
	cmp	sl, r5
	bge.n	.L_08109912
.L_081098f0:
	mov	r1, r8
	adds	r0, r7, #0
	bl	0x080ad020
	mov	sl, r0
	ldrh	r0, [r6, #0]
	subs	r5, #1
	negs	r0, r0
	bl	0x080ad1d8
	ldrh	r0, [r6, #0]
	bl	0x080ad258
	bl	Func_08109188
	cmp	r5, #0
	bne.n	.L_081098f0
.L_08109912:
	ldr	r0, [pc, #36]
	bl	0x0810857c
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
	bl	Owner_GetState
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
	bl	Item_Get
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
	bl	0x080ad1c0
	cmp	r0, #0
	beq.n	.L_081099d0
	mov	r3, sl
	ldrb	r1, [r3, #2]
	mov	r0, r8
	bl	0x080ad1d0
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_081099b8
	lsls	r3, r0, #1
	adds	r3, #216
	ldrh	r0, [r7, r3]
	bl	Item_Get
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08109a24
.L_081099b8:
	mov	r0, r8
	movs	r1, #1
	bl	UiText_DrawQuantity
	ldr	r0, [pc, #108]
	bl	0x081084f4
	movs	r0, #0
	bl	0x08108630
	cmp	r0, #0
	beq.n	.L_081099d4
.L_081099d0:
	movs	r0, #0
	b.n	.L_08109a24
.L_081099d4:
	mov	r0, r8
	mov	r1, r9
	bl	0x080ad048
	mov	r3, fp
	ldr	r0, [r3, #36]
	cmp	r0, #0
	beq.n	.L_081099ea
	mov	r1, r8
	bl	Func_0810a004
.L_081099ea:
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08109a1c
	movs	r0, #103
	bl	Audio_PlayCue
	bl	0x08038140
	movs	r1, #8
	movs	r2, #4
	movs	r3, #2
	ldr	r0, [pc, #44]
	bl	0x08038038
	b.n	.L_08109a14
.L_08109a0e:
	movs	r0, #1
	bl	WaitFrames
.L_08109a14:
	bl	0x08038048
	cmp	r0, #0
	beq.n	.L_08109a0e
.L_08109a1c:
	ldr	r0, [pc, #24]
	bl	0x0810857c
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
	bl	Owner_GetState
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
	bl	Item_Get
	ldrb	r3, [r0, #2]
	movs	r0, #0
	cmp	r3, #6
	beq.n	.L_08109a92
	adds	r0, r5, #0
	bl	Item_Get
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08109a92
	adds	r0, r7, #0
	adds	r1, r6, #0
	mov	r2, r8
	bl	Func_0810a108
	movs	r0, #1
.L_08109a92:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}