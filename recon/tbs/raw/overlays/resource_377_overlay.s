.syntax unified
	.thumb
	.section .text.x02008118,"ax",%progbits
	.balign 4
	.global FieldScene_RunActorCueBranch
	.thumb_func
FieldScene_RunActorCueBranch:
	push {r5, r6, lr}
	ldr r5, [pc, #64]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x020099c4
	movs r1, #0
	adds r0, r6, #0
	bl 0x020099cc
	movs r0, #0
	movs r1, #0
	bl 0x02009924
	cmp r0, #0
	bne .L_02000118_0
	movs r0, #10
	bl 0x02009904
	adds r0, r5, #1
	bl 0x020099c4
	b .L_02000118_1
.L_02000118_0:
	adds r0, r5, #2
	bl 0x020099c4
.L_02000118_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x020099d4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000022b9
	.section .text.x02008578,"ax",%progbits
	.balign 4
	.global FieldScene_RunComplexActorSequence
	.thumb_func
FieldScene_RunComplexActorSequence:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r2, [pc, #188]
	mov	r9, r2
	ldr	r3, [r2, #0]
	subs	r2, #76
	ldr	r7, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #17
	ldr	r6, [r3, #0]
	bl 0x0200992c
	ldr	r0, [r0, #80]
	mov	r8, r0
	bl 0x0200990c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r2, #0
	movs	r1, #0
	movs	r0, #16
	bl 0x02009974
	movs	r0, #0
	bl 0x0200992c
	movs	r1, #0
	bl 0x020098a4
	movs	r1, #18
	movs	r0, #0
	bl 0x0200997c
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [pc, #76]
	mov	r2, r8
	strh	r3, [r2, #30]
	movs	r0, #17
	bl 0x0200992c
	ldr	r5, [pc, #56]
	adds	r0, #85
	strb	r5, [r0, #0]
.L_02000608:
	movs	r0, #17
	bl 0x0200992c
	movs	r1, #0
	bl 0x020098a4
	movs	r1, #144
	lsls	r1, r1, #18
	ldr	r2, [pc, #44]
	movs	r0, #17
.L_0200061c:
	bl 0x02009974
	movs	r0, #7
	bl 0x02009884
	movs	r2, #172
	ldr	r1, [pc, #32]
	lsls	r2, r2, #18
	movs	r0, #8
	bl 0x02009974
	bl 0x020098b4
	movs	r0, #8
	b.n	.L_02000650
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001ebc
	.4byte 0x00000555
	.4byte 0x028a0000
	.2byte 0x0000
	.2byte 0x0216
.L_02000650:
	bl 0x020099f4
	ldr	r5, [pc, #880]
	movs	r1, #1
.L_02000658:
	adds	r0, r5, #0
	movs	r2, #0
	bl 0x020098dc
	movs	r0, #40
	bl 0x02009904
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
.L_0200066c:
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x020098ac
	movs	r0, #8
	bl 0x020099f4
	movs	r1, #1
	adds	r0, r5, #1
.L_02000680:
	movs	r2, #0
	bl 0x020098dc
	bl 0x020098bc
	movs	r0, #40
	bl 0x02009904
	adds	r2, r7, #0
	movs	r3, #164
.L_02000694:
	adds	r2, #236
	lsls	r3, r3, #17
	str	r3, [r2, #0]
	movs	r3, #150
	adds	r2, #4
	lsls	r3, r3, #18
	str	r3, [r2, #0]
	movs	r3, #156
	adds	r2, #4
	lsls	r3, r3, #18
.L_020006a8:
	str	r3, [r2, #0]
	movs	r3, #204
	adds	r2, #4
	lsls	r3, r3, #18
	str	r3, [r2, #0]
	movs	r3, #141
	lsls	r3, r3, #18
	str	r3, [r6, #8]
	mov	r3, sl
	str	r3, [r6, #12]
.L_020006bc:
	ldr	r3, [pc, #780]
	str	r3, [r6, #16]
	bl 0x0200986c
	movs	r0, #1
	bl 0x02009814
	mov	r2, r9
	ldr	r1, [r2, #0]
	movs	r3, #224
.L_020006d0:
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #73
	str	r3, [r2, #0]
	subs	r3, #65
	adds	r2, r1, r3
	movs	r3, #64
	str	r3, [r2, #0]
	bl 0x02009a3c
	mov	r2, r9
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #740]
	adds	r3, r3, r2
	movs	r2, #1
	strh	r2, [r3, #0]
	bl 0x02009a4c
	movs	r0, #30
	bl 0x02009814
	adds	r5, #2
	bl 0x02009a5c
	bl 0x02009a6c
	bl 0x02009a54
	movs	r1, #4
	movs	r0, #8
	bl 0x02009984
	adds	r0, r5, #0
	bl 0x020099c4
	movs	r2, #60
	ldr	r0, [pc, #696]
	movs	r1, #0
	bl 0x020099dc
	movs	r1, #2
	movs	r0, #0
	bl 0x0200999c
	movs	r0, #40
	bl 0x02009904
	movs	r1, #1
	movs	r0, #8
	bl 0x0200999c
	movs	r0, #40
	bl 0x02009904
	movs	r2, #20
	ldr	r0, [pc, #660]
	movs	r1, #0
	bl 0x020099dc
	movs	r1, #2
	movs	r0, #0
	bl 0x0200999c
	movs	r0, #7
	bl 0x0200988c
	movs	r0, #20
	bl 0x02009904
	movs	r0, #8
	bl 0x02009884
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #0
	lsls	r1, r1, #9
	bl 0x02009934
	movs	r0, #0
	movs	r1, #19
	bl 0x0200997c
	ldr	r1, [pc, #608]
	ldr	r2, [pc, #608]
	movs	r0, #0
	bl 0x0200995c
	movs	r0, #8
	bl 0x0200988c
	movs	r0, #9
	bl 0x02009884
	movs	r2, #170
	ldr	r1, [pc, #592]
	lsls	r2, r2, #2
	movs	r0, #0
	bl 0x0200995c
	movs	r0, #30
	bl 0x02009904
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x020099ec
	movs	r0, #0
	bl 0x0200992c
	movs	r1, #1
	bl 0x020098a4
	movs	r0, #0
	movs	r1, #4
	movs	r2, #0
	bl 0x0200998c
	ldr	r2, [pc, #544]
	movs	r0, #0
	ldr	r1, [pc, #544]
	bl 0x0200996c
	movs	r0, #0
	movs	r1, #3
	bl 0x020099fc
	movs	r1, #128
	movs	r2, #40
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x020099ec
	movs	r1, #4
	movs	r0, #8
	bl 0x02009984
	movs	r0, #20
	bl 0x02009904
	ldr	r0, [pc, #484]
	movs	r1, #0
	bl 0x020099d4
	bl 0x020097e4
	movs	r0, #8
	movs	r1, #2
	bl 0x02009994
	movs	r1, #0
	movs	r2, #20
	ldr	r0, [pc, #460]
	bl 0x020099dc
	movs	r0, #8
	bl 0x0200992c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #170
	lsls	r2, r2, #2
	strb	r3, [r0, #0]
	ldr	r1, [pc, #456]
	movs	r0, #8
	bl 0x0200996c
	movs	r0, #1
	bl 0x02009904
	movs	r0, #8
	bl 0x0200992c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x02009904
	movs	r1, #2
	movs	r0, #8
	bl 0x0200999c
	movs	r0, #0
	bl 0x0200992c
	movs	r1, #226
	bl 0x020098c4
	movs	r0, #33
	bl 0x020098f4
	movs	r0, #126
	bl 0x02009aac
	movs	r1, #7
	movs	r0, #0
	bl 0x020099b4
	movs	r0, #10
	bl 0x02009904
	movs	r1, #0
	movs	r0, #0
	bl 0x020099b4
	movs	r0, #20
	bl 0x02009904
	movs	r0, #8
	bl 0x0200992c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #172
	ands	r5, r3
	ldr	r1, [pc, #352]
	lsls	r2, r2, #2
	strb	r5, [r0, #0]
	movs	r0, #8
	bl 0x0200996c
	movs	r0, #1
	bl 0x02009904
	movs	r0, #8
	bl 0x0200992c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x02009904
	movs	r1, #192
	movs	r2, #192
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009934
	movs	r1, #192
	movs	r2, #192
	lsls	r2, r2, #8
	movs	r0, #0
	lsls	r1, r1, #9
	bl 0x02009934
	movs	r1, #1
	movs	r0, #8
	bl 0x02009a14
	movs	r0, #0
	bl 0x0200992c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	ldr	r5, [pc, #272]
	orrs	r6, r3
	adds	r1, r5, #0
	strb	r6, [r0, #0]
	movs	r0, #8
	bl 0x0200993c
	movs	r0, #20
	bl 0x02009904
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200993c
	movs	r0, #8
	bl 0x02009944
	movs	r0, #8
	ldr	r1, [pc, #240]
	ldr	r2, [pc, #240]
	bl 0x0200996c
	movs	r1, #204
	ldr	r2, [pc, #232]
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200996c
	movs	r0, #8
	movs	r1, #1
	bl 0x0200997c
	movs	r0, #0
	movs	r1, #1
	bl 0x0200997c
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #10
	bl 0x020099ec
	movs	r1, #0
	ldr	r0, [pc, #196]
	bl 0x020099cc
	movs	r0, #0
	movs	r1, #0
	bl 0x02009924
	cmp	r0, #0
	bne.n	.L_0200095a
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_0200095a:
	movs	r0, #20
	bl 0x02009904
	movs	r2, #20
	ldr	r0, [pc, #156]
	movs	r1, #0
	bl 0x020099dc
	movs	r0, #0
	movs	r1, #3
	bl 0x0200997c
	movs	r1, #3
	movs	r0, #8
	bl 0x02009984
	movs	r0, #20
	bl 0x02009904
	ldr	r1, [pc, #128]
	movs	r0, #8
	bl 0x0200993c
	ldr	r1, [pc, #124]
	movs	r0, #0
	bl 0x0200993c
	movs	r0, #20
	bl 0x02009904
	mov	r2, r9
	ldr	r1, [r2, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #65
	str	r3, [r2, #0]
	subs	r3, #57
	adds	r2, r1, r3
	movs	r3, #16
	str	r3, [r2, #0]
	bl 0x02009a64
	bl 0x02009a6c
	movs	r0, #20
	bl 0x02009a34
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000e52
	.4byte 0x02b30000
	.4byte 0x00001f84
	.4byte 0x00009008
	.4byte 0x0000022d
	.4byte 0x000002a7
	.4byte 0x0000022b
	.4byte 0x000002a2
	.4byte 0x0000021f
	.4byte 0x0000021e
	.4byte 0x00000216
	.4byte 0x02009ab4
	.4byte 0x000001a3
	.4byte 0x00000295
	.4byte 0x00008008
	.4byte 0x02009b04
	.2byte 0x9b34
	.2byte 0x0200
	.global FieldScene_RunPaletteRampSequence
	.thumb_func
FieldScene_RunPaletteRampSequence:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #10
	sub	sp, #8
	bl 0x0200992c
	adds	r5, r0, #0
	ldr	r6, [r5, #80]
	bl 0x0200990c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #8
	ldr	r1, [pc, #868]
	ldr	r2, [pc, #868]
	bl 0x02009974
	movs	r2, #202
	lsls	r2, r2, #17
	ldr	r1, [pc, #864]
	movs	r0, #10
.L_02000a72:
	bl 0x02009974
	movs	r0, #10
	bl 0x0200992c
	movs	r1, #0
	bl 0x020098a4
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	adds	r5, #85
	movs	r2, #0
	strb	r3, [r1, #0]
	strb	r2, [r5, #0]
	movs	r3, #13
	ldrb	r2, [r6, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
	ldr	r1, [pc, #816]
	movs	r0, #10
	bl 0x0200993c
	ldr	r2, [pc, #812]
	ldr	r3, [r2, #0]
	mov	sl, r2
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #65
	str	r2, [r3, #0]
	movs	r3, #4
	str	r3, [sp, #4]
	mov	r8, r3
	movs	r5, #5
	movs	r0, #83
	movs	r1, #15
	movs	r2, #83
	movs	r3, #19
	str	r5, [sp, #0]
	bl 0x0200987c
	mov	r2, r8
	str	r2, [sp, #4]
	movs	r0, #90
	movs	r1, #16
	movs	r2, #90
	movs	r3, #20
	str	r5, [sp, #0]
	bl 0x0200987c
	movs	r3, #7
	str	r3, [sp, #4]
	movs	r0, #77
	movs	r1, #23
	movs	r2, #82
	movs	r3, #23
	str	r5, [sp, #0]
	bl 0x0200987c
	movs	r5, #2
	movs	r0, #83
	movs	r1, #33
	movs	r2, #85
	movs	r3, #33
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200987c
	movs	r6, #1
	movs	r0, #91
	movs	r1, #28
	movs	r2, #90
	movs	r3, #28
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200987c
	movs	r0, #91
	movs	r1, #28
	movs	r2, #88
	movs	r3, #30
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200987c
	movs	r3, #6
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r0, #94
	movs	r1, #27
	movs	r2, #94
	movs	r3, #23
	bl 0x0200987c
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #92
	movs	r1, #28
	movs	r2, #87
	movs	r3, #23
	bl 0x0200987c
	movs	r0, #65
	movs	r1, #53
	movs	r2, #88
	movs	r3, #24
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200987c
	bl 0x02009894
	ldr	r2, [pc, #632]
	ldr	r3, [pc, #632]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #632]
	ldr	r5, [pc, #636]
	strh	r3, [r5, #0]
	bl 0x02009a3c
	mov	r2, sl
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #628]
	adds	r3, r3, r2
	strh	r6, [r3, #0]
	bl 0x02009a4c
	movs	r0, #30
	bl 0x02009814
	movs	r0, #8
	movs	r1, #1
	bl 0x02009a14
	movs	r1, #192
	movs	r2, #192
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009934
	movs	r1, #192
	movs	r2, #192
	movs	r0, #0
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009934
	movs	r1, #192
	movs	r2, #192
	lsls	r2, r2, #8
	movs	r0, #9
	lsls	r1, r1, #9
	bl 0x02009934
	ldr	r1, [pc, #564]
	movs	r0, #0
	bl 0x0200993c
	ldr	r1, [pc, #560]
	movs	r0, #8
	bl 0x0200993c
	bl 0x02009a5c
	movs	r0, #8
	bl 0x02009944
	movs	r0, #158
	bl 0x02009aac
	movs	r1, #128
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x02009a04
	movs	r0, #8
	movs	r1, #2
	bl 0x0200999c
	movs	r1, #128
	movs	r2, #10
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x020099ec
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x02009a1c
	movs	r0, #207
	movs	r1, #1
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	ldr	r2, [pc, #488]
	bl 0x02009a24
	movs	r1, #207
	movs	r0, #9
	lsls	r1, r1, #17
	ldr	r2, [pc, #476]
	bl 0x02009974
	ldr	r1, [pc, #472]
	ldr	r2, [pc, #476]
	movs	r0, #9
	bl 0x0200996c
	bl 0x02009a2c
	ldr	r0, [pc, #468]
	bl 0x020099c4
	movs	r2, #10
	ldr	r0, [pc, #464]
	movs	r1, #0
	bl 0x020099dc
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x02009a1c
	movs	r0, #240
	movs	r1, #1
	movs	r2, #222
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x02009a24
	bl 0x02009a2c
	movs	r0, #20
	bl 0x02009904
	movs	r1, #128
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x020099ec
	movs	r0, #8
	movs	r1, #3
	bl 0x0200997c
	movs	r0, #0
	movs	r1, #3
	bl 0x02009984
	movs	r0, #9
	movs	r1, #3
	bl 0x02009984
	ldr	r2, [pc, #384]
	ldr	r1, [pc, #384]
	movs	r0, #9
	bl 0x02009964
	movs	r0, #10
	bl 0x02009904
	ldr	r1, [pc, #376]
	movs	r0, #8
	bl 0x0200993c
	ldr	r1, [pc, #372]
	movs	r0, #0
	bl 0x0200993c
	movs	r0, #234
	bl 0x02009aac
	movs	r0, #20
	bl 0x02009904
	ldr	r1, [pc, #356]
	movs	r0, #10
	bl 0x0200993c
	movs	r6, #0
.L_02000cc0:
	ldr	r2, [pc, #348]
	adds	r3, r6, r2
	strh	r3, [r5, #0]
	movs	r0, #1
	adds	r6, #1
	bl 0x02009814
	cmp	r6, #3
	bls.n	.L_02000cc0
	movs	r0, #202
	bl 0x02009aac
	movs	r0, #10
	bl 0x02009814
	ldr	r7, [pc, #324]
	ldr	r5, [pc, #260]
	movs	r6, #0
.L_02000ce4:
	subs	r3, r7, r6
	strh	r3, [r5, #0]
	movs	r0, #1
	adds	r6, #1
	bl 0x02009814
	cmp	r6, #15
	bls.n	.L_02000ce4
	movs	r0, #0
	bl 0x02009944
	movs	r0, #8
	movs	r1, #1
	bl 0x0200997c
	movs	r0, #8
	movs	r1, #2
	bl 0x02009994
	movs	r1, #2
	movs	r0, #0
	bl 0x0200999c
	movs	r0, #10
	bl 0x02009904
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ec
	movs	r1, #192
	movs	r2, #20
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x020099ec
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x02009a0c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x02009a0c
	movs	r0, #80
	bl 0x02009904
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r2, #20
	movs	r0, #8
	movs	r1, #0
	bl 0x020099ac
	movs	r0, #8
	movs	r1, #3
	bl 0x0200997c
	movs	r1, #3
	movs	r0, #0
	bl 0x02009984
	movs	r0, #40
	bl 0x02009904
	ldr	r0, [pc, #176]
	ldr	r1, [pc, #180]
	bl 0x02009a1c
	movs	r0, #8
	movs	r1, #1
	bl 0x02009a14
	ldr	r5, [pc, #168]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200993c
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x0200994c
	ldr	r3, [pc, #64]
	ldr	r1, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	subs	r3, #192
	str	r3, [r2, #0]
	adds	r3, #200
	adds	r2, r1, r3
	movs	r3, #32
	str	r3, [r2, #0]
	bl 0x02009a64
	bl 0x02009a6c
	movs	r0, #21
	bl 0x02009a34
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x01af0000
	.4byte 0x01870000
	.4byte 0x01cf0000
	.4byte 0x02009cec
	.4byte 0x03001ebc
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x0000100c
	.4byte 0x04000052
	.4byte 0x00001f84
	.4byte 0x02009bb4
	.4byte 0x02009b78
	.4byte 0x02120000
	.4byte 0x000001ab
	.4byte 0x000001e3
	.4byte 0x00000e5b
	.4byte 0x00008009
	.4byte 0x0000024d
	.4byte 0x0000019f
	.4byte 0x02009c04
	.4byte 0x02009c54
	.4byte 0x02009d38
	.4byte 0x0000100e
	.4byte 0x0000100f
	.4byte 0x0000cccc
	.4byte 0x00001999
	.2byte 0x9ca4
	.2byte 0x0200
	.section .text.x02008f90,"ax",%progbits
	.balign 4
	.global HaidiaBabi_RunEventScript01
	.thumb_func
HaidiaBabi_RunEventScript01:
	push {r5, lr}
	bl 0x0200990c
	movs r0, #0
	ldr r1, [pc, #848]
	ldr r2, [pc, #852]
	bl 0x02009934
	movs r0, #0
	ldr r1, [pc, #848]
	ldr r2, [pc, #848]
	bl 0x0200996c
	movs r1, #128
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #7
	bl 0x020099ec
	movs r1, #2
	movs r0, #8
	bl 0x02009994
	ldr r0, [pc, #828]
	bl 0x020099c4
	movs r0, #8
	movs r1, #0
	movs r2, #80
	bl 0x020099dc
	movs r2, #60
	movs r0, #8
	ldr r1, [pc, #812]
	bl 0x02009a04
	movs r0, #8
	movs r1, #1
	bl 0x02009994
	movs r2, #60
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #2
	movs r0, #8
	bl 0x0200999c
	movs r0, #80
	bl 0x02009904
	movs r0, #8
	ldr r1, [pc, #776]
	ldr r2, [pc, #776]
	bl 0x02009934
	movs r1, #146
	movs r2, #203
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #8
	bl 0x02009954
	movs r0, #11
	bl 0x0200988c
	movs r0, #12
	bl 0x02009884
	movs r1, #12
	movs r0, #8
	bl 0x0200997c
	movs r0, #80
	bl 0x02009904
	movs r1, #2
	movs r0, #8
	bl 0x0200999c
	movs r0, #40
	bl 0x02009904
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x020099dc
	movs r1, #132
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009a04
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x020099dc
	movs r2, #60
	movs r0, #0
	ldr r1, [pc, #684]
	bl 0x02009a04
	movs r0, #8
	movs r1, #13
	bl 0x02009984
	movs r2, #0
	movs r0, #8
	ldr r1, [pc, #672]
	bl 0x02009a04
	movs r1, #11
	movs r0, #8
	bl 0x0200997c
	movs r0, #40
	bl 0x02009904
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #1
	movs r0, #8
	bl 0x0200999c
	movs r0, #20
	bl 0x02009904
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #12
	movs r0, #8
	bl 0x02009984
	movs r0, #20
	bl 0x02009904
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009a04
	movs r0, #8
	movs r1, #13
	bl 0x0200997c
	movs r1, #0
	movs r0, #8
	bl 0x020099cc
	movs r0, #0
	movs r1, #0
	bl 0x02009924
	cmp r0, #1
	bne .L_02000f90_0
	ldr r3, [pc, #568]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000f90_0:
	ldr r0, [pc, #556]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000f90_1
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009a04
.L_02000f90_1:
	movs r0, #20
	bl 0x02009904
	movs r0, #8
	movs r1, #0
	bl 0x020099d4
	ldr r1, [pc, #524]
	movs r2, #60
	movs r0, #8
	bl 0x02009a04
	ldr r5, [pc, #516]
	adds r0, r5, #0
	bl 0x020099c4
	movs r1, #0
	movs r0, #8
	bl 0x020099cc
	movs r0, #0
	movs r1, #0
	bl 0x02009924
	cmp r0, #1
	bne .L_02000f90_2
	ldr r3, [pc, #476]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000f90_2:
	ldr r0, [pc, #464]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000f90_3
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009a04
.L_02000f90_3:
	movs r0, #20
	bl 0x02009904
	movs r0, #8
	movs r1, #0
	bl 0x020099d4
	movs r2, #60
	ldr r1, [pc, #432]
	movs r0, #8
	bl 0x02009a04
	adds r0, r5, #3
	bl 0x020099c4
	movs r0, #8
	movs r1, #0
	bl 0x020099d4
	movs r1, #1
	movs r0, #8
	bl 0x0200999c
	movs r0, #20
	bl 0x02009904
	movs r0, #8
	movs r1, #13
	bl 0x02009984
	movs r0, #8
	movs r1, #2
	bl 0x02009994
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #1
	movs r0, #8
	bl 0x0200999c
	movs r0, #20
	bl 0x02009904
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #2
	movs r0, #8
	bl 0x0200999c
	movs r0, #40
	bl 0x02009904
	movs r0, #0
	bl 0x0200992c
	movs r3, #0
	strh r3, [r0, #6]
	movs r0, #1
	bl 0x02009814
	movs r0, #0
	bl 0x0200992c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #194
	strb r3, [r0]
	ldr r1, [pc, #304]
	movs r0, #0
	lsls r2, r2, #1
	bl 0x02009954
	ldr r2, [pc, #240]
	movs r0, #8
	ldr r1, [pc, #296]
	bl 0x02009934
	movs r0, #8
	movs r1, #14
	bl 0x0200997c
	movs r2, #200
	ldr r1, [pc, #284]
	lsls r2, r2, #1
	movs r0, #8
	bl 0x0200995c
	movs r0, #40
	bl 0x02009904
	movs r1, #145
	movs r2, #191
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200996c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #40
	movs r0, #8
	bl 0x020099ec
	movs r0, #0
	bl 0x0200992c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #8
	movs r0, #8
	movs r2, #8
	bl 0x020099ec
	movs r0, #8
	movs r1, #0
	movs r2, #8
	bl 0x020099ec
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #8
	bl 0x020099ec
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #10
	bl 0x020099ec
	movs r0, #8
	movs r1, #4
	movs r2, #20
	bl 0x0200998c
	movs r0, #8
	movs r1, #6
	movs r2, #40
	bl 0x0200998c
	movs r0, #8
	movs r1, #4
	movs r2, #20
	bl 0x0200998c
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x020099dc
	movs r0, #8
	ldr r1, [pc, #100]
	ldr r2, [pc, #140]
	bl 0x02009934
	movs r1, #143
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200996c
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #3
	movs r0, #0
	bl 0x02009984
	movs r0, #20
	bl 0x02009904
	movs r1, #3
	movs r0, #8
	bl 0x02009984
	ldr r0, [pc, #92]
	bl 0x020098f4
	ldr r0, [pc, #88]
	bl 0x020098f4
	bl 0x02009914
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00000239
	.4byte 0x00000189
	.4byte 0x00001c66
	.4byte 0x00000101
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000105
	.4byte 0x00000103
	.4byte 0x03001ebc
	.4byte 0x0000081c
	.4byte 0x00000107
	.4byte 0x00001c6f
	.4byte 0x0000022e
	.4byte 0x00013333
	.4byte 0x0000024a
	.4byte 0x00003333
	.4byte 0x0000081e
	.4byte 0x00000203
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020a0000
	.4byte 0x00000000
	.4byte 0x02ec0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01a30000
	.4byte 0x00000000
	.4byte 0x02ec0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01a30000
	.4byte 0x00000000
	.4byte 0x02b30000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01830000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01830000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fc0000
	.4byte 0x00000000
	.4byte 0x01920000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01af0000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01e60000
	.4byte 0x00000000
	.4byte 0x01890000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020d0000
	.4byte 0x00000000
	.4byte 0x019e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fd0000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01c70000
	.4byte 0x00000000
	.4byte 0x01c20000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020d0000
	.4byte 0x00000000
	.4byte 0x019e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fd0000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01e10000
	.4byte 0x00000000
	.4byte 0x01bb0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x019f0000
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x019f0000
	.4byte 0x00000000
	.4byte 0x024d0000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00013333
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000051e
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000051e
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000012
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffd71
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000a3d
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000e
	.4byte 0xc0020000
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x00080000
	.4byte 0x80030000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00007333
	.4byte 0x00000016
	.4byte 0x00000012
	.4byte 0x00007333
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000041
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffae2
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0xc0030000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiActor8Departure
gHaidiaBabiActor8Departure:
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x02370000
	.4byte 0x00000000
	.4byte 0x02b20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x021f0000
	.4byte 0x00000000
	.4byte 0x02a20000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiBagLiftScript
gHaidiaBabiBagLiftScript:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0x00000000
	.global gHaidiaBabiBagShowScript
gHaidiaBabiBagShowScript:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000010
	.global gHaidiaBabiEntrances
gHaidiaBabiEntrances:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000a0
	.4byte 0xc00000e4
	.4byte 0x00180000
	.4byte 0x00f80000
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000001e0
	.4byte 0xc00000b0
	.4byte 0x01080000
	.4byte 0x02300008
	.4byte 0x000000f0
	.4byte 0xffff0003
	.4byte 0x000002d1
	.4byte 0xc00000c0
	.4byte 0x02480000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000090
	.4byte 0xc00001ee
	.4byte 0x00080000
	.4byte 0x01400100
	.4byte 0x00000210
	.4byte 0xffff0005
	.4byte 0x00000077
	.4byte 0x40000154
	.4byte 0x00080000
	.4byte 0x01400100
	.4byte 0x00000210
	.4byte 0xffff0006
	.4byte 0x000001a0
	.4byte 0xc0000210
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0007
	.4byte 0x00000198
	.4byte 0x40000173
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0008
	.4byte 0x000000b8
	.4byte 0x00000279
	.4byte 0x00080000
	.4byte 0x01400240
	.4byte 0x000002e0
	.4byte 0xffff0009
	.4byte 0x00000198
	.4byte 0x00000299
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff000a
	.4byte 0x00000210
	.4byte 0x400002ab
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff000b
	.4byte 0x000002c1
	.4byte 0xc0000331
	.4byte 0x02800000
	.4byte 0x03b80228
	.4byte 0x00000360
	.4byte 0xffff000c
	.4byte 0x0000036f
	.4byte 0xc00002fd
	.4byte 0x02800000
	.4byte 0x03b80228
	.4byte 0x00000360
	.4byte 0xffff0010
	.4byte 0x0000021a
	.4byte 0x400002ab
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff0013
	.4byte 0x000002f8
	.4byte 0x40000148
	.4byte 0x02a00000
	.4byte 0x03c00110
	.4byte 0x000001e8
	.4byte 0xffff0014
	.4byte 0x00000237
	.4byte 0x000002a4
	.4byte 0x04000000
	.4byte 0x04000240
	.4byte 0x00000240
	.4byte 0xffff0015
	.4byte 0x00000198
	.4byte 0x40000173
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0016
	.4byte 0x000001e0
	.4byte 0xc00000b0
	.4byte 0x01080000
	.4byte 0x02300008
	.4byte 0x000000f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiExits
gHaidiaBabiExits:
	.4byte 0x00000008
	.4byte 0x00106005
	.4byte 0x00203006
	.4byte 0x00302005
	.4byte 0x00407005
	.4byte 0x00508008
	.4byte 0x00608004
	.4byte 0x00709008
	.4byte 0x00805008
	.4byte 0x00907008
	.4byte 0x00a0c003
	.4byte 0x00b0d003
	.4byte 0x0130b08a
	.4byte 0x01415008
	.4byte 0x0150f003
	.4byte 0x0160a006
	.4byte 0x000001ff
	.global gHaidiaBabiExits2
gHaidiaBabiExits2:
	.4byte 0x00000008
	.4byte 0x00106005
	.4byte 0x00203006
	.4byte 0x00302005
	.4byte 0x00407005
	.4byte 0x00508008
	.4byte 0x00608003
	.4byte 0x00709008
	.4byte 0x00805008
	.4byte 0x00907008
	.4byte 0x00a0c003
	.4byte 0x00b0d003
	.4byte 0x01415008
	.4byte 0x0150f003
	.4byte 0x000001ff
	.global gHaidiaBabiPlacements
gHaidiaBabiPlacements:
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff00d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00018000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x00000022
	.4byte 0x00000001
	.4byte 0x00360000
	.4byte 0x00000000
	.4byte 0x029d0000
	.4byte 0x00018000
	.4byte 0xffff00e2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002e000
	.4byte 0xffff001e
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
	.global gHaidiaBabiPlacements2
gHaidiaBabiPlacements2:
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x000000d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00008000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiPlacements3
gHaidiaBabiPlacements3:
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x024a0000
	.4byte 0x00000000
	.4byte 0x01960000
	.4byte 0x00020000
	.4byte 0x00000035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x000000d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00008000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x0001b000
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiPlacements4
gHaidiaBabiPlacements4:
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x0000c000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x03400000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents4
gHaidiaBabiEvents4:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200831d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008359
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008395
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020083a9
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020083d1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020083e5
	.4byte 0x000000d3
	.4byte 0xffff0064
	.4byte 0x00400955
	.4byte 0x00000023
	.4byte 0xffff0065
	.4byte 0x0040094a
	.4byte 0x00000033
	.4byte 0xffff0066
	.4byte 0x0040094b
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents
gHaidiaBabiEvents:
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00000f56
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000f57
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00000f59
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00000f5a
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020081e1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008e35
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200831d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008359
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008395
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020083a9
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020083d1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020083e5
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents2
gHaidiaBabiEvents2:
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000011a7
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000011a8
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008285
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000011af
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008e35
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000011de
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000011df
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000011e1
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000011e2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000011e0
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200831d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008359
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008395
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020083a9
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020083d1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020083e5
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents3
gHaidiaBabiEvents3:
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001c0b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001c0c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001c18
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001c19
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008e35
	.4byte 0x00000000
	.4byte 0x03000010
	.4byte 0x02008ef9
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c13
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c10
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001c11
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001c1c
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001c1d
	.4byte 0x00008d15
	.4byte 0x0300040d
	.4byte 0x02008e35
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x02008f39
	.4byte 0x00008d15
	.4byte 0x03010410
	.4byte 0x02008ef9
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x02008f65
	.4byte 0x00000000
	.4byte 0x081e0008
	.4byte 0x02008f91
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200933d
	.4byte 0x00008d15
	.4byte 0x081e0408
	.4byte 0x02008f91
	.4byte 0x00008d15
	.4byte 0x02030008
	.4byte 0x00001c7b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c78
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200831d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008359
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008395
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020083a9
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020083d1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020083e5
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0x00000023
	.4byte 0x08ad006c
	.4byte 0x001000b6
	.4byte 0x00000033
	.4byte 0x08ae006b
	.4byte 0x0020007b
	.4byte 0x00000003
	.4byte 0x08af006d
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents5
gHaidiaBabiEvents5:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002017
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002018
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002019
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000201a
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000201b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000201c
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000201d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000201e
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000201f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002020
	.4byte 0x00000021
	.4byte 0xffff0014
	.4byte 0x00000013
	.4byte 0x00000023
	.4byte 0x0f9e0067
	.4byte 0x001000c2
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029b7
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029b8
	.4byte 0x000000f3
	.4byte 0xffff00d1
	.4byte 0x004029b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents6
gHaidiaBabiEvents6:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000022b8
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008119
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000022bc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000022bd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000022be
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000022bf
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022c0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000022c1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000022c2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000022c3
	.4byte 0x00000021
	.4byte 0xffff0014
	.4byte 0x00000013
	.4byte 0x00000023
	.4byte 0x0f9e0067
	.4byte 0x001000c2
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029b7
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029b8
	.4byte 0x000000f3
	.4byte 0xffff00d1
	.4byte 0x004029b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
