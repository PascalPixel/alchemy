.syntax unified
	.thumb
	.section .text.x02008098,"ax",%progbits
	.p2align 2
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	lsls	r1, r1, #7
	ldr	r4, [sp, #48]
	mov	sl, r2
	adds	r1, r1, r0
	ldr	r2, [pc, #132]
	lsls	r1, r1, #2
	adds	r3, r4, r3
	adds	r5, r1, r2
	cmp	r4, r3
	bge.n	.L_02000126
	str	r3, [sp, #4]
	mov	r6, sl
	movs	r3, #128
	subs	r3, r3, r6
	lsls	r3, r3, #2
	mov	fp, r3
	ldr	r3, [sp, #40]
	lsls	r3, r3, #4
	mov	r9, r3
.L_020000ce:
	ldr	r0, [sp, #44]
	mov	r1, sl
	adds	r2, r0, r1
	cmp	r0, r2
	bge.n	.L_0200011c
	ldr	r3, [pc, #96]
	movs	r7, #15
	mov	r8, r3
	adds	r3, r4, #0
	ands	r3, r7
	add	r3, r9
	lsls	r3, r3, #5
	ldr	r6, [pc, #88]
	str	r3, [sp, #0]
	mov	lr, r6
	mov	ip, r2
.L_020000ee:
	ldr	r6, [sp, #0]
	ldmia	r5!, {r1}
	adds	r3, r0, #0
	mov	r2, r8
	ands	r3, r7
	ands	r1, r2
	adds	r3, r6, r3
	ldr	r6, [pc, #68]
	lsls	r1, r1, #3
	adds	r2, r1, r6
	ldr	r2, [r2, #0]
	lsls	r3, r3, #2
	mov	r6, lr
	str	r2, [r3, r6]
	ldr	r6, [pc, #60]
	adds	r2, r1, r6
	ldr	r1, [pc, #60]
	ldr	r2, [r2, #0]
	adds	r3, r3, r1
	adds	r0, #1
	str	r2, [r3, #0]
	cmp	r0, ip
	blt.n	.L_020000ee
.L_0200011c:
	ldr	r2, [sp, #4]
	adds	r4, #1
	add	r5, fp
	cmp	r4, r2
	blt.n	.L_020000ce
.L_02000126:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02010000
	.4byte 0x00000fff
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.2byte 0x2840
	.2byte 0x0600
	.section .text.x020083f0,"ax",%progbits
	.p2align 2
	.global Scene_RunKorimaMagariSequence
	.thumb_func
Scene_RunKorimaMagariSequence:
	push {r5, r6, lr}
	sub sp, #12
	bl 0x0200909c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x020090e4
	movs r0, #132
	movs r1, #1
	movs r2, #224
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl 0x020090ec
	bl 0x020090f4
	ldr r0, [pc, #800]
	movs r1, #1
	bl 0x02009084
	movs r0, #232
	bl 0x02009114
	ldr r3, [pc, #788]
	ldr r3, [r3]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020003f0_0
	b .L_020003f0_1
.L_020003f0_0:
	movs r1, #128
	movs r2, #231
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x020090d4
	movs r6, #25
	movs r5, #83
	movs r1, #34
	movs r2, #1
	movs r3, #2
	movs r0, #77
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #3
	bl 0x02008ffc
	movs r1, #34
	movs r2, #1
	movs r3, #2
	movs r0, #78
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #3
	bl 0x02008ffc
	movs r1, #34
	movs r2, #1
	movs r3, #2
	movs r0, #79
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #30
	bl 0x02008ffc
	movs r5, #79
	movs r1, #34
	movs r2, #2
	movs r3, #5
	movs r0, #67
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl 0x0200906c
	movs r0, #6
	bl 0x02008ffc
	movs r2, #2
	movs r3, #5
	movs r0, #69
	movs r1, #34
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl 0x0200906c
	movs r1, #1
	movs r0, #9
	bl 0x020090dc
	movs r0, #240
	bl 0x02009114
	movs r0, #6
	bl 0x02008ffc
	movs r1, #34
	movs r2, #2
	movs r3, #5
	movs r0, #71
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl 0x0200906c
	movs r0, #6
	bl 0x02008ffc
	movs r0, #73
	movs r1, #34
	movs r2, #2
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl 0x0200906c
	movs r6, #29
	movs r1, #38
	movs r2, #2
	movs r3, #1
	movs r0, #75
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #4
	bl 0x02008ffc
	movs r1, #38
	movs r2, #2
	movs r3, #1
	movs r0, #77
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #6
	bl 0x02008ffc
	movs r1, #38
	movs r2, #2
	movs r3, #1
	movs r0, #79
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #8
	bl 0x02008ffc
	movs r0, #65
	movs r1, #53
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r3, #15
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	movs r1, #40
	movs r2, #2
	movs r3, #4
	bl 0x0200906c
	b .L_020003f0_2
.L_020003f0_1:
	movs r1, #128
	movs r2, #240
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x020090d4
	movs r6, #25
	movs r5, #83
	movs r1, #34
	movs r2, #1
	movs r3, #2
	movs r0, #78
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #3
	bl 0x02008ffc
	movs r1, #34
	movs r2, #1
	movs r3, #2
	movs r0, #77
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #3
	bl 0x02008ffc
	movs r1, #34
	movs r2, #1
	movs r3, #2
	movs r0, #76
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #30
	bl 0x02008ffc
	movs r3, #15
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	movs r1, #45
	movs r2, #2
	movs r3, #4
	bl 0x0200906c
	movs r5, #79
	movs r2, #2
	movs r3, #5
	movs r0, #71
	movs r1, #50
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r1, #2
	movs r0, #9
	bl 0x020090dc
	movs r0, #230
	bl 0x02009114
	movs r0, #6
	bl 0x02008ffc
	movs r1, #50
	movs r2, #2
	movs r3, #5
	movs r0, #69
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #6
	bl 0x02008ffc
	movs r1, #50
	movs r2, #2
	movs r3, #5
	movs r0, #67
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #6
	bl 0x02008ffc
	movs r0, #65
	movs r1, #50
	movs r2, #2
	movs r3, #5
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200906c
	movs r0, #30
	bl 0x02008ffc
.L_020003f0_2:
	ldr r3, [pc, #280]
	ldr r3, [r3]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020003f0_3
	str r3, [sp, #0]
	movs r6, #9
	movs r5, #30
	movs r0, #9
	movs r1, #19
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
	movs r3, #1
	str r3, [sp, #0]
	movs r0, #9
	movs r1, #51
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #41
	movs r1, #51
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
	b .L_020003f0_6
.L_020003f0_3:
	movs r3, #0
	str r3, [sp, #0]
	movs r6, #9
	movs r5, #30
	movs r0, #9
	movs r1, #19
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
	movs r3, #1
	str r3, [sp, #0]
	movs r0, #9
	movs r1, #83
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #41
	movs r1, #83
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
.L_020003f0_6:
	ldr r5, [pc, #144]
	movs r6, #0
	movs r1, #200
	lsls r1, r1, #4
	str r6, [r5]
	ldr r0, [pc, #136]
	bl 0x02009004
	movs r0, #1
	bl 0x02008ffc
	ldr r2, [pc, #128]
	movs r0, #1
	movs r1, #0
	bl 0x02009024
	movs r0, #231
	bl 0x02009114
	str r6, [r5]
.L_020003f0_10:
	movs r0, #1
	bl 0x02008ffc
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, #100
	ble .L_020003f0_10
	ldr r0, [pc, #100]
	bl 0x02009114
	ldr r3, [pc, #76]
.L_020003f0_4:
	ldr r3, [r3]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020003f0_11
	str r3, [sp, #0]
	movs r6, #9
	movs r5, #19
	movs r0, #9
	movs r1, #19
.L_020003f0_5:
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
	movs r3, #1
	str r3, [sp, #0]
	movs r0, #9
	movs r1, #51
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
.L_020003f0_7:
	str r5, [sp, #8]
	bl 0x02008098
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #41
	movs r1, #51
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
.L_020003f0_8:
	str r5, [sp, #8]
	bl 0x02008098
	b .L_020003f0_14
	.4byte 0x00001528
	.4byte 0x020092c8
	.4byte 0x0200a0dc
.L_020003f0_9:
	.4byte 0x020083c1
	.4byte 0x0200836d
	.4byte 0x00000121
.L_020003f0_11:
	movs r3, #0
	str r3, [sp, #0]
	movs r6, #9
	movs r5, #19
	movs r0, #9
	movs r1, #19
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
	movs r3, #1
	str r3, [sp, #0]
	movs r0, #9
	movs r1, #83
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #41
	movs r1, #83
	movs r2, #16
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl 0x02008098
.L_020003f0_14:
	movs r0, #1
	bl 0x02008ffc
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x02009024
	movs r0, #1
	bl 0x02008ffc
	ldr r0, [pc, #36]
	bl 0x0200900c
	ldr r3, [pc, #32]
	ldr r1, [r3]
	ldr r2, [pc, #20]
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
.L_020003f0_12:
	bl 0x02008194
	bl 0x0200904c
	bl 0x020090a4
	sub sp, #-12
	b .L_020003f0_15
	.4byte 0x00000001
.L_020003f0_13:
	.4byte 0x020083c1
	.4byte 0x020092c8
.L_020003f0_15:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global KorimaMagari_RunReturnSequence
	.thumb_func
KorimaMagari_RunReturnSequence:
	push {r5, lr}
	sub sp, #8
	bl 0x0200909c
	movs r1, #8
	movs r0, #0
	bl 0x020090dc
	movs r0, #6
	bl 0x02009094
	movs r0, #239
	bl 0x02009114
	movs r1, #128
	ldr r2, [pc, #164]
	movs r0, #8
	lsls r1, r1, #8
	bl 0x020090b4
	movs r0, #8
	movs r1, #2
	bl 0x020090dc
	movs r2, #176
	movs r1, #72
	movs r0, #8
	bl 0x020090bc
	movs r0, #6
	bl 0x02009094
	movs r0, #0
	movs r1, #2
	bl 0x020090dc
	movs r0, #0
	ldr r1, [pc, #124]
	ldr r2, [pc, #116]
	bl 0x020090b4
	movs r1, #8
	movs r2, #0
	negs r1, r1
	movs r0, #0
	bl 0x020090c4
	movs r0, #24
	bl 0x02009094
	movs r1, #1
	movs r0, #0
	bl 0x020090dc
	movs r0, #8
	bl 0x020090cc
	movs r1, #1
	movs r0, #8
	bl 0x020090dc
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009114
	movs r0, #213
	bl 0x02009114
	movs r3, #6
	str r3, [sp, #0]
	movs r5, #9
	movs r0, #5
	movs r1, #9
	movs r2, #1
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009074
	movs r3, #4
	movs r0, #0
	movs r1, #0
	movs r2, #1
	str r3, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009074
	ldr r3, [pc, #28]
	ldr r2, [r3]
	ldr r3, [pc, #12]
	strh r3, [r2]
	bl 0x020090a4
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000001
	.4byte 0x00003333
	.4byte 0x00004ccc
	.4byte 0x020092c4
	.global Func_020008b0
	.thumb_func
Func_020008b0:
	push {r5, lr}
	sub sp, #8
	bl 0x0200909c
	movs r1, #8
	movs r0, #0
	bl 0x020090dc
	movs r0, #6
	bl 0x02009094
	movs r0, #239
	bl 0x02009114
	movs r1, #128
	ldr r2, [pc, #164]
	movs r0, #8
	lsls r1, r1, #8
	bl 0x020090b4
	movs r0, #8
	movs r1, #2
	bl 0x020090dc
	movs r2, #176
	movs r1, #104
	movs r0, #8
	bl 0x020090bc
	movs r0, #6
	bl 0x02009094
	movs r0, #0
	movs r1, #2
	bl 0x020090dc
	movs r0, #0
	ldr r1, [pc, #124]
	ldr r2, [pc, #116]
	bl 0x020090b4
	movs r2, #0
	movs r1, #8
	movs r0, #0
	bl 0x020090c4
	movs r0, #24
	bl 0x02009094
	movs r1, #1
	movs r0, #0
	bl 0x020090dc
	movs r0, #8
	bl 0x020090cc
	movs r1, #1
	movs r0, #8
	bl 0x020090dc
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009114
	movs r0, #213
	bl 0x02009114
	movs r5, #9
	movs r3, #4
	movs r0, #5
	movs r1, #9
	movs r2, #1
	str r3, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009074
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #0
	movs r2, #1
.L_02000952:
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009074
	ldr r3, [pc, #32]
	ldr r2, [r3]
	ldr r3, [pc, #16]
	strh r3, [r2]
	bl 0x020090a4
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.4byte 0x020092c4
	.global Func_02000980
	.thumb_func
Func_02000980:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #168]
	ldr r1, [pc, #172]
	ldr r2, [pc, #172]
	mov r8, r3
	ldr r7, [pc, #172]
	str r2, [r1]
	adds r3, r2, #2
	mov r10, r1
	adds r2, #4
	mov r1, r8
	sub sp, #8
	str r3, [r1]
	str r2, [r7]
	movs r6, #0
	movs r5, #64
	movs r0, #32
	movs r1, #0
	movs r2, #64
	movs r3, #32
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200906c
	movs r0, #0
	movs r1, #0
	movs r2, #32
	movs r3, #32
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009074
	movs r3, #32
	movs r0, #32
	movs r1, #0
	movs r2, #32
	str r6, [sp, #0]
	str r3, [sp, #4]
	bl 0x02009074
	ldr r0, [pc, #108]
	bl 0x0200908c
	cmp r0, #0
	bne .L_02000980_0
	ldr r3, [pc, #100]
	ldr r0, [pc, #104]
	ldr r1, [r7]
	ldr r2, [pc, #104]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r10
	ldr r3, [r2]
	strh r6, [r3]
	mov r3, r8
	ldr r2, [r3]
	ldr r3, [pc, #56]
	strh r3, [r2]
.L_02000980_0:
	ldr r0, [r7]
	bl 0x02008a90
	movs r1, #255
	ldr r0, [pc, #72]
	bl 0x02008b3c
	bl 0x02008194
	movs r1, #0
	movs r0, #9
	bl 0x020090dc
	movs r0, #9
	bl 0x020090ac
	adds r0, #85
	strb r6, [r0]
	movs r0, #10
	bl 0x020090ac
	movs r3, #8
	strh r3, [r0, #32]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r0, #24]
	b .L_02000980_1
	.4byte 0x00000001
	.4byte 0x020092c8
	.4byte 0x020092c4
	.4byte 0x02001000
	.4byte 0x020092c0
	.4byte 0x00000109
	.4byte 0x040000d4
	.4byte 0x0200911c
	.4byte 0x84000012
.L_02000980_1:
	str r3, [r0, #28]
	ldr r3, [pc, #48]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r0, [pc, #36]
	bl 0x0200908c
	cmp r0, #0
	bne .L_02000980_2
	movs r0, #4
	bl 0x02008e64
.L_02000980_2:
	movs r0, #0
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000845
	.section .text.x02008c2c,"ax",%progbits
	.global Scene_PushBlockAlongRun
	.thumb_func
Scene_PushBlockAlongRun:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	movs	r2, #0
	adds	r5, r0, #0
	movs	r0, #0
	str	r2, [sp, #4]
	bl 0x020090ac
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #8
	ands	r2, r3
	ldr	r1, [pc, #484]
	ldr	r3, [r7, #8]
	mov	r8, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	add	r6, sp, #8
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	movs	r0, #128
	mov	r1, r8
	str	r3, [r6, #8]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	bl 0x0200901c
	ldr	r1, [r6, #0]
	cmp	r1, #0
	bge.n	.L_02000c8c
	ldr	r3, [pc, #440]
.L_02000c8a:
	adds	r1, r1, r3
.L_02000c8c:
	ldr	r2, [r6, #8]
	asrs	r1, r1, #20
	cmp	r2, #0
	bge.n	.L_02000c98
	ldr	r3, [pc, #428]
	adds	r2, r2, r3
.L_02000c98:
	adds	r0, r5, #0
	asrs	r2, r2, #20
	bl 0x02008b8c
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_02000ca8
	b.n	.L_02000e2c
.L_02000ca8:
	movs	r2, #0
	mov	sl, r2
	adds	r4, r6, #0
.L_02000cae:
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	lsls	r3, r3, #20
	str	r3, [r4, #0]
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	movs	r0, #128
	lsls	r3, r3, #20
	str	r3, [r4, #8]
	lsls	r0, r0, #13
	adds	r2, r4, #0
	mov	r1, r8
	str	r4, [sp, #0]
	bl 0x0200901c
	ldr	r4, [sp, #0]
	ldr	r0, [r4, #0]
	cmp	r0, #0
	bge.n	.L_02000cd8
	ldr	r3, [pc, #364]
	adds	r0, r0, r3
.L_02000cd8:
	ldr	r1, [r6, #8]
	asrs	r0, r0, #20
	cmp	r1, #0
	bge.n	.L_02000ce4
	ldr	r2, [pc, #352]
	adds	r1, r1, r2
.L_02000ce4:
	asrs	r1, r1, #20
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	str	r4, [sp, #0]
	bl 0x02008be4
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_02000d4e
	movs	r2, #1
	str	r2, [sp, #4]
	movs	r2, #6
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_02000d14
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r2, r2, r3
	mov	fp, r2
	ldr	r3, [r6, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	b.n	.L_02000d24
.L_02000d14:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r2, r2, r3
	mov	fp, r2
	ldr	r3, [r6, #8]
	movs	r2, #128
	lsls	r2, r2, #14
.L_02000d24:
	adds	r2, r2, r3
	mov	r9, r2
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L_02000d32
	ldr	r2, [pc, #276]
	adds	r3, r3, r2
.L_02000d32:
	asrs	r3, r3, #20
	strh	r3, [r5, #2]
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_02000d40
	ldr	r2, [pc, #260]
	adds	r3, r3, r2
.L_02000d40:
	asrs	r3, r3, #20
	strh	r3, [r5, #4]
	movs	r3, #1
	add	sl, r3
	mov	r2, sl
	cmp	r2, #10
	ble.n	.L_02000cae
.L_02000d4e:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_02000e2c
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #232]
	movs	r0, #128
	lsls	r0, r0, #12
	ands	r3, r2
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	ands	r3, r2
	adds	r3, r3, r0
	mov	r1, r8
	str	r3, [r6, #8]
	adds	r2, r6, #0
	bl 0x0200901c
	mov	r1, r8
	ldr	r7, [r5, #8]
	cmp	r1, #0
	bge.n	.L_02000d82
	ldr	r2, [pc, #200]
	adds	r1, r1, r2
.L_02000d82:
	asrs	r5, r1, #14
	bl 0x0200909c
	movs	r1, #8
	movs	r0, #0
	bl 0x020090dc
	movs	r0, #6
	bl 0x02009094
	ldr	r6, [pc, #180]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #48]
	movs	r0, #239
	str	r6, [r7, #52]
	bl 0x02009114
	ldr	r3, [pc, #168]
	adds	r0, r7, #0
	ldrb	r1, [r3, r5]
	bl 0x02009034
	movs	r2, #0
	mov	r3, r9
	mov	r1, fp
	adds	r0, r7, #0
	bl 0x02009054
	movs	r0, #6
	bl 0x02009094
	movs	r0, #0
	movs	r1, #2
	bl 0x020090dc
	ldr	r1, [pc, #136]
	movs	r0, #27
	bl 0x0200902c
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	adds	r1, r7, #0
	bl 0x02009044
	movs	r0, #0
	ldr	r1, [pc, #116]
	adds	r2, r6, #0
	bl 0x020090b4
	ldr	r3, [pc, #112]
	ldrsb	r1, [r3, r5]
	ldr	r3, [pc, #112]
	movs	r0, #0
	ldrsb	r2, [r3, r5]
	bl 0x020090c4
	movs	r0, #24
	bl 0x02009094
	movs	r1, #1
	movs	r0, #0
	bl 0x020090dc
	adds	r0, r7, #0
	bl 0x0200905c
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x02009034
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x02009114
	movs	r0, #213
	bl 0x02009114
	movs	r0, #15
	bl 0x02009094
	bl 0x020090a4
.L_02000e2c:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0xfff00000
	.4byte 0x000fffff
	.4byte 0x00003fff
	.4byte 0x00003333
	.4byte 0x02009164
	.4byte 0x00000ccc
	.4byte 0x00004ccc
	.4byte 0x02009168
	.2byte 0x916c
	.2byte 0x0200
	.section .rodata,"a",%progbits
	.4byte 0x000b00cd
	.4byte 0x00010009
	.4byte 0x00000000
	.4byte 0x000c00cf
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x001100cf
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x000d00cd
	.4byte 0x0001000d
	.4byte 0x00000000
	.4byte 0x000a00cf
	.4byte 0x00000012
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03030202
	.4byte 0x00f80008
	.4byte 0xf8000800
	.4byte 0xffff0000
	.4byte 0x00000197
	.4byte 0x80000114
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00200032
	.4byte 0x00000171
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x002000a2
	.4byte 0x40000054
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002b
	.4byte 0x0010202a
	.4byte 0x0020102c
	.4byte 0x000001ff
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff00d6
	.4byte 0x00000007
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x0200808d
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02008055
	.4byte 0x00008602
	.4byte 0xffff000d
	.4byte 0x020087e1
	.4byte 0x00000602
	.4byte 0xffff000c
	.4byte 0x020088b1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001526
	.4byte 0x00000003
	.4byte 0xffff0009
	.4byte 0x020083f1
	.4byte 0x00000013
	.4byte 0x0f000064
	.4byte 0x0010005b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 8
	.global gKorimaMagariRecords
gKorimaMagariRecords:
	.space 8
	.global gKorimaMagariLayout
gKorimaMagariLayout:
	.space 8
	.global KorimaPalette_First
KorimaPalette_First:
	.space 1792
	.global KorimaPalette_Second
KorimaPalette_Second:
	.space 896
	.space 896
	.global KorimaMagari_ShakeScroll
KorimaMagari_ShakeScroll:
	.space 12
	.global KorimaMagari_ShakeChance
KorimaMagari_ShakeChance:
	.space 4
