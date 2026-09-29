.syntax unified
	.thumb
	.section .text.x0200a360,"ax",%progbits
	.p2align 2
	.global Scene_RunPairedActorEffectSequence
	.thumb_func
Scene_RunPairedActorEffectSequence:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #136
	bl 0x0200dd34
	movs	r3, #17
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
.L_0200237c:
	movs	r1, #10
	movs	r2, #4
	movs	r3, #2
	movs	r0, #17
	bl 0x0200dc9c
	movs	r0, #1
	bl 0x0200dd4c
	movs	r3, #128
	lsls	r3, r3, #8
	ldr	r2, [pc, #60]
	mov	r8, r3
	mov	r4, r8
	mov	r9, r2
	movs	r1, #164
	movs	r2, #168
	strh	r4, [r0, #6]
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #1
	bl 0x0200ddac
	movs	r0, #2
	bl 0x0200dd4c
	mov	r2, r8
	strh	r2, [r0, #6]
	movs	r1, #170
	movs	r2, #196
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #2
.L_020023be:
	bl 0x0200ddac
	movs	r0, #3
.L_020023c4:
	bl 0x0200dd4c
	mov	r3, r8
	movs	r1, #163
	movs	r2, #204
	b.n	.L_020023d4
	.2byte 0x0000
	.2byte 0x0000
.L_020023d4:
	strh	r3, [r0, #6]
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #3
	bl 0x0200ddac
	movs	r0, #6
	bl 0x0200dd4c
	movs	r4, #192
.L_020023e8:
	lsls	r4, r4, #6
	mov	sl, r4
	mov	r2, sl
	strh	r2, [r0, #6]
	movs	r1, #134
	movs	r2, #154
	lsls	r1, r1, #17
.L_020023f6:
	lsls	r2, r2, #16
	movs	r0, #6
	bl 0x0200ddac
	movs	r0, #21
	bl 0x0200dd4c
	mov	r3, sl
.L_02002406:
	movs	r1, #134
	movs	r2, #164
	strh	r3, [r0, #6]
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #21
	bl 0x0200ddac
	movs	r0, #20
	bl 0x0200dd4c
	adds	r7, r0, #0
	adds	r3, r7, #0
	movs	r6, #10
	adds	r3, #100
	strh	r6, [r3, #0]
.L_02002426:
	movs	r3, #208
	lsls	r3, r3, #8
	movs	r1, #147
.L_0200242c:
	movs	r2, #212
	strh	r3, [r7, #6]
	lsls	r2, r2, #16
	movs	r0, #20
	lsls	r1, r1, #17
	bl 0x0200ddac
	movs	r0, #20
	movs	r1, #9
	bl 0x0200ddb4
	ldr	r4, [pc, #1004]
	mov	fp, r4
	mov	r1, fp
	movs	r0, #20
	bl 0x0200dd64
	movs	r0, #20
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	movs	r0, #19
	bl 0x0200dd4c
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #100
	movs	r5, #0
	movs	r1, #143
	movs	r2, #192
	strh	r6, [r3, #0]
	lsls	r2, r2, #16
	strh	r5, [r7, #6]
	movs	r0, #19
	lsls	r1, r1, #17
	bl 0x0200ddac
	movs	r0, #19
	movs	r1, #7
	bl 0x0200ddb4
	mov	r1, fp
	movs	r0, #19
	bl 0x0200dd64
	movs	r0, #19
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	bl 0x0200de4c
	mov	r2, r9
	adds	r0, #85
	strb	r2, [r0, #0]
	movs	r1, #128
	movs	r0, #152
	movs	r2, #180
	movs	r3, #0
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	bl 0x0200de3c
	movs	r0, #1
	bl 0x0200dbfc
	bl 0x0200dc7c
	movs	r0, #1
	bl 0x0200dbfc
	movs	r6, #128
	bl 0x0200de94
	bl 0x0200dea4
	movs	r0, #80
	bl 0x0200dd2c
	lsls	r6, r6, #6
	movs	r2, #20
	movs	r0, #1
	movs	r1, #2
	bl 0x0200ddcc
	adds	r1, r6, #0
	movs	r0, #1
	bl 0x020088a8
	movs	r7, #160
.L_020024e8:
	ldr	r0, [pc, #840]
	bl 0x0200ddf4
	lsls	r7, r7, #8
	ldr	r0, [pc, #836]
	bl 0x02008894
.L_020024f6:
	adds	r1, r7, #0
	movs	r0, #3
	bl 0x020088a8
	movs	r0, #3
	bl 0x02008894
	movs	r2, #0
	movs	r0, #1
	mov	r1, r8
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #2
	bl 0x020088a8
	movs	r0, #21
	movs	r1, #2
.L_0200251a:
	bl 0x0200dddc
	ldr	r1, [pc, #796]
	ldr	r2, [pc, #796]
.L_02002522:
	movs	r0, #21
	bl 0x0200dd5c
	movs	r0, #21
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #140
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r2, #164
	movs	r0, #21
	bl 0x0200dd9c
	movs	r0, #1
	bl 0x0200dd2c
	movs	r0, #21
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #2
	bl 0x0200de24
	movs	r0, #2
	bl 0x02008894
	movs	r1, #4
	movs	r0, #21
	bl 0x0200ddbc
	movs	r0, #21
	bl 0x02008894
	movs	r2, #20
	movs	r0, #1
	ldr	r1, [pc, #700]
	bl 0x0200de24
	movs	r1, #1
	movs	r0, #1
	bl 0x0200dddc
	movs	r0, #1
	bl 0x02008894
	movs	r0, #21
	movs	r1, #1
	bl 0x0200dddc
	movs	r1, #0
	movs	r0, #21
	bl 0x020088a8
	movs	r0, #21
	bl 0x02008894
	movs	r1, #2
	movs	r0, #3
	bl 0x0200ddd4
	movs	r0, #3
	bl 0x02008894
	movs	r1, #4
	movs	r0, #3
	bl 0x0200ddb4
	movs	r0, #3
	bl 0x02008894
	movs	r2, #40
	movs	r0, #21
	ldr	r1, [pc, #632]
	bl 0x0200de24
	mov	r1, sl
	movs	r0, #21
	bl 0x020088a8
	movs	r0, #21
	bl 0x02008894
	movs	r2, #20
	movs	r1, #2
	movs	r0, #2
	bl 0x0200ddcc
	movs	r0, #2
	bl 0x02008894
	movs	r1, #3
	movs	r0, #21
	bl 0x0200ddbc
	movs	r0, #21
	bl 0x02008894
	movs	r0, #1
	movs	r1, #1
	bl 0x0200dddc
	movs	r0, #1
	adds	r1, r6, #0
	bl 0x020088a8
	movs	r1, #0
	movs	r0, #1
	bl 0x0200ddfc
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #224
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #0
	movs	r1, #0
	bl 0x0200dd44
	movs	r7, #1
.L_0200263a:
	cmp	r0, #0
	beq.n	.L_02002650
.L_0200263e:
	ldr	r3, [pc, #524]
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r7, #0
.L_02002650:
	movs	r0, #20
	bl 0x0200dbfc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
.L_0200265c:
	movs	r2, #0
	bl 0x0200de14
.L_02002662:
	movs	r1, #160
	movs	r2, #0
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200de14
	movs	r4, #160
	lsls	r4, r4, #8
	mov	sl, r4
	movs	r0, #3
	mov	r1, sl
.L_02002678:
	bl 0x020088a8
	movs	r0, #21
	movs	r1, #0
	bl 0x020088a8
	movs	r0, #21
	movs	r1, #0
	bl 0x0200de04
	movs	r0, #17
	bl 0x0200decc
	movs	r0, #40
	bl 0x0200dd2c
	cmp	r7, #0
	beq.n	.L_020026ac
	ldr	r3, [pc, #428]
	movs	r0, #236
	ldr	r2, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r2, r0
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_020026ac:
	movs	r0, #20
	bl 0x0200dd74
	movs	r0, #19
	bl 0x0200dd74
	movs	r0, #1
.L_020026ba:
	bl 0x0200dbfc
	movs	r0, #20
	bl 0x0200dd4c
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r7, r0, #0
	str	r2, [r7, #24]
	str	r2, [r7, #28]
	movs	r0, #19
.L_020026d0:
	mov	r8, r2
	bl 0x0200dd4c
.L_020026d6:
	mov	r3, r8
	adds	r7, r0, #0
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	movs	r0, #1
	bl 0x0200dbfc
	movs	r0, #20
	bl 0x02008894
	movs	r1, #192
.L_020026ec:
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
.L_0200270a:
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r2, #40
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #20
	bl 0x0200ddb4
	movs	r0, #20
	bl 0x0200dd4c
	movs	r1, #1
	bl 0x0200dcac
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #20
	ldr	r1, [pc, #268]
	ldr	r2, [pc, #268]
	bl 0x0200dd5c
	movs	r1, #150
	movs	r2, #206
.L_0200274e:
	lsls	r1, r1, #1
	movs	r0, #20
	bl 0x0200dd9c
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #2
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #20
	bl 0x0200dd4c
	ldr	r6, [pc, #228]
.L_02002772:
	str	r6, [r0, #24]
	movs	r0, #161
	bl 0x0200decc
	movs	r1, #8
	movs	r0, #20
	bl 0x0200ddb4
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #1
	movs	r0, #19
	bl 0x0200ddb4
	movs	r0, #19
	bl 0x0200dd4c
	movs	r1, #1
	bl 0x0200dcac
	movs	r0, #19
	movs	r1, #4
	movs	r2, #40
	bl 0x0200ddcc
	ldr	r1, [pc, #168]
	ldr	r2, [pc, #168]
	movs	r0, #19
	bl 0x0200dd5c
	movs	r0, #19
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #148
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #19
	movs	r2, #186
	bl 0x0200dd9c
	movs	r0, #19
	ldr	r1, [pc, #132]
	ldr	r2, [pc, #136]
	bl 0x0200dd5c
.L_020027d6:
	movs	r1, #146
	movs	r2, #186
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200dd9c
	movs	r0, #19
.L_020027e4:
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r4, #1
	orrs	r3, r4
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x0200dd4c
.L_020027f8:
	str	r6, [r0, #24]
	movs	r0, #161
	bl 0x0200decc
	movs	r1, #5
	movs	r0, #19
	bl 0x0200ddb4
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #2
	movs	r0, #19
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #80
	bl 0x0200dd2c
	movs	r0, #3
	movs	r1, #4
.L_0200282e:
	b.n	.L_02002860
	.4byte 0x0200e074
	.4byte 0x000027cf
	.4byte 0x00001001
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000103
	.4byte 0x00000105
	.4byte 0x03001ebc
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0xffff0000
	.2byte 0x0ccc
	.2byte 0x0000
.L_02002860:
	bl 0x0200ddbc
	movs	r2, #20
	movs	r0, #3
	movs	r1, #0
	bl 0x0200de0c
	movs	r1, #2
	movs	r0, #19
	bl 0x0200dddc
	movs	r0, #40
	bl 0x0200dd2c
	movs	r0, #19
	bl 0x02008894
	movs	r0, #0
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #224
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #20
	movs	r0, #3
	bl 0x0200de14
	movs	r0, #20
	bl 0x0200dd4c
	movs	r2, #192
	lsls	r2, r2, #6
	adds	r7, r0, #0
	mov	fp, r2
	ldr	r0, [pc, #60]
	mov	r4, r8
	mov	r3, fp
	strh	r3, [r7, #6]
	str	r4, [r7, #24]
	movs	r1, #1
	mov	r9, r0
	movs	r6, #208
	movs	r0, #20
	bl 0x0200ddb4
	lsls	r6, r6, #8
	movs	r0, #10
	bl 0x0200dd2c
	movs	r0, #20
	adds	r1, r6, #0
	bl 0x020088a8
	movs	r1, #6
	movs	r2, #0
	movs	r0, #20
	bl 0x0200ddcc
	movs	r0, #10
	bl 0x0200dd2c
	movs	r0, #0
	mov	r1, sl
	movs	r2, #0
	b.n	.L_02002930
	.2byte 0x0000
	.2byte 0x0000
.L_02002930:
	bl 0x0200de14
	movs	r0, #1
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #2
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #3
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #21
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #128
	ldr	r2, [r7, #12]
	lsls	r0, r0, #12
	mov	r8, r0
	ldr	r1, [r7, #8]
	add	r2, r8
	ldr	r3, [r7, #16]
	movs	r0, #22
	bl 0x0200dc6c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020029f2
	ldr	r6, [r5, #80]
	adds	r3, r6, #0
	adds	r3, #39
	mov	r2, r9
	strb	r2, [r3, #0]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	adds	r1, r5, #0
	movs	r2, #13
	adds	r1, #35
	negs	r2, r2
	ands	r3, r2
	ldrb	r2, [r1, #0]
	strb	r3, [r6, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	mov	r4, r9
	adds	r3, #85
	adds	r2, r5, #0
	strb	r4, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #664]
	str	r3, [r5, #48]
	ldr	r3, [pc, #664]
	movs	r1, #193
	str	r3, [r5, #52]
	lsls	r1, r1, #3
	movs	r0, #17
	bl 0x0200dc34
	str	r0, [sp, #40]
	movs	r0, #220
	bl 0x0200dcec
	movs	r3, #128
	ldr	r2, [sp, #40]
	lsls	r3, r3, #3
	adds	r3, r2, r3
	ldrb	r0, [r6, #28]
	movs	r1, #128
	adds	r2, r3, #0
	str	r3, [sp, #36]
	bl 0x0200dc4c
	movs	r0, #17
	bl 0x0200dc3c
.L_020029f2:
	movs	r1, #1
	movs	r0, #22
	bl 0x0200de1c
	movs	r0, #22
	bl 0x0200dd4c
	movs	r6, #128
	ldr	r3, [r7, #8]
	lsls	r6, r6, #14
	str	r3, [r0, #8]
	str	r6, [r0, #12]
	ldr	r3, [r7, #16]
	str	r3, [r0, #16]
	adds	r3, r0, #0
	adds	r3, #85
	movs	r1, #3
	strb	r1, [r3, #0]
	ldr	r3, [pc, #576]
	ldr	r2, [pc, #576]
	str	r3, [r0, #48]
	movs	r3, #192
	lsls	r3, r3, #8
	str	r2, [r0, #52]
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	cmp	r5, #0
	beq.n	.L_02002a4a
	adds	r3, r5, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	ldr	r3, [pc, #556]
	mov	r4, r8
	str	r3, [r5, #72]
	movs	r1, #154
	movs	r3, #164
	str	r2, [r5, #68]
	str	r4, [r5, #40]
	adds	r0, r5, #0
	lsls	r1, r1, #17
	adds	r2, r6, #0
	lsls	r3, r3, #16
	bl 0x0200dc84
.L_02002a4a:
	movs	r1, #154
	movs	r0, #22
	lsls	r1, r1, #1
	movs	r2, #164
	bl 0x0200dd8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ddac
	movs	r0, #21
	movs	r1, #0
	bl 0x0200de1c
	cmp	r5, #0
	beq.n	.L_02002afc
	ldr	r0, [pc, #500]
	bl 0x0200decc
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200dcac
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r1, #157
	movs	r3, #137
	lsls	r1, r1, #17
	adds	r2, r6, #0
	lsls	r3, r3, #16
	adds	r0, r5, #0
	bl 0x0200dc84
	adds	r0, r5, #0
	bl 0x0200dc8c
	ldr	r0, [pc, #460]
	bl 0x0200decc
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #146
	ldr	r1, [pc, #432]
	adds	r2, r6, #0
	lsls	r3, r3, #16
	adds	r0, r5, #0
	bl 0x0200dc84
	adds	r0, r5, #0
	bl 0x0200dc8c
	ldr	r0, [pc, #412]
	bl 0x0200decc
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r1, #150
	movs	r3, #154
	lsls	r1, r1, #17
	adds	r2, r6, #0
	lsls	r3, r3, #16
	adds	r0, r5, #0
	bl 0x0200dc84
	adds	r0, r5, #0
	bl 0x0200dc8c
	movs	r0, #6
	bl 0x0200dbfc
	movs	r0, #0
	str	r0, [r5, #8]
	str	r0, [r5, #12]
	str	r0, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200d688
.L_02002afc:
	movs	r1, #128
	movs	r0, #21
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r2, #30
	movs	r0, #3
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r1, #1
	movs	r0, #21
	bl 0x0200de1c
	movs	r0, #21
	bl 0x0200dd4c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
.L_02002b5a:
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #2
	bl 0x0200ddd4
	movs	r0, #2
	bl 0x02008894
	movs	r0, #3
	movs	r1, #1
	bl 0x0200dddc
	movs	r2, #40
	movs	r0, #3
	movs	r1, #0
	bl 0x0200de0c
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #20
	bl 0x02008894
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #0
	movs	r0, #6
	mov	r1, fp
	bl 0x0200de14
	movs	r0, #21
	mov	r1, fp
	bl 0x020088a8
	movs	r0, #21
	ldr	r1, [pc, #148]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #6
	ldr	r1, [pc, #140]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #0
	ldr	r1, [pc, #128]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #1
	ldr	r1, [pc, #120]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #2
	ldr	r1, [pc, #108]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #3
	ldr	r1, [pc, #100]
	movs	r2, #60
	bl 0x0200de24
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #20
	bl 0x0200de24
	movs	r0, #20
	bl 0x02008894
	movs	r0, #0
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200de14
	movs	r1, #0
	movs	r0, #1
	bl 0x0200ddfc
	movs	r0, #0
	movs	r1, #0
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02002c70
	movs	r0, #1
	movs	r1, #3
	bl 0x0200ddb4
	movs	r7, #1
	b.n	.L_02002c8a
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x00000135
	.4byte 0x011d0000
	.2byte 0x0101
	.2byte 0x0000
.L_02002c70:
	ldr	r3, [pc, #1008]
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #4
	bl 0x0200ddb4
	movs	r7, #0
.L_02002c8a:
	movs	r0, #1
	bl 0x02008894
	cmp	r7, #0
	beq.n	.L_02002ca4
	ldr	r3, [pc, #972]
	movs	r4, #236
	ldr	r2, [r3, #0]
	lsls	r4, r4, #1
	adds	r2, r2, r4
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_02002ca4:
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #24
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	movs	r1, #7
	movs	r0, #24
	bl 0x0200dde4
	movs	r0, #24
	bl 0x0200dd4c
	movs	r4, #0
	adds	r7, r0, #0
	ldr	r0, [pc, #924]
	mov	r8, r4
	ldr	r2, [pc, #924]
	adds	r3, r7, #0
	str	r0, [r7, #28]
	mov	r9, r0
	adds	r3, #85
	mov	r0, r8
	str	r2, [r7, #24]
	mov	sl, r2
	strb	r0, [r3, #0]
	movs	r6, #152
	movs	r3, #128
	movs	r2, #158
	lsls	r2, r2, #16
	lsls	r3, r3, #15
	lsls	r6, r6, #17
	str	r3, [r7, #12]
	str	r2, [r7, #16]
	str	r6, [r7, #8]
	movs	r0, #25
	mov	fp, r2
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	movs	r1, #7
	movs	r0, #25
	bl 0x0200dde4
	movs	r0, #25
	bl 0x0200dd4c
	mov	r3, r9
	adds	r7, r0, #0
	str	r3, [r7, #28]
	adds	r3, r7, #0
	mov	r4, sl
	mov	r0, r8
	adds	r3, #85
	str	r4, [r7, #24]
	strb	r0, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #15
	mov	r2, fp
	movs	r1, #128
	str	r3, [r7, #12]
	str	r6, [r7, #8]
	str	r2, [r7, #16]
	movs	r0, #21
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
.L_02002d8c:
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
.L_02002d96:
	movs	r1, #208
	movs	r0, #21
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #0
.L_02002da4:
	movs	r0, #6
	movs	r1, #0
	bl 0x0200de14
	ldr	r6, [pc, #704]
	movs	r0, #24
	adds	r1, r6, #0
	bl 0x0200dd64
	adds	r1, r6, #0
	movs	r0, #25
	bl 0x0200dd64
	movs	r0, #145
	bl 0x0200decc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200dcbc
	movs	r1, #0
	ldr	r0, [pc, #668]
	bl 0x0200de7c
	movs	r0, #16
	bl 0x0200de8c
	movs	r0, #20
	bl 0x0200dbfc
	movs	r1, #0
	ldr	r0, [pc, #652]
	bl 0x0200de7c
	movs	r0, #24
	bl 0x0200de8c
	movs	r0, #60
	bl 0x0200dbfc
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #632]
	bl 0x0200dc04
	movs	r0, #141
	bl 0x0200decc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
.L_02002e12:
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200dcbc
	movs	r1, #0
	ldr	r0, [pc, #596]
	bl 0x0200de7c
	movs	r0, #120
	bl 0x0200de8c
.L_02002e2a:
	ldr	r6, [pc, #596]
	movs	r0, #24
	adds	r1, r6, #0
	bl 0x0200dd64
	adds	r1, r6, #0
	movs	r0, #25
	bl 0x0200dd64
	movs	r0, #120
	bl 0x0200dbfc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
.L_02002e4e:
	bl 0x0200dcbc
	movs	r1, #0
	ldr	r0, [pc, #556]
	bl 0x0200de7c
	movs	r0, #120
.L_02002e5c:
	bl 0x0200de8c
	movs	r0, #120
	bl 0x0200dbfc
	movs	r0, #63
	bl 0x0200decc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
.L_02002e74:
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200dcbc
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
.L_02002e82:
	bl 0x0200de7c
	movs	r0, #120
	bl 0x0200de8c
	movs	r0, #120
	bl 0x0200dbfc
.L_02002e92:
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	bl 0x0200dcbc
	movs	r0, #19
	movs	r1, #1
	bl 0x0200ddb4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #19
	bl 0x0200de14
	movs	r0, #19
.L_02002eb6:
	bl 0x0200dd4c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	movs	r2, #40
	movs	r0, #19
	movs	r1, #4
	bl 0x0200ddcc
	movs	r0, #19
	movs	r1, #1
	bl 0x0200dddc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #20
	bl 0x0200de0c
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #6
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r6, #128
	movs	r0, #20
	lsls	r6, r6, #8
	bl 0x02008894
	movs	r0, #3
	movs	r1, #1
	bl 0x0200dddc
	adds	r1, r6, #0
	movs	r0, #3
	bl 0x020088a8
	movs	r0, #3
	bl 0x02008894
	movs	r0, #20
	movs	r1, #0
	bl 0x020088a8
	movs	r1, #4
	movs	r0, #20
	bl 0x0200ddbc
	movs	r0, #20
	bl 0x02008894
	movs	r2, #20
	movs	r0, #19
	ldr	r1, [pc, #288]
	bl 0x0200de24
	movs	r1, #2
	movs	r0, #19
	bl 0x0200ddd4
	movs	r0, #19
	bl 0x02008894
	movs	r1, #128
.L_02002f7c:
	movs	r2, #20
	movs	r0, #1
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r4, #192
	lsls	r4, r4, #7
	mov	fp, r4
	mov	r1, fp
	movs	r0, #1
	bl 0x020088a8
	movs	r0, #1
	bl 0x02008894
	movs	r1, #208
	movs	r0, #20
	lsls	r1, r1, #8
	bl 0x020088a8
	movs	r0, #20
	movs	r1, #3
	bl 0x0200ddbc
	movs	r2, #20
	movs	r0, #20
	movs	r1, #0
	bl 0x0200de0c
	movs	r1, #1
	movs	r0, #1
	bl 0x0200dddc
	movs	r0, #1
	bl 0x02008894
	movs	r1, #176
	movs	r2, #20
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #19
	bl 0x0200dddc
	ldr	r0, [pc, #176]
	mov	r8, r0
	bl 0x02008894
	movs	r0, #0
	movs	r1, #1
	bl 0x0200ddd4
	movs	r0, #1
	movs	r1, #1
	bl 0x0200ddd4
	movs	r0, #2
	movs	r1, #1
	bl 0x0200ddd4
	movs	r0, #3
	movs	r1, #1
	bl 0x0200dddc
	adds	r1, r6, #0
	movs	r0, #0
	movs	r2, #0
	bl 0x0200de14
	adds	r1, r6, #0
	movs	r0, #1
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #160
	lsls	r2, r2, #8
	mov	r9, r2
	mov	r1, r9
	movs	r0, #3
	bl 0x020088a8
	ldr	r1, [pc, #96]
	movs	r2, #80
	movs	r0, #21
	bl 0x0200de24
	movs	r0, #21
	bl 0x02008894
	mov	r1, r9
	movs	r0, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #2
	mov	r1, fp
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #224
	movs	r2, #40
	b.n	.L_02003094
	.4byte 0x03001ebc
	.4byte 0xffff0000
	.4byte 0x00001999
	.4byte 0x0200e088
	.4byte 0x004063ff
	.4byte 0x00007fff
	.4byte 0x0200b6d1
	.4byte 0x0200e0ac
	.4byte 0x00203210
	.4byte 0x00000103
	.4byte 0x00002013
	.2byte 0x0101
	.2byte 0x0000
.L_02003094:
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #20
	bl 0x02008894
	adds	r1, r6, #0
	movs	r0, #0
	movs	r2, #0
	bl 0x0200de14
	adds	r1, r6, #0
	movs	r0, #1
	movs	r2, #0
	bl 0x0200de14
	movs	r7, #176
	adds	r1, r6, #0
	movs	r0, #2
	movs	r2, #0
	bl 0x0200de14
	lsls	r7, r7, #8
	movs	r2, #20
	adds	r1, r6, #0
	movs	r0, #3
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #20
	bl 0x020088a8
	ldr	r3, [pc, #1008]
	mov	sl, r3
	mov	r0, sl
	bl 0x02008894
	movs	r0, #21
	movs	r1, #3
	bl 0x0200ddbc
	adds	r1, r7, #0
	movs	r0, #21
	movs	r2, #60
	bl 0x0200de14
	movs	r1, #192
	movs	r2, #40
	movs	r0, #21
	lsls	r1, r1, #6
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #19
	bl 0x0200dddc
	mov	r0, r8
	bl 0x02008894
.L_02003114:
	movs	r1, #0
	movs	r2, #40
	movs	r0, #21
	bl 0x0200de14
	movs	r0, #21
	bl 0x02008894
	movs	r1, #208
	movs	r2, #40
	movs	r0, #20
	lsls	r1, r1, #8
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #20
	bl 0x020088a8
	mov	r0, sl
	bl 0x02008894
	movs	r1, #192
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200de14
	movs	r1, #192
	movs	r2, #20
	movs	r0, #21
	lsls	r1, r1, #6
	bl 0x0200de14
	movs	r0, #21
	movs	r1, #3
	bl 0x0200ddbc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #40
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #19
	movs	r2, #20
	bl 0x0200de14
	movs	r2, #40
	ldr	r1, [pc, #860]
	movs	r0, #19
	bl 0x0200de24
	mov	r0, r8
	bl 0x02008894
	movs	r1, #192
	movs	r0, #21
	lsls	r1, r1, #6
	bl 0x020088a8
	ldr	r1, [pc, #836]
	movs	r2, #20
	movs	r0, #21
	bl 0x0200de24
	movs	r0, #21
	bl 0x02008894
	movs	r2, #20
	adds	r1, r7, #0
	movs	r0, #20
	bl 0x0200de14
	movs	r1, #4
	movs	r0, #19
	bl 0x0200ddb4
	mov	r0, r8
	bl 0x02008894
	movs	r2, #80
	ldr	r1, [pc, #800]
	movs	r0, #21
.L_020031ba:
	bl 0x0200de24
	movs	r0, #21
	bl 0x02008894
	movs	r1, #3
	movs	r0, #19
	bl 0x0200ddbc
	mov	r0, r8
	bl 0x02008894
	movs	r2, #20
	movs	r0, #21
	ldr	r1, [pc, #768]
	bl 0x0200de24
	movs	r1, #2
	movs	r0, #21
	bl 0x0200ddd4
	movs	r0, #21
	bl 0x02008894
	movs	r1, #4
	movs	r0, #19
	bl 0x0200ddb4
	mov	r0, r8
	bl 0x02008894
	movs	r1, #129
	movs	r2, #60
	movs	r0, #21
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r1, #1
	movs	r0, #21
	bl 0x0200dddc
	movs	r0, #21
	bl 0x02008894
	movs	r1, #132
	movs	r2, #40
	movs	r0, #20
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r0, #20
	movs	r1, #4
	bl 0x0200ddb4
	movs	r2, #40
	mov	r0, sl
	movs	r1, #0
	bl 0x0200de0c
	movs	r1, #1
	movs	r0, #21
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #21
	bl 0x02008894
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	mov	r1, r9
	movs	r0, #2
	movs	r2, #0
	bl 0x0200de14
	mov	r1, r9
	movs	r0, #3
	movs	r2, #20
	bl 0x0200de14
	movs	r1, #192
	movs	r2, #20
	movs	r0, #19
	lsls	r1, r1, #6
	bl 0x0200de14
	movs	r0, #19
	movs	r1, #3
	bl 0x0200ddb4
	movs	r0, #20
	movs	r1, #3
	bl 0x0200ddbc
	adds	r1, r7, #0
	movs	r2, #20
	movs	r0, #21
	bl 0x0200de14
	ldr	r0, [pc, #556]
	bl 0x02008894
	movs	r1, #128
	adds	r2, r6, #0
	movs	r0, #6
	lsls	r1, r1, #9
	bl 0x0200dd5c
	movs	r1, #128
	adds	r2, r6, #0
	movs	r0, #21
	lsls	r1, r1, #9
	bl 0x0200dd5c
	ldr	r7, [pc, #528]
	movs	r0, #21
	adds	r1, r7, #0
	bl 0x0200dd64
	movs	r0, #20
	bl 0x0200dd2c
	adds	r1, r7, #0
	movs	r0, #6
	bl 0x0200dd64
	movs	r0, #1
	mov	r1, fp
	bl 0x020088a8
	movs	r1, #2
	movs	r0, #1
	bl 0x0200ddd4
	movs	r0, #1
	bl 0x02008894
	movs	r0, #17
	bl 0x0200decc
	movs	r1, #1
	ldr	r0, [pc, #480]
	bl 0x0200de7c
	movs	r0, #40
	bl 0x0200de8c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #20
	bl 0x020088a8
	movs	r0, #20
	bl 0x02008894
	movs	r0, #0
	mov	r1, fp
	movs	r2, #0
	bl 0x0200de14
	adds	r1, r6, #0
	movs	r0, #2
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #0
	adds	r1, r6, #0
	movs	r0, #3
	bl 0x0200de14
	bl 0x0200b5c4
	movs	r0, #20
	movs	r1, #7
	bl 0x0200dde4
	movs	r1, #7
	movs	r0, #19
	bl 0x0200dde4
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #1
	bl 0x0200dde4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200dde4
	movs	r0, #20
	bl 0x0200dd2c
	bl 0x0200b5c4
	movs	r2, #20
	movs	r1, #2
	movs	r0, #3
	bl 0x0200ddcc
	movs	r0, #3
	bl 0x02008894
	movs	r1, #0
	movs	r0, #19
	bl 0x020088a8
	movs	r0, #19
	bl 0x02008894
	bl 0x0200b5c4
	movs	r0, #19
	bl 0x0200dd4c
	add	r4, sp, #96
	movs	r3, #7
	mov	sl, r0
	str	r3, [r4, #4]
	movs	r0, #128
	ldr	r3, [pc, #320]
	movs	r2, #84
	lsls	r0, r0, #9
	add	r2, sp
	mov	r9, r2
	str	r3, [r4, #36]
	str	r0, [r4, #8]
	str	r0, [r4, #12]
	mov	r8, r4
	movs	r7, #0
	mov	r6, r9
.L_020033bc:
	lsls	r3, r7, #12
	adds	r0, r3, #0
	str	r3, [sp, #32]
	bl 0x0200dc24
	movs	r3, #0
	str	r3, [r6, #4]
	str	r0, [r6, #0]
	ldr	r0, [sp, #32]
	bl 0x0200dc1c
	ldr	r3, [r6, #0]
	lsls	r2, r3, #1
	adds	r3, r3, r2
	lsls	r0, r0, #1
	str	r0, [r6, #8]
	str	r3, [r6, #0]
	mov	r4, sl
	ldr	r4, [r4, #8]
	str	r4, [sp, #28]
	mov	r2, sl
	ldr	r1, [r2, #12]
	ldr	r4, [r6, #4]
	ldr	r2, [r2, #16]
	str	r0, [sp, #4]
	ldr	r0, [pc, #252]
	str	r4, [sp, #0]
	str	r0, [sp, #8]
	mov	r4, r8
	ldr	r0, [sp, #28]
	adds	r7, #1
	str	r4, [sp, #12]
.L_020033fc:
	bl 0x0200813c
	cmp	r7, #16
	bls.n	.L_020033bc
	movs	r0, #212
	bl 0x0200decc
	movs	r0, #6
	bl 0x0200dbfc
	bl 0x0200b5c4
	movs	r0, #20
	bl 0x0200dd4c
	mov	sl, r0
	add	r0, sp, #44
	movs	r3, #7
	str	r3, [r0, #4]
.L_02003422:
	ldr	r3, [pc, #196]
	str	r3, [r0, #36]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #8]
	str	r3, [r0, #12]
	mov	r8, r0
	movs	r7, #0
	mov	r6, r9
.L_02003434:
	lsls	r2, r7, #12
	adds	r0, r2, #0
	str	r2, [sp, #24]
	bl 0x0200dc24
	movs	r3, #0
	str	r3, [r6, #4]
	str	r0, [r6, #0]
	ldr	r0, [sp, #24]
	bl 0x0200dc1c
	ldr	r3, [r6, #0]
	lsls	r2, r3, #1
	adds	r3, r3, r2
	lsls	r0, r0, #1
	str	r0, [r6, #8]
	str	r3, [r6, #0]
	mov	r4, sl
	ldr	r4, [r4, #8]
	str	r4, [sp, #20]
	mov	r2, sl
	ldr	r1, [r2, #12]
	ldr	r4, [r6, #4]
	ldr	r2, [r2, #16]
	str	r0, [sp, #4]
	ldr	r0, [pc, #132]
	str	r4, [sp, #0]
	str	r0, [sp, #8]
	mov	r4, r8
	ldr	r0, [sp, #20]
	adds	r7, #1
	str	r4, [sp, #12]
	bl 0x0200813c
.L_02003478:
	cmp	r7, #16
	bls.n	.L_02003434
	movs	r0, #212
	bl 0x0200decc
	movs	r2, #20
	movs	r1, #6
	movs	r0, #2
	bl 0x0200ddcc
	movs	r0, #54
	bl 0x0200decc
	movs	r0, #2
	bl 0x02008894
	movs	r1, #4
	movs	r0, #20
	bl 0x0200ddb4
	movs	r0, #20
	bl 0x02008894
	bl 0x0200b5c4
	movs	r0, #20
	ldr	r1, [pc, #64]
	ldr	r2, [pc, #68]
	bl 0x0200dd5c
	ldr	r1, [pc, #56]
	ldr	r2, [pc, #60]
	movs	r0, #19
	bl 0x0200dd5c
	movs	r0, #20
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r6, #254
	adds	r3, r6, #0
	b.n	.L_020034f8
	.2byte 0x0000
	.4byte 0x00002014
	.4byte 0x00000101
	.4byte 0x00000103
	.4byte 0x0000a015
	.4byte 0x0200e22c
	.4byte 0x0040250d
	.4byte 0x020083a1
	.4byte 0x01090000
	.4byte 0x00003333
	.2byte 0x1999
	.2byte 0x0000
.L_020034f8:
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #254
	ands	r2, r3
	movs	r1, #147
	str	r6, [sp, #16]
	lsls	r1, r1, #1
	strb	r2, [r0, #0]
	movs	r0, #20
	movs	r2, #196
	bl 0x0200dd84
	movs	r1, #147
	movs	r2, #196
	movs	r0, #19
	lsls	r1, r1, #1
.L_02003522:
	bl 0x0200dd84
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #128]
	bl 0x0200dc04
	movs	r0, #1
	movs	r1, #2
	bl 0x0200ddd4
	movs	r1, #0
.L_0200353a:
	movs	r0, #1
	bl 0x0200de04
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200dd14
	movs	r1, #0
	movs	r0, #2
	bl 0x0200de04
	ldr	r0, [pc, #92]
	bl 0x0200dd14
	bl 0x0200b5c4
	movs	r0, #20
	bl 0x0200dd2c
	bl 0x0200b5c4
	movs	r0, #20
	bl 0x0200dd2c
	ldr	r3, [pc, #72]
	ldr	r4, [pc, #72]
	movs	r2, #3
	adds	r3, r3, r4
	strb	r2, [r3, #0]
	ldr	r6, [pc, #68]
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x0200de6c
	adds	r0, r6, #0
	movs	r1, #9
	bl 0x0200de74
	movs	r1, #0
	movs	r0, #98
	bl 0x0200de64
	bl 0x0200dd24
	ldr	r0, [pc, #44]
	bl 0x0200dd14
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200b7c5
	.4byte 0x00000235
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x000000bb
	.2byte 0x0351
	.2byte 0x0000
	.section .text.x0200b6d0,"ax",%progbits
	.global SceneEffect_SpawnParticlesAboveActor
	.thumb_func
SceneEffect_SpawnParticlesAboveActor:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #52]
	bl 0x0200dd0c
	cmp	r0, #0
	bne.n	.L_020036ee
	ldr	r3, [pc, #44]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl 0x0200dbf4
	cmp	r0, #0
	bne.n	.L_020037b8
.L_020036ee:
	movs	r0, #24
	bl 0x0200dd4c
	adds	r5, r0, #0
	ldr	r0, [pc, #20]
	bl 0x0200dd0c
	cmp	r0, #0
	beq.n	.L_02003714
	bl 0x0200dc14
	adds	r2, r0, #0
	lsls	r2, r2, #8
	b.n	.L_0200371c
	.2byte 0x0000
	.4byte 0x00000236
	.2byte 0x1e40
	.2byte 0x0300
.L_02003714:
	bl 0x0200dc14
	adds	r2, r0, #0
	lsls	r2, r2, #6
.L_0200371c:
	ldr	r3, [r5, #12]
	lsrs	r2, r2, #16
	lsls	r2, r2, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #128]
	movs	r0, #142
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #1
	bl 0x0200dc6c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020037b8
	ldr	r1, [pc, #112]
	adds	r0, r7, #0
	ldr	r6, [r7, #80]
	bl 0x0200dc64
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200ddec
	adds	r3, r7, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	bl 0x0200dc14
	ldr	r3, [pc, #84]
	adds	r2, r7, #0
	adds	r2, #100
	ands	r3, r0
	strh	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r5, [r3, #0]
	ldr	r3, [pc, #72]
	ldr	r1, [pc, #56]
	str	r3, [r7, #108]
	mov	r8, r1
	bl 0x0200dc14
	adds	r3, r0, #0
	lsls	r0, r3, #16
	subs	r0, r0, r3
	lsrs	r0, r0, #20
	bl 0x0200dc1c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
.L_02003786:
	asrs	r3, r3, #16
	str	r3, [r7, #48]
	adds	r3, r6, #0
	adds	r3, #38
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r3, #13
	ldrb	r2, [r6, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
	b.n	.L_020037b8
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffe40000
	.4byte 0x0200e16c
	.4byte 0x0ffff000
	.2byte 0xb601
	.2byte 0x0200
.L_020037b8:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200df10
	.4byte 0x0200df48
	.4byte 0x0200df80
	.global VinasuChojo_PairActionScript
VinasuChojo_PairActionScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01ec0000
	.4byte 0x00000000
	.4byte 0x00a40000
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
	.global SceneAction_EntryGroup
SceneAction_EntryGroup:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global SceneAction_EntryPair
SceneAction_EntryPair:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_0200e074
Data_0200e074:
	.4byte 0x00000022
	.4byte 0x02008315
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200e088
Data_0200e088:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000008c
	.4byte 0x00000000
	.4byte 0x00000010
	.global Data_0200e0d0
Data_0200e0d0:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000010
	.global Data_0200e0f4
Data_0200e0f4:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_0200e130
Data_0200e130:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global VinasuChojo_OrbitParticleScript
	.type VinasuChojo_OrbitParticleScript, %object
VinasuChojo_OrbitParticleScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global VinasuChojo_RiseParticleScript
	.type VinasuChojo_RiseParticleScript, %object
VinasuChojo_RiseParticleScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
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
	.global VinasuChojo_RiseActionScript
	.type VinasuChojo_RiseActionScript, %object
VinasuChojo_RiseActionScript:
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffa00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000600
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200e324
Data_0200e324:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff4000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000078
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_0200e360
Data_0200e360:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xfffb8000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000050
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_0200e39c
Data_0200e39c:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x014a0000
	.4byte 0x00000000
	.4byte 0x00ad0000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000010
	.global Data_0200e3c0
Data_0200e3c0:
	.4byte 0x00000022
	.4byte 0x02008425
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200e3d4
Data_0200e3d4:
	.4byte 0xffff0000
	.4byte 0x00000130
	.4byte 0x300000d8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0001
	.4byte 0x000001f8
	.4byte 0x300000a8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0002
	.4byte 0x00000154
	.4byte 0x800000b8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0003
	.4byte 0x00000154
	.4byte 0x800000b8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x80000000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200e464
Data_0200e464:
	.4byte 0x000000bb
	.4byte 0x00149002
	.4byte 0x002630b4
	.4byte 0x0090c0b3
	.4byte 0x000001ff
	.global Data_0200e478
Data_0200e478:
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x0002d000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b40000
	.4byte 0x0002b000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00020000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x0002d000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00f4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
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
	.global Data_0200e6e8
Data_0200e6e8:
	.4byte 0x00000000
	.global Data_0200e6ec
Data_0200e6ec:
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02008519
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020086c1
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008519
	.4byte 0x00000002
	.4byte 0x0250000a
	.4byte 0x020085ed
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x020092c9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global VinasuChojo_BesideParticleScript
	.type VinasuChojo_BesideParticleScript, %object
VinasuChojo_BesideParticleScript:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
	.section .bss,"aw",%nobits
	.global VinasuChojo_TransitionTimer
VinasuChojo_TransitionTimer:
	.space 4
	.global VinasuChojo_TransitionStep
VinasuChojo_TransitionStep:
	.space 4
