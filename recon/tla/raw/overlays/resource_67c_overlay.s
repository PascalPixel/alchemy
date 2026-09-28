.syntax unified
	.thumb
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200007c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200007c
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
.L_0200007c:
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
	bl 0x0200bcd4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020000ca
	movs	r1, #0
	bl 0x02008038
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bd34
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200be0c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bd3c
	adds	r0, r5, #0
	b.n	.L_020000cc
.L_020000ca:
	movs	r0, #0
.L_020000cc:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
.L_020000d6:
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x0200bcd4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200011e
	movs	r1, #1
	bl 0x02008038
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bd34
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x0200be0c
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #34
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	b.n	.L_02000120
.L_0200011e:
	movs	r0, #0
.L_02000120:
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
	bl 0x0200bd94
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, sl
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_020001a4
	cmp	r7, #0
	beq.n	.L_020001a4
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_020001ac
.L_020001a4:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_020001ac:
	mov	r3, r8
	bl 0x0200bcd4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020001ba
	b.n	.L_0200031e
.L_020001ba:
	ldr	r3, [r6, #80]
	mov	r1, sl
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	mov	r8, r3
	bl 0x0200bcc4
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200bccc
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200bd34
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
	bl 0x02008038
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
	beq.n	.L_0200031e
	cmp	r7, #0
	beq.n	.L_0200031e
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200023c
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200be0c
.L_0200023c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
.L_02000244:
	cmp	r3, #0
	beq.n	.L_02000274
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
	bl 0x02008038
.L_02000274:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000288
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02000288:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002ce
	ldr	r3, [pc, #152]
	mov	r1, fp
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020002b6
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200bc2c
.L_020002aa:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020002c8
.L_020002b6:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200bc2c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x0200bc2c
	str	r0, [r6, #52]
.L_020002ce:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002ea
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200bcc4
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200bccc
.L_020002ea:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002fc
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #18]
.L_020002fc:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200030e
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200030e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200031e
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_0200031e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c4f8
	.4byte 0x02008125
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #10
	movs	r1, #32
	bl 0x0200be64
	pop	{pc}
	.global Func_02000348
	.thumb_func
Func_02000348:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xc504
	.2byte 0x0200
	.global Func_02000350
	.thumb_func
Func_02000350:
	movs	r0, #0
	bx	lr
	.global Func_02000354
	.thumb_func
Func_02000354:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xc534
	.2byte 0x0200
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push	{lr}
	ldr	r3, [pc, #116]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_02000374
	ldr	r0, [pc, #104]
	b.n	.L_020003d0
.L_02000374:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_0200037e
	ldr	r0, [pc, #104]
	b.n	.L_020003d0
.L_0200037e:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_02000388
	ldr	r0, [pc, #100]
	b.n	.L_020003d0
.L_02000388:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_02000392
	ldr	r0, [pc, #100]
	b.n	.L_020003d0
.L_02000392:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_0200039c
	ldr	r0, [pc, #96]
	b.n	.L_020003d0
.L_0200039c:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_020003a6
	ldr	r0, [pc, #96]
	b.n	.L_020003d0
.L_020003a6:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_020003b0
	ldr	r0, [pc, #92]
	b.n	.L_020003d0
.L_020003b0:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_020003ba
	ldr	r0, [pc, #92]
	b.n	.L_020003d0
.L_020003ba:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_020003c4
	ldr	r0, [pc, #88]
	b.n	.L_020003d0
.L_020003c4:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020003ce
	ldr	r0, [pc, #88]
	b.n	.L_020003d0
.L_020003ce:
	ldr	r0, [pc, #88]
.L_020003d0:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a0
	.4byte 0x0200c6a0
	.4byte 0x000000a1
	.4byte 0x0200c748
	.4byte 0x000000a2
	.4byte 0x0200c7a8
	.4byte 0x000000a3
	.4byte 0x0200c808
	.4byte 0x000000a4
	.4byte 0x0200c8c8
	.4byte 0x000000a5
	.4byte 0x0200c910
	.4byte 0x000000a6
	.4byte 0x0200c9e8
	.4byte 0x000000a7
	.4byte 0x0200ca00
	.4byte 0x000000a8
	.4byte 0x0200ca48
	.4byte 0x000000a9
	.4byte 0x0200cb98
	.2byte 0xc688
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	sub	sp, #8
	bl 0x0200bd94
	adds	r2, r0, #0
	cmp	r5, #10
	bne.n	.L_02000486
	ldr	r3, [r2, #16]
	asrs	r5, r3, #20
	cmp	r5, #12
	bne.n	.L_020004ac
	adds	r1, r2, #0
	adds	r1, #85
	movs	r3, #0
	strb	r3, [r1, #0]
	ldr	r1, [pc, #96]
	ldr	r3, [r2, #12]
	movs	r0, #144
	adds	r3, r3, r1
	lsls	r0, r0, #4
	str	r3, [r2, #12]
	adds	r0, #90
	bl 0x0200bca4
	movs	r3, #9
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bd1c
	movs	r3, #13
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bd1c
	b.n	.L_020004ac
.L_02000486:
	cmp	r5, #11
	bne.n	.L_020004ac
	ldr	r3, [r2, #8]
	asrs	r3, r3, #20
	cmp	r3, #51
	bne.n	.L_020004ac
	adds	r1, r2, #0
	adds	r1, #85
	movs	r3, #0
	strb	r3, [r1, #0]
	ldr	r1, [pc, #20]
	ldr	r3, [r2, #12]
	movs	r0, #144
	adds	r3, r3, r1
	lsls	r0, r0, #4
	str	r3, [r2, #12]
	adds	r0, #91
	bl 0x0200bca4
.L_020004ac:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	bl 0x0200be6c
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	adds	r5, r1, #0
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r0, r5, #0
	sub	sp, #8
	adds	r6, r3, r2
	bl 0x0200bd94
	movs	r1, #0
	.2byte 0xf003
	.2byte 0xfc99
	adds	r0, r5, #0
	bl 0x02009e78
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200bdec
	ldr	r2, [pc, #132]
	ldr	r3, [r6, #12]
	movs	r5, #0
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200bd54
	bl 0x0200bcdc
	movs	r0, #12
	bl 0x0200bd94
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	adds	r7, r3, #0
	.2byte 0xf003
	.2byte 0xfc39
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xfcce
	movs	r0, #139
	lsls	r0, r0, #2
	.2byte 0xf003
	.2byte 0xfcee
	subs	r7, #42
.L_0200051a:
	lsls	r0, r5, #16
	bl 0x0200b460
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200b49c
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r0, #4
	adds	r5, #1
	bl 0x0200bc34
	cmp	r5, #31
	ble.n	.L_0200051a
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_0200055e
	movs	r3, #43
	movs	r2, #44
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #41
	movs	r1, #42
	movs	r2, #1
	movs	r3, #1
	.2byte 0xf003
	.2byte 0xfbdf
.L_0200055e:
	.2byte 0xf003
	.2byte 0xfc11
	movs	r0, #140
	lsls	r0, r0, #2
	.2byte 0xf003
	.2byte 0xfb9d
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb560
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	adds	r5, r1, #0
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r0, r5, #0
	adds	r6, r3, r2
	bl 0x0200bd94
	movs	r1, #0
	.2byte 0xf003
	.2byte 0xfc3e
	adds	r0, r5, #0
	bl 0x02009e78
	adds	r0, r5, #0
	movs	r1, #3
	.2byte 0xf003
	.2byte 0xfc27
	ldr	r2, [pc, #80]
	ldr	r3, [r6, #12]
	movs	r5, #0
	adds	r3, r3, r2
	str	r3, [r6, #12]
	.2byte 0xf003
	.2byte 0xfbd4
	bl 0x0200bcdc
	.2byte 0xf003
	.2byte 0xfbe4
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xfc79
	movs	r0, #139
	lsls	r0, r0, #2
	bl 0x0200bef4
.L_020005c2:
	lsls	r0, r5, #16
	bl 0x0200b460
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r0, #4
	adds	r5, #1
	.2byte 0xf003
	.2byte 0xfb2d
	cmp	r5, #31
	ble.n	.L_020005c2
	.2byte 0xf003
	.2byte 0xfbd1
	movs	r0, #153
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bca4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0xffe00000
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #43
	movs	r2, #44
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #42
	movs	r2, #1
	movs	r3, #1
	movs	r0, #41
	.2byte 0xf003
	.2byte 0xfb86
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200bca4
	add	sp, #8
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	adds	r7, r6, #0
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
.L_02000628:
	movs	r0, #1
	bl 0x0200bc34
	ldr	r5, [r6, #40]
	cmp	r5, #0
	bne.n	.L_02000628
	movs	r0, #188
	bl 0x0200bef4
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfafa
	strb	r5, [r7, #0]
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	sub	sp, #8
	.2byte 0xf003
	.2byte 0xfba2
	adds	r6, r0, #0
	cmp	r5, #9
	bne.n	.L_0200068c
	ldr	r3, [r6, #8]
	asrs	r7, r3, #20
	cmp	r7, #39
	bne.n	.L_0200068c
	.2byte 0xf003
	.2byte 0xfb8d
	movs	r0, #0
	bl 0x0200beac
	adds	r0, r6, #0
	bl 0x0200861c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #92
	bl 0x0200bca4
	movs	r0, #41
	movs	r1, #9
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	.2byte 0xf003
	.2byte 0xfb4a
	.2byte 0xf003
	.2byte 0xfb7c
.L_0200068c:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r1, #4
	movs	r0, #8
	sub	sp, #8
	bl 0x0200bdec
	movs	r0, #8
	bl 0x0200bd94
	movs	r1, #6
	bl 0x0200be0c
	movs	r3, #110
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #123
	movs	r1, #15
	movs	r2, #5
	movs	r3, #3
	.2byte 0xf003
	.2byte 0xfb2c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #88
	strh	r3, [r2, #0]
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	ldr	r5, [pc, #64]
	sub	sp, #8
	ldr	r3, [r5, #0]
	subs	r1, r3, #1
	str	r1, [r5, #0]
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_020006e4
	adds	r2, r3, #6
.L_020006e4:
	movs	r3, #1
	ands	r3, r1
	lsls	r0, r3, #2
	asrs	r2, r2, #3
	adds	r0, r0, r3
	movs	r1, #47
	movs	r3, #110
	subs	r1, r1, r2
	str	r3, [sp, #0]
	adds	r0, #118
	movs	r3, #2
	movs	r2, #5
	str	r1, [sp, #4]
	.2byte 0xf003
	.2byte 0xfb09
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_0200070e
	ldr	r0, [pc, #12]
	.2byte 0xf003
	.2byte 0xfa9b
.L_0200070e:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200cd84
	.2byte 0x86d1
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	ldr	r3, [pc, #116]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #128
	ldr	r0, [r3, #0]
	movs	r2, #0
	lsls	r1, r1, #7
	bl 0x0200be24
	movs	r0, #151
	lsls	r0, r0, #1
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xfbd6
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #84]
	.2byte 0xf003
	.2byte 0xfa75
	movs	r0, #160
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200be44
	movs	r0, #194
	movs	r1, #1
	movs	r2, #162
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200be4c
	bl 0x0200be54
	movs	r0, #30
	bl 0x0200bd74
	bl 0x0200bd84
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200bca4
	movs	r3, #46
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #59
	movs	r1, #16
	movs	r2, #5
	movs	r3, #32
	bl 0x0200bd1c
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x86d1
	.2byte 0x0200
	push	{lr}
	movs	r1, #228
	lsls	r1, r1, #1
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xfb9c
	movs	r0, #20
	bl 0x0200bd94
	ldr	r1, [r0, #80]
	movs	r2, #1
	ldrb	r3, [r1, #17]
	orrs	r3, r2
	adds	r2, r0, #0
	strb	r3, [r1, #17]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #13
	str	r3, [r0, #12]
	pop	{pc}
	push	{r5, r6, lr}
	sub	sp, #8
	bl 0x0200bd7c
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xfb65
	ldr	r3, [pc, #360]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #192
	ldr	r0, [r3, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xfb17
	bl 0x020087a8
	movs	r0, #78
	.2byte 0xf003
	.2byte 0xfb7a
	movs	r0, #60
	bl 0x0200bd74
	movs	r0, #162
	.2byte 0xf003
	.2byte 0xfb74
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200bd44
	movs	r0, #20
	bl 0x0200bd74
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200bd44
	movs	r0, #20
	bl 0x0200bd74
	movs	r0, #128
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #10
	lsls	r0, r0, #9
	bl 0x0200bd44
	movs	r0, #40
	bl 0x0200bd74
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #252]
	bl 0x0200bc3c
	movs	r0, #160
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	.2byte 0xf003
	.2byte 0xfaf0
	movs	r0, #156
	movs	r1, #1
	movs	r2, #252
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #17
	.2byte 0xf003
	.2byte 0xfaeb
	movs	r0, #164
	.2byte 0xf003
	.2byte 0xfb3c
	movs	r3, #80
	movs	r2, #27
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #75
	movs	r1, #96
	movs	r2, #7
	movs	r3, #13
	bl 0x0200bd24
	movs	r6, #31
	movs	r5, #16
	movs	r0, #91
	movs	r1, #96
	movs	r2, #7
	movs	r3, #10
	str	r6, [sp, #4]
	str	r5, [sp, #0]
	bl 0x0200bd24
	movs	r3, #91
	str	r3, [sp, #4]
	movs	r0, #99
	movs	r1, #96
	movs	r2, #7
	movs	r3, #14
	str	r5, [sp, #0]
	bl 0x0200bd24
	movs	r3, #17
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #5
	movs	r3, #9
	str	r6, [sp, #4]
	bl 0x0200bd1c
	movs	r6, #0
.L_020008ca:
	movs	r5, #0
.L_020008cc:
	movs	r3, #1
	ldr	r2, [pc, #132]
	bics	r3, r5
	adds	r3, r6, r3
	lsls	r3, r3, #1
	ldrb	r0, [r2, r3]
	adds	r3, #1
	ldrb	r1, [r2, r3]
	movs	r3, #80
	movs	r2, #27
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #13
	movs	r2, #7
	.2byte 0xf003
	.2byte 0xfa1c
	adds	r5, #1
	movs	r0, #1
	bl 0x0200bc34
	cmp	r5, #6
	ble.n	.L_020008cc
	adds	r6, #1
	cmp	r6, #14
	ble.n	.L_020008ca
	.2byte 0xf003
	.2byte 0xfaa9
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	.2byte 0xf003
	.2byte 0xfa18
	movs	r0, #195
	lsls	r0, r0, #1
	.2byte 0xf003
	.2byte 0xfaec
	movs	r0, #30
	.2byte 0xf003
	.2byte 0xfa29
	movs	r0, #80
	.2byte 0xf003
	.2byte 0xfae6
	.2byte 0xf003
	.2byte 0xfae8
	.2byte 0xf003
	.2byte 0xfa2a
	.2byte 0xf003
	.2byte 0xfac4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #93
	bl 0x0200bca4
	movs	r0, #68
	adds	r0, #255
	bl 0x0200bca4
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x020086d1
	.2byte 0xbfb8
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #8
	mov	r8, r0
	.2byte 0xf003
	.2byte 0xfa0b
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xfaa0
	movs	r0, #188
	.2byte 0xf003
	.2byte 0xfac1
	movs	r5, #1
	movs	r6, #2
	movs	r1, #96
	movs	r2, #19
	movs	r3, #29
	movs	r0, #107
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	.2byte 0xf003
	.2byte 0xf9b7
	movs	r0, #8
	.2byte 0xf003
	.2byte 0xf9f4
	movs	r1, #96
	movs	r2, #19
	movs	r3, #29
	movs	r0, #108
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	.2byte 0xf003
	.2byte 0xf9ac
	movs	r0, #8
	.2byte 0xf003
	.2byte 0xf9e9
	movs	r1, #98
	movs	r2, #19
	movs	r3, #29
	movs	r0, #107
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200bcf4
	movs	r0, #8
	.2byte 0xf003
	.2byte 0xf9de
	movs	r1, #98
	movs	r2, #19
	movs	r3, #29
	movs	r0, #108
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	.2byte 0xf003
	.2byte 0xf996
	movs	r0, #15
	.2byte 0xf003
	.2byte 0xf9d3
	ldr	r5, [pc, #80]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	.2byte 0xf003
	.2byte 0xf9dc
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r0, [r5, #0]
	.2byte 0xf003
	.2byte 0xf9d6
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	.2byte 0xf003
	.2byte 0xf9d2
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #52]
	str	r3, [r6, #48]
	movs	r0, #123
	.2byte 0xf003
	.2byte 0xfa7b
	movs	r1, #156
	movs	r2, #248
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	.2byte 0xf003
	.2byte 0xf9d8
	movs	r0, #5
	.2byte 0xf003
	.2byte 0xf9b1
	mov	r0, r8
	.2byte 0xf003
	.2byte 0xfa22
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	.2byte 0xf003
	.2byte 0xf9a9
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xfa3e
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #93
	bl 0x0200bc9c
	ldr	r5, [pc, #88]
	cmp	r0, #0
	bne.n	.L_02000a72
	adds	r0, r5, #0
	movs	r1, #1
	.2byte 0xf003
	.2byte 0xf98a
	movs	r1, #1
	adds	r0, r5, #1
	.2byte 0xf003
	.2byte 0xf986
	movs	r0, #228
	lsls	r0, r0, #1
	.2byte 0xf003
	.2byte 0xf98a
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_02000a8c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000a8c
.L_02000a72:
	adds	r0, r5, #0
	movs	r1, #1
	.2byte 0xf003
	.2byte 0xf971
	movs	r0, #228
	lsls	r0, r0, #1
	movs	r1, #2
	.2byte 0xf003
	.2byte 0xf970
	adds	r0, r5, #2
	movs	r1, #1
	.2byte 0xf003
	.2byte 0xf968
.L_02000a8c:
	.2byte 0xf003
	.2byte 0xf97a
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x22a2
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200b628
	movs	r1, #4
	movs	r2, #16
	movs	r3, #2
	movs	r0, #16
	bl 0x0200b688
	movs	r0, #22
	movs	r1, #4
	movs	r2, #16
	movs	r3, #0
	bl 0x0200b688
	movs	r5, #5
.L_02000ab8:
	movs	r1, #55
	movs	r0, #33
	bl 0x0200b6b8
	subs	r5, #1
	movs	r0, #15
	.2byte 0xf003
	.2byte 0xf8b6
	cmp	r5, #0
	bge.n	.L_02000ab8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #102
	.2byte 0xf003
	.2byte 0xf8e7
	pop	{r5, pc}
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r3, #81
	str	r3, [sp, #0]
	movs	r6, #14
	movs	r0, #106
	movs	r1, #14
	movs	r2, #14
	movs	r3, #13
	str	r6, [sp, #4]
	.2byte 0xf003
	.2byte 0xf91a
	movs	r3, #78
	str	r3, [sp, #4]
	movs	r5, #17
	movs	r0, #44
	movs	r1, #78
	movs	r2, #14
	movs	r3, #12
	str	r5, [sp, #0]
	.2byte 0xf003
	.2byte 0xf910
	movs	r1, #14
	movs	r2, #14
	movs	r3, #13
	movs	r0, #81
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	.2byte 0xf003
	.2byte 0xf904
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #106
	.2byte 0xf003
	.2byte 0xf8c3
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r1, #0
	ldr	r5, [r3, #32]
	.2byte 0xf003
	.2byte 0xf931
	cmp	r0, #0
	beq.n	.L_02000b58
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #8]
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	asrs	r2, r2, #20
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r1, r1, r2
	ldrb	r2, [r1, #3]
	movs	r3, #128
	orrs	r3, r2
	strb	r6, [r1, #2]
	strb	r3, [r1, #3]
.L_02000b58:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #12
	sub	sp, #8
	.2byte 0xf003
	.2byte 0xf917
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #43
	movs	r2, #1
	movs	r3, #1
	.2byte 0xf003
	.2byte 0xf8cf
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #12
	sub	sp, #8
	.2byte 0xf003
	.2byte 0xf903
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #42
	movs	r2, #1
	movs	r3, #1
	.2byte 0xf003
	.2byte 0xf8bb
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #12
	sub	sp, #8
	.2byte 0xf003
	.2byte 0xf8ef
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #41
	movs	r2, #1
	movs	r3, #1
	.2byte 0xf003
	.2byte 0xf8a7
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xf85e
	cmp	r0, #0
	beq.n	.L_02000c24
	movs	r1, #38
	movs	r0, #12
	bl 0x02008b24
	.2byte 0xf002
	.2byte 0xfd1c
	movs	r0, #12
	movs	r1, #2
	movs	r2, #45
	movs	r3, #1
	.2byte 0xf002
	.2byte 0xfd46
	movs	r5, #3
.L_02000bfe:
	movs	r0, #33
	movs	r1, #55
	bl 0x0200b6b8
	subs	r5, #1
	adds	r6, r0, #0
	cmp	r5, #0
	bge.n	.L_02000bfe
	movs	r0, #146
	lsls	r0, r0, #2
	.2byte 0xf003
	.2byte 0xf847
	cmp	r6, #1
	bne.n	.L_02000c24
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #100
	.2byte 0xf003
	.2byte 0xf840
.L_02000c24:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #160
	lsls	r2, r2, #1
	movs	r0, #9
	adds	r6, r3, r2
	adds	r5, r1, #0
	.2byte 0xf003
	.2byte 0xf8ab
	movs	r1, #5
	.2byte 0xf003
	.2byte 0xf8e4
	adds	r0, r5, #0
	bl 0x02009e78
	adds	r0, r5, #0
	movs	r1, #3
	.2byte 0xf003
	.2byte 0xf8cd
	ldr	r2, [pc, #80]
	ldr	r3, [r6, #12]
	movs	r5, #31
	adds	r3, r3, r2
	str	r3, [r6, #12]
	.2byte 0xf003
	.2byte 0xf87a
	.2byte 0xf003
	.2byte 0xf83c
	.2byte 0xf003
	.2byte 0xf88a
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xf91f
	movs	r0, #151
	lsls	r0, r0, #1
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xf93e
.L_02000c78:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r0, #4
	subs	r5, #1
	.2byte 0xf002
	.2byte 0xffd5
	cmp	r5, #0
	bge.n	.L_02000c78
	.2byte 0xf003
	.2byte 0xf879
	bl 0x02008b5c
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r0, #255
	.2byte 0xf003
	.2byte 0xf802
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb500
	ldr	r3, [pc, #24]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #6
	movs	r2, #0
	.2byte 0xf003
	.2byte 0xf89c
	.2byte 0xf003
	.2byte 0xf862
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #256]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #248]
	sub	sp, #12
	adds	r6, r0, #0
	movs	r5, #1
	cmp	r2, r3
	bne.n	.L_02000d00
	movs	r0, #8
	.2byte 0xf003
	.2byte 0xf856
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r6, #77
	bne.n	.L_02000cf6
	cmp	r3, #17
	bne.n	.L_02000cf6
	movs	r5, #0
.L_02000cf6:
	cmp	r6, #78
	bne.n	.L_02000d00
	cmp	r3, #19
	bne.n	.L_02000d00
	movs	r5, #0
.L_02000d00:
	ldr	r3, [pc, #200]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #196]
	cmp	r2, r3
	bne.n	.L_02000d3a
	cmp	r6, #77
	bne.n	.L_02000d26
	movs	r0, #10
	.2byte 0xf003
	.2byte 0xf83c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #9
	bne.n	.L_02000d26
	movs	r5, #0
.L_02000d26:
	cmp	r6, #78
	bne.n	.L_02000d3a
	movs	r0, #11
	.2byte 0xf003
	.2byte 0xf832
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #13
	bne.n	.L_02000d3a
	movs	r5, #0
.L_02000d3a:
	cmp	r5, #0
	beq.n	.L_02000dc6
	ldr	r3, [pc, #140]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	.2byte 0xf003
	.2byte 0xf824
	adds	r5, r0, #0
	.2byte 0xf003
	.2byte 0xf815
	movs	r0, #0
	.2byte 0xf003
	.2byte 0xf8aa
	b.n	.L_02000da8
.L_02000d5a:
	cmp	r0, #0
	bge.n	.L_02000d6c
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
	adds	r0, r6, #0
	bl 0x02008ca8
	b.n	.L_02000dc6
.L_02000d6c:
	ldr	r3, [r5, #16]
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r3, r3, r1
	str	r3, [r5, #16]
	ldr	r2, [r5, #8]
	ldr	r3, [pc, #92]
	ldr	r1, [pc, #96]
	ands	r3, r2
	adds	r3, r3, r1
	movs	r1, #128
	lsls	r1, r1, #9
	cmp	r3, r1
	ble.n	.L_02000d8c
	movs	r3, #128
	lsls	r3, r3, #9
.L_02000d8c:
	ldr	r1, [pc, #80]
	cmp	r3, r1
	bge.n	.L_02000d94
	ldr	r3, [pc, #76]
.L_02000d94:
	subs	r3, r2, r3
	str	r3, [r5, #8]
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r5, #6]
	movs	r0, #1
	.2byte 0xf002
	.2byte 0xff46
.L_02000da8:
	ldr	r3, [r5, #8]
	mov	r1, sp
	str	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #12
	str	r3, [r1, #4]
	adds	r0, r5, #0
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r1, #8]
	.2byte 0xf002
	.2byte 0xffb5
	cmp	r0, #0
	ble.n	.L_02000d5a
.L_02000dc6:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a4
	.4byte 0x000000a5
	.4byte 0x000fffff
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb520
	.2byte 0xf002
	.2byte 0xfc1f
	movs	r0, #44
	movs	r1, #0
	movs	r2, #34
	movs	r3, #2
	bl 0x0200b688
	movs	r5, #4
.L_02000df8:
	movs	r1, #55
	movs	r0, #33
	bl 0x0200b6b8
	subs	r5, #1
	movs	r0, #15
	.2byte 0xf002
	.2byte 0xff16
	cmp	r5, #0
	bge.n	.L_02000df8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #103
	.2byte 0xf002
	.2byte 0xff47
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #168]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	mov	r8, r0
	ldr	r0, [r5, #0]
	.2byte 0xf002
	.2byte 0xffb3
	adds	r6, r0, #0
	.2byte 0xf002
	.2byte 0xffa4
	movs	r0, #0
	bl 0x0200beac
	ldr	r0, [r5, #0]
	movs	r1, #1
	.2byte 0xf002
	.2byte 0xfff5
	adds	r3, r6, #0
	adds	r3, #85
	movs	r7, #0
	strb	r7, [r3, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r6, #0
	movs	r1, #18
	.2byte 0xf002
	.2byte 0xff33
	movs	r0, #215
	.2byte 0xf003
	.2byte 0xf848
	movs	r0, #13
	.2byte 0xf002
	.2byte 0xfee5
	adds	r0, r6, #0
	movs	r1, #9
	.2byte 0xf002
	.2byte 0xff6d
	adds	r0, r6, #0
	movs	r1, #28
	bl 0x0200bcc4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #54
	.2byte 0xf003
	.2byte 0xf838
	movs	r5, #0
.L_02000e86:
	cmp	r5, #30
	bne.n	.L_02000e8e
	bl 0x0200be94
.L_02000e8e:
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r7, r7, r3
	ldr	r3, [r6, #12]
	adds	r3, r3, r7
	str	r3, [r6, #12]
	movs	r3, #7
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_02000eac
	movs	r0, #15
	.2byte 0xf002
	.2byte 0xff76
	bl 0x0200abd4
.L_02000eac:
	movs	r0, #1
	adds	r5, #1
	.2byte 0xf002
	.2byte 0xfec0
	cmp	r5, #59
	ble.n	.L_02000e86
	.2byte 0xf002
	.2byte 0xff64
	mov	r0, r8
	.2byte 0xf002
	.2byte 0xffcd
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r0, r1, #0
	sub	sp, #8
	.2byte 0xf002
	.2byte 0xff5f
	adds	r2, r0, #0
	ldr	r3, [r2, #8]
	asrs	r3, r3, #20
	cmp	r3, #49
	beq.n	.L_02000ee2
	b.n	.L_02001002
.L_02000ee2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #160
	lsls	r1, r1, #1
	adds	r6, r3, r1
	.2byte 0xf002
	.2byte 0xff45
	movs	r0, #0
	.2byte 0xf002
	.2byte 0xffda
	ldr	r3, [pc, #268]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #128
	ldr	r0, [r3, #0]
	movs	r2, #0
	lsls	r1, r1, #7
	.2byte 0xf002
	.2byte 0xff8c
	movs	r0, #151
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bef4
	movs	r0, #9
	movs	r1, #2
	bl 0x0200bdec
	movs	r3, #113
	movs	r2, #39
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #114
	movs	r1, #39
	movs	r2, #1
	movs	r3, #2
	.2byte 0xf002
	.2byte 0xfef9
	movs	r5, #31
.L_02000f34:
	ldr	r3, [r6, #12]
	ldr	r1, [pc, #212]
	movs	r0, #4
	adds	r3, r3, r1
	str	r3, [r6, #12]
	subs	r5, #1
	.2byte 0xf002
	.2byte 0xfe78
	cmp	r5, #0
	bge.n	.L_02000f34
	movs	r5, #13
.L_02000f4a:
	adds	r0, r5, #0
	.2byte 0xf002
	.2byte 0xff22
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	adds	r0, r5, #0
	lsls	r3, r3, #16
	str	r3, [r2, #8]
	adds	r3, r2, #0
	adds	r3, #102
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	adds	r1, r2, #0
	lsls	r3, r3, #16
	str	r3, [r2, #16]
	adds	r1, #85
	movs	r3, #0
	strb	r3, [r1, #0]
	ldr	r3, [pc, #152]
	movs	r1, #3
	str	r3, [r2, #20]
	str	r3, [r2, #12]
	adds	r5, #1
	.2byte 0xf002
	.2byte 0xff55
	cmp	r5, #15
	ble.n	.L_02000f4a
	.2byte 0xf002
	.2byte 0xfebd
	movs	r5, #12
.L_02000f8c:
	ldr	r1, [pc, #132]
	ldr	r0, [pc, #136]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_02000fc0
	lsls	r2, r3, #1
	adds	r2, r2, r3
.L_02000fa0:
	adds	r3, #1
	lsls	r2, r2, #2
	strh	r3, [r1, #0]
	movs	r3, #128
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r2, #4
	orrs	r3, r5
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_02000fc0:
	strh	r4, [r0, #0]
	movs	r0, #4
	subs	r5, #1
	bl 0x0200bc34
	cmp	r5, #0
	bge.n	.L_02000f8c
	movs	r5, #41
	movs	r0, #105
	movs	r1, #41
	movs	r2, #12
	movs	r3, #13
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	.2byte 0xf002
	.2byte 0xfe9e
	movs	r3, #105
	str	r3, [sp, #0]
	movs	r0, #105
	movs	r1, #105
	movs	r2, #12
	movs	r3, #13
	str	r5, [sp, #4]
	bl 0x0200bd24
	bl 0x0200bcfc
	.2byte 0xf002
	.2byte 0xfec5
	movs	r0, #142
	lsls	r0, r0, #2
	.2byte 0xf002
	.2byte 0xfe51
.L_02001002:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0xffe00000
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r3, r6, #0
	subs	r3, #13
	sub	sp, #8
	cmp	r3, #2
	bhi.n	.L_0200105a
	adds	r0, r6, #0
	.2byte 0xf002
	.2byte 0xfeb2
	adds	r5, r0, #0
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #47
	movs	r1, #45
	movs	r2, #1
	movs	r3, #1
	.2byte 0xf002
	.2byte 0xfe69
	movs	r3, #145
	lsls	r3, r3, #2
	adds	r0, r6, r3
	.2byte 0xf002
	.2byte 0xfe28
	adds	r5, #35
	movs	r3, #0
	strb	r3, [r5, #0]
.L_0200105a:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #8
	movs	r2, #34
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #63
	movs	r2, #7
	movs	r3, #1
	.2byte 0xf002
	.2byte 0xfe52
	add	sp, #8
	pop	{pc}
	push	{r5, r6, lr}
	subs	r1, #10
	sub	sp, #8
	cmp	r1, #1
	bhi.n	.L_020010ca
	movs	r0, #10
	.2byte 0xf002
	.2byte 0xfe84
	adds	r3, r0, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	movs	r1, #33
	ldr	r3, [r0, #8]
	movs	r2, #1
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	movs	r6, #34
	movs	r3, #1
	movs	r0, #8
	str	r6, [sp, #4]
	.2byte 0xf002
	.2byte 0xfe39
	movs	r0, #11
	.2byte 0xf002
	.2byte 0xfe72
	adds	r3, r0, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r1, #33
	ldr	r3, [r0, #8]
	movs	r2, #1
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	movs	r0, #8
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200bd1c
.L_020010ca:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #60
.L_020010d6:
	cmp	r5, #0
	beq.n	.L_020010e8
	movs	r0, #1
	.2byte 0xf002
	.2byte 0xfdaa
	ldr	r3, [r6, #40]
	subs	r5, #1
	cmp	r3, #0
	bne.n	.L_020010d6
.L_020010e8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	.2byte 0xf002
	.2byte 0xfe45
	movs	r0, #0
	.2byte 0xf002
	.2byte 0xfeda
	ldr	r5, [pc, #56]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200bd5c
	movs	r0, #126
	.2byte 0xf002
	.2byte 0xfef6
	movs	r0, #186
	lsls	r0, r0, #2
	movs	r1, #0
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfed0
	movs	r0, #10
	bl 0x0200bd74
	movs	r0, #161
	lsls	r0, r0, #1
	adds	r5, #1
	.2byte 0xf002
	.2byte 0xfdc4
	adds	r0, r5, #0
	movs	r1, #1
	.2byte 0xf002
	.2byte 0xfe18
	.2byte 0xf002
	.2byte 0xfe2a
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1a92
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r0, r1, #0
	sub	sp, #8
	.2byte 0xf002
	.2byte 0xfe29
	movs	r3, #20
	str	r3, [sp, #0]
	adds	r6, r0, #0
	movs	r5, #39
	movs	r0, #17
	movs	r1, #39
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #4]
	.2byte 0xf002
	.2byte 0xfde2
	ldr	r3, [r6, #8]
	movs	r0, #23
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	.2byte 0xf002
	.2byte 0xfdd8
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #166
	lsls	r0, r0, #1
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfd90
	cmp	r0, #0
	beq.n	.L_020011d2
	movs	r0, #12
	movs	r1, #38
	bl 0x02008b24
	movs	r0, #13
	movs	r1, #39
	bl 0x02008b24
	movs	r1, #37
	movs	r0, #15
	bl 0x02008b24
	.2byte 0xf002
	.2byte 0xfa46
	movs	r0, #38
	movs	r1, #2
	movs	r2, #19
	movs	r3, #1
	.2byte 0xf002
	.2byte 0xfa70
	movs	r5, #8
.L_020011aa:
	movs	r0, #33
	movs	r1, #55
	.2byte 0xf002
	.2byte 0xfa83
	subs	r5, #1
	adds	r6, r0, #0
	cmp	r5, #0
	bge.n	.L_020011aa
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #74
	.2byte 0xf002
	.2byte 0xfd70
	cmp	r6, #1
	bne.n	.L_020011d2
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200bca4
.L_020011d2:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	adds	r5, r1, #0
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r0, r5, #0
	adds	r6, r3, r2
	bl 0x0200bd94
	movs	r1, #5
	.2byte 0xf002
	.2byte 0xfe0e
	adds	r0, r5, #0
	bl 0x02009e78
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200bdec
	ldr	r2, [pc, #72]
	ldr	r3, [r6, #12]
	movs	r5, #31
	adds	r3, r3, r2
	str	r3, [r6, #12]
	.2byte 0xf002
	.2byte 0xfda4
	.2byte 0xf002
	.2byte 0xfd66
	.2byte 0xf002
	.2byte 0xfdb4
	movs	r0, #0
	.2byte 0xf002
	.2byte 0xfe49
	movs	r0, #139
	lsls	r0, r0, #2
	bl 0x0200bef4
.L_02001222:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r0, #4
	subs	r5, #1
	.2byte 0xf002
	.2byte 0xfd00
	cmp	r5, #0
	bge.n	.L_02001222
	.2byte 0xf002
	.2byte 0xfda4
	movs	r0, #166
	lsls	r0, r0, #1
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfd2f
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r2, r2, r3
	sub	sp, #8
	mov	r8, r2
	.2byte 0xf002
	.2byte 0xfd8b
	movs	r0, #0
	.2byte 0xf002
	.2byte 0xfe20
	mov	r2, r8
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #176]
	movs	r7, #14
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r2, #12]
	bl 0x0200bcdc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200bd44
.L_0200128e:
	movs	r1, #8
	movs	r2, #7
	movs	r3, #6
	movs	r5, #77
	movs	r6, #8
	movs	r0, #70
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200bd24
	movs	r0, #1
	.2byte 0xf002
	.2byte 0xfcc6
	movs	r0, #50
	movs	r1, #8
	movs	r2, #7
	movs	r3, #6
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	subs	r7, #1
	bl 0x0200bd24
	movs	r0, #1
	.2byte 0xf002
	.2byte 0xfcba
	cmp	r7, #0
	bge.n	.L_0200128e
	movs	r0, #10
	.2byte 0xf002
	.2byte 0xfcb5
	movs	r7, #127
.L_020012cc:
	mov	r2, r8
	ldr	r3, [r2, #12]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r2, #12]
	movs	r0, #1
	subs	r7, #1
	.2byte 0xf002
	.2byte 0xfca9
	cmp	r7, #0
	bge.n	.L_020012cc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	.2byte 0xf002
	.2byte 0xfd26
	.2byte 0xf002
	.2byte 0xfd44
	movs	r3, #13
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #9
	movs	r2, #7
	movs	r3, #6
	movs	r0, #38
	.2byte 0xf002
	.2byte 0xfd06
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #73
	.2byte 0xf002
	.2byte 0xfcc5
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb500
	sub	sp, #8
	movs	r3, #77
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #70
	movs	r1, #8
	movs	r2, #7
	movs	r3, #6
	.2byte 0xf002
	.2byte 0xfcf2
	movs	r3, #13
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #17
	movs	r2, #7
	movs	r3, #6
	movs	r0, #38
	.2byte 0xf002
	.2byte 0xfce4
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #73
	.2byte 0xf002
	.2byte 0xfca7
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #168
	lsls	r0, r0, #2
	.2byte 0xf002
	.2byte 0xfcb0
	movs	r3, #204
	adds	r5, r0, #0
	lsls	r3, r3, #7
	adds	r3, #102
	adds	r2, r5, #0
	adds	r2, #85
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #2
	bl 0x0200be0c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bcc4
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bd34
	adds	r0, r5, #0
	movs	r1, #1
	.2byte 0xf002
	.2byte 0xfcc7
	ldr	r1, [pc, #8]
	adds	r0, r5, #0
	.2byte 0xf002
	.2byte 0xfc8b
	pop	{r5, pc}
	.2byte 0xbfd8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r0, #0
	movs	r2, #102
	adds	r2, r2, r6
.L_020013cc:
	adds	r5, r6, #0
	mov	sl, r2
	ldrh	r2, [r2, #0]
	adds	r5, #100
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #16
	ldr	r7, [r6, #104]
	asrs	r2, r2, #17
	adds	r3, r3, r2
	strh	r3, [r5, #0]
	ldr	r3, [r7, #8]
	movs	r2, #128
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	movs	r2, #8
	str	r3, [r6, #16]
	adds	r2, r2, r6
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	ldr	r0, [r6, #76]
	mov	r8, r2
	.2byte 0xf002
	.2byte 0xfc31
	adds	r2, r6, #0
	adds	r2, #98
	ldrb	r3, [r2, #0]
	movs	r0, #0
	adds	r3, #255
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0200146c
	ldr	r3, [r6, #76]
	movs	r2, #128
	lsls	r2, r2, #10
	movs	r0, #1
	cmp	r3, r2
	beq.n	.L_0200146c
	adds	r0, r6, #0
	bl 0x02009364
	ldr	r2, [pc, #80]
	ldr	r3, [r6, #76]
	mov	r9, r2
	add	r3, r9
	str	r3, [r6, #76]
	mov	r3, sl
	ldrh	r2, [r3, #0]
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #16
	asrs	r2, r2, #17
	adds	r3, r3, r2
	strh	r3, [r5, #0]
	ldr	r3, [r7, #8]
	mov	r2, r8
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	ldr	r0, [r6, #76]
	str	r3, [r6, #16]
	mov	r2, r8
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	bl 0x0200bc64
	adds	r0, r6, #0
	bl 0x02009364
	ldr	r3, [r6, #76]
	movs	r0, #1
	add	r3, r9
	str	r3, [r6, #76]
.L_0200146c:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r0, #102
	adds	r0, r0, r7
	mov	r8, r0
	adds	r5, r7, #0
	adds	r5, #100
	mov	r1, r8
	ldrh	r3, [r1, #0]
	ldrh	r0, [r5, #0]
	adds	r0, r0, r3
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	.2byte 0xf002
	.2byte 0xfbde
	ldr	r1, [r7, #76]
	ldr	r6, [pc, #116]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x60b8
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	.2byte 0xf002
	.2byte 0xfbd1
	ldr	r1, [r7, #76]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68bb
	ldr	r2, [r7, #68]
	asrs	r0, r0, #1
	adds	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r3, [r7, #72]
	subs	r5, #1
	adds	r0, r0, r3
	str	r0, [r7, #16]
	ldrb	r3, [r5, #0]
	cmp	r3, #141
	beq.n	.L_020014f4
	ldr	r3, [pc, #72]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020014f4
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r0, [r3, #0]
	lsls	r0, r0, #10
	.2byte 0xf002
	.2byte 0xfbb6
	ldrb	r3, [r5, #0]
	muls	r3, r0
	str	r3, [r7, #76]
	ldrb	r3, [r5, #0]
	adds	r3, #10
	strb	r3, [r5, #0]
.L_020014f4:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_02001512
	ldrb	r3, [r5, #0]
	cmp	r3, #141
	bne.n	.L_02001510
	adds	r3, r2, #0
	subs	r3, #128
	mov	r1, r8
	strh	r3, [r1, #0]
.L_02001510:
	movs	r0, #1
.L_02001512:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r5, r0, #0
	ldr	r6, [r5, #104]
	ldr	r3, [r5, #8]
	ldr	r0, [r6, #8]
	movs	r1, #10
	subs	r0, r0, r3
	movs	r3, #10
	mov	r8, r3
	.2byte 0xf002
	.2byte 0xfb79
	str	r0, [r5, #68]
	ldr	r3, [r5, #12]
	ldr	r0, [r6, #12]
	movs	r1, #10
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	.2byte 0xf002
	.2byte 0xfb6f
	str	r0, [r5, #76]
	ldr	r3, [r5, #16]
	ldr	r0, [r6, #16]
	movs	r1, #10
	subs	r0, r0, r3
	.2byte 0xf002
	.2byte 0xfb68
	mov	r3, r8
	str	r0, [r5, #72]
	adds	r5, #98
	strb	r3, [r5, #0]
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x6c426883
	.4byte 0x6083189b
	.4byte 0x68c36cc2
	.4byte 0x60c3189b
	.4byte 0x69036c82
	.4byte 0x6103189b
	.4byte 0x78033062
	.4byte 0x700333ff
	.4byte 0x0e1b061b
	.4byte 0x43184258
	.2byte 0x0fc0
	.2byte 0x4770
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #120]
	sub	sp, #68
	ldr	r7, [r3, #0]
	movs	r3, #7
	ands	r7, r3
	mov	sl, r0
	cmp	r7, #0
	bne.n	.L_02001612
	add	r2, sp, #28
	str	r3, [r2, #4]
	movs	r3, #209
	lsls	r3, r3, #1
	adds	r3, #255
	strh	r3, [r2, #24]
	movs	r3, #1
	str	r3, [r2, #0]
	mov	r8, r2
	bl 0x0200bc4c
	movs	r6, #31
	mov	r2, sl
	ldr	r3, [r2, #8]
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #16
	add	r5, sp, #16
	adds	r3, r3, r0
	str	r3, [r5, #0]
	.2byte 0xf002
	.2byte 0xfb38
	mov	r3, sl
	ldr	r1, [r3, #12]
	ands	r0, r6
	lsls	r0, r0, #16
	movs	r2, #128
	adds	r1, r1, r0
	lsls	r2, r2, #12
	adds	r1, r1, r2
	str	r1, [r5, #4]
	ldr	r0, [r5, #0]
	ldr	r2, [r3, #16]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r2, [r5, #8]
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [sp, #0]
	movs	r3, #152
	lsls	r3, r3, #13
	str	r3, [sp, #8]
	mov	r3, r8
	str	r3, [sp, #12]
	movs	r3, #0
	str	r7, [sp, #4]
	bl 0x0200815c
.L_02001612:
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	mov	fp, r1
	mov	r9, r0
	bl 0x0200bd94
	adds	r7, r0, #0
	mov	r0, fp
	bl 0x0200bd94
	mov	sl, r0
	movs	r0, #78
	bl 0x0200bef4
	movs	r0, #30
	bl 0x0200bd74
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	bl 0x0200be44
	movs	r2, #10
	ldrsh	r0, [r7, r2]
	movs	r3, #14
	ldrsh	r1, [r7, r3]
	movs	r3, #18
	ldrsh	r2, [r7, r3]
	lsls	r1, r1, #16
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	bl 0x0200be4c
	movs	r0, #141
	bl 0x0200bef4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200bd44
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200be7c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200be74
	movs	r0, #60
	bl 0x0200be84
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	str	r3, [r7, #52]
	str	r3, [r7, #48]
	ldr	r1, [pc, #60]
	mov	r0, r9
	bl 0x0200bda4
	movs	r0, #194
	bl 0x0200bef4
	movs	r0, #45
	bl 0x0200bd74
	movs	r1, #128
	mov	r0, r9
	lsls	r1, r1, #1
	bl 0x0200be04
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #16]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r0, #0
	mov	r8, r0
	b.n	.L_020016f8
	.2byte 0x0000
	.4byte 0x00001008
	.4byte 0x00003f10
	.2byte 0xc034
	.2byte 0x0200
.L_020016f8:
	movs	r0, #168
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #2
	bl 0x0200bcd4
	adds	r5, r0, #0
	movs	r0, #246
	bl 0x0200bef4
.L_0200170e:
	.2byte 0xf002
	.2byte 0xfa9d
	movs	r3, #31
	ldr	r2, [r5, #8]
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r5, #8]
	bl 0x0200bc4c
	movs	r3, #15
	ldr	r2, [r5, #12]
	ands	r3, r0
	subs	r3, #8
	lsls	r3, r3, #16
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #48]
	movs	r3, #204
	lsls	r3, r3, #6
	str	r2, [r5, #12]
	adds	r3, #51
	adds	r2, r5, #0
	adds	r2, #85
	str	r3, [r5, #52]
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200be0c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200bcc4
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bd34
	adds	r0, r5, #0
	movs	r1, #1
.L_0200176c:
	bl 0x0200bd3c
	ldr	r1, [pc, #68]
	adds	r0, r5, #0
	bl 0x0200bccc
	movs	r6, #128
	ldr	r3, [pc, #48]
	ldr	r5, [pc, #48]
	lsls	r6, r6, #19
	adds	r6, #82
	strh	r3, [r6, #0]
	movs	r0, #2
	bl 0x0200bc34
	movs	r0, #2
	strh	r5, [r6, #0]
.L_0200178e:
	bl 0x0200bc34
	ldr	r3, [pc, #32]
	movs	r0, #2
	strh	r3, [r6, #0]
	bl 0x0200bc34
	strh	r5, [r6, #0]
	movs	r0, #2
	bl 0x0200bc34
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	b.n	.L_020017bc
	.4byte 0x00001004
	.4byte 0x0000100a
	.4byte 0x00001010
	.2byte 0xbfe8
	.2byte 0x0200
.L_020017bc:
	cmp	r3, #16
	bne.n	.L_020016f8
	ldr	r3, [pc, #56]
	movs	r0, #30
	strh	r3, [r6, #0]
	bl 0x0200bc34
	movs	r0, #0
	mov	r8, r0
.L_020017ce:
	mov	r0, sl
	ldr	r3, [r0, #16]
	mov	r2, sl
	movs	r0, #168
	ldr	r1, [r2, #8]
	lsls	r0, r0, #2
	ldr	r2, [r2, #12]
	bl 0x0200bcd4
	adds	r5, r0, #0
	movs	r0, #195
	bl 0x0200bef4
	bl 0x0200bc4c
	ldr	r3, [pc, #16]
	movs	r2, #128
.L_020017f0:
	ands	r0, r3
	lsls	r2, r2, #8
	adds	r6, r5, #0
	adds	r0, r0, r2
	adds	r6, #100
	b.n	.L_02001804
	.4byte 0x00001008
	.2byte 0x7fff
	.2byte 0x0000
.L_02001804:
	strh	r0, [r6, #0]
	bl 0x0200bc4c
	mov	r3, r8
	movs	r2, #1
	ands	r2, r3
	movs	r3, #3
	ands	r3, r0
	lsls	r2, r2, #1
	adds	r3, #9
	subs	r2, #1
	lsls	r2, r3
	ldr	r7, [pc, #56]
	adds	r3, r5, #0
	adds	r3, #102
	strh	r2, [r3, #0]
	subs	r3, #17
	strb	r7, [r3, #0]
	movs	r3, #244
	lsls	r3, r3, #15
	str	r3, [r5, #76]
	mov	r2, r8
	movs	r3, #16
	subs	r3, r3, r2
	lsls	r3, r3, #3
	adds	r2, r5, #0
	adds	r3, #15
	adds	r2, #98
	mov	r0, sl
	str	r0, [r5, #104]
	movs	r1, #2
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl 0x0200be0c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #7
	b.n	.L_0200185c
	.2byte 0x0000
	.2byte 0x0000
.L_0200185c:
	bl 0x0200bcc4
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bd34
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bd3c
	adds	r2, r5, #0
	movs	r3, #0
	ldrsh	r1, [r6, r3]
	ldr	r0, [r5, #76]
	adds	r2, #8
	bl 0x0200bc64
	adds	r0, r5, #0
	ldr	r1, [pc, #208]
	bl 0x0200bccc
	mov	r0, r8
.L_02001888:
	cmp	r0, #3
	bne.n	.L_020018b0
	movs	r1, #128
	mov	r0, fp
	lsls	r1, r1, #1
	bl 0x0200be04
	mov	r3, sl
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	mov	r2, sl
	str	r3, [r2, #52]
	str	r3, [r2, #48]
	mov	r0, fp
	ldr	r1, [pc, #172]
	bl 0x0200bda4
.L_020018b0:
	movs	r0, #8
	bl 0x0200bc34
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	cmp	r0, #16
	beq.n	.L_020018c2
	b.n	.L_020017ce
.L_020018c2:
	movs	r0, #220
	bl 0x0200bef4
	movs	r0, #16
	bl 0x0200bc34
	movs	r2, #2
	mov	r8, r2
.L_020018d2:
	mov	r3, sl
	ldr	r1, [r3, #8]
	ldr	r3, [r3, #12]
	mov	r0, r8
	lsls	r2, r0, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #124]
	mov	r0, sl
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	movs	r0, #168
	lsls	r0, r0, #2
	bl 0x0200bcd4
	adds	r5, r0, #0
	bl 0x0200bc4c
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	adds	r2, r5, #0
	movs	r3, #128
	adds	r2, #102
	lsls	r3, r3, #4
	strh	r3, [r2, #0]
	adds	r3, r5, #0
	mov	r2, r8
	adds	r3, #98
	strb	r2, [r3, #0]
	movs	r2, #1
	adds	r3, #1
	strb	r2, [r3, #0]
	ldr	r1, [pc, #60]
	ldr	r3, [r5, #8]
	movs	r6, #0
	str	r3, [r5, #68]
	ldr	r3, [r5, #16]
	str	r6, [r5, #76]
	str	r3, [r5, #72]
	mov	r3, sl
	str	r3, [r5, #104]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	subs	r3, #50
	strb	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200be0c
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200bcc4
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bd34
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bd3c
	b.n	.L_02001960
	.4byte 0x00000000
	.4byte 0x0200c004
	.4byte 0x0200c058
	.2byte 0x0000
	.2byte 0xfff8
.L_02001960:
	.2byte 0x1c28
	ldr	r1, [pc, #168]
	bl 0x0200bccc
	movs	r0, #2
	add	r8, r0
	mov	r2, r8
	cmp	r2, #32
	bne.n	.L_020018d2
	movs	r0, #220
	bl 0x0200bef4
	movs	r0, #50
	bl 0x0200bc34
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200be74
	movs	r0, #8
	bl 0x0200be84
	movs	r0, #16
	bl 0x0200bc34
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200bd44
	mov	r0, r9
	movs	r1, #0
	bl 0x0200be04
	mov	r0, fp
	movs	r1, #0
	bl 0x0200be04
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200bef4
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200be74
	movs	r0, #80
	bl 0x0200be84
	mov	r0, r9
	ldr	r1, [pc, #64]
	bl 0x0200bda4
	ldr	r1, [pc, #64]
	mov	r0, fp
	bl 0x0200bda4
	ldr	r3, [pc, #60]
	mov	r0, sl
	str	r3, [r0, #108]
	movs	r0, #120
	bl 0x0200bc34
	mov	r2, sl
	movs	r0, #195
	str	r6, [r2, #108]
	lsls	r0, r0, #1
	bl 0x0200bef4
	bl 0x0200bebc
	movs	r0, #1
	bl 0x0200bc34
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200c014
	.4byte 0x0200c07c
	.4byte 0x0200c0ac
	.2byte 0x9599
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	ldr	r6, [pc, #556]
	movs	r1, #1
	adds	r0, r6, #0
	bl 0x0200bd5c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #174
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_02001a46
	bl 0x0200bd84
	b.n	.L_02001c56
.L_02001a46:
	adds	r0, r6, #0
	bl 0x0200be14
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	ldr	r5, [pc, #512]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200bde4
	movs	r0, #1
	bl 0x0200bd74
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #184
	bl 0x0200bdbc
	movs	r1, #137
	lsls	r1, r1, #1
	movs	r2, #184
	movs	r0, #7
	bl 0x0200bdbc
	ldr	r0, [r5, #0]
	bl 0x0200bdd4
	movs	r0, #7
	bl 0x0200bdd4
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200be24
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200be24
	movs	r0, #5
	bl 0x0200bd74
	movs	r0, #7
	movs	r1, #0
	bl 0x0200be1c
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200bdec
	movs	r0, #30
	bl 0x0200bd74
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #184
	bl 0x0200bdc4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200be24
	movs	r0, #20
	bl 0x0200bd74
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200be24
	movs	r0, #20
	bl 0x0200bd74
	movs	r1, #3
	movs	r0, #7
	bl 0x0200bdec
	movs	r0, #30
	bl 0x0200bd74
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200be24
	movs	r0, #20
	bl 0x0200bd74
	movs	r1, #128
	movs	r2, #128
	movs	r0, #7
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200bd9c
	movs	r1, #132
	movs	r0, #7
	lsls	r1, r1, #1
	movs	r2, #168
	bl 0x0200bdc4
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200be24
	movs	r0, #20
	bl 0x0200bd74
	adds	r0, r6, #2
	movs	r1, #1
	bl 0x0200bd5c
	movs	r0, #14
	movs	r1, #7
	bl 0x02009620
	movs	r1, #138
	movs	r0, #7
	bl 0x0200bd8c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #174
	bl 0x0200bca4
	movs	r0, #107
	bl 0x0200bef4
	movs	r0, #30
	bl 0x0200bd74
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200be24
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x0200be24
	movs	r0, #10
	bl 0x0200bd74
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200be34
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200be34
	bl 0x0200924c
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200bef4
	movs	r0, #30
	bl 0x0200bd74
	ldr	r0, [pc, #164]
	bl 0x0200be14
	movs	r1, #6
	adds	r1, #255
	movs	r2, #20
	movs	r0, #7
	bl 0x0200be34
	movs	r2, #0
	movs	r1, #7
	ldr	r0, [r5, #0]
	bl 0x0200bdfc
	movs	r0, #10
	bl 0x0200bd74
	movs	r0, #7
	movs	r1, #0
	bl 0x0200be1c
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200be34
	movs	r1, #0
	movs	r0, #7
	bl 0x0200be1c
	movs	r0, #10
	bl 0x0200bd74
	movs	r0, #7
	movs	r1, #3
	bl 0x0200bdec
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200bdec
	movs	r0, #20
	bl 0x0200bd74
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #72]
	adds	r2, #153
	bl 0x0200bd9c
	movs	r0, #7
	movs	r1, #2
	bl 0x0200bdec
	ldr	r0, [r5, #0]
	bl 0x0200bd94
	cmp	r0, #0
	beq.n	.L_02001c42
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200bdac
.L_02001c42:
	movs	r0, #7
	bl 0x0200bdd4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bddc
	bl 0x0200bd84
.L_02001c56:
	pop	{r5, r6, pc}
	.4byte 0x0000229d
	.4byte 0x02000240
	.4byte 0x000022a0
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #174
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_02001c90
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	ldr	r0, [pc, #76]
	movs	r1, #1
	bl 0x0200bd5c
	bl 0x0200bd84
	b.n	.L_02001cce
.L_02001c90:
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #152
	bl 0x0200bdc4
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #184
	bl 0x0200bdc4
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
.L_02001cb2:
	movs	r2, #184
	bl 0x0200bdc4
	movs	r1, #192
.L_02001cba:
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200be24
	movs	r0, #10
	bl 0x0200bd74
	bl 0x02009a1c
.L_02001cce:
	pop	{r5, pc}
	.4byte 0x0000229d
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #174
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_02001d00
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	ldr	r0, [pc, #68]
	movs	r1, #1
	bl 0x0200bd5c
	bl 0x0200bd84
	b.n	.L_02001d36
.L_02001d00:
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #140
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #184
	bl 0x0200bdc4
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #184
	bl 0x0200bdc4
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200be24
	movs	r0, #10
	bl 0x0200bd74
	bl 0x02009a1c
.L_02001d36:
	pop	{r5, pc}
	.4byte 0x0000229d
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #174
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_02001d68
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	ldr	r0, [pc, #68]
	movs	r1, #1
	bl 0x0200bd5c
	bl 0x0200bd84
	b.n	.L_02001d9c
.L_02001d68:
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #184
	bl 0x0200bdc4
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #184
	bl 0x0200bdc4
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200be24
	movs	r0, #10
	bl 0x0200bd74
	bl 0x02009a1c
.L_02001d9c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0000229d
	.2byte 0x0240
	.2byte 0x0200
	.global Func_02001da8
	.thumb_func
Func_02001da8:
	push	{lr}
	ldr	r3, [pc, #116]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_02001dc0
	ldr	r0, [pc, #104]
	b.n	.L_02001e1c
.L_02001dc0:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_02001dca
	ldr	r0, [pc, #104]
	b.n	.L_02001e1c
.L_02001dca:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_02001dd4
	ldr	r0, [pc, #100]
	b.n	.L_02001e1c
.L_02001dd4:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_02001dde
	ldr	r0, [pc, #100]
	b.n	.L_02001e1c
.L_02001dde:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_02001de8
	ldr	r0, [pc, #96]
	b.n	.L_02001e1c
.L_02001de8:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_02001df2
	ldr	r0, [pc, #96]
	b.n	.L_02001e1c
.L_02001df2:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_02001dfc
	ldr	r0, [pc, #92]
	b.n	.L_02001e1c
.L_02001dfc:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02001e06
	ldr	r0, [pc, #92]
	b.n	.L_02001e1c
.L_02001e06:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02001e10
	ldr	r0, [pc, #88]
	b.n	.L_02001e1c
.L_02001e10:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02001e1a
	ldr	r0, [pc, #88]
	b.n	.L_02001e1c
.L_02001e1a:
	ldr	r0, [pc, #88]
.L_02001e1c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a0
	.4byte 0x0200cc7c
	.4byte 0x000000a1
	.4byte 0x0200cd88
	.4byte 0x000000a2
	.4byte 0x0200ce48
	.4byte 0x000000a3
	.4byte 0x0200ced8
	.4byte 0x000000a4
	.4byte 0x0200cfb0
	.4byte 0x000000a5
	.4byte 0x0200d07c
	.4byte 0x000000a6
	.4byte 0x0200d1b4
	.4byte 0x000000a7
	.4byte 0x0200d1fc
	.4byte 0x000000a8
	.4byte 0x0200d268
	.4byte 0x000000a9
	.4byte 0x0200d2f8
	.2byte 0xcc70
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200bd94
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001e9c
	movs	r1, #126
	adds	r1, #255
	ldr	r0, [r5, #80]
	bl 0x0200bcbc
	movs	r3, #0
	strb	r3, [r0, #5]
	strb	r3, [r0, #6]
	movs	r1, #2
	adds	r0, r5, #0
	bl 0x0200bcc4
.L_02001e9c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	ldr	r7, [r3, #0]
	movs	r3, #15
	ands	r7, r3
	cmp	r7, #0
	bne.n	.L_02001efa
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #14
	adds	r0, #255
	bl 0x0200bcd4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001efa
	ldr	r1, [pc, #60]
	ldr	r6, [r5, #80]
	bl 0x0200bccc
	adds	r3, r5, #0
	adds	r3, #85
	adds	r2, r5, #0
	strb	r7, [r3, #0]
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_02001efa
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200bcb4
	movs	r3, #128
	ldrb	r2, [r6, #9]
	lsls	r3, r3, #7
	strh	r3, [r6, #18]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r7, [r6, #26]
	strb	r3, [r6, #9]
.L_02001efa:
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.2byte 0xc0dc
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #180]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bd94
	movs	r3, #85
	adds	r6, r0, #0
	adds	r3, r3, r6
	ldrb	r2, [r3, #0]
	mov	r8, r3
	mov	sl, r2
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	bl 0x0200be8c
	adds	r0, r6, #0
	movs	r1, #9
	bl 0x0200bd4c
	movs	r3, #0
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r6, #12]
	movs	r7, #192
	lsls	r7, r7, #10
	movs	r5, #31
.L_02001f4e:
	ldr	r3, [r6, #12]
	movs	r2, #128
	subs	r3, r3, r7
	str	r3, [r6, #12]
	ldrh	r3, [r6, #6]
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	ldr	r3, [pc, #104]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r7, r7, r3
	subs	r5, #1
	bl 0x0200bd34
	movs	r0, #1
	bl 0x0200bc34
	cmp	r5, #0
	bge.n	.L_02001f4e
	adds	r0, r6, #0
	.2byte 0xf000
	.2byte 0xfe2c
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200bd4c
	ldr	r5, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200bd9c
	mov	r2, r8
	mov	r3, sl
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r6, #40]
	movs	r0, #215
	bl 0x0200bef4
	ldr	r0, [r5, #0]
	movs	r1, #168
	movs	r2, #200
	bl 0x0200bdb4
	bl 0x0200bd84
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xf334
	.2byte 0xffff
	push	{lr}
	bl 0x0200bbcc
	cmp	r0, #0
	beq.n	.L_02001fde
	ldr	r3, [pc, #64]
	cmp	r0, r3
	beq.n	.L_02001ff6
	b.n	.L_0200200e
.L_02001fde:
	movs	r1, #1
	movs	r0, #0
	bl 0x0200bbb8
	movs	r0, #1
	movs	r1, #1
	bl 0x0200bbb8
	movs	r0, #170
	bl 0x0200becc
	b.n	.L_0200200e
.L_02001ff6:
	movs	r1, #0
	movs	r0, #0
	bl 0x0200bbb8
	movs	r0, #1
	movs	r1, #0
	bl 0x0200bbb8
	movs	r0, #1
	negs	r0, r0
	bl 0x0200becc
.L_0200200e:
	ldr	r0, [pc, #12]
	bl 0x0200bbd8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00034bbf
	.2byte 0x4bc0
	.2byte 0x0003
	.global Func_02002020
	.thumb_func
Func_02002020:
	push	{r5, r6, lr}
	ldr	r2, [pc, #736]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	subs	r0, #54
	movs	r2, #129
	adds	r3, r3, r0
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	ldr	r3, [pc, #700]
	sub	sp, #8
	cmp	r1, r3
	bne.n	.L_020020a2
	adds	r0, #132
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_0200206a
	movs	r0, #8
	bl 0x02009e78
	movs	r0, #8
	movs	r1, #3
	bl 0x0200bdec
	b.n	.L_02002076
.L_0200206a:
	movs	r0, #8
	bl 0x0200bd94
	movs	r1, #6
	bl 0x0200be0c
.L_02002076:
	movs	r0, #153
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_02002094
	movs	r0, #9
	bl 0x02009e78
	movs	r0, #9
	movs	r1, #3
	bl 0x0200bdec
	b.n	.L_020022fe
.L_02002094:
	movs	r0, #9
	bl 0x0200bd94
	movs	r1, #6
	bl 0x0200be0c
	b.n	.L_020022fe
.L_020020a2:
	ldr	r3, [pc, #616]
	cmp	r1, r3
	bne.n	.L_02002140
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_020020ea
	movs	r3, #110
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #118
	movs	r1, #15
	movs	r2, #5
	movs	r3, #32
	bl 0x0200bd24
	movs	r3, #46
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #16
	movs	r0, #59
	movs	r2, #5
	movs	r3, #32
	bl 0x0200bd1c
	movs	r0, #8
	bl 0x0200bd94
	movs	r1, #6
	bl 0x0200be0c
	b.n	.L_020020f8
.L_020020ea:
	movs	r0, #8
	bl 0x02009e78
	movs	r0, #8
	movs	r1, #3
	bl 0x0200bdec
.L_020020f8:
	movs	r0, #9
	bl 0x0200bd94
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r0, #144
	adds	r3, #85
	movs	r6, #0
	lsls	r0, r0, #4
	strb	r6, [r3, #0]
	adds	r0, #92
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02002118
	b.n	.L_020022fe
.L_02002118:
	movs	r1, #158
	movs	r2, #152
	movs	r0, #9
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200bddc
	movs	r3, #39
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #41
	movs	r1, #9
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bd1c
	str	r6, [r5, #12]
	str	r6, [r5, #20]
	b.n	.L_020022fe
.L_02002140:
	ldr	r3, [pc, #460]
	cmp	r1, r3
	bne.n	.L_02002152
	cmp	r6, #7
	beq.n	.L_0200214c
	b.n	.L_020022fe
.L_0200214c:
	.2byte 0xf000
	.2byte 0xff56
	b.n	.L_020022fe
.L_02002152:
	ldr	r3, [pc, #448]
	cmp	r1, r3
	bne.n	.L_020021e8
	movs	r0, #10
	bl 0x02009e78
	movs	r1, #3
	movs	r0, #10
	bl 0x0200bdec
	movs	r0, #11
	bl 0x02009e78
	movs	r0, #11
	movs	r1, #3
	bl 0x0200bdec
	subs	r3, r6, #6
	cmp	r3, #1
	bhi.n	.L_020021b4
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_020021a8
	movs	r0, #9
	bl 0x0200bd94
	movs	r1, #5
	bl 0x0200be0c
	movs	r0, #9
	bl 0x02009e78
	movs	r0, #9
	movs	r1, #3
	bl 0x0200bdec
	bl 0x02008b5c
	b.n	.L_020021b4
.L_020021a8:
	movs	r0, #9
	bl 0x0200bd94
	movs	r1, #6
	bl 0x0200be0c
.L_020021b4:
	movs	r0, #13
	bl 0x0200bd94
	movs	r5, #1
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #14
	bl 0x0200bd94
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #64
	bl 0x0200bd94
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_020021d8
	b.n	.L_020022fe
.L_020021d8:
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	ldr	r3, [pc, #308]
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	b.n	.L_020022fe
.L_020021e8:
	ldr	r3, [pc, #304]
	cmp	r1, r3
	bne.n	.L_02002208
	subs	r3, r6, #5
	cmp	r3, #1
	bls.n	.L_020021f6
	b.n	.L_020022fe
.L_020021f6:
	ldr	r0, [pc, #296]
	ldr	r1, [pc, #296]
	ldr	r2, [pc, #300]
	bl 0x0200bab0
	movs	r0, #170
	bl 0x0200becc
	b.n	.L_020022fe
.L_02002208:
	ldr	r3, [pc, #288]
	cmp	r1, r3
	bne.n	.L_02002284
	cmp	r6, #5
	beq.n	.L_02002216
	cmp	r6, #7
	bne.n	.L_0200222e
.L_02002216:
	ldr	r1, [pc, #280]
	ldr	r2, [pc, #268]
	ldr	r0, [pc, #280]
	bl 0x0200bab0
	movs	r0, #170
	bl 0x0200becc
	movs	r0, #0
	movs	r1, #10
	bl 0x0200907c
.L_0200222e:
	movs	r0, #142
	lsls	r0, r0, #2
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02002248
	movs	r0, #9
	bl 0x02009e78
	movs	r0, #9
	movs	r1, #3
	bl 0x0200bdec
.L_02002248:
	movs	r0, #12
	bl 0x0200bd94
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200bd34
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #204]
	movs	r0, #10
	str	r3, [r5, #108]
	adds	r0, #255
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_020022fe
	cmp	r6, #13
	bne.n	.L_020022fe
	bl 0x02009f04
	b.n	.L_020022fe
.L_02002284:
	ldr	r3, [pc, #180]
	cmp	r1, r3
	bne.n	.L_020022fe
	movs	r0, #166
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_020022b4
	movs	r0, #9
	bl 0x0200bd94
	movs	r1, #5
	bl 0x0200be0c
	movs	r0, #9
	bl 0x02009e78
	movs	r0, #9
	movs	r1, #3
	bl 0x0200bdec
	b.n	.L_020022c0
.L_020022b4:
	movs	r0, #9
	bl 0x0200bd94
	movs	r1, #6
	bl 0x0200be0c
.L_020022c0:
	movs	r0, #10
	bl 0x0200bd94
	movs	r5, #1
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #11
	bl 0x0200bd94
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #14
	bl 0x0200bd94
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #16
	bl 0x0200bd94
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #17
	bl 0x0200bd94
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #18
	bl 0x0200bd94
	adds	r0, #98
	strb	r5, [r0, #0]
.L_020022fe:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x000000a0
	.4byte 0x000000a1
	.4byte 0x000000a2
	.4byte 0x000000a3
	.4byte 0xffe00000
	.4byte 0x000000a4
	.4byte 0x0200c0f4
	.4byte 0x0200c102
	.4byte 0x02009fcd
	.4byte 0x000000a5
	.4byte 0x0200c114
	.4byte 0x0200c106
	.4byte 0x02009ea1
	.2byte 0x00a8
	.2byte 0x0000
	.global Func_02002340
	.thumb_func
Func_02002340:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #764]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	movs	r2, #241
	lsls	r2, r2, #1
.L_02002356:
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	mov	r8, r3
	ldrb	r2, [r1, #23]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #23]
	sub	sp, #12
	bl 0x0200b278
	ldr	r3, [pc, #716]
	cmp	r5, r3
	beq.n	.L_02002380
	b.n	.L_0200252a
.L_02002380:
	subs	r3, r7, #3
	cmp	r3, #1
	bls.n	.L_0200238e
	cmp	r7, #6
	beq.n	.L_0200238e
	cmp	r7, #8
	bne.n	.L_0200239c
.L_0200238e:
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
	b.n	.L_020024c8
.L_0200239c:
	cmp	r7, #5
	beq.n	.L_020023a4
	cmp	r7, #7
	bne.n	.L_02002462
.L_020023a4:
	movs	r0, #12
	bl 0x0200bd94
	adds	r6, r0, #0
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r1, #0
	str	r5, [r6, #20]
	str	r5, [r6, #12]
	bl 0x0200bcc4
	movs	r0, #13
	bl 0x0200bd94
	adds	r6, r0, #0
	adds	r3, r6, #0
	adds	r3, #85
	movs	r0, #140
	strb	r5, [r3, #0]
	lsls	r0, r0, #2
	str	r5, [r6, #20]
	str	r5, [r6, #12]
	bl 0x0200bc9c
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_02002422
	movs	r3, #104
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #104
	movs	r1, #80
	movs	r2, #16
	movs	r3, #10
	bl 0x0200bd24
	movs	r3, #39
	movs	r2, #105
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #110
	movs	r1, #116
	movs	r2, #18
	movs	r3, #12
	bl 0x0200bd24
	movs	r3, #32
	movs	r0, #64
	movs	r1, #32
.L_0200240c:
	movs	r2, #64
	str	r5, [sp, #0]
	str	r3, [sp, #4]
	bl 0x0200bd1c
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2d4
	b.n	.L_02002440
.L_02002422:
	movs	r1, #4
	movs	r0, #33
	movs	r2, #0
	bl 0x0200b2d4
	movs	r0, #12
	bl 0x0200bd94
	adds	r6, r0, #0
	ldr	r0, [r6, #8]
	movs	r1, #0
	asrs	r0, r0, #20
	subs	r0, #42
	bl 0x0200b49c
.L_02002440:
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_020024c8
	movs	r3, #43
	movs	r2, #44
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #41
	movs	r1, #42
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bd1c
	b.n	.L_020024c8
.L_02002462:
	adds	r3, r7, #0
	subs	r3, #9
	cmp	r3, #1
	bhi.n	.L_020024c8
	movs	r0, #153
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bc9c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020024be
	movs	r3, #77
	str	r3, [sp, #0]
	movs	r5, #32
	movs	r0, #77
	movs	r1, #80
	movs	r2, #23
	movs	r3, #20
	str	r5, [sp, #4]
	bl 0x0200bd24
	movs	r3, #12
	movs	r2, #98
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #112
	movs	r1, #96
	movs	r2, #16
	movs	r3, #20
	bl 0x0200bd24
	movs	r0, #64
	movs	r1, #32
	movs	r2, #64
	movs	r3, #32
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bd1c
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2d4
	b.n	.L_020024c8
.L_020024be:
	movs	r0, #33
	movs	r1, #4
	movs	r2, #0
	bl 0x0200b2d4
.L_020024c8:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #90
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_0200250a
	movs	r1, #184
	movs	r2, #200
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200bddc
	movs	r3, #9
	movs	r5, #12
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bd1c
	movs	r3, #13
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bd1c
.L_0200250a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #91
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_0200251a
	b.n	.L_02002bc4
.L_0200251a:
	movs	r1, #206
	movs	r2, #216
	movs	r0, #11
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200bddc
	b.n	.L_02002bc4
.L_0200252a:
	ldr	r3, [pc, #288]
	cmp	r5, r3
	bne.n	.L_02002558
	subs	r3, r7, #1
	cmp	r3, #1
	bhi.n	.L_02002542
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
.L_02002542:
	movs	r0, #8
	bl 0x0200bd94
	adds	r6, r0, #0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	str	r3, [r6, #20]
	str	r3, [r6, #12]
	b.n	.L_02002bc4
.L_02002558:
	ldr	r3, [pc, #244]
	cmp	r5, r3
	bne.n	.L_02002654
	movs	r0, #0
	bl 0x0200bea4
	movs	r0, #9
	bl 0x0200bd94
	movs	r1, #0
	bl 0x0200bd34
	movs	r0, #10
	bl 0x0200bd94
	movs	r1, #0
	bl 0x0200bd34
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #8
	bl 0x0200b2d4
	movs	r0, #40
	movs	r1, #8
	movs	r2, #8
	bl 0x0200b2d4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #102
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_020025cc
	bl 0x0200b628
	movs	r1, #4
	movs	r2, #16
	movs	r3, #2
	movs	r0, #16
	bl 0x0200b688
	movs	r0, #22
	movs	r1, #4
	movs	r2, #16
	movs	r3, #0
	bl 0x0200b688
	movs	r5, #5
.L_020025be:
	movs	r0, #33
	movs	r1, #55
	subs	r5, #1
	bl 0x0200b6b8
	cmp	r5, #0
	bge.n	.L_020025be
.L_020025cc:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #93
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_0200262c
	movs	r3, #80
	movs	r2, #27
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #75
	movs	r1, #96
	movs	r2, #7
	movs	r3, #13
	bl 0x0200bd24
	movs	r5, #16
	movs	r6, #31
	movs	r0, #91
	movs	r1, #96
	movs	r2, #7
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200bd24
	movs	r3, #91
	str	r3, [sp, #4]
	movs	r0, #99
	movs	r1, #96
	movs	r2, #7
	movs	r3, #14
	str	r5, [sp, #0]
	bl 0x0200bd24
	movs	r3, #17
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #5
	movs	r3, #9
	str	r6, [sp, #4]
	bl 0x0200bd1c
	bl 0x020087a8
	b.n	.L_02002bc4
.L_0200262c:
	movs	r3, #17
	movs	r2, #31
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #17
	movs	r1, #32
	movs	r2, #5
	movs	r3, #1
	bl 0x0200bd1c
	b.n	.L_02002bc4
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a0
	.4byte 0x000000a1
	.2byte 0x00a2
	.2byte 0x0000
.L_02002654:
	ldr	r3, [pc, #200]
	cmp	r5, r3
	beq.n	.L_0200265c
	b.n	.L_020027cc
.L_0200265c:
	movs	r0, #0
	bl 0x0200bea4
	subs	r3, r7, #6
	cmp	r3, #1
	bls.n	.L_0200266a
	b.n	.L_020027aa
.L_0200266a:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl 0x0200bd94
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #5
	movs	r2, #4
	movs	r0, #12
	bl 0x0200b420
	movs	r1, #3
	movs	r2, #4
	movs	r0, #13
	bl 0x0200b420
	movs	r1, #4
	movs	r2, #4
	movs	r0, #14
	bl 0x0200b420
	movs	r0, #40
	movs	r1, #4
	movs	r2, #4
	bl 0x0200b2d4
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02002724
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02002724
	mov	r3, r8
	ldr	r6, [r3, #32]
	movs	r3, #71
	str	r3, [sp, #0]
	movs	r5, #39
	movs	r0, #64
	movs	r1, #64
	movs	r2, #16
	movs	r3, #13
	str	r5, [sp, #4]
	bl 0x0200bd24
	movs	r3, #7
	str	r3, [sp, #0]
	movs	r0, #71
	movs	r1, #39
	movs	r2, #16
	movs	r3, #15
	str	r5, [sp, #4]
	bl 0x0200bd1c
	bl 0x02008b84
	movs	r3, #131
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #10
	ldrh	r2, [r0, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r1, sp
	adds	r3, #252
	adds	r1, #10
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #8]
	orrs	r3, r2
	strh	r3, [r0, #0]
	b.n	.L_0200278c
	.2byte 0x0000
	.4byte 0x00000001
	.2byte 0x00a3
	.2byte 0x0000
.L_02002724:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_0200273e
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_0200278c
.L_0200273e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_0200275a
	movs	r1, #140
	movs	r2, #178
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200bddc
.L_0200275a:
	movs	r1, #38
	movs	r0, #12
	bl 0x02008b24
	bl 0x0200b628
	movs	r0, #12
	movs	r1, #2
	movs	r2, #45
	movs	r3, #1
	bl 0x0200b688
	movs	r5, #7
.L_02002774:
	movs	r0, #33
	movs	r1, #55
	subs	r5, #1
	bl 0x0200b6b8
	cmp	r5, #0
	bge.n	.L_02002774
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r0, #255
.L_02002788:
	bl 0x0200bca4
.L_0200278c:
	movs	r5, #12
.L_0200278e:
	adds	r0, r5, #0
	bl 0x0200bd94
	adds	r6, r0, #0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #1
	strb	r3, [r2, #0]
	str	r3, [r6, #20]
	str	r3, [r6, #12]
	cmp	r5, #14
	ble.n	.L_0200278e
	b.n	.L_020027b6
.L_020027aa:
	movs	r2, #4
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
.L_020027b6:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #106
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_020027c6
	b.n	.L_02002bc4
.L_020027c6:
	bl 0x02008ad8
	b.n	.L_02002bc4
.L_020027cc:
	ldr	r3, [pc, #640]
	cmp	r5, r3
	bne.n	.L_0200282c
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
	movs	r0, #40
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2d4
	movs	r0, #0
	bl 0x0200bea4
	subs	r3, r7, #1
	cmp	r3, #3
	bls.n	.L_020027fa
	cmp	r7, #7
	beq.n	.L_020027fa
	b.n	.L_02002bc4
.L_020027fa:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #103
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_0200280a
	b.n	.L_02002bc4
.L_0200280a:
	bl 0x0200b628
	movs	r0, #44
	movs	r1, #0
	movs	r2, #34
	movs	r3, #2
	bl 0x0200b688
	movs	r5, #0
.L_0200281c:
	movs	r0, #33
	movs	r1, #55
	adds	r5, #1
	bl 0x0200b6b8
	cmp	r5, #4
	ble.n	.L_0200281c
	b.n	.L_02002bc4
.L_0200282c:
	ldr	r3, [pc, #548]
	cmp	r5, r3
	bne.n	.L_02002900
	movs	r0, #0
	bl 0x0200bea4
	cmp	r7, #1
	beq.n	.L_02002840
	cmp	r7, #13
	bne.n	.L_0200284e
.L_02002840:
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
	b.n	.L_02002866
.L_0200284e:
	cmp	r7, #9
	beq.n	.L_0200285a
	cmp	r7, #11
	beq.n	.L_0200285a
	cmp	r7, #12
	bne.n	.L_02002866
.L_0200285a:
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
.L_02002866:
	movs	r0, #142
	lsls	r0, r0, #2
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02002890
	movs	r5, #13
.L_02002874:
	adds	r0, r5, #0
	bl 0x0200bd94
	adds	r6, r0, #0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #1
	strb	r3, [r2, #0]
	str	r3, [r6, #8]
	str	r3, [r6, #16]
	cmp	r5, #15
	ble.n	.L_02002874
	b.n	.L_02002bc4
.L_02002890:
	movs	r3, #113
	movs	r2, #39
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #114
	movs	r1, #39
	movs	r2, #1
	movs	r3, #2
	bl 0x0200bd24
	movs	r5, #41
	movs	r0, #105
	movs	r1, #41
	movs	r2, #12
	movs	r3, #13
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bd1c
	movs	r3, #105
	str	r3, [sp, #0]
	movs	r0, #105
	movs	r1, #105
	movs	r2, #12
	movs	r3, #13
	str	r5, [sp, #4]
	bl 0x0200bd24
	movs	r5, #13
.L_020028ca:
	adds	r0, r5, #0
	bl 0x0200bd94
	movs	r2, #145
	lsls	r2, r2, #2
	adds	r6, r0, #0
	adds	r0, r5, r2
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_020028f8
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #47
	movs	r1, #45
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bd1c
.L_020028f8:
	adds	r5, #1
	cmp	r5, #15
	ble.n	.L_020028ca
	b.n	.L_02002bc4
.L_02002900:
	ldr	r3, [pc, #340]
	cmp	r5, r3
	bne.n	.L_02002914
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
	b.n	.L_02002bc4
.L_02002914:
	ldr	r3, [pc, #324]
	cmp	r5, r3
	bne.n	.L_0200293c
	movs	r0, #0
	bl 0x0200bea4
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
	bl 0x0200bedc
	movs	r0, #0
	movs	r1, #8
	movs	r2, #9
	bl 0x0200bee4
	b.n	.L_02002bc4
.L_0200293c:
	ldr	r3, [pc, #288]
	cmp	r5, r3
	beq.n	.L_02002944
	b.n	.L_02002b46
.L_02002944:
	subs	r3, r7, #3
	cmp	r3, #1
	bls.n	.L_0200294c
	b.n	.L_02002afe
.L_0200294c:
	ldr	r3, [pc, #276]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bd94
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #6
	movs	r2, #4
	movs	r0, #10
	bl 0x0200b420
	movs	r1, #3
	movs	r2, #4
	movs	r0, #11
	bl 0x0200b420
	movs	r1, #5
	movs	r2, #4
	movs	r0, #12
	bl 0x0200b420
	movs	r1, #6
	movs	r2, #4
	movs	r0, #13
	bl 0x0200b420
	movs	r1, #5
	movs	r2, #4
	movs	r0, #14
	bl 0x0200b420
	movs	r1, #4
	movs	r2, #4
	movs	r0, #15
	bl 0x0200b420
	movs	r1, #4
	movs	r2, #4
	movs	r0, #16
	bl 0x0200b420
	movs	r1, #3
	movs	r2, #4
	movs	r0, #17
	bl 0x0200b420
	movs	r1, #6
	movs	r2, #4
	movs	r0, #18
	bl 0x0200b420
	movs	r0, #40
	movs	r1, #4
	movs	r2, #4
	bl 0x0200b2d4
	movs	r0, #166
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02002a68
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02002a68
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #32]
	movs	r3, #100
	str	r3, [sp, #0]
	movs	r5, #13
	movs	r0, #64
	movs	r1, #64
	movs	r2, #19
	movs	r3, #17
	str	r5, [sp, #4]
	bl 0x0200bd24
	movs	r3, #35
	movs	r2, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #105
	movs	r2, #23
	movs	r3, #24
	bl 0x0200bd24
	movs	r3, #36
	str	r3, [sp, #0]
	movs	r0, #100
	movs	r1, #13
	movs	r2, #19
	movs	r3, #19
	str	r5, [sp, #4]
	bl 0x0200bd1c
	movs	r3, #131
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #10
	ldrh	r2, [r0, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r1, sp
	adds	r3, #252
	adds	r1, #10
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #4]
	orrs	r3, r2
	strh	r3, [r0, #0]
	b.n	.L_02002afe
	.4byte 0x00000001
	.4byte 0x000000a4
	.4byte 0x000000a5
	.4byte 0x000000a6
	.4byte 0x000000a7
	.4byte 0x000000a8
	.2byte 0x0240
	.2byte 0x0200
.L_02002a68:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02002a84
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #74
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_02002afe
.L_02002a84:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_02002abc
	movs	r1, #202
	movs	r2, #148
	movs	r0, #12
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200bddc
	movs	r1, #170
	movs	r2, #180
	movs	r0, #13
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200bddc
	movs	r1, #186
	movs	r2, #212
	movs	r0, #15
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200bddc
.L_02002abc:
	movs	r0, #12
	movs	r1, #38
.L_02002ac0:
	bl 0x02008b24
	movs	r0, #13
	movs	r1, #39
	bl 0x02008b24
	movs	r1, #37
	movs	r0, #15
	bl 0x02008b24
	bl 0x0200b628
	movs	r0, #38
	movs	r1, #2
	movs	r2, #19
	movs	r3, #1
	bl 0x0200b688
	movs	r5, #7
.L_02002ae6:
	movs	r0, #33
	movs	r1, #55
	subs	r5, #1
	bl 0x0200b6b8
	cmp	r5, #0
	bge.n	.L_02002ae6
	movs	r0, #166
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bca4
.L_02002afe:
	movs	r5, #10
.L_02002b00:
	adds	r0, r5, #0
	bl 0x0200bd94
	adds	r6, r0, #0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #1
	strb	r3, [r2, #0]
	str	r3, [r6, #20]
	str	r3, [r6, #12]
	cmp	r5, #20
	ble.n	.L_02002b00
	movs	r0, #19
	bl 0x0200bd94
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #39
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #23
	movs	r1, #39
	movs	r2, #1
	bl 0x0200bd1c
	subs	r3, r7, #1
	cmp	r3, #1
	bhi.n	.L_02002bc4
	movs	r0, #0
	bl 0x0200bea4
	b.n	.L_02002bc4
.L_02002b46:
	ldr	r3, [pc, #136]
	cmp	r5, r3
	bne.n	.L_02002bc4
	movs	r0, #0
	bl 0x0200bea4
	movs	r2, #2
	negs	r2, r2
	movs	r0, #33
	movs	r1, #0
	bl 0x0200b2d4
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200bca4
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #73
	bl 0x0200bc9c
	cmp	r0, #0
	beq.n	.L_02002b8a
	movs	r3, #13
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #38
	movs	r1, #9
	movs	r2, #7
	movs	r3, #6
	bl 0x0200bd1c
	b.n	.L_02002bc4
.L_02002b8a:
	movs	r3, #50
	str	r3, [sp, #0]
	movs	r5, #8
	movs	r0, #77
	movs	r1, #8
	movs	r2, #7
	movs	r3, #6
	str	r5, [sp, #4]
	bl 0x0200bd24
	movs	r3, #77
	str	r3, [sp, #0]
	movs	r0, #70
	movs	r1, #8
	movs	r2, #7
	movs	r3, #6
	str	r5, [sp, #4]
	bl 0x0200bd24
	movs	r3, #13
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #38
	movs	r1, #17
	movs	r2, #7
	movs	r3, #6
	bl 0x0200bd1c
.L_02002bc4:
	movs	r0, #0
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x00a9
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r0, #128
	lsls	r0, r0, #11
	adds	r2, r2, r0
	adds	r3, r3, r0
	ldr	r1, [r6, #8]
	movs	r0, #14
	bl 0x0200bcd4
	ldr	r2, [r6, #80]
	adds	r5, r0, #0
	mov	r8, r2
	cmp	r5, #0
	beq.n	.L_02002c30
	ldr	r3, [r6, #20]
	ldr	r7, [r5, #80]
	str	r3, [r5, #20]
	ldr	r1, [pc, #52]
	bl 0x0200bccc
	adds	r3, r5, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	cmp	r7, #0
	beq.n	.L_02002c30
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200bcb4
	strb	r5, [r7, #26]
	mov	r2, r8
	ldrb	r3, [r2, #9]
	ldrb	r1, [r7, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #9]
.L_02002c30:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xc11c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	ldr	r7, [r3, #0]
	movs	r3, #15
	ands	r7, r3
	cmp	r7, #0
	bne.n	.L_02002c94
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #14
	adds	r0, #255
	bl 0x0200bcd4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02002c94
	ldr	r1, [pc, #60]
	ldr	r6, [r5, #80]
	bl 0x0200bccc
	adds	r3, r5, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	ldr	r3, [pc, #48]
	adds	r2, r5, #0
	str	r3, [r5, #12]
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_02002c94
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200bcb4
	ldrb	r3, [r6, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r7, [r6, #26]
	strb	r2, [r6, #9]
.L_02002c94:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x0200c128
	.2byte 0x8000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r6, [pc, #320]
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #133
	ldr	r2, [r3, #108]
	lsls	r0, r0, #2
	adds	r3, r6, r0
	movs	r1, #230
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r2, [r2, #0]
	mov	r8, r3
	mov	r0, r8
	mov	sl, r2
	sub	sp, #12
	bl 0x0200bd94
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r6, r2
	adds	r5, r0, #0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #276]
	cmp	r2, r3
	bne.n	.L_02002ce8
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_02002de4
.L_02002ce8:
	ldr	r3, [r5, #8]
	mov	r7, sp
	str	r3, [r7, #0]
	movs	r1, #128
	ldr	r3, [r5, #12]
	lsls	r1, r1, #10
	str	r3, [r7, #4]
	adds	r0, r5, #0
	ldr	r3, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
.L_02002d00:
	bl 0x0200bd2c
	ldr	r3, [pc, #240]
	movs	r2, #4
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02002d18
	adds	r0, r5, #0
	bl 0x0200abd4
.L_02002d18:
	cmp	r6, #0
	bge.n	.L_02002d6a
	movs	r1, #129
	mov	r0, r8
	lsls	r1, r1, #1
	bl 0x0200be3c
	ldr	r3, [r5, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r3, r3, r0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x0200bce4
	adds	r0, r5, #0
	movs	r1, #49
	bl 0x0200bcc4
	adds	r0, r5, #0
	bl 0x0200bcec
.L_02002d46:
	movs	r0, #1
	bl 0x0200bc34
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bne.n	.L_02002d46
	adds	r0, r5, #0
	bl 0x0200abd4
	adds	r0, r5, #0
	movs	r1, #49
	bl 0x0200bcc4
	movs	r0, #3
	bl 0x0200bc34
	b.n	.L_02002de4
.L_02002d6a:
	ldr	r3, [r5, #8]
	movs	r1, #128
	str	r3, [r7, #0]
	lsls	r1, r1, #12
	ldr	r3, [r5, #12]
	adds	r0, r5, #0
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
	bl 0x0200bd2c
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_02002de4
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #108]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r1, r7, #0
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r7, #8]
	bl 0x0200bd2c
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_02002de4
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #80]
	ldr	r0, [pc, #76]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r1, r7, #0
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	str	r3, [r7, #8]
	adds	r0, r5, #0
	bl 0x0200bd2c
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_02002de4
	mov	r1, sl
	ldr	r3, [r1, #16]
.L_02002dd0:
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r1, #16]
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
.L_02002de4:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000009e
	.4byte 0x0300122c
	.4byte 0x0005b333
	.2byte 0x4ccd
	.2byte 0xfffa
	.2byte 0xb520
	bl 0x0200bd94
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02002e30
	movs	r1, #126
	adds	r1, #255
	ldr	r0, [r5, #80]
	bl 0x0200bcbc
	movs	r3, #0
	strb	r3, [r0, #5]
	strb	r3, [r0, #6]
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200bcc4
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bcc4
.L_02002e30:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #148]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	bl 0x0200bd94
	adds	r6, r0, #0
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200be2c
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #215
	bl 0x0200bef4
	adds	r0, r6, #0
	movs	r1, #18
	bl 0x0200bcc4
	movs	r0, #153
	lsls	r0, r0, #2
	bl 0x0200bef4
	movs	r5, #0
.L_02002e86:
	cmp	r5, #30
	bne.n	.L_02002e8e
	bl 0x0200be94
.L_02002e8e:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #60]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldrh	r3, [r6, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	movs	r3, #7
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_02002eb2
	movs	r0, #15
	bl 0x0200bd94
	bl 0x0200abd4
.L_02002eb2:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200bc34
	cmp	r5, #59
	ble.n	.L_02002e86
	bl 0x0200bd84
	adds	r0, r7, #0
	bl 0x0200be5c
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #128]
	sub	sp, #56
	ldr	r2, [r3, #0]
	mov	r8, r3
	movs	r3, #1
	ands	r3, r2
	adds	r7, r0, #0
	cmp	r3, #0
	beq.n	.L_02002f50
	movs	r3, #7
	add	r6, sp, #16
	str	r3, [r6, #4]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02002efe
	movs	r3, #5
	str	r3, [r6, #4]
.L_02002efe:
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	movs	r5, #0
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	str	r5, [r6, #0]
	bl 0x0200bc4c
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r4, r0, #1
	adds	r4, r4, r0
	lsls	r3, r4, #4
	adds	r4, r4, r3
	lsls	r3, r4, #8
	adds	r4, r4, r3
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #15
	ldr	r0, [r7, #8]
	ands	r2, r3
	movs	r3, #8
	subs	r3, r3, r2
	ldr	r1, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	movs	r3, #208
	lsls	r3, r3, #13
	adds	r1, r1, r3
	movs	r3, #176
	lsls	r3, r3, #12
	ldr	r2, [r7, #16]
	negs	r4, r4
	str	r3, [sp, #8]
	movs	r3, #0
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	bl 0x0200815c
.L_02002f50:
	movs	r0, #0
	add	sp, #56
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r5, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	sl, r0
	ldr	r0, [r5, #0]
	bl 0x0200bd94
	adds	r6, r0, #0
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	movs	r0, #228
	bl 0x0200bef4
	ldr	r3, [pc, #108]
	movs	r2, #0
	str	r3, [r6, #108]
	mov	r8, r2
	adds	r3, r6, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	str	r3, [r6, #48]
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200bdec
	movs	r2, #8
	negs	r2, r2
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x0200bdcc
	ldr	r0, [r5, #0]
	bl 0x0200bdd4
	ldr	r0, [r5, #0]
	bl 0x0200bd94
	movs	r1, #9
	bl 0x0200be0c
	ldr	r0, [r5, #0]
	bl 0x0200bd94
	movs	r1, #0
	bl 0x0200bd34
	mov	r3, r8
	str	r3, [r6, #108]
	bl 0x0200be94
	bl 0x0200be9c
	mov	r0, sl
	bl 0x0200be5c
	bl 0x0200bd84
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xaed5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #228]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200bd94
	adds	r6, r0, #0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bc9c
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_020030e2
	bl 0x0200bd7c
	movs	r0, #0
	bl 0x0200beac
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	movs	r3, #0
	bl 0x0200be4c
	movs	r3, #85
	adds	r3, r3, r6
	strb	r7, [r3, #0]
	mov	r8, r3
	movs	r2, #10
	ldrsh	r1, [r6, r2]
	movs	r3, #18
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #156]
	lsls	r2, r2, #16
	adds	r2, r2, r3
	lsls	r1, r1, #16
	ldr	r0, [r5, #0]
	bl 0x0200bddc
	ldr	r0, [r5, #0]
	bl 0x0200bd94
	movs	r1, #9
	bl 0x0200be0c
	ldr	r0, [r5, #0]
	bl 0x0200bd94
.L_0200306c:
	movs	r1, #0
	bl 0x0200bd34
	bl 0x0200be8c
	movs	r0, #228
	bl 0x0200bef4
	ldr	r3, [pc, #112]
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	str	r3, [r6, #108]
	ldr	r0, [r5, #0]
	adds	r1, #102
	adds	r2, #51
	bl 0x0200bd9c
	movs	r2, #8
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x0200bec4
	ldr	r0, [r5, #0]
	bl 0x0200bd94
	movs	r1, #0
	bl 0x0200be0c
	ldr	r0, [r5, #0]
.L_020030aa:
	bl 0x0200bd94
	movs	r1, #1
	bl 0x0200bd34
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r2, #10
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200bec4
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	str	r7, [r6, #108]
	bl 0x0200bed4
	bl 0x0200be9c
	bl 0x0200bd84
.L_020030e2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfff00000
	.2byte 0xaed5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	sub	sp, #16
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	ldr	r0, [pc, #340]
	str	r3, [sp, #12]
	ldr	r4, [sp, #12]
	ldr	r3, [pc, #340]
	adds	r5, r0, #4
	ands	r4, r3
	str	r4, [sp, #12]
	ldr	r2, [r2, #4]
	ands	r2, r3
	ldr	r3, [r1, #0]
	mov	fp, r2
	ldr	r3, [r3, #4]
	ldr	r2, [pc, #324]
	mov	sl, r3
	ldrh	r3, [r0, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	str	r3, [sp, #0]
	movs	r3, #31
	str	r3, [sp, #4]
.L_0200313c:
	ldrh	r0, [r5, #18]
	adds	r3, r0, #0
	cmp	r3, #0
	bne.n	.L_02003146
	b.n	.L_0200324c
.L_02003146:
	ldr	r4, [r5, #12]
	ldr	r1, [r5, #20]
	ldr	r2, [r5, #4]
	mov	ip, r4
	cmp	r1, #0
	beq.n	.L_02003170
	ldr	r3, [r1, #0]
	ldr	r4, [sp, #12]
	subs	r6, r3, r4
	ldr	r3, [r1, #4]
	mov	r4, fp
	adds	r3, r3, r2
	mov	r2, sl
	subs	r2, r3, r2
	ldr	r3, [r1, #8]
	mov	r1, sl
	subs	r3, r3, r4
	subs	r7, r3, r1
	mov	r8, r2
	subs	r4, r7, r2
	b.n	.L_02003188
.L_02003170:
	ldr	r3, [r5, #0]
.L_02003172:
	ldr	r4, [sp, #12]
	mov	r1, sl
	subs	r6, r3, r4
	ldr	r3, [r5, #8]
	subs	r1, r2, r1
	mov	r2, fp
	mov	r4, sl
	subs	r3, r3, r2
	subs	r7, r3, r4
	mov	r8, r1
	subs	r4, r7, r1
.L_02003188:
	ldr	r3, [r5, #4]
	cmp	r3, ip
	bne.n	.L_0200319a
	adds	r3, r0, #0
	movs	r0, #192
	lsls	r0, r0, #4
	mov	r9, r0
	cmp	r3, #2
	bne.n	.L_020031a0
.L_0200319a:
	movs	r1, #128
	lsls	r1, r1, #4
	mov	r9, r1
.L_020031a0:
	mov	r0, r8
	adds	r3, r0, r7
	asrs	r3, r3, #16
	adds	r3, #50
	asrs	r2, r6, #16
	asrs	r1, r4, #16
	str	r3, [sp, #8]
	movs	r3, #135
	adds	r6, r2, #0
	adds	r4, r1, #0
	adds	r2, #7
	lsls	r3, r3, #1
	subs	r6, #8
	subs	r4, #8
	cmp	r2, r3
	bhi.n	.L_0200324c
	adds	r3, r1, #0
	adds	r3, #39
	cmp	r3, #238
	bhi.n	.L_020031fa
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r6, r3
	movs	r3, #255
	ands	r4, r3
	ldrh	r3, [r5, #16]
	mov	r0, r9
	lsls	r1, r3, #3
	movs	r3, #0
	str	r3, [r5, #24]
	lsls	r3, r6, #16
	orrs	r4, r3
	ldr	r3, [pc, #144]
	orrs	r4, r3
	str	r4, [r5, #28]
	ldr	r4, [sp, #0]
	adds	r3, r4, r1
	orrs	r3, r0
	str	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #24
	ldr	r1, [sp, #8]
	bl 0x0200bc94
.L_020031fa:
	mov	r1, r8
	subs	r4, r7, r1
	asrs	r3, r4, #16
	adds	r4, r3, #0
	adds	r3, #55
	adds	r4, #8
	cmp	r3, #238
	bhi.n	.L_0200324c
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #12]
	subs	r3, r3, r2
	asrs	r1, r3, #16
	cmp	r1, #0
	beq.n	.L_0200324c
	movs	r3, #255
	adds	r0, r5, #0
	subs	r1, #1
	ands	r4, r3
	adds	r0, #36
	cmp	r1, #7
	bls.n	.L_02003226
	movs	r1, #7
.L_02003226:
	lsls	r3, r1, #2
	adds	r1, r3, #0
	movs	r3, #0
	str	r3, [r0, #0]
	lsls	r3, r6, #16
	orrs	r4, r3
	movs	r3, #192
	lsls	r3, r3, #7
	orrs	r4, r3
	str	r4, [r5, #40]
	ldr	r2, [sp, #0]
	adds	r1, #64
	adds	r3, r2, r1
	mov	r4, r9
	orrs	r4, r3
	str	r4, [r5, #44]
	ldr	r1, [sp, #8]
	bl 0x0200bc94
.L_0200324c:
	ldr	r0, [sp, #4]
	adds	r5, #48
	subs	r0, #1
	str	r0, [sp, #4]
	cmp	r0, #0
	blt.n	.L_0200325a
	b.n	.L_0200313c
.L_0200325a:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d36c
	.4byte 0xffff0000
	.4byte 0x020036e0
	.2byte 0x2000
	.2byte 0x4000
	push	{r5, r6, lr}
	ldr	r6, [pc, #72]
	movs	r1, #192
	lsls	r1, r1, #3
	ldr	r3, [pc, #68]
	adds	r1, #4
.L_02003284:
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20c8
	lsls	r0, r0, #4
	bl 0x0200bc6c
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #52]
	bl 0x0200bc7c
	bl 0x0200bc8c
.L_020032a0:
	strh	r0, [r6, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	adds	r2, r5, #0
.L_020032a8:
	ldrh	r0, [r6, #0]
	bl 0x0200bc84
	adds	r0, r5, #0
	bl 0x0200bc74
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #28
	ldr	r0, [pc, #20]
	bl 0x0200bc3c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200d36c
	.4byte 0x03000258
	.4byte 0x0200c14c
	.2byte 0xb0f5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r1, [sp, #24]
	str	r2, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	mov	sl, r0
	mov	ip, r3
	movs	r3, #132
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r1, [r3, #48]
	mov	r2, ip
	adds	r2, #236
	ldr	r0, [r2, #0]
	mov	r8, r1
	ldr	r1, [r3, #8]
	adds	r2, #4
	adds	r1, r1, r0
	str	r1, [sp, #16]
	ldr	r1, [r2, #0]
	ldr	r3, [r3, #12]
	ldr	r2, [pc, #264]
	adds	r3, r3, r1
	str	r3, [sp, #12]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	mov	r9, r2
	subs	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #8]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r1
	asrs	r3, r3, #20
	str	r3, [sp, #4]
	asrs	r1, r1, #20
	ldrh	r2, [r2, #2]
	lsls	r1, r1, #7
	lsls	r3, r2, #1
	adds	r1, r1, r0
	str	r1, [sp, #0]
	adds	r3, r3, r2
	ldr	r1, [sp, #24]
	ldr	r2, [pc, #216]
	lsls	r3, r3, #4
	add	r3, r9
	adds	r5, r3, #4
	lsls	r3, r1, #8
	str	r3, [r2, #0]
	ldr	r2, [sp, #0]
	ldr	r1, [sp, #4]
	lsls	r3, r2, #2
	add	r8, r3
	movs	r3, #0
	mov	lr, r3
	cmp	lr, r1
	bge.n	.L_02003408
.L_0200335c:
	mov	r2, lr
	lsls	r2, r2, #16
	lsrs	r3, r2, #7
	mov	fp, r2
	ldr	r2, [sp, #8]
	mov	r1, r8
	movs	r7, #0
	adds	r6, r1, r3
	cmp	r7, r2
	bge.n	.L_020033f2
.L_02003370:
	ldrb	r4, [r6, #2]
	cmp	r4, #0
	beq.n	.L_020033de
	cmp	r4, sl
	bcc.n	.L_020033de
	mov	r3, sl
	adds	r3, #8
	cmp	r4, r3
	bcs.n	.L_020033de
	ldr	r2, [sp, #16]
	lsls	r1, r7, #16
	lsrs	r1, r1, #16
	lsls	r3, r1, #20
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [sp, #12]
	mov	r3, fp
	lsrs	r0, r3, #16
	lsls	r3, r0, #20
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r2, [sp, #20]
	lsls	r0, r0, #7
	lsls	r3, r2, #19
	str	r3, [r5, #12]
	ldr	r2, [sp, #24]
	adds	r1, r1, r0
	lsls	r3, r2, #19
	mov	r2, sl
	str	r3, [r5, #4]
	subs	r3, r4, r2
	strh	r3, [r5, #16]
	movs	r2, #0
	movs	r3, #1
	str	r2, [r5, #20]
	strh	r3, [r5, #18]
	mov	r2, r9
	ldrh	r3, [r2, #2]
	adds	r5, #48
	adds	r3, #1
	strh	r3, [r2, #2]
	movs	r3, #158
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r2, [r3, #0]
	ldr	r3, [sp, #0]
	adds	r2, r2, r3
	movs	r3, #120
	strb	r3, [r2, r1]
.L_020033de:
	movs	r1, #128
	lsls	r3, r7, #16
	lsls	r1, r1, #9
	ldr	r2, [sp, #8]
	adds	r3, r3, r1
	asrs	r7, r3, #16
	lsrs	r3, r3, #16
	adds	r6, #4
	cmp	r3, r2
	blt.n	.L_02003370
.L_020033f2:
	mov	r1, lr
	movs	r2, #128
	lsls	r3, r1, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	ldr	r2, [sp, #4]
	asrs	r1, r3, #16
	lsrs	r3, r3, #16
	mov	lr, r1
	cmp	r3, r2
	blt.n	.L_0200335c
.L_02003408:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d36c
	.2byte 0xc1e0
	.2byte 0x0202
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r2, #0
	bl 0x0200bd94
	ldr	r4, [pc, #48]
	ldrh	r2, [r4, #2]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	adds	r3, r3, r4
	adds	r1, r3, #4
	cmp	r0, #0
	beq.n	.L_0200345a
	movs	r3, #0
	str	r3, [r1, #0]
	str	r3, [r1, #8]
	lsls	r3, r5, #19
	str	r3, [r1, #12]
	str	r3, [r1, #4]
	movs	r3, #2
	strh	r3, [r1, #18]
	adds	r3, r0, #0
	adds	r3, #8
	strh	r6, [r1, #16]
	str	r3, [r1, #20]
	ldrh	r3, [r4, #2]
	adds	r3, #1
	strh	r3, [r4, #2]
.L_0200345a:
	pop	{r5, r6, pc}
	.2byte 0xd36c
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #44]
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_0200346e
	ldr	r2, [pc, #40]
	adds	r3, r0, r2
.L_0200346e:
	movs	r2, #255
	asrs	r3, r3, #19
	ands	r3, r2
	ldr	r2, [pc, #32]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	adds	r2, r1, #4
	movs	r1, #31
.L_0200347e:
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_02003486
	str	r0, [r2, #4]
.L_02003486:
	subs	r1, #1
	adds	r2, #48
	cmp	r1, #0
	bge.n	.L_0200347e
	pop	{pc}
	.4byte 0x0200d36c
	.4byte 0x0007ffff
	.2byte 0xc1e0
	.2byte 0x0202
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r4, r1, #0
	ldr	r2, [pc, #72]
	ldr	r1, [r3, #32]
	cmp	r0, #31
	bhi.n	.L_020034ec
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	adds	r3, r3, r2
	adds	r0, r3, #4
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_020034ec
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_020034c6
	ldr	r2, [pc, #48]
	adds	r3, r4, r2
.L_020034c6:
	movs	r2, #255
	asrs	r3, r3, #19
	ands	r3, r2
	ldr	r2, [pc, #40]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #0]
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	asrs	r2, r2, #20
	adds	r2, r2, r3
	movs	r3, #121
	strb	r3, [r1, r2]
	str	r4, [r0, #4]
.L_020034ec:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200d36c
	.4byte 0x0007ffff
	.2byte 0xc1e4
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	sub	sp, #16
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	ldr	r0, [pc, #252]
	str	r3, [sp, #12]
	ldr	r4, [sp, #12]
	ldr	r3, [pc, #252]
	ands	r4, r3
	str	r4, [sp, #12]
	ldr	r2, [r2, #4]
	ands	r2, r3
	ldr	r3, [r1, #0]
	mov	fp, r2
	ldr	r3, [r3, #4]
	ldr	r2, [pc, #240]
	str	r3, [sp, #8]
	ldrh	r3, [r0, #0]
	adds	r0, #36
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	mov	r9, r0
	lsrs	r3, r3, #5
	str	r3, [sp, #0]
	movs	r3, #31
	str	r3, [sp, #4]
.L_02003546:
	mov	r4, r9
	ldrh	r3, [r4, #12]
	cmp	r3, #0
	beq.n	.L_020035fa
	ldr	r3, [r4, #0]
	ldr	r2, [sp, #12]
	ldr	r5, [r4, #4]
	ldr	r4, [r4, #8]
	subs	r2, r3, r2
	ldr	r3, [sp, #8]
	mov	r8, r4
	ldr	r4, [sp, #8]
	mov	sl, r2
	subs	r5, r5, r3
	mov	r2, r8
	mov	r3, fp
	subs	r2, r2, r3
	subs	r2, r2, r4
	subs	r7, r2, r5
	mov	r8, r2
	bl 0x0200bc4c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	mov	r2, sl
	lsls	r3, r3, #1
	asrs	r6, r2, #16
	lsrs	r3, r3, #16
	adds	r6, r6, r3
	movs	r3, #10
	negs	r3, r3
	adds	r3, r3, r6
	mov	sl, r3
	bl 0x0200bc4c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	asrs	r2, r7, #16
	add	r5, r8
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	asrs	r5, r5, #16
	movs	r4, #135
	adds	r7, r2, #0
	adds	r5, #50
	adds	r6, #5
	lsls	r4, r4, #1
	subs	r7, #10
	mov	r8, r5
	cmp	r6, r4
	bhi.n	.L_020035fa
	adds	r3, r2, #0
	adds	r3, #37
	cmp	r3, #238
	bhi.n	.L_020035fa
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r2, sl
	ands	r2, r3
	movs	r3, #255
	mov	r4, r9
	ands	r7, r3
	movs	r3, #0
	str	r3, [r4, #16]
	lsls	r3, r2, #16
	orrs	r7, r3
	ldr	r3, [pc, #84]
	mov	r5, r9
	orrs	r7, r3
	str	r7, [r4, #20]
	bl 0x0200bc4c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	ldr	r2, [sp, #0]
	lsrs	r3, r3, #16
	lsls	r3, r3, #3
	movs	r4, #128
.L_020035e6:
	adds	r3, r2, r3
	lsls	r4, r4, #4
	adds	r5, #24
	orrs	r3, r4
.L_020035ee:
	mov	r0, r9
	str	r3, [r5, #0]
	adds	r0, #16
	mov	r1, r8
.L_020035f6:
	bl 0x0200bc94
.L_020035fa:
	ldr	r2, [sp, #4]
	movs	r3, #28
	subs	r2, #1
	str	r2, [sp, #4]
	add	r9, r3
	cmp	r2, #0
	bge.n	.L_02003546
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d970
	.4byte 0xffff0000
	.4byte 0x020036e0
	.2byte 0x2000
	.2byte 0x4000
	push	{r5, r6, lr}
	ldr	r6, [pc, #76]
	movs	r1, #233
	ldr	r3, [pc, #76]
	lsls	r1, r1, #2
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2080
	lsls	r0, r0, #3
	bl 0x0200bc6c
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #56]
	bl 0x0200bc7c
	bl 0x0200bc8c
	strh	r0, [r6, #0]
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #3
	ldrh	r0, [r6, #0]
	bl 0x0200bc84
	adds	r0, r5, #0
	bl 0x0200bc74
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #28
	ldr	r0, [pc, #24]
	bl 0x0200bc3c
	movs	r0, #220
	bl 0x0200bef4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200d970
	.4byte 0x03000258
	.4byte 0x0200c351
	.2byte 0xb4fd
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r2, #0
	ldr	r2, [pc, #36]
	adds	r6, r3, #0
	adds	r4, r2, #4
	ldr	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_0200369a
	adds	r4, #16
.L_0200369a:
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r3, r0, #20
	adds	r3, r3, r2
	str	r3, [r4, #0]
	lsls	r3, r1, #20
	str	r3, [r4, #4]
	lsls	r3, r5, #20
	adds	r3, r3, r2
	str	r3, [r4, #8]
	strh	r6, [r4, #14]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xd970
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #12]
	movs	r1, #0
	str	r1, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r4, [pc, #356]
	str	r3, [sp, #4]
	movs	r2, #1
	ldrh	r6, [r4, #2]
	mov	fp, r0
	mov	sl, r1
	adds	r7, r4, #4
	mov	r8, r2
.L_020036e4:
	ldr	r3, [r7, #0]
	cmp	r3, #0
.L_020036e8:
	beq.n	.L_020037ac
	ldrh	r2, [r4, #2]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r4, r3
	adds	r5, r3, #0
	adds	r5, #36
.L_020036f8:
	ldr	r1, [sp, #4]
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r7, #8]
	ldr	r0, [r7, #0]
	asrs	r3, r3, #20
	asrs	r2, r0, #20
	lsls	r3, r3, #7
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r1, r1, r2
	ldrb	r1, [r1, #2]
	cmp	r1, #255
	bne.n	.L_02003738
	str	r0, [r5, #0]
	ldr	r3, [r7, #8]
	movs	r1, #1
	str	r3, [r5, #8]
	ldr	r3, [r7, #4]
	mov	sl, r1
	str	r3, [r5, #4]
	movs	r3, #1
	strh	r3, [r5, #12]
	adds	r5, #28
	ldrh	r3, [r4, #2]
	adds	r3, #1
	strh	r3, [r4, #2]
	movs	r3, #0
	mov	r9, r3
	b.n	.L_02003782
.L_02003738:
	cmp	r1, fp
	blt.n	.L_0200375e
	mov	r3, fp
	adds	r3, #7
	cmp	r1, r3
	bgt.n	.L_0200375e
	ldrh	r3, [r7, #14]
	ldr	r2, [pc, #248]
	lsls	r3, r3, #3
	adds	r3, r3, r1
	mov	r1, fp
	subs	r3, r3, r1
.L_02003750:
	ldrsb	r3, [r2, r3]
	mov	r9, r3
	cmp	r3, #8
	beq.n	.L_0200375a
	ldrh	r6, [r4, #2]
.L_0200375a:
	movs	r2, #0
	b.n	.L_02003780
.L_0200375e:
	ldr	r3, [sp, #12]
	cmp	r1, r3
	bne.n	.L_02003776
	ldr	r2, [sp, #8]
	movs	r1, #8
	adds	r2, #1
	movs	r3, #0
	ldrh	r6, [r4, #2]
	mov	r9, r1
	str	r2, [sp, #8]
	mov	sl, r3
	b.n	.L_02003782
.L_02003776:
	cmp	r1, #0
	bne.n	.L_02003782
	movs	r1, #8
	movs	r2, #0
	mov	r9, r1
.L_02003780:
	mov	sl, r2
.L_02003782:
	mov	r3, r9
	cmp	r3, #8
	beq.n	.L_020037a4
	ldrh	r3, [r7, #14]
	movs	r2, #3
	add	r3, r9
	ands	r3, r2
	strh	r3, [r7, #14]
	movs	r0, #128
	ldrh	r1, [r7, #14]
	lsls	r0, r0, #13
	lsls	r1, r1, #14
	adds	r2, r7, #0
	str	r4, [sp, #0]
	bl 0x0200bc64
	ldr	r4, [sp, #0]
.L_020037a4:
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_020036f8
	strh	r6, [r4, #2]
.L_020037ac:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	adds	r7, #16
.L_020037b6:
	cmp	r3, #0
	bge.n	.L_020036e4
	lsls	r3, r6, #3
	subs	r3, r3, r6
	lsls	r3, r3, #2
	adds	r3, r4, r3
	adds	r5, r3, #0
	adds	r5, #36
	cmp	r6, #31
	bgt.n	.L_020037e8
	movs	r3, #32
	subs	r3, r3, r6
	mov	r8, r3
.L_020037d0:
	ldrh	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L_020037da
	movs	r3, #0
	strh	r3, [r5, #12]
.L_020037da:
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #0
	bne.n	.L_020037d0
.L_020037e8:
	adds	r5, r4, #0
	adds	r5, #36
	cmp	r6, #0
	beq.n	.L_0200382c
	mov	r8, r6
.L_020037f2:
	ldrh	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L_0200381e
	ldr	r1, [sp, #4]
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #0]
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	asrs	r2, r2, #20
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r1, r1, r2
	ldrb	r2, [r1, #3]
	movs	r3, #0
	strb	r3, [r1, #2]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r1, #3]
.L_0200381e:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #0
	bne.n	.L_020037f2
.L_0200382c:
	ldr	r0, [sp, #8]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d970
	.2byte 0xd34c
	.2byte 0x0200
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_0200385a
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02003864
	b.n	.L_02003894
.L_0200385a:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02003894
.L_02003864:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	subs	r3, #1
	cmp	r3, r2
	bhi.n	.L_02003894
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02003886
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02003890
	b.n	.L_02003894
.L_02003886:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02003894
.L_02003890:
	movs	r0, #1
	b.n	.L_02003896
.L_02003894:
	movs	r0, #0
.L_02003896:
	pop	{pc}
	.2byte 0xfffe
	.2byte 0x000f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #232]
	ldr	r3, [pc, #232]
	mov	r8, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl 0x0200bd94
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	str	r4, [sp, #0]
	bl 0x0200bc2c
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_020038ea
	adds	r3, #15
.L_020038ea:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02003936
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x0200b844
	cmp	r0, #0
	beq.n	.L_02003936
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	movs	r3, #0
	str	r3, [r6, #76]
.L_02003936:
	movs	r3, #164
	lsls	r3, r3, #1
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02003982
	adds	r5, r2, r3
.L_02003946:
	bl 0x0200bd94
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02003974
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x0200b844
	cmp	r0, #0
	beq.n	.L_02003974
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	movs	r3, #0
	str	r3, [r6, #76]
.L_02003974:
	adds	r7, #1
	cmp	r7, #3
	bgt.n	.L_02003982
	adds	r5, #2
	ldrh	r0, [r5, #0]
.L_0200397e:
	cmp	r0, #0
	bne.n	.L_02003946
.L_02003982:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r2, r5, #0
	adds	r3, #4
	strb	r6, [r3, #0]
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #3
	ldr	r0, [r5, #80]
	ands	r1, r3
	ldrb	r2, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r1, r1, #2
	orrs	r3, r1
	strb	r3, [r0, #9]
	movs	r1, #0
.L_020039c2:
	adds	r0, r5, #0
	bl 0x0200bd34
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bcc4
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl 0x0200bccc
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200be0c
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0xc4c8
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #4]
	adds	r6, r1, #0
	ldr	r1, [r0, #0]
	ldr	r0, [pc, #104]
	adds	r3, r3, r0
	movs	r0, #30
	adds	r0, #255
	bl 0x0200bcd4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02003a62
	bl 0x0200bc4c
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #12
	movs	r3, #192
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200bc5c
	str	r0, [r5, #68]
.L_02003a26:
	bl 0x0200bc4c
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [r5, #72]
	bl 0x0200bc4c
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r0, r0, r6
	str	r0, [r5, #76]
	bl 0x0200bc4c
	ldr	r3, [pc, #36]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200b994
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #1
	bl 0x0200bd3c
.L_02003a62:
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0xffff8000
	.2byte 0xb89d
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #56]
	movs	r6, #15
	ldr	r0, [r3, #4]
	adds	r5, r3, #0
	mov	lr, r0
	.2byte 0xf800
	.2byte 0x3508
.L_02003a80:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L_02003aa2
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #2]
	cmp	r3, #0
	bgt.n	.L_02003a9e
	adds	r0, r5, #4
	ldr	r1, [r5, #16]
	bl 0x0200b9f0
	movs	r3, #9
	b.n	.L_02003aa0
.L_02003a9e:
	subs	r3, r2, #1
.L_02003aa0:
	strh	r3, [r5, #2]
.L_02003aa2:
	subs	r6, #1
	adds	r5, #20
	cmp	r6, #0
	bge.n	.L_02003a80
	pop	{r5, r6, pc}
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r0, #0
	ldr	r0, [pc, #216]
	mov	r9, r2
	mov	sl, r0
	movs	r2, #8
	movs	r0, #10
	add	r2, sl
	adds	r0, #255
	sub	sp, #4
	adds	r7, r1, #0
	mov	r8, r2
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02003b58
	movs	r1, #168
	lsls	r1, r1, #1
	ldr	r3, [pc, #188]
	mov	r0, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x8831
	movs	r4, #0
	adds	r6, #2
	cmp	r1, #0
	ble.n	.L_02003b30
.L_02003aee:
	ldrh	r2, [r6, #0]
	mov	r0, r8
	movs	r3, #0
	ldrh	r5, [r6, #2]
	strh	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #13
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	str	r2, [r0, #12]
	str	r1, [r0, #4]
	adds	r2, r2, r3
	movs	r0, #0
	str	r4, [sp, #0]
	bl 0x0200bd0c
	ldr	r4, [sp, #0]
	mov	r2, r8
	mov	r3, r8
	lsls	r5, r5, #16
	str	r0, [r2, #8]
	str	r5, [r2, #16]
	movs	r0, #20
	strh	r4, [r3, #2]
	adds	r4, #1
	adds	r6, #4
	add	r8, r0
	cmp	r4, #15
	bgt.n	.L_02003b30
	ldrh	r1, [r6, #0]
	adds	r6, #2
	cmp	r1, #0
	bgt.n	.L_02003aee
.L_02003b30:
	cmp	r7, #0
	beq.n	.L_02003b58
	ldrh	r1, [r7, #0]
	movs	r4, #0
	adds	r7, #2
	cmp	r1, #0
	ble.n	.L_02003b58
.L_02003b3e:
	movs	r2, #164
	lsls	r3, r4, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r0, sl
	adds	r4, #1
	strh	r1, [r0, r3]
	cmp	r4, #3
	bgt.n	.L_02003b58
	ldrh	r1, [r7, #0]
	adds	r7, #2
	cmp	r1, #0
	bgt.n	.L_02003b3e
.L_02003b58:
	movs	r1, #128
	lsls	r1, r1, #19
	mov	r3, sl
	mov	r2, r9
	adds	r1, #80
	str	r2, [r3, #4]
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_02003b80
	movs	r3, #192
	movs	r2, #128
	lsls	r3, r3, #4
	lsls	r2, r2, #19
	adds	r3, #8
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #16
	strh	r3, [r1, #0]
.L_02003b80:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl 0x0200bc3c
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x03000258
	.4byte 0x0200ba71
	.4byte 0x00834a03
	.4byte 0x009b181b
	.4byte 0x2208189b
	.4byte 0x47705e98
	.4byte 0x0200254c
	.4byte 0x00834a03
	.4byte 0x009b181b
	.4byte 0x8119189b
	.4byte 0x00004770
	.4byte 0x0200254c
	.4byte 0x68184b01
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	ldr	r5, [pc, #24]
	bl 0x0200bc9c
	cmp	r0, #0
	bne.n	.L_02003bf6
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, r6
	blt.n	.L_02003bf6
	str	r0, [r5, #0]
.L_02003bf6:
	ldr	r0, [r5, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x00834a03
	.4byte 0x009b181b
	.4byte 0x6199189b
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #16]
	cmp	r0, #3
	bhi.n	.L_02003c26
	lsls	r3, r0, #1
	movs	r0, #164
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r1, [r2, r3]
.L_02003c26:
	pop	{pc}
	.4byte 0x0200254c
	.section .text.x0200bf04,"ax",%progbits
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
	.4byte 0x424b4243
	.4byte 0x425b4253
	.4byte 0x426b4263
	.4byte 0x51434273
	.4byte 0x5153514b
	.4byte 0x5163515b
	.4byte 0x5173516b
	.4byte 0x604b6043
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x020093bd
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200947d
	.4byte 0x0000002e
	.4byte 0x02009521
	.4byte 0x0000002e
	.4byte 0x0200956d
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000026
	.4byte 0x01580118
	.4byte 0x01380002
	.4byte 0x00020158
	.4byte 0x00080000
	.4byte 0x00980000
	.4byte 0x00020218
	.4byte 0x021800d8
	.4byte 0x00000002
	.4byte 0x000b000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000026
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000026
	.4byte 0x11071100
	.4byte 0xa8364b24
	.4byte 0x2d16a0e3
	.4byte 0xa0a20502
	.4byte 0x11e8f16b
	.4byte 0xd788a3cd
	.4byte 0x28f35902
	.2byte 0x8f10
	.2byte 0xc935
.L_0200416c:
	.2byte 0xb9f4
	ldr	r6, [r4, r4]
	ldrh	r7, [r0, #6]
	ldr	r1, [sp, #68]
	.2byte 0xf8d4
	.2byte 0xd73e
	subs	r0, r1, #0
	ldmia	r0, {r0, r3, r6, r7}
	.2byte 0xbe66
	subs	r4, #22
	ldr	r4, [pc, #576]
	cmp	r0, #56
.L_02004184:
	bx	sl
	.2byte 0xe08b
	.4byte 0xc788a089
	.4byte 0x3a788868
	.4byte 0x2225c912
	.4byte 0x9927de2e
	.4byte 0xe3c78dd7
	.2byte 0x8c08
.L_0200419e:
	push	{r0, r1, r2, r6, r7, lr}
	asrs	r2, r3, #10
	asrs	r2, r4, #24
	ldrsb	r3, [r6, r3]
	strb	r4, [r4, #28]
.L_020041a8:
	movs	r3, #242
	cmp	r5, #17
	str	r6, [r6, #4]
	adds	r0, #137
	movs	r4, #82
	ldr	r7, [pc, #232]
	subs	r0, r4, r0
	str	r7, [r1, r2]
	lsls	r0, r0, #12
	lsrs	r7, r2
	movs	r6, #0
	stmia	r0!, {r1, r2, r4, r6, r7}
	stmia	r7!, {r0, r1, r7}
	subs	r2, r3, #0
	str	r3, [r6, #116]
	.2byte 0xf440
	.2byte 0x9028
.L_020041ca:
	ldr	r0, [pc, #764]
	lsls	r4, r4, #1
	ldrh	r1, [r0, #6]
	ldrh	r7, [r3, #56]
	subs	r6, r2, #0
.L_020041d4:
	cmp	r5, #223
	ldrb	r6, [r3, #13]
	.2byte 0xdb7c
	ldr	r4, [sp, #396]
	subs	r4, #120
	.2byte 0xedbe
	.2byte 0x6e31
	adds	r1, #236
	ldrb	r6, [r7, #1]
	add	r6, sp, #240
	subs	r7, #249
	ldrh	r7, [r5, r6]
	ldrb	r0, [r7, #17]
	.2byte 0xf1e3
	.2byte 0xc6f8
	.2byte 0xf1e3
	.2byte 0xc78d
	subs	r3, r4, r7
	stmia	r7!, {r0, r1, r2, r3, r7}
	subs	r7, r6, #0
	ldr	r7, [r1, #120]
	subs	r4, r7, #0
	.2byte 0xfcd7
	.2byte 0x8f1e
.L_02004204:
	subs	r0, #111
	.2byte 0xf1e1
	.2byte 0xbe4d
	ldrb	r0, [r7, r5]
.L_0200420c:
	.2byte 0xf8f1
	.2byte 0xf1e6
	ldmia	r0!, {r1, r2, r7}
	ldrh	r3, [r5, #62]
	ldrh	r6, [r7, #56]
.L_02004216:
	ldrb	r7, [r5, #9]
	strh	r4, [r7, #8]
	.2byte 0xf17c
	.2byte 0x26f8
	ldr	r4, [sp, #316]
	subs	r4, #111
	svc	30
	ldmia	r6!, {r2, r3, r4, r5}
	.2byte 0xebd8
	.2byte 0x4df1
	bvc.n	.L_0200416c
	ldrh	r7, [r3, #56]
	ldrb	r7, [r5, #25]
	.2byte 0xf840
	.2byte 0x7c78
	.2byte 0xf3f7
	.2byte 0xa010
	movs	r1, #240
	subs	r6, #4
	svc	30
	subs	r0, #249
	.2byte 0xe07c
	.2byte 0x34eb
	.4byte 0x7cef969f
	.4byte 0xf6b3e69d
	.4byte 0x8f1f429d
	.4byte 0xf192bcef
	.4byte 0xc7c6f831
	.4byte 0xaf1f5be6
	.4byte 0x3a187c6f
	.4byte 0xc3e34061
	.4byte 0x9cc12df2
	.4byte 0xf9a9f1e7
	.2byte 0x0f8e
	.2byte 0xf7cf
.L_02004270:
	.2byte 0x4e7c
.L_02004272:
	ldrh	r6, [r7, #12]
	.2byte 0xf85a
	.2byte 0x8f39
.L_02004278:
	ldmia	r5!, {r0, r1, r2, r3, r6}
	ldrb	r7, [r6, #18]
	.2byte 0xbe6a
	stmia	r0!, {r0, r1, r2, r5, r6}
	ldrb	r7, [r4, #7]
.L_02004282:
	ldr	r2, [pc, #496]
.L_02004284:
	b.n	.L_02004204
	.2byte 0xf263
	.4byte 0xef9b1f0d
	.4byte 0x9faef9f9
	.4byte 0x8f3af8eb
	.4byte 0xfe7c0d57
	.2byte 0xe9be
.L_0200429a:
	.2byte 0xf043
	.2byte 0xf40d
	.2byte 0x47ca
	ldrb	r0, [r4, #15]
	lsls	r2, r4, #19
	adds	r3, r6, #5
	lsrs	r3, r0, #25
	stmia	r7!, {r0, r1, r2, r3, r4, r6, r7}
	.2byte 0xe1a7
	.4byte 0x7cb58e1b
	.4byte 0xf1e7be08
	.4byte 0x37ce452d
	.4byte 0x1df38fbc
	.4byte 0x706f839f
	.4byte 0x4b7c78f8
	.4byte 0xf033e04c
	.4byte 0x84f8d70d
	.4byte 0xc0e7ab7c
	.2byte 0x04a5
	.2byte 0x7b3d
.L_020042d4:
	lsrs	r6, r5, #20
	bcc.n	.L_0200429a
	ldr	r4, [sp, #40]
	.2byte 0xe80e
	.2byte 0xaf59
	ldrh	r6, [r3, #60]
	lsls	r3, r1, #31
	lsrs	r0, r4, #12
.L_020042e4:
	.2byte 0xbe3c
	strh	r0, [r0, #42]
	ldrb	r3, [r7, #16]
	.2byte 0xbe5c
	blt.n	.L_02004270
	ldr	r4, [sp, #992]
	strh	r4, [r3, #12]
	stmia	r1!, {r0, r1, r2, r3, r7}
	ldrb	r7, [r2, #0]
	subs	r6, r7, #4
	adds	r3, #144
	adds	r1, #240
	lsls	r0, r7, #11
.L_020042fe:
	strb	r7, [r5, #15]
	stmia	r7!, {r1, r2, r3, r6, r7}
	lsrs	r6, r4, #15
	add	r3, pc, #880
	stmia	r7!, {r6, r7}
	lsrs	r5, r4, #15
	subs	r4, #124
	stmia	r0!, {r1, r2, r3, r6, r7}
	.2byte 0xe2c7
	.4byte 0xce2c7c0b
	.4byte 0x0be2c7c0
	.4byte 0xc3cfadbc
	.4byte 0x7c78f819
	.4byte 0x19c38f81
	.4byte 0x817c78f8
	.2byte 0xf1a7
.L_0200432a:
	.2byte 0x3841
	subs	r3, r0, #4
.L_0200432e:
	cmp	r7, #147
	subs	r0, #240
	lsls	r0, r0, #8
	str	r7, [sp, #124]
	strb	r7, [r5, #0]
	ldrh	r3, [r1, #40]
	adds	r3, #175
.L_0200433c:
	strb	r0, [r6, #7]
	lsls	r1, r7, #11
	lsls	r7, r0, #19
	movs	r7, #156
	str	r0, [r6, #28]
	lsls	r1, r7, #11
	cmn	r7, r1
	stmia	r0!, {r4, r5}
	subs	r7, r4, #2
	lsls	r0, r7, #3
.L_02004350:
	movs	r0, r0
	cmp	r4, #1
	ldrb	r0, [r5, r2]
	add	r3, sp, #484
	bls.n	.L_0200432a
	.2byte 0xd13a
	.2byte 0xf087
	.2byte 0x5c4c
	.2byte 0xd956
	add	r6, pc, #520
.L_02004364:
	.2byte 0xb2cb
	stmia	r3!, {r0, r3, r4, r6}
	ldrh	r2, [r3, #44]
	cmp	r1, #231
	ldrh	r3, [r1, r4]
	push	{r3, r6, r7}
	adds	r4, #44
	subs	r6, #122
	add	r5, pc, #432
	str	r4, [sp, #468]
	add	r9, r1
	ldr	r6, [r5, r7]
	.2byte 0xf7e6
	.2byte 0xf44c
	.2byte 0xd93c
	adds	r6, #38
	subs	r5, r6, r0
	movs	r2, #30
.L_02004388:
	asrs	r1, r2, #1
	lsrs	r4, r4, #22
.L_0200438c:
	ldrh	r1, [r3, #44]
	.2byte 0xeb7a
	.2byte 0xcad3
	.2byte 0xd21f
	ldr	r6, [r4, #52]
	add	r4, pc, #280
	add	r0, pc, #844
.L_0200439a:
	str	r4, [r4, #92]
	.2byte 0xf28a
	.2byte 0xe90b
	str	r1, [r2, #108]
.L_020043a2:
	.2byte 0xfaf4
	.2byte 0xf336
	.2byte 0xbf3c
	.2byte 0x7997
	.2byte 0xa79e
	strb	r6, [r3, #29]
	subs	r6, r7, #1
	ldr	r4, [sp, #484]
	adds	r1, #23
	movs	r1, #161
.L_020043b6:
	ldr	r1, [pc, #196]
	pop	{r0, r4, r7}
.L_020043ba:
	movs	r2, #192
	ldrb	r1, [r7, #21]
	.2byte 0xfb2d
	.2byte 0xf5ed
	cmp	r1, #7
	beq.n	.L_020043b6
	subs	r1, r7, #1
.L_020043c8:
	str	r7, [sp, #252]
	stmia	r0!, {r1, r2, r3, r4, r7}
	.2byte 0xbf32
.L_020043ce:
	.2byte 0xf2af
	.2byte 0x3f65
.L_020043d2:
	.2byte 0x423b
.L_020043d4:
	.2byte 0x61e4
	.2byte 0xf04d
	.2byte 0x3474
	bge.n	.L_020043e0
.L_020043dc:
	str	r1, [sp, #32]
	subs	r1, r2, #7
.L_020043e0:
	ldmia	r4, {r3, r4, r5, r6}
.L_020043e2:
	ldmia	r2!, {r0, r5, r7}
	bcs.n	.L_02004450
	svc	6
	lsls	r4, r7, #25
	.2byte 0xf84c
	.2byte 0xc40c
	lsls	r0, r2, #12
	strh	r6, [r0, #32]
	beq.n	.L_020043c8
	ldr	r6, [sp, #336]
	muls	r0, r4
	str	r3, [sp, #216]
	subs	r7, r0, #1
.L_020043fc:
	ldrb	r1, [r7, #4]
	cmp	r5, #240
	strh	r4, [r4, #28]
	.2byte 0xf9c8
	.2byte 0x3139
	lsls	r5, r5, #31
	ldr	r7, [r4, #40]
.L_0200440a:
	lsls	r0, r1, #9
	subs	r7, r4, r2
.L_0200440e:
	subs	r1, #141
.L_02004410:
	adds	r6, r4, r4
	lsls	r4, r3, #4
.L_02004414:
	b.n	.L_02004a88
	.2byte 0x40e3
	.4byte 0x04e027c4
	.4byte 0x1882625c
	.4byte 0xf87031c1
	.4byte 0xa581f868
	.4byte 0x4100e0f1
	.4byte 0xbe868f64
	.4byte 0x0260f982
	.4byte 0xbf34282f
	.4byte 0x573956bd
	.4byte 0x2ee62c89
	.4byte 0x1c996591
	.4byte 0xe39c8d62
	.4byte 0xc2d98115
	.2byte 0xc9f9
	.2byte 0xf8fb
.L_02004450:
	.2byte 0x4f39
	strb	r2, [r3, #6]
	str	r1, [r2, #64]
	.2byte 0xbf11
	.2byte 0x21cc
	.2byte 0x8bb2
	.2byte 0x4371
	.2byte 0xf12e
	.2byte 0xc3f8
	lsls	r1, r6, #6
.L_02004464:
	ldrb	r4, [r2, #2]
	add	r6, pc, #664
	lsls	r6, r7, #16
	.2byte 0xf9bf
	.2byte 0xc1b3
	ldrsh	r7, [r2, r4]
	lsls	r6, r7, #24
	ldrb	r1, [r1, #2]
	b.n	.L_020045ea
	.2byte 0xe91c
	.4byte 0xc3857023
	.4byte 0xc7c8e18e
	.4byte 0x0e1f0402
	.4byte 0x28067c77
	.4byte 0x1b1cec14
	.4byte 0xd1f1debc
	.4byte 0xce39c948
	.4byte 0x65eb6111
	.4byte 0x1ee4778f
	.4byte 0x3c78735f
	.4byte 0xb8f4246d
	.4byte 0xf9f80201
	.4byte 0x11c2e7ca
	.4byte 0x21fb603f
	.4byte 0x3ef04650
	.4byte 0xd614ce34
	.4byte 0xca56a6c8
	.4byte 0xed5da3a8
	.4byte 0x03efdcf9
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x0200bf04
	.4byte 0x0200bf40
	.4byte 0x0200bf7c
	.4byte 0xffff0000
	.4byte 0x00000140
	.4byte 0x40000140
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000a0
	.4byte 0x001010a7
	.4byte 0x002030a0
	.4byte 0x003020a0
	.4byte 0x004050a0
	.4byte 0x005040a0
	.4byte 0x006070a0
	.4byte 0x007060a0
	.4byte 0x008090a0
	.4byte 0x009080a0
	.4byte 0x00a020a3
	.4byte 0x00b010a2
	.4byte 0x000000a1
	.4byte 0x001010a6
	.4byte 0x002030a1
	.4byte 0x003020a1
	.4byte 0x004050a7
	.4byte 0x005020a2
	.4byte 0x006080a2
	.4byte 0x007080a1
	.4byte 0x008070a1
	.4byte 0x009060a2
	.4byte 0x000000a2
	.4byte 0x0010b0a0
	.4byte 0x002050a1
	.4byte 0x003010a9
	.4byte 0x004040a3
	.4byte 0x005050a3
	.4byte 0x006090a1
	.4byte 0x007040a6
	.4byte 0x008060a1
	.4byte 0x000000a3
	.4byte 0x0010409e
	.4byte 0x0020a0a0
	.4byte 0x003020a8
	.4byte 0x004040a2
	.4byte 0x005050a2
	.4byte 0x006060a4
	.4byte 0x007080a3
	.4byte 0x008070a3
	.4byte 0x009040a7
	.4byte 0x000000a4
	.4byte 0x001010a5
	.4byte 0x002020a5
	.4byte 0x003030a5
	.2byte 0x50a4
.L_020045ea:
	lsls	r0, r0, #1
	lsls	r4, r4
	lsls	r0, r2, #1
	str	r3, [r4, #8]
	lsls	r0, r4, #1
	stmia	r0!, {r0, r2, r5, r7}
	lsls	r0, r6, #1
	lsls	r5, r4, #2
	movs	r0, r0
.L_020045fc:
	asrs	r4, r4, #2
	movs	r0, r2
	movs	r0, #164
	movs	r0, r4
	adds	r0, #164
	movs	r0, r6
	str	r5, [r4, r2]
	lsls	r0, r0, #1
	lsls	r5, r4
.L_0200460e:
	lsls	r0, r2, #1
	strb	r5, [r4, #2]
	lsls	r0, r4, #1
	str	r5, [r4, #8]
	lsls	r0, r6, #1
.L_02004618:
	str	r0, [sp, #660]
	lsls	r0, r0, #2
	strh	r5, [r4, #4]
	lsls	r0, r2, #2
	sub	sp, #148
	lsls	r0, r4, #2
	add	r0, pc, #660
	lsls	r0, r6, #2
	strb	r4, [r4, #2]
	lsls	r0, r0, #3
	asrs	r7, r3, #2
	lsls	r0, r2, #3
	lsls	r6, r4, #2
	movs	r0, r0
	asrs	r1, r4, #2
	movs	r0, r2
	adds	r0, #167
.L_0200463a:
	movs	r0, r4
	movs	r0, #167
	movs	r0, r6
	strb	r2, [r4, #2]
	lsls	r0, r0, #1
	lsls	r7, r4, #2
	movs	r0, r0
	asrs	r0, r4, #2
	movs	r0, r2
	adds	r0, #166
	movs	r0, r4
	movs	r0, #166
	movs	r0, r6
	str	r0, [sp, #652]
.L_02004656:
	lsls	r0, r0, #1
	lsls	r1, r4
	lsls	r0, r2, #1
	lsls	r0, r5
	lsls	r0, r4, #1
	adds	r0, #168
	lsls	r0, r6, #1
	asrs	r0, r5, #2
	lsls	r0, r0, #2
	lsls	r0, r5, #2
	movs	r0, r0
	strh	r7, [r4, #4]
	movs	r0, r2
	adds	r0, #163
	movs	r0, r4
	strb	r7, [r4, #2]
	movs	r0, r6
.L_02004678:
	str	r7, [r4, #8]
	lsls	r0, r0, #1
	lsls	r1, r5, #2
	movs	r0, r0
	adds	r0, #162
	movs	r0, r2
	lsls	r7, r7, #7
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
.L_0200468c:
	.2byte 0x0000
.L_0200468e:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02004694:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_0200469e:
	.2byte 0x0000
	lsls	r4, r7, #5
	.2byte 0xffff
	.2byte 0x0001
.L_020046a6:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #12
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_020046b2:
	lsls	r7, r4, #10
	ands	r0, r0
	movs	r2, r0
	lsls	r4, r7, #5
.L_020046ba:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
.L_020046c0:
	movs	r0, r0
	lsls	r0, r3, #4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r7, r4, #8
	ands	r0, r0
	movs	r2, r0
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r3, #2
	movs	r0, r0
	movs	r0, r0
.L_020046e0:
	movs	r0, r0
	lsls	r0, r7, #3
	ands	r0, r0
	movs	r2, r0
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
.L_020046f2:
	lsls	r0, r3, #12
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_020046fa:
	lsls	r0, r3, #3
	ands	r0, r0
.L_020046fe:
	movs	r2, r0
	lsls	r4, r4, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #12
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #11
.L_02004714:
	ands	r0, r0
	movs	r2, r0
	lsls	r2, r4, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #10
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_0200472a:
	lsls	r0, r1, #11
	ands	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02004740:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r4, r7, #5
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #12
.L_02004754:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r7, r6, #3
	ands	r0, r0
	movs	r2, r0
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #9
	movs	r0, r0
.L_0200476e:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r3, #2
	ands	r0, r0
	movs	r2, r0
.L_02004778:
	lsls	r4, r6, #3
.L_0200477a:
	lsls	r0, r2, #1
	movs	r1, r0
	movs	r0, r0
	movs	r0, r0
.L_02004782:
	lsls	r0, r1, #12
.L_02004784:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #4
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020047a2:
	.2byte 0x0000
.L_020047a4:
	.2byte 0x0000
	.2byte 0x0000
	lsls	r7, r7, #5
.L_020047aa:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #4
	ands	r0, r0
	movs	r2, r0
.L_020047c0:
	lsls	r1, r5, #7
.L_020047c2:
	.2byte 0xffff
	.2byte 0x0001
.L_020047c6:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r4, #10
.L_020047d4:
	ands	r0, r0
	movs	r2, r0
	lsls	r1, r5, #7
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #4
	movs	r0, r0
.L_020047e6:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r4, #10
	ands	r0, r0
.L_020047ee:
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r7, r7, #5
	.2byte 0xffff
	.2byte 0x0001
.L_0200480e:
	movs	r0, r0
	movs	r0, r0
.L_02004812:
	lsls	r0, r1, #3
	movs	r0, r0
.L_02004816:
	movs	r0, r0
	movs	r0, r0
.L_0200481a:
	lsls	r0, r5, #10
	ands	r0, r0
	movs	r2, r0
	lsls	r4, r7, #5
	.2byte 0xffff
	.2byte 0x0001
.L_02004826:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #4
	movs	r0, r0
.L_0200482e:
	movs	r0, r0
	movs	r0, r0
	lsls	r7, r0, #10
	ands	r0, r0
	movs	r2, r0
	lsls	r4, r7, #5
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #3
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r7, r2, #1
	ands	r0, r0
	movs	r2, r0
	lsls	r4, r7, #5
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r3, #7
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r7, r4, #1
.L_02004864:
	ands	r0, r0
	movs	r2, r0
.L_02004868:
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
.L_02004872:
	lsls	r0, r1, #4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_0200487a:
	lsls	r0, r7, #10
	ands	r0, r0
	movs	r2, r0
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #3
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #12
	ands	r0, r0
	lsls	r2, r0, #4
.L_02004898:
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
.L_020048a0:
	movs	r0, r0
	lsls	r0, r3, #4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #12
	ands	r0, r0
	lsls	r2, r0, #4
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020048b8:
	.2byte 0x0000
	.2byte 0x0000
.L_020048bc:
	.2byte 0x0000
	.2byte 0x0000
.L_020048c0:
	.2byte 0x0000
	.2byte 0x0000
.L_020048c4:
	.2byte 0x0000
.L_020048c6:
	.2byte 0x0000
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_020048da:
	lsls	r0, r5, #5
	ands	r0, r0
	movs	r2, r0
	lsls	r7, r7, #5
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
.L_020048e8:
	movs	r0, r0
	lsls	r0, r7, #11
	movs	r0, r0
	movs	r0, r0
.L_020048f0:
	movs	r0, r0
	lsls	r0, r5, #8
	ands	r0, r0
	movs	r2, r0
.L_020048f8:
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #12
	movs	r0, r0
	movs	r0, r0
.L_02004920:
	movs	r0, r0
	lsls	r0, r1, #10
	ands	r0, r0
	movs	r2, r0
	lsls	r4, r7, #5
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r3, #12
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r7, r6, #9
	ands	r0, r0
.L_0200493e:
	movs	r2, r0
	lsls	r0, r2, #4
.L_02004942:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
.L_02004948:
	movs	r0, r0
	lsls	r0, r1, #2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #8
	ands	r0, r0
	movs	r2, r0
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #3
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #8
	ands	r0, r0
	movs	r2, r0
	lsls	r1, r5, #7
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #2
	ands	r0, r0
	movs	r2, r0
	lsls	r2, r4, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r3, #11
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #10
	ands	r0, r0
	movs	r2, r0
	lsls	r2, r4, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r3, #11
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_020049b2:
	lsls	r0, r3, #11
	ands	r0, r0
	movs	r2, r0
	lsls	r2, r4, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #12
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #11
	ands	r0, r0
	movs	r2, r0
.L_020049d0:
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020049da:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020049e2:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r7, r2, #6
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r3, #3
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #5
.L_02004a14:
	.2byte 0xc000
	movs	r2, r0
	lsls	r7, r2, #6
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r3, #6
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #5
	.2byte 0xc000
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02004a3a:
	.2byte 0x0000
.L_02004a3c:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02004a42:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r7, r7, #5
	.2byte 0xffff
	.2byte 0x0001
.L_02004a4e:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #9
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #4
	ands	r0, r0
	movs	r2, r0
	lsls	r4, r7, #5
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #11
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r7, r4, #3
	ands	r0, r0
	movs	r2, r0
	lsls	r0, r7, #4
.L_02004a7a:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #11
	movs	r0, r0
	movs	r0, r0
.L_02004a88:
	movs	r0, r0
	lsls	r0, r5, #4
	ands	r0, r0
	movs	r2, r0
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #9
.L_02004a9c:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #5
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
.L_02004aae:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #10
	movs	r0, r0
	movs	r0, r0
.L_02004ab8:
	movs	r0, r0
	lsls	r0, r5, #5
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
.L_02004ac2:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #11
.L_02004acc:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #5
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
.L_02004ade:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #11
	movs	r0, r0
.L_02004ae6:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #5
.L_02004aec:
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #12
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #5
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #12
	movs	r0, r0
	movs	r0, r0
.L_02004b18:
	movs	r0, r0
	lsls	r0, r5, #5
	ands	r0, r0
	lsls	r2, r0, #4
.L_02004b20:
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #10
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #6
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #12
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #6
	ands	r0, r0
.L_02004b4e:
	lsls	r2, r0, #4
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #5
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #9
	ands	r0, r0
	movs	r2, r0
	lsls	r0, r2, #4
.L_02004b6a:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #5
	movs	r0, r0
.L_02004b76:
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #9
	ands	r0, r0
	lsls	r2, r0, #4
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #3
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #3
	ands	r0, r0
.L_02004bae:
	movs	r2, r0
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #5
.L_02004bbc:
	movs	r0, r0
	movs	r0, r0
.L_02004bc0:
	movs	r0, r0
	lsls	r0, r7, #3
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #3
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #4
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #3
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #5
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
.L_02004c02:
	lsls	r0, r5, #4
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #5
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r0, r7, #4
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r1, #5
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #4
	ands	r0, r0
	lsls	r2, r0, #4
	lsls	r3, r0, #5
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	lsls	r4, r4, #1
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02004c3a:
	movs	r0, r0
	ands	r0, r0
.L_02004c3e:
	lsls	r2, r0, #8
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
.L_02004c54:
	ands	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02004c68:
	.2byte 0x0000
	.2byte 0x0000
.L_02004c6c:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r4
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02004c8c:
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02004ca4:
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r1, r1
.L_02004ce2:
	.2byte 0xffff
	.2byte 0x0009
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x000a
	movs	r0, r0
	movs	r1, r0
.L_02004cf6:
	movs	r0, r0
	movs	r3, r1
.L_02004cfa:
	.2byte 0xffff
	.2byte 0x000b
	movs	r0, r0
	stmia	r6!, {r1}
	movs	r0, r0
	movs	r2, r6
	.2byte 0xffff
	.2byte 0x84b5
	lsls	r0, r0, #8
	strh	r2, [r0, #48]
	movs	r0, r0
	movs	r2, r6
.L_02004d12:
	.2byte 0xffff
	.2byte 0x84b5
	lsls	r0, r0, #8
	.2byte 0x4602
	movs	r0, r0
.L_02004d1c:
	movs	r3, r6
	.2byte 0xffff
	.2byte 0x84b5
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r2, r1
	lsrs	r2, r3, #5
	strh	r5, [r5, #32]
.L_02004d2e:
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r3, r1
	lsrs	r3, r3, #5
	strh	r5, [r5, #32]
	lsls	r0, r0, #8
	movs	r1, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x842d
	lsls	r0, r0, #8
	movs	r1, #21
	movs	r0, r0
	movs	r0, r1
	lsls	r0, r6, #8
	strh	r5, [r7, #36]
	lsls	r0, r0, #8
	movs	r1, #21
.L_02004d56:
	movs	r0, r0
	movs	r1, r1
	lsls	r1, r6, #8
	strh	r5, [r6, #42]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
.L_02004d64:
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x85f5
	lsls	r0, r0, #8
	adds	r5, r2, r0
	movs	r0, r0
	movs	r5, r1
	lsls	r0, r2, #9
	strh	r1, [r7, #46]
.L_02004d76:
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
.L_02004d7c:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r0, r0, #4
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
.L_02004d92:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	lsls	r1, r2, #1
.L_02004da2:
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
.L_02004dac:
	movs	r1, r4
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
.L_02004dc2:
	movs	r0, r0
.L_02004dc4:
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0006
.L_02004dce:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x0009
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	movs	r0, r5
	.2byte 0xffff
	.2byte 0xaca5
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x833d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r1, r1
	lsrs	r4, r3, #5
	strh	r5, [r0, #50]
.L_02004e16:
	lsls	r0, r0, #8
	movs	r1, r1
	movs	r0, r0
	movs	r0, r0
.L_02004e1e:
	.2byte 0xffff
	.2byte 0x8645
	lsls	r0, r0, #8
	ldrh	r5, [r0, #16]
	str	r0, [r0, r0]
	lsls	r0, r3, #1
	lsls	r0, r0, #9
	strh	r1, [r2, #52]
	lsls	r0, r0, #8
	movs	r6, r0
	movs	r0, r0
	lsls	r0, r3, #1
	lsls	r0, r0, #9
	strh	r5, [r3, #56]
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
.L_02004e42:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	lsrs	r2, r0, #8
	movs	r0, r0
	movs	r3, r0
.L_02004e66:
	.2byte 0xffff
	.2byte 0x8959
	lsls	r0, r0, #8
.L_02004e6c:
	movs	r1, r0
	movs	r0, r0
	movs	r4, r0
.L_02004e72:
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
	movs	r1, r0
.L_02004e86:
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
	stmia	r4!, {r1}
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0xaf61
	lsls	r0, r0, #8
	movs	r1, r0
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x8a25
.L_02004eb2:
	lsls	r0, r0, #8
	ldmia	r0!, {r2, r4}
	movs	r1, r0
	movs	r1, r1
	lsrs	r5, r3, #5
	strh	r5, [r2, #62]
	lsls	r0, r0, #8
	movs	r1, #21
	movs	r0, r0
	movs	r0, r1
	lsrs	r6, r4, #9
	ldrh	r1, [r3, #20]
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
.L_02004ed4:
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r0
.L_02004eda:
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
.L_02004ee8:
	movs	r2, r0
.L_02004eea:
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r1, r4
.L_02004ef2:
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
.L_02004efc:
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
.L_02004f14:
	movs	r1, r0
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x0009
	movs	r0, r0
	lsls	r2, r0, #8
	movs	r0, r0
	movs	r7, r6
	.2byte 0xffff
	.2byte 0x84b5
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x8bad
	lsls	r0, r0, #8
	movs	r0, r1
.L_02004f5e:
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8bad
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x8b85
	lsls	r0, r0, #8
	movs	r1, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8b85
	lsls	r0, r0, #8
	ldrh	r5, [r0, #16]
	str	r0, [r0, r0]
	movs	r2, r6
	lsrs	r2, r5, #9
	ldrh	r1, [r3, #22]
	lsls	r0, r0, #8
	movs	r1, #21
.L_02004f8e:
	movs	r0, r0
	movs	r0, r1
	lsrs	r4, r4, #9
	ldrh	r5, [r2, #30]
	lsls	r0, r0, #8
	movs	r1, #21
	movs	r0, r0
	movs	r1, r1
	lsls	r1, r1, #9
	ldrh	r1, [r5, #32]
.L_02004fa2:
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
.L_02004fbc:
	lsls	r1, r2, #1
.L_02004fbe:
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
.L_02004fc6:
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
.L_02004fe0:
	movs	r1, r0
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
	movs	r1, r0
.L_02004ffa:
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02005008:
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x0009
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x000a
	movs	r0, r0
	movs	r1, r0
.L_0200502a:
	movs	r0, r0
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x000b
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	lsls	r5, r1, #1
.L_0200503a:
	.2byte 0xffff
	.2byte 0x8cc9
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	lsls	r6, r1, #1
.L_02005046:
	.2byte 0xffff
	.2byte 0x8cc9
	lsls	r0, r0, #8
	.2byte 0x4602
	movs	r0, r0
	lsls	r6, r0, #1
	.2byte 0xffff
	.2byte 0x84b5
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0000
.L_02005062:
	movs	r0, r0
.L_02005064:
	movs	r1, #21
	movs	r0, r0
	movs	r1, r1
	lsrs	r7, r4, #9
	ldrh	r5, [r4, #46]
.L_0200506e:
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r0
.L_0200507e:
	movs	r0, r0
.L_02005080:
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
.L_020050c4:
	movs	r1, r0
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x0009
	movs	r0, r0
.L_020050e8:
	movs	r1, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x000a
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r3, r1
.L_020050fa:
	.2byte 0xffff
	.2byte 0x000b
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
.L_02005104:
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x000c
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x8e19
.L_02005116:
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	lsls	r5, r1, #1
	.2byte 0xffff
	.2byte 0x8cc9
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	lsls	r6, r1, #1
	.2byte 0xffff
	.2byte 0x8cc9
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r0, r1
	lsls	r0, r7, #8
	ldrh	r5, [r1, #54]
.L_0200513a:
	lsls	r0, r0, #8
	adds	r5, r2, r0
	movs	r0, r0
	movs	r5, r1
	lsls	r1, r2, #9
	str	r0, [sp, #116]
	lsls	r0, r0, #8
	adds	r5, r2, r0
.L_0200514a:
	movs	r0, r0
	movs	r6, r1
	lsls	r2, r2, #9
	str	r0, [sp, #116]
	lsls	r0, r0, #8
	adds	r5, r2, r0
	movs	r0, r0
	movs	r7, r1
	lsls	r3, r2, #9
	str	r0, [sp, #116]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
.L_02005164:
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x9061
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x9061
	lsls	r0, r0, #8
.L_02005178:
	movs	r0, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x9061
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
.L_02005186:
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x907d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x907d
	lsls	r0, r0, #8
	movs	r1, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x907d
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r0
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r4
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r1, r4
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r3, r0
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x90ed
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r6
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r4
	movs	r0, r0
	movs	r4, r0
.L_02005226:
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	lsls	r1, r2, #1
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	movs	r0, r5
	.2byte 0xffff
	.2byte 0xaca5
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r3, r2
	.2byte 0xffff
	.2byte 0x9139
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x0000
	movs	r0, r0
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x0000
	movs	r0, r0
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r7, r1
	.2byte 0xffff
	.2byte 0x0000
	movs	r0, r0
	movs	r1, #21
	movs	r0, r0
	movs	r0, r1
	lsrs	r5, r4, #9
	str	r1, [sp, #452]
	lsls	r0, r0, #8
	movs	r1, #21
	movs	r0, r0
	movs	r1, r1
	lsls	r3, r1, #9
	str	r1, [sp, #852]
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r0
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	stmia	r4!, {r0, r1}
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x9a1d
	lsls	r0, r0, #8
	.2byte 0x4403
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x9c69
	lsls	r0, r0, #8
	strh	r3, [r0, #32]
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x9cd9
	lsls	r0, r0, #8
	lsls	r3, r0, #16
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x9d41
	lsls	r0, r0, #8
	ldrh	r5, [r0, #16]
	str	r0, [r0, r0]
	movs	r0, r5
	asrs	r1, r1, #13
	str	r3, [sp, #164]
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsrs	r0, r1, #32
	lsrs	r0, r0, #32
	lsrs	r7, r7, #3
	lsrs	r1, r0, #32
	movs	r0, r1
	.2byte 0xff08
	.2byte 0x0801
	lsrs	r0, r1, #32
.L_0200535c:
	lsrs	r0, r1, #32
	lsls	r0, r0, #4
	.2byte 0xff08
	.2byte 0x0808
	movs	r0, r1
	lsrs	r0, r1, #32
	lsls	r0, r1, #4
	lsrs	r7, r7, #3
