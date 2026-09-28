.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200b679, 0x020088e5, 0x020088f1, 0x020088f9, 0x0200b419, 0x020088ed, 0x0200bee1
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
	bl 0x0200cca8
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
	bl 0x0200cd00
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200cdc8
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cd08
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
	bl 0x0200cca8
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
	bl 0x0200cd00
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x0200cdc8
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
	bl 0x0200cd68
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
	bl 0x0200cca8
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
	bl 0x0200cc98
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200cca0
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200cd00
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
	bl 0x0200cdc8
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
	bl 0x0200cc00
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
	bl 0x0200cc00
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x0200cc00
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
	bl 0x0200cc98
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200cca0
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
	.4byte 0x0200d698
	.4byte 0x02008125
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb520
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
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r1, r0, #0
	adds	r5, r3, #0
	movs	r4, #8
	adds	r5, #52
.L_02000384:
	ldmia	r5!, {r0}
	ldr	r2, [r1, #0]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020003aa
	ldr	r2, [r1, #4]
	ldr	r3, [r0, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020003aa
	ldr	r2, [r1, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	beq.n	.L_020003b2
.L_020003aa:
	adds	r4, #1
	cmp	r4, #63
	bls.n	.L_02000384
	movs	r0, #0
.L_020003b2:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #364]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200cd68
	ldrh	r3, [r0, #6]
	ldr	r1, [pc, #348]
	lsrs	r3, r3, #12
	lsls	r5, r3, #2
	ldr	r2, [pc, #348]
	mov	r9, r1
	ldr	r1, [r1, r5]
	mov	sl, r2
	mov	r3, sl
	adds	r2, r1, #0
	ands	r2, r3
	ldr	r3, [r0, #8]
	mov	r7, sp
	adds	r3, r3, r2
	str	r3, [r7, #0]
	lsls	r1, r1, #16
	ldr	r3, [r0, #12]
	mov	r8, r0
	str	r3, [r7, #4]
	ldr	r3, [r0, #16]
	adds	r0, r7, #0
	adds	r3, r3, r1
	str	r3, [r7, #8]
	mov	r1, r8
	bl 0x02008374
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200040a
	b.n	.L_02000520
.L_0200040a:
	mov	r0, r9
	ldr	r1, [r0, r5]
	mov	r3, sl
	adds	r2, r1, #0
	ands	r2, r3
	ldr	r3, [r6, #8]
	lsls	r1, r1, #16
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r7, #0
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r6, #0
	bl 0x02008374
	cmp	r0, #0
	beq.n	.L_02000440
	adds	r3, r0, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000520
.L_02000440:
	ldr	r3, [r6, #8]
	movs	r0, #128
	str	r3, [r7, #0]
	lsls	r0, r0, #13
	ldr	r3, [r6, #12]
	adds	r1, r6, #0
	adds	r3, r3, r0
	str	r3, [r7, #4]
	adds	r0, r7, #0
	ldr	r3, [r6, #16]
	str	r3, [r7, #8]
	bl 0x02008374
	cmp	r0, #0
	beq.n	.L_0200046c
	adds	r3, r0, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000520
.L_0200046c:
	adds	r2, r6, #0
	adds	r2, #34
	movs	r3, #2
	strb	r3, [r2, #0]
	mov	r2, r9
	ldr	r1, [r2, r5]
	mov	r3, sl
	adds	r2, r1, #0
	ands	r2, r3
	ldr	r3, [r6, #8]
	lsls	r1, r1, #16
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r6, #0
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
	bl 0x0200ccf8
	cmp	r0, #0
	bgt.n	.L_02000520
	adds	r3, r6, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	mov	sl, r3
	cmp	r3, #0
	bne.n	.L_02000520
	movs	r1, #8
	mov	r0, r8
	movs	r5, #204
	bl 0x0200cc98
	lsls	r5, r5, #6
	movs	r0, #15
	bl 0x0200cc10
	adds	r5, #51
	movs	r0, #185
	bl 0x0200cf30
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r6, #0
	bl 0x0200ccc8
	mov	r0, r8
	str	r5, [r0, #48]
	str	r5, [r0, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl 0x0200ccc8
	adds	r0, r6, #0
	bl 0x0200ccd0
	bl 0x0200ceb8
	ldr	r3, [r7, #0]
	mov	r1, sl
	str	r3, [r6, #8]
	ldr	r3, [r7, #8]
	str	r1, [r6, #36]
	str	r3, [r6, #16]
	str	r1, [r6, #44]
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #24
	str	r3, [r2, #56]
	str	r3, [r2, #64]
	movs	r0, #10
	ldrsh	r3, [r2, r0]
	str	r1, [r2, #36]
	lsls	r3, r3, #16
	str	r1, [r2, #44]
	str	r3, [r2, #8]
	movs	r1, #18
	ldrsh	r3, [r2, r1]
	mov	r0, r8
	lsls	r3, r3, #16
	str	r3, [r2, #16]
	movs	r1, #1
	bl 0x0200cc98
.L_02000520:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200cfec
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	adds	r5, r3, #0
	ldr	r3, [sp, #12]
	ldr	r6, [sp, #16]
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	beq.n	.L_0200058a
	cmp	r0, #2
	bhi.n	.L_02000560
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r0, r0, #1
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r0, [r4, r3]
	b.n	.L_02000562
.L_02000560:
	ldr	r0, [pc, #44]
.L_02000562:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	movs	r1, #0
	adds	r0, r0, r3
	cmp	r1, ip
	bcs.n	.L_0200058a
.L_02000570:
	lsls	r3, r1, #9
	movs	r2, #0
	adds	r3, r0, r3
	cmp	r2, r5
	bcs.n	.L_02000584
.L_0200057a:
	adds	r2, #1
	strb	r6, [r3, #2]
	adds	r3, #4
	cmp	r2, r5
	bcc.n	.L_0200057a
.L_02000584:
	adds	r1, #1
	cmp	r1, ip
	bcc.n	.L_02000570
.L_0200058a:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{lr}
	movs	r0, #15
	movs	r1, #56
	bl 0x0200ce30
	pop	{pc}
	push	{lr}
	movs	r0, #17
	movs	r1, #2
	movs	r2, #19
	bl 0x0200cea8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
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
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	sl, r3
	movs	r3, #192
	sub	sp, #4
	adds	r4, r1, #0
	lsls	r3, r3, #18
	mov	r8, r0
	str	r4, [sp, #0]
	adds	r6, r2, #0
	ldr	r5, [r3, #32]
	bl 0x0200cd20
	mov	r2, r8
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r5, [r5, r3]
	ldr	r3, [pc, #40]
	ldr	r4, [sp, #0]
	ldr	r2, [pc, #40]
	adds	r5, r5, r3
	lsls	r6, r6, #7
	asrs	r5, r5, #2
	adds	r1, r0, #0
	adds	r4, r4, r6
	adds	r5, r5, r2
	mov	r0, r8
	mov	r2, sl
	adds	r5, r5, r4
	bl 0x0200cef0
	strb	r0, [r5, #0]
	add	sp, #4
	movs	r0, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #60
.L_02000642:
	cmp	r5, #0
	beq.n	.L_02000654
	movs	r0, #1
	bl 0x0200cc10
	ldr	r3, [r6, #40]
	subs	r5, #1
	cmp	r3, #0
	bne.n	.L_02000642
.L_02000654:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_02000668
	movs	r0, #0
	b.n	.L_0200068e
.L_02000668:
	cmp	r0, #2
	bhi.n	.L_0200067c
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_0200067e
.L_0200067c:
	ldr	r4, [pc, #16]
.L_0200067e:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
.L_0200068e:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r1, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_020006a8
	movs	r0, #0
	b.n	.L_020006d4
.L_020006a8:
	cmp	r0, #2
	bhi.n	.L_020006bc
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_020006be
.L_020006bc:
	ldr	r4, [pc, #24]
.L_020006be:
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
.L_020006d4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	b.n	.L_0200074c
.L_020006e2:
	ldrh	r0, [r6, #0]
	bl 0x0200cd68
	adds	r5, r0, #0
	adds	r7, r5, #0
	movs	r3, #0
	adds	r7, #99
	strb	r3, [r7, #0]
	movs	r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	bl 0x02008658
	movs	r3, #64
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0200074a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	cmp	r0, #0
	beq.n	.L_02000758
	movs	r3, #212
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #8]
	subs	r2, r2, r3
	asrs	r2, r2, #20
	adds	r3, r2, #1
	asrs	r1, r1, #20
	ldr	r4, [r0, #0]
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	subs	r2, #1
	movs	r3, #255
	strb	r3, [r4, #2]
	lsls	r2, r2, #7
	ldr	r4, [r0, #0]
	adds	r1, r1, r2
	lsls	r1, r1, #2
	movs	r3, #1
	negs	r3, r3
	adds	r4, r4, r1
	strb	r3, [r4, #2]
	movs	r3, #1
	strb	r3, [r7, #0]
.L_0200074a:
	adds	r6, #2
.L_0200074c:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020006e2
.L_02000758:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #99
	adds	r2, r2, r5
	ldrb	r3, [r2, #0]
	mov	ip, r2
	cmp	r3, #0
	beq.n	.L_020007aa
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	cmp	r0, #0
.L_02000774:
	beq.n	.L_020007aa
	movs	r3, #212
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #8]
	subs	r2, r2, r3
	asrs	r2, r2, #20
	adds	r3, r2, #1
	asrs	r1, r1, #20
	ldr	r4, [r0, #0]
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	subs	r2, #1
	movs	r3, #0
	strb	r3, [r4, #2]
	lsls	r2, r2, #7
	ldr	r4, [r0, #0]
	adds	r1, r1, r2
.L_020007a0:
	lsls	r1, r1, #2
	adds	r4, r4, r1
	mov	r2, ip
	strb	r3, [r4, #2]
	strb	r3, [r2, #0]
.L_020007aa:
	pop	{r5, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r0, #91
	cmp	r3, #0
	bne.n	.L_020007f4
	subs	r1, #12
	adds	r3, r2, r1
	movs	r4, #0
	ldrsh	r3, [r3, r4]
.L_020007cc:
	cmp	r3, #0
	bne.n	.L_020007f4
	adds	r1, #4
	adds	r3, r2, r1
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	bne.n	.L_020007f4
	adds	r1, #10
	adds	r3, r2, r1
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	cmp	r1, #0
	bne.n	.L_020007f4
	movs	r4, #217
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020007fa
.L_020007f4:
	movs	r3, #1
	strb	r3, [r0, #0]
	b.n	.L_020007fc
.L_020007fa:
	strb	r1, [r0, #0]
.L_020007fc:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	lsls	r5, r2, #16
	adds	r7, r1, #0
	bl 0x0200cd68
	movs	r1, #4
	adds	r6, r0, #0
	bl 0x0200cc98
	adds	r2, r6, #0
	adds	r2, #90
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	subs	r2, #5
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	asrs	r5, r5, #16
	movs	r3, #128
	orrs	r3, r2
	lsls	r5, r5, #16
	strb	r3, [r1, #0]
	adds	r0, r6, #0
	movs	r1, #0
	lsrs	r5, r5, #16
	bl 0x0200cd00
	adds	r0, r5, #0
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_0200086a
	movs	r3, #128
	lsls	r3, r3, #13
	movs	r0, #10
	str	r3, [r6, #20]
	str	r3, [r6, #12]
	adds	r0, #255
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02000870
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl 0x0200cca0
	b.n	.L_02000870
.L_0200086a:
	ldr	r3, [pc, #12]
	str	r3, [r6, #20]
	str	r3, [r6, #12]
.L_02000870:
	ldr	r3, [pc, #8]
	str	r3, [r6, #108]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xfff20000
	.2byte 0x87ad
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	b.n	.L_020008d0
.L_0200088a:
	ldrh	r0, [r7, #0]
	bl 0x0200cd68
	movs	r3, #34
	adds	r6, r0, #0
	adds	r3, r3, r6
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	mov	r8, r3
	bl 0x0200cce0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	adds	r5, r0, #0
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200cd00
	mov	r2, r8
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	asrs	r5, r5, #19
	adds	r5, #4
	asrs	r2, r2, #20
	adds	r3, r5, #0
	asrs	r1, r1, #20
	adds	r7, #6
	bl 0x020085dc
.L_020008d0:
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200088a
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd6a4
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd6d4
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #92]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #84]
	cmp	r2, r3
	beq.n	.L_02000952
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000916
	ldr	r0, [pc, #80]
	b.n	.L_02000954
.L_02000916:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000920
	ldr	r0, [pc, #76]
	b.n	.L_02000954
.L_02000920:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200092a
	ldr	r0, [pc, #76]
	b.n	.L_02000954
.L_0200092a:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02000934
	ldr	r0, [pc, #72]
	b.n	.L_02000954
.L_02000934:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_0200093e
	ldr	r0, [pc, #72]
	b.n	.L_02000954
.L_0200093e:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02000948
	ldr	r0, [pc, #68]
	b.n	.L_02000954
.L_02000948:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000952
	ldr	r0, [pc, #68]
	b.n	.L_02000954
.L_02000952:
	ldr	r0, [pc, #68]
.L_02000954:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000e2
	.4byte 0x000000e3
	.4byte 0x0200d870
	.4byte 0x000000e4
	.4byte 0x0200d978
	.4byte 0x000000e5
	.4byte 0x0200da50
	.4byte 0x000000e6
	.4byte 0x0200dc30
	.4byte 0x000000e7
	.4byte 0x0200dca8
	.4byte 0x000000e8
	.4byte 0x0200dd38
	.4byte 0x000000e9
	.4byte 0x0200ded0
	.2byte 0xd840
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #92]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_020009be
	ldr	r0, [pc, #84]
	bl 0x0200cee0
	b.n	.L_020009f4
.L_020009be:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_020009cc
	ldr	r0, [pc, #76]
	bl 0x0200cee0
	b.n	.L_020009f4
.L_020009cc:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020009da
	ldr	r0, [pc, #72]
	bl 0x0200cee0
	b.n	.L_020009f4
.L_020009da:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_020009e8
	ldr	r0, [pc, #64]
	bl 0x0200cee0
	b.n	.L_020009f4
.L_020009e8:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020009f4
	ldr	r0, [pc, #60]
	bl 0x0200cee0
.L_020009f4:
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200875c
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x000000e3
	.4byte 0x0200d4fc
	.4byte 0x000000e4
	.4byte 0x0200d508
	.4byte 0x000000e5
	.4byte 0x0200d520
	.4byte 0x000000e7
	.4byte 0x0200d528
	.4byte 0x000000e8
	.2byte 0xd52e
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	adds	r3, r3, r1
	ldr	r7, [r3, #0]
	sub	sp, #8
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	bl 0x0200cee8
	ldr	r3, [pc, #176]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #168]
	cmp	r2, r3
	bne.n	.L_02000b3a
	ldr	r0, [pc, #164]
	bl 0x020086dc
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r0, #2
	bl 0x0200cce0
	ldr	r3, [r7, #12]
	cmp	r0, r3
	bne.n	.L_02000a84
	b.n	.L_02000bf4
.L_02000a84:
	movs	r2, #34
	adds	r2, r2, r7
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #85
	adds	r3, r3, r7
	mov	r8, r3
	mov	r1, r8
	movs	r3, #3
	movs	r0, #1
	strb	r3, [r1, #0]
	mov	sl, r2
	bl 0x0200cc10
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02000b22
	movs	r1, #128
	lsls	r1, r1, #5
	str	r1, [r7, #72]
	adds	r0, r7, #0
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r1, [r3, #0]
	movs	r6, #128
	ldr	r3, [pc, #60]
	lsls	r6, r6, #19
	adds	r6, #80
	ldr	r2, [r7, #16]
	strh	r3, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r2, r2, r3
	movs	r3, #156
	lsls	r3, r3, #1
	ldr	r1, [r7, #20]
	ldr	r0, [r7, #8]
	bl 0x02008080
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl 0x0200863c
	movs	r0, #213
	bl 0x0200cf30
	adds	r0, r5, #0
	bl 0x0200ccb0
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200cc98
	movs	r0, #1
	b.n	.L_02000b18
	.2byte 0x0000
	.4byte 0x00003f10
	.4byte 0x02000240
	.4byte 0x000000e7
	.2byte 0xd528
	.2byte 0x0200
.L_02000b18:
	bl 0x0200cc10
	movs	r3, #0
	strh	r3, [r6, #0]
	b.n	.L_02000b2e
.L_02000b22:
	adds	r0, r7, #0
	bl 0x0200863c
	movs	r0, #188
	bl 0x0200cf30
.L_02000b2e:
	movs	r3, #0
	mov	r1, r8
	mov	r2, sl
	strb	r3, [r1, #0]
	strb	r3, [r2, #0]
	b.n	.L_02000bf4
.L_02000b3a:
	ldr	r3, [pc, #152]
	cmp	r2, r3
	bne.n	.L_02000bf4
	ldr	r0, [pc, #148]
	bl 0x020086dc
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	movs	r0, #2
	bl 0x0200cce0
	ldr	r3, [r7, #12]
	cmp	r0, r3
	beq.n	.L_02000bf4
	movs	r3, #34
	adds	r3, r3, r7
	mov	r9, r3
	mov	r2, r9
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #85
.L_02000b64:
	adds	r3, r3, r7
	mov	sl, r3
	movs	r1, #0
	movs	r3, #3
	mov	r8, r1
	mov	r1, sl
	strb	r3, [r1, #0]
	movs	r0, #1
	bl 0x0200cc10
	movs	r2, #128
	lsls	r2, r2, #5
	str	r2, [r7, #72]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r5, #2
	orrs	r5, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strb	r5, [r1, #0]
	movs	r6, #128
	strh	r2, [r3, #0]
	ldr	r3, [pc, #56]
	lsls	r6, r6, #19
	adds	r6, #80
	ldr	r2, [r7, #16]
	strh	r3, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r2, r2, r3
	movs	r3, #156
	lsls	r3, r3, #1
	ldr	r1, [r7, #20]
	ldr	r0, [r7, #8]
	bl 0x02008080
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl 0x0200863c
	movs	r0, #213
	bl 0x0200cf30
	adds	r0, r5, #0
	bl 0x0200ccb0
	movs	r1, #4
	adds	r0, r7, #0
	bl 0x0200cc98
	movs	r0, #1
	b.n	.L_02000bdc
	.4byte 0x00003f10
	.4byte 0x000000e8
	.2byte 0xd52e
	.2byte 0x0200
.L_02000bdc:
	bl 0x0200cc10
	ldr	r5, [pc, #48]
	mov	r1, r8
	strh	r1, [r6, #0]
	adds	r0, r7, #0
	bl 0x0200863c
	mov	r2, sl
	mov	r3, r9
	strb	r5, [r2, #0]
	strb	r5, [r3, #0]
.L_02000bf4:
	ldr	r3, [r7, #8]
	movs	r1, #240
	asrs	r6, r3, #20
	ldr	r3, [r7, #16]
	lsls	r1, r1, #1
	asrs	r5, r3, #20
	ldr	r3, [pc, #20]
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000c2a
	cmp	r5, #16
	bne.n	.L_02000cc2
	b.n	.L_02000c20
	.4byte 0x00000000
	.4byte 0x02000240
	.2byte 0x00e3
	.2byte 0x0000
.L_02000c20:
	movs	r0, #159
	lsls	r0, r0, #4
	bl 0x0200cc88
	b.n	.L_02000cc2
.L_02000c2a:
	ldr	r3, [pc, #168]
	cmp	r2, r3
.L_02000c2e:
	bne.n	.L_02000c40
	cmp	r5, #41
	bne.n	.L_02000cc2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #241
	bl 0x0200cc88
	b.n	.L_02000cc2
.L_02000c40:
	ldr	r3, [pc, #148]
	cmp	r2, r3
	bne.n	.L_02000c76
	cmp	r6, #21
	bne.n	.L_02000cc2
	cmp	r5, #43
	bne.n	.L_02000c58
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #242
	bl 0x0200cc88
.L_02000c58:
	cmp	r5, #45
	bne.n	.L_02000c66
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #243
	bl 0x0200cc88
.L_02000c66:
	cmp	r5, #47
	bne.n	.L_02000cc2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #244
	bl 0x0200cc88
	b.n	.L_02000cc2
.L_02000c76:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_02000cae
	cmp	r6, #47
	bne.n	.L_02000c9e
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #245
	bl 0x0200cc88
	movs	r3, #46
	movs	r2, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #46
	movs	r1, #74
	movs	r2, #1
	movs	r3, #1
	bl 0x0200cce8
.L_02000c9e:
	cmp	r6, #57
	bne.n	.L_02000cc2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #246
	bl 0x0200cc88
	b.n	.L_02000cc2
.L_02000cae:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000cc2
	cmp	r6, #43
	bne.n	.L_02000cc2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #247
	bl 0x0200cc88
.L_02000cc2:
	bl 0x0200cd48
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x000000e7
	.2byte 0x00e8
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r1, #1
	movs	r0, #151
	bl 0x0200ce70
	lsls	r5, r5, #16
	ldr	r3, [pc, #36]
	asrs	r5, r5, #16
	movs	r2, #133
	lsls	r2, r2, #2
	lsls	r5, r5, #16
	adds	r3, r3, r2
	lsrs	r5, r5, #16
	ldr	r0, [r3, #0]
	adds	r1, r5, #0
	bl 0x0200ce78
	movs	r0, #1
	bl 0x0200ce68
	bl 0x0200ce80
	bl 0x0200ce88
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #116]
	sub	sp, #56
	ldr	r2, [r3, #0]
	movs	r3, #3
	mov	r8, r3
	mov	r4, r8
	ands	r4, r2
	adds	r6, r0, #0
	mov	r8, r4
	cmp	r4, #0
	bne.n	.L_02000d92
	movs	r3, #148
	add	r7, sp, #16
	adds	r3, #255
	strh	r3, [r7, #24]
	movs	r3, #2
	str	r3, [r7, #0]
	ldr	r3, [pc, #88]
	str	r3, [r7, #28]
	movs	r3, #15
	mov	sl, r3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000d5a
	movs	r1, #136
.L_02000d56:
	bl 0x0200cf28
.L_02000d5a:
	bl 0x0200cc28
	mov	r4, sl
	adds	r5, r0, #0
	ands	r5, r4
	bl 0x0200cc28
	movs	r3, #31
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #12
	ldr	r2, [r6, #16]
	str	r3, [sp, #4]
	movs	r3, #200
	subs	r5, #8
	lsls	r3, r3, #14
	adds	r3, #1
	lsls	r5, r5, #15
	mov	r4, r8
	str	r3, [sp, #8]
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #12]
	adds	r2, r2, r5
	movs	r3, #0
	str	r4, [sp, #0]
	str	r7, [sp, #12]
	bl 0x0200815c
.L_02000d92:
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.2byte 0xd02c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	adds	r2, r0, #0
	adds	r2, #99
	ldrb	r2, [r2, #0]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000dbe
	movs	r1, #10
	bl 0x0200cdc8
	b.n	.L_02000dc4
.L_02000dbe:
	movs	r1, #0
	bl 0x0200cdc8
.L_02000dc4:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r6, [r3, #0]
	adds	r5, r0, #0
	ldr	r1, [r6, #4]
	ldr	r2, [r6, #12]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #0
	bl 0x02008658
	movs	r7, #0
	asrs	r2, r0, #8
	b.n	.L_02000df0
.L_02000dec:
	ldrh	r7, [r5, #0]
	adds	r5, #2
.L_02000df0:
	movs	r1, #255
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	beq.n	.L_02000e10
	adds	r5, #2
	adds	r0, r3, #0
	ldrh	r3, [r5, #0]
	adds	r5, #2
	cmp	r2, r3
	bne.n	.L_02000dec
	bl 0x0200cd68
	ldrh	r7, [r5, #0]
	str	r0, [r6, #20]
.L_02000e10:
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r0, #0
	bl 0x0200ce90
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r6, r0, #0
	movs	r0, #0
	ldr	r7, [r3, #0]
	ldr	r5, [pc, #88]
	bl 0x0200ce90
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	beq.n	.L_02000e54
	ldr	r3, [pc, #76]
	cmp	r2, r3
	beq.n	.L_02000e54
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02000e54
	ldr	r5, [pc, #72]
.L_02000e54:
	cmp	r6, #2
	bne.n	.L_02000e6c
	adds	r0, r5, #0
	bl 0x02008dcc
	ldr	r3, [r7, #20]
	movs	r2, #4
	adds	r1, r3, #0
	adds	r1, #99
	strb	r2, [r1, #0]
	ldr	r2, [pc, #52]
	str	r2, [r3, #108]
.L_02000e6c:
	cmp	r6, #3
	bne.n	.L_02000e84
	adds	r0, r5, #0
	bl 0x02008dcc
	ldr	r3, [r7, #20]
	movs	r2, #2
	adds	r1, r3, #0
	adds	r1, #99
	strb	r2, [r1, #0]
	ldr	r2, [pc, #28]
	str	r2, [r3, #108]
.L_02000e84:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d500
	.4byte 0x02000240
	.4byte 0x000000e3
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0200d50c
	.4byte 0x02008da5
	.4byte 0x68836c44
	.4byte 0x191b6c81
	.4byte 0x68c36083
	.4byte 0x185b6cc2
	.4byte 0x690360c3
	.4byte 0x6103189b
	.4byte 0x1ae410e3
	.4byte 0x1ac910cb
	.4byte 0x1ad210d3
	.4byte 0x64816444
	.2byte 0x64c2
	.2byte 0x4770
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	mov	r8, r0
	movs	r0, #148
	adds	r0, #255
	sub	sp, #68
.L_02000eec:
	bl 0x0200cca8
	mov	r1, r8
	adds	r6, r0, #0
	ldrh	r0, [r1, #6]
	bl 0x0200cc38
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	lsls	r0, r0, #1
	strb	r3, [r2, #0]
	movs	r1, #1
	mov	sl, r0
	adds	r0, r6, #0
	bl 0x0200cc98
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200cd00
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	ldr	r3, [r6, #8]
	add	r2, sp, #56
	str	r3, [r2, #0]
	ldr	r3, [r6, #12]
	str	r3, [r2, #4]
	ldr	r3, [r6, #16]
	str	r3, [r2, #8]
.L_02000f2e:
	add	r7, sp, #56
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	ldr	r3, [r7, #4]
	add	r1, sl
	subs	r2, r2, r3
	str	r1, [r7, #0]
	asrs	r2, r2, #20
	asrs	r1, r1, #20
	movs	r0, #2
	bl 0x02008658
	asrs	r0, r0, #8
	mov	fp, r0
	cmp	r0, #212
	beq.n	.L_02000fc8
	cmp	r0, #222
	beq.n	.L_02000fc8
	movs	r2, #8
	mov	r9, r2
.L_02000f56:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	mov	r0, r9
	lsls	r3, r0, #2
	adds	r3, #20
	ldr	r5, [r2, r3]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02000fbc
	mov	r1, r8
	ldr	r2, [r1, #8]
	ldr	r3, [r5, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02000f90
	ldr	r2, [r1, #12]
	ldr	r3, [r5, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02000f90
	ldr	r2, [r1, #16]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	beq.n	.L_02000fbc
.L_02000f90:
	adds	r0, r5, #0
	adds	r0, #8
	adds	r1, r7, #0
	bl 0x020085b0
	cmp	r0, #8
	bgt.n	.L_02000fbc
	ldr	r2, [r5, #12]
	ldr	r3, [r7, #4]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02000fbc
	ldr	r3, [r5, #80]
	movs	r0, #136
	ldr	r3, [r3, #40]
	lsls	r0, r0, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r0
	bne.n	.L_020010b0
	b.n	.L_02000fc8
.L_02000fbc:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #64
	bne.n	.L_02000f56
	b.n	.L_02000f2e
.L_02000fc8:
	ldr	r3, [pc, #360]
	movs	r5, #128
	lsls	r5, r5, #10
	add	r7, sp, #56
	str	r5, [r6, #52]
	str	r5, [r6, #48]
	ldr	r2, [r7, #4]
	str	r3, [r6, #108]
	ldr	r1, [r7, #0]
	ldr	r3, [r7, #8]
	adds	r0, r6, #0
	bl 0x0200ccc8
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #7
	bl 0x0200ce10
	ldr	r1, [r7, #4]
	ldr	r2, [r7, #8]
	movs	r3, #1
	ldr	r0, [r7, #0]
	bl 0x0200ce18
	adds	r0, r6, #0
	bl 0x0200ccd0
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	bl 0x0200ccb0
	movs	r3, #0
	mov	r9, r3
.L_0200100c:
	ldr	r3, [pc, #296]
	movs	r2, #1
	ldr	r3, [r3, #0]
	mov	sl, r3
	mov	r0, sl
	ands	r0, r2
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_0200106a
	add	r1, sp, #16
	mov	r8, r1
	movs	r3, #148
	mov	r0, r8
	adds	r3, #255
	strh	r3, [r0, #24]
	ldr	r3, [pc, #272]
	str	r2, [r0, #0]
	str	r3, [r0, #36]
	ldr	r3, [pc, #268]
	movs	r6, #31
	str	r3, [r0, #28]
	bl 0x0200cc28
	adds	r5, r0, #0
	bl 0x0200cc28
	ands	r0, r6
	ldr	r4, [r7, #0]
	mov	r3, sl
	ands	r5, r6
	subs	r0, #16
	ldr	r1, [r7, #4]
	ldr	r2, [r7, #8]
	lsls	r0, r0, #13
	subs	r5, #16
	str	r3, [sp, #4]
	movs	r3, #153
	lsls	r3, r3, #17
	lsls	r5, r5, #13
	str	r0, [sp, #0]
	mov	r0, r8
	str	r3, [sp, #8]
	str	r0, [sp, #12]
	adds	r3, r5, #0
	adds	r0, r4, #0
	bl 0x0200815c
.L_0200106a:
	movs	r0, #1
	bl 0x0200cc10
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #16
	bne.n	.L_0200100c
	movs	r0, #24
	bl 0x0200cc10
	mov	r3, fp
	cmp	r3, #222
	bne.n	.L_02001090
	bl 0x02009350
	bl 0x0200cd48
	b.n	.L_02001126
.L_02001090:
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200ce08
	bl 0x0200ce20
	movs	r0, #10
	bl 0x0200cc10
	bl 0x0200cd48
	b.n	.L_02001126
.L_020010b0:
	ldr	r3, [pc, #128]
	movs	r5, #128
	lsls	r5, r5, #10
	str	r5, [r6, #52]
	str	r5, [r6, #48]
	ldr	r2, [r7, #4]
	str	r3, [r6, #108]
	ldr	r1, [r7, #0]
	ldr	r3, [r7, #8]
	adds	r0, r6, #0
	bl 0x0200ccc8
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #7
	bl 0x0200ce10
	ldr	r2, [r7, #8]
	ldr	r1, [r7, #4]
	movs	r3, #1
	ldr	r0, [r7, #0]
	bl 0x0200ce18
	adds	r0, r6, #0
	bl 0x0200ccd0
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	bl 0x0200ccb0
	movs	r0, #188
	lsls	r0, r0, #2
	bl 0x0200cc88
	mov	r1, r9
	lsls	r0, r1, #16
	lsrs	r0, r0, #16
	bl 0x02008ce4
	movs	r0, #188
	lsls	r0, r0, #2
	bl 0x0200cc90
	ldr	r3, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200ce08
	bl 0x0200ce20
	movs	r0, #10
	bl 0x0200cc10
	bl 0x0200cd48
.L_02001126:
	add	sp, #68
.L_02001128:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02008d1d
	.4byte 0x0300122c
	.4byte 0x02008ea5
	.4byte 0x0200d02c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
.L_0200114c:
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
.L_0200115c:
	movs	r0, #10
	bl 0x0200cd38
.L_02001162:
	ldr	r0, [r5, #20]
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r1, #7
	bl 0x0200cdc8
	movs	r0, #3
	bl 0x0200cd38
	movs	r1, #0
	ldr	r0, [r5, #20]
	bl 0x0200cdc8
	movs	r0, #1
	bl 0x0200cd38
	ldr	r0, [r5, #20]
	bl 0x02008ed0
	bl 0x0200cd48
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r6, r1, #0
	adds	r7, r0, #0
	ldr	r5, [r3, #0]
	cmp	r6, #0
	bne.n	.L_020011a6
	bl 0x0200cb18
.L_020011a6:
	cmp	r7, #2
	bne.n	.L_020011be
	ldr	r3, [r5, #20]
	movs	r2, #240
	ldr	r1, [r3, #12]
	lsls	r2, r2, #12
	ldr	r0, [r3, #8]
	adds	r1, r1, r2
	ldr	r2, [r3, #16]
	movs	r3, #1
	bl 0x0200ca5c
.L_020011be:
	cmp	r7, #3
	bne.n	.L_020011e0
	ldr	r3, [r5, #20]
.L_020011c4:
	movs	r2, #240
	ldr	r1, [r3, #12]
	lsls	r2, r2, #12
	ldr	r0, [r3, #8]
	adds	r1, r1, r2
	ldr	r2, [r3, #16]
	movs	r3, #8
	bl 0x0200ca5c
	ldr	r2, [r5, #20]
	movs	r3, #0
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	str	r3, [r2, #16]
.L_020011e0:
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r6, r3
	bne.n	.L_020011ee
	bl 0x0200cbd4
.L_020011ee:
	cmp	r6, #178
	bne.n	.L_020011fa
.L_020011f2:
	ldr	r0, [r5, #20]
	movs	r1, #3
	bl 0x0200cc98
.L_020011fa:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #312]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200cd68
	adds	r5, r0, #0
	ldrh	r1, [r5, #6]
	movs	r3, #128
.L_02001220:
	lsls	r3, r3, #6
	adds	r1, r1, r3
	movs	r3, #192
.L_02001226:
	lsls	r3, r3, #8
	ldr	r2, [pc, #284]
	ands	r1, r3
	ldr	r3, [r5, #8]
	movs	r0, #128
	lsls	r0, r0, #12
	ands	r3, r2
	mov	r6, sp
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	ands	r3, r2
	adds	r3, r3, r0
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r2, r6, #0
	str	r3, [r6, #8]
	bl 0x0200cc40
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x02008374
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02001336
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	movs	r1, #6
	adds	r0, r5, #0
	bl 0x0200cc98
	movs	r0, #6
	bl 0x0200cc10
	movs	r0, #152
	bl 0x0200cf30
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200cc98
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #52]
	ldr	r3, [pc, #184]
	movs	r2, #85
	str	r3, [r5, #40]
	adds	r2, r2, r5
	mov	sl, r2
	ldrb	r2, [r2, #0]
	movs	r3, #126
	ands	r3, r2
	mov	r2, sl
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cd00
	ldr	r3, [r5, #8]
	ldr	r0, [r6, #0]
	movs	r1, #12
	subs	r0, r0, r3
	bl 0x0200cc00
	ldr	r3, [r5, #16]
	mov	fp, r0
	ldr	r0, [r6, #8]
	movs	r1, #12
	subs	r0, r0, r3
	bl 0x0200cc00
	movs	r3, #0
	mov	r9, r0
	mov	r8, r3
.L_020012ce:
	ldr	r2, [r7, #8]
	ldr	r3, [r6, #0]
	movs	r0, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #8]
	add	r2, fp
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #8]
	subs	r2, r2, r3
	ldr	r3, [r5, #16]
	add	r2, r9
	adds	r3, r3, r2
	str	r3, [r5, #16]
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl 0x0200cc10
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #12
	bne.n	.L_020012ce
	ldr	r3, [r7, #8]
	movs	r2, #128
	str	r3, [r5, #8]
	ldr	r3, [r7, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	ldr	r3, [r7, #16]
	adds	r0, r5, #0
	str	r3, [r5, #16]
	movs	r1, #6
	bl 0x0200cc98
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cd00
	mov	r3, sl
	ldrb	r2, [r3, #0]
	movs	r3, #3
	orrs	r3, r2
	mov	r2, sl
	strb	r3, [r2, #0]
	bl 0x0200cd48
.L_02001336:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfff00000
	.2byte 0xcccc
	.2byte 0x0004
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #0
	sub	sp, #76
	str	r1, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r2, r3, r2
	add	r3, sp, #36
	mov	r8, r3
	movs	r3, #148
	adds	r3, #255
	mov	fp, r1
	mov	r9, r1
	mov	sl, r1
	mov	r1, r8
	str	r2, [sp, #16]
	strh	r3, [r1, #24]
	movs	r3, #1
	str	r3, [r1, #0]
	ldr	r3, [pc, #808]
	movs	r2, #240
	str	r3, [r1, #28]
	movs	r3, #230
	lsls	r3, r3, #9
	adds	r3, #204
	str	r3, [r1, #8]
	str	r3, [r1, #12]
	ldr	r3, [pc, #796]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #792]
	add	r2, sp, #24
	cmp	r1, r3
	bne.n	.L_020013c6
	movs	r3, #164
	lsls	r3, r3, #17
	str	r3, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, #4]
	movs	r3, #200
	lsls	r3, r3, #16
	movs	r0, #192
	str	r3, [r2, #8]
	movs	r1, #20
	movs	r2, #69
	lsls	r0, r0, #2
	b.n	.L_0200141c
.L_020013c6:
	ldr	r3, [pc, #760]
	cmp	r1, r3
	bne.n	.L_020013fa
	movs	r1, #232
	movs	r3, #128
	lsls	r3, r3, #14
	lsls	r1, r1, #16
	str	r1, [sp, #20]
	movs	r0, #192
	str	r3, [r2, #4]
	movs	r3, #248
	lsls	r3, r3, #16
	lsls	r0, r0, #2
	str	r3, [r2, #8]
	adds	r0, #1
	movs	r2, #14
	movs	r3, #72
	mov	fp, r2
	mov	r9, r3
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_0200142c
	movs	r1, #212
	mov	sl, r1
	b.n	.L_0200142c
.L_020013fa:
	ldr	r3, [pc, #712]
	cmp	r1, r3
	bne.n	.L_0200142c
	movs	r3, #196
	lsls	r3, r3, #17
	str	r3, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, #4]
	movs	r0, #192
	movs	r3, #134
	lsls	r3, r3, #18
	lsls	r0, r0, #2
	str	r3, [r2, #8]
	movs	r1, #24
	movs	r2, #90
	adds	r0, #2
.L_0200141c:
	mov	fp, r1
	mov	r9, r2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_0200142c
	movs	r3, #212
	mov	sl, r3
.L_0200142c:
	movs	r7, #0
.L_0200142e:
	bl 0x0200cc28
	movs	r5, #31
	ldr	r1, [sp, #20]
	ands	r0, r5
	lsls	r0, r0, #16
	add	r6, sp, #24
.L_0200143c:
	adds	r0, r1, r0
	str	r0, [r6, #0]
	movs	r0, #145
	bl 0x0200cf30
	bl 0x0200cc28
	movs	r3, #0
	ldr	r4, [r6, #0]
	ldr	r1, [r6, #4]
	ldr	r2, [r6, #8]
	str	r3, [sp, #4]
	movs	r3, #232
	lsls	r3, r3, #14
	adds	r3, #1
	ands	r0, r5
	str	r3, [sp, #8]
	lsls	r0, r0, #12
	mov	r3, r8
	str	r0, [sp, #0]
	str	r3, [sp, #12]
	adds	r0, r4, #0
	movs	r3, #0
	bl 0x0200815c
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cd10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200cd10
	adds	r7, #1
	movs	r0, #8
	bl 0x0200cd38
	cmp	r7, #16
	bne.n	.L_0200142e
	movs	r0, #148
	bl 0x0200cf30
	mov	r1, r8
	movs	r3, #1
	str	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r1, #8]
	str	r3, [r1, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r1, #16]
	str	r3, [r1, #20]
	ldr	r2, [sp, #20]
	ldr	r3, [pc, #524]
	movs	r7, #0
	adds	r2, r2, r3
	str	r2, [sp, #20]
	adds	r5, r2, #0
.L_020014c4:
	str	r5, [r6, #0]
	bl 0x0200cc28
	movs	r3, #0
	movs	r4, #31
	ldr	r1, [r6, #4]
	ldr	r2, [r6, #8]
	ands	r4, r0
	ldr	r0, [r6, #0]
	str	r3, [sp, #0]
	movs	r3, #224
	lsls	r3, r3, #12
	adds	r3, #1
	str	r3, [sp, #8]
	mov	r3, r8
	lsls	r4, r4, #10
	str	r3, [sp, #12]
	movs	r3, #0
	str	r4, [sp, #4]
	bl 0x0200815c
	movs	r1, #192
	lsls	r1, r1, #11
	adds	r7, #1
	adds	r5, r5, r1
	cmp	r7, #8
	bne.n	.L_020014c4
	mov	r3, r9
	movs	r5, #3
	movs	r7, #6
	movs	r0, #68
	movs	r1, #70
	mov	r2, fp
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	bl 0x0200ccd8
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cd10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200cd10
	movs	r0, #30
	bl 0x0200cd38
	mov	r2, fp
	mov	r3, r9
	movs	r0, #71
	movs	r1, #70
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	bl 0x0200ccd8
	movs	r3, #148
.L_02001548:
	mov	r2, r8
	adds	r3, #255
	strh	r3, [r2, #24]
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #10
	str	r3, [r2, #4]
	ldr	r3, [pc, #372]
	str	r3, [r2, #28]
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_02001580
	movs	r1, #70
	mov	r3, r9
	movs	r0, #68
	mov	r2, fp
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	bl 0x0200ccd8
	movs	r0, #30
	bl 0x0200cd38
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #340]
	adds	r3, r3, r1
	str	r3, [r6, #8]
	b.n	.L_02001742
.L_02001580:
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200cd10
	movs	r0, #107
	bl 0x0200cf30
	movs	r5, #0
.L_02001598:
	movs	r7, #0
.L_0200159a:
	bl 0x0200cc28
	movs	r1, #48
	bl 0x0200cc08
	ldr	r2, [sp, #20]
	lsls	r0, r0, #16
	adds	r0, r2, r0
	ldr	r2, [r6, #8]
	lsls	r3, r5, #18
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #12
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #9
	str	r0, [r6, #0]
	ldr	r1, [r6, #4]
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #204
	lsls	r3, r3, #14
.L_020015c8:
	str	r3, [sp, #8]
	mov	r3, r8
.L_020015cc:
	str	r3, [sp, #12]
	adds	r7, #1
	movs	r3, #0
	bl 0x0200815c
	cmp	r7, #6
	bne.n	.L_0200159a
	movs	r0, #3
	bl 0x0200cc10
	lsrs	r3, r5, #2
	adds	r3, #6
	str	r3, [sp, #4]
	movs	r7, #3
	movs	r0, #74
	movs	r1, #70
	mov	r2, fp
	mov	r3, r9
	adds	r5, #1
	str	r7, [sp, #0]
	bl 0x0200ccd8
	cmp	r5, #12
	bne.n	.L_02001598
	movs	r3, #8
	str	r3, [sp, #4]
	movs	r1, #70
	movs	r0, #74
	mov	r2, fp
	mov	r3, r9
	str	r7, [sp, #0]
	bl 0x0200ccd8
	movs	r1, #0
	mov	sl, r1
.L_02001612:
	movs	r3, #3
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200165c
	movs	r7, #0
.L_0200161e:
	bl 0x0200cc28
	movs	r1, #48
	add	r5, sp, #24
	bl 0x0200cc08
	ldr	r3, [sp, #20]
	ldr	r2, [r5, #8]
	lsls	r0, r0, #16
	adds	r0, r3, r0
	movs	r3, #192
	lsls	r3, r3, #14
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #9
	str	r0, [r5, #0]
	ldr	r1, [r5, #4]
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #204
.L_02001648:
	lsls	r3, r3, #14
	str	r3, [sp, #8]
	mov	r3, r8
	str	r3, [sp, #12]
	adds	r7, #1
	movs	r3, #0
	bl 0x0200815c
	cmp	r7, #4
	bne.n	.L_0200161e
.L_0200165c:
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_02001678
	ldr	r2, [sp, #16]
	ldr	r1, [pc, #108]
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
	bl 0x0200ccc0
	movs	r0, #1
	bl 0x0200cd38
	b.n	.L_0200172c
.L_02001678:
	mov	r2, sl
	cmp	r2, #63
	bhi.n	.L_02001696
	ldr	r3, [pc, #40]
	lsrs	r2, r2, #2
	subs	r3, r3, r2
	movs	r1, #128
	lsls	r1, r1, #19
	lsls	r3, r3, #8
	orrs	r3, r2
	adds	r1, #82
	strh	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [pc, #24]
	b.n	.L_020016a0
.L_02001696:
	mov	r3, sl
	cmp	r3, #64
	bne.n	.L_020016d8
	movs	r2, #128
	ldr	r3, [pc, #16]
.L_020016a0:
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	b.n	.L_0200172c
	.4byte 0x00000010
	.4byte 0x00003f44
	.4byte 0x00000000
	.4byte 0x0200d050
	.4byte 0x02000240
	.4byte 0x000000e3
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0xfff80000
	.4byte 0x0200d02c
	.4byte 0xffd80000
	.2byte 0x0000
	.2byte 0xffe0
.L_020016d8:
	.2byte 0x4651
	cmp	r1, #127
	bhi.n	.L_02001714
	ldr	r2, [sp, #16]
	movs	r1, #128
	ldr	r3, [r2, #12]
	lsls	r1, r1, #8
	adds	r3, r3, r1
	str	r3, [r2, #12]
	add	r2, sp, #24
	ldr	r3, [r2, #8]
	ldr	r1, [pc, #292]
	movs	r0, #71
	adds	r3, r3, r1
	str	r3, [r2, #8]
	mov	r3, sl
	subs	r3, #47
	mov	r2, r9
	lsrs	r3, r3, #5
	subs	r3, r2, r3
	movs	r1, #1
	movs	r2, #3
	str	r2, [sp, #0]
	str	r1, [sp, #4]
	adds	r3, #8
	movs	r1, #76
	mov	r2, fp
	bl 0x0200ccd8
	b.n	.L_0200172c
.L_02001714:
	mov	r3, sl
	cmp	r3, #128
	bne.n	.L_0200172c
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200cd10
.L_0200172c:
	movs	r1, #1
	add	sl, r1
.L_02001730:
	movs	r0, #1
	bl 0x0200cc10
	mov	r2, sl
	cmp	r2, #160
	beq.n	.L_0200173e
	b.n	.L_02001612
.L_0200173e:
	bl 0x0200cd18
.L_02001742:
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200cf30
	movs	r0, #136
	bl 0x0200cf30
	movs	r3, #230
	lsls	r3, r3, #9
	add	r6, sp, #36
	adds	r3, #204
	str	r3, [r6, #12]
	str	r3, [r6, #8]
	movs	r7, #0
.L_0200175e:
	bl 0x0200cc28
	movs	r1, #48
	add	r5, sp, #24
	bl 0x0200cc08
	ldr	r3, [sp, #20]
	ldr	r2, [r5, #8]
	lsls	r0, r0, #16
	adds	r0, r3, r0
	movs	r3, #192
	lsls	r3, r3, #14
	adds	r2, r2, r3
	movs	r3, #236
	lsls	r3, r3, #14
	str	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r7, #1
	movs	r5, #0
	str	r3, [sp, #8]
	movs	r3, #0
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	bl 0x0200815c
	cmp	r7, #6
	bne.n	.L_0200175e
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cd10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
.L_020017b2:
	adds	r2, #102
	bl 0x0200cd10
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #64
	mov	r2, fp
	mov	r3, r9
	movs	r0, #68
	str	r7, [sp, #4]
	bl 0x0200ccd8
	bl 0x0200cd18
	movs	r0, #30
	bl 0x0200cd38
	ldr	r1, [sp, #16]
	ldr	r3, [pc, #64]
	str	r5, [r1, #12]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_020017f0
	bl 0x0200b4bc
	b.n	.L_02001806
.L_020017f0:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020017fc
	bl 0x0200b4f0
	b.n	.L_02001806
.L_020017fc:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02001806
	bl 0x0200b548
.L_02001806:
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff8000
	.4byte 0x02000240
	.4byte 0x000000e3
	.4byte 0x000000e4
	.2byte 0x00e5
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #612]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r7, r0
	ldr	r0, [r5, #0]
	sub	sp, #92
	bl 0x0200cd68
	ldrh	r3, [r0, #6]
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r1, r3, r1
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r2, #0
	ands	r1, r3
	str	r1, [sp, #36]
	str	r2, [sp, #32]
	str	r2, [sp, #28]
	str	r2, [sp, #24]
	mov	sl, r0
	mov	fp, r2
	mov	r9, r2
	mov	r8, r2
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	mov	r3, sl
	ldr	r1, [r3, #8]
	ldr	r0, [pc, #552]
	movs	r3, #128
	lsls	r3, r3, #12
	ands	r1, r0
	add	r6, sp, #80
	adds	r1, r1, r3
	str	r1, [r6, #0]
	mov	r4, sl
	ldr	r2, [r4, #16]
	asrs	r1, r1, #16
	ands	r2, r0
	adds	r2, r2, r3
	str	r2, [r6, #8]
	asrs	r2, r2, #16
	ldr	r0, [r5, #0]
	bl 0x0200cd90
	movs	r0, #20
	bl 0x0200cd38
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200cf30
	mov	r0, sl
	adds	r0, #35
	str	r0, [sp, #20]
	movs	r3, #191
	ldrb	r2, [r0, #0]
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200cd38
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200cd10
	movs	r0, #107
	bl 0x0200cf30
	movs	r0, #30
	bl 0x0200cd38
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #452]
	cmp	r2, r3
	bne.n	.L_02001948
	movs	r0, #20
	movs	r3, #26
	movs	r4, #160
	str	r3, [sp, #24]
.L_020018ec:
	mov	r9, r0
	movs	r2, #84
	mov	r0, fp
	lsls	r4, r4, #17
	movs	r3, #212
	str	r2, [sp, #28]
	str	r4, [sp, #32]
	lsls	r3, r3, #17
	str	r0, [r6, #4]
	movs	r0, #192
	movs	r1, #89
	str	r3, [r6, #8]
	lsls	r0, r0, #2
	mov	r8, r1
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02001914
	movs	r1, #212
	mov	fp, r1
.L_02001914:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200cc90
	movs	r2, #0
	str	r2, [sp, #36]
	movs	r1, #15
	movs	r2, #13
	movs	r3, #64
	movs	r0, #0
	bl 0x02008694
	movs	r0, #15
	movs	r1, #3
	bl 0x0200cdf0
	movs	r6, #0
.L_02001936:
	adds	r0, r6, #0
	adds	r0, #9
	movs	r1, #1
	adds	r6, #1
	bl 0x0200cdb0
	cmp	r6, #7
	bne.n	.L_02001936
	b.n	.L_02001a46
.L_02001948:
	ldr	r1, [pc, #336]
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r1, r4
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #336]
	cmp	r2, r3
	bne.n	.L_020019b8
	movs	r3, #92
	movs	r0, #224
	str	r3, [sp, #28]
	lsls	r0, r0, #17
	movs	r3, #0
	movs	r4, #21
	str	r0, [sp, #32]
	str	r4, [sp, #24]
	movs	r0, #192
	str	r3, [r6, #4]
	movs	r3, #172
	lsls	r3, r3, #17
	lsls	r0, r0, #2
	movs	r1, #28
	movs	r2, #83
	str	r3, [r6, #8]
	adds	r0, #1
	mov	r9, r1
	mov	r8, r2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_0200198c
	movs	r1, #212
	mov	fp, r1
.L_0200198c:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200cc90
	movs	r1, #24
	movs	r2, #15
	movs	r3, #64
	movs	r0, #0
	bl 0x02008694
	movs	r1, #9
	movs	r0, #0
	movs	r2, #16
	movs	r3, #64
	bl 0x02008694
	movs	r0, #11
	movs	r1, #4
	bl 0x0200cdb0
	b.n	.L_02001a46
.L_020019b8:
	ldr	r3, [pc, #240]
	cmp	r2, r3
	bne.n	.L_02001a46
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #8
	bgt.n	.L_02001a0a
	movs	r3, #13
	movs	r4, #198
	str	r3, [sp, #24]
	movs	r0, #49
	movs	r3, #0
	movs	r2, #113
	lsls	r4, r4, #18
	str	r2, [sp, #28]
	str	r4, [sp, #32]
	mov	r9, r0
	str	r3, [r6, #4]
	movs	r0, #192
	movs	r3, #216
	lsls	r3, r3, #16
	lsls	r0, r0, #2
	movs	r1, #75
	str	r3, [r6, #8]
	adds	r0, #2
	mov	r8, r1
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_020019fe
	movs	r0, #212
	mov	fp, r0
.L_020019fe:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200cc90
	b.n	.L_02001a46
.L_02001a0a:
	movs	r3, #115
	movs	r0, #206
	str	r3, [sp, #28]
	lsls	r0, r0, #18
	movs	r3, #0
	movs	r4, #42
	str	r0, [sp, #32]
	str	r4, [sp, #24]
	movs	r0, #192
	str	r3, [r6, #4]
	movs	r3, #170
	lsls	r3, r3, #18
	lsls	r0, r0, #2
	movs	r1, #51
	movs	r2, #104
	str	r3, [r6, #8]
	adds	r0, #2
	mov	r9, r1
	mov	r8, r2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02001a3c
	movs	r1, #212
	mov	fp, r1
.L_02001a3c:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200cc90
.L_02001a46:
	movs	r0, #181
	bl 0x0200cf30
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cd10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200cd10
	mov	r2, r8
	subs	r2, #1
	str	r2, [sp, #16]
	movs	r3, #3
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [sp, #16]
	mov	r0, r9
	mov	r1, r8
	mov	r2, r9
	bl 0x0200ccd8
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_02001ab0
	bl 0x0200cd18
	movs	r0, #20
	bl 0x0200cd38
	b.n	.L_02001b90
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x000000e3
	.4byte 0x000000e4
	.2byte 0x00e6
	.2byte 0x0000
.L_02001ab0:
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200cd10
	movs	r3, #148
	add	r7, sp, #40
	adds	r3, #255
	strh	r3, [r7, #24]
	movs	r3, #2
	str	r3, [r7, #0]
	movs	r3, #10
	str	r3, [r7, #4]
	ldr	r3, [pc, #164]
	movs	r4, #0
	str	r3, [r7, #28]
	mov	r8, r4
.L_02001ad8:
	movs	r6, #0
.L_02001ada:
	bl 0x0200cc28
	movs	r1, #48
	add	r5, sp, #80
	bl 0x0200cc08
	mov	r2, r8
	lsls	r3, r2, #18
	ldr	r2, [r5, #8]
	ldr	r1, [sp, #32]
	adds	r2, r2, r3
	movs	r3, #184
	lsls	r3, r3, #13
	adds	r2, r2, r3
	lsls	r0, r0, #16
	movs	r3, #128
	adds	r0, r1, r0
	lsls	r3, r3, #9
	str	r0, [r5, #0]
	ldr	r1, [r5, #4]
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #204
	lsls	r3, r3, #14
	str	r3, [sp, #8]
	adds	r6, #1
	movs	r3, #0
	str	r7, [sp, #12]
	bl 0x0200815c
	cmp	r6, #6
	bne.n	.L_02001ada
	movs	r0, #3
	bl 0x0200cc10
	mov	r4, r8
	lsrs	r3, r4, #1
	movs	r2, #3
	adds	r3, #1
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #75
	movs	r1, #64
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #24]
	bl 0x0200ccd8
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #10
	bne.n	.L_02001ad8
	movs	r6, #0
.L_02001b46:
	ldr	r3, [pc, #40]
	lsrs	r1, r6, #2
	movs	r0, #128
	subs	r3, r3, r1
	lsls	r2, r1, #8
	lsls	r0, r0, #19
	orrs	r2, r3
	adds	r0, #82
	strh	r2, [r0, #0]
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r0, #3
	adds	r6, #1
	bl 0x0200cc10
	cmp	r6, #64
	bne.n	.L_02001b46
	b.n	.L_02001b7c
	.4byte 0x00000010
	.4byte 0x00003f44
	.2byte 0xd02c
	.2byte 0x0200
.L_02001b7c:
	movs	r3, #3
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #78
	movs	r1, #64
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #24]
	bl 0x0200ccd8
.L_02001b90:
	ldr	r5, [pc, #256]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #248]
	cmp	r2, r3
	bne.n	.L_02001bb2
	movs	r0, #15
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02001bb2:
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200cf30
	movs	r0, #181
	bl 0x0200cf30
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cd10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200cd10
	movs	r3, #3
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [sp, #16]
	movs	r1, #63
	mov	r2, r9
	movs	r0, #72
	bl 0x0200ccd8
	bl 0x0200cd18
	movs	r0, #146
	lsls	r0, r0, #2
	bl 0x0200cf30
	ldr	r0, [sp, #20]
	movs	r3, #64
	ldrb	r2, [r0, #0]
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200cd38
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r6, r5, r1
	ldr	r0, [r6, #0]
	bl 0x0200cd68
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
.L_02001c28:
	mov	r2, sl
	ldr	r3, [r2, #8]
	ldr	r1, [pc, #108]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	add	r5, sp, #80
	adds	r3, r3, r2
	str	r3, [r5, #0]
	mov	r4, sl
	ldr	r3, [r4, #12]
	str	r3, [r5, #4]
.L_02001c40:
	ldr	r3, [r4, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r0, [sp, #36]
	movs	r1, #128
	lsls	r1, r1, #8
	eors	r1, r0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #13
	bl 0x0200cc40
	add	r1, sp, #36
	ldrh	r1, [r1, #0]
	mov	r2, sl
	strh	r1, [r2, #6]
	movs	r2, #2
	ldrsh	r1, [r5, r2]
	ldr	r0, [r6, #0]
	movs	r3, #10
	ldrsh	r2, [r5, r3]
	bl 0x0200cd90
	ldr	r0, [r6, #0]
.L_02001c72:
	bl 0x0200cd68
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl 0x0200cd48
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000e3
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb560
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r6, r0, #0
	movs	r0, #9
	ldr	r5, [r3, #0]
	mov	r8, r1
	bl 0x0200cd68
	mov	r1, r8
	str	r0, [r5, #20]
	adds	r0, r6, #0
	bl 0x02009190
	movs	r0, #212
	lsls	r0, r0, #2
	bl 0x0200cc88
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r6, r0, #0
	movs	r0, #11
	ldr	r5, [r3, #0]
	mov	r8, r1
	bl 0x0200cd68
	mov	r1, r8
	str	r0, [r5, #20]
	adds	r0, r6, #0
	bl 0x02009190
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #81
	bl 0x0200cc88
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #100
	ldrh	r3, [r1, #0]
	movs	r2, #128
	adds	r3, #1
	strh	r3, [r1, #0]
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	sub	sp, #56
	cmp	r3, r2
	ble.n	.L_02001d22
	movs	r3, #0
	str	r3, [r5, #108]
.L_02001d22:
	ldrh	r3, [r1, #0]
	movs	r6, #1
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02001d5c
	movs	r3, #148
	add	r4, sp, #16
	adds	r3, #255
	strh	r3, [r4, #24]
	movs	r3, #2
	str	r3, [r4, #0]
	movs	r3, #10
	str	r3, [r4, #4]
	ldr	r3, [pc, #32]
	ldr	r0, [r5, #8]
	str	r3, [r4, #28]
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [sp, #0]
	movs	r3, #204
	lsls	r3, r3, #14
	str	r3, [sp, #8]
	ldr	r1, [r5, #12]
	ldr	r2, [r5, #16]
	movs	r3, #0
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	bl 0x0200815c
.L_02001d5c:
	add	sp, #56
	pop	{r5, r6, pc}
	.2byte 0xd02c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	movs	r0, #107
	bl 0x0200cf30
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200cd10
	movs	r0, #30
	bl 0x0200cd38
	movs	r0, #194
	movs	r1, #1
	movs	r2, #136
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	bl 0x0200ce18
	bl 0x0200ce20
	movs	r3, #148
	add	r6, sp, #16
	adds	r3, #255
	strh	r3, [r6, #24]
	movs	r3, #2
	str	r3, [r6, #0]
	movs	r3, #10
	str	r3, [r6, #4]
	ldr	r3, [pc, #864]
	movs	r2, #0
	str	r3, [r6, #28]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r6, #12]
	str	r3, [r6, #8]
	add	r3, sp, #56
	str	r2, [r3, #4]
	mov	r8, r3
	movs	r7, #0
.L_02001dce:
	bl 0x0200cc28
	movs	r5, #31
	ands	r0, r5
	movs	r2, #190
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	adds	r0, r0, r2
	mov	r3, r8
	str	r0, [r3, #0]
.L_02001de2:
	bl 0x0200cc28
	adds	r2, r0, #0
	ands	r2, r5
	movs	r3, #128
	lsls	r3, r3, #17
	lsls	r2, r2, #16
	adds	r2, r2, r3
	mov	r3, r8
	str	r2, [r3, #8]
	ldr	r0, [r3, #0]
	ldr	r1, [r3, #4]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #0]
	movs	r3, #236
	lsls	r3, r3, #14
	str	r3, [sp, #8]
	movs	r5, #0
	movs	r3, #0
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	bl 0x0200815c
	movs	r3, #7
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_02001e24
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cf30
.L_02001e24:
	cmp	r7, #32
	bne.n	.L_02001e68
	movs	r1, #194
	movs	r2, #248
	movs	r0, #13
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #186
	lsls	r1, r1, #2
	movs	r2, #216
	movs	r0, #13
	bl 0x0200cd80
	movs	r0, #13
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #13
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #72]
	movs	r0, #13
	bl 0x0200cd68
	adds	r0, #100
	strh	r5, [r0, #0]
	movs	r0, #13
	b.n	.L_0200204c
.L_02001e68:
	cmp	r7, #40
	bne.n	.L_02001eae
	movs	r1, #194
	movs	r2, #248
	movs	r0, #20
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #194
	movs	r2, #164
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #20
	bl 0x0200cd80
	movs	r0, #20
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #20
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #72]
	movs	r0, #20
	bl 0x0200cd68
	adds	r0, #100
	strh	r5, [r0, #0]
	movs	r0, #20
	b.n	.L_0200204c
.L_02001eae:
	cmp	r7, #48
	bne.n	.L_02001ef2
	movs	r1, #194
	movs	r2, #248
.L_02001eb6:
	movs	r0, #14
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #214
	lsls	r1, r1, #2
	movs	r2, #232
	movs	r0, #14
	bl 0x0200cd80
	movs	r0, #14
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #14
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #72]
	movs	r0, #14
	bl 0x0200cd68
	adds	r0, #100
	strh	r5, [r0, #0]
	movs	r0, #14
	b.n	.L_0200204c
.L_02001ef2:
	cmp	r7, #56
	bne.n	.L_02001f38
	movs	r1, #194
	movs	r2, #248
	movs	r0, #19
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #174
	movs	r2, #156
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #19
	bl 0x0200cd80
	movs	r0, #19
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #19
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #72]
	movs	r0, #19
	bl 0x0200cd68
	adds	r0, #100
	strh	r5, [r0, #0]
	movs	r0, #19
	b.n	.L_0200204c
.L_02001f38:
	cmp	r7, #64
	bne.n	.L_02001f7e
	movs	r1, #194
	movs	r2, #248
	movs	r0, #15
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #182
	movs	r2, #140
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #15
	bl 0x0200cd80
	movs	r0, #15
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #15
	bl 0x0200cd68
	movs	r3, #128
.L_02001f6c:
	lsls	r3, r3, #8
	str	r3, [r0, #72]
	movs	r0, #15
	bl 0x0200cd68
	adds	r0, #100
	strh	r5, [r0, #0]
	movs	r0, #15
	b.n	.L_0200204c
.L_02001f7e:
	cmp	r7, #72
	bne.n	.L_02001fc4
	movs	r1, #194
	movs	r2, #248
	movs	r0, #18
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #206
	movs	r2, #140
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #18
	bl 0x0200cd80
	movs	r0, #18
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #18
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #72]
	movs	r0, #18
	bl 0x0200cd68
	adds	r0, #100
.L_02001fbe:
	strh	r5, [r0, #0]
	movs	r0, #18
	b.n	.L_0200204c
.L_02001fc4:
	cmp	r7, #80
	bne.n	.L_0200200a
	movs	r1, #194
	movs	r2, #248
	movs	r0, #16
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #206
	movs	r2, #156
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #16
	bl 0x0200cd80
	movs	r0, #16
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #16
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #72]
	movs	r0, #16
	bl 0x0200cd68
	adds	r0, #100
	strh	r5, [r0, #0]
	movs	r0, #16
	b.n	.L_0200204c
.L_0200200a:
	cmp	r7, #88
	bne.n	.L_02002056
	movs	r1, #194
	movs	r2, #248
	movs	r0, #17
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #194
	lsls	r1, r1, #2
	movs	r2, #232
	movs	r0, #17
	bl 0x0200cd80
	movs	r0, #17
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r0, #40]
.L_02002034:
	movs	r0, #17
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #72]
.L_02002040:
	movs	r0, #17
	bl 0x0200cd68
	adds	r0, #100
	strh	r5, [r0, #0]
	movs	r0, #17
.L_0200204c:
	bl 0x0200cd68
	ldr	r3, [pc, #204]
	str	r3, [r0, #108]
	b.n	.L_020020c4
.L_02002056:
	cmp	r7, #112
	bne.n	.L_020020c4
	movs	r1, #194
	movs	r2, #248
	movs	r0, #12
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200cda0
	movs	r1, #174
	movs	r0, #12
	lsls	r1, r1, #2
	movs	r2, #232
	bl 0x0200cd80
	movs	r0, #174
	movs	r1, #1
	movs	r2, #232
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200ce18
	movs	r0, #12
	bl 0x0200cd68
	movs	r3, #208
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #12
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #72]
	movs	r0, #12
	bl 0x0200cd68
	adds	r0, #100
	strh	r5, [r0, #0]
	movs	r0, #12
	bl 0x0200cd68
	ldr	r3, [pc, #112]
	movs	r2, #230
	str	r3, [r0, #108]
	movs	r1, #1
	movs	r0, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200cd10
.L_020020c4:
	movs	r0, #3
	adds	r7, #1
	bl 0x0200cc10
	cmp	r7, #128
	beq.n	.L_020020d2
	b.n	.L_02001dce
.L_020020d2:
	movs	r0, #12
	bl 0x0200cd98
	bl 0x0200cd18
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200cf30
	movs	r0, #40
	bl 0x0200cd38
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #248
	bl 0x0200cc88
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200ce08
	bl 0x0200ce20
	movs	r0, #10
	bl 0x0200cc10
	bl 0x0200cd48
	add	sp, #68
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d050
	.4byte 0x02009d05
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #56
	adds	r3, r3, r2
	movs	r0, #193
	movs	r2, #1
	strb	r2, [r3, #0]
	lsls	r0, r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02002178
	movs	r0, #22
	bl 0x0200cd68
	movs	r1, #15
	bl 0x0200cdc8
	movs	r1, #158
	movs	r2, #156
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #22
	bl 0x0200cda0
	movs	r0, #22
	bl 0x0200cd68
	adds	r5, r0, #0
	movs	r0, #22
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r0, #28]
	str	r3, [r5, #24]
.L_02002178:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_020021bc
	movs	r0, #21
	bl 0x0200cd68
	movs	r1, #15
	bl 0x0200cdc8
	movs	r1, #150
	movs	r2, #156
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #21
	bl 0x0200cda0
	movs	r0, #21
	bl 0x0200cd68
	adds	r5, r0, #0
	movs	r0, #21
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r0, #28]
	str	r3, [r5, #24]
.L_020021bc:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	movs	r2, #211
	lsls	r2, r2, #4
	adds	r3, r7, r2
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	bl 0x0200cd68
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200cdc8
	movs	r0, #136
	bl 0x0200cf30
	movs	r6, #0
.L_020021e6:
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r0, #5
	adds	r6, #1
	bl 0x0200cc10
	cmp	r6, #16
	bne.n	.L_020021e6
	movs	r2, #211
	lsls	r2, r2, #4
	adds	r3, r7, r2
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #238
	adds	r0, r0, r3
	bl 0x0200cc88
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02002224
	b.n	.L_02002314
.L_02002224:
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02002314
	movs	r0, #11
	bl 0x0200cd68
	adds	r5, r0, #0
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200cd10
	movs	r0, #141
	bl 0x0200cf30
	movs	r0, #30
	bl 0x0200cd38
	movs	r0, #154
	movs	r1, #1
	movs	r2, #148
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200ce18
	bl 0x0200ce20
	movs	r0, #30
	bl 0x0200cd38
	movs	r0, #151
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cf30
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [r5, #16]
	movs	r3, #192
	lsls	r3, r3, #14
	adds	r2, r2, r3
	movs	r3, #156
	lsls	r3, r3, #1
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #20]
	bl 0x02008080
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r7, r0, #0
	movs	r6, #0
.L_020022b4:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #16]
	movs	r0, #5
	adds	r3, r3, r2
	str	r3, [r5, #12]
	b.n	.L_020022cc
	.4byte 0x00001000
	.4byte 0x00003f10
	.2byte 0x0000
	.2byte 0xffff
.L_020022cc:
	.2byte 0x3601
	bl 0x0200cd38
	cmp	r6, #32
	bne.n	.L_020022b4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #42
	bl 0x0200cf30
	movs	r1, #4
	adds	r0, r5, #0
	bl 0x0200cc98
	adds	r0, r7, #0
	bl 0x0200ccb0
	movs	r0, #1
	bl 0x0200cd38
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r1, #38
	movs	r2, #18
	movs	r3, #0
	movs	r0, #0
	bl 0x02008694
	bl 0x0200cd48
	b.n	.L_02002314
	.2byte 0x0000
	.2byte 0x0000
.L_02002314:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #211
	lsls	r2, r2, #4
	adds	r3, r3, r2
.L_02002326:
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02002362
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_0200234a
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
.L_0200234a:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02002362
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
.L_02002362:
	pop	{pc}
	push	{r5, lr}
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	ldr	r5, [pc, #108]
	movs	r1, #1
	adds	r0, r5, #0
	adds	r5, #1
	bl 0x0200cd28
	adds	r0, r5, #0
	bl 0x0200cdd0
	movs	r1, #0
	movs	r0, #12
	bl 0x0200cdd8
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200cd58
	cmp	r0, #0
	bne.n	.L_020023da
	movs	r0, #230
	movs	r1, #0
	lsls	r0, r0, #1
	bl 0x0200cd50
	movs	r0, #230
	lsls	r0, r0, #1
	bl 0x0200cd30
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_020023be
	bl 0x0200cd48
	b.n	.L_020023de
.L_020023be:
	movs	r0, #230
	lsls	r0, r0, #1
	movs	r1, #3
	bl 0x0200ce58
	movs	r0, #12
	movs	r1, #2
	bl 0x0200cdb0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #249
	bl 0x0200cc88
.L_020023da:
	bl 0x0200cd48
.L_020023de:
	pop	{r5, pc}
	.4byte 0x000028cc
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #8
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	movs	r0, #188
	bl 0x0200cf30
	movs	r5, #1
	movs	r6, #2
	movs	r1, #67
	movs	r2, #15
	movs	r3, #19
	movs	r0, #72
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ccd8
	movs	r0, #8
	bl 0x0200cd38
	movs	r1, #67
	movs	r2, #15
	movs	r3, #19
	movs	r0, #73
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ccd8
	movs	r0, #8
	bl 0x0200cd38
	movs	r1, #67
	movs	r2, #15
	movs	r3, #19
	movs	r0, #74
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ccd8
	movs	r0, #8
	bl 0x0200cd38
	movs	r1, #67
	movs	r2, #15
	movs	r3, #19
	movs	r0, #75
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ccd8
	movs	r0, #15
	bl 0x0200cd38
	ldr	r5, [pc, #76]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200cd68
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r0, [r5, #0]
	bl 0x0200cd68
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #52]
	str	r3, [r6, #48]
	movs	r0, #123
	bl 0x0200cf30
	movs	r2, #164
	movs	r1, #248
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	bl 0x0200cd88
	movs	r0, #5
	bl 0x0200cd38
	movs	r0, #1
	bl 0x0200ce28
	bl 0x0200cd48
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	sub	sp, #8
	bl 0x0200cf00
	bl 0x0200cd68
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	asrs	r5, r3, #20
	lsls	r3, r6, #16
	adds	r1, r3, r5
	ldr	r3, [r0, #36]
	cmp	r3, #0
	bne.n	.L_0200253c
	ldr	r3, [r0, #44]
	cmp	r3, #0
	bne.n	.L_0200253c
	ldr	r2, [pc, #108]
	movs	r0, #1
	ldr	r3, [r2, #0]
	negs	r0, r0
	cmp	r3, r0
	beq.n	.L_0200253c
	cmp	r1, r3
	beq.n	.L_0200253c
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
	bl 0x0200ccf0
	ldr	r3, [pc, #72]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_0200253c
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #2
	adds	r1, r6, #0
	adds	r2, r5, #0
	ldr	r7, [r3, #108]
	bl 0x02008658
	asrs	r0, r0, #8
	cmp	r0, #0
	beq.n	.L_0200253c
	movs	r1, #181
	adds	r3, r0, #0
	lsls	r1, r1, #1
	adds	r3, #200
	adds	r2, r7, r1
	strh	r3, [r2, #0]
.L_0200253c:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x0200e710
	.4byte 0x0200e7f4
	.4byte 0x0200e7f8
	.4byte 0x0200e7ec
	.4byte 0x0200e7f0
	.4byte 0x02000240
	.2byte 0x00e9
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #1
	negs	r2, r2
	movs	r1, #144
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x0200cc18
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200e710
	.2byte 0xa4ad
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #8
	bl 0x0200cf00
	bl 0x0200cd68
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
	beq.n	.L_020025f0
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
	bl 0x0200ccf0
	mov	r3, sl
	mov	r1, r8
	ldr	r2, [r3, #0]
	ldr	r0, [r5, #0]
	ldr	r3, [r1, #0]
	ldr	r1, [r6, #0]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	movs	r0, #70
	movs	r1, #72
	bl 0x0200ccf0
.L_020025f0:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e710
	.4byte 0x0200e7ec
	.4byte 0x0200e7f0
	.4byte 0x0200e7f4
	.2byte 0xe7f8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #8
	bl 0x0200cf00
	bl 0x0200cd68
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
	beq.n	.L_02002684
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
	bl 0x0200ccf0
	mov	r3, sl
	mov	r1, r8
	ldr	r2, [r3, #0]
	ldr	r0, [r5, #0]
	ldr	r3, [r1, #0]
	ldr	r1, [r6, #0]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	movs	r0, #71
	movs	r1, #72
	bl 0x0200ccf0
.L_02002684:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e710
	.4byte 0x0200e7ec
	.4byte 0x0200e7f0
	.4byte 0x0200e7f4
	.2byte 0xe7f8
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
	bl 0x0200cca8
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
	bl 0x02008038
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cd00
	ldr	r1, [r7, #80]
	movs	r3, #13
	ldrb	r2, [r1, #5]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #5]
	bl 0x0200cd40
	movs	r0, #0
	b.n	.L_02002700
	.4byte 0x00000010
	.2byte 0x3f44
	.2byte 0x0000
.L_02002700:
	bl 0x0200ce90
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #74
	strh	r3, [r2, #0]
.L_0200270e:
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	movs	r0, #106
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl 0x0200cf30
	movs	r5, #0
.L_02002722:
	ldr	r1, [pc, #32]
	movs	r3, #128
	lsls	r2, r5, #9
	lsls	r3, r3, #19
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200cc10
	b.n	.L_02002748
	.2byte 0x0000
	.4byte 0x00003f1f
	.4byte 0x00008000
	.2byte 0x0010
	.2byte 0x0000
.L_02002748:
	cmp	r5, #8
	bne.n	.L_02002722
	movs	r5, #0
.L_0200274e:
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
	bl 0x0200cc10
	cmp	r5, #16
	bne.n	.L_0200274e
	b.n	.L_02002774
	.4byte 0x00000010
	.2byte 0x1000
	.2byte 0x0000
.L_02002774:
	mov	r3, r8
	asrs	r6, r6, #4
	asrs	r5, r3, #4
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
	bl 0x0200ccd8
	movs	r3, #255
	adds	r1, r6, #0
	adds	r2, r5, #0
	lsls	r3, r3, #8
	movs	r0, #0
	bl 0x02008694
	adds	r0, r7, #0
	bl 0x0200ccb0
	bl 0x0200cd48
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
	b.n	.L_020027cc
	.2byte 0x0000
	.2byte 0x0000
.L_020027cc:
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
	bl 0x0200cd68
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008658
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
	beq.n	.L_02002848
	cmp	r0, #7
	beq.n	.L_02002848
	bl 0x0200a57c
	movs	r0, #132
	lsls	r1, r6, #3
	lsls	r0, r0, #1
	adds	r1, #10
	bl 0x0200a6a4
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02002838
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200cc88
.L_02002838:
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r0, r0, r2
	bl 0x0200cc88
	b.n	.L_02002860
.L_02002848:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02002860
	movs	r0, #132
	lsls	r1, r6, #3
	lsls	r0, r0, #1
	adds	r1, #10
	bl 0x0200a6a4
.L_02002860:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_020028a8
	movs	r0, #11
	bl 0x0200cd68
	movs	r1, #15
	bl 0x0200cdc8
	movs	r1, #136
	movs	r2, #180
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	movs	r0, #11
	bl 0x0200cda0
	movs	r0, #11
	bl 0x0200cd68
	adds	r5, r0, #0
	movs	r0, #11
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r0, #28]
	str	r3, [r5, #24]
.L_020028a8:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #211
	lsls	r2, r2, #4
	adds	r3, r3, r2
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	bl 0x0200cd68
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200cdc8
	movs	r0, #136
	bl 0x0200cf30
	movs	r6, #0
.L_020028d2:
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r0, #5
	adds	r6, #1
	bl 0x0200cc10
	cmp	r6, #16
	bne.n	.L_020028d2
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x0200cc88
	movs	r0, #9
	bl 0x0200cd68
	adds	r5, r0, #0
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200cd10
	movs	r0, #141
	bl 0x0200cf30
	movs	r0, #30
	bl 0x0200cd38
	movs	r0, #240
	movs	r1, #1
	movs	r2, #172
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #15
	bl 0x0200ce18
	bl 0x0200ce20
	movs	r0, #30
	bl 0x0200cd38
	movs	r0, #151
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200cf30
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [r5, #16]
	movs	r3, #192
	lsls	r3, r3, #14
	adds	r2, r2, r3
	movs	r3, #156
	lsls	r3, r3, #1
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #20]
	bl 0x02008080
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r7, r0, #0
	movs	r6, #0
.L_02002978:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #16]
	movs	r0, #5
	adds	r3, r3, r2
	str	r3, [r5, #12]
	b.n	.L_02002990
	.4byte 0x00001000
	.4byte 0x00003f10
	.2byte 0x0000
	.2byte 0xffff
.L_02002990:
	.2byte 0x3601
	bl 0x0200cd38
	cmp	r6, #32
	bne.n	.L_02002978
	movs	r0, #128
.L_0200299c:
	lsls	r0, r0, #2
	adds	r0, #42
	bl 0x0200cf30
	movs	r1, #4
	adds	r0, r5, #0
	bl 0x0200cc98
	adds	r0, r7, #0
	bl 0x0200ccb0
	movs	r0, #1
	bl 0x0200cd38
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r1, #7
	movs	r2, #21
	movs	r3, #128
	movs	r0, #0
	bl 0x02008694
	bl 0x0200cd48
	b.n	.L_020029d8
	.2byte 0x0000
	.2byte 0x0000
.L_020029d8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #211
	lsls	r2, r2, #4
	adds	r3, r3, r2
.L_020029ea:
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02002a10
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02002a10
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
.L_02002a10:
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #168
	lsls	r0, r0, #2
	bl 0x0200cca8
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
	movs	r1, #10
	bl 0x0200cdc8
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cc98
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cd00
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cd08
	ldr	r1, [pc, #8]
	adds	r0, r5, #0
	bl 0x0200cca0
	pop	{r5, pc}
	.2byte 0xd534
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r0, #0
	movs	r2, #102
	adds	r2, r2, r6
.L_02002a7c:
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
	bl 0x0200cc40
	adds	r2, r6, #0
	adds	r2, #98
	ldrb	r3, [r2, #0]
	movs	r0, #0
	adds	r3, #255
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_02002b1c
	ldr	r3, [r6, #76]
	movs	r2, #128
	lsls	r2, r2, #10
	movs	r0, #1
	cmp	r3, r2
	beq.n	.L_02002b1c
	adds	r0, r6, #0
	bl 0x0200aa14
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
	bl 0x0200cc40
	adds	r0, r6, #0
	bl 0x0200aa14
	ldr	r3, [r6, #76]
	movs	r0, #1
	add	r3, r9
	str	r3, [r6, #76]
.L_02002b1c:
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
	bl 0x0200cc38
	ldr	r1, [r7, #76]
	ldr	r6, [pc, #116]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x60b8
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	bl 0x0200cc30
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
	beq.n	.L_02002ba4
.L_02002b80:
	ldr	r3, [pc, #72]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002ba4
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r0, [r3, #0]
	lsls	r0, r0, #10
	bl 0x0200cc30
	ldrb	r3, [r5, #0]
	muls	r3, r0
	str	r3, [r7, #76]
	ldrb	r3, [r5, #0]
	adds	r3, #10
	strb	r3, [r5, #0]
.L_02002ba4:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_02002bc2
	ldrb	r3, [r5, #0]
	cmp	r3, #141
	bne.n	.L_02002bc0
	adds	r3, r2, #0
	subs	r3, #128
	mov	r1, r8
	strh	r3, [r1, #0]
.L_02002bc0:
	movs	r0, #1
.L_02002bc2:
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
	bl 0x0200cc00
	str	r0, [r5, #68]
	ldr	r3, [r5, #12]
	ldr	r0, [r6, #12]
	movs	r1, #10
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	bl 0x0200cc00
	str	r0, [r5, #76]
	ldr	r3, [r5, #16]
	ldr	r0, [r6, #16]
	movs	r1, #10
	subs	r0, r0, r3
	bl 0x0200cc00
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
	bne.n	.L_02002cc2
	add	r2, sp, #28
	str	r3, [r2, #4]
	movs	r3, #209
	lsls	r3, r3, #1
	adds	r3, #255
	strh	r3, [r2, #24]
	movs	r3, #1
	str	r3, [r2, #0]
	mov	r8, r2
	bl 0x0200cc28
	movs	r6, #31
	mov	r2, sl
	ldr	r3, [r2, #8]
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #16
	add	r5, sp, #16
	adds	r3, r3, r0
	str	r3, [r5, #0]
	bl 0x0200cc28
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
.L_02002cc2:
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
	bl 0x0200cd68
	adds	r7, r0, #0
	mov	r0, fp
	bl 0x0200cd68
	mov	sl, r0
	movs	r0, #78
	bl 0x0200cf30
	movs	r0, #30
	bl 0x0200cd38
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	bl 0x0200ce10
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
	bl 0x0200ce18
	movs	r0, #141
	bl 0x0200cf30
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200cd10
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200ce48
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200ce40
	movs	r0, #60
	bl 0x0200ce50
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
	bl 0x0200cd78
	movs	r0, #194
	bl 0x0200cf30
	movs	r0, #45
	bl 0x0200cd38
	movs	r1, #128
	mov	r0, r9
	lsls	r1, r1, #1
	bl 0x0200cdc0
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
	b.n	.L_02002da8
	.2byte 0x0000
	.4byte 0x00001008
	.4byte 0x00003f10
	.2byte 0xd590
	.2byte 0x0200
.L_02002da8:
	movs	r0, #168
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #2
	bl 0x0200cca8
	adds	r5, r0, #0
	movs	r0, #246
	bl 0x0200cf30
	bl 0x0200cc28
	movs	r3, #31
	ldr	r2, [r5, #8]
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r5, #8]
	bl 0x0200cc28
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
	movs	r1, #10
	bl 0x0200cdc8
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200cc98
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cd00
	adds	r0, r5, #0
	movs	r1, #1
.L_02002e1c:
	bl 0x0200cd08
	ldr	r1, [pc, #68]
	adds	r0, r5, #0
	bl 0x0200cca0
	movs	r6, #128
	ldr	r3, [pc, #48]
	ldr	r5, [pc, #48]
	lsls	r6, r6, #19
	adds	r6, #82
.L_02002e32:
	strh	r3, [r6, #0]
	movs	r0, #2
	bl 0x0200cc10
	movs	r0, #2
	strh	r5, [r6, #0]
.L_02002e3e:
	bl 0x0200cc10
	ldr	r3, [pc, #32]
	movs	r0, #2
	strh	r3, [r6, #0]
	bl 0x0200cc10
	strh	r5, [r6, #0]
.L_02002e4e:
	movs	r0, #2
.L_02002e50:
	bl 0x0200cc10
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	b.n	.L_02002e6c
	.4byte 0x00001004
	.4byte 0x0000100a
	.4byte 0x00001010
	.2byte 0xd544
	.2byte 0x0200
.L_02002e6c:
	cmp	r3, #16
	bne.n	.L_02002da8
	ldr	r3, [pc, #56]
	movs	r0, #30
	strh	r3, [r6, #0]
	bl 0x0200cc10
	movs	r0, #0
	mov	r8, r0
.L_02002e7e:
	mov	r0, sl
.L_02002e80:
	ldr	r3, [r0, #16]
	mov	r2, sl
	movs	r0, #168
	ldr	r1, [r2, #8]
	lsls	r0, r0, #2
	ldr	r2, [r2, #12]
	bl 0x0200cca8
	adds	r5, r0, #0
	movs	r0, #195
	bl 0x0200cf30
	bl 0x0200cc28
	ldr	r3, [pc, #16]
	movs	r2, #128
	ands	r0, r3
	lsls	r2, r2, #8
	adds	r6, r5, #0
	adds	r0, r0, r2
	adds	r6, #100
	b.n	.L_02002eb4
	.4byte 0x00001008
	.2byte 0x7fff
	.2byte 0x0000
.L_02002eb4:
	strh	r0, [r6, #0]
	bl 0x0200cc28
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
	movs	r1, #10
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl 0x0200cdc8
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02008038
	adds	r0, r5, #0
	movs	r1, #7
	b.n	.L_02002f0c
	.2byte 0x0000
	.2byte 0x0000
.L_02002f0c:
	bl 0x0200cc98
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cd00
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cd08
	adds	r2, r5, #0
	movs	r3, #0
	ldrsh	r1, [r6, r3]
	ldr	r0, [r5, #76]
	adds	r2, #8
	bl 0x0200cc40
	adds	r0, r5, #0
	ldr	r1, [pc, #208]
	bl 0x0200cca0
	mov	r0, r8
.L_02002f38:
	cmp	r0, #3
	bne.n	.L_02002f60
	movs	r1, #128
	mov	r0, fp
	lsls	r1, r1, #1
	bl 0x0200cdc0
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
	bl 0x0200cd78
.L_02002f60:
	movs	r0, #8
	bl 0x0200cc10
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	cmp	r0, #16
	beq.n	.L_02002f72
	b.n	.L_02002e7e
.L_02002f72:
	movs	r0, #220
	bl 0x0200cf30
	movs	r0, #16
	bl 0x0200cc10
	movs	r2, #2
	mov	r8, r2
.L_02002f82:
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
	bl 0x0200cca8
	adds	r5, r0, #0
	bl 0x0200cc28
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
	movs	r1, #10
	bl 0x0200cdc8
	adds	r0, r5, #0
	movs	r1, #7
	bl 0x0200cc98
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200cd00
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200cd08
	b.n	.L_02003010
	.4byte 0x00000000
	.4byte 0x0200d560
	.4byte 0x0200d5b4
	.2byte 0x0000
	.2byte 0xfff8
.L_02003010:
	.2byte 0x1c28
	ldr	r1, [pc, #168]
	bl 0x0200cca0
	movs	r0, #2
	add	r8, r0
	mov	r2, r8
	cmp	r2, #32
	bne.n	.L_02002f82
	movs	r0, #220
	bl 0x0200cf30
	movs	r0, #50
	bl 0x0200cc10
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200ce40
	movs	r0, #8
	bl 0x0200ce50
	movs	r0, #16
	bl 0x0200cc10
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200cd10
	mov	r0, r9
	movs	r1, #0
	bl 0x0200cdc0
	mov	r0, fp
	movs	r1, #0
	bl 0x0200cdc0
.L_02003060:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200cf30
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200ce40
	movs	r0, #80
	bl 0x0200ce50
	mov	r0, r9
	ldr	r1, [pc, #64]
	bl 0x0200cd78
	ldr	r1, [pc, #64]
	mov	r0, fp
	bl 0x0200cd78
	ldr	r3, [pc, #60]
	mov	r0, sl
	str	r3, [r0, #108]
	movs	r0, #120
	bl 0x0200cc10
	mov	r2, sl
	movs	r0, #195
	str	r6, [r2, #108]
	lsls	r0, r0, #1
	bl 0x0200cf30
	bl 0x0200cea0
	movs	r0, #1
	bl 0x0200cc10
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d570
	.4byte 0x0200d5d8
	.4byte 0x0200d608
	.2byte 0xac49
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	ldr	r6, [pc, #496]
	movs	r1, #1
	adds	r0, r6, #0
	bl 0x0200cd28
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #250
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_020030f6
	bl 0x0200cd48
	b.n	.L_020032c8
.L_020030f6:
	adds	r0, r6, #0
	bl 0x0200cdd0
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
	bl 0x0200cd70
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #153
	movs	r0, #5
	adds	r1, #51
	bl 0x0200cd70
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x0200cda8
	movs	r0, #1
	bl 0x0200cd38
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200cd88
	movs	r1, #136
	lsls	r1, r1, #1
	movs	r2, #136
	movs	r0, #5
	bl 0x0200cd88
	ldr	r0, [r5, #0]
	bl 0x0200cd98
	movs	r0, #5
	bl 0x0200cd98
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cde8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200cde8
	movs	r0, #10
	bl 0x0200cd38
	movs	r0, #5
	movs	r1, #0
	bl 0x0200cde0
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200cdb0
	movs	r0, #30
	bl 0x0200cd38
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #152
	bl 0x0200cd90
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200cde8
	movs	r0, #20
	bl 0x0200cd38
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x0200cde8
	movs	r0, #20
	bl 0x0200cd38
	movs	r1, #3
	movs	r0, #5
	bl 0x0200cdb0
	movs	r0, #30
	bl 0x0200cd38
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200cde8
	movs	r0, #20
	bl 0x0200cd38
	movs	r1, #132
	movs	r0, #5
	lsls	r1, r1, #1
	movs	r2, #137
	bl 0x0200cd90
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200cde8
	movs	r0, #20
	bl 0x0200cd38
	adds	r0, r6, #3
	movs	r1, #1
	bl 0x0200cd28
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200cd10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200cd10
	bl 0x0200cd18
	movs	r0, #8
	movs	r1, #5
	bl 0x0200acd0
	movs	r1, #154
	movs	r0, #5
	bl 0x0200cd60
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #250
	bl 0x0200cc88
	movs	r2, #0
	movs	r1, #5
	ldr	r0, [r5, #0]
	bl 0x0200cdb8
	movs	r0, #10
	bl 0x0200cd38
	movs	r0, #5
	movs	r1, #3
	bl 0x0200cdb0
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200cdb0
	movs	r0, #20
	bl 0x0200cd38
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #80]
	adds	r2, #153
	bl 0x0200cd70
	movs	r0, #5
	movs	r1, #2
	bl 0x0200cdb0
	ldr	r0, [r5, #0]
	bl 0x0200cd68
	cmp	r0, #0
	beq.n	.L_020032a8
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200cd80
.L_020032a8:
	movs	r0, #5
	bl 0x0200cd98
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200cda0
	movs	r0, #8
	bl 0x0200cd68
	movs	r1, #9
	bl 0x0200ceb0
	bl 0x0200cd48
.L_020032c8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x000028cf
	.4byte 0x02000240
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #250
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003300
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	ldr	r0, [pc, #76]
	movs	r1, #1
	bl 0x0200cd28
	bl 0x0200cd48
	b.n	.L_0200333e
.L_02003300:
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #107
	bl 0x0200cd90
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #136
	bl 0x0200cd90
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200cd90
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cde8
	movs	r0, #10
	bl 0x0200cd38
	bl 0x0200b0cc
.L_0200333e:
	pop	{r5, pc}
	.4byte 0x000028cf
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #250
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003370
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	ldr	r0, [pc, #68]
	movs	r1, #1
	bl 0x0200cd28
	bl 0x0200cd48
	b.n	.L_020033a6
.L_02003370:
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #140
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200cd90
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200cd90
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cde8
	movs	r0, #10
	bl 0x0200cd38
	bl 0x0200b0cc
.L_020033a6:
	pop	{r5, pc}
	.4byte 0x000028cf
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #250
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_020033d8
	bl 0x0200cd40
	movs	r0, #0
	bl 0x0200ce90
	ldr	r0, [pc, #68]
	movs	r1, #1
	bl 0x0200cd28
	bl 0x0200cd48
	b.n	.L_0200340c
.L_020033d8:
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #248
	movs	r2, #136
	bl 0x0200cd90
	movs	r1, #132
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #136
	bl 0x0200cd90
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cde8
	movs	r0, #10
	bl 0x0200cd38
	bl 0x0200b0cc
.L_0200340c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000028cf
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #92]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #84]
	cmp	r2, r3
	beq.n	.L_02003472
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02003436
	ldr	r0, [pc, #80]
	b.n	.L_02003474
.L_02003436:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02003440
	ldr	r0, [pc, #76]
	b.n	.L_02003474
.L_02003440:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200344a
	ldr	r0, [pc, #76]
	b.n	.L_02003474
.L_0200344a:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02003454
	ldr	r0, [pc, #72]
	b.n	.L_02003474
.L_02003454:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_0200345e
	ldr	r0, [pc, #72]
	b.n	.L_02003474
.L_0200345e:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02003468
	ldr	r0, [pc, #68]
	b.n	.L_02003474
.L_02003468:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02003472
	ldr	r0, [pc, #68]
	b.n	.L_02003474
.L_02003472:
	ldr	r0, [pc, #68]
.L_02003474:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000e2
	.4byte 0x000000e3
	.4byte 0x0200dfe4
	.4byte 0x000000e4
	.4byte 0x0200e0bc
	.4byte 0x000000e5
	.4byte 0x0200e1c4
	.4byte 0x000000e6
	.4byte 0x0200e3a4
	.4byte 0x000000e7
	.4byte 0x0200e458
	.4byte 0x000000e8
	.4byte 0x0200e59c
	.4byte 0x000000e9
	.4byte 0x0200e714
	.2byte 0xdf60
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200cc88
	movs	r0, #0
	movs	r1, #15
	movs	r2, #13
	movs	r3, #0
	bl 0x02008694
	movs	r5, #0
.L_020034d4:
	adds	r0, r5, #0
	adds	r0, #9
	movs	r1, #2
	adds	r5, #1
	bl 0x0200cdb0
	cmp	r5, #6
	bne.n	.L_020034d4
	movs	r0, #15
	movs	r1, #4
	bl 0x0200cdb0
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200cc88
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02003530
	movs	r1, #24
	movs	r2, #15
	movs	r3, #0
	movs	r0, #0
	bl 0x02008694
	movs	r1, #9
	movs	r0, #0
	movs	r2, #16
	movs	r3, #0
	bl 0x02008694
	movs	r0, #11
	movs	r1, #4
	bl 0x0200cdb0
	b.n	.L_0200353c
.L_02003530:
	movs	r0, #0
	movs	r1, #26
	movs	r2, #14
	movs	r3, #0
	bl 0x02008694
.L_0200353c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x00e4
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200cc88
	ldr	r3, [pc, #192]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #184]
	cmp	r2, r3
	bne.n	.L_02003614
	ldr	r6, [pc, #184]
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #2
	adds	r1, r6, #0
	movs	r0, #15
	adds	r2, r5, #0
	bl 0x02008800
	adds	r1, r6, #0
	movs	r0, #16
	adds	r2, r5, #0
	bl 0x02008800
	ldr	r6, [pc, #160]
	movs	r0, #17
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl 0x02008800
	adds	r1, r6, #0
	movs	r0, #18
	adds	r2, r5, #0
	bl 0x02008800
	ldr	r6, [pc, #140]
	movs	r0, #19
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl 0x02008800
	adds	r1, r6, #0
	movs	r0, #20
	adds	r2, r5, #0
	bl 0x02008800
	ldr	r6, [pc, #124]
	movs	r0, #21
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl 0x02008800
	adds	r2, r5, #0
	movs	r0, #22
	adds	r1, r6, #0
	bl 0x02008800
	movs	r0, #16
	bl 0x0200cd68
	movs	r2, #4
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_020035f4
	movs	r0, #16
	bl 0x0200cd68
	movs	r5, #10
	strh	r5, [r0, #4]
	movs	r0, #18
	bl 0x0200cd68
	strh	r5, [r0, #4]
	movs	r0, #20
	bl 0x0200cd68
	strh	r5, [r0, #4]
	movs	r0, #22
	bl 0x0200cd68
	strh	r5, [r0, #4]
.L_020035f4:
	movs	r0, #23
	movs	r1, #4
	bl 0x0200cdb0
	movs	r0, #24
	movs	r1, #4
	bl 0x0200cdb0
	movs	r0, #25
	movs	r1, #4
	bl 0x0200cdb0
	movs	r0, #26
	movs	r1, #2
	bl 0x0200cdb0
.L_02003614:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000e5
	.4byte 0x0200d25c
	.4byte 0x0200d2bc
	.4byte 0x0200d31c
	.2byte 0xd37c
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #64]
	adds	r5, r0, #0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	movs	r1, #14
	ldrsh	r3, [r5, r1]
	movs	r1, #14
	ldrsh	r2, [r0, r1]
	adds	r1, r5, #0
	adds	r1, #35
	cmp	r2, r3
	bne.n	.L_0200365e
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #18
	ldrsh	r3, [r5, r0]
	cmp	r2, r3
	bge.n	.L_02003664
.L_0200365e:
	movs	r3, #1
	strb	r3, [r1, #0]
	b.n	.L_02003670
.L_02003664:
	movs	r3, #0
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x02008038
.L_02003670:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	ldr	r5, [pc, #112]
	str	r2, [r3, #0]
	subs	r2, #36
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #104]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_0200370c
	movs	r2, #133
	lsls	r2, r2, #2
.L_020036a6:
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	movs	r5, #192
	orrs	r3, r2
	lsls	r5, r5, #2
	strb	r3, [r0, #0]
	ldr	r1, [pc, #72]
	movs	r0, #8
	adds	r2, r5, #0
	bl 0x02008800
	adds	r0, r5, #0
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_020036d4
	bl 0x0200bec6
.L_020036d4:
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #71
	movs	r1, #64
	movs	r2, #47
	movs	r3, #98
	bl 0x0200ccd8
	b.n	.L_02003ec6
	.4byte 0x00001000
	.4byte 0x00003f44
	.4byte 0x02000240
	.4byte 0x000000e2
	.2byte 0xd074
	.2byte 0x0200
.L_0200370c:
	ldr	r3, [pc, #460]
	cmp	r2, r3
	bne.n	.L_020037ae
	movs	r0, #0
	bl 0x0200ce60
	movs	r0, #15
	bl 0x0200cd68
	ldr	r3, [pc, #448]
	movs	r5, #0
	str	r3, [r0, #108]
.L_02003724:
	adds	r0, r5, #0
	adds	r0, #9
	bl 0x0200cd68
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #1
	strb	r3, [r2, #0]
	str	r3, [r0, #20]
	str	r3, [r0, #12]
	cmp	r5, #6
	bne.n	.L_02003724
	movs	r0, #159
	lsls	r0, r0, #4
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003756
	movs	r0, #15
	bl 0x0200cd68
	movs	r3, #132
	lsls	r3, r3, #17
	str	r3, [r0, #16]
.L_02003756:
	ldr	r0, [pc, #396]
	bl 0x0200ced8
	ldr	r0, [pc, #392]
	bl 0x02008880
	ldr	r3, [pc, #392]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #16
	bl 0x0200cd68
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r0, #20]
	str	r3, [r0, #12]
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_020037a8
	b.n	.L_02003b24
.L_020037a8:
	bl 0x0200b4bc
	b.n	.L_02003ec6
.L_020037ae:
	ldr	r3, [pc, #320]
	cmp	r2, r3
	bne.n	.L_0200384c
	movs	r0, #0
	bl 0x0200ce60
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #241
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_020037d4
	movs	r0, #11
	bl 0x0200cd68
	movs	r3, #166
	lsls	r3, r3, #18
	str	r3, [r0, #16]
.L_020037d4:
	ldr	r0, [pc, #284]
	bl 0x0200ced8
	bl 0x0200cec0
	movs	r1, #13
	movs	r2, #14
	movs	r0, #0
	bl 0x0200cec8
	ldr	r0, [pc, #252]
	bl 0x02008880
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200cd68
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r0, #20]
	str	r3, [r0, #12]
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r5, #192
	movs	r3, #128
	lsls	r5, r5, #2
	orrs	r3, r2
	adds	r5, #1
	strb	r3, [r0, #0]
	ldr	r1, [pc, #204]
	movs	r0, #10
	adds	r2, r5, #0
	bl 0x02008800
	movs	r0, #12
	ldr	r1, [pc, #196]
	adds	r2, r5, #0
	bl 0x02008800
	adds	r0, r5, #0
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_0200384a
	b.n	.L_02003b24
.L_0200384a:
	b.n	.L_020038aa
.L_0200384c:
	ldr	r3, [pc, #176]
	cmp	r2, r3
	beq.n	.L_02003854
	b.n	.L_02003a84
.L_02003854:
	movs	r0, #0
	bl 0x0200ce60
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [pc, #144]
	bl 0x02008880
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r6, #0
	cmp	r3, #7
	bgt.n	.L_02003910
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #1
	ldr	r1, [pc, #120]
	movs	r0, #10
	adds	r2, r5, #0
	bl 0x02008800
	movs	r0, #12
	ldr	r1, [pc, #112]
	adds	r2, r5, #0
	bl 0x02008800
	adds	r0, r5, #0
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_020038b0
.L_020038aa:
	bl 0x0200b4f0
	b.n	.L_02003ec6
.L_020038b0:
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #71
	movs	r1, #64
	movs	r2, #35
	movs	r3, #82
	bl 0x0200ccd8
	b.n	.L_02003ec6
	.4byte 0x00001000
	.4byte 0x00003f44
	.4byte 0x000000e3
	.4byte 0x0200b631
	.4byte 0x0200d4fc
	.4byte 0x0200d500
	.4byte 0x02000240
	.4byte 0x000000e4
	.4byte 0x0200d508
	.4byte 0x0200d0d4
	.4byte 0x0200d134
	.4byte 0x000000e5
	.4byte 0x0200d50c
	.4byte 0x0200d19c
	.2byte 0xd1fc
	.2byte 0x0200
.L_02003910:
	movs	r0, #23
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #24
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #25
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #3
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #23
	bl 0x0200cdf0
	movs	r0, #24
	movs	r1, #3
	bl 0x0200cdf0
	movs	r0, #25
	movs	r1, #3
	bl 0x0200cdf0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #242
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_0200396e
	movs	r0, #23
	bl 0x0200cd68
	movs	r3, #172
	lsls	r3, r3, #17
	str	r3, [r0, #8]
.L_0200396e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #243
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003988
	movs	r0, #24
	bl 0x0200cd68
	movs	r3, #172
	lsls	r3, r3, #17
	str	r3, [r0, #8]
.L_02003988:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #244
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_020039a2
	movs	r0, #25
	bl 0x0200cd68
	movs	r3, #172
	lsls	r3, r3, #17
	str	r3, [r0, #8]
.L_020039a2:
	ldr	r0, [pc, #412]
	bl 0x0200ced8
	movs	r0, #26
	bl 0x0200cd68
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #26
	bl 0x0200cd68
	adds	r5, r0, #0
	movs	r0, #26
	bl 0x0200cd68
	str	r6, [r0, #20]
	str	r6, [r5, #12]
	movs	r0, #15
	bl 0x0200cd68
	movs	r5, #3
	adds	r0, #92
	strb	r5, [r0, #0]
	movs	r0, #16
	bl 0x0200cd68
	adds	r0, #92
	strb	r5, [r0, #0]
	movs	r0, #17
	bl 0x0200cd68
	adds	r0, #92
	strb	r5, [r0, #0]
	movs	r0, #18
	bl 0x0200cd68
	adds	r0, #92
	strb	r5, [r0, #0]
	movs	r0, #19
	bl 0x0200cd68
	adds	r0, #92
	strb	r5, [r0, #0]
	movs	r0, #20
	bl 0x0200cd68
	adds	r0, #92
	strb	r5, [r0, #0]
	movs	r0, #21
	bl 0x0200cd68
	adds	r0, #92
	strb	r5, [r0, #0]
	movs	r0, #22
	bl 0x0200cd68
	adds	r0, #92
	ldr	r6, [pc, #300]
	strb	r5, [r0, #0]
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #2
	adds	r1, r6, #0
	movs	r0, #15
	adds	r2, r5, #0
	bl 0x02008800
	adds	r1, r6, #0
	movs	r0, #16
	adds	r2, r5, #0
	bl 0x02008800
	ldr	r6, [pc, #276]
	movs	r0, #17
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl 0x02008800
	adds	r1, r6, #0
	movs	r0, #18
	adds	r2, r5, #0
	bl 0x02008800
	ldr	r6, [pc, #256]
	movs	r0, #19
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl 0x02008800
	adds	r1, r6, #0
	movs	r0, #20
	adds	r2, r5, #0
	bl 0x02008800
	ldr	r6, [pc, #240]
	movs	r0, #21
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl 0x02008800
	movs	r0, #22
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl 0x02008800
	adds	r0, r5, #0
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003b24
	bl 0x0200b548
	b.n	.L_02003ec6
.L_02003a84:
	ldr	r3, [pc, #204]
	cmp	r2, r3
	bne.n	.L_02003b58
	movs	r0, #0
	bl 0x0200ce60
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #10
	bl 0x0200cf08
	movs	r1, #1
	movs	r0, #11
	bl 0x0200cf08
	movs	r0, #8
	bl 0x0200cd68
	movs	r1, #2
	mov	r8, r1
	adds	r3, r0, #0
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r6, #160
	lsls	r6, r6, #13
	str	r6, [r0, #20]
	str	r6, [r0, #12]
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #128
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200cd68
	adds	r3, r0, #0
	adds	r3, #85
	mov	r1, r8
	strb	r1, [r3, #0]
	str	r6, [r0, #20]
	str	r6, [r0, #12]
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003b24
	movs	r1, #22
	movs	r2, #26
	movs	r3, #0
	movs	r0, #0
	bl 0x02008694
	movs	r1, #25
	movs	r0, #0
	movs	r2, #26
	movs	r3, #0
	bl 0x02008694
	movs	r0, #64
	movs	r1, #3
	bl 0x0200cdf0
	b.n	.L_02003ec6
.L_02003b24:
	ldr	r3, [pc, #16]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	subs	r2, #2
	strh	r3, [r2, #0]
	b.n	.L_02003ec6
	.2byte 0x0000
	.4byte 0x00001000
	.4byte 0x00003f44
	.4byte 0x0200d520
	.4byte 0x0200d25c
	.4byte 0x0200d2bc
	.4byte 0x0200d31c
	.4byte 0x0200d37c
	.2byte 0x00e6
	.2byte 0x0000
.L_02003b58:
	ldr	r3, [pc, #288]
	cmp	r2, r3
	beq.n	.L_02003b60
	b.n	.L_02003c90
.L_02003b60:
	movs	r0, #0
	bl 0x0200ce60
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #245
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003baa
	movs	r3, #46
	movs	r2, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #46
	movs	r1, #74
	movs	r2, #1
	bl 0x0200cce8
	movs	r0, #9
	bl 0x0200cd68
	movs	r3, #190
	lsls	r3, r3, #18
	str	r3, [r0, #8]
.L_02003baa:
	movs	r0, #144
.L_02003bac:
	lsls	r0, r0, #4
	adds	r0, #246
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003bc6
.L_02003bb8:
	movs	r1, #230
	movs	r2, #154
	movs	r0, #12
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200cda0
.L_02003bc6:
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #2
	ldr	r0, [pc, #176]
.L_02003bce:
	bl 0x0200ced8
	ldr	r1, [pc, #176]
	movs	r0, #8
	adds	r2, r5, #0
	bl 0x02008800
	ldr	r1, [pc, #168]
	movs	r0, #10
	adds	r2, r5, #0
	bl 0x02008800
	movs	r0, #11
	ldr	r1, [pc, #160]
	adds	r2, r5, #0
	bl 0x02008800
	adds	r0, r5, #0
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003c4e
	movs	r0, #9
	movs	r1, #4
	bl 0x0200cdb0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #246
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003c18
	movs	r0, #12
	movs	r1, #4
	bl 0x0200cdb0
.L_02003c18:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #245
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003c34
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r0, #2
	movs	r1, #46
	movs	r2, #8
	bl 0x02008694
.L_02003c34:
	movs	r1, #10
	movs	r2, #45
	movs	r3, #0
	movs	r0, #0
	bl 0x02008694
	movs	r0, #0
	movs	r1, #53
	movs	r2, #40
	movs	r3, #0
	bl 0x02008694
	b.n	.L_02003ec6
.L_02003c4e:
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #71
	movs	r1, #64
	movs	r2, #50
	movs	r3, #102
	bl 0x0200ccd8
	b.n	.L_02003ec6
	.2byte 0x0000
	.4byte 0x00001000
	.4byte 0x00003f44
	.4byte 0x000000e7
	.4byte 0x0200d528
	.4byte 0x0200d3dc
	.4byte 0x0200d43c
	.2byte 0xd49c
	.2byte 0x0200
.L_02003c90:
	ldr	r3, [pc, #572]
	cmp	r2, r3
	beq.n	.L_02003c98
	b.n	.L_02003dd4
.L_02003c98:
	movs	r6, #0
.L_02003c9a:
	adds	r0, r6, #0
	adds	r0, #8
	bl 0x0200cd68
	movs	r1, #4
	adds	r5, r0, #0
	ldr	r7, [pc, #556]
	bl 0x0200cc98
	movs	r1, #0
	adds	r3, r5, #0
	mov	r8, r1
	adds	r3, #85
	mov	r2, r8
	adds	r6, #1
	strb	r2, [r3, #0]
	str	r7, [r5, #20]
	str	r7, [r5, #12]
	cmp	r6, #3
	bne.n	.L_02003c9a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #247
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003ce6
	movs	r0, #23
	movs	r1, #4
	bl 0x0200cdb0
	movs	r1, #174
	movs	r2, #162
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200cda0
.L_02003ce6:
	ldr	r0, [pc, #496]
	bl 0x0200ced8
	movs	r0, #0
	bl 0x0200ce60
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02003d0a
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
.L_02003d0a:
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02003d20
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
.L_02003d20:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003d62
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003d62
	movs	r0, #11
	bl 0x0200cd68
	mov	r3, r8
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200cd68
	adds	r5, r0, #0
	movs	r0, #11
	bl 0x0200cd68
	movs	r1, #4
	str	r7, [r0, #20]
	str	r7, [r5, #12]
	movs	r0, #11
	bl 0x0200cdb0
	b.n	.L_02003d70
.L_02003d62:
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r0, #0
	movs	r1, #38
	movs	r2, #18
	bl 0x02008694
.L_02003d70:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #249
.L_02003d76:
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003d86
	movs	r0, #12
.L_02003d80:
	movs	r1, #0
	bl 0x0200cdb0
.L_02003d86:
	movs	r6, #0
.L_02003d88:
	adds	r0, r6, #0
	adds	r0, #13
.L_02003d8c:
	movs	r1, #2
	adds	r6, #1
	bl 0x0200cdb0
	cmp	r6, #4
.L_02003d96:
	bne.n	.L_02003d88
	movs	r6, #0
.L_02003d9a:
	adds	r0, r6, #0
	adds	r0, #12
	bl 0x0200cd68
.L_02003da2:
	adds	r6, #1
	movs	r1, #0
	bl 0x0200cd00
	cmp	r6, #11
.L_02003dac:
	bne.n	.L_02003d9a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #248
	bl 0x0200cc80
.L_02003db8:
	cmp	r0, #0
	beq.n	.L_02003dbe
	b.n	.L_02003ec6
.L_02003dbe:
	movs	r6, #0
.L_02003dc0:
	adds	r0, r6, #0
.L_02003dc2:
	adds	r0, #12
	movs	r1, #0
	movs	r2, #0
	adds	r6, #1
	bl 0x0200cda0
	cmp	r6, #9
	bne.n	.L_02003dc0
	b.n	.L_02003ec6
.L_02003dd4:
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #252]
	cmp	r2, r3
	bne.n	.L_02003ec6
	bl 0x0200a55c
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200cc88
	movs	r6, #0
.L_02003df2:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #2
	adds	r0, r6, r2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003e2a
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
	bl 0x0200ccd8
	movs	r3, #255
	movs	r0, #0
	movs	r1, #16
	adds	r2, r5, #0
	lsls	r3, r3, #8
	bl 0x02008694
.L_02003e2a:
	adds	r6, #1
	cmp	r6, #6
	bne.n	.L_02003df2
	movs	r0, #10
	bl 0x0200cd68
	movs	r1, #0
	bl 0x0200cd00
	movs	r0, #11
	bl 0x0200cd68
	movs	r1, #0
	bl 0x0200cd00
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003e80
	movs	r0, #9
	bl 0x0200cd68
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200cd68
	adds	r5, r0, #0
	movs	r0, #9
	bl 0x0200cd68
	ldr	r3, [pc, #96]
	movs	r1, #4
	str	r3, [r0, #20]
	str	r3, [r5, #12]
	movs	r0, #9
	bl 0x0200cdb0
	b.n	.L_02003e98
.L_02003e80:
	movs	r3, #255
	movs	r1, #7
	movs	r2, #21
	lsls	r3, r3, #8
	movs	r0, #0
	bl 0x02008694
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
.L_02003e98:
	movs	r0, #8
	bl 0x0200cd68
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200cd68
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200cd68
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r0, #20]
	str	r3, [r5, #12]
	movs	r0, #8
	bl 0x0200cd68
	movs	r1, #9
	bl 0x0200ceb0
.L_02003ec6:
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x000000e8
	.4byte 0xffe00000
	.4byte 0x0200d52e
	.2byte 0x00e9
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r2, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	movs	r1, #241
	lsls	r1, r1, #1
	movs	r0, #10
	adds	r3, r2, r1
	adds	r0, #255
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02003f34
	ldr	r3, [pc, #52]
	cmp	r6, r3
	bne.n	.L_02003f0e
	cmp	r5, #8
	beq.n	.L_02003f18
.L_02003f0e:
	ldr	r3, [pc, #48]
	cmp	r6, r3
	bne.n	.L_02003f34
	cmp	r5, #11
	bne.n	.L_02003f34
.L_02003f18:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200cc90
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200cc90
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200cc90
.L_02003f34:
	movs	r0, #0
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x000000e3
	.2byte 0x00e8
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
	sub	sp, #20
	str	r3, [sp, #16]
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	mov	fp, r0
	cmp	r3, r2
	beq.n	.L_0200402a
.L_02003f6a:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	str	r3, [sp, #12]
	bl 0x0200cd68
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r7, r0, #0
	str	r2, [sp, #8]
	movs	r3, #34
	adds	r3, r3, r7
	adds	r0, r2, #0
	ldrb	r2, [r3, #0]
	mov	r9, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #16]
	adds	r0, #1
	ldr	r5, [r2, r3]
	ldr	r2, [pc, #196]
	adds	r3, r5, r2
	ldr	r2, [pc, #196]
	asrs	r3, r3, #2
	adds	r6, r3, r2
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02003fb8
	ldr	r0, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
	b.n	.L_02004018
.L_02003fb8:
	adds	r0, r7, #0
	bl 0x0200ced0
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r5, r5, r3
	str	r5, [sp, #4]
	mov	r2, r9
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl 0x0200cd20
	mov	r3, r9
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	sl, r0
	ldrb	r0, [r3, #0]
	bl 0x0200cce0
	adds	r5, r0, #0
	ldr	r0, [sp, #12]
	bl 0x0200cdf8
	ldr	r2, [sp, #4]
	movs	r3, #128
	asrs	r5, r5, #19
	strb	r3, [r2, #3]
	adds	r5, #4
	mov	r3, r9
	adds	r2, r5, #0
	ldrb	r0, [r3, #0]
	mov	r1, sl
	bl 0x0200cef0
	add	r8, r6
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_02004018
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc98
.L_02004018:
	movs	r3, #4
	add	fp, r3
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02003f6a
.L_0200402a:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
.L_02004032:
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r5, r0, #0
	adds	r3, r5, #0
.L_0200403c:
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200cce0
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_02004052
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_02004052:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0x0240
	.2byte 0x0200
.L_0200406c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	adds	r5, r0, #0
	bl 0x0200ce38
	cmp	r0, #0
	beq.n	.L_02004082
	b.n	.L_020041f2
.L_02004082:
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
.L_02004094:
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
.L_0200409e:
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	bl 0x0200cef8
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_020040ae
	b.n	.L_020041f2
.L_020040ae:
	b.n	.L_020041e4
.L_020040b0:
	ldrh	r7, [r5, #0]
	adds	r0, r7, #0
	bl 0x0200cd68
	cmp	r0, r8
	beq.n	.L_020040c0
	adds	r5, #4
	b.n	.L_020041e4
.L_020040c0:
	ldrh	r5, [r5, #2]
	bl 0x0200cd40
	adds	r0, r5, #0
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_0200412e
	movs	r0, #125
	bl 0x0200cf30
	adds	r0, r7, #0
	bl 0x0200cd68
	movs	r1, #7
	bl 0x0200cdc8
	movs	r0, #2
	bl 0x0200cc10
	movs	r1, #0
	mov	r0, r8
	bl 0x0200cc98
	adds	r0, r7, #0
	bl 0x0200cd68
	movs	r1, #0
	bl 0x0200cdc8
	movs	r0, #2
	bl 0x0200cc10
	adds	r0, r7, #0
	bl 0x0200cd68
	movs	r1, #7
	bl 0x0200cdc8
	movs	r0, #4
	bl 0x0200cc10
	adds	r0, r7, #0
	bl 0x0200cd68
	movs	r1, #0
	bl 0x0200cdc8
	movs	r0, #0
	bl 0x0200c6e8
	adds	r0, r5, #0
	bl 0x0200cc88
	b.n	.L_020041de
.L_0200412e:
	adds	r5, #1
	mov	sl, r5
	mov	r0, sl
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_020041de
	adds	r6, #85
	strb	r0, [r6, #0]
	movs	r0, #185
	bl 0x0200cf30
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200cd10
	movs	r0, #0
	bl 0x0200c6e8
	movs	r5, #2
	movs	r0, #8
	mov	r7, r8
	bl 0x0200cc10
	negs	r5, r5
	mov	r0, r8
	movs	r1, #2
	adds	r7, #34
	bl 0x0200cc98
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x0200c9c0
	movs	r0, #1
	bl 0x0200c6e8
	movs	r0, #16
	bl 0x0200cc10
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x0200c9c0
	movs	r0, #4
	bl 0x0200cc10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
.L_020041a0:
	negs	r0, r0
	bl 0x0200cd10
	movs	r0, #8
	bl 0x0200cc10
.L_020041ac:
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #5
	bl 0x0200cc10
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	mov	r0, r8
	bl 0x0200ccb8
	movs	r0, #2
	bl 0x0200cc10
	movs	r0, #188
	bl 0x0200cf30
	bl 0x0200c81c
	movs	r0, #20
	bl 0x0200cc10
	mov	r0, sl
	bl 0x0200cc88
.L_020041de:
	bl 0x0200cd48
	b.n	.L_020041f2
.L_020041e4:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_020041f2
	b.n	.L_020040b0
.L_020041f2:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200cd68
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r7, r0, #0
	cmp	r3, r2
	beq.n	.L_020042da
.L_0200421e:
	ldrh	r3, [r5, #0]
	cmp	r3, r6
	beq.n	.L_02004228
	adds	r5, #4
.L_02004226:
	b.n	.L_020042ce
.L_02004228:
	ldrh	r5, [r5, #2]
	bl 0x0200cd40
	adds	r3, r5, #1
.L_02004230:
	mov	r8, r3
	mov	r0, r8
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_020042c8
.L_0200423c:
	movs	r0, #185
	bl 0x0200cf30
	movs	r0, #128
	movs	r1, #128
.L_02004246:
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200cd10
.L_02004252:
	movs	r0, #0
	bl 0x0200c6e8
	movs	r0, #8
	bl 0x0200cc10
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200cc98
	adds	r3, r7, #0
.L_02004268:
	adds	r3, #34
	movs	r0, #4
	ldrb	r1, [r3, #0]
	adds	r2, r6, #0
	negs	r0, r0
.L_02004272:
	bl 0x0200c934
	movs	r0, #1
	bl 0x0200c6e8
	movs	r0, #16
	bl 0x0200cc10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200cd10
	movs	r0, #8
	bl 0x0200cc10
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200ccb8
	movs	r0, #2
	bl 0x0200cc10
	movs	r0, #188
	bl 0x0200cf30
	bl 0x0200c81c
	movs	r0, #20
	bl 0x0200cc10
	adds	r0, r5, #0
	bl 0x0200cc88
	mov	r0, r8
	bl 0x0200cc88
.L_020042c8:
	bl 0x0200cd48
	b.n	.L_020042da
.L_020042ce:
	movs	r2, #255
	ldrh	r3, [r5, #0]
.L_020042d2:
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200421e
.L_020042da:
	pop	{r3}
	mov	r8, r3
.L_020042de:
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200cd68
	movs	r3, #3
	adds	r0, #92
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200cdf8
	pop	{r5, pc}
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
	sub	sp, #16
	ldr	r5, [pc, #324]
	str	r3, [sp, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	fp, r0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200cda8
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200cda8
	ldr	r1, [r5, #0]
	movs	r0, #10
	bl 0x0200cda8
	movs	r0, #1
	bl 0x0200cc10
	movs	r0, #8
	bl 0x0200c2e0
	movs	r0, #9
	bl 0x0200c2e0
	movs	r0, #10
	bl 0x0200c2e0
	movs	r1, #0
	movs	r0, #9
	bl 0x0200cdb0
	movs	r0, #1
	bl 0x0200cc10
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200cda0
	movs	r0, #1
	bl 0x0200cc10
	b.n	.L_02004432
.L_0200437e:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	mov	r9, r3
	mov	r0, r9
	bl 0x0200cd68
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r5, r0, #0
	str	r2, [sp, #8]
	adds	r7, r5, #0
	adds	r7, #34
	adds	r0, r2, #0
	ldrb	r2, [r7, #0]
	adds	r0, #1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	ldr	r6, [r2, r3]
	ldr	r2, [pc, #168]
	adds	r3, r6, r2
	ldr	r2, [pc, #168]
	asrs	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #3
	strb	r3, [r2, #0]
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_020043d4
	mov	r0, r9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cda0
	b.n	.L_0200442e
.L_020043d4:
	adds	r0, r5, #0
	bl 0x0200ced0
.L_020043da:
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r6, r6, r3
	str	r6, [sp, #4]
.L_020043e4:
	add	r8, sl
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r7, #0]
	bl 0x0200cd20
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	mov	sl, r0
	ldrb	r0, [r7, #0]
	bl 0x0200cce0
	adds	r5, r0, #0
	mov	r0, r9
	bl 0x0200cdf8
	ldr	r6, [sp, #4]
	asrs	r5, r5, #19
	movs	r3, #128
	adds	r5, #4
	adds	r2, r5, #0
	strb	r3, [r6, #3]
	ldrb	r0, [r7, #0]
	mov	r1, sl
	bl 0x0200cef0
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200cc80
	cmp	r0, #0
	beq.n	.L_0200442e
	mov	r0, r9
	movs	r1, #9
	bl 0x0200ce00
.L_0200442e:
	movs	r3, #4
	add	fp, r3
.L_02004432:
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200437e
	movs	r0, #10
	bl 0x0200cc10
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, lr}
	adds	r5, r1, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	bl 0x0200ccb8
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200ccb8
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl 0x0200ce38
	cmp	r0, #0
	beq.n	.L_02004494
	b.n	.L_02004656
.L_02004494:
	ldr	r3, [pc, #460]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200cd68
	ldr	r3, [r5, #8]
	adds	r7, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
.L_020044bc:
	bl 0x0200cef8
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_020044c8
	b.n	.L_02004656
.L_020044c8:
	b.n	.L_02004648
.L_020044ca:
	ldrh	r3, [r6, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200cd68
	cmp	r0, sl
	beq.n	.L_020044dc
	adds	r6, #4
	b.n	.L_02004648
.L_020044dc:
	ldrh	r6, [r6, #2]
	bl 0x0200cd40
	adds	r0, r6, #0
	bl 0x0200cc80
	cmp	r0, #0
	bne.n	.L_02004576
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200cc98
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200c460
	movs	r0, #1
	bl 0x0200cc10
	movs	r0, #125
	bl 0x0200cf30
	movs	r0, #8
	bl 0x0200cd68
	movs	r1, #7
	bl 0x0200cdc8
	movs	r0, #2
	bl 0x0200cc10
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc98
	movs	r1, #9
	mov	r0, r8
	bl 0x0200ce00
	movs	r0, #8
	bl 0x0200cd68
	movs	r1, #0
	bl 0x0200cdc8
	movs	r0, #2
	bl 0x0200cc10
	movs	r0, #8
	bl 0x0200cd68
	movs	r1, #7
	bl 0x0200cdc8
	movs	r0, #4
	bl 0x0200cc10
	movs	r0, #8
	bl 0x0200cd68
	movs	r1, #0
	bl 0x0200cdc8
	movs	r0, #0
	bl 0x0200c6e8
	mov	r0, sl
	adds	r1, r7, #0
	bl 0x0200c460
	movs	r0, #1
	bl 0x0200cc10
	adds	r0, r6, #0
	bl 0x0200cc88
	b.n	.L_02004642
.L_02004576:
	adds	r6, #1
	mov	r9, r6
	mov	r0, r9
	bl 0x0200cc80
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02004642
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200cc98
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200c460
	adds	r5, #85
	movs	r0, #1
	bl 0x0200cc10
	strb	r6, [r5, #0]
	movs	r0, #185
	bl 0x0200cf30
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200cd10
	movs	r0, #0
	bl 0x0200c6e8
	mov	r8, r5
	movs	r0, #8
	movs	r6, #2
	mov	r5, sl
	bl 0x0200cc10
	negs	r6, r6
	adds	r0, r7, #0
	movs	r1, #2
	adds	r5, #34
	bl 0x0200cc98
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200c9c0
	movs	r0, #1
	bl 0x0200c6e8
	movs	r0, #16
	bl 0x0200cc10
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200c9c0
	movs	r0, #4
	bl 0x0200cc10
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200cd10
	movs	r0, #8
	bl 0x0200cc10
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r0, #5
	bl 0x0200cc10
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200ccb8
	movs	r0, #2
	bl 0x0200cc10
	movs	r0, #188
	bl 0x0200cf30
	bl 0x0200c81c
	movs	r0, #20
	bl 0x0200cc10
	mov	r0, r9
	bl 0x0200cc88
.L_02004642:
	bl 0x0200cd48
	b.n	.L_02004656
.L_02004648:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02004656
	b.n	.L_020044ca
.L_02004656:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
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
	bl 0x0200cc00
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02004698
	adds	r3, #15
.L_02004698:
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
	bl 0x0200cd68
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
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #256]
	mov	r8, r0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	movs	r2, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r2
.L_0200470a:
	bl 0x0200cc28
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r2, [r7, #12]
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	subs	r2, r2, r3
	mov	r3, sl
	lsls	r1, r3, #17
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #212]
	adds	r1, r1, r3
	ldr	r3, [pc, #212]
	adds	r1, r1, r0
	movs	r0, #30
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r7, #16]
	bl 0x0200cca8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020047de
	mov	r1, r9
	ldr	r0, [r6, #80]
	bl 0x0200ce98
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r6, #0
	bl 0x0200cd00
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200cc98
	adds	r0, r6, #0
	ldr	r1, [pc, #152]
	bl 0x0200cca0
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	ldr	r1, [r6, #80]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	mov	r2, r8
	strb	r3, [r1, #9]
	cmp	r2, #0
	beq.n	.L_020047a8
	mov	r3, sl
	lsls	r5, r3, #13
	adds	r0, r5, #0
	bl 0x0200cc38
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6470
	adds	r0, r5, #0
	bl 0x0200cc30
	b.n	.L_020047ac
.L_020047a8:
	mov	r0, r8
	str	r0, [r6, #68]
.L_020047ac:
	str	r0, [r6, #76]
	bl 0x0200cc28
	movs	r2, #192
	lsls	r0, r0, #14
	lsls	r2, r2, #7
	lsrs	r0, r0, #16
	adds	r0, r0, r2
	negs	r0, r0
	str	r0, [r6, #72]
	bl 0x0200cc28
	ldr	r3, [pc, #68]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r6, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [pc, #60]
	str	r3, [r6, #48]
	ldr	r3, [pc, #60]
	str	r3, [r6, #52]
	ldr	r3, [pc, #60]
	str	r3, [r6, #108]
.L_020047de:
	movs	r0, #1
	add	sl, r0
	mov	r2, sl
	cmp	r2, #7
	bls.n	.L_0200470a
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0xfffe0000
	.4byte 0x0200d638
	.4byte 0x0300021c
	.4byte 0x00013333
	.4byte 0xffffff00
	.4byte 0xfffff800
	.4byte 0xfffffa00
	.2byte 0xc669
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #244]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200cd68
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02004910
	movs	r3, #0
	mov	r9, r3
	mov	sl, r3
.L_02004840:
	movs	r0, #30
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, #255
	bl 0x0200cca8
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02004906
	mov	r1, r9
	ldr	r0, [r7, #80]
	bl 0x0200ce98
	movs	r4, #0
	mov	r8, r4
	adds	r3, r7, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, #4
	strb	r2, [r3, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r7, #0
	bl 0x0200cd00
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200cc98
	ldr	r1, [pc, #160]
	adds	r0, r7, #0
	bl 0x0200cca0
	mov	r3, sl
	lsls	r5, r3, #12
	adds	r0, r5, #0
	bl 0x0200cc38
	mov	r4, r8
	str	r4, [r7, #72]
	str	r0, [r7, #68]
	adds	r0, r5, #0
	bl 0x0200cc30
	ldr	r3, [r7, #68]
	str	r0, [r7, #76]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r7, #68]
	bl 0x0200cc28
	lsls	r3, r0, #1
	ldr	r2, [r7, #68]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #108]
	adds	r2, r2, r3
	str	r2, [r7, #68]
	bl 0x0200cc28
	lsls	r3, r0, #1
	ldr	r2, [r7, #76]
	adds	r3, r3, r0
	ldr	r4, [pc, #96]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r7, #76]
	bl 0x0200cc28
	ldr	r2, [pc, #84]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	adds	r3, r7, #0
	adds	r0, r0, r2
	adds	r3, #100
	strh	r0, [r3, #0]
	mov	r3, r8
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	ldr	r3, [pc, #68]
	ldr	r0, [r7, #80]
	str	r3, [r7, #108]
	ldr	r3, [r6, #80]
	movs	r1, #12
	ldrb	r3, [r3, #9]
	movs	r4, #13
	ands	r1, r3
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #9]
.L_02004906:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02004840
.L_02004910:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200d668
	.4byte 0xffffa000
	.4byte 0xffffd000
	.4byte 0xfffff800
	.2byte 0xc669
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	adds	r0, r2, #0
	adds	r5, r1, #0
	bl 0x0200cd68
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldr	r2, [pc, #88]
	adds	r7, r0, #0
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	adds	r5, r7, #0
	asrs	r3, r3, #2
	adds	r5, #34
	adds	r6, r3, r1
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200cd20
	ldr	r2, [r7, #16]
	mov	r8, r0
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200cce0
	ldr	r3, [r7, #8]
	asrs	r2, r0, #19
	add	r2, sl
	cmp	r3, #0
	bge.n	.L_0200498e
	ldr	r1, [pc, #48]
	adds	r3, r3, r1
.L_0200498e:
	ldr	r0, [r7, #16]
	asrs	r1, r3, #20
	cmp	r0, #0
	bge.n	.L_0200499a
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L_0200499a:
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r0, [r5, #0]
	mov	r1, r8
	adds	r6, r6, r3
	bl 0x0200cef0
	strb	r0, [r6, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0xffff
	.2byte 0x000f
	push	{lr}
	ldr	r3, [pc, #16]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	bl 0x0200c934
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r3, #192
	adds	r5, r7, r2
	lsls	r3, r3, #4
	movs	r2, #63
	adds	r6, r7, r3
	mov	r8, r2
.L_020049f6:
	ldr	r3, [r5, #24]
	cmp	r3, #19
	bhi.n	.L_02004a44
	movs	r2, #176
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	ldrh	r1, [r1, #0]
	movs	r2, #7
	asrs	r3, r3, #2
	ands	r3, r2
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r6, #8]
	adds	r0, r6, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	adds	r1, r5, #0
	bl 0x0200cf18
	adds	r0, r5, #0
	movs	r1, #63
	ldr	r2, [pc, #20]
	bl 0x0200cf20
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	b.n	.L_02004a44
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x8000
	.2byte 0xffff
.L_02004a44:
	.2byte 0x2301
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r2, #0
	bge.n	.L_020049f6
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
	sub	sp, #12
	str	r2, [sp, #0]
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	adds	r2, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r2, #0
	ble.n	.L_02004b08
	adds	r7, r2, #0
.L_02004a84:
	bl 0x0200cc28
	movs	r1, #176
	lsls	r1, r1, #5
	add	r1, fp
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	sl, r1
	lsls	r6, r3, #3
	subs	r6, r6, r3
	lsls	r6, r6, #2
	movs	r3, #160
	add	r6, fp
	lsls	r3, r3, #3
	adds	r5, r6, r3
	movs	r1, #0
	str	r1, [r5, #24]
	ldr	r2, [sp, #8]
	mov	r8, r1
	str	r2, [r5, #0]
	ldr	r3, [sp, #4]
	mov	r9, r0
	str	r3, [r5, #4]
	ldr	r1, [sp, #0]
	subs	r7, #1
	str	r1, [r5, #8]
	bl 0x0200cc28
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r0, r0, #3
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r5, #0
	bl 0x0200cc40
	mov	r3, r8
	str	r3, [r5, #12]
	movs	r3, #160
	lsls	r3, r3, #11
	mov	r1, r8
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	bl 0x0200cc28
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #12
	movs	r2, #128
	adds	r6, r6, r3
	lsls	r2, r2, #10
	lsls	r0, r0, #1
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r6, #0
	bl 0x0200cc40
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #63
	adds	r3, #1
	ands	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	cmp	r7, #0
	bne.n	.L_02004a84
.L_02004b08:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #8
	movs	r0, #220
	sub	sp, #4
	bl 0x0200cc48
	adds	r6, r0, #0
	ldr	r0, [pc, #152]
	bl 0x0200cc78
	adds	r1, r6, #0
	bl 0x0200cc58
	bl 0x0200cc70
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r2, r6, #0
	adds	r5, r0, #0
	bl 0x0200cc68
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #2
	mov	sl, r0
	adds	r3, r6, r1
	mov	r2, sl
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r2, #160
	movs	r3, #192
	lsls	r2, r2, #3
	lsls	r3, r3, #4
	movs	r1, #63
	adds	r7, r6, r2
	adds	r5, r6, r3
	mov	r8, r1
.L_02004b70:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl 0x0200cf10
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
	movs	r3, #15
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	subs	r3, #241
	add	r8, r3
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r5, #40
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_02004b70
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r2, r6, r1
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200cc18
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001f0
	.2byte 0xc9d9
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl 0x0200cc20
	movs	r3, #176
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl 0x0200cc60
	movs	r0, #220
	bl 0x0200cc50
	pop	{r5, pc}
	.4byte 0x0200c9d9
	.irp EntryTarget, 0x03000528, 0x03000514, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000129, 0x08000141, 0x08000151, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x08000291, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020179, 0x080201c1, 0x080201e9, 0x080201f1, 0x08020211, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x080202f1, 0x08038041, 0x080ad039, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8061, 0x080c8071, 0x080c8081, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80c1, 0x080c80d1, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8111, 0x080c8119, 0x080c8161, 0x080c8169, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c8209, 0x080c8221, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c8289, 0x080c82e1, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83e1, 0x080c8481, 0x080c8499, 0x080c84a1, 0x080c84a9, 0x080c84b1, 0x080c84b9, 0x080c84e1, 0x080c8519, 0x080c8571, 0x080c8581, 0x080c8681, 0x080c8691, 0x080c86d1, 0x080c86d9, 0x080c8711, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c8731, 0x080c8739, 0x080c8779, 0x080c8781, 0x080c87c1, 0x080c87d1, 0x080c87e1, 0x080c88c9, 0x081c0011
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
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000012
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x03080000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02e80000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02e80000
	.4byte 0x00100000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03080000
	.4byte 0x00100000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00100000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00100000
	.4byte 0x02380000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x02580000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00100000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00100000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00100000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00100000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00100000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01f80000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01f80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02580000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02580000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00100000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0xffff000f
	.4byte 0x00320008
	.4byte 0xffffffff
	.4byte 0xffff000b
	.4byte 0x00320008
	.4byte 0x000dffff
	.4byte 0xffff0033
	.4byte 0x0034000e
	.4byte 0xffffffff
	.4byte 0x00180017
	.4byte 0xffff0019
	.4byte 0x000c0009
	.4byte 0x0017ffff
	.4byte 0x0000ffff
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
	.4byte 0x0200aa6d
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200ab2d
	.4byte 0x0000002e
	.4byte 0x0200abd1
	.4byte 0x0000002e
	.4byte 0x0200ac1d
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
	.4byte 0x00000021
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
	.4byte 0x0200cf38
	.4byte 0x0200cf74
	.4byte 0x0200cfb0
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
	.4byte 0x000000e2
	.4byte 0x001040e2
	.4byte 0x002010e3
	.4byte 0x003050e3
	.4byte 0x004010e2
	.4byte 0x005020e3
	.4byte 0x006030e3
	.4byte 0x007040e3
	.4byte 0x008010e4
	.4byte 0x000000e3
	.4byte 0x001020e2
	.4byte 0x002050e2
	.4byte 0x003060e2
	.4byte 0x004070e2
	.4byte 0x005030e2
	.4byte 0x006070e3
	.4byte 0x007060e3
	.4byte 0x008030e1
	.4byte 0x000000e4
	.4byte 0x001080e2
	.4byte 0x002060e4
	.4byte 0x003010e5
	.4byte 0x004020e5
	.4byte 0x005030e5
	.4byte 0x006020e4
	.4byte 0x007040e5
	.4byte 0x008090e4
	.4byte 0x009080e4
	.4byte 0x00a050e5
	.4byte 0x00b060e5
	.4byte 0x000000e5
	.4byte 0x001030e4
	.4byte 0x002040e4
	.4byte 0x003050e4
	.4byte 0x004070e4
	.4byte 0x0050a0e4
	.4byte 0x0060b0e4
	.4byte 0x007070e7
	.4byte 0x008070e6
	.4byte 0x009080e6
	.4byte 0x00a010e6
	.4byte 0x00b010e7
	.4byte 0x00c020e7
	.4byte 0x00d050e7
	.4byte 0x00e060e7
	.4byte 0x000000e6
	.4byte 0x0010a0e5
	.4byte 0x002030e7
	.4byte 0x003090e6
	.4byte 0x0040a0e6
	.4byte 0x0050b0e7
	.4byte 0x006040e7
	.4byte 0x007080e5
	.4byte 0x008090e5
	.4byte 0x009030e6
	.4byte 0x00a040e6
	.4byte 0x000000e7
	.4byte 0x0010b0e5
	.4byte 0x0020c0e5
	.4byte 0x003020e6
	.4byte 0x004060e6
	.4byte 0x0050d0e5
	.4byte 0x0060e0e5
	.4byte 0x007070e5
	.4byte 0x0080e0e7
	.4byte 0x009100e7
	.4byte 0x00a110e7
	.4byte 0x00b050e6
	.4byte 0x00c120e7
	.4byte 0x00d130e7
	.4byte 0x00e080e7
	.4byte 0x00f0a0e8
	.4byte 0x010090e7
	.4byte 0x0110a0e7
	.4byte 0x0120c0e7
	.4byte 0x0130d0e7
	.4byte 0x000000e8
	.4byte 0x001010e9
	.4byte 0x002050e8
	.4byte 0x003060e8
	.4byte 0x004070e8
	.4byte 0x005020e8
	.4byte 0x006030e8
	.4byte 0x007040e8
	.4byte 0x008090e8
	.4byte 0x009080e8
	.4byte 0x00a0f0e7
	.4byte 0x00b020df
	.4byte 0x000000e9
	.4byte 0x001010e8
	.4byte 0x000001ff
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x006b00f5
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0x006800f5
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0x03500194
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x03510194
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000012
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000013
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0145
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0145
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0145
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0145
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
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
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0005
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
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte 0x020091fd
	.4byte 0x00000202
	.4byte 0xffff004e
	.4byte 0x020091fd
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
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x02009829
	.4byte 0x0000c400
	.4byte 0xffff0011
	.4byte 0x020085a1
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200899d
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a31
	.4byte 0x10008c15
	.4byte 0x09f0000f
	.4byte 0x0200899d
	.4byte 0x00008c15
	.4byte 0x09f0000f
	.4byte 0x02008a31
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte 0x02008e15
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x02008e21
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte 0x02009149
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
	.4byte 0x00000001
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
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x02009829
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte 0x020091fd
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008595
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200899d
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a31
	.4byte 0x10008c15
	.4byte 0x09f1000b
	.4byte 0x0200899d
	.4byte 0x00008c15
	.4byte 0x09f1000b
	.4byte 0x02008a31
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte 0x02008e15
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x02008e21
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte 0x02009149
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
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte 0x020091fd
	.4byte 0x00000202
	.4byte 0x13010047
	.4byte 0x020091fd
	.4byte 0x00000202
	.4byte 0xffff0048
	.4byte 0x020091fd
	.4byte 0x0000c602
	.4byte 0xffff0049
	.4byte 0x020091fd
	.4byte 0x00008602
	.4byte 0xffff004b
	.4byte 0x020091fd
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200899d
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a31
	.4byte 0x10008c15
	.4byte 0x09f20017
	.4byte 0x0200899d
	.4byte 0x00008c15
	.4byte 0x09f20017
	.4byte 0x02008a31
	.4byte 0x10008c15
	.4byte 0x09f30018
	.4byte 0x0200899d
	.4byte 0x00008c15
	.4byte 0x09f30018
	.4byte 0x02008a31
	.4byte 0x10008c15
	.4byte 0x09f40019
	.4byte 0x0200899d
	.4byte 0x00008c15
	.4byte 0x09f40019
	.4byte 0x02008a31
	.4byte 0x50009705
	.4byte 0x03500047
	.4byte 0x02009ca1
	.4byte 0x50009705
	.4byte 0x03510048
	.4byte 0x02009cd1
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte 0x02008e15
	.4byte 0x50009705
	.4byte 0x02f00032
	.4byte 0x02008e21
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x02009ca1
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte 0x02009149
	.4byte 0x80009705
	.4byte 0xffff0033
	.4byte 0x02008e15
	.4byte 0x50009705
	.4byte 0xffff0033
	.4byte 0x02008e21
	.4byte 0x00009705
	.4byte 0xffff0033
	.4byte 0x02009149
	.4byte 0x80009705
	.4byte 0xffff0034
	.4byte 0x02008e15
	.4byte 0x50009705
	.4byte 0xffff0034
	.4byte 0x02008e21
	.4byte 0x00009705
	.4byte 0xffff0034
	.4byte 0x02009149
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
	.4byte 0x00000001
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
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x02009829
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02009829
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x00000000
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
	.4byte 0x00000031
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
	.4byte 0x00000202
	.4byte 0x13020046
	.4byte 0x020091fd
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200899d
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a31
	.4byte 0x10008c15
	.4byte 0x09f50009
	.4byte 0x0200899d
	.4byte 0x00008c15
	.4byte 0x09f50009
	.4byte 0x02008a31
	.4byte 0x10008c15
	.4byte 0x09f6000c
	.4byte 0x0200899d
	.4byte 0x00008c15
	.4byte 0x09f6000c
	.4byte 0x02008a31
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200a3e9
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
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000002
	.4byte 0x09f8001e
	.4byte 0x02009d65
	.4byte 0x00000003
	.4byte 0x09f9001f
	.4byte 0x0200a365
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200899d
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a31
	.4byte 0x10008c15
	.4byte 0x09f70017
	.4byte 0x0200899d
	.4byte 0x00008c15
	.4byte 0x09f70017
	.4byte 0x02008a31
	.4byte 0x10009a15
	.4byte 0xffff0011
	.4byte 0x0200a129
	.4byte 0x10009a15
	.4byte 0xffff0012
	.4byte 0x0200a129
	.4byte 0x10009a15
	.4byte 0xffff0013
	.4byte 0x0200a129
	.4byte 0x10009a15
	.4byte 0xffff0014
	.4byte 0x0200a129
	.4byte 0x10009a15
	.4byte 0xffff0015
	.4byte 0x00000000
	.4byte 0x10009a15
	.4byte 0xffff0016
	.4byte 0x0200a17d
	.4byte 0x20009a15
	.4byte 0x03040016
	.4byte 0x0200a1c1
	.4byte 0x20009a15
	.4byte 0x03030015
	.4byte 0x0200a1c1
	.4byte 0x00009a15
	.4byte 0xffff0011
	.4byte 0x0200a319
	.4byte 0x00009a15
	.4byte 0xffff0012
	.4byte 0x0200a319
	.4byte 0x00009a15
	.4byte 0xffff0013
	.4byte 0x0200a319
	.4byte 0x00009a15
	.4byte 0xffff0014
	.4byte 0x0200a319
	.4byte 0x00009a15
	.4byte 0xffff0016
	.4byte 0x0200a319
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte 0x0200a57d
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte 0x0200a611
	.4byte 0x0000c403
	.4byte 0xffff001e
	.4byte 0x0200b0cd
	.4byte 0x00004403
	.4byte 0xffff001e
	.4byte 0x0200b2d9
	.4byte 0x00008403
	.4byte 0xffff001e
	.4byte 0x0200b349
	.4byte 0x00000403
	.4byte 0xffff001e
	.4byte 0x0200b3b1
	.4byte 0x00000006
	.4byte 0xffff00ca
	.4byte 0x0200a7d5
	.4byte 0x00000006
	.4byte 0xffff00cb
	.4byte 0x0200a7d5
	.4byte 0x00000006
	.4byte 0xffff00cc
	.4byte 0x0200a7d5
	.4byte 0x00000006
	.4byte 0xffff00cd
	.4byte 0x0200a7d5
	.4byte 0x00000006
	.4byte 0xffff00ce
	.4byte 0x0200a7d5
	.4byte 0x10009a15
	.4byte 0xffff000a
	.4byte 0x0200a869
	.4byte 0x10009a15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x20009a15
	.4byte 0x0305000b
	.4byte 0x0200a8ad
	.4byte 0x00009a15
	.4byte 0xffff000a
	.4byte 0x0200a9dd
	.4byte 0x00009a15
	.4byte 0xffff000b
	.4byte 0x0200a9dd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
