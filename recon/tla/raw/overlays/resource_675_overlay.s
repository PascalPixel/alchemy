.syntax unified
	.thumb
	push	{r5, r6, lr}
	adds	r4, r1, #0
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r1, #128
	ldr	r6, [r1, #0]
	ldr	r1, [pc, #148]
	adds	r5, r0, #0
	str	r5, [r1, #0]
	ldr	r1, [pc, #148]
	str	r4, [r1, #0]
	ldr	r1, [pc, #148]
	str	r2, [r1, #0]
	ldr	r2, [pc, #148]
	str	r3, [r2, #0]
	movs	r2, #255
	ldrh	r3, [r5, #0]
	b.n	.L_02000082
.L_0200005c:
	ldrh	r0, [r4, #0]
	adds	r4, #2
	ldrh	r2, [r4, #0]
	adds	r4, #2
	ldrh	r1, [r5, #0]
	ldrh	r3, [r4, #0]
	adds	r5, #2
	adds	r4, #2
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	movs	r2, #160
	lsls	r2, r2, #19
	lsls	r1, r1, #1
	orrs	r3, r0
	adds	r1, r1, r2
	strh	r3, [r1, #0]
	ldrh	r3, [r5, #0]
	movs	r2, #255
.L_02000082:
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02000090
	ldrh	r3, [r4, #0]
	cmp	r3, r2
	bne.n	.L_0200005c
.L_02000090:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r0, #160
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r0, r0, #19
	adds	r1, r6, #0
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r6, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [pc, #56]
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x020092ec
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_020000da
	bl 0x02008328
.L_020000da:
	pop	{r5, r6, pc}
	.4byte 0x02009a24
	.4byte 0x02009a28
	.4byte 0x02009a2c
	.4byte 0x02009a18
	.4byte 0x05000200
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	ldr	r5, [pc, #64]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	ldr	r0, [r3, #0]
	bl 0x020081d4
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #32]
	strh	r0, [r5, #0]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #28]
	movs	r1, #144
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #24]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #48]
	bl 0x020091e4
	b.n	.L_02000154
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0x02009a34
	.4byte 0x02009a30
	.4byte 0x02009a24
	.4byte 0x02009a20
	.4byte 0x02009a1c
	.4byte 0x02009a14
	.2byte 0x81f9
	.2byte 0x0200
.L_02000154:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	ldr	r0, [pc, #8]
	bl 0x020091ec
	pop	{pc}
	.2byte 0x0000
	.2byte 0x81f9
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #40]
	ldr	r5, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	ldr	r0, [r3, #0]
	bl 0x020081d4
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #48]
	strh	r0, [r5, #0]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	movs	r1, #144
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #20]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	bl 0x020091e4
	b.n	.L_020001d0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x02009a34
	.4byte 0x02009a30
	.4byte 0x02009a24
	.4byte 0x02009a20
	.4byte 0x02009a1c
	.4byte 0x02009a14
	.2byte 0x81f9
	.2byte 0x0200
.L_020001d0:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #20]
	ldrh	r3, [r0, #0]
	movs	r2, #0
	cmp	r3, r1
	beq.n	.L_020001f0
.L_020001e0:
	adds	r0, #2
	ldrh	r3, [r0, #0]
	adds	r2, #1
	cmp	r3, r1
	bne.n	.L_020001e0
	b.n	.L_020001f0
	.2byte 0xffff
	.2byte 0x0000
.L_020001f0:
	subs	r2, #1
	adds	r0, r2, #0
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #172]
	movs	r4, #0
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_02000232
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r2, r0
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02000232
	ldr	r0, [pc, #148]
	movs	r4, #1
	ldrh	r2, [r0, #0]
	strh	r2, [r1, #0]
	movs	r1, #128
	lsls	r3, r2, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	bls.n	.L_02000232
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	strh	r3, [r0, #0]
.L_02000232:
	cmp	r4, #0
	bne.n	.L_02000238
	b.n	.L_02000324
.L_02000238:
	ldr	r3, [pc, #116]
	ldr	r6, [pc, #120]
	ldr	r1, [r3, #0]
	ldrh	r3, [r6, #0]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L_02000286
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #112]
	ldr	r7, [r3, #0]
	mov	lr, r2
	mov	ip, r6
.L_02000250:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	ldrh	r3, [r6, #0]
	movs	r0, #160
	muls	r3, r2
	adds	r3, r3, r5
	lsls	r3, r3, #1
	ldrh	r3, [r3, r7]
	lsls	r0, r0, #19
	lsls	r3, r3, #1
	adds	r4, r3, r0
	ldrh	r0, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r1, #2
	ldrh	r3, [r1, #0]
	adds	r1, #2
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #0]
	adds	r5, #1
	mov	r2, ip
	ldrh	r3, [r2, #0]
	cmp	r5, r3
	bcc.n	.L_02000250
.L_02000286:
	ldr	r3, [pc, #44]
	movs	r0, #160
	ldrh	r1, [r3, #0]
	ldr	r3, [pc, #48]
	lsls	r2, r1, #1
	ldr	r3, [r3, #0]
	lsls	r0, r0, #19
	ldrh	r3, [r2, r3]
	adds	r2, r2, r1
	lsls	r3, r3, #1
	adds	r4, r3, r0
	ldr	r3, [pc, #36]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020002cc
	ldr	r3, [pc, #32]
	b.n	.L_020002ce
	.4byte 0x02009a1c
	.4byte 0x02009a20
	.4byte 0x02009a2c
	.4byte 0x02009a30
	.4byte 0x02009a18
	.4byte 0x02009a34
	.4byte 0x02009a24
	.4byte 0x02009a14
	.2byte 0x9a28
	.2byte 0x0200
.L_020002cc:
	ldr	r3, [pc, #68]
.L_020002ce:
	lsls	r2, r2, #1
	ldr	r3, [r3, #0]
	adds	r1, r3, r2
	ldrh	r0, [r1, #0]
	adds	r1, #2
.L_020002d8:
	ldrh	r2, [r1, #0]
	ldrh	r3, [r1, #2]
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #0]
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #32]
	ldrh	r3, [r1, #0]
	eors	r3, r2
	strh	r3, [r1, #0]
	ldr	r1, [pc, #40]
	ldr	r2, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r3, #1
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	ldrh	r2, [r2, #0]
	lsrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_02000324
	ldr	r3, [pc, #8]
	strh	r3, [r1, #0]
	b.n	.L_02000324
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x02009a2c
	.4byte 0x02009a14
	.4byte 0x02009a34
	.2byte 0x9a30
	.2byte 0x0200
.L_02000324:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r5, [r6, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	mov	r8, r1
	add	r5, r8
	ldr	r2, [r5, #0]
	ldr	r0, [pc, #100]
	movs	r1, #1
	mov	sl, r2
	bl 0x020092ec
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x020092ec
	ldr	r2, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, r8
.L_0200035c:
	str	r3, [r2, r1]
	bl 0x02009304
	bl 0x02009314
	bl 0x02008178
	bl 0x0200932c
	movs	r0, #40
	bl 0x020091dc
	bl 0x02008158
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x020092ec
	movs	r0, #16
	bl 0x020092fc
	movs	r0, #16
	bl 0x020091dc
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
	mov	r3, sl
	str	r3, [r5, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x00202108
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #15
	movs	r1, #12
	bl 0x020092dc
	pop	{pc}
	push	{lr}
	movs	r0, #9
	movs	r1, #37
	bl 0x020092dc
	pop	{pc}
	push	{lr}
	movs	r1, #0
	bl 0x0200923c
	movs	r0, #0
	pop	{pc}
	push	{r5, r6, lr}
	adds	r2, r0, #0
	adds	r5, r2, #0
	adds	r5, #98
	ldrb	r3, [r5, #0]
	movs	r0, #63
	adds	r1, r2, #0
	ands	r0, r3
	adds	r1, #85
	movs	r3, #3
	ldr	r6, [r2, #80]
	strb	r3, [r1, #0]
	cmp	r0, #31
	bgt.n	.L_0200040a
	cmp	r0, #0
	bne.n	.L_020003fa
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r2, #40]
.L_020003fa:
	lsls	r0, r0, #12
	bl 0x020091f4
.L_02000400:
	cmp	r0, #0
	bge.n	.L_02000406
	adds	r0, #127
.L_02000406:
	asrs	r3, r0, #7
	strh	r3, [r6, #18]
.L_0200040a:
	ldrb	r3, [r5, #0]
	movs	r0, #1
	adds	r3, #1
	strb	r3, [r5, #0]
	negs	r0, r0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.global Func_02000418
	.thumb_func
Func_02000418:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9460
	.2byte 0x0200
	.global Func_02000420
	.thumb_func
Func_02000420:
	movs	r0, #0
	bx	lr
	.global Func_02000424
	.thumb_func
Func_02000424:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9490
	.2byte 0x0200
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000444
	ldr	r0, [pc, #24]
	b.n	.L_02000450
.L_02000444:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_0200044e
	ldr	r0, [pc, #24]
	b.n	.L_02000450
.L_0200044e:
	ldr	r0, [pc, #24]
.L_02000450:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000088
	.4byte 0x02009668
	.4byte 0x00000089
	.4byte 0x02009740
	.2byte 0x9548
	.2byte 0x0200
	.global Func_0200046c
	.thumb_func
Func_0200046c:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000484
	ldr	r0, [pc, #24]
	b.n	.L_02000490
.L_02000484:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_0200048e
	ldr	r0, [pc, #24]
	b.n	.L_02000490
.L_0200048e:
	ldr	r0, [pc, #24]
.L_02000490:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000089
	.4byte 0x02009914
	.4byte 0x0000008a
	.4byte 0x020099b0
	.2byte 0x9788
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r5, r5, r2
	ldr	r6, [r5, #0]
	bl 0x02008178
	bl 0x0200932c
	movs	r0, #40
	bl 0x020091dc
	bl 0x02008158
.L_020004ce:
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x020092ec
	movs	r0, #16
	bl 0x020092fc
	movs	r0, #16
.L_020004e0:
	bl 0x020091dc
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r1, r3, r2
	movs	r2, #0
	strb	r2, [r1, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x020092bc
	str	r6, [r5, #0]
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x020080f4
	bl 0x02009334
	ldr	r5, [pc, #44]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #194
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #40
	bl 0x0200924c
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #232
	movs	r2, #136
	ldr	r0, [r5, #0]
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200929c
	bl 0x020084ac
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x020080f4
	bl 0x02009334
	ldr	r5, [pc, #44]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #194
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #40
	bl 0x0200924c
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #240
	movs	r2, #240
	ldr	r0, [r5, #0]
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200929c
	bl 0x020084ac
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	mov	ip, r0
	ldr	r0, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #4
	movs	r4, #224
	adds	r1, r0, r3
	mov	lr, r4
	movs	r7, #0
.L_0200059c:
	ldrh	r3, [r0, #0]
	movs	r5, #31
	lsrs	r2, r3, #10
	ldr	r6, [pc, #84]
	ands	r2, r5
	movs	r4, #248
	lsls	r2, r2, #10
	lsls	r4, r4, #7
	adds	r0, #2
	mov	r8, r6
	cmp	r2, r4
	bls.n	.L_020005b6
	adds	r2, r4, #0
.L_020005b6:
	strh	r2, [r1, #0]
	lsrs	r2, r3, #5
	ands	r2, r5
	mov	r6, ip
	muls	r6, r2
	adds	r2, r6, #0
	mov	r6, r8
	ands	r2, r6
	adds	r1, #2
	cmp	r2, r4
	bls.n	.L_020005ce
	adds	r2, r4, #0
.L_020005ce:
	strh	r2, [r1, #0]
	adds	r2, r5, #0
	ands	r2, r3
	mov	r3, ip
	muls	r3, r2
	mov	r6, r8
	adds	r2, r3, #0
	ands	r2, r6
	adds	r1, #2
	cmp	r2, r4
	bls.n	.L_020005e6
	adds	r2, r4, #0
.L_020005e6:
	adds	r7, #1
	strh	r2, [r1, #0]
	adds	r1, #2
	cmp	r7, lr
	bcc.n	.L_0200059c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xfc00
	.2byte 0xffff
	push	{r5, r6, lr}
	movs	r3, #192
.L_02000600:
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x02009254
	movs	r0, #0
	bl 0x02009324
	movs	r5, #8
.L_02000610:
	adds	r0, r5, #0
	bl 0x02009264
	cmp	r0, #0
	beq.n	.L_02000622
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000622:
	adds	r5, #1
.L_02000624:
	cmp	r5, #63
	bls.n	.L_02000610
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r5, [r6, r3]
	movs	r0, #158
	subs	r5, #1
.L_02000636:
	bl 0x02009344
	lsrs	r5, r5, #1
.L_0200063c:
	ldr	r0, [pc, #84]
	lsls	r5, r5, #3
	adds	r3, r5, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r5]
	bl 0x0200922c
	ldr	r5, [pc, #72]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x020092a4
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200926c
	movs	r2, #4
	movs	r1, #0
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x02009294
	movs	r0, #6
	bl 0x0200924c
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x020092cc
	bl 0x0200930c
	bl 0x02009314
	bl 0x0200925c
	pop	{r5, r6, pc}
	.4byte 0x020099ec
	.2byte 0x0240
	.2byte 0x0200
	.global Func_0200069c
	.thumb_func
Func_0200069c:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	movs	r0, #0
	bl 0x0200931c
	ldr	r5, [pc, #248]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #240]
	cmp	r2, r3
	bne.n	.L_020006dc
	bl 0x020088c8
	ldr	r0, [pc, #236]
	ldr	r1, [pc, #236]
	ldr	r2, [pc, #240]
	ldr	r3, [pc, #240]
	bl 0x02008038
	b.n	.L_02000754
.L_020006dc:
	ldr	r3, [pc, #236]
	cmp	r2, r3
	bne.n	.L_02000712
	movs	r0, #9
	bl 0x02009264
	movs	r1, #0
	bl 0x0200923c
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #2
	bne.n	.L_02000754
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200929c
	movs	r0, #141
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009204
.L_02000710:
	b.n	.L_02000754
.L_02000712:
	ldr	r3, [pc, #188]
	cmp	r2, r3
	bne.n	.L_02000766
	movs	r0, #85
	bl 0x020091fc
	cmp	r0, #0
	bne.n	.L_0200072c
	movs	r0, #165
	bl 0x020091fc
	cmp	r0, #0
	beq.n	.L_02000736
.L_0200072c:
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x02009204
	b.n	.L_02000754
.L_02000736:
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x020091fc
	cmp	r0, #0
	bne.n	.L_02000754
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200929c
	ldr	r1, [pc, #132]
	movs	r0, #8
	bl 0x02009274
.L_02000754:
	ldr	r3, [pc, #92]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #108]
	cmp	r2, r3
	beq.n	.L_0200077e
.L_02000766:
	ldr	r3, [pc, #76]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020007b0
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000794
.L_0200077e:
	ldr	r3, [pc, #52]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	b.n	.L_02000796
.L_02000794:
	movs	r3, #3
.L_02000796:
	movs	r0, #128
	lsls	r3, r3, #7
	lsls	r0, r0, #3
	subs	r0, r0, r3
	bl 0x02008580
	bl 0x0200933c
	movs	r0, #1
	bl 0x020092fc
	bl 0x02009314
.L_020007b0:
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00000088
	.4byte 0x0200934c
	.4byte 0x0200935c
	.4byte 0x02009388
	.4byte 0x020093b4
	.4byte 0x0000008a
	.4byte 0x00000089
	.2byte 0x9454
	.2byte 0x0200
	.global Func_020007d8
	.thumb_func
Func_020007d8:
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #8
	adds	r6, r0, #0
	mov	r8, r1
	mov	sl, r2
	adds	r7, r3, #0
	bl 0x02009264
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200083c
	ldr	r0, [sp, #40]
	bl 0x020091fc
	cmp	r0, #0
	beq.n	.L_0200080e
	ldr	r2, [sp, #32]
	lsls	r1, r7, #16
	lsls	r2, r2, #16
	adds	r0, r6, #0
	bl 0x0200929c
.L_0200080e:
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x020092c4
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	mov	r0, r8
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	subs	r3, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	mov	r1, sl
	movs	r2, #3
	movs	r3, #1
	bl 0x02009234
.L_0200083c:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	adds	r6, r3, #0
	bl 0x02009264
	cmp	r0, #0
	beq.n	.L_020008aa
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x020092c4
	ldr	r0, [sp, #36]
	bl 0x020091fc
	cmp	r0, #0
	beq.n	.L_0200087e
	ldr	r2, [sp, #28]
	lsls	r1, r6, #16
	lsls	r2, r2, #16
	adds	r0, r5, #0
	bl 0x0200929c
.L_0200087e:
	adds	r0, r5, #0
	bl 0x02009264
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	mov	r1, r8
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #8]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	subs	r3, #1
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r7, #0
	movs	r2, #1
	movs	r3, #3
	bl 0x02009234
.L_020008aa:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r1, #1
	bl 0x020092a4
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x020092a4
	pop	{r5, pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #252
	sub	sp, #12
	lsls	r3, r3, #1
	str	r3, [sp, #0]
	movs	r3, #20
	str	r3, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #2
	str	r3, [sp, #8]
	movs	r0, #8
	movs	r1, #27
	movs	r2, #0
	movs	r3, #136
	bl 0x02008848
	movs	r3, #232
	str	r3, [sp, #0]
	movs	r3, #21
	str	r3, [sp, #4]
	movs	r3, #193
	lsls	r3, r3, #1
	adds	r3, #255
	str	r3, [sp, #8]
	movs	r0, #9
	movs	r1, #28
	movs	r2, #0
	movs	r3, #136
	bl 0x02008848
	movs	r3, #214
	lsls	r3, r3, #2
	mov	r8, r3
	movs	r3, #22
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #130
	str	r3, [sp, #8]
	movs	r5, #152
	mov	r3, r8
	movs	r0, #10
	movs	r1, #29
	movs	r2, #0
	str	r5, [sp, #0]
	bl 0x02008848
	movs	r3, #23
	str	r3, [sp, #4]
	movs	r3, #194
	movs	r6, #198
	lsls	r3, r3, #1
	lsls	r6, r6, #2
	adds	r3, #255
	str	r3, [sp, #8]
	movs	r0, #11
	adds	r3, r6, #0
	movs	r1, #30
	movs	r2, #0
	str	r5, [sp, #0]
	bl 0x02008848
	movs	r3, #24
	str	r3, [sp, #4]
	movs	r3, #161
	lsls	r3, r3, #2
	str	r3, [sp, #8]
	adds	r5, #224
	mov	r3, r8
	movs	r0, #12
	movs	r1, #31
	movs	r2, #0
	str	r5, [sp, #0]
	bl 0x02008848
	movs	r3, #25
	str	r3, [sp, #4]
	movs	r3, #195
	lsls	r3, r3, #1
	adds	r3, #255
	str	r3, [sp, #8]
	movs	r0, #13
	adds	r3, r6, #0
	movs	r1, #32
	movs	r2, #0
	str	r5, [sp, #0]
	bl 0x02008848
	movs	r2, #120
	str	r2, [sp, #0]
	movs	r2, #26
	str	r2, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #134
	movs	r3, #206
	str	r2, [sp, #8]
	lsls	r3, r3, #2
	movs	r0, #14
	movs	r1, #33
	movs	r2, #0
	bl 0x020087dc
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #8
	bl 0x020088b4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #9
	bl 0x020088b4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #10
	bl 0x020088b4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #11
	bl 0x020088b4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #12
	bl 0x020088b4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #13
	bl 0x020088b4
.L_020009e8:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #14
	bl 0x020088b4
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	ldr	r3, [pc, #216]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02009264
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x02009264
.L_02000a24:
	adds	r7, r0, #0
	bl 0x02009254
	movs	r0, #0
	bl 0x02009324
	ldr	r3, [sp, #4]
	ldr	r2, [pc, #180]
	lsls	r3, r3, #16
	mov	fp, r3
	ldr	r3, [r6, #8]
	movs	r5, #128
	add	r3, fp
	lsls	r5, r5, #12
	ands	r3, r2
	adds	r1, r3, r5
	ldr	r3, [sp, #0]
	mov	sl, r2
	lsls	r3, r3, #16
	mov	r9, r3
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	add	r3, r9
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #9
	str	r2, [r6, #48]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r5
	mov	r8, r2
	str	r2, [r6, #52]
	ldr	r2, [r6, #12]
	bl 0x0200921c
	adds	r0, r6, #0
	movs	r1, #27
	bl 0x02009214
	ldr	r3, [r7, #8]
	mov	r2, sl
	add	r3, fp
	ands	r3, r2
	adds	r1, r3, r5
	ldr	r3, [r7, #16]
	adds	r0, r7, #0
	add	r3, r9
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #9
	str	r2, [r7, #48]
	mov	r2, r8
	adds	r3, r3, r5
	str	r2, [r7, #52]
	ldr	r2, [r7, #12]
	bl 0x0200921c
	ldr	r3, [sp, #4]
	cmp	r3, #0
	blt.n	.L_02000aa2
	ldr	r2, [sp, #0]
	cmp	r2, #0
	bge.n	.L_02000aac
.L_02000aa2:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x02009214
	b.n	.L_02000ab4
.L_02000aac:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x02009214
.L_02000ab4:
	movs	r0, #226
	bl 0x02009344
	adds	r0, r6, #0
	bl 0x02009224
	movs	r1, #2
	adds	r0, r7, #0
	bl 0x02009214
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x02009344
	bl 0x0200925c
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb560
	movs	r0, #8
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #8
	bl 0x02009264
	movs	r1, #32
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #8
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #8
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #160
	lsls	r0, r0, #2
	bl 0x0200920c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #8
	bl 0x02009264
	movs	r1, #32
	negs	r1, r1
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #8
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #8
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #160
	lsls	r0, r0, #2
	bl 0x02009204
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #9
	bl 0x02009264
	movs	r1, #112
	ldr	r5, [r0, #16]
	movs	r2, #0
	movs	r0, #9
	bl 0x020089f8
	movs	r1, #96
	movs	r2, #0
	movs	r0, #9
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #9
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200920c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #9
	bl 0x02009264
	movs	r1, #112
	ldr	r5, [r0, #16]
	negs	r1, r1
	movs	r0, #9
	movs	r2, #0
	bl 0x020089f8
	movs	r1, #96
	negs	r1, r1
	movs	r2, #0
	movs	r0, #9
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #9
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009204
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #10
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #10
	bl 0x02009264
	movs	r1, #80
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #10
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #10
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #130
	bl 0x02009204
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #10
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #10
	bl 0x02009264
	movs	r1, #80
	negs	r1, r1
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #10
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #10
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
.L_02000d20:
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #130
	bl 0x0200920c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #11
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #11
	bl 0x02009264
	movs	r1, #80
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #11
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #11
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200920c
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #11
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #11
	bl 0x02009264
	movs	r1, #80
	negs	r1, r1
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #11
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #11
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200920c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #12
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #12
	bl 0x02009264
	movs	r1, #80
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #12
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #12
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #161
	lsls	r0, r0, #2
	bl 0x02009204
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #12
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #12
	bl 0x02009264
	movs	r1, #80
	negs	r1, r1
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #12
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #12
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #161
	lsls	r0, r0, #2
	bl 0x0200920c
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #13
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #13
	bl 0x02009264
	movs	r1, #80
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #13
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #13
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #195
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200920c
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #13
	sub	sp, #8
	bl 0x02009264
	ldr	r6, [r0, #8]
	movs	r0, #13
	bl 0x02009264
	movs	r1, #80
	negs	r1, r1
	movs	r2, #0
	ldr	r5, [r0, #16]
	movs	r0, #13
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #13
	bl 0x02009264
	ldr	r3, [r0, #8]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #0]
.L_02000f94:
	str	r5, [sp, #4]
	bl 0x02009234
	movs	r0, #195
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009204
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #14
	sub	sp, #8
	bl 0x02009264
	ldr	r5, [r0, #8]
	movs	r0, #14
	bl 0x02009264
	ldr	r0, [r0, #16]
	movs	r6, #112
	mov	r8, r0
	negs	r6, r6
	mov	r3, r8
	asrs	r3, r3, #20
	adds	r2, r6, #0
	movs	r0, #14
	movs	r1, #0
	mov	r8, r3
	bl 0x020089f8
	adds	r2, r6, #0
	movs	r0, #14
	movs	r1, #0
	bl 0x020089f8
	movs	r2, #128
	negs	r2, r2
	movs	r1, #0
	movs	r0, #14
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #14
	bl 0x02009264
	ldr	r3, [r0, #16]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r3, r3, #20
	str	r3, [sp, #4]
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x02009234
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	movs	r0, #0
	str	r5, [sp, #0]
	bl 0x02009234
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #134
	bl 0x02009204
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #14
	sub	sp, #8
	bl 0x02009264
	ldr	r5, [r0, #8]
	movs	r0, #14
	bl 0x02009264
	movs	r1, #0
	ldr	r6, [r0, #16]
	movs	r2, #112
	movs	r0, #14
	bl 0x020089f8
	movs	r0, #14
	movs	r1, #0
	movs	r2, #112
	bl 0x020089f8
	movs	r1, #0
	movs	r2, #128
	movs	r0, #14
	bl 0x020089f8
	movs	r0, #2
	bl 0x020091dc
	movs	r0, #14
	bl 0x02009264
	ldr	r3, [r0, #16]
	asrs	r5, r5, #20
	subs	r5, #1
	asrs	r6, r6, #20
	asrs	r3, r3, #20
	str	r3, [sp, #4]
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x02009234
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	movs	r0, #0
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x02009234
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #134
	bl 0x0200920c
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x02009254
	movs	r0, #0
	bl 0x02009324
	movs	r1, #2
	movs	r0, #8
	bl 0x020092ac
	movs	r0, #20
	bl 0x0200924c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x020092f4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x020092ec
	movs	r0, #60
	bl 0x020092fc
	movs	r0, #60
	bl 0x0200924c
	movs	r2, #2
	ldr	r0, [pc, #100]
	movs	r1, #0
	bl 0x02009244
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x020092ec
	movs	r0, #60
	bl 0x020092fc
	movs	r0, #60
	bl 0x0200924c
	ldr	r5, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02009264
	cmp	r0, #0
	beq.n	.L_02001128
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x02009284
.L_02001128:
	movs	r0, #20
	bl 0x0200924c
	ldr	r0, [pc, #40]
	movs	r1, #2
.L_02001132:
	bl 0x020092e4
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #103
	movs	r1, #2
	bl 0x020092d4
	bl 0x0200925c
.L_0200114e:
	pop	{r5, pc}
	.4byte 0x000030aa
	.4byte 0x02000240
	.2byte 0x008a
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x020091fc
	cmp	r0, #0
	bne.n	.L_020011d2
	movs	r0, #9
.L_02001170:
	bl 0x02009264
.L_02001174:
	ldr	r5, [pc, #96]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	bl 0x02009264
	mov	r8, r0
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x02009204
.L_0200118e:
	movs	r1, #148
	movs	r2, #160
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200929c
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r6, #48]
	movs	r3, #240
	lsls	r3, r3, #12
	str	r3, [r6, #40]
.L_020011a8:
	mov	r3, r8
	ldr	r2, [r3, #16]
	movs	r1, #172
	asrs	r2, r2, #19
	lsls	r2, r2, #3
	adds	r2, #24
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200928c
	movs	r0, #8
	bl 0x0200927c
.L_020011c2:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #48]
	movs	r1, #9
	ldr	r0, [r5, #0]
	movs	r2, #0
	bl 0x020092b4
.L_020011d2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.section .rodata,"a",%progbits
	.4byte 0x000a0009
	.4byte 0x000c000b
	.4byte 0x000e000d
	.4byte 0xffff000f
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0xffff0016
	.4byte 0x001b001e
	.4byte 0x0016001f
	.4byte 0x001f0015
	.4byte 0x00120014
	.4byte 0x000d001f
	.4byte 0x001b000b
	.4byte 0x0008000b
	.4byte 0x00080018
	.4byte 0x00150005
	.4byte 0x001f001f
	.4byte 0xffff001f
	.4byte 0x0009000e
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x000e000d
	.4byte 0x000a0009
	.4byte 0x000c000b
	.4byte 0x000d000c
	.4byte 0x0009000e
	.4byte 0x000b000a
	.4byte 0x000c000b
	.4byte 0x000e000d
	.4byte 0x000a0009
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x0009000e
	.4byte 0x000a0009
	.4byte 0x000c000b
	.4byte 0x000e000d
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00014ccc
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00014ccc
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffc0000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000002e
	.4byte 0x020083c9
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x020083d5
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
	.4byte 0x00000088
	.4byte 0x10105087
	.4byte 0xffffffff
	.4byte 0x10203088
	.4byte 0xffffffff
	.4byte 0x10302088
	.4byte 0xffffffff
	.4byte 0x10405088
	.4byte 0xffffffff
	.4byte 0x10504088
	.4byte 0xffffffff
	.4byte 0x10607088
	.4byte 0xffffffff
	.4byte 0x10706088
	.4byte 0xffffffff
	.4byte 0x10801089
	.4byte 0xffffffff
	.4byte 0x1090a088
	.4byte 0xffffffff
	.4byte 0x10a09088
	.4byte 0xffffffff
	.4byte 0x00000089
	.4byte 0x10108088
	.4byte 0xffffffff
	.4byte 0x10203089
	.4byte 0xffffffff
	.4byte 0x10302089
	.4byte 0xffffffff
	.4byte 0x10405089
	.4byte 0xffffffff
	.4byte 0x10504089
	.4byte 0xffffffff
	.4byte 0x10607089
	.4byte 0xffffffff
	.4byte 0x10706089
	.4byte 0xffffffff
	.4byte 0x10809089
	.4byte 0xffffffff
	.4byte 0x10908089
	.4byte 0xffffffff
	.4byte 0x10a0108a
	.4byte 0xffffffff
	.4byte 0x0000008a
	.4byte 0x1010a089
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x09cf00aa
	.4byte 0x020093fc
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte 0x02009418
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0x003c00f3
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x005500f4
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
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
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000602
	.4byte 0xffff0014
	.4byte 0x02008aed
	.4byte 0x00008602
	.4byte 0xffff0014
	.4byte 0x02008b4d
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte 0x02008bb1
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x02008c1d
	.4byte 0x00000602
	.4byte 0xffff0016
	.4byte 0x02008c8d
	.4byte 0x00008602
	.4byte 0xffff0016
	.4byte 0x02008cf1
	.4byte 0x00000602
	.4byte 0xffff0017
	.4byte 0x02008d55
	.4byte 0x00008602
	.4byte 0xffff0017
	.4byte 0x02008db9
	.4byte 0x00000602
	.4byte 0xffff0018
	.4byte 0x02008e1d
	.4byte 0x00008602
	.4byte 0xffff0018
	.4byte 0x02008e7d
	.4byte 0x00000602
	.4byte 0xffff0019
	.4byte 0x02008ee1
	.4byte 0x00008602
	.4byte 0xffff0019
	.4byte 0x02008f45
	.4byte 0x0000c602
	.4byte 0xffff001a
	.4byte 0x02008fa9
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte 0x02009035
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020089a5
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020089b1
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte 0x020089bd
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte 0x020089c9
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte 0x020089d5
	.4byte 0x00000002
	.4byte 0xffff0019
	.4byte 0x020089e1
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte 0x020089ed
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020083b1
	.4byte 0x00009c05
	.4byte 0xffff0032
	.4byte 0x02008509
	.4byte 0x00009c05
	.4byte 0xffff0033
	.4byte 0x02008545
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x020085fd
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x020085fd
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x020085fd
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x020085fd
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x020085fd
	.4byte 0x00008715
	.4byte 0x02300008
	.4byte 0x0200915d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x09cf000a
	.4byte 0x020090ad
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00040048
	.4byte 0x00020002
	.4byte 0x00490002
	.4byte 0x00020004
	.4byte 0x00020002
	.4byte 0x0000ffff
	.4byte 0x020099d4
	.4byte 0x00040048
	.4byte 0x020099d4
	.4byte 0x00040054
	.4byte 0x020099d4
	.4byte 0x00040060
	.4byte 0x020099d4
	.4byte 0x0004006b
	.4byte 0x020099d4
	.4byte 0x00040076
