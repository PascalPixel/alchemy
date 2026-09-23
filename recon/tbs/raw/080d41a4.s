.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080d41a4
	.thumb_func
Func_080d41a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #96]
	adds	r3, r5, #0
	ldmia	r3!, {r1}
	sub	sp, #52
	str	r1, [sp, #40]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r3, [pc, #84]
	ldr	r2, [r5, #8]
	adds	r6, r1, r3
	str	r2, [sp, #28]
	str	r0, [r6, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	movs	r1, #7
	strh	r3, [r2, #0]
	movs	r0, #46
	movs	r3, #3
	movs	r2, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r5, #28]
	str	r3, [sp, #44]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	sub_080ed408
	ldr	r3, [r5, #32]
	mov	r5, sp
	adds	r5, #44
	str	r5, [sp, #12]
	str	r3, [r5, #4]
	ldr	r1, [sp, #40]
	ldr	r0, [pc, #24]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	b.n	.L_080d4224
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.2byte 0x00d1
	.2byte 0x0000
.L_080d4224:
	movs	r3, #0
	ldr	r0, [pc, #700]
	ldr	r1, [sp, #28]
	movs	r2, #0
	bl	sub_080e0524
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	beq.n	.L_080d424c
	ldr	r0, [pc, #684]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #680]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
.L_080d424c:
	movs	r7, #0
	movs	r2, #128
	ldr	r3, [pc, #668]
	mov	r8, r7
	movs	r1, #0
	lsls	r2, r2, #3
.L_080d4258:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080d4258
	ldr	r1, [sp, #40]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r5, [pc, #648]
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r5
	movs	r3, #50
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #632]
	bl	sub_080041d8
	ldr	r0, [pc, #632]
	ldr	r7, [sp, #40]
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #628]
	ldr	r3, [r3, #24]
	ldrb	r2, [r2, r3]
	str	r2, [sp, #24]
	ldr	r5, [sp, #24]
	lsls	r2, r2, #3
	movs	r7, #48
	movs	r1, #0
	subs	r3, r2, r5
	negs	r7, r7
	mov	fp, r1
	str	r2, [sp, #8]
	cmp	r3, r7
	bne.n	.L_080d42a8
	b.n	.L_080d45b6
.L_080d42a8:
	ldr	r1, [pc, #592]
	ldr	r0, [sp, #40]
	adds	r1, r0, r1
	str	r1, [sp, #20]
.L_080d42b0:
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	bne.n	.L_080d42ee
	mov	r3, fp
	cmp	r3, #63
	bgt.n	.L_080d42ee
	ldr	r3, [pc, #576]
	movs	r2, #128
	mov	r5, fp
	ldr	r1, [r3, #0]
	lsls	r2, r2, #1
	cmp	r5, #55
	ble.n	.L_080d42d8
	mov	r7, fp
	movs	r3, #176
	lsls	r2, r7, #3
	lsls	r3, r3, #2
	subs	r2, r3, r2
.L_080d42d8:
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d42e8
	ldrh	r3, [r1, #54]
	subs	r3, r3, r2
	b.n	.L_080d42ec
.L_080d42e8:
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
.L_080d42ec:
	strh	r3, [r1, #54]
.L_080d42ee:
	mov	r1, fp
	cmp	r1, #32
	bne.n	.L_080d42fa
	movs	r0, #134
	bl	sub_080b50e8
.L_080d42fa:
	ldr	r3, [sp, #24]
	movs	r2, #0
	str	r2, [sp, #32]
	cmp	r3, #0
	bne.n	.L_080d4306
	b.n	.L_080d44b0
.L_080d4306:
	ldr	r7, [sp, #32]
	lsls	r5, r7, #3
	cmp	fp, r5
	bne.n	.L_080d4322
	movs	r0, #134
	bl	sub_080f9010
	movs	r1, #128
	ldr	r3, [pc, #496]
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #492]
	bl	sub_080072f0
.L_080d4322:
	adds	r0, r5, #4
	mov	sl, r0
	cmp	fp, r5
	bge.n	.L_080d432c
	b.n	.L_080d445e
.L_080d432c:
	adds	r3, r5, #0
	adds	r3, #9
	cmp	fp, r3
	blt.n	.L_080d4336
	b.n	.L_080d445e
.L_080d4336:
	adds	r3, r5, #1
	adds	r6, r5, #2
	cmp	fp, r3
	blt.n	.L_080d436a
	cmp	fp, r6
	bge.n	.L_080d4372
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #4]
	ldr	r7, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r0, [pc, #448]
	adds	r3, r7, r3
	ldrb	r2, [r0, r3]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r3, #112
	str	r3, [sp, #4]
	subs	r2, #24
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #40]
	movs	r3, #0
	bl	sub_080072f4
.L_080d436a:
	adds	r1, r5, #4
	mov	sl, r1
	cmp	fp, r6
	blt.n	.L_080d445e
.L_080d4372:
	adds	r5, #4
	mov	sl, r5
	cmp	fp, sl
	bge.n	.L_080d43a8
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #4]
	ldr	r5, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r7, [pc, #392]
	adds	r3, r5, r3
	ldrb	r2, [r7, r3]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r3, #112
	str	r3, [sp, #4]
	movs	r5, #168
	ldr	r3, [sp, #40]
	lsls	r5, r5, #5
	adds	r1, r3, r5
	subs	r2, #24
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	movs	r3, #0
	bl	sub_080072f4
.L_080d43a8:
	cmp	fp, r6
	bne.n	.L_080d445e
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #332]
	movs	r7, #0
	str	r7, [sp, #16]
	movs	r0, #0
	adds	r1, r1, r2
	ldr	r7, [pc, #344]
	mov	r8, r0
	mov	r9, r1
.L_080d43be:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_080d443a
	bl	sub_08004458
	ldr	r6, [pc, #332]
	ands	r6, r0
	bl	sub_08004458
	ldr	r5, [pc, #328]
	ldr	r3, [pc, #332]
	ands	r5, r0
	mov	r0, r9
	adds	r5, r5, r3
	ldr	r3, [r0, #0]
	ldr	r2, [r3, #4]
	ldr	r1, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #296]
	adds	r3, r1, r3
	ldrb	r3, [r2, r3]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #208
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
	mov	r5, r9
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #244]
	lsls	r3, r3, #2
	ldrh	r3, [r1, r3]
	ldr	r0, [sp, #16]
	cmp	r0, r3
	beq.n	.L_080d444a
.L_080d443a:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_080d43be
	ldr	r1, [pc, #216]
.L_080d444a:
	ldr	r3, [sp, #40]
	ldr	r7, [sp, #20]
	ldr	r5, [pc, #216]
	adds	r2, r3, r5
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #2
	adds	r3, #2
	ldrh	r3, [r1, r3]
	str	r3, [r2, #0]
.L_080d445e:
	cmp	fp, sl
	bne.n	.L_080d44a2
	ldr	r3, [pc, #152]
	ldr	r1, [sp, #40]
	ldr	r3, [r1, r3]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_080d44a2
	ldr	r2, [pc, #136]
	movs	r6, #36
	adds	r5, r1, r2
.L_080d4478:
	ldr	r3, [r5, #0]
	movs	r1, #1
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	movs	r1, #7
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L_080d4478
.L_080d44a2:
	ldr	r3, [sp, #32]
	ldr	r5, [sp, #24]
	adds	r3, #1
	str	r3, [sp, #32]
	cmp	r3, r5
	beq.n	.L_080d44b0
	b.n	.L_080d4306
.L_080d44b0:
	movs	r7, #0
	ldr	r6, [pc, #96]
	mov	r8, r7
.L_080d44b6:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L_080d457a
	subs	r3, #1
	movs	r2, #128
	adds	r0, r6, #0
	lsls	r2, r2, #5
	str	r3, [r6, #24]
	movs	r1, #60
	bl	sub_080e3908
	movs	r0, #208
	ldr	r2, [r6, #4]
	lsls	r0, r0, #15
	cmp	r2, r0
	ble.n	.L_080d452c
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L_080d457a
	.4byte 0x00000073
	.4byte 0x00000060
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x080ee25e
	.4byte 0x03001e80
	.4byte 0x03000168
	.4byte 0x10101010
	.4byte 0x080ee250
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x00007fff
	.4byte 0xffffc000
	.4byte 0x080ee244
	.2byte 0x77a8
	.2byte 0x0000
.L_080d452c:
	ldr	r3, [r6, #0]
	asrs	r7, r3, #16
	cmp	r3, #0
	blt.n	.L_080d457a
	cmp	r7, #119
	bgt.n	.L_080d457a
	cmp	r2, #0
	blt.n	.L_080d457a
	ldr	r0, [r6, #24]
	asrs	r2, r2, #16
	mov	ip, r2
	cmp	r0, #0
	bge.n	.L_080d4548
	adds	r0, #7
.L_080d4548:
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r5, r0, #1
	ldr	r2, [pc, #144]
	mov	r1, r8
	subs	r3, r5, #2
	movs	r4, #1
	ands	r4, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r2, r7, r2
	mov	r7, ip
	str	r0, [sp, #0]
	subs	r3, r7, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #12]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #36]
	bl	sub_080072f4
.L_080d457a:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L_080d44b6
	movs	r1, #16
	movs	r0, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r5, [pc, #76]
	ldr	r3, [sp, #40]
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #24]
	movs	r7, #1
	subs	r3, r0, r1
	add	fp, r7
	adds	r3, #48
	cmp	fp, r3
	beq.n	.L_080d45b6
	b.n	.L_080d42b0
.L_080d45b6:
	ldr	r0, [pc, #48]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x080cd261
