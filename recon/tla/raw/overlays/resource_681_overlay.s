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
	bl 0x0200cdb4
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
	bl 0x0200ce14
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200cf04
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ce1c
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
	bl 0x0200cdb4
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
	bl 0x0200ce14
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x0200cf04
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
	bl 0x0200ce74
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
	bl 0x0200cdb4
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
	bl 0x0200cd9c
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200cdac
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200ce14
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
	bl 0x0200cf04
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
	bl 0x0200cd14
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
	bl 0x0200cd14
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x0200cd14
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
	bl 0x0200cd9c
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200cdac
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
	.4byte 0x0200d41c
	.4byte 0x02008125
	.2byte 0x0000
	.2byte 0xffff
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	.2byte 0x4800
	bx	lr
	.2byte 0xd428
	.2byte 0x0200
	.global Func_02000344
	.thumb_func
Func_02000344:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_0200035c
	ldr	r0, [pc, #12]
.L_0200035c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b5
	.2byte 0xd458
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r5, [pc, #92]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #3
	movs	r6, #178
	bl 0x0200cf24
	lsls	r6, r6, #18
	movs	r2, #234
	movs	r3, #143
	lsls	r3, r3, #1
	lsls	r2, r2, #17
	adds	r0, r6, #0
	movs	r1, #0
	mov	r8, r3
	bl 0x02008080
	movs	r2, #245
	mov	r3, r8
	lsls	r2, r2, #17
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02008080
	movs	r2, #128
	movs	r1, #6
	lsls	r2, r2, #4
	ldr	r0, [r5, #0]
	bl 0x0200ceec
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200d014
	movs	r0, #145
	lsls	r0, r0, #1
	bl 0x0200cd74
	movs	r0, #8
	bl 0x0200cf6c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
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
	bl 0x0200cd14
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02000404
	adds	r3, #15
.L_02000404:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
.L_02000416:
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
.L_02000424:
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #392]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200ce74
	adds	r7, r0, #0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200cf4c
	bl 0x0200cdc4
	movs	r0, #1
	bl 0x0200cd24
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
	bl 0x0200cfac
	bl 0x0200cfbc
	movs	r0, #204
	bl 0x0200d014
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200ce54
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #272]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_020004c6:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200cd44
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200cd3c
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200cd34
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #204]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200cd34
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #192]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #172]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x0200815c
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020004c6
	movs	r0, #188
	bl 0x0200d014
	ldr	r5, [pc, #128]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200cf34
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200cecc
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ce24
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200ce24
	bl 0x0200ce2c
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200cf34
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200ce54
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200cecc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200ce64
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x020083d5
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd478
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #9
	bl 0x0200ce74
	movs	r1, #3
	adds	r5, r0, #0
	bl 0x0200cd9c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cda4
	movs	r0, #0
	pop	{r5, pc}
	push	{lr}
	movs	r0, #9
	bl 0x0200ce74
	ldr	r2, [r0, #80]
	movs	r1, #192
	ldrh	r3, [r2, #18]
	lsls	r1, r1, #2
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	movs	r0, #0
	pop	{pc}
	push	{lr}
	movs	r0, #9
	bl 0x0200ce74
	ldr	r2, [r0, #80]
	ldr	r1, [pc, #12]
	ldrh	r3, [r2, #18]
	movs	r0, #0
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	pop	{pc}
	.2byte 0x0000
	.2byte 0xfd00
	.2byte 0xffff
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200cd34
	ldr	r3, [r6, #24]
	ldr	r5, [pc, #60]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r3, r3, r5
	str	r3, [r6, #24]
	bl 0x0200cd34
	ldr	r3, [r6, #28]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r3, r3, r5
	str	r3, [r6, #28]
	movs	r2, #240
	ldr	r3, [r6, #24]
	lsls	r2, r2, #4
	adds	r2, #255
	cmp	r3, r2
	bgt.n	.L_02000664
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r6, #24]
.L_02000664:
	ldr	r3, [r6, #28]
	cmp	r3, r2
	bgt.n	.L_02000670
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r6, #28]
.L_02000670:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, lr}
	movs	r0, #9
	bl 0x0200ce74
	ldr	r1, [r0, #8]
	movs	r3, #128
	lsls	r3, r3, #12
	adds	r1, r1, r3
.L_02000688:
	ldr	r2, [r0, #12]
	ldr	r3, [pc, #72]
	movs	r6, #0
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	movs	r0, #14
	bl 0x0200cdb4
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r1, #0
	bl 0x0200ce14
	adds	r0, r5, #0
	ldr	r1, [pc, #44]
	bl 0x0200cdac
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r3, [pc, #16]
	str	r6, [r5, #68]
	str	r6, [r5, #76]
	str	r3, [r5, #108]
	movs	r0, #0
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0x0200d0d0
	.2byte 0x862d
	.2byte 0x0200
	push	{lr}
	movs	r0, #9
	bl 0x0200ce74
	ldr	r1, [pc, #8]
	bl 0x0200cdac
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd544
	.2byte 0x0200
	push	{lr}
	movs	r0, #9
	bl 0x0200ce74
	ldr	r1, [pc, #16]
	bl 0x0200cdac
	movs	r1, #129
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200cf34
	movs	r0, #0
	pop	{pc}
	.2byte 0xd59c
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #9
	bl 0x0200ce74
	ldr	r1, [pc, #32]
	adds	r5, r0, #0
	bl 0x0200cdac
	movs	r3, #236
	lsls	r3, r3, #17
	str	r3, [r5, #8]
	adds	r0, r5, #0
	movs	r1, #16
	bl 0x0200cda4
	movs	r1, #129
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200cf34
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0xd5c0
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #9
	bl 0x0200ce74
	ldr	r1, [pc, #28]
	adds	r5, r0, #0
	bl 0x0200cdac
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cda4
	movs	r1, #129
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200cf34
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xd610
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
	bl 0x0200cd14
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020007a4
	adds	r3, #15
.L_020007a4:
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
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	ldr	r3, [r0, #80]
	ldr	r4, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r4, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	bl 0x0200ce74
	adds	r7, r0, #0
	movs	r0, #202
	bl 0x0200d014
	movs	r0, #3
	mov	r8, r0
.L_0200080c:
	movs	r0, #30
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	adds	r0, #255
	bl 0x0200cdb4
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000894
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r3, #4
	strb	r5, [r3, #0]
	movs	r1, #0
	bl 0x0200ce14
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200cd9c
	adds	r0, r6, #0
	ldr	r1, [pc, #104]
	bl 0x0200cdac
	ldr	r1, [r6, #80]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	bl 0x0200cd34
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r0, r0, #14
	bl 0x0200cd44
	asrs	r0, r0, #1
	str	r0, [r6, #68]
	bl 0x0200cd34
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r0, r0, #14
	bl 0x0200cd3c
	asrs	r0, r0, #1
	str	r0, [r6, #76]
	str	r5, [r6, #72]
	bl 0x0200cd34
	ldr	r3, [pc, #44]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r6, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [pc, #32]
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	str	r3, [r6, #108]
.L_02000894:
	movs	r0, #1
	negs	r0, r0
	add	r8, r0
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0200080c
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d0f4
	.4byte 0xffffff00
	.2byte 0x8775
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #9
	bl 0x0200ce74
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	ldr	r3, [r0, #16]
	asrs	r3, r3, #19
	cmp	r3, #45
	bgt.n	.L_020008e6
	ldr	r3, [r0, #8]
	asrs	r3, r3, #19
	cmp	r3, #57
	bgt.n	.L_020008e6
	ldr	r1, [pc, #24]
	adds	r0, r5, #0
	bl 0x0200cdac
	b.n	.L_020008ee
.L_020008e6:
	ldr	r1, [pc, #20]
	adds	r0, r5, #0
	bl 0x0200cdac
.L_020008ee:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200d728
	.2byte 0xd6c4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	movs	r0, #9
	bl 0x0200ce74
	mov	r7, r8
	adds	r7, #96
	ldrb	r1, [r7, #0]
	adds	r5, r0, #0
	cmp	r1, #30
	beq.n	.L_02000924
	adds	r3, r5, #0
	adds	r3, #96
	ldrb	r3, [r3, #0]
	cmp	r3, #30
	bne.n	.L_0200094a
.L_02000924:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #181
	lsls	r0, r0, #1
	adds	r2, r3, r0
	movs	r3, #42
	strh	r3, [r2, #0]
	cmp	r1, #30
	bne.n	.L_0200093c
	movs	r3, #31
	strb	r3, [r7, #0]
.L_0200093c:
	adds	r2, r5, #0
	adds	r2, #96
	ldrb	r3, [r2, #0]
	cmp	r3, #30
	bne.n	.L_0200094a
	movs	r3, #31
	strb	r3, [r2, #0]
.L_0200094a:
	ldrb	r3, [r7, #0]
	cmp	r3, #2
	bls.n	.L_02000962
	adds	r2, r5, #0
	movs	r0, #128
	adds	r2, #91
	movs	r3, #1
	lsls	r0, r0, #2
	strb	r3, [r2, #0]
	adds	r0, #6
	bl 0x0200cd74
.L_02000962:
	adds	r6, r5, #0
	adds	r6, #96
	ldrb	r3, [r6, #0]
	cmp	r3, #2
	bls.n	.L_0200099a
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_0200099a
	ldr	r2, [r5, #0]
	ldr	r3, [pc, #96]
	cmp	r2, r3
	beq.n	.L_0200099a
	ldr	r3, [pc, #96]
	cmp	r2, r3
	beq.n	.L_0200099a
	mov	r2, r8
	movs	r0, #131
	adds	r2, #91
	movs	r3, #1
	lsls	r0, r0, #1
	strb	r3, [r2, #0]
	adds	r0, #255
	bl 0x0200cd74
.L_0200099a:
	ldrb	r2, [r7, #0]
	cmp	r2, #0
	bne.n	.L_020009b0
	adds	r3, r5, #0
	movs	r0, #128
	adds	r3, #91
	lsls	r0, r0, #2
	strb	r2, [r3, #0]
	adds	r0, #6
	bl 0x0200cd7c
.L_020009b0:
	ldrb	r2, [r6, #0]
	cmp	r2, #0
	bne.n	.L_020009c6
	mov	r3, r8
	movs	r0, #131
	adds	r3, #91
	lsls	r0, r0, #1
	strb	r2, [r3, #0]
	adds	r0, #255
	bl 0x0200cd7c
.L_020009c6:
	ldrb	r3, [r7, #0]
	cmp	r3, #35
	bls.n	.L_020009d0
	movs	r3, #35
	strb	r3, [r7, #0]
.L_020009d0:
	ldrb	r3, [r6, #0]
	cmp	r3, #35
	bls.n	.L_020009da
	movs	r3, #35
	strb	r3, [r6, #0]
.L_020009da:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d6c4
	.2byte 0xd728
	.2byte 0x0200
	.global Func_020009e8
	.thumb_func
Func_020009e8:
	push	{lr}
	ldr	r1, [pc, #72]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02000a00
	ldr	r0, [pc, #60]
	b.n	.L_02000a32
.L_02000a00:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000a0a
	ldr	r0, [pc, #60]
	b.n	.L_02000a32
.L_02000a0a:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000a14
	ldr	r0, [pc, #56]
	b.n	.L_02000a32
.L_02000a14:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000a1e
	ldr	r0, [pc, #56]
	b.n	.L_02000a32
.L_02000a1e:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #8
	ble.n	.L_02000a30
	ldr	r0, [pc, #40]
	b.n	.L_02000a32
.L_02000a30:
	ldr	r0, [pc, #40]
.L_02000a32:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000b2
	.4byte 0x0200da54
	.4byte 0x000000b3
	.4byte 0x0200dafc
	.4byte 0x000000b4
	.4byte 0x0200db8c
	.4byte 0x000000b5
	.4byte 0x0200dc7c
	.4byte 0x0200d9a0
	.2byte 0xd970
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r6, [pc, #64]
	movs	r2, #7
	ldr	r3, [r6, #0]
	adds	r5, r0, #0
	ands	r3, r2
	movs	r1, #0
	cmp	r3, #0
	bgt.n	.L_02000a74
	movs	r1, #7
.L_02000a74:
	ands	r1, r2
	adds	r0, r5, #0
	bl 0x0200ce34
	ldr	r3, [r6, #0]
	movs	r2, #31
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000a9a
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	str	r3, [r5, #24]
	bl 0x0200cdac
	movs	r0, #106
	bl 0x0200d014
.L_02000a9a:
	ldr	r3, [r6, #0]
	adds	r3, #1
	str	r3, [r6, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200dee4
	.2byte 0xde28
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r6, [pc, #64]
	movs	r2, #7
	ldr	r3, [r6, #0]
	adds	r5, r0, #0
	ands	r3, r2
	movs	r1, #0
	cmp	r3, #0
	bgt.n	.L_02000ac0
	movs	r1, #7
.L_02000ac0:
	ands	r1, r2
	adds	r0, r5, #0
	bl 0x0200ce34
	ldr	r3, [r6, #0]
	movs	r2, #31
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000ae6
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	str	r3, [r5, #28]
	bl 0x0200cdac
	movs	r0, #106
	bl 0x0200d014
.L_02000ae6:
	ldr	r3, [r6, #0]
	adds	r3, #1
	str	r3, [r6, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200dee4
	.2byte 0xdd6c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r0, r1, #0
	bl 0x0200ce74
	adds	r6, r0, #0
	bl 0x0200c904
	ldr	r3, [pc, #84]
	movs	r1, #192
	adds	r5, r0, #0
	lsls	r1, r1, #8
	adds	r7, r6, #0
	ands	r1, r5
	mov	r8, r3
	adds	r7, #89
	movs	r3, #0
	lsrs	r1, r1, #14
	strb	r3, [r7, #0]
	adds	r1, #2
	adds	r0, r6, #0
	bl 0x0200cd9c
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_02000b36
	ldr	r3, [pc, #48]
	b.n	.L_02000b38
.L_02000b36:
	ldr	r3, [pc, #48]
.L_02000b38:
	str	r3, [r6, #108]
	movs	r3, #1
	strb	r3, [r7, #0]
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r3, #165
	lsls	r3, r3, #4
	adds	r0, r0, r3
	bl 0x0200cd74
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200cd74
	movs	r0, #20
	bl 0x0200cd24
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200264c
	.4byte 0x02008aad
	.2byte 0x8a61
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #196]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x0200ce74
	movs	r2, #61
	movs	r1, #0
	movs	r3, #3
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r2, #0
	movs	r1, #26
	adds	r5, r0, #0
	movs	r0, #37
	str	r3, [sp, #0]
	bl 0x0200cff4
	movs	r1, #152
	movs	r2, #200
	movs	r0, #2
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200cdec
	cmp	r0, #0
	bne.n	.L_02000c2e
	movs	r3, #38
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #36
	movs	r1, #23
	movs	r2, #1
	bl 0x0200cdfc
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #38
	bne.n	.L_02000bf6
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #25
	bne.n	.L_02000bf6
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200cf34
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x0200ceac
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x0200ceb4
.L_02000bf6:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02000c20
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x0200cd74
	movs	r0, #30
	bl 0x0200ce54
	bl 0x0200cfdc
	movs	r0, #20
	bl 0x0200cf6c
	b.n	.L_02000c2e
.L_02000c20:
	movs	r0, #140
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200cd74
	bl 0x0200ce64
.L_02000c2e:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r2, #40
	movs	r1, #8
	movs	r3, #5
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #16
	movs	r1, #25
	movs	r2, #0
	str	r3, [sp, #0]
	bl 0x0200cff4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #99
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02000c70
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #99
	bl 0x0200cd74
	movs	r0, #2
	bl 0x0200acb4
.L_02000c70:
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r2, #31
	movs	r1, #0
	movs	r3, #3
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #18
	movs	r1, #17
	movs	r2, #0
	str	r3, [sp, #0]
	bl 0x0200cff4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #183
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02000cce
	movs	r1, #164
	movs	r2, #172
	movs	r0, #249
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d00c
	ldr	r2, [pc, #40]
	movs	r3, #149
	lsls	r3, r3, #2
	adds	r1, r2, r3
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #183
	strh	r3, [r1, #0]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #106
	movs	r1, #2
	bl 0x0200cf74
.L_02000cce:
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #3
	movs	r2, #31
	movs	r1, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #19
	movs	r1, #24
	movs	r2, #0
	movs	r3, #4
	bl 0x0200cff4
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r2, #31
	movs	r1, #0
	movs	r3, #3
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #18
	movs	r1, #17
	movs	r2, #0
	str	r3, [sp, #0]
	bl 0x0200cff4
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	movs	r2, #117
	movs	r3, #8
	movs	r1, #20
	str	r2, [sp, #4]
	movs	r0, #104
	movs	r2, #1
	str	r3, [sp, #0]
	str	r1, [sp, #8]
	bl 0x0200cff4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #196
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02000d46
	b.n	.L_02000ebc
.L_02000d46:
	movs	r5, #128
	movs	r0, #0
	lsls	r5, r5, #9
	mov	r8, r0
	movs	r3, #0
	movs	r7, #2
	adds	r6, r5, #0
.L_02000d54:
	ldr	r2, [pc, #368]
	lsls	r3, r3, #2
	ldrsh	r1, [r2, r3]
	ldrsh	r2, [r2, r7]
	lsls	r1, r1, #20
	lsls	r2, r2, #20
	movs	r0, #2
	bl 0x0200cdec
	cmp	r0, #0
	bne.n	.L_02000d76
	adds	r3, r6, #0
	movs	r0, #128
	lsls	r0, r0, #9
	asrs	r3, r3, #16
	adds	r6, r6, r0
	mov	r8, r3
.L_02000d76:
	adds	r3, r5, #0
	movs	r2, #128
	lsls	r2, r2, #9
	asrs	r3, r3, #16
	adds	r5, r5, r2
	adds	r7, #4
	cmp	r3, #11
	ble.n	.L_02000d54
	mov	r3, r8
	cmp	r3, #0
	bgt.n	.L_02000d8e
	b.n	.L_02000ebc
.L_02000d8e:
	movs	r0, #15
	bl 0x0200ce74
	ldr	r3, [pc, #308]
	adds	r7, r0, #0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r6, r3, r0
	ldr	r0, [r6, #0]
	bl 0x0200ce74
	adds	r5, r0, #0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	ldr	r3, [r5, #16]
	asrs	r3, r3, #19
	cmp	r3, #53
	ble.n	.L_02000dde
	ldr	r3, [r5, #8]
	asrs	r3, r3, #19
	cmp	r3, #83
	ble.n	.L_02000dde
	cmp	r3, #91
	bgt.n	.L_02000dde
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r0, [r6, #0]
	movs	r2, #0
	bl 0x0200cf2c
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #244]
	bl 0x0200ce84
.L_02000dde:
	ldr	r1, [pc, #244]
	movs	r0, #15
	bl 0x0200ce84
	movs	r0, #60
	bl 0x0200ce54
	ldr	r1, [pc, #232]
	movs	r0, #15
	bl 0x0200ce84
	movs	r0, #30
	bl 0x0200ce54
	movs	r3, #104
	str	r3, [sp, #0]
	movs	r5, #20
	movs	r0, #117
	movs	r1, #20
	movs	r2, #7
	movs	r3, #8
	str	r5, [sp, #4]
	bl 0x0200ce04
	movs	r3, #111
	str	r3, [sp, #0]
	movs	r0, #111
	movs	r1, #12
	movs	r2, #1
	movs	r3, #6
	str	r5, [sp, #4]
	bl 0x0200ce04
	movs	r3, #40
	movs	r2, #84
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #84
	movs	r2, #8
	movs	r3, #8
	movs	r0, #49
	bl 0x0200cdfc
	movs	r5, #175
	movs	r0, #30
	movs	r6, #143
	bl 0x0200ce54
	lsls	r6, r6, #1
	lsls	r5, r5, #18
	movs	r2, #234
	lsls	r2, r2, #17
	adds	r0, r5, #0
	movs	r1, #0
	adds	r3, r6, #0
	mov	sl, r2
	bl 0x02008080
	movs	r3, #245
	lsls	r3, r3, #17
	mov	r8, r3
	adds	r0, r5, #0
	movs	r5, #181
	movs	r1, #0
	mov	r2, r8
	adds	r3, r6, #0
	lsls	r5, r5, #18
	bl 0x02008080
	movs	r1, #0
	mov	r2, sl
	adds	r3, r6, #0
	adds	r0, r5, #0
	bl 0x02008080
	movs	r1, #0
	mov	r2, r8
	adds	r3, r6, #0
	adds	r0, r5, #0
	bl 0x02008080
	movs	r0, #204
	bl 0x0200d014
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	ldr	r1, [r7, #80]
	movs	r2, #12
	ldrb	r3, [r1, #9]
	movs	r0, #10
	orrs	r3, r2
	strb	r3, [r1, #9]
	bl 0x0200ce54
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	bl 0x0200cfdc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #196
	bl 0x0200cd74
	movs	r0, #9
	bl 0x0200cf6c
.L_02000ebc:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d1d8
	.4byte 0x02000240
	.4byte 0x0200d1b8
	.4byte 0x0200d118
	.2byte 0xd168
	.2byte 0x0200
	push	{lr}
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	ldr	r0, [pc, #16]
	bl 0x0200cf0c
	movs	r0, #10
	movs	r1, #0
.L_02000ef2:
	bl 0x0200cf14
	bl 0x0200ce64
	pop	{pc}
	.2byte 0x2300
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	bl 0x0200ce74
	adds	r7, r0, #0
	movs	r0, #10
	bl 0x0200ce74
	adds	r5, r0, #0
	ldr	r6, [r5, #80]
	ldr	r3, [pc, #112]
	ldr	r1, [r6, #40]
	movs	r2, #128
	mov	r8, r1
	ldr	r0, [pc, #108]
	adds	r1, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4641
	ldrb	r3, [r1, #21]
	ldr	r2, [pc, #100]
	str	r3, [r2, #0]
	ldr	r2, [pc, #100]
	ldrb	r3, [r6, #24]
	str	r3, [r2, #0]
	ldr	r3, [pc, #100]
	ldr	r1, [r5, #8]
	movs	r2, #0
	str	r1, [r3, #0]
	ldr	r3, [pc, #96]
	ldr	r0, [r5, #16]
	str	r2, [r5, #108]
	str	r0, [r3, #0]
	str	r2, [r5, #44]
	ldr	r3, [r5, #12]
	str	r2, [r5, #40]
	str	r2, [r5, #36]
	str	r3, [r5, #60]
	str	r1, [r5, #56]
	str	r0, [r5, #64]
	movs	r0, #10
	bl 0x0200cffc
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200cda4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	ldr	r2, [r7, #0]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	beq.n	.L_02000f76
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000f86
.L_02000f76:
	adds	r2, r7, #0
	adds	r2, #91
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cda4
.L_02000f86:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x03000730
	.4byte 0x0200e46c
	.4byte 0x0200e4f8
	.4byte 0x0200e4f4
	.4byte 0x0200e4ec
	.4byte 0x0200e4f0
	.4byte 0x0200d5c0
	.2byte 0xd59c
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #9
	bl 0x0200ce74
	adds	r5, r0, #0
	movs	r0, #10
	bl 0x0200ce74
	ldr	r3, [pc, #104]
	adds	r6, r0, #0
	ldr	r1, [r3, #0]
	ldr	r3, [r6, #8]
	cmp	r1, r3
	bne.n	.L_02000fd2
	ldr	r3, [pc, #96]
	ldr	r2, [r3, #0]
	ldr	r3, [r6, #16]
	cmp	r2, r3
	beq.n	.L_02000fe6
.L_02000fd2:
	ldr	r3, [pc, #88]
	asrs	r1, r1, #16
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	movs	r0, #10
	bl 0x0200cea4
	movs	r0, #10
	bl 0x0200ceb4
.L_02000fe6:
	ldr	r2, [r5, #0]
	ldr	r3, [pc, #68]
	cmp	r2, r3
	beq.n	.L_02000ff4
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02001004
.L_02000ff4:
	adds	r2, r5, #0
	adds	r2, #91
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #16
	bl 0x0200cda4
.L_02001004:
	movs	r2, #128
	ldr	r1, [pc, #48]
	ldr	r3, [pc, #48]
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0b
	movs	r0, #10
	ldr	r1, [r3, #0]
	bl 0x0200cecc
	ldr	r3, [pc, #40]
	adds	r0, r6, #0
	ldr	r1, [r3, #0]
	bl 0x0200cda4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200e4ec
	.4byte 0x0200e4f0
	.4byte 0x0200d5c0
	.4byte 0x0200d59c
	.4byte 0x0200e46c
	.4byte 0x03000730
	.4byte 0x0200e4f4
	.2byte 0xe4f8
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #9
	bl 0x0200ce74
	adds	r5, r0, #0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	ldr	r0, [pc, #324]
	bl 0x0200cf0c
	movs	r0, #10
	movs	r1, #1
	bl 0x0200cf3c
	ldr	r2, [r5, #0]
	ldr	r3, [pc, #312]
	cmp	r2, r3
	beq.n	.L_02001078
	ldr	r3, [pc, #312]
	cmp	r2, r3
	bne.n	.L_0200107e
.L_02001078:
	movs	r0, #9
	bl 0x0200ce8c
.L_0200107e:
	bl 0x02008f00
	movs	r0, #10
	movs	r1, #1
	bl 0x0200cecc
	ldr	r3, [pc, #292]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #10
	movs	r2, #0
	bl 0x0200cef4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200cf2c
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r1, #248
	movs	r2, #228
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #10
	bl 0x0200cea4
	movs	r0, #10
	bl 0x0200ceb4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #5
	movs	r0, #10
	bl 0x0200cf1c
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r1, #6
	movs	r2, #0
	adds	r1, #255
	movs	r0, #10
	bl 0x0200cf2c
	movs	r0, #45
	bl 0x0200ce54
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200cf1c
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #244
	movs	r1, #1
	movs	r2, #196
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200cf4c
	bl 0x0200cf54
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r0, #10
	movs	r1, #1
	bl 0x0200cf3c
	bl 0x0200cf54
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #5
	movs	r0, #10
	bl 0x0200cf1c
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r0, #10
	movs	r1, #3
	bl 0x0200ced4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x0200cf4c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #222
	bl 0x0200cd74
	ldr	r0, [r5, #0]
	ldr	r3, [pc, #56]
	cmp	r0, r3
	beq.n	.L_02001186
	ldr	r3, [pc, #52]
	cmp	r0, r3
	bne.n	.L_02001198
.L_02001186:
	ldr	r2, [pc, #52]
	ldr	r1, [pc, #52]
	ldr	r3, [r2, #56]
	str	r3, [r1, #0]
	str	r3, [r2, #8]
	ldr	r1, [pc, #48]
	ldr	r3, [r2, #64]
	str	r3, [r1, #0]
	str	r3, [r2, #16]
.L_02001198:
	bl 0x02008fac
	bl 0x0200ce64
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000022fa
	.4byte 0x0200d610
	.4byte 0x0200d59c
	.4byte 0x02000240
	.4byte 0x0200d6c4
	.4byte 0x0200d728
	.4byte 0x0200e46c
	.4byte 0x0200e4ec
	.2byte 0xe4f0
	.2byte 0x0200
	push	{lr}
	movs	r0, #10
	bl 0x0200ce74
	adds	r0, #96
	ldrb	r3, [r0, #0]
	cmp	r3, #2
	bhi.n	.L_02001294
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #172
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02001240
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #188
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02001228
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #189
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02001228
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #190
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02001228
	movs	r0, #140
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02001230
.L_02001228:
	ldr	r0, [pc, #108]
	bl 0x0200cf0c
	b.n	.L_02001236
.L_02001230:
	ldr	r0, [pc, #104]
	bl 0x0200cf0c
.L_02001236:
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	b.n	.L_02001290
.L_02001240:
	movs	r0, #9
	bl 0x0200ce74
	ldr	r3, [pc, #88]
	ldr	r2, [r0, #0]
	cmp	r2, r3
	bne.n	.L_02001254
	movs	r0, #9
	bl 0x0200ce8c
.L_02001254:
	bl 0x02008f00
	ldr	r0, [pc, #72]
	bl 0x0200cf0c
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #10
	bl 0x0200cef4
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r0, #10
	movs	r1, #3
	bl 0x0200ced4
	movs	r0, #10
	bl 0x0200ce54
	bl 0x02008fac
.L_02001290:
	bl 0x0200ce64
.L_02001294:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000023eb
	.4byte 0x00002308
	.4byte 0x0200d610
	.4byte 0x000022fe
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r0, #196
	adds	r0, #255
	bl 0x0200ce4c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_020012d8
	ldr	r0, [pc, #60]
	bl 0x0200cf0c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	b.n	.L_02001302
.L_020012d8:
	movs	r0, #9
	bl 0x0200ce74
	ldr	r3, [pc, #44]
	ldr	r2, [r0, #0]
	cmp	r2, r3
	bne.n	.L_020012ec
	movs	r0, #9
	bl 0x0200ce8c
.L_020012ec:
	bl 0x02008f00
	ldr	r0, [pc, #28]
	bl 0x0200cf0c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	bl 0x02008fac
.L_02001302:
	bl 0x0200ce64
	pop	{pc}
	.4byte 0x0000230a
	.4byte 0x0200d610
	.2byte 0x22ff
	.2byte 0x0000
	push	{r5, lr}
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #168
	lsls	r0, r0, #2
	bl 0x0200cdb4
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
	bl 0x0200cf04
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cd9c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ce14
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ce1c
	ldr	r1, [pc, #8]
	adds	r0, r5, #0
	bl 0x0200cdac
	pop	{r5, pc}
	.2byte 0xd208
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r0, #0
	movs	r2, #102
	adds	r2, r2, r6
.L_0200137c:
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
	bl 0x0200cd4c
	adds	r2, r6, #0
	adds	r2, #98
	ldrb	r3, [r2, #0]
	movs	r0, #0
	adds	r3, #255
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0200141c
	ldr	r3, [r6, #76]
	movs	r2, #128
	lsls	r2, r2, #10
	movs	r0, #1
	cmp	r3, r2
	beq.n	.L_0200141c
	adds	r0, r6, #0
	bl 0x02009314
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
	bl 0x0200cd4c
	adds	r0, r6, #0
	bl 0x02009314
	ldr	r3, [r6, #76]
	movs	r0, #1
	add	r3, r9
	str	r3, [r6, #76]
.L_0200141c:
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
	bl 0x0200cd44
	ldr	r1, [r7, #76]
	ldr	r6, [pc, #116]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x60b8
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	bl 0x0200cd3c
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
	beq.n	.L_020014a4
	ldr	r3, [pc, #72]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020014a4
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r0, [r3, #0]
	lsls	r0, r0, #10
	bl 0x0200cd3c
	ldrb	r3, [r5, #0]
	muls	r3, r0
	str	r3, [r7, #76]
	ldrb	r3, [r5, #0]
	adds	r3, #10
	strb	r3, [r5, #0]
.L_020014a4:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_020014c2
	ldrb	r3, [r5, #0]
	cmp	r3, #141
	bne.n	.L_020014c0
	adds	r3, r2, #0
	subs	r3, #128
	mov	r1, r8
	strh	r3, [r1, #0]
.L_020014c0:
	movs	r0, #1
.L_020014c2:
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
	bl 0x0200cd14
	str	r0, [r5, #68]
	ldr	r3, [r5, #12]
	ldr	r0, [r6, #12]
	movs	r1, #10
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	bl 0x0200cd14
	str	r0, [r5, #76]
	ldr	r3, [r5, #16]
	ldr	r0, [r6, #16]
	movs	r1, #10
	subs	r0, r0, r3
	bl 0x0200cd14
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
	bne.n	.L_020015c2
	add	r2, sp, #28
	str	r3, [r2, #4]
	movs	r3, #209
	lsls	r3, r3, #1
	adds	r3, #255
	strh	r3, [r2, #24]
	movs	r3, #1
	str	r3, [r2, #0]
	mov	r8, r2
	bl 0x0200cd34
	movs	r6, #31
	mov	r2, sl
	ldr	r3, [r2, #8]
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #16
	add	r5, sp, #16
	adds	r3, r3, r0
	str	r3, [r5, #0]
	bl 0x0200cd34
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
.L_020015c2:
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
	bl 0x0200ce74
	adds	r7, r0, #0
	mov	r0, fp
	bl 0x0200ce74
	mov	sl, r0
	movs	r0, #78
	bl 0x0200d014
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	bl 0x0200cf44
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
	bl 0x0200cf4c
	movs	r0, #141
	bl 0x0200d014
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200ce24
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200cf9c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200cf94
	movs	r0, #60
	bl 0x0200cfa4
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
	bl 0x0200ce84
	movs	r0, #194
	bl 0x0200d014
	movs	r0, #45
	bl 0x0200ce54
	movs	r1, #128
	mov	r0, r9
	lsls	r1, r1, #1
	bl 0x0200cefc
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
	b.n	.L_020016a8
	.2byte 0x0000
	.4byte 0x00001008
	.4byte 0x00003f10
	.2byte 0xd264
	.2byte 0x0200
.L_020016a8:
	movs	r0, #168
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #2
	bl 0x0200cdb4
	adds	r5, r0, #0
	movs	r0, #246
	bl 0x0200d014
	bl 0x0200cd34
	movs	r3, #31
	ldr	r2, [r5, #8]
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r5, #8]
	bl 0x0200cd34
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
	bl 0x0200cf04
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200cd9c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ce14
	adds	r0, r5, #0
	movs	r1, #1
.L_0200171c:
	bl 0x0200ce1c
	ldr	r1, [pc, #68]
	adds	r0, r5, #0
	bl 0x0200cdac
	movs	r6, #128
	ldr	r3, [pc, #48]
	ldr	r5, [pc, #48]
	lsls	r6, r6, #19
	adds	r6, #82
	strh	r3, [r6, #0]
	movs	r0, #2
	bl 0x0200cd24
	movs	r0, #2
	strh	r5, [r6, #0]
.L_0200173e:
	bl 0x0200cd24
	ldr	r3, [pc, #32]
	movs	r0, #2
	strh	r3, [r6, #0]
	bl 0x0200cd24
	strh	r5, [r6, #0]
	movs	r0, #2
	bl 0x0200cd24
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	b.n	.L_0200176c
	.4byte 0x00001004
	.4byte 0x0000100a
	.4byte 0x00001010
	.2byte 0xd218
	.2byte 0x0200
.L_0200176c:
	cmp	r3, #16
	bne.n	.L_020016a8
	ldr	r3, [pc, #56]
	movs	r0, #30
	strh	r3, [r6, #0]
	bl 0x0200cd24
	movs	r0, #0
	mov	r8, r0
.L_0200177e:
	mov	r0, sl
	ldr	r3, [r0, #16]
	mov	r2, sl
	movs	r0, #168
	ldr	r1, [r2, #8]
	lsls	r0, r0, #2
	ldr	r2, [r2, #12]
	bl 0x0200cdb4
	adds	r5, r0, #0
	movs	r0, #195
	bl 0x0200d014
	bl 0x0200cd34
	ldr	r3, [pc, #16]
	movs	r2, #128
	ands	r0, r3
	lsls	r2, r2, #8
	adds	r6, r5, #0
	adds	r0, r0, r2
	adds	r6, #100
	b.n	.L_020017b4
	.4byte 0x00001008
	.2byte 0x7fff
	.2byte 0x0000
.L_020017b4:
	strh	r0, [r6, #0]
	bl 0x0200cd34
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
	bl 0x0200cf04
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #7
	b.n	.L_0200180c
	.2byte 0x0000
	.2byte 0x0000
.L_0200180c:
	bl 0x0200cd9c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ce14
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ce1c
	adds	r2, r5, #0
	movs	r3, #0
	ldrsh	r1, [r6, r3]
	ldr	r0, [r5, #76]
	adds	r2, #8
	bl 0x0200cd4c
	adds	r0, r5, #0
	ldr	r1, [pc, #208]
	bl 0x0200cdac
	mov	r0, r8
.L_02001838:
	cmp	r0, #3
	bne.n	.L_02001860
	movs	r1, #128
	mov	r0, fp
	lsls	r1, r1, #1
	bl 0x0200cefc
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
	bl 0x0200ce84
.L_02001860:
	movs	r0, #8
	bl 0x0200cd24
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	cmp	r0, #16
	beq.n	.L_02001872
	b.n	.L_0200177e
.L_02001872:
	movs	r0, #220
	bl 0x0200d014
	movs	r0, #16
	bl 0x0200cd24
	movs	r2, #2
	mov	r8, r2
.L_02001882:
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
	bl 0x0200cdb4
	adds	r5, r0, #0
	bl 0x0200cd34
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
	bl 0x0200cf04
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200cd9c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ce14
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ce1c
	b.n	.L_02001910
	.4byte 0x00000000
	.4byte 0x0200d234
	.4byte 0x0200d288
	.2byte 0x0000
	.2byte 0xfff8
.L_02001910:
	.2byte 0x1c28
	ldr	r1, [pc, #168]
	bl 0x0200cdac
	movs	r0, #2
	add	r8, r0
	mov	r2, r8
	cmp	r2, #32
	bne.n	.L_02001882
	movs	r0, #220
	bl 0x0200d014
	movs	r0, #50
	bl 0x0200cd24
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200cf94
	movs	r0, #8
	bl 0x0200cfa4
	movs	r0, #16
	bl 0x0200cd24
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200ce24
	mov	r0, r9
	movs	r1, #0
	bl 0x0200cefc
	mov	r0, fp
	movs	r1, #0
	bl 0x0200cefc
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200d014
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200cf94
	movs	r0, #80
	bl 0x0200cfa4
	mov	r0, r9
	ldr	r1, [pc, #64]
	bl 0x0200ce84
	ldr	r1, [pc, #64]
	mov	r0, fp
	bl 0x0200ce84
	ldr	r3, [pc, #60]
	mov	r0, sl
	str	r3, [r0, #108]
	movs	r0, #120
	bl 0x0200cd24
	mov	r2, sl
	movs	r0, #195
	str	r6, [r2, #108]
	lsls	r0, r0, #1
	bl 0x0200d014
	bl 0x0200cfd4
	movs	r0, #1
	bl 0x0200cd24
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d244
	.4byte 0x0200d2ac
	.4byte 0x0200d2dc
	.2byte 0x9549
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #384]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #4
	bl 0x0200ce74
	adds	r5, r0, #0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	ldr	r7, [pc, #360]
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200ce44
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200cf2c
	ldr	r3, [r5, #16]
	asrs	r3, r3, #19
	cmp	r3, #39
	bgt.n	.L_02001a1c
	movs	r1, #220
	movs	r2, #164
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200cea4
	ldr	r0, [r6, #0]
	bl 0x0200ceb4
.L_02001a1c:
	ldr	r1, [r6, #0]
	movs	r0, #11
	bl 0x0200cec4
	movs	r1, #235
	movs	r2, #177
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200cea4
	movs	r1, #223
	movs	r2, #177
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #11
	bl 0x0200cea4
	ldr	r0, [r6, #0]
	bl 0x0200ceb4
	movs	r0, #11
	bl 0x0200ceb4
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cf1c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #11
	bl 0x0200cf1c
	adds	r0, r7, #1
	bl 0x0200cf0c
	movs	r1, #0
	movs	r0, #11
	bl 0x0200cf14
	movs	r0, #10
	bl 0x0200ce54
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200cf1c
	movs	r0, #10
	bl 0x0200ce54
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200ced4
	movs	r0, #10
	bl 0x0200ce54
	movs	r1, #3
	movs	r0, #11
	bl 0x0200ced4
	movs	r0, #10
	bl 0x0200ce54
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cf1c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x0200cf1c
	movs	r0, #10
	bl 0x0200ce54
	movs	r1, #233
	movs	r2, #171
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200cea4
	ldr	r0, [r6, #0]
	bl 0x0200ceb4
	adds	r0, r7, #2
	movs	r1, #1
	bl 0x0200ce44
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ce24
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200ce24
	bl 0x0200ce2c
	bl 0x0200cddc
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	ldrh	r2, [r3, #0]
	mov	r6, sp
	movs	r3, #255
	adds	r6, #2
	ands	r3, r2
	adds	r7, r6, #0
	strh	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02001b58
.L_02001b20:
	ldrh	r3, [r7, #0]
	ldr	r2, [pc, #40]
	movs	r1, #128
	lsls	r1, r1, #19
	orrs	r3, r2
	adds	r1, #82
	strh	r3, [r1, #0]
	ldrh	r3, [r7, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
	strh	r3, [r7, #0]
	adds	r5, r7, #0
	movs	r0, #1
	bl 0x0200cd24
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02001b20
	b.n	.L_02001b58
	.2byte 0x0000
	.4byte 0x00001000
	.4byte 0x02000240
	.2byte 0x23e5
	.2byte 0x0000
.L_02001b58:
	movs	r5, #128
	lsls	r5, r5, #19
	ldrh	r2, [r5, #0]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r5, #0]
	movs	r1, #4
	movs	r0, #12
	bl 0x020095d0
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #44]
	movs	r1, #128
	orrs	r3, r2
	strh	r3, [r5, #0]
	ldr	r2, [pc, #40]
	ldrh	r3, [r6, #0]
	lsls	r1, r1, #19
	orrs	r3, r2
	adds	r1, #82
	strh	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldrh	r3, [r6, #0]
	cmp	r3, #8
	beq.n	.L_02001bc6
.L_02001b96:
	ldrh	r3, [r6, #0]
	ldr	r2, [pc, #8]
	movs	r1, #128
	lsls	r1, r1, #19
	b.n	.L_02001bac
	.4byte 0x00000f00
	.4byte 0x00001000
	.2byte 0x3f42
	.2byte 0x0000
.L_02001bac:
	orrs	r3, r2
	adds	r1, #82
	strh	r3, [r1, #0]
	adds	r5, r6, #0
	ldrh	r3, [r6, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r6, #0]
	bl 0x0200cd24
	ldrh	r3, [r5, #0]
	cmp	r3, #8
	bne.n	.L_02001b96
.L_02001bc6:
	bl 0x0200cdd4
	movs	r2, #0
	movs	r0, #12
	movs	r1, #0
	bl 0x0200cebc
	movs	r1, #139
	movs	r0, #4
	bl 0x0200ce6c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #186
	bl 0x0200cd74
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #11
	ldr	r1, [pc, #76]
	bl 0x0200ce7c
	movs	r0, #11
	movs	r1, #2
	bl 0x0200cecc
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_02001c1c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #11
	bl 0x0200ce9c
.L_02001c1c:
	movs	r0, #11
	bl 0x0200ceb4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #11
	bl 0x0200cebc
	movs	r0, #20
	bl 0x0200ce54
	bl 0x0200ce64
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00013333
	.2byte 0x0240
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
	bl 0x0200cd14
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001c74
	adds	r3, #15
.L_02001c74:
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
	ldr	r3, [pc, #100]
	sub	sp, #68
	add	r2, sp, #16
	str	r3, [r2, #36]
	mov	sl, r2
	movs	r7, #0
.L_02001cb0:
	lsls	r6, r7, #12
	adds	r0, r6, #0
	bl 0x0200cd44
	add	r5, sp, #56
	movs	r3, #0
	str	r0, [r5, #0]
	adds	r0, r6, #0
	str	r3, [r5, #4]
	bl 0x0200cd3c
	ldr	r6, [r5, #0]
	mov	r8, r0
	str	r0, [r5, #8]
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x0200cd14
	ldr	r3, [r5, #4]
	adds	r6, r6, r0
	str	r6, [r5, #0]
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r3, #128
	mov	r2, sl
	lsls	r3, r3, #17
	adds	r3, #1
	str	r2, [sp, #12]
	movs	r0, #232
	movs	r2, #164
	str	r3, [sp, #8]
	lsls	r0, r0, #17
	ldr	r1, [pc, #28]
	lsls	r2, r2, #17
	adds	r3, r6, #0
	adds	r7, #2
	bl 0x0200815c
	cmp	r7, #16
	bls.n	.L_02001cb0
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02009c45
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #238
	sub	sp, #4
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02001d2c
	b.n	.L_02001eb2
.L_02001d2c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #186
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02001d3c
	b.n	.L_02001eb2
.L_02001d3c:
	movs	r0, #12
	bl 0x0200ce74
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	ldrh	r3, [r3, #0]
	mov	r5, sp
	adds	r5, #2
	strh	r3, [r5, #0]
	mov	r8, r0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r1, r1, #12
	lsls	r2, r2, #9
	lsls	r0, r0, #12
	bl 0x0200ce24
	movs	r0, #107
	bl 0x0200d014
	bl 0x0200cddc
	ldrb	r3, [r5, #0]
	strh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001dac
.L_02001d7e:
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #36]
	movs	r1, #128
	lsls	r1, r1, #19
	orrs	r3, r2
	adds	r1, #82
	strh	r3, [r1, #0]
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200cd24
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02001d7e
	b.n	.L_02001dac
	.2byte 0x0000
	.2byte 0x1000
	.2byte 0x0000
.L_02001dac:
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #30
	movs	r6, #232
	movs	r5, #174
	bl 0x0200ce54
	lsls	r5, r5, #17
	lsls	r6, r6, #17
	movs	r3, #68
	adds	r3, #255
	adds	r2, r5, #0
	ldr	r1, [pc, #180]
	adds	r0, r6, #0
	bl 0x02008080
	adds	r2, r5, #0
	adds	r1, r6, #0
	adds	r7, r0, #0
	movs	r0, #12
	bl 0x0200cebc
	bl 0x02009c9c
	movs	r5, #0
.L_02001dea:
	cmp	r5, #10
	bne.n	.L_02001df2
	bl 0x02009c9c
.L_02001df2:
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r2, [pc, #148]
	movs	r0, #2
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r2, #16]
	adds	r5, #1
	bl 0x0200ce54
	cmp	r5, #19
	ble.n	.L_02001dea
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #238
	bl 0x0200cd74
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200ce24
	movs	r3, #0
	str	r3, [r7, #8]
	str	r3, [r7, #16]
	movs	r0, #1
	bl 0x0200ce54
	adds	r0, r7, #0
	bl 0x0200cdbc
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d014
	movs	r0, #30
	bl 0x0200ce54
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	mov	r6, sp
	orrs	r3, r2
	strh	r3, [r1, #0]
	adds	r6, #2
.L_02001e56:
	ldrh	r3, [r6, #0]
.L_02001e58:
	ldr	r2, [pc, #36]
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldrh	r3, [r6, #0]
	cmp	r3, #8
	beq.n	.L_02001eaa
.L_02001e70:
	ldrh	r3, [r6, #0]
	ldr	r2, [pc, #12]
	movs	r1, #128
	lsls	r1, r1, #19
	b.n	.L_02001e90
	.2byte 0x0000
	.4byte 0x00000f00
	.4byte 0x00001000
	.4byte 0x00003f42
	.4byte 0xffe00000
	.2byte 0x0000
	.2byte 0xffff
.L_02001e90:
	.2byte 0x4313
	adds	r1, #82
	strh	r3, [r1, #0]
	adds	r5, r6, #0
	ldrh	r3, [r6, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r6, #0]
	bl 0x0200cd24
	ldrh	r3, [r5, #0]
	cmp	r3, #8
	bne.n	.L_02001e70
.L_02001eaa:
	bl 0x0200cdd4
	bl 0x0200ce64
.L_02001eb2:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02001eda
	ldr	r0, [pc, #136]
	movs	r1, #1
	bl 0x0200ce44
	b.n	.L_02001f54
.L_02001eda:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #188
	bl 0x0200cd6c
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #189
	bl 0x0200cd6c
	mov	r8, r0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #190
	bl 0x0200cd6c
	adds	r6, r0, #0
	movs	r0, #140
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200cd6c
	add	r5, r8
	adds	r5, r5, r6
	adds	r5, r5, r0
	ldr	r3, [pc, #80]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r6, [pc, #68]
	movs	r1, #10
	adds	r0, r6, #0
	bl 0x0200cf7c
	movs	r0, #196
	adds	r0, #255
	bl 0x0200ce4c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_02001f44
	cmp	r5, #0
	bne.n	.L_02001f4c
	adds	r0, r6, #0
	movs	r1, #12
	bl 0x0200cf84
	b.n	.L_02001f4c
.L_02001f44:
	adds	r0, r6, #0
	movs	r1, #11
	bl 0x0200cf84
.L_02001f4c:
	movs	r0, #12
	adds	r1, r5, #0
	bl 0x0200cf74
.L_02001f54:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00002313
	.4byte 0x02000240
	.2byte 0x00b1
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	movs	r3, #2
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #66
	bl 0x0200ce74
	movs	r3, #138
	lsls	r3, r3, #18
	str	r3, [r0, #8]
	movs	r3, #148
	lsls	r3, r3, #17
	str	r3, [r0, #16]
	ldr	r3, [pc, #24]
	str	r3, [r0, #12]
	ldr	r3, [pc, #24]
	str	r3, [r0, #20]
	movs	r0, #161
	lsls	r0, r0, #1
	bl 0x0200cd7c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #66
	bl 0x0200cf8c
	pop	{pc}
	.4byte 0xffe20000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb520
	ldr	r5, [pc, #64]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200ce44
	movs	r0, #196
	adds	r0, #255
	bl 0x0200ce4c
	movs	r3, #1
	negs	r3, r3
	adds	r2, r5, #2
	cmp	r0, r3
	beq.n	.L_02002010
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	adds	r0, r2, #0
	movs	r1, #1
	bl 0x0200ce44
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002018
.L_02002010:
	adds	r0, r2, #0
	movs	r1, #1
	bl 0x0200ce44
.L_02002018:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x22f5
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #8
	sub	sp, #8
	bl 0x0200ce74
	adds	r7, r0, #0
	movs	r0, #9
	bl 0x0200ce74
	mov	r8, r0
	movs	r0, #13
	bl 0x0200ce74
	mov	sl, r0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r1, #1
	ldr	r0, [pc, #516]
	bl 0x0200ce44
	movs	r0, #193
	bl 0x0200d014
	mov	r0, sl
	movs	r1, #1
	bl 0x0200ce3c
	ldr	r5, [pc, #500]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r1, [r5, #0]
	movs	r0, #13
	bl 0x0200cec4
	movs	r1, #248
	movs	r2, #178
	lsls	r1, r1, #1
	movs	r0, #13
	lsls	r2, r2, #2
	bl 0x0200ce9c
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, sl
	str	r3, [r2, #40]
	movs	r0, #13
	bl 0x0200ceb4
	movs	r0, #45
	bl 0x0200ce54
	ldr	r1, [r5, #0]
	movs	r0, #8
	movs	r2, #0
	bl 0x0200cef4
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200cef4
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r0, #8
	movs	r1, #5
	bl 0x0200cecc
	movs	r1, #5
	movs	r0, #9
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200cd24
	movs	r0, #8
	movs	r1, #2
	bl 0x0200cecc
	movs	r1, #2
	movs	r0, #9
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #8
	movs	r1, #3
	bl 0x0200cecc
	movs	r1, #3
	movs	r0, #9
	bl 0x0200cecc
	movs	r0, #40
	bl 0x0200ce54
	movs	r0, #191
	bl 0x0200d014
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #98
	bl 0x0200cd6c
	movs	r5, #119
	cmp	r0, #0
	bne.n	.L_020021d8
	movs	r5, #19
.L_0200210a:
	bl 0x0200cd34
	str	r0, [r7, #40]
	bl 0x0200cd34
	mov	r3, r8
	str	r0, [r3, #40]
	subs	r5, #1
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_0200210a
	movs	r2, #192
	movs	r1, #128
	lsls	r2, r2, #4
	movs	r0, #8
	lsls	r1, r1, #7
	adds	r2, #204
	bl 0x0200ce7c
	movs	r2, #192
	movs	r1, #128
	lsls	r2, r2, #4
	movs	r0, #9
	lsls	r1, r1, #7
	adds	r2, #204
	bl 0x0200ce7c
	movs	r0, #8
.L_02002146:
	movs	r1, #24
	movs	r2, #0
	bl 0x0200ceac
	adds	r6, r7, #0
	movs	r1, #24
	movs	r0, #9
	negs	r1, r1
.L_02002156:
	movs	r2, #0
	adds	r6, #86
	bl 0x0200ceac
	ldrb	r3, [r6, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0200218a
.L_02002166:
	bl 0x0200cd34
	str	r0, [r7, #40]
	bl 0x0200cd34
	mov	r2, r8
	str	r0, [r2, #40]
	movs	r0, #1
	bl 0x0200cd24
	movs	r3, #44
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	bgt.n	.L_0200218a
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_02002166
.L_0200218a:
	movs	r5, #9
.L_0200218c:
	bl 0x0200cd34
	str	r0, [r7, #40]
	bl 0x0200cd34
	mov	r2, r8
	str	r0, [r2, #40]
	subs	r5, #1
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_0200218c
	movs	r3, #89
	str	r3, [sp, #0]
	movs	r5, #40
	movs	r0, #119
	movs	r1, #31
	movs	r2, #1
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200cdf4
	movs	r3, #100
	str	r3, [sp, #0]
	movs	r0, #119
	movs	r1, #31
	movs	r2, #1
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200cdf4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #98
	bl 0x0200cd74
	b.n	.L_020021f8
.L_020021d8:
	bl 0x0200cd34
	str	r0, [r7, #40]
	bl 0x0200cd34
	mov	r3, r8
	str	r0, [r3, #40]
	subs	r5, #1
	movs	r0, #1
	bl 0x0200cd24
.L_020021ee:
	cmp	r5, #0
	bge.n	.L_020021d8
	movs	r0, #30
	bl 0x0200ce54
.L_020021f8:
	movs	r0, #8
	movs	r1, #4
	bl 0x0200cecc
	movs	r0, #9
	movs	r1, #4
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_0200222e
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #13
	bl 0x0200ce9c
.L_0200222e:
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, sl
	str	r3, [r2, #40]
	movs	r0, #13
	bl 0x0200ceb4
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	bl 0x0200ce64
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000022f8
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #8
	sub	sp, #8
	bl 0x0200ce74
	adds	r7, r0, #0
	movs	r0, #12
	bl 0x0200ce74
	mov	r8, r0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r1, #1
	ldr	r0, [pc, #556]
	bl 0x0200ce44
	movs	r0, #193
	bl 0x0200d014
	mov	r0, r8
	movs	r1, #1
	bl 0x0200ce3c
	ldr	r5, [pc, #540]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r1, [r5, #0]
	movs	r0, #12
	bl 0x0200cec4
	movs	r1, #132
	movs	r2, #186
	lsls	r1, r1, #2
	movs	r0, #12
.L_020022ac:
	lsls	r2, r2, #1
	bl 0x0200ce9c
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #12
	bl 0x0200ceb4
	movs	r0, #45
	bl 0x0200ce54
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200cef4
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r1, #5
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200cd24
	movs	r1, #2
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	movs	r1, #3
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #40
	bl 0x0200ce54
	movs	r0, #191
	bl 0x0200d014
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #189
	bl 0x0200cd6c
	movs	r5, #119
	cmp	r0, #0
	beq.n	.L_0200231a
	b.n	.L_02002444
.L_0200231a:
	movs	r5, #19
.L_0200231c:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_0200231c
	movs	r2, #192
	movs	r1, #128
	lsls	r2, r2, #4
	movs	r0, #8
	lsls	r1, r1, #7
	adds	r2, #204
	bl 0x0200ce7c
	movs	r0, #8
	movs	r1, #24
	movs	r2, #0
	bl 0x0200ceac
	movs	r3, #28
	movs	r5, #22
	str	r3, [sp, #0]
	movs	r0, #27
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200cdfc
	adds	r6, r7, #0
	movs	r3, #29
	str	r3, [sp, #0]
	movs	r0, #32
	movs	r3, #1
	movs	r1, #16
	movs	r2, #2
	adds	r6, #86
	str	r5, [sp, #4]
	bl 0x0200cdfc
	ldrb	r3, [r6, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02002396
.L_0200237a:
	bl 0x0200cd34
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	movs	r3, #44
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	bgt.n	.L_02002396
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_0200237a
.L_02002396:
	movs	r3, #28
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #2
	movs	r1, #93
	movs	r2, #1
	movs	r3, #5
	bl 0x0200cdf4
	movs	r5, #9
.L_020023ac:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_020023ac
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r0, #8
	movs	r1, #4
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	ldr	r3, [pc, #220]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_020023f4
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
	bl 0x0200ce9c
.L_020023f4:
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #12
	bl 0x0200ceb4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02002434
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x0200cd74
	movs	r0, #30
	bl 0x0200ce54
	bl 0x0200cfdc
	movs	r0, #20
	bl 0x0200cf6c
	b.n	.L_020024a6
.L_02002434:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #189
	bl 0x0200cd74
	bl 0x0200ce64
	b.n	.L_020024a6
.L_02002444:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_02002444
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #8
	movs	r1, #4
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_0200248a
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
	bl 0x0200ce9c
.L_0200248a:
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #12
	bl 0x0200ceb4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	bl 0x0200ce64
.L_020024a6:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000022f8
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	sub	sp, #8
	bl 0x0200ce74
	adds	r7, r0, #0
	movs	r0, #12
	bl 0x0200ce74
	mov	r8, r0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r1, #1
	ldr	r0, [pc, #560]
	bl 0x0200ce44
	movs	r0, #193
	bl 0x0200d014
	mov	r0, r8
	movs	r1, #1
	bl 0x0200ce3c
	ldr	r5, [pc, #544]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r1, [r5, #0]
	movs	r0, #12
	bl 0x0200cec4
	movs	r1, #168
	movs	r2, #186
	lsls	r1, r1, #2
	movs	r0, #12
	lsls	r2, r2, #1
	bl 0x0200ce9c
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #12
	bl 0x0200ceb4
	movs	r0, #45
	bl 0x0200ce54
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200cef4
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r1, #5
	movs	r0, #9
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200cd24
	movs	r1, #2
	movs	r0, #9
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	movs	r1, #3
	movs	r0, #9
	bl 0x0200cecc
	movs	r0, #40
	bl 0x0200ce54
	movs	r0, #191
	bl 0x0200d014
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #190
	bl 0x0200cd6c
	movs	r5, #119
	cmp	r0, #0
	beq.n	.L_02002576
	b.n	.L_020026a6
.L_02002576:
	movs	r5, #19
.L_02002578:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_02002578
	movs	r2, #192
	movs	r1, #128
	lsls	r2, r2, #4
	movs	r0, #9
	lsls	r1, r1, #7
	adds	r2, #204
	bl 0x0200ce7c
	movs	r1, #152
	movs	r2, #176
	movs	r0, #9
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200ce9c
	movs	r3, #36
	movs	r5, #22
	str	r3, [sp, #0]
	movs	r0, #35
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200cdfc
	adds	r6, r7, #0
	movs	r3, #37
	str	r3, [sp, #0]
	movs	r0, #32
	movs	r3, #1
	movs	r1, #16
	movs	r2, #2
	adds	r6, #86
	str	r5, [sp, #4]
	bl 0x0200cdfc
	ldrb	r3, [r6, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_020025f6
.L_020025da:
	bl 0x0200cd34
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	movs	r3, #44
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	bgt.n	.L_020025f6
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_020025da
.L_020025f6:
	movs	r3, #36
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #2
	movs	r1, #93
	movs	r2, #1
	movs	r3, #5
	bl 0x0200cdf4
	movs	r5, #9
.L_0200260c:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_0200260c
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r0, #9
	movs	r1, #4
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	ldr	r3, [pc, #220]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_02002654
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
	bl 0x0200ce9c
.L_02002654:
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #12
	bl 0x0200ceb4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02002696
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #9
	bl 0x0200cd74
	movs	r0, #30
	bl 0x0200ce54
	bl 0x0200cfdc
	movs	r0, #20
	bl 0x0200cf6c
	b.n	.L_02002708
.L_02002696:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #190
	bl 0x0200cd74
	bl 0x0200ce64
	b.n	.L_02002708
.L_020026a6:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_020026a6
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #9
	movs	r1, #4
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_020026ec
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
.L_020026e8:
	bl 0x0200ce9c
.L_020026ec:
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #12
	bl 0x0200ceb4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	bl 0x0200ce64
.L_02002708:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x000022f8
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #8
	sub	sp, #8
	bl 0x0200ce74
.L_02002726:
	adds	r7, r0, #0
	movs	r0, #14
	bl 0x0200ce74
	mov	r8, r0
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r1, #1
	ldr	r0, [pc, #556]
	bl 0x0200ce44
	movs	r0, #193
	bl 0x0200d014
	mov	r0, r8
	movs	r1, #1
	bl 0x0200ce3c
	ldr	r5, [pc, #540]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r1, [r5, #0]
	movs	r0, #14
	bl 0x0200cec4
	movs	r1, #168
	movs	r2, #198
	lsls	r1, r1, #1
	movs	r0, #14
	lsls	r2, r2, #1
	bl 0x0200ce9c
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #14
	bl 0x0200ceb4
	movs	r0, #45
	bl 0x0200ce54
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200cef4
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r1, #5
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200cd24
	movs	r1, #2
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	movs	r1, #3
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #40
	bl 0x0200ce54
	movs	r0, #191
	bl 0x0200d014
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #188
	bl 0x0200cd6c
	movs	r5, #119
	cmp	r0, #0
	beq.n	.L_020027d6
	b.n	.L_02002902
.L_020027d6:
	movs	r5, #19
.L_020027d8:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_020027d8
	movs	r2, #192
	movs	r1, #128
	lsls	r2, r2, #4
	movs	r0, #8
	lsls	r1, r1, #7
	adds	r2, #204
	bl 0x0200ce7c
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #0
	bl 0x0200ceac
	movs	r5, #23
	movs	r0, #27
	movs	r1, #15
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200cdfc
	adds	r6, r7, #0
	movs	r3, #25
	str	r3, [sp, #0]
	movs	r0, #25
	movs	r3, #1
	movs	r1, #25
	movs	r2, #1
	adds	r6, #86
	str	r5, [sp, #4]
	bl 0x0200cdfc
	ldrb	r3, [r6, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02002852
.L_02002836:
	bl 0x0200cd34
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	movs	r3, #44
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	bgt.n	.L_02002852
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_02002836
.L_02002852:
	movs	r3, #25
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #1
	movs	r1, #85
	movs	r2, #1
	movs	r3, #4
	bl 0x0200cdf4
	movs	r5, #9
.L_02002868:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_02002868
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r0, #8
	movs	r1, #4
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	ldr	r3, [pc, #220]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_020028b0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #14
	bl 0x0200ce9c
.L_020028b0:
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #14
	bl 0x0200ceb4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_020028f2
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200cd74
	movs	r0, #30
	bl 0x0200ce54
	bl 0x0200cfdc
	movs	r0, #20
	bl 0x0200cf6c
	b.n	.L_02002964
.L_020028f2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #188
	bl 0x0200cd74
	bl 0x0200ce64
	b.n	.L_02002964
.L_02002902:
	bl 0x0200cd34
	subs	r5, #1
	str	r0, [r7, #40]
	movs	r0, #1
	bl 0x0200cd24
	cmp	r5, #0
	bge.n	.L_02002902
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #8
	movs	r1, #4
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_02002948
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #14
	bl 0x0200ce9c
.L_02002948:
	movs	r3, #192
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r0, #14
	bl 0x0200ceb4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	bl 0x0200ce64
.L_02002964:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x000022f8
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #10
	bl 0x0200ce74
	ldr	r3, [r0, #8]
	movs	r1, #0
	asrs	r3, r3, #19
	mov	r8, r3
	ldr	r3, [r0, #16]
	movs	r0, #11
	mov	r9, r1
	mov	sl, r1
	asrs	r7, r3, #19
	bl 0x0200ce74
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #108]
	asrs	r6, r3, #19
	ldr	r3, [r0, #16]
	movs	r4, #1
	asrs	r3, r3, #19
	mov	fp, r3
	lsls	r1, r4, #24
	mov	ip, r2
.L_020029b0:
	mov	r0, ip
	asrs	r2, r1, #22
	ldrsh	r3, [r0, r2]
	mov	lr, r3
	cmp	r8, lr
	bne.n	.L_020029c8
	adds	r3, r2, #2
	ldrsh	r3, [r0, r3]
	cmp	r7, r3
	bne.n	.L_020029c8
	lsrs	r1, r1, #24
	mov	r9, r1
.L_020029c8:
	lsls	r2, r4, #24
	asrs	r1, r2, #22
	ldrsh	r3, [r0, r1]
	cmp	r6, r3
	bne.n	.L_020029de
	adds	r3, r1, #2
	ldrsh	r3, [r0, r3]
	cmp	fp, r3
	bne.n	.L_020029de
	lsrs	r1, r2, #24
	mov	sl, r1
.L_020029de:
	movs	r4, #128
	lsls	r4, r4, #17
	adds	r3, r2, r4
	lsrs	r4, r3, #24
	movs	r5, #176
	lsls	r1, r4, #24
	lsls	r5, r5, #20
	cmp	r1, r5
	ble.n	.L_020029b0
	mov	r1, sl
	mov	r2, r9
	lsls	r3, r1, #28
	lsls	r1, r2, #24
	orrs	r1, r3
	movs	r0, #132
	asrs	r1, r1, #24
	lsls	r0, r0, #2
	bl 0x0200cd8c
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xd30c
	.2byte 0x0200
	push	{lr}
	movs	r1, #129
	lsls	r1, r1, #2
	adds	r1, #255
	movs	r0, #11
	bl 0x0200caf0
	pop	{pc}
	push	{lr}
	movs	r1, #193
	lsls	r1, r1, #2
	movs	r0, #12
	bl 0x0200caf0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #5
	movs	r0, #13
	bl 0x0200caf0
	pop	{pc}
	.global Func_02002a44
	.thumb_func
Func_02002a44:
	push	{lr}
	ldr	r1, [pc, #80]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02002a5c
	ldr	r0, [pc, #68]
	b.n	.L_02002a96
.L_02002a5c:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02002a66
	ldr	r0, [pc, #68]
	b.n	.L_02002a96
.L_02002a66:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02002a70
	ldr	r0, [pc, #64]
	b.n	.L_02002a96
.L_02002a70:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02002a7a
	ldr	r0, [pc, #64]
	b.n	.L_02002a96
.L_02002a7a:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #9
	bne.n	.L_02002a8c
	ldr	r0, [pc, #48]
	b.n	.L_02002a96
.L_02002a8c:
	cmp	r3, #9
	ble.n	.L_02002a94
	ldr	r0, [pc, #44]
	b.n	.L_02002a96
.L_02002a94:
	ldr	r0, [pc, #44]
.L_02002a96:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000b2
	.4byte 0x0200e038
	.4byte 0x000000b3
	.4byte 0x0200e0f8
	.4byte 0x000000b4
	.4byte 0x0200e164
	.4byte 0x000000b5
	.4byte 0x0200e1a0
	.4byte 0x0200df30
	.4byte 0x0200dfcc
	.2byte 0xdee8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl 0x0200ce74
	adds	r1, r6, #0
	adds	r7, r0, #0
	movs	r2, #0
	adds	r0, r5, #0
	bl 0x0200cef4
	cmp	r7, #0
	beq.n	.L_02002b02
	movs	r1, #134
	adds	r1, #255
	ldr	r0, [r7, #80]
	bl 0x0200cd94
	movs	r3, #0
	strb	r3, [r0, #5]
	strb	r3, [r0, #6]
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200cd9c
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200cd9c
.L_02002b02:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	lsls	r5, r1, #24
	bl 0x0200ce74
	adds	r1, r0, #0
	adds	r2, r1, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r1, #52]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r1, #48]
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r1, #12]
	movs	r3, #10
	ldrsh	r2, [r1, r3]
	adds	r3, r1, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	lsrs	r5, r5, #24
	movs	r2, #18
	ldrsh	r3, [r1, r2]
	lsls	r5, r5, #24
	adds	r2, r1, #0
	adds	r2, #102
	asrs	r5, r5, #24
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200cecc
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r2, #0
	adds	r5, r1, #0
	bl 0x0200ce74
	adds	r3, r0, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r5, r5, #4
	adds	r5, r5, r3
	adds	r3, r0, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r6, r6, #4
	adds	r6, r6, r3
	lsls	r5, r5, #16
	lsls	r6, r6, #16
	ldr	r2, [r0, #12]
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl 0x0200cdcc
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #204]
	movs	r2, #2
	mov	r8, r1
	movs	r3, #192
	movs	r0, #131
	add	r2, r8
	lsls	r3, r3, #18
	lsls	r0, r0, #1
	mov	sl, r2
	ldr	r7, [r3, #108]
	bl 0x0200cd6c
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r6, r0, #0
	cmp	r3, #0
	beq.n	.L_02002bc0
	movs	r3, #1
	orrs	r6, r3
.L_02002bc0:
	movs	r5, #8
.L_02002bc2:
	adds	r0, r5, #0
	bl 0x0200ce74
	adds	r5, #1
	adds	r0, #91
	strb	r6, [r0, #0]
	cmp	r5, #16
	ble.n	.L_02002bc2
	cmp	r6, #0
	bne.n	.L_02002cac
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r1, #171
	adds	r2, r3, #1
	mov	r3, sl
	strh	r2, [r3, #0]
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002cac
	lsls	r3, r2, #16
	cmp	r3, #0
	beq.n	.L_02002cac
.L_02002bf4:
	mov	r2, r8
	ldrh	r3, [r2, #0]
	cmp	r3, #60
	beq.n	.L_02002c02
	cmp	r3, #180
	beq.n	.L_02002c68
	b.n	.L_02002c7a
.L_02002c02:
	movs	r0, #8
	movs	r1, #0
	movs	r2, #3
	bl 0x0200ab58
	movs	r0, #9
	movs	r1, #3
	movs	r2, #0
	bl 0x0200ab58
	movs	r5, #3
	movs	r0, #10
	movs	r1, #0
	movs	r2, #3
	bl 0x0200ab58
	negs	r5, r5
	movs	r0, #11
	movs	r1, #0
	movs	r2, #3
	bl 0x0200ab58
	movs	r0, #12
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x0200ab58
	movs	r0, #13
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x0200ab58
	movs	r0, #14
	movs	r1, #0
	movs	r2, #3
	bl 0x0200ab58
	movs	r0, #15
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x0200ab58
	movs	r0, #16
	movs	r1, #3
	movs	r2, #0
	bl 0x0200ab58
	b.n	.L_02002c7a
	.2byte 0x0000
	.2byte 0x234c
	.2byte 0x0200
.L_02002c68:
	movs	r5, #8
.L_02002c6a:
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	adds	r5, #1
	bl 0x0200ab58
	cmp	r5, #16
	ble.n	.L_02002c6a
.L_02002c7a:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	mov	r2, r8
	adds	r3, #1
	movs	r1, #240
	strh	r3, [r2, #0]
	lsls	r1, r1, #16
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L_02002c92
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
.L_02002c92:
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02002bf4
	b.n	.L_02002cac
	.2byte 0x0000
	.2byte 0x0000
.L_02002cac:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	ldr	r5, [pc, #100]
	mov	r8, r0
	movs	r0, #10
	adds	r0, #255
	adds	r6, r5, #2
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02002d14
	strh	r0, [r5, #0]
	strh	r0, [r6, #0]
	bl 0x0200cfe4
	bl 0x0200ce74
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r7, #0
	movs	r5, #8
	movs	r6, #2
.L_02002cec:
	ldr	r2, [pc, #56]
	lsls	r3, r7, #2
	ldrsh	r1, [r2, r3]
	ldrsh	r2, [r2, r6]
	adds	r0, r5, #0
	lsls	r1, r1, #19
	lsls	r2, r2, #19
	bl 0x0200cebc
	mov	r0, r8
	lsls	r1, r0, #24
	asrs	r1, r1, #24
	adds	r0, r5, #0
	adds	r5, #1
	bl 0x0200ab04
	adds	r6, #4
	adds	r7, #1
	cmp	r5, #16
	ble.n	.L_02002cec
.L_02002d14:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200cd2c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200234c
	.4byte 0x0200d33c
	.2byte 0xab8d
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #188
	sub	sp, #8
	bl 0x0200cd6c
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #190
	bl 0x0200cd6c
	mov	r8, r0
	mov	r2, r8
	movs	r0, #144
	lsls	r2, r2, #24
	lsls	r0, r0, #4
	lsrs	r2, r2, #24
	adds	r0, #189
	mov	r8, r2
	bl 0x0200cd6c
	adds	r6, r0, #0
	movs	r0, #140
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200cd6c
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
	lsls	r5, r5, #24
	asrs	r5, r5, #24
	mov	sl, r5
	mov	r3, r8
	lsls	r3, r3, #24
	add	r6, sl
	asrs	r7, r3, #24
	lsls	r6, r6, #24
	lsrs	r6, r6, #24
	adds	r0, r7, r0
	lsls	r0, r0, #24
	lsls	r6, r6, #24
	lsrs	r0, r0, #24
	mov	r9, r3
	asrs	r3, r6, #24
	mov	r8, r0
	lsls	r5, r0, #24
	cmp	r3, #0
	beq.n	.L_02002e8e
	cmp	r3, #2
	bne.n	.L_02002de8
	movs	r3, #24
	movs	r2, #76
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #12
	movs	r0, #74
	movs	r1, #115
	movs	r2, #7
	bl 0x0200ce04
	asrs	r3, r5, #24
	cmp	r3, #0
	bne.n	.L_02002dbc
	b.n	.L_02002f22
.L_02002dbc:
	cmp	r3, #2
	bne.n	.L_02002dcc
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #71
	b.n	.L_02002e6e
.L_02002dcc:
	cmp	r7, #0
	beq.n	.L_02002ddc
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #67
	b.n	.L_02002e6e
.L_02002ddc:
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #79
	b.n	.L_02002e6e
.L_02002de8:
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_02002e36
	movs	r3, #24
	movs	r2, #76
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #12
	movs	r0, #66
	movs	r1, #95
	movs	r2, #7
	bl 0x0200ce04
	asrs	r3, r5, #24
	cmp	r3, #0
	bne.n	.L_02002e0a
	b.n	.L_02002f22
.L_02002e0a:
	cmp	r3, #2
	bne.n	.L_02002e1a
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #81
	b.n	.L_02002e6e
.L_02002e1a:
	cmp	r7, #0
	beq.n	.L_02002e2a
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #75
	b.n	.L_02002e6e
.L_02002e2a:
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #65
	b.n	.L_02002e6e
.L_02002e36:
	movs	r3, #26
	movs	r2, #76
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #12
	movs	r0, #69
	movs	r1, #82
	movs	r2, #5
	bl 0x0200ce04
	asrs	r3, r5, #24
	cmp	r3, #0
	beq.n	.L_02002f22
	cmp	r3, #2
	bne.n	.L_02002e60
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #69
	b.n	.L_02002e6e
.L_02002e60:
	cmp	r7, #0
	beq.n	.L_02002e7a
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #73
.L_02002e6e:
	movs	r1, #110
	movs	r2, #1
	movs	r3, #3
	bl 0x0200ce04
	b.n	.L_02002e8e
.L_02002e7a:
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #77
	movs	r1, #110
	movs	r2, #1
	movs	r3, #3
	bl 0x0200ce04
.L_02002e8e:
	mov	r2, r8
	lsls	r3, r2, #24
	asrs	r0, r3, #24
	cmp	r0, #0
	beq.n	.L_02002f22
	cmp	r0, #2
	bne.n	.L_02002ec2
	movs	r3, #31
	movs	r2, #76
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #83
	movs	r1, #115
	movs	r2, #6
	movs	r3, #12
	bl 0x0200ce04
	cmp	r6, #0
	bne.n	.L_02002f22
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #82
	movs	r1, #124
	b.n	.L_02002eec
.L_02002ec2:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_02002ef6
	movs	r3, #31
	movs	r2, #76
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #76
	movs	r1, #82
	movs	r2, #4
	movs	r3, #12
	bl 0x0200ce04
	cmp	r6, #0
	bne.n	.L_02002f22
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #75
	movs	r1, #91
.L_02002eec:
	movs	r2, #1
	movs	r3, #3
	bl 0x0200ce04
	b.n	.L_02002f22
.L_02002ef6:
	movs	r3, #31
	movs	r2, #76
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #82
	movs	r1, #82
	movs	r2, #6
	movs	r3, #12
	bl 0x0200ce04
	cmp	r6, #0
	bne.n	.L_02002f22
	movs	r3, #30
	movs	r2, #85
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #81
	movs	r1, #91
	movs	r2, #1
	movs	r3, #3
	bl 0x0200ce04
.L_02002f22:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.global Func_02002f30
	.thumb_func
Func_02002f30:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r2, #129
	adds	r3, r3, r0
	lsls	r2, r2, #2
	ldr	r7, [pc, #684]
	str	r2, [r3, #0]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #672]
	sub	sp, #28
	cmp	r2, r3
	beq.n	.L_02002f68
	bl 0x0200bd8a
.L_02002f68:
	adds	r1, #2
	adds	r5, r7, r1
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #8
	ble.n	.L_02002f84
	movs	r0, #8
	bl 0x0200ce74
	movs	r1, #0
	bl 0x0200ce14
	bl 0x0200ad30
.L_02002f84:
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	cmp	r3, #20
	beq.n	.L_02002f8e
	b.n	.L_020030de
.L_02002f8e:
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200cf4c
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200cebc
	movs	r0, #9
	bl 0x0200ce94
	movs	r0, #10
	bl 0x0200ce94
	bl 0x0200cfac
	bl 0x0200cfbc
	movs	r0, #60
	bl 0x0200ce54
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	movs	r5, #20
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_0200300e
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200cd7c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #188
	bl 0x0200cd74
	movs	r5, #21
.L_0200300e:
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_0200302e
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x0200cd7c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #189
	bl 0x0200cd74
	movs	r5, #22
.L_0200302e:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #9
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02003052
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #9
	bl 0x0200cd7c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #190
	bl 0x0200cd74
	movs	r5, #23
.L_02003052:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02003076
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x0200cd7c
	movs	r0, #140
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200cd74
	movs	r5, #24
.L_02003076:
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200d014
	bl 0x0200ad30
	movs	r0, #15
	bl 0x0200ce54
	movs	r1, #3
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d014
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200ce24
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r1, r1
	negs	r0, r0
	bl 0x0200ce24
	movs	r0, #120
	bl 0x0200ce54
	movs	r1, #1
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #60
	bl 0x0200ce54
	movs	r0, #10
	adds	r0, #255
	bl 0x0200cd74
	adds	r0, r5, #0
	bl 0x0200cf6c
.L_020030de:
	ldr	r3, [pc, #284]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r0, #128
	subs	r3, #9
	lsls	r3, r3, #16
	lsls	r0, r0, #9
	cmp	r3, r0
	bhi.n	.L_02003164
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200cd74
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_0200313a
	movs	r0, #8
	movs	r1, #4
	bl 0x0200cecc
	movs	r3, #30
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #85
	movs	r1, #72
	movs	r2, #5
	movs	r3, #2
	bl 0x0200ce04
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
.L_0200313a:
	movs	r0, #144
	lsls	r0, r0, #4
.L_0200313e:
	adds	r0, #238
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02003164
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #186
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02003164
	movs	r1, #232
	movs	r2, #164
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200cebc
.L_02003164:
	ldr	r1, [pc, #148]
	movs	r6, #241
	mov	r8, r1
	lsls	r6, r6, #1
	add	r6, r8
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #9
	beq.n	.L_02003178
	b.n	.L_020036f8
.L_02003178:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_0200318a
	bl 0x0200c24c
.L_0200318a:
	movs	r0, #53
	bl 0x0200d014
	movs	r0, #196
	adds	r0, #255
	bl 0x0200ce4c
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	bne.n	.L_020031a2
	b.n	.L_020036ba
.L_020031a2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #172
	bl 0x0200cd6c
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L_02003204
	movs	r0, #8
	movs	r1, #2
	bl 0x0200cecc
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r1, #216
	movs	r2, #160
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #17
	bl 0x0200cebc
	movs	r0, #10
	movs	r1, #3
	bl 0x0200cf24
	movs	r3, #27
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #30
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	bl 0x0200cdfc
	movs	r0, #10
	movs	r1, #5
	bl 0x0200cecc
	bl 0x0200c24c
	movs	r0, r0
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	lsls	r1, r6, #2
	movs	r0, r0
.L_02003204:
	movs	r0, #10
	bl 0x0200ce74
	mov	fp, r0
	movs	r0, #8
	bl 0x0200ce74
	mov	r8, r0
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d014
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #172
	bl 0x0200cd74
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #222
	bl 0x0200cd74
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200cebc
	movs	r0, #9
	movs	r1, #1
	bl 0x0200ce84
	movs	r1, #228
	movs	r2, #180
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #17
	bl 0x0200cebc
	movs	r0, #10
	movs	r1, #1
	bl 0x0200ce84
	movs	r1, #224
	movs	r2, #0
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200cf1c
	movs	r0, #8
	movs	r1, #2
	bl 0x0200cecc
	bl 0x0200cfac
	bl 0x0200cfbc
	movs	r0, #244
	movs	r2, #192
	movs	r3, #1
	adds	r1, r5, #0
	lsls	r0, r0, #17
	lsls	r2, r2, #17
	bl 0x0200cf4c
	bl 0x0200cf54
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200cf2c
	movs	r0, #40
	bl 0x0200ce54
	ldr	r0, [pc, #712]
	bl 0x0200cf0c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r0, #10
	movs	r1, #3
	bl 0x0200ced4
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r0, #10
	movs	r1, #8
	bl 0x0200cecc
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r3, #192
	lsls	r3, r3, #9
	mov	r0, fp
	movs	r1, #244
	movs	r2, #164
	str	r3, [r0, #48]
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200ce9c
	movs	r0, #5
	bl 0x0200ce54
	movs	r1, #6
	movs	r0, #10
	bl 0x0200cecc
	movs	r0, #10
	bl 0x0200ce54
	movs	r1, #192
	mov	r2, fp
	lsls	r1, r1, #11
	str	r1, [r2, #40]
	movs	r0, #10
	bl 0x0200ce54
	movs	r3, #91
	add	r3, fp
	mov	r9, r3
	mov	r7, r8
	movs	r3, #1
	adds	r7, #91
	mov	r0, r9
	strb	r3, [r0, #0]
	movs	r1, #0
	strb	r3, [r7, #0]
	mov	r0, fp
	bl 0x0200cda4
	mov	r0, r8
	movs	r1, #0
	bl 0x0200cda4
	mov	r0, fp
	movs	r1, #6
	bl 0x0200ce34
	mov	r0, r8
	movs	r1, #6
	bl 0x0200ce34
	movs	r5, #30
	movs	r2, #5
	movs	r3, #2
	movs	r6, #19
	movs	r1, #76
	movs	r0, #85
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce04
	movs	r0, #143
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200cf94
	movs	r0, #10
	bl 0x0200cfa4
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200cf94
	movs	r0, #10
	bl 0x0200cfa4
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #60
	bl 0x0200ce54
	mov	r0, fp
	movs	r1, #0
	bl 0x0200ce34
	mov	r0, r8
	movs	r1, #0
	bl 0x0200ce34
	movs	r3, #2
	movs	r0, #85
	movs	r1, #74
	movs	r2, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce04
	mov	r2, r9
	mov	r1, sl
	strb	r1, [r2, #0]
	mov	r0, fp
	strb	r1, [r7, #0]
	movs	r1, #16
	bl 0x0200cda4
	movs	r1, #16
	mov	r0, r8
	bl 0x0200cda4
	movs	r0, #10
	bl 0x0200ce74
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #228
	movs	r2, #180
	lsls	r2, r2, #1
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #10
	movs	r6, #128
	bl 0x0200cea4
	lsls	r6, r6, #11
	mov	r3, fp
	movs	r1, #129
	str	r6, [r3, #40]
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200cf34
	movs	r0, #10
	bl 0x0200ceb4
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #10
	bl 0x0200cf2c
	movs	r0, #30
	bl 0x0200ce54
	movs	r1, #8
	movs	r0, #10
	bl 0x0200cecc
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #192
	lsls	r0, r0, #9
	mov	r1, fp
	str	r0, [r1, #48]
	movs	r2, #164
	movs	r1, #244
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #10
	bl 0x0200ce9c
	movs	r0, #14
	bl 0x0200ce54
	movs	r2, #192
	lsls	r2, r2, #11
	mov	r3, fp
	str	r2, [r3, #40]
	movs	r1, #6
	movs	r0, #10
	bl 0x0200cecc
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #143
	lsls	r0, r0, #2
	bl 0x0200d014
	movs	r0, #10
	bl 0x0200ce74
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #228
	ands	r5, r3
	movs	r2, #180
	lsls	r2, r2, #1
	strb	r5, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200cea4
	mov	r0, fp
	movs	r1, #129
	lsls	r1, r1, #1
	str	r6, [r0, #40]
	movs	r0, #10
	bl 0x0200cf34
	movs	r0, #10
	bl 0x0200ceb4
	movs	r1, #0
	movs	r0, #10
	bl 0x0200cf14
	movs	r0, #15
	bl 0x0200ce54
	movs	r1, #0
	mov	r0, r8
	bl 0x0200cda4
	movs	r0, #15
	bl 0x0200ce54
	movs	r0, #215
	bl 0x0200d014
	movs	r0, #30
	movs	r1, #244
	ldr	r3, [pc, #192]
	ldr	r2, [pc, #196]
	lsls	r1, r1, #17
	adds	r0, #255
	bl 0x0200cdb4
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200ce14
	adds	r0, r5, #0
	ldr	r1, [pc, #180]
	bl 0x0200cdac
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cd9c
	adds	r0, r5, #0
	movs	r1, #6
	bl 0x0200ce34
	movs	r1, #2
	movs	r2, #0
	movs	r0, #10
	bl 0x0200cedc
	movs	r0, #5
	bl 0x0200ce54
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200cf2c
	movs	r0, #55
	bl 0x0200ce54
	movs	r0, #5
	bl 0x0200ce54
	movs	r0, #136
	bl 0x0200d014
	movs	r0, #206
	movs	r1, #244
	ldr	r2, [pc, #104]
	ldr	r3, [pc, #96]
	lsls	r1, r1, #17
	lsls	r0, r0, #1
	bl 0x0200cdb4
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200ce14
	ldr	r1, [pc, #92]
	adds	r0, r5, #0
	bl 0x0200cdac
	ldr	r2, [r5, #80]
	movs	r3, #128
	ldr	r1, [pc, #60]
	lsls	r3, r3, #6
	strh	r3, [r2, #18]
	movs	r2, #192
	adds	r3, r5, #0
	lsls	r2, r2, #9
	adds	r3, #85
	str	r2, [r5, #48]
	strb	r1, [r3, #0]
	movs	r3, #228
	lsls	r3, r3, #17
	str	r3, [r5, #56]
	ldr	r3, [pc, #60]
	movs	r0, #10
	str	r3, [r5, #60]
	movs	r3, #179
	lsls	r3, r3, #17
	str	r3, [r5, #64]
	movs	r3, #192
	lsls	r3, r3, #7
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	bl 0x0200ce54
	movs	r1, #16
	mov	r0, r8
	bl 0x0200cda4
	movs	r0, #10
	b.n	.L_02003588
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00002303
	.4byte 0x01590000
	.4byte 0xfff00000
	.4byte 0x0200d39c
	.4byte 0x0200d360
	.2byte 0x0000
	.2byte 0xffe0
.L_02003588:
	.2byte 0xf001
	.2byte 0xfc64
	.2byte 0x2085
	bl 0x0200d014
	mov	r3, sl
	movs	r2, #128
	str	r3, [r5, #8]
	str	r3, [r5, #16]
	adds	r0, r6, #0
	adds	r1, r6, #0
	lsls	r2, r2, #9
	bl 0x0200ce24
	movs	r7, #0
.L_020035a6:
	movs	r1, #8
	negs	r1, r1
	cmp	r7, #1
	ble.n	.L_020035b0
	movs	r1, #8
.L_020035b0:
	movs	r3, #1
	movs	r2, #8
	ands	r3, r7
	negs	r2, r2
	cmp	r3, #0
	bne.n	.L_020035be
	movs	r2, #8
.L_020035be:
	movs	r0, #228
	lsls	r0, r0, #17
	lsls	r3, r2, #16
	lsls	r1, r1, #16
	movs	r2, #179
	adds	r1, r1, r0
	lsls	r2, r2, #17
	movs	r0, #148
	adds	r3, r3, r2
	adds	r0, #255
	ldr	r2, [pc, #272]
	bl 0x0200cdb4
	lsls	r5, r7, #2
	add	r6, sp, #12
	str	r0, [r6, r5]
	movs	r1, #2
	bl 0x0200cd9c
	ldr	r0, [r6, r5]
	movs	r1, #0
	bl 0x0200ce14
	adds	r7, #1
	ldr	r0, [r6, r5]
	ldr	r1, [pc, #244]
	bl 0x0200cdac
	cmp	r7, #3
	ble.n	.L_020035a6
	movs	r3, #160
	lsls	r3, r3, #12
	mov	r0, fp
	str	r3, [r0, #40]
	movs	r0, #10
	bl 0x0200ce74
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
.L_02003610:
	movs	r1, #216
	movs	r2, #160
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #10
	bl 0x0200cea4
	movs	r0, #10
	bl 0x0200ce54
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200ce24
	movs	r1, #5
	movs	r0, #10
	bl 0x0200cecc
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #10
	movs	r1, #1
	bl 0x0200cf5c
	bl 0x0200cf54
	ldr	r5, [pc, #152]
	movs	r0, #10
	adds	r1, r5, #0
	bl 0x0200ce84
	movs	r0, #10
	bl 0x0200ce8c
	movs	r0, #20
	bl 0x0200ce54
	adds	r1, r5, #0
	movs	r0, #10
	bl 0x0200ce84
	movs	r0, #20
	bl 0x0200ce54
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	ldr	r3, [pc, #108]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200cf3c
	bl 0x0200cf54
	movs	r0, #10
	movs	r1, #3
	bl 0x0200cf24
	movs	r3, #27
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #30
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	bl 0x0200cdfc
	bl 0x0200ce64
	bl 0x0200c24c
.L_020036ba:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_020036ca
	bl 0x0200c24c
.L_020036ca:
	movs	r1, #236
	movs	r2, #228
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200cebc
	ldr	r1, [pc, #24]
	movs	r0, #10
	bl 0x0200ce84
	bl 0x0200c24c
	movs	r0, r0
	.2byte 0xffe0
	.2byte 0xd39c
	lsls	r0, r0, #8
	udf	#40
	lsls	r0, r0, #8
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	bvc.n	.L_02003610
	lsls	r0, r0, #8
.L_020036f8:
	cmp	r3, #10
	beq.n	.L_020036fe
	b.n	.L_02003ca0
.L_020036fe:
	movs	r0, #7
	bl 0x0200d014
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd6c
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L_02003718
	bl 0x0200c24c
.L_02003718:
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200cd74
	ldr	r0, [pc, #908]
	bl 0x0200cf0c
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200cebc
	movs	r1, #3
	movs	r0, #8
	bl 0x0200cecc
	movs	r0, #11
	bl 0x0200ce74
	movs	r2, #128
	lsls	r2, r2, #9
	str	r2, [r0, #48]
	movs	r0, #10
	mov	sl, r2
	bl 0x0200ce74
	movs	r6, #128
	adds	r5, r0, #0
	lsls	r6, r6, #8
	movs	r1, #220
	movs	r2, #156
	movs	r7, #133
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	str	r6, [r5, #48]
	lsls	r7, r7, #2
	add	r7, r8
	bl 0x0200cebc
	movs	r1, #228
	movs	r2, #164
	ldr	r0, [r7, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200cebc
	movs	r1, #244
	movs	r2, #212
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200cebc
	ldr	r0, [r7, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cf1c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cf1c
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cf1c
	bl 0x0200cfac
	bl 0x0200cfbc
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200cf2c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d014
	movs	r1, #0
	movs	r0, #11
	bl 0x0200cf14
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d014
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r7, #0]
	bl 0x0200cf2c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #11
	bl 0x0200cf2c
	movs	r0, #15
	bl 0x0200ce54
	movs	r1, #128
	ldr	r0, [r7, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cf1c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #11
	bl 0x0200cf1c
	movs	r0, #30
	bl 0x0200ce54
	movs	r0, #10
	movs	r1, #1
	bl 0x0200cf5c
	bl 0x0200cf54
	movs	r0, #10
	movs	r1, #2
	bl 0x0200cee4
	movs	r0, #10
	movs	r1, #1
	bl 0x0200cf3c
	movs	r1, #244
	movs	r2, #188
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200cea4
	movs	r0, #10
	bl 0x0200ceb4
	movs	r1, #2
	movs	r0, #10
	bl 0x0200cee4
	movs	r0, #20
	bl 0x0200ce54
	movs	r1, #236
	movs	r2, #180
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #10
	bl 0x0200cea4
	movs	r0, #10
	bl 0x0200ceb4
	ldr	r0, [r7, #0]
	movs	r1, #10
	movs	r2, #0
	bl 0x0200cef4
	movs	r0, #11
	movs	r1, #10
	movs	r2, #0
	bl 0x0200cef4
	movs	r0, #10
	movs	r1, #8
	movs	r2, #0
	bl 0x0200cef4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	bl 0x0200cf4c
	movs	r0, #10
	movs	r1, #8
	bl 0x0200cecc
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r3, #192
	lsls	r3, r3, #9
	movs	r1, #244
	movs	r2, #164
	lsls	r2, r2, #1
	str	r3, [r5, #48]
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200ce9c
	movs	r0, #5
	bl 0x0200ce54
	movs	r1, #6
	movs	r0, #10
	bl 0x0200cecc
	movs	r0, #10
	bl 0x0200ce54
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #143
	lsls	r0, r0, #2
	bl 0x0200d014
	adds	r2, r5, #0
	movs	r0, #254
	adds	r2, #91
	movs	r3, #1
	lsls	r0, r0, #7
	strb	r3, [r2, #0]
	movs	r1, #0
	adds	r0, #255
	bl 0x0200cf94
	movs	r0, #10
	bl 0x0200cfa4
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d014
	movs	r1, #0
	mov	r0, sl
	bl 0x0200cf94
	movs	r0, #10
	bl 0x0200cfa4
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200cf94
	movs	r0, #10
	bl 0x0200cfa4
	movs	r0, #10
	bl 0x0200ce54
	movs	r1, #0
	mov	r0, sl
	bl 0x0200cf94
	movs	r0, #10
	bl 0x0200cfa4
	movs	r0, #10
	bl 0x0200ce54
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200cf94
	movs	r0, #10
	bl 0x0200cfa4
	movs	r0, #60
	bl 0x0200ce54
	movs	r0, #8
	movs	r1, #4
	bl 0x0200cecc
	movs	r3, #30
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #85
	movs	r1, #72
	movs	r2, #5
	movs	r3, #2
	bl 0x0200ce04
	ldr	r3, [pc, #300]
	movs	r1, #228
	str	r3, [r5, #12]
	movs	r2, #180
	mov	r3, r9
	str	r3, [r5, #40]
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #17
	str	r6, [r5, #48]
	bl 0x0200cebc
	movs	r0, #10
	movs	r1, #1
	bl 0x0200cecc
	movs	r1, #0
	mov	r0, sl
	bl 0x0200cf94
	movs	r0, #10
	bl 0x0200cfa4
	movs	r0, #10
	bl 0x0200ce54
	movs	r2, #0
	ldr	r1, [r7, #0]
	movs	r0, #10
	bl 0x0200cef4
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r0, #10
	movs	r1, #3
	bl 0x0200ced4
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r1, #128
	movs	r2, #0
	movs	r0, #10
	lsls	r1, r1, #7
	bl 0x0200cf1c
	movs	r0, #10
	movs	r1, #2
	bl 0x0200cecc
	movs	r1, #244
	movs	r2, #196
	lsls	r2, r2, #1
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200cea4
	movs	r1, #1
	movs	r0, #11
	bl 0x0200cee4
	movs	r0, #5
	bl 0x0200ce54
	movs	r1, #0
	movs	r0, #11
	bl 0x0200cf14
	movs	r0, #10
	bl 0x0200ceb4
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cecc
	movs	r2, #0
	movs	r1, #11
	movs	r0, #10
	bl 0x0200cef4
	movs	r0, #15
	bl 0x0200ce54
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cf14
	movs	r0, #10
	movs	r1, #3
	bl 0x0200ced4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #10
	lsls	r1, r1, #7
	bl 0x0200cf1c
	movs	r0, #10
	movs	r1, #2
	bl 0x0200cecc
	movs	r1, #244
	movs	r2, #134
	lsls	r2, r2, #2
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200cea4
	ldr	r0, [r7, #0]
	movs	r1, #1
	bl 0x0200cf3c
	ldr	r0, [r7, #0]
	movs	r1, #11
	movs	r2, #0
	bl 0x0200cef4
	movs	r2, #0
.L_02003a82:
	ldr	r1, [r7, #0]
	movs	r0, #11
	bl 0x0200cef4
	movs	r0, #30
.L_02003a8c:
	bl 0x0200ce54
	movs	r0, #11
	movs	r1, #3
	bl 0x0200ced4
	movs	r0, #11
	movs	r1, #2
	bl 0x0200cecc
	ldr	r0, [r7, #0]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_02003ac4
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #11
	bl 0x0200ce9c
	b.n	.L_02003ac4
	.2byte 0x0000
	.4byte 0x0000230c
	.2byte 0x0000
	.2byte 0xffe0
.L_02003ac4:
	.2byte 0x200b
	bl 0x0200ceb4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #11
	bl 0x0200cebc
	movs	r0, #11
	bl 0x0200ceb4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r1, #244
	movs	r2, #188
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r7, #0]
	bl 0x0200cea4
	ldr	r0, [r7, #0]
	bl 0x0200ceb4
	movs	r0, #160
	movs	r1, #160
	lsls	r1, r1, #11
	mov	r2, sl
	lsls	r0, r0, #11
	bl 0x0200ce24
	movs	r0, #107
	bl 0x0200d014
	movs	r0, #30
	bl 0x0200ce54
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r7, #0]
	bl 0x0200cf2c
	movs	r0, #15
	bl 0x0200ce54
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r7, #0]
	bl 0x0200cf1c
	movs	r0, #15
	bl 0x0200ce54
	movs	r0, #12
	bl 0x0200ce74
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	ldrh	r3, [r3, #0]
	mov	r5, sp
	adds	r5, #10
	mov	r8, r0
	movs	r1, #160
	movs	r0, #160
	strh	r3, [r5, #0]
	lsls	r1, r1, #12
	mov	r2, sl
	lsls	r0, r0, #12
	bl 0x0200ce24
	movs	r0, #107
	bl 0x0200d014
	bl 0x0200cddc
	ldrb	r3, [r5, #0]
	strh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02003b98
.L_02003b6c:
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #36]
	movs	r1, #128
	lsls	r1, r1, #19
	orrs	r3, r2
	adds	r1, #82
	strh	r3, [r1, #0]
	movs	r0, #255
	ldrh	r3, [r5, #0]
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r3, r0
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200cd24
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02003b6c
	b.n	.L_02003b98
	.2byte 0x1000
	.2byte 0x0000
.L_02003b98:
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #30
	movs	r6, #232
	movs	r5, #174
	bl 0x0200ce54
	lsls	r5, r5, #17
	lsls	r6, r6, #17
	movs	r3, #68
	adds	r2, r5, #0
	ldr	r1, [pc, #184]
	adds	r3, #255
	adds	r0, r6, #0
	bl 0x02008080
	adds	r2, r5, #0
	adds	r7, r0, #0
	adds	r1, r6, #0
	movs	r0, #12
	bl 0x0200cebc
	movs	r5, #0
.L_02003bd2:
	adds	r0, r5, #0
	movs	r1, #5
	bl 0x0200cd1c
	cmp	r0, #0
	bne.n	.L_02003be2
	bl 0x02009c9c
.L_02003be2:
	mov	r1, r8
	ldr	r3, [r1, #16]
	ldr	r2, [pc, #144]
	movs	r0, #2
	adds	r3, r3, r2
	str	r3, [r1, #16]
	adds	r5, #1
	bl 0x0200ce54
	cmp	r5, #19
	ble.n	.L_02003bd2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #238
	bl 0x0200cd74
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200ce24
	movs	r3, #0
	str	r3, [r7, #8]
	str	r3, [r7, #16]
	movs	r0, #1
	bl 0x0200ce54
	adds	r0, r7, #0
	bl 0x0200cdbc
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d014
	movs	r0, #30
	bl 0x0200ce54
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	mov	r6, sp
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_02003c42:
	adds	r6, #10
	ldrh	r3, [r6, #0]
	ldr	r2, [pc, #36]
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldrh	r3, [r6, #0]
	cmp	r3, #8
	beq.n	.L_02003c96
.L_02003c5e:
	ldrh	r3, [r6, #0]
	ldr	r2, [pc, #8]
	movs	r1, #128
	lsls	r1, r1, #19
	b.n	.L_02003c7c
	.4byte 0x00000f00
	.4byte 0x00001000
	.4byte 0x00003f42
	.4byte 0xffe00000
	.2byte 0x0000
	.2byte 0xffff
.L_02003c7c:
	.2byte 0x4313
	adds	r1, #82
	strh	r3, [r1, #0]
	adds	r5, r6, #0
	ldrh	r3, [r6, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r6, #0]
	bl 0x0200cd24
	ldrh	r3, [r5, #0]
	cmp	r3, #8
	bne.n	.L_02003c5e
.L_02003c96:
	bl 0x0200cdd4
	bl 0x0200ce64
	b.n	.L_0200424c
.L_02003ca0:
	cmp	r3, #11
	bne.n	.L_02003cea
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r5, #133
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	lsls	r5, r5, #2
	movs	r1, #224
	movs	r2, #172
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	add	r5, r8
	bl 0x0200cebc
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #10
	bl 0x0200cef4
	movs	r1, #19
	ldr	r0, [r5, #0]
	bl 0x0200cecc
	bl 0x0200cfac
	bl 0x0200cfbc
	ldr	r0, [pc, #916]
	b.n	.L_02003d46
.L_02003cea:
	cmp	r3, #12
	beq.n	.L_02003cf0
	b.n	.L_0200424c
.L_02003cf0:
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cebc
	movs	r1, #215
	movs	r2, #160
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #17
	bl 0x0200cebc
	movs	r0, #10
	movs	r1, #5
	bl 0x0200cecc
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #19
	bl 0x0200cecc
	bl 0x0200cfac
	bl 0x0200cfbc
	movs	r0, #30
	bl 0x0200ce54
	movs	r1, #2
	movs	r0, #10
	bl 0x0200cee4
	movs	r0, #30
	bl 0x0200ce54
	ldr	r0, [pc, #824]
.L_02003d46:
	bl 0x0200cf0c
	movs	r1, #0
	movs	r0, #10
	bl 0x0200cf14
	bl 0x0200cfb4
	bl 0x0200cfbc
	movs	r3, #242
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r3, #243
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl 0x0200cf64
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r2, #0
	ldrsh	r1, [r6, r2]
	bl 0x0200d004
	bl 0x0200ce64
	b.n	.L_0200424c
	.2byte 0x4bbe
	.4byte 0xd171429a
	.4byte 0xf0012008
	.4byte 0x1c02f86f
	.4byte 0x78133223
	.4byte 0x432b2520
	.4byte 0x21037013
	.4byte 0xf84af001
	.4byte 0xf0012009
	.4byte 0x1c02f863
	.4byte 0x78133223
	.4byte 0x431d2103
	.4byte 0xf0017015
	.4byte 0x2008f83f
	.4byte 0xf7fe210c
	.4byte 0x2009fe81
	.4byte 0xf7fe210c
	.4byte 0x20a0fe7d
	.4byte 0x30620100
	.4byte 0xffcaf000
	.4byte 0xd00e2800
	.4byte 0x22ac21dc
	.4byte 0x04492008
	.4byte 0xf0010492
	.4byte 0x218af869
	.4byte 0x200922ac
	.4byte 0x04920489
	.4byte 0xf862f001
	.4byte 0x2359e012
	.4byte 0x93002528
	.4byte 0x21282058
	.4byte 0x23022201
	.4byte 0xf0009501
	.4byte 0x2364fff3
	.4byte 0x20639300
	.4byte 0x22012128
	.4byte 0x95012302
	.4byte 0xffeaf000
	.4byte 0x00802084
	.4byte 0xffaef000
	.4byte 0x20841c05
	.4byte 0x0080230f
	.4byte 0xf000401d
	.4byte 0x1100ffa7
	.4byte 0x0e060600
	.4byte 0xd0092d00
	.4byte 0x00ab4a91
	.4byte 0x33025ed1
	.4byte 0x04c95ed2
	.4byte 0x200a04d2
	.4byte 0xf834f001
	.4byte 0x16180633
	.4byte 0xd1002800
	.4byte 0x4a8ae1f6
	.4byte 0x5ed10083
	.4byte 0x5ed23302
	.4byte 0x04d204c9
	.4byte 0xf001200b
	.4byte 0xe1ebf825
	.4byte 0x429a4b85
	.4byte 0x20a0d10b
	.4byte 0x30630100
	.4byte 0xff74f000
	.4byte 0xd1002800
	.4byte 0x2001e1e0
	.4byte 0xff12f7fe
	.4byte 0x4b7fe1dc
	.4byte 0xd000429a
	.4byte 0x200ae13a
	.4byte 0xffeaf000
	.4byte 0x1c062103
	.4byte 0xffcaf000
	.4byte 0x210a2008
	.4byte 0xfe0cf7fe
	.4byte 0x210b2009
	.4byte 0xfe08f7fe
	.4byte 0x01002090
	.4byte 0xf00030bc
	.4byte 0x2800ff55
	.4byte 0x231ad109
	.4byte 0x9300224d
	.4byte 0x20199201
	.4byte 0x2201214d
	.4byte 0xf000230f
	.4byte 0x2090ff8d
	.4byte 0x30bd0100
	.4byte 0xff44f000
	.4byte 0xd0222800
	.4byte 0x22b021f0
	.4byte 0x04492008
	.4byte 0xf0000452
	.4byte 0x2355ffe3
	.4byte 0x251c9301
	.4byte 0x215d2002
	.4byte 0x23052201
	.4byte 0xf0009500
	.4byte 0x2616ff75
	.4byte 0x2117201b
	.4byte 0x23012201
	.4byte 0x96019500
	.4byte 0xff70f000
	.4byte 0x9300231d
	.4byte 0x21102020
	.4byte 0x23012202
	.4byte 0xf0009601
	.4byte 0x2090ff67
	.4byte 0x30be0100
	.4byte 0xff1af000
	.4byte 0xd0222800
	.4byte 0x22b02198
	.4byte 0x04892009
	.4byte 0xf0000452
	.4byte 0x2355ffb9
	.4byte 0x25249301
	.4byte 0x215d2002
	.4byte 0x23052201
	.4byte 0xf0009500
	.4byte 0x2616ff4b
	.4byte 0x21172023
	.4byte 0x23012201
	.4byte 0x96019500
	.4byte 0xff46f000
	.4byte 0x93002325
	.4byte 0x21102020
	.4byte 0x23012202
	.4byte 0xf0009601
	.4byte 0x208cff3d
	.4byte 0x30ff0100
	.4byte 0xfef0f000
	.4byte 0xd0132800
	.4byte 0x221a2325
	.4byte 0x92019300
	.4byte 0x2100203d
	.4byte 0x23032203
	.4byte 0xff30f000
	.4byte 0x22192326
	.4byte 0x92019300
	.4byte 0x21172024
	.4byte 0x23012201
	.4byte 0xff22f000
	.4byte 0x004921f1
	.4byte 0x2200187b
	.4byte 0x2b065e9b
	.4byte 0xe084d000
	.4byte 0x4698233c
	.4byte 0xff46f000
	.4byte 0xf0002000
	.4byte 0x2001fffb
	.4byte 0x22012101
	.4byte 0x42402300
	.4byte 0x42524249
	.4byte 0xffb2f000
	.4byte 0x2100200b
	.4byte 0xf0002200
	.4byte 0x2085ff65
	.4byte 0x183b0080
	.4byte 0x21006818
	.4byte 0xf0002200
	.4byte 0xf000ff5d
	.4byte 0x200fffd3
	.4byte 0xff24f000
	.4byte 0x22c021a8
	.4byte 0x04890452
	.4byte 0xf000200b
	.4byte 0x200bff51
	.4byte 0xff2af000
	.4byte 0xf0002100
	.4byte 0x200bfef7
	.4byte 0xff24f000
	.4byte 0x20a21c06
	.4byte 0x68f268b1
	.4byte 0x00406933
	.4byte 0xfebcf000
	.4byte 0x2d001c05
	.4byte 0x2100d016
	.4byte 0xfee6f000
	.4byte 0x1c284912
	.4byte 0xfeaef000
	.4byte 0x21011c28
	.4byte 0xfea2f000
	.4byte 0x1c28210e
	.4byte 0xfeeaf000
	.4byte 0x220c6d29
	.4byte 0x43137a4b
	.4byte 0x4b0b724b
	.4byte 0x61eb61ab
	.4byte 0x218068f3
	.4byte 0x185b0409
	.4byte 0xe01260f3
	.4byte 0x00002302
	.4byte 0x0000230b
	.4byte 0x000000b2
	.4byte 0x0200d30c
	.4byte 0x000000b4
	.4byte 0x000000b3
	.4byte 0x0200d3d8
	.4byte 0x00013333
	.4byte 0x42522201
	.4byte 0x46434490
	.4byte 0xd0072b00
	.4byte 0xf0002001
	.4byte 0x6ab3fe3b
	.4byte 0x42402001
	.4byte 0xddf14283
	.4byte 0xf00020bc
	.4byte 0x201effab
	.4byte 0xfec8f000
	.4byte 0x30ff200a
	.4byte 0xfe54f000
	.4byte 0xf0002003
	.4byte 0x2090ff4d
	.4byte 0x30c40100
	.4byte 0xfe48f000
	.4byte 0xd1042800
	.4byte 0x2100200b
	.4byte 0xf0002200
	.4byte 0x200afee9
	.4byte 0xf00030ff
	.4byte 0x2800fe3d
	.4byte 0xe0a9d000
	.4byte 0x21f14b5a
	.4byte 0x185b0049
	.4byte 0x5e9b2200
	.4byte 0xd0002b03
	.4byte 0xf7fce0a0
	.4byte 0xe09df98f
	.4byte 0x429a4b55
	.4byte 0xe099d000
	.4byte 0x2101200a
	.4byte 0xff66f000
	.4byte 0x21092008
	.4byte 0xfcd0f7fe
	.4byte 0x004020f1
	.4byte 0x2100183b
	.4byte 0x2b015e5b
	.4byte 0x2b03d003
	.4byte 0x2b05d001
	.4byte 0xf000d101
	.4byte 0x2081fddf
	.4byte 0x30ff0080
	.4byte 0xfe10f000
	.4byte 0xd0082800
	.4byte 0x229621c0
	.4byte 0x044923aa
	.4byte 0x049b03d2
	.4byte 0xf000200b
	.4byte 0x20c1fc7d
	.4byte 0xf0000080
	.4byte 0x2800fe01
	.4byte 0x22ccd007
	.4byte 0x493e23aa
	.4byte 0x049b0392
	.4byte 0xf000200c
	.4byte 0x20c0fc6f
	.4byte 0x30050080
	.4byte 0xfdf2f000
	.4byte 0xd0072800
	.4byte 0x23aa22dc
	.4byte 0x03924937
	.4byte 0x200d049b
	.4byte 0xfc60f000
	.4byte 0x01002090
	.4byte 0xf00030bc
	.4byte 0x2800fde3
	.4byte 0x21c0d021
	.4byte 0x200822b8
	.4byte 0x04520449
	.4byte 0xfe82f000
	.4byte 0x93012355
	.4byte 0x20012519
	.4byte 0x22012155
	.4byte 0x95002304
	.4byte 0xfe14f000
	.4byte 0x201b2617
	.4byte 0x2202210f
	.4byte 0x96002301
	.4byte 0xf0009601
	.4byte 0x2019fe0f
	.4byte 0x22012119
	.4byte 0x95002301
	.4byte 0xf0009601
	.4byte 0x200ffe07
	.4byte 0xfe40f000
	.4byte 0xf0002100
	.4byte 0x2090fe0d
	.4byte 0x30c40100
	.4byte 0xfdb4f000
	.4byte 0xd0212800
	.4byte 0x93002368
	.4byte 0x20752514
	.4byte 0x22072114
	.4byte 0x95012308
	.4byte 0xfdf4f000
	.4byte 0x9300236f
	.4byte 0x210c206f
	.4byte 0x23062201
	.4byte 0xf0009501
	.4byte 0x2328fdeb
	.4byte 0x93002254
	.4byte 0x20319201
	.4byte 0x22082154
	.4byte 0xf0002308
	.4byte 0x200ffddd
	.4byte 0x22002100
	.2byte 0xf000
	.2byte 0xfe38
.L_0200424c:
	movs	r0, #0
	bl 0x0200cfc4
	movs	r0, #0
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b5
	.4byte 0x01cf0000
	.2byte 0x0000
	.2byte 0x0235
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #1
	bl 0x0200c95c
	pop	{pc}
	.global Func_02004280
	.thumb_func
Func_02004280:
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #176]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #168]
	sub	sp, #4
	cmp	r2, r3
	beq.n	.L_02004298
	b.n	.L_0200441c
.L_02004298:
	movs	r3, #192
	movs	r2, #241
	lsls	r3, r3, #18
	lsls	r2, r2, #1
	ldr	r7, [r3, #32]
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #4
	ble.n	.L_020042ae
	b.n	.L_020043ac
.L_020042ae:
	adds	r2, #50
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl 0x0200cd64
	adds	r2, r0, #0
	movs	r5, #0
	adds	r2, #13
.L_020042be:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	adds	r5, r5, r3
	cmp	r0, r2
	ble.n	.L_020042be
	adds	r0, r5, #0
	bl 0x0200c4f8
	bl 0x0200cfe4
	bl 0x0200ce74
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r3, #6
	adds	r3, #255
	adds	r2, r7, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #12
	ldrh	r2, [r0, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r1, sp
	adds	r3, #252
	adds	r1, #2
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	orrs	r3, r2
	strh	r3, [r0, #0]
	bl 0x0200c8a0
	ldr	r5, [pc, #48]
	ldr	r3, [r5, #0]
	cmp	r3, #3
	bne.n	.L_02004340
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #246
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02004340
	movs	r1, #208
	movs	r2, #176
	movs	r0, #64
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	bl 0x0200cebc
	b.n	.L_02004340
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x000000b1
	.2byte 0x264c
	.2byte 0x0200
.L_02004340:
	ldr	r3, [r5, #0]
	cmp	r3, #9
	bne.n	.L_02004354
	movs	r1, #208
	movs	r2, #144
	movs	r0, #65
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	bl 0x0200cebc
.L_02004354:
	ldr	r0, [r5, #0]
	movs	r2, #165
	lsls	r2, r2, #4
	adds	r0, r0, r2
	bl 0x0200cd6c
	cmp	r0, #0
	bne.n	.L_02004366
	b.n	.L_020044c6
.L_02004366:
	movs	r0, #8
	bl 0x0200ce74
	adds	r6, r0, #0
	bl 0x0200c904
	movs	r1, #192
	adds	r5, r0, #0
	lsls	r1, r1, #8
	adds	r7, r6, #0
	ands	r1, r5
	movs	r3, #0
	adds	r7, #89
	lsrs	r1, r1, #14
	strb	r3, [r7, #0]
	adds	r1, #2
	adds	r0, r6, #0
	bl 0x0200cd9c
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0200439a
	ldr	r3, [pc, #308]
	b.n	.L_0200439c
.L_0200439a:
	ldr	r3, [pc, #308]
.L_0200439c:
	str	r3, [r6, #108]
	movs	r3, #1
	movs	r0, #128
	strb	r3, [r7, #0]
	lsls	r0, r0, #2
	bl 0x0200cd74
	b.n	.L_020044c6
.L_020043ac:
	cmp	r3, #8
	bgt.n	.L_020043b2
	b.n	.L_020044c6
.L_020043b2:
	ldr	r2, [pc, #288]
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #188
	lsls	r3, r3, #1
	adds	r5, r7, r3
	movs	r3, #127
	strh	r3, [r5, #40]
	movs	r1, #128
	movs	r3, #254
	movs	r2, #0
	lsls	r1, r1, #9
	lsls	r3, r3, #6
	str	r2, [r5, #24]
	str	r2, [r5, #28]
	str	r2, [r5, #32]
	str	r2, [r5, #36]
	str	r1, [r5, #16]
	str	r1, [r5, #20]
	strh	r3, [r5, #42]
	adds	r3, r7, #0
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r6, [pc, #244]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68ab
	ldr	r1, [r5, #20]
	adds	r0, r0, r3
	str	r0, [r5, #0]
	adds	r3, r7, #0
	adds	r3, #232
	ldr	r0, [r3, #0]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68eb
	adds	r0, r0, r3
	str	r0, [r5, #4]
	bl 0x0200cdc4
	movs	r0, #1
	bl 0x0200cd24
	ldr	r1, [pc, #208]
	ldr	r3, [pc, #212]
	ldr	r0, [pc, #212]
	movs	r2, #24
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4833
	bl 0x0200cde4
	b.n	.L_020044c6
.L_0200441c:
	ldr	r3, [pc, #200]
	cmp	r2, r3
	bne.n	.L_02004474
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #1
	cmp	r3, #9
	bhi.n	.L_020044c6
	ldr	r2, [pc, #184]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200c4b0
	.4byte 0x0200c4b0
	.4byte 0x0200c46c
	.4byte 0x0200c46c
	.4byte 0x0200c4b0
	.4byte 0x0200c4b8
	.4byte 0x0200c4b8
	.4byte 0x0200c4b0
	.4byte 0x0200c464
	.4byte 0x0200c464
	.4byte 0xf7ff2066
	.4byte 0xe02cff05
	.4byte 0xf7ff2067
	.2byte 0xff01
	.2byte 0xe028
.L_02004474:
	ldr	r3, [pc, #120]
	cmp	r2, r3
	bne.n	.L_020044c6
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #1
	cmp	r3, #6
	bhi.n	.L_020044c6
	ldr	r2, [pc, #104]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200c4b0
	.4byte 0x0200c4b8
	.4byte 0x0200c4b0
	.4byte 0x0200c4b8
	.4byte 0x0200c4b0
	.4byte 0x0200c4c0
	.4byte 0x0200c4b8
	.4byte 0xf7ff2064
	.4byte 0xe006fedf
	.4byte 0xf7ff2065
	.4byte 0xe002fedb
	.4byte 0xf7ff2066
	.2byte 0xfed7
.L_020044c6:
	movs	r0, #0
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.4byte 0x02008aad
	.4byte 0x02008a61
	.4byte 0x0200264c
	.4byte 0x0300021c
	.4byte 0x0200d3e4
	.4byte 0x03000730
	.4byte 0x0202de00
	.4byte 0x000000b2
	.4byte 0x0200c43c
	.4byte 0x000000b5
	.2byte 0xc494
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #712]
	adds	r5, r0, #0
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #708]
	sub	sp, #8
	str	r0, [sp, #4]
	movs	r0, #10
	adds	r1, #4
	adds	r0, #255
	mov	fp, r1
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02004524
	b.n	.L_02004846
.L_02004524:
	movs	r0, #252
	lsls	r0, r0, #2
	bl 0x0200cd6c
	cmp	r0, #0
	beq.n	.L_02004532
	b.n	.L_02004846
.L_02004532:
	str	r5, [r6, #0]
.L_02004534:
	movs	r0, #196
	lsls	r0, r0, #4
	bl 0x0200cd54
	movs	r1, #196
	adds	r5, r0, #0
	lsls	r1, r1, #4
	ldr	r3, [pc, #660]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x220e
	adds	r4, r5, #0
	mov	sl, r2
	mov	r8, r2
	movs	r6, #0
	adds	r4, #112
	movs	r7, #28
	adds	r0, r5, #0
	adds	r1, r5, #0
.L_0200455a:
	movs	r3, #196
	lsls	r3, r3, #4
	movs	r2, #99
	adds	r3, r3, r6
	subs	r7, #1
	strb	r2, [r0, #0]
	adds	r6, #4
	strb	r2, [r4, #0]
	adds	r0, #112
	strb	r2, [r1, #0]
	adds	r4, #112
	adds	r1, #4
	strb	r2, [r5, r3]
	cmp	r7, #0
	bge.n	.L_0200455a
	movs	r7, #1
.L_0200457a:
	movs	r0, #0
	mov	r9, r0
.L_0200457e:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #16
	bgt.n	.L_02004534
	mov	r1, r8
	str	r1, [sp, #0]
	bl 0x0200cd34
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	mov	r6, sl
	ldr	r1, [sp, #0]
	cmp	r0, #0
	bne.n	.L_020045a4
	cmp	r7, #1
	beq.n	.L_0200457e
	cmp	r7, #13
	beq.n	.L_0200457e
.L_020045a4:
	mov	r2, r8
	lsls	r3, r2, #3
	cmp	r0, #1
	beq.n	.L_020045f8
	cmp	r0, #1
	bgt.n	.L_020045b6
	cmp	r0, #0
	beq.n	.L_020045c0
	b.n	.L_020046a4
.L_020045b6:
	cmp	r0, #2
	beq.n	.L_0200462e
	cmp	r0, #3
	beq.n	.L_02004668
	b.n	.L_020046a4
.L_020045c0:
	mov	r0, r8
	subs	r3, r3, r0
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r2, r3, #2
	adds	r3, r2, #0
	adds	r3, #112
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	adds	r3, r2, #0
	adds	r3, #224
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	adds	r3, r2, #0
	adds	r3, #108
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	adds	r3, r2, #0
	adds	r3, #116
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	mov	r1, r8
	adds	r1, #1
	b.n	.L_020046a4
.L_020045f8:
	mov	r2, r8
	subs	r3, r3, r2
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r2, r3, #2
	subs	r3, r2, #4
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	adds	r3, r2, #0
	subs	r3, #8
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	adds	r3, r2, #0
	subs	r3, #116
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	adds	r3, r2, #0
	adds	r3, #108
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	mov	r6, sl
	subs	r6, #1
	b.n	.L_020046a4
.L_0200462e:
	mov	r0, r8
	subs	r3, r3, r0
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r2, r3, #2
	adds	r3, r2, #4
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	adds	r3, r2, #0
	adds	r3, #8
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0200457e
	adds	r3, r2, #0
	subs	r3, #108
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_02004656
	b.n	.L_0200457e
.L_02004656:
	adds	r3, r2, #0
	adds	r3, #116
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_02004662
	b.n	.L_0200457e
.L_02004662:
	mov	r6, sl
	adds	r6, #1
	b.n	.L_020046a4
.L_02004668:
	mov	r1, r8
	subs	r3, r3, r1
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r2, r3, #2
	adds	r3, r2, #0
	subs	r3, #112
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_0200467e
	b.n	.L_0200457e
.L_0200467e:
	adds	r3, r2, #0
	subs	r3, #224
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_0200468a
	b.n	.L_0200457e
.L_0200468a:
	adds	r3, r2, #0
	subs	r3, #116
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_02004696
	b.n	.L_0200457e
.L_02004696:
	adds	r3, r2, #0
	subs	r3, #108
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_020046a2
	b.n	.L_0200457e
.L_020046a2:
	subs	r1, #1
.L_020046a4:
	mov	r2, r8
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r3, r3, #2
	adds	r3, r3, r5
	strb	r7, [r3, #0]
	adds	r7, #1
	strb	r6, [r3, #1]
	strb	r1, [r3, #2]
	mov	sl, r6
	mov	r8, r1
	cmp	r7, #14
	bgt.n	.L_020046c4
	b.n	.L_0200457a
.L_020046c4:
	movs	r3, #14
	mov	sl, r3
	mov	r8, r3
	movs	r7, #1
.L_020046cc:
	mov	r0, r8
	lsls	r0, r0, #3
	mov	r1, r8
	subs	r3, r0, r1
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r3, r3, #2
	adds	r2, r3, r5
	adds	r3, #112
	ldrsb	r1, [r5, r3]
	adds	r4, r7, #1
	mov	ip, r2
	mov	lr, r0
	adds	r2, r7, #0
	cmp	r1, r4
	bne.n	.L_020046f0
	adds	r2, r4, #0
	b.n	.L_0200470c
.L_020046f0:
	subs	r3, r7, #1
	mov	r0, ip
	cmp	r1, r3
	bne.n	.L_020046fc
	adds	r2, r1, #0
	b.n	.L_0200470c
.L_020046fc:
	subs	r0, #112
	movs	r1, #0
	ldrsb	r1, [r0, r1]
	subs	r3, r2, #1
	cmp	r1, r3
	bne.n	.L_0200470c
	adds	r2, r1, #0
	b.n	.L_020046fc
.L_0200470c:
	cmp	r2, #0
	bne.n	.L_02004712
	movs	r2, #1
.L_02004712:
	lsls	r3, r7, #3
	mov	r0, lr
	mov	r1, r8
	mov	r9, r3
	add	r3, fp
	strb	r2, [r3, #2]
	subs	r3, r0, r1
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r0, r3, #2
	adds	r3, r0, #0
	subs	r3, #112
	ldrsb	r1, [r5, r3]
	adds	r2, r7, #0
	cmp	r1, r4
	bne.n	.L_02004736
	adds	r2, r4, #0
	b.n	.L_02004752
.L_02004736:
	subs	r3, r7, #1
	adds	r0, r0, r5
	cmp	r1, r3
	bne.n	.L_02004742
	adds	r2, r1, #0
	b.n	.L_02004752
.L_02004742:
	adds	r0, #112
	movs	r1, #0
	ldrsb	r1, [r0, r1]
	subs	r3, r2, #1
	cmp	r1, r3
	bne.n	.L_02004752
	adds	r2, r1, #0
	b.n	.L_02004742
.L_02004752:
	cmp	r2, #0
	bne.n	.L_02004758
	movs	r2, #1
.L_02004758:
	mov	r3, r9
	add	r3, fp
	mov	r1, r8
	mov	r0, lr
	strb	r2, [r3, #5]
	subs	r3, r0, r1
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r3, r3, #2
	subs	r3, #4
	ldrsb	r1, [r5, r3]
	adds	r2, r7, #0
	cmp	r1, r4
	bne.n	.L_02004778
	adds	r2, r4, #0
	b.n	.L_0200479e
.L_02004778:
	subs	r3, r7, #1
	mov	r6, sl
	cmp	r1, r3
	bne.n	.L_02004784
	adds	r2, r1, #0
	b.n	.L_0200479e
.L_02004784:
	mov	r1, r8
	mov	r0, lr
	subs	r3, r0, r1
	lsls	r3, r3, #2
	adds	r6, #1
	adds	r3, r3, r6
	lsls	r3, r3, #2
	ldrsb	r1, [r5, r3]
	subs	r3, r2, #1
	cmp	r1, r3
	bne.n	.L_0200479e
	adds	r2, r1, #0
	b.n	.L_02004784
.L_0200479e:
	cmp	r2, #0
	bne.n	.L_020047a4
	movs	r2, #1
.L_020047a4:
	mov	r3, r9
	add	r3, fp
	strb	r2, [r3, #3]
	mov	r3, r8
	lsls	r0, r3, #3
	subs	r3, r0, r3
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r3, r3, #2
	adds	r3, #4
	ldrsb	r1, [r5, r3]
	adds	r2, r7, #0
	cmp	r1, r4
	bne.n	.L_020047c4
	adds	r2, r4, #0
	b.n	.L_020047f4
.L_020047c4:
	subs	r3, r7, #1
	mov	r6, sl
	cmp	r1, r3
	bne.n	.L_020047dc
	adds	r2, r1, #0
	b.n	.L_020047f4
	.4byte 0x030011bc
	.4byte 0x0200264c
	.2byte 0x0258
	.2byte 0x0300
.L_020047dc:
	mov	r1, r8
	subs	r3, r0, r1
	lsls	r3, r3, #2
	subs	r6, #1
	adds	r3, r3, r6
	lsls	r3, r3, #2
	ldrsb	r1, [r5, r3]
	subs	r3, r2, #1
	cmp	r1, r3
	bne.n	.L_020047f4
	adds	r2, r1, #0
	b.n	.L_020047dc
.L_020047f4:
	cmp	r2, #0
	bne.n	.L_020047fa
	movs	r2, #1
.L_020047fa:
	mov	r3, r9
	add	r3, fp
	strb	r2, [r3, #4]
	strb	r7, [r3, #0]
	mov	r2, ip
	mov	r3, ip
	ldrb	r2, [r2, #1]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r7, r4, #0
	mov	sl, r2
	mov	r8, r3
	cmp	r7, #14
	bgt.n	.L_0200481e
	b.n	.L_020046cc
.L_0200481e:
	mov	r2, fp
	adds	r2, #117
	ldr	r1, [pc, #48]
	movs	r3, #251
	strb	r3, [r2, #0]
	mov	r0, fp
	movs	r3, #250
	strb	r3, [r0, #10]
	movs	r3, #1
	str	r3, [r1, #0]
	adds	r0, r5, #0
	bl 0x0200cd5c
	movs	r0, #252
	lsls	r0, r0, #2
	bl 0x0200cd74
	ldr	r3, [pc, #20]
	ldr	r2, [sp, #4]
	str	r2, [r3, #0]
.L_02004846:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200264c
	.2byte 0x11bc
	.2byte 0x0300
	push	{r5, lr}
	ldr	r4, [pc, #60]
	adds	r5, r0, #0
	ldr	r3, [r4, #0]
	subs	r2, r5, #1
	lsls	r3, r3, #3
	movs	r1, #3
	adds	r3, r3, r4
	ands	r2, r1
	adds	r3, r3, r2
	ldrb	r0, [r3, #6]
	cmp	r0, #0
	beq.n	.L_02004888
	cmp	r0, #0
	blt.n	.L_0200488a
	cmp	r0, #251
	bgt.n	.L_0200488a
	cmp	r0, #250
	blt.n	.L_0200488a
	bl 0x0200cf6c
	b.n	.L_02004892
.L_02004888:
	movs	r0, #1
.L_0200488a:
	str	r0, [r4, #0]
	adds	r0, r5, #0
	bl 0x0200cf6c
.L_02004892:
	movs	r0, #123
	bl 0x0200d014
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x264c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #84]
	sub	sp, #8
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #80]
	lsls	r2, r1, #1
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	cmp	r1, #1
	ble.n	.L_020048f4
	movs	r1, #0
	ldrsb	r1, [r2, r1]
	movs	r7, #1
	ldrsb	r7, [r2, r7]
	movs	r2, #132
	lsls	r2, r2, #1
	mov	r8, r1
	adds	r5, r3, r2
	movs	r6, #1
.L_020048ce:
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	mov	r1, r8
	adds	r0, r1, r2
	adds	r1, r7, r3
	adds	r2, #1
	adds	r3, #1
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #15
	movs	r3, #10
	subs	r6, #1
	bl 0x0200ce0c
	adds	r5, #56
	cmp	r6, #0
	bge.n	.L_020048ce
.L_020048f4:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200264c
	.2byte 0xd3fc
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #80]
	ldr	r1, [r2, #0]
	lsls	r3, r1, #3
	adds	r3, r3, r2
	adds	r3, #4
	ldrb	r2, [r3, #2]
	adds	r1, #1
	cmp	r2, #251
	beq.n	.L_0200491c
	cmp	r2, r1
	bne.n	.L_02004922
.L_0200491c:
	movs	r0, #128
	lsls	r0, r0, #7
	b.n	.L_02004954
.L_02004922:
	ldrb	r2, [r3, #3]
	cmp	r2, #251
	beq.n	.L_0200492c
	cmp	r2, r1
	bne.n	.L_02004932
.L_0200492c:
	movs	r0, #128
	lsls	r0, r0, #8
	b.n	.L_02004954
.L_02004932:
	ldrb	r2, [r3, #4]
	cmp	r2, #251
	beq.n	.L_0200493c
	cmp	r2, r1
	bne.n	.L_02004940
.L_0200493c:
	movs	r0, #0
	b.n	.L_02004954
.L_02004940:
	ldrb	r2, [r3, #5]
	cmp	r2, #251
	beq.n	.L_0200494a
	cmp	r2, r1
	bne.n	.L_02004950
.L_0200494a:
	movs	r0, #192
	lsls	r0, r0, #8
	b.n	.L_02004954
.L_02004950:
	movs	r0, #1
	negs	r0, r0
.L_02004954:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x264c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	ip, r1
	ldr	r1, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	movs	r0, #132
	adds	r3, r1, r3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r3, #48]
	adds	r2, r1, #0
	adds	r3, r1, #0
	adds	r2, #236
	adds	r3, #244
	ldr	r0, [r2, #0]
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
	asrs	r7, r3, #20
	adds	r3, r1, #0
	adds	r3, #248
	adds	r1, #240
	ldr	r2, [r3, #0]
	ldr	r3, [r1, #0]
	asrs	r0, r0, #20
	subs	r2, r2, r3
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r4, r4, r3
	asrs	r5, r2, #20
	movs	r1, #0
	ldr	r6, [r4, #0]
	cmp	r1, r5
	bge.n	.L_020049e0
.L_020049a8:
	lsls	r2, r1, #16
	lsrs	r3, r2, #7
	movs	r0, #0
	adds	r1, r4, r3
	cmp	r0, r7
	bge.n	.L_020049d2
.L_020049b4:
	ldrb	r3, [r1, #2]
	cmp	r3, #0
	beq.n	.L_020049c0
	cmp	r3, ip
	beq.n	.L_020049c0
	str	r6, [r1, #0]
.L_020049c0:
	lsls	r3, r0, #16
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r3, r3, r0
	asrs	r0, r3, #16
	lsrs	r3, r3, #16
	adds	r1, #4
	cmp	r3, r7
	blt.n	.L_020049b4
.L_020049d2:
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r2, r1
	asrs	r1, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, r5
	blt.n	.L_020049a8
.L_020049e0:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r2, [pc, #104]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_02004a4e
	movs	r3, #192
	movs	r1, #133
	lsls	r3, r3, #18
	lsls	r1, r1, #2
	ldr	r6, [r3, #108]
	ldr	r5, [r3, #32]
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	ldr	r3, [r0, #8]
	cmp	r3, #0
	bge.n	.L_02004a14
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
.L_02004a14:
	ldr	r2, [r0, #16]
	asrs	r1, r3, #20
	ldr	r3, [r0, #12]
	subs	r0, r2, r3
	movs	r2, #254
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r3, r0, r2
	cmp	r3, #0
	bge.n	.L_02004a2c
	ldr	r2, [pc, #44]
	adds	r3, r0, r2
.L_02004a2c:
	movs	r0, #212
	lsls	r0, r0, #1
	asrs	r3, r3, #20
	adds	r2, r5, r0
	ldr	r2, [r2, #0]
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r2, [r2, #2]
	subs	r3, r2, #1
	cmp	r3, #229
	bhi.n	.L_02004a4e
	movs	r1, #170
	lsls	r1, r1, #1
	adds	r3, r6, r1
	strh	r2, [r3, #0]
.L_02004a4e:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x000fffff
	.2byte 0x7ffe
	.2byte 0x0010
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	mov	r8, r2
	adds	r6, r3, #0
	bl 0x0200ce74
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #212
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r4, r0, #0
	ldr	r0, [r3, #0]
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_02004a86
	ldr	r2, [pc, #96]
	adds	r3, r5, r2
.L_02004a86:
	asrs	r7, r3, #20
	mov	r3, r8
	subs	r2, r6, r3
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	adds	r1, r2, r3
	cmp	r1, #0
	bge.n	.L_02004a9c
	ldr	r3, [pc, #76]
	adds	r1, r2, r3
.L_02004a9c:
	asrs	r1, r1, #20
	lsls	r3, r1, #7
	adds	r3, r7, r3
	lsls	r3, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	strb	r3, [r0, #2]
	ldr	r0, [pc, #64]
	movs	r3, #128
	ands	r5, r0
	ands	r6, r0
	lsls	r3, r3, #12
	adds	r2, r5, r3
	lsls	r1, r1, #20
	adds	r3, r6, r3
	str	r2, [r4, #8]
	str	r3, [r4, #16]
	adds	r2, r4, #0
	subs	r3, r3, r1
	str	r3, [r4, #12]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r4, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r4, #0
	movs	r1, #0
	bl 0x0200ce14
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x000fffff
	.4byte 0x00107ffe
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	ldr	r3, [pc, #492]
	str	r1, [sp, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200ce74
	adds	r5, r0, #0
	ldr	r3, [r5, #72]
	str	r3, [sp, #8]
	bl 0x0200ce5c
	movs	r0, #0
	bl 0x0200cfcc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200cf4c
	adds	r0, r6, #0
	bl 0x0200ce74
	ldr	r2, [r5, #12]
	adds	r7, r0, #0
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	bl 0x0200ca5c
	ldr	r1, [pc, #424]
	adds	r0, r7, #0
	bl 0x0200cdac
	movs	r2, #15
	mov	fp, r2
.L_02004b54:
	ldr	r2, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	movs	r0, #255
	bl 0x0200cdb4
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02004bcc
	ldr	r1, [pc, #392]
	bl 0x0200cdac
	bl 0x0200cd34
	mov	sl, r0
	bl 0x0200cd34
	adds	r6, r0, #0
	bl 0x0200cd34
	movs	r3, #128
	lsls	r3, r3, #6
	lsrs	r2, r0, #1
	adds	r2, r2, r3
	str	r0, [sp, #4]
	mov	r0, sl
	mov	r9, r2
	bl 0x0200cd44
	ldr	r2, [pc, #356]
	lsls	r6, r6, #3
	adds	r1, r0, #0
	mov	r8, r2
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x62b8
	mov	r0, sl
	bl 0x0200cd3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r7, #52]
	mov	r3, r9
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r0, [r7, #36]
	str	r3, [r7, #68]
.L_02004bcc:
	movs	r2, #1
	negs	r2, r2
	add	fp, r2
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_02004b54
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #78
	bl 0x0200d014
	movs	r0, #217
	bl 0x0200d014
	movs	r2, #230
	movs	r0, #128
	movs	r1, #128
	lsls	r2, r2, #8
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	adds	r2, #102
	bl 0x0200ce24
	ldr	r3, [pc, #244]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #2
	ldr	r0, [r3, #0]
	adds	r1, #255
	bl 0x0200cf34
	movs	r1, #40
	adds	r0, r5, #0
	bl 0x0200cd9c
	movs	r0, #40
	bl 0x0200ce54
	movs	r0, #204
	bl 0x0200d014
	adds	r2, r5, #0
	movs	r3, #3
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	movs	r6, #0
	str	r3, [r5, #72]
	b.n	.L_02004c36
.L_02004c34:
	adds	r6, #1
.L_02004c36:
	cmp	r6, #179
	bgt.n	.L_02004c48
	movs	r0, #1
	bl 0x0200cd24
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bgt.n	.L_02004c34
.L_02004c48:
	movs	r0, #188
	bl 0x0200d014
	ldr	r3, [sp, #8]
	ldr	r6, [pc, #156]
	str	r3, [r5, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200cf34
	adds	r0, r5, #0
	movs	r1, #38
	bl 0x0200cd9c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200ce24
	movs	r0, #10
	bl 0x0200cd24
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200ce24
	movs	r0, #20
	bl 0x0200cd24
	ldr	r2, [r5, #16]
	movs	r3, #1
	ldr	r1, [r5, #12]
	ldr	r0, [r5, #8]
	bl 0x0200cf4c
	bl 0x0200cf54
	movs	r0, #20
	bl 0x0200ce54
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ce14
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200cd9c
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r0, #20
	bl 0x0200cd24
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	bl 0x0200ce64
	ldr	r0, [sp, #12]
	bl 0x0200cd74
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200e2d4
	.4byte 0x0200e290
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl 0x0200cd2c
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200c9e5
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
	.4byte 0x00000000
	.4byte 0x000000e6
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000019
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfffc0000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00040000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00040000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x001e002a
	.4byte 0x0014002c
	.4byte 0x0015002e
	.4byte 0x0017002f
	.4byte 0x0018002f
	.4byte 0x001a002e
	.4byte 0x001b002c
	.4byte 0x001b002b
	.4byte 0x001a0029
	.4byte 0x00180028
	.4byte 0x00170028
	.4byte 0x00150029
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
	.4byte 0x0200936d
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200942d
	.4byte 0x0000002e
	.4byte 0x020094d1
	.4byte 0x0000002e
	.4byte 0x0200951d
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
	.4byte 0x002f0071
	.4byte 0x002f0073
	.4byte 0x002d0073
	.4byte 0x002d0075
	.4byte 0x002d0077
	.4byte 0x002f0077
	.4byte 0x002f0079
	.4byte 0x00310075
	.4byte 0x00310077
	.4byte 0x00330077
	.4byte 0x00330079
	.4byte 0x0033002d
	.4byte 0x0035001f
	.4byte 0x0039001f
	.4byte 0x003b0027
	.4byte 0x003b0029
	.4byte 0x003f002b
	.4byte 0x003f002d
	.4byte 0x00450023
	.4byte 0x00450025
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
	.4byte 0x00000011
	.4byte 0x10043f42
	.4byte 0x1006001e
	.4byte 0x1008001e
	.4byte 0x1006001e
	.4byte 0xfe00001e
	.4byte 0x0000ffff
	.4byte 0x01010000
	.4byte 0x01210111
	.4byte 0x0c310131
	.4byte 0x22311731
	.4byte 0x2d212d31
	.4byte 0x2d012d11
	.4byte 0x17012201
	.4byte 0x00000c01
	.4byte 0x0200d01c
	.4byte 0x0200d058
	.4byte 0x0200d094
	.4byte 0xffff0000
	.4byte 0x00000018
	.4byte 0x00000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe202bc
	.4byte 0x02d0014a
	.4byte 0x015efff6
	.4byte 0x0008ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000b1
	.4byte 0x001040b1
	.4byte 0x002030b1
	.4byte 0x003020b1
	.4byte 0x004010b1
	.4byte 0x009040b2
	.4byte 0x015040b5
	.4byte 0x016030b3
	.4byte 0x017030b3
	.4byte 0x018030b3
	.4byte 0x0fa040b2
	.4byte 0x0fb090b1
	.4byte 0x000000b2
	.4byte 0x001040ae
	.4byte 0x002030b2
	.4byte 0x003020b2
	.4byte 0x004010b1
	.4byte 0x005060b2
	.4byte 0x006050b2
	.4byte 0x007010b4
	.4byte 0x008090b2
	.4byte 0x009080b2
	.4byte 0x00a020b4
	.4byte 0x000000b3
	.4byte 0x001030b4
	.4byte 0x002070b5
	.4byte 0x003060b5
	.4byte 0x014140b1
	.4byte 0x000000b4
	.4byte 0x0010a0b2
	.4byte 0x002070b2
	.4byte 0x003010b3
	.4byte 0x000000b5
	.4byte 0x001020b5
	.4byte 0x002010b5
	.4byte 0x003040b5
	.4byte 0x004030b5
	.4byte 0x005060b5
	.4byte 0x006050b5
	.4byte 0x007020b3
	.4byte 0x008030b3
	.4byte 0x009060b3
	.4byte 0x014140b1
	.4byte 0x000001ff
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000002
	.4byte 0x00003000
	.4byte 0x00000002
	.4byte 0x01c80000
	.4byte 0xffe00000
	.4byte 0x01c80000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0xffe00000
	.4byte 0x0000002e
	.4byte 0x020085dd
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004000
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0xffe00000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x020085dd
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00040000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x0000002e
	.4byte 0x020085f9
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x0000002a
	.4byte 0x000000c5
	.4byte 0x0000002e
	.4byte 0x02008679
	.4byte 0x00000000
	.4byte 0x000000f0
	.4byte 0x80020000
	.4byte 0x0000002e
	.4byte 0x02008611
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0xc0020000
	.4byte 0x00000005
	.4byte 0xfffc0000
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087f5
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000004
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087f5
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x02008901
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x0000002e
	.4byte 0x020086e1
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000009
	.4byte 0x0000002e
	.4byte 0x020086f9
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000005
	.4byte 0xfff00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x0000002e
	.4byte 0x02008719
	.4byte 0x00000027
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x02008749
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000104
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000002e
	.4byte 0x020088b5
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffff018a
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0143
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
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0xffff0184
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00024000
	.4byte 0xffff0184
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00024000
	.4byte 0xffff0111
	.4byte 0x0200da30
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0111
	.4byte 0x0200da30
	.4byte 0x03b80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0181
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff018d
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
	.4byte 0xffff0184
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00024000
	.4byte 0xffff0184
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00024000
	.4byte 0xffff0181
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00024000
	.4byte 0xffff0181
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00024000
	.4byte 0xffff018d
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
	.4byte 0xffff0140
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0140
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0140
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0140
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0140
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0140
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0140
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0140
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0140
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
	.4byte 0xffff0184
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00024000
	.4byte 0xffff0181
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff017a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff018d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0181
	.4byte 0x0200d528
	.4byte 0x02c00000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00024000
	.4byte 0xffff015b
	.4byte 0x0200d528
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
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00001800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x80030000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000002
	.4byte 0xc0030000
	.4byte 0x80040000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000002
	.4byte 0xc0040000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x80030000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000002
	.4byte 0xc0030000
	.4byte 0x80040000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000002
	.4byte 0xc0040000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200c85d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200c85d
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200c85d
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200c85d
	.4byte 0x00000c15
	.4byte 0x02000008
	.4byte 0x02008af9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0xffff002a
	.4byte 0x02008edd
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000002
	.4byte 0x09de0028
	.4byte 0x02009049
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020091c9
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x020092ad
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020099cd
	.4byte 0x00000053
	.4byte 0xffff0032
	.4byte 0x00402301
	.4byte 0x00000003
	.4byte 0xffff0037
	.4byte 0x02009ebd
	.4byte 0x00000003
	.4byte 0xffff0038
	.4byte 0x02009ebd
	.4byte 0x00000053
	.4byte 0x0e41004d
	.4byte 0x02009fa1
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02009f69
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02009f85
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000002
	.4byte 0xffff002a
	.4byte 0x02009d15
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020099cd
	.4byte 0x00000053
	.4byte 0xffff0032
	.4byte 0x00402301
	.4byte 0x00000003
	.4byte 0xffff0037
	.4byte 0x00402313
	.4byte 0x00000053
	.4byte 0x0e41004d
	.4byte 0x02009fa1
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02009f69
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x02009f85
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
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
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02009fd9
	.4byte 0x0001c314
	.4byte 0xffff040c
	.4byte 0x0200a021
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x0200a975
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x0200a975
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200a975
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02009fd9
	.4byte 0x0001c314
	.4byte 0xffff040a
	.4byte 0x0200a25d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02009fd9
	.4byte 0x0001c314
	.4byte 0xffff040b
	.4byte 0x0200a4b9
	.4byte 0x50008905
	.4byte 0xffff0014
	.4byte 0x02008b6d
	.4byte 0x50008905
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x50008905
	.4byte 0xffff0014
	.4byte 0x02008c39
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ce01
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000ce01
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000ce01
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00004602
	.4byte 0xffff002b
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0x0303001e
	.4byte 0x0200aa15
	.4byte 0x00000002
	.4byte 0x0304001f
	.4byte 0x0200aa25
	.4byte 0x00000002
	.4byte 0x03050020
	.4byte 0x0200aa35
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02009fd9
	.4byte 0x0001c314
	.4byte 0xffff0409
	.4byte 0x0200a719
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x50008905
	.4byte 0xffff0028
	.4byte 0x02008cd9
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte 0x02008cf9
	.4byte 0x50008905
	.4byte 0xffff002a
	.4byte 0x02008d19
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte 0x02008d19
	.4byte 0x50008905
	.4byte 0xffff002c
	.4byte 0x02008c75
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000011
