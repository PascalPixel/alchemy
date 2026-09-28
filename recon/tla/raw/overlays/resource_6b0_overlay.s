.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02009e65, 0x02008805, 0x02008811, 0x02008819, 0x02009bed, 0x0200880d, 0x0200a6fd
	overlay_veneer \EntryTarget
	.endr
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
	bl 0x0200b458
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
	.4byte 0x0200c444
	.4byte 0x0200c448
	.4byte 0x0200c44c
	.4byte 0x0200c438
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
	bl 0x0200b258
	b.n	.L_02000154
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0x0200c454
	.4byte 0x0200c450
	.4byte 0x0200c444
	.4byte 0x0200c440
	.4byte 0x0200c43c
	.4byte 0x0200c434
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
	bl 0x0200b260
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
	bl 0x0200b258
	b.n	.L_020001d0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0200c454
	.4byte 0x0200c450
	.4byte 0x0200c444
	.4byte 0x0200c440
	.4byte 0x0200c43c
	.4byte 0x0200c434
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
	.4byte 0x0200c43c
	.4byte 0x0200c440
	.4byte 0x0200c44c
	.4byte 0x0200c450
	.4byte 0x0200c438
	.4byte 0x0200c454
	.4byte 0x0200c444
	.4byte 0x0200c434
	.2byte 0xc448
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
	.4byte 0x0200c44c
	.4byte 0x0200c434
	.4byte 0x0200c454
	.2byte 0xc450
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
	bl 0x0200b458
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200b458
	ldr	r2, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, r8
.L_0200035c:
	str	r3, [r2, r1]
	bl 0x0200b470
	bl 0x0200b480
	bl 0x02008178
	bl 0x0200b4a8
	movs	r0, #40
	bl 0x0200b250
	bl 0x02008158
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x0200b458
	movs	r0, #16
	bl 0x0200b468
	movs	r0, #16
	bl 0x0200b250
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
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_020003f4
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020003f4
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
.L_020003f4:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
.L_02000400:
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x0200b2f0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000442
	movs	r1, #0
	bl 0x020083b0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200b358
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200b3f0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200b360
	adds	r0, r5, #0
	b.n	.L_02000444
.L_02000442:
	movs	r0, #0
.L_02000444:
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
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r5, r0, #0
	movs	r0, #0
	mov	r8, r2
	str	r3, [sp, #0]
	mov	sl, r1
	ldr	r7, [sp, #48]
	bl 0x0200b3a8
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r2, sl
	ands	r3, r2
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_020004be
	cmp	r7, #0
	beq.n	.L_020004be
	movs	r3, #24
	ldrsh	r0, [r7, r3]
	adds	r2, r6, #0
	b.n	.L_020004c4
.L_020004be:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
.L_020004c4:
	adds	r1, r5, #0
	mov	r3, r8
	bl 0x0200b2f0
	adds	r6, r0, #0
.L_020004ce:
	cmp	r6, #0
	bne.n	.L_020004d4
	b.n	.L_02000638
.L_020004d4:
	ldr	r1, [r6, #80]
	movs	r5, #15
	mov	r8, r1
	mov	r1, sl
	adds	r1, #1
	ands	r1, r5
.L_020004e0:
	adds	r0, r6, #0
	bl 0x0200b2e0
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200b2e8
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200b358
	ldr	r3, [pc, #320]
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
	bl 0x020083b0
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
	beq.n	.L_02000638
	cmp	r7, #0
	beq.n	.L_02000638
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000556
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	.2byte 0xf002
	.2byte 0xff4d
.L_02000556:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200058e
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
	bl 0x020083b0
.L_0200058e:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020005a2
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020005a2:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020005e8
	ldr	r3, [pc, #152]
	mov	r1, fp
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020005d0
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200b248
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020005e2
.L_020005d0:
	ldr	r2, [pc, #124]
	adds	r0, r3, r2
	bl 0x0200b248
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #112]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020005e2:
	bl 0x0200b248
	str	r0, [r6, #52]
.L_020005e8:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000604
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200b2e0
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200b2e8
.L_02000604:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000616
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #18]
.L_02000616:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000628
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000628:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000638
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000638:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b840
	.4byte 0x02008449
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r1, #0
	bl 0x0200b358
	movs	r0, #0
	pop	{pc}
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r2, r5, #0
	adds	r1, #100
	adds	r2, #102
	ldrh	r2, [r2, #0]
	ldrh	r3, [r1, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [r5, #68]
	ldr	r0, [r5, #76]
	str	r3, [r5, #8]
	ldr	r3, [r5, #72]
	str	r3, [r5, #12]
	movs	r3, #152
	lsls	r3, r3, #16
	str	r3, [r5, #16]
	movs	r2, #0
	ldrsh	r1, [r1, r2]
	adds	r2, r5, #0
	adds	r2, #8
	bl 0x0200b288
	ldr	r3, [r5, #76]
	ldr	r2, [pc, #12]
	adds	r3, r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	str	r3, [r5, #76]
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	adds	r6, r0, #0
	movs	r0, #168
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #2
	bl 0x0200b2f0
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r6, #98
	ldrb	r1, [r6, #0]
	bl 0x0200b3f0
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x020083b0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200b2e0
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200b358
	adds	r0, r5, #0
	ldr	r1, [pc, #16]
	bl 0x0200b2e8
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200b360
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xb674
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	ldr	r2, [r7, #76]
	ldr	r3, [r7, #48]
	movs	r0, #0
	cmp	r2, r3
	blt.n	.L_0200078c
	movs	r3, #102
	adds	r3, r3, r7
	ldrh	r2, [r3, #0]
	adds	r5, r7, #0
	adds	r5, #100
	mov	r8, r3
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #16
	asrs	r2, r2, #17
	adds	r3, r3, r2
	strh	r3, [r5, #0]
	ldr	r3, [r7, #68]
	adds	r6, r7, #0
	str	r3, [r7, #8]
	ldr	r3, [r7, #52]
	adds	r6, #8
	str	r3, [r7, #12]
	ldr	r3, [r7, #72]
	adds	r2, r6, #0
	str	r3, [r7, #16]
	ldr	r0, [r7, #76]
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	bl 0x0200b288
	adds	r0, r7, #0
	bl 0x020086a4
	ldr	r2, [r7, #48]
	ldr	r3, [r7, #76]
	asrs	r2, r2, #1
	subs	r3, r3, r2
	str	r3, [r7, #76]
	mov	r3, r8
	ldrh	r2, [r3, #0]
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #16
	asrs	r2, r2, #17
	adds	r3, r3, r2
	strh	r3, [r5, #0]
	ldr	r3, [r7, #68]
	adds	r2, r6, #0
	str	r3, [r6, #0]
	ldr	r3, [r7, #52]
	ldr	r0, [r7, #76]
	str	r3, [r7, #12]
	ldr	r3, [r7, #72]
	str	r3, [r7, #16]
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	bl 0x0200b288
	adds	r0, r7, #0
	bl 0x020086a4
	ldr	r2, [r7, #48]
	ldr	r3, [r7, #76]
	asrs	r2, r2, #1
	subs	r3, r3, r2
	str	r3, [r7, #76]
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #52]
	movs	r0, #1
	subs	r3, r3, r2
	str	r3, [r7, #52]
.L_0200078c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #56
	add	r4, sp, #16
	movs	r3, #2
	str	r3, [r4, #0]
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	ldr	r0, [r5, #8]
	str	r3, [r4, #4]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	ldr	r1, [r5, #12]
	ldr	r2, [r5, #16]
	movs	r3, #0
	str	r4, [sp, #12]
	bl 0x02008480
.L_020007c6:
	ldr	r2, [r5, #68]
	ldr	r3, [r5, #72]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	adds	r2, #64
	adds	r1, r3, #0
	movs	r0, #1
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, #26
	adds	r0, r2, #0
	bl 0x0200b328
	movs	r0, #0
	add	sp, #56
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #60
.L_020007ee:
	cmp	r5, #0
	beq.n	.L_02000800
	movs	r0, #1
	bl 0x0200b250
	ldr	r3, [r6, #40]
	subs	r5, #1
	cmp	r3, #0
	bne.n	.L_020007ee
.L_02000800:
	pop	{r5, r6, pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb908
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb938
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000830
	ldr	r0, [pc, #52]
	b.n	.L_0200085a
.L_02000830:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_0200083a
	ldr	r0, [pc, #52]
	b.n	.L_0200085a
.L_0200083a:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000844
	ldr	r0, [pc, #48]
	b.n	.L_0200085a
.L_02000844:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_0200084e
	ldr	r0, [pc, #48]
	b.n	.L_0200085a
.L_0200084e:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000858
	ldr	r0, [pc, #44]
	b.n	.L_0200085a
.L_02000858:
	ldr	r0, [pc, #44]
.L_0200085a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x0200bb50
	.4byte 0x00000130
	.4byte 0x0200bb68
	.4byte 0x00000131
	.4byte 0x0200bbc8
	.4byte 0x00000132
	.4byte 0x0200bc88
	.4byte 0x00000133
	.4byte 0x0200bce8
	.2byte 0xbb38
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	lsls	r2, r2, #16
	sub	sp, #8
	adds	r6, r0, #0
	asrs	r2, r2, #16
	movs	r0, #168
	str	r2, [sp, #4]
	mov	r9, r1
	ldr	r2, [r6, #4]
	ldr	r1, [r6, #0]
	ldr	r3, [r6, #8]
	lsls	r0, r0, #2
	bl 0x0200b2f0
	mov	r2, r9
	ldr	r3, [r2, #0]
	ldr	r1, [r6, #0]
	adds	r7, r0, #0
	subs	r1, r1, r3
	asrs	r3, r1, #16
	mov	sl, r3
	ldr	r0, [r6, #8]
	ldr	r3, [r2, #8]
	subs	r0, r0, r3
	asrs	r2, r0, #16
	mov	r8, r2
	bl 0x0200b270
	adds	r3, r7, #0
	adds	r3, #100
	ldr	r1, [pc, #60]
.L_020008d6:
	adds	r2, r3, #0
	str	r3, [sp, #0]
	strh	r0, [r2, #0]
	mov	fp, r1
	bl 0x0200b268
	adds	r5, r0, #0
	bl 0x0200b268
	movs	r2, #2
	ands	r2, r5
	movs	r3, #15
	subs	r2, #1
	ands	r3, r0
	muls	r3, r2
	add	r1, sp, #4
	adds	r2, r7, #0
	ldrb	r1, [r1, #0]
	lsls	r3, r3, #8
	adds	r2, #102
	strh	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #98
	strb	r1, [r3, #0]
	mov	r2, fp
	subs	r3, #13
	strb	r2, [r3, #0]
	mov	r1, r9
	ldr	r3, [r1, #0]
	b.n	.L_02000918
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02000918:
	mov	r2, sl
	str	r3, [r7, #68]
	mov	r0, sl
	muls	r0, r2
	ldr	r3, [r1, #8]
	mov	r1, r8
	str	r3, [r7, #72]
	mov	r3, r8
	muls	r3, r1
	adds	r0, r0, r3
	ldr	r3, [pc, #128]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0400
	str	r0, [r7, #76]
	cmp	r0, #0
	bge.n	.L_0200093c
	adds	r0, #15
.L_0200093c:
	asrs	r3, r0, #4
	str	r3, [r7, #48]
	mov	r1, r9
	ldr	r3, [r6, #4]
	str	r3, [r7, #52]
	ldr	r2, [r6, #4]
	ldr	r3, [r1, #4]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_02000952
	adds	r0, #15
.L_02000952:
	asrs	r3, r0, #4
	str	r3, [r7, #20]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200b360
	ldr	r2, [sp, #4]
	adds	r0, r7, #0
	lsls	r1, r2, #16
	lsrs	r1, r1, #16
	.2byte 0xf002
	.2byte 0xfd43
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x020083b0
	adds	r0, r7, #0
	movs	r1, #7
	.2byte 0xf002
	.2byte 0xfcb3
	adds	r0, r7, #0
	movs	r1, #0
	.2byte 0xf002
	.2byte 0xfceb
	ldr	r2, [sp, #0]
	ldr	r0, [r7, #76]
	movs	r3, #0
	ldrsh	r1, [r2, r3]
	adds	r2, r7, #0
	adds	r2, #8
	.2byte 0xf002
	.2byte 0xfc7b
	ldr	r1, [pc, #32]
	adds	r0, r7, #0
	.2byte 0xf002
	.2byte 0xfca7
	adds	r0, r7, #0
	bl 0x020086f8
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x030002d4
	.2byte 0xb684
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	asrs	r2, r2, #16
	asrs	r3, r3, #16
	mov	r9, r0
	mov	sl, r1
	mov	r8, r2
	mov	fp, r3
	movs	r7, #0
.L_020009d8:
	movs	r0, #168
	movs	r3, #152
	lsls	r3, r3, #16
	mov	r1, r9
	mov	r2, sl
	lsls	r0, r0, #2
	.2byte 0xf002
	.2byte 0xfc84
	adds	r5, r0, #0
	lsls	r6, r7, #12
	adds	r2, r5, #0
	movs	r0, #128
	adds	r2, #8
	lsls	r0, r0, #14
	adds	r1, r6, #0
	.2byte 0xf002
	.2byte 0xfc47
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	mov	r1, fp
	adds	r3, #2
	strh	r1, [r3, #0]
	subs	r3, #4
	strb	r7, [r3, #0]
	mov	r1, r8
	adds	r3, #1
	strb	r1, [r3, #0]
	mov	r3, r9
	str	r3, [r5, #68]
	movs	r3, #128
	ldr	r2, [pc, #60]
	lsls	r3, r3, #14
	str	r3, [r5, #76]
	adds	r3, r5, #0
	adds	r3, #85
	mov	r1, sl
	str	r1, [r5, #72]
	adds	r0, r5, #0
	strb	r2, [r3, #0]
	movs	r1, #1
	.2byte 0xf002
	.2byte 0xfc99
	mov	r3, r8
	lsls	r1, r3, #16
	adds	r0, r5, #0
	lsrs	r1, r1, #16
	bl 0x0200b3f0
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x020083b0
	adds	r0, r5, #0
	movs	r1, #7
	.2byte 0xf002
	.2byte 0xfc4b
	adds	r0, r5, #0
	movs	r1, #0
	.2byte 0xf002
	.2byte 0xfc83
	b.n	.L_02000a58
	.2byte 0x0000
	.2byte 0x0000
.L_02000a58:
	adds	r7, #2
	adds	r0, r5, #0
	ldr	r1, [pc, #20]
	.2byte 0xf002
	.2byte 0xfc43
	cmp	r7, #16
	bne.n	.L_020009d8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r2, r7, lr}
	lsls	r0, r0, #8
	push	{r5, r6, lr}
	adds	r3, r1, #0
	adds	r3, #3
.L_02000a7e:
	adds	r4, r0, #0
	lsls	r3, r3, #16
	lsls	r4, r4, #16
	lsls	r5, r2, #16
	movs	r0, #168
	adds	r2, r3, #0
	movs	r3, #152
	adds	r1, r4, #0
	lsls	r3, r3, #16
	lsls	r0, r0, #2
	.2byte 0xf002
	.2byte 0xfc2d
	asrs	r5, r5, #16
	adds	r6, r0, #0
	adds	r2, r6, #0
	lsls	r5, r5, #16
	adds	r2, #85
	movs	r3, #4
	lsrs	r5, r5, #16
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	.2byte 0xf002
	.2byte 0xfca2
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x020083b0
	adds	r0, r6, #0
	movs	r1, #7
	.2byte 0xf002
	.2byte 0xfc12
	adds	r0, r6, #0
	movs	r1, #0
	.2byte 0xf002
	.2byte 0xfc4a
	pop	{r5, r6, pc}
	.2byte 0x0000
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
	sub	sp, #108
	str	r3, [sp, #36]
	ldr	r3, [pc, #228]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #106
	adds	r3, #82
	ldrh	r3, [r3, #0]
	add	r1, sp
	mov	r9, r1
	mov	r2, r9
	strh	r3, [r2, #0]
	ldr	r4, [sp, #36]
	movs	r3, #0
	str	r3, [sp, #24]
	str	r3, [sp, #20]
	movs	r5, #170
	lsls	r5, r5, #1
	adds	r3, r4, r5
	ldrh	r2, [r3, #0]
	mov	r8, r0
	adds	r3, r2, #0
	adds	r3, #193
	lsls	r2, r2, #16
	asrs	r0, r2, #16
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r1, r0, #0
	subs	r1, #50
	mov	sl, r3
	movs	r3, #1
	ands	r3, r1
	movs	r2, #204
	lsls	r3, r3, #8
	lsls	r2, r2, #1
	adds	r2, r3, r2
	str	r2, [sp, #32]
	movs	r2, #2
	ands	r2, r1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #5
	movs	r6, #0
	adds	r3, #168
	mov	fp, r6
	str	r6, [sp, #16]
	str	r3, [sp, #28]
	cmp	r0, #51
	beq.n	.L_02000b70
	cmp	r0, #51
	bgt.n	.L_02000b54
	cmp	r0, #50
	beq.n	.L_02000b5e
	b.n	.L_02000bcc
.L_02000b54:
	cmp	r0, #52
	beq.n	.L_02000b86
	cmp	r0, #53
	beq.n	.L_02000b9e
	b.n	.L_02000bcc
.L_02000b5e:
	movs	r0, #160
	movs	r3, #130
	lsls	r0, r0, #4
	movs	r6, #48
	lsls	r3, r3, #2
	adds	r0, #135
	str	r3, [sp, #24]
	str	r6, [sp, #20]
	b.n	.L_02000bb4
.L_02000b70:
	movs	r0, #48
	str	r0, [sp, #20]
	movs	r0, #160
	movs	r5, #138
	lsls	r0, r0, #4
	movs	r4, #3
	lsls	r5, r5, #2
	adds	r0, #136
	str	r4, [sp, #16]
	movs	r6, #68
	b.n	.L_02000bb2
.L_02000b86:
	movs	r0, #160
	movs	r2, #252
	lsls	r0, r0, #4
	movs	r1, #10
	lsls	r2, r2, #1
	movs	r3, #40
	adds	r0, #137
	str	r1, [sp, #16]
	movs	r6, #88
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	b.n	.L_02000bb4
.L_02000b9e:
	movs	r0, #40
	str	r0, [sp, #20]
	movs	r0, #160
	movs	r5, #142
	lsls	r0, r0, #4
	movs	r4, #2
	lsls	r5, r5, #2
	adds	r0, #138
	str	r4, [sp, #16]
	movs	r6, #108
.L_02000bb2:
	str	r5, [sp, #24]
.L_02000bb4:
	.2byte 0xf002
	.2byte 0xfb80
	cmp	r0, #0
	beq.n	.L_02000bcc
	movs	r0, #128
	lsls	r0, r0, #2
	.2byte 0xf002
	.2byte 0xfb7e
	b.n	.L_02001078
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
.L_02000bcc:
	lsls	r5, r6, #16
	lsrs	r7, r5, #16
	adds	r3, r7, #0
	adds	r3, #18
	cmp	r7, r3
	beq.n	.L_02000bfe
.L_02000bd8:
	adds	r0, r7, #0
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02000bf4
	subs	r3, r7, r6
	mov	r1, fp
	add	r2, sp, #84
	strb	r3, [r2, r1]
	mov	r3, fp
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	fp, r3
.L_02000bf4:
	lsrs	r3, r5, #16
	adds	r7, #1
	adds	r3, #18
	cmp	r7, r3
	bne.n	.L_02000bd8
.L_02000bfe:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_02000c06
	b.n	.L_02001078
.L_02000c06:
	.2byte 0xf002
	.2byte 0xfbc7
	movs	r0, #0
	.2byte 0xf002
	.2byte 0xfc3c
	ldr	r5, [pc, #256]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #204
	movs	r2, #204
	adds	r5, r5, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r2, #102
	adds	r1, #204
	bl 0x0200b3b0
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	.2byte 0xf002
	.2byte 0xfbf3
	ldr	r0, [r5, #0]
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #28]
	.2byte 0xf002
	.2byte 0xfbbe
.L_02000c44:
	ldr	r1, [sp, #28]
	ldr	r4, [sp, #32]
	lsls	r2, r1, #16
	movs	r1, #1
	lsls	r0, r4, #16
	movs	r3, #1
	negs	r1, r1
	.2byte 0xf002
	.2byte 0xfbe9
	ldr	r0, [r5, #0]
	.2byte 0xf002
	.2byte 0xfbbe
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	.2byte 0xf002
	.2byte 0xfbcd
	ldr	r0, [r5, #0]
	movs	r1, #28
	.2byte 0xf002
	.2byte 0xfbbd
	mov	r2, sl
	mov	r3, r8
	lsls	r0, r2, #16
	ldr	r2, [r3, #12]
	movs	r4, #128
	lsls	r4, r4, #12
	adds	r2, r2, r4
	ldr	r1, [r3, #8]
	lsrs	r0, r0, #16
	ldr	r3, [r3, #16]
	.2byte 0xf002
	.2byte 0xfb35
	mov	sl, r0
	mov	r2, sl
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	mov	r5, sl
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r1, #0
	.2byte 0xf002
	.2byte 0xfb5b
	ldr	r1, [pc, #116]
	mov	r0, sl
	.2byte 0xf002
	.2byte 0xfb1f
	mov	r0, sl
	movs	r1, #1
	.2byte 0xf002
	.2byte 0xfb57
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	.2byte 0xf002
	.2byte 0xfbd2
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	.2byte 0xf002
	.2byte 0xfbc8
	movs	r0, #60
	.2byte 0xf002
	.2byte 0xfbcd
	bl 0x0200b338
	mov	r0, r9
	ldrb	r3, [r0, #0]
	mov	r1, r9
	strh	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [pc, #48]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #28]
	asrs	r2, r2, #4
	asrs	r3, r3, #4
	str	r2, [sp, #12]
	adds	r1, r3, #0
	adds	r2, #63
	movs	r0, #3
	movs	r4, #5
	str	r3, [sp, #8]
	str	r0, [sp, #0]
	subs	r3, #3
	adds	r1, #23
	adds	r0, r2, #0
	str	r4, [sp, #4]
	bl 0x0200b328
	mov	r4, r9
	ldrh	r3, [r4, #0]
	movs	r7, #0
	b.n	.L_02000d1c
	.2byte 0x0000
	.4byte 0x00001000
	.2byte 0x0240
	.2byte 0x0200
	push	{r2, r3, r6, r7, lr}
	lsls	r0, r0, #8
.L_02000d1c:
	cmp	r3, #0
	beq.n	.L_02000d44
.L_02000d20:
	ldr	r2, [pc, #28]
	movs	r1, #128
	adds	r3, r7, #0
	lsls	r1, r1, #19
	orrs	r3, r2
	adds	r1, #82
	strh	r3, [r1, #0]
	movs	r0, #8
	mov	r5, r9
	.2byte 0xf002
	.2byte 0xfa8d
	ldrh	r3, [r5, #0]
	adds	r7, #1
	cmp	r7, r3
	bne.n	.L_02000d20
	b.n	.L_02000d44
	.2byte 0x1000
	.2byte 0x0000
.L_02000d44:
	.2byte 0xf002
	.2byte 0xfaf4
	movs	r0, #246
	.2byte 0xf002
	.2byte 0xfbb5
	movs	r0, #48
	.2byte 0xf002
	.2byte 0xfb1e
	mov	r3, fp
	adds	r3, #255
	lsls	r3, r3, #24
	mov	r0, fp
	lsrs	r5, r3, #24
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02000e18
.L_02000d64:
	.2byte 0xf002
	.2byte 0xfa80
	add	r3, sp, #84
	ands	r0, r5
	ldrb	r2, [r3, r0]
	add	r1, sp, #64
	strb	r2, [r1, r7]
	adds	r7, #1
	ldrb	r2, [r3, r5]
	strb	r2, [r3, r0]
	adds	r3, r5, #0
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r7, fp
	bne.n	.L_02000d64
	mov	r1, fp
	movs	r7, #0
	cmp	r1, #0
	beq.n	.L_02000e18
.L_02000d8c:
	.2byte 0xf002
	.2byte 0xfa6c
	mov	r4, r8
	ldr	r3, [r4, #8]
	movs	r2, #31
	ands	r2, r0
	ldr	r5, [pc, #248]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	add	r6, sp, #52
	adds	r3, r3, r5
	str	r3, [r6, #0]
	.2byte 0xf002
	.2byte 0xfa60
	mov	r1, r8
	ldr	r3, [r1, #12]
	movs	r5, #15
	ands	r0, r5
	lsls	r0, r0, #16
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r0
	adds	r3, r3, r2
	str	r3, [r6, #4]
	.2byte 0xf002
	.2byte 0xfa54
	mov	r4, r8
	ldr	r3, [r4, #16]
	ands	r0, r5
	ldr	r5, [pc, #208]
	lsls	r0, r0, #16
	adds	r3, r3, r0
	adds	r3, r3, r5
	str	r3, [r6, #8]
	add	r3, sp, #64
	ldrb	r0, [r3, r7]
	ldr	r4, [pc, #196]
	lsls	r0, r0, #3
	ldr	r2, [r4, r0]
	mov	r5, r8
	ldr	r3, [r5, #8]
	lsls	r2, r2, #16
	add	r1, sp, #40
	adds	r3, r3, r2
.L_02000de4:
	str	r3, [r1, #0]
	adds	r0, #4
	ldr	r3, [r5, #12]
	str	r3, [r1, #4]
	ldr	r2, [r4, r0]
	ldr	r3, [r5, #16]
.L_02000df0:
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r1, #8]
	adds	r0, r6, #0
	ldr	r2, [sp, #16]
	bl 0x0200888c
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_02000e0c
	movs	r0, #195
	.2byte 0xf002
	.2byte 0xfb56
.L_02000e0c:
	movs	r0, #8
	adds	r7, #1
	.2byte 0xf002
	.2byte 0xfabe
	cmp	r7, fp
	bne.n	.L_02000d8c
.L_02000e18:
	mov	r0, fp
.L_02000e1a:
	cmp	r0, #18
	beq.n	.L_02000eea
	movs	r0, #96
	.2byte 0xf002
	.2byte 0xfab6
	ldr	r1, [pc, #120]
	mov	r0, sl
	.2byte 0xf002
	.2byte 0xfa5e
	.2byte 0xf002
	.2byte 0xfa84
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	ldrh	r2, [r3, #0]
	mov	r5, sp
	movs	r3, #255
	ands	r3, r2
	adds	r5, #106
	movs	r0, #128
	strh	r3, [r5, #0]
	lsls	r0, r0, #9
	movs	r1, #1
	.2byte 0xf002
	.2byte 0xfb06
	movs	r0, #60
	.2byte 0xf002
	.2byte 0xfb0b
	ldrh	r7, [r5, #0]
	cmp	r7, #0
	beq.n	.L_02000e72
.L_02000e58:
	ldr	r2, [pc, #52]
	movs	r1, #128
	adds	r3, r7, #0
	lsls	r1, r1, #19
	adds	r1, #82
.L_02000e62:
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #8
	subs	r7, #1
	bl 0x0200b250
	cmp	r7, #0
	bne.n	.L_02000e58
.L_02000e72:
	mov	r1, fp
	movs	r7, #0
	cmp	r1, #0
	beq.n	.L_02000ec8
.L_02000e7a:
	add	r3, sp, #64
	ldrb	r1, [r3, r7]
	ldr	r0, [pc, #28]
	lsls	r1, r1, #3
	ldr	r3, [r0, r1]
	mov	r4, r8
	ldr	r2, [r4, #8]
	adds	r1, #4
	ldr	r1, [r0, r1]
	b.n	.L_02000ea4
	.2byte 0x0000
	.4byte 0x00001000
	.4byte 0xfff00000
	.4byte 0xfff80000
	.4byte 0x0200b6fc
	.2byte 0xb620
	.2byte 0x0200
.L_02000ea4:
	lsls	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [r4, #16]
	lsls	r1, r1, #16
	adds	r3, r3, r1
	asrs	r2, r2, #20
	movs	r1, #1
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	asrs	r3, r3, #20
	adds	r2, #64
	movs	r0, #64
	movs	r1, #0
	adds	r7, #1
	.2byte 0xf002
	.2byte 0xfa32
	cmp	r7, fp
	bne.n	.L_02000e7a
.L_02000ec8:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	movs	r1, #3
	movs	r0, #5
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	adds	r2, #63
	subs	r3, #3
	movs	r0, #64
	movs	r1, #0
	.2byte 0xf002
	.2byte 0xfa24
	.2byte 0xf002
	.2byte 0xfa26
	.2byte 0xf002
	.2byte 0xfacc
	b.n	.L_0200104c
.L_02000eea:
	ldr	r5, [sp, #36]
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r2, #160
	lsls	r2, r2, #4
	adds	r2, #85
	adds	r0, r0, r2
	.2byte 0xf002
	.2byte 0xf9df
	ldr	r1, [pc, #388]
	mov	r0, sl
	.2byte 0xf002
	.2byte 0xf9ef
	movs	r0, #60
	.2byte 0xf002
	.2byte 0xfa40
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	.2byte 0xf002
	.2byte 0xfa82
	movs	r0, #134
	movs	r1, #1
	movs	r2, #240
	negs	r1, r1
	lsls	r2, r2, #15
	movs	r3, #1
	lsls	r0, r0, #18
	.2byte 0xf002
	.2byte 0xfa7d
	.2byte 0xf002
	.2byte 0xfa7f
	movs	r0, #220
	.2byte 0xf002
	.2byte 0xfac0
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #20]
	lsls	r5, r3, #16
	lsls	r6, r4, #16
	movs	r3, #128
	ldr	r2, [sp, #16]
	lsls	r3, r3, #4
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl 0x020089b8
	movs	r0, #10
	.2byte 0xf002
	.2byte 0xfa1e
	movs	r3, #144
	ldr	r2, [sp, #16]
	lsls	r3, r3, #4
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl 0x020089b8
	movs	r0, #10
	.2byte 0xf002
	.2byte 0xfa14
	movs	r3, #160
	ldr	r2, [sp, #16]
	lsls	r3, r3, #4
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl 0x020089b8
	movs	r0, #12
	.2byte 0xf002
	.2byte 0xfa0a
	movs	r0, #16
	.2byte 0xf002
	.2byte 0xf967
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	.2byte 0xf002
	.2byte 0xfa65
	movs	r0, #1
	bl 0x0200b468
	movs	r0, #1
	.2byte 0xf002
	.2byte 0xf95b
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b458
	movs	r0, #32
	bl 0x0200b468
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200b4b8
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #16]
	bl 0x02008a78
	movs	r0, #80
	.2byte 0xf002
	.2byte 0xf9e6
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #135
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_0200104c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #136
	.2byte 0xf002
	.2byte 0xf96e
	cmp	r0, #0
	beq.n	.L_0200104c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #137
	.2byte 0xf002
	.2byte 0xf967
	cmp	r0, #0
	beq.n	.L_0200104c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #138
	.2byte 0xf002
	.2byte 0xf960
	cmp	r0, #0
	beq.n	.L_0200104c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #139
	.2byte 0xf002
	.2byte 0xf95d
	movs	r0, #188
	bl 0x0200b4b8
	movs	r5, #1
	movs	r1, #7
	movs	r2, #33
	movs	r3, #7
	movs	r6, #2
	movs	r0, #52
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	.2byte 0xf002
	.2byte 0xf984
	movs	r0, #5
	.2byte 0xf002
	.2byte 0xf915
	movs	r0, #54
	movs	r1, #7
	movs	r2, #33
	movs	r3, #7
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	.2byte 0xf002
	.2byte 0xf979
	movs	r0, #134
	movs	r1, #136
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r3, #0
	.2byte 0xf002
	.2byte 0xf99d
	movs	r0, #60
	.2byte 0xf002
	.2byte 0xf902
.L_0200104c:
	mov	r0, sl
	.2byte 0xf002
	.2byte 0xf953
	movs	r0, #128
	lsls	r0, r0, #2
	.2byte 0xf002
	.2byte 0xf933
	ldr	r3, [pc, #48]
	movs	r5, #133
	lsls	r5, r5, #2
	adds	r3, r3, r5
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200b418
	bl 0x0200b430
	movs	r0, #60
	.2byte 0xf002
	.2byte 0xf98e
	.2byte 0xf002
	.2byte 0xf994
.L_02001078:
	add	sp, #108
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b620
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
.L_02001096:
	.2byte 0xf002
	.2byte 0xf917
	pop	{pc}
	push	{lr}
	ldr	r3, [pc, #164]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #156]
	cmp	r2, r3
	bne.n	.L_020010b8
	ldr	r0, [pc, #152]
	bl 0x0200b498
	b.n	.L_02001142
.L_020010b8:
	ldr	r3, [pc, #148]
	cmp	r2, r3
	bne.n	.L_020010d6
	ldr	r0, [pc, #148]
	bl 0x0200b498
	movs	r1, #144
	movs	r2, #220
	movs	r0, #2
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #255
	bl 0x0200b378
	b.n	.L_02001142
.L_020010d6:
	ldr	r3, [pc, #128]
	cmp	r2, r3
	bne.n	.L_02001142
	ldr	r0, [pc, #124]
	bl 0x0200b498
	movs	r1, #140
	movs	r2, #172
	movs	r0, #2
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #255
	bl 0x0200b378
	movs	r1, #140
	movs	r2, #188
	movs	r0, #2
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #255
	bl 0x0200b378
	movs	r1, #148
	movs	r2, #180
	movs	r0, #2
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #255
	bl 0x0200b378
	movs	r1, #140
	movs	r2, #180
	movs	r0, #2
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #232
	bl 0x0200b378
	movs	r1, #130
	movs	r2, #158
	movs	r0, #2
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	movs	r3, #255
	bl 0x0200b378
	movs	r1, #130
	movs	r2, #154
	movs	r0, #2
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	movs	r3, #232
	bl 0x0200b378
.L_02001142:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000130
	.4byte 0x0200b6e4
	.4byte 0x00000131
	.4byte 0x0200b6ea
	.4byte 0x00000132
	.2byte 0xb6f6
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
	ldr	r5, [r3, #0]
	sub	sp, #8
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	bl 0x0200b4a0
	ldr	r3, [r5, #8]
	movs	r2, #240
	asrs	r7, r3, #19
	ldr	r3, [r5, #16]
	lsls	r2, r2, #1
	asrs	r3, r3, #19
	mov	r8, r3
	ldr	r3, [pc, #496]
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #492]
	cmp	r2, r3
	bne.n	.L_020011c6
	movs	r1, #144
	movs	r2, #220
	movs	r0, #2
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #232
	bl 0x0200b378
	cmp	r7, #8
	beq.n	.L_020011c6
	movs	r1, #144
	movs	r2, #212
	movs	r0, #2
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	movs	r3, #232
	bl 0x0200b378
.L_020011c6:
	ldr	r3, [pc, #448]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r5, r3, r2
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #444]
	cmp	r2, r3
	bne.n	.L_02001218
	movs	r1, #140
	movs	r2, #172
	movs	r0, #2
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x0200b378
	movs	r1, #140
	movs	r2, #188
	movs	r0, #2
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #232
	bl 0x0200b378
	movs	r1, #148
	movs	r2, #180
	movs	r0, #2
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x0200b378
	movs	r1, #130
	movs	r2, #158
	movs	r0, #2
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	movs	r3, #232
	bl 0x0200b378
.L_02001218:
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	ldr	r3, [pc, #372]
	cmp	r2, r3
	bne.n	.L_02001256
	cmp	r7, #37
	bne.n	.L_0200123c
	mov	r2, r8
	cmp	r2, #31
	bne.n	.L_0200123c
	movs	r0, #168
	lsls	r0, r0, #4
	bl 0x0200b2c0
	movs	r0, #8
	movs	r1, #2
	bl 0x0200b408
.L_0200123c:
	cmp	r7, #55
	beq.n	.L_02001242
	b.n	.L_02001356
.L_02001242:
	mov	r3, r8
	cmp	r3, #95
	beq.n	.L_0200124a
	b.n	.L_02001356
.L_0200124a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #129
	bl 0x0200b2c0
	b.n	.L_02001356
.L_02001256:
	ldr	r3, [pc, #308]
	cmp	r2, r3
	bne.n	.L_02001356
	cmp	r7, #7
	bne.n	.L_02001270
	mov	r1, r8
	cmp	r1, #53
	bne.n	.L_02001270
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x0200b2c0
.L_02001270:
	cmp	r7, #67
	bne.n	.L_020012f2
	mov	r2, r8
	cmp	r2, #45
	bne.n	.L_020012f2
	movs	r3, #22
	movs	r5, #1
	movs	r0, #50
	movs	r1, #22
	movs	r2, #33
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b328
	movs	r1, #0
	movs	r2, #0
	movs	r0, #14
	bl 0x0200b3e0
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200b4b8
	movs	r0, #30
	bl 0x0200b390
	movs	r0, #188
	bl 0x0200b4b8
	movs	r6, #2
	movs	r1, #20
	movs	r2, #31
	movs	r3, #20
	movs	r0, #50
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b328
	movs	r0, #5
	bl 0x0200b390
	movs	r1, #20
	movs	r2, #31
	movs	r3, #20
	movs	r0, #51
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b328
	movs	r0, #5
	bl 0x0200b390
	movs	r0, #248
	movs	r1, #168
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x0200b380
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #131
	bl 0x0200b2c0
.L_020012f2:
	cmp	r7, #93
	bne.n	.L_02001306
	mov	r3, r8
	cmp	r3, #57
	bne.n	.L_02001306
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #132
	bl 0x0200b2c0
.L_02001306:
	cmp	r7, #91
	bne.n	.L_02001356
	mov	r1, r8
	cmp	r1, #59
	bne.n	.L_02001356
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #132
	bl 0x0200b2b8
	cmp	r0, #0
	bne.n	.L_0200134c
	movs	r0, #12
	bl 0x0200b3a8
	movs	r2, #10
	ldrsh	r5, [r0, r2]
	movs	r0, #12
	bl 0x0200b3a8
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	lsls	r5, r5, #16
	lsls	r2, r2, #16
	movs	r0, #13
	adds	r1, r5, #0
	bl 0x0200b3e0
	movs	r1, #182
	movs	r2, #236
	movs	r0, #12
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200b3e0
.L_0200134c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #133
	bl 0x0200b2c0
.L_02001356:
	ldr	r3, [pc, #48]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_0200137c
	cmp	r7, #33
	bne.n	.L_0200137c
	mov	r2, r8
	cmp	r2, #45
	bne.n	.L_0200137c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #134
	bl 0x0200b2c0
.L_0200137c:
	bl 0x0200b3a0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00000131
	.4byte 0x00000132
	.2byte 0x0130
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #368]
	movs	r3, #192
	movs	r1, #133
	lsls	r1, r1, #2
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200b3a8
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #340]
	adds	r6, r0, #0
	ldr	r1, [r6, #8]
	ldr	r0, [r6, #16]
	cmp	r2, r3
	bne.n	.L_020013dc
	asrs	r3, r0, #20
	asrs	r2, r1, #20
	movs	r1, #1
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	adds	r3, #64
	movs	r0, #17
	movs	r1, #71
	bl 0x0200b328
	b.n	.L_020013f0
.L_020013dc:
	asrs	r3, r0, #20
	asrs	r2, r1, #20
	movs	r1, #1
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	adds	r3, #64
	movs	r0, #54
	movs	r1, #87
	bl 0x0200b328
.L_020013f0:
	movs	r0, #106
	bl 0x0200b4b8
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	asrs	r1, r1, #20
	asrs	r3, r3, #20
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r1, r1, #20
	lsls	r3, r3, #20
	movs	r0, #94
	adds	r1, r1, r2
	adds	r3, r3, r2
	adds	r0, #255
	movs	r2, #0
	bl 0x0200b2f0
	adds	r5, r0, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r3, r5, #0
	adds	r3, #85
	movs	r1, #0
	strb	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #4
	bl 0x0200b3f0
	movs	r3, #176
	lsls	r3, r3, #9
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x020083b0
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200b2e0
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200b358
	ldr	r1, [pc, #192]
	adds	r0, r5, #0
	bl 0x0200b2e8
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	adds	r2, #122
	adds	r0, r0, r2
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b2b8
	movs	r6, #1
	adds	r5, r0, #0
.L_02001476:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r0, r6, r3
	bl 0x0200b2b8
	adds	r6, #1
	ands	r5, r0
	cmp	r6, #7
	bne.n	.L_02001476
	cmp	r5, #0
	beq.n	.L_02001506
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	movs	r0, #10
	bl 0x0200b390
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r1, #153
	adds	r0, #204
	bl 0x0200b420
	ldr	r3, [pc, #92]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #84]
	movs	r5, #9
	cmp	r2, r3
	bne.n	.L_020014c2
	movs	r5, #8
.L_020014c2:
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200b438
	bl 0x0200b430
	movs	r0, #10
	bl 0x0200b390
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #70
	bl 0x0200b4b8
	movs	r1, #2
	adds	r0, r5, #0
	bl 0x0200b3e8
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2c0
	adds	r0, r5, #0
	bl 0x0200b3a8
	movs	r3, #4
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #80
	bl 0x0200b390
	bl 0x0200b3a0
.L_02001506:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0132
	.2byte 0x0000
	push	{r2, r4, r5, r6, lr}
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #240]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	adds	r7, r0, #0
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #208]
	cmp	r2, r3
	bne.n	.L_02001554
	movs	r0, #8
	bl 0x0200b3a8
	adds	r6, r0, #0
	movs	r0, #13
	bl 0x0200b450
	b.n	.L_02001562
.L_02001554:
	movs	r0, #9
	bl 0x0200b3a8
	adds	r6, r0, #0
	movs	r0, #11
	bl 0x0200b450
.L_02001562:
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	ldr	r5, [pc, #164]
	movs	r3, #133
	lsls	r3, r3, #2
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	adds	r5, r5, r3
	lsls	r1, r1, #4
	lsls	r2, r2, #4
	ldr	r0, [r5, #0]
	adds	r1, #8
	adds	r2, #8
	bl 0x0200b3c8
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #10
	bl 0x0200b3f8
	movs	r0, #30
	bl 0x0200b390
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	negs	r0, r0
	bl 0x0200b428
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #48]
	movs	r3, #128
	lsls	r3, r3, #4
	str	r3, [r6, #52]
	movs	r0, #198
	bl 0x0200b4b8
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #11
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	bl 0x0200b310
	adds	r0, r6, #0
	bl 0x0200b318
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x020083b0
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x020083b0
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #16
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	bl 0x0200b310
	movs	r0, #80
	bl 0x0200b390
	bl 0x0200b478
	bl 0x0200b480
	bl 0x0200b3a0
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0132
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #40]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	adds	r5, r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #2
	bl 0x0200b348
	cmp	r0, #232
	bne.n	.L_0200163c
	adds	r2, r5, #0
	adds	r2, #34
	movs	r3, #2
	strb	r3, [r2, #0]
.L_0200163c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #8
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #134
	ldr	r0, [r3, #0]
	lsls	r1, r1, #2
	subs	r2, #172
	bl 0x0200b3c8
	movs	r0, #8
	bl 0x0200b390
.L_02001688:
	movs	r5, #1
	movs	r3, #22
	movs	r0, #50
	movs	r1, #22
	movs	r2, #33
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b328
	movs	r1, #0
	movs	r2, #0
	movs	r0, #14
	bl 0x0200b3e0
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200b4b8
	movs	r0, #30
	bl 0x0200b390
	movs	r0, #188
	bl 0x0200b4b8
	movs	r6, #2
	movs	r1, #20
	movs	r2, #31
	movs	r3, #20
	movs	r0, #50
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b328
	movs	r0, #5
	bl 0x0200b390
	movs	r1, #20
	movs	r2, #31
	movs	r3, #20
	movs	r0, #51
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b328
	movs	r0, #5
	bl 0x0200b390
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b2c0
	bl 0x0200b3a0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #14
	sub	sp, #8
	bl 0x0200b3a8
	adds	r5, r0, #0
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	movs	r3, #134
	lsls	r3, r3, #18
	str	r3, [r5, #8]
	movs	r3, #180
	lsls	r3, r3, #17
	str	r3, [r5, #16]
	ldr	r3, [pc, #104]
	movs	r1, #22
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	movs	r2, #33
	movs	r5, #1
	movs	r3, #22
	movs	r0, #51
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b328
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200b4b8
	movs	r0, #30
	bl 0x0200b390
	movs	r0, #188
	bl 0x0200b4b8
	movs	r6, #2
	movs	r1, #20
	movs	r2, #31
	movs	r3, #20
	movs	r0, #50
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b328
	movs	r0, #5
	bl 0x0200b390
	movs	r1, #20
	movs	r2, #31
	movs	r3, #20
	movs	r0, #52
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b328
	movs	r0, #5
	bl 0x0200b390
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b2c8
	bl 0x0200b3a0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xfff2
	.2byte 0xb500
	bl 0x0200b0e0
	cmp	r0, #0
	beq.n	.L_0200179c
	movs	r0, #11
	bl 0x0200b450
.L_0200179c:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200b0e0
	cmp	r0, #0
	beq.n	.L_020017b0
	movs	r0, #12
	bl 0x0200b450
.L_020017b0:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #364]
	adds	r6, r0, #0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r5, r0
	ldr	r0, [r5, #0]
	sub	sp, #20
	bl 0x0200b3a8
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	bl 0x0200b3a8
	movs	r1, #18
	ldrsh	r3, [r0, r1]
	cmp	r3, #248
	ble.n	.L_02001800
	movs	r0, #128
	movs	r3, #0
	lsls	r0, r0, #2
	str	r3, [r6, #108]
	bl 0x0200b2c8
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #320]
	movs	r2, #128
.L_020017ee:
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r6, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r6, #16]
	b.n	.L_02001920
.L_02001800:
	add	r2, sp, #8
	ldr	r3, [r7, #8]
	mov	r8, r2
	ldr	r2, [r6, #76]
	ldr	r1, [r6, #8]
	subs	r3, r3, r2
	subs	r3, r1, r3
	mov	r5, r8
	str	r3, [r5, #0]
	str	r3, [sp, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	ldr	r0, [r6, #52]
	ldr	r4, [r6, #16]
	subs	r3, r3, r0
	adds	r3, r4, r3
	str	r3, [r5, #8]
	str	r3, [sp, #4]
	ldr	r3, [r7, #8]
	cmp	r2, r3
	bne.n	.L_02001832
	ldr	r3, [r7, #16]
	cmp	r0, r3
	beq.n	.L_020018b2
.L_02001832:
	movs	r0, #168
	adds	r3, r4, #0
	ldr	r2, [r6, #12]
	lsls	r0, r0, #2
	bl 0x0200b2f0
	adds	r5, r0, #0
	bl 0x0200b268
	movs	r3, #15
	ldr	r2, [r5, #8]
	ands	r3, r0
	subs	r3, #8
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r5, #8]
	bl 0x0200b268
	movs	r3, #31
	ldr	r2, [r5, #12]
	ands	r3, r0
	subs	r3, #4
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r5, #12]
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r3, r6, #0
	adds	r3, #98
	ldrb	r1, [r3, #0]
	adds	r0, r5, #0
	bl 0x0200b3f0
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x020083b0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200b2e0
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200b358
	ldr	r1, [pc, #156]
	adds	r0, r5, #0
	bl 0x0200b2e8
	ldr	r3, [r7, #8]
	movs	r2, #3
	str	r3, [r6, #76]
	ldr	r3, [r7, #16]
	str	r3, [r6, #52]
	ldr	r3, [pc, #144]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020018b2
	movs	r0, #106
	bl 0x0200b4b8
.L_020018b2:
	mov	r1, r8
	movs	r0, #10
	ldrsh	r3, [r1, r0]
	cmp	r3, #248
	bgt.n	.L_02001920
	ldrh	r3, [r7, #6]
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #224
	subs	r1, r1, r3
	lsls	r0, r0, #11
	mov	r2, r8
	adds	r5, r6, #0
	bl 0x0200b288
	adds	r5, #34
	mov	r2, r8
	ldr	r1, [r2, #0]
	ldrb	r0, [r5, #0]
	ldr	r2, [r2, #8]
	bl 0x0200b348
	cmp	r0, #255
	beq.n	.L_02001920
	ldr	r3, [sp, #0]
	ldr	r2, [sp, #4]
	str	r3, [r6, #8]
	str	r2, [r6, #16]
	adds	r1, r3, #0
	ldrb	r0, [r5, #0]
	bl 0x0200b348
	cmp	r0, #52
	bne.n	.L_02001908
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r5, #181
	lsls	r5, r5, #1
	adds	r1, r3, r5
	movs	r2, #0
.L_02001904:
	movs	r3, #201
	b.n	.L_0200191c
.L_02001908:
	cmp	r0, #51
	bne.n	.L_02001920
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #181
	lsls	r0, r0, #1
	adds	r1, r3, r0
.L_02001918:
	movs	r2, #0
	movs	r3, #200
.L_0200191c:
	strh	r3, [r1, #0]
	str	r2, [r6, #108]
.L_02001920:
	add	sp, #20
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb594
	lsls	r0, r0, #8
	asrs	r4, r5, #8
	lsls	r0, r0, #12
	push	{r5, r6, lr}
	ldr	r3, [pc, #100]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_0200194e
	movs	r0, #8
	b.n	.L_02001958
.L_0200194e:
	cmp	r3, #3
	bne.n	.L_02001956
	movs	r0, #9
	b.n	.L_02001958
.L_02001956:
	movs	r0, #10
.L_02001958:
	bl 0x0200b3a8
	adds	r5, r0, #0
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x0200b3a8
	movs	r2, #18
	ldrsh	r3, [r0, r2]
	cmp	r3, #247
	bgt.n	.L_0200199c
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [pc, #36]
	ldr	r0, [r6, #0]
	str	r3, [r5, #108]
	bl 0x0200b3a8
	ldr	r3, [r0, #8]
	str	r3, [r5, #76]
.L_0200198a:
	ldr	r0, [r6, #0]
	bl 0x0200b3a8
	ldr	r3, [r0, #16]
.L_02001992:
	movs	r0, #128
	str	r3, [r5, #52]
	lsls	r0, r0, #2
	bl 0x0200b2c0
.L_0200199c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x97b5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #28]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	sub	sp, #12
	cmp	r3, #1
	bne.n	.L_020019c0
	movs	r0, #8
	b.n	.L_020019ce
.L_020019c0:
	cmp	r3, #3
	bne.n	.L_020019cc
	movs	r0, #9
	b.n	.L_020019ce
	.2byte 0x0240
	.2byte 0x0200
.L_020019cc:
	movs	r0, #10
.L_020019ce:
	bl 0x0200b3a8
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	ldr	r1, [pc, #164]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	mov	r5, sp
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	str	r3, [r7, #48]
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	ldr	r1, [r5, #0]
	ldr	r3, [r5, #8]
	adds	r0, r7, #0
	movs	r2, #0
	bl 0x0200b310
	ldr	r2, [r5, #8]
	movs	r3, #236
	lsls	r3, r3, #14
	adds	r2, r2, r3
	movs	r3, #143
	ldr	r0, [r7, #8]
	lsls	r3, r3, #1
	movs	r1, #0
	bl 0x020083f8
	movs	r3, #192
	adds	r6, r0, #0
	lsls	r3, r3, #9
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #5]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
.L_02001a34:
	orrs	r3, r2
	strb	r3, [r1, #5]
	movs	r2, #128
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #74
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	movs	r0, #204
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl 0x0200b4b8
	adds	r2, r7, #0
	movs	r3, #3
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r5, #0
	adds	r0, r7, #0
	bl 0x020087e8
	movs	r1, #0
	movs	r2, #0
	str	r5, [r7, #8]
	str	r5, [r7, #12]
	str	r5, [r7, #16]
	movs	r0, #8
	b.n	.L_02001a80
	.2byte 0x0000
	.4byte 0x00002f3f
	.4byte 0x00008000
	.2byte 0x0000
	.2byte 0xfff0
.L_02001a80:
	.2byte 0xf001
	.2byte 0xfcae
	.2byte 0x1c30
	bl 0x0200b2f8
	bl 0x0200b3a0
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #232]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	sub	sp, #20
	cmp	r3, #1
	bne.n	.L_02001abe
	movs	r0, #8
	bl 0x0200b3a8
	adds	r6, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200b2c0
	movs	r7, #11
	b.n	.L_02001aec
.L_02001abe:
	cmp	r3, #3
	bne.n	.L_02001ad8
	movs	r0, #9
	bl 0x0200b3a8
	adds	r6, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200b2c0
	movs	r7, #26
	b.n	.L_02001aec
.L_02001ad8:
	movs	r0, #10
	bl 0x0200b3a8
	adds	r6, r0, #0
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200b2c0
	movs	r7, #53
.L_02001aec:
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #136]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	add	r5, sp, #8
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r6, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #52]
	str	r3, [r6, #48]
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	ldr	r1, [r5, #0]
	ldr	r3, [r5, #8]
	movs	r2, #0
	adds	r0, r6, #0
	bl 0x0200b310
	adds	r0, r6, #0
	bl 0x0200b318
	movs	r0, #188
	bl 0x0200b4b8
	movs	r5, #1
	adds	r2, r7, #0
	movs	r6, #2
	movs	r1, #4
	movs	r3, #4
	movs	r0, #18
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b328
	movs	r0, #5
	bl 0x0200b250
	adds	r2, r7, #0
	movs	r1, #7
	movs	r3, #4
	movs	r0, #18
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b328
	movs	r0, #5
	bl 0x0200b250
	movs	r1, #176
	lsls	r0, r7, #20
	lsls	r1, r1, #15
	movs	r2, #0
	movs	r3, #0
	bl 0x0200b380
	bl 0x0200b3a0
	add	sp, #20
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b460
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200b458
	movs	r0, #60
	bl 0x0200b468
	movs	r0, #60
	bl 0x0200b390
	movs	r2, #6
	ldr	r0, [pc, #44]
	movs	r1, #0
	bl 0x0200b388
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b458
	movs	r0, #60
	bl 0x0200b468
	movs	r0, #60
	bl 0x0200b390
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #140
	bl 0x0200b2c0
	bl 0x0200b3a0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x30ab
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02001c04
	ldr	r0, [pc, #52]
	b.n	.L_02001c2e
.L_02001c04:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02001c0e
	ldr	r0, [pc, #52]
	b.n	.L_02001c2e
.L_02001c0e:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02001c18
	ldr	r0, [pc, #48]
	b.n	.L_02001c2e
.L_02001c18:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02001c22
	ldr	r0, [pc, #48]
	b.n	.L_02001c2e
.L_02001c22:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02001c2c
	ldr	r0, [pc, #44]
	b.n	.L_02001c2e
.L_02001c2c:
	ldr	r0, [pc, #44]
.L_02001c2e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x0200bdcc
	.4byte 0x00000130
	.4byte 0x0200be38
	.4byte 0x00000131
	.4byte 0x0200bfac
	.4byte 0x00000132
	.4byte 0x0200c150
	.4byte 0x00000133
	.4byte 0x0200c2c4
	.2byte 0xbdc0
	.2byte 0x0200
	push	{lr}
	bl 0x020080f4
	bl 0x0200b4b0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r1, #99
	ldr	r0, [pc, #20]
	bl 0x0200b448
	movs	r0, #40
	bl 0x0200b390
	bl 0x02008158
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x00ec
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	movs	r5, #8
.L_02001ca8:
	adds	r0, r5, #0
	bl 0x0200b3a8
	cmp	r0, #0
	beq.n	.L_02001cba
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02001cba:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_02001ca8
	movs	r0, #158
	bl 0x0200b4b8
	ldr	r3, [pc, #144]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #136]
	cmp	r2, r3
	bne.n	.L_02001ce4
	ldr	r0, [pc, #132]
	movs	r1, #33
	movs	r2, #7
	bl 0x0200b320
	b.n	.L_02001d0a
.L_02001ce4:
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	ldr	r4, [pc, #116]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	lsrs	r3, r3, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #1
	lsls	r0, r0, #3
	adds	r3, r0, #4
	ldrh	r1, [r4, r3]
	adds	r3, r3, r4
	ldrh	r2, [r3, #2]
	ldr	r0, [r4, r0]
	bl 0x0200b320
.L_02001d0a:
	ldr	r5, [pc, #76]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200b3e8
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200b3b0
	movs	r2, #4
	negs	r2, r2
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b3d0
	movs	r0, #6
	bl 0x0200b390
	movs	r1, #170
	lsls	r1, r1, #1
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200b450
	bl 0x0200b478
	bl 0x0200b480
	bl 0x0200b3a0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x0200c3e4
	.2byte 0xc410
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #9
	bl 0x0200b3a8
	adds	r6, r0, #0
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	movs	r0, #9
	movs	r1, #2
	bl 0x0200b3e8
	ldr	r3, [pc, #96]
	movs	r1, #182
	movs	r2, #240
	str	r3, [r6, #12]
	movs	r0, #9
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	bl 0x0200b3e0
	ldr	r5, [pc, #84]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #182
	movs	r2, #240
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	ldr	r0, [r5, #0]
	bl 0x0200b3e0
	ldr	r0, [r5, #0]
	bl 0x0200b3a8
	ldr	r3, [pc, #60]
	str	r3, [r0, #12]
	bl 0x0200b470
	movs	r3, #128
	lsls	r3, r3, #9
	ldr	r2, [r6, #12]
	str	r3, [r6, #52]
	str	r3, [r6, #48]
	movs	r3, #192
	lsls	r3, r3, #14
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	bl 0x0200b310
	adds	r0, r6, #0
	bl 0x0200b318
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200b2c0
	bl 0x0200b3a0
	pop	{r5, r6, pc}
	.4byte 0xffc00000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffd0
	.2byte 0xb520
	movs	r0, #11
	bl 0x0200b3a8
	adds	r5, r0, #0
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r2, #133
.L_02001e14:
	str	r3, [r5, #12]
	ldr	r3, [pc, #68]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	ldr	r3, [pc, #60]
	str	r3, [r0, #12]
	bl 0x0200b470
	movs	r3, #128
	lsls	r3, r3, #9
	ldr	r2, [r5, #12]
	str	r3, [r5, #52]
	str	r3, [r5, #48]
	movs	r3, #192
	lsls	r3, r3, #14
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200b310
	adds	r0, r5, #0
	bl 0x0200b318
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200b2c0
	bl 0x0200b3a0
	pop	{r5, pc}
	.4byte 0xffc00000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffd0
	.2byte 0xb5e0
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r7, #129
	adds	r3, r3, r0
	lsls	r7, r7, #2
	ldr	r6, [pc, #796]
	str	r7, [r3, #0]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r6, r1
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	ldr	r3, [pc, #784]
	sub	sp, #8
	cmp	r1, r3
	bne.n	.L_02001f86
	ldr	r0, [pc, #780]
	ldr	r1, [pc, #784]
	ldr	r2, [pc, #784]
	ldr	r3, [pc, #788]
	bl 0x02008038
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #135
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02001ec6
	movs	r0, #130
	lsls	r0, r0, #2
	movs	r1, #48
	movs	r2, #0
	bl 0x02008a78
	movs	r3, #9
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #85
	movs	r1, #34
	movs	r2, #85
	movs	r3, #8
	bl 0x0200b328
.L_02001ec6:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #136
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02001ef4
	movs	r0, #138
	lsls	r0, r0, #2
	movs	r1, #48
	movs	r2, #3
	bl 0x02008a78
	movs	r3, #9
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #101
	movs	r1, #34
	movs	r2, #101
	movs	r3, #8
	bl 0x0200b328
.L_02001ef4:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #137
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02001f22
	movs	r0, #252
	lsls	r0, r0, #1
	movs	r1, #40
	movs	r2, #10
	bl 0x02008a78
	movs	r3, #9
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #85
	movs	r1, #46
	movs	r2, #85
	movs	r3, #20
	bl 0x0200b328
.L_02001f22:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02001f50
	movs	r0, #142
	lsls	r0, r0, #2
	movs	r1, #40
	movs	r2, #2
	bl 0x02008a78
	movs	r3, #9
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #101
	movs	r1, #46
	movs	r2, #101
	movs	r3, #20
	bl 0x0200b328
.L_02001f50:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #139
	bl 0x0200b2b8
	cmp	r0, #0
	bne.n	.L_02001f60
	b.n	.L_020026e6
.L_02001f60:
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #54
	movs	r1, #7
	movs	r2, #33
	movs	r3, #7
	bl 0x0200b328
	movs	r0, #134
	movs	r1, #136
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r3, #0
	bl 0x0200b380
	b.n	.L_020026e6
.L_02001f86:
	ldr	r3, [pc, #548]
	cmp	r1, r3
	beq.n	.L_02001f8e
	b.n	.L_020021b4
.L_02001f8e:
	movs	r0, #8
	movs	r1, #1
	bl 0x0200b408
	movs	r0, #168
	lsls	r0, r0, #4
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02001fb8
	movs	r1, #148
	movs	r2, #248
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200b3e0
	movs	r0, #8
	movs	r1, #2
	bl 0x0200b408
.L_02001fb8:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #129
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02001fd4
	movs	r1, #220
	movs	r2, #190
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200b3e0
.L_02001fd4:
	ldr	r0, [pc, #472]
	bl 0x0200b490
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200b3a8
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #9
	bl 0x0200b3a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	movs	r2, #241
	lsls	r2, r2, #1
	strb	r3, [r0, #0]
	adds	r3, r6, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #17
	bne.n	.L_0200203c
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_0200203c
	movs	r0, #9
	movs	r1, #2
	bl 0x0200b3e8
	movs	r1, #182
	movs	r2, #240
	movs	r0, #9
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	bl 0x0200b3e0
.L_0200203c:
	ldr	r3, [pc, #340]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #18
	bne.n	.L_0200205c
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	bne.n	.L_0200205c
	bl 0x02009d68
.L_0200205c:
	ldr	r5, [pc, #308]
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #10
	beq.n	.L_0200206e
	b.n	.L_020026e6
.L_0200206e:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002084
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b3e0
.L_02002084:
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_0200209e
	movs	r0, #9
	bl 0x0200b3a8
	movs	r3, #4
	adds	r0, #85
	strb	r3, [r0, #0]
.L_0200209e:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_020020bc
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #87
	movs	r2, #44
	movs	r3, #86
	bl 0x0200b328
.L_020020bc:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_020020dc
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #87
	movs	r2, #46
	movs	r3, #86
	bl 0x0200b328
.L_020020dc:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_020020fc
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #87
	movs	r2, #43
	movs	r3, #87
	bl 0x0200b328
.L_020020fc:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_0200211c
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #87
	movs	r2, #45
	movs	r3, #87
	bl 0x0200b328
.L_0200211c:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_0200213a
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #87
	movs	r2, #47
	movs	r3, #87
	bl 0x0200b328
.L_0200213a:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_0200215a
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #87
	movs	r2, #44
	movs	r3, #88
	bl 0x0200b328
.L_0200215a:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_0200217a
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #87
	movs	r2, #46
	movs	r3, #88
	bl 0x0200b328
.L_0200217a:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_020026e6
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x0200b84c
	.4byte 0x0200b85c
	.4byte 0x0200b888
	.4byte 0x0200b8b4
	.4byte 0x00000130
	.2byte 0xb6e4
	.2byte 0x0200
.L_020021b4:
	ldr	r3, [pc, #864]
	cmp	r1, r3
	beq.n	.L_020021bc
	b.n	.L_020022ce
.L_020021bc:
	movs	r0, #14
	bl 0x0200b3a8
	adds	r3, r0, #0
	adds	r3, #85
	movs	r7, #2
	strb	r7, [r3, #0]
	ldr	r3, [pc, #848]
	movs	r5, #1
	str	r3, [r0, #20]
	str	r3, [r0, #12]
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r6, r0
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200b3a8
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002218
	movs	r1, #224
	movs	r2, #212
	movs	r0, #8
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x0200b3e0
.L_02002218:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #131
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002254
	movs	r1, #134
	movs	r2, #180
	movs	r0, #10
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200b3e0
	movs	r0, #51
	movs	r1, #20
	movs	r2, #31
	movs	r3, #20
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	bl 0x0200b328
	movs	r0, #248
	movs	r1, #168
	lsls	r0, r0, #17
.L_0200224a:
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x0200b380
.L_02002254:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #132
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002270
	movs	r1, #186
	movs	r2, #228
	movs	r0, #13
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200b3e0
.L_02002270:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #133
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_020022c6
	movs	r0, #10
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	bne.n	.L_020022b8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #132
	bl 0x0200b2b8
	cmp	r0, #0
	bne.n	.L_020022b8
	movs	r0, #12
	bl 0x0200b3a8
	movs	r1, #10
	ldrsh	r5, [r0, r1]
	movs	r0, #12
	bl 0x0200b3a8
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	lsls	r5, r5, #16
	lsls	r2, r2, #16
	movs	r0, #13
	adds	r1, r5, #0
	bl 0x0200b3e0
.L_020022b8:
	movs	r1, #182
	movs	r2, #236
	movs	r0, #12
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200b3e0
.L_020022c6:
	ldr	r0, [pc, #600]
	bl 0x0200b490
	b.n	.L_020026e6
.L_020022ce:
	ldr	r3, [pc, #596]
	cmp	r1, r3
	beq.n	.L_020022d6
	b.n	.L_0200252c
.L_020022d6:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #134
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_020022f2
	movs	r1, #132
	movs	r2, #180
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b3e0
.L_020022f2:
	ldr	r0, [pc, #564]
	bl 0x0200b490
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r2, r6, r0
	ldrh	r3, [r2, #0]
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	cmp	r2, #1
	beq.n	.L_0200230a
	b.n	.L_020024a2
.L_0200230a:
	movs	r3, #68
	movs	r1, #160
	movs	r2, #224
	str	r3, [sp, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	movs	r3, #18
	movs	r0, #33
	bl 0x0200aa74
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200b2b8
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0200235e
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200b3a8
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #8
	bl 0x0200b3a8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_020023a8
.L_0200235e:
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b3e0
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b2c0
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200b2c0
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2c0
	adds	r0, r7, #0
	bl 0x0200b2c0
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200b2c0
.L_020023a8:
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_020023c2
	movs	r0, #8
	bl 0x0200b3a8
	movs	r3, #4
	adds	r0, #85
	strb	r3, [r0, #0]
.L_020023c2:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_020023e0
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #71
	movs	r2, #8
	movs	r3, #69
	bl 0x0200b328
.L_020023e0:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002400
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #71
	movs	r2, #6
	movs	r3, #71
	bl 0x0200b328
.L_02002400:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002420
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #71
	movs	r2, #5
	movs	r3, #75
	bl 0x0200b328
.L_02002420:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002440
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #71
	movs	r2, #8
	movs	r3, #75
	bl 0x0200b328
.L_02002440:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002460
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #71
	movs	r2, #11
	movs	r3, #75
	bl 0x0200b328
.L_02002460:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_0200247e
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #71
	movs	r2, #8
	movs	r3, #73
	bl 0x0200b328
.L_0200247e:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200b2b8
	cmp	r0, #0
	bne.n	.L_0200248e
	b.n	.L_020026e6
.L_0200248e:
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #71
	movs	r2, #10
	movs	r3, #71
	bl 0x0200b328
	b.n	.L_020026e6
.L_020024a2:
	cmp	r2, #11
	bne.n	.L_020024c0
	movs	r3, #68
	movs	r1, #160
	movs	r2, #224
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	str	r3, [sp, #0]
	movs	r0, #33
	movs	r3, #18
	bl 0x0200aa74
	movs	r0, #22
	movs	r1, #0
	b.n	.L_020024ec
.L_020024c0:
	cmp	r2, #12
	bne.n	.L_020024f4
	movs	r3, #68
	movs	r1, #160
	movs	r2, #224
	str	r3, [sp, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	movs	r0, #35
	movs	r3, #18
	bl 0x0200aa74
	movs	r0, #10
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_020024e6
	b.n	.L_020026e6
.L_020024e6:
	movs	r1, #36
	negs	r1, r1
	movs	r0, #36
.L_020024ec:
	adds	r2, r7, #0
	bl 0x0200b190
	b.n	.L_020026e6
.L_020024f4:
	subs	r3, #4
	movs	r0, #128
	lsls	r3, r3, #16
	lsls	r0, r0, #9
	cmp	r3, r0
	bls.n	.L_02002502
	b.n	.L_020026e6
.L_02002502:
	movs	r3, #68
	movs	r1, #160
	movs	r2, #224
	str	r3, [sp, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	movs	r0, #35
	movs	r3, #18
	bl 0x0200aa74
	b.n	.L_020026e6
	.4byte 0x00000131
	.4byte 0xfff20000
	.4byte 0x0200b6ea
	.4byte 0x00000132
	.2byte 0xb6f6
	.2byte 0x0200
.L_0200252c:
	ldr	r3, [pc, #444]
	cmp	r1, r3
	beq.n	.L_02002534
	b.n	.L_020026e6
.L_02002534:
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldr	r5, [r2, #32]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #1
	cmp	r3, #11
	bls.n	.L_02002548
	b.n	.L_020026e6
.L_02002548:
	ldr	r2, [pc, #420]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200a606
	.4byte 0x0200a606
	.4byte 0x0200a63a
	.4byte 0x0200a63a
	.4byte 0x0200a686
	.4byte 0x0200a686
	.4byte 0x0200a580
	.4byte 0x0200a6e6
	.4byte 0x0200a6e6
	.4byte 0x0200a6e6
	.4byte 0x0200a6e6
	.4byte 0x0200a596
	.4byte 0x008020c1
	.4byte 0xfe98f000
	.4byte 0xd1042800
	.4byte 0x2100200b
	.4byte 0xf0002200
	.4byte 0x4e57ff25
	.4byte 0x00802085
	.4byte 0x68181833
	.4byte 0xff02f000
	.4byte 0x78023023
	.4byte 0x43132340
	.4byte 0x21027003
	.4byte 0xf000200b
	.4byte 0x200bff19
	.4byte 0xfef6f000
	.4byte 0x30552500
	.4byte 0x200b7005
	.4byte 0xfef0f000
	.4byte 0x78023023
	.4byte 0x43132380
	.4byte 0x21027003
	.4byte 0xf000200b
	.4byte 0x20c1ff07
	.4byte 0xf0000080
	.4byte 0x2800fe6b
	.4byte 0x21f1d109
	.4byte 0x18730049
	.4byte 0x5e9b2200
	.4byte 0xd1022b0c
	.4byte 0xfbfef7ff
	.4byte 0x200be075
	.4byte 0xfed4f000
	.4byte 0x60c34b3d
	.4byte 0x20c0e06f
	.4byte 0x30010080
	.4byte 0xfe54f000
	.4byte 0xd0042800
	.4byte 0x22f021f0
	.4byte 0x03c92008
	.4byte 0x2301e018
	.4byte 0x93002202
	.4byte 0x20129201
	.4byte 0x220b2101
	.4byte 0xf0002304
	.4byte 0x20b8fe7b
	.4byte 0x040021b0
	.4byte 0x20c0e01f
	.4byte 0x30020080
	.4byte 0xfe3af000
	.4byte 0xd00b2800
	.4byte 0x22f0218a
	.4byte 0x04892009
	.4byte 0xf00003d2
	.4byte 0x2080fec5
	.4byte 0xf0000080
	.4byte 0xe042fe31
	.4byte 0x22022301
	.4byte 0x92019300
	.4byte 0x21012012
	.4byte 0x2304221a
	.4byte 0xfe5af000
	.4byte 0x21b020d4
	.4byte 0x03c90440
	.4byte 0x23022200
	.4byte 0xfe7ef000
	.4byte 0x2081e02f
	.4byte 0x30ff0080
	.4byte 0xfe14f000
	.4byte 0xd00b2800
	.4byte 0x22f021c6
	.4byte 0x0489200a
	.4byte 0xf00003d2
	.4byte 0x2080fe9f
	.4byte 0xf0000080
	.4byte 0xe011fe0b
	.4byte 0x22022301
	.4byte 0x92019300
	.4byte 0x21012012
	.4byte 0x23042235
	.4byte 0xfe34f000
	.4byte 0x21b020d6
	.4byte 0x03c90480
	.4byte 0x23022200
	.4byte 0xfe58f000
	.4byte 0x32ec1c2a
	.4byte 0x21806813
	.4byte 0x185b0309
	.4byte 0x32086013
	.4byte 0x185b6813
	.2byte 0x6013
.L_020026e6:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x00000133
	.4byte 0x0200a550
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0x2000
	bx	lr
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02002744
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002744
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
.L_02002744:
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
	bl 0x0200b3a8
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_020027c8
	cmp	r7, #0
	beq.n	.L_020027c8
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_020027d0
.L_020027c8:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_020027d0:
	mov	r3, sl
	bl 0x0200b2f0
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020027de
	b.n	.L_0200292a
.L_020027de:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200b2e0
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200b2e8
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200b358
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
	bl 0x0200a700
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
	beq.n	.L_0200292a
	cmp	r7, #0
	beq.n	.L_0200292a
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02002860
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200b3f0
.L_02002860:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002880
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x0200a700
.L_02002880:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02002894
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02002894:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020028da
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020028c2
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200b248
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020028d4
.L_020028c2:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200b248
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020028d4:
	bl 0x0200b248
	str	r0, [r6, #52]
.L_020028da:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020028f6
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200b2e0
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200b2e8
.L_020028f6:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002908
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02002908:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200291a
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200291a:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200292a
	ldr	r3, [r7, #36]
.L_02002928:
	str	r3, [r6, #108]
.L_0200292a:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c428
	.4byte 0x0200a749
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r4, [pc, #268]
	movs	r1, #1
	movs	r0, #12
.L_02002956:
	ldrsh	r3, [r4, r0]
	negs	r1, r1
	sub	sp, #4
	cmp	r3, r1
	beq.n	.L_02002a54
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, #32
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl 0x0200b3a8
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
.L_02002984:
	cmp	r3, r2
	bne.n	.L_02002994
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_0200299c
.L_02002994:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_0200299c:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02002a54
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02002a54
.L_020029b2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	str	r4, [sp, #0]
	adds	r3, r2, #0
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r5, [r3, #4]
	ldr	r3, [r2, #0]
	ands	r0, r1
	ands	r5, r1
	ldr	r6, [r3, #4]
	movs	r1, #16
	ldrsh	r3, [r4, r1]
	ldr	r2, [pc, #156]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r3
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	lsls	r1, r1, #20
	subs	r7, r1, r0
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	lsls	r2, r2, #20
	bl 0x0200b340
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	subs	r0, r0, r6
	lsls	r3, r3, #20
	subs	r3, r3, r5
	subs	r3, r3, r6
	subs	r2, r3, r0
	asrs	r7, r7, #16
	adds	r0, r0, r3
	asrs	r0, r0, #16
	adds	r3, r7, #0
	movs	r5, #167
	asrs	r2, r2, #16
	adds	r1, r0, #0
	adds	r3, #15
	lsls	r5, r5, #1
	adds	r2, #14
	adds	r1, #58
	ldr	r4, [sp, #0]
	cmp	r3, r5
	bhi.n	.L_02002a54
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02002a54
	cmp	r2, #239
	bgt.n	.L_02002a54
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r4, #20]
	lsls	r3, r7, #16
	orrs	r2, r3
	ldr	r3, [pc, #48]
	adds	r0, r4, #0
	orrs	r2, r3
	movs	r3, #128
	str	r2, [r4, #24]
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r4, #28]
	adds	r0, #20
	bl 0x0200b2b0
.L_02002a54:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200c458
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x020036e0
	.2byte 0x8800
	.2byte 0x8000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r0, [sp, #44]
	ldr	r0, [pc, #540]
	str	r1, [sp, #40]
	mov	r8, r0
	movs	r1, #32
	add	r1, r8
	mov	r9, r1
	mov	ip, r9
	adds	r5, r2, #0
	mov	r2, ip
	adds	r6, r3, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #520]
	movs	r1, #4
	ldr	r7, [sp, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xa80b
	ldrh	r0, [r0, #0]
	mov	r1, r8
	strh	r0, [r1, #4]
	add	r1, sp, #40
	ldrh	r1, [r1, #0]
	mov	r3, r8
	strh	r1, [r3, #0]
	strh	r5, [r3, #2]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r5, r8
	mov	r0, r8
	adds	r3, #255
	mov	r1, r8
	strh	r6, [r5, #6]
	movs	r2, #0
	strh	r7, [r0, #8]
	strh	r3, [r1, #12]
	mov	r3, r8
	strh	r2, [r3, #10]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	mov	ip, r3
	lsls	r2, r2, #1
	mov	r1, ip
	add	r2, ip
	adds	r1, #236
	ldr	r0, [r1, #0]
	ldr	r3, [r2, #8]
	ldr	r5, [r2, #48]
	adds	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	adds	r1, #4
	ldr	r3, [r2, #12]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r2
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	adds	r2, r2, r0
	lsls	r2, r2, #2
	asrs	r3, r3, #20
	adds	r5, r5, r2
	movs	r0, #0
	str	r3, [sp, #20]
	str	r5, [sp, #36]
	str	r0, [sp, #12]
	cmp	r0, r3
	bge.n	.L_02002bf8
.L_02002b28:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_02002bec
.L_02002b3c:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_02002bdc
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_02002bdc
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_02002bdc
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #32]
	mov	r0, r9
	adds	r7, r2, r3
	strh	r7, [r0, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #28]
	add	r0, sp, #40
	ldrh	r0, [r0, #0]
	adds	r6, r1, r2
	mov	r3, r9
	mov	r1, r9
	strh	r6, [r3, #2]
	strh	r0, [r1, #4]
	ldr	r1, [sp, #40]
	movs	r0, #10
	adds	r1, #1
	adds	r0, #255
	str	r1, [sp, #40]
	bl 0x0200b2b8
	cmp	r0, #0
	bne.n	.L_02002b90
	cmp	r5, sl
	bne.n	.L_02002bce
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200b2c0
	b.n	.L_02002bce
.L_02002b90:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002bce
	mov	r2, r8
	ldrh	r4, [r2, #6]
	ldrh	r5, [r2, #8]
	movs	r3, #8
	ldrsh	r1, [r2, r3]
	movs	r3, #6
	ldrsh	r0, [r2, r3]
	movs	r2, #64
	adds	r3, r2, #0
	ands	r3, r4
	ands	r2, r5
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	asrs	r3, r3, #16
	asrs	r2, r2, #16
	orrs	r7, r3
	orrs	r6, r2
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b350
.L_02002bce:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_02002bdc:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02002b3c
.L_02002bec:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_02002b28
.L_02002bf8:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200b2b8
	cmp	r0, #0
	beq.n	.L_02002c50
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	ldr	r3, [r0, #8]
	movs	r2, #0
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	mov	r0, r8
	asrs	r1, r3, #20
	ldr	r3, [sp, #8]
	mov	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	cmp	r2, r3
	bge.n	.L_02002c50
.L_02002c2a:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02002c40
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02002c40
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02002c40:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_02002c2a
.L_02002c50:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200b290
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_02002c5e:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02002c5e
	bl 0x0200b2a8
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200b2a0
	adds	r0, r5, #0
	bl 0x0200b298
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200b258
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200c458
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0xa949
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #116]
	movs	r2, #133
	mov	r8, r1
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200b3b0
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200b3b8
	movs	r0, #1
	bl 0x0200b250
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #20
	lsls	r6, r6, #20
	adds	r5, r5, r3
	mov	r1, sl
	adds	r6, r6, r3
	ldr	r2, [r1, #12]
	adds	r3, r6, #0
	adds	r1, r5, #0
	mov	r0, sl
	bl 0x0200b300
	movs	r0, #4
	bl 0x0200b390
	bl 0x0200b440
	ldr	r2, [pc, #20]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #100]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200b3a8
	mov	r2, r8
	ldrh	r1, [r2, #6]
	movs	r2, #64
	ldr	r6, [r0, #8]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	asrs	r6, r6, #20
	orrs	r6, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r0, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200acb8
	movs	r0, #161
	bl 0x0200b4b8
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b350
	movs	r0, #12
	bl 0x0200b390
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r1, r1, r2
	mov	r8, r0
	ldr	r0, [r1, #0]
	sub	sp, #8
	mov	sl, r1
	bl 0x0200b3a8
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #64
	asrs	r7, r3, #20
	mov	r3, r8
	ldrh	r1, [r3, #6]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	orrs	r7, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r6, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200acb8
	movs	r0, #229
	bl 0x0200b4b8
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #2
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b350
	movs	r0, #12
	bl 0x0200b390
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	movs	r1, #0
	bl 0x0200b358
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #16]
	movs	r7, #0
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r2, sl
	b.n	.L_02002e64
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_02002e64:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200b3e8
	movs	r0, #16
	bl 0x0200b390
.L_02002e78:
	cmp	r7, #5
	bne.n	.L_02002e82
	movs	r0, #204
	bl 0x0200b4b8
.L_02002e82:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r7, #1
	bl 0x0200b250
	cmp	r7, #39
	ble.n	.L_02002e78
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #14]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200b478
	bl 0x0200b480
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
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
	bl 0x0200b248
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002f20
	adds	r3, #15
.L_02002f20:
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
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200b3a8
	adds	r7, r0, #0
	bl 0x0200b398
	movs	r0, #0
	bl 0x0200b488
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200b428
	bl 0x0200b308
	movs	r0, #1
	bl 0x0200b250
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
	bl 0x0200b470
	bl 0x0200b480
	movs	r0, #204
	bl 0x0200b4b8
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200b390
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #256]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_02002fe2:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200b280
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200b278
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200b268
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200b268
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #176]
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
	ldr	r4, [pc, #156]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x0200a780
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02002fe2
	movs	r0, #188
	bl 0x0200b4b8
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200b410
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200b3e8
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200b368
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200b368
	bl 0x0200b370
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200b410
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200b390
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200b3e8
	bl 0x0200b3a0
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200aef1
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #156]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200b3a8
	ldr	r3, [r0, #8]
	ldr	r6, [pc, #144]
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r0, #16]
	adds	r5, r6, #0
	asrs	r3, r3, #20
	mov	sl, r3
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	movs	r7, #0
	adds	r5, #32
	ldrh	r2, [r6, #10]
	cmp	r7, r3
	bge.n	.L_0200317c
.L_02003114:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_02003170
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_02003170
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200b2b8
	cmp	r0, #0
	bne.n	.L_02003144
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200ad40
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200b2c0
	strh	r7, [r6, #12]
	b.n	.L_0200317c
.L_02003144:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_0200317c
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200adb0
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200b2d8
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200b2d8
	movs	r0, #1
	b.n	.L_0200317e
.L_02003170:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_02003114
.L_0200317c:
	movs	r0, #0
.L_0200317e:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xc458
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	ldr	r3, [pc, #156]
	str	r2, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r1
	bl 0x0200b3a8
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200b2d0
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200b2d0
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_020031de
	cmp	r0, #0
	beq.n	.L_02003232
.L_020031de:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200b2d8
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200b2d8
	mov	r3, r9
	adds	r2, r7, r3
	mov	r3, r8
	movs	r1, #128
	add	r3, fp
	lsls	r1, r1, #12
	lsls	r3, r3, #20
	adds	r3, r3, r1
	str	r3, [r6, #16]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r2, r2, #20
	adds	r2, r2, r1
	ldr	r1, [r3, #0]
	str	r2, [r6, #8]
	str	r2, [r1, #8]
	ldr	r3, [r6, #16]
	str	r3, [r1, #16]
	bl 0x0200b308
	bl 0x0200af48
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_02003232:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c458
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000101, 0x08000119, 0x08000121, 0x08000129, 0x08000169, 0x08000179, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c9, 0x080003d1, 0x080003d9, 0x080003e9, 0x080003f1, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020171, 0x08020179, 0x080201a9, 0x080201b1, 0x080201c1, 0x080201c9, 0x080201e1, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x080202c1, 0x08020361, 0x08038211, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80c9, 0x080c80d1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8171, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8249, 0x080c8259, 0x080c8269, 0x080c8279, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c88d9, 0x080c88e9, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0x00000014
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x02008661
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe667
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe667
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00001999
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00030000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe667
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe667
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffd0000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00020000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe667
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe667
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x0000002e
	.4byte 0x020086f9
	.4byte 0x0000002e
	.4byte 0x02008795
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x000a0008
	.4byte 0x0008ffff
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x0009ffff
	.4byte 0xffff000a
	.4byte 0xffffffd0
	.4byte 0xffffffe0
	.4byte 0x00000030
	.4byte 0xffffffe0
	.4byte 0xffffffc0
	.4byte 0xfffffff0
	.4byte 0x00000040
	.4byte 0xfffffff0
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0xfffffff0
	.4byte 0xffffffd0
	.4byte 0x00000000
	.4byte 0x00000030
	.4byte 0x00000000
	.4byte 0xffffffe0
	.4byte 0x00000000
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xffffffc0
	.4byte 0x00000010
	.4byte 0x00000040
	.4byte 0x00000010
	.4byte 0xffffffe0
	.4byte 0x00000010
	.4byte 0x00000020
	.4byte 0x00000010
	.4byte 0xffffffd0
	.4byte 0x00000020
	.4byte 0x00000030
	.4byte 0x00000020
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000010
	.4byte 0x00000020
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
	.4byte 0x0200b4c0
	.2byte 0xb4fc
	.2byte 0x0200
	push	{r3, r4, r5, lr}
.L_0200384a:
	lsls	r0, r0, #8
	lsls	r1, r3, #1
	lsls	r2, r3, #1
	lsls	r3, r3, #1
	lsls	r4, r3, #1
	lsls	r5, r3, #1
	lsls	r6, r3, #1
	lsls	r7, r3, #1
	.2byte 0xffff
	.2byte 0x0017
.L_0200385e:
	movs	r1, r3
	movs	r6, r2
	movs	r7, r2
	movs	r1, r3
	movs	r6, r2
	movs	r7, r2
	movs	r1, r3
	movs	r6, r2
	movs	r7, r2
	movs	r1, r3
	movs	r6, r2
	movs	r7, r2
	movs	r1, r3
	movs	r6, r2
	movs	r7, r2
	movs	r1, r3
	movs	r6, r2
	movs	r7, r2
	movs	r1, r3
	movs	r6, r2
	.2byte 0xffff
	.2byte 0x001e
	movs	r3, r3
	movs	r7, r3
	movs	r6, r2
	movs	r5, r2
	movs	r7, r3
	movs	r4, r2
	movs	r2, r2
	movs	r7, r3
	movs	r5, r1
	movs	r3, r1
	movs	r3, r3
	movs	r3, r1
	movs	r0, r1
	movs	r0, r3
	movs	r0, r1
	movs	r5, r0
	movs	r5, r2
	movs	r7, r3
	movs	r7, r3
	movs	r7, r3
	.2byte 0xffff
	.2byte 0x005e
	lsls	r1, r3, #1
	lsls	r2, r3, #1
	lsls	r3, r3, #1
	lsls	r4, r3, #1
	lsls	r5, r3, #1
	lsls	r5, r3, #1
	lsls	r6, r3, #1
	lsls	r1, r3, #1
	lsls	r2, r3, #1
	lsls	r3, r3, #1
	lsls	r4, r3, #1
	lsls	r4, r3, #1
	lsls	r5, r3, #1
	lsls	r6, r3, #1
	lsls	r1, r3, #1
	lsls	r2, r3, #1
	lsls	r3, r3, #1
	lsls	r3, r3, #1
.L_020038da:
	lsls	r4, r3, #1
	lsls	r5, r3, #1
	lsls	r6, r3, #1
	lsls	r1, r3, #1
	lsls	r2, r3, #1
	lsls	r2, r3, #1
	lsls	r3, r3, #1
	lsls	r4, r3, #1
.L_020038ea:
	lsls	r5, r3, #1
	lsls	r6, r3, #1
	lsls	r1, r3, #1
	lsls	r1, r3, #1
	lsls	r2, r3, #1
	lsls	r3, r3, #1
	lsls	r4, r3, #1
	lsls	r5, r3, #1
	lsls	r6, r3, #1
	movs	r6, r5
	movs	r0, r0
	strh	r5, [r2, #50]
	lsls	r0, r0, #8
	movs	r1, r2
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x00ac
	movs	r0, r0
	lsls	r5, r2, #2
	ands	r0, r0
	movs	r0, r0
.L_02003916:
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
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
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r7, r5, #4
	movs	r0, r0
	movs	r1, #52
	asrs	r0, r2, #32
	.2byte 0xffff
	.2byte 0xffff
.L_02003944:
	add	r1, pc, #204
	asrs	r0, r4, #32
	.2byte 0xffff
	.2byte 0xffff
	adds	r0, #236
	asrs	r6, r6, #24
	.2byte 0xffff
	.2byte 0xffff
	lsls	r0, r6, #4
	movs	r0, r0
	str	r0, [r6, r4]
	asrs	r0, r2, #32
	.2byte 0xffff
	.2byte 0xffff
	stmia	r1!, {r4, r5}
	asrs	r0, r4, #32
	.2byte 0xffff
	.2byte 0xffff
	asrs	r0, r6, #4
	asrs	r1, r6, #32
	.2byte 0xffff
	.2byte 0xffff
	strh	r1, [r6, #8]
	asrs	r0, r0, #1
	.2byte 0xffff
	.2byte 0xffff
	asrs	r0, r6, #4
	asrs	r0, r2, #1
	.2byte 0xffff
	.2byte 0xffff
	movs	r1, #51
	asrs	r0, r4, #1
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xd130
	asrs	r0, r6, #1
	.2byte 0xffff
	.2byte 0xffff
	asrs	r3, r6, #4
	asrs	r0, r0, #2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xf130
	.2byte 0x1090
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xe130
	.2byte 0x10a0
	.4byte 0xffffffff
	.4byte 0x10b12130
	.4byte 0xffffffff
	.4byte 0x10c02130
	.4byte 0xffffffff
	.4byte 0x10d07130
	.4byte 0xffffffff
	.4byte 0x10e0a130
	.4byte 0xffffffff
	.4byte 0x10f09130
	.4byte 0xffffffff
	.4byte 0x11009133
	.4byte 0xffffffff
	.4byte 0x11103130
	.4byte 0xffffffff
	.4byte 0x00000131
	.4byte 0x1010d131
	.2byte 0xffff
	.2byte 0xffff
.L_020039ec:
	strb	r2, [r6, #4]
	asrs	r0, r4, #32
	.2byte 0xffff
	.2byte 0xffff
	b.n	.L_02003c5a
	.2byte 0x1030
	.4byte 0xffffffff
	.4byte 0x1040f131
	.4byte 0xffffffff
	.4byte 0x1050a131
	.4byte 0xffffffff
	.4byte 0x10607133
	.4byte 0xffffffff
	.4byte 0x10702132
	.4byte 0xffffffff
	.4byte 0x10804130
	.4byte 0xffffffff
	.4byte 0x10901132
	.4byte 0xffffffff
	.4byte 0x10a05131
	.4byte 0xffffffff
	.4byte 0x10b11131
	.4byte 0xffffffff
	.4byte 0x10c04133
	.4byte 0xffffffff
	.4byte 0x10d01131
	.4byte 0xffffffff
	.4byte 0x10e03131
	.4byte 0xffffffff
	.4byte 0x10f04131
	.4byte 0xffffffff
	.4byte 0x11003133
	.4byte 0xffffffff
	.4byte 0x1110b131
	.4byte 0xffffffff
	.4byte 0x00000132
	.4byte 0x10109131
	.4byte 0xffffffff
	.4byte 0x10207131
	.4byte 0xffffffff
	.4byte 0x10309132
	.4byte 0xffffffff
	.4byte 0x10408132
	.4byte 0xffffffff
	.4byte 0x10505134
	.4byte 0xffffffff
	.4byte 0x10605133
	.4byte 0xffffffff
	.4byte 0x10702131
	.4byte 0xffffffff
	.4byte 0x10804132
	.4byte 0xffffffff
	.4byte 0x10903132
	.4byte 0xffffffff
	.4byte 0x10a06133
	.4byte 0xffffffff
	.4byte 0x10b0b132
	.4byte 0xffffffff
	.4byte 0x10c0c132
	.4byte 0xffffffff
	.4byte 0x10d0c133
	.4byte 0xffffffff
	.4byte 0x00000133
	.4byte 0x10108130
	.4byte 0xffffffff
	.4byte 0x10206130
	.4byte 0xffffffff
	.4byte 0x10310131
	.4byte 0xffffffff
	.4byte 0x1040c131
	.4byte 0xffffffff
	.4byte 0x10506132
	.4byte 0xffffffff
	.4byte 0x1060a132
	.4byte 0xffffffff
	.4byte 0x10706131
	.4byte 0xffffffff
	.4byte 0x1080b133
	.4byte 0xffffffff
	.4byte 0x10910130
	.4byte 0xffffffff
	.4byte 0x10a0212f
	.4byte 0xffffffff
	.4byte 0x10b08133
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
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
	.4byte 0xffff0110
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff01b0
	.4byte 0x0200b8fc
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0110
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0110
	.4byte 0x00000007
	.2byte 0x0003
	.2byte 0x0000
.L_02003c04:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
.L_02003c18:
	movs	r4, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r5, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r6, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
.L_02003c58:
	lsls	r1, r5, #7
.L_02003c5a:
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r7, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02003c80:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r0, r6, #6
	.2byte 0xffff
	.2byte 0xb8fc
	lsls	r0, r0, #8
	movs	r3, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r4, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	lsls	r0, r2, #4
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r5, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
.L_02003cd6:
	.2byte 0x0000
.L_02003cd8:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r7, r5, #6
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	lsls	r7, r5, #6
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r2, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	lsls	r7, r5, #6
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r3, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02003d28:
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
	lsls	r0, r6, #6
	.2byte 0xffff
	.2byte 0xb8fc
	lsls	r0, r0, #8
	movs	r4, r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xc000
	lsls	r2, r0, #8
.L_02003d48:
	lsls	r5, r5, #6
	.2byte 0xffff
	.2byte 0xb8fc
	lsls	r0, r0, #8
	movs	r0, r0
	lsls	r0, r1, #9
	movs	r0, r0
	movs	r0, r0
.L_02003d58:
	movs	r0, r0
	lsls	r6, r0, #9
	strh	r0, [r0, #0]
	movs	r1, r0
	lsls	r5, r5, #6
	.2byte 0xffff
	.2byte 0xb8fc
	lsls	r0, r0, #8
	movs	r0, r0
	lsls	r0, r1, #9
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r6, r6, #9
	strh	r0, [r0, #0]
	lsls	r1, r0, #4
	lsls	r5, r5, #6
	.2byte 0xffff
	.2byte 0xb8fc
	lsls	r0, r0, #8
	movs	r0, r0
	lsls	r0, r5, #10
.L_02003d84:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r6, r0, #9
.L_02003d8c:
	movs	r0, r0
	lsls	r1, r0, #4
	lsls	r5, r5, #6
	.2byte 0xffff
	.2byte 0xb8fc
	lsls	r0, r0, #8
	movs	r0, r0
.L_02003d9a:
	lsls	r0, r5, #10
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r6, r6, #9
	movs	r0, r0
	lsls	r1, r0, #4
.L_02003da8:
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
	movs	r1, r0
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	movs	r6, r6
	.2byte 0xffff
	.2byte 0x9091
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r2, r6
	lsls	r0, r0, #8
	ldrh	r1, [r1, #22]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r3, r6
	lsls	r0, r0, #8
	ldrh	r1, [r1, #22]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r4, r6
	lsls	r0, r0, #8
	ldrh	r1, [r1, #22]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r5, r6
.L_02003e1a:
	lsls	r0, r0, #8
	ldrh	r1, [r1, #22]
	lsls	r0, r0, #8
	ldr	r4, [sp, #20]
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x9c61
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
	movs	r1, r0
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r4
	movs	r0, r0
	movs	r5, r0
.L_02003e6e:
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
.L_02003e82:
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
	movs	r1, r0
.L_02003e9a:
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
	movs	r1, r4
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x000c
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x000d
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r1
	.2byte 0xffff
	.2byte 0x000e
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r7, r1
	.2byte 0xffff
	.2byte 0x000f
	movs	r0, r0
	movs	r1, r4
	movs	r0, r0
	movs	r0, r2
	.2byte 0xffff
	.2byte 0x0010
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r1, r2
	.2byte 0xffff
	.2byte 0x0011
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	movs	r3, r1
	asrs	r7, r0, #8
	str	r5, [sp, #100]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r2, r6
	lsls	r0, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r3, r6
	lsls	r1, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r4, r6
	lsls	r2, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r5, r6
	lsls	r3, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r6, r6
	lsls	r4, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r7, r6
	lsls	r5, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r0, r7
	lsls	r6, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r0, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r1, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x9161
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
.L_02003f74:
	movs	r0, r1
	lsrs	r0, r0, #10
	str	r0, [sp, #628]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r0, r1
	lsrs	r0, r0, #10
.L_02003f84:
	str	r1, [sp, #388]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r2, r1
	lsrs	r1, r0, #10
.L_02003f90:
	str	r0, [sp, #628]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r2, r1
	lsrs	r1, r0, #10
	str	r1, [sp, #388]
	lsls	r0, r0, #8
.L_02003fa0:
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
	movs	r1, r6
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
.L_02003fce:
	movs	r0, r0
	movs	r1, r0
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
	movs	r1, r4
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0006
	movs	r0, r0
	movs	r1, r6
.L_02003ff6:
	movs	r0, r0
.L_02003ff8:
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	movs	r1, r4
.L_02004002:
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	movs	r1, r0
.L_0200400e:
	movs	r0, r0
.L_02004010:
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
.L_02004026:
	movs	r0, r0
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x000b
	movs	r0, r0
	movs	r1, r0
.L_02004032:
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x000c
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x000d
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r1
	.2byte 0xffff
	.2byte 0x000e
	movs	r0, r0
.L_02004054:
	movs	r1, r0
	movs	r0, r0
.L_02004058:
	movs	r7, r1
	.2byte 0xffff
	.2byte 0x000f
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02004064:
	movs	r0, r2
	.2byte 0xffff
	.2byte 0x0010
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r1, r2
	.2byte 0xffff
	.2byte 0x0011
.L_02004076:
	movs	r0, r0
	ldrh	r5, [r0, #28]
	movs	r0, r0
.L_0200407c:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x9615
	lsls	r0, r0, #8
	ldrh	r5, [r0, #28]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x9645
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r2, r6
.L_02004096:
	lsls	r0, r0, #8
	str	r6, [sp, #388]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r3, r6
	asrs	r0, r0, #8
	str	r6, [sp, #1012]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #56]
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x0000
	movs	r0, r0
	movs	r0, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r1, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x9161
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r0, r1
	lsrs	r2, r0, #10
	str	r0, [sp, #628]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r0, r1
	lsrs	r2, r0, #10
	str	r1, [sp, #388]
.L_020040e2:
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r2, r1
	lsrs	r3, r0, #10
	str	r0, [sp, #628]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r2, r1
	lsrs	r3, r0, #10
	str	r1, [sp, #388]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x9161
.L_02004112:
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r4, r1
.L_0200411a:
	lsrs	r5, r0, #10
	str	r0, [sp, #628]
.L_0200411e:
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r4, r1
	lsrs	r5, r0, #10
	str	r1, [sp, #388]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r5, r1
	lsrs	r4, r0, #10
.L_02004134:
	str	r0, [sp, #628]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r5, r1
	lsrs	r4, r0, #10
	str	r1, [sp, #388]
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
.L_02004166:
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
	movs	r1, r4
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
.L_020041a2:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0008
	movs	r0, r0
	movs	r1, r6
.L_020041b2:
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x0009
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_020041c0:
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x000a
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	movs	r3, r1
	asrs	r7, r0, #8
.L_020041d0:
	str	r5, [sp, #100]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r2, r6
	lsls	r0, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r3, r6
	lsls	r1, r0, #8
.L_020041e8:
	str	r3, [sp, #612]
.L_020041ea:
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
.L_020041f0:
	movs	r4, r6
	lsls	r2, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r5, r6
	lsls	r3, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
.L_02004204:
	movs	r2, r0
.L_02004206:
	movs	r0, r0
	movs	r6, r6
	lsls	r4, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
.L_02004212:
	movs	r0, r0
	movs	r7, r6
	lsls	r5, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r0, r7
	lsls	r6, r0, #8
	str	r3, [sp, #612]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r1, r4
	.2byte 0xffff
	.2byte 0x978d
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r2, r4
	.2byte 0xffff
	.2byte 0x978d
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r3, r4
	.2byte 0xffff
	.2byte 0x97a1
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r4, r4
	.2byte 0xffff
	.2byte 0x97a1
	lsls	r0, r0, #8
	ldrh	r5, [r0, #28]
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x9615
	lsls	r0, r0, #8
	ldrh	r5, [r0, #28]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x9645
	lsls	r0, r0, #8
	movs	r0, r1
	movs	r0, r0
.L_02004274:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r1, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x9161
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r1, r1
	lsrs	r6, r0, #10
	str	r0, [sp, #628]
.L_02004292:
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r1, r1
.L_0200429a:
	lsrs	r6, r0, #10
	str	r1, [sp, #388]
.L_0200429e:
	lsls	r0, r0, #8
.L_020042a0:
	ldrh	r5, [r2, #32]
.L_020042a2:
	asrs	r0, r0, #32
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
.L_020042ae:
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x9161
	lsls	r0, r0, #8
.L_020042b8:
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020042c2:
	.2byte 0x0000
.L_020042c4:
	movs	r1, r0
	movs	r0, r0
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
.L_020042e2:
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r0
.L_020042f6:
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
.L_0200430a:
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
.L_02004318:
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
	movs	r1, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x000a
.L_0200433a:
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x000b
.L_02004346:
	movs	r0, r0
.L_02004348:
	movs	r2, r0
	movs	r0, r0
	movs	r2, r6
	lsls	r0, r0, #8
	ldr	r1, [sp, #228]
.L_02004352:
	lsls	r0, r0, #8
.L_02004354:
	movs	r2, r0
	movs	r0, r0
	movs	r3, r6
	lsls	r0, r0, #8
.L_0200435c:
	ldr	r1, [sp, #228]
	lsls	r0, r0, #8
.L_02004360:
	movs	r2, r0
	movs	r0, r0
	movs	r4, r6
.L_02004366:
	lsls	r0, r0, #8
	ldr	r1, [sp, #228]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r6, r3
	lsrs	r4, r1, #10
	ldr	r3, [sp, #548]
	lsls	r0, r0, #8
	movs	r6, r0
	movs	r0, r0
	lsls	r0, r1, #3
	.2byte 0xffff
	.2byte 0x99a9
	lsls	r0, r0, #8
	movs	r6, r0
	movs	r0, r0
	lsls	r1, r1, #3
	.2byte 0xffff
	.2byte 0x9a95
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
	movs	r1, r0
	movs	r0, r0
.L_020043c4:
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
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020043e4:
	movs	r4, r6
	movs	r7, r0
	movs	r1, r0
	movs	r2, r0
	movs	r2, r0
.L_020043ee:
	movs	r6, r6
	movs	r7, r0
	movs	r1, r0
	movs	r2, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0012
	movs	r4, r0
	movs	r1, r0
	movs	r2, r0
.L_02004402:
	movs	r2, r0
	movs	r2, r2
	movs	r7, r0
	movs	r1, r0
	movs	r2, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0xc3fa
	lsls	r0, r0, #8
	movs	r3, r1
.L_02004416:
	movs	r4, r0
	stmia	r3!, {r1, r3, r4, r5, r6, r7}
	lsls	r0, r0, #8
	movs	r2, r3
	movs	r4, r0
	stmia	r3!, {r1, r3, r4, r5, r6, r7}
	lsls	r0, r0, #8
	movs	r5, r6
	movs	r4, r0
	.2byte 0xb78c
	lsls	r0, r0, #8
	.2byte 0xb7c8
	lsls	r0, r0, #8
	.2byte 0xb804
	lsls	r0, r0, #8
