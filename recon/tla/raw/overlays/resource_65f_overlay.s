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
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x0200d1c4
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000100
	cmp	r7, #0
	beq.n	.L_02000100
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000108
.L_02000100:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000108:
	mov	r3, sl
	bl 0x0200d174
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000116
	b.n	.L_02000262
.L_02000116:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200d164
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200d16c
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d19c
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
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
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000262
	cmp	r7, #0
	beq.n	.L_02000262
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000198
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200d244
.L_02000198:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
.L_020001a0:
	cmp	r3, #0
	beq.n	.L_020001b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02008038
.L_020001b8:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020001cc
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020001cc:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000212
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020001fa
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200d11c
.L_020001ee:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_0200020c
.L_020001fa:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200d11c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x0200d11c
	str	r0, [r6, #52]
.L_02000212:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200022e
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200d164
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200d16c
.L_0200022e:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000240
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02000240:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000252
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000252:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000262
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000262:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200d520
	.4byte 0x02008081
	.4byte 0xffff0000
	.4byte 0x049b23c0
	.4byte 0x20a06a1b
	.4byte 0x18190040
	.4byte 0x181a3838
	.4byte 0x610b6913
	.4byte 0x614b6953
	.4byte 0x850b8d13
	.4byte 0x854b8d53
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #204]
	movs	r2, #4
	negs	r2, r2
	ands	r3, r2
	ldr	r3, [r3, #4]
	movs	r2, #192
	lsls	r2, r2, #18
	mov	fp, r3
	ldr	r3, [r2, #32]
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r1, r3, r0
	ldr	r3, [r2, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r5, [pc, #168]
	cmp	r3, #0
	bne.n	.L_02000374
	str	r3, [r1, #28]
	ldr	r3, [pc, #164]
	movs	r6, #0
	ldrb	r2, [r3, #0]
	movs	r3, #6
	ldrsh	r0, [r1, r3]
	movs	r3, #2
	ldrsh	r4, [r1, r3]
	movs	r3, #204
	lsls	r3, r3, #6
	muls	r3, r0
	lsls	r2, r2, #10
	movs	r1, #153
	adds	r3, r3, r2
	lsls	r1, r1, #5
	mov	sl, r3
	adds	r3, r0, #0
	muls	r3, r1
	mov	r9, r4
	subs	r7, r3, r2
	cmp	r6, #160
	beq.n	.L_02000340
.L_0200030c:
	mov	r0, sl
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4680
	adds	r0, r7, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4644
	lsls	r2, r0, #3
	lsls	r3, r4, #1
	subs	r2, r2, r0
	add	r3, r8
	adds	r3, r3, r2
	asrs	r3, r3, #19
	movs	r0, #204
	movs	r2, #153
	add	r3, r9
	lsls	r0, r0, #6
	lsls	r2, r2, #5
	adds	r6, #1
	strh	r3, [r5, #0]
	add	sl, r0
	adds	r5, #2
	adds	r7, r7, r2
	cmp	r6, #160
	bne.n	.L_0200030c
.L_02000340:
	ldr	r5, [pc, #64]
	movs	r1, #128
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #19
	adds	r1, #24
	strh	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r0, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r0
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r0, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r0
	strh	r2, [r3, #10]
	adds	r0, r5, #2
	ldrh	r2, [r3, #10]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_02000374:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d13d
	.4byte 0x0200e340
	.4byte 0x0300122c
	.2byte 0x0001
	.2byte 0xa260
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r3, [r3, #0]
	ldr	r7, [pc, #56]
	adds	r0, r3, #0
	movs	r5, #0
	adds	r0, #48
	movs	r4, #4
	movs	r6, #0
.L_020003a6:
	movs	r3, #160
	lsls	r3, r3, #19
	lsls	r1, r5, #1
	adds	r3, #48
	adds	r1, r1, r3
	subs	r3, r4, #2
	ldrh	r2, [r7, r4]
	ldrh	r3, [r7, r3]
	lsls	r2, r2, #10
	lsls	r3, r3, #5
	orrs	r2, r3
	ldrh	r3, [r7, r6]
	adds	r5, #1
	orrs	r2, r3
	strh	r2, [r1, #0]
	adds	r4, #6
	ldrh	r3, [r1, #0]
	adds	r6, #6
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r5, #7
	bls.n	.L_020003a6
	pop	{r5, r6, r7, pc}
	.2byte 0xd4ee
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r3, [r3, #0]
	ldr	r7, [pc, #56]
	adds	r0, r3, #0
	movs	r5, #0
	adds	r0, #194
	movs	r4, #4
	movs	r6, #0
.L_020003ee:
	movs	r3, #160
	lsls	r3, r3, #19
	lsls	r1, r5, #1
	adds	r3, #194
	adds	r1, r1, r3
	subs	r3, r4, #2
	ldrh	r2, [r7, r4]
	ldrh	r3, [r7, r3]
	lsls	r2, r2, #10
	lsls	r3, r3, #5
	orrs	r2, r3
	ldrh	r3, [r7, r6]
	adds	r5, #1
	orrs	r2, r3
	strh	r2, [r1, #0]
	adds	r4, #6
	ldrh	r3, [r1, #0]
	adds	r6, #6
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r5, #8
	bls.n	.L_020003ee
	pop	{r5, r6, r7, pc}
	.2byte 0xd4b8
	.2byte 0x0200
	push	{lr}
	movs	r1, #0
	bl 0x0200d19c
	movs	r0, #0
	pop	{pc}
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd574
	.2byte 0x0200
	.global Func_02000434
	.thumb_func
Func_02000434:
	movs	r0, #0
	bx	lr
	.global Func_02000438
	.thumb_func
Func_02000438:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd5a4
	.2byte 0x0200
	.global Func_02000440
	.thumb_func
Func_02000440:
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200046a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000466
	ldr	r0, [pc, #60]
	b.n	.L_02000494
.L_02000466:
	ldr	r0, [pc, #60]
	b.n	.L_02000494
.L_0200046a:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000474
	ldr	r0, [pc, #56]
	b.n	.L_02000494
.L_02000474:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_0200047e
	ldr	r0, [pc, #56]
	b.n	.L_02000494
.L_0200047e:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000488
	ldr	r0, [pc, #52]
	b.n	.L_02000494
.L_02000488:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000492
	ldr	r0, [pc, #52]
	b.n	.L_02000494
.L_02000492:
	ldr	r0, [pc, #52]
.L_02000494:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000003b
	.4byte 0x0200d874
	.4byte 0x0200d6f4
	.4byte 0x0000003c
	.4byte 0x0200db5c
	.4byte 0x0000003d
	.4byte 0x0200d9dc
	.4byte 0x0000003e
	.4byte 0x0200da54
	.4byte 0x0000003f
	.4byte 0x0200dc1c
	.2byte 0xd6dc
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	movs	r5, #8
.L_020004e0:
	adds	r0, r5, #0
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_020004f2
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_020004f2:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_020004e0
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	movs	r0, #158
	bl 0x0200d394
	subs	r6, #1
	ldr	r0, [pc, #108]
	lsls	r4, r6, #3
	adds	r3, r4, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r4]
	bl 0x0200d17c
	ldr	r5, [pc, #92]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200d21c
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200d1cc
	ldr	r0, [r5, #0]
	cmp	r6, #5
	bne.n	.L_0200054c
	movs	r2, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x0200d1fc
	b.n	.L_02000556
.L_0200054c:
	movs	r2, #4
	movs	r1, #2
	negs	r2, r2
	bl 0x0200d1f4
.L_02000556:
	movs	r0, #6
	bl 0x0200d1a4
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200d2c4
	bl 0x0200d2e4
	bl 0x0200d2ec
	bl 0x0200d1b4
	pop	{r5, r6, r7, pc}
	.4byte 0x0200dde0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200d24c
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200d254
	bl 0x0200d384
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	bne.n	.L_020005b0
	movs	r0, #10
	bl 0x0200d1a4
	adds	r0, r5, #1
	bl 0x0200d24c
	b.n	.L_020005bc
.L_020005b0:
	movs	r0, #20
	bl 0x0200d1a4
	adds	r0, r5, #2
	bl 0x0200d24c
.L_020005bc:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d264
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x1974
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200d24c
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200d254
	bl 0x0200d384
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	bne.n	.L_020005fc
	movs	r0, #10
	bl 0x0200d1a4
	adds	r0, r5, #1
	bl 0x0200d24c
	b.n	.L_02000608
.L_020005fc:
	movs	r0, #20
	bl 0x0200d1a4
	adds	r0, r5, #2
	bl 0x0200d24c
.L_02000608:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d264
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x197c
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200d24c
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200d254
	bl 0x0200d384
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	bne.n	.L_02000648
	movs	r0, #10
	bl 0x0200d1a4
	adds	r0, r5, #1
	bl 0x0200d24c
	b.n	.L_02000654
.L_02000648:
	movs	r0, #20
	bl 0x0200d1a4
	adds	r0, r5, #2
	bl 0x0200d24c
.L_02000654:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d264
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x19fd
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	ldr	r0, [pc, #32]
	bl 0x0200d24c
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #10
	bl 0x0200d25c
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #10
	bl 0x0200d25c
	bl 0x0200d1b4
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x19fb
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200d14c
	ldr	r3, [pc, #56]
	cmp	r0, #0
	beq.n	.L_020006bc
	adds	r0, r3, #0
	bl 0x0200d24c
	b.n	.L_020006d6
.L_020006bc:
	adds	r0, r3, #0
	bl 0x0200d24c
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #10
	bl 0x0200d25c
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #10
	bl 0x0200d25c
.L_020006d6:
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #10
	bl 0x0200d25c
	bl 0x0200d1b4
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1978
	.2byte 0x0000
	push	{lr}
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	movs	r0, #13
	bl 0x0200d1dc
	movs	r1, #1
	movs	r0, #13
	bl 0x0200d21c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #13
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d23c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_0200072c
	ldr	r0, [pc, #112]
	bl 0x0200d24c
	b.n	.L_02000774
.L_0200072c:
	ldr	r0, [pc, #108]
	bl 0x0200d24c
	movs	r0, #13
	movs	r1, #0
	movs	r2, #10
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #15
	movs	r0, #4
	bl 0x0200d28c
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #10
	movs	r0, #13
	bl 0x0200d28c
	movs	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200d25c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #15
	movs	r0, #13
	bl 0x0200d28c
	movs	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200d25c
.L_02000774:
	movs	r2, #10
	movs	r1, #0
	movs	r0, #13
	bl 0x0200d25c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200d154
	movs	r0, #13
	movs	r1, #2
	bl 0x0200d1d4
	bl 0x0200d1b4
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001a04
	.2byte 0x1a01
	.2byte 0x0000
	push	{r5, lr}
	mov	r5, r9
	push	{r5}
	sub	sp, #4
	mov	r3, r9
	adds	r5, r0, #0
	movs	r1, #1
	movs	r0, #144
	str	r3, [sp, #0]
	bl 0x0200d304
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200d30c
	movs	r0, #1
	bl 0x0200d2fc
	bl 0x0200d314
	bl 0x0200d31c
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, pc}
	push	{r5, lr}
	mov	r5, r9
	push	{r5}
	sub	sp, #4
	mov	r3, r9
	adds	r5, r0, #0
	movs	r1, #1
	movs	r0, #144
	str	r3, [sp, #0]
	bl 0x0200d304
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200d30c
	movs	r0, #3
	bl 0x0200d2fc
	bl 0x0200d314
	bl 0x0200d31c
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, pc}
	push	{r5, r6, lr}
	mov	r6, r9
	push	{r6}
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x0200d14c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0200081e
	b.n	.L_02000950
.L_0200081e:
	movs	r0, #200
	lsls	r0, r0, #2
	bl 0x0200d154
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	movs	r0, #22
	bl 0x0200d1c4
	movs	r5, #128
	movs	r1, #0
	bl 0x0200d19c
	lsls	r5, r5, #7
	movs	r1, #180
	movs	r2, #199
	movs	r0, #22
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	adds	r3, r5, #0
	bl 0x0200d214
	movs	r1, #1
	movs	r2, #220
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	ldr	r0, [pc, #252]
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #22
	mov	r9, sp
	bl 0x020087a0
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #180
	movs	r2, #199
	adds	r3, r5, #0
	movs	r0, #21
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200d214
	movs	r2, #16
	movs	r1, #0
	movs	r0, #21
	bl 0x0200d344
	movs	r0, #22
	mov	r9, sp
	bl 0x020087d4
	ldr	r0, [pc, #196]
	bl 0x0200d24c
	movs	r1, #160
	movs	r0, #21
	lsls	r1, r1, #7
	bl 0x0200d274
	movs	r1, #2
	movs	r0, #21
	bl 0x0200d234
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #21
	movs	r1, #0
	movs	r2, #10
	bl 0x0200d25c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #21
	bl 0x0200d28c
	movs	r0, #21
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #21
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #22
	mov	r9, sp
	bl 0x020087a0
	movs	r0, #20
	bl 0x0200d1a4
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #21
	bl 0x0200d344
	movs	r0, #21
	bl 0x0200d1c4
	movs	r2, #16
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r1, #0
	movs	r0, #21
	negs	r2, r2
	bl 0x0200d344
	movs	r1, #0
	movs	r2, #0
	movs	r0, #21
	bl 0x0200d20c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #22
	mov	r9, sp
	bl 0x020087d4
	movs	r0, #60
	bl 0x0200d1a4
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	bl 0x0200d1b4
.L_02000950:
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x01670000
	.2byte 0x1984
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r5, #192
	lsls	r5, r5, #18
	movs	r0, #123
	ldr	r6, [r5, #108]
	bl 0x0200d394
	ldr	r3, [r5, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2ac
	movs	r0, #34
	adds	r0, #255
	bl 0x0200d154
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	bl 0x0200d2c4
	pop	{r5, r6, pc}
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_020009c8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #30
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_020009da
	bl 0x0200a694
	b.n	.L_020009da
.L_020009c8:
	movs	r0, #130
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_020009da
	bl 0x0200b878
.L_020009da:
	pop	{pc}
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02000a06
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000a02
	ldr	r0, [pc, #60]
	b.n	.L_02000a30
.L_02000a02:
	ldr	r0, [pc, #60]
	b.n	.L_02000a30
.L_02000a06:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000a10
	ldr	r0, [pc, #56]
	b.n	.L_02000a30
.L_02000a10:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000a1a
	ldr	r0, [pc, #56]
	b.n	.L_02000a30
.L_02000a1a:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000a24
	ldr	r0, [pc, #52]
	b.n	.L_02000a30
.L_02000a24:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000a2e
	ldr	r0, [pc, #52]
	b.n	.L_02000a30
.L_02000a2e:
	ldr	r0, [pc, #52]
.L_02000a30:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000003b
	.4byte 0x0200df78
	.4byte 0x0200de34
	.4byte 0x0000003c
	.4byte 0x0200e0d4
	.4byte 0x0000003d
	.4byte 0x0200e11c
	.4byte 0x0000003e
	.4byte 0x0200e1d0
	.4byte 0x0000003f
	.4byte 0x0200e278
	.2byte 0xde10
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_02000aa2
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000aa2
	ldr	r3, [pc, #28]
	ldr	r2, [r3, #0]
	adds	r2, #1
	str	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000aa2
	movs	r0, #135
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d394
.L_02000aa2:
	pop	{pc}
	.2byte 0xe32c
	.2byte 0x0200
	.global Func_02000aa8
	.thumb_func
Func_02000aa8:
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #280]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r1, r6, r2
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	ldr	r3, [pc, #272]
	cmp	r2, r3
	bne.n	.L_02000ad0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	b.n	.L_02000bc0
.L_02000ad0:
	ldr	r3, [pc, #248]
	cmp	r2, r3
	bne.n	.L_02000b5e
	movs	r0, #13
	movs	r1, #1
	bl 0x0200d284
	movs	r0, #14
	movs	r1, #1
	bl 0x0200d284
	movs	r0, #130
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000b06
	movs	r3, #128
	movs	r1, #164
	movs	r2, #162
	lsls	r3, r3, #7
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200d214
.L_02000b06:
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r7, #129
	adds	r3, r3, r2
	lsls	r7, r7, #2
	str	r7, [r3, #0]
	movs	r0, #0
	bl 0x0200d2f4
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #6
	bne.n	.L_02000bc0
	ldr	r3, [r5, #108]
	movs	r2, #133
	subs	r1, #54
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	movs	r0, #144
	str	r2, [r3, #0]
	lsls	r0, r0, #1
	bl 0x0200d15c
	movs	r0, #34
	adds	r0, #255
	bl 0x0200d15c
	ldr	r3, [r5, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	str	r7, [r3, #0]
	bl 0x0200d2dc
	bl 0x0200d2ec
	b.n	.L_02000bc0
.L_02000b5e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	str	r2, [r3, #0]
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02000b88
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #25
	bl 0x0200d154
	bl 0x02008be4
	b.n	.L_02000bc0
.L_02000b88:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02000b94
	bl 0x02008d30
	b.n	.L_02000bc0
.L_02000b94:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02000bc0
	bl 0x02008e00
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #25
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000bc0
	ldr	r3, [pc, #44]
	movs	r1, #242
	lsls	r1, r1, #1
	adds	r2, r6, r1
	strh	r3, [r2, #0]
	movs	r3, #243
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #2
	strh	r3, [r2, #0]
.L_02000bc0:
	movs	r0, #0
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0000003e
	.4byte 0x0000003f
	.4byte 0x0000003b
	.4byte 0x0000003c
	.4byte 0x0000003d
	.2byte 0x0042
	.2byte 0x0000
	.global Func_02000be0
	.thumb_func
Func_02000be0:
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	movs	r1, #144
	ldr	r0, [pc, #148]
	lsls	r1, r1, #3
	sub	sp, #8
	bl 0x0200d12c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #144
	adds	r3, r3, r2
	lsls	r0, r0, #4
	movs	r2, #0
	str	r2, [r3, #0]
	adds	r0, #27
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000c1c
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000c84
.L_02000c1c:
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d154
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #7
	movs	r1, #1
	bl 0x0200d2cc
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #17
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000c4a
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #2
	bl 0x0200d2cc
.L_02000c4a:
	movs	r0, #1
	bl 0x0200d2d4
	bl 0x0200d2ec
	bl 0x0200d38c
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r1, [r3, #0]
	movs	r2, #160
	ldr	r3, [pc, #24]
	lsls	r2, r2, #19
	adds	r2, #60
	strh	r3, [r2, #0]
	movs	r0, #128
	ldrh	r3, [r2, #0]
	lsls	r0, r0, #9
	strh	r3, [r1, #60]
	movs	r1, #1
	bl 0x0200d2cc
	b.n	.L_02000ce6
	.2byte 0x0000
	.4byte 0x00007fff
	.2byte 0x8a69
	.2byte 0x0200
.L_02000c84:
	movs	r5, #1
	movs	r0, #10
	movs	r1, #32
	movs	r2, #10
	movs	r3, #10
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r0, #12
	movs	r1, #32
	movs	r2, #25
	movs	r3, #22
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r0, #11
	movs	r1, #33
	movs	r2, #8
	movs	r3, #22
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r0, #74
	movs	r1, #32
	movs	r2, #74
	movs	r3, #10
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r0, #76
	movs	r1, #32
	movs	r2, #89
	movs	r3, #22
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r0, #75
	movs	r1, #33
	movs	r2, #72
	movs	r3, #22
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
.L_02000ce6:
	ldr	r3, [pc, #68]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #12
	bne.n	.L_02000d26
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r2, [r5, #108]
	movs	r3, #133
	movs	r6, #214
	lsls	r3, r3, #1
	adds	r3, #255
	lsls	r6, r6, #1
	movs	r0, #144
	str	r3, [r2, r6]
	lsls	r0, r0, #1
	bl 0x0200d15c
	movs	r0, #34
	adds	r0, #255
	bl 0x0200d15c
	bl 0x0200d2dc
	bl 0x0200d2ec
	ldr	r2, [r5, #108]
	movs	r3, #0
	str	r3, [r2, r6]
.L_02000d26:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #184]
	sub	sp, #8
	bl 0x0200d12c
	bl 0x0200d35c
	movs	r0, #0
	movs	r1, #10
	movs	r2, #11
	bl 0x0200d364
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000da6
	ldr	r3, [pc, #152]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #12
	bne.n	.L_02000d72
	ldr	r0, [pc, #140]
	movs	r1, #1
	bl 0x0200d2cc
	b.n	.L_02000d7e
.L_02000d72:
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #7
	movs	r1, #1
	bl 0x0200d2cc
.L_02000d7e:
	movs	r0, #1
	bl 0x0200d2d4
	bl 0x0200d2ec
	bl 0x0200d38c
	bl 0x020083d8
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	bl 0x0200d2cc
	movs	r1, #200
	ldr	r0, [pc, #92]
	lsls	r1, r1, #4
	bl 0x0200d12c
	b.n	.L_02000dea
.L_02000da6:
	movs	r5, #5
	movs	r0, #20
	movs	r1, #7
	movs	r2, #9
	movs	r3, #7
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r0, #20
	movs	r1, #71
	movs	r2, #9
	movs	r3, #71
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r0, #84
	movs	r1, #7
	movs	r2, #73
	movs	r3, #7
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	bl 0x02008390
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	bl 0x0200d2cc
	bl 0x02008280
.L_02000dea:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02008a69
	.4byte 0x02000240
	.4byte 0x002048c9
	.2byte 0x82a5
	.2byte 0x0200
	push	{lr}
	movs	r1, #144
	ldr	r0, [pc, #72]
	lsls	r1, r1, #3
	bl 0x0200d12c
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000e3e
	movs	r0, #198
	lsls	r0, r0, #2
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02000e32
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #1
	bl 0x0200d2cc
	b.n	.L_02000e3e
.L_02000e32:
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #7
	movs	r1, #1
	bl 0x0200d2cc
.L_02000e3e:
	ldr	r0, [pc, #20]
	bl 0x0200d36c
	movs	r0, #1
	bl 0x0200d2d4
	bl 0x0200d2ec
	pop	{pc}
	.4byte 0x02008a69
	.2byte 0xd450
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200d374
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd450
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200d374
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd450
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r7, r0, #0
	movs	r6, #16
.L_02000e88:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #32]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r5, #12]
	subs	r6, #1
	bl 0x0200d1a4
	cmp	r6, #0
	bgt.n	.L_02000e88
	movs	r0, #136
	bl 0x0200d394
	adds	r0, r7, #0
	bl 0x020092a8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb5e0
	movs	r0, #8
	bl 0x0200d1c4
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r0, #129
	asrs	r7, r3, #20
	ldr	r3, [r5, #16]
	lsls	r0, r0, #1
	adds	r0, #255
	asrs	r6, r3, #20
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_02000eea
	cmp	r7, #10
	bne.n	.L_02000eea
	cmp	r6, #29
	bne.n	.L_02000eea
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x02008e78
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d154
.L_02000eea:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	movs	r0, #9
	bl 0x0200d1c4
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r0, #128
	asrs	r7, r3, #20
	ldr	r3, [r5, #16]
	lsls	r0, r0, #2
	adds	r0, #2
	asrs	r6, r3, #20
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_02000f26
	cmp	r7, #15
	bne.n	.L_02000f26
	cmp	r6, #28
	bne.n	.L_02000f26
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x02008e78
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200d154
.L_02000f26:
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r0, #10
	bl 0x0200d1c4
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d14c
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r0, #11
	bl 0x0200d1c4
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r0, #129
	asrs	r7, r3, #20
	ldr	r3, [r5, #16]
	lsls	r0, r0, #2
	asrs	r6, r3, #20
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_02000f72
	cmp	r7, #15
	bne.n	.L_02000f72
	cmp	r6, #25
	bne.n	.L_02000f72
	movs	r0, #11
	adds	r1, r5, #0
	bl 0x02008e78
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200d154
.L_02000f72:
	pop	{r5, r6, r7, pc}
	push	{lr}
	bl 0x0200d37c
	bl 0x02008eb0
	pop	{pc}
	push	{lr}
	bl 0x0200d37c
	bl 0x02008eec
	pop	{pc}
	push	{lr}
	bl 0x0200d37c
	bl 0x02008f28
	pop	{pc}
	push	{lr}
	bl 0x0200d37c
	bl 0x02008f3c
	pop	{pc}
	push	{lr}
	bl 0x0200d37c
	bl 0x02008eb0
	bl 0x02008eec
	bl 0x02008f28
	bl 0x02008f3c
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #98
	movs	r1, #8
	movs	r2, #86
	movs	r3, #8
	bl 0x0200d184
	movs	r3, #22
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #34
	movs	r1, #9
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d18c
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #25
	movs	r1, #47
	movs	r2, #11
	movs	r3, #33
	bl 0x0200d184
	movs	r3, #11
	movs	r2, #33
	str	r3, [sp, #0]
.L_0200100a:
	str	r2, [sp, #4]
	movs	r0, #25
	movs	r1, #47
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d18c
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	sub	sp, #8
	movs	r5, #3
	movs	r0, #44
	movs	r1, #5
	movs	r2, #19
	movs	r3, #5
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #108
	movs	r1, #5
	movs	r2, #83
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200d184
	movs	r3, #19
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #44
	movs	r1, #5
	movs	r2, #3
	movs	r3, #3
	bl 0x0200d18c
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
.L_02001066:
	str	r2, [sp, #4]
.L_02001068:
	movs	r0, #102
	movs	r1, #8
	movs	r2, #86
	movs	r3, #8
	bl 0x0200d184
	movs	r3, #22
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #22
	movs	r1, #10
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d18c
	add	sp, #8
	pop	{pc}
	push	{lr}
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d14c
	pop	{pc}
	push	{lr}
.L_0200109a:
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020010d2
	movs	r0, #198
	lsls	r0, r0, #2
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020010c0
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #1
	bl 0x0200d2cc
	b.n	.L_020010cc
.L_020010c0:
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #7
	movs	r1, #1
	bl 0x0200d2cc
.L_020010cc:
	movs	r0, #16
	bl 0x0200d2d4
.L_020010d2:
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #30
	movs	r2, #11
	movs	r3, #33
	bl 0x0200d184
	movs	r3, #11
	movs	r2, #33
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #30
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d18c
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	sub	sp, #8
	movs	r5, #3
	movs	r0, #39
	movs	r1, #5
.L_0200110e:
	movs	r2, #19
	movs	r3, #5
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d184
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #103
	movs	r1, #5
	movs	r2, #83
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200d184
	movs	r3, #19
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #39
	movs	r1, #5
	movs	r2, #3
	movs	r3, #3
	bl 0x0200d18c
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_020011c2
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x0200d154
	ldr	r3, [pc, #100]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
.L_02001166:
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
.L_0200116a:
	bne.n	.L_0200118a
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #1
	bl 0x0200d2cc
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d154
	movs	r0, #198
	lsls	r0, r0, #2
	bl 0x0200d154
	b.n	.L_020011a4
.L_0200118a:
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	bl 0x0200d2cc
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d15c
	movs	r0, #198
	lsls	r0, r0, #2
	bl 0x0200d15c
.L_020011a4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_020011c2
	movs	r0, #120
	bl 0x0200d2d4
.L_020011c2:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_0200124a
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d154
	ldr	r3, [pc, #104]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_02001210
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #3
	movs	r1, #1
	bl 0x0200d2cc
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d154
	movs	r0, #198
	lsls	r0, r0, #2
	bl 0x0200d154
	b.n	.L_0200122c
.L_02001210:
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #7
	movs	r1, #1
	bl 0x0200d2cc
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x0200d154
	movs	r0, #198
	lsls	r0, r0, #2
	bl 0x0200d15c
.L_0200122c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0200124a
	movs	r0, #120
	bl 0x0200d2d4
.L_0200124a:
	pop	{pc}
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
	bl 0x0200d11c
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001280
	adds	r3, #15
.L_02001280:
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
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #68
	bl 0x0200d1c4
	ldr	r3, [pc, #108]
	add	r2, sp, #16
	str	r3, [r2, #36]
	movs	r3, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r3
.L_020012c6:
	mov	r2, sl
	lsls	r6, r2, #12
	adds	r0, r6, #0
	bl 0x0200d144
	add	r5, sp, #56
	movs	r3, #0
	str	r0, [r5, #0]
	adds	r0, r6, #0
	str	r3, [r5, #4]
	bl 0x0200d13c
	ldr	r6, [r5, #0]
	mov	r8, r0
	str	r0, [r5, #8]
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x0200d11c
	ldr	r3, [r5, #4]
	adds	r6, r6, r0
	str	r6, [r5, #0]
	ldr	r2, [r7, #16]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #17
	adds	r3, #1
	str	r3, [sp, #8]
	mov	r3, r9
	str	r3, [sp, #12]
	adds	r3, r6, #0
	bl 0x020080b8
	movs	r2, #2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020012c6
	add	sp, #68
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9251
	.2byte 0x0200
	push	{lr}
	movs	r1, #10
	movs	r2, #11
	movs	r0, #1
	bl 0x0200d354
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d154
	pop	{pc}
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200d14c
	cmp	r0, #0
	bne.n	.L_0200135e
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #30
	bl 0x0200d14c
.L_0200135e:
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r4, r0, #0
	adds	r6, r2, #0
	adds	r5, r1, #0
	lsls	r3, r3, #16
	movs	r0, #244
	asrs	r7, r3, #16
	lsls	r0, r0, #1
	adds	r3, r6, #0
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl 0x0200d174
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020013a4
	movs	r1, #1
	ldr	r5, [r6, #80]
	bl 0x0200d164
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	bl 0x0200d16c
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [sp, #16]
	ldr	r1, [pc, #12]
	adds	r2, #9
	strh	r3, [r2, #0]
	strb	r1, [r5, #26]
	strh	r7, [r5, #18]
.L_020013a4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xe330
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #4
	bl 0x0200d1c4
	adds	r5, r0, #0
	ldr	r0, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #12
	ldr	r1, [r5, #12]
.L_020013c6:
	adds	r0, r0, r3
	movs	r3, #224
	lsls	r3, r3, #13
	mov	r8, r3
	movs	r3, #128
	ldr	r2, [r5, #16]
	add	r1, r8
	lsls	r3, r3, #5
	movs	r6, #20
	str	r6, [sp, #0]
	bl 0x02009360
	movs	r0, #151
	bl 0x0200d394
	movs	r0, #20
	bl 0x0200d1a4
	ldr	r0, [r5, #8]
	ldr	r3, [pc, #36]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
	movs	r3, #240
	ldr	r2, [r5, #16]
	add	r1, r8
	lsls	r3, r3, #8
	str	r6, [sp, #0]
	bl 0x02009360
	movs	r0, #151
	bl 0x0200d394
	movs	r0, #20
	bl 0x0200d1a4
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb520
	adds	r5, r0, #0
	movs	r1, #1
	movs	r0, #144
	sub	sp, #8
	bl 0x0200d304
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200d30c
	movs	r0, #1
	bl 0x0200d2fc
	movs	r3, #19
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #44
	movs	r1, #5
	movs	r2, #3
	movs	r3, #3
	bl 0x0200d194
	bl 0x0200d314
	bl 0x0200d31c
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r1, #1
	movs	r0, #144
	sub	sp, #8
	bl 0x0200d304
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200d30c
	movs	r0, #3
	bl 0x0200d2fc
	movs	r3, #19
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #39
	movs	r1, #5
	movs	r2, #3
	movs	r3, #3
	bl 0x0200d194
	bl 0x0200d314
	bl 0x0200d31c
	add	sp, #8
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #28
	sub	sp, #12
	bl 0x0200d154
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #15
	lsls	r2, r2, #12
	movs	r0, #8
	bl 0x0200d20c
	ldr	r0, [pc, #860]
	bl 0x0200d24c
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #4
	bl 0x0200d28c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #4
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1cc
	movs	r2, #152
	movs	r0, #4
	movs	r1, #152
	lsls	r2, r2, #1
	bl 0x0200d1ec
	movs	r2, #16
	movs	r3, #176
	movs	r0, #9
	movs	r1, #32
	negs	r2, r2
	lsls	r3, r3, #8
	bl 0x0200d334
	movs	r3, #192
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200d334
	movs	r3, #176
	movs	r0, #6
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200d334
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_0200153c
	movs	r3, #192
	movs	r0, #7
	movs	r1, #8
	movs	r2, #16
	lsls	r3, r3, #8
	bl 0x0200d334
.L_0200153c:
	movs	r1, #16
	movs	r2, #16
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d344
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #9
	bl 0x0200d204
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d28c
	movs	r1, #2
	movs	r0, #9
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020015d6
	movs	r1, #2
	movs	r0, #7
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
.L_020015d6:
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #4
	bl 0x0200d28c
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020015fe
	movs	r0, #7
	movs	r1, #2
	bl 0x0200d22c
.L_020015fe:
	movs	r0, #6
	movs	r1, #2
	bl 0x0200d22c
	movs	r0, #9
	movs	r1, #2
	bl 0x0200d22c
	movs	r0, #4
	movs	r1, #2
	bl 0x0200d22c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d234
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #144
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #8
	bl 0x0200d1c4
	movs	r1, #2
	mov	sl, r0
	movs	r0, #8
	bl 0x0200d21c
	movs	r6, #0
.L_02001682:
	lsls	r7, r6, #13
	adds	r0, r7, #0
	bl 0x0200d13c
	ldr	r3, [pc, #404]
	mov	r5, sp
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_02001696
	adds	r3, #7
.L_02001696:
	lsls	r2, r0, #1
	movs	r1, #208
	lsls	r1, r1, #15
	adds	r2, r2, r0
	asrs	r3, r3, #3
	lsls	r2, r2, #2
	adds	r3, r3, r1
	subs	r3, r3, r2
	str	r3, [r5, #0]
	adds	r0, r7, #0
	bl 0x0200d13c
	movs	r3, #176
	lsls	r3, r3, #15
	adds	r2, r6, #0
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L_020016bc
	adds	r2, #7
.L_020016bc:
	movs	r3, #128
	lsls	r3, r3, #14
	asrs	r2, r2, #3
	adds	r2, r2, r3
	lsls	r3, r0, #3
	subs	r3, r3, r0
	subs	r2, r2, r3
	movs	r3, #0
	str	r2, [r5, #8]
	str	r3, [r5, #4]
	bl 0x0200d134
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	movs	r2, #200
	lsrs	r3, r3, #16
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #153
	adds	r1, r3, #0
	muls	r1, r2
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #204
	muls	r2, r3
	movs	r0, #8
	bl 0x0200d1cc
	ldr	r1, [r5, #0]
	cmp	r1, #0
	bge.n	.L_02001704
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r1, r1, r2
.L_02001704:
	ldr	r2, [r5, #8]
	asrs	r1, r1, #16
	cmp	r2, #0
	bge.n	.L_02001714
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_02001714:
	asrs	r2, r2, #16
	movs	r0, #8
	adds	r6, #1
	bl 0x0200d1ec
	cmp	r6, #7
	ble.n	.L_02001682
	movs	r1, #1
	movs	r0, #8
	bl 0x0200d21c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #2
	bl 0x0200d234
	ldr	r1, [pc, #232]
	movs	r0, #8
	bl 0x0200d1d4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	movs	r1, #0
	subs	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #8
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #8
	bl 0x0200d1dc
	movs	r0, #20
	bl 0x0200d1a4
.L_02001768:
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #1
	bl 0x0200d29c
	movs	r1, #2
	movs	r0, #8
	bl 0x0200d21c
	mov	r2, sl
	ldr	r2, [r2, #8]
	mov	r3, sl
	mov	r8, r2
	movs	r1, #128
	movs	r2, #184
	ldr	r7, [r3, #16]
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	mov	r9, r1
	mov	sl, r2
	movs	r6, #0
.L_020017a2:
	lsls	r0, r6, #13
	bl 0x0200d13c
	mov	r1, r9
	mov	r2, r8
	subs	r3, r1, r2
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_020017b6
	adds	r3, #7
.L_020017b6:
	asrs	r3, r3, #3
	add	r3, r8
	lsls	r2, r0, #2
	subs	r3, r3, r2
	mov	r1, sl
	str	r3, [r5, #0]
	subs	r3, r1, r7
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_020017cc
	adds	r3, #7
.L_020017cc:
	asrs	r3, r3, #3
	adds	r3, r7, r3
	str	r3, [r5, #8]
	movs	r3, #0
	str	r3, [r5, #4]
	bl 0x0200d134
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	movs	r2, #200
	lsrs	r3, r3, #16
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #153
	adds	r1, r3, #0
	muls	r1, r2
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #204
	muls	r2, r3
	movs	r0, #8
	bl 0x0200d1cc
	ldr	r1, [r5, #0]
	cmp	r1, #0
	bge.n	.L_0200180a
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r1, r1, r2
.L_0200180a:
	ldr	r2, [r5, #8]
	asrs	r1, r1, #16
	cmp	r2, #0
	bge.n	.L_02001828
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
	b.n	.L_02001828
	.4byte 0x00001952
	.4byte 0xfff00000
	.2byte 0xd45c
	.2byte 0x0200
.L_02001828:
	asrs	r2, r2, #16
	movs	r0, #8
	adds	r6, #1
	bl 0x0200d1ec
	cmp	r6, #7
	ble.n	.L_020017a2
	movs	r0, #8
	movs	r1, #1
	bl 0x0200d21c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #8
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #8
	bl 0x0200d28c
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #153
	adds	r2, #204
	movs	r0, #8
	bl 0x0200d1cc
	movs	r0, #8
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
.L_020018c0:
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #148
	movs	r0, #8
	bl 0x0200d1ec
	movs	r0, #1
	bl 0x0200d1a4
	movs	r0, #8
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #160
	movs	r2, #160
	strb	r3, [r0, #0]
	lsls	r1, r1, #10
	movs	r0, #8
	lsls	r2, r2, #9
	bl 0x0200d1cc
	movs	r1, #0
	movs	r2, #4
	movs	r0, #8
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d2ac
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #8
	ldr	r1, [pc, #884]
	ldr	r2, [pc, #888]
	bl 0x0200d1cc
	movs	r1, #24
	movs	r2, #8
	movs	r0, #8
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d344
	movs	r1, #8
	movs	r2, #100
	negs	r1, r1
	negs	r2, r2
	movs	r0, #8
	bl 0x0200d344
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r0, #168
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #16
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #10
.L_0200199c:
	bl 0x0200d1a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d28c
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200d28c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020019ce
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200d28c
.L_020019ce:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200d224
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001a44
	movs	r1, #208
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02001a44:
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001ac6
	movs	r1, #2
	movs	r0, #7
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
.L_02001ac6:
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d28c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d28c
	movs	r1, #2
	movs	r2, #50
	adds	r1, #255
	movs	r0, #4
	bl 0x0200d28c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001bb6
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
.L_02001bb6:
	movs	r1, #3
	movs	r0, #4
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #8
	movs	r2, #8
	movs	r0, #9
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d344
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #10
	movs	r2, #6
	movs	r0, #9
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d344
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #14
	movs	r2, #4
	movs	r0, #9
	negs	r1, r1
	negs	r2, r2
	bl 0x0200d344
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001c68
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02001c68:
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001cd0
	movs	r1, #176
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	b.n	.L_02001cd0
	.2byte 0x0000
	.4byte 0x00026666
	.2byte 0x3333
	.2byte 0x0001
.L_02001cd0:
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #5
	bl 0x0200d254
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	bne.n	.L_02001d58
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001d36
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02001d36:
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	movs	r2, #0
	adds	r0, #9
	bl 0x0200d25c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_02001d56:
	b.n	.L_02001db4
.L_02001d58:
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001d94
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02001d94:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
.L_02001db4:
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200d224
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d234
	movs	r0, #10
.L_02001de0:
	bl 0x0200d1a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d26c
	movs	r0, #15
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #8
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #8
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #128
.L_02001e5c:
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #6
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
.L_02001e74:
	movs	r0, #9
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	movs	r2, #0
	adds	r0, #9
	bl 0x0200d25c
	movs	r0, #6
	bl 0x020093b0
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #128
.L_02001e9c:
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #6
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
.L_02001ebc:
	lsls	r2, r2, #6
	adds	r1, #102
	adds	r2, #51
	movs	r0, #9
	bl 0x0200d1cc
	movs	r0, #9
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #16
	strb	r3, [r0, #0]
	movs	r1, #0
.L_02001edc:
	negs	r2, r2
	movs	r0, #9
	bl 0x0200d344
	movs	r0, #1
	bl 0x0200d1a4
	movs	r0, #9
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
.L_02001ef4:
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001f68
	movs	r1, #176
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02001f68:
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001f88
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
.L_02001f88:
	movs	r1, #3
	movs	r0, #4
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #35
	bl 0x0200d1a4
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02001fb2
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02001fb2:
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d1cc
	movs	r1, #0
	movs	r2, #16
	movs	r0, #9
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d28c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200d28c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02002054
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200d28c
.L_02002054:
	movs	r1, #128
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200d28c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200d1cc
	movs	r1, #16
	movs	r0, #9
	negs	r1, r1
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020020dc
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_020020dc:
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #4
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200d224
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02002150
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200d28c
.L_02002150:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d28c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d28c
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200d28c
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #4
	bl 0x0200d274
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020021ac
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
.L_020021ac:
	movs	r1, #3
	movs	r0, #5
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #30
	bl 0x0200d1a4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #292]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #9
	movs	r1, #2
	bl 0x0200d21c
	ldr	r3, [pc, #280]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r3, r1
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_020021f6
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x0200d1e4
.L_020021f6:
	movs	r0, #9
	bl 0x0200d204
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #220]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #5
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_02002234
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200d1e4
.L_02002234:
	movs	r0, #5
	bl 0x0200d204
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #160]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #6
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_02002272
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200d1e4
.L_02002272:
	movs	r0, #6
	bl 0x0200d204
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020022ca
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #88]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #7
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_020022ba
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200d1e4
.L_020022ba:
	movs	r0, #7
	bl 0x0200d204
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
.L_020022ca:
	ldr	r3, [pc, #36]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200d29c
	bl 0x0200d1b4
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #29
	bl 0x0200d154
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	ldr	r0, [pc, #888]
	bl 0x0200d24c
	movs	r1, #144
	movs	r2, #188
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200d20c
	movs	r1, #140
	movs	r2, #194
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d1ec
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r3, #176
	lsls	r3, r3, #8
	movs	r1, #16
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d334
	movs	r0, #9
	bl 0x0200d204
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x0200d294
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200d294
	movs	r0, #45
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #140
	movs	r1, #1
	movs	r2, #188
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #17
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #144
	movs	r2, #180
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200d20c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1cc
	movs	r2, #180
	movs	r1, #168
	lsls	r2, r2, #2
	movs	r0, #8
	bl 0x0200d1ec
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200d1cc
	movs	r0, #8
	movs	r1, #6
	movs	r2, #0
	bl 0x0200d224
	movs	r2, #180
	movs	r1, #200
	lsls	r2, r2, #2
	movs	r0, #8
	bl 0x0200d1ec
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1cc
	movs	r2, #178
	movs	r0, #8
	movs	r1, #216
	lsls	r2, r2, #2
	bl 0x0200d1ec
	movs	r1, #160
	movs	r2, #178
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d1ec
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #8
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #153
	adds	r2, #204
	movs	r0, #8
	bl 0x0200d1cc
	movs	r0, #8
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #148
	movs	r2, #178
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #8
	bl 0x0200d1ec
	movs	r0, #1
	bl 0x0200d1a4
	movs	r0, #8
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200d1a4
	movs	r1, #144
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #240
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #144
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #45
	movs	r0, #8
	bl 0x0200d28c
	movs	r0, #8
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r0, #8
	movs	r1, #6
	movs	r2, #23
	bl 0x0200d224
	movs	r0, #8
	ldr	r1, [pc, #324]
	ldr	r2, [pc, #324]
	bl 0x0200d1cc
	movs	r2, #178
	movs	r1, #200
	lsls	r2, r2, #2
	movs	r0, #8
	bl 0x0200d1ec
	movs	r0, #3
	bl 0x0200d1a4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200d1cc
	movs	r0, #8
	movs	r1, #6
	movs	r2, #0
	bl 0x0200d224
	movs	r2, #178
	movs	r1, #168
	lsls	r2, r2, #2
	movs	r0, #8
	bl 0x0200d1ec
	movs	r0, #3
	bl 0x0200d1a4
	movs	r0, #8
	ldr	r1, [pc, #252]
	ldr	r2, [pc, #256]
	bl 0x0200d1cc
	movs	r2, #178
	movs	r0, #8
	movs	r1, #144
	lsls	r2, r2, #2
	bl 0x0200d1ec
	movs	r2, #0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200d20c
	movs	r0, #30
	bl 0x0200d1a4
	ldr	r5, [pc, #224]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200d29c
	bl 0x0200d2b4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200d224
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #68]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #9
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_0200266c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x0200d1e4
.L_0200266c:
	movs	r0, #9
	bl 0x0200d204
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	bl 0x0200d1b4
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000019ab
	.4byte 0x00026666
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #30
	bl 0x0200d154
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	ldr	r0, [pc, #360]
	bl 0x0200d24c
	movs	r1, #164
	movs	r2, #160
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	bl 0x0200d20c
	movs	r1, #164
	movs	r2, #160
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	bl 0x0200d20c
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r0, #4
	lsls	r1, r1, #1
	movs	r2, #224
	bl 0x0200d1ec
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #16
	movs	r2, #8
	movs	r3, #192
	negs	r2, r2
	lsls	r3, r3, #8
	negs	r1, r1
	movs	r0, #9
	bl 0x0200d334
	movs	r0, #9
	bl 0x0200d204
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200d2a4
	movs	r0, #164
.L_0200273c:
	movs	r1, #1
	movs	r2, #152
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200d2ac
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
.L_0200275e:
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #6
	bl 0x0200d26c
	movs	r0, #4
	movs	r1, #1
	bl 0x0200d2bc
	bl 0x0200d2b4
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200d2a4
	movs	r1, #0
	movs	r0, #9
	bl 0x0200d254
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	bne.n	.L_02002818
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002846
	.2byte 0x0000
	.2byte 0x19b0
	.2byte 0x0000
.L_02002818:
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #9
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
.L_02002846:
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #4
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r3, #192
	movs	r0, #5
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200d334
	movs	r3, #192
	movs	r0, #6
	movs	r1, #32
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200d334
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020028b2
	movs	r2, #8
	movs	r3, #160
	movs	r0, #7
	movs	r1, #48
	negs	r2, r2
	lsls	r3, r3, #8
	bl 0x0200d334
.L_020028b2:
	movs	r0, #6
	bl 0x0200d204
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d28c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d26c
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #224
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #164
	movs	r1, #1
	movs	r2, #144
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #164
	movs	r2, #136
	lsls	r2, r2, #16
	lsls	r1, r1, #17
	movs	r0, #11
	bl 0x0200d20c
	movs	r0, #11
	bl 0x0200d1c4
	movs	r1, #0
	bl 0x0200d19c
	movs	r0, #11
	bl 0x02009418
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #51
	adds	r2, #153
	bl 0x0200d1cc
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #51
	adds	r2, #153
	bl 0x0200d1cc
	movs	r1, #164
	movs	r2, #128
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #10
	bl 0x0200d20c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #16
	bl 0x0200d344
	movs	r1, #164
	movs	r2, #128
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #8
	bl 0x0200d20c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #16
	bl 0x0200d33c
	movs	r1, #0
	movs	r2, #16
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #11
	bl 0x02009454
	movs	r2, #0
	movs	r0, #11
	movs	r1, #0
	bl 0x0200d20c
	movs	r1, #1
	movs	r0, #10
	bl 0x0200d29c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #24
	bl 0x0200d33c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #24
	bl 0x0200d344
	movs	r0, #8
	movs	r1, #16
	movs	r2, #8
	bl 0x0200d344
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #164
	movs	r1, #1
	movs	r2, #208
	movs	r3, #1
	lsls	r2, r2, #16
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #8
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #2
	bl 0x0200d22c
	movs	r0, #6
	movs	r1, #2
	bl 0x0200d22c
	movs	r0, #5
	movs	r1, #2
	bl 0x0200d22c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02002b98
	movs	r0, #7
	movs	r1, #2
	bl 0x0200d22c
.L_02002b98:
	movs	r1, #2
	movs	r0, #4
	bl 0x0200d234
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #8
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r0, #8
	movs	r1, #6
	movs	r2, #23
	bl 0x0200d224
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d28c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d28c
	movs	r1, #2
	movs	r0, #6
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02002c86
	movs	r1, #2
	movs	r0, #7
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
.L_02002c86:
	movs	r1, #2
	movs	r2, #60
	adds	r1, #255
	movs	r0, #9
	bl 0x0200d28c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #0
	movs	r0, #10
	bl 0x0200d254
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	beq.n	.L_02002ce6
	movs	r0, #40
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d25c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002d16
.L_02002ce6:
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #10
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
.L_02002d16:
	movs	r1, #2
	movs	r0, #8
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d28c
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d28c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d28c
	movs	r1, #2
	movs	r0, #6
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02002e32
	movs	r1, #2
	movs	r0, #7
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
.L_02002e32:
	movs	r1, #2
	adds	r1, #255
	movs	r2, #60
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	bl 0x0200d254
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02002e8e
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02002e8e:
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	beq.n	.L_02002ed2
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d234
	movs	r0, #20
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d25c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002f08
.L_02002ed2:
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #5
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
.L_02002f08:
	movs	r1, #224
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02002f6a
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02002f6a:
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02002ffc
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
.L_02002ffc:
	movs	r1, #3
	movs	r0, #9
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #129
.L_02003058:
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200d28c
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #8
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d28c
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #0
	movs	r2, #16
	movs	r0, #8
	bl 0x0200d344
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r0, #8
	movs	r1, #6
	movs	r2, #23
	bl 0x0200d224
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	movs	r2, #160
	movs	r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200d1cc
	movs	r1, #160
	movs	r2, #160
	movs	r0, #8
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200d1cc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #8
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #4
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d224
	movs	r0, #10
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #8
	strb	r3, [r0, #0]
	negs	r1, r1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d33c
	movs	r0, #8
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #16
	ands	r5, r3
	movs	r2, #0
	strb	r5, [r0, #0]
	movs	r0, #8
	bl 0x0200d344
	movs	r0, #1
	bl 0x0200d1a4
	movs	r0, #8
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #5
	movs	r0, #8
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #166
	movs	r2, #166
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1cc
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #51
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200d28c
	movs	r0, #8
	movs	r1, #1
	bl 0x0200d21c
	movs	r0, #8
	movs	r1, #6
	movs	r2, #0
	bl 0x0200d224
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200d294
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #15
	bl 0x0200d1a4
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200d294
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200d28c
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #45
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #45
	bl 0x0200d1a4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200d26c
	movs	r0, #35
	bl 0x0200d1a4
	movs	r0, #8
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #4
	movs	r1, #8
	bl 0x0200d34c
	movs	r0, #9
	movs	r1, #8
	bl 0x0200d34c
	movs	r0, #5
	movs	r1, #8
	bl 0x0200d34c
	movs	r0, #6
	movs	r1, #8
	bl 0x0200d34c
	movs	r0, #10
	movs	r1, #8
	bl 0x0200d34c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_0200346e
	movs	r0, #7
	movs	r1, #8
	bl 0x0200d34c
.L_0200346e:
	movs	r1, #188
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #208
	bl 0x0200d1ec
	movs	r0, #8
	movs	r1, #0
	movs	r2, #16
	bl 0x0200d344
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #24
	bl 0x0200d344
	movs	r1, #32
	movs	r0, #8
	negs	r1, r1
	movs	r2, #16
	bl 0x0200d344
	movs	r1, #0
	movs	r2, #48
	movs	r0, #8
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #4
	bl 0x0200d1dc
	movs	r0, #9
	bl 0x0200d1dc
	movs	r0, #5
	bl 0x0200d1dc
	movs	r0, #6
	bl 0x0200d1dc
	movs	r0, #10
	bl 0x0200d1dc
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020034da
	movs	r0, #7
	bl 0x0200d1dc
.L_020034da:
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200d20c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02003542
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02003542:
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #5
	bl 0x0200d28c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020036f4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_020036f4:
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_0200371c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
.L_0200371c:
	movs	r1, #3
	movs	r0, #9
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #9
	ldr	r1, [pc, #288]
	bl 0x0200d1cc
	movs	r0, #9
	movs	r1, #2
	bl 0x0200d21c
	ldr	r3, [pc, #280]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_0200377a
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x0200d1e4
.L_0200377a:
	movs	r0, #9
	bl 0x0200d204
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #220]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #5
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_020037b8
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200d1e4
.L_020037b8:
	movs	r0, #5
	bl 0x0200d204
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #160]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #6
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_020037f6
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200d1e4
.L_020037f6:
	movs	r0, #6
	bl 0x0200d204
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_0200384e
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #88]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #7
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_0200383e
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200d1e4
.L_0200383e:
	movs	r0, #7
	bl 0x0200d204
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
.L_0200384e:
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200d29c
	bl 0x02009344
	movs	r0, #11
	bl 0x0200d2c4
	bl 0x0200d1b4
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #130
	lsls	r0, r0, #4
	adds	r0, #255
	sub	sp, #28
	bl 0x0200d154
	bl 0x0200d1ac
	movs	r0, #0
	bl 0x0200d324
	ldr	r0, [pc, #496]
	bl 0x0200d24c
	movs	r1, #164
	movs	r2, #160
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	bl 0x0200d20c
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #4
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1cc
	movs	r1, #160
	movs	r0, #4
	lsls	r1, r1, #1
	movs	r2, #224
	bl 0x0200d1ec
	movs	r1, #16
	movs	r3, #192
	movs	r0, #9
	negs	r1, r1
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200d334
	movs	r3, #192
	movs	r0, #5
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200d334
	movs	r3, #192
	movs	r0, #6
	movs	r1, #32
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200d334
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02003920
	movs	r3, #192
	movs	r0, #7
	movs	r1, #48
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200d334
.L_02003920:
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #6
	bl 0x0200d204
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #164
	movs	r1, #1
	movs	r2, #144
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #164
	movs	r2, #136
	lsls	r2, r2, #16
	lsls	r1, r1, #17
	movs	r0, #11
	bl 0x0200d20c
	movs	r0, #11
	bl 0x0200d1c4
	movs	r1, #0
	bl 0x0200d19c
	movs	r0, #11
	bl 0x02009418
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #51
	adds	r2, #153
	bl 0x0200d1cc
	movs	r1, #164
	movs	r2, #128
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #10
	bl 0x0200d20c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #0
	movs	r2, #16
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #11
	bl 0x02009454
	movs	r2, #0
	movs	r0, #11
	movs	r1, #0
	bl 0x0200d20c
	movs	r1, #1
	movs	r0, #10
	bl 0x0200d29c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #48
	bl 0x0200d344
	movs	r1, #224
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02003a1e
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02003a1e:
	movs	r0, #164
	movs	r1, #1
	movs	r2, #184
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02003a8c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
	b.n	.L_02003a8c
	.2byte 0x1a2e
	.2byte 0x0000
.L_02003a8c:
	movs	r1, #3
	movs	r0, #9
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r2, #16
	negs	r2, r2
	movs	r1, #0
	movs	r0, #9
	bl 0x0200d344
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #8
	movs	r0, #10
	negs	r1, r1
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #16
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #5
	bl 0x0200d28c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #8
	movs	r0, #10
	negs	r1, r1
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #35
	bl 0x0200d1a4
	movs	r1, #8
	movs	r0, #10
	negs	r1, r1
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #45
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200d224
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #192
	lsls	r0, r0, #7
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r2, #12
	negs	r2, r2
	movs	r1, #4
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r2, #12
	movs	r1, #4
	negs	r2, r2
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
.L_02003e82:
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02003f32
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02003f32:
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02003f6a
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02003f6a:
	movs	r1, #224
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #15
	bl 0x0200d1a4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #16
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #16
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004216
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02004216:
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #16
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #172
	movs	r1, #1
	movs	r2, #208
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	bl 0x0200d2ac
	bl 0x0200d2b4
	movs	r0, #20
.L_02004250:
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #10
	ldr	r1, [pc, #456]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #10
	movs	r1, #16
	movs	r2, #16
	bl 0x0200d344
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #51
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020042bc
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_020042bc:
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #6
	bl 0x0200d28c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #0
	movs	r0, #6
	bl 0x0200d254
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004400
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02004400:
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	bne.n	.L_02004434
	movs	r0, #25
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d25c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02004456
	.2byte 0x3333
	.2byte 0x0001
.L_02004434:
	movs	r0, #40
	bl 0x0200d1a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #6
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
.L_02004456:
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d28c
	movs	r1, #2
	movs	r0, #5
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004484
	movs	r1, #2
	movs	r0, #7
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d28c
.L_02004484:
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #10
	bl 0x0200d28c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200d28c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #6
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d28c
	movs	r1, #6
	movs	r2, #50
	adds	r1, #255
	movs	r0, #10
	bl 0x0200d28c
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004596
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
.L_02004596:
	movs	r1, #3
	movs	r0, #9
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #6
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200d294
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200d224
	movs	r2, #23
	movs	r1, #6
	movs	r0, #9
	bl 0x0200d224
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r3, #5
	movs	r2, #17
	movs	r1, #14
	movs	r0, #20
	movs	r4, #9
	str	r3, [sp, #0]
	str	r2, [sp, #8]
	str	r1, [sp, #12]
	str	r0, [sp, #16]
	movs	r5, #0
	movs	r0, #10
	movs	r1, #9
	movs	r2, #2
	movs	r3, #9
	str	r4, [sp, #4]
	str	r4, [sp, #20]
	str	r5, [sp, #24]
	bl 0x0200d27c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004694
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02004694:
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #18
	movs	r0, #9
	negs	r1, r1
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004880
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	b.n	.L_02004892
.L_02004880:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_02004892:
	movs	r1, #42
	movs	r0, #9
	negs	r1, r1
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #16
	movs	r0, #10
	negs	r1, r1
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #25
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #40
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x020093b0
	movs	r0, #10
	bl 0x020093b0
	movs	r0, #20
	bl 0x0200d1a4
	movs	r2, #0
	movs	r1, #16
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #16
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200d294
	movs	r0, #40
	bl 0x0200d1a4
	movs	r2, #12
	movs	r1, #0
	negs	r2, r2
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r2, #12
	movs	r1, #0
	negs	r2, r2
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #10
	bl 0x0200d28c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #5
	bl 0x0200d1a4
	movs	r0, #12
	bl 0x0200d1c4
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r2, #12]
	movs	r1, #1
	movs	r0, #12
	mov	r8, r2
	bl 0x0200d284
	movs	r0, #12
	bl 0x0200d1c4
	movs	r1, #0
	bl 0x0200d19c
	movs	r1, #164
	movs	r2, #160
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200d20c
	movs	r0, #12
	movs	r1, #9
	movs	r2, #0
	bl 0x0200d224
	movs	r7, #0
	movs	r6, #20
.L_02004af6:
	adds	r0, r7, #0
	movs	r1, #20
	bl 0x0200d11c
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	adds	r0, r0, r3
	mov	r2, r8
	str	r0, [r2, #12]
	movs	r2, #128
	ldrh	r3, [r5, #6]
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r5, #6]
	movs	r0, #1
	bl 0x0200d124
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	subs	r6, #1
	adds	r7, r7, r3
	cmp	r6, #0
	bge.n	.L_02004af6
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
	movs	r2, #8
	movs	r1, #0
	movs	r0, #12
	bl 0x0200d344
	movs	r0, #12
	bl 0x0200d1c4
	movs	r1, #1
	bl 0x0200d19c
	movs	r0, #5
	bl 0x0200d1a4
	movs	r1, #0
	movs	r2, #8
	movs	r0, #12
	bl 0x0200d344
	movs	r0, #5
	bl 0x0200d1a4
	movs	r1, #0
	movs	r2, #8
	movs	r0, #12
	bl 0x0200d344
	movs	r0, #5
	bl 0x0200d1a4
	movs	r1, #0
	movs	r2, #8
	movs	r0, #12
	bl 0x0200d344
	movs	r0, #5
	bl 0x0200d1a4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d28c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200d28c
	movs	r1, #192
	lsls	r1, r1, #6
.L_02004ba4:
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #24
	bl 0x0200d344
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #10
	bl 0x0200d254
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004c42
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02004c42:
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200d1bc
	cmp	r0, #0
	bne.n	.L_02004c7c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #0
	adds	r0, #10
	movs	r1, #0
	bl 0x0200d25c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02004ca2
.L_02004c7c:
	movs	r0, #40
	bl 0x0200d1a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
.L_02004ca2:
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #224
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004cdc
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02004cdc:
	movs	r0, #30
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #10
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	ldr	r1, [pc, #976]
	ldr	r2, [pc, #976]
	movs	r0, #10
	bl 0x0200d1cc
	movs	r0, #10
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #160
	strb	r3, [r0, #0]
	lsls	r1, r1, #7
	movs	r0, #10
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #10
	movs	r1, #0
	movs	r2, #8
	bl 0x0200d344
	movs	r2, #8
	movs	r1, #0
	negs	r2, r2
	movs	r0, #10
	bl 0x0200d344
	movs	r0, #1
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1c4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #1
	bl 0x0200d1a4
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #51
	adds	r2, #153
	bl 0x0200d1cc
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004da2
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
.L_02004da2:
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r0, #12
	movs	r1, #3
	movs	r2, #9
	bl 0x0200d32c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	bl 0x0200d2ac
	movs	r1, #1
	movs	r0, #4
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #132
	adds	r3, #1
.L_02004e08:
	strh	r3, [r2, #0]
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200d28c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d26c
	movs	r0, #30
	bl 0x0200d1a4
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d21c
	movs	r0, #33
	bl 0x0200d1a4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	bl 0x0200d1a4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200d28c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004f2a
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
.L_02004f2a:
	movs	r1, #3
	movs	r0, #9
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #12
	movs	r2, #0
	bl 0x0200d344
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d26c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #9
	bl 0x0200d26c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r1, #2
	movs	r0, #9
	bl 0x0200d234
	movs	r0, #10
	bl 0x0200d1a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d25c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_02004fbc
	movs	r0, #7
	movs	r1, #3
	bl 0x0200d21c
.L_02004fbc:
	movs	r0, #5
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200d21c
	movs	r0, #20
	bl 0x0200d1a4
	movs	r0, #20
	bl 0x0200d1a4
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #9
	ldr	r1, [pc, #232]
	bl 0x0200d1cc
	movs	r0, #9
	movs	r1, #2
	bl 0x0200d21c
	ldr	r3, [pc, #224]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_02005016
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x0200d1e4
.L_02005016:
	movs	r0, #9
	bl 0x0200d204
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #164]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #5
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_02005054
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200d1e4
.L_02005054:
	movs	r0, #5
	bl 0x0200d204
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #104]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #6
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_02005092
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200d1e4
.L_02005092:
	movs	r0, #6
	bl 0x0200d204
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
	movs	r0, #7
	bl 0x0200d14c
	cmp	r0, #0
	beq.n	.L_020050fa
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #32]
	adds	r2, #153
	bl 0x0200d1cc
	movs	r0, #7
	movs	r1, #2
	bl 0x0200d21c
	ldr	r0, [r5, #0]
	bl 0x0200d1c4
	cmp	r0, #0
	beq.n	.L_020050ea
	b.n	.L_020050dc
	.2byte 0x0000
	.4byte 0x00026666
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
.L_020050dc:
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200d1e4
.L_020050ea:
	movs	r0, #7
	bl 0x0200d204
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d20c
.L_020050fa:
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200d29c
	bl 0x0200d1b4
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
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
	.4byte 0x00090008
	.4byte 0x000b000a
	.4byte 0x0000ffff
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x001f001f
	.4byte 0x001d001f
	.4byte 0x001e001d
	.4byte 0x001c001b
	.4byte 0x0019001d
	.4byte 0x001c001b
	.4byte 0x00190017
	.4byte 0x0015001b
	.4byte 0x001a0018
	.4byte 0x0012000f
	.4byte 0x000a0013
	.4byte 0x000d000c
	.4byte 0x00060005
	.4byte 0x00180006
	.4byte 0x001e001c
	.4byte 0x00180013
	.4byte 0x000f001b
	.4byte 0x00190014
	.4byte 0x0010000b
	.4byte 0x00070017
	.4byte 0x0015000c
	.4byte 0x00080003
	.4byte 0x001b0013
	.4byte 0x001b001b
	.4byte 0x00180010
	.4byte 0x0000001d
	.4byte 0x0200d39c
	.4byte 0x0200d3d8
	.4byte 0x0200d414
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffc0000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00060000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000002e
	.4byte 0x02008421
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003b
	.4byte 0x10101040
	.4byte 0xffffffff
	.4byte 0x10204041
	.4byte 0xffffffff
	.4byte 0x10303040
	.4byte 0xffffffff
	.4byte 0x10404040
	.4byte 0xffffffff
	.4byte 0x10505040
	.4byte 0xffffffff
	.4byte 0x10602042
	.4byte 0xffffffff
	.4byte 0x1070403e
	.4byte 0xffffffff
	.4byte 0x1080103e
	.4byte 0xffffffff
	.4byte 0x1090b03e
	.4byte 0xffffffff
	.4byte 0x10a0203c
	.4byte 0xffffffff
	.4byte 0x10b0303c
	.4byte 0xffffffff
	.4byte 0x10c0103f
	.4byte 0xffffffff
	.4byte 0x0000003c
	.4byte 0x1010203d
	.4byte 0xffffffff
	.4byte 0x1020a03b
	.4byte 0xffffffff
	.4byte 0x1030b03b
	.4byte 0xffffffff
	.4byte 0x0000003d
	.4byte 0x1010f002
	.4byte 0xffffffff
	.4byte 0x1020103c
	.4byte 0xffffffff
	.4byte 0x0000003e
	.4byte 0x1010803b
	.4byte 0xffffffff
	.4byte 0x1020303e
	.4byte 0xffffffff
	.4byte 0x1030203e
	.4byte 0xffffffff
	.4byte 0x1040703b
	.4byte 0xffffffff
	.4byte 0x10503041
	.4byte 0xffffffff
	.4byte 0x10605041
	.4byte 0xffffffff
	.4byte 0x10706040
	.4byte 0xffffffff
	.4byte 0x10807040
	.4byte 0xffffffff
	.4byte 0x1090a03e
	.4byte 0xffffffff
	.4byte 0x10a0903e
	.4byte 0xffffffff
	.4byte 0x10b0903b
	.4byte 0xffffffff
	.4byte 0x0000003f
	.4byte 0x1010c03b
	.4byte 0xffffffff
	.4byte 0x1020403f
	.4byte 0xffffffff
	.4byte 0x1030503f
	.4byte 0xffffffff
	.4byte 0x1040203f
	.4byte 0xffffffff
	.4byte 0x1050303f
	.4byte 0xffffffff
	.4byte 0x1060703f
	.4byte 0xffffffff
	.4byte 0x1070603f
	.4byte 0xffffffff
	.4byte 0x10b63040
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00010000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00010000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00018000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0102c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0102c000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00010000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff00b9
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00010000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00018000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000002
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00014000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0102c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0001c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x007500f6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x02320000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02720000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x0200d52c
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03100000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200052
	.4byte 0x00020001
	.4byte 0x00500006
	.4byte 0x00010020
	.4byte 0x00060002
	.4byte 0x0052ffff
	.4byte 0x00010023
	.4byte 0x00060002
	.4byte 0x00230050
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x0020004e
	.4byte 0x00020002
	.4byte 0x004e0006
	.4byte 0x00020023
	.4byte 0x00060002
	.4byte 0x0000ffff
	.4byte 0x0200dd9c
	.4byte 0x0015004e
	.4byte 0x0200dd9c
	.4byte 0x00000000
	.4byte 0x0200dd9c
	.4byte 0x000a0048
	.4byte 0x0200ddb2
	.4byte 0x0016004a
	.4byte 0x0200dd9c
	.4byte 0x00170055
	.4byte 0x0200ddc8
	.4byte 0x000d004e
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x020084cd
	.4byte 0x0000c401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020084cd
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x020084cd
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x020084cd
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x020084cd
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c401
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c402
	.4byte 0xffff000c
	.4byte 0x02008961
	.4byte 0x00000002
	.4byte 0x091e0014
	.4byte 0x02008809
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008581
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001977
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008699
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000197b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085cd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000197f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001980
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001981
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001982
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001983
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008fbd
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200905d
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303f
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x020084cd
	.4byte 0x0000c401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020084cd
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x020084cd
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x020084cd
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x020084cd
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c401
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c402
	.4byte 0xffff000c
	.4byte 0x02008961
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000019f9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000019fa
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008665
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008619
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a00
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020086ed
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a05
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a06
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a07
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a08
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a09
	.4byte 0x00008d15
	.4byte 0x0301040d
	.4byte 0x020086ed
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a0a
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008fbd
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200905d
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303f
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
	.4byte 0x00000002
	.4byte 0x091c001e
	.4byte 0x02009491
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200908d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x02100014
	.4byte 0x02009145
	.4byte 0x00000002
	.4byte 0x02110015
	.4byte 0x020091c9
	.4byte 0x10008c15
	.4byte 0x02010008
	.4byte 0x02008e59
	.4byte 0x00008c15
	.4byte 0x02010008
	.4byte 0x02008f75
	.4byte 0x10008c15
	.4byte 0x02020009
	.4byte 0x02008e59
	.4byte 0x00008c15
	.4byte 0x02020009
	.4byte 0x02008f81
	.4byte 0x10008c15
	.4byte 0x0203000a
	.4byte 0x02008e59
	.4byte 0x00008c15
	.4byte 0x0203000a
	.4byte 0x02008f8d
	.4byte 0x10008c15
	.4byte 0x0204000b
	.4byte 0x02008e59
	.4byte 0x00008c15
	.4byte 0x0204000b
	.4byte 0x02008f99
	.4byte 0x00000008
	.4byte 0x02010000
	.4byte 0x02008e69
	.4byte 0x00000009
	.4byte 0x02010000
	.4byte 0x02008fa5
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02009099
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000031
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000021
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008fed
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020090d5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x0000c402
	.4byte 0xffff0006
	.4byte 0x02008961
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0x091d001e
	.4byte 0x0200a2f5
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x020089a5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a8d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a8e
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200901d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02009105
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000026
