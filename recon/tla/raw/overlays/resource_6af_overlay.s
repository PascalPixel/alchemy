.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008cf5, 0x0200833d, 0x02008349, 0x02008351, 0x02008b3d, 0x02008345, 0x02008f49
	overlay_veneer \EntryTarget
	.endr
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
	bl 0x02008f8c
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
	bl 0x02008fa4
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x02008ffc
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008fac
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
	bl 0x02008f8c
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
	bl 0x02008fa4
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x02008ffc
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
	bl 0x02008fdc
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
	bl 0x02008f8c
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
	bl 0x02008f7c
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x02008f84
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02008fa4
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
	bl 0x02008ffc
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
	bl 0x02008f54
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
	bl 0x02008f54
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x02008f54
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
	bl 0x02008f7c
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x02008f84
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
	.4byte 0x02009170
	.4byte 0x02008125
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x4800
	bx	lr
	.2byte 0x91fc
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x922c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #56]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000368
	ldr	r0, [pc, #44]
	b.n	.L_02000388
.L_02000368:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000372
	ldr	r0, [pc, #44]
	b.n	.L_02000388
.L_02000372:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_0200037c
	ldr	r0, [pc, #40]
	b.n	.L_02000388
.L_0200037c:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000386
	ldr	r0, [pc, #40]
	b.n	.L_02000388
.L_02000386:
	ldr	r0, [pc, #40]
.L_02000388:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000012b
	.4byte 0x02009390
	.4byte 0x0000012c
	.4byte 0x02009480
	.4byte 0x0000012d
	.4byte 0x02009504
	.4byte 0x0000012e
	.4byte 0x0200969c
	.2byte 0x9330
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r1, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_020003c8
	movs	r0, #0
	b.n	.L_020003f4
.L_020003c8:
	cmp	r0, #2
	bhi.n	.L_020003dc
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_020003de
.L_020003dc:
	ldr	r4, [pc, #24]
.L_020003de:
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
.L_020003f4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	lsls	r5, r1, #16
	asrs	r5, r5, #12
	adds	r5, r5, r0
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	sub	sp, #8
	adds	r2, #63
	subs	r3, #2
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #100
	movs	r2, #3
	movs	r3, #4
	bl 0x02008f9c
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #4]
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r0, #1
	bl 0x02008f94
	ldrh	r0, [r5, #12]
	bl 0x02008f6c
	add	sp, #8
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	lsls	r1, r1, #16
	mov	fp, r1
	mov	r3, fp
	adds	r7, r0, #0
	asrs	r3, r3, #16
	movs	r0, #192
	movs	r1, #192
	mov	fp, r3
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	sub	sp, #56
	bl 0x02009004
	mov	r3, fp
	lsls	r6, r3, #4
	adds	r6, r6, r7
	ldr	r0, [r6, #0]
	ldr	r2, [r6, #4]
	movs	r3, #128
	lsls	r3, r3, #12
	mov	r9, r3
	lsls	r0, r0, #20
	lsls	r2, r2, #20
	movs	r1, #1
	movs	r3, #1
	add	r0, r9
	add	r2, r9
	negs	r1, r1
	bl 0x0200900c
	bl 0x02009014
	movs	r1, #128
	movs	r2, #128
	movs	r0, #0
	lsls	r1, r1, #13
	lsls	r2, r2, #9
	bl 0x02008fb4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x02008fb4
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200906c
	ldr	r3, [pc, #232]
	add	r5, sp, #16
	str	r3, [r5, #8]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #12]
	movs	r3, #1
	str	r3, [r5, #0]
	movs	r3, #0
	mov	r8, r3
	movs	r3, #22
	adds	r3, #255
	strh	r3, [r5, #24]
	ldr	r3, [pc, #212]
	ldr	r2, [r6, #4]
	str	r3, [r5, #28]
	movs	r3, #160
	ldr	r0, [r6, #0]
	lsls	r3, r3, #13
	lsls	r2, r2, #20
	adds	r2, r2, r3
	mov	r3, r8
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r3, #232
	lsls	r3, r3, #14
	lsls	r0, r0, #20
	str	r3, [sp, #8]
	mov	sl, r3
	add	r0, r9
	movs	r1, #0
	movs	r3, #0
	str	r5, [sp, #12]
	bl 0x0200815c
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	str	r3, [r5, #8]
	ldr	r3, [pc, #164]
	ldr	r0, [r6, #0]
	ldr	r2, [r6, #4]
	lsls	r0, r0, #20
	adds	r0, r0, r3
	mov	r3, r8
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	lsls	r2, r2, #20
	mov	r3, sl
	str	r3, [sp, #8]
	add	r2, r9
	movs	r1, #0
	movs	r3, #0
	str	r5, [sp, #12]
	bl 0x0200815c
	ldr	r0, [r6, #0]
	ldr	r2, [r6, #4]
	movs	r3, #144
	lsls	r3, r3, #13
	lsls	r0, r0, #20
	adds	r0, r0, r3
	mov	r3, r8
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	lsls	r2, r2, #20
	mov	r3, sl
	str	r3, [sp, #8]
	add	r2, r9
	movs	r1, #0
	movs	r3, #0
	str	r5, [sp, #12]
	bl 0x0200815c
	movs	r0, #10
	bl 0x02008fc4
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r2, #63
	subs	r3, #2
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #100
	movs	r2, #3
	movs	r3, #4
	movs	r0, #6
	bl 0x02008f9c
	movs	r0, #10
	bl 0x02008fc4
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r2, #63
	subs	r3, #2
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #3
	movs	r3, #4
	movs	r1, #100
	movs	r0, #3
	bl 0x02008f9c
	movs	r0, #10
	bl 0x02008fc4
	adds	r0, r7, #0
	mov	r1, fp
	bl 0x020083fc
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00013333
	.4byte 0x02009128
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb520
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	movs	r5, #0
	bl 0x02008f64
	cmp	r0, #0
	bne.n	.L_020005dc
	bl 0x02008fcc
	movs	r0, #0
	bl 0x0200904c
	ldr	r0, [pc, #180]
	movs	r1, #0
	bl 0x0200843c
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02008f6c
	movs	r5, #1
.L_020005dc:
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02008f64
	cmp	r0, #0
	bne.n	.L_0200060c
	cmp	r5, #0
	bne.n	.L_020005f0
	bl 0x02008fcc
.L_020005f0:
	movs	r0, #0
	bl 0x0200904c
	ldr	r0, [pc, #136]
	movs	r1, #1
	bl 0x0200843c
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02008f6c
	adds	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
.L_0200060c:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x02008f64
	cmp	r0, #0
	bne.n	.L_02000640
	cmp	r5, #0
	bne.n	.L_02000622
	bl 0x02008fcc
.L_02000622:
	movs	r0, #0
	bl 0x0200904c
	ldr	r0, [pc, #84]
	movs	r1, #2
	bl 0x0200843c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x02008f6c
	adds	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
.L_02000640:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02008f64
	cmp	r0, #0
	bne.n	.L_02000674
	cmp	r5, #0
	bne.n	.L_02000656
	bl 0x02008fcc
.L_02000656:
	movs	r0, #0
	bl 0x0200904c
	ldr	r0, [pc, #32]
	movs	r1, #3
	bl 0x0200843c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02008f6c
	adds	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
.L_02000674:
	cmp	r5, #0
	beq.n	.L_0200067c
	bl 0x02008fd4
.L_0200067c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x917c
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #10
	movs	r5, #0
	bl 0x02008f64
	cmp	r0, #0
	bne.n	.L_020006b4
	bl 0x02008fcc
	movs	r0, #0
	bl 0x0200904c
	ldr	r0, [pc, #128]
.L_020006a2:
	movs	r1, #0
	bl 0x0200843c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x02008f6c
	movs	r5, #1
.L_020006b4:
	movs	r0, #131
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02008f64
	cmp	r0, #0
	bne.n	.L_020006e8
	cmp	r5, #0
	bne.n	.L_020006ca
	bl 0x02008fcc
.L_020006ca:
	movs	r0, #0
	bl 0x0200904c
	ldr	r0, [pc, #80]
	movs	r1, #1
	bl 0x0200843c
	movs	r0, #131
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02008f6c
	adds	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
.L_020006e8:
	movs	r0, #195
	lsls	r0, r0, #2
	bl 0x02008f64
	cmp	r0, #0
	bne.n	.L_02000718
	cmp	r5, #0
	bne.n	.L_020006fc
	bl 0x02008fcc
.L_020006fc:
	movs	r0, #0
	bl 0x0200904c
	ldr	r0, [pc, #32]
	movs	r1, #2
	bl 0x0200843c
	movs	r0, #195
	lsls	r0, r0, #2
	bl 0x02008f6c
	adds	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
.L_02000718:
	cmp	r5, #0
	beq.n	.L_02000720
	bl 0x02008fd4
.L_02000720:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x91cc
	.2byte 0x0200
	push	{lr}
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02008f64
	cmp	r0, #0
	bne.n	.L_02000758
	bl 0x02008fcc
	movs	r0, #0
	bl 0x0200904c
	ldr	r0, [pc, #24]
	movs	r1, #4
	bl 0x0200843c
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02008f6c
	bl 0x02008fd4
.L_02000758:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x917c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r0, r6, #0
	bl 0x02008fdc
	ldr	r3, [pc, #228]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #220]
	adds	r5, r0, #0
	cmp	r2, r3
	bne.n	.L_020007be
	cmp	r6, #8
	bne.n	.L_02000792
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #16
	bne.n	.L_02000792
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008f6c
.L_02000792:
	cmp	r6, #9
	bne.n	.L_020007a8
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #20
	bne.n	.L_020007a8
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008f6c
.L_020007a8:
	cmp	r6, #10
	bne.n	.L_020007be
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #27
	bne.n	.L_020007be
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008f6c
.L_020007be:
	ldr	r3, [pc, #144]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #140]
	cmp	r2, r3
	bne.n	.L_020007fc
	cmp	r6, #10
	bne.n	.L_020007e4
	ldr	r3, [r5, #8]
	asrs	r3, r3, #19
	cmp	r3, #33
	bne.n	.L_020007e4
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x02008f6c
.L_020007e4:
	cmp	r6, #11
	bne.n	.L_020007fc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #19
	cmp	r3, #85
	bne.n	.L_020007fc
	ldr	r3, [r5, #16]
	asrs	r3, r3, #19
	cmp	r3, #83
	bne.n	.L_020007fc
	bl 0x02008728
.L_020007fc:
	ldr	r3, [pc, #80]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000824
	cmp	r6, #10
	bne.n	.L_02000824
	ldr	r3, [r5, #8]
	asrs	r3, r3, #19
	cmp	r3, #83
	bne.n	.L_02000824
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #9
	bl 0x02008f6c
.L_02000824:
	ldr	r3, [pc, #40]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_0200084c
	cmp	r6, #8
	bne.n	.L_0200084c
	ldr	r3, [r5, #8]
	asrs	r3, r3, #19
	cmp	r3, #87
	bne.n	.L_0200084c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #13
	bl 0x02008f6c
.L_0200084c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000012a
	.4byte 0x0000012b
	.4byte 0x0000012c
	.2byte 0x012d
	.2byte 0x0000
	push	{lr}
	movs	r0, #23
	movs	r1, #77
	bl 0x02009024
	pop	{pc}
	push	{lr}
	movs	r0, #9
	movs	r1, #0
	movs	r2, #19
	bl 0x02009054
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	cmp	r2, #49
	bgt.n	.L_0200088e
	ldr	r0, [pc, #128]
	subs	r2, #40
	b.n	.L_02000892
.L_0200088e:
	ldr	r0, [pc, #128]
	subs	r2, #50
.L_02000892:
	cmp	r1, #15
	bne.n	.L_020008aa
	lsls	r3, r2, #4
	adds	r3, r3, r0
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	adds	r2, #63
	subs	r3, #2
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #3
	b.n	.L_020008c0
.L_020008aa:
	cmp	r1, #30
	bne.n	.L_020008cc
	lsls	r3, r2, #4
	adds	r3, r3, r0
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	adds	r2, #63
	subs	r3, #2
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #6
.L_020008c0:
	movs	r1, #100
	movs	r2, #3
	movs	r3, #4
	bl 0x02008f9c
	b.n	.L_02000906
.L_020008cc:
	cmp	r1, #45
	bne.n	.L_02000906
	lsls	r5, r2, #4
	adds	r5, r5, r0
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r2, #63
	subs	r3, #2
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #9
	movs	r1, #100
	movs	r2, #3
	movs	r3, #4
	bl 0x02008f9c
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #4]
	movs	r0, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x02008f94
	ldrh	r0, [r5, #12]
	bl 0x02008f74
.L_02000906:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200917c
	.2byte 0x91cc
	.2byte 0x0200
	push	{lr}
	movs	r0, #9
	bl 0x02008fdc
	ldr	r1, [r0, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	adds	r0, #35
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldrb	r2, [r0, #0]
	strb	r3, [r1, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008f6c
	pop	{pc}
	push	{lr}
	movs	r0, #10
	bl 0x02008fdc
	ldr	r1, [r0, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	adds	r0, #35
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldrb	r2, [r0, #0]
	strb	r3, [r1, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008f6c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #11
	bl 0x02008fdc
	ldr	r1, [r0, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	adds	r0, #35
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldrb	r2, [r0, #0]
	strb	r3, [r1, #9]
.L_0200098c:
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008f6c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #12
	bl 0x02008fdc
	ldr	r1, [r0, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	adds	r0, #35
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldrb	r2, [r0, #0]
	strb	r3, [r1, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008f6c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #13
	bl 0x02008fdc
	ldr	r1, [r0, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	adds	r0, #35
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldrb	r2, [r0, #0]
	strb	r3, [r1, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02008f6c
	pop	{pc}
	push	{lr}
	movs	r0, #14
	bl 0x02008fdc
	ldr	r1, [r0, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
.L_02000a0a:
	negs	r3, r3
	adds	r0, #35
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldrb	r2, [r0, #0]
	strb	r3, [r1, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008f6c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #15
	bl 0x02008fdc
	ldr	r1, [r0, #80]
	movs	r3, #13
.L_02000a38:
	ldrb	r2, [r1, #9]
	negs	r3, r3
	adds	r0, #35
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldrb	r2, [r0, #0]
	strb	r3, [r1, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02008f6c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #16
	bl 0x02008fdc
	ldr	r1, [r0, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
.L_02000a6a:
	negs	r3, r3
	adds	r0, #35
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	ldrb	r2, [r0, #0]
	strb	r3, [r1, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008f6c
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x02008fcc
	movs	r0, #0
	bl 0x0200904c
	movs	r1, #2
	movs	r0, #8
	bl 0x02008ff4
	movs	r0, #20
	bl 0x02008fc4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200903c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x02009034
	movs	r0, #60
	bl 0x02009044
	movs	r0, #60
	bl 0x02008fc4
	movs	r2, #6
	ldr	r0, [pc, #104]
	movs	r1, #0
	bl 0x02008fbc
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x02009034
	movs	r0, #60
	bl 0x02009044
.L_02000ae2:
	movs	r0, #60
	bl 0x02008fc4
	ldr	r5, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02008fdc
	cmp	r0, #0
	beq.n	.L_02000b08
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x02008fe4
.L_02000b08:
	movs	r0, #20
	bl 0x02008fc4
	movs	r3, #149
	lsls	r3, r3, #2
.L_02000b12:
	adds	r2, r5, r3
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #187
	strh	r3, [r2, #0]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #103
	movs	r1, #0
	bl 0x0200901c
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000030a8
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #56]
	movs	r1, #240
	lsls	r1, r1, #1
.L_02000b44:
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000b54
	ldr	r0, [pc, #44]
	b.n	.L_02000b74
.L_02000b54:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000b5e
	ldr	r0, [pc, #44]
	b.n	.L_02000b74
.L_02000b5e:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000b68
	ldr	r0, [pc, #40]
	b.n	.L_02000b74
.L_02000b68:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000b72
	ldr	r0, [pc, #40]
	b.n	.L_02000b74
.L_02000b72:
	ldr	r0, [pc, #40]
.L_02000b74:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000012b
	.4byte 0x02009780
	.4byte 0x0000012c
	.4byte 0x0200984c
	.4byte 0x0000012d
	.4byte 0x020098f4
	.4byte 0x0000012e
	.4byte 0x02009a5c
	.2byte 0x96e4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #0
	beq.n	.L_02000bbc
	b.n	.L_02000cca
.L_02000bbc:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r3
	cmp	r3, #0
	beq.n	.L_02000bd2
	b.n	.L_02000cca
.L_02000bd2:
	ldr	r7, [pc, #252]
	movs	r2, #1
	ldr	r3, [r7, #0]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000c14
	ldr	r6, [pc, #244]
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r5, [pc, #240]
	adds	r0, r6, #0
	adds	r1, #96
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c31
	movs	r2, #32
	ldr	r0, [pc, #228]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4e39
	ldr	r1, [pc, #228]
	movs	r2, #32
	adds	r0, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4838
	adds	r1, r6, #0
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4643
	str	r3, [r7, #0]
.L_02000c14:
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_02000c1c
	adds	r3, #7
.L_02000c1c:
	asrs	r2, r3, #3
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02000c26
	adds	r3, r2, #3
.L_02000c26:
	asrs	r1, r3, #2
	lsls	r3, r1, #2
	subs	r1, r2, r3
	ldr	r5, [pc, #184]
	ldr	r2, [pc, #176]
	ldr	r7, [pc, #168]
	ldr	r6, [pc, #160]
	movs	r4, #0
	mov	ip, r5
	mov	r8, r2
	movs	r0, #4
.L_02000c3c:
	adds	r2, r1, r4
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02000c46
	adds	r3, r2, #3
.L_02000c46:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	ldrh	r2, [r6, r0]
	lsls	r3, r3, #1
	adds	r3, #4
	strh	r2, [r7, r3]
	mov	r5, r8
	ldrh	r5, [r5, r0]
	mov	r2, ip
	adds	r4, #1
	strh	r5, [r2, r3]
	adds	r0, #2
	cmp	r4, #3
	ble.n	.L_02000c3c
	ldr	r0, [pc, #132]
	ldr	r1, [pc, #136]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000c94
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r0
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r7}
	strh	r2, [r0, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #96
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_02000c94:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000cc0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	adds	r3, #4
	strh	r2, [r0, #0]
	mov	r2, ip
	stmia	r3!, {r2}
	ldr	r2, [pc, #44]
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_02000cc0:
	strh	r4, [r1, #0]
	ldr	r2, [pc, #12]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_02000cca:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02009aa4
	.4byte 0x02009aa8
	.4byte 0x03000730
	.4byte 0x02009ac8
	.4byte 0x02009ae8
	.4byte 0x05000140
	.4byte 0x02009b08
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	bl 0x0200905c
	ldr	r3, [pc, #536]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #528]
	cmp	r2, r3
	bne.n	.L_02000d8c
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008f6c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008f6c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008f6c
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000d54
	movs	r1, #132
	movs	r2, #220
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02008fec
.L_02000d54:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000d70
	movs	r1, #164
	movs	r2, #220
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02008fec
.L_02000d70:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000d8c
	movs	r1, #220
	movs	r2, #172
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02008fec
.L_02000d8c:
	ldr	r3, [pc, #408]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #404]
	cmp	r2, r3
	bne.n	.L_02000e38
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #400]
	bl 0x02008f5c
	movs	r0, #0
	movs	r1, #8
	movs	r2, #9
	bl 0x02009064
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000dcc
	movs	r1, #132
	movs	r2, #184
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02008fec
.L_02000dcc:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000de2
	ldr	r0, [pc, #348]
	movs	r1, #0
	bl 0x020083fc
.L_02000de2:
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000df6
	ldr	r0, [pc, #328]
	movs	r1, #1
	bl 0x020083fc
.L_02000df6:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000e0c
	ldr	r0, [pc, #304]
	movs	r1, #2
	bl 0x020083fc
.L_02000e0c:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000e22
	ldr	r0, [pc, #284]
	movs	r1, #3
	bl 0x020083fc
.L_02000e22:
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000e38
	ldr	r0, [pc, #260]
	movs	r1, #4
	bl 0x020083fc
.L_02000e38:
	ldr	r3, [pc, #236]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #244]
	cmp	r2, r3
	bne.n	.L_02000eba
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #228]
	bl 0x02008f5c
	movs	r0, #0
	movs	r1, #8
	movs	r2, #9
	bl 0x02009064
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #9
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000e7a
	movs	r1, #166
	movs	r2, #248
	movs	r0, #10
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x02008fec
.L_02000e7a:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000e90
	ldr	r0, [pc, #180]
	movs	r1, #0
	bl 0x020083fc
.L_02000e90:
	movs	r0, #131
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000ea6
	ldr	r0, [pc, #160]
	movs	r1, #1
	bl 0x020083fc
.L_02000ea6:
	movs	r0, #195
	lsls	r0, r0, #2
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000eba
	ldr	r0, [pc, #140]
	movs	r1, #2
	bl 0x020083fc
.L_02000eba:
	ldr	r3, [pc, #108]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_02000f22
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #13
	bl 0x02008f64
	cmp	r0, #0
	beq.n	.L_02000ee8
	movs	r1, #174
	movs	r2, #150
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02008fec
.L_02000ee8:
	movs	r1, #17
	movs	r2, #18
	movs	r0, #0
	bl 0x02009064
	movs	r1, #19
	movs	r2, #20
	movs	r0, #1
	bl 0x02009064
	movs	r0, #2
	movs	r1, #21
	movs	r2, #22
	bl 0x02009064
	movs	r5, #0
.L_02000f08:
	adds	r0, r5, #0
	adds	r0, #9
	bl 0x02008fdc
	ldr	r2, [r0, #80]
	movs	r1, #12
	adds	r2, #37
	ldrb	r3, [r2, #0]
	adds	r5, #1
	orrs	r3, r1
	strb	r3, [r2, #0]
	cmp	r5, #7
	ble.n	.L_02000f08
.L_02000f22:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000012a
	.4byte 0x0000012b
	.4byte 0x02008ba1
	.4byte 0x0200917c
	.4byte 0x0000012c
	.4byte 0x020091cc
	.2byte 0x012d
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{lr}
	bl 0x0200902c
	pop	{pc}
	.irp EntryTarget, 0x03000528, 0x080000d1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080201e9, 0x080201f1, 0x08020219, 0x08020221, 0x08020229, 0x08038211, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c80c1, 0x080c80f9, 0x080c8141, 0x080c8171, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8281, 0x080c8289, 0x080c82e1, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c84e1, 0x080c8581, 0x080c86d1, 0x080c86d9, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0xc0010000
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
	.4byte 0x02009074
	.4byte 0x020090b0
	.4byte 0x020090ec
	.4byte 0x00000007
	.4byte 0x00000015
	.4byte 0x0000000c
	.4byte 0x00000303
	.4byte 0x00000007
	.4byte 0x00000011
	.4byte 0x0000000d
	.4byte 0x00000304
	.4byte 0x0000000c
	.4byte 0x0000000f
	.4byte 0x0000000e
	.4byte 0x00000305
	.4byte 0x00000009
	.4byte 0x0000000b
	.4byte 0x0000000f
	.4byte 0x00000306
	.4byte 0x0000002e
	.4byte 0x00000026
	.4byte 0x00000010
	.4byte 0x00000307
	.4byte 0x00000029
	.4byte 0x00000017
	.4byte 0x0000000b
	.4byte 0x0000030a
	.4byte 0x0000002f
	.4byte 0x00000017
	.4byte 0x0000000c
	.4byte 0x0000030b
	.4byte 0x00000035
	.4byte 0x00000017
	.4byte 0x0000000d
	.4byte 0x0000030c
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
	.4byte 0x0000012a
	.4byte 0x10157002
	.4byte 0xffffffff
	.4byte 0x1020312a
	.4byte 0xffffffff
	.4byte 0x1030212a
	.4byte 0xffffffff
	.4byte 0x1040512a
	.4byte 0xffffffff
	.4byte 0x1050412a
	.4byte 0xffffffff
	.4byte 0x1060712a
	.4byte 0xffffffff
	.4byte 0x1070612a
	.4byte 0xffffffff
	.4byte 0x1080112b
	.4byte 0xffffffff
	.4byte 0x0000012b
	.4byte 0x1010812a
	.4byte 0xffffffff
	.4byte 0x1020312b
	.4byte 0xffffffff
	.4byte 0x1030212b
	.4byte 0xffffffff
	.4byte 0x1040112c
	.4byte 0xffffffff
	.4byte 0x1050212c
	.4byte 0xffffffff
	.4byte 0x0000012c
	.4byte 0x1010412b
	.4byte 0xffffffff
	.4byte 0x1020512b
	.4byte 0xffffffff
	.4byte 0x1030412c
	.4byte 0xffffffff
	.4byte 0x1040312c
	.4byte 0xffffffff
	.4byte 0x1050112d
	.4byte 0xffffffff
	.4byte 0x0000012d
	.4byte 0x1010512c
	.4byte 0xffffffff
	.4byte 0x1020312d
	.4byte 0xffffffff
	.4byte 0x1030212d
	.4byte 0xffffffff
	.4byte 0x1040512d
	.4byte 0xffffffff
	.4byte 0x1050412d
	.4byte 0xffffffff
	.4byte 0x1060112e
	.4byte 0xffffffff
	.4byte 0x0000012e
	.4byte 0x1010612d
	.4byte 0xffffffff
	.4byte 0x1020312e
	.4byte 0xffffffff
	.4byte 0x1030212e
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
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
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00024000
	.4byte 0xffff0149
	.4byte 0x020094e0
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x020094e0
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x020094e0
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x020094e0
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x020094e0
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x020094e0
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x020094e0
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x020094e0
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x0200931c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0x007d00f6
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x09bb00a9
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x004300f3
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008761
	.4byte 0x00008c15
	.4byte 0x03000008
	.4byte 0x02008761
	.4byte 0x00008c15
	.4byte 0x03010009
	.4byte 0x02008761
	.4byte 0x00008c15
	.4byte 0x0302000a
	.4byte 0x02008761
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
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x020085ad
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02008729
	.4byte 0x00000602
	.4byte 0xffff0023
	.4byte 0x02008f4d
	.4byte 0x50009805
	.4byte 0x13030028
	.4byte 0x02008881
	.4byte 0x50009805
	.4byte 0x13040029
	.4byte 0x02008881
	.4byte 0x50009805
	.4byte 0x1305002a
	.4byte 0x02008881
	.4byte 0x50009805
	.4byte 0x1306002b
	.4byte 0x02008881
	.4byte 0x50009805
	.4byte 0x1307002c
	.4byte 0x02008881
	.4byte 0x00008c15
	.4byte 0x0308000a
	.4byte 0x02008761
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02008761
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008761
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
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x0000c602
	.4byte 0xffff0023
	.4byte 0x02008f4d
	.4byte 0x00000602
	.4byte 0xffff0023
	.4byte 0x02008f4d
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x02008685
	.4byte 0x50009805
	.4byte 0x130a0032
	.4byte 0x02008881
	.4byte 0x50009805
	.4byte 0x130b0033
	.4byte 0x02008881
	.4byte 0x50009805
	.4byte 0x130c0034
	.4byte 0x02008881
	.4byte 0x00008c15
	.4byte 0x0309000a
	.4byte 0x02008761
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008761
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
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00008602
	.4byte 0x0200001f
	.4byte 0x02008f4d
	.4byte 0x00000602
	.4byte 0x02000020
	.4byte 0x02008f4d
	.4byte 0x00008602
	.4byte 0x02010021
	.4byte 0x02008f4d
	.4byte 0x00000602
	.4byte 0x02010022
	.4byte 0x02008f4d
	.4byte 0x00008602
	.4byte 0x02020023
	.4byte 0x02008f4d
	.4byte 0x00000602
	.4byte 0x02020024
	.4byte 0x02008f4d
	.4byte 0x00008602
	.4byte 0x02030025
	.4byte 0x02008f4d
	.4byte 0x00000602
	.4byte 0x02030026
	.4byte 0x02008f4d
	.4byte 0x00008602
	.4byte 0x02040027
	.4byte 0x02008f4d
	.4byte 0x00000602
	.4byte 0x02040028
	.4byte 0x02008f4d
	.4byte 0x00008602
	.4byte 0x02050029
	.4byte 0x02008f4d
	.4byte 0x00000602
	.4byte 0x0205002a
	.4byte 0x02008f4d
	.4byte 0x00000400
	.4byte 0xffff0017
	.4byte 0x02008865
	.4byte 0x00008c15
	.4byte 0x030d0008
	.4byte 0x02008761
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008761
	.4byte 0x00008f15
	.4byte 0x02000009
	.4byte 0x02008915
	.4byte 0x00008f15
	.4byte 0x0201000a
	.4byte 0x02008941
	.4byte 0x00008f15
	.4byte 0x0202000b
	.4byte 0x02008971
	.4byte 0x00008f15
	.4byte 0x0203000c
	.4byte 0x020089a1
	.4byte 0x00008f15
	.4byte 0x0204000d
	.4byte 0x020089d1
	.4byte 0x00008f15
	.4byte 0x0205000e
	.4byte 0x020089fd
	.4byte 0x00008f15
	.4byte 0x0206000f
	.4byte 0x02008a2d
	.4byte 0x00008f15
	.4byte 0x02070010
	.4byte 0x02008a5d
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
	.4byte 0x09bb002a
	.4byte 0x02008a8d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008871
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
