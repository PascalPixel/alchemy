.syntax unified
	.thumb
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	sub	sp, #4
	cmp	r3, r2
	beq.n	.L_0200009c
	adds	r7, r0, #0
.L_0200004e:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200de2c
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #89
	movs	r2, #2
	ldrsh	r6, [r7, r2]
	ldrb	r2, [r1, #0]
	movs	r3, #4
	ldrsh	r4, [r7, r3]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	str	r4, [sp, #0]
	bl 0x0200ddbc
	ldr	r4, [sp, #0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	lsrs	r4, r4, #16
	lsrs	r6, r6, #16
	adds	r5, #34
	ldrb	r3, [r5, #0]
	adds	r2, r4, #0
	mov	r0, r8
	adds	r1, r6, #0
	adds	r7, #6
	bl 0x02008128
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200004e
.L_0200009c:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r5, r0, #0
	mov	r9, r1
	mov	sl, r2
	movs	r1, #255
	ldr	r2, [r3, #0]
	b.n	.L_02000110
.L_020000c0:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_0200010c
	adds	r0, r7, #0
	bl 0x0200de2c
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_020000e8
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_020000e8:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_0200011a
	adds	r0, r5, #0
	bl 0x0200dd4c
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl 0x02008128
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_0200011a
.L_0200010c:
	adds	r5, #6
	movs	r1, #255
.L_02000110:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_020000c0
.L_0200011a:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	mov	r8, r2
	adds	r6, r1, #0
	bl 0x0200de2c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r5, [r2, r3]
	adds	r7, r0, #0
	bl 0x0200df64
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_0200019c
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_02000188
	cmp	r6, #1
	bcc.n	.L_0200017e
	cmp	r6, #2
	beq.n	.L_02000192
	b.n	.L_020001ca
.L_0200017e:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200dd5c
	b.n	.L_020001ca
.L_02000188:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200dd5c
	b.n	.L_020001ca
.L_02000192:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x0200dd5c
	b.n	.L_020001ca
.L_0200019c:
	movs	r3, #255
	strb	r3, [r5, #2]
	cmp	r6, #1
	beq.n	.L_020001b8
	cmp	r6, #1
	bcc.n	.L_020001ae
	cmp	r6, #2
	beq.n	.L_020001c2
	b.n	.L_020001ca
.L_020001ae:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200dd5c
.L_020001b6:
	b.n	.L_020001ca
.L_020001b8:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200dd5c
	b.n	.L_020001ca
.L_020001c2:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x0200dd5c
.L_020001ca:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02000214
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000214
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
.L_02000214:
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
	bl 0x0200dd74
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000262
	movs	r1, #0
	bl 0x020081d0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200de9c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddc4
	adds	r0, r5, #0
	b.n	.L_02000264
.L_02000262:
	movs	r0, #0
.L_02000264:
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
	bl 0x0200dd74
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020002b6
	movs	r1, #1
	bl 0x020081d0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x0200de9c
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #34
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	b.n	.L_020002b8
.L_020002b6:
	movs	r0, #0
.L_020002b8:
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
	bl 0x0200de2c
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, sl
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_0200033c
	cmp	r7, #0
	beq.n	.L_0200033c
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000344
.L_0200033c:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000344:
	mov	r3, r8
	bl 0x0200dd74
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000352
	b.n	.L_020004b6
.L_02000352:
	ldr	r3, [r6, #80]
	mov	r1, sl
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	mov	r8, r3
	bl 0x0200dd5c
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200dd6c
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200ddbc
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
	bl 0x020081d0
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
	beq.n	.L_020004b6
	cmp	r7, #0
	beq.n	.L_020004b6
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020003d4
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200de9c
.L_020003d4:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200040c
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
.L_02000406:
	ldr	r1, [r7, #0]
	bl 0x020081d0
.L_0200040c:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000420
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02000420:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000466
	ldr	r3, [pc, #152]
	mov	r1, fp
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_0200044e
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200dcdc
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02000460
.L_0200044e:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200dcdc
.L_02000456:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02000460:
	bl 0x0200dcdc
	str	r0, [r6, #52]
.L_02000466:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000482
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200dd5c
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200dd6c
.L_02000482:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000494
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #18]
.L_02000494:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020004a6
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_020004a6:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020004b6
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_020004b6:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200e188
	.4byte 0x020082bd
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	ldr	r3, [pc, #116]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200de2c
	adds	r7, r0, #0
	bl 0x0200dcf4
	movs	r6, #255
	ands	r0, r6
	cmp	r0, #0
	beq.n	.L_02000524
	bl 0x0200dcf4
	movs	r2, #176
	ands	r0, r6
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	adds	r0, r0, r2
	str	r0, [r5, #8]
	bl 0x0200dcf4
	ldr	r3, [r7, #12]
	movs	r2, #31
	ands	r2, r0
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #12]
	bl 0x0200dcf4
	ldr	r3, [r7, #16]
	movs	r2, #127
	ands	r2, r0
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #16]
.L_02000524:
	adds	r2, r5, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #102
	strh	r3, [r1, #0]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #212
	bne.n	.L_02000542
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x0200dd5c
	b.n	.L_0200054a
.L_02000542:
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200dd5c
.L_0200054a:
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	adds	r6, r0, #0
	cmp	r3, #0
	bne.n	.L_02000592
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
.L_02000574:
	bne.n	.L_02000592
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02000592
	movs	r1, #180
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020005a0
.L_02000592:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200dd64
	bl 0x0200dddc
	b.n	.L_02000608
.L_020005a0:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200dd64
	bl 0x0200dde4
	adds	r0, r6, #0
	adds	r0, #99
	ldrb	r1, [r0, #0]
	adds	r3, r1, #0
	cmp	r3, #0
	beq.n	.L_020005cc
	ldr	r3, [pc, #80]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000608
	adds	r3, r1, #0
	adds	r3, #255
	strb	r3, [r0, #0]
	b.n	.L_02000608
.L_020005cc:
	adds	r5, r6, #0
	adds	r5, #102
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_020005de
	adds	r0, r6, #0
.L_020005da:
	bl 0x020084d4
.L_020005de:
	ldrh	r3, [r5, #0]
	subs	r3, #1
	strh	r3, [r5, #0]
	ldr	r0, [r6, #16]
	ldr	r3, [r6, #12]
	asrs	r0, r0, #14
	asrs	r3, r3, #15
	adds	r0, r0, r3
	lsls	r0, r0, #8
	bl 0x0200dd04
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r6, #8]
	asrs	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r6, #8]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
.L_02000608:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	movs	r0, #0
	bl 0x0200df2c
	pop	{pc}
	.2byte 0x0000
	.4byte 0x049b23c0
	.4byte 0x681b33e0
	.4byte 0x33342201
	.2byte 0x701a
	.2byte 0x4770
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r1, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_02000640
	movs	r0, #0
	b.n	.L_0200066c
.L_02000640:
	cmp	r0, #2
	bhi.n	.L_02000654
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_02000656
.L_02000654:
	ldr	r4, [pc, #24]
.L_02000656:
	lsls	r3, r2, #7
	adds	r3, r5, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
	asrs	r3, r1, #8
	strb	r3, [r4, #2]
	strb	r1, [r4, #3]
.L_0200066c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #60
.L_0200067a:
	cmp	r5, #0
	beq.n	.L_0200068c
	movs	r0, #1
	bl 0x0200dce4
	ldr	r3, [r6, #40]
	subs	r5, #1
	cmp	r3, #0
	bne.n	.L_0200067a
.L_0200068c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_020006a0
	movs	r0, #0
	b.n	.L_020006c6
.L_020006a0:
	cmp	r0, #2
	bhi.n	.L_020006b4
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_020006b6
.L_020006b4:
	ldr	r4, [pc, #16]
.L_020006b6:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
.L_020006c6:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r0, #0
	asrs	r3, r3, #20
	mov	sl, r3
	ldr	r3, [r5, #16]
	mov	r1, sl
	asrs	r3, r3, #20
	mov	r8, r3
	mov	r2, r8
	sub	sp, #8
	bl 0x02008690
	mov	r1, sl
	mov	r2, r8
	mov	r9, r0
	movs	r0, #2
	bl 0x02008690
	movs	r2, #34
	adds	r2, r2, r5
	ldr	r1, [r5, #8]
	adds	r7, r0, #0
	mov	fp, r2
	ldrb	r0, [r2, #0]
	ldr	r2, [r5, #16]
	bl 0x0200dda4
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r6, r0, #19
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200dd44
	mov	r3, sl
	mov	r2, r8
	adds	r1, r5, #0
	lsls	r3, r3, #20
	lsls	r2, r2, #20
	adds	r1, #35
	str	r3, [sp, #4]
	str	r2, [sp, #0]
	cmp	r0, #0
	beq.n	.L_02000758
	ldr	r3, [pc, #120]
	ands	r7, r3
	movs	r3, #0
	mov	r9, r3
	ldr	r3, [r5, #20]
	asrs	r3, r3, #19
	cmp	r3, r6
	beq.n	.L_02000746
	subs	r6, #4
.L_02000746:
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x020081d0
	b.n	.L_0200077c
.L_02000758:
	movs	r3, #255
	lsls	r3, r3, #8
	orrs	r7, r3
	movs	r3, #212
	lsls	r3, r3, #8
	adds	r3, #128
	mov	r2, r9
	orrs	r2, r3
	mov	r9, r2
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	movs	r1, #1
	adds	r6, #4
	bl 0x020081d0
.L_0200077c:
	mov	r1, sl
	mov	r2, r8
	mov	r3, r9
	movs	r0, #0
	bl 0x0200862c
	mov	r1, sl
	mov	r2, r8
	adds	r3, r7, #0
	movs	r0, #2
	bl 0x0200862c
	mov	r3, fp
	ldrb	r2, [r3, #0]
	ldr	r0, [sp, #4]
	ldr	r1, [sp, #0]
	adds	r3, r6, #0
	bl 0x0200ddf4
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x00ff
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	mov	r8, r1
	ldrh	r2, [r3, #26]
	mov	r9, r0
	movs	r5, #1
	movs	r1, #26
	ldrsh	r0, [r3, r1]
	ands	r5, r2
	lsls	r5, r5, #1
	sub	sp, #4
	subs	r5, r0, r5
	bl 0x0200de2c
	adds	r5, #1
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200de2c
	mov	r2, r8
	adds	r7, r0, #0
	cmp	r2, #0
	bne.n	.L_0200085a
	ldr	r3, [r6, #80]
	ldr	r5, [r7, #80]
	mov	sl, r3
	adds	r3, r7, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	ldr	r3, [r7, #12]
	ldr	r0, [pc, #244]
	adds	r3, r3, r0
	str	r3, [r7, #12]
	movs	r0, #1
	bl 0x0200dce4
	ldrb	r3, [r5, #16]
	ldr	r1, [pc, #232]
	lsls	r3, r3, #2
	adds	r4, r3, r1
	ldrh	r3, [r4, #2]
	ldr	r2, [pc, #228]
	mov	r0, sl
	adds	r3, r3, r2
	str	r3, [r7, #104]
	ldrb	r3, [r0, #16]
	mov	r0, sp
	lsls	r3, r3, #2
	adds	r4, r3, r1
	ldrh	r3, [r4, #2]
	mov	r1, r8
	adds	r3, r3, r2
	ldrh	r2, [r4, #0]
	str	r3, [r6, #104]
	movs	r4, #133
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	lsls	r3, r3, #19
	str	r1, [r0, #0]
	adds	r3, #212
	ldr	r1, [r7, #104]
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [r6, #104]
	ldr	r1, [r7, #104]
	adds	r2, #192
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r6, #104]
	movs	r2, #192
	lsls	r2, r2, #2
	b.n	.L_02000882
.L_0200085a:
	mov	r2, r8
	cmp	r2, #5
	bgt.n	.L_0200088c
	ldr	r3, [r7, #12]
	ldr	r0, [pc, #156]
	movs	r2, #132
	adds	r3, r3, r0
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [r6, #104]
	ldr	r1, [r7, #104]
	adds	r2, #48
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r6, #104]
	movs	r2, #128
	lsls	r2, r2, #1
.L_02000882:
	adds	r3, r3, r2
	str	r3, [r6, #104]
	ldr	r3, [r7, #104]
	adds	r3, r3, r2
	str	r3, [r7, #104]
.L_0200088c:
	mov	r1, r8
	cmp	r1, #7
	bgt.n	.L_0200089a
	ldr	r3, [r6, #28]
	ldr	r2, [pc, #108]
	adds	r3, r3, r2
	str	r3, [r6, #28]
.L_0200089a:
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_020008e6
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	ldr	r3, [r7, #20]
	str	r3, [r7, #12]
	adds	r3, r6, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200dd4c
	adds	r3, r7, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200dd54
	ldr	r3, [r6, #80]
	adds	r0, r6, #0
	ldrb	r1, [r3, #24]
	adds	r1, #1
	bl 0x0200dd5c
	ldr	r3, [r7, #80]
	adds	r0, r7, #0
	ldrb	r1, [r3, #24]
	subs	r1, #1
	bl 0x0200dd5c
	adds	r0, r6, #0
	bl 0x020086cc
	adds	r0, r7, #0
	bl 0x020086cc
.L_020008e6:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffe40000
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x00053333
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02000980
	adds	r7, r0, #0
.L_0200091c:
	ldrh	r0, [r7, #0]
	bl 0x0200de2c
	movs	r3, #4
.L_02000924:
	ldrsh	r2, [r7, r3]
	movs	r1, #0
	mov	r8, r2
	adds	r6, r0, #0
	movs	r3, #2
	ldrsh	r5, [r7, r3]
	bl 0x0200ddbc
	mov	r2, r8
	lsls	r0, r2, #16
	lsrs	r0, r0, #16
	bl 0x0200dd44
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	lsls	r3, r5, #1
	adds	r5, r5, r3
	adds	r5, r5, r0
	adds	r1, r5, #0
.L_0200094a:
	adds	r0, r6, #0
	bl 0x0200dd5c
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r6, #0
	adds	r3, #100
	mov	r2, r8
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	adds	r7, #6
	bl 0x020086cc
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200091c
.L_02000980:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #828]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl 0x0200de2c
	adds	r7, r0, #0
	ldr	r6, [r7, #104]
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r2, #85
	adds	r2, r2, r7
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	adds	r0, r7, #0
	mov	fp, r2
	bl 0x0200ddbc
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200dd4c
	movs	r0, #137
	bl 0x0200df94
	movs	r3, #99
	adds	r3, r3, r6
	mov	r8, r3
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02000a28
.L_020009e2:
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #752]
	str	r3, [r7, #8]
	ldr	r3, [r6, #12]
	adds	r3, r3, r5
	str	r3, [r7, #12]
	ldr	r3, [r6, #16]
	str	r3, [r7, #16]
	cmp	r5, r1
	bgt.n	.L_020009fe
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #153
	adds	r5, r5, r2
.L_020009fe:
	ldr	r3, [pc, #732]
	adds	r1, r7, #0
	ldr	r2, [r3, #0]
	ldrb	r3, [r3, #0]
	adds	r1, #35
	lsls	r3, r3, #12
	strh	r3, [r7, #6]
	movs	r3, #1
	ands	r2, r3
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	movs	r0, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	bl 0x0200dce4
	mov	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_020009e2
.L_02000a28:
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200df94
	ldr	r3, [pc, #672]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #672]
	cmp	r2, r3
	bne.n	.L_02000a56
	ldr	r3, [r7, #8]
	asrs	r3, r3, #19
	cmp	r3, #39
	bne.n	.L_02000a56
	movs	r0, #16
	bl 0x0200defc
	bl 0x0200de1c
	b.n	.L_02000cc6
.L_02000a56:
	ldrh	r3, [r6, #6]
	movs	r2, #35
	movs	r1, #192
	adds	r2, r2, r7
	lsls	r1, r1, #8
	mov	r9, r2
	cmp	r3, r1
	beq.n	.L_02000a68
	b.n	.L_02000c00
.L_02000a68:
	ldr	r2, [r6, #104]
	movs	r3, #0
	mov	sl, r3
	mov	r8, r3
	mov	r1, r9
	movs	r3, #4
	str	r2, [sp, #0]
	strb	r3, [r1, #0]
	ldr	r3, [r7, #8]
	movs	r2, #2
	negs	r2, r2
	asrs	r3, r3, #19
	ands	r3, r2
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r7, #8]
	movs	r5, #0
	ldr	r3, [r6, #12]
	asrs	r3, r3, #19
	ands	r3, r2
	adds	r3, #3
	lsls	r3, r3, #19
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	asrs	r3, r3, #19
	ands	r3, r2
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r7, #16]
.L_02000aa2:
	ldr	r0, [pc, #576]
	movs	r2, #64
	ldr	r3, [r0, #0]
	movs	r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000abe
.L_02000ab0:
	movs	r3, #192
	ldr	r2, [pc, #564]
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	mov	sl, r1
	mov	r8, r2
	movs	r1, #1
.L_02000abe:
	ldr	r3, [r0, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000ad8
	movs	r1, #0
	mov	r8, r1
	mov	r2, r8
	movs	r3, #128
	strh	r2, [r7, #6]
	lsls	r3, r3, #13
	mov	sl, r3
	movs	r1, #1
.L_02000ad8:
	ldr	r3, [r0, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000af2
	ldr	r3, [pc, #516]
	movs	r1, #0
	mov	sl, r3
	movs	r3, #128
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	mov	r8, r1
	movs	r1, #1
.L_02000af2:
	cmp	r1, #0
	beq.n	.L_02000b12
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #12]
	ldr	r1, [r6, #8]
	subs	r2, r2, r3
	add	r1, sl
	add	r2, r8
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008690
	asrs	r0, r0, #8
	cmp	r0, #255
	bne.n	.L_02000b7c
.L_02000b12:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200dce4
	cmp	r5, #60
	bne.n	.L_02000aa2
	ldr	r3, [pc, #456]
	movs	r2, #0
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	mov	sl, r2
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #16]
	ldr	r1, [r6, #8]
	subs	r2, r2, r3
	add	r2, r8
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008690
	lsls	r0, r0, #8
	lsrs	r0, r0, #16
	cmp	r0, #255
	bne.n	.L_02000b7c
	movs	r2, #0
	mov	r8, r2
	mov	r3, r8
	strh	r3, [r7, #6]
	movs	r1, #128
	lsls	r1, r1, #13
	mov	sl, r1
	ldr	r2, [r6, #16]
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #12]
	add	r1, sl
	subs	r2, r2, r3
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008690
	lsls	r0, r0, #8
	lsrs	r0, r0, #16
	cmp	r0, #255
	bne.n	.L_02000b7c
	movs	r3, #128
	ldr	r1, [pc, #368]
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	mov	sl, r1
.L_02000b7c:
	ldr	r5, [sp, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	adds	r5, #98
	add	r2, r8
	add	r1, sl
	ldrb	r0, [r5, #0]
	bl 0x0200dda4
	movs	r1, #6
	str	r0, [r7, #12]
	str	r0, [r7, #20]
	adds	r0, r7, #0
	bl 0x0200dd5c
	movs	r0, #6
	bl 0x0200dce4
	movs	r0, #152
	bl 0x0200df94
	adds	r0, r7, #0
	movs	r1, #7
	bl 0x0200dd5c
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	mov	r2, fp
	movs	r3, #2
	strb	r3, [r2, #0]
	mov	r1, r9
	movs	r3, #1
	strb	r3, [r1, #0]
	adds	r2, r7, #0
	ldrb	r3, [r5, #0]
	adds	r2, #34
	strb	r3, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	ldr	r2, [r7, #12]
	add	r3, r8
	add	r1, sl
	adds	r0, r7, #0
	bl 0x0200dd8c
	adds	r0, r7, #0
	bl 0x02008674
	adds	r0, r7, #0
	bl 0x0200dd94
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200ddbc
	movs	r3, #3
	mov	r2, fp
	strb	r3, [r2, #0]
	b.n	.L_02000c90
.L_02000c00:
	ldr	r3, [r6, #8]
	ldrh	r1, [r6, #6]
	movs	r2, #2
	negs	r2, r2
	asrs	r3, r3, #19
	ands	r3, r2
	asrs	r1, r1, #13
	adds	r3, r3, r1
	subs	r3, #1
	lsls	r3, r3, #19
	str	r3, [r7, #8]
	adds	r0, r7, #0
	ldr	r3, [r6, #16]
	movs	r6, #0
	asrs	r3, r3, #19
	ands	r3, r2
	movs	r2, #2
	ands	r1, r2
	subs	r3, r3, r1
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r7, #16]
	movs	r1, #1
	bl 0x0200ddbc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	mov	r1, r9
	movs	r3, #33
	strb	r3, [r1, #0]
	mov	r2, fp
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200dedc
	bl 0x0200def4
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #12
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r0, #0
	bl 0x0200dda4
	ldr	r1, [r7, #8]
	adds	r2, r0, #0
	ldr	r3, [r7, #16]
	adds	r0, r5, #0
	bl 0x0200dd8c
	adds	r0, r7, #0
	bl 0x02008674
	adds	r0, r5, #0
	bl 0x0200dd94
.L_02000c90:
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200decc
	bl 0x0200dee4
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200dd54
	movs	r0, #10
	bl 0x0200dce4
	bl 0x0200de1c
.L_02000cc6:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x000bffff
	.4byte 0x0300122c
	.4byte 0x00000055
	.4byte 0x03001150
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	ldr	r3, [r1, #0]
	ldr	r4, [r0, #0]
	ldr	r2, [r1, #8]
	subs	r4, r4, r3
	ldr	r3, [r0, #8]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r4, #0
	muls	r0, r4
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.2byte 0x0000
	.2byte 0x02d4
	.2byte 0x0300
	push	{lr}
	ldmia	r1!, {r3}
	ldmia	r0!, {r4}
	ldr	r2, [r1, #0]
	subs	r4, r4, r3
	ldr	r3, [r0, #0]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	movs	r2, #192
	lsls	r2, r2, #11
	adds	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r4, #0
	muls	r0, r4
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #4]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r2, [pc, #228]
	movs	r3, #133
	mov	sl, r2
	lsls	r3, r3, #2
	add	r3, sl
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200de2c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r7, r0, #0
	mov	r8, r3
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000db6
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, r8
.L_02000d82:
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000db6
	movs	r3, #175
	lsls	r3, r3, #1
.L_02000d8e:
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000db6
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	cmp	r5, #0
	bne.n	.L_02000db6
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	sl, r3
	mov	r2, sl
	ldrb	r3, [r2, #0]
.L_02000db2:
	cmp	r3, #5
	bne.n	.L_02000dc0
.L_02000db6:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200dd64
.L_02000dbe:
	b.n	.L_02000f24
.L_02000dc0:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200dd64
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #12]
	ldr	r1, [r6, #8]
	subs	r2, r2, r3
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008690
	asrs	r0, r0, #8
	cmp	r0, #212
	bne.n	.L_02000e0c
	adds	r3, r6, #0
	movs	r1, #142
	adds	r3, #99
	lsls	r1, r1, #1
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	adds	r1, #255
	str	r5, [r6, #108]
	bl 0x0200df8c
	ldrh	r3, [r6, #6]
	movs	r2, #192
	lsls	r2, r2, #8
	cmp	r3, r2
	bne.n	.L_02000e02
	ldr	r1, [pc, #60]
	b.n	.L_02000e04
.L_02000e02:
	ldr	r1, [pc, #60]
.L_02000e04:
	adds	r0, r6, #0
	bl 0x0200dd6c
	b.n	.L_02000f24
.L_02000e0c:
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02000eb6
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r5, #0
	adds	r0, #8
	adds	r1, #8
	cmp	r3, #2
	bne.n	.L_02000e44
	bl 0x02008d18
	cmp	r0, #16
	bgt.n	.L_02000e5a
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #16]
	b.n	.L_02000e50
	.4byte 0x02000240
	.4byte 0x0200e1ac
	.2byte 0xe194
	.2byte 0x0200
.L_02000e44:
	bl 0x02008cec
	cmp	r0, #8
	bgt.n	.L_02000e5a
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
.L_02000e50:
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02000e5a
	movs	r5, #1
.L_02000e5a:
	cmp	r5, #0
	beq.n	.L_02000eb6
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02000eb6
	ldrh	r3, [r6, #6]
	str	r6, [r7, #104]
	strh	r3, [r7, #6]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	ldr	r0, [r7, #80]
	ldr	r3, [r6, #80]
	ldrb	r1, [r0, #9]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r0, #9]
	movs	r3, #200
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	movs	r2, #128
	ldr	r4, [pc, #52]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	strb	r4, [r3, #0]
	mov	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #99
	strb	r3, [r2, #0]
.L_02000eb6:
	ldrh	r3, [r6, #6]
	movs	r2, #192
	lsls	r2, r2, #8
	adds	r0, r3, #0
	cmp	r3, r2
	bne.n	.L_02000efc
	bl 0x0200dd04
	movs	r1, #192
	lsls	r1, r1, #9
	ldr	r5, [pc, #20]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	b.n	.L_02000ee4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_02000ee4:
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200dcfc
	movs	r1, #192
	lsls	r1, r1, #9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	subs	r3, r3, r0
	str	r3, [r6, #12]
	b.n	.L_02000f24
.L_02000efc:
	bl 0x0200dd04
	movs	r1, #192
	lsls	r1, r1, #9
	ldr	r5, [pc, #40]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200dcfc
	movs	r1, #192
	lsls	r1, r1, #9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	adds	r3, r3, r0
	str	r3, [r6, #16]
.L_02000f24:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	sub	sp, #68
	adds	r7, r0, #0
	cmp	r3, #0
	beq.n	.L_02000f56
	b.n	.L_0200106c
.L_02000f56:
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_02000f66
	b.n	.L_0200106c
.L_02000f66:
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_02000f76
	b.n	.L_0200106c
.L_02000f76:
	movs	r1, #180
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200106c
	movs	r3, #100
	adds	r3, r3, r7
	mov	sl, r3
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #240
	bne.n	.L_02001062
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200dd44
	add	r2, sp, #16
	add	r6, sp, #56
	mov	r8, r2
	cmp	r0, #0
	bne.n	.L_02000fac
	adds	r0, r7, #0
	movs	r1, #202
	bl 0x0200df8c
.L_02000fac:
	ldrh	r3, [r7, #6]
	movs	r1, #192
.L_02000fb0:
	lsls	r1, r1, #8
	cmp	r3, r1
	bne.n	.L_02000fbe
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #16]
	b.n	.L_02000fea
.L_02000fbe:
	ldrh	r0, [r7, #6]
	bl 0x0200dd04
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r5, [pc, #172]
	lsls	r0, r0, #12
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68bb
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldrh	r0, [r7, #6]
	bl 0x0200dcfc
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #12
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x693b
	adds	r3, r3, r0
.L_02000fea:
	str	r3, [r6, #8]
	add	r6, sp, #56
	movs	r0, #140
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	lsls	r0, r0, #1
	bl 0x0200dd74
	movs	r1, #2
	adds	r5, r0, #0
	bl 0x0200dd5c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r3, r5, #0
	movs	r4, #0
	adds	r3, #85
	strb	r4, [r3, #0]
	adds	r3, #13
	strb	r4, [r3, #0]
	adds	r3, #1
	strb	r4, [r3, #0]
	ldrh	r3, [r7, #6]
	mov	r1, r8
	strh	r3, [r5, #6]
	ldr	r3, [pc, #72]
	mov	r2, sl
	str	r3, [r5, #108]
	movs	r3, #1
	str	r7, [r5, #104]
	strh	r4, [r2, #0]
.L_0200103e:
	str	r3, [r1, #0]
	movs	r3, #7
	str	r3, [r1, #4]
	ldr	r3, [pc, #56]
	ldr	r2, [r6, #8]
	ldr	r0, [r6, #0]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #10
	ldr	r1, [r7, #12]
	str	r3, [sp, #8]
	mov	r3, r8
	str	r3, [sp, #12]
	movs	r3, #0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	bl 0x020082f4
.L_02001062:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_0200106c:
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300021c
	.4byte 0x02008d49
	.2byte 0x0000
	.2byte 0xfffa
	.2byte 0xb5e0
	adds	r5, r0, #0
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #24
	adds	r0, #255
	asrs	r6, r1, #16
	lsrs	r7, r2, #24
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_020010ae
	adds	r3, r5, #0
	adds	r3, #90
	strb	r0, [r3, #0]
	adds	r3, #10
	strh	r6, [r3, #0]
	subs	r3, #2
	strb	r7, [r3, #0]
	ldr	r3, [pc, #4]
	str	r3, [r5, #108]
.L_020010ae:
	pop	{r5, r6, r7, pc}
	.2byte 0x8f35
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r2, [pc, #216]
	movs	r3, #133
	mov	r9, r2
	lsls	r3, r3, #2
	add	r3, r9
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200de2c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r5, r0, #0
	mov	r8, r3
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200111e
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200111e
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200111e
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	cmp	r7, #0
	bne.n	.L_0200111e
	movs	r3, #217
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02001128
.L_0200111e:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200dd64
	b.n	.L_02001280
.L_02001128:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200dd64
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #12]
	ldr	r1, [r6, #8]
	subs	r2, r2, r3
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008690
	asrs	r0, r0, #8
	cmp	r0, #212
	bne.n	.L_02001166
	movs	r1, #142
	lsls	r1, r1, #1
	adds	r0, r6, #0
	adds	r1, #255
	bl 0x0200df8c
	adds	r3, r6, #0
	adds	r3, #99
	strb	r7, [r3, #0]
	ldr	r1, [pc, #64]
	adds	r0, r6, #0
	str	r7, [r6, #108]
	bl 0x0200dd6c
	b.n	.L_02001280
.L_02001166:
	movs	r3, #98
	adds	r3, r3, r6
	mov	sl, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02001212
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, r9
	ldrb	r3, [r3, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r7, #0
	adds	r0, #8
	adds	r1, #8
	cmp	r3, #2
	bne.n	.L_020011a0
	bl 0x02008d18
	cmp	r0, #16
	bgt.n	.L_020011b6
	ldr	r2, [r5, #16]
	ldr	r3, [r6, #16]
	b.n	.L_020011ac
	.4byte 0x02000240
	.2byte 0xe1cc
	.2byte 0x0200
.L_020011a0:
	bl 0x02008cec
	cmp	r0, #8
	bgt.n	.L_020011b6
	ldr	r2, [r5, #12]
	ldr	r3, [r6, #12]
.L_020011ac:
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020011b6
	movs	r7, #1
.L_020011b6:
	cmp	r7, #0
	beq.n	.L_02001212
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02001212
	ldrh	r3, [r6, #6]
	str	r6, [r5, #104]
	strh	r3, [r5, #6]
	adds	r1, r5, #0
.L_020011ce:
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r2, #12
	ldr	r3, [r6, #80]
	ldr	r0, [r5, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r0, #9]
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r0, #9]
	movs	r3, #200
	strh	r3, [r2, #0]
.L_020011f8:
	ldr	r3, [pc, #60]
	movs	r2, #128
	ldr	r4, [pc, #52]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	strb	r4, [r3, #0]
	mov	r2, sl
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #99
	strb	r3, [r2, #0]
.L_02001212:
	ldrh	r3, [r6, #6]
	movs	r2, #192
	lsls	r2, r2, #8
	adds	r0, r3, #0
	cmp	r3, r2
	bne.n	.L_02001258
	bl 0x0200dd04
	movs	r1, #192
	lsls	r1, r1, #9
	ldr	r5, [pc, #20]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	b.n	.L_02001240
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_02001240:
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200dcfc
	movs	r1, #192
	lsls	r1, r1, #9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	subs	r3, r3, r0
	str	r3, [r6, #12]
	b.n	.L_02001280
.L_02001258:
	bl 0x0200dd04
.L_0200125c:
	movs	r1, #192
	lsls	r1, r1, #9
	ldr	r5, [pc, #40]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200dcfc
	movs	r1, #192
	lsls	r1, r1, #9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	adds	r3, r3, r0
	str	r3, [r6, #16]
.L_02001280:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	sub	sp, #68
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_020012b2
	b.n	.L_02001408
.L_020012b2:
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_020012c2
	b.n	.L_02001408
.L_020012c2:
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_020012d2
	b.n	.L_02001408
.L_020012d2:
	movs	r1, #180
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020012e2
	b.n	.L_02001408
.L_020012e2:
	movs	r5, #0
.L_020012e4:
	adds	r0, r5, #0
	adds	r0, #13
	adds	r5, #1
	bl 0x0200de2c
	cmp	r5, #3
	bne.n	.L_020012e4
	movs	r3, #100
	add	r3, r8
	mov	sl, r3
	movs	r1, #0
	ldrsh	r3, [r3, r1]
.L_020012fc:
	cmp	r3, #240
	beq.n	.L_02001302
	b.n	.L_020013fe
.L_02001302:
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200dd44
	movs	r7, #0
	add	r6, sp, #56
	cmp	r0, #0
	bne.n	.L_0200131a
	mov	r0, r8
	movs	r1, #202
	bl 0x0200df8c
.L_0200131a:
	mov	r2, r8
	ldrh	r3, [r2, #6]
	movs	r1, #192
	lsls	r1, r1, #8
	cmp	r3, r1
	bne.n	.L_0200132e
	ldr	r3, [r2, #8]
	str	r3, [r6, #0]
	ldr	r3, [r2, #16]
	b.n	.L_02001360
.L_0200132e:
	mov	r2, r8
	ldrh	r0, [r2, #6]
	bl 0x0200dd04
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r5, [pc, #216]
	lsls	r0, r0, #12
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4641
	ldr	r3, [r1, #8]
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldrh	r0, [r1, #6]
	bl 0x0200dcfc
	adds	r1, r0, #0
	movs	r0, #128
.L_02001354:
	lsls	r0, r0, #12
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4642
	ldr	r3, [r2, #16]
	adds	r3, r3, r0
.L_02001360:
	str	r3, [r6, #8]
	movs	r5, #0
	b.n	.L_02001368
.L_02001366:
	adds	r5, #1
.L_02001368:
	cmp	r5, #3
	beq.n	.L_0200137c
	adds	r0, r5, #0
	adds	r0, #13
	bl 0x0200de2c
	adds	r7, r0, #0
	ldr	r3, [r7, #108]
	cmp	r3, #0
	bne.n	.L_02001366
.L_0200137c:
	add	r6, sp, #56
	ldr	r3, [r6, #0]
	mov	r1, r8
	str	r3, [r7, #8]
	adds	r0, r7, #0
	ldr	r3, [r1, #12]
	movs	r1, #2
	str	r3, [r7, #12]
	movs	r5, #0
	ldr	r3, [r6, #8]
	str	r3, [r7, #16]
	bl 0x0200dd5c
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r1, [r7, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r3, #13
	strb	r5, [r3, #0]
	adds	r3, #1
	strb	r5, [r3, #0]
	mov	r2, r8
	ldrh	r3, [r2, #6]
	add	r4, sp, #16
	strh	r3, [r7, #6]
	ldr	r3, [pc, #68]
	str	r2, [r7, #104]
	str	r3, [r7, #108]
	mov	r3, sl
	strh	r5, [r3, #0]
	movs	r3, #1
	str	r3, [r4, #0]
	movs	r3, #7
	str	r3, [r4, #4]
	ldr	r3, [pc, #56]
	ldr	r1, [r2, #12]
	ldr	r2, [r6, #8]
	ldr	r0, [r6, #0]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	movs	r3, #0
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	str	r4, [sp, #12]
	bl 0x020082f4
.L_020013fe:
	mov	r2, r8
	adds	r2, #100
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_02001408:
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300021c
	.4byte 0x020090b5
	.2byte 0x0000
	.2byte 0xfffa
	.2byte 0xb5e0
	adds	r5, r0, #0
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #24
	adds	r0, #255
	asrs	r6, r1, #16
	lsrs	r7, r2, #24
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02001452
	adds	r3, r5, #0
	adds	r3, #90
	strb	r0, [r3, #0]
	adds	r3, #10
	strh	r6, [r3, #0]
	subs	r3, #2
	strb	r7, [r3, #0]
	ldr	r3, [pc, #12]
	adds	r0, r5, #0
	movs	r1, #0
	str	r3, [r5, #108]
	bl 0x0200ddbc
.L_02001452:
	pop	{r5, r6, r7, pc}
	.2byte 0x9291
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #408]
	movs	r1, #133
	lsls	r1, r1, #2
.L_02001468:
	adds	r3, r3, r1
	movs	r5, #192
	lsls	r5, r5, #18
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	ldr	r7, [r5, #108]
	bl 0x0200de2c
	adds	r5, #128
	ldr	r3, [r5, #0]
	movs	r2, #168
	lsls	r2, r2, #6
	adds	r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_02001492
	b.n	.L_020015f0
.L_02001492:
	movs	r1, #217
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020014a0
	b.n	.L_020015f0
.L_020014a0:
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_020014d2
	subs	r2, #12
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_020014d2
	adds	r2, #4
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_020014d2
	adds	r2, #10
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r5, [r3, r1]
	cmp	r5, #0
	beq.n	.L_020014dc
.L_020014d2:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200dd64
	b.n	.L_020015f0
.L_020014dc:
	movs	r1, #16
	adds	r0, r6, #0
	bl 0x0200dd64
	movs	r2, #100
	adds	r2, r2, r6
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L_02001506
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bne.n	.L_02001500
	adds	r0, r6, #0
	bl 0x0200dd7c
	b.n	.L_020015f0
.L_02001500:
	str	r5, [r6, #16]
	str	r5, [r6, #8]
	b.n	.L_020015f0
.L_02001506:
	movs	r2, #8
	adds	r2, r2, r6
	mov	r0, r8
	mov	sl, r2
	adds	r0, #8
	mov	r1, sl
	bl 0x02008cec
	cmp	r0, #8
	bgt.n	.L_02001546
	mov	r1, r8
	movs	r3, #14
	ldrsh	r2, [r1, r3]
	movs	r1, #14
	ldrsh	r3, [r6, r1]
	cmp	r2, r3
	bne.n	.L_02001546
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #201
	strh	r3, [r2, #0]
	str	r5, [r6, #76]
	ldr	r3, [r6, #48]
	mov	r1, r8
	negs	r3, r3
	asrs	r3, r3, #1
	str	r3, [r6, #48]
	ldr	r2, [pc, #192]
	ldr	r3, [r1, #16]
	adds	r3, r3, r2
	str	r3, [r6, #16]
.L_02001546:
	ldr	r3, [pc, #180]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #176]
	cmp	r2, r3
	bne.n	.L_02001582
	movs	r0, #8
	bl 0x0200de2c
	mov	r1, sl
	adds	r5, r0, #0
	adds	r0, #8
	bl 0x02008cec
	cmp	r0, #8
	bgt.n	.L_020015be
	ldr	r3, [r6, #48]
	ldr	r2, [pc, #144]
	negs	r3, r3
	asrs	r3, r3, #1
	str	r3, [r6, #48]
	movs	r3, #0
	str	r3, [r6, #76]
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	b.n	.L_020015be
.L_02001582:
	ldr	r3, [pc, #132]
	cmp	r2, r3
	bne.n	.L_020015be
	movs	r5, #0
.L_0200158a:
	adds	r0, r5, #0
	adds	r0, #12
	bl 0x0200de2c
	mov	r1, sl
	adds	r7, r0, #0
	adds	r0, #8
	bl 0x02008cec
	cmp	r0, #8
	bgt.n	.L_020015b4
	ldr	r3, [r6, #48]
	ldr	r1, [pc, #92]
	negs	r3, r3
	asrs	r3, r3, #1
	str	r3, [r6, #48]
	movs	r3, #0
	str	r3, [r6, #76]
	ldr	r3, [r7, #16]
	adds	r3, r3, r1
	str	r3, [r6, #16]
.L_020015b4:
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #3
	bne.n	.L_0200158a
.L_020015be:
	mov	r2, r9
	ldrh	r3, [r2, #0]
	mov	r1, r9
	subs	r3, #1
	strh	r3, [r1, #0]
	ldr	r1, [r6, #48]
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #52]
	adds	r3, r3, r1
	str	r3, [r6, #16]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r6, #76]
	movs	r2, #192
	subs	r1, r1, r3
	ldr	r3, [r6, #24]
	lsls	r2, r2, #4
	adds	r2, #204
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	str	r1, [r6, #48]
	adds	r3, r3, r2
	str	r3, [r6, #28]
.L_020015f0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0x00000057
	.2byte 0x005c
	.2byte 0x0000
	push	{r5, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	movs	r0, #30
	adds	r1, r4, #0
	adds	r3, r2, #0
	adds	r0, #255
	adds	r2, r5, #0
	bl 0x0200dd74
	movs	r1, #1
	adds	r5, r0, #0
	bl 0x0200dd5c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #6
	bl 0x0200de9c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddc4
	adds	r2, r5, #0
	ldr	r1, [pc, #60]
	adds	r2, #100
	movs	r3, #27
	strh	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	adds	r3, #5
	strb	r1, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #48]
	bl 0x0200dcf4
	movs	r3, #254
	lsls	r3, r3, #7
	ldr	r2, [pc, #24]
	adds	r3, #255
	ands	r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r2
	str	r3, [r5, #52]
	movs	r3, #224
	lsls	r3, r3, #6
	str	r3, [r5, #76]
	ldr	r3, [pc, #12]
	str	r3, [r5, #108]
	b.n	.L_0200168c
	.4byte 0x00000000
	.4byte 0xffff8001
	.2byte 0x9459
	.2byte 0x0200
.L_0200168c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_020016a8
	b.n	.L_020017da
.L_020016a8:
	subs	r1, #12
	adds	r3, r2, r1
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_020016b6
	b.n	.L_020017da
.L_020016b6:
	adds	r1, #4
	adds	r3, r2, r1
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_020016c4
	b.n	.L_020017da
.L_020016c4:
	adds	r1, #10
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020016d2
	b.n	.L_020017da
.L_020016d2:
	ldr	r3, [pc, #264]
	ldr	r0, [r3, #0]
	movs	r3, #6
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_020017da
	ldr	r2, [pc, #256]
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r2, r4
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	ldr	r3, [pc, #248]
	cmp	r1, r3
	bne.n	.L_02001716
.L_020016f0:
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0200170a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #155
	bl 0x0200dd44
	movs	r3, #188
	lsls	r3, r3, #17
	cmp	r0, #0
	beq.n	.L_0200170e
.L_0200170a:
	movs	r3, #140
	lsls	r3, r3, #17
.L_0200170e:
	movs	r2, #154
	lsls	r2, r2, #18
	adds	r0, r3, #0
	b.n	.L_020017b6
.L_02001716:
	ldr	r3, [pc, #208]
	cmp	r1, r3
	bne.n	.L_020017da
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_02001776
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r2, r1
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	ldr	r3, [r0, #16]
	asrs	r3, r3, #19
	cmp	r3, #41
	ble.n	.L_02001766
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #19
	cmp	r3, #44
	bgt.n	.L_0200174e
.L_02001746:
	movs	r0, #168
	lsls	r0, r0, #16
	ldr	r2, [pc, #160]
	b.n	.L_020017b6
.L_0200174e:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02001798
	movs	r0, #252
	movs	r2, #228
	lsls	r0, r0, #17
	lsls	r2, r2, #17
	b.n	.L_020017b6
.L_02001766:
	movs	r0, #154
	movs	r2, #136
	lsls	r0, r0, #18
	ldr	r1, [pc, #128]
	lsls	r2, r2, #16
	bl 0x0200960c
	b.n	.L_020017da
.L_02001776:
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r2, r3
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	ldr	r3, [r0, #16]
	asrs	r3, r3, #19
	cmp	r3, #41
	ble.n	.L_020017be
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #19
	cmp	r3, #49
	ble.n	.L_020017a2
.L_02001798:
	movs	r0, #138
	movs	r2, #228
	lsls	r0, r0, #18
	lsls	r2, r2, #17
	b.n	.L_020017b6
.L_020017a2:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02001746
	movs	r0, #132
	movs	r2, #150
	lsls	r0, r0, #17
	lsls	r2, r2, #18
.L_020017b6:
	movs	r1, #0
	bl 0x0200960c
	b.n	.L_020017da
.L_020017be:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02001766
	movs	r0, #162
	movs	r2, #136
.L_020017d0:
	lsls	r0, r0, #18
	ldr	r1, [pc, #28]
	lsls	r2, r2, #16
	bl 0x0200960c
.L_020017da:
	pop	{r5, pc}
	.4byte 0x0300122c
	.4byte 0x02000240
	.4byte 0x00000057
	.4byte 0x0000005c
	.4byte 0x025b0000
	.4byte 0xffe00000
	.4byte 0x22234b0b
	.4byte 0x46941812
	.4byte 0x2301681a
	.4byte 0x4664401a
	.4byte 0x40932302
	.4byte 0x6e817822
	.4byte 0x70234053
	.4byte 0x688b2280
	.4byte 0x60830312
	.4byte 0x189b68cb
	.4byte 0x690b60c3
	.4byte 0x47706103
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #932]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #132
	bl 0x0200de2c
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r1, #0
	str	r1, [sp, #16]
	mov	r8, r3
	mov	sl, r0
	mov	r9, r1
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r0, #207
	bl 0x0200df94
	mov	r4, sl
	mov	r2, sl
	movs	r0, #140
	ldr	r3, [r4, #16]
	ldr	r1, [r2, #8]
	lsls	r0, r0, #1
	ldr	r2, [r2, #12]
	bl 0x0200dd74
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200dd5c
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r3, [pc, #852]
	add	r5, sp, #16
	str	r3, [r7, #24]
	movs	r3, #204
	lsls	r3, r3, #6
	ldrb	r5, [r5, #0]
	adds	r3, #51
	str	r3, [r7, #28]
	adds	r3, r7, #0
	adds	r3, #85
	mov	r1, r8
	strb	r5, [r3, #0]
	ldr	r0, [r1, #20]
	ldr	r4, [r7, #80]
	ldr	r3, [r0, #80]
	ldrb	r1, [r4, #9]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	ldr	r4, [pc, #808]
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	adds	r3, r3, r4
	adds	r0, r7, #0
	bl 0x0200dd8c
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r0, #0
	bl 0x0200dda4
	movs	r5, #166
	ldr	r3, [r7, #28]
	lsls	r5, r5, #9
	adds	r5, #203
	str	r0, [r7, #20]
	cmp	r3, r5
	bgt.n	.L_020018fe
.L_020018e2:
	movs	r0, #200
	lsls	r0, r0, #5
	adds	r0, #153
	adds	r3, r3, r0
	str	r3, [r7, #28]
	movs	r0, #1
	bl 0x0200de0c
	movs	r1, #166
	ldr	r3, [r7, #28]
	lsls	r1, r1, #9
	adds	r1, #203
	cmp	r3, r1
	ble.n	.L_020018e2
.L_020018fe:
	adds	r0, r7, #0
	bl 0x0200dd94
	mov	r2, r8
	ldr	r3, [r2, #20]
	ldr	r4, [pc, #728]
	ldr	r3, [r3, #16]
	movs	r5, #230
	adds	r3, r3, r4
	str	r3, [r7, #16]
	b.n	.L_020019d2
.L_02001914:
	ldr	r0, [pc, #720]
	movs	r3, #3
	ldr	r6, [r0, #0]
	mov	fp, r0
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02001996
	mov	r1, r8
	ldr	r3, [r1, #20]
	movs	r0, #168
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #12]
	lsls	r0, r0, #2
	ldr	r3, [r3, #16]
	bl 0x0200dd74
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r0, #0
	ldr	r2, [r5, #16]
	ldr	r1, [r5, #8]
	bl 0x0200dda4
	ldr	r3, [r5, #8]
	str	r0, [r5, #20]
	str	r3, [r5, #68]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r5, #72]
	ldr	r3, [r5, #16]
	str	r3, [r5, #76]
	bl 0x0200dcf4
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #48]
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200dd5c
	adds	r0, r5, #0
	ldr	r1, [pc, #620]
	bl 0x0200dd6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200de9c
	ldr	r3, [pc, #604]
	str	r3, [r5, #108]
.L_02001996:
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020019c0
	ldr	r3, [r7, #24]
	movs	r2, #200
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	str	r3, [r7, #24]
	ldr	r3, [r7, #28]
	movs	r1, #15
	adds	r3, r3, r2
	str	r3, [r7, #28]
	mov	r3, r8
	ldr	r0, [r3, #20]
	bl 0x0200de9c
	b.n	.L_020019ca
.L_020019c0:
	mov	r4, r8
	ldr	r0, [r4, #20]
	movs	r1, #4
	bl 0x0200de9c
.L_020019ca:
	movs	r0, #1
	bl 0x0200de0c
	movs	r5, #230
.L_020019d2:
	ldr	r3, [r7, #28]
	lsls	r5, r5, #9
	adds	r5, #203
	cmp	r3, r5
	ble.n	.L_02001914
	mov	r1, r8
	ldr	r0, [r1, #20]
	movs	r1, #0
	bl 0x0200de9c
	movs	r0, #136
	bl 0x0200df94
	mov	r3, r8
	ldr	r2, [r3, #20]
	mov	r4, sl
	ldr	r3, [r2, #8]
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	ldr	r3, [r2, #16]
	ldr	r5, [sp, #24]
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	ldr	r3, [r4, #8]
	asrs	r3, r3, #20
	cmp	r3, r5
	bge.n	.L_02001a12
	movs	r0, #1
	movs	r1, #0
	str	r0, [sp, #32]
	str	r1, [sp, #36]
	b.n	.L_02001a40
.L_02001a12:
	ldr	r2, [sp, #24]
	cmp	r3, r2
	ble.n	.L_02001a20
	movs	r3, #1
	negs	r3, r3
	movs	r4, #0
	b.n	.L_02001a3c
.L_02001a20:
	mov	r5, sl
	ldr	r3, [r5, #16]
	ldr	r0, [sp, #28]
	asrs	r3, r3, #20
	cmp	r3, r0
	bge.n	.L_02001a36
	movs	r1, #0
	movs	r2, #1
	str	r1, [sp, #32]
	str	r2, [sp, #36]
	b.n	.L_02001a40
.L_02001a36:
	movs	r4, #1
	movs	r3, #0
	negs	r4, r4
.L_02001a3c:
	str	r3, [sp, #32]
	str	r4, [sp, #36]
.L_02001a40:
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r0, #2
	bl 0x02008690
	ldr	r3, [sp, #28]
	adds	r5, r0, #0
	ldr	r0, [sp, #24]
	lsls	r2, r3, #20
	lsls	r1, r0, #20
	asrs	r5, r5, #8
	movs	r0, #2
	str	r5, [sp, #16]
	bl 0x0200dda4
	ldr	r3, [r7, #20]
	cmp	r0, r3
	beq.n	.L_02001a68
	movs	r4, #0
	str	r4, [sp, #16]
.L_02001a68:
	ldr	r5, [sp, #24]
	ldr	r0, [sp, #32]
	ldr	r3, [sp, #28]
	ldr	r4, [sp, #36]
	adds	r1, r5, r0
	ldr	r0, [sp, #16]
	adds	r2, r3, r4
	movs	r5, #2
	str	r1, [sp, #24]
	str	r2, [sp, #28]
	add	r9, r5
	cmp	r0, #0
	beq.n	.L_02001a40
	movs	r0, #2
	bl 0x02008690
	mov	r3, r8
	ldr	r5, [sp, #32]
	movs	r1, #2
	ldr	r2, [r3, #20]
	negs	r1, r1
	add	r9, r1
	mov	r3, r9
	muls	r3, r5
	movs	r4, #10
	ldrsh	r6, [r2, r4]
	ldr	r1, [sp, #36]
	lsls	r3, r3, #3
	adds	r6, r6, r3
	mov	r3, r9
	muls	r3, r1
	ldr	r1, [r7, #80]
	movs	r0, #18
	ldrsh	r5, [r2, r0]
	lsls	r3, r3, #3
	ldrb	r2, [r1, #9]
	adds	r5, r5, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r3, #128
	adds	r4, r5, #0
	lsls	r3, r3, #11
	adds	r2, r6, #0
	lsls	r1, r2, #16
	str	r3, [r7, #48]
	ldr	r2, [r7, #12]
	str	r3, [r7, #52]
	adds	r0, r7, #0
	lsls	r3, r4, #16
	str	r6, [sp, #24]
	str	r5, [sp, #28]
	bl 0x0200dd8c
	movs	r0, #160
	movs	r1, #160
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200ded4
	lsls	r5, r5, #16
	lsls	r6, r6, #16
	movs	r1, #1
	adds	r2, r5, #0
	adds	r0, r6, #0
	negs	r1, r1
	movs	r3, #1
	bl 0x0200dedc
	movs	r5, #0
	mov	r9, r5
	b.n	.L_02001cea
.L_02001afe:
	ldr	r3, [pc, #220]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #232]
	cmp	r2, r3
	bne.n	.L_02001b6e
	movs	r0, #8
	bl 0x0200de2c
	adds	r5, r0, #0
	ldr	r2, [r7, #8]
	ldr	r3, [r5, #8]
	asrs	r2, r2, #19
	asrs	r3, r3, #19
	cmp	r2, r3
	beq.n	.L_02001b26
	b.n	.L_02001c6e
.L_02001b26:
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_02001b2e
	b.n	.L_02001c6e
.L_02001b2e:
	ldr	r3, [pc, #200]
	str	r7, [r5, #104]
.L_02001b32:
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
.L_02001b44:
	strb	r3, [r1, #0]
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r3, [r7, #80]
	ldr	r0, [r5, #80]
	ldrb	r3, [r3, #9]
	movs	r1, #12
	ands	r1, r3
	movs	r4, #13
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	str	r5, [r7, #104]
	movs	r5, #156
	ands	r3, r2
	lsls	r5, r5, #14
	orrs	r3, r1
	adds	r5, #37
	strb	r3, [r0, #9]
	b.n	.L_02001c6c
.L_02001b6e:
	ldr	r3, [pc, #140]
	cmp	r2, r3
	bne.n	.L_02001c00
	movs	r0, #9
	bl 0x0200de2c
	adds	r5, r0, #0
	ldr	r2, [r7, #8]
	ldr	r3, [r5, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001c6e
	ldr	r2, [r7, #16]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001c6e
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_02001c6e
	ldr	r3, [pc, #92]
	str	r7, [r5, #104]
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r3, [r7, #80]
	ldr	r0, [r5, #80]
	ldrb	r3, [r3, #9]
	movs	r1, #12
	ands	r1, r3
	movs	r4, #13
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	str	r5, [r7, #104]
	movs	r5, #164
	ands	r3, r2
	lsls	r5, r5, #14
	orrs	r3, r1
	adds	r5, #105
	strb	r3, [r0, #9]
	b.n	.L_02001c6c
	.4byte 0x02000240
	.4byte 0x0001b333
	.4byte 0xffff0000
	.4byte 0x0300122c
	.4byte 0x0200dfd8
	.4byte 0x0200abd1
	.4byte 0x0000005a
	.4byte 0x020097f5
	.2byte 0x005c
	.2byte 0x0000
.L_02001c00:
	ldr	r3, [pc, #868]
	cmp	r2, r3
	bne.n	.L_02001c6e
	movs	r0, #10
	bl 0x0200de2c
	adds	r5, r0, #0
	ldr	r2, [r7, #8]
	ldr	r3, [r5, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001c6e
	ldr	r2, [r7, #16]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001c6e
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_02001c6e
	ldr	r3, [pc, #828]
	str	r7, [r5, #104]
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r3, [r7, #80]
	ldr	r0, [r5, #80]
	ldrb	r3, [r3, #9]
	movs	r1, #12
	ands	r1, r3
	movs	r4, #13
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	orrs	r3, r1
	str	r5, [r7, #104]
	movs	r5, #240
	lsls	r5, r5, #12
	strb	r3, [r0, #9]
	adds	r5, #51
.L_02001c6c:
	mov	r9, r5
.L_02001c6e:
	ldr	r0, [pc, #768]
	movs	r3, #1
	mov	fp, r0
	ldr	r0, [r0, #0]
	mov	sl, r0
	mov	r1, sl
	ands	r1, r3
	mov	sl, r1
	cmp	r1, #0
	bne.n	.L_02001ce4
	add	r6, sp, #80
	str	r3, [r6, #0]
	movs	r2, #128
	movs	r3, #128
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, #162
	ands	r0, r2
	strh	r3, [r6, #24]
	movs	r3, #2
	str	r3, [r6, #4]
	lsls	r0, r0, #12
	mov	r8, r2
	bl 0x0200dd04
	add	r5, sp, #120
	lsls	r0, r0, #1
	str	r0, [r5, #0]
	bl 0x0200dcf4
	ldr	r3, [r7, #12]
	movs	r2, #31
	ands	r2, r0
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r3, fp
	ldr	r0, [r3, #0]
	mov	r4, r8
	ands	r0, r4
	lsls	r0, r0, #12
	bl 0x0200dcfc
	str	r0, [r5, #8]
	ldr	r4, [r7, #8]
	ldr	r1, [r5, #4]
	ldr	r3, [r5, #0]
	ldr	r2, [r7, #16]
	str	r0, [sp, #4]
	movs	r0, #152
	lsls	r0, r0, #13
	mov	r5, sl
	str	r0, [sp, #8]
	adds	r0, r4, #0
	str	r5, [sp, #0]
	str	r6, [sp, #12]
	bl 0x020082f4
.L_02001ce4:
	movs	r0, #1
	bl 0x0200de0c
.L_02001cea:
	adds	r0, r7, #0
	bl 0x0200ddec
	cmp	r0, #0
	bne.n	.L_02001cf6
	b.n	.L_02001afe
.L_02001cf6:
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_02001cfe
	b.n	.L_02001e0a
.L_02001cfe:
	ldr	r5, [r7, #104]
	asrs	r3, r0, #16
	lsls	r3, r3, #19
	str	r3, [r5, #8]
	lsls	r3, r0, #19
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r1, #85
	str	r3, [r5, #40]
	adds	r1, r1, r5
	movs	r3, #2
	movs	r6, #0
	str	r6, [r5, #108]
	strb	r3, [r1, #0]
	mov	r8, r1
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r2, #249
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x020081d0
	ldr	r3, [pc, #564]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #556]
	cmp	r2, r3
	bne.n	.L_02001d74
	ldr	r3, [pc, #552]
	movs	r2, #18
	str	r3, [r5, #20]
	movs	r3, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #19
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r0, #244
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200dd4c
	b.n	.L_02001e0a
.L_02001d74:
	ldr	r3, [pc, #520]
	cmp	r2, r3
	bne.n	.L_02001dba
	movs	r3, #20
	movs	r2, #38
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #38
	movs	r2, #1
	movs	r3, #1
	movs	r0, #19
	bl 0x0200ddac
	adds	r0, r5, #0
	bl 0x02008674
	movs	r0, #1
	bl 0x0200dce4
	adds	r0, r5, #0
	bl 0x02008674
	mov	r0, r8
	strb	r6, [r0, #0]
	movs	r1, #208
	movs	r0, #160
	str	r6, [r5, #20]
	str	r6, [r5, #12]
	lsls	r0, r0, #17
	lsls	r1, r1, #18
	movs	r2, #0
	movs	r3, #4
	bl 0x0200ddf4
	b.n	.L_02001e0a
.L_02001dba:
	ldr	r3, [pc, #428]
	cmp	r2, r3
	bne.n	.L_02001e0a
	adds	r0, r5, #0
	bl 0x02008674
	movs	r0, #1
	bl 0x0200dce4
	adds	r0, r5, #0
	bl 0x02008674
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	mov	r1, r8
	strb	r6, [r1, #0]
	movs	r0, #240
	movs	r1, #204
	lsls	r0, r0, #15
	str	r6, [r5, #20]
	str	r6, [r5, #12]
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #4
	bl 0x0200ddf4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #153
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02001e0a
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #0
	bl 0x02009084
.L_02001e0a:
	ldr	r2, [sp, #16]
	cmp	r2, #255
	bne.n	.L_02001e18
	movs	r0, #136
	bl 0x0200df94
	b.n	.L_0200208c
.L_02001e18:
	ldr	r4, [sp, #32]
	movs	r3, #10
	ldrsh	r2, [r7, r3]
	lsls	r3, r4, #1
	ldr	r0, [sp, #36]
	adds	r3, r3, r4
	lsls	r3, r3, #4
	adds	r3, r2, r3
	str	r3, [sp, #24]
	movs	r5, #18
	ldrsh	r2, [r7, r5]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	adds	r3, r2, r3
	str	r3, [sp, #28]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #28]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	lsls	r1, r2, #16
	adds	r0, r7, #0
	lsls	r3, r4, #16
	ldr	r2, [r7, #12]
	bl 0x0200dd8c
	ldr	r5, [sp, #16]
	movs	r0, #0
	subs	r5, #30
	str	r5, [sp, #16]
	str	r0, [sp, #20]
	b.n	.L_02002078
.L_02001e5c:
	ldr	r1, [sp, #16]
	cmp	r1, #4
	ble.n	.L_02001e64
	b.n	.L_0200208c
.L_02001e64:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L_02001e74
	movs	r0, #235
	bl 0x0200df94
	movs	r3, #1
	str	r3, [sp, #20]
.L_02001e74:
	ldr	r4, [pc, #248]
	movs	r5, #1
	ldr	r3, [r4, #0]
	mov	fp, r4
	ands	r3, r5
	mov	r9, r5
	cmp	r3, #0
	bne.n	.L_02001eec
	add	r0, sp, #40
	str	r5, [r0, #0]
	mov	r8, r0
	bl 0x0200dcf4
.L_02001e8e:
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, #255
	mov	sl, r1
	ands	r3, r0
	mov	r2, r8
	add	r3, sl
	str	r3, [r2, #12]
	str	r3, [r2, #8]
.L_02001ea4:
	mov	r3, fp
	ldr	r0, [r3, #0]
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r0, r6
	lsls	r0, r0, #12
	bl 0x0200dd04
	mov	r4, sl
	add	r5, sp, #120
	lsls	r0, r0, #1
	str	r4, [r5, #4]
	str	r0, [r5, #0]
	mov	r1, fp
	ldr	r0, [r1, #0]
	ands	r0, r6
	lsls	r0, r0, #12
	bl 0x0200dcfc
	str	r0, [r5, #8]
	ldr	r6, [r7, #8]
	ldr	r4, [r5, #4]
	ldr	r1, [r7, #12]
	ldr	r2, [r7, #16]
	ldr	r3, [r5, #0]
	str	r0, [sp, #4]
	movs	r0, #160
	lsls	r0, r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #8]
	mov	r4, r8
	adds	r0, r6, #0
	str	r4, [sp, #12]
	bl 0x020082f4
.L_02001eec:
	mov	r5, fp
	ldr	r3, [r5, #0]
	movs	r5, #3
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_02001efa
	b.n	.L_02002072
.L_02001efa:
	ldr	r3, [pc, #120]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_02001f88
.L_02001f0c:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200dd4c
	ldr	r3, [r7, #8]
	mov	r4, r9
	asrs	r3, r3, #20
	adds	r3, #64
	str	r3, [sp, #24]
	ldr	r2, [sp, #24]
	ldr	r3, [r7, #16]
	adds	r2, #1
	asrs	r3, r3, #20
	movs	r0, #90
	movs	r1, #25
	str	r3, [sp, #28]
	str	r5, [sp, #4]
	str	r4, [sp, #0]
	bl 0x0200dd9c
	movs	r3, #2
	str	r3, [sp, #4]
	mov	r5, r9
	movs	r0, #21
	movs	r1, #88
	movs	r2, #24
	movs	r3, #89
	str	r5, [sp, #0]
	bl 0x0200dd9c
	movs	r3, #89
	movs	r5, #23
	str	r3, [sp, #4]
	movs	r0, #23
	movs	r1, #92
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	movs	r3, #25
	str	r3, [sp, #4]
	movs	r0, #18
	movs	r1, #33
	movs	r2, #2
	b.n	.L_0200201a
	.4byte 0x0000005d
	.4byte 0x020097f5
	.4byte 0x0300122c
	.4byte 0x02000240
	.4byte 0x0000005a
	.4byte 0xffe00000
	.4byte 0x0000005c
	.2byte 0x0058
	.2byte 0x0000
.L_02001f88:
	ldr	r3, [pc, #744]
	cmp	r2, r3
	bne.n	.L_02002024
	ldr	r3, [r7, #8]
	ldr	r2, [r7, #12]
	asrs	r6, r3, #20
	ldr	r3, [r7, #16]
	ldr	r0, [sp, #16]
	subs	r3, r3, r2
	asrs	r3, r3, #20
	adds	r5, r3, #0
	adds	r5, #64
	str	r6, [sp, #24]
	str	r5, [sp, #28]
	cmp	r0, #0
	bne.n	.L_02001fe0
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200dd4c
	mov	r1, r9
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	adds	r3, r5, #0
	movs	r0, #39
	movs	r1, #82
	adds	r2, r6, #0
	bl 0x0200dd9c
	movs	r3, #82
	movs	r5, #38
	str	r3, [sp, #4]
	movs	r0, #39
	movs	r1, #82
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
.L_02001fd6:
	movs	r3, #18
	str	r3, [sp, #4]
	movs	r0, #38
	movs	r1, #19
	b.n	.L_02002018
.L_02001fe0:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x0200dd4c
	mov	r2, r9
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	adds	r3, r5, #0
	movs	r0, #39
	movs	r1, #82
	adds	r2, r6, #0
	bl 0x0200dd9c
	movs	r3, #99
	movs	r5, #38
	str	r3, [sp, #4]
	movs	r0, #38
	movs	r1, #98
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	movs	r3, #37
	str	r3, [sp, #4]
	movs	r0, #38
	movs	r1, #35
.L_02002018:
	movs	r2, #1
.L_0200201a:
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	b.n	.L_0200206e
.L_02002024:
	ldr	r3, [pc, #592]
	cmp	r2, r3
	bne.n	.L_0200206e
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200dd4c
	ldr	r2, [r7, #8]
	mov	r3, r9
	asrs	r2, r2, #20
	str	r3, [sp, #0]
	movs	r0, #15
	movs	r1, #80
	movs	r3, #77
	str	r2, [sp, #24]
	str	r5, [sp, #4]
	bl 0x0200dd9c
	movs	r3, #79
	movs	r5, #15
	str	r3, [sp, #4]
	movs	r0, #15
	movs	r1, #80
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	movs	r0, #17
	movs	r1, #15
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ddac
.L_0200206e:
	bl 0x0200dd84
.L_02002072:
	movs	r0, #1
	bl 0x0200de0c
.L_02002078:
	adds	r0, r7, #0
	bl 0x0200ddec
	cmp	r0, #0
	bne.n	.L_02002084
	b.n	.L_02001e5c
.L_02002084:
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200df94
.L_0200208c:
	movs	r3, #0
	str	r3, [r7, #52]
	str	r3, [r7, #48]
	str	r3, [r7, #64]
	str	r3, [r7, #60]
	str	r3, [r7, #56]
	str	r3, [sp, #20]
.L_0200209a:
	ldr	r3, [r7, #24]
	ldr	r4, [pc, #476]
	ldr	r5, [pc, #480]
	adds	r3, r3, r4
	str	r3, [r7, #24]
	ldr	r3, [r7, #28]
	movs	r0, #224
	adds	r3, r3, r5
	str	r3, [r7, #28]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #10
	adds	r3, r3, r0
	str	r3, [r7, #12]
	movs	r0, #1
	bl 0x0200de0c
	ldr	r1, [sp, #20]
	adds	r1, #1
	str	r1, [sp, #20]
	cmp	r1, #8
	bne.n	.L_0200209a
	adds	r0, r7, #0
	bl 0x0200dd7c
	ldr	r3, [pc, #440]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #432]
.L_020020d8:
	cmp	r2, r3
	beq.n	.L_020020de
	b.n	.L_0200228c
.L_020020de:
	ldr	r5, [sp, #16]
	cmp	r5, #5
	bne.n	.L_020021a8
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	ldr	r0, [sp, #16]
	movs	r3, #85
	str	r0, [sp, #0]
	movs	r1, #64
	movs	r2, #29
.L_02002116:
	movs	r5, #7
	movs	r0, #79
	str	r5, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #30
	bl 0x0200de0c
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	ldr	r1, [sp, #16]
	movs	r0, #69
	str	r1, [sp, #0]
	movs	r2, #29
	movs	r1, #64
	movs	r3, #85
	str	r5, [sp, #4]
	bl 0x0200dd9c
	movs	r3, #29
	movs	r2, #21
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #46
	movs	r1, #20
	movs	r2, #5
	movs	r3, #5
	bl 0x0200ddac
	movs	r3, #31
	movs	r2, #89
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r1, #89
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r3, #33
	movs	r2, #87
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #86
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200dd4c
	b.n	.L_02002590
.L_020021a8:
	ldr	r2, [sp, #16]
	cmp	r2, #6
	beq.n	.L_020021b0
	b.n	.L_02002590
.L_020021b0:
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	movs	r5, #5
	movs	r3, #85
	movs	r1, #64
.L_020021de:
	movs	r2, #29
	movs	r6, #7
	movs	r0, #74
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #30
	bl 0x0200de0c
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
.L_02002200:
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	movs	r0, #64
	movs	r1, #64
	movs	r2, #29
	movs	r3, #85
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r3, #29
	movs	r2, #21
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #52
	movs	r1, #20
	movs	r2, #5
	movs	r3, #5
	bl 0x0200ddac
	movs	r3, #33
	movs	r2, #87
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #88
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
.L_02002252:
	movs	r3, #31
	movs	r2, #89
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #48
	movs	r1, #88
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200dd54
	b.n	.L_02002590
	.2byte 0x0000
	.4byte 0x0000005b
	.4byte 0x0000005d
	.4byte 0xffffd99a
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x0058
	.2byte 0x0000
.L_0200228c:
	ldr	r3, [pc, #812]
	cmp	r2, r3
	beq.n	.L_02002294
	b.n	.L_02002590
.L_02002294:
	ldr	r3, [sp, #16]
	cmp	r3, #5
	beq.n	.L_0200229c
	b.n	.L_0200243e
.L_0200229c:
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	ldr	r4, [sp, #16]
	movs	r3, #79
	movs	r0, #79
	movs	r1, #64
	movs	r2, #32
	movs	r5, #7
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #8
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #19
	cmp	r3, #65
	bne.n	.L_020023b6
	movs	r0, #8
	bl 0x0200de2c
	ldr	r5, [pc, #720]
	ldr	r3, [r0, #8]
	add	r7, sp, #120
	adds	r3, r3, r5
	str	r3, [r7, #0]
	movs	r0, #8
	bl 0x0200de2c
	ldr	r3, [r0, #12]
	movs	r0, #8
	str	r3, [r7, #4]
	bl 0x0200de2c
	ldr	r3, [r0, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r3, r3, r0
	movs	r1, #2
	str	r3, [r7, #8]
	movs	r0, #8
	bl 0x0200debc
	movs	r1, #0
	str	r1, [sp, #20]
.L_0200231c:
	add	r6, sp, #40
	movs	r3, #1
	str	r3, [r6, #0]
	bl 0x0200dcf4
	ldr	r5, [r7, #0]
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #16
	adds	r5, r5, r3
	bl 0x0200dcf4
	ldr	r1, [r7, #4]
	movs	r3, #31
	ands	r3, r0
	lsls	r3, r3, #16
	adds	r1, r1, r3
	movs	r3, #0
	ldr	r2, [r7, #8]
	str	r3, [sp, #0]
	ldr	r3, [pc, #636]
	adds	r0, r5, #0
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	movs	r3, #0
	str	r6, [sp, #12]
	bl 0x020082f4
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
	cmp	r2, #8
	bne.n	.L_0200231c
	movs	r0, #30
	bl 0x0200dce4
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	movs	r3, #5
	movs	r2, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #79
	movs	r2, #32
	movs	r0, #64
	movs	r1, #64
	bl 0x0200dd9c
	movs	r0, #8
	bl 0x0200de2c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_02002590
.L_020023b6:
	movs	r0, #30
	bl 0x0200de0c
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	ldr	r3, [sp, #16]
	movs	r0, #69
	str	r3, [sp, #0]
	movs	r1, #64
	movs	r2, #32
	movs	r3, #79
	str	r5, [sp, #4]
	bl 0x0200dd9c
	movs	r3, #32
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #49
	movs	r1, #15
	movs	r2, #5
	movs	r3, #5
.L_02002406:
	bl 0x0200ddac
	movs	r3, #34
	movs	r2, #82
	str	r3, [sp, #0]
	str	r2, [sp, #4]
.L_02002412:
	movs	r0, #33
	movs	r1, #82
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r3, #36
	movs	r2, #81
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #52
	movs	r1, #81
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
.L_02002438:
	bl 0x0200dd54
	b.n	.L_02002590
.L_0200243e:
	ldr	r4, [sp, #16]
	cmp	r4, #6
	beq.n	.L_02002446
.L_02002444:
	b.n	.L_02002590
.L_02002446:
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	movs	r0, #8
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #19
	cmp	r3, #65
	bne.n	.L_020024f2
	movs	r0, #8
	bl 0x0200de2c
	ldr	r5, [pc, #316]
	ldr	r3, [r0, #8]
	add	r7, sp, #120
	adds	r3, r3, r5
	str	r3, [r7, #0]
	movs	r0, #8
	bl 0x0200de2c
	ldr	r3, [r0, #12]
	movs	r0, #8
	str	r3, [r7, #4]
	bl 0x0200de2c
	ldr	r3, [r0, #16]
	movs	r0, #0
	str	r3, [r7, #8]
	str	r0, [sp, #20]
.L_020024a4:
	add	r6, sp, #40
	movs	r3, #1
	str	r3, [r6, #0]
	bl 0x0200dcf4
	ldr	r5, [r7, #0]
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #16
	adds	r5, r5, r3
	bl 0x0200dcf4
	ldr	r2, [r7, #8]
	movs	r3, #31
	ands	r3, r0
	lsls	r3, r3, #16
	subs	r2, r2, r3
	movs	r3, #0
	ldr	r1, [r7, #4]
	str	r3, [sp, #0]
	ldr	r3, [pc, #244]
	adds	r0, r5, #0
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	movs	r3, #0
	str	r6, [sp, #12]
	bl 0x020082f4
	ldr	r1, [sp, #20]
	adds	r1, #1
	str	r1, [sp, #20]
	cmp	r1, #8
	bne.n	.L_020024a4
	movs	r0, #20
	bl 0x0200dce4
	b.n	.L_02002590
.L_020024f2:
	movs	r3, #79
	movs	r1, #64
	movs	r2, #32
	movs	r5, #5
	movs	r6, #7
	movs	r0, #79
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #30
	bl 0x0200de0c
	movs	r0, #157
	bl 0x0200df94
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	movs	r0, #64
	movs	r1, #64
	movs	r2, #32
	movs	r3, #79
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r3, #32
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #49
	movs	r1, #21
	movs	r2, #5
	movs	r3, #5
	bl 0x0200ddac
	movs	r3, #36
	movs	r2, #81
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #81
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r3, #34
	movs	r2, #82
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #82
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200dd4c
.L_02002590:
	movs	r0, #30
	bl 0x0200de0c
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200deec
	bl 0x0200dee4
	bl 0x0200de1c
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0000005a
	.4byte 0xfff80000
	.4byte 0xffff8000
	.2byte 0x0240
	.2byte 0x0200
	.global Func_020025cc
	.thumb_func
Func_020025cc:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_020025e4
	ldr	r0, [pc, #12]
	b.n	.L_020025e6
.L_020025e4:
	ldr	r0, [pc, #12]
.L_020025e6:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x0200e268
	.2byte 0xe238
	.2byte 0x0200
	.global Func_020025f8
	.thumb_func
Func_020025f8:
	movs	r0, #0
	bx	lr
	.global Func_020025fc
	.thumb_func
Func_020025fc:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe400
	.2byte 0x0200
	.global Func_02002604
	.thumb_func
Func_02002604:
	push	{lr}
	ldr	r3, [pc, #104]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_0200261c
	ldr	r0, [pc, #92]
	b.n	.L_0200266e
.L_0200261c:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02002626
	ldr	r0, [pc, #92]
	b.n	.L_0200266e
.L_02002626:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02002630
	ldr	r0, [pc, #88]
	b.n	.L_0200266e
.L_02002630:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_0200263a
	ldr	r0, [pc, #88]
	b.n	.L_0200266e
.L_0200263a:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02002644
	ldr	r0, [pc, #84]
	b.n	.L_0200266e
.L_02002644:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200264e
.L_0200264a:
	ldr	r0, [pc, #84]
	b.n	.L_0200266e
.L_0200264e:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02002658
	ldr	r0, [pc, #80]
	b.n	.L_0200266e
.L_02002658:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02002662
	ldr	r0, [pc, #80]
	b.n	.L_0200266e
.L_02002662:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_0200266c
	ldr	r0, [pc, #76]
	b.n	.L_0200266e
.L_0200266c:
	ldr	r0, [pc, #76]
.L_0200266e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x0200e564
	.4byte 0x00000056
	.4byte 0x0200e63c
	.4byte 0x00000057
	.4byte 0x0200e6b4
	.4byte 0x00000058
	.4byte 0x0200e75c
	.4byte 0x00000059
	.4byte 0x0200e8ac
	.4byte 0x0000005a
	.4byte 0x0200e8f4
	.4byte 0x0000005b
	.4byte 0x0200e9e4
	.4byte 0x0000005c
	.4byte 0x0200eaec
	.4byte 0x0000005d
	.4byte 0x0200ebac
	.2byte 0xe54c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	sub	sp, #8
	bl 0x0200df84
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	asrs	r5, r3, #20
	lsls	r3, r6, #16
	adds	r1, r3, r5
	ldr	r3, [r0, #36]
	cmp	r3, #0
	bne.n	.L_02002750
	ldr	r3, [r0, #44]
	cmp	r3, #0
	bne.n	.L_02002750
	ldr	r2, [pc, #108]
	movs	r0, #1
	ldr	r3, [r2, #0]
	negs	r0, r0
	cmp	r3, r0
	beq.n	.L_02002750
	cmp	r1, r3
	beq.n	.L_02002750
	str	r0, [r2, #0]
	ldr	r1, [pc, #96]
	movs	r5, #255
	lsls	r5, r5, #8
	adds	r5, #255
	ldr	r0, [r1, #0]
	asrs	r6, r3, #16
	ldr	r1, [pc, #88]
	ands	r5, r3
	ldr	r3, [pc, #88]
	ldr	r1, [r1, #0]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	movs	r0, #73
	movs	r1, #72
	bl 0x0200ddb4
	ldr	r3, [pc, #72]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02002750
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #2
	adds	r1, r6, #0
	adds	r2, r5, #0
	ldr	r7, [r3, #108]
	bl 0x02008690
	asrs	r0, r0, #8
	cmp	r0, #0
	beq.n	.L_02002750
	movs	r1, #181
	adds	r3, r0, #0
	lsls	r1, r1, #1
	adds	r3, #200
	adds	r2, r7, r1
	strh	r3, [r2, #0]
.L_02002750:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x0200ec0c
	.4byte 0x0200f3e4
	.4byte 0x0200f3e8
	.4byte 0x0200f3dc
	.4byte 0x0200f3e0
	.4byte 0x02000240
	.2byte 0x0059
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #1
	negs	r2, r2
	movs	r1, #144
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x0200dcec
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200ec0c
	.2byte 0xa6c1
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	sub	sp, #8
	bl 0x0200df84
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	ldr	r1, [pc, #184]
	asrs	r4, r3, #20
	ldr	r3, [r0, #12]
	asrs	r5, r3, #20
	ldr	r3, [r0, #16]
	asrs	r0, r3, #20
	lsls	r3, r4, #16
	adds	r2, r3, r0
	ldr	r3, [r1, #0]
	cmp	r2, r3
	beq.n	.L_02002856
	str	r2, [r1, #0]
	ldr	r2, [pc, #164]
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #164]
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [pc, #160]
	movs	r1, #240
	lsls	r1, r1, #1
.L_020027d2:
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #152]
	ldr	r1, [pc, #156]
	cmp	r2, r3
	beq.n	.L_020027ec
	ldr	r3, [pc, #152]
	cmp	r2, r3
	beq.n	.L_020027ec
	ldr	r3, [pc, #152]
	cmp	r2, r3
	bne.n	.L_020027f6
.L_020027ec:
	ldr	r2, [pc, #148]
	subs	r3, r0, r5
	adds	r3, #64
	str	r4, [r1, #0]
	b.n	.L_02002818
.L_020027f6:
	ldr	r3, [pc, #144]
	cmp	r2, r3
	beq.n	.L_0200280e
	ldr	r3, [pc, #140]
	cmp	r2, r3
	beq.n	.L_0200280e
	ldr	r3, [pc, #140]
	cmp	r2, r3
	beq.n	.L_0200280e
	ldr	r3, [pc, #136]
	cmp	r2, r3
	bne.n	.L_0200281a
.L_0200280e:
	adds	r3, r4, #0
	ldr	r2, [pc, #112]
	adds	r3, #64
	str	r3, [r1, #0]
	subs	r3, r0, r5
.L_02002818:
	str	r3, [r2, #0]
.L_0200281a:
	ldr	r2, [pc, #92]
	ldr	r3, [pc, #100]
	ldr	r4, [pc, #72]
	ldr	r6, [pc, #72]
	ldr	r0, [r2, #0]
	ldr	r1, [r3, #0]
	mov	r9, r2
	mov	sl, r3
	ldr	r2, [r4, #0]
	ldr	r3, [r6, #0]
	mov	r8, r4
	movs	r5, #72
	movs	r4, #73
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ddb4
	mov	r4, r9
	ldr	r1, [r4, #0]
	mov	r0, r8
	mov	r4, sl
	ldr	r2, [r0, #0]
	ldr	r0, [r4, #0]
	ldr	r3, [r6, #0]
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #72
	movs	r0, #70
	bl 0x0200ddb4
.L_02002856:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200ec0c
	.4byte 0x0200f3dc
	.4byte 0x0200f3e0
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x0200f3e4
	.4byte 0x00000056
	.4byte 0x00000057
	.4byte 0x0200f3e8
	.4byte 0x00000059
	.4byte 0x0000005a
	.4byte 0x0000005b
	.2byte 0x005d
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	sub	sp, #8
	bl 0x0200df84
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	ldr	r1, [pc, #164]
	asrs	r4, r3, #20
	ldr	r3, [r0, #12]
	asrs	r5, r3, #20
	ldr	r3, [r0, #16]
	asrs	r0, r3, #20
	lsls	r3, r4, #16
	adds	r2, r3, r0
	ldr	r3, [r1, #0]
	cmp	r2, r3
	beq.n	.L_0200294c
	str	r2, [r1, #0]
	ldr	r2, [pc, #144]
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #144]
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [pc, #140]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #132]
	ldr	r1, [pc, #136]
	cmp	r2, r3
	beq.n	.L_020028ee
	ldr	r3, [pc, #132]
	cmp	r2, r3
	bne.n	.L_020028f8
.L_020028ee:
	ldr	r2, [pc, #132]
	subs	r3, r0, r5
	adds	r3, #64
	str	r4, [r1, #0]
	b.n	.L_0200290e
.L_020028f8:
	ldr	r3, [pc, #124]
	cmp	r2, r3
	beq.n	.L_02002904
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_02002910
.L_02002904:
	adds	r3, r4, #0
	ldr	r2, [pc, #108]
	adds	r3, #64
	str	r3, [r1, #0]
	subs	r3, r0, r5
.L_0200290e:
	str	r3, [r2, #0]
.L_02002910:
	ldr	r2, [pc, #88]
	ldr	r3, [pc, #96]
	ldr	r4, [pc, #68]
	ldr	r6, [pc, #72]
	ldr	r0, [r2, #0]
	ldr	r1, [r3, #0]
	mov	r9, r2
	mov	sl, r3
	ldr	r2, [r4, #0]
	ldr	r3, [r6, #0]
	mov	r8, r4
	movs	r5, #72
	movs	r4, #73
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ddb4
	mov	r4, r9
	ldr	r1, [r4, #0]
	mov	r0, r8
	mov	r4, sl
	ldr	r2, [r0, #0]
	ldr	r0, [r4, #0]
	ldr	r3, [r6, #0]
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #72
	movs	r0, #71
	bl 0x0200ddb4
.L_0200294c:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x0200ec0c
	.4byte 0x0200f3dc
	.4byte 0x0200f3e0
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x0200f3e4
	.4byte 0x00000056
	.4byte 0x0200f3e8
	.4byte 0x00000059
	.2byte 0x005a
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #8
	bl 0x0200df84
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r4, r3, #20
	ldr	r3, [r0, #12]
	asrs	r7, r3, #20
	ldr	r3, [r0, #16]
	ldr	r0, [pc, #96]
	asrs	r1, r3, #20
	lsls	r3, r4, #16
	adds	r2, r3, r1
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L_020029f4
	str	r2, [r0, #0]
	ldr	r2, [pc, #84]
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #84]
	ldr	r6, [pc, #84]
	mov	r8, r3
	ldr	r5, [pc, #84]
	mov	sl, r2
	movs	r3, #3
	mov	r2, r8
	subs	r1, r1, r7
	str	r3, [r2, #0]
	adds	r1, #64
	movs	r3, #73
	movs	r2, #72
	str	r4, [r6, #0]
	adds	r0, r4, #0
	str	r1, [r5, #0]
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #3
	movs	r2, #1
	bl 0x0200ddb4
	mov	r3, sl
	mov	r1, r8
	ldr	r2, [r3, #0]
	ldr	r0, [r5, #0]
	ldr	r3, [r1, #0]
	ldr	r1, [r6, #0]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	movs	r0, #72
	movs	r1, #72
	bl 0x0200ddb4
.L_020029f4:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ec0c
	.4byte 0x0200f3dc
	.4byte 0x0200f3e0
	.4byte 0x0200f3e4
	.2byte 0xf3e8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #8
	bl 0x0200df84
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r4, r3, #20
	ldr	r3, [r0, #12]
	asrs	r7, r3, #20
	ldr	r3, [r0, #16]
	ldr	r0, [pc, #96]
	asrs	r1, r3, #20
	lsls	r3, r4, #16
	adds	r2, r3, r1
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L_02002a88
	str	r2, [r0, #0]
	ldr	r2, [pc, #84]
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #84]
	ldr	r6, [pc, #84]
	mov	r8, r3
	ldr	r5, [pc, #84]
	mov	sl, r2
	movs	r3, #3
	mov	r2, r8
	subs	r1, r1, r7
	str	r3, [r2, #0]
	adds	r1, #63
	movs	r3, #73
	movs	r2, #72
	str	r4, [r6, #0]
	adds	r0, r4, #0
	str	r1, [r5, #0]
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #3
	movs	r2, #1
	bl 0x0200ddb4
	mov	r3, sl
	mov	r1, r8
	ldr	r2, [r3, #0]
	ldr	r0, [r5, #0]
	ldr	r3, [r1, #0]
	ldr	r1, [r6, #0]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	movs	r0, #72
	movs	r1, #72
	bl 0x0200ddb4
.L_02002a88:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ec0c
	.4byte 0x0200f3dc
	.4byte 0x0200f3e0
	.4byte 0x0200f3e4
	.2byte 0xf3e8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #68
	add	r6, sp, #28
	movs	r3, #1
	str	r3, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #16]
	str	r3, [r6, #20]
	mov	sl, r0
	mov	r8, r1
	movs	r7, #0
.L_02002ace:
	bl 0x0200dcf4
	movs	r3, #63
	ldr	r2, [pc, #120]
	ands	r3, r0
	lsls	r3, r3, #16
	add	r3, sl
	add	r5, sp, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	bl 0x0200dcf4
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #12
	str	r3, [r5, #4]
	bl 0x0200dcf4
	movs	r2, #7
	ands	r2, r0
	ldr	r3, [pc, #92]
	lsls	r2, r2, #16
	add	r2, r8
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r2, [r5, #8]
	ldr	r0, [r5, #0]
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #12
	adds	r3, #1
	str	r3, [sp, #8]
	movs	r1, #0
	movs	r3, #0
	adds	r7, #1
	str	r6, [sp, #12]
	bl 0x020082f4
	cmp	r7, #8
	bne.n	.L_02002ace
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffe00000
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	movs	r3, #168
	add	r5, sp, #28
	lsls	r3, r3, #2
	strh	r3, [r5, #24]
	movs	r3, #1
	str	r3, [r5, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r5, #8]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r5, #12]
	mov	r8, r0
	adds	r7, r1, #0
	movs	r6, #0
.L_02002b80:
	bl 0x0200dcf4
	adds	r3, r0, #0
	movs	r0, #31
	ands	r0, r3
	ldr	r3, [pc, #64]
	lsls	r0, r0, #16
	add	r0, r8
	adds	r0, r0, r3
	movs	r3, #128
	add	r2, sp, #16
	lsls	r3, r3, #11
	str	r0, [r2, #0]
	str	r3, [r2, #4]
	str	r7, [r2, #8]
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #208
	lsls	r3, r3, #13
	str	r3, [sp, #8]
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #0
	str	r5, [sp, #12]
	adds	r6, #1
	bl 0x020082f4
	movs	r0, #5
	bl 0x0200de0c
	cmp	r6, #12
	bne.n	.L_02002b80
	add	sp, #68
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb560
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r6, r0, #0
	ldr	r3, [r6, #20]
	ldr	r5, [r6, #12]
	ldr	r0, [r6, #48]
	subs	r5, r5, r3
	movs	r3, #128
	lsls	r3, r3, #13
	asrs	r5, r5, #1
	adds	r5, r5, r3
	movs	r3, #255
	ands	r0, r3
	lsls	r0, r0, #11
	mov	r8, r3
	bl 0x0200dd04
	ldr	r3, [pc, #68]
	adds	r1, r5, #0
	mov	sl, r3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6c73
	adds	r3, r3, r0
	ldr	r0, [r6, #48]
	str	r3, [r6, #8]
	mov	r3, r8
	ands	r0, r3
	lsls	r0, r0, #11
	bl 0x0200dcfc
	adds	r1, r5, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2103
	bl 0x0200dcdc
	ldr	r3, [r6, #76]
	ldr	r2, [r6, #72]
	adds	r3, r3, r0
	str	r3, [r6, #16]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #48]
	adds	r3, #1
	str	r3, [r6, #48]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	ldr	r6, [r5, #12]
	ldr	r0, [r5, #48]
	movs	r3, #192
	lsls	r3, r3, #12
	asrs	r6, r6, #2
	adds	r6, r6, r3
	movs	r3, #255
	ands	r0, r3
	lsls	r0, r0, #11
	mov	r8, r3
	bl 0x0200dd04
	ldr	r3, [pc, #84]
	adds	r1, r6, #0
	mov	sl, r3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6c6b
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r5, #48]
	str	r3, [r5, #8]
	mov	r3, r8
	ands	r0, r3
	lsls	r0, r0, #11
	bl 0x0200dcfc
	adds	r1, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6ceb
	ldr	r2, [r5, #72]
	adds	r3, r3, r0
	str	r3, [r5, #16]
	ldr	r3, [r5, #12]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r5, #48]
	movs	r2, #2
	adds	r3, #1
	str	r3, [r5, #48]
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ands	r3, r2
	lsrs	r3, r3, #1
	lsls	r1, r3, #3
	adds	r1, r1, r3
	bl 0x0200de9c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0300021c
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	adds	r2, r0, #0
	adds	r2, #99
	ldrb	r2, [r2, #0]
	lsrs	r3, r2
	movs	r2, #1
	ands	r3, r2
	adds	r2, r0, #0
	adds	r2, #98
	ldrb	r2, [r2, #0]
	adds	r1, r2, #0
	muls	r1, r3
	lsls	r1, r1, #24
	lsrs	r1, r1, #24
	bl 0x0200de9c
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #128
	lsls	r0, r0, #4
	sub	sp, #12
	bl 0x0200dd14
	adds	r5, r0, #0
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #3
	adds	r3, #8
	strh	r2, [r3, #0]
	ldr	r0, [pc, #88]
	bl 0x0200dd3c
	adds	r1, r5, #0
	bl 0x0200dd24
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	adds	r0, r5, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl 0x0200dd1c
	ldr	r3, [pc, #48]
	mov	r0, sp
	adds	r0, #10
	strh	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #44]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	bl 0x0200dd14
	adds	r5, r0, #0
	ldr	r0, [pc, #36]
	bl 0x0200dd3c
	adds	r1, r5, #0
	bl 0x0200dd24
	movs	r7, #0
	adds	r4, r5, #0
	b.n	.L_02002d98
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x000001c0
	.4byte 0x84000200
	.4byte 0x06002000
	.4byte 0x81000400
	.2byte 0x01c1
	.2byte 0x0000
.L_02002d98:
	ldrh	r2, [r4, #0]
	movs	r3, #252
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	adds	r7, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, #64
	bne.n	.L_02002d98
	adds	r4, r5, #0
	movs	r7, #0
.L_02002db0:
	ldr	r2, [pc, #432]
	lsls	r1, r7, #6
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
.L_02002dc6:
	subs	r3, #12
	adds	r7, #1
	adds	r4, #8
.L_02002dcc:
	cmp	r7, #16
	bne.n	.L_02002db0
	adds	r0, r5, #0
	bl 0x0200dd1c
	movs	r0, #246
	bl 0x0200df94
	movs	r0, #11
	bl 0x0200de2c
	movs	r3, #9
.L_02002de4:
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200de2c
	movs	r3, #1
	adds	r0, #99
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200de2c
	ldr	r3, [pc, #364]
	movs	r7, #0
	str	r3, [r0, #108]
.L_02002e00:
	ldr	r3, [pc, #360]
	ldr	r3, [r3, #0]
	mov	r8, r3
	mov	r0, r8
	movs	r3, #3
	ands	r0, r3
	mov	sl, r3
	mov	r8, r0
.L_02002e10:
	cmp	r0, #0
	bne.n	.L_02002eac
	movs	r0, #11
	bl 0x0200de2c
	adds	r6, r0, #0
	movs	r0, #11
	bl 0x0200de2c
	adds	r5, r0, #0
	movs	r0, #11
	bl 0x0200de2c
	ldr	r3, [r0, #16]
	movs	r0, #168
	ldr	r2, [r5, #12]
	ldr	r1, [r6, #8]
	lsls	r0, r0, #2
	bl 0x0200dd74
	adds	r5, r0, #0
	movs	r0, #11
	bl 0x0200de2c
	ldr	r3, [r0, #80]
	ldr	r4, [r5, #80]
	ldrb	r3, [r3, #9]
	movs	r1, #12
	ands	r1, r3
	movs	r0, #13
	ldrb	r3, [r4, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	orrs	r3, r1
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	strb	r3, [r4, #9]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	mov	r1, r8
	adds	r3, #85
	strb	r1, [r3, #0]
	ldr	r3, [r5, #8]
	str	r3, [r5, #68]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #72]
	ldr	r3, [r5, #16]
	str	r3, [r5, #76]
	bl 0x0200dcf4
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #48]
	adds	r0, r5, #0
	movs	r1, #7
.L_02002e8c:
	bl 0x0200dd5c
	adds	r0, r5, #0
	ldr	r1, [pc, #220]
	bl 0x0200dd6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200de9c
	ldr	r3, [pc, #200]
.L_02002eaa:
	str	r3, [r5, #108]
.L_02002eac:
	movs	r0, #1
	adds	r7, #1
	bl 0x0200de0c
	cmp	r7, #45
	bne.n	.L_02002e00
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200df04
	movs	r0, #60
	bl 0x0200df14
	ldr	r3, [pc, #160]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200decc
	bl 0x0200dee4
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200ded4
	movs	r0, #156
	movs	r1, #1
	movs	r2, #170
	lsls	r2, r2, #18
	negs	r1, r1
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x0200dedc
	bl 0x0200dee4
	movs	r0, #11
	bl 0x0200de2c
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r0, #11
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [pc, #72]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #152
	strh	r3, [r2, #0]
	movs	r3, #42
	strh	r3, [r2, #2]
	movs	r0, #30
	bl 0x0200dce4
	movs	r0, #138
	bl 0x0200df94
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #254
	lsls	r0, r0, #7
	b.n	.L_02002f80
	.4byte 0x00001010
	.4byte 0x00003f41
	.4byte 0x06002000
	.4byte 0x0200acc1
	.4byte 0x0300122c
	.4byte 0x0200dfd8
	.4byte 0x0200ac41
	.4byte 0x02000240
	.2byte 0x1120
	.2byte 0x0300
.L_02002f80:
	movs	r1, #0
	adds	r0, #255
	bl 0x0200df04
	movs	r0, #1
	bl 0x0200df14
	movs	r0, #1
	bl 0x0200dce4
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200df04
	movs	r0, #8
	bl 0x0200df14
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #56]
	movs	r0, #64
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r3, sl
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #78
	movs	r3, #105
	movs	r2, #18
	bl 0x0200dd9c
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	b.n	.L_02002ff8
	.2byte 0x0100
	.2byte 0x0000
.L_02002ff8:
	movs	r7, #0
.L_02002ffa:
	ldr	r2, [pc, #32]
	lsrs	r3, r7, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #28]
	movs	r6, #128
	lsls	r6, r6, #19
	orrs	r2, r3
	adds	r6, #82
	strh	r2, [r6, #0]
	movs	r0, #1
	adds	r7, #1
	bl 0x0200dce4
	cmp	r7, #32
	bne.n	.L_02002ffa
	b.n	.L_02003024
	.2byte 0x0000
	.4byte 0x00000010
	.2byte 0x1000
	.2byte 0x0000
.L_02003024:
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200df04
	movs	r0, #60
	bl 0x0200df14
	movs	r0, #30
	bl 0x0200dce4
	movs	r0, #148
	bl 0x0200df94
	movs	r0, #156
	movs	r1, #180
	lsls	r0, r0, #17
	lsls	r1, r1, #18
	bl 0x0200aaa8
	movs	r3, #3
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #105
	movs	r2, #18
	movs	r0, #64
	movs	r1, #78
	bl 0x0200dd9c
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200ded4
	movs	r1, #1
	movs	r0, #10
	bl 0x0200deec
	bl 0x0200dee4
	movs	r0, #146
	bl 0x0200df94
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200de94
	movs	r0, #10
	bl 0x0200de2c
	adds	r5, r0, #0
	movs	r0, #10
	bl 0x0200de2c
	ldr	r3, [r5, #8]
	ldr	r1, [r0, #16]
	adds	r0, r3, #0
	bl 0x0200ab58
	movs	r1, #0
	movs	r0, #10
	bl 0x0200de94
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #7
	bl 0x0200de9c
	movs	r0, #3
	bl 0x0200dce4
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200de9c
	movs	r0, #9
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200de9c
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200de9c
	movs	r0, #40
	bl 0x0200dce4
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #8
	movs	r2, #0
	bl 0x02009084
	bl 0x0200dd34
	bl 0x0200dd2c
	ldr	r3, [pc, #88]
	movs	r0, #147
	movs	r1, #128
	lsls	r0, r0, #1
	lsls	r1, r1, #2
	adds	r0, #255
	adds	r1, #38
	adds	r2, r3, r0
	adds	r3, r3, r1
	ldrb	r0, [r2, #0]
	ldrb	r1, [r3, #0]
	bl 0x0200de04
	ldr	r3, [pc, #48]
	movs	r2, #128
	strh	r3, [r6, #0]
	ldr	r3, [pc, #48]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #36]
	movs	r0, #128
	orrs	r3, r2
	ldr	r2, [pc, #40]
	strh	r3, [r1, #0]
	lsls	r0, r0, #4
	movs	r3, #0
	strh	r3, [r2, #0]
	strh	r3, [r2, #2]
	adds	r0, #154
	bl 0x0200dd4c
	bl 0x0200de1c
	add	sp, #12
	b.n	.L_02003168
	.4byte 0x00001008
	.4byte 0x00003f10
	.4byte 0x00000100
	.4byte 0x02000240
	.2byte 0x1120
	.2byte 0x0300
.L_02003168:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #220]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	sub	sp, #8
	bl 0x0200de2c
	ldr	r2, [r0, #16]
	ldr	r1, [r0, #8]
	asrs	r2, r2, #20
	asrs	r1, r1, #20
	subs	r2, #1
	movs	r0, #1
	bl 0x02008690
	asrs	r0, r0, #8
	cmp	r0, #30
	bne.n	.L_0200324a
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r0, #188
	bl 0x0200df94
	movs	r5, #1
	movs	r6, #2
	movs	r1, #76
	movs	r2, #19
	movs	r3, #92
	movs	r0, #65
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #8
	bl 0x0200de0c
	movs	r1, #76
	movs	r2, #19
	movs	r3, #92
	movs	r0, #66
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #8
	bl 0x0200de0c
	movs	r1, #76
	movs	r2, #19
	movs	r3, #92
	movs	r0, #67
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #8
	bl 0x0200de0c
	movs	r1, #76
	movs	r2, #19
	movs	r3, #92
	movs	r0, #62
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #15
	bl 0x0200de0c
	ldr	r0, [r7, #0]
	bl 0x0200de2c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r0, [r7, #0]
	bl 0x0200de2c
	adds	r5, r0, #0
	ldr	r0, [r7, #0]
	bl 0x0200de2c
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #52]
	str	r3, [r5, #48]
	movs	r0, #123
	bl 0x0200df94
	movs	r1, #156
	movs	r2, #198
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	ldr	r0, [r7, #0]
	bl 0x0200de54
	movs	r0, #5
	bl 0x0200de0c
	movs	r0, #15
	bl 0x0200defc
	bl 0x0200de1c
.L_0200324a:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #152]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #144]
	cmp	r2, r3
	bne.n	.L_02003270
	ldr	r0, [pc, #140]
	bl 0x0200df74
	b.n	.L_020032ee
.L_02003270:
	ldr	r3, [pc, #136]
	cmp	r2, r3
	bne.n	.L_0200327e
	ldr	r0, [pc, #136]
	bl 0x0200df74
	b.n	.L_020032ee
.L_0200327e:
	ldr	r3, [pc, #132]
	cmp	r2, r3
	bne.n	.L_0200328c
	ldr	r0, [pc, #128]
	bl 0x0200df74
	b.n	.L_020032ee
.L_0200328c:
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_0200329a
	ldr	r0, [pc, #124]
	bl 0x0200df74
	b.n	.L_020032ee
.L_0200329a:
	ldr	r3, [pc, #120]
	cmp	r2, r3
	bne.n	.L_020032c8
	movs	r0, #152
	movs	r1, #168
	lsls	r0, r0, #18
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #0
	bl 0x0200ddf4
	movs	r0, #156
	movs	r1, #168
	lsls	r0, r0, #18
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #0
	bl 0x0200ddf4
	ldr	r0, [pc, #84]
	bl 0x0200df74
	b.n	.L_020032ee
.L_020032c8:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_020032d6
	ldr	r0, [pc, #80]
	bl 0x0200df74
	b.n	.L_020032ee
.L_020032d6:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020032ee
	movs	r0, #10
	bl 0x0200de2c
	movs	r3, #0
	adds	r0, #100
	strh	r3, [r0, #0]
	ldr	r0, [pc, #60]
	bl 0x0200df74
.L_020032ee:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x0200e050
	.4byte 0x00000056
	.4byte 0x0200e054
	.4byte 0x00000057
	.4byte 0x0200e05a
	.4byte 0x00000058
	.4byte 0x0200e064
	.4byte 0x0000005b
	.4byte 0x0200e068
	.4byte 0x0000005c
	.4byte 0x0200e072
	.4byte 0x0000005d
	.2byte 0xe07e
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	adds	r3, r3, r1
	ldr	r7, [r3, #0]
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	bl 0x0200df7c
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r0, #2
	bl 0x0200dda4
	ldr	r3, [r7, #12]
	cmp	r0, r3
	beq.n	.L_0200338c
	movs	r2, #34
	adds	r2, r2, r7
	movs	r3, #2
	adds	r6, r7, #0
	strb	r3, [r2, #0]
	adds	r6, #85
	movs	r3, #3
	strb	r3, [r6, #0]
	adds	r0, r7, #0
	mov	r8, r2
	bl 0x02008674
	movs	r0, #188
	bl 0x0200df94
	adds	r0, r7, #0
	bl 0x02008674
	movs	r5, #0
	mov	r3, r8
	strb	r5, [r6, #0]
	strb	r5, [r3, #0]
.L_0200338c:
	ldr	r3, [r7, #8]
	movs	r1, #240
	asrs	r5, r3, #19
	ldr	r3, [r7, #16]
	lsls	r1, r1, #1
	asrs	r6, r3, #19
	ldr	r3, [pc, #664]
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #660]
	cmp	r2, r3
	bne.n	.L_020033be
	cmp	r5, #103
	beq.n	.L_020033ac
	b.n	.L_02003628
.L_020033ac:
	cmp	r6, #41
	beq.n	.L_020033b2
	b.n	.L_02003628
.L_020033b2:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #142
	bl 0x0200dd4c
	b.n	.L_02003628
.L_020033be:
	ldr	r3, [pc, #636]
	cmp	r2, r3
	bne.n	.L_020033f0
	cmp	r5, #13
	bne.n	.L_020033d8
	cmp	r6, #49
	bne.n	.L_020033d8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #152
	bl 0x0200dd4c
	b.n	.L_02003628
.L_020033d8:
	cmp	r5, #41
	beq.n	.L_020033de
	b.n	.L_02003628
.L_020033de:
	cmp	r6, #49
	beq.n	.L_020033e4
	b.n	.L_02003628
.L_020033e4:
	movs	r0, #243
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200dd4c
	b.n	.L_02003628
.L_020033f0:
	ldr	r3, [pc, #588]
	cmp	r2, r3
	bne.n	.L_02003436
	cmp	r5, #33
	bne.n	.L_0200340a
	cmp	r6, #79
	bne.n	.L_0200340a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #155
	bl 0x0200dd4c
	b.n	.L_02003628
.L_0200340a:
	cmp	r5, #77
	bne.n	.L_0200341e
	cmp	r6, #71
	bne.n	.L_0200341e
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #156
	bl 0x0200dd4c
	b.n	.L_02003628
.L_0200341e:
	cmp	r5, #59
	beq.n	.L_02003424
	b.n	.L_02003628
.L_02003424:
	cmp	r6, #65
	beq.n	.L_0200342a
	b.n	.L_02003628
.L_0200342a:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #157
	bl 0x0200dd4c
	b.n	.L_02003628
.L_02003436:
	ldr	r3, [pc, #524]
	cmp	r2, r3
	bne.n	.L_02003454
	cmp	r5, #81
	beq.n	.L_02003442
	b.n	.L_02003628
.L_02003442:
	cmp	r6, #37
	beq.n	.L_02003448
	b.n	.L_02003628
.L_02003448:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #158
	bl 0x0200dd4c
	b.n	.L_02003628
.L_02003454:
	ldr	r3, [pc, #496]
	cmp	r2, r3
	bne.n	.L_0200351c
	movs	r0, #152
	movs	r1, #168
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #17
	movs	r2, #2
	negs	r3, r3
	bl 0x0200ddf4
	movs	r0, #156
	movs	r1, #168
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #17
	movs	r2, #2
	negs	r3, r3
	bl 0x0200ddf4
	cmp	r5, #43
	bne.n	.L_0200348e
	cmp	r6, #87
	bne.n	.L_0200348e
	movs	r0, #141
	lsls	r0, r0, #4
	bl 0x0200dd4c
.L_0200348e:
	movs	r0, #9
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #19
	cmp	r3, #73
	bne.n	.L_020034aa
	movs	r0, #9
	bl 0x0200de2c
	ldr	r3, [r0, #16]
	asrs	r3, r3, #19
	cmp	r3, #43
	beq.n	.L_020034c6
.L_020034aa:
	movs	r0, #10
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #19
	cmp	r3, #73
	bne.n	.L_020034d0
	movs	r0, #10
	bl 0x0200de2c
	ldr	r3, [r0, #16]
	asrs	r3, r3, #19
	cmp	r3, #43
	bne.n	.L_020034d0
.L_020034c6:
	movs	r0, #13
	bl 0x0200de2c
	movs	r3, #1
	b.n	.L_020034d8
.L_020034d0:
	movs	r0, #13
	bl 0x0200de2c
	movs	r3, #0
.L_020034d8:
	adds	r0, #99
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200de2c
	ldr	r2, [r7, #8]
	ldr	r3, [r0, #8]
	cmp	r2, r3
	beq.n	.L_020034ec
	b.n	.L_02003628
.L_020034ec:
	movs	r0, #11
	bl 0x0200de2c
	ldr	r2, [r7, #16]
	ldr	r3, [r0, #16]
	cmp	r2, r3
	beq.n	.L_020034fc
	b.n	.L_02003628
.L_020034fc:
	cmp	r5, #83
	bne.n	.L_0200350e
	cmp	r6, #43
	bne.n	.L_0200350e
	movs	r0, #14
	bl 0x0200de2c
	movs	r3, #1
	b.n	.L_02003516
.L_0200350e:
	movs	r0, #14
	bl 0x0200de2c
	movs	r3, #0
.L_02003516:
	adds	r0, #99
	strb	r3, [r0, #0]
	b.n	.L_02003628
.L_0200351c:
	ldr	r3, [pc, #300]
	cmp	r2, r3
	bne.n	.L_020035f2
	cmp	r5, #35
	bne.n	.L_02003534
	cmp	r6, #59
	bne.n	.L_02003534
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #209
	bl 0x0200dd4c
.L_02003534:
	cmp	r5, #45
	bne.n	.L_02003546
	cmp	r6, #105
	bne.n	.L_02003546
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #210
	bl 0x0200dd4c
.L_02003546:
	cmp	r5, #23
	bne.n	.L_02003560
	cmp	r6, #77
	bne.n	.L_02003560
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200dd4c
	movs	r0, #153
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200dd4c
.L_02003560:
	cmp	r6, #59
	bne.n	.L_0200357a
	cmp	r5, #67
	beq.n	.L_02003570
	cmp	r5, #71
	beq.n	.L_02003570
	cmp	r5, #73
	bne.n	.L_0200357a
.L_02003570:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200dd4c
.L_0200357a:
	cmp	r6, #19
	bne.n	.L_02003590
	cmp	r5, #75
	beq.n	.L_02003586
	cmp	r5, #79
	bne.n	.L_02003590
.L_02003586:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200dd4c
.L_02003590:
	cmp	r5, #69
	bne.n	.L_020035a2
	cmp	r6, #59
	bne.n	.L_020035a2
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200dd54
.L_020035a2:
	cmp	r5, #77
	bne.n	.L_020035b4
	cmp	r6, #19
	bne.n	.L_020035b4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200dd54
.L_020035b4:
	movs	r0, #8
	bl 0x0200de2c
	ldr	r5, [r0, #8]
	movs	r0, #8
	bl 0x0200de2c
	ldr	r2, [r0, #16]
	asrs	r5, r5, #20
	adds	r1, r5, #0
	asrs	r2, r2, #20
	movs	r3, #0
	movs	r0, #2
	bl 0x0200862c
	movs	r0, #9
	bl 0x0200de2c
	ldr	r5, [r0, #8]
	movs	r0, #9
	bl 0x0200de2c
	ldr	r2, [r0, #16]
	asrs	r5, r5, #20
	asrs	r2, r2, #20
	movs	r0, #2
	adds	r1, r5, #0
	movs	r3, #0
	bl 0x0200862c
	b.n	.L_02003628
.L_020035f2:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02003628
	cmp	r5, #45
	bne.n	.L_0200360a
	cmp	r6, #33
	bne.n	.L_0200360a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #211
	bl 0x0200dd4c
.L_0200360a:
	movs	r0, #10
	bl 0x0200de2c
	ldr	r5, [r0, #8]
	movs	r0, #10
	bl 0x0200de2c
	ldr	r2, [r0, #16]
	asrs	r5, r5, #20
	asrs	r2, r2, #20
	movs	r0, #2
	adds	r1, r5, #0
	movs	r3, #0
	bl 0x0200862c
.L_02003628:
	bl 0x0200de1c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x00000056
	.4byte 0x00000057
	.4byte 0x00000058
	.4byte 0x0000005b
	.4byte 0x0000005c
	.2byte 0x005d
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #128
	lsls	r0, r0, #4
	sub	sp, #12
	bl 0x0200dd14
	adds	r5, r0, #0
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #3
	adds	r3, #8
	strh	r2, [r3, #0]
	ldr	r0, [pc, #88]
	bl 0x0200dd3c
	adds	r1, r5, #0
	bl 0x0200dd24
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	adds	r0, r5, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl 0x0200dd1c
	ldr	r3, [pc, #48]
	mov	r0, sp
	adds	r0, #10
	strh	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #44]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	bl 0x0200dd14
	adds	r5, r0, #0
	ldr	r0, [pc, #36]
	bl 0x0200dd3c
	adds	r1, r5, #0
	bl 0x0200dd24
	movs	r7, #0
	adds	r4, r5, #0
	b.n	.L_020036fc
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x000001c0
	.4byte 0x84000200
	.4byte 0x06002000
	.4byte 0x81000400
	.2byte 0x01c1
	.2byte 0x0000
.L_020036fc:
	ldrh	r2, [r4, #0]
	movs	r3, #252
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	adds	r7, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, #64
	bne.n	.L_020036fc
	adds	r4, r5, #0
	movs	r7, #0
.L_02003714:
	ldr	r2, [pc, #432]
	lsls	r1, r7, #6
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r7, #1
	adds	r4, #8
	cmp	r7, #16
	bne.n	.L_02003714
	adds	r0, r5, #0
	bl 0x0200dd1c
	movs	r0, #246
	bl 0x0200df94
	movs	r0, #10
	bl 0x0200de2c
	movs	r3, #9
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200de2c
	movs	r3, #1
	adds	r0, #99
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200de2c
	ldr	r3, [pc, #364]
	movs	r7, #0
	str	r3, [r0, #108]
.L_02003764:
	ldr	r3, [pc, #360]
	ldr	r3, [r3, #0]
	mov	r8, r3
	mov	r0, r8
	movs	r3, #3
	ands	r0, r3
	mov	sl, r3
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_02003810
	movs	r0, #10
	bl 0x0200de2c
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200de2c
	adds	r5, r0, #0
	movs	r0, #10
	bl 0x0200de2c
	ldr	r3, [r0, #16]
	movs	r0, #168
	ldr	r2, [r5, #12]
	ldr	r1, [r6, #8]
	lsls	r0, r0, #2
	bl 0x0200dd74
	adds	r5, r0, #0
	movs	r0, #10
	bl 0x0200de2c
	ldr	r3, [r0, #80]
	ldr	r4, [r5, #80]
	ldrb	r3, [r3, #9]
	movs	r1, #12
	ands	r1, r3
	movs	r0, #13
	ldrb	r3, [r4, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	orrs	r3, r1
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	strb	r3, [r4, #9]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	mov	r1, r8
	adds	r3, #85
	strb	r1, [r3, #0]
	ldr	r3, [r5, #8]
	str	r3, [r5, #68]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #72]
	ldr	r3, [r5, #16]
	str	r3, [r5, #76]
	bl 0x0200dcf4
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #48]
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200dd5c
	adds	r0, r5, #0
	ldr	r1, [pc, #220]
	bl 0x0200dd6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200de9c
	ldr	r3, [pc, #200]
	str	r3, [r5, #108]
.L_02003810:
	movs	r0, #1
	adds	r7, #1
	bl 0x0200de0c
	cmp	r7, #45
	bne.n	.L_02003764
	movs	r0, #128
	lsls	r0, r0, #9
.L_02003820:
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200df04
	movs	r0, #60
	bl 0x0200df14
	ldr	r3, [pc, #160]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200decc
	bl 0x0200dee4
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200ded4
	movs	r0, #216
	movs	r1, #1
	movs	r2, #224
	lsls	r2, r2, #16
	negs	r1, r1
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200dedc
	bl 0x0200dee4
	movs	r0, #10
	bl 0x0200de2c
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [pc, #72]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #152
	strh	r3, [r2, #0]
	movs	r3, #42
	strh	r3, [r2, #2]
	movs	r0, #30
	bl 0x0200dce4
	movs	r0, #138
	bl 0x0200df94
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #254
	lsls	r0, r0, #7
	b.n	.L_020038e4
	.4byte 0x00001010
	.4byte 0x00003f41
	.4byte 0x06002000
	.4byte 0x0200acc1
	.4byte 0x0300122c
	.4byte 0x0200dfd8
	.4byte 0x0200ac41
	.4byte 0x02000240
	.2byte 0x1120
	.2byte 0x0300
.L_020038e4:
	movs	r1, #0
	adds	r0, #255
	bl 0x0200df04
	movs	r0, #1
	bl 0x0200df14
	movs	r0, #1
	bl 0x0200dce4
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200df04
	movs	r0, #8
	bl 0x0200df14
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #56]
	movs	r0, #68
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r3, sl
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #84
	movs	r3, #77
	movs	r2, #12
	bl 0x0200dd9c
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ddcc
	b.n	.L_0200395c
	.2byte 0x0100
	.2byte 0x0000
.L_0200395c:
	movs	r7, #0
.L_0200395e:
	ldr	r2, [pc, #32]
	lsrs	r3, r7, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #28]
	movs	r6, #128
	lsls	r6, r6, #19
	orrs	r2, r3
	adds	r6, #82
	strh	r2, [r6, #0]
	movs	r0, #1
	adds	r7, #1
	bl 0x0200dce4
	cmp	r7, #32
	bne.n	.L_0200395e
	b.n	.L_02003988
	.2byte 0x0000
	.4byte 0x00000010
	.2byte 0x1000
	.2byte 0x0000
.L_02003988:
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200df04
	movs	r0, #60
	bl 0x0200df14
	movs	r0, #30
	bl 0x0200dce4
	movs	r0, #148
	bl 0x0200df94
	movs	r0, #216
	lsls	r0, r0, #16
	ldr	r1, [pc, #248]
	bl 0x0200aaa8
	movs	r3, #3
	movs	r2, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #77
	movs	r2, #12
	movs	r0, #68
	movs	r1, #84
	bl 0x0200dd9c
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200ded4
	movs	r1, #1
	movs	r0, #8
	bl 0x0200deec
	bl 0x0200dee4
	movs	r0, #146
	bl 0x0200df94
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200de94
.L_020039f6:
	movs	r0, #8
	bl 0x0200de2c
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200de2c
	ldr	r1, [r0, #16]
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r1, r1, r0
	ldr	r0, [r5, #8]
	bl 0x0200ab58
	movs	r1, #0
	movs	r0, #8
	bl 0x0200de94
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #7
	bl 0x0200de9c
	movs	r0, #3
	bl 0x0200dce4
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200de9c
	movs	r0, #40
	bl 0x0200dce4
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #8
	movs	r2, #0
	bl 0x02009084
	bl 0x0200dd34
	bl 0x0200dd2c
	ldr	r3, [pc, #92]
	movs	r1, #147
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #38
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl 0x0200de04
	ldr	r3, [pc, #52]
	movs	r2, #128
	strh	r3, [r6, #0]
	ldr	r3, [pc, #48]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	movs	r0, #128
	orrs	r3, r2
	ldr	r2, [pc, #44]
	strh	r3, [r1, #0]
	lsls	r0, r0, #4
	movs	r3, #0
	strh	r3, [r2, #0]
	strh	r3, [r2, #2]
	adds	r0, #153
	bl 0x0200dd4c
	bl 0x0200de1c
	add	sp, #12
	b.n	.L_02003abc
	.2byte 0x0000
	.4byte 0x00001008
	.4byte 0x00003f10
	.4byte 0x00000100
	.4byte 0x012d0000
	.4byte 0x02000240
	.2byte 0x1120
	.2byte 0x0300
.L_02003abc:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{lr}
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r0, #216
	movs	r2, #136
	movs	r3, #143
	ldr	r1, [pc, #40]
	lsls	r2, r2, #18
	lsls	r3, r3, #1
	lsls	r0, r0, #16
	bl 0x02008218
	ldr	r3, [pc, #32]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #6
	movs	r2, #0
	bl 0x0200de84
	movs	r0, #3
	bl 0x0200defc
	bl 0x0200de1c
	pop	{pc}
	.4byte 0xffe00000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #108]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	movs	r2, #18
	ldrsh	r3, [r0, r2]
	movs	r2, #199
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	r3, r2
	bgt.n	.L_02003b76
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r0, #186
	bl 0x0200df94
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200de34
	movs	r1, #188
	movs	r2, #167
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200de4c
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl 0x0200de1c
.L_02003b76:
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	adds	r6, r0, #0
	mov	r2, r8
	movs	r0, #144
	lsls	r3, r2, #16
	lsls	r1, r6, #16
	movs	r2, #0
	lsls	r0, r0, #1
	sub	sp, #8
	bl 0x0200dd74
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	strh	r3, [r2, #0]
	adds	r7, r0, #0
	movs	r1, #0
	bl 0x020081d0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r1, [r7, #80]
	movs	r3, #13
	ldrb	r2, [r1, #5]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #5]
	bl 0x0200de14
	movs	r0, #0
	b.n	.L_02003bd8
	.4byte 0x00000010
	.2byte 0x3f44
	.2byte 0x0000
.L_02003bd8:
	bl 0x0200df2c
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #74
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	movs	r0, #106
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl 0x0200df94
	movs	r5, #0
.L_02003bfa:
	ldr	r1, [pc, #32]
.L_02003bfc:
	movs	r3, #128
	lsls	r2, r5, #9
	lsls	r3, r3, #19
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200dce4
	b.n	.L_02003c20
	.2byte 0x0000
	.4byte 0x00003f1f
	.4byte 0x00008000
	.2byte 0x0010
	.2byte 0x0000
.L_02003c20:
	cmp	r5, #8
	bne.n	.L_02003bfa
	movs	r5, #0
.L_02003c26:
	ldr	r2, [pc, #28]
	ldr	r1, [pc, #28]
	movs	r3, #128
	subs	r2, r2, r5
	lsls	r3, r3, #19
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200dce4
	cmp	r5, #16
	bne.n	.L_02003c26
	b.n	.L_02003c4c
	.4byte 0x00000010
	.2byte 0x1000
	.2byte 0x0000
.L_02003c4c:
	mov	r3, r8
	asrs	r6, r6, #4
	asrs	r5, r3, #4
.L_02003c52:
	subs	r5, #1
	adds	r2, r6, #0
	movs	r3, #1
	movs	r1, #2
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r3, r5, #0
	adds	r2, #64
	movs	r0, #65
	movs	r1, #1
	bl 0x0200dd9c
	movs	r3, #255
	adds	r1, r6, #0
	adds	r2, r5, #0
	lsls	r3, r3, #8
	movs	r0, #0
	bl 0x0200862c
	adds	r0, r7, #0
	bl 0x0200dd7c
	bl 0x0200de1c
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	add	sp, #8
	b.n	.L_02003ca4
	.2byte 0x0000
	.2byte 0x0000
.L_02003ca4:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200de2c
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008690
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r5, r5, r3
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	movs	r6, #54
	subs	r3, #201
	lsls	r3, r3, #2
	asrs	r0, r0, #8
	subs	r6, r6, r3
	cmp	r0, #1
	beq.n	.L_02003d20
	cmp	r0, #7
	beq.n	.L_02003d20
	bl 0x0200a790
	movs	r0, #132
	lsls	r1, r6, #3
	lsls	r0, r0, #1
	adds	r1, #10
.L_02003cf8:
	bl 0x0200bb7c
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02003d10
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200dd4c
.L_02003d10:
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r0, r0, r2
	bl 0x0200dd4c
	b.n	.L_02003d38
.L_02003d20:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02003d38
	movs	r0, #132
	lsls	r1, r6, #3
	lsls	r0, r0, #1
	adds	r1, #10
	bl 0x0200bb7c
.L_02003d38:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #8
	bl 0x0200df84
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r4, r3, #20
	ldr	r3, [r0, #12]
	asrs	r7, r3, #20
	ldr	r3, [r0, #16]
	ldr	r0, [pc, #96]
	asrs	r1, r3, #20
	lsls	r3, r4, #16
	adds	r2, r3, r1
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L_02003db4
	str	r2, [r0, #0]
	ldr	r2, [pc, #84]
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #84]
	ldr	r6, [pc, #84]
	mov	r8, r3
	ldr	r5, [pc, #84]
	mov	sl, r2
	movs	r3, #2
.L_02003d7e:
	mov	r2, r8
	adds	r0, r4, #0
	str	r3, [r2, #0]
	adds	r0, #64
	subs	r1, r1, r7
	movs	r3, #73
	movs	r2, #72
	str	r0, [r6, #0]
	str	r1, [r5, #0]
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #2
	movs	r2, #1
	bl 0x0200ddb4
	mov	r3, sl
	mov	r1, r8
	ldr	r2, [r3, #0]
	ldr	r0, [r5, #0]
	ldr	r3, [r1, #0]
	ldr	r1, [r6, #0]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	movs	r0, #70
	movs	r1, #74
	bl 0x0200ddb4
.L_02003db4:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ec0c
	.4byte 0x0200f3dc
	.4byte 0x0200f3e0
	.4byte 0x0200f3e4
	.2byte 0xf3e8
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r5, #19
	movs	r0, #26
	movs	r1, #17
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ddac
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r0, #26
	movs	r1, #17
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ddac
	movs	r5, #1
	movs	r6, #2
	movs	r0, #90
	movs	r1, #17
	movs	r2, #72
	movs	r3, #19
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #90
	movs	r1, #17
	movs	r2, #88
	movs	r3, #19
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r5, #19
	movs	r0, #25
	movs	r1, #17
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ddac
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r0, #25
	movs	r1, #17
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ddac
	movs	r5, #1
	movs	r6, #2
	movs	r0, #89
	movs	r1, #17
	movs	r2, #72
	movs	r3, #19
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	movs	r0, #89
	movs	r1, #17
	movs	r2, #88
	movs	r3, #19
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200dd9c
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #168
	lsls	r0, r0, #2
	bl 0x0200dd74
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
	bl 0x0200de9c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x020081d0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200dd5c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddc4
	ldr	r1, [pc, #8]
	adds	r0, r5, #0
	bl 0x0200dd6c
	pop	{r5, pc}
	.2byte 0xe084
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r0, #0
	movs	r2, #102
	adds	r2, r2, r6
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
	bl 0x0200dd0c
	adds	r2, r6, #0
	adds	r2, #98
	ldrb	r3, [r2, #0]
	movs	r0, #0
	adds	r3, #255
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_02003f84
	ldr	r3, [r6, #76]
	movs	r2, #128
	lsls	r2, r2, #10
	movs	r0, #1
	cmp	r3, r2
	beq.n	.L_02003f84
	adds	r0, r6, #0
	bl 0x0200be7c
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
	bl 0x0200dd0c
	adds	r0, r6, #0
	bl 0x0200be7c
	ldr	r3, [r6, #76]
	movs	r0, #1
	add	r3, r9
	str	r3, [r6, #76]
.L_02003f84:
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
	bl 0x0200dd04
	ldr	r1, [r7, #76]
	ldr	r6, [pc, #116]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x60b8
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	bl 0x0200dcfc
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
	beq.n	.L_0200400c
	ldr	r3, [pc, #72]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200400c
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r0, [r3, #0]
	lsls	r0, r0, #10
	bl 0x0200dcfc
	ldrb	r3, [r5, #0]
	muls	r3, r0
	str	r3, [r7, #76]
	ldrb	r3, [r5, #0]
	adds	r3, #10
	strb	r3, [r5, #0]
.L_0200400c:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_0200402a
	ldrb	r3, [r5, #0]
	cmp	r3, #141
	bne.n	.L_02004028
	adds	r3, r2, #0
	subs	r3, #128
	mov	r1, r8
	strh	r3, [r1, #0]
.L_02004028:
	movs	r0, #1
.L_0200402a:
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
	bl 0x0200dcdc
	str	r0, [r5, #68]
	ldr	r3, [r5, #12]
	ldr	r0, [r6, #12]
	movs	r1, #10
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	bl 0x0200dcdc
	str	r0, [r5, #76]
	ldr	r3, [r5, #16]
	ldr	r0, [r6, #16]
	movs	r1, #10
	subs	r0, r0, r3
	bl 0x0200dcdc
	mov	r3, r8
	str	r0, [r5, #72]
	adds	r5, #98
	strb	r3, [r5, #0]
.L_0200407c:
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
	bne.n	.L_0200412a
	add	r2, sp, #28
	str	r3, [r2, #4]
	movs	r3, #209
	lsls	r3, r3, #1
	adds	r3, #255
	strh	r3, [r2, #24]
	movs	r3, #1
	str	r3, [r2, #0]
	mov	r8, r2
	bl 0x0200dcf4
	movs	r6, #31
	mov	r2, sl
	ldr	r3, [r2, #8]
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #16
	add	r5, sp, #16
	adds	r3, r3, r0
	str	r3, [r5, #0]
	bl 0x0200dcf4
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
	bl 0x020082f4
.L_0200412a:
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
	bl 0x0200de2c
	adds	r7, r0, #0
	mov	r0, fp
	bl 0x0200de2c
	mov	sl, r0
	movs	r0, #78
	bl 0x0200df94
	movs	r0, #30
	bl 0x0200de0c
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	bl 0x0200ded4
	movs	r2, #10
	ldrsh	r0, [r7, r2]
	movs	r3, #14
	ldrsh	r1, [r7, r3]
	movs	r3, #18
	ldrsh	r2, [r7, r3]
	lsls	r1, r1, #16
.L_02004180:
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	bl 0x0200dedc
	movs	r0, #141
	bl 0x0200df94
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200ddcc
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200df0c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200df04
	movs	r0, #60
	bl 0x0200df14
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
	bl 0x0200de3c
	movs	r0, #194
	bl 0x0200df94
	movs	r0, #45
	bl 0x0200de0c
	movs	r1, #128
	mov	r0, r9
	lsls	r1, r1, #1
	bl 0x0200de94
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
	b.n	.L_02004210
	.2byte 0x0000
	.4byte 0x00001008
	.4byte 0x00003f10
	.2byte 0xe0e0
	.2byte 0x0200
.L_02004210:
	movs	r0, #168
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #2
	bl 0x0200dd74
	adds	r5, r0, #0
	movs	r0, #246
	bl 0x0200df94
	bl 0x0200dcf4
	movs	r3, #31
	ldr	r2, [r5, #8]
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r5, #8]
	bl 0x0200dcf4
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
	bl 0x0200de9c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x020081d0
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200dd5c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddc4
	ldr	r1, [pc, #68]
	adds	r0, r5, #0
	bl 0x0200dd6c
	movs	r6, #128
	ldr	r3, [pc, #48]
	ldr	r5, [pc, #48]
	lsls	r6, r6, #19
	adds	r6, #82
	strh	r3, [r6, #0]
	movs	r0, #2
	bl 0x0200dce4
.L_020042a2:
	movs	r0, #2
	strh	r5, [r6, #0]
	bl 0x0200dce4
	ldr	r3, [pc, #32]
	movs	r0, #2
	strh	r3, [r6, #0]
	bl 0x0200dce4
	strh	r5, [r6, #0]
	movs	r0, #2
	bl 0x0200dce4
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	b.n	.L_020042d4
	.4byte 0x00001004
	.4byte 0x0000100a
	.4byte 0x00001010
	.2byte 0xe094
	.2byte 0x0200
.L_020042d4:
	cmp	r3, #16
	bne.n	.L_02004210
	ldr	r3, [pc, #56]
	movs	r0, #30
	strh	r3, [r6, #0]
	bl 0x0200dce4
	movs	r0, #0
	mov	r8, r0
.L_020042e6:
	mov	r0, sl
	ldr	r3, [r0, #16]
	mov	r2, sl
	movs	r0, #168
	ldr	r1, [r2, #8]
	lsls	r0, r0, #2
	ldr	r2, [r2, #12]
	bl 0x0200dd74
	adds	r5, r0, #0
	movs	r0, #195
	bl 0x0200df94
	bl 0x0200dcf4
	ldr	r3, [pc, #16]
	movs	r2, #128
	ands	r0, r3
	lsls	r2, r2, #8
	adds	r6, r5, #0
	adds	r0, r0, r2
	adds	r6, #100
	b.n	.L_0200431c
	.4byte 0x00001008
	.2byte 0x7fff
	.2byte 0x0000
.L_0200431c:
	strh	r0, [r6, #0]
	bl 0x0200dcf4
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
.L_0200435e:
	bl 0x0200de9c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x020081d0
	adds	r0, r5, #0
	movs	r1, #7
	b.n	.L_02004374
	.2byte 0x0000
	.2byte 0x0000
.L_02004374:
	bl 0x0200dd5c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddc4
	adds	r2, r5, #0
	movs	r3, #0
	ldrsh	r1, [r6, r3]
	ldr	r0, [r5, #76]
	adds	r2, #8
	bl 0x0200dd0c
	adds	r0, r5, #0
	ldr	r1, [pc, #208]
	bl 0x0200dd6c
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L_020043c8
	movs	r1, #128
	mov	r0, fp
	lsls	r1, r1, #1
	bl 0x0200de94
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
	bl 0x0200de3c
.L_020043c8:
	movs	r0, #8
	bl 0x0200dce4
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	cmp	r0, #16
	beq.n	.L_020043da
	b.n	.L_020042e6
.L_020043da:
	movs	r0, #220
	bl 0x0200df94
	movs	r0, #16
	bl 0x0200dce4
	movs	r2, #2
	mov	r8, r2
.L_020043ea:
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
	bl 0x0200dd74
	adds	r5, r0, #0
	bl 0x0200dcf4
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	adds	r2, r5, #0
	movs	r3, #128
	adds	r2, #102
	lsls	r3, r3, #4
	strh	r3, [r2, #0]
.L_0200441c:
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
.L_0200443e:
	adds	r3, #85
	strb	r1, [r3, #0]
	subs	r3, #50
	strb	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200de9c
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200dd5c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddc4
	b.n	.L_02004478
	.4byte 0x00000000
	.4byte 0x0200e0b0
	.4byte 0x0200e104
	.2byte 0x0000
	.2byte 0xfff8
.L_02004478:
	.2byte 0x1c28
	ldr	r1, [pc, #168]
	bl 0x0200dd6c
	movs	r0, #2
	add	r8, r0
	mov	r2, r8
	cmp	r2, #32
	bne.n	.L_020043ea
	movs	r0, #220
	bl 0x0200df94
	movs	r0, #50
	bl 0x0200dce4
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200df04
	movs	r0, #8
	bl 0x0200df14
	movs	r0, #16
	bl 0x0200dce4
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200ddcc
	mov	r0, r9
	movs	r1, #0
	bl 0x0200de94
	mov	r0, fp
	movs	r1, #0
	bl 0x0200de94
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200df94
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200df04
	movs	r0, #80
	bl 0x0200df14
	mov	r0, r9
	ldr	r1, [pc, #64]
	bl 0x0200de3c
	ldr	r1, [pc, #64]
	mov	r0, fp
	bl 0x0200de3c
	ldr	r3, [pc, #60]
	mov	r0, sl
	str	r3, [r0, #108]
	movs	r0, #120
	bl 0x0200dce4
	mov	r2, sl
	movs	r0, #195
	str	r6, [r2, #108]
	lsls	r0, r0, #1
	bl 0x0200df94
	bl 0x0200df3c
	movs	r0, #1
	bl 0x0200dce4
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e0c0
	.4byte 0x0200e128
	.4byte 0x0200e158
	.2byte 0xc0b1
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	ldr	r6, [pc, #496]
	movs	r1, #1
	adds	r0, r6, #0
	bl 0x0200ddfc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #212
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_0200455e
	bl 0x0200de1c
	b.n	.L_02004730
.L_0200455e:
	adds	r0, r6, #0
	bl 0x0200dea4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	ldr	r5, [pc, #452]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #179
	movs	r2, #178
	adds	r5, r5, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #51
	adds	r2, #153
	bl 0x0200de34
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #153
	movs	r0, #6
	adds	r1, #51
	bl 0x0200de34
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200de74
	movs	r0, #1
	bl 0x0200de0c
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200de54
	movs	r1, #136
	lsls	r1, r1, #1
	movs	r2, #136
	movs	r0, #6
	bl 0x0200de54
	ldr	r0, [r5, #0]
	bl 0x0200de64
	movs	r0, #6
	bl 0x0200de64
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200deb4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200deb4
	movs	r0, #10
	bl 0x0200de0c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200deac
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200de7c
	movs	r0, #30
	bl 0x0200de0c
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #152
	bl 0x0200de5c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200deb4
	movs	r0, #20
	bl 0x0200de0c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #6
	bl 0x0200deb4
	movs	r0, #20
	bl 0x0200de0c
	movs	r1, #3
	movs	r0, #6
	bl 0x0200de7c
	movs	r0, #30
	bl 0x0200de0c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200deb4
	movs	r0, #20
	bl 0x0200de0c
	movs	r1, #132
	movs	r0, #6
	lsls	r1, r1, #1
	movs	r2, #137
	bl 0x0200de5c
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200deb4
	movs	r0, #20
	bl 0x0200de0c
	adds	r0, r6, #2
	movs	r1, #1
	bl 0x0200ddfc
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200ddcc
	bl 0x0200ddd4
	movs	r0, #8
	movs	r1, #6
	bl 0x0200c138
	movs	r1, #144
	movs	r0, #6
	bl 0x0200de24
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #212
	bl 0x0200dd4c
	movs	r2, #0
	movs	r1, #6
	ldr	r0, [r5, #0]
	bl 0x0200de8c
	movs	r0, #10
	bl 0x0200de0c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200de7c
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200de7c
	movs	r0, #20
	bl 0x0200de0c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #80]
	adds	r2, #153
	bl 0x0200de34
	movs	r0, #6
	movs	r1, #2
	bl 0x0200de7c
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	cmp	r0, #0
	beq.n	.L_02004710
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200de44
.L_02004710:
	movs	r0, #6
	bl 0x0200de64
	movs	r1, #0
	movs	r2, #0
	movs	r0, #6
	bl 0x0200de6c
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #9
	bl 0x0200df44
	bl 0x0200de1c
.L_02004730:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001a8f
	.4byte 0x02000240
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #212
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02004768
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	ldr	r0, [pc, #76]
	movs	r1, #1
	bl 0x0200ddfc
	bl 0x0200de1c
	b.n	.L_020047a6
.L_02004768:
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #107
	bl 0x0200de5c
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #136
	bl 0x0200de5c
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200de5c
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200deb4
	movs	r0, #10
	bl 0x0200de0c
	bl 0x0200c534
.L_020047a6:
	pop	{r5, pc}
	.4byte 0x00001a8f
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #212
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_020047d8
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	ldr	r0, [pc, #68]
	movs	r1, #1
	bl 0x0200ddfc
	bl 0x0200de1c
	b.n	.L_0200480e
.L_020047d8:
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #140
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200de5c
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200de5c
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200deb4
	movs	r0, #10
	bl 0x0200de0c
	bl 0x0200c534
.L_0200480e:
	pop	{r5, pc}
	.4byte 0x00001a8f
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #212
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02004840
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	ldr	r0, [pc, #68]
	movs	r1, #1
	bl 0x0200ddfc
	bl 0x0200de1c
	b.n	.L_02004874
.L_02004840:
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #136
	bl 0x0200de5c
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200de5c
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200deb4
	movs	r0, #10
	bl 0x0200de0c
	bl 0x0200c534
.L_02004874:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001a8f
	.4byte 0x02000240
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r0, #8
	bl 0x0200de2c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #19
	cmp	r3, #65
	bne.n	.L_020048aa
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200dd4c
	b.n	.L_020048b4
.L_020048aa:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200dd54
.L_020048b4:
	bl 0x0200de1c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x020087b4
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #456]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	sub	sp, #4
	bl 0x0200de2c
	ldr	r6, [pc, #440]
	movs	r1, #255
	ldrh	r3, [r6, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	movs	r7, #0
	mov	r9, r0
	cmp	r3, r1
	beq.n	.L_02004938
.L_020048f4:
	ldrh	r5, [r6, #0]
	adds	r0, r5, #0
	bl 0x0200de2c
	mov	r1, r9
	ldr	r2, [r0, #8]
	ldr	r3, [r1, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	mov	r8, r0
	cmp	r2, r3
	bne.n	.L_0200492a
	ldr	r2, [r0, #16]
	ldr	r3, [r1, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_0200492a
	movs	r0, #1
	ands	r0, r5
	lsls	r0, r0, #1
	subs	r0, r5, r0
	adds	r0, #1
	bl 0x0200de2c
	adds	r7, r0, #0
	b.n	.L_02004938
.L_0200492a:
	adds	r6, #6
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020048f4
.L_02004938:
	mov	r3, r8
	adds	r1, r7, #0
	adds	r3, #99
	adds	r1, #99
	ldrb	r2, [r3, #0]
	ldrb	r3, [r1, #0]
	adds	r1, r3, #0
	orrs	r1, r2
	mov	sl, r1
	cmp	r1, #0
	beq.n	.L_02004950
	b.n	.L_02004a8c
.L_02004950:
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	adds	r3, r7, #0
	mov	r1, sl
	mov	r2, r8
	adds	r3, #85
	ldr	r6, [r2, #80]
	ldr	r5, [r7, #80]
	strb	r1, [r3, #0]
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #308]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	bl 0x0200dce4
	ldrb	r3, [r5, #16]
	ldr	r1, [pc, #296]
	lsls	r3, r3, #2
	adds	r4, r3, r1
	ldrh	r3, [r4, #2]
	ldr	r2, [pc, #292]
	mov	r0, sp
	adds	r3, r3, r2
	str	r3, [r7, #104]
	ldrb	r3, [r6, #16]
	lsls	r3, r3, #2
	adds	r4, r3, r1
	ldrh	r3, [r4, #2]
	mov	r1, r8
	adds	r3, r3, r2
	mov	r2, sl
	str	r2, [r0, #0]
	ldrh	r2, [r4, #0]
	str	r3, [r1, #104]
	movs	r4, #133
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [r7, #104]
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	mov	r1, r8
	lsls	r2, r2, #24
	ldr	r0, [r1, #104]
	adds	r2, #192
	ldr	r1, [r7, #104]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	ldr	r3, [r2, #104]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r1, r8
	str	r3, [r1, #104]
	ldr	r3, [r7, #104]
	mov	r6, r9
	adds	r3, r3, r2
	adds	r6, #85
	mov	r2, sl
	str	r3, [r7, #104]
	strb	r2, [r6, #0]
	movs	r5, #0
.L_020049de:
	cmp	r5, #4
	bhi.n	.L_02004a12
	ldr	r3, [r7, #12]
	ldr	r1, [pc, #196]
	mov	r2, r8
	adds	r3, r3, r1
	str	r3, [r7, #12]
	ldr	r0, [r2, #104]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [r7, #104]
	adds	r2, #48
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r1, r8
	ldr	r3, [r1, #104]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r1, #104]
	ldr	r3, [r7, #104]
	adds	r3, r3, r2
	str	r3, [r7, #104]
.L_02004a12:
	mov	r2, r9
	ldr	r3, [r2, #12]
	ldr	r1, [pc, #152]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r2, #12]
	str	r3, [r2, #20]
	mov	r2, r8
	ldr	r3, [r2, #28]
	ldr	r1, [pc, #140]
	adds	r5, #1
	adds	r3, r3, r1
	str	r3, [r2, #28]
	bl 0x0200dce4
	cmp	r5, #8
	bne.n	.L_020049de
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #9
	str	r3, [r2, #28]
	ldr	r3, [r7, #20]
	str	r3, [r7, #12]
	mov	r3, r8
	adds	r3, #100
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200dd4c
	adds	r3, r7, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200dd54
	mov	r1, r8
	ldr	r3, [r1, #80]
	mov	r0, r8
	ldrb	r1, [r3, #24]
	adds	r1, #1
	bl 0x0200dd5c
	ldr	r3, [r7, #80]
	adds	r0, r7, #0
	ldrb	r1, [r3, #24]
	subs	r1, #1
	bl 0x0200dd5c
	mov	r0, r8
	bl 0x020086cc
	adds	r0, r7, #0
	bl 0x020086cc
	movs	r0, #10
	bl 0x0200de0c
	bl 0x0200de1c
.L_02004a8c:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200e21c
	.4byte 0xffe40000
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x00053333
	.4byte 0xfffc8000
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #170
	lsls	r1, r1, #1
	adds	r1, r1, r3
	movs	r2, #0
	ldrsh	r3, [r1, r2]
.L_02004ace:
	movs	r7, #0
	movs	r6, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_02004ba6
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r0, #186
	bl 0x0200df94
	ldr	r5, [pc, #192]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	ldr	r0, [r5, #0]
	bl 0x0200de34
	mov	r2, r8
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	subs	r3, #50
	cmp	r3, #5
	bhi.n	.L_02004b80
	ldr	r2, [pc, #148]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200cb3c
	.4byte 0x0200cb58
	.4byte 0x0200cb62
	.4byte 0x0200cb6c
	.4byte 0x0200cb76
	.4byte 0x0200cb7a
	.4byte 0x23854d1b
	.4byte 0x18ed009b
	.4byte 0xf0016828
	.4byte 0x269ef971
	.4byte 0xf0016828
	.4byte 0x2794f96d
	.4byte 0xe01300b6
	.4byte 0x26a32784
	.4byte 0x00b6007f
	.4byte 0x27fce00e
	.4byte 0x007f26fe
	.4byte 0xe0090076
	.4byte 0x26fe278a
	.4byte 0x007600bf
	.4byte 0x279ae004
	.4byte 0x27a2e000
	.2byte 0x00bf
	.2byte 0x26bc
.L_02004b80:
	ldr	r5, [pc, #40]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	adds	r2, r6, #0
	ldr	r0, [r5, #0]
	adds	r1, r7, #0
	bl 0x0200de4c
	ldr	r0, [r5, #0]
	bl 0x0200de2c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl 0x0200de1c
.L_02004ba6:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xcb24
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	ldr	r5, [pc, #56]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200ddfc
	movs	r0, #126
	bl 0x0200df94
	movs	r0, #186
	lsls	r0, r0, #2
	movs	r1, #0
	adds	r0, #255
	bl 0x0200df34
	movs	r0, #10
	bl 0x0200de0c
	movs	r0, #161
	lsls	r0, r0, #1
	adds	r5, #1
	bl 0x0200dd54
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddfc
	bl 0x0200de1c
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1a92
	.2byte 0x0000
	.global Func_02004c00
	.thumb_func
Func_02004c00:
	push	{lr}
	ldr	r3, [pc, #112]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_02004c18
	ldr	r0, [pc, #100]
	b.n	.L_02004c70
.L_02004c18:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	beq.n	.L_02004c6e
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_02004c28
	ldr	r0, [pc, #96]
	b.n	.L_02004c70
.L_02004c28:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_02004c32
	ldr	r0, [pc, #96]
	b.n	.L_02004c70
.L_02004c32:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_02004c3c
	ldr	r0, [pc, #92]
	b.n	.L_02004c70
.L_02004c3c:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02004c46
	ldr	r0, [pc, #92]
	b.n	.L_02004c70
.L_02004c46:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02004c50
	ldr	r0, [pc, #88]
	b.n	.L_02004c70
.L_02004c50:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02004c5a
	ldr	r0, [pc, #88]
	b.n	.L_02004c70
.L_02004c5a:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02004c64
	ldr	r0, [pc, #84]
	b.n	.L_02004c70
.L_02004c64:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02004c6e
	ldr	r0, [pc, #84]
	b.n	.L_02004c70
.L_02004c6e:
	ldr	r0, [pc, #84]
.L_02004c70:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x0200ec10
	.4byte 0x00000056
	.4byte 0x00000057
	.4byte 0x0200ee2c
	.4byte 0x00000058
	.4byte 0x0200eeec
	.4byte 0x00000059
	.4byte 0x0200efa0
	.4byte 0x0000005a
	.4byte 0x0200f06c
	.4byte 0x0000005b
	.4byte 0x0200f0fc
	.4byte 0x0000005c
	.4byte 0x0200f228
	.4byte 0x0000005d
	.4byte 0x0200f318
	.4byte 0x0000005e
	.4byte 0x0200f3a8
	.2byte 0xed6c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #452]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl 0x0200de2c
	adds	r7, r0, #0
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r2, #35
	adds	r2, r2, r7
	ldrb	r3, [r2, #0]
	mov	r8, r2
	str	r3, [sp, #0]
	movs	r0, #216
	movs	r2, #136
	movs	r3, #143
	lsls	r2, r2, #18
	lsls	r3, r3, #1
	ldr	r1, [pc, #408]
	lsls	r0, r0, #16
	bl 0x02008218
	movs	r1, #85
	adds	r1, r1, r7
	movs	r5, #0
	strb	r5, [r1, #0]
	mov	fp, r0
	mov	sl, r1
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200ddbc
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	ldr	r3, [pc, #372]
	movs	r0, #140
	adds	r2, r2, r3
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #1
	bl 0x0200dd74
	movs	r1, #2
	adds	r6, r0, #0
	bl 0x0200dd5c
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200ddbc
	adds	r3, r6, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r3, #179
	lsls	r3, r3, #9
	adds	r3, #102
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	ldr	r2, [r7, #12]
	movs	r3, #160
	lsls	r3, r3, #15
	ldr	r1, [r7, #8]
	adds	r2, r2, r3
	adds	r0, r7, #0
	ldr	r3, [r7, #16]
	bl 0x0200dd8c
	ldr	r2, [r6, #12]
	movs	r3, #144
	lsls	r3, r3, #15
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	bl 0x0200dd8c
	movs	r0, #137
	bl 0x0200df94
	bl 0x0200df1c
	b.n	.L_02004dac
.L_02004d8c:
	ldr	r3, [pc, #280]
	mov	r1, r8
	ldr	r2, [r3, #0]
	ldrb	r3, [r3, #0]
	movs	r0, #1
	lsls	r3, r3, #12
	strh	r3, [r7, #6]
	movs	r3, #1
	ands	r2, r3
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	eors	r3, r2
	strb	r3, [r1, #0]
	bl 0x0200dce4
.L_02004dac:
	adds	r0, r7, #0
	bl 0x0200ddec
	cmp	r0, #0
	beq.n	.L_02004d8c
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200df94
	movs	r0, #10
	bl 0x0200de0c
	ldr	r5, [pc, #212]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200deb4
	ldr	r3, [pc, #208]
	ldr	r2, [r6, #16]
	mov	r9, r3
	add	r2, r9
	ldr	r1, [r6, #8]
	movs	r0, #0
	bl 0x0200dda4
	movs	r1, #6
	str	r0, [r7, #12]
	str	r0, [r7, #20]
	adds	r0, r7, #0
	bl 0x0200dd5c
	movs	r0, #6
	bl 0x0200dce4
	movs	r0, #152
	bl 0x0200df94
	adds	r0, r7, #0
	movs	r1, #7
	bl 0x0200dd5c
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	mov	r1, sl
	movs	r3, #2
	movs	r2, #35
	strb	r3, [r1, #0]
	adds	r2, r2, r7
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [r7, #16]
	ldr	r1, [r7, #8]
	add	r3, r9
	mov	r8, r2
	adds	r0, r7, #0
	ldr	r2, [r7, #12]
	bl 0x0200dd8c
	adds	r0, r7, #0
	bl 0x02008674
	adds	r0, r7, #0
	bl 0x0200dd94
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200de7c
	movs	r1, #5
	adds	r0, r6, #0
	bl 0x0200dd5c
	movs	r0, #12
	bl 0x0200dce4
	adds	r0, r6, #0
	bl 0x0200dd7c
	mov	r1, sl
	movs	r3, #3
	strb	r3, [r1, #0]
	ldr	r3, [r7, #12]
	movs	r1, #1
	str	r3, [r7, #20]
	adds	r0, r7, #0
	bl 0x0200ddbc
	mov	r2, sp
	ldrb	r2, [r2, #0]
	mov	r3, r8
	strb	r2, [r3, #0]
	mov	r0, fp
	bl 0x0200dd7c
	movs	r0, #1
	bl 0x0200dce4
	bl 0x0200df24
	bl 0x0200de1c
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xffe00000
	.4byte 0xfff80000
	.4byte 0x0300122c
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb5e0
	movs	r6, #0
.L_02004eb4:
	movs	r0, #168
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	lsls	r0, r0, #2
	bl 0x0200dd74
	adds	r5, r0, #0
	ldr	r1, [r5, #80]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	adds	r2, r5, #0
	strb	r3, [r1, #9]
	adds	r2, #99
	lsls	r3, r6, #4
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	movs	r7, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r2, #128
	subs	r3, #50
	strb	r7, [r3, #0]
	lsls	r2, r2, #8
	lsls	r3, r6, #12
	adds	r3, r3, r2
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	ldr	r3, [pc, #64]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #0
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200de9c
	movs	r2, #1
	ands	r2, r6
	adds	r1, r5, #0
	adds	r0, r5, #0
	adds	r1, #100
	adds	r0, #102
	adds	r6, #1
	cmp	r2, #0
	beq.n	.L_02004f24
	movs	r3, #212
	strh	r3, [r1, #0]
	strh	r7, [r0, #0]
	b.n	.L_02004f2a
.L_02004f24:
	movs	r3, #116
	strh	r3, [r1, #0]
	strh	r2, [r0, #0]
.L_02004f2a:
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ddc4
	cmp	r6, #8
	bne.n	.L_02004eb4
	pop	{r5, r6, r7, pc}
	.2byte 0x8551
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #8
	bl 0x0200df84
	bl 0x0200de2c
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r0, #12]
	lsls	r2, r2, #14
	adds	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #17
	movs	r6, #2
	cmp	r3, r2
	bge.n	.L_02004f72
	movs	r2, #216
	lsls	r2, r2, #16
	movs	r6, #1
	cmp	r3, r2
	bge.n	.L_02004f72
	movs	r6, #0
.L_02004f72:
	ldr	r3, [pc, #140]
	ldr	r3, [r3, #0]
	cmp	r3, r6
	beq.n	.L_02004ffc
	cmp	r6, #1
	beq.n	.L_02004faa
	cmp	r6, #1
	bgt.n	.L_02004f88
	cmp	r6, #0
	beq.n	.L_02004fd2
	b.n	.L_02004ff8
.L_02004f88:
	cmp	r6, #2
	bne.n	.L_02004ff8
	movs	r3, #7
	movs	r5, #39
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #6
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200ddac
	movs	r3, #26
	str	r3, [sp, #0]
	movs	r0, #70
	movs	r1, #64
	b.n	.L_02004fc6
.L_02004faa:
	movs	r3, #7
	movs	r5, #39
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #68
	movs	r2, #6
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200ddac
	movs	r3, #26
	str	r3, [sp, #0]
	movs	r0, #70
	movs	r1, #68
.L_02004fc6:
	movs	r2, #6
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200ddac
	b.n	.L_02004ff8
.L_02004fd2:
	movs	r3, #7
	movs	r5, #39
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #72
	movs	r2, #6
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200ddac
	movs	r3, #26
	str	r3, [sp, #0]
	movs	r0, #70
	movs	r1, #68
	movs	r2, #6
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200ddac
.L_02004ff8:
	ldr	r3, [pc, #4]
	str	r6, [r3, #0]
.L_02004ffc:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0xf3d8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
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
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200dcdc
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02005034
	adds	r3, #15
.L_02005034:
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
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #380]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200de2c
.L_02005074:
	adds	r7, r0, #0
	bl 0x0200de14
	movs	r0, #0
	bl 0x0200df2c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200dedc
	bl 0x0200dd84
	movs	r0, #1
	bl 0x0200dce4
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl 0x0200df1c
	bl 0x0200df24
	movs	r0, #204
	bl 0x0200df94
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200de0c
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #260]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_020050f6:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200dd04
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200dcfc
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200dcf4
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #192]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200dcf4
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #180]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
.L_02005146:
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
.L_0200514c:
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #160]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x020082f4
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020050f6
	movs	r0, #188
	bl 0x0200df94
	ldr	r5, [pc, #116]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200dec4
	ldr	r0, [r5, #0]
	movs	r1, #22
	bl 0x0200de7c
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ddcc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200ddcc
	bl 0x0200ddd4
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200dec4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200de1c
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200d005
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	.global Func_020051f8
	.thumb_func
Func_020051f8:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	ldr	r5, [pc, #208]
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	adds	r2, #16
	adds	r6, r5, r2
	ldr	r0, [r6, #0]
	sub	sp, #8
	bl 0x0200de2c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200dd4c
	movs	r0, #244
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200dd4c
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #156]
	cmp	r2, r3
	beq.n	.L_02005248
	b.n	.L_02005424
.L_02005248:
	bl 0x0200a770
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #16
	bne.n	.L_02005282
	movs	r0, #10
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02005282
	ldr	r0, [r6, #0]
	bl 0x0200de2c
	movs	r3, #1
	adds	r0, #34
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200dce4
	bl 0x0200dd84
	movs	r0, #1
	bl 0x0200dce4
.L_02005282:
	ldr	r3, [pc, #88]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #11
	ble.n	.L_02005296
	cmp	r3, #16
	bne.n	.L_020052b4
.L_02005296:
	movs	r1, #144
	ldr	r0, [pc, #72]
	lsls	r1, r1, #3
	bl 0x0200dcec
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	bl 0x0200ceb0
.L_020052b4:
	ldr	r3, [pc, #36]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #13
	ble.n	.L_020052fc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #142
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_020052f6
	b.n	.L_020052e8
	.4byte 0x00001008
	.4byte 0x00003f10
	.4byte 0x02000240
	.4byte 0x00000055
	.2byte 0xcf3d
	.2byte 0x0200
.L_020052e8:
	movs	r1, #206
	movs	r2, #164
	movs	r0, #12
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200de6c
.L_020052f6:
	ldr	r0, [pc, #292]
	bl 0x0200df6c
.L_020052fc:
	movs	r0, #10
	bl 0x0200de2c
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #10
	bl 0x0200de2c
	str	r5, [r0, #12]
	movs	r0, #10
	bl 0x0200de2c
	movs	r3, #2
	adds	r0, #35
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #10
	bl 0x0200debc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #154
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02005340
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	b.n	.L_0200536e
.L_02005340:
	movs	r0, #10
	bl 0x0200de2c
	movs	r2, #0
	movs	r1, #8
	bl 0x02009084
	movs	r0, #11
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	movs	r3, #3
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #78
	movs	r2, #18
	movs	r3, #105
	bl 0x0200dd9c
.L_0200536e:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #153
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_020053a2
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	movs	r0, #9
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	movs	r0, #12
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	b.n	.L_020053cc
.L_020053a2:
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #180
	movs	r2, #0
	bl 0x02009084
	movs	r0, #9
	bl 0x0200de2c
	movs	r1, #60
	movs	r2, #0
	bl 0x02009084
	movs	r0, #12
	bl 0x0200de2c
	movs	r1, #60
	movs	r2, #0
	bl 0x02009420
.L_020053cc:
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200ddbc
	movs	r0, #9
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200ddbc
	movs	r0, #12
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r3, [pc, #44]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #11
	beq.n	.L_02005404
	bl 0x0200dca4
.L_02005404:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005414
	bl 0x0200dca4
.L_02005414:
	bl 0x0200d05c
	bl 0x0200dca4
	b.n	.L_020054c0
	.2byte 0x0200
	.2byte 0x0240
	.2byte 0x0200
.L_02005424:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02005510
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	subs	r2, #2
	strh	r3, [r2, #0]
	bl 0x0200a770
	movs	r0, #243
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005468
	movs	r1, #164
	movs	r2, #196
.L_02005450:
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200de6c
	b.n	.L_02005468
	.4byte 0x00001008
	.4byte 0x00003f10
	.2byte 0x0056
	.2byte 0x0000
.L_02005468:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #152
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005484
	movs	r1, #208
	movs	r2, #196
	movs	r0, #11
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200de6c
.L_02005484:
	ldr	r0, [pc, #128]
	bl 0x0200df6c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #153
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_020054a6
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	b.n	.L_020054d4
.L_020054a6:
	movs	r0, #8
	bl 0x0200de2c
	movs	r2, #0
	movs	r1, #60
.L_020054b0:
	bl 0x02009084
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
.L_020054c0:
	movs	r3, #3
	movs	r2, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #68
	movs	r1, #84
	movs	r2, #12
	movs	r3, #77
	bl 0x0200dd9c
.L_020054d4:
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200ddbc
	ldr	r3, [pc, #40]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #3
	beq.n	.L_020054f2
	b.n	.L_02005ca4
.L_020054f2:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005500
	b.n	.L_02005ca4
.L_02005500:
	bl 0x0200ccc8
	b.n	.L_02005ca4
	.2byte 0x0000
	.4byte 0x0200e054
	.2byte 0x0240
	.2byte 0x0200
.L_02005510:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	beq.n	.L_02005518
	b.n	.L_02005644
.L_02005518:
	bl 0x0200a770
	movs	r0, #245
	bl 0x0200df4c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	movs	r1, #144
	strh	r3, [r2, #0]
	ldr	r0, [pc, #48]
	lsls	r1, r1, #3
	bl 0x0200dcec
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #155
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_0200556c
	movs	r1, #132
	movs	r2, #158
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200de6c
	b.n	.L_0200556c
	.2byte 0x0000
	.4byte 0x00000c08
	.4byte 0x00003f10
	.4byte 0x00000057
	.2byte 0x9691
	.2byte 0x0200
.L_0200556c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #156
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005588
	movs	r1, #154
	movs	r2, #142
	movs	r0, #11
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200de6c
.L_02005588:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #157
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_020055a4
	movs	r1, #236
	movs	r2, #130
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200de6c
.L_020055a4:
	ldr	r0, [pc, #804]
	bl 0x0200df6c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #153
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_020055de
	movs	r0, #9
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	movs	r0, #13
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	b.n	.L_02005608
.L_020055de:
	movs	r0, #9
	bl 0x0200de2c
	movs	r1, #60
	movs	r2, #0
	bl 0x02009084
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #180
	movs	r2, #0
	bl 0x02009084
	movs	r0, #13
	bl 0x0200de2c
	movs	r1, #200
	movs	r2, #0
	bl 0x02009084
.L_02005608:
	movs	r0, #9
	bl 0x0200de2c
	movs	r5, #0
	adds	r0, #90
	strb	r5, [r0, #0]
	movs	r0, #10
	bl 0x0200de2c
	adds	r0, #90
	strb	r5, [r0, #0]
	movs	r0, #13
	bl 0x0200de2c
	adds	r0, #90
	strb	r5, [r0, #0]
	movs	r0, #9
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200ddbc
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200ddbc
	movs	r0, #13
	b.n	.L_02005bc6
.L_02005644:
	ldr	r3, [pc, #648]
	cmp	r2, r3
	beq.n	.L_0200564c
	b.n	.L_020057bc
.L_0200564c:
	bl 0x0200df54
	movs	r1, #11
	movs	r2, #12
	movs	r0, #0
	bl 0x0200df5c
	movs	r1, #13
	movs	r2, #14
	movs	r0, #1
	bl 0x0200df5c
	movs	r1, #15
	movs	r2, #16
	movs	r0, #2
	bl 0x0200df5c
	movs	r1, #17
	movs	r2, #18
	movs	r0, #3
	bl 0x0200df5c
	movs	r1, #19
	movs	r2, #20
	movs	r0, #4
	bl 0x0200df5c
	movs	r0, #18
	bl 0x0200de2c
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_020056ee
	movs	r3, #5
	movs	r2, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #69
	movs	r1, #64
	movs	r2, #29
	movs	r3, #85
	bl 0x0200dd9c
	movs	r3, #29
	movs	r2, #21
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #46
	movs	r1, #20
	movs	r2, #5
	movs	r3, #5
	bl 0x0200ddac
	movs	r3, #31
	movs	r2, #89
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r1, #89
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r3, #33
	movs	r2, #87
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #86
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	b.n	.L_0200573e
.L_020056ee:
	movs	r3, #5
	movs	r2, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #29
	movs	r3, #85
	bl 0x0200dd9c
	movs	r3, #29
	movs	r2, #21
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #52
	movs	r1, #20
	movs	r2, #5
	movs	r3, #5
	bl 0x0200ddac
	movs	r3, #33
	movs	r2, #87
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #88
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r3, #31
	movs	r2, #89
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #48
	movs	r1, #88
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
.L_0200573e:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005798
	movs	r3, #4
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #90
	movs	r1, #25
	movs	r2, #86
	movs	r3, #25
	bl 0x0200dd9c
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #21
	movs	r1, #88
	movs	r2, #24
	movs	r3, #89
	bl 0x0200dd9c
	movs	r3, #89
	movs	r5, #23
	str	r3, [sp, #4]
	movs	r0, #23
	movs	r1, #92
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	movs	r3, #25
	str	r3, [sp, #4]
	movs	r0, #18
	movs	r1, #33
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
.L_02005798:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #158
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_020057b4
	movs	r1, #162
	movs	r2, #148
	movs	r0, #9
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200de6c
.L_020057b4:
	ldr	r0, [pc, #284]
	bl 0x0200df6c
	b.n	.L_02005ca4
.L_020057bc:
	ldr	r3, [pc, #280]
	cmp	r2, r3
	bne.n	.L_0200583e
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200dd4c
	bl 0x0200a770
	movs	r6, #0
.L_020057d0:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #2
	adds	r0, r6, r2
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005808
	lsls	r3, r6, #1
	movs	r5, #24
	subs	r5, r5, r3
	movs	r2, #2
	movs	r3, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #65
	movs	r1, #1
	movs	r2, #80
	adds	r3, r5, #0
	bl 0x0200dd9c
	movs	r3, #255
	movs	r0, #0
	movs	r1, #16
	adds	r2, r5, #0
	lsls	r3, r3, #8
	bl 0x0200862c
.L_02005808:
	adds	r6, #1
	cmp	r6, #6
	bne.n	.L_020057d0
	movs	r0, #8
	bl 0x0200de2c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200de2c
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200de2c
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r0, #20]
	str	r3, [r5, #12]
	movs	r0, #8
	bl 0x0200de2c
	movs	r1, #9
	bl 0x0200df44
	b.n	.L_02005ca4
.L_0200583e:
	ldr	r3, [pc, #156]
	cmp	r2, r3
	beq.n	.L_02005846
	b.n	.L_0200599e
.L_02005846:
	bl 0x0200a770
	bl 0x0200df54
	movs	r1, #11
	movs	r2, #12
	movs	r0, #0
	bl 0x0200df5c
	movs	r1, #13
	movs	r2, #14
	movs	r0, #1
	bl 0x0200df5c
	movs	r0, #2
	movs	r1, #15
	movs	r2, #16
	bl 0x0200df5c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_020058e0
	movs	r3, #5
	movs	r2, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #69
	movs	r1, #64
	movs	r2, #32
	movs	r3, #79
	bl 0x0200dd9c
	movs	r3, #32
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #49
	movs	r1, #15
	movs	r2, #5
	movs	r3, #5
	bl 0x0200ddac
	movs	r3, #34
	movs	r2, #82
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #82
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r3, #36
	movs	r2, #81
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #52
	movs	r1, #81
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	b.n	.L_02005930
	.4byte 0x0200e05a
	.4byte 0x00000058
	.4byte 0x0200e064
	.4byte 0x00000059
	.2byte 0x005a
	.2byte 0x0000
.L_020058e0:
	movs	r3, #5
	movs	r2, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #32
	movs	r3, #79
	bl 0x0200dd9c
	movs	r3, #32
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #49
	movs	r1, #21
	movs	r2, #5
	movs	r3, #5
	bl 0x0200ddac
	movs	r3, #36
	movs	r2, #81
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #81
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	movs	r3, #34
	movs	r2, #82
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #82
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
.L_02005930:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_0200594c
	movs	r1, #130
	movs	r2, #148
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200de6c
.L_0200594c:
	movs	r0, #244
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_0200595c
	b.n	.L_02005ca4
.L_0200595c:
	movs	r1, #156
	movs	r2, #148
	lsls	r2, r2, #17
	movs	r0, #8
	lsls	r1, r1, #17
	bl 0x0200de6c
	movs	r1, #2
	movs	r0, #8
	bl 0x0200debc
	movs	r0, #8
	bl 0x0200de2c
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200de2c
	ldr	r3, [pc, #288]
	movs	r2, #18
	str	r3, [r0, #20]
	movs	r3, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #19
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ddac
	b.n	.L_02005ca4
.L_0200599e:
	ldr	r3, [pc, #268]
	cmp	r2, r3
	beq.n	.L_020059a6
	b.n	.L_02005ab8
.L_020059a6:
	bl 0x0200a770
	movs	r0, #12
	bl 0x0200de2c
	movs	r6, #1
	adds	r0, #98
	strb	r6, [r0, #0]
	movs	r0, #13
	bl 0x0200de2c
	adds	r0, #98
	strb	r6, [r0, #0]
	movs	r0, #14
	bl 0x0200de2c
	adds	r0, #98
	strb	r6, [r0, #0]
	movs	r0, #15
	bl 0x0200de2c
	adds	r0, #98
	strb	r6, [r0, #0]
	movs	r0, #10
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_020059f4
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200dd4c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200dd4c
.L_020059f4:
	ldr	r0, [pc, #184]
	bl 0x02008908
	movs	r0, #141
	lsls	r0, r0, #4
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005a14
	movs	r1, #172
	movs	r2, #174
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200de6c
.L_02005a14:
	ldr	r0, [pc, #156]
	bl 0x0200df6c
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005a5e
	movs	r3, #3
	str	r3, [sp, #4]
	movs	r0, #39
	movs	r1, #82
	movs	r2, #38
	movs	r3, #82
	str	r6, [sp, #0]
	bl 0x0200dd9c
	movs	r3, #82
	movs	r5, #38
	str	r3, [sp, #4]
	movs	r0, #39
	movs	r1, #82
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	movs	r3, #18
	str	r3, [sp, #4]
	movs	r0, #38
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
.L_02005a5e:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02005a6e
	b.n	.L_02005ca4
.L_02005a6e:
	movs	r3, #3
	str	r3, [sp, #4]
	movs	r0, #39
	movs	r1, #82
	movs	r2, #38
	movs	r3, #99
	str	r6, [sp, #0]
	bl 0x0200dd9c
	movs	r3, #99
	movs	r5, #38
	str	r3, [sp, #4]
	movs	r0, #38
	movs	r1, #98
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	movs	r3, #37
	str	r3, [sp, #4]
	movs	r0, #38
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	b.n	.L_02005ca4
	.4byte 0xffe00000
	.4byte 0x0000005b
	.4byte 0x0200e21c
	.2byte 0xe068
	.2byte 0x0200
.L_02005ab8:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	beq.n	.L_02005ac0
	b.n	.L_02005bd2
.L_02005ac0:
	movs	r0, #245
	bl 0x0200df4c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	movs	r1, #144
	strh	r3, [r2, #0]
	ldr	r0, [pc, #48]
	lsls	r1, r1, #3
	bl 0x0200dcec
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #209
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005b10
	movs	r1, #140
	movs	r2, #236
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200de6c
	b.n	.L_02005b10
	.2byte 0x0000
	.4byte 0x00000c08
	.4byte 0x00003f10
	.4byte 0x0000005c
	.2byte 0x9691
	.2byte 0x0200
.L_02005b10:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #210
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005b2c
	movs	r1, #180
	movs	r2, #210
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200de6c
.L_02005b2c:
	movs	r0, #153
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005b50
	movs	r1, #184
	movs	r2, #154
	movs	r0, #12
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200de6c
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200dd4c
.L_02005b50:
	movs	r1, #2
	movs	r0, #9
	bl 0x0200debc
	ldr	r0, [pc, #336]
	bl 0x0200df6c
	movs	r0, #8
	bl 0x0200de2c
	ldr	r5, [r0, #8]
	movs	r0, #8
	bl 0x0200de2c
	ldr	r2, [r0, #16]
	asrs	r5, r5, #20
	adds	r1, r5, #0
	asrs	r2, r2, #20
	movs	r3, #0
	movs	r0, #2
	bl 0x0200862c
	movs	r0, #9
	bl 0x0200de2c
	ldr	r5, [r0, #8]
	movs	r0, #9
	bl 0x0200de2c
	ldr	r2, [r0, #16]
	asrs	r5, r5, #20
	asrs	r2, r2, #20
	movs	r0, #2
	adds	r1, r5, #0
	movs	r3, #0
	bl 0x0200862c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #153
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02005bb6
	movs	r0, #11
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	b.n	.L_02005bc4
.L_02005bb6:
	movs	r0, #11
	bl 0x0200de2c
	movs	r1, #8
	movs	r2, #0
	bl 0x02009084
.L_02005bc4:
	movs	r0, #11
.L_02005bc6:
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200ddbc
	b.n	.L_02005ca4
.L_02005bd2:
	ldr	r3, [pc, #220]
	cmp	r2, r3
	bne.n	.L_02005ca4
	bl 0x0200a770
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #211
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005bf8
	movs	r1, #180
	movs	r2, #132
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200de6c
.L_02005bf8:
	ldr	r0, [pc, #184]
	bl 0x0200df6c
	movs	r0, #10
	bl 0x0200de2c
	ldr	r5, [r0, #8]
	movs	r0, #10
	bl 0x0200de2c
	ldr	r2, [r0, #16]
	asrs	r5, r5, #20
	movs	r3, #0
	asrs	r2, r2, #20
	adds	r1, r5, #0
	movs	r0, #2
	bl 0x0200862c
	movs	r0, #10
	bl 0x0200de2c
	movs	r3, #0
	adds	r0, #90
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #153
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02005c44
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #6
	bl 0x0200de9c
	b.n	.L_02005c52
.L_02005c44:
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #60
	movs	r2, #0
	bl 0x02009084
.L_02005c52:
	movs	r0, #10
	bl 0x0200de2c
	movs	r1, #0
	bl 0x0200ddbc
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200dd44
	cmp	r0, #0
	beq.n	.L_02005ca4
	movs	r3, #2
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #15
	movs	r1, #80
	movs	r2, #15
	movs	r3, #77
	bl 0x0200dd9c
	movs	r3, #79
	movs	r5, #15
	str	r3, [sp, #4]
	movs	r0, #15
	movs	r1, #80
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ddac
	movs	r0, #17
	movs	r1, #15
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ddac
.L_02005ca4:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200e072
	.4byte 0x0000005d
	.2byte 0xe07e
	.2byte 0x0200
	.global Func_02005cb8
	.thumb_func
Func_02005cb8:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02005cd0
	bl 0x0200cf3c
.L_02005cd0:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000055
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
	.4byte 0xffff000c
	.4byte 0x000b0009
	.4byte 0x0008ffff
	.4byte 0x000c000b
	.4byte 0xffff000a
	.4byte 0xffff0009
	.4byte 0x00090008
	.4byte 0x000b000a
	.4byte 0x000cffff
	.4byte 0x00090008
	.4byte 0x000e000d
	.4byte 0x0009ffff
	.4byte 0xffff000a
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
	.4byte 0x0200bed5
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200bf95
	.4byte 0x0000002e
	.4byte 0x0200c039
	.4byte 0x0000002e
	.4byte 0x0200c085
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
	.4byte 0x0200df9c
	.4byte 0x0200dfd8
	.4byte 0x0200e014
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
	.4byte 0x0001000c
	.4byte 0x000d0200
	.4byte 0x02010001
	.4byte 0x0001000e
	.4byte 0x000f0202
	.4byte 0x02030001
	.4byte 0x0000ffff
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
	.4byte 0xffff0001
	.4byte 0x000000e8
	.4byte 0x40000078
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0x40000088
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0003
	.4byte 0x00000098
	.4byte 0x400000e8
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0004
	.4byte 0x01400098
	.4byte 0x40000288
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0005
	.4byte 0x00800098
	.4byte 0x40000288
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0006
	.4byte 0x000000e8
	.4byte 0x40000278
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0007
	.4byte 0x00000138
	.4byte 0x40000268
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0008
	.4byte 0x000001d8
	.4byte 0x40000088
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0009
	.4byte 0x000001d8
	.4byte 0x400000e8
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff000a
	.4byte 0x00000198
	.4byte 0xc00003b8
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff000b
	.4byte 0x00000138
	.4byte 0x40000348
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff000c
	.4byte 0x00000328
	.4byte 0xc0000088
	.4byte 0x02600000
	.4byte 0x03600010
	.4byte 0x000000e0
	.4byte 0xffff000d
	.4byte 0x000002a8
	.4byte 0x400000a8
	.4byte 0x02600000
	.4byte 0x03600010
	.4byte 0x000000e0
	.4byte 0xffff000e
	.4byte 0x00000368
	.4byte 0x40000158
	.4byte 0x02500000
	.4byte 0x03d00100
	.4byte 0x000003f0
	.4byte 0xffff000f
	.4byte 0x00000388
	.4byte 0x40000388
	.4byte 0x02500000
	.4byte 0x03d00100
	.4byte 0x000003f0
	.4byte 0xffff0010
	.4byte 0x00000138
	.4byte 0x40000328
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000055
	.4byte 0x10102056
	.4byte 0xffffffff
	.4byte 0x10201057
	.4byte 0xffffffff
	.4byte 0x10304058
	.4byte 0xffffffff
	.4byte 0x1040205a
	.4byte 0xffffffff
	.4byte 0x1050c055
	.4byte 0xffffffff
	.4byte 0x1060105e
	.4byte 0xffffffff
	.4byte 0x1070205e
	.4byte 0xffffffff
	.4byte 0x1080105b
	.4byte 0xffffffff
	.4byte 0x1090105c
	.4byte 0xffffffff
	.4byte 0x10a04053
	.4byte 0xffffffff
	.4byte 0x10b0e055
	.4byte 0xffffffff
	.4byte 0x10c05055
	.4byte 0xffffffff
	.4byte 0x10d0d055
	.4byte 0xffffffff
	.4byte 0x10e0205d
	.4byte 0xffffffff
	.4byte 0x10f01059
	.4byte 0xffffffff
	.4byte 0x11003056
	.4byte 0xffffffff
	.4byte 0x00000056
	.4byte 0x1010e054
	.4byte 0xffffffff
	.4byte 0x10201055
	.4byte 0xffffffff
	.4byte 0x1030b055
	.4byte 0xffffffff
	.4byte 0x00000057
	.4byte 0x10102055
	.4byte 0xffffffff
	.4byte 0x10201058
	.4byte 0xffffffff
	.4byte 0x00000058
	.4byte 0x10102057
	.4byte 0xffffffff
	.4byte 0x1020105a
	.4byte 0xffffffff
	.4byte 0x1030105d
	.4byte 0xffffffff
	.4byte 0x10403055
	.4byte 0xffffffff
	.4byte 0x00000059
	.4byte 0x10110055
	.4byte 0xffffffff
	.4byte 0x0000005a
	.4byte 0x10102058
	.4byte 0xffffffff
	.4byte 0x10204055
	.4byte 0xffffffff
	.4byte 0x0000005b
	.4byte 0x10108055
	.4byte 0xffffffff
	.4byte 0x1020205c
	.4byte 0xffffffff
	.4byte 0x0000005c
	.4byte 0x10109055
	.4byte 0xffffffff
	.4byte 0x1020205b
	.4byte 0xffffffff
	.4byte 0x0000005d
	.4byte 0x10103058
	.4byte 0xffffffff
	.4byte 0x1020f055
	.4byte 0xffffffff
	.4byte 0x0000005e
	.4byte 0x10106055
	.4byte 0xffffffff
	.4byte 0x10207055
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff017e
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00024000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0118
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0118
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0118
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff017e
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0145
	.4byte 0x0200e1f8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0145
	.4byte 0x0200e1f8
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0143
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0145
	.4byte 0x0200e1f8
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0145
	.4byte 0x0200e1f8
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0145
	.4byte 0x0200e1f8
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0145
	.4byte 0x0200e1f8
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0145
	.4byte 0x0200e1f8
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0145
	.4byte 0x0200e1f8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000021
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000031
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x0000c602
	.4byte 0xffff001e
	.4byte 0x0200b171
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200a899
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte 0x0200a791
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte 0x0200a899
	.4byte 0x00000002
	.4byte 0xffff0052
	.4byte 0x0200a981
	.4byte 0x00000002
	.4byte 0xffff0053
	.4byte 0x0200aa15
	.4byte 0x80004e15
	.4byte 0x089a000b
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0x089a000b
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0x089a000b
	.4byte 0x0200acf1
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200b255
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x0200b32d
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02008989
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte 0x0200bac5
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte 0x0200a791
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte 0x0200a899
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200b255
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x08970009
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x08970009
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x0898000b
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x0898000b
	.4byte 0x0200b32d
	.4byte 0x80004e15
	.4byte 0x0899000a
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0x0899000a
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0x0899000a
	.4byte 0x0200b655
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02008989
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte 0x0200a791
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200b255
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x089b0008
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x089b0008
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x089c000b
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x089c000b
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x089d000c
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x089d000c
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x0200b32d
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02008989
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte 0x0200bb09
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200b255
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x089e0009
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x089e0009
	.4byte 0x0200b32d
	.4byte 0x80004e15
	.4byte 0xffff0008
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0xffff0008
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x02009829
	.4byte 0x80004e15
	.4byte 0xffff000a
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0xffff000a
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x02009829
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte 0x0200a791
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte 0x0200a899
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte 0x0200bd41
	.4byte 0x00008602
	.4byte 0xffff000c
	.4byte 0x0200c881
	.4byte 0x0000c403
	.4byte 0xffff001e
	.4byte 0x0200c535
	.4byte 0x00004403
	.4byte 0xffff001e
	.4byte 0x0200c741
	.4byte 0x00008403
	.4byte 0xffff001e
	.4byte 0x0200c7b1
	.4byte 0x00000403
	.4byte 0xffff001e
	.4byte 0x0200c819
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200be29
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200bdd5
	.4byte 0x00000006
	.4byte 0xffff00ca
	.4byte 0x0200bcad
	.4byte 0x00000006
	.4byte 0xffff00cb
	.4byte 0x0200bcad
	.4byte 0x00000006
	.4byte 0xffff00cc
	.4byte 0x0200bcad
	.4byte 0x00000006
	.4byte 0xffff00cd
	.4byte 0x0200bcad
	.4byte 0x00000006
	.4byte 0xffff00ce
	.4byte 0x0200bcad
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte 0x0200a791
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte 0x0200a899
	.4byte 0x00008c15
	.4byte 0x089f0008
	.4byte 0x0200c885
	.4byte 0x80004e15
	.4byte 0xffff0009
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0xffff0009
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x02009829
	.4byte 0x80004e15
	.4byte 0xffff000a
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0xffff000a
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x02009829
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte 0x0200a791
	.4byte 0x00000002
	.4byte 0xffff00d4
	.4byte 0x0200c8c5
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200b255
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x08d00008
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x08d00008
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0xffff0009
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x0200b32d
	.4byte 0x50008615
	.4byte 0x0200000c
	.4byte 0x0200c8bd
	.4byte 0x50008615
	.4byte 0x0201000d
	.4byte 0x0200c8bd
	.4byte 0x50008615
	.4byte 0x0202000e
	.4byte 0x0200c8bd
	.4byte 0x50008615
	.4byte 0x0203000f
	.4byte 0x0200c8bd
	.4byte 0x80004e15
	.4byte 0xffff0010
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0xffff0010
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0xffff0010
	.4byte 0x02009829
	.4byte 0x80004e15
	.4byte 0xffff0011
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0xffff0011
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0xffff0011
	.4byte 0x02009829
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte 0x0200cab9
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02008989
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200b255
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x0a8f000c
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x0a8f000c
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x08d10008
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x08d10008
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x08d20009
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x08d20009
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x0200b32d
	.4byte 0x80004e15
	.4byte 0xffff000a
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0xffff000a
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x02009829
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte 0x0200a791
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02008989
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200b255
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200b32d
	.4byte 0x10008c15
	.4byte 0x08d30009
	.4byte 0x0200b255
	.4byte 0x00008c15
	.4byte 0x08d30009
	.4byte 0x0200b32d
	.4byte 0x80004e15
	.4byte 0xffff0008
	.4byte 0x02008611
	.4byte 0x10004e15
	.4byte 0xffff0008
	.4byte 0x0200861d
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x02009829
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x0200cbb5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
