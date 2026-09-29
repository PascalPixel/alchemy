.syntax unified
	.thumb
	.section .text.x020080c4,"ax",%progbits
	.p2align 2
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #98
	ldrb	r1, [r6, #0]
	mov	r8, r1
	cmp	r1, #0
	bne.n	.L_020000da
	b.n	.L_0200021a
.L_020000da:
	adds	r3, r1, #0
	subs	r3, #1
	cmp	r3, #7
	bls.n	.L_020000e4
	b.n	.L_020002b2
.L_020000e4:
	ldr	r2, [pc, #488]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200810c
	.4byte 0x020081d4
	.4byte 0x0200812c
	.4byte 0x020081d4
	.4byte 0x020081aa
	.4byte 0x020081d4
	.4byte 0x020081dc
	.4byte 0x02008214
	.4byte 0x02db2380
	.4byte 0x2380632b
	.4byte 0x636b029b
	.4byte 0x22a02186
	.4byte 0x049b23ad
	.4byte 0x04491c28
	.4byte 0xf0040352
	.4byte 0xe053f8b1
	.4byte 0x6bab2280
	.4byte 0x42930612
	.4byte 0xe0bcd000
	.4byte 0x429a6bea
	.4byte 0xe0b8d000
	.4byte 0x42936c2b
	.4byte 0xe0b4d000
	.4byte 0x20927833
	.4byte 0x70333301
	.4byte 0xf984f004
	.4byte 0x33631c2b
	.4byte 0x2b00781b
	.4byte 0x21d0d006
	.4byte 0x02092015
	.4byte 0xf0042200
	.4byte 0xe005f935
	.4byte 0x201521b0
	.4byte 0x22000209
	.4byte 0xf92ef004
	.4byte 0xf878f004
	.4byte 0x0c000080
	.4byte 0xd0062800
	.4byte 0xf0042015
	.4byte 0x2380f8c1
	.4byte 0x6283029b
	.4byte 0x2015e08f
	.4byte 0x2200494f
	.4byte 0xf928f004
	.4byte 0xf0042015
	.4byte 0x23c0f8b5
	.4byte 0x628302db
	.4byte 0x1c2be083
	.4byte 0x781b3363
	.4byte 0xd0072b00
	.4byte 0x1c28218d
	.4byte 0x22000449
	.4byte 0xf0044b46
	.4byte 0xe007f865
	.4byte 0x23a721fe
	.4byte 0x04091c28
	.4byte 0x049b2200
	.4byte 0xf85cf004
	.4byte 0x33017833
	.4byte 0xe06a7033
	.4byte 0x6bab2180
	.4byte 0x428b0609
	.4byte 0x6bead165
	.4byte 0xd162429a
	.4byte 0x42936c2b
	.4byte 0x2380d15f
	.4byte 0x632b029b
	.4byte 0x025b2380
	.4byte 0x1c2b636b
	.4byte 0x33642200
	.4byte 0x3302801a
	.4byte 0x7833801a
	.4byte 0x70333301
	.4byte 0xe04e64ea
	.4byte 0x70332300
	.2byte 0xe04b
.L_0200021a:
	adds	r7, r5, #0
	adds	r7, #100
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_02000240
	bl 0x0200c26c
	ldr	r3, [r5, #76]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	ldr	r1, [pc, #168]
	subs	r3, r3, r0
	str	r3, [r5, #76]
	cmp	r3, r1
	bge.n	.L_0200025a
	mov	r2, r8
	strh	r2, [r7, #0]
	b.n	.L_0200025a
.L_02000240:
	bl 0x0200c26c
	ldr	r3, [r5, #76]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	movs	r1, #128
	adds	r3, r3, r0
	lsls	r1, r1, #7
	str	r3, [r5, #76]
	cmp	r3, r1
	ble.n	.L_0200025a
	movs	r3, #1
	strh	r3, [r7, #0]
.L_0200025a:
	ldr	r1, [pc, #132]
	ldr	r2, [r5, #8]
	adds	r3, r2, r1
	ldr	r1, [pc, #128]
	cmp	r3, r1
	bhi.n	.L_0200026c
	ldr	r3, [r5, #76]
	adds	r3, r2, r3
	str	r3, [r5, #8]
.L_0200026c:
	adds	r7, r5, #0
	adds	r7, #102
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_02000292
	bl 0x0200c26c
	ldr	r3, [r5, #12]
	lsls	r0, r0, #15
	lsrs	r0, r0, #16
	ldr	r1, [pc, #100]
	subs	r3, r3, r0
	adds	r3, r3, r1
	str	r3, [r5, #12]
	cmp	r3, #0
	bge.n	.L_020002b2
	movs	r3, #0
	b.n	.L_020002b0
.L_02000292:
	bl 0x0200c26c
	ldr	r3, [r5, #12]
	lsls	r0, r0, #15
	lsrs	r0, r0, #16
	movs	r2, #128
	adds	r3, r3, r0
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r3, r3, r2
	lsls	r1, r1, #12
	str	r3, [r5, #12]
	cmp	r3, r1
	ble.n	.L_020002b2
	movs	r3, #1
.L_020002b0:
	strh	r3, [r7, #0]
.L_020002b2:
	bl 0x0200c26c
	movs	r3, #100
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020002c4
	movs	r3, #1
	strb	r3, [r6, #0]
.L_020002c4:
	movs	r0, #1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x020080ec
	.4byte 0x00000103
	.4byte 0x02920000
	.4byte 0xffffc000
	.4byte 0xff07ffff
	.4byte 0x002bfffe
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #98
	ldrb	r1, [r6, #0]
	mov	r8, r1
	cmp	r1, #0
	bne.n	.L_02000302
	b.n	.L_02000444
.L_02000302:
	adds	r3, r1, #0
	subs	r3, #1
	cmp	r3, #7
	bls.n	.L_0200030c
	b.n	.L_020004dc
.L_0200030c:
	ldr	r2, [pc, #492]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x02008334
	.4byte 0x020083fe
	.4byte 0x02008354
	.4byte 0x020083fe
	.4byte 0x020083d2
	.4byte 0x020083fe
	.4byte 0x02008406
	.4byte 0x0200843e
	.4byte 0x02db2380
	.4byte 0x2380632b
	.4byte 0x636b029b
	.4byte 0x22a02180
	.4byte 0x049b23a0
	.4byte 0x04491c28
	.4byte 0xf0030352
	.4byte 0xe054ff9d
	.4byte 0x6bab2280
	.4byte 0x42930612
	.4byte 0xe0bdd000
	.4byte 0x429a6bea
	.4byte 0xe0b9d000
	.4byte 0x42936c2b
	.4byte 0xe0b5d000
	.4byte 0x20927833
	.4byte 0x70333301
	.4byte 0xf870f004
	.4byte 0x33631c2b
	.4byte 0x2b00781b
	.4byte 0x21d0d006
	.4byte 0x02092016
	.4byte 0xf0042200
	.4byte 0xe005f821
	.4byte 0x201621b0
	.4byte 0x22000209
	.4byte 0xf81af004
	.4byte 0xff64f003
	.4byte 0x0c000080
	.4byte 0xd0062800
	.4byte 0xf0032016
	.4byte 0x2380ffad
	.4byte 0x6283029b
	.4byte 0x2016e090
	.4byte 0x22004950
	.4byte 0xf814f004
	.4byte 0xf0032016
	.4byte 0x23c0ffa1
	.4byte 0x628302db
	.4byte 0x1c2be084
	.4byte 0x781b3363
	.4byte 0xd0082b00
	.4byte 0x23962184
	.4byte 0x04491c28
	.4byte 0x049b2200
	.4byte 0xff50f003
	.4byte 0x21f2e007
	.4byte 0x1c282397
	.4byte 0x22000409
	.4byte 0xf003049b
	.4byte 0x7833ff47
	.4byte 0x70333301
	.4byte 0x2180e06a
	.4byte 0x06096bab
	.4byte 0xd165428b
	.4byte 0x429a6bea
	.4byte 0x6c2bd162
	.4byte 0xd15f4293
	.4byte 0x029b2380
	.4byte 0x2380632b
	.4byte 0x636b025b
	.4byte 0x22001c2b
	.4byte 0x801a3364
	.4byte 0x801a3302
	.4byte 0x33017833
	.4byte 0x64ea7033
	.4byte 0x2300e04e
	.2byte 0x7033
	.2byte 0xe04b
.L_02000444:
	adds	r7, r5, #0
	adds	r7, #100
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_0200046a
	bl 0x0200c26c
	ldr	r3, [r5, #76]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	ldr	r1, [pc, #168]
	subs	r3, r3, r0
	str	r3, [r5, #76]
	cmp	r3, r1
	bge.n	.L_02000484
	mov	r2, r8
	strh	r2, [r7, #0]
	b.n	.L_02000484
.L_0200046a:
	bl 0x0200c26c
	ldr	r3, [r5, #76]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	movs	r1, #128
	adds	r3, r3, r0
	lsls	r1, r1, #7
	str	r3, [r5, #76]
	cmp	r3, r1
	ble.n	.L_02000484
	movs	r3, #1
	strh	r3, [r7, #0]
.L_02000484:
	ldr	r1, [pc, #128]
	ldr	r2, [r5, #8]
	adds	r3, r2, r1
	ldr	r1, [pc, #128]
	cmp	r3, r1
	bhi.n	.L_02000496
	ldr	r3, [r5, #76]
	adds	r3, r2, r3
	str	r3, [r5, #8]
.L_02000496:
	adds	r7, r5, #0
	adds	r7, #102
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_020004bc
	bl 0x0200c26c
	ldr	r3, [r5, #12]
	lsls	r0, r0, #15
	lsrs	r0, r0, #16
	ldr	r1, [pc, #96]
	subs	r3, r3, r0
	adds	r3, r3, r1
	str	r3, [r5, #12]
	cmp	r3, #0
	bge.n	.L_020004dc
	movs	r3, #0
	b.n	.L_020004da
.L_020004bc:
	bl 0x0200c26c
	ldr	r3, [r5, #12]
	lsls	r0, r0, #15
	lsrs	r0, r0, #16
	movs	r2, #128
	adds	r3, r3, r0
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r3, r3, r2
	lsls	r1, r1, #12
	str	r3, [r5, #12]
	cmp	r3, r1
	ble.n	.L_020004dc
	movs	r3, #1
.L_020004da:
	strh	r3, [r7, #0]
.L_020004dc:
	bl 0x0200c26c
	movs	r3, #100
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020004ee
	movs	r3, #1
	strb	r3, [r6, #0]
.L_020004ee:
	movs	r0, #1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x02008314
	.4byte 0x00000103
	.4byte 0xffffc000
	.4byte 0xff17ffff
	.4byte 0x0027fffe
	.2byte 0x8000
	.2byte 0xffff
	.section .text.x02008950,"ax",%progbits
	.p2align 2
	.global Func_02000950
	.thumb_func
Func_02000950:
	push {r5, lr}
	ldr r0, [pc, #124]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000950_0
	ldr r0, [pc, #116]
	b .L_02000950_1
.L_02000950_0:
	ldr r0, [pc, #116]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000950_2
	ldr r0, [pc, #112]
	b .L_02000950_1
.L_02000950_2:
	ldr r0, [pc, #112]
	bl 0x0200c2cc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000950_3
	ldr r0, [pc, #104]
	b .L_02000950_1
.L_02000950_3:
	ldr r0, [pc, #104]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000950_4
	ldr r0, [pc, #96]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000950_5
	ldr r2, [pc, #92]
	movs r1, #167
	lsls r1, r1, #1
	adds r3, r2, r1
	strb r5, [r3]
	movs r3, #215
	lsls r3, r3, #1
	adds r1, r2, r3
	movs r3, #2
	b .L_02000950_6
.L_02000950_5:
	ldr r0, [pc, #76]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000950_7
	ldr r2, [pc, #60]
	movs r3, #215
	lsls r3, r3, #1
	adds r1, r2, r3
	movs r3, #1
.L_02000950_6:
	strb r3, [r1]
	movs r1, #227
	lsls r1, r1, #1
	adds r2, r2, r1
	strb r3, [r2]
.L_02000950_7:
	ldr r0, [pc, #40]
	b .L_02000950_1
.L_02000950_4:
	ldr r0, [pc, #44]
.L_02000950_1:
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x0000093e
	.4byte 0x0200d508
	.4byte 0x00000927
	.4byte 0x0200cef0
	.4byte 0x00000928
	.4byte 0x0200d028
	.4byte 0x00000911
	.4byte 0x00000925
	.4byte 0x0200ccf8
	.4byte 0x00000922
	.4byte 0x0200cba8
	.global Func_020009fc
	.thumb_func
Func_020009fc:
	push {lr}
	ldr r0, [pc, #48]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_020009fc_0
	ldr r0, [pc, #40]
	b .L_020009fc_1
.L_020009fc_0:
	movs r0, #138
	lsls r0, r0, #4
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_020009fc_2
	ldr r0, [pc, #28]
	b .L_020009fc_1
.L_020009fc_2:
	ldr r0, [pc, #28]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_020009fc_3
	ldr r0, [pc, #24]
	b .L_020009fc_1
.L_020009fc_3:
	ldr r0, [pc, #24]
.L_020009fc_1:
	pop {r1}
	bx r1
	.4byte 0x0000093e
	.4byte 0x0200d9d0
	.4byte 0x0200da54
	.4byte 0x00000928
	.4byte 0x0200d958
	.4byte 0x0200d778
	.section .text.x02008ca0,"ax",%progbits
	.p2align 2
	.global Func_02000ca0
	.thumb_func
Func_02000ca0:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	ldr r0, [pc, #936]
	bl 0x0200c2cc
	cmp r0, #0
	bne .L_02000ca0_0
	b .L_02000ca0_1
.L_02000ca0_0:
	bl 0x0200c2ec
	bl 0x0200c454
	movs r2, #10
	movs r0, #0
	movs r1, #20
	bl 0x0200c39c
	ldr r0, [pc, #908]
	ldr r1, [pc, #912]
	bl 0x0200c40c
	movs r0, #190
	movs r1, #1
	movs r2, #177
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200c414
	bl 0x0200c41c
	movs r0, #40
	ldr r5, [pc, #884]
	bl 0x0200c2e4
	movs r1, #1
	movs r0, #22
	bl 0x0200c394
	ldr r0, [pc, #876]
	bl 0x0200c3ac
	adds r0, r5, #0
	bl 0x0200b9ec
	movs r1, #129
	movs r2, #60
	movs r0, #20
	lsls r1, r1, #1
	bl 0x0200c3ec
	movs r1, #2
	movs r0, #20
	bl 0x0200c38c
	movs r0, #20
	bl 0x0200b9ec
	movs r0, #22
	movs r1, #1
	bl 0x0200c394
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #22
	bl 0x0200c3d4
	adds r0, r5, #0
	bl 0x0200b9ec
	movs r0, #20
	movs r1, #1
	bl 0x0200c394
	movs r3, #176
	lsls r3, r3, #8
	mov r11, r3
	mov r1, r11
	movs r0, #20
	bl 0x0200ba00
	movs r0, #20
	bl 0x0200b9ec
	movs r3, #192
	lsls r3, r3, #6
	mov r9, r3
	ldr r6, [pc, #780]
	movs r0, #23
	mov r1, r9
	bl 0x0200ba00
	movs r1, #3
	movs r0, #23
	bl 0x0200c374
	adds r0, r6, #0
	bl 0x0200b9ec
	movs r0, #22
	ldr r1, [pc, #760]
	movs r2, #40
	bl 0x0200c3ec
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #22
	bl 0x0200c3d4
	adds r0, r5, #0
	bl 0x0200b9ec
	movs r0, #23
	movs r1, #0
	bl 0x0200ba00
	movs r1, #4
	movs r0, #23
	bl 0x0200c37c
	adds r0, r6, #0
	bl 0x0200b9ec
	movs r1, #128
	movs r2, #40
	movs r0, #20
	lsls r1, r1, #1
	bl 0x0200c3ec
	movs r1, #2
	movs r0, #20
	bl 0x0200c38c
	movs r0, #20
	bl 0x0200b9ec
	movs r1, #3
	movs r0, #22
	bl 0x0200c37c
	adds r0, r5, #0
	bl 0x0200b9ec
	movs r3, #208
	lsls r3, r3, #8
	mov r10, r3
	movs r0, #20
	mov r1, r10
	bl 0x0200ba00
	movs r0, #23
	movs r1, #3
	bl 0x0200c374
	movs r1, #3
	movs r0, #20
	bl 0x0200c37c
	movs r0, #60
	bl 0x0200c2e4
	movs r1, #131
	movs r2, #40
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200c3ec
	movs r3, #160
	lsls r3, r3, #7
	mov r8, r3
	mov r1, r8
	movs r0, #22
	bl 0x0200ba00
	ldr r0, [pc, #612]
	bl 0x0200c3ac
	movs r1, #1
	movs r0, #22
	bl 0x0200c38c
	adds r0, r5, #0
	bl 0x0200b9ec
	movs r2, #40
	movs r0, #20
	ldr r1, [pc, #584]
	bl 0x0200c3ec
	movs r1, #2
	movs r0, #20
	bl 0x0200c38c
	movs r0, #20
	bl 0x0200b9ec
	movs r1, #132
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c3ec
	adds r0, r5, #0
	movs r1, #0
	movs r2, #20
	bl 0x0200c3c4
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #23
	bl 0x0200c3ec
	adds r0, r6, #0
	bl 0x0200b9ec
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #22
	bl 0x0200ba00
	movs r0, #22
	movs r1, #3
	bl 0x0200c37c
	adds r0, r5, #0
	movs r1, #0
	movs r2, #20
	bl 0x0200c3c4
	movs r1, #129
	movs r2, #40
	movs r0, #20
	lsls r1, r1, #1
	bl 0x0200c3ec
	movs r1, #2
	movs r0, #20
	bl 0x0200c38c
	movs r0, #20
	bl 0x0200b9ec
	movs r0, #22
	mov r1, r8
	bl 0x0200ba00
	movs r1, #4
	movs r0, #22
	bl 0x0200c374
	movs r0, #22
	bl 0x0200b9ec
	movs r0, #20
	mov r1, r11
	movs r2, #0
	bl 0x0200c3d4
	movs r0, #23
	mov r1, r9
	movs r2, #40
	bl 0x0200c3d4
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #20
	mov r1, r10
	bl 0x0200c3d4
	movs r1, #2
	movs r0, #22
	bl 0x0200c394
	movs r0, #20
	bl 0x0200c2e4
	adds r0, r5, #0
	bl 0x0200b9ec
	movs r1, #129
	movs r0, #23
	lsls r1, r1, #1
	bl 0x0200c3f4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200c3f4
	movs r0, #40
	bl 0x0200c2e4
	movs r1, #3
	movs r0, #22
	bl 0x0200c37c
	adds r0, r5, #0
	bl 0x0200b9ec
	ldr r0, [pc, #356]
	ldr r1, [pc, #360]
	bl 0x0200c40c
	movs r0, #182
	movs r1, #1
	movs r2, #190
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200c414
	ldr r2, [pc, #340]
	movs r0, #23
	ldr r1, [pc, #328]
	bl 0x0200c31c
	ldr r1, [pc, #332]
	movs r0, #23
	bl 0x0200c324
	ldr r2, [pc, #320]
	movs r0, #22
	ldr r1, [pc, #308]
	bl 0x0200c31c
	ldr r1, [pc, #320]
	movs r0, #22
	bl 0x0200c324
	movs r0, #20
	ldr r1, [pc, #292]
	ldr r2, [pc, #300]
	bl 0x0200c31c
	movs r2, #190
	lsls r2, r2, #2
	movs r0, #20
	movs r1, #182
	bl 0x0200c35c
	movs r0, #20
	movs r1, #2
	bl 0x0200c38c
	movs r1, #128
	movs r2, #60
	movs r0, #20
	lsls r1, r1, #1
	bl 0x0200c3ec
	movs r0, #20
	mov r1, r10
	bl 0x0200ba00
	movs r2, #20
	movs r0, #20
	movs r1, #0
	bl 0x0200c3c4
	movs r0, #20
	movs r1, #3
	bl 0x0200c37c
	movs r0, #20
	movs r1, #4
	movs r2, #0
	bl 0x0200c384
	movs r2, #40
	movs r0, #20
	mov r1, r9
	bl 0x0200c3d4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200c40c
	movs r0, #216
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #208]
	movs r3, #1
	lsls r0, r0, #16
	bl 0x0200c414
	movs r0, #20
	bl 0x0200c30c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r1, [pc, #188]
	movs r0, #20
	ldr r2, [pc, #188]
	bl 0x0200c31c
	movs r0, #20
	movs r1, #182
	ldr r2, [pc, #180]
	bl 0x0200c35c
	movs r2, #202
	movs r0, #20
	movs r1, #192
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #202
	lsls r2, r2, #2
	movs r0, #20
	movs r1, #216
	bl 0x0200c35c
	movs r0, #20
	mov r1, r10
	bl 0x0200ba00
	movs r0, #20
	movs r1, #2
	bl 0x0200c394
	bl 0x02008bf0
	movs r0, #20
	movs r1, #216
	ldr r2, [pc, #132]
	bl 0x0200c35c
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl 0x0200c36c
	movs r0, #20
	bl 0x0200c30c
	movs r5, #128
	lsls r5, r5, #9
	str r5, [r0, #24]
	movs r0, #20
	bl 0x0200c30c
	str r5, [r0, #28]
	movs r0, #146
	lsls r0, r0, #4
	bl 0x0200c2d4
	bl 0x0200c2f4
.L_02000ca0_1:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000911
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x00004016
	.4byte 0x00001d26
	.4byte 0x00004017
	.4byte 0x00000101
	.4byte 0x00001d40
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x00006666
	.4byte 0x0200c464
	.4byte 0x0200c49c
	.4byte 0x03160000
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x0000030e
	.4byte 0x0000031e
	.section .text.x020093d4,"ax",%progbits
	.p2align 2
	.global Func_020013d4
	.thumb_func
Func_020013d4:
	push {r5, r6, lr}
	ldr r6, [pc, #88]
	movs r2, #130
	ldr r5, [r6]
	movs r0, #142
	lsls r2, r2, #1
	lsls r0, r0, #1
	adds r5, r5, r2
	bl 0x0200c2dc
	ldr r3, [r6, #76]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r6, #0
	adds r2, #73
	str r2, [r3]
	str r6, [r5, #28]
	ldr r5, [pc, #56]
	bl 0x0200c26c
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5]
	ldr r5, [pc, #48]
	bl 0x0200c26c
	ldr r3, [pc, #48]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5]
	str r6, [r3]
	str r6, [r3, #4]
	ldr r3, [pc, #40]
	str r6, [r3]
	bl 0x0200c284
	movs r0, #1
	bl 0x0200c25c
	bl 0x02009444
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x03001e70
	.4byte 0x0200db58
	.4byte 0x0200db38
	.4byte 0x0200db50
	.4byte 0x0200db60
	.section .text.x0200a618,"ax",%progbits
	.p2align 2
	.global FuneKanpan_RunJumpScene
	.thumb_func
FuneKanpan_RunJumpScene:
	push {r5, lr}
	bl 0x0200c2ec
	ldr r0, [pc, #320]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #1
	movs r0, #25
	bl 0x0200c404
	movs r0, #1
	bl 0x0200c25c
	movs r1, #5
	movs r0, #21
	bl 0x0200c374
	movs r0, #21
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200c36c
	movs r0, #0
	bl 0x0200c30c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r0, #6]
	ldr r3, [pc, #256]
	movs r0, #224
	ldr r3, [r3]
	ldr r2, [pc, #252]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c2e4
	bl 0x02008bb8
	movs r0, #10
	bl 0x0200c2e4
	movs r1, #216
	movs r0, #0
	lsls r1, r1, #16
	ldr r2, [pc, #220]
	bl 0x0200c36c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c31c
	ldr r2, [pc, #204]
	movs r1, #216
	movs r0, #0
	bl 0x0200c35c
	movs r0, #20
	bl 0x0200c2e4
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200ba00
	movs r0, #0
	movs r1, #2
	movs r2, #10
	bl 0x0200c384
	movs r0, #0
	ldr r1, [pc, #168]
	ldr r2, [pc, #172]
	bl 0x0200c31c
	movs r2, #156
	movs r1, #194
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200c35c
	movs r0, #181
	bl 0x0200c45c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200c2ac
	movs r0, #20
	bl 0x0200c2e4
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #124]
	negs r0, r0
	bl 0x0200c2ac
	movs r0, #10
	bl 0x0200c2e4
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #0
	bl 0x0200c3d4
	movs r0, #25
	bl 0x0200c30c
	ldr r5, [pc, #60]
	adds r0, #85
	movs r1, #128
	movs r2, #128
	strb r5, [r0]
	lsls r1, r1, #10
	movs r0, #25
	lsls r2, r2, #9
	bl 0x0200c31c
	movs r2, #153
	lsls r2, r2, #2
	movs r1, #216
	movs r0, #25
	bl 0x0200c344
	movs r0, #149
	bl 0x0200c45c
	movs r0, #22
	movs r1, #2
	bl 0x0200c3e4
	movs r1, #5
	movs r0, #22
	bl 0x0200c374
	movs r0, #22
	bl 0x0200c30c
	b .L_02002618_0
	.4byte 0x00000000
	.4byte 0x0200d418
	.4byte 0x03001ebc
	.4byte 0x00000202
	.4byte 0x024a0000
	.4byte 0x00000256
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x0000e666
.L_02002618_0:
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #40]
	ldr r3, [pc, #548]
	str r3, [r0, #72]
	movs r3, #208
	lsls r3, r3, #9
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r5, #128
	ldr r3, [pc, #536]
	lsls r5, r5, #8
	movs r1, #192
	movs r2, #192
	str r3, [r0, #108]
	str r5, [r0, #68]
	lsls r1, r1, #11
	movs r0, #22
	lsls r2, r2, #10
	bl 0x0200c31c
	ldr r2, [pc, #520]
	movs r1, #182
	movs r0, #22
	bl 0x0200c34c
	movs r0, #22
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200ba00
	movs r0, #0
	movs r1, #6
	movs r2, #80
	bl 0x0200c384
	movs r2, #141
	movs r0, #25
	movs r1, #232
	lsls r2, r2, #2
	bl 0x0200c344
	movs r0, #0
	movs r1, #204
	ldr r2, [pc, #464]
	bl 0x0200c35c
	movs r0, #0
	movs r1, #208
	ldr r2, [pc, #460]
	bl 0x0200c35c
	movs r0, #0
	movs r1, #248
	ldr r2, [pc, #448]
	bl 0x0200c35c
	movs r0, #0
	bl 0x0200c30c
	cmp r0, #0
	beq .L_02002618_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200c36c
.L_02002618_1:
	movs r0, #0
	bl 0x0200c30c
	cmp r0, #0
	beq .L_02002618_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200c36c
.L_02002618_2:
	movs r0, #0
	bl 0x0200c30c
	cmp r0, #0
	beq .L_02002618_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200c36c
.L_02002618_3:
	movs r1, #128
	adds r2, r5, #0
	movs r0, #1
	lsls r1, r1, #9
	bl 0x0200c31c
	movs r1, #128
	adds r2, r5, #0
	movs r0, #2
	lsls r1, r1, #9
	bl 0x0200c31c
	movs r1, #128
	adds r2, r5, #0
	movs r0, #3
	lsls r1, r1, #9
	bl 0x0200c31c
	movs r2, #146
	movs r0, #0
	movs r1, #250
	lsls r2, r2, #2
	bl 0x0200c354
	movs r2, #150
	movs r0, #1
	movs r1, #240
	lsls r2, r2, #2
	bl 0x0200c354
	movs r2, #150
	movs r0, #2
	movs r1, #254
	lsls r2, r2, #2
	bl 0x0200c354
	movs r2, #154
	lsls r2, r2, #2
	movs r0, #3
	movs r1, #248
	bl 0x0200c35c
	movs r0, #0
	movs r1, #1
	bl 0x0200c374
	movs r0, #1
	movs r1, #1
	bl 0x0200c374
	movs r0, #2
	movs r1, #1
	bl 0x0200c374
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200c3d4
	movs r0, #149
	bl 0x0200c45c
	movs r0, #40
	bl 0x0200c2e4
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200c3f4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c3f4
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200c3f4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200c3f4
	movs r0, #60
	bl 0x0200c2e4
	movs r0, #0
	ldr r1, [pc, #172]
	ldr r2, [pc, #172]
	bl 0x0200c31c
	movs r0, #1
	ldr r1, [pc, #160]
	ldr r2, [pc, #164]
	bl 0x0200c31c
	movs r0, #2
	ldr r1, [pc, #152]
	ldr r2, [pc, #152]
	bl 0x0200c31c
	movs r0, #3
	ldr r1, [pc, #140]
	ldr r2, [pc, #144]
	bl 0x0200c31c
	movs r2, #141
	movs r0, #0
	movs r1, #248
	lsls r2, r2, #2
	bl 0x0200c354
	movs r2, #141
	movs r1, #248
	lsls r2, r2, #2
	movs r0, #1
	bl 0x0200c354
	movs r0, #20
	bl 0x0200c2e4
	movs r2, #141
	movs r0, #2
	movs r1, #248
	lsls r2, r2, #2
	bl 0x0200c354
	movs r2, #141
	movs r1, #248
	lsls r2, r2, #2
	movs r0, #3
	bl 0x0200c354
	movs r0, #20
	bl 0x0200c2e4
	ldr r2, [pc, #80]
	movs r0, #226
	ldr r1, [pc, #80]
	lsls r0, r0, #1
	adds r3, r2, r0
	strh r1, [r3]
	movs r3, #227
	lsls r3, r3, #1
	adds r1, r2, r3
	movs r3, #30
	strh r3, [r1]
	adds r0, #103
	adds r2, r2, r0
	movs r3, #3
	strb r3, [r2]
	ldr r0, [pc, #60]
	movs r1, #16
	bl 0x0200c434
	movs r0, #62
	movs r1, #3
	bl 0x0200c42c
	bl 0x0200c2f4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000b333
	.4byte 0x020088c1
	.4byte 0x0000026a
	.4byte 0x00000262
	.4byte 0x00000256
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x02000240
	.4byte 0x0000006f
	.4byte 0x0000006d
	.section .text.x0200b284,"ax",%progbits
	.p2align 2
	.global SceneEffect_InitSlotsEightToNineteen
	.thumb_func
SceneEffect_InitSlotsEightToNineteen:
	push {r5, r6, lr}
	movs r0, #8
	bl 0x0200c30c
	movs r5, #0
	adds r0, #89
	strb r5, [r0]
	movs r0, #9
	bl 0x0200c30c
	adds r0, #89
	strb r5, [r0]
	movs r0, #10
	bl 0x0200c30c
	adds r0, #89
	strb r5, [r0]
	movs r0, #11
	bl 0x0200c30c
	adds r0, #89
	strb r5, [r0]
	movs r0, #8
	bl 0x0200b380
	movs r0, #9
	bl 0x0200b380
	movs r0, #10
	bl 0x0200b380
	movs r0, #11
	bl 0x0200b380
	movs r0, #12
	bl 0x0200b380
	movs r0, #13
	bl 0x0200b380
	movs r0, #14
	bl 0x0200b380
	movs r0, #15
	bl 0x0200b380
	movs r0, #12
	bl 0x0200c30c
	ldr r6, [pc, #144]
	ldr r3, [r0, #16]
	movs r0, #13
	str r3, [r6]
	bl 0x0200c30c
	ldr r3, [r0, #16]
	movs r0, #14
	str r3, [r6, #4]
	bl 0x0200c30c
	ldr r3, [r0, #16]
	movs r0, #15
	str r3, [r6, #8]
	bl 0x0200c30c
	ldr r3, [r0, #16]
	movs r0, #16
	str r3, [r6, #12]
	bl 0x0200b380
	movs r0, #17
	bl 0x0200b380
	movs r0, #18
	bl 0x0200b380
	movs r0, #19
	bl 0x0200b380
	movs r0, #16
	bl 0x0200c30c
	ldr r5, [pc, #80]
	str r5, [r0, #24]
	movs r0, #17
	bl 0x0200c30c
	str r5, [r0, #24]
	movs r0, #18
	bl 0x0200c30c
	str r5, [r0, #24]
	movs r0, #19
	bl 0x0200c30c
	str r5, [r0, #24]
	movs r0, #16
	bl 0x0200c30c
	ldr r3, [r0, #16]
	movs r0, #17
	str r3, [r6, #16]
	bl 0x0200c30c
	ldr r3, [r0, #16]
	movs r0, #18
	str r3, [r6, #20]
	bl 0x0200c30c
	ldr r3, [r0, #16]
	movs r0, #19
	str r3, [r6, #24]
	bl 0x0200c30c
	ldr r3, [r0, #16]
	str r3, [r6, #28]
	bl 0x0200b3b8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200db90
	.4byte 0xffff0000
	.section .text.x0200b3b8,"ax",%progbits
	.p2align 2
	.global Func_020033b8
	.thumb_func
Func_020033b8:
	push {r5, r6, r7, lr}
	ldr r6, [pc, #164]
	movs r5, #0
	movs r7, #3
.L_020033b8_2:
	adds r0, r5, #0
	bl 0x0200b150
	cmp r0, #0
	beq .L_020033b8_0
	adds r0, r5, #0
	bl 0x0200b464
	str r0, [r6]
	b .L_020033b8_1
.L_020033b8_0:
	str r7, [r6]
.L_020033b8_1:
	adds r5, #1
	adds r6, #4
	cmp r5, #3
	bls .L_020033b8_2
	movs r0, #0
	bl 0x0200b150
	cmp r0, #0
	beq .L_020033b8_3
	movs r0, #0
	bl 0x0200b464
	ldr r3, [pc, #112]
	str r0, [r3]
	b .L_020033b8_4
.L_020033b8_3:
	ldr r2, [pc, #104]
	movs r3, #3
	str r3, [r2]
.L_020033b8_4:
	movs r0, #2
	bl 0x0200b150
	cmp r0, #0
	beq .L_020033b8_5
	movs r0, #2
	bl 0x0200b464
	ldr r3, [pc, #84]
	str r0, [r3, #4]
	b .L_020033b8_6
.L_020033b8_5:
	ldr r2, [pc, #76]
	movs r3, #3
	str r3, [r2, #4]
.L_020033b8_6:
	ldr r6, [pc, #72]
	movs r5, #3
	str r5, [r6, #8]
	str r5, [r6, #12]
	movs r0, #1
	bl 0x0200b150
	cmp r0, #0
	beq .L_020033b8_7
	movs r0, #1
	bl 0x0200b464
	str r0, [r6, #16]
	b .L_020033b8_8
.L_020033b8_7:
	str r5, [r6, #16]
.L_020033b8_8:
	movs r0, #3
	bl 0x0200b150
	cmp r0, #0
	beq .L_020033b8_9
	movs r0, #3
	bl 0x0200b464
	ldr r3, [pc, #24]
	str r0, [r3, #20]
	b .L_020033b8_10
.L_020033b8_9:
	ldr r2, [pc, #20]
	movs r3, #3
	str r3, [r2, #20]
.L_020033b8_10:
	ldr r2, [pc, #12]
	movs r3, #3
	str r3, [r2, #24]
	str r3, [r2, #28]
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200db70
	.section .text.x0200b4bc,"ax",%progbits
	.p2align 2
	.global SceneState_ConfigureEntries8Through19
	.thumb_func
SceneState_ConfigureEntries8Through19:
	push {r5, lr}
	movs r0, #8
	movs r1, #0
	bl 0x0200b558
	ldr r5, [pc, #140]
	movs r3, #0
	strh r3, [r5]
	movs r0, #9
	movs r1, #1
	bl 0x0200b558
	ldrh r3, [r5, #2]
	adds r3, #128
	strh r3, [r5, #2]
	movs r0, #10
	movs r1, #2
	bl 0x0200b558
	ldrh r3, [r5, #4]
	movs r2, #128
	lsls r2, r2, #1
	adds r3, r3, r2
	strh r3, [r5, #4]
	movs r0, #11
	movs r1, #3
	bl 0x0200b558
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200b5ec
	movs r0, #13
	movs r1, #1
	movs r2, #0
	bl 0x0200b5ec
	movs r0, #14
	movs r1, #2
	movs r2, #0
	bl 0x0200b5ec
	movs r0, #15
	movs r1, #3
	movs r2, #0
	bl 0x0200b5ec
	movs r0, #16
	movs r1, #4
	movs r2, #1
	bl 0x0200b5ec
	movs r0, #17
	movs r1, #5
	movs r2, #1
	bl 0x0200b5ec
	movs r0, #18
	movs r1, #6
	movs r2, #1
	bl 0x0200b5ec
	movs r0, #19
	movs r1, #7
	movs r2, #1
	bl 0x0200b5ec
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200db30
	.global Func_02003558
	.thumb_func
Func_02003558:
	push {r5, lr}
	ldr r3, [pc, #112]
	lsls r1, r1, #1
	ldr r4, [pc, #112]
	ldrh r2, [r3, r1]
	ldr r5, [pc, #112]
	adds r3, r2, r4
	lsls r3, r3, #16
	ldr r4, [pc, #108]
	cmp r3, r5
	bhi .L_02003558_0
	ldr r2, [pc, #108]
	ldrh r3, [r2, r1]
	adds r3, #112
	b .L_02003558_1
.L_02003558_0:
	ldr r5, [pc, #104]
	adds r3, r2, r5
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, r4
	bhi .L_02003558_2
	ldr r2, [pc, #88]
	ldrh r3, [r2, r1]
	adds r3, #224
.L_02003558_1:
	strh r3, [r2, r1]
	movs r1, #3
	bl 0x0200c374
	b .L_02003558_3
.L_02003558_2:
	ldr r4, [pc, #80]
	ldr r5, [pc, #80]
	adds r3, r2, r4
	lsls r3, r3, #16
	cmp r3, r5
	bhi .L_02003558_4
	ldr r2, [pc, #60]
	movs r4, #224
	ldrh r3, [r2, r1]
	lsls r4, r4, #1
	adds r3, r3, r4
	strh r3, [r2, r1]
	movs r1, #2
	bl 0x0200c374
	b .L_02003558_3
.L_02003558_4:
	ldr r2, [pc, #40]
	movs r5, #192
	ldrh r3, [r2, r1]
	lsls r5, r5, #2
	adds r3, r3, r5
	strh r3, [r2, r1]
	movs r1, #1
	bl 0x0200c374
.L_02003558_3:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200db40
	.4byte 0xffff97ff
	.4byte 0x07fe0000
	.4byte 0x000007fe
	.4byte 0x0200db30
	.4byte 0x000017ff
	.4byte 0xffff8fff
	.4byte 0x7ffe0000
	.global Func_020035ec
	.thumb_func
Func_020035ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	mov r9, r2
	adds r7, r0, #0
	bl 0x0200c30c
	movs r3, #2
	ldr r1, [r0, #80]
	mov r2, r9
	ands r3, r2
	mov r10, r0
	mov r11, r1
	cmp r3, #0
	bne .L_020035ec_0
	ldr r3, [pc, #232]
	lsls r1, r5, #2
	ldr r3, [r3, r1]
	mov r8, r1
	cmp r3, #2
	beq .L_020035ec_1
	cmp r3, #2
	bhi .L_020035ec_2
	cmp r3, #1
	beq .L_020035ec_3
	b .L_020035ec_4
.L_020035ec_2:
	cmp r3, #3
	beq .L_020035ec_5
	cmp r3, #4
	beq .L_020035ec_6
	b .L_020035ec_4
.L_020035ec_3:
	ldr r2, [pc, #204]
	ldr r3, [pc, #208]
	ldrh r2, [r2]
	lsls r6, r5, #1
	strh r2, [r3, r6]
	adds r0, r7, #0
	movs r1, #8
	bl 0x0200c3fc
	b .L_020035ec_7
.L_020035ec_1:
	ldr r2, [pc, #184]
	ldr r3, [pc, #188]
	ldrh r2, [r2, #2]
	lsls r6, r5, #1
	strh r2, [r3, r6]
	adds r0, r7, #0
	movs r1, #9
	bl 0x0200c3fc
	b .L_020035ec_7
.L_020035ec_5:
	ldr r2, [pc, #164]
	ldr r3, [pc, #168]
	ldrh r2, [r2, #4]
	lsls r6, r5, #1
	strh r2, [r3, r6]
	adds r0, r7, #0
	movs r1, #10
	bl 0x0200c3fc
	b .L_020035ec_7
.L_020035ec_6:
	ldr r2, [pc, #144]
	ldr r3, [pc, #148]
	ldrh r2, [r2, #6]
	lsls r6, r5, #1
	strh r2, [r3, r6]
	adds r0, r7, #0
	movs r1, #11
	bl 0x0200c3fc
	b .L_020035ec_7
.L_020035ec_0:
	lsls r2, r5, #2
	mov r8, r2
.L_020035ec_4:
	lsls r6, r5, #1
.L_020035ec_7:
	movs r3, #1
	mov r1, r9
	ands r3, r1
	cmp r3, #0
	beq 0x0200b6c0
	ldr r5, [pc, #112]
	ldrh r0, [r5, r6]
	bl 0x0200c274
	movs r2, #128
	adds r7, r0, #0
	ldrh r0, [r5, r6]
	lsls r2, r2, #8
	adds r0, r0, r2
	bl 0x0200c274
	mov r3, r11
	asrs r0, r0, #5
	strh r0, [r3, #30]
	ldr r3, [pc, #88]
	mov r1, r8
	ldr r3, [r3, r1]
	lsls r2, r7, #2
	subs r3, r3, r2
	lsls r2, r7, #1
	subs r3, r3, r2
.L_020036be:
	b .L_020036be_0
	.2byte 0x4d11
	.2byte 0x2380
	.2byte 0x5ba8
	.2byte 0x021b
	.2byte 0x18c0
	.2byte 0xf000
	.2byte 0xfdd3
	.2byte 0x1c07
	.2byte 0x5ba8
	.2byte 0xf000
	.2byte 0xfdcf
	.2byte 0x4659
	.2byte 0x1140
	.2byte 0x83c8
	.2byte 0x4b0b
	.2byte 0x4641
	.2byte 0x585b
	.2byte 0x00ba
	.2byte 0x189b
	.2byte 0x007a
	.2byte 0x189b
.L_020036be_0:
	mov r2, r10
	str r3, [r2, #16]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xdb70
	.2byte 0x0200
	.2byte 0xdb30
	.2byte 0x0200
	.2byte 0xdb40
	.2byte 0x0200
	.2byte 0xdb90
	.2byte 0x0200
	.global SceneState_InitActorSlots8To19
	.thumb_func
SceneState_InitActorSlots8To19:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, [pc, #4]
	ldr r3, [pc, #8]
	movs r2, #0
	b .L_02003710_0
	.4byte 0x0000c000
	.4byte 0x0200db40
.L_02003710_0:
	adds r2, #1
	strh r1, [r3]
	adds r3, #2
	cmp r2, #7
	bls .L_02003710_0
	movs r0, #8
	bl 0x0200b380
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl 0x0200c36c
.L_02003760:
	movs r0, #13
	bl 0x0200b380
	movs r0, #14
	bl 0x0200b380
	movs r0, #15
	bl 0x0200b380
	ldr r2, [pc, #464]
	movs r6, #0
	str r6, [r2]
	str r6, [r2, #4]
	str r6, [r2, #8]
	str r6, [r2, #12]
	movs r0, #8
	mov r8, r2
	bl 0x0200c30c
	ldr r3, [pc, #448]
	mov r10, r3
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2]
	movs r0, #13
	bl 0x0200c30c
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2, #4]
	movs r0, #14
	bl 0x0200c30c
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2, #8]
	movs r0, #15
	bl 0x0200c30c
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2, #12]
	movs r0, #16
	bl 0x0200b380
	movs r0, #17
	bl 0x0200b380
	movs r0, #18
	bl 0x0200b380
	movs r0, #19
	bl 0x0200b380
	movs r0, #16
	bl 0x0200c30c
	ldr r5, [pc, #376]
	str r5, [r0, #24]
	movs r0, #17
	bl 0x0200c30c
	str r5, [r0, #24]
	movs r0, #18
	bl 0x0200c30c
	str r5, [r0, #24]
	movs r0, #19
	bl 0x0200c30c
	mov r3, r8
	str r5, [r0, #24]
	str r6, [r3, #16]
	str r6, [r3, #20]
	str r6, [r3, #24]
	str r6, [r3, #28]
	movs r0, #16
	bl 0x0200c30c
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2, #16]
	movs r0, #17
	bl 0x0200c30c
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2, #20]
	movs r0, #18
	bl 0x0200c30c
	ldr r3, [r0, #16]
.L_02003818:
	mov r2, r10
	str r3, [r2, #24]
	movs r0, #19
	bl 0x0200c30c
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2, #28]
	movs r0, #0
	bl 0x0200c30c
	cmp r0, #0
	beq .L_02003818_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #8
	bl 0x0200c36c
.L_02003818_0:
	movs r0, #1
	bl 0x0200c25c
	movs r0, #13
	movs r1, #8
	bl 0x0200c3fc
	movs r0, #14
	movs r1, #8
	bl 0x0200c3fc
	movs r0, #15
	movs r1, #8
	bl 0x0200c3fc
	movs r0, #16
	movs r1, #8
	bl 0x0200c3fc
	movs r0, #17
	movs r1, #8
	bl 0x0200c3fc
	movs r0, #18
	movs r1, #8
	bl 0x0200c3fc
	movs r1, #8
	movs r0, #19
	bl 0x0200c3fc
	movs r0, #8
	bl 0x0200c30c
	movs r5, #1
	adds r0, #92
	strb r5, [r0]
	movs r0, #13
	bl 0x0200c30c
	adds r0, #92
	strb r5, [r0]
	movs r0, #14
	bl 0x0200c30c
	adds r0, #92
	strb r5, [r0]
	movs r0, #15
	bl 0x0200c30c
	adds r0, #92
	strb r5, [r0]
	movs r0, #16
	bl 0x0200c30c
	adds r0, #92
	strb r5, [r0]
	movs r0, #17
	bl 0x0200c30c
	adds r0, #92
	strb r5, [r0]
	movs r0, #18
	bl 0x0200c30c
	adds r0, #92
	strb r5, [r0]
	movs r0, #19
	bl 0x0200c30c
	adds r0, #92
	strb r5, [r0]
	movs r0, #1
	bl 0x0200c25c
	movs r1, #132
	movs r2, #158
	lsls r1, r1, #16
	lsls r2, r2, #18
	movs r0, #8
	bl 0x0200c36c
	movs r0, #1
	bl 0x0200c25c
	movs r0, #8
	movs r1, #0
	movs r2, #2
	bl 0x0200b5ec
	movs r0, #13
	movs r1, #1
	movs r2, #2
	bl 0x0200b5ec
	movs r0, #14
	movs r1, #2
	movs r2, #2
	bl 0x0200b5ec
	movs r0, #15
	movs r1, #3
	movs r2, #2
	bl 0x0200b5ec
	movs r0, #16
	movs r1, #4
	movs r2, #3
	bl 0x0200b5ec
	movs r0, #17
	movs r1, #5
	movs r2, #3
	bl 0x0200b5ec
	movs r0, #18
	movs r1, #6
	movs r2, #3
	bl 0x0200b5ec
	movs r0, #19
	movs r1, #7
	movs r2, #3
	bl 0x0200b5ec
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xdb70
	.2byte 0x0200
	.2byte 0xdb90
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xffff
	.section .text.x0200ba0c,"ax",%progbits
	.p2align 2
	.global FieldScene_ConfigureFourActorPresentation
	.thumb_func
FieldScene_ConfigureFourActorPresentation:
	push {r5, r6, lr}
	bl 0x0200c2ec
	ldr r2, [pc, #560]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r2]
	ldr r3, [pc, #556]
	ldr r2, [pc, #556]
.L_02003a1e:
	ldr r0, [pc, #560]
	str r3, [r2]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #182
	lsls r1, r1, #16
	ldr r2, [pc, #544]
	movs r0, #21
	bl 0x0200c36c
	movs r0, #21
	bl 0x0200c30c
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #218
	movs r2, #129
	strh r3, [r0, #6]
	lsls r1, r1, #16
	lsls r2, r2, #18
	movs r0, #20
	bl 0x0200c36c
	movs r0, #20
	bl 0x0200c30c
	movs r5, #176
	lsls r5, r5, #8
	movs r1, #204
	strh r5, [r0, #6]
	lsls r1, r1, #16
	ldr r2, [pc, #500]
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r2, #0
	strh r5, [r0, #6]
	movs r1, #0
	movs r0, #23
	bl 0x0200c36c
	movs r1, #15
	movs r0, #0
	bl 0x0200c3a4
	movs r0, #0
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	movs r0, #1
	bl 0x0200c25c
	ldr r3, [pc, #452]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #21
	ldr r1, [pc, #432]
	ldr r2, [pc, #432]
	bl 0x0200c31c
	movs r2, #133
	movs r0, #21
	movs r1, #182
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #40
	adds r1, r5, #0
	movs r0, #21
	bl 0x0200c3d4
	ldr r0, [pc, #408]
	bl 0x0200c3ac
	movs r0, #21
	bl 0x0200b9ec
	ldr r6, [pc, #400]
	movs r0, #20
	movs r1, #2
	bl 0x0200c394
	movs r1, #4
	movs r0, #20
	bl 0x0200c374
	adds r0, r6, #0
	bl 0x0200b9ec
	movs r1, #208
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #208
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r0, #21
	ldr r1, [pc, #356]
	movs r2, #0
	bl 0x0200c3ec
	movs r2, #60
	movs r0, #22
	ldr r1, [pc, #344]
	bl 0x0200c3ec
	movs r1, #3
	movs r0, #20
	bl 0x0200c374
	adds r0, r6, #0
	bl 0x0200b9ec
	movs r1, #129
	movs r0, #21
	lsls r1, r1, #1
	bl 0x0200c3f4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl 0x0200c3f4
	movs r0, #80
	bl 0x0200c2e4
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c3ec
	movs r0, #21
	ldr r1, [pc, #288]
	ldr r2, [pc, #264]
	bl 0x0200c31c
	movs r2, #250
	movs r0, #21
	movs r1, #194
	lsls r2, r2, #1
	bl 0x0200c35c
	adds r1, r5, #0
	movs r0, #21
	movs r2, #20
	bl 0x0200c3d4
	movs r0, #22
	ldr r1, [pc, #236]
	ldr r2, [pc, #236]
	bl 0x0200c31c
	movs r0, #22
	movs r1, #192
	ldr r2, [pc, #248]
	bl 0x0200c35c
	adds r1, r5, #0
	movs r0, #22
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #128
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c31c
	movs r2, #254
	lsls r2, r2, #1
	movs r0, #20
	movs r1, #210
	bl 0x0200c35c
	adds r1, r5, #0
	movs r0, #20
	bl 0x0200ba00
	movs r1, #1
	movs r0, #21
	bl 0x0200c394
	ldr r0, [pc, #192]
	bl 0x0200b9ec
	movs r0, #20
	movs r1, #3
	bl 0x0200c37c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #22
	bl 0x0200ba00
	ldr r0, [pc, #172]
	bl 0x0200b9ec
	movs r1, #128
	movs r2, #20
	movs r0, #20
	lsls r1, r1, #8
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #20
	bl 0x0200c374
	ldr r0, [pc, #152]
	bl 0x0200b9ec
	movs r2, #134
	movs r0, #20
	movs r1, #204
	lsls r2, r2, #2
	bl 0x0200c35c
	adds r1, r5, #0
	movs r0, #22
	movs r2, #0
	bl 0x0200c3d4
	movs r2, #137
	movs r0, #20
	movs r1, #182
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #148
	movs r0, #20
	movs r1, #182
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #166
	movs r1, #182
	lsls r2, r2, #2
	movs r0, #20
	bl 0x0200c354
	movs r0, #40
	bl 0x0200c2e4
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #16
	bl 0x0200c424
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0xdb50
	.2byte 0x0200
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xdb60
	.2byte 0x0200
	.4byte 0x0200d160
	.4byte 0x026a0000
	.4byte 0x020e0000
	.4byte 0x03001ebc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00001f23
	.4byte 0x00006014
	.4byte 0x00000101
	.4byte 0x00019999
	.4byte 0x00000206
	.4byte 0x00005015
	.4byte 0x00009016
	.4byte 0x0000a014
	.section .text.x0200bf30,"ax",%progbits
	.p2align 2
	.global Func_02003f30
	.thumb_func
Func_02003f30:
	.global FieldScene_RunEncounterClosingSequence
	.thumb_func
FieldScene_RunEncounterClosingSequence:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl 0x0200c2ec
	ldr r0, [pc, #676]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	bl 0x0200c43c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c31c
	movs r2, #164
	movs r0, #0
	movs r1, #148
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r1, #128
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #1
	movs r5, #160
	bl 0x0200c3ec
	lsls r5, r5, #7
	movs r0, #22
	movs r1, #1
	bl 0x0200c394
	adds r1, r5, #0
	movs r0, #22
	bl 0x0200ba00
	ldr r0, [pc, #604]
	bl 0x0200c3ac
	movs r1, #0
	ldr r0, [pc, #600]
	bl 0x0200c3b4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r0, #0
	movs r1, #0
	bl 0x0200c304
	cmp r0, #1
	bne .L_02003f30_0
	ldr r0, [pc, #572]
	bl 0x0200b9ec
	bl 0x0200c2f4
	b .L_02003f30_1
.L_02003f30_0:
	ldr r2, [pc, #564]
	movs r3, #236
	mov r8, r2
	ldr r2, [r2]
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	ldr r0, [pc, #540]
	bl 0x0200c3cc
	bl 0x02008bb8
	movs r1, #216
	movs r2, #147
	movs r0, #26
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r0, #26
	ldr r1, [pc, #520]
	ldr r2, [pc, #524]
	bl 0x0200c31c
	movs r2, #149
	movs r0, #26
	movs r1, #216
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #154
	movs r0, #26
	movs r1, #188
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #208
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #208
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #8
	bl 0x0200c3d4
	adds r1, r5, #0
	movs r0, #26
	bl 0x0200ba00
	movs r2, #0
	movs r0, #26
	movs r1, #2
	bl 0x0200c384
	movs r0, #26
	movs r1, #4
	bl 0x0200c374
	movs r0, #26
	movs r1, #0
	bl 0x0200c3bc
	movs r1, #180
	movs r0, #20
	lsls r1, r1, #16
	ldr r2, [pc, #420]
	bl 0x0200c36c
	movs r1, #128
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c31c
	movs r2, #166
	movs r0, #20
	movs r1, #180
	lsls r2, r2, #2
	bl 0x0200c35c
	ldr r6, [pc, #392]
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #20
	bl 0x0200c3d4
	adds r0, r6, #0
	bl 0x0200b9ec
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3d4
	movs r2, #60
	movs r0, #26
	ldr r1, [pc, #348]
	bl 0x0200c3ec
	movs r1, #1
	movs r0, #20
	bl 0x0200c394
	adds r0, r6, #0
	bl 0x0200b9ec
	movs r1, #2
	movs r0, #21
	bl 0x0200c38c
	movs r0, #21
	bl 0x0200b9ec
	movs r2, #20
	adds r1, r5, #0
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #20
	bl 0x0200c374
	ldr r0, [pc, #300]
	bl 0x0200b9ec
	movs r2, #20
	movs r0, #26
	movs r1, #2
	bl 0x0200c384
	movs r1, #4
	movs r0, #26
	bl 0x0200c374
	movs r0, #26
	bl 0x0200b9ec
	movs r2, #160
	movs r0, #20
	movs r1, #182
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #20
	bl 0x0200c3d4
	ldr r0, [pc, #252]
	bl 0x0200b9ec
	movs r1, #128
	movs r2, #20
	movs r0, #26
	lsls r1, r1, #1
	bl 0x0200c3ec
	movs r1, #2
	movs r0, #26
	bl 0x0200c38c
	movs r0, #26
	bl 0x0200b9ec
	movs r0, #20
	movs r1, #3
	bl 0x0200c37c
	movs r0, #22
	movs r1, #0
	bl 0x0200ba00
	movs r1, #1
	movs r0, #22
	bl 0x0200c394
	movs r0, #22
	bl 0x0200b9ec
	ldr r2, [pc, #192]
	movs r0, #22
	ldr r1, [pc, #192]
	bl 0x0200c31c
	ldr r5, [pc, #192]
	movs r0, #22
	adds r1, r5, #0
	bl 0x0200c324
	movs r0, #21
	ldr r1, [pc, #176]
	ldr r2, [pc, #168]
	bl 0x0200c31c
	movs r2, #158
	lsls r2, r2, #2
	movs r0, #21
	movs r1, #168
	bl 0x0200c35c
	adds r1, r5, #0
	movs r0, #21
	bl 0x0200c324
	movs r0, #80
	bl 0x0200c2e4
	adds r1, r5, #0
	movs r0, #26
	bl 0x0200c324
	movs r0, #40
	bl 0x0200c2e4
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #20
	bl 0x0200ba00
	adds r0, r6, #0
	bl 0x0200b9ec
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200ba00
	movs r0, #0
	movs r1, #3
	bl 0x0200c37c
	movs r0, #20
	movs r1, #3
	bl 0x0200c37c
	mov r2, r8
	ldr r3, [r2]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #65
	str r2, [r3]
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #17
	bl 0x0200c424
.L_02003f30_1:
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d1d8
	.4byte 0x00001f69
	.4byte 0x00002016
	.4byte 0x03001ebc
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x03090000
	.4byte 0x00002014
	.4byte 0x00000101
	.4byte 0x00006014
	.4byte 0x00008014
	.4byte 0x0000cccc
	.4byte 0x00019999
	.4byte 0x0200c918
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ca0000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x029a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000010
	.global FuneKanpan_RandomActorActions
FuneKanpan_RandomActorActions:
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_DeckEventActions
FuneKanpan_DeckEventActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02560000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000022
	.4byte 0x020080b5
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02540000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_LeadActionsA
FuneKanpan_LeadActionsA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_LeadActionsB
FuneKanpan_LeadActionsB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_CrewActionsA
FuneKanpan_CrewActionsA:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x020080c5
	.global FuneKanpan_CrewActionsB
FuneKanpan_CrewActionsB:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x020082ed
	.global FuneKanpan_CrewActionsC
FuneKanpan_CrewActionsC:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008515
	.global FuneKanpan_CrewActionsD
FuneKanpan_CrewActionsD:
	.4byte 0x00000022
	.4byte 0x02008571
	.global FuneKanpan_CrewActionsE
FuneKanpan_CrewActionsE:
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000b333
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000b333
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000f5c
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000147a
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global FuneKanpan_SailorActions
FuneKanpan_SailorActions:
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_DeckActionsB
FuneKanpan_DeckActionsB:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008599
	.global FuneKanpan_DeckActionsA
FuneKanpan_DeckActionsA:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008725
	.global FuneKanpan_DeckActionsC
FuneKanpan_DeckActionsC:
	.4byte 0x00000022
	.4byte 0x0200889d
	.global FuneKanpan_RosterActions
FuneKanpan_RosterActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00520000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global FuneKanpan_LeadActionsC
FuneKanpan_LeadActionsC:
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x020088e1
	.global FuneKanpan_SceneTableA
FuneKanpan_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x000000b3
	.4byte 0x4000028d
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0x40000258
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000e8
	.4byte 0x00000298
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000000d8
	.4byte 0x40000328
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000400d8
	.4byte 0x40000288
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0xfffc0138
	.4byte 0x80000290
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0xfffc0078
	.4byte 0x00000290
	.4byte 0x00410000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000000e8
	.4byte 0x00000298
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000d4
	.4byte 0x00000209
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x000000c0
	.4byte 0x0000028a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x000000e8
	.4byte 0x0000028e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x000000d8
	.4byte 0x00000291
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x000000d8
	.4byte 0x40000280
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x000000d8
	.4byte 0xc000021c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x000000d8
	.4byte 0xc000021c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0012
	.4byte 0x000000c4
	.4byte 0x4000028c
	.4byte 0x00410000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0013
	.4byte 0x000000d4
	.4byte 0x40000209
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_SceneTableB
FuneKanpan_SceneTableB:
	.4byte 0x001800d0
	.4byte 0x00e0027a
	.4byte 0x028a0028
	.4byte 0x0005ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_SceneTableC
FuneKanpan_SceneTableC:
	.4byte 0x0000006d
	.4byte 0x0010106f
	.4byte 0x0020306f
	.4byte 0x0030506f
	.4byte 0x0040206b
	.4byte 0x0050106e
	.4byte 0x00601070
	.4byte 0x0074a002
	.4byte 0x00a0a06e
	.4byte 0x00b0e06f
	.4byte 0x00c0f06f
	.4byte 0x00d1106f
	.4byte 0x00e1306f
	.4byte 0x00f46002
	.4byte 0x0101606f
	.4byte 0x0111806f
	.4byte 0x000001ff
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x010a0000
	.4byte 0x00000000
	.4byte 0x02bf0000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00b70000
	.4byte 0x00000000
	.4byte 0x02d00000
	.4byte 0x0001d000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x0002b000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x02c20000
	.4byte 0x00015000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00ac0000
	.4byte 0x00000000
	.4byte 0x02c20000
	.4byte 0x00000000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00023000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x025a0000
	.4byte 0x00003000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00ec0000
	.4byte 0x00000000
	.4byte 0x026d0000
	.4byte 0x0000b000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x022c0000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScript
FuneKanpan_CrewScript:
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x0002b000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x0000b000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x0000b000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptB
FuneKanpan_CrewScriptB:
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptC
FuneKanpan_CrewScriptC:
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c6
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00d6
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptD
FuneKanpan_CrewScriptD:
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00c5
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptE
FuneKanpan_CrewScriptE:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x02820000
	.4byte 0x0000b000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x02160000
	.4byte 0x0000b000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0xffff00c2
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00bc0000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00003000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x00013000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x02a40000
	.4byte 0x0001d000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c29
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte 0x02008c29
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c29
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x09230014
	.4byte 0x00001d24
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001d8f
	.4byte 0x00008d15
	.4byte 0x09230014
	.4byte 0x00001d25
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001d90
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001d4a
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008a49
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008af1
	.4byte 0x00000000
	.4byte 0x09220019
	.4byte 0x00001d38
	.4byte 0x00000000
	.4byte 0x09250019
	.4byte 0x00001d71
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001e0a
	.4byte 0x00000000
	.4byte 0x0922001a
	.4byte 0x00001d39
	.4byte 0x00000000
	.4byte 0x0925001a
	.4byte 0x00001d72
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001e0b
	.4byte 0x00000000
	.4byte 0x0922001b
	.4byte 0x00001d3a
	.4byte 0x00000000
	.4byte 0x0925001b
	.4byte 0x00001d73
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001e0c
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001d4c
	.4byte 0x00008d15
	.4byte 0x09220015
	.4byte 0x00001d3b
	.4byte 0x00008d15
	.4byte 0x09250015
	.4byte 0x00001d74
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001e0d
	.4byte 0x00008d15
	.4byte 0x09220018
	.4byte 0x00001d3c
	.4byte 0x00008d15
	.4byte 0x09250018
	.4byte 0x00001d75
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001e0e
	.4byte 0x00008d15
	.4byte 0x09220019
	.4byte 0x00001d3d
	.4byte 0x00008d15
	.4byte 0x09250019
	.4byte 0x00001d76
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001e0f
	.4byte 0x00008d15
	.4byte 0x0922001a
	.4byte 0x00001d3e
	.4byte 0x00008d15
	.4byte 0x0925001a
	.4byte 0x00001d77
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e10
	.4byte 0x00008d15
	.4byte 0x0922001b
	.4byte 0x00001d3f
	.4byte 0x00008d15
	.4byte 0x0925001b
	.4byte 0x00001d78
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001e11
	.4byte 0x00000002
	.4byte 0x0920000a
	.4byte 0x02008ca1
	.4byte 0x00000002
	.4byte 0x0923000b
	.4byte 0x020090a1
	.4byte 0x00000002
	.4byte 0x0923000c
	.4byte 0x020091c9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c29
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte 0x02008c29
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c29
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001e77
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001eb0
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001eb1
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001e94
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001eb4
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001eb5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c29
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte 0x02008c29
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c29
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001eff
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008b99
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f03
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001f04
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001f05
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001f06
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008c29
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte 0x02008c29
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c29
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001f54
	.4byte 0x00000000
	.4byte 0x09030016
	.4byte 0x0200bc89
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f5c
	.4byte 0x00000000
	.4byte 0x09030015
	.4byte 0x0200bc89
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001f5d
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001f5e
	.4byte 0x00008d15
	.4byte 0x09030016
	.4byte 0x00001f5f
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001f61
	.4byte 0x00008d15
	.4byte 0x09030015
	.4byte 0x00001f60
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001f62
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_FlagGroupEntries
FuneKanpan_FlagGroupEntries:
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
