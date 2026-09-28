.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	adds r5, r0, #0
	adds r5, #100
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	beq .L_02000030_0
	cmp r3, #2
	bgt .L_02000030_1
	cmp r3, #0
	beq .L_02000030_2
	b .L_02000030_3
.L_02000030_1:
	cmp r3, #4
	beq .L_02000030_4
	cmp r3, #6
	bne .L_02000030_3
	ldr r3, [r0, #24]
	ldr r2, [pc, #84]
	adds r3, r3, r2
	str r3, [r0, #24]
	movs r2, #128
	ldr r3, [r0, #28]
	lsls r2, r2, #6
	b .L_02000030_5
.L_02000030_4:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #64]
	b .L_02000030_6
.L_02000030_0:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #52]
.L_02000030_6:
	ldr r3, [r0, #28]
.L_02000030_5:
	adds r3, r3, r2
	str r3, [r0, #28]
	b .L_02000030_3
.L_02000030_2:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	str r3, [r0, #28]
	bl 0x0200c26c
	movs r1, #90
	bl 0x0200c254
	adds r0, #60
	strh r0, [r5]
.L_02000030_3:
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0xfffff000
	.4byte 0xfffff800
	.global Func_020000b4
	.thumb_func
Func_020000b4:
	push {lr}
	movs r1, #1
	bl 0x0200c2a4
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
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
	.global Func_02000514
	.thumb_func
Func_02000514:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #102
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_02000514_0
	bl 0x0200c26c
	ldr r3, [r5, #12]
	lsls r0, r0, #15
	ldr r2, [pc, #60]
	lsrs r0, r0, #16
	subs r3, r3, r0
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #11
	str r3, [r5, #12]
	cmp r3, r2
	bge .L_02000514_1
	movs r3, #0
	b .L_02000514_2
.L_02000514_0:
	bl 0x0200c26c
	ldr r3, [r5, #12]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r0
	adds r3, r3, r2
	movs r2, #192
	lsls r2, r2, #12
	str r3, [r5, #12]
	cmp r3, r2
	ble .L_02000514_1
	movs r3, #1
.L_02000514_2:
	strh r3, [r6]
.L_02000514_1:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xffff8000
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x0200c26c
	lsls r0, r0, #5
	lsrs r0, r0, #16
	cmp r0, #6
	bne .L_02000570_0
	movs r3, #208
	b .L_02000570_1
.L_02000570_0:
	cmp r0, #9
	bne .L_02000570_2
	movs r3, #176
.L_02000570_1:
	lsls r3, r3, #8
	strh r3, [r5, #6]
.L_02000570_2:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000598
	.thumb_func
Func_02000598:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #100
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #9
	bls .L_02000598_0
	b .L_02000598_1
.L_02000598_0:
	ldr r2, [pc, #356]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	strh r4, [r3, #46]
	lsls r0, r0, #8
	strh r4, [r6, #46]
	lsls r0, r0, #8
	strh r2, [r7, #46]
	lsls r0, r0, #8
	strh r4, [r3, #48]
	lsls r0, r0, #8
	strh r2, [r4, #48]
	lsls r0, r0, #8
	strh r2, [r3, #52]
	lsls r0, r0, #8
	strh r0, [r4, #52]
	lsls r0, r0, #8
	strh r2, [r4, #54]
	lsls r0, r0, #8
	strh r0, [r5, #54]
	lsls r0, r0, #8
	strh r4, [r0, #56]
	lsls r0, r0, #8
	bl 0x0200c26c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	cmp r3, #0
	beq .L_02000598_2
	b .L_02000598_1
.L_02000598_2:
	ldrh r3, [r6]
	adds r3, #1
	b .L_02000598_3
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe085
	.2byte 0x2380
	.2byte 0x02db
	.2byte 0x62ab
	.2byte 0x632b
	.2byte 0x2380
	.2byte 0x029b
	.2byte 0x2184
	.2byte 0x636b
	.2byte 0x1c28
	.2byte 0x4b41
	.2byte 0x0449
	.2byte 0x2200
	.2byte 0xf003
	.2byte 0xfe3b
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe074
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe071
	.2byte 0x2280
	.2byte 0x6bab
	.2byte 0x0612
	.2byte 0x4293
	.2byte 0xd16d
	.2byte 0x6bea
	.2byte 0x429a
	.2byte 0xd16a
	.2byte 0x6c2b
	.2byte 0x4293
	.2byte 0xd167
	.2byte 0x8833
	.2byte 0x2098
	.2byte 0x3301
	.2byte 0x8033
	.2byte 0xf003
	.2byte 0xff0c
	.2byte 0x1c2b
	.2byte 0x3363
	.2byte 0x781b
	.2byte 0x2b00
	.2byte 0xd006
	.2byte 0x21b0
	.2byte 0x2015
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0xf003
	.2byte 0xfebd
	.2byte 0xe005
	.2byte 0x21a0
	.2byte 0x2015
	.2byte 0x01c9
	.2byte 0x2200
	.2byte 0xf003
	.2byte 0xfeb6
	.2byte 0xf003
	.2byte 0xfe00
	.2byte 0x0080
	.2byte 0x0c00
	.2byte 0x2800
	.2byte 0xd006
	.2byte 0x2015
	.2byte 0xf003
	.2byte 0xfe49
	.2byte 0x2380
	.2byte 0x029b
	.2byte 0x6283
	.2byte 0xe042
	.2byte 0x2015
	.2byte 0x4924
	.2byte 0x2200
	.2byte 0xf003
	.2byte 0xfeb0
	.2byte 0x2015
	.2byte 0xf003
	.2byte 0xfe3d
	.2byte 0x23c0
	.2byte 0x02db
	.2byte 0x6283
	.2byte 0xe036
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe032
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0x8033
	.2byte 0x2380
	.2byte 0x02db
	.2byte 0x62ab
	.2byte 0x2380
	.2byte 0x029b
	.2byte 0x632b
	.2byte 0x2380
	.2byte 0x025b
	.2byte 0x636b
	.2byte 0x1c2b
	.2byte 0x3363
	.2byte 0x781b
	.2byte 0x2b00
	.2byte 0xd007
	.2byte 0x21fc
	.2byte 0x1c28
	.2byte 0x0409
	.2byte 0x2200
	.2byte 0x4b14
	.2byte 0xf003
	.2byte 0xfdde
	.2byte 0xe01a
	.2byte 0x2180
	.2byte 0x1c28
	.2byte 0x0449
	.2byte 0x2200
	.2byte 0x4b11
	.2byte 0xf003
	.2byte 0xfdd6
	.2byte 0xe012
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe00e
	.2byte 0x2280
	.2byte 0x6bab
	.2byte 0x0612
	.2byte 0x4293
	.2byte 0xd10a
	.2byte 0x6bea
	.2byte 0x429a
	.2byte 0xd107
	.2byte 0x6c2b
	.2byte 0x4293
	.2byte 0xd104
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe000
	.2byte 0x2300
.L_02000598_3:
	strh r3, [r6]
.L_02000598_1:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x020085b4
	.2byte 0x0000
	.2byte 0x0296
	.2byte 0x0103
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0286
	.2byte 0x0000
	.2byte 0x02ae
	.global Func_02000724
	.thumb_func
Func_02000724:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #100
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #9
	bls .L_02000724_0
	b .L_02000724_1
.L_02000724_0:
	ldr r2, [pc, #348]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	strh r0, [r5, #58]
	lsls r0, r0, #8
	strh r0, [r0, #60]
	lsls r0, r0, #8
	strh r6, [r0, #60]
	lsls r0, r0, #8
	strh r2, [r5, #60]
	lsls r0, r0, #8
	strh r0, [r6, #60]
	lsls r0, r0, #8
	ldrh r6, [r4]
	lsls r0, r0, #8
	ldrh r4, [r5]
	lsls r0, r0, #8
	ldrh r6, [r4, #2]
	lsls r0, r0, #8
	ldrh r4, [r5, #2]
	lsls r0, r0, #8
	ldrh r0, [r1, #4]
	lsls r0, r0, #8
	bl 0x0200c26c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	cmp r3, #0
	beq .L_02000724_2
	b .L_02000724_1
.L_02000724_2:
	ldrh r3, [r6]
	adds r3, #1
	b .L_02000724_3
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe081
	.2byte 0x2380
	.2byte 0x02db
	.2byte 0x62ab
	.2byte 0x632b
	.2byte 0x2380
	.2byte 0x029b
	.2byte 0x636b
	.2byte 0x21b0
	.2byte 0x23ae
	.2byte 0x049b
	.2byte 0x1c28
	.2byte 0x0409
	.2byte 0x2200
	.2byte 0xf003
	.2byte 0xfd74
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe06f
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe06c
	.2byte 0x2280
	.2byte 0x6bab
	.2byte 0x0612
	.2byte 0x4293
	.2byte 0xd168
	.2byte 0x6bea
	.2byte 0x429a
	.2byte 0xd165
	.2byte 0x6c2b
	.2byte 0x4293
	.2byte 0xd162
	.2byte 0x8833
	.2byte 0x2098
	.2byte 0x3301
	.2byte 0x8033
	.2byte 0xf003
	.2byte 0xfe45
	.2byte 0x1c2b
	.2byte 0x3363
	.2byte 0x781b
	.2byte 0x2b00
	.2byte 0xd006
	.2byte 0x21d0
	.2byte 0x2016
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0xf003
	.2byte 0xfdf6
	.2byte 0xe004
	.2byte 0x2016
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf003
	.2byte 0xfdf0
	.2byte 0xf003
	.2byte 0xfd3a
	.2byte 0x0080
	.2byte 0x0c00
	.2byte 0x2800
	.2byte 0xd006
	.2byte 0x2016
	.2byte 0xf003
	.2byte 0xfd83
	.2byte 0x2380
	.2byte 0x029b
	.2byte 0x6283
	.2byte 0xe03e
	.2byte 0x2016
	.2byte 0x4921
	.2byte 0x2200
	.2byte 0xf003
	.2byte 0xfdea
	.2byte 0x2016
	.2byte 0xf003
	.2byte 0xfd77
	.2byte 0x23c0
	.2byte 0x02db
	.2byte 0x6283
	.2byte 0xe032
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe02e
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0x8033
	.2byte 0x2380
	.2byte 0x02db
	.2byte 0x62ab
	.2byte 0x2380
	.2byte 0x029b
	.2byte 0x632b
	.2byte 0x2380
	.2byte 0x025b
	.2byte 0x636b
	.2byte 0x1c2b
	.2byte 0x3363
	.2byte 0x781b
	.2byte 0x2b00
	.2byte 0xd002
	.2byte 0x21b8
	.2byte 0x23a8
	.2byte 0xe001
	.2byte 0x21ca
	.2byte 0x23ad
	.2byte 0x1c28
	.2byte 0x0409
	.2byte 0x2200
	.2byte 0x049b
	.2byte 0xf003
	.2byte 0xfd14
	.2byte 0xe012
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe00e
	.2byte 0x2280
	.2byte 0x6bab
	.2byte 0x0612
	.2byte 0x4293
	.2byte 0xd10a
	.2byte 0x6bea
	.2byte 0x429a
	.2byte 0xd107
	.2byte 0x6c2b
	.2byte 0x4293
	.2byte 0xd104
	.2byte 0x8833
	.2byte 0x3301
	.2byte 0xe000
	.2byte 0x2300
.L_02000724_3:
	strh r3, [r6]
.L_02000724_1:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x02008740
	.2byte 0x0103
	.2byte 0x0000
	.global Func_0200089c
	.thumb_func
Func_0200089c:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x0200c26c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	cmp r3, #0
	bne .L_0200089c_0
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
.L_0200089c_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020008c0
	.thumb_func
Func_020008c0:
	push {lr}
	movs r2, #128
	ldr r3, [r0, #24]
	lsls r2, r2, #9
	cmp r3, r2
	ble .L_020008c0_0
	ldr r2, [pc, #12]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
.L_020008c0_0:
	pop {r0}
	bx r0
	.4byte 0xfffff800
	.global Func_020008e0
	.thumb_func
Func_020008e0:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #98
	ldrb r3, [r6]
	adds r7, r3, #0
	cmp r7, #0
	beq .L_020008e0_0
	adds r3, #255
	b .L_020008e0_1
.L_020008e0_0:
	bl 0x0200c26c
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	lsrs r3, r3, #16
	cmp r3, #200
	bls .L_020008e0_2
	movs r3, #208
	lsls r3, r3, #8
	strh r3, [r5, #6]
	b .L_020008e0_3
.L_020008e0_2:
	cmp r3, #100
	bls .L_020008e0_4
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r5, #6]
	b .L_020008e0_3
.L_020008e0_4:
	strh r7, [r5, #6]
.L_020008e0_3:
	bl 0x0200c26c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r3, #80
.L_020008e0_1:
	strb r3, [r6]
	movs r0, #1
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000938
	.thumb_func
Func_02000938:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c994
	.global Func_02000940
	.thumb_func
Func_02000940:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200cb44
	.global Func_02000948
	.thumb_func
Func_02000948:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200cb64
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
	.global Func_02000a48
	.thumb_func
Func_02000a48:
	push {r5, lr}
	bl 0x0200c2ec
	ldr r0, [pc, #132]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000a48_0
	ldr r0, [pc, #124]
	bl 0x0200c3ac
	movs r0, #21
	movs r1, #0
	bl 0x0200c3bc
	b .L_02000a48_1
.L_02000a48_0:
	ldr r0, [pc, #112]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000a48_2
	movs r1, #2
	movs r0, #21
	bl 0x0200c394
	ldr r0, [pc, #100]
	bl 0x0200c3ac
	movs r1, #0
	movs r0, #21
	bl 0x0200c3bc
	movs r0, #21
	bl 0x0200c30c
	adds r5, r0, #0
	bl 0x0200c26c
	movs r3, #90
	muls r3, r0
	lsrs r3, r3, #16
	adds r3, #60
	adds r5, #100
	strh r3, [r5]
	ldr r1, [pc, #64]
	movs r0, #21
	bl 0x0200c324
	b .L_02000a48_1
.L_02000a48_2:
	movs r2, #0
	movs r0, #21
	ldr r1, [pc, #56]
	bl 0x0200c3ec
	movs r1, #3
	movs r0, #21
	bl 0x0200c38c
	ldr r0, [pc, #44]
	bl 0x0200c3ac
	movs r0, #21
	movs r1, #0
	bl 0x0200c3bc
.L_02000a48_1:
	bl 0x0200c2f4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000925
	.4byte 0x00001e08
	.4byte 0x00000922
	.4byte 0x00001d6f
	.4byte 0x0200c4d8
	.4byte 0x00000103
	.4byte 0x00001d36
	.global Func_02000af0
	.thumb_func
Func_02000af0:
	push {r5, lr}
	bl 0x0200c2ec
	ldr r0, [pc, #132]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000af0_0
	ldr r0, [pc, #124]
	bl 0x0200c3ac
	movs r0, #24
	movs r1, #0
	bl 0x0200c3bc
	b .L_02000af0_1
.L_02000af0_0:
	ldr r0, [pc, #112]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02000af0_2
	movs r1, #2
	movs r0, #24
	bl 0x0200c394
	ldr r0, [pc, #100]
	bl 0x0200c3ac
	movs r1, #0
	movs r0, #24
	bl 0x0200c3bc
	movs r0, #24
	bl 0x0200c30c
	adds r5, r0, #0
	bl 0x0200c26c
	movs r3, #90
	muls r3, r0
	lsrs r3, r3, #16
	adds r3, #60
	adds r5, #100
	strh r3, [r5]
	ldr r1, [pc, #64]
	movs r0, #24
	bl 0x0200c324
	b .L_02000af0_1
.L_02000af0_2:
	movs r2, #0
	movs r0, #24
	ldr r1, [pc, #56]
	bl 0x0200c3ec
	movs r1, #3
	movs r0, #24
	bl 0x0200c38c
	ldr r0, [pc, #44]
	bl 0x0200c3ac
	movs r0, #24
	movs r1, #0
	bl 0x0200c3bc
.L_02000af0_1:
	bl 0x0200c2f4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000925
	.4byte 0x00001e09
	.4byte 0x00000922
	.4byte 0x00001d70
	.4byte 0x0200c4d8
	.4byte 0x00000103
	.4byte 0x00001d37
	.global Func_02000b98
	.thumb_func
Func_02000b98:
	push {lr}
	bl 0x0200c2ec
	ldr r0, [pc, #20]
	bl 0x0200c3ac
	movs r1, #0
	movs r0, #21
	bl 0x0200c3cc
	bl 0x0200c2f4
	pop {r0}
	bx r0
	.4byte 0x00001f00
	.global Func_02000bb8
	.thumb_func
Func_02000bb8:
	push {lr}
	ldr r0, [pc, #48]
	sub sp, #8
	bl 0x0200c2cc
	cmp r0, #0
	bne .L_02000bb8_0
	movs r0, #158
	bl 0x0200c45c
	movs r3, #1
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #94
	movs r2, #13
	movs r3, #94
	bl 0x0200c294
	ldr r0, [pc, #8]
	bl 0x0200c2d4
.L_02000bb8_0:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000271
	.global Func_02000bf0
	.thumb_func
Func_02000bf0:
	push {lr}
	ldr r0, [pc, #48]
	sub sp, #8
	bl 0x0200c2cc
	cmp r0, #0
	bne .L_02000bf0_0
	movs r0, #158
	bl 0x0200c45c
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #108
	movs r2, #13
	movs r3, #108
	bl 0x0200c294
	ldr r0, [pc, #8]
	bl 0x0200c2d4
.L_02000bf0_0:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000272
	.global Func_02000c28
	.thumb_func
Func_02000c28:
	push {r5, r6, lr}
	ldr r3, [pc, #104]
	ldr r6, [r3]
	bl 0x0200c2ec
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r5, #0
	cmp r3, #1
	beq .L_02000c28_0
	cmp r3, #3
	beq .L_02000c28_1
	b .L_02000c28_2
.L_02000c28_0:
	movs r5, #1
	bl 0x02008bb8
	b .L_02000c28_2
.L_02000c28_1:
	movs r5, #1
	bl 0x02008bf0
.L_02000c28_2:
	cmp r5, #0
	beq .L_02000c28_3
	movs r0, #0
	ldr r1, [pc, #56]
	ldr r2, [pc, #60]
	bl 0x0200c31c
	movs r2, #10
	movs r0, #0
	movs r1, #1
	negs r2, r2
	bl 0x0200c364
	movs r0, #10
	bl 0x0200c2e4
	b .L_02000c28_4
.L_02000c28_3:
	movs r0, #123
	bl 0x0200c45c
.L_02000c28_4:
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl 0x0200c424
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00009999
	.4byte 0x00004ccc
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
	.global Func_020010a0
	.thumb_func
Func_020010a0:
	push {r5, r6, lr}
	ldr r0, [pc, #252]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_020010a0_0
	ldr r0, [pc, #244]
	bl 0x0200c2cc
	cmp r0, #0
	bne .L_020010a0_0
	bl 0x0200c2ec
	bl 0x0200c454
	bl 0x020092f0
	ldr r1, [pc, #228]
	ldr r2, [pc, #228]
	movs r0, #20
	bl 0x0200c31c
	movs r0, #20
	bl 0x0200c30c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #204
	lsls r2, r2, #2
	strb r3, [r0]
	movs r1, #232
	movs r0, #20
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c2e4
	movs r0, #20
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl 0x0200c2e4
	movs r1, #2
	movs r0, #20
	bl 0x0200c38c
	movs r0, #20
	bl 0x0200b9ec
	ldr r1, [pc, #152]
	ldr r2, [pc, #156]
	movs r0, #20
	bl 0x0200c31c
	movs r0, #20
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #201
	ands r5, r3
	movs r1, #244
	lsls r2, r2, #2
	strb r5, [r0]
	movs r0, #20
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c2e4
	movs r0, #20
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r0, #20
	bl 0x0200c2e4
	movs r0, #20
	ldr r1, [pc, #96]
	ldr r2, [pc, #100]
	bl 0x0200c31c
	movs r0, #20
	movs r1, #248
	ldr r2, [pc, #92]
	bl 0x0200c35c
	movs r2, #175
	movs r0, #20
	movs r1, #248
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r1, #246
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200c3d4
	movs r0, #0
	ldr r1, [pc, #52]
	movs r2, #60
	bl 0x0200c3ec
	bl 0x0200c2f4
.L_020010a0_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000911
	.4byte 0x00000922
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00033333
	.4byte 0x00019999
	.4byte 0x0000030a
	.4byte 0x00000101
	.global Func_020011c8
	.thumb_func
Func_020011c8:
	push {r5, r6, lr}
	ldr r0, [pc, #252]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_020011c8_0
	ldr r0, [pc, #244]
	bl 0x0200c2cc
	cmp r0, #0
	bne .L_020011c8_0
	bl 0x0200c2ec
	bl 0x0200c454
	bl 0x020092f0
	ldr r1, [pc, #228]
	ldr r2, [pc, #228]
	movs r0, #20
	bl 0x0200c31c
	movs r0, #20
	bl 0x0200c30c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #204
	lsls r2, r2, #2
	strb r3, [r0]
	movs r1, #202
	movs r0, #20
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c2e4
	movs r0, #20
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl 0x0200c2e4
	movs r1, #2
	movs r0, #20
	bl 0x0200c38c
	movs r0, #20
	bl 0x0200b9ec
	ldr r1, [pc, #152]
	ldr r2, [pc, #156]
	movs r0, #20
	bl 0x0200c31c
	movs r0, #20
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #201
	ands r5, r3
	movs r1, #192
	lsls r2, r2, #2
	strb r5, [r0]
	movs r0, #20
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c2e4
	movs r0, #20
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r0, #20
	bl 0x0200c2e4
	movs r0, #20
	ldr r1, [pc, #96]
	ldr r2, [pc, #100]
	bl 0x0200c31c
	movs r0, #20
	movs r1, #180
	ldr r2, [pc, #92]
	bl 0x0200c35c
	movs r2, #175
	movs r0, #20
	movs r1, #180
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r1, #246
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200c3d4
	movs r0, #0
	ldr r1, [pc, #52]
	movs r2, #60
	bl 0x0200c3ec
	bl 0x0200c2f4
.L_020011c8_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000911
	.4byte 0x00000922
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00033333
	.4byte 0x00019999
	.4byte 0x0000030a
	.4byte 0x00000101
	.global Func_020012f0
	.thumb_func
Func_020012f0:
	push {lr}
	ldr r0, [pc, #196]
	ldr r1, [pc, #196]
	sub sp, #8
	bl 0x0200c40c
	movs r0, #216
	movs r1, #1
	movs r2, #206
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #16
	bl 0x0200c414
	bl 0x0200c41c
	movs r0, #20
	bl 0x0200c2e4
	bl 0x02008bf0
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #108
	movs r1, #108
	movs r2, #13
	movs r0, #30
	bl 0x0200c294
	movs r0, #10
	bl 0x0200c2e4
	movs r1, #216
	movs r2, #200
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r0, #20
	ldr r1, [pc, #120]
	ldr r2, [pc, #120]
	bl 0x0200c31c
	movs r0, #20
	movs r1, #216
	ldr r2, [pc, #116]
	bl 0x0200c35c
	movs r2, #10
	movs r0, #0
	movs r1, #20
	bl 0x0200c39c
	movs r0, #20
	movs r1, #4
	bl 0x0200c37c
	movs r0, #20
	movs r1, #2
	bl 0x0200c38c
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c3ec
	movs r2, #20
	movs r0, #20
	movs r1, #0
	bl 0x0200c39c
	movs r1, #2
	movs r0, #20
	bl 0x0200c38c
	ldr r0, [pc, #56]
	bl 0x0200c3ac
	movs r0, #20
	movs r1, #0
	movs r2, #20
	bl 0x0200c3c4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl 0x0200c3ec
	ldr r0, [pc, #32]
	bl 0x0200c2d4
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x0000032e
	.4byte 0x00001d8d
	.4byte 0x00000923
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
	.global Func_02001444
	.thumb_func
Func_02001444:
	push {r5, lr}
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200c2d4
	ldr r0, [pc, #496]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_0
	ldr r0, [pc, #488]
	bl 0x0200c2dc
	ldr r0, [pc, #488]
	bl 0x0200c2dc
.L_02001444_0:
	ldr r0, [pc, #484]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_1
	bl 0x0200b950
	bl 0x0200b9b8
	movs r0, #24
	movs r1, #2
	bl 0x0200c3a4
	b .L_02001444_2
.L_02001444_1:
	movs r0, #138
	lsls r0, r0, #4
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_3
	bl 0x0200b950
	bl 0x0200b9b8
	b .L_02001444_2
.L_02001444_3:
	ldr r0, [pc, #440]
	bl 0x0200c2cc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001444_4
	bl 0x0200b950
	bl 0x0200b284
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #420]
	bl 0x0200c264
	ldr r2, [pc, #416]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #4]
	ldr r2, [pc, #412]
	ldr r3, [pc, #416]
	movs r1, #200
	str r3, [r2, #4]
	ldr r0, [pc, #412]
	lsls r1, r1, #4
	bl 0x0200c264
	b .L_02001444_2
.L_02001444_4:
	ldr r0, [pc, #408]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_2
	bl 0x0200b950
	ldr r3, [pc, #376]
	str r5, [r3, #4]
	ldr r3, [pc, #376]
	movs r1, #200
	str r5, [r3, #4]
	ldr r0, [pc, #380]
	lsls r1, r1, #4
	bl 0x0200c264
.L_02001444_2:
	ldr r0, [pc, #352]
	bl 0x0200c2cc
	cmp r0, #0
	bne .L_02001444_5
	bl 0x0200b710
.L_02001444_5:
	ldr r3, [pc, #364]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #4
	cmp r3, #15
	bhi .L_02001444_6
	ldr r2, [pc, #352]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	str r5, [sp, #352]
	lsls r0, r0, #8
	str r5, [sp, #832]
	lsls r0, r0, #8
	str r5, [sp, #832]
	lsls r0, r0, #8
	str r5, [sp, #832]
	lsls r0, r0, #8
	str r5, [sp, #832]
	lsls r0, r0, #8
	str r5, [sp, #832]
	lsls r0, r0, #8
	str r5, [sp, #448]
	lsls r0, r0, #8
	str r5, [sp, #536]
	lsls r0, r0, #8
	str r5, [sp, #560]
	lsls r0, r0, #8
	str r5, [sp, #584]
	lsls r0, r0, #8
	str r5, [sp, #608]
	lsls r0, r0, #8
	str r5, [sp, #632]
	lsls r0, r0, #8
	str r5, [sp, #656]
	lsls r0, r0, #8
	str r5, [sp, #720]
	lsls r0, r0, #8
	str r5, [sp, #744]
	lsls r0, r0, #8
	str r5, [sp, #808]
	lsls r0, r0, #8
	movs r0, #0
	bl 0x0200c30c
	ldr r1, [r0, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	b .L_02001444_6
	.2byte 0x483d
	.2byte 0xf002
	.2byte 0xfeab
	.2byte 0x2800
	.2byte 0xd002
	.2byte 0xf000
	.2byte 0xfaed
	.2byte 0xe05b
	.2byte 0xf000
	.2byte 0xfa8a
	.2byte 0xe058
	.2byte 0xf000
	.2byte 0xfbc1
	.2byte 0xe055
	.2byte 0xf000
	.2byte 0xfc10
	.2byte 0xe052
	.2byte 0xf000
	.2byte 0xfcfd
	.2byte 0xe04f
	.2byte 0xf000
	.2byte 0xfe92
	.2byte 0xe04c
	.2byte 0xf001
	.2byte 0xf83b
	.2byte 0xe049
	.2byte 0x4826
	.2byte 0xf002
	.2byte 0xfe91
	.2byte 0x2800
	.2byte 0xd12a
	.2byte 0xf001
	.2byte 0xfa11
	.2byte 0xe041
	.2byte 0xf001
	.2byte 0xfae2
	.2byte 0xe03e
	.2byte 0x4821
	.2byte 0xf002
	.2byte 0xfe86
	.2byte 0x2800
	.2byte 0xd139
	.2byte 0xf001
	.2byte 0xfb5e
	.2byte 0xe036
	.2byte 0xf002
	.2byte 0xfa1f
	.2byte 0xe033
.L_02001444_6:
	ldr r0, [pc, #120]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_7
	ldr r3, [pc, #152]
	ldr r3, [r3]
	movs r2, #130
	adds r3, #236
	lsls r2, r2, #15
	str r2, [r3]
	b .L_02001444_8
.L_02001444_7:
	movs r0, #138
	lsls r0, r0, #4
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_9
	bl 0x020099c0
	b .L_02001444_8
.L_02001444_9:
	ldr r0, [pc, #124]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_10
	bl 0x02009920
	b .L_02001444_8
.L_02001444_10:
	ldr r0, [pc, #92]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_11
	bl 0x0200985c
	b .L_02001444_8
.L_02001444_11:
	ldr r0, [pc, #96]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_12
	bl 0x020097a0
	b .L_02001444_8
.L_02001444_12:
	ldr r0, [pc, #84]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001444_8
	bl 0x02009684
.L_02001444_8:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000109
	.4byte 0x00000271
	.4byte 0x00000272
	.4byte 0x0000093e
	.4byte 0x00000927
	.4byte 0x0200b4bd
	.4byte 0x0200db50
	.4byte 0x0200db60
	.4byte 0x00013333
	.4byte 0x0200b1a9
	.4byte 0x00000928
	.4byte 0x02000240
	.4byte 0x02009518
	.4byte 0x03001e70
	.4byte 0x0000092b
	.4byte 0x00000925
	.4byte 0x00000911
	.global Func_02001684
	.thumb_func
Func_02001684:
	push {r5, r6, lr}
	movs r0, #27
	movs r1, #1
	bl 0x0200c3e4
	movs r0, #23
	movs r1, #1
	bl 0x0200c3e4
	movs r0, #22
	movs r1, #1
	bl 0x0200c3e4
	movs r0, #26
	movs r1, #1
	bl 0x0200c3e4
	movs r0, #24
	movs r1, #1
	bl 0x0200c3e4
	movs r0, #146
	lsls r0, r0, #4
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001684_0
	movs r1, #162
	lsls r1, r1, #16
	ldr r2, [pc, #204]
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r1, #0
	movs r0, #23
	movs r2, #0
	bl 0x0200c36c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
.L_02001684_0:
	ldr r0, [pc, #168]
	bl 0x0200c2cc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001684_1
	movs r1, #132
	ldr r2, [pc, #156]
	lsls r1, r1, #17
	movs r0, #21
	bl 0x0200c36c
	movs r0, #21
	bl 0x0200c30c
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #21
	bl 0x0200c30c
	adds r5, r0, #0
	bl 0x0200c26c
	movs r1, #90
	bl 0x0200c254
	ldr r6, [pc, #120]
	adds r0, #60
	adds r5, #100
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #21
	bl 0x0200c324
	movs r1, #248
	movs r2, #170
	lsls r2, r2, #18
	lsls r1, r1, #16
	movs r0, #24
	bl 0x0200c36c
	movs r0, #24
	bl 0x0200c30c
	adds r5, r0, #0
	bl 0x0200c26c
	movs r1, #90
	bl 0x0200c254
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #24
	bl 0x0200c324
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	b .L_02001684_2
.L_02001684_1:
	ldr r0, [pc, #52]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001684_2
	movs r1, #246
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r0, #20
	bl 0x0200c30c
	strh r5, [r0, #6]
.L_02001684_2:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x029a0000
	.4byte 0x00000922
	.4byte 0x02be0000
	.4byte 0x0200c4d8
	.4byte 0x00000923
	.global Func_020017a0
	.thumb_func
Func_020017a0:
	push {r5, r6, lr}
	movs r1, #131
	lsls r1, r1, #17
	ldr r2, [pc, #164]
	movs r0, #21
	bl 0x0200c36c
	movs r0, #21
	bl 0x0200c30c
	movs r6, #160
	lsls r6, r6, #7
	movs r1, #164
	movs r2, #162
	lsls r2, r2, #18
	strh r6, [r0, #6]
	lsls r1, r1, #16
	movs r0, #24
	bl 0x0200c36c
	movs r0, #24
	bl 0x0200c30c
	movs r5, #0
	strh r5, [r0, #6]
	movs r1, #1
	movs r0, #24
	bl 0x0200c3e4
	movs r1, #198
	ldr r2, [pc, #112]
	lsls r1, r1, #16
	movs r0, #25
	bl 0x0200c36c
	movs r0, #25
	bl 0x0200c30c
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r1, #1
	movs r0, #25
	bl 0x0200c3e4
	movs r1, #188
	lsls r1, r1, #16
	ldr r2, [pc, #84]
	movs r0, #26
	bl 0x0200c36c
	movs r0, #26
	bl 0x0200c30c
	movs r3, #176
	lsls r3, r3, #8
	movs r1, #186
	strh r3, [r0, #6]
	lsls r1, r1, #16
	ldr r2, [pc, #64]
	movs r0, #27
	bl 0x0200c36c
	movs r0, #27
	bl 0x0200c30c
	movs r1, #0
	strh r6, [r0, #6]
	movs r2, #0
	movs r0, #22
	bl 0x0200c36c
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
.L_02001844:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x02c2
	.2byte 0x0000
	.2byte 0x0299
	.2byte 0x0000
	.2byte 0x02a6
	.2byte 0x0000
	.2byte 0x027b
	.global Func_0200185c
	.thumb_func
Func_0200185c:
	push {r5, lr}
	bl 0x0200c2ec
	ldr r0, [pc, #148]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r1, #238
	movs r0, #23
	lsls r1, r1, #16
	ldr r2, [pc, #124]
	bl 0x0200c36c
	movs r1, #204
	lsls r1, r1, #16
	ldr r2, [pc, #116]
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #12]
	movs r0, #22
	bl 0x0200c30c
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	ldr r2, [pc, #84]
	movs r0, #22
	ldr r1, [pc, #84]
	bl 0x0200c31c
	ldr r1, [pc, #84]
	movs r0, #22
	bl 0x0200c324
	movs r0, #21
	bl 0x0200c30c
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	ldr r1, [pc, #64]
	movs r0, #21
	ldr r2, [pc, #64]
	bl 0x0200c31c
	movs r0, #21
	ldr r1, [pc, #60]
	bl 0x0200c324
	ldr r0, [pc, #60]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_0200185c_0
	bl 0x0200c218
.L_0200185c_0:
	bl 0x0200c2f4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d160
	.4byte 0x02720000
	.4byte 0x02090000
	.4byte 0x00004ccc
	.4byte 0x00009999
	.4byte 0x0200c58c
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0200c628
	.4byte 0x00000109
	.global Func_02001920
	.thumb_func
Func_02001920:
	push {lr}
	bl 0x0200c2ec
	ldr r0, [pc, #120]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r1, #238
	movs r0, #23
	lsls r1, r1, #16
	ldr r2, [pc, #96]
	bl 0x0200c36c
	movs r1, #134
	ldr r2, [pc, #92]
	lsls r1, r1, #17
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r3, #0
	strh r3, [r0, #6]
	ldr r1, [pc, #76]
	movs r0, #22
	bl 0x0200c324
	movs r0, #21
	bl 0x0200c30c
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	ldr r1, [pc, #56]
	movs r0, #21
	ldr r2, [pc, #56]
	bl 0x0200c31c
	movs r0, #21
	ldr r1, [pc, #52]
	bl 0x0200c324
	ldr r0, [pc, #48]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_02001920_0
	bl 0x0200c218
.L_02001920_0:
	bl 0x0200c2f4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d160
	.4byte 0x02720000
	.4byte 0x02a60000
	.4byte 0x0200c980
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0200c628
	.4byte 0x00000109
	.global Func_020019c0
	.thumb_func
Func_020019c0:
	push {r5, lr}
	ldr r3, [pc, #188]
	ldr r3, [r3]
	movs r2, #130
	adds r3, #236
	lsls r2, r2, #15
	str r2, [r3]
	bl 0x0200c2ec
	ldr r0, [pc, #176]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r0, #24
	bl 0x0200c314
	movs r1, #238
	movs r0, #23
	lsls r1, r1, #16
	ldr r2, [pc, #156]
	bl 0x0200c36c
	movs r0, #23
	bl 0x0200c30c
	movs r5, #192
	lsls r5, r5, #6
	strh r5, [r0, #6]
	ldr r0, [pc, #140]
	bl 0x0200c2cc
	cmp r0, #0
	beq .L_020019c0_0
	movs r1, #162
	lsls r1, r1, #16
	ldr r2, [pc, #132]
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r1, #162
	movs r2, #169
	strh r5, [r0, #6]
	lsls r1, r1, #16
	movs r0, #21
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r0, #21
	bl 0x0200c30c
	movs r3, #208
	b .L_020019c0_1
.L_020019c0_0:
	movs r1, #160
	movs r2, #163
	lsls r1, r1, #16
	lsls r2, r2, #18
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r1, #166
	movs r2, #167
	strh r5, [r0, #6]
	lsls r1, r1, #16
	movs r0, #21
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r0, #21
	bl 0x0200c30c
	movs r3, #176
.L_020019c0_1:
	lsls r3, r3, #8
	strh r3, [r0, #6]
	ldr r3, [pc, #48]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	bne .L_020019c0_2
	bl 0x0200bf30
.L_020019c0_2:
	bl 0x0200c2f4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0x0200d418
	.4byte 0x02720000
	.4byte 0x00000903
	.4byte 0x027a0000
	.4byte 0x02000240
	.global Func_02001a98
	.thumb_func
Func_02001a98:
	push {lr}
	bl 0x0200c2ec
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c414
	movs r0, #1
	bl 0x0200c25c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r1, #0
	movs r2, #0
	movs r0, #23
	bl 0x0200c36c
	movs r0, #23
	bl 0x0200c30c
	movs r3, #192
	lsls r3, r3, #6
	movs r1, #232
	strh r3, [r0, #6]
	lsls r1, r1, #16
	ldr r2, [pc, #60]
	movs r0, #21
	bl 0x0200c36c
	movs r0, #21
	bl 0x0200c30c
	movs r3, #176
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r1, #1
	movs r0, #232
	movs r2, #159
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #0
	lsls r0, r0, #16
	bl 0x0200c414
	bl 0x0200c284
	movs r0, #1
	bl 0x0200c25c
	movs r0, #23
	movs r1, #21
	bl 0x02009c14
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x028a0000
	.global Func_02001b58
	.thumb_func
Func_02001b58:
	push {lr}
	bl 0x0200c2ec
	ldr r0, [pc, #172]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #232
	movs r2, #159
	lsls r2, r2, #18
	movs r0, #0
	lsls r1, r1, #16
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
	movs r1, #0
	movs r0, #0
	bl 0x0200c404
	bl 0x0200c284
	movs r0, #1
	bl 0x0200c25c
	movs r0, #22
	bl 0x0200c334
	movs r0, #21
	bl 0x0200c334
	movs r0, #1
	bl 0x0200c25c
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200c36c
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl 0x0200c36c
	movs r0, #20
	bl 0x0200c30c
	movs r3, #192
	lsls r3, r3, #6
	movs r1, #232
	strh r3, [r0, #6]
	lsls r1, r1, #16
	ldr r2, [pc, #40]
	movs r0, #23
	bl 0x0200c36c
	movs r0, #23
	bl 0x0200c30c
	movs r3, #176
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #1
	bl 0x0200c25c
	movs r0, #20
.L_02001c00:
	movs r1, #23
	bl 0x02009c14
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xd160
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x028a
	.global Func_02001c14
	.thumb_func
Func_02001c14:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r6, r1, #0
	movs r3, #224
	ldr r1, [pc, #208]
	lsls r3, r3, #1
	ldr r2, [r1]
	mov r10, r3
	mov r8, r1
	subs r3, #192
	mov r1, r10
	str r3, [r2, r1]
	adds r5, r0, #0
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c2e4
	bl 0x02008bb8
	movs r1, #216
	movs r2, #147
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	adds r0, r5, #0
	ldr r1, [pc, #160]
	ldr r2, [pc, #164]
	bl 0x0200c31c
	movs r2, #150
	adds r0, r5, #0
	movs r1, #216
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #151
	adds r0, r5, #0
	movs r1, #218
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #151
	adds r0, r5, #0
	movs r1, #234
	lsls r2, r2, #2
	bl 0x0200c35c
	adds r0, r5, #0
	movs r1, #236
	ldr r2, [pc, #120]
	bl 0x0200c35c
	movs r1, #160
	movs r2, #20
	adds r0, r5, #0
	lsls r1, r1, #7
	bl 0x0200c3d4
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200c37c
	movs r0, #20
	bl 0x0200c2e4
	movs r1, #160
	adds r0, r6, #0
	lsls r1, r1, #7
	bl 0x0200ba00
	movs r2, #40
	adds r0, r6, #0
	movs r1, #4
	bl 0x0200c384
	adds r0, r6, #0
	movs r1, #2
	bl 0x0200c38c
	ldr r0, [pc, #64]
	bl 0x0200c3ac
	adds r0, r6, #0
	movs r1, #0
	movs r2, #20
	bl 0x0200c3c4
	mov r3, r8
	ldr r2, [r3]
	ldr r3, [pc, #48]
	mov r1, r10
	str r3, [r2, r1]
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #10
	bl 0x0200c424
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000026a
	.4byte 0x00001e39
	.4byte 0x00000202
	.global Func_02001d0c
	.thumb_func
Func_02001d0c:
	push {lr}
	bl 0x0200c2ec
	ldr r0, [pc, #140]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #15
	movs r0, #0
	bl 0x0200c3a4
	movs r0, #0
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	ldr r3, [pc, #112]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c2e4
	movs r1, #1
	movs r0, #20
	bl 0x0200c394
	ldr r0, [pc, #80]
	bl 0x0200c3ac
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl 0x0200c3c4
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #22
	bl 0x0200ba00
	movs r2, #20
	movs r0, #22
	movs r1, #4
	bl 0x0200c384
	movs r0, #22
	movs r1, #2
	bl 0x0200c38c
	ldr r0, [pc, #40]
	movs r1, #0
	movs r2, #20
	bl 0x0200c3c4
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #11
	bl 0x0200c424
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d160
	.4byte 0x03001ebc
	.4byte 0x00001e41
	.4byte 0x00006016
	.global Func_02001db0
	.thumb_func
Func_02001db0:
	push {r5, lr}
	bl 0x0200c2ec
	ldr r0, [pc, #416]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	ldr r0, [pc, #408]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #248
	movs r2, #182
	lsls r2, r2, #18
	movs r0, #21
	lsls r1, r1, #16
	bl 0x0200c36c
	movs r1, #15
	movs r0, #0
	bl 0x0200c3a4
	movs r0, #0
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	ldr r3, [pc, #364]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200c43c
	movs r0, #21
	ldr r1, [pc, #348]
	ldr r2, [pc, #352]
	bl 0x0200c31c
	movs r2, #173
	movs r0, #21
	movs r1, #242
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r0, #21
	movs r1, #196
	ldr r2, [pc, #332]
	bl 0x0200c35c
	ldr r2, [pc, #332]
	movs r0, #21
	movs r1, #182
	bl 0x0200c35c
	movs r1, #2
	movs r0, #21
	bl 0x0200c394
	ldr r0, [pc, #316]
	bl 0x0200c3ac
	ldr r0, [pc, #316]
	bl 0x0200b9ec
	movs r0, #0
	ldr r1, [pc, #312]
	ldr r2, [pc, #312]
	bl 0x0200c31c
	movs r1, #154
	ldr r2, [pc, #308]
	movs r0, #0
	bl 0x0200c344
	movs r0, #146
	bl 0x0200c45c
	movs r0, #24
	bl 0x0200c30c
	movs r5, #0
	adds r0, #100
	strh r5, [r0]
	movs r0, #25
	bl 0x0200c30c
	adds r0, #100
	strh r5, [r0]
	movs r0, #26
	bl 0x0200c30c
	movs r1, #128
	adds r0, #100
	movs r2, #242
	strh r5, [r0]
	lsls r1, r1, #14
	movs r0, #24
	lsls r2, r2, #17
	bl 0x0200c36c
	movs r1, #168
	movs r2, #248
	movs r0, #25
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl 0x0200c36c
	movs r1, #128
	movs r2, #149
	movs r0, #26
	lsls r1, r1, #13
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r0, #24
	ldr r1, [pc, #212]
	ldr r2, [pc, #212]
	bl 0x0200c31c
	movs r0, #25
	ldr r1, [pc, #200]
	ldr r2, [pc, #204]
	bl 0x0200c31c
	ldr r2, [pc, #196]
	movs r0, #26
	ldr r1, [pc, #188]
	bl 0x0200c31c
	ldr r5, [pc, #196]
	movs r0, #24
	adds r1, r5, #0
	bl 0x0200c324
	movs r0, #25
	adds r1, r5, #0
	bl 0x0200c324
	movs r0, #26
	adds r1, r5, #0
	bl 0x0200c324
	movs r0, #24
	movs r1, #3
	bl 0x0200c3a4
	movs r0, #25
	movs r1, #3
	bl 0x0200c3a4
	movs r0, #26
	movs r1, #3
	bl 0x0200c3a4
.L_02001db0_0:
	movs r0, #1
	bl 0x0200c25c
	movs r0, #24
	bl 0x0200c30c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #0
	beq .L_02001db0_0
	bl 0x02008bb8
	movs r0, #21
	movs r2, #153
	movs r1, #196
	lsls r2, r2, #2
	bl 0x0200c354
	movs r0, #24
	bl 0x0200c32c
	movs r0, #10
	bl 0x0200c2e4
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #10
	bl 0x0200c2e4
	bl 0x0200c2b4
	movs r0, #21
	bl 0x0200c3dc
	ldr r0, [pc, #76]
	movs r1, #1
	movs r2, #0
	bl 0x0200c2c4
	bl 0x0200c2bc
	movs r0, #12
	bl 0x0200c424
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200d160
	.4byte 0x0200d208
	.4byte 0x03001ebc
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x000002a6
	.4byte 0x0000028e
	.4byte 0x00001e44
	.4byte 0x0000a015
	.4byte 0x00026666
	.4byte 0x00013333
	.4byte 0x00000261
	.4byte 0x0200c4ec
	.4byte 0x00001e45
	.global Func_02001f90
	.thumb_func
Func_02001f90:
	push {r5, r6, lr}
	bl 0x0200c2ec
	movs r1, #15
	movs r0, #0
	bl 0x0200c3a4
	movs r0, #0
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	ldr r0, [pc, #728]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	ldr r0, [pc, #720]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #0
	movs r0, #31
	bl 0x0200c374
	movs r0, #24
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	movs r0, #25
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	movs r0, #26
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	movs r0, #27
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	movs r0, #28
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	movs r0, #29
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	movs r1, #128
	movs r2, #160
	lsls r2, r2, #18
	movs r0, #22
	lsls r1, r1, #17
	bl 0x0200c36c
	ldr r5, [pc, #616]
	movs r0, #22
	adds r1, r5, #0
	bl 0x0200c324
	movs r1, #134
	movs r2, #173
	lsls r2, r2, #18
	movs r0, #21
	lsls r1, r1, #17
	bl 0x0200c36c
	adds r1, r5, #0
	movs r0, #22
	bl 0x0200c324
	movs r1, #242
	movs r2, #151
	movs r0, #24
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r1, #132
	movs r2, #150
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r1, #254
	movs r2, #167
	movs r0, #26
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r1, #141
	ldr r2, [pc, #544]
	lsls r1, r1, #17
	movs r0, #27
	bl 0x0200c36c
	movs r0, #24
	bl 0x0200c30c
	movs r6, #0
	adds r0, #99
	strb r6, [r0]
	movs r0, #25
	bl 0x0200c30c
	movs r5, #1
	adds r0, #99
	strb r5, [r0]
	movs r0, #26
	bl 0x0200c30c
	adds r0, #99
	strb r6, [r0]
	movs r0, #27
	bl 0x0200c30c
	adds r0, #99
	strb r5, [r0]
	ldr r5, [pc, #496]
	movs r0, #24
	adds r1, r5, #0
	bl 0x0200c324
	adds r1, r5, #0
	movs r0, #25
	bl 0x0200c324
	ldr r5, [pc, #480]
	movs r0, #26
	adds r1, r5, #0
	bl 0x0200c324
	adds r1, r5, #0
	movs r0, #27
	bl 0x0200c324
	movs r1, #0
	movs r0, #20
	movs r2, #0
	bl 0x0200c36c
	ldr r3, [pc, #456]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #200
	lsls r0, r0, #1
	bl 0x0200c2e4
	movs r1, #254
	movs r2, #185
	movs r0, #28
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #13
	ldr r2, [pc, #412]
	bl 0x0200c36c
	movs r0, #28
	ldr r1, [pc, #408]
	ldr r2, [pc, #408]
	bl 0x0200c31c
	movs r0, #29
	ldr r1, [pc, #396]
	ldr r2, [pc, #400]
	bl 0x0200c31c
	movs r2, #161
	movs r0, #29
	movs r1, #172
	lsls r2, r2, #2
	bl 0x0200c344
	movs r2, #165
	movs r0, #28
	movs r1, #200
	lsls r2, r2, #2
	bl 0x0200c34c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl 0x0200c31c
	movs r2, #155
	movs r0, #0
	movs r1, #174
	lsls r2, r2, #2
	bl 0x0200c344
	movs r2, #145
	lsls r2, r2, #2
	movs r1, #180
	movs r0, #28
	bl 0x0200c34c
	movs r0, #146
	bl 0x0200c45c
	ldr r5, [pc, #328]
	movs r0, #28
	adds r1, r5, #0
	bl 0x0200c324
	adds r1, r5, #0
	movs r0, #29
	bl 0x0200c324
	movs r0, #240
	bl 0x0200c45c
	movs r1, #134
	ldr r2, [pc, #308]
	movs r0, #31
	lsls r1, r1, #16
	bl 0x0200c36c
	ldr r1, [pc, #300]
	movs r0, #31
	bl 0x0200c324
	movs r0, #10
	bl 0x0200c2e4
	movs r1, #134
	movs r2, #146
	movs r0, #30
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	movs r0, #30
	bl 0x0200c31c
	movs r0, #30
	bl 0x0200c30c
	movs r3, #128
	lsls r3, r3, #12
	movs r2, #153
	str r3, [r0, #40]
	lsls r2, r2, #2
	movs r1, #186
	movs r0, #30
	bl 0x0200c34c
	movs r0, #30
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	movs r0, #10
	bl 0x0200c2e4
	movs r1, #128
	movs r2, #128
	movs r0, #30
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c31c
	movs r2, #150
	lsls r2, r2, #2
	movs r0, #30
	movs r1, #216
	bl 0x0200c34c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #30
	bl 0x0200ba00
	bl 0x02008bb8
	movs r0, #10
	bl 0x0200c2e4
	ldr r5, [pc, #176]
	movs r0, #30
	adds r1, r5, #0
	bl 0x0200c324
	movs r0, #10
	bl 0x0200c2e4
	adds r1, r5, #0
	movs r0, #28
	bl 0x0200c324
	movs r0, #10
	bl 0x0200c2e4
	adds r1, r5, #0
	movs r0, #29
	bl 0x0200c33c
	movs r0, #20
	bl 0x0200c2e4
	movs r0, #147
	bl 0x0200c45c
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #24
	bl 0x0200c334
	movs r0, #25
	bl 0x0200c334
	movs r0, #26
	bl 0x0200c334
	movs r0, #27
	bl 0x0200c334
	movs r0, #10
	bl 0x0200c2e4
	bl 0x0200c2b4
	movs r0, #21
	bl 0x0200c3dc
	ldr r0, [pc, #80]
	movs r1, #1
	movs r2, #0
	bl 0x0200c2c4
	bl 0x0200c2bc
	movs r0, #13
	bl 0x0200c424
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d160
	.4byte 0x0200d268
	.4byte 0x0200c80c
	.4byte 0x02920000
	.4byte 0x0200c7a8
	.4byte 0x0200c764
	.4byte 0x03001ebc
	.4byte 0x024a0000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x0200c7ec
	.4byte 0x02520000
	.4byte 0x0200c814
	.4byte 0x0200c888
	.4byte 0x00001e45
	.global Func_020022c0
	.thumb_func
Func_020022c0:
	push {r5, r6, r7, lr}
	bl 0x0200c2ec
	movs r1, #15
	movs r0, #0
	bl 0x0200c3a4
	movs r0, #0
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	ldr r0, [pc, #224]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	ldr r0, [pc, #216]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #176
	movs r2, #174
	lsls r1, r1, #16
	lsls r2, r2, #18
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r3, #208
	lsls r3, r3, #8
	movs r1, #132
	strh r3, [r0, #6]
	lsls r1, r1, #17
	ldr r2, [pc, #176]
	movs r0, #21
	bl 0x0200c36c
	movs r0, #21
	bl 0x0200c30c
	movs r3, #176
	lsls r3, r3, #8
	movs r1, #184
	movs r2, #168
	strh r3, [r0, #6]
	lsls r1, r1, #16
	movs r0, #24
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r1, #202
	movs r2, #173
	movs r0, #25
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r1, #252
	movs r0, #26
	lsls r1, r1, #16
	ldr r2, [pc, #128]
	bl 0x0200c36c
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #17
	ldr r2, [pc, #120]
	bl 0x0200c36c
	movs r1, #172
	movs r2, #158
	movs r0, #28
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c36c
	movs r1, #128
	lsls r1, r1, #17
	ldr r2, [pc, #100]
	movs r0, #29
	bl 0x0200c36c
	movs r0, #24
	bl 0x0200c30c
	ldr r5, [pc, #60]
	adds r0, #99
	strb r5, [r0]
	movs r0, #25
	bl 0x0200c30c
	movs r3, #1
	adds r0, #99
	strb r3, [r0]
	movs r0, #26
	bl 0x0200c30c
	adds r0, #99
	strb r5, [r0]
	movs r0, #27
	bl 0x0200c30c
	movs r3, #2
	adds r0, #99
	strb r3, [r0]
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl 0x0200c36c
	ldr r5, [pc, #40]
	movs r0, #24
	adds r1, r5, #0
	bl 0x0200c324
	b .L_020022c0_0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200d160
	.4byte 0x0200d340
	.4byte 0x02960000
	.4byte 0x02860000
	.4byte 0x02ae0000
	.4byte 0x026e0000
	.4byte 0x0200c8c4
.L_020022c0_0:
	adds r1, r5, #0
	movs r0, #25
	bl 0x0200c324
	ldr r5, [pc, #332]
	movs r0, #26
	adds r1, r5, #0
	bl 0x0200c324
	adds r1, r5, #0
	movs r0, #27
	bl 0x0200c324
	ldr r5, [pc, #320]
	movs r0, #28
	adds r1, r5, #0
	bl 0x0200c324
	adds r1, r5, #0
	movs r0, #29
	bl 0x0200c324
	movs r0, #24
	movs r1, #3
	bl 0x0200c3a4
	movs r0, #25
	movs r1, #3
	bl 0x0200c3a4
	movs r0, #26
	movs r1, #3
	bl 0x0200c3a4
	movs r0, #27
	movs r1, #3
	bl 0x0200c3a4
	movs r0, #28
	movs r1, #3
	bl 0x0200c3a4
	movs r1, #3
	movs r0, #29
	bl 0x0200c3a4
	ldr r3, [pc, #256]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #80
	bl 0x0200c2e4
	movs r0, #147
	bl 0x0200c45c
	movs r0, #31
	bl 0x0200c30c
	ldr r3, [pc, #220]
	adds r6, r0, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r3, #194
	lsls r3, r3, #16
	str r3, [r6, #8]
	ldr r3, [pc, #212]
	ldr r7, [pc, #212]
	str r3, [r6, #16]
	movs r5, #0
.L_020022c0_1:
	ldr r3, [r6, #24]
	adds r3, r3, r7
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r3, r3, r7
	str r3, [r6, #28]
	movs r0, #1
	adds r5, #1
	bl 0x0200c25c
	cmp r5, #15
	bls .L_020022c0_1
	movs r0, #30
	bl 0x0200c30c
	ldr r3, [pc, #180]
	adds r6, r0, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r3, #194
	lsls r3, r3, #16
	str r3, [r6, #8]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r6, #12]
	ldr r3, [pc, #152]
	str r3, [r6, #16]
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, [pc, #156]
	str r3, [r6, #68]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #72]
	movs r0, #80
	bl 0x0200c2e4
	movs r0, #147
	bl 0x0200c45c
	movs r2, #0
	movs r1, #0
	movs r0, #31
	bl 0x0200c36c
	movs r0, #30
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	ldr r1, [pc, #116]
	ldr r2, [pc, #116]
	movs r0, #0
	bl 0x0200c31c
	movs r0, #0
	bl 0x0200c30c
	adds r6, r0, #0
	ldr r5, [pc, #60]
	adds r3, r6, #0
	adds r3, #85
	movs r2, #153
	strb r5, [r3]
	movs r0, #0
	movs r1, #216
	lsls r2, r2, #2
	bl 0x0200c344
	movs r0, #30
	ldr r1, [pc, #76]
	ldr r2, [pc, #76]
	bl 0x0200c31c
	movs r2, #150
	movs r0, #30
	movs r1, #196
	lsls r2, r2, #2
	bl 0x0200c34c
	movs r2, #150
	movs r1, #216
	lsls r2, r2, #2
	movs r0, #30
	bl 0x0200c34c
	movs r0, #28
	bl 0x0200c334
	b .L_020022c0_2
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200c8b0
	.4byte 0x0200c8d8
	.4byte 0x03001ebc
	.4byte 0x00001999
	.4byte 0x02820000
	.4byte 0x00000f5c
	.4byte 0x00011999
	.4byte 0x00006666
	.4byte 0x00019999
	.4byte 0x0000cccc
.L_020022c0_2:
	movs r0, #1
	bl 0x0200c25c
	ldr r2, [pc, #168]
	movs r0, #28
	ldr r1, [pc, #168]
	bl 0x0200c31c
	ldr r5, [pc, #164]
	movs r0, #28
	adds r1, r5, #0
	bl 0x0200c324
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #30
	bl 0x0200ba00
	bl 0x02008bb8
	movs r0, #10
	bl 0x0200c2e4
	adds r1, r5, #0
	movs r0, #30
	bl 0x0200c324
	movs r0, #29
	bl 0x0200c334
	movs r0, #1
	bl 0x0200c25c
	ldr r2, [pc, #108]
	movs r0, #29
	ldr r1, [pc, #108]
	bl 0x0200c31c
	adds r1, r5, #0
	movs r0, #29
	bl 0x0200c33c
	movs r0, #20
	bl 0x0200c2e4
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #24
	bl 0x0200c334
	movs r0, #25
	bl 0x0200c334
	movs r0, #26
	bl 0x0200c334
	movs r0, #27
	bl 0x0200c334
	movs r0, #28
	bl 0x0200c334
	movs r0, #29
	bl 0x0200c334
	movs r0, #10
	bl 0x0200c2e4
	bl 0x0200c2b4
	movs r0, #21
	bl 0x0200c3dc
	ldr r0, [pc, #36]
	movs r1, #1
	movs r2, #0
	bl 0x0200c2c4
	bl 0x0200c2bc
	movs r0, #14
	bl 0x0200c424
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0000cccc
	.4byte 0x00019999
	.4byte 0x0200c888
	.4byte 0x00001e45
	.global Func_02002618
	.thumb_func
Func_02002618:
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
	.global Func_020029d4
	.thumb_func
Func_020029d4:
	push {r5, r6, lr}
	bl 0x0200c2ec
	ldr r0, [pc, #356]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #182
	movs r0, #20
	lsls r1, r1, #16
	ldr r2, [pc, #340]
	bl 0x0200c36c
	movs r1, #238
	movs r0, #23
	lsls r1, r1, #16
	ldr r2, [pc, #332]
	bl 0x0200c36c
	movs r1, #134
	ldr r2, [pc, #328]
	lsls r1, r1, #17
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r3, #0
	strh r3, [r0, #6]
	ldr r1, [pc, #312]
	movs r0, #22
	bl 0x0200c324
	movs r0, #21
	bl 0x0200c30c
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	ldr r2, [pc, #292]
	movs r0, #21
	ldr r1, [pc, #292]
	bl 0x0200c31c
	ldr r1, [pc, #292]
	movs r0, #21
	bl 0x0200c324
	ldr r5, [pc, #288]
	movs r6, #224
	ldr r2, [r5]
	movs r3, #128
	lsls r3, r3, #1
	lsls r6, r6, #1
	str r3, [r2, r6]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c2e4
	movs r0, #20
	ldr r1, [pc, #260]
	ldr r2, [pc, #248]
	bl 0x0200c31c
	movs r2, #137
	lsls r2, r2, #2
	movs r0, #20
	movs r1, #182
	bl 0x0200c35c
	movs r0, #20
	movs r1, #0
	bl 0x0200ba00
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200ba00
	movs r1, #1
	movs r0, #20
	bl 0x0200c394
	ldr r0, [pc, #220]
	bl 0x0200c3ac
	movs r0, #20
	bl 0x0200b9ec
	movs r1, #3
	movs r0, #0
	bl 0x0200c37c
	movs r0, #40
	bl 0x0200c2e4
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200c3d4
	movs r0, #20
	ldr r1, [pc, #184]
	movs r2, #60
	bl 0x0200c3ec
	movs r2, #40
	movs r0, #20
	movs r1, #0
	bl 0x0200c3c4
.L_02002ac4:
	movs r1, #0
	movs r0, #20
	bl 0x0200ba00
	movs r0, #20
	bl 0x0200b9ec
	movs r0, #0
	movs r1, #3
	bl 0x0200c37c
	movs r0, #20
	movs r1, #3
	bl 0x0200c37c
	movs r2, #150
	movs r0, #20
	movs r1, #182
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #150
	lsls r2, r2, #2
	movs r0, #20
	movs r1, #216
	bl 0x0200c35c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #20
	bl 0x0200ba00
	bl 0x02008bb8
	movs r0, #10
	bl 0x0200c2e4
	movs r2, #145
	movs r0, #20
	movs r1, #216
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r1, #0
	movs r0, #20
	movs r2, #0
	bl 0x0200c36c
	ldr r2, [r5]
	ldr r3, [pc, #72]
	ldr r0, [pc, #72]
	str r3, [r2, r6]
	bl 0x0200c2d4
	ldr r0, [pc, #68]
	bl 0x0200c2dc
	bl 0x0200c2f4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0xd160
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x026a
	.2byte 0x0000
	.2byte 0x0272
	.2byte 0x0000
	.2byte 0x02a6
	.2byte 0xc980
	.2byte 0x0200
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0xc628
	.2byte 0x0200
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0x1ee1
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.4byte 0x00000209
	.4byte 0x0000092b
	.4byte 0x00000302
	.global Func_02002b7c
	.thumb_func
Func_02002b7c:
	push {r5, lr}
	bl 0x0200c2ec
	movs r1, #15
	movs r0, #0
	bl 0x0200c3a4
	movs r0, #0
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	ldr r0, [pc, #220]
	bl 0x0200c2fc
	movs r0, #1
	bl 0x0200c25c
	movs r1, #196
	movs r2, #251
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r0, #20
	bl 0x0200c36c
	movs r0, #20
	bl 0x0200c30c
	movs r3, #160
	lsls r3, r3, #8
	movs r1, #184
	movs r2, #131
	strh r3, [r0, #6]
	lsls r2, r2, #18
	lsls r1, r1, #16
	movs r0, #22
	bl 0x0200c36c
	movs r0, #22
	bl 0x0200c30c
	movs r5, #176
	lsls r5, r5, #8
	strh r5, [r0, #6]
	movs r1, #1
	movs r0, #21
	bl 0x0200c3e4
	movs r1, #184
	movs r2, #158
	lsls r1, r1, #16
	lsls r2, r2, #18
	movs r0, #21
	bl 0x0200c36c
	movs r0, #21
	bl 0x0200c30c
	ldr r3, [pc, #132]
	strh r5, [r0, #6]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c2e4
	movs r0, #22
	movs r1, #4
	movs r2, #10
	bl 0x0200c384
	movs r2, #20
	movs r1, #6
	movs r0, #22
	bl 0x0200c384
	ldr r0, [pc, #84]
	bl 0x0200c3ac
	movs r0, #22
	bl 0x0200b9ec
	movs r0, #20
	movs r1, #3
	bl 0x0200c37c
	movs r1, #192
	movs r2, #192
	movs r0, #21
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c31c
	movs r0, #21
	movs r1, #180
	ldr r2, [pc, #52]
	bl 0x0200c35c
	movs r2, #40
	adds r1, r5, #0
	movs r0, #21
	bl 0x0200c3d4
	movs r1, #1
	movs r0, #21
	bl 0x0200c394
	movs r0, #21
	bl 0x0200b9ec
	movs r0, #15
	bl 0x0200c424
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200d160
	.4byte 0x03001ebc
	.4byte 0x00001ee5
	.4byte 0x00000222
	.global Func_02002c84
	.thumb_func
Func_02002c84:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #52
	movs r1, #4
	movs r6, #0
	add r1, sp
	movs r7, #0
	add r4, sp, #36
	mov r8, r1
	mov r10, r6
	movs r5, #0
.L_02002c84_0:
	adds r0, r6, #0
	str r4, [sp, #0]
	bl 0x0200b150
	ldr r4, [sp, #0]
	mov r3, r10
	mov r2, r8
	adds r6, #1
	str r0, [r5, r4]
	str r3, [r5, r2]
	adds r5, #4
	cmp r6, #3
	bls .L_02002c84_0
	ldr r3, [r4]
	movs r1, #23
	movs r6, #0
	cmp r3, #23
	bne .L_02002c84_1
	add r2, sp, #20
	mov r3, r8
	str r1, [r2, r6]
	b .L_02002c84_2
.L_02002c84_1:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_3
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_1
	add r2, sp, #20
	mov r3, r8
	str r1, [r2]
.L_02002c84_2:
	mov r10, r2
	str r7, [r3]
	movs r7, #1
	b .L_02002c84_4
.L_02002c84_3:
	add r1, sp, #20
	mov r10, r1
.L_02002c84_4:
	ldr r3, [r4]
	movs r1, #24
	movs r6, #0
	cmp r3, #24
	beq .L_02002c84_5
.L_02002c84_7:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_6
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_7
.L_02002c84_5:
	lsls r3, r7, #2
	mov r2, r10
	str r1, [r2, r3]
	mov r1, r8
	str r7, [r1, r3]
	adds r7, #1
.L_02002c84_6:
	ldr r3, [r4]
	movs r1, #25
	movs r6, #0
	cmp r3, #25
	beq .L_02002c84_8
.L_02002c84_10:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_9
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_10
.L_02002c84_8:
	lsls r3, r7, #2
	mov r2, r10
	str r1, [r2, r3]
	mov r1, r8
	str r7, [r1, r3]
	adds r7, #1
.L_02002c84_9:
	ldr r3, [r4]
	movs r1, #27
	movs r6, #0
	cmp r3, #27
	beq .L_02002c84_11
.L_02002c84_13:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_12
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_13
.L_02002c84_11:
	lsls r3, r7, #2
	mov r2, r10
	str r1, [r2, r3]
	mov r1, r8
	str r7, [r1, r3]
	adds r7, #1
.L_02002c84_12:
	cmp r7, #4
	beq .L_02002c84_14
	ldr r3, [r4]
	movs r1, #28
	movs r6, #0
	cmp r3, #28
	beq .L_02002c84_15
.L_02002c84_17:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_16
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_17
.L_02002c84_15:
	lsls r3, r7, #2
	mov r2, r10
	str r1, [r2, r3]
	mov r1, r8
	str r7, [r1, r3]
	adds r7, #1
.L_02002c84_16:
	cmp r7, #4
	beq .L_02002c84_14
	ldr r3, [r4]
	movs r1, #29
	movs r6, #0
	cmp r3, #29
	beq .L_02002c84_18
.L_02002c84_20:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_19
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_20
.L_02002c84_18:
	lsls r3, r7, #2
	mov r2, r10
	str r1, [r2, r3]
	mov r1, r8
	str r7, [r1, r3]
	adds r7, #1
.L_02002c84_19:
	cmp r7, #4
	beq .L_02002c84_14
	ldr r3, [r4]
	movs r1, #26
	movs r6, #0
	cmp r3, #26
	beq .L_02002c84_21
.L_02002c84_23:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_22
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_23
.L_02002c84_21:
	lsls r2, r7, #2
	mov r3, r10
	str r1, [r3, r2]
	movs r3, #10
	mov r1, r8
	str r3, [r1, r2]
	adds r7, #1
.L_02002c84_22:
	cmp r7, #4
	beq .L_02002c84_14
	ldr r3, [r4]
	movs r1, #30
	movs r6, #0
	cmp r3, #30
	beq .L_02002c84_24
.L_02002c84_26:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_25
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_26
.L_02002c84_24:
	lsls r2, r7, #2
	mov r3, r10
	str r1, [r3, r2]
	movs r3, #11
	mov r1, r8
	str r3, [r1, r2]
	adds r7, #1
.L_02002c84_25:
	cmp r7, #4
	beq .L_02002c84_14
	ldr r3, [r4]
	movs r1, #31
	movs r6, #0
	cmp r3, #31
	beq .L_02002c84_27
.L_02002c84_28:
	adds r6, #1
	cmp r6, #3
	bhi .L_02002c84_14
	lsls r3, r6, #2
	ldr r3, [r4, r3]
	cmp r3, r1
	bne .L_02002c84_28
.L_02002c84_27:
	lsls r2, r7, #2
	mov r3, r10
	str r1, [r3, r2]
	movs r3, #20
	mov r1, r8
	str r3, [r1, r2]
.L_02002c84_14:
	bl 0x0200c2ec
	movs r1, #15
	movs r0, #0
	bl 0x0200c3a4
	movs r0, #0
	bl 0x0200c30c
	movs r1, #0
	bl 0x0200c2a4
	movs r0, #0
	bl 0x0200c30c
	cmp r0, #0
	beq .L_02002c84_29
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #32
	bl 0x0200c36c
.L_02002c84_29:
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c2e4
	movs r0, #32
	movs r1, #1
	bl 0x0200c404
	bl 0x02008bb8
	movs r0, #10
	bl 0x0200c2e4
	movs r6, #0
.L_02002c84_34:
	lsls r7, r6, #2
	mov r2, r10
	ldr r5, [r2, r7]
	movs r1, #216
	movs r2, #146
	lsls r1, r1, #16
	adds r0, r5, #0
	lsls r2, r2, #18
	bl 0x0200c36c
	mov r1, r8
	ldr r3, [r1, r7]
	cmp r3, #20
	bne .L_02002c84_30
	adds r0, r5, #0
	ldr r1, [pc, #636]
	ldr r2, [pc, #636]
	bl 0x0200c31c
	b .L_02002c84_31
.L_02002c84_30:
	movs r1, #128
	movs r2, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c31c
.L_02002c84_31:
	movs r2, #150
	adds r0, r5, #0
	movs r1, #216
	lsls r2, r2, #2
	bl 0x0200c35c
	adds r0, r5, #0
	movs r1, #192
	ldr r2, [pc, #604]
	bl 0x0200c35c
	movs r2, #164
	lsls r2, r2, #2
	adds r0, r5, #0
	movs r1, #192
	bl 0x0200c35c
	mov r2, r8
	ldr r3, [r2, r7]
	cmp r3, #20
	bhi .L_02002c84_32
	ldr r2, [pc, #580]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	add r7, sp, #176
	lsls r0, r0, #8
	add r7, sp, #256
	lsls r0, r0, #8
	add r7, sp, #376
	lsls r0, r0, #8
	add r7, sp, #448
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #512
	lsls r0, r0, #8
	add r7, sp, #576
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #736
	lsls r0, r0, #8
	add r7, sp, #640
	lsls r0, r0, #8
	movs r1, #129
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c3ec
	ldr r0, [pc, #480]
	bl 0x0200c3ac
	b .L_02002c84_32
	.2byte 0x21d0
	.2byte 0x0209
	.2byte 0x1c28
	.2byte 0xf000
	.2byte 0xfd5b
	.2byte 0x2181
	.2byte 0x1c28
	.2byte 0x0049
	.2byte 0x223c
	.2byte 0xf001
	.2byte 0xfa4b
	.2byte 0x4872
	.2byte 0xf001
	.2byte 0xfa28
	.2byte 0xe02c
	.2byte 0x1c28
	.2byte 0x4970
	.2byte 0x223c
	.2byte 0xf001
	.2byte 0xfa42
	.2byte 0x486f
	.2byte 0xf001
	.2byte 0xfa1f
	.2byte 0xe023
	.2byte 0x1c28
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfa0e
	.2byte 0x486c
	.2byte 0xf001
	.2byte 0xfa17
	.2byte 0xe01b
	.2byte 0x1c28
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xf9fa
	.2byte 0x4869
	.2byte 0xf001
	.2byte 0xfa0f
	.2byte 0xe013
	.2byte 0x1c28
	.2byte 0x2104
	.2byte 0xf001
	.2byte 0xf9ee
	.2byte 0x4866
	.2byte 0xf001
	.2byte 0xfa07
	.2byte 0xe00b
	.2byte 0x1c28
	.2byte 0x2104
	.2byte 0xf001
	.2byte 0xf9ea
	.2byte 0x1c28
	.2byte 0x4963
	.2byte 0x2228
	.2byte 0xf001
	.2byte 0xfa1d
	.2byte 0x4862
	.2byte 0xf001
	.2byte 0xf9fa
.L_02002c84_32:
	adds r0, r5, #0
	bl 0x0200b9ec
	adds r6, #1
	ldr r1, [pc, #380]
	adds r0, r5, #0
	bl 0x0200c324
	cmp r6, #3
	bhi .L_02002c84_33
	b .L_02002c84_34
.L_02002c84_33:
	adds r0, r5, #0
	bl 0x0200c32c
	movs r0, #40
	bl 0x0200c2e4
	movs r1, #216
	movs r2, #146
	lsls r2, r2, #18
	lsls r1, r1, #16
	movs r0, #0
	bl 0x0200c36c
	movs r0, #1
	bl 0x0200c25c
	movs r1, #0
	movs r0, #0
	bl 0x0200c3a4
	movs r0, #0
	bl 0x0200c30c
	movs r1, #1
	bl 0x0200c2a4
	movs r0, #0
	ldr r1, [pc, #260]
	ldr r2, [pc, #264]
	bl 0x0200c31c
	movs r2, #150
	movs r0, #0
	movs r1, #216
	lsls r2, r2, #2
	bl 0x0200c35c
	movs r2, #153
	movs r1, #190
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200c35c
	ldr r0, [pc, #284]
	bl 0x0200c3ac
	movs r0, #20
	bl 0x0200b9ec
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200c3d4
	movs r0, #32
	bl 0x0200c30c
	movs r3, #0
	adds r0, #85
	movs r1, #128
	movs r2, #128
	strb r3, [r0]
	lsls r1, r1, #10
	movs r0, #32
	lsls r2, r2, #9
	bl 0x0200c31c
	movs r2, #141
	movs r0, #32
	movs r1, #196
	lsls r2, r2, #2
	bl 0x0200c344
	movs r0, #20
	ldr r1, [pc, #164]
	ldr r2, [pc, #168]
	bl 0x0200c31c
	movs r0, #20
	movs r1, #182
	ldr r2, [pc, #212]
	bl 0x0200c35c
	movs r1, #192
	movs r2, #20
	movs r0, #20
	lsls r1, r1, #6
	bl 0x0200c3d4
	movs r1, #1
	movs r0, #20
	bl 0x0200c394
	movs r0, #20
	bl 0x0200b9ec
	movs r1, #3
	movs r0, #20
	bl 0x0200c37c
	movs r0, #20
	bl 0x0200b9ec
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #8
	movs r0, #20
	movs r5, #192
	bl 0x0200c3d4
	lsls r5, r5, #6
	movs r0, #20
	bl 0x0200b9ec
	adds r1, r5, #0
	movs r0, #20
	bl 0x0200ba00
	movs r0, #20
	bl 0x0200b9ec
	movs r0, #20
	movs r1, #3
	bl 0x0200c37c
	movs r0, #0
	movs r1, #1
	bl 0x0200c404
	movs r2, #128
	movs r0, #20
	movs r1, #188
	lsls r2, r2, #2
	bl 0x0200c35c
	adds r1, r5, #0
	movs r0, #20
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #0
	movs r2, #0
	movs r0, #32
	bl 0x0200c36c
	ldr r0, [pc, #88]
	bl 0x0200c2dc
	bl 0x0200c2f4
	sub sp, #-52
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000026a
	.4byte 0x0200aed8
	.4byte 0x00001ee7
	.2byte 0x1ee8
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x1ee9
	.2byte 0x0000
	.2byte 0x1eea
	.2byte 0x0000
	.2byte 0x1eeb
	.2byte 0x0000
	.2byte 0x1eec
	.2byte 0x0000
	.2byte 0x0107
	.2byte 0x0000
	.2byte 0x1eed
	.2byte 0x0000
	.4byte 0x0200c8e0
	.4byte 0x00001eee
	.4byte 0x0000022b
	.4byte 0x0000012f
	.global Func_02003150
	.thumb_func
Func_02003150:
	push {r5, r6, r7, lr}
	movs r6, #0
	movs r7, #23
	cmp r0, #1
	beq .L_02003150_0
	cmp r0, #1
	bcc .L_02003150_1
	cmp r0, #2
	beq .L_02003150_2
	cmp r0, #3
	beq .L_02003150_3
	b .L_02003150_4
.L_02003150_1:
	ldr r6, [pc, #48]
	b .L_02003150_4
.L_02003150_0:
	ldr r6, [pc, #48]
	b .L_02003150_4
.L_02003150_2:
	ldr r6, [pc, #48]
	b .L_02003150_4
.L_02003150_6:
	adds r0, r7, #0
	b .L_02003150_5
.L_02003150_3:
	movs r6, #153
	lsls r6, r6, #4
.L_02003150_4:
	movs r5, #0
.L_02003150_7:
	adds r0, r6, #0
	bl 0x0200c2cc
	cmp r0, #0
	bne .L_02003150_6
	adds r5, #1
	adds r6, #1
	adds r7, #1
	cmp r5, #8
	bls .L_02003150_7
	movs r0, #0
.L_02003150_5:
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000092c
	.4byte 0x00000935
	.4byte 0x00000917
	.global Func_020031a8
	.thumb_func
Func_020031a8:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r3, [pc, #176]
	ldr r0, [pc, #180]
	ldr r3, [r3]
	mov r8, r0
	ldr r0, [r0]
	ldr r6, [r3]
	mov r9, r3
	bl 0x0200c27c
	ldr r1, [pc, #168]
	adds r5, r0, #0
	ldr r0, [r1]
	mov r10, r1
	bl 0x0200c274
	ldr r3, [r6]
	asrs r5, r5, #1
	adds r3, r3, r5
	stmia r6!, {r3}
	ldr r3, [r6]
	adds r3, r3, r0
	str r3, [r6]
	bl 0x0200c26c
	lsls r3, r0, #1
	mov r5, r8
	adds r3, r3, r0
	ldr r2, [r5]
	lsls r3, r3, #7
	lsrs r3, r3, #16
	adds r2, r2, r3
	str r2, [r5]
	bl 0x0200c26c
	mov r1, r10
	ldr r3, [r1]
	lsls r0, r0, #9
	ldrh r2, [r5]
	lsrs r0, r0, #16
	ldr r1, [pc, #112]
	adds r3, r3, r0
	str r2, [r5]
	ands r3, r1
	mov r2, r10
	str r3, [r2]
	ldr r1, [pc, #104]
	movs r0, #130
	ldr r3, [r1]
	lsls r0, r0, #1
	add r0, r9
	str r3, [r0, #8]
	ldr r4, [pc, #96]
	ldr r2, [r1]
	ldr r3, [r4]
	subs r2, r2, r3
	str r2, [r1]
	cmp r2, #0
	bge .L_020031a8_0
	movs r5, #128
	lsls r5, r5, #14
	adds r3, r2, r5
	str r3, [r1]
.L_020031a8_0:
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_020031a8_1
	ldr r5, [pc, #68]
	adds r3, r3, r5
	str r3, [r1]
.L_020031a8_1:
	ldr r3, [r1, #4]
	str r3, [r0, #12]
	ldr r2, [r1, #4]
	ldr r3, [r4, #4]
	subs r2, r2, r3
	str r2, [r1, #4]
	cmp r2, #0
	bge .L_020031a8_2
	movs r0, #128
	lsls r0, r0, #14
	adds r3, r2, r0
	str r3, [r1, #4]
.L_020031a8_2:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0x0200db58
	.4byte 0x0200db38
	.4byte 0x0000ffff
	.4byte 0x0200db50
	.4byte 0x0200db60
	.4byte 0xffe00000
	.global Func_02003284
	.thumb_func
Func_02003284:
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
	.global Func_02003380
	.thumb_func
Func_02003380:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x0200c30c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02003380_0
	adds r0, r6, #0
	movs r1, #3
	bl 0x0200c3e4
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200c2a4
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
.L_02003380_0:
	pop {r5, r6}
	pop {r0}
	bx r0
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
	.global Func_02003464
	.thumb_func
Func_02003464:
	push {r5, r6, lr}
	movs r6, #0
	cmp r0, #1
	beq .L_02003464_0
	cmp r0, #1
	bcc .L_02003464_1
	cmp r0, #2
	beq .L_02003464_2
	cmp r0, #3
	beq .L_02003464_3
	b .L_02003464_4
.L_02003464_1:
	ldr r6, [pc, #48]
	b .L_02003464_4
.L_02003464_0:
	ldr r6, [pc, #48]
	b .L_02003464_4
.L_02003464_2:
	ldr r6, [pc, #48]
	b .L_02003464_4
.L_02003464_6:
	ldr r3, [pc, #48]
	lsls r2, r5, #2
	ldr r0, [r3, r2]
	b .L_02003464_5
.L_02003464_3:
	movs r6, #153
	lsls r6, r6, #4
.L_02003464_4:
	movs r5, #0
.L_02003464_7:
	adds r0, r6, r5
	bl 0x0200c2cc
	cmp r0, #0
	bne .L_02003464_6
	adds r5, #1
	cmp r5, #8
	bls .L_02003464_7
	movs r0, #0
.L_02003464_5:
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x0000092c
	.4byte 0x00000935
	.4byte 0x00000917
	.4byte 0x0200db08
	.global Func_020034bc
	.thumb_func
Func_020034bc:
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
	.global Func_02003710
	.thumb_func
Func_02003710:
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
	.global Func_02003950
	.thumb_func
Func_02003950:
	push {r5, r6, lr}
	sub sp, #8
	movs r5, #1
	movs r6, #5
	movs r0, #78
	movs r1, #39
	movs r2, #78
	movs r3, #40
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c294
	movs r0, #78
	movs r1, #39
	movs r2, #78
	movs r3, #41
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c294
	movs r3, #4
	str r3, [sp, #0]
	movs r0, #78
	movs r1, #39
	movs r2, #79
	movs r3, #42
	str r5, [sp, #4]
	bl 0x0200c294
	movs r0, #78
	movs r1, #39
	movs r2, #82
	movs r3, #43
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c294
	movs r3, #17
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #38
	movs r2, #5
	movs r3, #2
	bl 0x0200c29c
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020039b8
	.thumb_func
Func_020039b8:
	push {r5, lr}
	sub sp, #8
	movs r3, #4
	str r3, [sp, #4]
	movs r5, #5
	movs r0, #66
	movs r1, #61
	movs r2, #64
	movs r3, #40
	str r5, [sp, #0]
	bl 0x0200c294
	movs r3, #39
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #5
	movs r3, #4
	str r5, [sp, #0]
	bl 0x0200c29c
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020039ec
	.thumb_func
Func_020039ec:
	push {lr}
	movs r1, #0
	bl 0x0200c3bc
	movs r0, #10
	bl 0x0200c2e4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003a00
	.thumb_func
Func_02003a00:
	push {lr}
	movs r2, #10
	bl 0x0200c3d4
	pop {r0}
	bx r0
	.global Func_02003a0c
	.thumb_func
Func_02003a0c:
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
	.global Func_02003c88
	.thumb_func
Func_02003c88:
	push {r5, r6, lr}
	bl 0x0200c2ec
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c31c
	movs r0, #0
	movs r1, #180
	ldr r2, [pc, #620]
	bl 0x0200c35c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r0, #0
	bl 0x0200c30c
	cmp r0, #0
	beq .L_02003c88_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200c36c
.L_02003c88_0:
	movs r0, #0
	bl 0x0200c30c
	cmp r0, #0
	beq .L_02003c88_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200c36c
.L_02003c88_1:
	movs r0, #0
	bl 0x0200c30c
	cmp r0, #0
	beq .L_02003c88_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200c36c
.L_02003c88_2:
	movs r0, #1
	ldr r1, [pc, #544]
	ldr r2, [pc, #548]
	bl 0x0200c31c
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c31c
	movs r0, #3
	ldr r1, [pc, #520]
	ldr r2, [pc, #524]
	bl 0x0200c31c
	movs r2, #160
	movs r0, #1
	movs r1, #194
	lsls r2, r2, #2
	bl 0x0200c354
	movs r0, #2
	movs r1, #198
	ldr r2, [pc, #492]
	bl 0x0200c354
	movs r2, #168
	lsls r2, r2, #2
	movs r0, #3
	movs r1, #194
	bl 0x0200c35c
	movs r0, #1
	movs r1, #1
	bl 0x0200c374
	movs r1, #1
	movs r0, #2
	bl 0x0200c374
	movs r0, #10
	bl 0x0200c2e4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #128
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200c3d4
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200ba00
	movs r1, #0
	movs r0, #22
	bl 0x0200ba00
	ldr r0, [pc, #424]
	bl 0x0200c3ac
	movs r0, #22
	bl 0x0200b9ec
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #21
	bl 0x0200ba00
	movs r0, #21
	movs r1, #0
	movs r2, #40
	bl 0x0200c3c4
	movs r1, #128
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200c3ec
	movs r0, #22
	movs r1, #1
	bl 0x0200c394
	movs r1, #0
	movs r0, #22
	bl 0x0200c3b4
	movs r0, #0
	movs r1, #0
	bl 0x0200c304
	cmp r0, #1
	bne .L_02003c88_3
	movs r1, #4
	movs r0, #2
	bl 0x0200c37c
	movs r0, #2
	bl 0x0200b9ec
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200ba00
	movs r1, #3
	movs r0, #3
	bl 0x0200c374
	movs r0, #3
	bl 0x0200b9ec
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200ba00
	movs r0, #1
	movs r1, #1
	bl 0x0200c394
	movs r0, #1
	movs r1, #0
	bl 0x0200c3b4
.L_02003c88_4:
	movs r0, #0
	movs r1, #0
	bl 0x0200c304
	cmp r0, #1
	bne .L_02003c88_3
	movs r1, #1
	movs r0, #2
	bl 0x0200c394
	ldr r0, [pc, #272]
	bl 0x0200c3ac
	movs r0, #2
	movs r1, #0
	bl 0x0200c3b4
	b .L_02003c88_4
.L_02003c88_3:
	movs r0, #20
	bl 0x0200c2e4
	movs r1, #3
	movs r0, #22
	bl 0x0200c37c
	ldr r0, [pc, #244]
	bl 0x0200c3ac
	movs r0, #22
	bl 0x0200b9ec
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c31c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #21
	bl 0x0200c31c
	movs r0, #22
	bl 0x0200c30c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #162
	ldr r2, [pc, #188]
	movs r0, #22
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c2e4
	movs r0, #22
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #21
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #169
	ands r5, r3
	movs r1, #162
	lsls r2, r2, #2
	strb r5, [r0]
	movs r0, #21
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c2e4
	movs r0, #21
	bl 0x0200c30c
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #192
	orrs r6, r3
	movs r2, #0
	strb r6, [r0]
	lsls r1, r1, #6
	movs r0, #22
	bl 0x0200c3d4
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #21
	bl 0x0200ba00
	movs r0, #22
	bl 0x0200b9ec
	movs r0, #1
	movs r1, #180
	ldr r2, [pc, #56]
	bl 0x0200c354
	movs r0, #2
	movs r1, #180
	ldr r2, [pc, #48]
	bl 0x0200c354
	movs r1, #180
	ldr r2, [pc, #40]
	movs r0, #3
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c314
	movs r0, #2
	bl 0x0200c314
	movs r0, #3
	bl 0x0200c314
	ldr r0, [pc, #40]
	bl 0x0200c2d4
	bl 0x0200c2f4
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0000028e
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00001f55
	.4byte 0x00001f53
	.4byte 0x00001f5b
	.4byte 0x0000027a
	.4byte 0x00000903
	.global Func_02003f30
	.thumb_func
Func_02003f30:
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
	.global Func_02004218
	.thumb_func
Func_02004218:
	push {lr}
	movs r0, #232
	movs r1, #1
	movs r2, #169
	movs r3, #0
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200c414
	bl 0x0200c284
	movs r1, #232
	movs r2, #169
	lsls r1, r1, #16
	lsls r2, r2, #18
	movs r0, #0
	bl 0x0200c36c
	movs r0, #0
	bl 0x0200c30c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #1
	bl 0x0200c25c
	pop {r0}
	bx r0
	.include "games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/IMPORT.INC"
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
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
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
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008515
	.4byte 0x00000022
	.4byte 0x02008571
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
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008599
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008725
	.4byte 0x00000022
	.4byte 0x0200889d
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
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x020088e1
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
	.4byte 0x001800d0
	.4byte 0x00e0027a
	.4byte 0x028a0028
	.4byte 0x0005ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
