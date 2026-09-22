.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015068, 0x08015068
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_080156e8, 0x080156e8
	.set sub_08020098, 0x08020098
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e21c, 0x0815e21c
	.set sub_0817e698, 0x0817e698
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.global Overlay_0817e0c0
Overlay_0817e0c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #96
	str	r1, [sp, #40]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #96]
	ldr	r1, [r3, #92]
	str	r2, [sp, #36]
	mov	fp, r0
	ldr	r5, [r3, #48]
	movs	r0, #0
	str	r5, [sp, #28]
	mov	r9, r1
	ldr	r3, [r3, #100]
	movs	r5, #224
	str	r3, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	mov	r1, sp
	adds	r2, #82
	strh	r3, [r2, #0]
	adds	r1, #52
	movs	r0, #0
	str	r1, [sp, #20]
	bl	sub_08144aac
	ldr	r0, [pc, #40]
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #32]
	ldr	r1, [pc, #32]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	lsls	r5, r5, #3
	mov	r2, r9
	adds	r1, r2, r5
	movs	r3, #0
	ldr	r0, [pc, #20]
	movs	r2, #0
	b.n	.L_0817e140
	.4byte 0x00001010
	.4byte 0x00000134
	.4byte 0x000000c2
	.4byte 0x02010000
	.2byte 0x0181
	.2byte 0x0000
.L_0817e140:
	bl	sub_08157cf4
	movs	r6, #248
	movs	r3, #176
	lsls	r3, r3, #4
	lsls	r6, r6, #2
	movs	r7, #0
	mov	ip, r3
	add	r6, r9
	mov	r4, r9
.L_0817e154:
	mov	r2, ip
	adds	r1, r6, r2
	movs	r0, #0
	adds	r2, r4, r5
.L_0817e15c:
	ldrb	r3, [r2, #0]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, #32
	bne.n	.L_0817e15c
	adds	r7, #1
	adds	r4, #32
	subs	r6, #32
	cmp	r7, #32
	bne.n	.L_0817e154
	movs	r1, #240
	lsls	r1, r1, #4
	movs	r3, #0
	ldr	r0, [pc, #844]
	add	r1, r9
	movs	r2, #1
	bl	sub_08157cf4
	ldr	r3, [sp, #40]
	cmp	r3, #5
	bhi.n	.L_0817e1bc
	ldr	r2, [pc, #832]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0817e1ac
	.4byte 0x0817e1b0
	.4byte 0x0817e1b4
	.4byte 0x0817e1b8
	.4byte 0x0817e1bc
	.2byte 0xe1b8
	.2byte 0x0817
	ldr	r0, [pc, #800]
	b.n	.L_0817e1be
	ldr	r0, [pc, #800]
	b.n	.L_0817e1be
	ldr	r0, [pc, #800]
	b.n	.L_0817e1be
	ldr	r0, [pc, #800]
	b.n	.L_0817e1be
.L_0817e1bc:
	ldr	r0, [pc, #800]
.L_0817e1be:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #796]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ef
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #50
	movs	r1, #200
	mov	r5, fp
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #760]
	bl	sub_080145a8
	movs	r3, #80
	movs	r2, #36
	ldrsh	r1, [r5, r2]
	ldr	r0, [r5, #8]
	movs	r2, #10
	bl	sub_08157530
	ldr	r0, [r5, #8]
	bl	sub_08118098
	movs	r1, #48
	ldr	r0, [r0, #0]
	bl	sub_08020098
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	mov	r2, sp
	adds	r2, #84
	mov	sl, r0
	movs	r1, #36
	ldrsh	r0, [r5, r1]
	adds	r1, r2, #0
	str	r2, [sp, #16]
	bl	sub_0815e21c
	mov	r5, sp
	mov	r1, sp
	movs	r3, #0
	adds	r5, #72
	adds	r1, #44
	str	r3, [sp, #32]
	str	r5, [sp, #8]
	str	r1, [sp, #12]
.L_0817e23a:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_0817e334
	ldr	r5, [pc, #680]
	movs	r3, #0
	mov	r8, r3
	movs	r6, #255
.L_0817e248:
	mov	r1, sl
	ldr	r3, [r1, #8]
	str	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_0817e258
	ldr	r2, [pc, #668]
	adds	r3, r3, r2
	b.n	.L_0817e25e
.L_0817e258:
	movs	r1, #128
	lsls	r1, r1, #12
	adds	r3, r3, r1
.L_0817e25e:
	str	r3, [r5, #0]
	mov	r2, sl
	ldr	r3, [r2, #12]
	movs	r1, #224
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r0
	lsls	r3, r3, #11
	str	r3, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	ldr	r3, [r5, #0]
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_0817e2a4
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_0817e2a4:
	mov	r2, r8
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_0817e248
	movs	r2, #0
	mov	r8, r2
	movs	r6, #255
	mov	r5, r9
.L_0817e2c4:
	mov	r1, sl
	ldr	r3, [r1, #8]
	str	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_0817e2d4
	ldr	r2, [pc, #544]
	adds	r3, r3, r2
	b.n	.L_0817e2da
.L_0817e2d4:
	movs	r1, #128
	lsls	r1, r1, #12
	adds	r3, r3, r1
.L_0817e2da:
	str	r3, [r5, #0]
	mov	r2, sl
	ldr	r3, [r2, #12]
	movs	r1, #224
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #128
	ldr	r2, [pc, #512]
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r0
	adds	r3, r3, r2
	lsls	r3, r3, #10
	str	r3, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08014878
	ldr	r3, [r5, #0]
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_0817e324
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_0817e324:
	movs	r3, #0
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_0817e2c4
.L_0817e334:
	mov	r2, fp
	ldr	r0, [r2, #8]
	ldr	r1, [sp, #8]
	bl	sub_0815e21c
	movs	r0, #32
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #420]
	ldr	r3, [sp, #44]
	adds	r5, r0, #0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #412]
	movs	r1, #0
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #44]
	ldr	r2, [sp, #12]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r3, r9
	str	r3, [r2, #4]
	ldr	r3, [pc, #392]
	str	r2, [r5, #16]
	str	r3, [r5, #8]
	movs	r3, #6
	str	r3, [r5, #0]
	mov	r3, r8
	str	r3, [r5, #12]
	str	r1, [r5, #20]
	ldr	r1, [sp, #32]
	cmp	r1, #14
	bgt.n	.L_0817e3dc
	movs	r6, #128
	bl	sub_08014de4
	lsls	r6, r6, #8
	movs	r1, #128
	adds	r0, r6, #0
	lsls	r1, r1, #9
	adds	r2, r6, #0
	bl	sub_080151e4
	ldr	r2, [sp, #8]
	ldr	r0, [r2, #0]
	ldr	r1, [r2, #4]
	subs	r0, #128
	subs	r1, #88
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #332]
	lsls	r3, r1, #2
	ldr	r2, [r2, r3]
	lsls	r1, r2, #1
	adds	r0, r1, #0
	bl	sub_080151e4
	mov	r2, fp
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_0817e3cc
	adds	r0, r6, #0
	bl	sub_08015068
.L_0817e3cc:
	ldr	r0, [pc, #308]
	mov	r1, r8
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
.L_0817e3dc:
	ldr	r7, [sp, #32]
	subs	r7, #8
	cmp	r7, #11
	bhi.n	.L_0817e48a
	ldr	r3, [sp, #40]
	cmp	r3, #4
	bne.n	.L_0817e3f4
	movs	r1, #128
	lsls	r3, r7, #12
	lsls	r1, r1, #5
	adds	r6, r3, r1
	b.n	.L_0817e406
.L_0817e3f4:
	lsls	r2, r7, #1
	adds	r2, r2, r7
	lsls	r3, r2, #7
	subs	r3, r3, r2
	movs	r2, #176
	lsls	r2, r2, #4
	lsls	r3, r3, #3
	adds	r2, #232
	adds	r6, r3, r2
.L_0817e406:
	lsls	r2, r7, #3
	movs	r3, #32
	subs	r1, r3, r2
	cmp	r1, #0
	ble.n	.L_0817e412
	movs	r1, #0
.L_0817e412:
	str	r1, [r5, #20]
	ldr	r3, [sp, #12]
	movs	r2, #7
	strb	r2, [r3, #0]
	add	r3, sp, #44
	str	r3, [sp, #12]
	strb	r2, [r3, #1]
	ldr	r3, [sp, #12]
	ldr	r1, [pc, #228]
	str	r2, [r5, #0]
	str	r1, [r3, #4]
	ldr	r3, [pc, #224]
	str	r3, [r5, #8]
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r0, r2, #0
	bl	sub_080151e4
	mov	r1, fp
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0817e45c
	ldr	r2, [sp, #16]
	ldr	r0, [r2, #0]
	ldr	r1, [r2, #4]
	subs	r0, #144
	subs	r1, #84
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_0817e470
.L_0817e45c:
	ldr	r3, [sp, #16]
	movs	r2, #0
	ldr	r0, [r3, #0]
	ldr	r1, [r3, #4]
	subs	r0, #112
	subs	r1, #84
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	bl	sub_08015160
.L_0817e470:
	lsls	r1, r6, #2
	adds	r0, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	ldr	r0, [pc, #136]
	mov	r1, r8
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
.L_0817e48a:
	adds	r0, r5, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	ldr	r5, [sp, #32]
	cmp	r5, #8
	bne.n	.L_0817e522
	mov	r2, fp
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r3, #0
	str	r5, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r5, fp
	bl	sub_0814cd48
	movs	r1, #4
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08118088
	ldr	r1, [sp, #40]
	cmp	r1, #5
	bne.n	.L_0817e510
	movs	r0, #145
	bl	sub_081180e8
	b.n	.L_0817e516
	.4byte 0x0000013e
	.4byte 0x0817e194
	.4byte 0x00000186
	.4byte 0x0000017d
	.4byte 0x0000017f
	.4byte 0x00000184
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02014000
	.4byte 0xfff80000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x081992b0
	.4byte 0x081994b8
	.4byte 0x081991e0
	.4byte 0x02010000
	.2byte 0x9364
	.2byte 0x0819
.L_0817e510:
	movs	r0, #134
	bl	sub_081180e8
.L_0817e516:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #8
	str	r3, [r2, #0]
.L_0817e522:
	cmp	r7, #55
	bhi.n	.L_0817e608
	bl	sub_08014de4
	ldr	r0, [sp, #28]
	ldr	r6, [pc, #296]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080156e8
	movs	r2, #0
	mov	r8, r2
	add	r7, sp, #60
.L_0817e53c:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	blt.n	.L_0817e592
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r2, [r7, #0]
	asrs	r5, r5, #4
	adds	r5, #2
	ldr	r1, [pc, #264]
	lsls	r4, r5, #1
	asrs	r2, r2, #1
	mov	r3, r8
	movs	r0, #1
	str	r2, [r7, #0]
	ands	r0, r3
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #24]
	lsls	r0, r0, #2
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r5, [sp, #0]
	subs	r3, r3, r5
	str	r4, [sp, #4]
	ldr	r5, [sp, #20]
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	ldr	r2, [pc, #216]
	bl	sub_08138058
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_0817e592:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #64
	bne.n	.L_0817e53c
	ldr	r3, [sp, #40]
	cmp	r3, #5
	bne.n	.L_0817e608
	movs	r5, #0
	mov	r8, r5
	add	r7, sp, #60
	mov	r6, r9
.L_0817e5ac:
	ldr	r5, [r6, #24]
	cmp	r5, #23
	bhi.n	.L_0817e5fc
	cmp	r5, #0
	bge.n	.L_0817e5b8
	adds	r5, #3
.L_0817e5b8:
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	asrs	r5, r5, #2
	ldr	r2, [r7, #0]
	lsls	r5, r5, #11
	movs	r1, #240
	ldr	r3, [r7, #4]
	lsls	r1, r1, #4
	add	r5, r9
	asrs	r2, r2, #1
	adds	r5, r5, r1
	movs	r1, #32
	str	r2, [r7, #0]
	str	r1, [sp, #0]
	movs	r1, #64
	subs	r3, #32
	str	r1, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #36]
	adds	r1, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r6, #0
	movs	r1, #60
	lsls	r2, r2, #6
	bl	sub_08138058
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
.L_0817e5fc:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r6, #28
	cmp	r3, #16
	bne.n	.L_0817e5ac
.L_0817e608:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #32]
	adds	r5, #1
	str	r5, [sp, #32]
	cmp	r5, #60
	beq.n	.L_0817e632
	b.n	.L_0817e23a
.L_0817e632:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02014000
	.4byte 0x08197410
	.4byte 0xfffffe00
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_0817e698
	pop	{pc}
