.syntax unified
	.thumb
	.4byte 0x049b23c0
	.4byte 0x228f6a1b
	.4byte 0x189b0052
	.4byte 0x881b6d01
	.4byte 0x824b4a02
	.4byte 0x2001768a
	.4byte 0x00004770
	.2byte 0x0000
	.2byte 0x0000
	push	{lr}
	movs	r0, #21
	movs	r1, #49
	bl 0x020092c4
	pop	{pc}
	.global Func_02000064
	.thumb_func
Func_02000064:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_0200007c
	ldr	r0, [pc, #12]
	b.n	.L_0200007e
.L_0200007c:
	ldr	r0, [pc, #12]
.L_0200007e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000067
	.4byte 0x02009728
	.2byte 0x9698
	.2byte 0x0200
	.global Func_02000090
	.thumb_func
Func_02000090:
	movs	r0, #0
	bx	lr
	.global Func_02000094
	.thumb_func
Func_02000094:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9758
	.2byte 0x0200
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_020000b4
	ldr	r0, [pc, #12]
	b.n	.L_020000b6
.L_020000b4:
	ldr	r0, [pc, #12]
.L_020000b6:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000067
	.4byte 0x0200993c
	.2byte 0x978c
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200010c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200010c
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_0200010c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x020091c4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200015a
	movs	r1, #0
	bl 0x020080c8
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x020091e4
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x02009284
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x020091ec
	adds	r0, r5, #0
	b.n	.L_0200015c
.L_0200015a:
	movs	r0, #0
.L_0200015c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x020091c4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020001ae
	movs	r1, #1
	bl 0x020080c8
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x020091e4
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x02009284
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #34
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	b.n	.L_020001b0
.L_020001ae:
	movs	r0, #0
.L_020001b0:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
.L_020001f6:
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	str	r3, [sp, #0]
	ldr	r3, [pc, #444]
	mov	r8, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	sl, r1
	ldr	r7, [sp, #48]
	bl 0x0200921c
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, sl
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000234
	cmp	r7, #0
	beq.n	.L_02000234
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_0200023c
.L_02000234:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_0200023c:
	mov	r3, r8
	bl 0x020091c4
	adds	r6, r0, #0
	cmp	r6, #0
.L_02000246:
	bne.n	.L_0200024a
	b.n	.L_020003ae
.L_0200024a:
	ldr	r3, [r6, #80]
	mov	r1, sl
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	mov	r8, r3
	bl 0x020091b4
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x020091bc
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x020091e4
	ldr	r3, [pc, #324]
	mov	r1, r9
	str	r3, [r6, #108]
	ldr	r3, [sp, #0]
	adds	r0, r6, #0
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x020080c8
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #280]
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020003ae
	cmp	r7, #0
	beq.n	.L_020003ae
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002cc
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x02009284
.L_020002cc:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000304
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #3
	ldrb	r2, [r7, #0]
	adds	r0, r6, #0
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	lsls	r2, r2, #2
	mov	r1, r8
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r1, [r7, #0]
	bl 0x020080c8
.L_02000304:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000318
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02000318:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200035e
	ldr	r3, [pc, #152]
	mov	r1, fp
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02000346
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x02009134
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02000358
.L_02000346:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x02009134
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02000358:
	bl 0x02009134
	str	r0, [r6, #52]
.L_0200035e:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
.L_02000364:
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200037a
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x020091b4
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x020091bc
.L_0200037a:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200038c
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #18]
.L_0200038c:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200039e
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200039e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020003ae
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_020003ae:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x020099b4
	.4byte 0x020081b5
	.4byte 0xffff0000
	.4byte 0x49026d02
	.4byte 0x185b8a53
	.4byte 0x47708253
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r6, r1, #0
	movs	r5, #60
.L_020003e4:
	cmp	r5, #0
	beq.n	.L_020003f6
	movs	r0, #1
	bl 0x0200914c
	ldr	r3, [r7, #12]
	subs	r5, #1
	cmp	r3, r6
	bgt.n	.L_020003e4
.L_020003f6:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r1, #0
	adds	r0, r6, #0
	sub	sp, #68
	bl 0x0200921c
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #76
	beq.n	.L_02000416
	b.n	.L_0200056c
.L_02000416:
	bl 0x0200920c
	movs	r0, #0
	bl 0x020092e4
	movs	r3, #74
	movs	r2, #33
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #74
	movs	r1, #31
	movs	r2, #1
	bl 0x020091d4
	movs	r0, #15
	bl 0x0200921c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #76
	beq.n	.L_02000444
	b.n	.L_02000568
.L_02000444:
	movs	r1, #85
	adds	r2, r7, #0
	adds	r1, r1, r7
	movs	r3, #0
	adds	r2, #89
	strb	r3, [r1, #0]
	strb	r3, [r2, #0]
	mov	r8, r1
	movs	r5, #0
.L_02000456:
	movs	r0, #1
	bl 0x0200914c
	ldr	r2, [r7, #80]
	ldr	r1, [pc, #280]
	ldrh	r3, [r2, #18]
	adds	r5, #1
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	ldr	r3, [r7, #80]
	ldrh	r0, [r3, #18]
	bl 0x02009164
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r7, #8]
	asrs	r0, r0, #1
	subs	r3, r3, r0
	str	r3, [r7, #8]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	cmp	r5, #17
	bls.n	.L_02000456
	ldr	r3, [pc, #244]
	movs	r1, #192
	movs	r2, #192
	str	r3, [r7, #108]
	adds	r0, r6, #0
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x02009224
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #3
	lsls	r2, r2, #2
	adds	r0, r6, #0
	adds	r1, #169
	adds	r2, #18
	bl 0x02009234
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	mov	r1, r8
	movs	r3, #3
	str	r2, [r7, #72]
	strb	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #34
	movs	r5, #0
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	mov	sl, r2
	bl 0x02009244
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x020083dc
	movs	r0, #188
	bl 0x0200931c
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x020091f4
	movs	r0, #141
	bl 0x0200931c
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x020091f4
	ldr	r2, [r7, #80]
	movs	r3, #128
	lsls	r3, r3, #5
	strh	r3, [r2, #18]
	movs	r3, #138
	add	r4, sp, #16
	lsls	r3, r3, #1
	str	r5, [r7, #108]
	strh	r3, [r4, #24]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r4, #8]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r4, #16]
	ldr	r3, [pc, #96]
	mov	r2, sl
	str	r3, [r4, #20]
	movs	r3, #224
	str	r2, [r4, #12]
	lsls	r3, r3, #13
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #12]
	ldr	r0, [r7, #8]
	str	r3, [sp, #8]
	movs	r3, #0
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	str	r4, [sp, #12]
	bl 0x020081ec
	movs	r0, #154
	bl 0x0200931c
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200925c
	bl 0x020091fc
	movs	r0, #15
	bl 0x0200921c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #132
	bl 0x020091ac
.L_02000568:
	bl 0x02009214
.L_0200056c:
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffffff00
	.4byte 0x020083cd
	.2byte 0x3333
	.2byte 0x0001
	push	{lr}
	movs	r0, #12
	sub	sp, #8
	bl 0x0200921c
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #35
	adds	r0, #85
	strb	r3, [r2, #0]
	strb	r3, [r0, #0]
	movs	r2, #28
	movs	r3, #81
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #28
	movs	r2, #1
	movs	r3, #1
	movs	r0, #79
	bl 0x020091d4
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x020091ac
	add	sp, #8
	pop	{pc}
	push	{lr}
	movs	r0, #13
	sub	sp, #8
	bl 0x0200921c
	adds	r2, r0, #0
	adds	r1, r2, #0
	movs	r3, #0
	adds	r1, #35
	strb	r3, [r1, #0]
	adds	r1, #50
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, #20]
	str	r3, [r2, #12]
	movs	r3, #117
	movs	r2, #35
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	movs	r0, #115
	bl 0x020091d4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x020091ac
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #133
	bl 0x020091ac
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	cmp	r1, #30
	bne.n	.L_0200065a
	movs	r0, #17
	bl 0x0200921c
	movs	r3, #119
	movs	r2, #43
	adds	r5, r0, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #124
	movs	r1, #1
	movs	r2, #1
	bl 0x020091dc
	movs	r1, #239
	movs	r2, #182
	movs	r0, #17
	lsls	r1, r1, #19
	lsls	r2, r2, #18
	bl 0x0200924c
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x020091cc
	str	r0, [r5, #12]
	str	r0, [r5, #20]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #134
	bl 0x020091ac
.L_0200065a:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #136]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200921c
	ldr	r3, [r0, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r2, r5, #0
	ldr	r3, [r0, #12]
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	str	r3, [r5, #8]
	ldrh	r1, [r0, #6]
	movs	r0, #128
	lsls	r0, r0, #13
	bl 0x0200916c
	ldr	r3, [r5, #0]
	asrs	r3, r3, #20
	cmp	r3, #119
	bne.n	.L_020006ae
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #45
	bne.n	.L_020006ae
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020006e8
.L_020006ae:
	bl 0x0200920c
	movs	r0, #0
	bl 0x020092e4
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #16
	bl 0x0200927c
	ldr	r0, [pc, #36]
	bl 0x0200928c
	movs	r0, #16
	movs	r1, #0
	bl 0x02009294
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200929c
	bl 0x02009214
.L_020006e8:
	add	sp, #12
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x1d7f
	.2byte 0x0000
	push	{lr}
	adds	r0, r1, #0
	movs	r2, #0
	movs	r1, #0
	bl 0x0200924c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #230
	bl 0x020091ac
	pop	{pc}
	.global Func_0200070c
	.thumb_func
Func_0200070c:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000724
	ldr	r0, [pc, #12]
	b.n	.L_02000726
.L_02000724:
	ldr	r0, [pc, #12]
.L_02000726:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000067
	.4byte 0x020099c0
	.2byte 0x99f0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r0, #160
	ldr	r3, [r3, #0]
	ldr	r4, [pc, #32]
	lsls	r0, r0, #19
	adds	r0, #24
	lsrs	r2, r3, #4
	movs	r1, #3
.L_0200074a:
	movs	r3, #3
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r4, r3]
	subs	r1, #1
	strh	r3, [r0, #0]
	adds	r2, #1
	adds	r0, #2
	cmp	r1, #0
	bge.n	.L_0200074a
	pop	{pc}
	.4byte 0x0300122c
	.2byte 0x9ae0
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #88]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_0200079a
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r2, r1, r3
	movs	r3, #7
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #118
	adds	r2, r1, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	ldr	r0, [pc, #52]
	movs	r1, #1
	bl 0x020092b4
.L_0200079a:
	bl 0x020092fc
	movs	r1, #9
	movs	r2, #10
	movs	r0, #0
	bl 0x02009304
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r3, [pc, #28]
	adds	r1, #24
	movs	r2, #8
	ldr	r0, [pc, #28]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2190
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x02009154
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000067
	.4byte 0x03000730
	.4byte 0x02009ae0
	.2byte 0x8739
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200920c
	movs	r0, #0
	bl 0x020092e4
	bl 0x020092cc
	bl 0x020092dc
	ldr	r5, [pc, #400]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #138
	movs	r2, #244
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	bl 0x0200923c
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x02009254
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x020092ec
	movs	r1, #142
	movs	r2, #240
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	bl 0x0200923c
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x020092ec
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x02009254
	movs	r1, #146
	movs	r2, #236
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	bl 0x0200923c
	ldr	r1, [r5, #0]
	movs	r0, #22
	bl 0x020092ec
	ldr	r1, [r5, #0]
	movs	r0, #22
	bl 0x02009254
	movs	r1, #152
	movs	r2, #236
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200923c
	ldr	r0, [pc, #292]
	bl 0x0200928c
	movs	r1, #129
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x020092a4
	movs	r0, #22
	movs	r1, #0
	bl 0x02009294
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #10
	bl 0x0200929c
	movs	r0, #5
	movs	r1, #6
	movs	r2, #15
	bl 0x0200926c
	movs	r2, #23
	movs	r0, #5
	movs	r1, #6
	bl 0x0200926c
	movs	r1, #0
	movs	r0, #5
	bl 0x02009294
	movs	r0, #6
	bl 0x0200922c
	movs	r2, #0
	movs	r0, #6
	movs	r1, #5
.L_020008aa:
	bl 0x02009274
	movs	r0, #6
	movs	r1, #0
	bl 0x02009294
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x020092ec
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x020092ac
	movs	r0, #30
	bl 0x02009204
	movs	r0, #22
	movs	r1, #3
	bl 0x02009264
	movs	r0, #22
	movs	r1, #0
	bl 0x02009294
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200925c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200925c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200925c
	movs	r1, #3
	movs	r0, #22
	bl 0x02009264
	movs	r0, #30
	bl 0x02009204
	movs	r1, #146
	movs	r2, #236
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200923c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200924c
	movs	r1, #142
	movs	r2, #240
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200923c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200924c
	movs	r1, #138
	movs	r2, #244
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200923c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200924c
	movs	r1, #132
	movs	r2, #244
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200923c
	bl 0x020092d4
	movs	r0, #123
	bl 0x0200931c
	movs	r1, #128
	movs	r2, #244
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200923c
	bl 0x020092dc
	movs	r0, #1
	bl 0x020092bc
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1d86
	.2byte 0x0000
	.global Func_02000988
	.thumb_func
Func_02000988:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #129
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	ldr	r3, [pc, #500]
	subs	r2, #33
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #492]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_020009b6
	bl 0x02008768
	b.n	.L_02000b8c
.L_020009b6:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #243
	bl 0x020091a4
	cmp	r0, #0
	beq.n	.L_020009d8
	movs	r0, #254
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x020091a4
	cmp	r0, #0
	bne.n	.L_020009d8
	bl 0x020087d8
	b.n	.L_02000b8c
.L_020009d8:
	bl 0x02009314
	bl 0x0200921c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r3, [pc, #424]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #420]
	cmp	r2, r3
	beq.n	.L_020009fe
	b.n	.L_02000b8c
.L_020009fe:
	bl 0x020092f4
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #10
	movs	r3, #11
	bl 0x0200930c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x020091a4
	cmp	r0, #0
	beq.n	.L_02000a2c
	bl 0x020092fc
	movs	r0, #0
	movs	r1, #8
	movs	r2, #9
	bl 0x02009304
.L_02000a2c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #133
	bl 0x020091a4
	cmp	r0, #0
	beq.n	.L_02000a46
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200924c
	b.n	.L_02000a52
.L_02000a46:
	movs	r0, #14
	bl 0x0200921c
	movs	r3, #160
	lsls	r3, r3, #9
	str	r3, [r0, #28]
.L_02000a52:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x020091a4
	cmp	r0, #0
	beq.n	.L_02000a74
	movs	r3, #81
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #79
	movs	r1, #28
	movs	r2, #1
	movs	r3, #1
	bl 0x020091d4
.L_02000a74:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x020091a4
	cmp	r0, #0
	beq.n	.L_02000a96
	movs	r3, #117
	movs	r2, #35
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #115
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	bl 0x020091d4
.L_02000a96:
	movs	r0, #0
	movs	r1, #240
	bl 0x02008fcc
	movs	r3, #64
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x02009068
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #134
	bl 0x020091a4
	cmp	r0, #0
	beq.n	.L_02000af4
	movs	r0, #17
	bl 0x0200921c
	movs	r3, #119
	movs	r2, #43
	adds	r5, r0, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #124
	movs	r1, #1
	movs	r2, #1
	bl 0x020091dc
	movs	r1, #239
	movs	r2, #182
	movs	r0, #17
	lsls	r1, r1, #19
	lsls	r2, r2, #18
	bl 0x0200924c
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x020091cc
	str	r0, [r5, #12]
	str	r0, [r5, #20]
.L_02000af4:
	movs	r0, #16
	movs	r1, #4
	bl 0x0200925c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #132
	bl 0x020091a4
	cmp	r0, #0
	beq.n	.L_02000b4e
	movs	r3, #74
	movs	r2, #33
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #74
	movs	r1, #31
	movs	r2, #1
	bl 0x020091d4
	ldr	r1, [pc, #128]
	ldr	r2, [pc, #128]
	movs	r0, #15
	bl 0x0200924c
	movs	r0, #15
	bl 0x0200921c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200921c
	movs	r5, #0
	adds	r0, #89
	strb	r5, [r0, #0]
	movs	r1, #2
	movs	r0, #15
	bl 0x0200925c
	b.n	.L_02000b5e
.L_02000b4e:
	movs	r0, #15
	bl 0x0200921c
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #16
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02000b5e:
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r3, [pc, #68]
	movs	r2, #8
	adds	r1, #24
	ldr	r0, [pc, #64]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2190
	lsls	r1, r1, #3
	ldr	r0, [pc, #60]
	bl 0x02009154
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	movs	r3, #13
	ldrb	r2, [r1, #23]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #23]
.L_02000b8c:
	movs	r0, #0
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000067
	.4byte 0x00000066
	.4byte 0x04a90000
	.4byte 0x02120000
	.4byte 0x03000730
	.4byte 0x02009ae0
	.2byte 0x8739
	.2byte 0x0200
	.global Func_02000bb4
	.thumb_func
Func_02000bb4:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000bd4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r2, [pc, #16]
	adds	r3, #252
	str	r2, [r3, #0]
.L_02000bd4:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000066
	.2byte 0x9ac8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #420]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	mov	r9, r0
	movs	r0, #158
	mov	r4, r9
	lsls	r0, r0, #1
	movs	r3, #2
	ldrsh	r2, [r4, r3]
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	sub	sp, #4
	str	r3, [sp, #0]
	movs	r4, #156
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldr	r1, [pc, #388]
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	mov	r5, r9
	movs	r0, #0
	adds	r1, r1, r2
	adds	r5, #4
	mov	fp, r3
	mov	sl, r0
	mov	r8, r1
.L_02000c28:
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	cmp	r0, #0
	bne.n	.L_02000c32
	b.n	.L_02000d78
.L_02000c32:
	bl 0x0200921c
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	cmp	r3, #0
	bne.n	.L_02000c46
	ldr	r3, [r7, #16]
	cmp	r3, #0
	bne.n	.L_02000c46
	b.n	.L_02000d78
.L_02000c46:
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #8
	ldrsh	r3, [r5, r4]
	cmp	r2, r3
	bne.n	.L_02000c5c
	movs	r0, #206
	bl 0x0200931c
	movs	r3, #4
	strh	r3, [r5, #18]
.L_02000c5c:
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	bne.n	.L_02000c78
	movs	r0, #140
	adds	r0, #255
	bl 0x0200931c
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	strh	r3, [r5, #18]
.L_02000c78:
	movs	r4, #18
	ldrsh	r3, [r5, r4]
	ldrh	r2, [r5, #18]
	cmp	r3, #0
	beq.n	.L_02000cb2
	ldrh	r3, [r5, #4]
	movs	r0, #0
	adds	r3, r3, r2
	movs	r4, #12
	ldrsh	r2, [r5, r4]
	strh	r3, [r5, #4]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	ldrh	r1, [r5, #12]
	cmp	r3, r2
	blt.n	.L_02000c9e
	strh	r1, [r5, #4]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
.L_02000c9e:
	movs	r1, #4
	ldrsh	r2, [r5, r1]
	movs	r4, #14
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #14]
	cmp	r2, r3
	bgt.n	.L_02000cb2
	strh	r1, [r5, #4]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
.L_02000cb2:
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x020091a4
	cmp	r0, #0
	bne.n	.L_02000cd2
	ldrh	r3, [r5, #2]
	movs	r1, #6
	ldrsh	r2, [r5, r1]
	adds	r3, #1
	strh	r3, [r5, #2]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	blt.n	.L_02000cd2
	strh	r0, [r5, #2]
.L_02000cd2:
	ldr	r3, [r7, #16]
	ldr	r2, [r7, #8]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	lsls	r3, r3, #7
	adds	r1, r2, r3
	mov	r2, r9
	ldrb	r3, [r2, #2]
	ldr	r4, [sp, #0]
	add	r3, sl
	strb	r3, [r4, r1]
	movs	r0, #14
	ldrsh	r2, [r7, r0]
	movs	r4, #4
	ldrsh	r3, [r5, r4]
	adds	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_02000cf8
	adds	r2, #7
.L_02000cf8:
	asrs	r3, r2, #3
	lsls	r3, r3, #8
	mov	r0, r8
	str	r3, [r0, #0]
	mov	r2, fp
	lsls	r3, r1, #2
	adds	r1, r2, r3
	movs	r4, #18
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_02000d12
	movs	r3, #0
	b.n	.L_02000d18
.L_02000d12:
	ldrb	r2, [r1, #3]
	movs	r3, #128
	orrs	r3, r2
.L_02000d18:
	strb	r3, [r1, #3]
	movs	r0, #4
	ldrsh	r3, [r5, r0]
	cmp	r3, #0
	beq.n	.L_02000d28
	ldrb	r2, [r1, #3]
	movs	r3, #16
	orrs	r3, r2
.L_02000d28:
	strb	r3, [r1, #3]
	ldr	r6, [r5, #24]
	cmp	r6, #0
	beq.n	.L_02000d78
	movs	r1, #4
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_02000d40
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #100]
	adds	r3, r3, r2
	b.n	.L_02000d6e
.L_02000d40:
	ldrh	r0, [r5, #22]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r0, r0, r3
	strh	r0, [r5, #22]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl 0x0200915c
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r3, [pc, #76]
	lsls	r0, r0, #10
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2104
	ldrsh	r2, [r5, r1]
	ldr	r3, [r7, #12]
	ldr	r4, [pc, #64]
	lsls	r2, r2, #16
	adds	r0, r0, r4
	adds	r3, r3, r2
	adds	r3, r3, r0
.L_02000d6e:
	str	r3, [r6, #12]
	ldr	r3, [r7, #8]
	str	r3, [r6, #8]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
.L_02000d78:
	movs	r3, #1
	add	sl, r3
	movs	r2, #4
	mov	r4, sl
	add	r8, r2
	adds	r5, #28
	cmp	r4, #15
	bgt.n	.L_02000d8a
	b.n	.L_02000c28
.L_02000d8a:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200254c
	.4byte 0x0202c000
	.4byte 0xfff00000
	.4byte 0x0300021c
	.2byte 0x0000
	.2byte 0xfff2
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #500]
	sub	sp, #36
	adds	r0, r2, #4
	str	r0, [sp, #32]
	ldr	r1, [pc, #496]
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	lsls	r3, r3, #2
	adds	r3, r3, r1
	ldrh	r3, [r3, #2]
	movs	r1, #226
	lsrs	r3, r3, #5
	str	r3, [sp, #24]
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r2, [r2, #0]
	movs	r3, #192
	mov	r9, r2
	movs	r2, #0
	str	r2, [sp, #16]
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	str	r3, [sp, #12]
	ldr	r0, [sp, #12]
	ldr	r3, [pc, #452]
	ands	r0, r3
	str	r0, [sp, #12]
	ldr	r2, [r2, #4]
	ands	r2, r3
	str	r2, [sp, #8]
	ldr	r3, [r1, #0]
	movs	r1, #15
	ldr	r3, [r3, #4]
	str	r1, [sp, #28]
	str	r3, [sp, #4]
.L_02000e06:
	ldr	r3, [sp, #32]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	bne.n	.L_02000e12
	b.n	.L_02000f90
.L_02000e12:
	movs	r1, #4
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02000e1c
	b.n	.L_02000f90
.L_02000e1c:
	bl 0x0200921c
	ldr	r3, [pc, #408]
	movs	r1, #12
	mov	sl, r0
	ldr	r0, [r3, #0]
	bl 0x02009144
	movs	r1, #3
	bl 0x0200913c
	lsls	r0, r0, #3
	adds	r0, #32
	str	r0, [sp, #20]
	ldr	r1, [sp, #32]
	movs	r2, #0
	movs	r0, #4
	ldrsh	r3, [r1, r0]
	mov	fp, r2
	cmp	fp, r3
	bge.n	.L_02000ee0
.L_02000e46:
	ldr	r2, [sp, #16]
	cmp	r2, #79
	bgt.n	.L_02000ed2
	mov	r0, sl
	ldr	r3, [r0, #8]
	ldr	r1, [sp, #12]
	subs	r7, r3, r1
	mov	r3, fp
	lsls	r2, r3, #16
	ldr	r3, [r0, #12]
	ldr	r0, [sp, #4]
	adds	r3, r3, r2
	mov	r1, sl
	subs	r0, r3, r0
	ldr	r2, [sp, #8]
	ldr	r3, [r1, #16]
	mov	r8, r0
	ldr	r0, [sp, #4]
	subs	r3, r3, r2
	subs	r5, r3, r0
	mov	r1, r8
	subs	r6, r5, r1
	asrs	r3, r6, #16
	adds	r6, r3, #0
	adds	r3, r1, r5
	asrs	r3, r3, #16
	asrs	r2, r7, #16
	adds	r1, r3, #0
	movs	r3, #167
	adds	r7, r2, #0
	lsls	r3, r3, #1
	adds	r2, #7
	subs	r7, #8
	subs	r6, #16
	adds	r1, #58
	cmp	r2, r3
	bhi.n	.L_02000ed2
	movs	r0, #16
	negs	r0, r0
	cmp	r6, r0
	ble.n	.L_02000ed2
	cmp	r6, #239
	bgt.n	.L_02000ed2
	adds	r3, #177
	ands	r7, r3
	movs	r3, #255
	mov	r4, r9
	ands	r6, r3
	movs	r3, #0
	stmia	r4!, {r3}
	lsls	r3, r7, #16
	orrs	r6, r3
	ldr	r3, [pc, #272]
	orrs	r6, r3
	stmia	r4!, {r6}
	ldr	r2, [sp, #24]
	ldr	r0, [sp, #20]
	adds	r3, r2, r0
	movs	r2, #128
	lsls	r2, r2, #4
	orrs	r3, r2
	mov	r0, r9
	str	r3, [r4, #0]
	bl 0x0200919c
	ldr	r2, [sp, #16]
	movs	r1, #12
	adds	r2, #1
	str	r2, [sp, #16]
	add	r9, r1
.L_02000ed2:
	ldr	r1, [sp, #32]
	movs	r3, #16
	add	fp, r3
	movs	r0, #4
	ldrsh	r3, [r1, r0]
	cmp	fp, r3
	blt.n	.L_02000e46
.L_02000ee0:
	ldr	r2, [sp, #16]
	cmp	r2, #79
	bgt.n	.L_02000f90
	mov	r0, sl
	ldr	r3, [r0, #8]
	ldr	r1, [sp, #12]
	ldr	r0, [sp, #32]
	subs	r7, r3, r1
	movs	r3, #4
	ldrsh	r2, [r0, r3]
	mov	r1, sl
	ldr	r3, [r1, #12]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #4]
	ldr	r0, [sp, #8]
	subs	r2, r3, r2
	ldr	r3, [r1, #16]
	ldr	r1, [sp, #4]
	subs	r3, r3, r0
	subs	r5, r3, r1
	ldr	r3, [sp, #32]
	subs	r6, r5, r2
	mov	r8, r2
	movs	r2, #22
	ldrsh	r0, [r3, r2]
	bl 0x0200915c
	ldr	r3, [pc, #168]
	adds	r1, r0, #0
	ldr	r0, [pc, #168]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4929
	adds	r0, r6, r0
	mov	r2, r8
	asrs	r7, r7, #16
	adds	r0, r0, r1
	adds	r3, r2, r5
	mov	sl, r7
	asrs	r0, r0, #16
	asrs	r3, r3, #16
	adds	r1, r3, #0
	subs	r6, r0, #4
	mov	r3, sl
	movs	r0, #167
	adds	r3, #7
	lsls	r0, r0, #1
	subs	r7, #8
	adds	r1, #58
	cmp	r3, r0
	bhi.n	.L_02000f90
	movs	r2, #16
	negs	r2, r2
	cmp	r6, r2
	ble.n	.L_02000f90
	cmp	r6, #239
	bgt.n	.L_02000f90
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	mov	r4, r9
	ands	r6, r3
	movs	r3, #0
	stmia	r4!, {r3}
	lsls	r3, r7, #16
	orrs	r6, r3
	ldr	r3, [pc, #84]
	orrs	r6, r3
	stmia	r4!, {r6}
	ldr	r0, [sp, #24]
	ldr	r2, [sp, #20]
	adds	r3, r0, r2
	movs	r2, #128
	lsls	r2, r2, #4
	subs	r3, #32
	orrs	r3, r2
	mov	r0, r9
	str	r3, [r4, #0]
	bl 0x0200919c
	ldr	r0, [sp, #16]
	movs	r3, #12
	adds	r0, #1
	str	r0, [sp, #16]
	add	r9, r3
.L_02000f90:
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #32]
	subs	r1, #1
	adds	r2, #28
	str	r1, [sp, #28]
	str	r2, [sp, #32]
	cmp	r1, #0
	blt.n	.L_02000fa2
	b.n	.L_02000e06
.L_02000fa2:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200254c
	.4byte 0x020036e0
	.4byte 0xffff0000
	.4byte 0x0300122c
	.4byte 0x40002000
	.4byte 0x0300021c
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb5e0
	movs	r0, #10
	adds	r0, #255
	adds	r7, r1, #0
	ldr	r6, [pc, #124]
	bl 0x020091a4
	cmp	r0, #0
	bne.n	0x02008fea
	movs	r1, #228
	ldr	r3, [pc, #116]
	adds	r0, r6, #0
	lsls	r1, r1, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2080
	lsls	r0, r0, #4
	bl 0x02009174
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #100]
	bl 0x02009184
	bl 0x02009194
	movs	r1, #128
	strh	r0, [r6, #0]
	lsls	r0, r0, #16
	adds	r2, r5, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200918c
	adds	r0, r5, #0
	bl 0x0200917c
	movs	r0, #240
	lsls	r0, r0, #2
	bl 0x02009174
	movs	r2, #226
	lsls	r2, r2, #1
	movs	r1, #128
	adds	r3, r6, r2
	lsls	r1, r1, #3
	str	r0, [r3, #0]
	adds	r1, #141
	strh	r7, [r6, #2]
	ldr	r0, [pc, #48]
	bl 0x02009154
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x02009154
	bl 0x02009314
	bl 0x0200921c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x03000258
	.4byte 0x020093d8
	.4byte 0x02008be5
	.2byte 0x8dad
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r2
	mov	sl, r3
	ldr	r2, [pc, #132]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	mov	r8, r1
	lsls	r3, r3, #2
	adds	r3, r3, r2
	mov	r0, r8
	ldr	r7, [sp, #28]
	adds	r5, r3, #4
	bl 0x0200921c
	adds	r6, r0, #0
	mov	r0, r8
	bl 0x0200921c
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x020091e4
	movs	r0, #10
	adds	r0, #255
	bl 0x020091a4
	cmp	r0, #0
	bne.n	.L_02001100
	cmp	r7, sl
	bge.n	.L_020010b2
	mov	ip, sl
	mov	sl, r7
	mov	r7, ip
.L_020010b2:
	mov	r3, r8
	strh	r3, [r5, #0]
	mov	r3, r9
	strh	r3, [r5, #2]
	movs	r3, #180
	lsls	r3, r3, #1
	strh	r3, [r5, #6]
	movs	r3, #60
	strh	r3, [r5, #8]
	movs	r3, #240
	strh	r3, [r5, #10]
	mov	r3, sl
	ldr	r2, [pc, #44]
	strh	r3, [r5, #14]
	movs	r3, #1
	strh	r3, [r5, #16]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r2, r6, #0
	adds	r2, #89
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strh	r0, [r5, #4]
	strh	r7, [r5, #12]
	strh	r0, [r5, #18]
	strh	r0, [r5, #22]
	strh	r0, [r5, #20]
	strb	r3, [r1, #0]
	b.n	.L_02001100
	.4byte 0x00000000
	.2byte 0x254c
	.2byte 0x0200
.L_02001100:
	movs	r0, #128
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #8
	bl 0x020091c4
	adds	r1, r0, #0
	adds	r2, r1, #0
	movs	r3, #0
	adds	r2, #89
	strb	r3, [r2, #0]
	subs	r2, #4
.L_0200111a:
	strb	r3, [r2, #0]
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	str	r1, [r5, #24]
	adds	r0, r5, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0xc13c0100
	.4byte 0xb9d2cf52
	.4byte 0x13465bb3
	.4byte 0x5afce9e8
	.4byte 0xafd1ba81
	.4byte 0x9a39eb47
	.4byte 0x7445b435
	.4byte 0x2ff63444
	.4byte 0x74759d7d
	.4byte 0x581ed0e8
	.4byte 0x499ece98
	.4byte 0x66cc9bb5
	.4byte 0x28a91658
	.4byte 0xd6468754
	.4byte 0xb8fc3ca3
	.4byte 0x0394251c
	.4byte 0xa3921e4e
	.4byte 0xed0a619d
	.4byte 0x290915e5
	.4byte 0xd0d9ce84
	.4byte 0xcb0f067c
	.4byte 0x3a788051
	.4byte 0x4d0804f7
	.4byte 0xe32e6224
	.4byte 0x26d300f1
	.4byte 0xc780ce20
	.4byte 0x883c5604
	.4byte 0x991f8fbc
	.4byte 0x64fd4ba6
	.4byte 0x5026593a
	.4byte 0x42e4728b
	.4byte 0x4d0a3424
	.4byte 0xbe74d99b
	.4byte 0xe6f939cd
	.4byte 0xb126f91e
	.4byte 0xf32807ec
	.4byte 0x7c84cbcd
	.4byte 0xdc069263
	.4byte 0x984841e2
	.4byte 0x998e2689
	.4byte 0xd98df5e0
	.4byte 0x7ce0c6f9
	.4byte 0x70784b43
	.4byte 0xf7f6bc36
	.4byte 0xc7b95e4d
	.4byte 0xe7719be3
	.4byte 0x17a9ad75
	.4byte 0x586f8f25
	.4byte 0x75fcd4d3
	.4byte 0x08068958
	.4byte 0x9c9a2b93
	.4byte 0xfbcfa216
	.4byte 0x7e458240
	.4byte 0x86f9f834
	.4byte 0xfc221ebe
	.4byte 0xef811f5e
	.4byte 0x813ce478
	.4byte 0x242f015d
	.4byte 0xc6de6c1d
	.4byte 0x6faf3beb
	.4byte 0x5013be3c
	.4byte 0xf8f0a8bc
	.4byte 0x78c181c6
	.4byte 0xcf386e27
	.4byte 0xde5c3c49
	.4byte 0x6677cfc8
	.4byte 0x2a0df303
	.4byte 0x8f8df3f1
	.4byte 0xdf1e77c1
	.4byte 0x3e78c038
	.4byte 0x3de7e014
	.4byte 0x638c3f8f
	.4byte 0x4481687e
	.4byte 0x401077ce
	.4byte 0xf831f014
	.4byte 0x36800cc6
	.4byte 0x7d8b83c0
	.4byte 0x80cfbf02
	.4byte 0xe1c3400a
	.4byte 0xb40d23c6
	.4byte 0xde213ce2
	.4byte 0x9e3c2f19
	.4byte 0xe39df5f3
	.4byte 0xa8c6f9ae
	.4byte 0x8291fe62
	.4byte 0x80556f9c
	.4byte 0x88e0f404
	.4byte 0x38c0bf22
	.4byte 0xe8dadf3f
	.4byte 0xe01df023
	.4byte 0x102cef93
	.4byte 0x12c8cc41
	.4byte 0xbe40f187
	.4byte 0x39f09e71
	.4byte 0xb7dfcef8
	.4byte 0x0b10cf5e
	.4byte 0x3e012227
	.4byte 0x0e4fbaf0
	.4byte 0x344d6c12
	.4byte 0xc30f8e72
	.4byte 0xf8225e77
	.4byte 0x3bebd6fb
	.4byte 0xbdc9e3ef
	.4byte 0x3cf1a54a
	.4byte 0x323dcfe0
	.4byte 0x859f5350
	.4byte 0x38339cab
	.4byte 0xf5e0510f
	.4byte 0x3410af12
	.4byte 0x1f8088f5
	.4byte 0x9fbaf811
	.4byte 0xf19223df
	.4byte 0x607f04a3
	.4byte 0xadf82f92
	.4byte 0x04abd09e
	.4byte 0xc14f8168
	.4byte 0xaf1efc75
	.4byte 0xf82f19f3
	.4byte 0x301f82fa
	.4byte 0xd7cc2978
	.4byte 0xe0bebc73
	.4byte 0xc17dfdfd
	.4byte 0x033e31e3
	.4byte 0x1f45f3f4
	.4byte 0x18ebc7bf
	.4byte 0x043e08c6
	.4byte 0x60fe70ff
	.4byte 0x904fe2be
	.4byte 0xbaf1735e
	.4byte 0xbb803205
	.4byte 0xa18451f8
	.4byte 0xc00d1f80
	.4byte 0xf0ba1e1e
	.4byte 0x9f9af819
	.4byte 0xc17d79e3
	.4byte 0x1f7008cf
	.4byte 0x2df20be7
	.4byte 0x31f1147f
	.4byte 0xc04f8120
	.4byte 0x32147c17
	.4byte 0x5e1eefe1
	.4byte 0x7c2640a1
	.4byte 0x181fd39d
	.4byte 0xefe15ebc
	.4byte 0x67053ea8
	.4byte 0x8e7809f0
	.4byte 0xd1fc1bc7
	.4byte 0xfc0bd7c0
	.4byte 0x4aebc0dd
	.4byte 0x39f48a5e
	.4byte 0xc10f8eb8
	.4byte 0xbe147c63
	.4byte 0x3e1defe0
	.4byte 0x49f24f08
	.4byte 0x00b382f9
	.4byte 0xf82f891f
	.4byte 0xe3240ab9
	.4byte 0x0c3ea7e7
	.4byte 0x2009f2ef
	.4byte 0xf907af8a
	.4byte 0xc688dc18
	.4byte 0x1e07e087
	.4byte 0xe754707f
	.4byte 0x3f13b043
	.4byte 0xb851f3b3
	.4byte 0x09008f99
	.4byte 0xebeb483c
	.4byte 0x4b053e49
	.4byte 0xf3f2c632
	.4byte 0x3e026724
	.4byte 0xa3f2ef0e
	.4byte 0xf09a5f2b
	.4byte 0x00003efe
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000002a8
	.4byte 0x40000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000218
	.4byte 0x000001e8
	.4byte 0x02000000
	.4byte 0x08000000
	.4byte 0x00000360
	.4byte 0xffff0002
	.4byte 0x000007e8
	.4byte 0x80000318
	.4byte 0x02000000
	.4byte 0x08000000
	.4byte 0x00000360
	.4byte 0xffff0003
	.4byte 0x00000628
	.4byte 0x400002a8
	.4byte 0x02000000
	.4byte 0x08000000
	.4byte 0x00000360
	.4byte 0xffff0063
	.4byte 0x000004f8
	.4byte 0x40000228
	.4byte 0x02000000
	.4byte 0x08000000
	.4byte 0x00000360
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x40000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000066
	.4byte 0x10115002
	.4byte 0xffffffff
	.4byte 0x10216002
	.4byte 0xffffffff
	.4byte 0x10301068
	.4byte 0xffffffff
	.4byte 0x00000067
	.4byte 0x10155002
	.4byte 0xffffffff
	.4byte 0x10256002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x06180000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x06580000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x07580000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00004000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x06280000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x0002c000
	.4byte 0xffff0111
	.4byte 0x00000001
	.4byte 0x04d00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x07880000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00028000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x08ab0053
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00018000
	.4byte 0x08ab0053
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00018000
	.4byte 0x08e601a8
	.4byte 0x00000001
	.4byte 0x03300000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00028000
	.4byte 0x006100f5
	.4byte 0x00000001
	.4byte 0x05380000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00004000
	.4byte 0x18f30005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x18f30006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x18f30038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200968c
	.4byte 0x014a0000
	.4byte 0x00000000
	.4byte 0x004f0000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0x08e601a8
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02009324
	.4byte 0x02009360
	.4byte 0x0200939c
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00009815
	.4byte 0xffff000b
	.4byte 0x020086f5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0x18850003
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008661
	.4byte 0x00008d15
	.4byte 0x08860010
	.4byte 0x00001d80
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001d81
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001d82
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001d83
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001d84
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001d85
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008059
	.4byte 0x00008515
	.4byte 0x0200000a
	.4byte 0x00000000
	.4byte 0x00001815
	.4byte 0x0201000c
	.4byte 0x02008585
	.4byte 0x00001815
	.4byte 0x0202000d
	.4byte 0x020085bd
	.4byte 0x00004e15
	.4byte 0xffff000e
	.4byte 0x02008601
	.4byte 0x00008c15
	.4byte 0x0884000f
	.4byte 0x020083f9
	.4byte 0x50008805
	.4byte 0x0886000a
	.4byte 0x02008611
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02000200
	.4byte 0x03600400
	.4byte 0x00000600
	.4byte 0x01601000
	.4byte 0xffffffff
	.4byte 0xffffffff
