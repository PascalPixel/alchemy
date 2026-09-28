.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8894
	.2byte 0x0200
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs	r0, #0
	bx	lr
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88c4
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88c8
	.2byte 0x0200
	.global Func_02000054
	.thumb_func
Func_02000054:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88e0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #40]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_02000084
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #32]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r3, r3, #16
	str	r3, [r0, #12]
	ldr	r3, [pc, #28]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r3, r3, #16
	str	r3, [r0, #16]
	bl 0x0200883c
.L_02000084:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02008a34
	.4byte 0x02008a38
	.4byte 0x02008a00
	.2byte 0x8a2c
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #128
	lsls	r0, r0, #4
	sub	sp, #4
	bl 0x0200879c
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	mov	r8, r0
	adds	r3, #212
	ldr	r0, [pc, #120]
	ldr	r1, [pc, #124]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #120]
	ldr	r6, [pc, #120]
	bl 0x020087d4
	mov	r1, r8
	bl 0x020087ac
	ldr	r5, [pc, #112]
	bl 0x020087bc
	movs	r1, #128
	mov	r2, r8
	str	r0, [r5, #0]
	lsls	r1, r1, #4
	ldr	r5, [pc, #104]
	bl 0x020087b4
	movs	r3, #192
	str	r0, [r5, #0]
	movs	r1, #0
	str	r0, [sp, #0]
	movs	r2, #0
	adds	r0, r6, #0
	lsls	r3, r3, #24
	bl 0x02008834
	movs	r3, #240
	strh	r3, [r6, #30]
	ldrb	r3, [r6, #9]
	movs	r2, #13
	ldrb	r1, [r6, #5]
	negs	r2, r2
	ands	r2, r3
	movs	r3, #33
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r2, r3
	movs	r3, #224
	orrs	r2, r3
	strb	r2, [r6, #9]
	mov	r0, r8
	bl 0x020087a4
	ldr	r5, [pc, #16]
	ldr	r3, [pc, #44]
	movs	r1, #144
	strb	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x02008794
	add	sp, #4
	b.n	.L_02000150
	.4byte 0x00000000
	.4byte 0x02008874
	.4byte 0x050003c0
	.4byte 0x000001f8
	.4byte 0x02008a00
	.4byte 0x020088f0
	.4byte 0x02008a30
	.4byte 0x02008a34
	.2byte 0x805d
	.2byte 0x0200
.L_02000150:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r2, #0
	movs	r2, #128
	lsls	r2, r2, #8
	mov	r8, r2
	adds	r7, r0, #0
	mov	sl, r3
	mov	r3, r8
	ands	r3, r7
	mov	r8, r3
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r7, r3
	adds	r5, r1, #0
	adds	r0, r7, #0
	movs	r1, #0
	mov	r9, r1
	bl 0x0200882c
	adds	r3, r0, #0
	movs	r2, #4
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [pc, #160]
	adds	r1, r5, #0
	ldr	r0, [r2, #0]
	adds	r2, r6, #0
	bl 0x020087f4
	mov	r1, r8
	adds	r5, r0, #0
	cmp	r1, #0
	bne.n	.L_020001c8
	movs	r1, #0
	mov	r2, sl
	ldr	r3, [sp, #28]
	adds	r0, r7, #0
	bl 0x02008804
	ldr	r2, [pc, #132]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #132]
	mov	r1, sl
	lsls	r3, r1, #3
	strh	r3, [r2, #0]
	ldr	r1, [sp, #28]
	ldr	r2, [pc, #124]
	lsls	r3, r1, #3
	strh	r3, [r2, #0]
	mov	r9, r0
.L_020001c8:
	ldr	r2, [sp, #36]
	cmp	r2, #2
	bne.n	.L_020001e2
	ldrh	r3, [r5, #14]
	movs	r2, #0
	adds	r3, #1
	strh	r3, [r5, #14]
	strh	r2, [r5, #8]
	strh	r2, [r5, #10]
	b.n	.L_020001e2
.L_020001dc:
	movs	r0, #1
	bl 0x0200878c
.L_020001e2:
	bl 0x020087fc
	cmp	r0, #0
	beq.n	.L_020001dc
	movs	r0, #1
	bl 0x0200878c
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x020087ec
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_0200020c
	movs	r1, #2
	mov	r0, r9
	bl 0x020087ec
	ldr	r3, [pc, #48]
	mov	r1, r8
	strb	r1, [r3, #0]
.L_0200020c:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	ble.n	.L_02000220
	adds	r5, r2, #0
.L_02000214:
	movs	r0, #1
	subs	r5, #1
	bl 0x0200878c
	cmp	r5, #0
	bne.n	.L_02000214
.L_02000220:
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02008a28
	.4byte 0x02008a34
	.4byte 0x02008a38
	.2byte 0x8a2c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #28
	movs	r4, #0
	movs	r3, #0
	str	r4, [sp, #24]
	str	r4, [sp, #20]
	mov	r9, r3
	ldr	r3, [pc, #184]
	movs	r5, #128
	lsls	r5, r5, #8
	add	r4, sp, #12
	mov	r8, r0
	adds	r6, r1, #0
	mov	sl, r2
	ands	r5, r0
	add	r2, sp, #20
	ldr	r0, [r3, #0]
	add	r1, sp, #24
	add	r3, sp, #16
	str	r4, [sp, #0]
	bl 0x0200880c
	ldr	r2, [sp, #16]
	movs	r7, #0
	cmp	r2, #24
	bgt.n	.L_020002a4
	cmp	r5, #0
	bne.n	.L_02000292
	movs	r3, #25
	subs	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r3, #5
	b.n	.L_0200029c
.L_02000292:
	movs	r3, #30
	subs	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
.L_0200029c:
	str	r3, [sp, #24]
	ldr	r3, [sp, #24]
	subs	r4, r3, #5
	b.n	.L_020002b2
.L_020002a4:
	movs	r3, #30
	subs	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #24]
	adds	r4, r3, #0
.L_020002b2:
	ldr	r3, [sp, #12]
	subs	r3, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r2, r3, #1
	cmp	r6, #0
	bne.n	.L_020002c8
	movs	r3, #3
	str	r3, [sp, #20]
	adds	r7, r2, #1
	b.n	.L_020002f2
.L_020002c8:
	cmp	r6, #1
	bne.n	.L_020002d8
	movs	r3, #14
	subs	r3, r3, r2
	str	r3, [sp, #20]
	adds	r3, r3, r2
	subs	r7, r3, #2
	b.n	.L_020002f2
.L_020002d8:
	cmp	r6, #2
	bne.n	.L_020002f2
	cmp	r2, #3
	bne.n	.L_020002ec
	movs	r3, #13
	str	r3, [sp, #20]
	movs	r3, #2
	movs	r7, #15
	mov	r9, r3
	b.n	.L_020002f2
.L_020002ec:
	movs	r3, #16
	str	r3, [sp, #20]
	movs	r7, #15
.L_020002f2:
	mov	r3, sl
	str	r3, [sp, #4]
	mov	r3, r9
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	str	r3, [sp, #8]
	mov	r0, r8
	adds	r3, r4, #0
	str	r7, [sp, #0]
	bl 0x02008158
	add	sp, #28
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x8a28
	.2byte 0x0200
	.global Func_02000318
	.thumb_func
Func_02000318:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #840]
	movs	r2, #139
	lsls	r2, r2, #2
	adds	r6, r3, r2
	subs	r2, #24
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldrb	r7, [r6, #0]
	bl 0x0200881c
	movs	r3, #0
	mov	r8, r3
	mov	r2, r8
	movs	r5, #1
	adds	r0, #85
	strb	r2, [r0, #0]
	strb	r5, [r6, #0]
	bl 0x02008844
	bl 0x020087e4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #140
	strb	r5, [r3, #6]
	adds	r3, r3, r2
	strb	r5, [r3, #0]
	ldr	r2, [pc, #784]
	ldr	r3, [pc, #784]
	str	r3, [r2, #0]
	bl 0x02008098
	movs	r0, #30
	bl 0x02008814
	movs	r0, #5
	movs	r1, #0
	movs	r2, #40
	bl 0x02008244
	movs	r0, #1
	movs	r1, #1
	movs	r2, #40
	bl 0x02008244
	movs	r0, #6
	movs	r1, #0
	movs	r2, #40
	bl 0x02008244
	movs	r0, #2
	movs	r1, #1
	movs	r2, #40
	bl 0x02008244
	movs	r0, #3
	movs	r1, #0
	movs	r2, #30
	bl 0x02008244
	movs	r0, #7
	movs	r1, #1
	movs	r2, #30
	bl 0x02008244
	movs	r0, #56
	movs	r1, #0
	movs	r2, #40
	bl 0x02008244
	movs	r0, #60
	movs	r1, #1
	movs	r2, #30
	bl 0x02008244
	movs	r0, #61
	movs	r1, #1
	movs	r2, #40
	bl 0x02008244
	movs	r0, #0
	movs	r1, #0
	movs	r2, #40
	bl 0x02008244
	movs	r0, #62
	movs	r1, #1
	movs	r2, #30
	bl 0x02008244
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	bl 0x02008244
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	bl 0x02008244
	movs	r0, #5
	movs	r1, #1
	movs	r2, #40
	bl 0x02008244
	movs	r0, #4
	movs	r1, #0
.L_020003fa:
	movs	r2, #30
	bl 0x02008244
	movs	r1, #1
	movs	r2, #30
	movs	r0, #1
	bl 0x02008244
	movs	r0, #67
	bl 0x0200886c
	ldr	r5, [pc, #608]
	movs	r3, #16
	strh	r3, [r5, #14]
	strh	r3, [r5, #10]
	ldr	r1, [pc, #604]
	movs	r2, #0
	ldr	r0, [pc, #604]
	bl 0x0200884c
	movs	r0, #100
	bl 0x02008814
	movs	r0, #60
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #61
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #0
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #62
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #1
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r1, #2
	movs	r2, #40
.L_0200045e:
	movs	r0, #5
	bl 0x02008244
	movs	r0, #120
	bl 0x02008854
	movs	r0, #1
	bl 0x0200878c
	bl 0x0200885c
	movs	r0, #30
	bl 0x02008814
	ldr	r1, [pc, #508]
	movs	r2, #120
	ldr	r0, [pc, #512]
	bl 0x0200884c
	movs	r0, #120
	bl 0x02008814
	movs	r0, #5
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #7
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #56
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #4
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #4
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #4
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #4
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #4
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #0
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #62
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #1
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #1
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #6
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #5
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #3
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r1, #2
	movs	r2, #30
	movs	r0, #1
	bl 0x02008244
	movs	r0, #78
	bl 0x0200886c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #64
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #0
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #65
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #0
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #69
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #1
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	ldr	r1, [pc, #252]
	movs	r2, #0
	ldr	r0, [pc, #260]
	bl 0x0200884c
	movs	r0, #60
	bl 0x02008814
	movs	r0, #73
	bl 0x0200886c
	movs	r0, #30
	bl 0x02008814
	movs	r0, #64
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #65
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #67
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #62
	movs	r1, #2
.L_020005b6:
	movs	r2, #30
	bl 0x02008244
	movs	r0, #70
	movs	r1, #2
.L_020005c0:
	movs	r2, #30
	bl 0x02008244
	movs	r0, #68
	movs	r1, #2
.L_020005ca:
	movs	r2, #30
	bl 0x02008244
	movs	r0, #56
	movs	r1, #2
.L_020005d4:
	movs	r2, #30
	bl 0x02008244
	movs	r0, #1
	movs	r1, #2
.L_020005de:
	movs	r2, #40
	bl 0x02008244
	movs	r1, #2
	movs	r2, #40
.L_020005e8:
	movs	r0, #56
	bl 0x02008244
	movs	r0, #200
	bl 0x02008854
	movs	r0, #1
	bl 0x0200878c
	bl 0x0200885c
	movs	r0, #30
	bl 0x02008814
	movs	r0, #62
.L_02000606:
	movs	r1, #2
	movs	r2, #20
	bl 0x02008244
	movs	r0, #0
.L_02000610:
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	movs	r0, #66
.L_0200061a:
	movs	r1, #2
	movs	r2, #20
	bl 0x02008244
	movs	r0, #62
.L_02000624:
	movs	r1, #2
	movs	r2, #30
	bl 0x02008244
	movs	r0, #66
.L_0200062e:
	movs	r1, #2
	movs	r2, #40
	bl 0x02008244
	ldr	r1, [pc, #64]
.L_02000638:
	movs	r2, #0
	movs	r0, #0
	bl 0x0200884c
	movs	r0, #60
.L_02000642:
	bl 0x02008814
	mov	r3, r8
	mov	r2, r8
	strh	r3, [r5, #14]
.L_0200064c:
	strh	r2, [r5, #10]
	movs	r0, #0
	bl 0x02008864
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #32]
	movs	r2, #0
	bl 0x0200884c
	movs	r0, #60
	bl 0x02008814
	b.n	.L_02000692
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008a28
	.4byte 0x0000306e
	.4byte 0x03001120
	.4byte 0x02010000
	.4byte 0x00000079
	.4byte 0x0000007a
	.4byte 0x0000007b
	.2byte 0x007d
	.2byte 0x0000
.L_0200068c:
	movs	r0, #1
	bl 0x0200878c
.L_02000692:
	ldr	r3, [pc, #212]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq.n	.L_0200068c
	ldr	r1, [pc, #208]
.L_0200069c:
	movs	r2, #0
	ldr	r0, [pc, #208]
	bl 0x0200884c
	movs	r0, #120
.L_020006a6:
	bl 0x02008814
	movs	r0, #120
	bl 0x02008854
	movs	r0, #1
	bl 0x0200878c
	bl 0x0200885c
	movs	r0, #180
	bl 0x02008814
	ldr	r1, [pc, #168]
	movs	r2, #0
	ldr	r0, [pc, #172]
	bl 0x0200884c
	movs	r0, #150
	lsls	r0, r0, #1
	bl 0x02008814
	ldr	r1, [pc, #152]
	movs	r2, #0
.L_020006d6:
	ldr	r0, [pc, #160]
	bl 0x0200884c
	movs	r0, #180
	bl 0x02008814
	movs	r0, #78
	bl 0x0200886c
	ldr	r3, [pc, #144]
.L_020006ea:
	movs	r2, #139
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #150
	strb	r7, [r3, #0]
.L_020006f4:
	lsls	r0, r0, #1
	bl 0x02008814
	ldr	r3, [pc, #108]
	movs	r5, #0
.L_020006fe:
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_0200071e
.L_02000704:
	movs	r0, #1
	bl 0x0200878c
	movs	r3, #168
	lsls	r3, r3, #6
	adds	r5, #1
	adds	r3, #47
.L_02000712:
	cmp	r5, r3
	bgt.n	.L_0200071e
	ldr	r3, [pc, #80]
	ldr	r3, [r3, #4]
	cmp	r3, #0
.L_0200071c:
	beq.n	.L_02000704
.L_0200071e:
	ldr	r1, [pc, #76]
	movs	r2, #0
	movs	r0, #0
	bl 0x0200884c
	movs	r0, #60
	bl 0x02008814
	movs	r2, #0
.L_02000730:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #130
.L_0200073a:
	lsls	r2, r2, #5
	subs	r3, #80
	strh	r2, [r3, #0]
	bl 0x020087cc
.L_02000744:
	bl 0x020087c4
	ldr	r2, [pc, #52]
	movs	r3, #1
	movs	r0, #190
.L_0200074e:
	strb	r3, [r2, #0]
	lsls	r0, r0, #1
	bl 0x020087dc
	ldr	r0, [pc, #44]
.L_02000758:
	movs	r1, #2
	bl 0x02008824
	movs	r0, #0
	pop	{r3}
.L_02000762:
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x03001150
	.4byte 0x02010000
	.4byte 0x0000007e
	.4byte 0x0000007f
	.4byte 0x00000080
	.4byte 0x02000240
	.4byte 0x0300120c
	.2byte 0x0001
	.2byte 0x0000
	.global Func_02000788
	.thumb_func
Func_02000788:
	movs	r0, #0
.L_0200078a:
	bx	lr
	.section .rodata,"a",%progbits
	.4byte 0x7fff44e0
	.4byte 0x0000318c
	.4byte 0x01400180
	.4byte 0x00c00100
	.4byte 0x020001c0
	.4byte 0x294a0240
	.4byte 0x001f5294
	.4byte 0x7c0003ff
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
