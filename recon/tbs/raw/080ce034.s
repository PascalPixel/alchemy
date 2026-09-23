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
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08009080, 0x08009080
	.set sub_08009140, 0x08009140
	.set sub_08009150, 0x08009150
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e155c, 0x080e155c
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080ce034
	.global Func_080ce034
	.thumb_func
Unnamed_080ce034:
Func_080ce034:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #940]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #60
	str	r1, [sp, #36]
	ldr	r3, [r3, #0]
	str	r3, [sp, #32]
	ldr	r2, [r6, #8]
	str	r2, [sp, #28]
	ldr	r2, [pc, #924]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r0, [pc, #916]
	bl	sub_08002f40
	ldr	r1, [sp, #28]
	bl	sub_08005340
	ldr	r0, [pc, #908]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #904]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	ldr	r1, [sp, #36]
	adds	r0, r5, #0
	bl	sub_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #28]
	movs	r1, #7
	str	r3, [sp, #40]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #32]
	mov	r0, sp
	adds	r0, #40
	str	r0, [sp, #8]
	str	r3, [r0, #4]
	ldr	r1, [sp, #36]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r1, r2
	str	r5, [r3, #0]
	ldr	r3, [pc, #832]
	adds	r2, r1, r3
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #824]
	bl	sub_080041d8
	movs	r0, #0
	movs	r2, #128
	ldr	r3, [pc, #816]
	mov	r8, r0
	movs	r1, #0
	lsls	r2, r2, #3
.L_080ce0e0:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080ce0e0
	ldr	r1, [sp, #36]
	ldr	r2, [pc, #772]
	adds	r5, r1, r2
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	sl, r0
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	str	r0, [sp, #24]
	mov	r2, sl
	ldr	r3, [r2, #8]
	ldr	r0, [pc, #764]
	str	r0, [sp, #20]
	cmp	r3, #0
	bgt.n	.L_080ce11e
	movs	r1, #240
	lsls	r1, r1, #12
	str	r1, [sp, #20]
.L_080ce11e:
	ldr	r0, [pc, #724]
	ldr	r3, [sp, #36]
	mov	r1, sp
	adds	r0, r3, r0
	adds	r1, #48
	movs	r2, #0
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	mov	fp, r2
.L_080ce130:
	ldr	r3, [pc, #736]
	ldr	r5, [r3, #0]
	bl	sub_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080051d8
	mov	r2, fp
	cmp	r2, #17
	bgt.n	.L_080ce14c
	cmp	r2, #0
	bne.n	.L_080ce164
.L_080ce14c:
	ldr	r0, [sp, #16]
	ldr	r5, [sp, #12]
	ldr	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r0, [r3, #8]
	bl	sub_080e3980
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #0]
.L_080ce164:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_080ce188
	ldr	r5, [sp, #12]
	movs	r1, #32
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #16
	subs	r3, #64
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #36]
	bl	sub_080072f4
.L_080ce188:
	mov	r2, fp
	subs	r2, #4
	cmp	r2, #11
	bhi.n	.L_080ce1ec
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	movs	r1, #0
	lsls	r3, r3, #11
	mov	r8, r1
	add	r7, sp, #48
	mov	r9, r3
.L_080ce1a0:
	mov	r2, r8
	lsls	r6, r2, #12
	adds	r0, r6, #0
	bl	sub_08002322
	mov	r3, fp
	muls	r3, r0
	ldr	r5, [r7, #0]
	asrs	r3, r3, #16
	adds	r0, r6, #0
	adds	r5, r5, r3
	bl	sub_0800231c
	mov	r2, fp
	muls	r2, r0
	ldr	r3, [r7, #4]
	asrs	r2, r2, #16
	adds	r3, r3, r2
	mov	r0, fp
	movs	r2, #32
	ldr	r1, [sp, #36]
	subs	r3, r3, r0
	str	r2, [sp, #0]
	subs	r5, #16
	movs	r2, #64
	str	r2, [sp, #4]
	add	r1, r9
	adds	r2, r5, #0
	subs	r3, #64
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #16
	bne.n	.L_080ce1a0
.L_080ce1ec:
	mov	r3, fp
	cmp	r3, #4
	bne.n	.L_080ce23a
	movs	r3, #160
	mov	r0, sl
	lsls	r3, r3, #13
	str	r3, [r0, #40]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #52]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r0, #48]
	ldr	r3, [pc, #528]
	str	r3, [r0, #72]
	mov	r3, sl
	movs	r2, #0
	adds	r3, #90
	strb	r2, [r3, #0]
	subs	r3, #2
	strb	r2, [r3, #0]
	ldr	r3, [r0, #8]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	ldr	r3, [r0, #16]
	bl	sub_08009150
	mov	r0, sl
	movs	r1, #2
	bl	sub_08009080
	ldr	r2, [pc, #496]
	ldr	r1, [sp, #36]
	mov	r0, fp
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #136
	bl	sub_080f9010
.L_080ce23a:
	mov	r1, fp
	cmp	r1, #16
	bne.n	.L_080ce276
	ldr	r0, [pc, #476]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #436]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	adds	r0, r5, #0
	ldr	r1, [sp, #36]
	bl	sub_08005340
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #72]
	str	r3, [r2, #36]
	str	r3, [r2, #40]
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	mov	r0, sl
	str	r3, [r2, #16]
	bl	sub_08009140
.L_080ce276:
	mov	r1, fp
	cmp	r1, #17
	bgt.n	.L_080ce27e
	b.n	.L_080ce392
.L_080ce27e:
	mov	r3, sl
	ldr	r2, [r3, #12]
	cmp	r2, #0
	ble.n	.L_080ce2f0
	ldr	r0, [sp, #20]
	ldr	r3, [r3, #8]
	adds	r3, r3, r0
	ldr	r0, [pc, #404]
	mov	r1, sl
	str	r3, [r1, #8]
	adds	r3, r2, r0
	str	r3, [r1, #12]
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080ce2c8
	ldr	r5, [sp, #12]
	movs	r1, #40
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	subs	r2, #20
	subs	r3, #52
	str	r1, [sp, #4]
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #36]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	subs	r3, #8
	str	r3, [r5, #0]
	mov	r3, sl
	ldr	r2, [r3, #12]
	b.n	.L_080ce2f0
.L_080ce2c8:
	ldr	r5, [sp, #12]
	movs	r1, #40
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	ldr	r0, [sp, #8]
	subs	r2, #26
	subs	r3, #52
	ldr	r4, [r0, #4]
	ldr	r1, [sp, #36]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	ldr	r3, [r5, #4]
	adds	r3, #8
	str	r3, [r5, #4]
	mov	r1, sl
	ldr	r2, [r1, #12]
.L_080ce2f0:
	cmp	r2, #0
	bge.n	.L_080ce392
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #12]
	mov	r8, r3
	add	r3, sp, #48
	ldr	r7, [pc, #296]
	mov	r9, r3
.L_080ce302:
	bl	sub_08004458
	ldr	r5, [pc, #292]
	ands	r5, r0
	bl	sub_08004458
	ldr	r3, [pc, #288]
	adds	r6, r0, #0
	mov	r0, r9
	ands	r6, r3
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	subs	r3, #24
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #8]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	movs	r1, #1
	movs	r2, #128
	adds	r3, #32
	add	r8, r1
	lsls	r2, r2, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_080ce302
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #184]
	movs	r5, #8
	adds	r3, r0, r1
	str	r5, [r3, #0]
	movs	r0, #145
	bl	sub_080b50e8
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	sub_080b5088
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	movs	r2, #5
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #7
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_080d6888
.L_080ce392:
	movs	r2, #0
	ldr	r6, [pc, #144]
	mov	r8, r2
.L_080ce398:
	ldr	r4, [r6, #24]
	cmp	r4, #0
	ble.n	.L_080ce47a
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	mov	ip, r3
	str	r3, [r6, #0]
	ldr	r1, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r7, r3, r1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	subs	r0, r4, #1
	lsls	r3, r3, #3
	str	r0, [r6, #24]
	str	r7, [r6, #4]
	cmp	r3, #0
	bge.n	.L_080ce3c0
	adds	r3, #63
.L_080ce3c0:
	asrs	r3, r3, #6
	str	r3, [r6, #8]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080ce3d0
	adds	r3, #63
.L_080ce3d0:
	movs	r1, #128
	asrs	r3, r3, #6
	lsls	r1, r1, #6
	movs	r2, #224
	adds	r3, r3, r1
	lsls	r2, r2, #15
	str	r3, [r6, #16]
	cmp	r7, r2
	ble.n	.L_080ce434
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L_080ce47a
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000073
	.4byte 0x0000007d
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010018
	.4byte 0xfff10000
	.4byte 0x03001e80
	.4byte 0x0000ab85
	.4byte 0x000077a8
	.4byte 0x00000089
	.4byte 0xfff80000
	.4byte 0x02010000
	.4byte 0x000003ff
	.2byte 0xffff
	.2byte 0x0000
.L_080ce434:
	ldr	r3, [pc, #160]
	cmp	ip, r3
	bhi.n	.L_080ce47a
	cmp	r7, #0
	blt.n	.L_080ce47a
	cmp	r0, #0
	bge.n	.L_080ce444
	adds	r0, r4, #6
.L_080ce444:
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
	mov	r3, ip
	adds	r1, r2, r1
	asrs	r2, r3, #16
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	asrs	r3, r7, #16
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #8]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
.L_080ce47a:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #1
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L_080ce398
	movs	r1, #16
	movs	r0, #16
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r0, [pc, #72]
	ldr	r3, [sp, #36]
	adds	r2, r3, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #88
	beq.n	.L_080ce4b0
	b.n	.L_080ce130
.L_080ce4b0:
	ldr	r0, [pc, #48]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x080cd261
