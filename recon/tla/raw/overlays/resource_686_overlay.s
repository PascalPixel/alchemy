.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200a439, 0x0200833d, 0x0200837d, 0x02008601, 0x0200a339, 0x02008345, 0x0200ad51
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
	bl 0x0200ade4
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
	bl 0x0200ae14
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200ae84
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200ae1c
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
	bl 0x0200ade4
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
	bl 0x0200ae14
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x0200ae84
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
	bl 0x0200ae54
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
	bl 0x0200ade4
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
	bl 0x0200add4
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200addc
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200ae14
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
	bl 0x0200ae84
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
	bl 0x0200ad54
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
	bl 0x0200ad54
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x0200ad54
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
	bl 0x0200add4
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200addc
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
	.4byte 0x0200b040
	.4byte 0x02008125
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x4800
	bx	lr
	.2byte 0xb04c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_0200035c
	ldr	r0, [pc, #20]
	b.n	.L_02000366
.L_0200035c:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000366
	ldr	r0, [pc, #16]
.L_02000366:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000c1
	.4byte 0x0200b07c
	.4byte 0x000000c4
	.2byte 0xb09c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb0bc
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #52]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200ae8c
	movs	r2, #128
	movs	r1, #6
	lsls	r2, r2, #5
	ldr	r0, [r5, #0]
	bl 0x0200ae7c
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200aeec
	movs	r0, #145
	lsls	r0, r0, #1
	bl 0x0200adbc
	movs	r0, #13
	bl 0x0200aebc
	pop	{r5, pc}
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
	bl 0x0200ad54
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020003f0
	adds	r3, #15
.L_020003f0:
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
	ldr	r3, [pc, #392]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200ae54
	adds	r7, r0, #0
	bl 0x0200ae44
	movs	r0, #0
	bl 0x0200aedc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200aeac
	bl 0x0200adec
	movs	r0, #1
	bl 0x0200ad5c
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
	bl 0x0200aec4
	bl 0x0200aecc
	movs	r0, #204
	bl 0x0200aeec
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200ae3c
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
.L_020004b2:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200ad84
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200ad7c
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200ad74
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #204]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200ad74
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
	bls.n	.L_020004b2
	movs	r0, #188
	bl 0x0200aeec
	ldr	r5, [pc, #128]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200ae9c
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200ae74
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ae24
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200ae24
	bl 0x0200ae2c
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200ae9c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200ae3c
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200ae74
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200ae4c
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x020083c1
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, lr}
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200ae14
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	ldr	r2, [pc, #12]
	ldr	r3, [r5, #12]
	movs	r0, #0
	adds	r3, r3, r2
	str	r3, [r5, #12]
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb500
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02000618
	ldr	r0, [pc, #72]
	b.n	.L_02000656
.L_02000618:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02000622
	ldr	r0, [pc, #72]
	b.n	.L_02000656
.L_02000622:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_0200062c
	ldr	r0, [pc, #68]
	b.n	.L_02000656
.L_0200062c:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000636
	ldr	r0, [pc, #68]
	b.n	.L_02000656
.L_02000636:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000640
	ldr	r0, [pc, #64]
	b.n	.L_02000656
.L_02000640:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_0200064a
	ldr	r0, [pc, #64]
	b.n	.L_02000656
.L_0200064a:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02000654
	ldr	r0, [pc, #60]
	b.n	.L_02000656
.L_02000654:
	ldr	r0, [pc, #60]
.L_02000656:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000bd
	.4byte 0x0200b2b8
	.4byte 0x000000be
	.4byte 0x0200b4f8
	.2byte 0x00bf
	.2byte 0x0000
	push	{r4, r5, r6, lr}
	lsls	r0, r0, #8
.L_02000674:
	lsls	r0, r0, #3
	movs	r0, r0
	push	{r4, r6, r7, lr}
	lsls	r0, r0, #8
	lsls	r1, r0, #3
	movs	r0, r0
	.2byte 0xb708
	lsls	r0, r0, #8
	lsls	r2, r0, #3
	movs	r0, r0
	.2byte 0xb750
	lsls	r0, r0, #8
	lsls	r4, r0, #3
	movs	r0, r0
	.2byte 0xb768
	lsls	r0, r0, #8
	.2byte 0xb2a0
	lsls	r0, r0, #8
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ae54
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
	bl 0x0200ae54
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #360]
	adds	r6, r1, #0
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #348]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000714
	cmp	r6, #8
	bne.n	.L_02000714
	movs	r3, #4
	movs	r2, #69
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #4
	movs	r1, #68
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	movs	r3, #3
	movs	r2, #70
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #3
	movs	r1, #68
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
.L_02000714:
	ldr	r3, [pc, #292]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #288]
	cmp	r2, r3
	bne.n	.L_02000782
	cmp	r6, #19
	bne.n	.L_02000752
	movs	r3, #38
	movs	r2, #72
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #72
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	movs	r3, #37
	movs	r2, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #36
	movs	r1, #72
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
.L_02000752:
	cmp	r6, #8
	bne.n	.L_0200076a
	movs	r3, #3
	movs	r2, #113
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #2
	movs	r1, #113
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
.L_0200076a:
	cmp	r6, #18
	bne.n	.L_02000782
	movs	r3, #40
	movs	r2, #93
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #39
	movs	r1, #93
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
.L_02000782:
	ldr	r3, [pc, #184]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #184]
	cmp	r2, r3
	bne.n	.L_020007c0
	cmp	r6, #8
	bne.n	.L_020007c0
	movs	r3, #32
	movs	r2, #70
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r1, #69
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	movs	r3, #33
	movs	r2, #71
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #69
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
.L_020007c0:
	ldr	r3, [pc, #120]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_02000836
	cmp	r6, #8
	bne.n	.L_02000810
	movs	r3, #17
	str	r3, [sp, #0]
	movs	r5, #83
	movs	r0, #17
	movs	r1, #85
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200adfc
	movs	r3, #18
	movs	r2, #82
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #19
	movs	r1, #84
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	movs	r3, #19
	str	r3, [sp, #0]
	movs	r0, #19
	movs	r1, #84
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200adfc
.L_02000810:
	cmp	r6, #9
	bne.n	.L_02000836
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02000836
	movs	r3, #25
	movs	r2, #98
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #98
	movs	r2, #3
	movs	r3, #5
	bl 0x0200adf4
.L_02000836:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000bd
	.4byte 0x000000c0
	.4byte 0x000000bf
	.2byte 0x00c1
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	sub	sp, #8
	bl 0x0200ae54
	adds	r7, r0, #0
	cmp	r5, #8
	bne.n	.L_02000924
	ldr	r3, [pc, #496]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #488]
	cmp	r2, r3
	bne.n	.L_020008c2
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #4
	bne.n	.L_0200089a
	movs	r3, #2
	movs	r2, #69
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #4
	movs	r1, #68
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #198
	bl 0x0200adbc
.L_0200089a:
	movs	r3, #4
	movs	r2, #69
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #3
	movs	r1, #68
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	movs	r3, #3
	movs	r2, #70
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #3
	movs	r1, #69
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
.L_020008c2:
	cmp	r5, #8
	bne.n	.L_02000924
	ldr	r3, [pc, #396]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #392]
	cmp	r2, r3
	bne.n	.L_02000924
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #32
	movs	r1, #71
	movs	r2, #1
	movs	r3, #1
	movs	r6, #33
	str	r6, [sp, #0]
	bl 0x0200adfc
	movs	r3, #32
	movs	r2, #70
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #31
	movs	r1, #70
	movs	r2, #1
	bl 0x0200adfc
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #32
	bne.n	.L_02000924
	movs	r3, #69
	str	r3, [sp, #4]
	movs	r0, #32
	movs	r1, #69
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200adfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #199
	bl 0x0200adbc
.L_02000924:
	ldr	r3, [pc, #300]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #300]
	cmp	r2, r3
	bne.n	.L_020009da
	cmp	r5, #8
	bne.n	.L_02000960
	movs	r3, #3
	movs	r2, #113
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #3
	movs	r1, #114
	movs	r2, #1
	bl 0x0200adfc
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #2
	bne.n	.L_02000960
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #200
	bl 0x0200adbc
.L_02000960:
	cmp	r5, #18
	bne.n	.L_0200098a
	movs	r3, #40
	movs	r2, #93
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #40
	movs	r1, #94
	movs	r2, #1
	bl 0x0200adfc
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #39
	bne.n	.L_0200098a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #201
	bl 0x0200adbc
.L_0200098a:
	cmp	r5, #19
	bne.n	.L_020009da
	movs	r3, #38
	movs	r2, #72
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #36
	movs	r1, #72
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	movs	r3, #73
	str	r3, [sp, #4]
	movs	r6, #37
	movs	r3, #1
	movs	r0, #37
	movs	r1, #74
	movs	r2, #1
	str	r6, [sp, #0]
	bl 0x0200adfc
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #38
	bne.n	.L_020009da
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #37
	movs	r1, #70
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200adfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #202
	bl 0x0200adbc
.L_020009da:
	cmp	r5, #8
	bne.n	.L_02000a4e
	ldr	r3, [pc, #116]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #120]
	cmp	r2, r3
	bne.n	.L_02000a4e
	movs	r3, #17
	str	r3, [sp, #0]
	movs	r5, #83
	movs	r0, #16
	movs	r1, #83
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200adfc
	movs	r3, #82
	str	r3, [sp, #4]
	movs	r0, #19
	movs	r1, #82
	movs	r2, #1
	movs	r3, #1
	movs	r6, #18
	str	r6, [sp, #0]
	bl 0x0200adfc
	movs	r3, #19
	str	r3, [sp, #0]
	movs	r0, #19
	movs	r3, #1
	movs	r1, #82
	movs	r2, #1
	str	r5, [sp, #4]
	bl 0x0200adfc
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #17
	bne.n	.L_02000a4e
	movs	r3, #84
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r1, #84
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200adfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #203
	bl 0x0200adbc
.L_02000a4e:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000bd
	.4byte 0x000000bf
	.4byte 0x000000c0
	.2byte 0x00c1
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x0200ae54
	adds	r6, r0, #0
	ldr	r3, [r6, #16]
	movs	r2, #60
	asrs	r3, r3, #20
	mov	r8, r2
	cmp	r3, #31
	bne.n	.L_02000b26
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r1, #3
	bl 0x0200ae34
	movs	r0, #204
	bl 0x0200aeec
	movs	r7, #236
	lsls	r7, r7, #14
	b.n	.L_02000aa8
.L_02000a9e:
	ldr	r2, [pc, #140]
	adds	r3, r7, #0
	asrs	r3, r3, #16
	adds	r7, r7, r2
	mov	r8, r3
.L_02000aa8:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02000aba
	movs	r0, #1
	bl 0x0200ad5c
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02000a9e
.L_02000aba:
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
	cmp	r5, #10
	bne.n	.L_02000ad2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #210
	bl 0x0200adbc
.L_02000ad2:
	cmp	r5, #11
	bne.n	.L_02000ae0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #211
	bl 0x0200adbc
.L_02000ae0:
	cmp	r5, #12
	bne.n	.L_02000aee
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #212
	bl 0x0200adbc
.L_02000aee:
	cmp	r5, #13
	bne.n	.L_02000afc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #213
	bl 0x0200adbc
.L_02000afc:
	cmp	r5, #14
	bne.n	.L_02000b0a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #214
	bl 0x0200adbc
.L_02000b0a:
	cmp	r5, #15
	bne.n	.L_02000b18
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #215
	bl 0x0200adbc
.L_02000b18:
	cmp	r5, #16
	bne.n	.L_02000b26
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #216
	bl 0x0200adbc
.L_02000b26:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	bl 0x0200ae54
	movs	r2, #85
	adds	r5, r0, #0
	adds	r2, r2, r5
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r6, #60
	mov	r8, r2
.L_02000b4a:
	cmp	r6, #0
	beq.n	.L_02000b5c
	movs	r0, #1
	bl 0x0200ad5c
	ldr	r3, [r5, #40]
	subs	r6, #1
	cmp	r3, #0
	bne.n	.L_02000b4a
.L_02000b5c:
	cmp	r7, #0
	beq.n	.L_02000b66
	adds	r0, r7, #0
	bl 0x0200aeec
.L_02000b66:
	movs	r0, #10
	bl 0x0200ad5c
	movs	r3, #0
	mov	r2, r8
	strb	r3, [r2, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r2, [sp, #20]
	movs	r2, #28
	str	r3, [sp, #16]
	add	r2, sp
	movs	r3, #1
	str	r1, [sp, #24]
	str	r3, [r2, #4]
	movs	r3, #192
	lsls	r3, r3, #8
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	fp, r0
	mov	r9, r3
	mov	r8, r2
	cmp	r9, fp
	bge.n	.L_02000c16
	ldr	r2, [sp, #100]
	mov	sl, r2
.L_02000bae:
	ldr	r3, [sp, #104]
	cmp	r3, #0
	ble.n	.L_02000c04
	ldr	r2, [sp, #20]
	adds	r6, r3, #0
	lsls	r7, r2, #16
.L_02000bba:
	bl 0x0200ad74
	movs	r5, #15
	ands	r5, r0
	bl 0x0200ad74
	ldr	r3, [sp, #16]
	movs	r1, #31
	ands	r1, r0
	adds	r1, r3, r1
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	str	r3, [sp, #4]
	movs	r3, #128
	subs	r5, #8
	lsls	r3, r3, #12
	adds	r3, #1
	lsls	r5, r5, #16
	adds	r5, r7, r5
	str	r3, [sp, #8]
	mov	r2, r8
	mov	r3, sl
	str	r2, [sp, #12]
	lsls	r1, r1, #16
	lsls	r2, r3, #16
	adds	r0, r5, #0
	movs	r3, #0
	bl 0x0200815c
	movs	r2, #128
	lsls	r2, r2, #13
	subs	r6, #1
	adds	r7, r7, r2
	cmp	r6, #0
	bne.n	.L_02000bba
.L_02000c04:
	ldr	r0, [sp, #24]
	bl 0x0200ae3c
	movs	r2, #1
	movs	r3, #2
	add	r9, r2
	add	sl, r3
	cmp	r9, fp
	blt.n	.L_02000bae
.L_02000c16:
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r1, #0
	adds	r0, r5, #0
	sub	sp, #8
	bl 0x0200ae54
	ldr	r3, [r0, #8]
	asrs	r7, r3, #20
	ldr	r3, [r0, #16]
	asrs	r6, r3, #20
	cmp	r5, #9
	bne.n	.L_02000c5e
	cmp	r7, #12
	bne.n	.L_02000c5e
	cmp	r6, #45
	bne.n	.L_02000c5e
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #217
	bl 0x0200adbc
	movs	r0, #9
	movs	r1, #181
	bl 0x02008b30
.L_02000c5e:
	cmp	r5, #10
	bne.n	.L_02000c7c
	cmp	r7, #14
	bne.n	.L_02000c7c
	cmp	r6, #45
	bne.n	.L_02000c7c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #218
	bl 0x0200adbc
	movs	r0, #10
	movs	r1, #181
	bl 0x02008b30
.L_02000c7c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #218
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02000c8c
	b.n	.L_02000e50
.L_02000c8c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #217
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02000c9c
	b.n	.L_02000e50
.L_02000c9c:
	bl 0x0200ae44
	movs	r0, #0
	bl 0x0200aedc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200ae24
	movs	r0, #216
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #412]
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200aeac
	movs	r0, #15
	bl 0x0200ae3c
	movs	r0, #202
	bl 0x0200aeec
	movs	r5, #12
	movs	r6, #111
	movs	r1, #91
	movs	r2, #3
	movs	r3, #1
	movs	r0, #12
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ae04
	movs	r0, #60
	bl 0x0200ae3c
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aeec
	movs	r3, #47
	str	r3, [sp, #4]
	mov	r9, r3
	movs	r0, #16
	movs	r1, #27
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r0, #16
	movs	r1, #91
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r3, #40
	negs	r3, r3
	mov	sl, r3
	movs	r3, #186
	lsls	r3, r3, #2
	str	r3, [sp, #0]
	movs	r3, #3
	str	r3, [sp, #4]
	mov	r8, r3
	movs	r0, #3
	movs	r1, #15
	movs	r2, #200
	mov	r3, sl
	bl 0x02008b78
	mov	r3, r9
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r1, #27
	movs	r2, #3
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r0, #16
	movs	r1, #91
	movs	r2, #3
	movs	r3, #3
.L_02000d52:
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r3, #190
	lsls	r3, r3, #2
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r0, #3
	movs	r1, #10
	movs	r2, #200
	mov	r3, sl
	bl 0x02008b78
	mov	r3, r9
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r1, #27
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r0, #16
	movs	r1, #91
	movs	r2, #3
	movs	r3, #4
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r3, #194
	lsls	r3, r3, #2
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r0, #3
	movs	r1, #10
	movs	r2, #200
	mov	r3, sl
	bl 0x02008b78
	mov	r3, r9
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r1, #27
	movs	r2, #3
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r0, #16
	movs	r1, #91
	movs	r2, #3
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r3, #198
	lsls	r3, r3, #2
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r0, #3
	movs	r1, #10
	movs	r2, #200
	mov	r3, sl
	bl 0x02008b78
	mov	r3, r9
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r1, #27
	movs	r2, #3
	movs	r3, #6
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #6
	movs	r0, #16
	movs	r1, #91
	movs	r2, #3
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ae24
	movs	r3, #202
	lsls	r3, r3, #2
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r0, #3
	movs	r1, #10
	movs	r2, #200
	mov	r3, sl
	bl 0x02008b78
	mov	r3, r9
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r1, #27
	movs	r2, #3
	movs	r3, #13
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r0, #16
	movs	r1, #91
	movs	r2, #3
	movs	r3, #13
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	bl 0x0200ae4c
.L_02000e50:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0302
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r1, #0
	adds	r0, r5, #0
	sub	sp, #8
	bl 0x0200ae54
	ldr	r3, [r0, #8]
	asrs	r7, r3, #20
	ldr	r3, [r0, #16]
	asrs	r6, r3, #20
	cmp	r5, #8
	beq.n	.L_02000e80
	b.n	.L_02001040
.L_02000e80:
	cmp	r7, #12
	beq.n	.L_02000e86
	b.n	.L_02001040
.L_02000e86:
	cmp	r6, #5
	beq.n	.L_02000e8c
	b.n	.L_02001040
.L_02000e8c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #220
	bl 0x0200adbc
	movs	r1, #181
	movs	r0, #8
	bl 0x02008b30
	bl 0x0200ae44
	movs	r0, #0
	bl 0x0200aedc
	movs	r0, #15
	bl 0x0200ae3c
	movs	r0, #202
	bl 0x0200aeec
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #29
	movs	r1, #70
	movs	r2, #3
	movs	r3, #1
	str	r7, [sp, #0]
	bl 0x0200adf4
	movs	r0, #216
	movs	r1, #1
	movs	r2, #240
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #15
	bl 0x0200aeac
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200ae24
	movs	r0, #60
	bl 0x0200ae3c
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aeec
	movs	r3, #7
	str	r3, [sp, #4]
	movs	r5, #10
	mov	sl, r3
	movs	r0, #35
	movs	r1, #6
	movs	r2, #7
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #70
	movs	r2, #7
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #40
	negs	r3, r3
	mov	r8, r3
	movs	r3, #124
	str	r3, [sp, #0]
	movs	r3, #3
	str	r3, [sp, #4]
	movs	r0, #6
	movs	r1, #10
	movs	r2, #200
	mov	r3, r8
	bl 0x02008b78
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #6
	movs	r2, #7
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #70
	movs	r2, #7
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #140
	str	r3, [sp, #0]
	movs	r0, #3
	movs	r1, #10
	movs	r2, #184
	mov	r3, r8
	str	r6, [sp, #4]
	bl 0x02008b78
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #6
	movs	r2, #7
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #70
	movs	r2, #7
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #156
	str	r3, [sp, #0]
	movs	r0, #3
	movs	r1, #10
	movs	r2, #184
	mov	r3, r8
	str	r6, [sp, #4]
	bl 0x02008b78
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #6
	movs	r2, #7
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #70
	movs	r2, #7
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #172
	str	r3, [sp, #0]
	movs	r0, #3
	mov	r3, r8
	movs	r1, #10
	movs	r2, #184
	str	r6, [sp, #4]
	bl 0x02008b78
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ae24
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #6
	movs	r2, #7
	movs	r3, #6
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r1, #70
	movs	r2, #7
	movs	r3, #6
	movs	r0, #35
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r0, #15
	bl 0x0200ae3c
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #6
	movs	r2, #7
	movs	r3, #16
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #70
	movs	r2, #7
	movs	r3, #16
	str	r5, [sp, #0]
	bl 0x0200adf4
	bl 0x0200ae4c
.L_02001040:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x0200ae54
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r2, r3, #20
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	cmp	r5, #10
	bne.n	.L_020010e6
	cmp	r2, #15
	bne.n	.L_020010e6
	cmp	r3, #24
	bne.n	.L_020010e6
	movs	r0, #10
	bl 0x0200ae54
	movs	r1, #0
	bl 0x0200ae14
	ldr	r5, [pc, #108]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r5
	str	r3, [r6, #12]
	bl 0x0200ae3c
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r5
	str	r3, [r6, #12]
	bl 0x0200ae3c
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #84]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200ae3c
	ldr	r2, [pc, #76]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200ae3c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200ae74
	movs	r2, #0
	movs	r1, #0
	movs	r0, #10
	bl 0x0200ae6c
	movs	r0, #188
	bl 0x0200aeec
	movs	r0, #45
	bl 0x0200ae3c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200ae74
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200aeec
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #221
	bl 0x0200adbc
.L_020010e6:
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0xfffc0000
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #188]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200ae54
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	movs	r2, #230
	lsls	r2, r2, #1
	ldr	r5, [r3, #32]
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	mov	r8, r3
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_020011b0
	cmp	r5, #0
	bne.n	.L_02001134
	ldr	r1, [pc, #140]
	adds	r0, r1, #0
	b.n	.L_02001144
.L_02001134:
	movs	r1, #156
	lsls	r1, r1, #1
	movs	r2, #212
	adds	r3, r5, r1
	lsls	r2, r2, #1
	ldr	r0, [r3, #0]
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
.L_02001144:
	ldr	r3, [r6, #16]
	ldr	r4, [r6, #8]
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	asrs	r2, r4, #20
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r0, r0, r2
	adds	r1, r1, r2
	ldrb	r3, [r0, #3]
	ldrb	r1, [r1, #2]
	movs	r2, #4
	ands	r2, r3
	movs	r3, #232
	ands	r3, r1
	orrs	r2, r3
	cmp	r2, #236
	bne.n	.L_020011b0
	ldr	r3, [pc, #76]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #5
	beq.n	.L_020011b0
	mov	r1, sp
	str	r4, [r1, #0]
	movs	r2, #128
	ldr	r3, [r6, #12]
	lsls	r2, r2, #12
	str	r3, [r1, #4]
	adds	r0, r6, #0
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r1, #8]
	bl 0x0200ae0c
	cmp	r0, #0
	bge.n	.L_0200119e
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #66
	strh	r3, [r2, #0]
.L_0200119e:
	ldr	r3, [r6, #16]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r6, #16]
	mov	r1, r8
	ldr	r3, [r1, #16]
	adds	r3, r3, r2
	str	r3, [r1, #16]
.L_020011b0:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, lr}
	ldr	r5, [pc, #132]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200ae54
	adds	r6, r0, #0
	bl 0x0200ae44
	movs	r0, #0
	bl 0x0200aedc
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200ae74
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200ae9c
	adds	r2, r6, #0
	movs	r3, #0
	adds	r6, #90
	adds	r2, #85
	strb	r3, [r2, #0]
	strb	r3, [r6, #0]
	movs	r1, #0
	ldr	r0, [r5, #0]
	movs	r2, #8
	bl 0x0200ae5c
	ldr	r0, [r5, #0]
	bl 0x0200ae64
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200ae94
	movs	r0, #10
	bl 0x0200ae3c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r1, r1
	negs	r0, r0
	bl 0x0200aeac
	movs	r0, #204
	bl 0x0200aeec
	ldr	r0, [r5, #0]
	movs	r1, #188
	bl 0x02008b30
	movs	r3, #1
	strb	r3, [r6, #0]
	bl 0x0200ae4c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r3, r1, #0
	subs	r3, #9
	cmp	r3, #1
	bhi.n	.L_0200125c
	bl 0x02008c24
	b.n	.L_02001260
.L_0200125c:
	bl 0x02008850
.L_02001260:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #76]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r6, r3, r2
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #68]
	adds	r5, r1, #0
	cmp	r2, r3
	bne.n	.L_02001286
	movs	r2, #136
	lsls	r2, r2, #4
	adds	r2, #255
	adds	r0, r5, r2
	bl 0x0200adbc
.L_02001286:
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_0200129c
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #149
	adds	r0, r5, r2
	bl 0x0200adbc
.L_0200129c:
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020012b2
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #150
	adds	r0, r5, r2
	bl 0x0200adbc
.L_020012b2:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x000000bd
	.4byte 0x000000be
	.2byte 0x00c0
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
	ldr	r3, [r3, #32]
	sub	sp, #4
	mov	r8, r3
	mov	r2, r8
	adds	r2, #228
	ldr	r7, [r2, #0]
	ldr	r3, [pc, #292]
	ldr	r6, [r2, #4]
	mov	r1, r8
	ands	r7, r3
	ands	r6, r3
	ldr	r3, [r1, #0]
	ldr	r0, [pc, #284]
	ldr	r3, [r3, #4]
	ldr	r2, [pc, #284]
	str	r3, [sp, #0]
	mov	sl, r0
	movs	r1, #2
	ldrsh	r3, [r0, r1]
	movs	r0, #131
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsls	r0, r0, #1
	lsrs	r3, r3, #5
	mov	r5, sl
	adds	r0, #255
	mov	fp, r3
	adds	r5, #16
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_020013e8
	mov	r2, sl
	ldr	r3, [r2, #4]
	movs	r0, #128
	lsls	r0, r0, #14
	cmp	r3, r0
	bge.n	.L_020013e8
	movs	r1, #0
	mov	r9, r1
.L_02001328:
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #0]
	ldr	r0, [r5, #8]
	ldr	r4, [r5, #4]
	cmp	r3, #4
	bhi.n	.L_02001388
	ldr	r2, [pc, #220]
	lsls	r3, r3, #2
	ldr	r2, [r3, r2]
	mov	r3, sl
	mov	ip, r2
	ldr	r2, [r3, #4]
	mov	pc, ip
	.2byte 0x0000
	.4byte 0x02009358
	.4byte 0x02009366
	.4byte 0x0200936c
	.4byte 0x02009372
	.4byte 0x0200937a
	.4byte 0x035b2380
	.4byte 0x18801a89
	.4byte 0xdd11429a
	.4byte 0x1a89e00f
	.4byte 0xe00d1880
	.4byte 0x18801889
	.4byte 0x2380e00a
	.4byte 0x1889035b
	.4byte 0x2380e7f1
	.4byte 0x429a035b
	.4byte 0x1880da01
	.2byte 0xe000
	.2byte 0x4924
.L_02001388:
	subs	r1, r1, r7
	subs	r3, r4, r6
	subs	r2, r3, r0
	asrs	r3, r1, #16
	movs	r0, #167
	adds	r1, r3, #0
	asrs	r2, r2, #16
	adds	r3, #7
	lsls	r0, r0, #1
	subs	r1, #8
	subs	r2, #8
	cmp	r3, r0
	bhi.n	.L_020013dc
	movs	r3, #15
	negs	r3, r3
	cmp	r2, r3
	blt.n	.L_020013dc
	cmp	r2, #239
	bgt.n	.L_020013dc
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r1, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r5, #16]
	lsls	r3, r1, #16
	orrs	r2, r3
	ldr	r3, [pc, #88]
	mov	r0, fp
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #3
	orrs	r3, r0
	adds	r0, r5, #0
	str	r2, [r5, #20]
	str	r3, [r5, #24]
	adds	r0, #16
	movs	r1, #255
	bl 0x0200adac
.L_020013dc:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r5, #28
	cmp	r2, #7
	ble.n	.L_02001328
.L_020013e8:
	mov	r0, sl
	ldr	r2, [r0, #12]
	ldr	r1, [r0, #4]
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, r8
	adds	r2, r2, r1
	str	r2, [r3, #12]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffff0000
	.4byte 0x0200c030
	.4byte 0x020036e0
	.4byte 0x02009344
	.4byte 0xfff00000
	.2byte 0x0800
	.2byte 0x4000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #128
	sub	sp, #4
	ldr	r7, [pc, #168]
	bl 0x0200ad8c
	adds	r6, r0, #0
	str	r6, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r0, r0, r3
	mov	r8, r0
	movs	r0, #10
	adds	r5, r7, #0
	adds	r0, #255
	adds	r5, #16
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	0x0200945c
	ldr	r3, [pc, #132]
	adds	r0, r7, #0
	movs	r1, #240
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x221f
.L_0200145e:
	ldr	r0, [sp, #0]
	ldr	r3, [pc, #120]
	subs	r2, #1
	stmia	r0!, {r3}
	adds	r1, r0, #0
	str	r1, [sp, #0]
	cmp	r2, #0
	bge.n	.L_0200145e
	bl 0x0200ada4
	strh	r0, [r7, #2]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r6, #0
	asrs	r0, r0, #16
	bl 0x0200ad9c
	adds	r0, r6, #0
	bl 0x0200ad94
	mov	r1, r8
	ldr	r3, [r1, #8]
	str	r3, [r7, #8]
	ldr	r3, [r1, #12]
	str	r3, [r7, #12]
	movs	r2, #209
	movs	r3, #128
	lsls	r2, r2, #5
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	ldr	r0, [pc, #56]
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r1, #144
	lsls	r1, r1, #3
	bl 0x0200ad64
	ldr	r3, [pc, #44]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	subs	r3, #1
	lsls	r3, r3, #16
	lsls	r0, r0, #9
	mov	ip, r2
	cmp	r3, r0
	bhi.n	.L_0200153e
	b.n	.L_020014e8
	.2byte 0x0000
	.4byte 0x00008000
	.4byte 0x0200c030
	.4byte 0x03000258
	.4byte 0x11111111
	.4byte 0x020092c5
	.2byte 0x0240
	.2byte 0x0200
.L_020014e8:
	movs	r3, #160
	lsls	r3, r3, #14
	movs	r2, #240
	str	r3, [r5, #0]
	movs	r1, #0
	lsls	r2, r2, #15
	movs	r0, #1
	movs	r3, #224
	str	r0, [r5, #12]
	str	r1, [r5, #8]
	str	r2, [r5, #4]
	lsls	r3, r3, #14
	adds	r5, #28
	str	r3, [r5, #0]
	movs	r3, #216
	str	r0, [r5, #12]
	str	r1, [r5, #8]
	str	r2, [r5, #4]
	lsls	r3, r3, #16
	adds	r5, #28
	str	r3, [r5, #0]
	movs	r0, #2
	movs	r3, #232
	str	r1, [r5, #8]
	str	r2, [r5, #4]
	str	r0, [r5, #12]
	lsls	r3, r3, #16
	adds	r5, #28
	str	r3, [r5, #0]
	movs	r3, #184
	str	r1, [r5, #8]
	str	r2, [r5, #4]
	str	r0, [r5, #12]
	lsls	r3, r3, #16
	adds	r5, #28
	str	r3, [r5, #0]
	movs	r3, #176
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	movs	r3, #4
	str	r1, [r5, #8]
	str	r3, [r5, #12]
	adds	r5, #28
.L_0200153e:
	mov	r1, ip
	ldrh	r3, [r1, #0]
	movs	r2, #128
	subs	r3, #3
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_020015a4
	movs	r3, #160
	lsls	r3, r3, #14
	movs	r1, #220
	str	r3, [r5, #0]
	movs	r2, #0
	lsls	r1, r1, #17
	movs	r0, #1
	movs	r3, #224
	str	r1, [r5, #4]
	str	r2, [r5, #8]
	str	r0, [r5, #12]
	lsls	r3, r3, #14
	adds	r5, #28
	str	r3, [r5, #0]
	movs	r3, #216
	str	r1, [r5, #4]
	str	r2, [r5, #8]
	str	r0, [r5, #12]
	lsls	r3, r3, #16
	adds	r5, #28
	movs	r1, #204
	str	r3, [r5, #0]
	lsls	r1, r1, #17
	movs	r3, #232
	str	r2, [r5, #8]
	str	r1, [r5, #4]
	str	r2, [r5, #12]
	lsls	r3, r3, #16
	adds	r5, #28
	str	r3, [r5, #0]
	movs	r3, #144
	str	r2, [r5, #8]
	str	r1, [r5, #4]
	str	r0, [r5, #12]
	lsls	r3, r3, #15
	adds	r5, #28
	str	r3, [r5, #0]
	movs	r3, #188
	lsls	r3, r3, #17
	str	r3, [r5, #4]
	movs	r3, #4
	str	r2, [r5, #8]
	str	r3, [r5, #12]
.L_020015a4:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r3, #192
	movs	r0, #130
	lsls	r3, r3, #18
	lsls	r0, r0, #1
	ldr	r5, [r3, #108]
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_020015f8
	ldr	r2, [pc, #56]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	lsls	r3, r3, #16
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	asrs	r3, r3, #16
	cmp	r3, r2
	blt.n	.L_020015f8
	ldr	r0, [pc, #44]
	bl 0x0200ad6c
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_020015ee
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #68
	b.n	.L_020015f6
.L_020015ee:
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r5, r1
	movs	r3, #69
.L_020015f6:
	strh	r3, [r2, #0]
.L_020015f8:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200b782
	.4byte 0x0200b784
	.4byte 0x020095ad
	.2byte 0xb78c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #868]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r5, r0
	ldr	r0, [r5, #0]
	sub	sp, #20
	bl 0x0200ae54
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r7, [pc, #848]
	adds	r6, r0, #0
	movs	r1, #0
	ldr	r0, [r5, #0]
	mov	r9, r3
	bl 0x0200ae74
	ldr	r3, [r7, #4]
	movs	r1, #85
	asrs	r3, r3, #19
	adds	r1, r1, r6
	mov	r8, r3
	movs	r3, #0
	strb	r3, [r1, #0]
	mov	sl, r1
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200ae24
	movs	r0, #30
	bl 0x0200ae3c
	movs	r0, #139
	lsls	r0, r0, #2
	bl 0x0200aeec
	movs	r3, #66
	str	r3, [sp, #4]
	movs	r5, #7
	movs	r0, #22
	movs	r1, #66
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #1
	str	r3, [sp, #4]
	movs	r2, #3
	movs	r0, #22
	movs	r1, #1
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r2, #0
	str	r2, [sp, #16]
.L_02001696:
	ldr	r3, [pc, #752]
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	ble.n	.L_020016be
	movs	r5, #128
	lsls	r5, r5, #9
.L_020016a4:
	movs	r0, #1
	bl 0x0200ae3c
	ldr	r2, [pc, #732]
	adds	r3, r5, #0
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	movs	r0, #128
	lsls	r0, r0, #9
	asrs	r3, r3, #16
	adds	r5, r5, r0
	cmp	r3, r2
	blt.n	.L_020016a4
.L_020016be:
	ldr	r3, [pc, #716]
	ldr	r2, [r7, #4]
	ldr	r0, [r3, #0]
	movs	r3, #212
	adds	r2, r2, r0
	str	r2, [r7, #4]
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r2, [r3, #0]
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_020016da
	ldr	r4, [pc, #696]
	adds	r3, r3, r4
.L_020016da:
	asrs	r1, r3, #20
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bge.n	.L_020016e6
	ldr	r4, [pc, #684]
	adds	r3, r3, r4
.L_020016e6:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	cmp	r3, #232
	bne.n	.L_020016fe
	ldr	r3, [r6, #12]
	adds	r3, r3, r0
	str	r3, [r6, #12]
	str	r3, [r6, #20]
.L_020016fe:
	movs	r0, #1
	bl 0x0200ae3c
	mov	r0, r8
	cmp	r0, #2
	bne.n	.L_02001748
	ldr	r1, [sp, #16]
	cmp	r1, #0
	bne.n	.L_02001748
	movs	r3, #69
	movs	r5, #11
	str	r3, [sp, #4]
	movs	r0, #11
	movs	r1, #70
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #68
	str	r3, [sp, #4]
	movs	r0, #10
	movs	r1, #68
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ae04
	movs	r3, #5
	str	r3, [sp, #4]
	movs	r0, #10
	movs	r1, #5
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adfc
.L_02001748:
	mov	r2, r8
	cmp	r2, #1
	bne.n	.L_02001768
	ldr	r3, [sp, #16]
	cmp	r3, #15
	bne.n	.L_02001768
	movs	r3, #11
	movs	r2, #68
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #73
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ae04
.L_02001768:
	ldr	r3, [sp, #16]
	adds	r3, #1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #16]
	cmp	r3, #15
	ble.n	.L_02001696
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ae24
	movs	r0, #5
	bl 0x0200ae3c
	movs	r3, #3
	mov	r4, sl
	strb	r3, [r4, #0]
	ldr	r3, [r7, #4]
	movs	r0, #128
	lsls	r0, r0, #14
	cmp	r3, r0
	bge.n	.L_020017b0
	ldr	r3, [pc, #500]
	movs	r2, #0
	movs	r1, #144
	strh	r2, [r3, #0]
	ldr	r0, [pc, #496]
	lsls	r1, r1, #3
.L_020017aa:
	bl 0x0200ad64
	b.n	.L_020017d4
.L_020017b0:
	movs	r3, #68
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #69
	movs	r1, #6
	movs	r2, #1
	movs	r3, #2
	bl 0x0200ae04
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200adbc
	movs	r0, #126
	adds	r0, #255
	bl 0x0200adc4
.L_020017d4:
	movs	r3, #66
	str	r3, [sp, #4]
	movs	r5, #7
	movs	r0, #18
	movs	r1, #66
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #1
	str	r3, [sp, #4]
	movs	r2, #3
	movs	r3, #5
	movs	r0, #18
	movs	r1, #1
	str	r5, [sp, #0]
	bl 0x0200adf4
	ldr	r3, [r7, #4]
	ldr	r2, [pc, #412]
	asrs	r3, r3, #19
	str	r3, [sp, #16]
	ldrb	r3, [r2, #0]
	cmp	r3, #99
	beq.n	.L_020018b4
	movs	r1, #4
	mov	fp, r1
	movs	r7, #0
.L_0200180e:
	ldrb	r3, [r2, r7]
	ldr	r4, [sp, #16]
	adds	r1, r2, #0
	cmp	r4, r3
	blt.n	.L_020018a6
	adds	r3, r7, r2
	ldrb	r3, [r3, #2]
	movs	r0, #0
	mov	r8, r0
	cmp	r8, r3
	bge.n	.L_020018a6
.L_02001824:
	str	r7, [sp, #12]
	adds	r3, r7, r1
	ldrb	r3, [r3, #1]
	movs	r4, #0
	mov	r9, r1
	cmp	r4, r3
	bge.n	.L_02001890
	movs	r1, #128
	lsls	r1, r1, #9
	mov	sl, r1
.L_02001838:
	ldr	r2, [sp, #16]
	mov	r3, r9
	adds	r6, r7, r3
	mov	r0, r9
	mov	r1, fp
	movs	r5, #14
	ldrb	r3, [r6, #3]
	subs	r5, r5, r2
	ldrb	r2, [r0, r1]
	adds	r3, r3, r4
	add	r2, r8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #21
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	str	r4, [sp, #8]
	bl 0x0200adfc
	mov	r0, r9
	mov	r1, fp
	ldrb	r3, [r0, r1]
	ldrb	r2, [r6, #3]
	ldr	r4, [sp, #8]
	add	r3, r8
	adds	r2, r2, r4
	adds	r3, #64
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r3, #1
	movs	r0, #25
	adds	r1, r5, #0
	bl 0x0200adfc
	mov	r3, sl
	asrs	r4, r3, #16
	ldrb	r3, [r6, #1]
	movs	r2, #128
	lsls	r2, r2, #9
	add	sl, r2
	cmp	r4, r3
	blt.n	.L_02001838
.L_02001890:
	mov	r3, r8
	adds	r3, #1
	ldr	r1, [pc, #260]
	ldr	r4, [sp, #12]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r8, r3
	adds	r3, r4, r1
	ldrb	r3, [r3, #2]
	cmp	r8, r3
	blt.n	.L_02001824
.L_020018a6:
	ldr	r2, [pc, #244]
	adds	r7, #8
	ldrb	r3, [r2, r7]
	movs	r0, #8
.L_020018ae:
	add	fp, r0
	cmp	r3, #99
	bne.n	.L_0200180e
.L_020018b4:
	ldr	r1, [sp, #16]
	movs	r5, #14
	subs	r5, r5, r1
	movs	r3, #7
	str	r3, [sp, #4]
	movs	r2, #2
	mov	r8, r3
	movs	r0, #19
	adds	r1, r5, #0
	movs	r3, #1
	str	r2, [sp, #0]
	mov	fp, r2
	bl 0x0200adfc
	mov	r0, r8
	str	r0, [sp, #4]
	movs	r4, #13
	movs	r0, #22
	adds	r1, r5, #0
	movs	r2, #2
	movs	r3, #1
	str	r4, [sp, #0]
	mov	r9, r4
	bl 0x0200adfc
.L_020018e6:
	mov	r1, r8
	str	r1, [sp, #0]
	movs	r7, #5
	movs	r0, #18
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #4]
	bl 0x0200adfc
	movs	r2, #9
	str	r2, [sp, #0]
	mov	sl, r2
	movs	r0, #24
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #4]
	bl 0x0200adfc
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r6, #71
	movs	r0, #19
	adds	r1, r5, #0
	movs	r2, #2
	movs	r3, #1
	str	r6, [sp, #4]
.L_0200191e:
	bl 0x0200adfc
	mov	r4, r9
	movs	r0, #22
	adds	r1, r5, #0
	movs	r2, #2
	movs	r3, #1
	str	r6, [sp, #4]
	str	r4, [sp, #0]
	bl 0x0200adfc
	mov	r0, r8
	str	r0, [sp, #0]
	movs	r6, #69
	movs	r0, #26
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200adfc
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #1
	movs	r0, #27
	adds	r1, r5, #0
	movs	r3, #1
	str	r6, [sp, #4]
.L_02001956:
	bl 0x0200adfc
	ldr	r2, [sp, #16]
	cmp	r2, #2
	bne.n	.L_02001972
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r0, #18
	movs	r1, #8
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #4]
	bl 0x0200adfc
.L_02001972:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c030
	.4byte 0x0200b780
	.4byte 0x0200b788
	.4byte 0x000fffff
	.4byte 0x0200b782
	.4byte 0x020095ad
	.2byte 0xafa8
	.2byte 0x0200
.L_020019a0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #868]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r5, r0
	ldr	r0, [r5, #0]
	sub	sp, #20
	bl 0x0200ae54
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r7, [pc, #848]
	adds	r6, r0, #0
	movs	r1, #0
	ldr	r0, [r5, #0]
.L_020019cc:
	mov	r9, r3
	bl 0x0200ae74
	ldr	r3, [r7, #4]
	movs	r1, #85
	asrs	r3, r3, #19
	adds	r1, r1, r6
	mov	r8, r3
	movs	r3, #0
	strb	r3, [r1, #0]
	mov	sl, r1
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200ae24
	movs	r0, #30
	bl 0x0200ae3c
	movs	r0, #139
	lsls	r0, r0, #2
	bl 0x0200aeec
	movs	r3, #84
	str	r3, [sp, #4]
	movs	r5, #7
	movs	r0, #22
	movs	r1, #66
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #19
	str	r3, [sp, #4]
	movs	r2, #3
	movs	r0, #22
	movs	r1, #1
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r2, #0
	str	r2, [sp, #16]
.L_02001a2a:
	ldr	r3, [pc, #752]
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	ble.n	.L_02001a52
	movs	r5, #128
	lsls	r5, r5, #9
.L_02001a38:
	movs	r0, #1
	bl 0x0200ae3c
	ldr	r2, [pc, #732]
	adds	r3, r5, #0
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	movs	r0, #128
	lsls	r0, r0, #9
	asrs	r3, r3, #16
	adds	r5, r5, r0
	cmp	r3, r2
	blt.n	.L_02001a38
.L_02001a52:
	ldr	r3, [pc, #716]
	ldr	r2, [r7, #4]
	ldr	r0, [r3, #0]
	movs	r3, #212
	adds	r2, r2, r0
	str	r2, [r7, #4]
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r2, [r3, #0]
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_02001a6e
	ldr	r4, [pc, #696]
	adds	r3, r3, r4
.L_02001a6e:
	asrs	r1, r3, #20
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bge.n	.L_02001a7a
	ldr	r4, [pc, #684]
	adds	r3, r3, r4
.L_02001a7a:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	cmp	r3, #232
	bne.n	.L_02001a92
	ldr	r3, [r6, #12]
	adds	r3, r3, r0
	str	r3, [r6, #12]
	str	r3, [r6, #20]
.L_02001a92:
	movs	r0, #1
	bl 0x0200ae3c
	mov	r0, r8
	cmp	r0, #2
	bne.n	.L_02001ade
	ldr	r1, [sp, #16]
	cmp	r1, #0
	bne.n	.L_02001ade
	movs	r3, #11
	movs	r2, #69
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #11
.L_02001aae:
	movs	r1, #70
.L_02001ab0:
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adf4
	movs	r3, #86
	movs	r5, #4
	str	r3, [sp, #4]
	movs	r0, #10
	movs	r1, #68
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200ae04
	movs	r3, #23
	str	r3, [sp, #4]
	movs	r0, #4
	movs	r1, #24
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adfc
.L_02001ade:
	mov	r2, r8
	cmp	r2, #1
	bne.n	.L_02001afe
	ldr	r3, [sp, #16]
.L_02001ae6:
	cmp	r3, #15
	bne.n	.L_02001afe
	movs	r3, #4
	movs	r2, #86
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #73
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ae04
.L_02001afe:
	ldr	r3, [sp, #16]
	adds	r3, #1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #16]
	cmp	r3, #15
	ble.n	.L_02001a2a
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ae24
	movs	r0, #5
	bl 0x0200ae3c
	movs	r3, #3
	mov	r4, sl
	strb	r3, [r4, #0]
	ldr	r3, [r7, #4]
	movs	r0, #128
	lsls	r0, r0, #14
	cmp	r3, r0
	bge.n	.L_02001b46
	ldr	r3, [pc, #496]
	movs	r2, #0
	movs	r1, #144
	strh	r2, [r3, #0]
	ldr	r0, [pc, #492]
	lsls	r1, r1, #3
	bl 0x0200ad64
	b.n	.L_02001b6a
.L_02001b46:
	movs	r3, #68
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #69
	movs	r1, #24
.L_02001b52:
	movs	r2, #1
	movs	r3, #4
	bl 0x0200ae04
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200adbc
	movs	r0, #126
	adds	r0, #255
	bl 0x0200adc4
.L_02001b6a:
	movs	r3, #84
	str	r3, [sp, #4]
	movs	r5, #7
	movs	r0, #18
	movs	r1, #66
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #19
	str	r3, [sp, #4]
	movs	r2, #3
	movs	r3, #5
	movs	r0, #18
	movs	r1, #1
	str	r5, [sp, #0]
	bl 0x0200adf4
	ldr	r3, [r7, #4]
	ldr	r2, [pc, #412]
	asrs	r3, r3, #19
	str	r3, [sp, #16]
	ldrb	r3, [r2, #0]
	cmp	r3, #99
	beq.n	.L_02001c4a
	movs	r1, #4
	mov	fp, r1
	movs	r7, #0
.L_02001ba4:
	ldrb	r3, [r2, r7]
	ldr	r4, [sp, #16]
	adds	r1, r2, #0
	cmp	r4, r3
	blt.n	.L_02001c3c
	adds	r3, r7, r2
	ldrb	r3, [r3, #2]
	movs	r0, #0
	mov	r8, r0
	cmp	r8, r3
	bge.n	.L_02001c3c
.L_02001bba:
	str	r7, [sp, #12]
	adds	r3, r7, r1
	ldrb	r3, [r3, #1]
	movs	r4, #0
	mov	r9, r1
	cmp	r4, r3
	bge.n	.L_02001c26
	movs	r1, #128
	lsls	r1, r1, #9
	mov	sl, r1
.L_02001bce:
	ldr	r2, [sp, #16]
	mov	r3, r9
	adds	r6, r7, r3
	mov	r0, r9
	mov	r1, fp
	movs	r5, #14
	ldrb	r3, [r6, #3]
	subs	r5, r5, r2
	ldrb	r2, [r0, r1]
	adds	r3, r3, r4
	add	r2, r8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #21
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	str	r4, [sp, #8]
	bl 0x0200adfc
	mov	r0, r9
	mov	r1, fp
	ldrb	r3, [r0, r1]
	ldrb	r2, [r6, #3]
	ldr	r4, [sp, #8]
	add	r3, r8
	adds	r2, r2, r4
	adds	r3, #64
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r3, #1
	movs	r0, #25
	adds	r1, r5, #0
	bl 0x0200adfc
	mov	r3, sl
	asrs	r4, r3, #16
	ldrb	r3, [r6, #1]
	movs	r2, #128
	lsls	r2, r2, #9
	add	sl, r2
	cmp	r4, r3
	blt.n	.L_02001bce
.L_02001c26:
	mov	r3, r8
	adds	r3, #1
	ldr	r1, [pc, #260]
	ldr	r4, [sp, #12]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r8, r3
	adds	r3, r4, r1
	ldrb	r3, [r3, #2]
	cmp	r8, r3
	blt.n	.L_02001bba
.L_02001c3c:
	ldr	r2, [pc, #240]
	adds	r7, #8
	ldrb	r3, [r2, r7]
	movs	r0, #8
	add	fp, r0
	cmp	r3, #99
	bne.n	.L_02001ba4
.L_02001c4a:
	ldr	r1, [sp, #16]
	movs	r5, #14
	subs	r5, r5, r1
	movs	r3, #27
	str	r3, [sp, #4]
	movs	r2, #2
	movs	r0, #19
	adds	r1, r5, #0
	movs	r3, #1
	str	r2, [sp, #0]
	mov	r9, r2
	bl 0x0200adfc
	movs	r3, #25
	str	r3, [sp, #4]
	movs	r6, #13
	movs	r0, #19
	adds	r1, r5, #0
	movs	r2, #2
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200adfc
	movs	r3, #7
	str	r3, [sp, #0]
	mov	sl, r3
	movs	r7, #23
	movs	r0, #18
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #4]
	bl 0x0200adfc
	movs	r4, #9
	movs	r0, #24
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	mov	r8, r4
	str	r4, [sp, #0]
	str	r7, [sp, #4]
	bl 0x0200adfc
	movs	r3, #91
	mov	r0, r9
	str	r0, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #19
	adds	r1, r5, #0
	movs	r2, #2
	movs	r3, #1
	bl 0x0200adfc
	movs	r3, #89
	str	r3, [sp, #4]
	movs	r0, #19
	adds	r1, r5, #0
	movs	r2, #2
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200adfc
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r6, #87
	movs	r0, #26
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200adfc
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r3, #1
	movs	r0, #27
	adds	r1, r5, #0
	movs	r2, #1
	str	r6, [sp, #4]
	bl 0x0200adfc
	ldr	r3, [sp, #16]
	cmp	r3, #2
	bne.n	.L_02001d06
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r0, #18
	movs	r1, #7
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #4]
	bl 0x0200adfc
.L_02001d06:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c030
	.4byte 0x0200b780
	.4byte 0x0200b788
	.4byte 0x000fffff
	.4byte 0x0200b782
	.4byte 0x020095ad
	.2byte 0xaff0
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	sub	sp, #8
	bl 0x0200adb4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02001dd8
	movs	r0, #0
	bl 0x0200aedc
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200aeec
	movs	r0, #15
	bl 0x0200ae3c
	movs	r0, #202
	bl 0x0200aeec
	movs	r3, #66
	str	r3, [sp, #4]
	movs	r5, #7
	movs	r3, #1
.L_02001d6a:
	movs	r0, #18
	movs	r1, #66
	movs	r2, #3
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200ae24
	movs	r0, #15
	bl 0x0200ae3c
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #56]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #56]
	movs	r0, #126
	strh	r6, [r3, #0]
	ldr	r3, [pc, #36]
	adds	r0, #255
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r2, [pc, #44]
	movs	r3, #1
	str	r3, [r2, #0]
	bl 0x0200adbc
	bl 0x0200960c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200adbc
	b.n	.L_02001dd8
	.4byte 0x000000c8
	.4byte 0x0200b780
	.4byte 0x0200b784
	.4byte 0x0200b782
	.4byte 0x0200b788
	.2byte 0xb78c
	.2byte 0x0200
.L_02001dd8:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #226
	sub	sp, #8
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02001e50
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #226
	bl 0x0200adbc
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200aeec
	bl 0x0200ae44
	movs	r0, #0
	bl 0x0200aedc
	movs	r5, #14
	movs	r1, #70
	movs	r2, #1
	movs	r3, #3
	movs	r6, #65
	movs	r0, #26
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r0, #20
	bl 0x0200ae3c
	movs	r0, #181
	bl 0x0200aeec
	movs	r0, #26
	movs	r1, #67
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r3, #5
	str	r3, [sp, #4]
	movs	r0, #15
	movs	r1, #5
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adfc
	bl 0x0200ae4c
.L_02001e50:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, lr}
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x0200adb4
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_02001f00
	movs	r0, #0
	bl 0x0200aedc
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200aeec
	movs	r0, #15
	bl 0x0200ae3c
.L_02001e7c:
	movs	r0, #202
	bl 0x0200aeec
	movs	r3, #7
	movs	r2, #84
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #18
	movs	r1, #66
	movs	r2, #3
	bl 0x0200adf4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200ae24
	movs	r0, #15
	bl 0x0200ae3c
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #52]
	movs	r0, #126
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #60]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #44]
	adds	r0, #255
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r2, [pc, #48]
	movs	r3, #2
	str	r3, [r2, #0]
	bl 0x0200adbc
	bl 0x020099a0
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200adbc
	b.n	.L_02001f00
	.2byte 0x0000
	.4byte 0x00000003
	.4byte 0x00000064
	.4byte 0x0200b780
	.4byte 0x0200b782
	.4byte 0x0200b784
	.4byte 0x0200b788
	.2byte 0xb78c
	.2byte 0x0200
.L_02001f00:
	add	sp, #8
	pop	{r5, pc}
	push	{r5, r6, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #227
	sub	sp, #8
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02001f78
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #227
	bl 0x0200adbc
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200aeec
	bl 0x0200ae44
	movs	r0, #0
	bl 0x0200aedc
	movs	r5, #2
	movs	r1, #70
	movs	r2, #1
	movs	r3, #3
	movs	r6, #83
	movs	r0, #26
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r0, #20
	bl 0x0200ae3c
	movs	r0, #181
	bl 0x0200aeec
	movs	r0, #26
	movs	r1, #67
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adf4
	movs	r3, #23
	str	r3, [sp, #4]
	movs	r0, #1
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adfc
	bl 0x0200ae4c
.L_02001f78:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #108]
	movs	r1, #230
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r3, [r3, #0]
	movs	r0, #9
	sub	sp, #8
	mov	sl, r3
	bl 0x0200ae54
	ldr	r2, [pc, #332]
	movs	r3, #133
	mov	r8, r2
	lsls	r3, r3, #2
	add	r3, r8
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200ae54
	adds	r6, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	ldr	r5, [r5, #108]
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02001fce
	movs	r1, #170
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #40
	beq.n	.L_02001fce
	b.n	.L_020020e0
.L_02001fce:
	ldr	r1, [r7, #16]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	adds	r0, r1, r3
	str	r0, [r7, #16]
	ldr	r3, [r6, #8]
	asrs	r2, r3, #20
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #5
	beq.n	.L_0200202a
	cmp	r2, #12
	ble.n	.L_0200202a
	cmp	r2, #15
	bgt.n	.L_0200202a
	ldr	r3, [pc, #248]
	adds	r2, r1, r3
	ldr	r3, [r6, #16]
	cmp	r2, r3
	blt.n	.L_02002010
	cmp	r3, r0
	blt.n	.L_02002010
	str	r2, [r6, #16]
	ldr	r3, [r7, #16]
	movs	r1, #192
	lsls	r1, r1, #13
	adds	r3, r3, r1
	mov	r2, sl
	str	r3, [r2, #16]
.L_02002010:
	ldr	r3, [r7, #16]
	ldr	r2, [r6, #16]
	cmp	r3, r2
	blt.n	.L_0200202a
	ldr	r1, [pc, #216]
	adds	r3, r3, r1
	cmp	r2, r3
	blt.n	.L_0200202a
	str	r3, [r6, #16]
	ldr	r3, [r7, #16]
	mov	r2, sl
	adds	r3, r3, r1
	str	r3, [r2, #16]
.L_0200202a:
	ldr	r3, [r7, #16]
	movs	r1, #128
	asrs	r5, r3, #20
	adds	r3, r5, #0
	subs	r3, #31
	lsls	r3, r3, #16
	lsls	r1, r1, #11
	cmp	r3, r1
	bhi.n	.L_02002064
	movs	r0, #163
	lsls	r0, r0, #1
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02002064
	movs	r3, #12
	movs	r2, #96
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #83
	movs	r2, #5
	movs	r3, #4
	bl 0x0200adfc
	movs	r0, #163
	lsls	r0, r0, #1
	bl 0x0200adbc
.L_02002064:
	cmp	r5, #37
	bne.n	.L_02002090
	movs	r0, #163
	lsls	r0, r0, #1
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002090
	movs	r3, #12
	movs	r2, #96
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #40
	movs	r1, #83
	movs	r2, #5
	movs	r3, #4
	bl 0x0200adfc
	movs	r0, #163
	lsls	r0, r0, #1
	bl 0x0200adc4
.L_02002090:
	cmp	r5, #41
	ble.n	.L_020020e0
	ldr	r0, [pc, #96]
	bl 0x0200ad6c
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200ae24
	movs	r3, #13
	movs	r2, #41
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #48
	movs	r2, #3
	movs	r3, #2
	movs	r0, #41
	bl 0x0200adfc
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200adbc
	movs	r0, #126
	adds	r0, #255
	bl 0x0200adc4
	movs	r0, #98
	adds	r0, #255
	bl 0x0200adc4
	movs	r0, #208
	bl 0x0200aeec
.L_020020e0:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0018cccc
	.4byte 0xfff00000
	.2byte 0x9f7d
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x0200ae54
	ldr	r3, [r0, #16]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #221
	asrs	r6, r3, #20
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020021c6
	cmp	r6, #24
	bne.n	.L_020021c6
	ldr	r7, [pc, #172]
	adds	r0, r7, #0
	bl 0x0200adcc
	movs	r2, #1
	adds	r5, r0, #0
	negs	r2, r2
	cmp	r5, r2
	bne.n	.L_020021c6
	bl 0x0200ae44
	movs	r0, #0
	bl 0x0200aedc
	movs	r0, #207
	bl 0x0200aeec
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200ae24
	movs	r0, #10
	bl 0x0200ae3c
	movs	r0, #232
	movs	r2, #196
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #16
	adds	r1, r5, #0
	bl 0x0200aeac
	bl 0x0200aeb4
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r7, #0
	bl 0x0200ad64
	movs	r0, #40
	bl 0x0200ae3c
	ldr	r3, [pc, #84]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200aea4
	bl 0x0200aeb4
	movs	r3, #13
	str	r3, [sp, #0]
	movs	r0, #13
	movs	r1, #30
	movs	r2, #3
	movs	r3, #2
	str	r6, [sp, #4]
	bl 0x0200adfc
	movs	r3, #14
	movs	r2, #88
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #87
	movs	r2, #1
	movs	r3, #1
	movs	r0, #14
	bl 0x0200adfc
	movs	r0, #126
	adds	r0, #255
	bl 0x0200adbc
	movs	r0, #98
	adds	r0, #255
	bl 0x0200adbc
	bl 0x0200ae4c
.L_020021c6:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02009f7d
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x0200ae54
	ldr	r3, [r0, #16]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #221
	asrs	r6, r3, #20
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002258
	cmp	r6, #24
	bne.n	.L_02002258
	ldr	r5, [pc, #100]
	adds	r0, r5, #0
	bl 0x0200adcc
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_02002258
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200ae24
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200ad64
	movs	r3, #13
	str	r3, [sp, #0]
	movs	r0, #13
	movs	r1, #30
	movs	r2, #3
	movs	r3, #2
	str	r6, [sp, #4]
	bl 0x0200adfc
	movs	r3, #14
	movs	r2, #88
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #87
	movs	r2, #1
	movs	r3, #1
	movs	r0, #14
	bl 0x0200adfc
	movs	r0, #126
	adds	r0, #255
	bl 0x0200adbc
	movs	r0, #98
	adds	r0, #255
	bl 0x0200adbc
	bl 0x0200ae4c
.L_02002258:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x9f7d
	.2byte 0x0200
	push	{lr}
	movs	r0, #9
	bl 0x0200ae54
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #26
	bne.n	.L_020022a4
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #35
	bne.n	.L_020022a4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #219
	bl 0x0200adbc
	movs	r0, #204
	bl 0x0200aeec
	movs	r0, #9
	movs	r1, #0
	bl 0x02008b30
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
	bl 0x0200aee4
	movs	r0, #20
	bl 0x0200aebc
.L_020022a4:
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x0200adbc
	movs	r3, #34
	str	r3, [sp, #4]
	movs	r5, #25
	movs	r0, #35
	movs	r1, #34
	movs	r2, #3
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #98
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #98
	movs	r2, #3
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200adf4
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x0200adc4
	movs	r3, #34
	str	r3, [sp, #4]
	movs	r5, #25
	movs	r0, #40
	movs	r1, #34
	movs	r2, #3
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #98
	str	r3, [sp, #4]
	movs	r0, #40
	movs	r1, #98
	movs	r2, #3
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200adf4
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200adbc
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200adc4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02002350
	ldr	r0, [pc, #72]
	b.n	.L_0200238e
.L_02002350:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_0200235a
	ldr	r0, [pc, #72]
	b.n	.L_0200238e
.L_0200235a:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02002364
	ldr	r0, [pc, #68]
	b.n	.L_0200238e
.L_02002364:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_0200236e
	ldr	r0, [pc, #68]
	b.n	.L_0200238e
.L_0200236e:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02002378
	ldr	r0, [pc, #64]
	b.n	.L_0200238e
.L_02002378:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02002382
	ldr	r0, [pc, #64]
	b.n	.L_0200238e
.L_02002382:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_0200238c
	ldr	r0, [pc, #60]
	b.n	.L_0200238e
.L_0200238c:
	ldr	r0, [pc, #60]
.L_0200238e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000bd
	.4byte 0x0200b7b4
	.4byte 0x000000be
	.4byte 0x0200ba78
	.4byte 0x000000bf
	.4byte 0x0200bb98
	.4byte 0x000000c0
	.4byte 0x0200bc94
	.4byte 0x000000c1
	.4byte 0x0200be38
	.4byte 0x000000c2
	.4byte 0x0200bf4c
	.4byte 0x000000c4
	.4byte 0x0200c018
	.2byte 0xb790
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r6, #7
	ldr	r7, [r3, #0]
	sub	sp, #56
	ands	r7, r6
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_0200242a
	bl 0x0200ad74
	movs	r5, #15
	ands	r5, r0
	bl 0x0200ad74
	movs	r3, #209
	lsls	r3, r3, #1
	ands	r0, r6
	adds	r3, #255
	add	r6, sp, #16
	strh	r3, [r6, #24]
	mov	r3, r8
	ldr	r4, [r3, #8]
	ldr	r1, [r3, #12]
	ldr	r2, [r3, #16]
	movs	r3, #128
	lsls	r3, r3, #8
	subs	r5, #8
	lsls	r5, r5, #16
	subs	r0, #8
	str	r3, [sp, #0]
	movs	r3, #128
	lsls	r0, r0, #16
	lsls	r3, r3, #13
	adds	r4, r4, r5
	adds	r1, r1, r0
	str	r3, [sp, #8]
	adds	r0, r4, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl 0x0200815c
.L_0200242a:
	add	sp, #56
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r7, [pc, #776]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #768]
	sub	sp, #8
	cmp	r2, r3
	beq.n	.L_02002456
	b.n	.L_02002686
.L_02002456:
	movs	r0, #0
	bl 0x0200aed4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #210
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002474
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002474:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #211
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200248c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200248c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #212
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020024a4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020024a4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #213
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020024bc
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020024bc:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #214
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020024d4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020024d4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #215
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020024ec
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020024ec:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #216
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002504
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002504:
	movs	r0, #153
	lsls	r0, r0, #4
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200251a
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200251a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #145
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002532
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002532:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #146
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200254a
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200254a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #147
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002562
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002562:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #148
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200257a
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200257a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #149
	bl 0x0200adb4
.L_02002584:
	cmp	r0, #0
	beq.n	.L_02002592
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002592:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #150
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020025aa
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020025aa:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #151
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020025c2
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020025c2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #152
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020025da
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020025da:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #153
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020025f2
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020025f2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #154
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200260a
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200260a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #155
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002622
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002622:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #156
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200263a
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200263a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #157
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002652
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002652:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #198
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02002662
	b.n	.L_02002d2a
.L_02002662:
	movs	r1, #144
	movs	r2, #176
	movs	r0, #8
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	bl 0x0200ae6c
	movs	r3, #2
	movs	r2, #69
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #4
	movs	r1, #68
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	b.n	.L_02002d2a
.L_02002686:
	ldr	r3, [pc, #204]
	cmp	r2, r3
	bne.n	.L_02002758
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #158
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020026a4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020026a4:
	movs	r0, #138
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020026bc
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020026bc:
	movs	r0, #154
	lsls	r0, r0, #4
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020026d2
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020026d2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #220
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_020026e2
	b.n	.L_02002d2a
.L_020026e2:
	movs	r0, #8
	bl 0x0200ae54
	adds	r5, r0, #0
	adds	r2, r5, #0
	movs	r3, #3
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r1, #200
	movs	r2, #176
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #15
	bl 0x0200ae6c
	movs	r3, #7
	str	r3, [sp, #4]
	movs	r5, #10
	movs	r0, #35
	movs	r1, #6
	movs	r2, #7
	movs	r3, #16
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r1, #70
	movs	r2, #7
	movs	r3, #16
	movs	r0, #35
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #6
	beq.n	.L_0200273e
	cmp	r3, #17
	beq.n	.L_0200273e
	cmp	r3, #15
	beq.n	.L_0200273e
	b.n	.L_02002d2a
.L_0200273e:
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aeec
	b.n	.L_02002d2a
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000bd
	.2byte 0x00be
	.2byte 0x0000
.L_02002758:
	ldr	r3, [pc, #896]
	cmp	r2, r3
	bne.n	.L_020027fa
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #199
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200278e
	movs	r1, #130
	movs	r2, #208
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	bl 0x0200ae6c
	movs	r3, #33
	movs	r2, #69
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r1, #69
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
.L_0200278e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #221
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020027ae
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200ae74
.L_020027ae:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_020027be
	b.n	.L_02002d2a
.L_020027be:
	movs	r3, #24
	str	r3, [sp, #4]
	movs	r5, #13
	movs	r0, #13
	movs	r1, #30
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200adfc
	movs	r3, #14
	movs	r2, #88
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #14
	movs	r1, #87
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
	movs	r3, #41
	str	r3, [sp, #4]
	movs	r0, #41
	movs	r1, #48
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200adfc
	b.n	.L_02002d2a
.L_020027fa:
	ldr	r3, [pc, #740]
	cmp	r2, r3
	beq.n	.L_02002802
	b.n	.L_02002ae8
.L_02002802:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #161
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200281a
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200281a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #162
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002832
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002832:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #163
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200284a
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200284a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002862
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002862:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #165
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200287a
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_0200287a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #166
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002892
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002892:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #167
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020028aa
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_020028aa:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #200
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020028c6
	movs	r1, #160
	movs	r2, #194
	movs	r0, #8
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	bl 0x0200ae6c
.L_020028c6:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #201
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020028e2
	movs	r1, #158
	movs	r2, #228
	movs	r0, #18
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200ae6c
.L_020028e2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #202
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002912
	movs	r1, #154
	movs	r2, #136
	movs	r0, #19
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200ae6c
	movs	r3, #37
	movs	r2, #71
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #37
	movs	r1, #70
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adf4
.L_02002912:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #217
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200293e
	movs	r0, #9
	bl 0x0200ae54
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r1, #200
	movs	r2, #182
	movs	r0, #9
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200ae6c
.L_0200293e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #218
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_0200296c
	movs	r0, #10
	bl 0x0200ae54
	movs	r1, #232
	movs	r2, #182
	adds	r5, r0, #0
	lsls	r2, r2, #18
	movs	r0, #10
	lsls	r1, r1, #16
	bl 0x0200ae6c
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	b.n	.L_02002994
.L_0200296c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #219
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002994
	movs	r0, #10
	adds	r0, #255
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02002994
	movs	r1, #164
	movs	r2, #190
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200ae6c
.L_02002994:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #218
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020029f6
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #217
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_020029f6
	ldr	r3, [pc, #304]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r1, #128
	subs	r3, #13
	lsls	r3, r3, #16
	lsls	r1, r1, #11
	cmp	r3, r1
	bhi.n	.L_020029d0
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aeec
.L_020029d0:
	movs	r3, #47
	movs	r5, #12
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r1, #27
	movs	r2, #3
	movs	r3, #13
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #111
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r1, #91
	movs	r2, #3
	movs	r3, #13
	str	r5, [sp, #0]
	bl 0x0200adf4
.L_020029f6:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02002a16
	ldr	r3, [pc, #224]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #17
	bne.n	.L_02002a16
	bl 0x02008418
.L_02002a16:
	ldr	r5, [pc, #204]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #20
	bne.n	.L_02002ac2
	bl 0x0200ae44
	movs	r0, #0
	bl 0x0200aedc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200aeac
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200ae6c
	bl 0x0200aec4
	movs	r0, #15
	bl 0x0200ae3c
	movs	r0, #10
	bl 0x0200ae54
	movs	r1, #164
	movs	r2, #190
	adds	r5, r0, #0
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	movs	r0, #10
	bl 0x0200ae6c
	ldr	r3, [r5, #12]
	movs	r1, #128
	lsls	r1, r1, #16
	adds	r3, r3, r1
	adds	r2, r5, #0
	str	r3, [r5, #12]
	adds	r2, #85
	movs	r3, #3
	movs	r6, #60
	strb	r3, [r2, #0]
	b.n	.L_02002a94
.L_02002a92:
	subs	r6, #1
.L_02002a94:
	cmp	r6, #0
	beq.n	.L_02002aa8
	movs	r0, #1
	bl 0x0200ad5c
	ldr	r3, [r5, #40]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	ble.n	.L_02002a92
.L_02002aa8:
	movs	r0, #188
	bl 0x0200aeec
	movs	r0, #60
	bl 0x0200ae3c
	movs	r0, #10
	adds	r0, #255
	bl 0x0200adbc
	movs	r0, #20
	bl 0x0200aebc
.L_02002ac2:
	ldr	r3, [pc, #32]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r3, [r3, #0]
	movs	r2, #192
	subs	r3, #13
	lsls	r3, r3, #16
	lsls	r2, r2, #10
	cmp	r3, r2
	bls.n	.L_02002ada
	b.n	.L_02002d2a
.L_02002ada:
	b.n	.L_02002b54
	.4byte 0x000000bf
	.4byte 0x000000c0
	.2byte 0x0240
	.2byte 0x0200
.L_02002ae8:
	ldr	r3, [pc, #588]
	cmp	r2, r3
	bne.n	.L_02002b60
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #219
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002b06
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ae6c
.L_02002b06:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #203
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002b36
	movs	r1, #140
	movs	r2, #156
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ae6c
	movs	r3, #18
	movs	r2, #84
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #16
	movs	r1, #84
	movs	r2, #1
	movs	r3, #1
	bl 0x0200adfc
.L_02002b36:
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	beq.n	.L_02002b4a
	cmp	r3, #12
	beq.n	.L_02002b4a
	b.n	.L_02002d2a
.L_02002b4a:
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200aeec
.L_02002b54:
	movs	r1, #144
	ldr	r0, [pc, #484]
	lsls	r1, r1, #3
	bl 0x0200ad64
	b.n	.L_02002d2a
.L_02002b60:
	ldr	r3, [pc, #476]
	cmp	r2, r3
	beq.n	.L_02002b68
	b.n	.L_02002d04
.L_02002b68:
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r3, r3, r7
	mov	sl, r3
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #4
	bgt.n	.L_02002b7c
	bl 0x02009420
.L_02002b7c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #226
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002bb0
	movs	r3, #65
	movs	r5, #14
	str	r3, [sp, #4]
	movs	r0, #26
	movs	r1, #67
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #5
	str	r3, [sp, #4]
	movs	r0, #15
	movs	r1, #5
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adfc
.L_02002bb0:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #227
	bl 0x0200adb4
	cmp	r0, #0
	beq.n	.L_02002be4
	movs	r3, #83
	movs	r5, #2
	str	r3, [sp, #4]
	movs	r0, #26
	movs	r1, #67
	movs	r2, #1
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200adf4
	movs	r3, #23
	str	r3, [sp, #4]
	movs	r0, #1
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200adfc
.L_02002be4:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02002bf2
	b.n	.L_02002d2a
.L_02002bf2:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200adb4
	cmp	r0, #0
	bne.n	.L_02002c00
	b.n	.L_02002d2a
.L_02002c00:
	ldr	r3, [pc, #320]
	movs	r2, #128
	lsls	r2, r2, #14
	str	r2, [r3, #4]
	mov	r1, sl
	ldrh	r3, [r1, #0]
	mov	r8, r2
	subs	r3, #1
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02002c76
	movs	r5, #4
	movs	r6, #5
	movs	r0, #25
	movs	r1, #22
	movs	r2, #9
	movs	r3, #2
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adfc
	movs	r3, #69
	str	r3, [sp, #4]
	movs	r0, #25
	movs	r1, #86
	movs	r2, #9
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200adfc
	movs	r5, #1
	movs	r3, #7
	movs	r0, #22
	movs	r1, #24
	movs	r2, #15
	str	r3, [sp, #4]
	str	r5, [sp, #0]
	bl 0x0200adfc
	movs	r3, #71
	str	r3, [sp, #4]
	movs	r0, #22
	movs	r1, #88
	movs	r2, #15
	movs	r3, #7
	str	r5, [sp, #0]
	bl 0x0200adfc
	movs	r3, #68
	str	r3, [sp, #0]
	movs	r0, #69
	movs	r1, #6
	movs	r2, #1
	movs	r3, #2
	str	r6, [sp, #4]
	bl 0x0200ae04
.L_02002c76:
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #128
	subs	r3, #3
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02002ce4
	movs	r5, #4
	movs	r6, #23
	movs	r0, #25
	movs	r1, #22
	movs	r2, #9
	movs	r3, #2
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200adfc
	movs	r3, #87
	str	r3, [sp, #4]
	movs	r0, #25
	movs	r1, #86
	movs	r2, #9
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200adfc
	movs	r3, #25
	str	r3, [sp, #4]
	movs	r5, #1
	movs	r0, #22
	movs	r1, #24
	movs	r2, #15
	movs	r3, #7
	str	r5, [sp, #0]
	bl 0x0200adfc
	movs	r3, #89
	str	r3, [sp, #4]
	movs	r0, #22
	movs	r1, #88
	movs	r2, #15
	movs	r3, #7
	str	r5, [sp, #0]
	bl 0x0200adfc
	movs	r3, #68
	str	r3, [sp, #0]
	movs	r0, #69
	movs	r1, #24
	movs	r2, #1
	movs	r3, #4
	str	r6, [sp, #4]
	bl 0x0200ae04
.L_02002ce4:
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl 0x0200ae54
	mov	r2, r8
	adds	r5, r0, #0
	str	r2, [r5, #20]
	str	r2, [r5, #12]
	movs	r0, #1
	bl 0x0200ae3c
	bl 0x0200adec
	b.n	.L_02002d2a
.L_02002d04:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02002d2a
	movs	r0, #64
	bl 0x0200ae54
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02002d2a
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	ldr	r3, [pc, #36]
	str	r3, [r5, #108]
.L_02002d2a:
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000000c1
	.4byte 0x020090f5
	.4byte 0x000000c2
	.4byte 0x0200c030
	.4byte 0x000000c4
	.2byte 0xa3d1
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000169, 0x08000179, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c9, 0x080003d1, 0x080003d9, 0x08000419, 0x08020091, 0x080200a9, 0x080200c1, 0x08020121, 0x080201e1, 0x080201e9, 0x080201f1, 0x08020211, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x08020291, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8151, 0x080c8171, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c83a9, 0x080c83b9, 0x080c8481, 0x080c84e1, 0x080c8629, 0x081c0011
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
	.4byte 0x01040f00
	.4byte 0x00000008
	.4byte 0x04030300
	.4byte 0x00000005
	.4byte 0x07020600
	.4byte 0x00000006
	.4byte 0x0a010100
	.4byte 0x00000005
	.4byte 0x0c010100
	.4byte 0x00000005
	.4byte 0x0b010102
	.4byte 0x00000005
	.4byte 0x01010f02
	.4byte 0x0000000c
	.4byte 0x01010f04
	.4byte 0x0000000d
	.4byte 0x00000063
	.4byte 0x00000000
	.4byte 0x01020f00
	.4byte 0x0000001c
	.4byte 0x04020c00
	.4byte 0x0000001a
	.4byte 0x04020800
	.4byte 0x00000018
	.4byte 0x05010200
	.4byte 0x00000017
	.4byte 0x0a010200
	.4byte 0x00000017
	.4byte 0x0f010100
	.4byte 0x00000019
	.4byte 0x04010102
	.4byte 0x00000017
	.4byte 0x01010f02
	.4byte 0x0000001e
	.4byte 0x01010f04
	.4byte 0x0000001f
	.4byte 0x00000063
	.4byte 0x00000000
	.4byte 0x0200aef4
	.4byte 0x0200af30
	.4byte 0x0200af6c
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x80000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00ee0333
	.4byte 0x033b0354
	.4byte 0x035c00f6
	.4byte 0x000cffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe600b4
	.4byte 0x00bc00a4
	.4byte 0x00acffee
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000c3
	.4byte 0x00129002
	.4byte 0x002010bd
	.4byte 0x000000bd
	.4byte 0x001020c3
	.4byte 0x0020b0bd
	.4byte 0x003050bd
	.4byte 0x004060bd
	.4byte 0x005030bd
	.4byte 0x006040bd
	.4byte 0x007010be
	.4byte 0x0080c0bd
	.4byte 0x0090a0bd
	.4byte 0x00a090bd
	.4byte 0x00b020bd
	.4byte 0x00c080bd
	.4byte 0x00d140bd
	.4byte 0x00e150bd
	.4byte 0x00f160bd
	.4byte 0x010170c2
	.4byte 0x011180bd
	.4byte 0x012190bd
	.4byte 0x0131a0c1
	.4byte 0x0140d0bd
	.4byte 0x0150e0bd
	.4byte 0x0160f0bd
	.4byte 0x018110bd
	.4byte 0x019120bd
	.4byte 0x01b1e0bd
	.4byte 0x01d1f0bd
	.4byte 0x01e1b0bd
	.4byte 0x01f1d0bd
	.4byte 0x020030be
	.4byte 0x021020be
	.4byte 0x000000be
	.4byte 0x001070bd
	.4byte 0x002210bd
	.4byte 0x003200bd
	.4byte 0x0041c0c2
	.4byte 0x006090bf
	.4byte 0x007040bf
	.4byte 0x00a030bf
	.4byte 0x00b0c0be
	.4byte 0x00c0b0be
	.4byte 0x00e0f0be
	.4byte 0x00f0e0be
	.4byte 0x010110be
	.4byte 0x011100be
	.4byte 0x0120d0c0
	.4byte 0x0130a0c0
	.4byte 0x000000bf
	.4byte 0x001010c0
	.4byte 0x002090c0
	.4byte 0x0030a0be
	.4byte 0x004070be
	.4byte 0x005080bf
	.4byte 0x0060a0bf
	.4byte 0x007010c2
	.4byte 0x008050bf
	.4byte 0x009060be
	.4byte 0x00a060bf
	.4byte 0x00b050c0
	.4byte 0x00c060c0
	.4byte 0x000000c0
	.4byte 0x001010bf
	.4byte 0x002080c1
	.4byte 0x0030e0c0
	.4byte 0x004070c1
	.4byte 0x0050b0bf
	.4byte 0x0060c0bf
	.4byte 0x007040c2
	.4byte 0x0080b0c0
	.4byte 0x009020bf
	.4byte 0x00a130be
	.4byte 0x00b080c0
	.4byte 0x00c030c2
	.4byte 0x00d120be
	.4byte 0x00e030c0
	.4byte 0x00f040c1
	.4byte 0x010030c1
	.4byte 0x014040c1
	.4byte 0x000000c1
	.4byte 0x001020c1
	.4byte 0x002010c1
	.4byte 0x003100c0
	.4byte 0x0040f0c0
	.4byte 0x005060c1
	.4byte 0x006050c1
	.4byte 0x007040c0
	.4byte 0x008020c0
	.4byte 0x009020c2
	.4byte 0x00a0b0c1
	.4byte 0x00b0a0c1
	.4byte 0x00c010c4
	.4byte 0x00d110c0
	.4byte 0x014140c0
	.4byte 0x01a130bd
	.4byte 0x000000c2
	.4byte 0x001070bf
	.4byte 0x002090c1
	.4byte 0x0030c0c0
	.4byte 0x004070c0
	.4byte 0x017100bd
	.4byte 0x01c040be
	.4byte 0x000000c4
	.4byte 0x0010c0c1
	.4byte 0x000001ff
	.4byte 0x00000016
	.4byte 0x0000000b
	.4byte 0x00018000
	.4byte 0x0000002e
	.4byte 0x020085c1
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0187
	.4byte 0x0200b280
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0186
	.4byte 0x0200b290
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0189
	.4byte 0x0200b280
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff0187
	.4byte 0x0200b280
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0xffff0189
	.4byte 0x0200b280
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte 0x0200b268
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0189
	.4byte 0x0200b280
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
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
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
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
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000001
	.4byte 0xffff001b
	.4byte 0x0000001b
	.4byte 0x00000021
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0xffff001d
	.4byte 0x0000001d
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000001
	.4byte 0xffff001f
	.4byte 0x0000001f
	.4byte 0x00000021
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000021
	.4byte 0xffff0021
	.4byte 0x00000021
	.4byte 0x00008c15
	.4byte 0x09c60008
	.4byte 0x02008851
	.4byte 0x10008c15
	.4byte 0x09c60008
	.4byte 0x020086d1
	.4byte 0x00008c15
	.4byte 0x09d2000a
	.4byte 0x02008a69
	.4byte 0x00008c15
	.4byte 0x09d3000b
	.4byte 0x02008a69
	.4byte 0x00008c15
	.4byte 0x09d4000c
	.4byte 0x02008a69
	.4byte 0x00008c15
	.4byte 0x09d5000d
	.4byte 0x02008a69
	.4byte 0x00008c15
	.4byte 0x09d6000e
	.4byte 0x02008a69
	.4byte 0x00008c15
	.4byte 0x09d7000f
	.4byte 0x02008a69
	.4byte 0x00008c15
	.4byte 0x09d80010
	.4byte 0x02008a69
	.4byte 0x00004e15
	.4byte 0xffff0011
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0012
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0013
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0014
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0015
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0016
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0017
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0018
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0019
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff001a
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff001b
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff001c
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff001d
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff001e
	.4byte 0x02009265
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008699
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020086b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000021
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000021
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00008c15
	.4byte 0x09dc0008
	.4byte 0x02008e61
	.4byte 0x00000009
	.4byte 0x09dc0008
	.4byte 0x02008e61
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x02009265
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008699
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020086b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
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
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte 0x0200a0fd
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte 0x0200a1d5
	.4byte 0x10008c15
	.4byte 0x09c70008
	.4byte 0x020086d1
	.4byte 0x00008c15
	.4byte 0x09c70008
	.4byte 0x02008851
	.4byte 0x00008c15
	.4byte 0x09dd000a
	.4byte 0x0200904d
	.4byte 0x00000009
	.4byte 0x09dd000a
	.4byte 0x0200904d
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008699
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020086b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
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
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000031
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000021
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000021
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff000e
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff000f
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0010
	.4byte 0x02009265
	.4byte 0x00004e15
	.4byte 0xffff0011
	.4byte 0x02009265
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200924d
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x02008c25
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x02008c25
	.4byte 0x10008c15
	.4byte 0x09c80008
	.4byte 0x020086d1
	.4byte 0x00008c15
	.4byte 0x09c80008
	.4byte 0x02008851
	.4byte 0x10008c15
	.4byte 0x09c90012
	.4byte 0x020086d1
	.4byte 0x00008c15
	.4byte 0x09c90012
	.4byte 0x02008851
	.4byte 0x10008c15
	.4byte 0x09ca0013
	.4byte 0x020086d1
	.4byte 0x00008c15
	.4byte 0x09ca0013
	.4byte 0x02008851
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008699
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020086b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
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
	.4byte 0x00004602
	.4byte 0xffff000d
	.4byte 0x02008385
	.4byte 0x00000001
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0xffff0042
	.4byte 0x020091c1
	.4byte 0x10008c15
	.4byte 0x09cb0008
	.4byte 0x020086d1
	.4byte 0x00008c15
	.4byte 0x09cb0008
	.4byte 0x02008851
	.4byte 0x10008c15
	.4byte 0x09db0009
	.4byte 0x020086d1
	.4byte 0x00008c15
	.4byte 0x09db0009
	.4byte 0x0200a261
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200a2a9
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200a2e1
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008699
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020086b5
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
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000021
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x02020028
	.4byte 0x02009d35
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte 0x02009ddd
	.4byte 0x00000002
	.4byte 0x0203002a
	.4byte 0x02009e55
	.4byte 0x00000002
	.4byte 0xffff002b
	.4byte 0x02009f05
	.4byte 0x00000006
	.4byte 0xffff0044
	.4byte 0x0200960d
	.4byte 0x00000006
	.4byte 0xffff0045
	.4byte 0x020099a1
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02008699
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020086b5
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200a319
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200a329
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
