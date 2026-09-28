.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200b699, 0x020087b9, 0x020087c5, 0x020087cd, 0x02008875, 0x020087c1, 0x0200b6ed
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
	bl 0x0200c1cc
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
	.4byte 0x0200e010
	.4byte 0x0200e014
	.4byte 0x0200e018
	.4byte 0x0200e004
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
	bl 0x0200bf7c
	b.n	.L_02000154
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0x0200e020
	.4byte 0x0200e01c
	.4byte 0x0200e010
	.4byte 0x0200e00c
	.4byte 0x0200e008
	.4byte 0x0200e000
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
	bl 0x0200bf84
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
	bl 0x0200bf7c
	b.n	.L_020001d0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0200e020
	.4byte 0x0200e01c
	.4byte 0x0200e010
	.4byte 0x0200e00c
	.4byte 0x0200e008
	.4byte 0x0200e000
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
	.4byte 0x0200e008
	.4byte 0x0200e00c
	.4byte 0x0200e018
	.4byte 0x0200e01c
	.4byte 0x0200e004
	.4byte 0x0200e020
	.4byte 0x0200e010
	.4byte 0x0200e000
	.2byte 0xe014
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
	.4byte 0x0200e018
	.4byte 0x0200e000
	.4byte 0x0200e020
	.2byte 0xe01c
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
	bl 0x0200c1cc
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200c1cc
	ldr	r2, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, r8
.L_0200035c:
	str	r3, [r2, r1]
	bl 0x0200c1e4
	bl 0x0200c1f4
	bl 0x02008178
	bl 0x0200c284
	movs	r0, #40
	bl 0x0200bf74
	bl 0x02008158
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x0200c1cc
	movs	r0, #16
	bl 0x0200c1dc
	movs	r0, #16
	bl 0x0200bf74
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
	bl 0x0200c08c
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000478
	cmp	r7, #0
	beq.n	.L_02000478
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000480
.L_02000478:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000480:
	mov	r3, sl
	bl 0x0200bfec
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200048e
	b.n	.L_020005da
.L_0200048e:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200bfdc
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200bfe4
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c014
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
.L_020004ce:
	adds	r0, r6, #0
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
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020005da
	cmp	r7, #0
	beq.n	.L_020005da
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000510
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200c124
.L_02000510:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000530
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x020083b0
.L_02000530:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000544
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02000544:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200058a
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02000572
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200bf5c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02000584
.L_02000572:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200bf5c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02000584:
	bl 0x0200bf5c
	str	r0, [r6, #52]
.L_0200058a:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020005a6
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200bfdc
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200bfe4
.L_020005a6:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005b8
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_020005b8:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005ca
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_020005ca:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020005da
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_020005da:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c74c
	.4byte 0x020083f9
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #16
	movs	r1, #0
	movs	r2, #14
	bl 0x0200c214
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #17
	movs	r1, #2
	movs	r2, #15
	bl 0x0200c214
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #230
	bl 0x0200bfcc
	pop	{pc}
	push	{r5, lr}
	ldmia	r0!, {r5}
	ldmia	r1!, {r3}
	ldmia	r0!, {r4}
	subs	r5, r5, r3
	ldmia	r1!, {r3}
	asrs	r5, r5, #16
	ldr	r2, [r1, #0]
	subs	r4, r4, r3
	ldr	r3, [r0, #0]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r0, r5, #0
	muls	r0, r5
	adds	r2, r4, #0
	muls	r2, r4
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #4]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd20
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	adds	r6, r1, #0
	adds	r7, r6, #0
	mov	r5, r8
	adds	r7, #8
	adds	r5, #8
	mov	sl, r2
	adds	r0, r7, #0
	movs	r2, #0
	adds	r1, r5, #0
	mov	r9, r2
	bl 0x02008620
	cmp	r0, sl
	bge.n	.L_020006c0
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r0, [r6, #16]
	ldr	r1, [r7, #0]
	subs	r0, r0, r3
	ldr	r3, [r5, #0]
	movs	r5, #128
	subs	r1, r1, r3
	bl 0x0200bf94
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldr	r3, [pc, #48]
	lsls	r5, r5, #5
	adds	r1, r0, r5
	mov	r5, r8
	ldrh	r2, [r5, #6]
	adds	r4, r0, r3
	movs	r3, #240
	lsls	r3, r3, #8
	ands	r4, r3
	ands	r1, r3
	ands	r0, r3
	ands	r3, r2
	cmp	r0, r3
	beq.n	.L_020006bc
	cmp	r1, r3
	beq.n	.L_020006bc
	cmp	r4, r3
	bne.n	.L_020006c0
.L_020006bc:
	movs	r2, #1
	mov	r9, r2
.L_020006c0:
	mov	r0, r9
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #196]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r7, #0]
	bl 0x0200c08c
	bl 0x0200c254
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl 0x0200c02c
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	adds	r5, r6, #0
	ands	r3, r2
	adds	r5, #91
	cmp	r3, #141
	bne.n	.L_02000726
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #19
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0200078e
.L_02000726:
	adds	r3, r6, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_0200078e
	ldr	r0, [r7, #0]
	bl 0x0200c08c
	adds	r1, r0, #0
	adds	r0, r6, #0
	adds	r0, #8
	adds	r1, #8
	bl 0x02008620
	cmp	r0, #11
	ble.n	.L_02000784
	adds	r3, r6, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_02000754
	movs	r0, #15
	b.n	.L_02000756
.L_02000754:
	movs	r0, #14
.L_02000756:
	bl 0x0200c08c
	adds	r1, r0, #0
	adds	r0, r6, #0
	movs	r2, #18
	bl 0x02008658
	cmp	r0, #0
	bne.n	.L_0200078e
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c08c
	movs	r2, #26
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl 0x02008658
	cmp	r0, #0
	bne.n	.L_0200078e
.L_02000784:
	movs	r3, #0
	adds	r0, r6, #0
	strb	r3, [r5, #0]
	movs	r1, #2
	b.n	.L_02000796
.L_0200078e:
	movs	r3, #1
	adds	r0, r6, #0
	strb	r3, [r5, #0]
	movs	r1, #1
.L_02000796:
	bl 0x0200bfdc
	movs	r0, #0
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #11
	bl 0x0200c174
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xcbc0
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xcbf0
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #116]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_020007e4
	ldr	r0, [pc, #104]
	b.n	.L_02000842
.L_020007e4:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_020007ee
	ldr	r0, [pc, #104]
	b.n	.L_02000842
.L_020007ee:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_02000836
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #4
	bne.n	.L_02000832
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000814
	ldr	r0, [pc, #72]
	b.n	.L_02000842
.L_02000814:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000832
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_02000832
	ldr	r0, [pc, #48]
	b.n	.L_02000842
.L_02000832:
	ldr	r0, [pc, #48]
	b.n	.L_02000842
.L_02000836:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000840
	ldr	r0, [pc, #44]
	b.n	.L_02000842
.L_02000840:
	ldr	r0, [pc, #44]
.L_02000842:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ec
	.4byte 0x0200cc88
	.4byte 0x000000ef
	.4byte 0x0200cd90
	.4byte 0x000000ed
	.4byte 0x0200d318
	.4byte 0x0200d228
	.4byte 0x0200d018
	.4byte 0x000000ee
	.4byte 0x0200d390
	.2byte 0xcc70
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #72]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_0200088c
	ldr	r0, [pc, #60]
	b.n	.L_020008be
.L_0200088c:
	ldr	r3, [pc, #60]
	cmp	r2, r3
.L_02000890:
	bne.n	.L_02000896
	ldr	r0, [pc, #60]
	b.n	.L_020008be
.L_02000896:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020008b2
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #4
	bne.n	.L_020008ae
	ldr	r0, [pc, #44]
	b.n	.L_020008be
.L_020008ae:
	ldr	r0, [pc, #44]
	b.n	.L_020008be
.L_020008b2:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020008bc
	ldr	r0, [pc, #40]
	b.n	.L_020008be
.L_020008bc:
	ldr	r0, [pc, #40]
.L_020008be:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ec
	.4byte 0x0200d4ec
	.4byte 0x000000ef
	.4byte 0x0200d870
	.4byte 0x000000ed
	.4byte 0x0200de70
	.4byte 0x0200da44
	.4byte 0x000000ee
	.4byte 0x0200ded0
	.2byte 0xd4e0
	.2byte 0x0200
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2924
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2925
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2926
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2927
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2928
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2936
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #16]
	bl 0x0200c12c
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c14c
	bl 0x0200c074
	pop	{pc}
	.2byte 0x28d3
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	bl 0x0200bf8c
	ldrh	r6, [r7, #6]
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r5, r0, #0
	adds	r0, r6, #0
	adds	r5, r5, r1
	bl 0x0200bfa4
	ldr	r2, [pc, #140]
	adds	r1, r0, #0
	mov	r8, r2
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4682
	adds	r0, r6, #0
	bl 0x0200bf9c
	adds	r1, r0, #0
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x68bb
	movs	r1, #255
	add	r3, sl
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	lsls	r1, r1, #8
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldrh	r3, [r7, #6]
	adds	r1, #240
	adds	r3, r3, r1
	strh	r3, [r7, #6]
	adds	r5, r7, #0
	adds	r5, #102
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02000a40
	subs	r3, r2, #1
	strh	r3, [r5, #0]
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	b.n	.L_02000a58
.L_02000a40:
	bl 0x0200bf8c
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_02000a58
	bl 0x0200bf8c
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	adds	r0, #8
	strh	r0, [r5, #0]
.L_02000a58:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r1, #202
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L_02000a72
	adds	r0, r7, #0
	bl 0x0200bff4
.L_02000a72:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, lr}
	ldr	r3, [pc, #100]
	ldr	r6, [r3, #0]
	movs	r3, #7
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02000ae4
	movs	r2, #204
	movs	r0, #154
	lsls	r2, r2, #8
	movs	r3, #232
	lsls	r0, r0, #1
	ldr	r1, [pc, #80]
	adds	r2, #204
	lsls	r3, r3, #15
	bl 0x0200c234
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000ae4
	movs	r1, #0
	bl 0x0200c014
	ldr	r3, [pc, #64]
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	bl 0x0200bf8c
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	negs	r0, r0
	lsls	r0, r0, #1
	adds	r3, r5, #0
	adds	r0, #20
	adds	r3, #100
	strh	r0, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #72]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r5, #40]
	bl 0x0200bf8c
	strh	r0, [r5, #6]
.L_02000ae4:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x02c20000
	.2byte 0x89d1
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000b18
	ldr	r0, [pc, #408]
	movs	r1, #1
	bl 0x0200c034
	b.n	.L_02000b20
.L_02000b18:
	ldr	r0, [pc, #400]
	movs	r1, #1
	bl 0x0200c034
.L_02000b20:
	movs	r0, #17
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200c1d4
	movs	r0, #129
	lsls	r0, r0, #14
	movs	r1, #1
	adds	r0, #132
	bl 0x0200c1cc
	movs	r0, #16
	bl 0x0200c1dc
	bl 0x0200c264
	movs	r0, #17
	bl 0x0200c08c
	movs	r1, #2
	bl 0x0200c27c
	movs	r0, #140
	movs	r3, #232
	lsls	r0, r0, #1
	ldr	r1, [pc, #340]
	movs	r2, #0
	lsls	r3, r3, #15
	bl 0x0200bfec
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000baa
	movs	r1, #0
	bl 0x0200c014
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200bfdc
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	movs	r5, #0
.L_02000b8c:
	ldr	r3, [r6, #24]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #102
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r5, #1
	bl 0x0200bf74
	cmp	r5, #27
	bls.n	.L_02000b8c
.L_02000baa:
	ldr	r5, [pc, #264]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200bf7c
	movs	r0, #120
	bl 0x0200c064
	adds	r0, r5, #0
	bl 0x0200bf84
	cmp	r6, #0
	beq.n	.L_02000bf2
	movs	r5, #0
.L_02000bc8:
	ldr	r3, [r6, #24]
	ldr	r2, [pc, #236]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	adds	r5, #1
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200bf74
	cmp	r5, #27
	bls.n	.L_02000bc8
	adds	r0, r6, #0
	bl 0x0200bff4
.L_02000bf2:
	movs	r0, #17
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c27c
	bl 0x0200c274
	bl 0x0200c26c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	bl 0x0200c1cc
	movs	r0, #16
	bl 0x0200c1dc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000c2c
	ldr	r0, [pc, #148]
	bl 0x0200c12c
	b.n	.L_02000c32
.L_02000c2c:
	ldr	r0, [pc, #144]
	bl 0x0200c12c
.L_02000c32:
	movs	r0, #17
	movs	r1, #2
	bl 0x0200c10c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #17
	movs	r1, #1
	bl 0x0200c0ec
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #14
	bl 0x0200c174
	movs	r1, #128
	movs	r2, #20
	movs	r0, #14
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c15c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #14
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #15
	movs	r1, #3
	bl 0x0200c0f4
	movs	r1, #208
	movs	r2, #0
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r1, #192
	movs	r0, #14
.L_02000c9a:
	lsls	r1, r1, #6
	bl 0x0200c15c
	bl 0x0200c074
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00002bfd
	.4byte 0x000028f2
	.4byte 0x02c20000
	.4byte 0x02008a81
	.4byte 0xfffff99a
	.4byte 0x00002bfe
	.2byte 0x28f3
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c08c
	movs	r2, #190
	ldrh	r3, [r0, #6]
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r3, r3, r2
	ldr	r2, [pc, #16]
	lsls	r3, r3, #16
	movs	r0, #1
	cmp	r3, r2
	bls.n	.L_02000cea
	movs	r0, #0
.L_02000cea:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x3ffe
	push	{lr}
	bl 0x02008cc4
	cmp	r0, #0
	beq.n	.L_02000d08
	movs	r0, #28
	movs	r1, #13
	bl 0x0200c294
	b.n	.L_02000d3a
.L_02000d08:
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000d28
	ldr	r0, [pc, #24]
	bl 0x0200c12c
	b.n	.L_02000d2e
.L_02000d28:
	ldr	r0, [pc, #20]
	bl 0x0200c12c
.L_02000d2e:
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c144
	bl 0x0200c074
.L_02000d3a:
	pop	{pc}
	.4byte 0x00002c3b
	.2byte 0x28fe
	.2byte 0x0000
	push	{lr}
	bl 0x02008cc4
	cmp	r0, #0
	beq.n	.L_02000d58
	movs	r0, #26
	movs	r1, #18
	bl 0x0200c294
	b.n	.L_02000d8a
.L_02000d58:
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000d78
	ldr	r0, [pc, #24]
	bl 0x0200c12c
	b.n	.L_02000d7e
.L_02000d78:
	ldr	r0, [pc, #20]
	bl 0x0200c12c
.L_02000d7e:
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c144
	bl 0x0200c074
.L_02000d8a:
	pop	{pc}
	.4byte 0x00002c37
	.2byte 0x28fa
	.2byte 0x0000
	push	{lr}
	bl 0x02008cc4
	cmp	r0, #0
	beq.n	.L_02000da8
	movs	r0, #27
	movs	r1, #19
	bl 0x0200c294
	b.n	.L_02000dda
.L_02000da8:
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000dc8
	ldr	r0, [pc, #24]
	bl 0x0200c12c
	b.n	.L_02000dce
.L_02000dc8:
	ldr	r0, [pc, #20]
	bl 0x0200c12c
.L_02000dce:
	movs	r0, #19
	movs	r1, #0
	bl 0x0200c144
.L_02000dd6:
	bl 0x0200c074
.L_02000dda:
	pop	{pc}
	.4byte 0x00002c39
	.2byte 0x28fc
	.2byte 0x0000
	push	{lr}
	bl 0x02008cc4
	cmp	r0, #0
	beq.n	.L_02000df8
	movs	r0, #12
	movs	r1, #20
	bl 0x0200c2a4
	b.n	.L_02000e40
.L_02000df8:
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000e18
	ldr	r0, [pc, #48]
	bl 0x0200c12c
	b.n	.L_02000e34
.L_02000e18:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000e2e
	ldr	r0, [pc, #32]
	bl 0x0200c12c
	b.n	.L_02000e34
.L_02000e2e:
	ldr	r0, [pc, #28]
	bl 0x0200c12c
.L_02000e34:
	movs	r0, #20
	movs	r1, #0
	bl 0x0200c144
	bl 0x0200c074
.L_02000e40:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002c3d
	.4byte 0x00002bae
	.2byte 0x2900
	.2byte 0x0000
	push	{lr}
	bl 0x02008cc4
	cmp	r0, #0
	beq.n	.L_02000e62
	movs	r0, #28
	bl 0x0200c29c
	b.n	.L_02000eaa
.L_02000e62:
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000e82
	ldr	r0, [pc, #48]
	bl 0x0200c12c
	b.n	.L_02000e9e
.L_02000e82:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02000e98
	ldr	r0, [pc, #28]
	bl 0x0200c12c
	b.n	.L_02000e9e
.L_02000e98:
	ldr	r0, [pc, #24]
	bl 0x0200c12c
.L_02000e9e:
	movs	r0, #28
	movs	r1, #0
	bl 0x0200c144
	bl 0x0200c074
.L_02000eaa:
	pop	{pc}
	.4byte 0x00002c45
	.4byte 0x00002bb6
	.2byte 0x290c
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #16]
	bl 0x0200c12c
	movs	r1, #0
	movs	r0, #10
	bl 0x0200c14c
	bl 0x0200c074
	pop	{pc}
	.2byte 0x13d8
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r5, [pc, #80]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200c034
	adds	r0, r5, #1
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #2
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #3
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #4
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #5
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #6
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #7
	movs	r1, #1
	adds	r5, #8
	bl 0x0200c034
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x13cf
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r5, [pc, #40]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200c034
	adds	r0, r5, #1
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #2
	movs	r1, #1
	adds	r5, #3
	bl 0x0200c034
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1389
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r5, [pc, #56]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200c034
	adds	r0, r5, #1
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #2
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #3
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #4
	movs	r1, #1
	adds	r5, #5
	bl 0x0200c034
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x138d
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #0
	bl 0x02009938
	bl 0x0200c074
	pop	{pc}
	push	{r5, lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r5, [pc, #56]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200c034
	adds	r0, r5, #1
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #2
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #3
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #4
	movs	r1, #1
	adds	r5, #5
	bl 0x0200c034
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x13b0
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r5, [pc, #48]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200c034
	adds	r0, r5, #1
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #2
	movs	r1, #1
	bl 0x0200c034
	adds	r0, r5, #3
	movs	r1, #1
	adds	r5, #4
	bl 0x0200c034
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c034
	bl 0x0200c074
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x13b6
	.2byte 0x0000
	push	{lr}
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r0, [pc, #16]
	bl 0x0200c12c
	movs	r1, #0
	movs	r0, #14
	bl 0x0200c14c
	bl 0x0200c074
	pop	{pc}
	.2byte 0x291a
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #14
	ldr	r7, [r3, #108]
	bl 0x0200c08c
	adds	r5, r0, #0
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	adds	r6, r5, #0
	mov	r8, r2
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r2, #179
	movs	r3, #1
	lsls	r2, r2, #1
	adds	r6, #99
	strb	r3, [r6, #0]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02001114
	ldr	r3, [pc, #160]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #14
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200c114
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_020010f6
	ldr	r0, [pc, #132]
	bl 0x0200c12c
	b.n	.L_02001146
.L_020010f6:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_0200110c
	ldr	r0, [pc, #112]
	bl 0x0200c12c
	b.n	.L_02001146
.L_0200110c:
	ldr	r0, [pc, #108]
	bl 0x0200c12c
	b.n	.L_02001146
.L_02001114:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_0200112a
	ldr	r0, [pc, #92]
	bl 0x0200c12c
	b.n	.L_02001146
.L_0200112a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02001140
	ldr	r0, [pc, #72]
	bl 0x0200c12c
	b.n	.L_02001146
.L_02001140:
	ldr	r0, [pc, #68]
	bl 0x0200c12c
.L_02001146:
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c144
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02001160
	mov	r3, r8
	strh	r3, [r5, #6]
.L_02001160:
	movs	r3, #0
	strb	r3, [r6, #0]
	bl 0x0200c074
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002c5e
	.4byte 0x00002c5a
	.4byte 0x000028db
	.4byte 0x00002c60
	.4byte 0x00002c5c
	.2byte 0x28e3
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #15
	ldr	r7, [r3, #108]
	bl 0x0200c08c
	adds	r5, r0, #0
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	adds	r6, r5, #0
	mov	r8, r2
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r2, #179
	movs	r3, #1
	lsls	r2, r2, #1
	adds	r6, #99
	strb	r3, [r6, #0]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020011f6
	ldr	r3, [pc, #116]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #15
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200c114
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_020011ee
	ldr	r0, [pc, #88]
	bl 0x0200c12c
	b.n	.L_02001212
.L_020011ee:
	ldr	r0, [pc, #84]
	bl 0x0200c12c
	b.n	.L_02001212
.L_020011f6:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_0200120c
	ldr	r0, [pc, #64]
	bl 0x0200c12c
	b.n	.L_02001212
.L_0200120c:
	ldr	r0, [pc, #60]
	bl 0x0200c12c
.L_02001212:
	movs	r0, #15
	movs	r1, #0
	bl 0x0200c144
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200122c
	mov	r3, r8
	strh	r3, [r5, #6]
.L_0200122c:
	movs	r3, #0
	strb	r3, [r6, #0]
	bl 0x0200c074
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002be9
	.4byte 0x000028dc
	.4byte 0x00002bef
	.2byte 0x28e4
	.2byte 0x0000
	push	{lr}
	bl 0x020080f4
	bl 0x0200c28c
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r1, #99
	ldr	r0, [pc, #20]
	bl 0x0200c1ac
	movs	r0, #40
	bl 0x0200c064
	bl 0x02008158
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x012f
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #8
	movs	r2, #54
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #3
	movs	r1, #50
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #2
	movs	r2, #43
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #4
	movs	r1, #43
	movs	r2, #2
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x0200c08c
	movs	r3, #17
	movs	r2, #37
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #37
	movs	r0, #16
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c23c
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #199
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_0200134c
	movs	r1, #140
	movs	r2, #166
	movs	r0, #66
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200c0d4
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #17
	bne.n	.L_0200134c
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #41
	bne.n	.L_0200134c
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r2, #16
	ldr	r0, [r6, #0]
	movs	r1, #0
	negs	r2, r2
	bl 0x0200c0c4
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x0200c0cc
.L_02001348:
	bl 0x0200c074
.L_0200134c:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #23
	movs	r2, #47
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #29
	movs	r1, #35
	movs	r2, #1
	movs	r3, #2
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #29
	movs	r2, #58
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #32
	movs	r1, #55
	movs	r2, #2
	movs	r3, #3
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #3
	movs	r2, #56
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #3
	movs	r1, #60
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #7
	movs	r2, #58
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #7
	movs	r1, #62
	movs	r2, #2
	movs	r3, #1
	bl 0x0200c23c
.L_020013d0:
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #10
	movs	r2, #59
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #10
	movs	r1, #63
	movs	r2, #2
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #40
	movs	r2, #57
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #32
	movs	r1, #55
	movs	r2, #3
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #39
	movs	r2, #58
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #32
	movs	r1, #55
	movs	r2, #5
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #39
	movs	r2, #59
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #32
	movs	r1, #55
	movs	r2, #5
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #39
	movs	r2, #60
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #32
	movs	r1, #55
	movs	r2, #5
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #40
	movs	r2, #61
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #32
	movs	r1, #55
	movs	r2, #3
	movs	r3, #1
	bl 0x0200c23c
	add	sp, #12
	pop	{pc}
	push	{lr}
	movs	r1, #128
	movs	r2, #220
	movs	r3, #128
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	lsls	r3, r3, #19
	bl 0x0200c1a4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bfcc
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #128
	movs	r2, #128
	movs	r3, #128
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	lsls	r3, r3, #19
	bl 0x0200c1a4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bfd4
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #62
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_02001524
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200c06c
	movs	r0, #1
	bl 0x0200c204
	movs	r1, #166
	movs	r2, #238
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	movs	r0, #16
	bl 0x0200c0d4
	movs	r0, #1
	bl 0x0200bf74
	ldr	r3, [pc, #32]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #16
	bl 0x0200c114
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #200
	strh	r3, [r2, #0]
	bl 0x0200c074
.L_02001524:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r5, #1
	movs	r6, #2
	movs	r0, #27
	movs	r1, #4
	movs	r2, #48
	movs	r3, #12
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c004
	movs	r0, #27
	movs	r1, #6
	movs	r2, #49
	movs	r3, #12
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c004
	movs	r3, #48
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #48
	movs	r1, #1
	movs	r2, #2
	movs	r3, #1
	bl 0x0200c00c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r5, #1
	movs	r6, #2
	movs	r0, #27
	movs	r1, #8
	movs	r2, #48
	movs	r3, #12
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c004
	movs	r0, #27
	movs	r1, #10
	movs	r2, #49
	movs	r3, #12
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c004
	movs	r3, #48
	movs	r2, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #48
	movs	r1, #0
	movs	r2, #2
	movs	r3, #1
	bl 0x0200c00c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #17
	sub	sp, #68
	bl 0x0200c08c
	adds	r7, r0, #0
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r1, #224
	lsls	r1, r1, #14
	ldr	r2, [pc, #332]
	movs	r0, #17
	bl 0x0200c0d4
	movs	r0, #1
	bl 0x0200bf74
	add	r2, sp, #28
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r0, #188
	mov	r9, r2
	bl 0x0200c2b4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c01c
	movs	r3, #0
	mov	sl, r3
.L_020015fe:
	mov	r4, sl
	lsls	r6, r4, #12
	adds	r0, r6, #0
	bl 0x0200bfa4
	movs	r3, #0
	add	r5, sp, #16
	str	r3, [r5, #4]
	str	r0, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200bf9c
	ldr	r6, [r5, #0]
	mov	r8, r0
	str	r0, [r5, #8]
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x0200bf5c
	ldr	r3, [r5, #4]
	movs	r4, #200
	lsls	r4, r4, #5
	adds	r6, r6, r0
	adds	r4, #153
	str	r6, [r5, #0]
	adds	r3, r3, r4
	ldr	r2, [r7, #16]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r4, #128
	mov	r3, r9
	lsls	r4, r4, #10
	str	r3, [sp, #12]
	adds	r3, r6, #0
	mov	r8, r4
	str	r4, [sp, #8]
	bl 0x02008430
	movs	r4, #1
	add	sl, r4
	mov	r2, sl
	cmp	r2, #16
	bls.n	.L_020015fe
	movs	r0, #17
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c124
	movs	r0, #17
	bl 0x0200c08c
	movs	r1, #1
	bl 0x0200c014
	movs	r1, #192
	movs	r2, #192
	movs	r0, #17
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200c094
	movs	r3, #192
	lsls	r3, r3, #12
	movs	r2, #170
	lsls	r2, r2, #2
	str	r3, [r7, #40]
	movs	r1, #40
	movs	r0, #17
	bl 0x0200c0b4
	movs	r0, #10
	bl 0x0200c064
	ldr	r3, [pc, #132]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r3, r4
	ldr	r0, [r5, #0]
	bl 0x0200c08c
	ldr	r2, [r7, #8]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020016e0
	ldr	r2, [r7, #16]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020016e0
	ldr	r0, [r5, #0]
	movs	r1, #6
	movs	r2, #0
	bl 0x0200c104
	movs	r2, #128
	ldr	r0, [r5, #0]
	mov	r1, r8
	lsls	r2, r2, #9
	bl 0x0200c094
	movs	r2, #170
	ldr	r0, [r5, #0]
	movs	r1, #68
	lsls	r2, r2, #2
	bl 0x0200c0b4
.L_020016e0:
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c01c
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #17
	movs	r2, #0
	bl 0x0200c11c
	movs	r0, #10
	bl 0x0200c064
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #231
	bl 0x0200bfcc
	bl 0x0200c074
	add	sp, #68
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02ba0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r5, #1
	movs	r6, #2
	movs	r0, #0
	movs	r1, #1
	movs	r2, #26
	movs	r3, #42
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c004
	movs	r0, #0
	movs	r1, #1
	movs	r2, #26
	movs	r3, #48
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c004
	movs	r0, #0
	movs	r1, #1
	movs	r2, #11
	movs	r3, #54
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c004
	movs	r0, #0
	movs	r1, #1
	movs	r2, #16
	movs	r3, #54
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c004
	movs	r3, #45
	str	r3, [sp, #4]
	movs	r5, #26
	movs	r0, #0
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200c00c
	movs	r3, #51
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200c00c
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r5, #57
	movs	r0, #0
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200c00c
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200c00c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200c08c
	movs	r1, #3
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200c16c
	adds	r2, r6, #0
	adds	r2, #89
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r3, r6, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r6, #12]
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #19
	movs	r1, #0
	bl 0x0200c0ec
	movs	r0, #20
	movs	r1, #1
	bl 0x0200c0ec
	movs	r0, #21
	movs	r1, #2
	bl 0x0200c0ec
	movs	r0, #22
	movs	r1, #0
	bl 0x0200c0ec
	movs	r0, #23
	movs	r1, #1
	bl 0x0200c0ec
	movs	r0, #24
	movs	r1, #2
	bl 0x0200c0ec
	movs	r0, #25
	movs	r1, #3
	bl 0x0200c0ec
	movs	r1, #4
	movs	r0, #26
	bl 0x0200c0ec
	movs	r0, #19
	bl 0x020097bc
	movs	r0, #20
	bl 0x020097bc
	movs	r0, #21
	bl 0x020097bc
	movs	r0, #22
	bl 0x020097bc
	movs	r0, #23
	bl 0x020097bc
	movs	r0, #24
	bl 0x020097bc
	movs	r0, #25
	bl 0x020097bc
	movs	r0, #26
	bl 0x020097bc
	movs	r0, #27
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
	movs	r0, #28
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
	movs	r0, #29
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
	movs	r0, #30
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
	movs	r0, #31
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
	movs	r0, #32
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
	movs	r0, #33
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
	movs	r0, #27
	bl 0x0200c08c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #8
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #28
	bl 0x0200c08c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200c08c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #30
	bl 0x0200c08c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #31
	bl 0x0200c08c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #32
	bl 0x0200c08c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #33
	bl 0x0200c08c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r1, #0
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #27
	bl 0x0200c0ec
	movs	r0, #32
	movs	r1, #0
	bl 0x0200c0ec
	movs	r0, #33
	movs	r1, #0
	bl 0x0200c0ec
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r0, #228
	bl 0x0200c054
	adds	r6, r0, #0
	bl 0x0200c22c
	cmp	r5, #0
	bne.n	.L_020019d2
	ldr	r7, [pc, #240]
	adds	r0, r7, #0
	bl 0x0200c12c
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c144
	cmp	r6, #0
	beq.n	.L_020019fa
	adds	r0, r7, #2
	bl 0x0200c12c
	adds	r0, r6, #0
	movs	r1, #5
	bl 0x0200c03c
	movs	r1, #0
	movs	r0, #18
	bl 0x0200c134
	ldr	r3, [pc, #204]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c084
	cmp	r0, #1
	beq.n	.L_020019fa
	bl 0x0200c05c
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_020019a4
	adds	r0, r7, #4
	bl 0x0200c12c
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c134
	b.n	.L_020019b6
.L_020019a4:
	cmp	r5, #6
	bgt.n	.L_02001a04
	adds	r0, r7, #5
	bl 0x0200c12c
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c134
.L_020019b6:
	cmp	r5, #6
	bgt.n	.L_02001a04
	ldr	r3, [pc, #136]
.L_020019bc:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c084
	cmp	r0, #1
	bne.n	.L_02001a04
	ldr	r0, [pc, #120]
	b.n	.L_020019f6
.L_020019d2:
	cmp	r6, #0
	bne.n	.L_020019da
	ldr	r0, [pc, #116]
	b.n	.L_020019f6
.L_020019da:
	ldr	r0, [pc, #116]
	bl 0x0200c12c
	movs	r1, #0
	movs	r0, #18
	bl 0x0200c134
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c084
	cmp	r0, #1
	bne.n	.L_02001a04
	ldr	r0, [pc, #92]
.L_020019f6:
	bl 0x0200c12c
.L_020019fa:
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c144
	b.n	.L_02001a3c
.L_02001a04:
	ldr	r0, [pc, #80]
	bl 0x0200c12c
	movs	r1, #0
.L_02001a0c:
	movs	r0, #18
	bl 0x0200c144
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bfcc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #229
	bl 0x0200bfcc
	movs	r0, #1
	bl 0x0200bf74
	movs	r0, #254
	lsls	r0, r0, #1
	movs	r1, #0
	bl 0x0200c1c4
	ldr	r0, [pc, #36]
	movs	r1, #20
	bl 0x0200c1bc
.L_02001a3c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0000139f
	.4byte 0x02000240
	.4byte 0x000013a5
	.4byte 0x000013ae
	.4byte 0x000013af
	.4byte 0x000013ad
	.4byte 0x000013a6
	.2byte 0x00ee
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r7, #0
	blt.n	.L_02001aae
	cmp	r7, #5
	bne.n	.L_02001a7c
	bl 0x0200bf8c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsrs	r7, r3, #16
.L_02001a7c:
	ldr	r3, [pc, #52]
.L_02001a7e:
	mov	r8, r3
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r7, r3
	mov	r3, r8
	ldrsb	r5, [r3, r6]
	bl 0x0200bf8c
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r5, r5, r0
	adds	r5, #4
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200bf64
	mov	r3, r8
	strb	r0, [r3, r6]
	lsls	r3, r7, #1
	ldr	r2, [pc, #16]
	adds	r3, r3, r7
	adds	r3, r3, r0
	lsls	r3, r3, #2
.L_02001aac:
	ldr	r0, [r2, r3]
.L_02001aae:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xc630
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200c04c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r5, r3, #1
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r6, [pc, #156]
	ldr	r3, [r6, #16]
	cmp	r3, r5
	bcs.n	.L_02001aea
	ldr	r0, [pc, #152]
	bl 0x0200c12c
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c134
	b.n	.L_02001b6e
.L_02001aea:
	ldr	r0, [pc, #140]
	bl 0x0200c12c
	adds	r0, r5, #0
	movs	r1, #5
	bl 0x0200c03c
	movs	r1, #0
	movs	r0, #14
	bl 0x0200c134
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c084
	cmp	r0, #0
	bne.n	.L_02001b50
	ldr	r3, [pc, #104]
	ldr	r2, [r6, #16]
	str	r2, [r3, #0]
.L_02001b18:
	bl 0x0200c22c
	movs	r1, #0
	movs	r0, #14
	bl 0x0200c144
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bfcc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #229
	bl 0x0200bfcc
	movs	r0, #1
	bl 0x0200bf74
	movs	r0, #254
	adds	r0, #255
	movs	r1, #0
	bl 0x0200c1c4
	ldr	r0, [pc, #56]
	movs	r1, #19
	bl 0x0200c1bc
	b.n	.L_02001b6a
.L_02001b50:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #14
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200c144
.L_02001b6a:
	bl 0x0200c074
.L_02001b6e:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x00001386
	.4byte 0x00001382
	.4byte 0x0200234c
	.2byte 0x00ee
	.2byte 0x0000
	push	{r5, r6, lr}
	bl 0x0200c04c
	lsls	r5, r0, #2
	adds	r5, r5, r0
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	ldr	r6, [pc, #148]
	lsls	r5, r5, #1
	ldr	r3, [r6, #16]
	cmp	r3, r5
	bcs.n	.L_02001bb2
	ldr	r0, [pc, #144]
	bl 0x0200c12c
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c134
	b.n	.L_02001c2e
.L_02001bb2:
	ldr	r0, [pc, #132]
	bl 0x0200c12c
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c134
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c084
	cmp	r0, #0
	bne.n	.L_02001c10
	ldr	r3, [pc, #104]
	ldr	r2, [r6, #16]
	str	r2, [r3, #0]
	bl 0x0200c22c
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c144
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bfcc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #229
	bl 0x0200bfcc
	movs	r0, #1
	bl 0x0200bf74
	movs	r0, #252
	adds	r0, #255
	movs	r1, #0
	bl 0x0200c1c4
	ldr	r0, [pc, #56]
	movs	r1, #18
	bl 0x0200c1bc
	b.n	.L_02001c2a
.L_02001c10:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #8
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200c144
.L_02001c2a:
	bl 0x0200c074
.L_02001c2e:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x000013e2
	.4byte 0x000013cb
	.4byte 0x0200234c
	.2byte 0x00ee
	.2byte 0x0000
	push	{lr}
	bl 0x0200c08c
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	movs	r3, #8
	strh	r3, [r0, #32]
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #8
	mov	fp, r3
	mov	r9, r0
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #188
	bl 0x0200c2b4
	movs	r3, #1
	mov	r8, r3
	movs	r3, #2
	mov	sl, r3
	mov	r3, r8
	str	r3, [sp, #0]
	mov	r3, sl
	str	r3, [sp, #4]
	mov	r0, r9
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	r3, fp
	bl 0x0200c004
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r5, #2
	mov	r3, sl
	adds	r6, #1
	str	r3, [sp, #4]
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	r3, fp
	mov	r0, r9
	bl 0x0200c004
	movs	r0, #20
	bl 0x0200c064
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c094
	ldr	r1, [pc, #44]
	ldr	r0, [r5, #0]
	bl 0x0200c09c
	movs	r0, #12
	bl 0x0200bf74
	movs	r0, #123
	bl 0x0200c2b4
	bl 0x0200c1ec
	bl 0x0200c1f4
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0xc66c
	.2byte 0x0200
	push	{lr}
	movs	r0, #27
	movs	r1, #0
	movs	r2, #42
	movs	r3, #13
	bl 0x02009c5c
	movs	r0, #22
	bl 0x0200c1b4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #27
	movs	r1, #4
	movs	r2, #48
	movs	r3, #12
	bl 0x02009c5c
	movs	r0, #23
	bl 0x0200c1b4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #27
	movs	r1, #0
	movs	r2, #54
	movs	r3, #13
	bl 0x02009c5c
	movs	r0, #24
	bl 0x0200c1b4
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #8
	sub	sp, #28
	bl 0x0200c08c
	adds	r7, r0, #0
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #78
	bl 0x0200c2b4
	bl 0x0200c1e4
	bl 0x0200c1f4
	ldr	r5, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r1, [r5, #0]
	movs	r0, #0
	bl 0x0200c224
	ldr	r1, [r5, #0]
	movs	r0, #1
	bl 0x0200c224
	ldr	r1, [r5, #0]
	movs	r0, #3
	bl 0x0200c224
	ldr	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200c224
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200c224
	movs	r0, #10
	bl 0x0200c064
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r2, #191
	ldr	r0, [r5, #0]
	movs	r1, #160
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200c154
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x0200c0e4
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200c0e4
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200c0e4
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200c0e4
	movs	r0, #1
	bl 0x0200bf74
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #6
	adds	r1, #204
	bl 0x0200c094
	ldr	r1, [pc, #816]
	movs	r0, #5
	bl 0x0200c09c
	ldr	r1, [pc, #812]
	movs	r0, #9
	bl 0x0200c09c
	ldr	r1, [pc, #808]
	movs	r0, #6
	bl 0x0200c09c
	ldr	r1, [pc, #804]
	movs	r0, #7
	bl 0x0200c0ac
	ldr	r0, [r5, #0]
	bl 0x0200c0a4
	movs	r0, #1
	bl 0x0200c0a4
	movs	r0, #3
	bl 0x0200c0a4
	movs	r0, #2
	bl 0x0200c0a4
	movs	r0, #8
	bl 0x0200c0a4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	negs	r1, r1
	negs	r0, r0
	bl 0x0200c194
	movs	r0, #43
	bl 0x0200c2b4
	movs	r0, #20
	bl 0x0200c064
	movs	r1, #3
	movs	r0, #0
	bl 0x0200c0f4
	ldr	r0, [pc, #732]
	bl 0x0200c12c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #1
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200c17c
	movs	r0, #20
	bl 0x0200c064
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #3
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #2
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #0
	bl 0x0200c174
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #192
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c18c
	movs	r0, #148
	movs	r1, #1
	movs	r2, #184
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c194
	movs	r2, #171
	movs	r0, #9
	movs	r1, #172
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r2, #171
	lsls	r2, r2, #1
	movs	r0, #9
	movs	r1, #146
	bl 0x0200c0bc
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r2, #10
	movs	r0, #1
	movs	r1, #2
	bl 0x0200c104
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #1
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #3
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #3
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200c174
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #1
	bl 0x0200c174
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #9
	movs	r1, #4
	bl 0x0200c0f4
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #192
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #5
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #1
	bl 0x0200c174
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x0200c174
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #1
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #9
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #10
	adds	r1, #255
	movs	r2, #20
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #0
	movs	r1, #0
	movs	r2, #40
	bl 0x0200c13c
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #2
	bl 0x0200c174
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #3
	bl 0x0200c174
	movs	r1, #128
	movs	r0, #3
	b.n	.L_0200218c
	.4byte 0x02000240
	.4byte 0x0200c8a0
	.4byte 0x0200c980
	.4byte 0x0200c93c
	.4byte 0x0200c8f8
	.2byte 0x2b5d
	.2byte 0x0000
.L_0200218c:
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #2
	movs	r1, #3
	bl 0x0200c0f4
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x0200c174
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #3
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #1
	bl 0x0200c174
	movs	r1, #224
	movs	r0, #1
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #3
	bl 0x0200c174
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #6
	bl 0x0200c15c
	movs	r1, #0
	movs	r0, #3
	bl 0x0200c134
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c154
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c084
	ldr	r5, [r5, #0]
	cmp	r0, #0
	bne.n	.L_0200228a
	adds	r0, r5, #0
	bl 0x0200c0fc
	movs	r0, #2
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020022b2
.L_0200228a:
	adds	r0, r5, #0
	bl 0x0200c0fc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #2
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200c0ec
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c144
.L_020022b2:
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200c0f4
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	ldr	r5, [pc, #548]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #3
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x0200c174
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #224
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r2, #20
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #7
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #1
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #3
	bl 0x0200c174
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #2
	bl 0x0200c174
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200c154
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #3
	bl 0x0200c174
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #224
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #9
	bl 0x0200c174
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #1
	bl 0x0200c17c
	movs	r0, #20
	bl 0x0200c064
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0f4
	movs	r2, #80
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c13c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #9
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	bl 0x0200c15c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #1
	bl 0x0200c134
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c154
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c084
	ldr	r5, [r5, #0]
	cmp	r0, #0
	bne.n	.L_020024f8
	adds	r0, r5, #0
	bl 0x0200c0fc
	movs	r1, #129
	movs	r0, #1
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200c144
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002528
	.2byte 0x0240
	.2byte 0x0200
.L_020024f8:
	adds	r0, r5, #0
	bl 0x0200c0fc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #1
	bl 0x0200c174
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #7
	strh	r3, [r2, #0]
	adds	r0, #1
	movs	r1, #0
	bl 0x0200c144
.L_02002528:
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #0
	bl 0x0200c174
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #1
	bl 0x0200c174
	movs	r0, #0
	bl 0x0200c2b4
	movs	r0, #8
	bl 0x0200c08c
	movs	r1, #15
	bl 0x0200c124
	movs	r2, #176
	lsls	r2, r2, #8
	mov	r8, r2
	movs	r1, #160
	movs	r2, #233
	lsls	r2, r2, #17
	mov	r3, r8
	lsls	r1, r1, #16
	movs	r0, #8
	bl 0x0200c0dc
	movs	r0, #1
	bl 0x0200bf74
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	ldr	r6, [pc, #1020]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	ldr	r0, [r6, #0]
	movs	r1, #8
	bl 0x0200c224
	movs	r0, #5
	movs	r1, #8
	bl 0x0200c224
	movs	r0, #7
	movs	r1, #8
	bl 0x0200c224
	movs	r0, #6
	movs	r1, #8
	bl 0x0200c224
	movs	r0, #0
	movs	r1, #8
	bl 0x0200c224
	movs	r0, #1
	movs	r1, #8
	bl 0x0200c224
	movs	r0, #3
	movs	r1, #8
	bl 0x0200c224
	movs	r1, #8
	movs	r0, #2
	bl 0x0200c224
	movs	r0, #8
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c124
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #936]
	adds	r1, #204
	bl 0x0200c18c
	movs	r0, #180
	movs	r1, #1
	movs	r2, #205
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c194
	bl 0x0200c19c
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c18c
	movs	r0, #148
	movs	r1, #1
	movs	r2, #184
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c194
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r2, #203
	movs	r0, #8
	movs	r1, #146
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #3
	bl 0x0200c174
	movs	r1, #0
	movs	r0, #3
	bl 0x0200c144
	ldr	r0, [r6, #0]
	bl 0x0200c0a4
	movs	r0, #5
	bl 0x0200c0a4
	movs	r0, #7
	bl 0x0200c0a4
	movs	r0, #6
	bl 0x0200c0a4
	movs	r0, #0
	bl 0x0200c0a4
	movs	r0, #1
	bl 0x0200c0a4
	movs	r0, #3
	bl 0x0200c0a4
	movs	r0, #2
	bl 0x0200c0a4
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c2b4
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #1
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #3
	movs	r1, #3
	bl 0x0200c0ec
	movs	r1, #3
	movs	r0, #2
	bl 0x0200c0f4
	movs	r0, #20
	bl 0x0200c064
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x0200c174
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r1, #3
	movs	r0, #0
	bl 0x0200c0ec
	movs	r0, #10
	bl 0x0200c064
	movs	r3, #10
	movs	r2, #8
	movs	r1, #9
	movs	r0, #20
	movs	r4, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	str	r0, [sp, #12]
	str	r3, [sp, #20]
	str	r3, [sp, #24]
	movs	r0, #0
	movs	r3, #8
	movs	r1, #5
	movs	r2, #5
	str	r4, [sp, #16]
	bl 0x0200c164
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r6, #0]
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #1
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #3
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #2
	bl 0x0200c174
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200c174
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #1
	bl 0x0200c17c
	movs	r0, #20
	bl 0x0200c064
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #8
	bl 0x0200c174
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c15c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #2
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r2, #200
	movs	r0, #2
	movs	r1, #130
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r2, #0
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #2
	bl 0x0200c174
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #3
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r2, #40
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
.L_02002910:
	bl 0x0200c15c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200c154
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c13c
	ldr	r5, [pc, #76]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl 0x0200be54
	movs	r0, #80
	bl 0x0200c064
	adds	r0, r7, #0
	bl 0x0200bf54
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c0f4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c13c
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl 0x0200be54
	b.n	.L_020029dc
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00026666
	.2byte 0xe030
	.2byte 0x0200
.L_020029dc:
	movs	r0, #80
	bl 0x0200c064
	adds	r0, r7, #0
	bl 0x0200bf54
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	movs	r0, #0
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #129
	movs	r0, #1
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #129
	movs	r0, #3
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #2
	bl 0x0200c17c
	movs	r0, #20
	bl 0x0200c064
	movs	r0, #8
	mov	r1, r8
	bl 0x0200c15c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #3
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r2, #10
	movs	r0, #9
	movs	r1, #4
	bl 0x0200c104
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #1
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #3
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #2
	movs	r1, #3
	bl 0x0200c0ec
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c0ec
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200c174
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c0ec
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c144
	movs	r0, #78
	bl 0x0200c2b4
	movs	r0, #180
	movs	r1, #1
	movs	r2, #205
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c194
	movs	r2, #233
	movs	r0, #8
	movs	r1, #160
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c0d4
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #656]
	adds	r1, #204
	bl 0x0200c18c
	movs	r0, #148
	movs	r1, #1
	movs	r2, #184
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c194
	bl 0x0200c19c
	bl 0x0200c20c
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200c174
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200c154
	movs	r2, #0
	movs	r0, #1
	movs	r1, #0
	bl 0x0200c154
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c15c
	movs	r0, #1
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c0f4
	movs	r2, #0
	movs	r0, #5
	movs	r1, #2
	bl 0x0200c104
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #3
	bl 0x0200c174
	movs	r0, #3
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #7
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #1
	bl 0x0200c174
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200c174
	movs	r1, #128
	movs	r2, #20
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200c154
	movs	r1, #0
	movs	r0, #5
	bl 0x0200c134
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200c084
	cmp	r0, #0
	bne.n	.L_02002d02
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002d32
.L_02002d02:
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	bl 0x0200c15c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #9
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c144
.L_02002d32:
	movs	r2, #184
	movs	r0, #9
	movs	r1, #146
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200c154
	movs	r2, #40
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c154
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #1
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #3
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #2
	movs	r1, #3
	bl 0x0200c0ec
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #5
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #6
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #7
	movs	r1, #3
	bl 0x0200c0f4
	ldr	r5, [pc, #96]
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x0200c09c
	adds	r1, r5, #0
	movs	r0, #1
	bl 0x0200c09c
	adds	r1, r5, #0
	movs	r0, #3
	bl 0x0200c09c
	adds	r1, r5, #0
	movs	r0, #2
	bl 0x0200c09c
	adds	r1, r5, #0
	movs	r0, #9
	bl 0x0200c09c
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x0200c09c
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x0200c09c
	adds	r1, r5, #0
	movs	r0, #7
	bl 0x0200c0ac
	bl 0x0200c044
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x0200bfcc
	bl 0x0200c074
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x00026666
	.4byte 0x02000240
	.2byte 0xc9c4
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_02002e2c
	bl 0x0200b68a
.L_02002e2c:
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200c18c
	movs	r0, #160
	movs	r1, #1
	movs	r2, #184
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200c194
	ldr	r5, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r2, #197
	ldr	r0, [r5, #0]
	movs	r1, #158
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r1, #160
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200c154
	ldr	r1, [r5, #0]
	movs	r0, #2
	bl 0x0200c0e4
	movs	r0, #1
	bl 0x0200bf74
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #2
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r2, #187
	lsls	r2, r2, #1
	movs	r0, #2
	movs	r1, #144
	bl 0x0200c0bc
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200c174
	ldr	r0, [pc, #904]
	bl 0x0200c12c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #2
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #2
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200c154
	movs	r2, #182
	movs	r1, #180
	lsls	r2, r2, #1
	movs	r0, #2
	bl 0x0200c0bc
	movs	r0, #20
	bl 0x0200c064
	movs	r2, #194
	movs	r1, #186
	lsls	r2, r2, #1
	movs	r0, #2
	bl 0x0200c0bc
	movs	r0, #40
	bl 0x0200c064
	movs	r2, #190
	movs	r0, #2
	movs	r1, #146
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #2
	bl 0x0200c174
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200c174
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #2
	bl 0x0200c174
	movs	r1, #224
	movs	r2, #40
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #2
	bl 0x0200c174
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	movs	r0, #2
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200c174
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	ldr	r1, [r5, #0]
	movs	r0, #0
	bl 0x0200c0e4
	movs	r0, #1
	bl 0x0200bf74
.L_02002ff6:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
.L_02002ffc:
	movs	r0, #0
	bl 0x0200c174
.L_02003002:
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #0
.L_02003008:
	ldr	r1, [pc, #592]
	adds	r2, #153
	bl 0x0200c094
	movs	r2, #190
	lsls	r2, r2, #1
.L_02003014:
	movs	r0, #0
	movs	r1, #168
	bl 0x0200c0bc
	movs	r1, #160
	movs	r0, #0
.L_02003020:
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #8
	bl 0x0200c174
	movs	r1, #192
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200c154
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200c0e4
	movs	r0, #1
	bl 0x0200bf74
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	ldr	r0, [r5, #0]
	adds	r1, #204
	bl 0x0200c094
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #500]
	bl 0x0200c09c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c094
	movs	r2, #189
	movs	r0, #9
	movs	r1, #182
	lsls	r2, r2, #1
	bl 0x0200c0bc
	movs	r2, #180
	lsls	r2, r2, #1
	movs	r0, #9
	movs	r1, #182
	bl 0x0200c0bc
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x0200c174
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c154
	movs	r1, #129
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x0200c174
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200c174
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #2
	bl 0x0200c174
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c15c
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c0f4
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200c154
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #8
	bl 0x0200c174
	movs	r1, #160
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200c154
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #40
	bl 0x0200c154
	movs	r1, #128
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c154
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200c15c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #2
	bl 0x0200c174
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200c174
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c0ec
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #160
	b.n	.L_02003264
	.4byte 0x02000240
	.4byte 0x00002c05
	.4byte 0x00013333
	.2byte 0xcac0
	.2byte 0x0200
.L_02003264:
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r2, #40
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	bl 0x0200c154
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x0200c17c
	movs	r0, #20
	bl 0x0200c064
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	movs	r0, #8
	bl 0x0200c174
	movs	r1, #192
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200c154
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #2
	movs	r1, #1
	bl 0x0200c10c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #6
	adds	r1, #255
	movs	r2, #20
	movs	r0, #9
	bl 0x0200c174
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	movs	r0, #8
	movs	r1, #0
	movs	r2, #80
	bl 0x0200c154
	movs	r1, #6
	adds	r1, #255
	movs	r2, #40
	movs	r0, #9
	bl 0x0200c174
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200c174
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #2
	bl 0x0200c174
	movs	r1, #131
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200c174
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #9
	bl 0x0200c174
	movs	r1, #160
	movs	r2, #20
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c154
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r2, #10
	movs	r0, #2
	movs	r1, #2
	bl 0x0200c104
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0f4
	movs	r1, #0
	movs	r0, #9
	bl 0x0200c134
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #2
	bl 0x0200c174
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200c084
	cmp	r0, #0
	bne.n	.L_0200342e
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #2
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200345e
.L_0200342e:
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #2
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
.L_0200345e:
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200c0ec
	movs	r1, #0
	movs	r0, #9
	bl 0x0200c134
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c174
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c154
	ldr	r3, [pc, #504]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c084
	cmp	r0, #0
	bne.n	.L_020034d4
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02003502
.L_020034d4:
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #0
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	bl 0x0200c144
.L_02003502:
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #9
	bl 0x0200c174
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	movs	r0, #2
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200c174
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #9
	bl 0x0200c174
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #129
	movs	r0, #2
	lsls	r1, r1, #1
	bl 0x0200c17c
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #2
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200c15c
	movs	r0, #8
	movs	r1, #3
	bl 0x0200c0f4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c144
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c15c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200c0ec
	movs	r0, #9
	movs	r1, #0
	bl 0x0200c144
	ldr	r3, [pc, #96]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl 0x0200c0f4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #2
	ldr	r1, [pc, #80]
	adds	r2, #153
	bl 0x0200c094
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #0
	ldr	r1, [pc, #64]
	adds	r2, #153
	bl 0x0200c094
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #9
	ldr	r1, [pc, #48]
	bl 0x0200c094
	ldr	r5, [pc, #48]
	movs	r0, #2
	adds	r1, r5, #0
	bl 0x0200c09c
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x0200c09c
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x0200c0ac
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #209
	bl 0x0200bfcc
	bl 0x0200c074
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00013333
	.2byte 0xcb18
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #60]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_020036b2
	bl 0x0200b720
	b.n	.L_020036d4
.L_020036b2:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020036be
	bl 0x0200b880
	b.n	.L_020036d4
.L_020036be:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_020036ca
	bl 0x0200b9d4
	b.n	.L_020036d4
.L_020036ca:
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020036d4
	bl 0x0200ba8c
.L_020036d4:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ec
	.4byte 0x000000ef
	.4byte 0x000000ed
	.2byte 0x00ee
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{lr}
	movs	r0, #65
	bl 0x0200c08c
	cmp	r0, #0
	beq.n	.L_02003718
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c08c
	ldr	r3, [r0, #80]
	movs	r0, #65
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x0200c16c
.L_02003718:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #14
	bl 0x0200c08c
	movs	r1, #144
	adds	r5, r0, #0
	lsls	r1, r1, #3
	ldr	r0, [pc, #308]
	bl 0x0200bf7c
	adds	r1, r5, #0
	movs	r3, #1
	adds	r1, #98
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #99
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #288]
	movs	r0, #160
	lsls	r0, r0, #4
	str	r3, [r5, #108]
	adds	r0, #33
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02003784
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_02003784
	movs	r0, #15
	bl 0x0200c0a4
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	bl 0x02009724
	b.n	.L_0200379c
.L_02003784:
	movs	r0, #15
	bl 0x0200c08c
	adds	r5, r0, #0
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #98
	strb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #204]
	str	r3, [r5, #108]
.L_0200379c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #231
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_020037c8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #230
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_020037c8
	movs	r1, #128
	movs	r2, #177
	movs	r0, #17
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	bl 0x0200c0d4
	b.n	.L_020037e0
.L_020037c8:
	movs	r0, #17
	bl 0x0200c08c
	movs	r1, #15
	bl 0x0200c124
	movs	r0, #17
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
.L_020037e0:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_0200381c
	ldr	r3, [pc, #124]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #4
	bgt.n	.L_0200380a
	cmp	r3, #1
	blt.n	.L_0200380a
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bfcc
	b.n	.L_0200381c
.L_0200380a:
	movs	r1, #128
	movs	r2, #128
	movs	r3, #128
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	lsls	r3, r3, #19
	bl 0x0200c1a4
.L_0200381c:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02003854
	movs	r1, #128
	movs	r2, #220
	movs	r3, #128
	movs	r0, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	lsls	r3, r3, #19
	bl 0x0200c1a4
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200c184
	bl 0x0200bffc
	movs	r0, #1
	bl 0x0200bf74
.L_02003854:
	ldr	r0, [pc, #24]
	ldr	r1, [pc, #28]
	ldr	r2, [pc, #28]
	ldr	r3, [pc, #32]
	bl 0x02008038
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200b6f1
	.4byte 0x020086d1
	.4byte 0x02000240
	.4byte 0x0200c69c
	.4byte 0x0200c6ac
	.4byte 0x0200c6d8
	.2byte 0xc704
	.2byte 0x0200
	push	{lr}
	movs	r0, #18
	sub	sp, #8
	bl 0x02009c44
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200bfc4
	cmp	r0, #0
.L_02003896:
	bne.n	.L_0200389a
	b.n	.L_020039cc
.L_0200389a:
	movs	r3, #44
	movs	r2, #36
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #3
	movs	r0, #44
	movs	r1, #40
	movs	r2, #8
	bl 0x0200c00c
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #31
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #32
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #13
	movs	r1, #0
.L_02003970:
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_020039b8
	movs	r0, #10
	movs	r1, #0
.L_02003988:
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	b.n	.L_020039d0
.L_020039b8:
	movs	r0, #10
	bl 0x0200c08c
	movs	r3, #208
	lsls	r3, r3, #8
	strh	r3, [r0, #6]
	movs	r0, #1
	bl 0x0200bf74
	b.n	.L_020039d0
.L_020039cc:
	bl 0x020097f4
.L_020039d0:
	add	sp, #8
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	ldr	r3, [pc, #156]
	subs	r2, #39
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #4
	bne.n	.L_02003a14
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02003a80
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_02003a80
	bl 0x02009d50
	b.n	.L_02003a80
.L_02003a14:
	cmp	r3, #21
	bne.n	.L_02003a22
	movs	r0, #48
	adds	r0, #255
	bl 0x0200bfd4
	b.n	.L_02003a80
.L_02003a22:
	movs	r1, #2
	movs	r0, #17
	bl 0x0200c0ec
	movs	r0, #17
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c014
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02003a4c
	ldr	r1, [pc, #64]
	movs	r0, #11
	bl 0x0200c09c
.L_02003a4c:
	movs	r0, #157
	lsls	r0, r0, #4
	bl 0x0200bfc4
	cmp	r0, #0
	beq.n	.L_02003a80
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c0d4
.L_02003a80:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xca1c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
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
	movs	r0, #13
	bl 0x02009c44
	movs	r0, #16
	bl 0x02009c44
	movs	r0, #17
	bl 0x02009c44
	movs	r0, #19
	bl 0x02009c44
	movs	r0, #20
	bl 0x02009c44
	movs	r0, #14
	bl 0x0200c08c
	movs	r1, #1
	bl 0x0200c124
	movs	r0, #15
	bl 0x0200c08c
	movs	r1, #2
	bl 0x0200c124
	movs	r0, #8
	bl 0x0200c08c
	movs	r1, #0
	bl 0x0200c124
	movs	r0, #9
	bl 0x0200c08c
	movs	r1, #3
	bl 0x0200c124
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #229
	bl 0x0200bfc4
	cmp	r0, #0
	bne.n	.L_02003b00
	b.n	.L_02003cb2
.L_02003b00:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #229
	bl 0x0200bfd4
	ldr	r2, [pc, #424]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #18
	bne.n	.L_02003b4e
	ldr	r3, [pc, #412]
	ldr	r2, [r2, #16]
	ldr	r3, [r3, #0]
	subs	r5, r2, r3
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	bl 0x0200c1e4
	bl 0x0200c1f4
	cmp	r5, #0
	ble.n	.L_02003bbc
	movs	r2, #156
	lsls	r2, r2, #7
	adds	r2, #31
	cmp	r5, r2
	bgt.n	.L_02003b7a
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #135
	cmp	r5, r3
	bgt.n	.L_02003b8c
	b.n	.L_02003b94
.L_02003b4e:
	cmp	r3, #19
	bne.n	.L_02003bdc
	ldr	r3, [pc, #356]
	ldr	r2, [r2, #16]
	ldr	r3, [r3, #0]
	subs	r5, r2, r3
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	bl 0x0200c1e4
	bl 0x0200c1f4
	cmp	r5, #0
	ble.n	.L_02003bbc
	movs	r1, #156
	lsls	r1, r1, #7
	adds	r1, #31
	cmp	r5, r1
	ble.n	.L_02003b82
.L_02003b7a:
	movs	r0, #93
	bl 0x0200c2b4
	b.n	.L_02003b9a
.L_02003b82:
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #135
	cmp	r5, r2
	ble.n	.L_02003b94
.L_02003b8c:
	movs	r0, #92
	bl 0x0200c2b4
	b.n	.L_02003b9a
.L_02003b94:
	movs	r0, #91
	bl 0x0200c2b4
.L_02003b9a:
	movs	r0, #20
	bl 0x0200c064
	ldr	r0, [pc, #280]
	bl 0x0200c12c
	adds	r0, r5, #0
	movs	r1, #5
	bl 0x0200c03c
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c144
	bl 0x0200c2ac
	b.n	.L_02003bd6
.L_02003bbc:
	cmp	r5, #0
	bge.n	.L_02003bd6
	ldr	r0, [pc, #252]
	bl 0x0200c12c
	negs	r0, r5
	movs	r1, #5
	bl 0x0200c03c
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c144
.L_02003bd6:
	bl 0x0200c074
	b.n	.L_02003cb2
.L_02003bdc:
	cmp	r3, #20
	bne.n	.L_02003cb2
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r7, r2, r3
	bl 0x0200c06c
	movs	r0, #0
	bl 0x0200c204
	bl 0x0200c1e4
	bl 0x0200c1f4
	movs	r0, #20
	bl 0x0200c064
	movs	r3, #0
	ldrsb	r3, [r7, r3]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_02003c12
	movs	r0, #1
	bl 0x02009938
	b.n	.L_02003cae
.L_02003c12:
	movs	r1, #2
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_02003c2a
	ldr	r0, [pc, #168]
	bl 0x0200c12c
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c144
	b.n	.L_02003cae
.L_02003c2a:
	ldr	r0, [pc, #156]
	bl 0x0200c12c
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c144
	movs	r3, #0
	ldrsb	r3, [r7, r3]
	cmp	r3, r5
	beq.n	.L_02003c94
	adds	r6, r7, #0
.L_02003c42:
	cmp	r6, r7
	bne.n	.L_02003c4e
	ldr	r0, [pc, #132]
	bl 0x0200c12c
	b.n	.L_02003c54
.L_02003c4e:
	ldr	r0, [pc, #128]
	bl 0x0200c12c
.L_02003c54:
	movs	r0, #0
	ldrsb	r0, [r6, r0]
	bl 0x02009a60
	movs	r1, #2
	adds	r5, r0, #0
	bl 0x0200c03c
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c144
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200c1fc
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200c07c
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #8
	adds	r6, #1
	bl 0x0200c15c
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02003c42
.L_02003c94:
	ldr	r3, [pc, #28]
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #254
	ldr	r0, [pc, #52]
	strb	r2, [r3, #0]
	bl 0x0200c12c
	movs	r0, #18
	movs	r1, #0
	bl 0x0200c144
.L_02003cae:
	bl 0x0200c074
.L_02003cb2:
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200234c
	.4byte 0x00001387
	.4byte 0x00001388
	.4byte 0x000013a2
	.4byte 0x000013aa
	.4byte 0x000013ab
	.4byte 0x000013ac
	.2byte 0x13ad
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	ldr	r2, [r7, #104]
	adds	r6, r7, #0
	adds	r6, #99
	mov	r8, r2
	ldrb	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	sub	sp, #24
	cmp	r3, #0
	beq.n	.L_02003d12
	ldrb	r0, [r6, #0]
	movs	r1, #6
	lsrs	r0, r0, #1
	bl 0x0200bf6c
	adds	r1, r0, #0
	lsls	r1, r1, #24
	lsrs	r1, r1, #24
	adds	r0, r7, #0
	bl 0x0200c024
.L_02003d12:
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L_02003d50
	ldrb	r2, [r6, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02003d86
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #86
	bl 0x0200c2b4
	mov	r1, r8
	adds	r1, #166
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	r2, r8
	lsls	r3, r3, #1
	adds	r3, #160
	strh	r5, [r2, r3]
	ldr	r2, [pc, #8]
	ldrh	r3, [r1, #0]
	eors	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_02003d86
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0000
.L_02003d50:
	cmp	r5, #1
	bne.n	.L_02003d86
	mov	r3, r8
	adds	r3, #160
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #31
	ble.n	.L_02003d86
	mov	r3, r8
	adds	r3, #162
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #31
	ble.n	.L_02003d86
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200c024
	mov	r3, r8
	adds	r3, #164
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200bfac
	movs	r3, #0
	str	r3, [r7, #108]
	b.n	.L_02003e40
.L_02003d86:
	ldrb	r3, [r6, #0]
	movs	r2, #1
	adds	r3, #1
	strb	r3, [r6, #0]
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r6, r8
	mov	fp, r2
	adds	r6, #160
.L_02003d98:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	lsls	r0, r0, #10
	bl 0x0200bf9c
	str	r0, [sp, #4]
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	blt.n	.L_02003e2c
	cmp	r3, #31
	bgt.n	.L_02003e2c
	ldr	r3, [r7, #8]
	add	r5, sp, #12
	str	r3, [r5, #0]
	adds	r0, r5, #0
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r3, [r7, #12]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl 0x0200c21c
	ldr	r2, [r5, #0]
	movs	r3, #0
	str	r2, [sp, #8]
	mov	sl, r3
	ldr	r5, [r5, #8]
	mov	r9, r5
	ldr	r5, [sp, #0]
	add	r5, r8
.L_02003ddc:
	ldr	r2, [sp, #8]
	mov	r3, r9
	str	r3, [r5, #16]
	str	r2, [r5, #12]
	ldr	r2, [sp, #4]
	mov	r3, sl
	str	r2, [r5, #20]
	str	r2, [r5, #24]
	cmp	r3, #0
	bne.n	.L_02003dfc
	adds	r0, r7, #0
	bl 0x0200c25c
	subs	r0, #1
	strh	r0, [r5, #30]
	b.n	.L_02003e14
.L_02003dfc:
	adds	r0, r7, #0
	bl 0x0200c25c
	ldr	r3, [r5, #16]
	ldr	r2, [pc, #72]
	adds	r0, #1
	adds	r3, r3, r2
	str	r3, [r5, #16]
	ldr	r3, [r5, #24]
	strh	r0, [r5, #30]
	negs	r3, r3
	str	r3, [r5, #24]
.L_02003e14:
	adds	r0, r5, #0
	bl 0x0200c24c
	movs	r3, #1
	add	sl, r3
	mov	r2, sl
	adds	r5, #40
	cmp	r2, #1
	ble.n	.L_02003ddc
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
.L_02003e2c:
	ldr	r3, [sp, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r3, #80
	add	fp, r2
	str	r3, [sp, #0]
	mov	r3, fp
	adds	r6, #2
	cmp	r3, #0
	bge.n	.L_02003d98
.L_02003e40:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r7, r1, #0
	sub	sp, #4
	bl 0x0200bfbc
	movs	r1, #164
	adds	r1, r1, r7
	mov	r8, r1
	ldr	r2, [pc, #60]
	mov	r3, r8
	strh	r0, [r3, #0]
	movs	r1, #128
	lsls	r0, r0, #16
	mov	sl, r2
	lsls	r1, r1, #1
	ldr	r2, [pc, #48]
	asrs	r0, r0, #16
	bl 0x0200bfb4
	adds	r3, r7, #0
	movs	r2, #186
	movs	r5, #0
	adds	r3, #166
	lsls	r2, r2, #2
	strh	r5, [r3, #0]
	adds	r2, #255
	subs	r3, #6
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #6
	str	r6, [r3, #0]
	adds	r3, r6, #0
	mov	r0, sl
	adds	r3, #98
	strb	r0, [r3, #0]
	adds	r3, #1
	b.n	.L_02003eb8
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xc2bc
	.2byte 0x0200
.L_02003eb8:
	strb	r0, [r3, #0]
	ldr	r3, [pc, #140]
	mov	r0, r8
	str	r3, [r6, #108]
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	ldr	r2, [pc, #132]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	str	r7, [r6, #104]
	lsrs	r3, r3, #5
	mov	fp, r3
	mov	r9, r5
	mov	sl, r5
.L_02003ed6:
	movs	r1, #1
	mov	r2, sl
	mov	r8, r1
	adds	r5, r2, r7
.L_02003ede:
	mov	r3, fp
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #16
	movs	r2, #16
	ldr	r3, [pc, #100]
	bl 0x0200c244
	ldrb	r3, [r5, #5]
	movs	r0, #33
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r5, #9]
	adds	r0, r6, #0
	bl 0x0200c254
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #1
	lsls	r0, r0, #2
	negs	r2, r2
	orrs	r3, r0
	add	r8, r2
	strb	r3, [r5, #9]
	mov	r3, r8
	adds	r5, #40
	cmp	r3, #0
	bge.n	.L_02003ede
	movs	r1, #1
	add	r9, r1
	movs	r0, #80
	mov	r2, r9
	add	sl, r0
	cmp	r2, #1
	ble.n	.L_02003ed6
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200bcd9
	.4byte 0x020036e0
	.4byte 0x80004000
	.4byte 0x23013062
	.4byte 0x47707003
	.irp EntryTarget, 0x03000528, 0x03000508, 0x03000514, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000101, 0x08000119, 0x08000121, 0x080001b9, 0x080001c9, 0x080001d1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x08020121, 0x08020179, 0x080201e9, 0x08020219, 0x08020229, 0x08020279, 0x08020291, 0x08038041, 0x08038121, 0x080ad241, 0x080ad291, 0x080ad2a9, 0x080ad2d1, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8061, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80b1, 0x080c80b9, 0x080c80c9, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8131, 0x080c8139, 0x080c8149, 0x080c8159, 0x080c8161, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81a9, 0x080c81d1, 0x080c81d9, 0x080c81f1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8261, 0x080c8269, 0x080c8279, 0x080c8291, 0x080c82a1, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c83e1, 0x080c84e1, 0x080c8571, 0x080c8581, 0x080c85c1, 0x080c8601, 0x080c8629, 0x080c8759, 0x080c8761, 0x080c87c1, 0x080c87c9, 0x080c87e9, 0x080c87f1, 0x080c8809, 0x080c8811, 0x080c8819, 0x080c8821, 0x080c88d9, 0x080c88e9, 0x08108009, 0x08108011, 0x08108019, 0x08108079, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x70000000
	.4byte 0xf7700000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x77770000
	.4byte 0xffff7770
	.4byte 0xfffffff7
	.4byte 0x7777ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00007777
	.4byte 0x0777ffff
	.4byte 0x7fffffff
	.4byte 0xffff7777
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x0000077f
	.4byte 0xfff70000
	.4byte 0xffff7000
	.4byte 0x7ffff700
	.4byte 0x07ffff70
	.4byte 0x007fff70
	.4byte 0x007ffff7
	.4byte 0x0007fff7
	.4byte 0x0007fff7
	.4byte 0x000077ff
	.4byte 0x00000077
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff770000
	.4byte 0x77000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00007fff
	.4byte 0x0007ffff
	.4byte 0x007ffff7
	.4byte 0x07ffff70
	.4byte 0x07fff700
	.4byte 0x7ffff700
	.4byte 0x7fff7000
	.4byte 0x7fff7000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xf4400000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x44440000
	.4byte 0xffff4440
	.4byte 0xfffffff4
	.4byte 0x4444ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004444
	.4byte 0x0444ffff
	.4byte 0x4fffffff
	.4byte 0xffff4444
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000044f
	.4byte 0xfff40000
	.4byte 0xffff4000
	.4byte 0x4ffff400
	.4byte 0x04ffff40
	.4byte 0x004fff40
	.4byte 0x004ffff4
	.4byte 0x0004fff4
	.4byte 0x0004fff4
	.4byte 0x000044ff
	.4byte 0x00000044
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff440000
	.4byte 0x44000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004fff
	.4byte 0x0004ffff
	.4byte 0x004ffff4
	.4byte 0x04ffff40
	.4byte 0x04fff400
	.4byte 0x4ffff400
	.4byte 0x4fff4000
	.4byte 0x4fff4000
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xeb000000
	.4byte 0x000000eb
	.4byte 0xedeb0000
	.4byte 0x0000ebed
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xedeb0000
	.4byte 0x0000ebed
	.4byte 0xeb000000
	.4byte 0x000000eb
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0x0000e900
	.4byte 0x00e90000
	.4byte 0x00ebe9e9
	.4byte 0xe9e9eb00
	.4byte 0xedebeb00
	.4byte 0x00ebebed
	.4byte 0xefed0000
	.4byte 0x0000edef
	.4byte 0xefed0000
	.4byte 0x0000edef
	.4byte 0xedebeb00
	.4byte 0x00ebebed
	.4byte 0x00ebe9e9
	.4byte 0xe9e9eb00
	.4byte 0x0000e900
	.4byte 0x00e90000
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xebe90000
	.4byte 0x0000e9eb
	.4byte 0xedebe900
	.4byte 0x00e9ebed
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xedebe900
	.4byte 0x00e9ebed
	.4byte 0xebe90000
	.4byte 0x0000e9eb
	.4byte 0xe9000000
	.4byte 0x000000e9
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
	.4byte 0x000000fa
	.4byte 0x000000fb
	.4byte 0x000000fc
	.4byte 0x00000100
	.4byte 0x00000101
	.4byte 0x00000102
	.4byte 0x00000106
	.4byte 0x00000107
	.4byte 0x00000108
	.4byte 0x000000b7
	.4byte 0x000000b6
	.4byte 0x000000b5
	.4byte 0x000000bd
	.4byte 0x000000ba
	.4byte 0x000000bc
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x005a0059
	.4byte 0x005c005b
	.4byte 0x005e005d
	.4byte 0xffff005f
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
	.4byte 0x0059005e
	.4byte 0x005b005a
	.4byte 0x005d005c
	.4byte 0x005e005d
	.4byte 0x005a0059
	.4byte 0x005c005b
	.4byte 0x005d005c
	.4byte 0x0059005e
	.4byte 0x005b005a
	.4byte 0x005c005b
	.4byte 0x005e005d
	.4byte 0x005a0059
	.4byte 0x005b005a
	.4byte 0x005d005c
	.4byte 0x0059005e
	.4byte 0x005a0059
	.4byte 0x005c005b
	.4byte 0x005e005d
	.4byte 0x0200c57c
	.4byte 0x0200c5b8
	.4byte 0x0200c5f4
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000004
	.4byte 0x01520000
	.4byte 0x00000000
	.4byte 0x034c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x013e0000
	.4byte 0x00000000
	.4byte 0x034c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000004
	.4byte 0x013e0000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01520000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01520000
	.4byte 0x00000000
	.4byte 0x034c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x013e0000
	.4byte 0x00000000
	.4byte 0x034c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00be0000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00be0000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x017e0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffd800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000a00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000a00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x01670000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000002e
	.4byte 0x020087a5
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x0000010f
	.4byte 0x40000328
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000ec
	.4byte 0x00136002
	.4byte 0x002010ed
	.4byte 0x003020ed
	.4byte 0x0040a0ee
	.4byte 0x005070ed
	.4byte 0x006080ee
	.4byte 0x007090ee
	.4byte 0x008050ed
	.4byte 0x009060ed
	.4byte 0x00a030ed
	.4byte 0x00b040ed
	.4byte 0x00c150ef
	.4byte 0x000000ef
	.4byte 0x0150c0ec
	.4byte 0x01603134
	.4byte 0x01701134
	.4byte 0x01804134
	.4byte 0x000000ed
	.4byte 0x001020ec
	.4byte 0x002030ec
	.4byte 0x0030a0ec
	.4byte 0x0040b0ec
	.4byte 0x005080ec
	.4byte 0x006090ec
	.4byte 0x007050ec
	.4byte 0x015030eb
	.4byte 0x000000ee
	.4byte 0x008060ec
	.4byte 0x009070ec
	.4byte 0x00a040ec
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x01540000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03ae0000
	.4byte 0x0000c000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0xffff006b
	.4byte 0x00000003
	.4byte 0x00740000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0000c000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00013000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x03240000
	.4byte 0x0001b000
	.4byte 0xffff0088
	.4byte 0x0200c758
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0002c000
	.4byte 0xffff008a
	.4byte 0x0200c7fc
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00024000
	.4byte 0x003e00f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x006700f5
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02c20000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x034e0000
	.4byte 0x00000000
	.4byte 0x02940000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001d000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x02f40000
	.4byte 0x00015000
	.4byte 0xffff004f
	.4byte 0x0200cb54
	.4byte 0x03060000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00018000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x024c0000
	.4byte 0x00010000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x03260000
	.4byte 0x00000000
	.4byte 0x02540000
	.4byte 0x00015000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x03400000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x00005000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001d000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x015a0000
	.4byte 0x0001d000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01570000
	.4byte 0x00005000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01a6
	.4byte 0x00000007
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x025e0000
	.4byte 0x00024000
	.4byte 0xffff01a6
	.4byte 0x00000007
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x025e0000
	.4byte 0x00024000
	.4byte 0xffff01a6
	.4byte 0x00000007
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x024e0000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x025e0000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02660000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x033a0000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x02bc0000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x029c0000
	.4byte 0x01024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x02f60000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x035a0000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x01024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x030e0000
	.4byte 0x00000000
	.4byte 0x02a60000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x029d0000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00005000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00003000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00004000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x005e0000
	.4byte 0x0000b000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00005000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02dd0000
	.4byte 0x00000000
	.4byte 0x005b0000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x0001d000
	.4byte 0xffff0088
	.4byte 0x00000003
	.4byte 0x02c40000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00004000
	.4byte 0xffff00a1
	.4byte 0x00000001
	.4byte 0x02f40000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00003000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00005000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00005000
	.4byte 0xffff0087
	.4byte 0x00000003
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00013000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x0001d000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x005e0000
	.4byte 0x00000000
	.4byte 0x023e0000
	.4byte 0x00015000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x0001d000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000003
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x02660000
	.4byte 0x00008000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02b00000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff003b
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
	.4byte 0x00028000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x017e0000
	.4byte 0x00020000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x016e0000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01620000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x018e0000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x009e0000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00034000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
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
	.4byte 0xffff00ec
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00003000
	.4byte 0xffff00ec
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00005000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x03860000
	.4byte 0x0001d000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0001b000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x005a0000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0001d000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00003000
	.4byte 0xffff00ec
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00005000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x01540000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00003000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x01fc0000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02240000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000ce01
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000ce01
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000ce01
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000ce01
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x0000ce01
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x0000ce01
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000ce01
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x0000ce01
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000ce01
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x0000ce01
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00009c05
	.4byte 0xffff001e
	.4byte 0x02009251
	.4byte 0x50008905
	.4byte 0xffff0027
	.4byte 0x02009285
	.4byte 0x50008905
	.4byte 0xffff0028
	.4byte 0x020092a5
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte 0x020092c5
	.4byte 0x50008905
	.4byte 0xffff002a
	.4byte 0x02009355
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte 0x02009375
	.4byte 0x50008905
	.4byte 0xffff002c
	.4byte 0x02009395
	.4byte 0x50008905
	.4byte 0xffff002d
	.4byte 0x020093b5
	.4byte 0x50008905
	.4byte 0xffff002e
	.4byte 0x020093d5
	.4byte 0x50008905
	.4byte 0xffff002f
	.4byte 0x020093f5
	.4byte 0x50008905
	.4byte 0xffff0030
	.4byte 0x02009415
	.4byte 0x50008905
	.4byte 0xffff0031
	.4byte 0x02009435
	.4byte 0x50008905
	.4byte 0xffff0032
	.4byte 0x02009455
	.4byte 0x50008905
	.4byte 0xffff0033
	.4byte 0x02009475
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020094b5
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x02009495
	.4byte 0x00008e15
	.4byte 0x09e70011
	.4byte 0x020095ad
	.4byte 0x00000000
	.4byte 0x19e70011
	.4byte 0x02008609
	.4byte 0x00000000
	.4byte 0x0a210008
	.4byte 0x020089ad
	.4byte 0x00000000
	.4byte 0x09b20008
	.4byte 0x00002b55
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002be4
	.4byte 0x00008d15
	.4byte 0x0a210008
	.4byte 0x000028dd
	.4byte 0x00008d15
	.4byte 0x09b20008
	.4byte 0x00002b59
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002bea
	.4byte 0x00000000
	.4byte 0x0a210009
	.4byte 0x000028d6
	.4byte 0x00000000
	.4byte 0x09b20009
	.4byte 0x00002b56
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002be5
	.4byte 0x00008d15
	.4byte 0x0a210009
	.4byte 0x000028de
	.4byte 0x00008d15
	.4byte 0x09b20009
	.4byte 0x00002b5a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002beb
	.4byte 0x00000000
	.4byte 0x0a21000a
	.4byte 0x000028d7
	.4byte 0x00000000
	.4byte 0x09b2000a
	.4byte 0x00002b57
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002be6
	.4byte 0x00008d15
	.4byte 0x0a21000a
	.4byte 0x000028df
	.4byte 0x00008d15
	.4byte 0x09b2000a
	.4byte 0x00002b5b
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002bec
	.4byte 0x00000000
	.4byte 0x0a21000b
	.4byte 0x000028d8
	.4byte 0x00000000
	.4byte 0x09b2000b
	.4byte 0x00002b58
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002be7
	.4byte 0x00008d15
	.4byte 0x0a21000b
	.4byte 0x000028e0
	.4byte 0x00008d15
	.4byte 0x09b2000b
	.4byte 0x00002b5c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002bed
	.4byte 0x00000000
	.4byte 0x0a21000c
	.4byte 0x000028d9
	.4byte 0x00000000
	.4byte 0x09b2000c
	.4byte 0x00002c59
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002c5d
	.4byte 0x00008d15
	.4byte 0x0a21000c
	.4byte 0x000028e1
	.4byte 0x00008d15
	.4byte 0x09b2000c
	.4byte 0x00002c5b
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002c5f
	.4byte 0x00000000
	.4byte 0x09b2000d
	.4byte 0x000028da
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002be8
	.4byte 0x00008d15
	.4byte 0x09b2000d
	.4byte 0x000028e2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002bee
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02009095
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x02009095
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200918d
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0200918d
	.4byte 0x50008805
	.4byte 0xffff0020
	.4byte 0x020094d5
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x020085f9
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200892d
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte 0x0200894d
	.4byte 0x00000003
	.4byte 0xffff001a
	.4byte 0x0200896d
	.4byte 0x0000c403
	.4byte 0xffff0013
	.4byte 0x0200898d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x00000015
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte 0x02009d09
	.4byte 0x0000ce01
	.4byte 0xffff000e
	.4byte 0x00000017
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte 0x02009d39
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000290e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002914
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000290f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002915
	.4byte 0x00000000
	.4byte 0x09b2000a
	.4byte 0x00002910
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002c47
	.4byte 0x00008d15
	.4byte 0x09b2000a
	.4byte 0x00002916
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002c48
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002911
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002917
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002912
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002918
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002913
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002919
	.4byte 0x00000000
	.4byte 0x09b2000e
	.4byte 0x02009071
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002c49
	.4byte 0x00008d15
	.4byte 0x09b2000e
	.4byte 0x00002920
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002c4d
	.4byte 0x00000000
	.4byte 0x09b2000f
	.4byte 0x0000291d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002c4a
	.4byte 0x00008d15
	.4byte 0x09b2000f
	.4byte 0x00002921
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002c4e
	.4byte 0x00000000
	.4byte 0x09b20010
	.4byte 0x0000291e
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002c4b
	.4byte 0x00008d15
	.4byte 0x09b20010
	.4byte 0x00002922
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002c4f
	.4byte 0x00000000
	.4byte 0x09b20011
	.4byte 0x0000291f
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002c4c
	.4byte 0x00008d15
	.4byte 0x09b20011
	.4byte 0x00002923
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002c50
	.4byte 0x00000003
	.4byte 0xffff0016
	.4byte 0x020088ed
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0200890d
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200952d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200956d
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
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000000
	.4byte 0x09b20008
	.4byte 0x000028e5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002bf0
	.4byte 0x00008d15
	.4byte 0x09b20008
	.4byte 0x000028e8
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002bf3
	.4byte 0x00000000
	.4byte 0x09b20009
	.4byte 0x000028e6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002bf1
	.4byte 0x00008d15
	.4byte 0x09b20009
	.4byte 0x000028e9
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002bf4
	.4byte 0x00000000
	.4byte 0x09b2000a
	.4byte 0x000028e7
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002bf2
	.4byte 0x00008d15
	.4byte 0x09b2000a
	.4byte 0x000028ea
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002bf5
	.4byte 0x00000000
	.4byte 0x09b2000b
	.4byte 0x000028eb
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002bf6
	.4byte 0x00008d15
	.4byte 0x09b2000b
	.4byte 0x000028ed
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002bf8
	.4byte 0x00000000
	.4byte 0x09b2000c
	.4byte 0x000028ec
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002bf7
	.4byte 0x00008d15
	.4byte 0x09b2000c
	.4byte 0x000028ee
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002bf9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008cf5
	.4byte 0x00008d15
	.4byte 0x09b2000d
	.4byte 0x000028ff
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002c3c
	.4byte 0x00000000
	.4byte 0x09b2000e
	.4byte 0x000028ef
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002bfa
	.4byte 0x00008d15
	.4byte 0x09b2000e
	.4byte 0x000028f6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002c01
	.4byte 0x00000000
	.4byte 0x09b2000f
	.4byte 0x000028f0
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002bfb
	.4byte 0x00008d15
	.4byte 0x09b2000f
	.4byte 0x000028f7
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002c02
	.4byte 0x00000000
	.4byte 0x09b20010
	.4byte 0x000028f1
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002bfc
	.4byte 0x00008d15
	.4byte 0x09b20010
	.4byte 0x000028f8
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002c03
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008af5
	.4byte 0x00008d15
	.4byte 0x09b20011
	.4byte 0x000028f9
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002c04
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008d45
	.4byte 0x00008d15
	.4byte 0x09b20012
	.4byte 0x000028fb
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002c38
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008d95
	.4byte 0x00008d15
	.4byte 0x09b20013
	.4byte 0x000028fd
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002c3a
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02008de5
	.4byte 0x00008d15
	.4byte 0x0a210014
	.4byte 0x00002929
	.4byte 0x00008d15
	.4byte 0x09b20014
	.4byte 0x00002bb2
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002c41
	.4byte 0x00000000
	.4byte 0x0a210015
	.4byte 0x00002901
	.4byte 0x00000000
	.4byte 0x09b20015
	.4byte 0x00002baf
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002c3e
	.4byte 0x00008d15
	.4byte 0x0a210015
	.4byte 0x0000292a
	.4byte 0x00008d15
	.4byte 0x09b20015
	.4byte 0x00002bb3
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002c42
	.4byte 0x00000000
	.4byte 0x0a210016
	.4byte 0x00002902
	.4byte 0x00000000
	.4byte 0x09b20016
	.4byte 0x00002bb0
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002c3f
	.4byte 0x00008d15
	.4byte 0x0a210016
	.4byte 0x0000292b
	.4byte 0x00008d15
	.4byte 0x09b20016
	.4byte 0x00002bb4
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002c43
	.4byte 0x00000000
	.4byte 0x0a210017
	.4byte 0x00002903
	.4byte 0x00000000
	.4byte 0x09b20017
	.4byte 0x00002bb1
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002c40
	.4byte 0x00008d15
	.4byte 0x0a210017
	.4byte 0x0000292c
	.4byte 0x00008d15
	.4byte 0x09b20017
	.4byte 0x00002bb5
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002c44
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002904
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00002906
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00002905
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00002907
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00002908
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x0000290a
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00002909
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x0000290b
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x02008e51
	.4byte 0x00008d15
	.4byte 0x0a21001c
	.4byte 0x0000290d
	.4byte 0x00008d15
	.4byte 0x09b2001c
	.4byte 0x00002bb7
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00002c46
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403058
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403059
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x0040305a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002c35
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002c36
	.4byte 0x00000002
	.4byte 0x09d10014
	.4byte 0x0200ae19
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403058
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403059
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x0040305a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02009b85
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000013dd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000013ce
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000013de
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008edd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008eb9
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000013df
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000013db
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000013e0
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000013dc
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000013e1
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02009abd
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000139a
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001385
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000139b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008f41
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008f7d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008fc9
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000013ca
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008fe1
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0200902d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
