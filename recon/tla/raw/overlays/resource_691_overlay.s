.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200a619, 0x02008885, 0x020088cd, 0x020088d5, 0x0200a1e9, 0x0200888d, 0x0200ad05
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
	bl 0x0200ba64
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
	bl 0x0200bab4
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x0200bb34
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200babc
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
	bl 0x0200ba64
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
	bl 0x0200bab4
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x0200bb34
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
	bl 0x0200bafc
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
	bl 0x0200ba64
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
	bl 0x0200ba54
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x0200ba5c
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200bab4
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
	bl 0x0200bb34
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
	bl 0x0200b9c4
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
	bl 0x0200b9c4
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x0200b9c4
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
	bl 0x0200ba54
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200ba5c
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
	.4byte 0x0200be6c
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
	bl 0x0200bafc
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
	bl 0x0200baac
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
	bl 0x0200ba54
	lsls	r5, r5, #6
	movs	r0, #15
	bl 0x0200b9cc
	adds	r5, #51
	movs	r0, #185
	bl 0x0200bc54
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r6, #0
	bl 0x0200ba84
	mov	r0, r8
	str	r5, [r0, #48]
	str	r5, [r0, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl 0x0200ba84
	adds	r0, r6, #0
	bl 0x0200ba8c
	bl 0x0200bbdc
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
	bl 0x0200ba54
.L_02000520:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200bd10
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
	movs	r0, #8
	movs	r1, #56
	bl 0x0200bb84
	pop	{pc}
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
	bl 0x0200badc
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
	bl 0x0200bc24
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
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_0200063c
	movs	r0, #0
	b.n	.L_02000662
.L_0200063c:
	cmp	r0, #2
	bhi.n	.L_02000650
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_02000652
.L_02000650:
	ldr	r4, [pc, #16]
.L_02000652:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
.L_02000662:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	b.n	.L_020006b8
.L_02000672:
	ldrh	r0, [r7, #0]
	bl 0x0200bafc
	movs	r3, #34
	adds	r6, r0, #0
	adds	r3, r3, r6
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	mov	r8, r3
	bl 0x0200ba9c
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	adds	r5, r0, #0
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200bab4
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
	bl 0x020085cc
.L_020006b8:
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02000672
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	b.n	.L_0200073c
.L_020006d2:
	ldrh	r0, [r6, #0]
	bl 0x0200bafc
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
	bl 0x0200862c
	movs	r3, #64
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0200073a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	cmp	r0, #0
	beq.n	.L_02000748
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
.L_0200073a:
	adds	r6, #2
.L_0200073c:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020006d2
.L_02000748:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #99
	adds	r2, r2, r5
	ldrb	r3, [r2, #0]
	mov	ip, r2
	cmp	r3, #0
	beq.n	.L_0200079a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	cmp	r0, #0
	beq.n	.L_0200079a
	movs	r3, #212
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #8]
	subs	r2, r2, r3
.L_02000774:
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
	lsls	r1, r1, #2
	adds	r4, r4, r1
	mov	r2, ip
	strb	r3, [r4, #2]
	strb	r3, [r2, #0]
.L_0200079a:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
.L_020007a0:
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	mov	r8, r0
	cmp	r3, r2
	beq.n	.L_0200087c
.L_020007b2:
	mov	r3, r8
	ldrh	r0, [r3, #0]
	bl 0x0200bafc
	movs	r2, #34
	adds	r7, r0, #0
	adds	r2, r2, r7
	mov	sl, r2
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl 0x0200ba9c
.L_020007cc:
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	adds	r6, r0, #0
	asrs	r2, r2, #20
	asrs	r1, r1, #20
	movs	r0, #0
	bl 0x0200862c
	movs	r3, #255
	ands	r3, r0
	str	r3, [r7, #76]
	adds	r3, r7, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r3, #4
	strb	r5, [r3, #0]
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bab4
	asrs	r6, r6, #19
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	r3, sl
	adds	r6, #4
	ldrb	r0, [r3, #0]
	adds	r3, r6, #0
	adds	r6, r7, #0
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	adds	r6, #99
	bl 0x020085cc
	strb	r5, [r6, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #0
	bl 0x0200862c
	movs	r3, #64
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0200086a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	cmp	r0, #0
	beq.n	.L_0200087c
	movs	r2, #212
	lsls	r2, r2, #1
	adds	r0, r0, r2
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #8]
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
	strb	r3, [r6, #0]
.L_0200086a:
	movs	r3, #2
	add	r8, r3
	mov	r2, r8
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020007b2
.L_0200087c:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xbe78
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_020008a4
	ldr	r0, [pc, #24]
	b.n	.L_020008b0
.L_020008a4:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020008ae
	ldr	r0, [pc, #24]
	b.n	.L_020008b0
.L_020008ae:
	ldr	r0, [pc, #24]
.L_020008b0:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000e0
	.4byte 0x0200bed8
	.4byte 0x000000e1
	.4byte 0x0200bf88
	.2byte 0xbea8
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xbfc8
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #68]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_0200090e
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bgt.n	.L_020008fa
	ldr	r0, [pc, #44]
	b.n	.L_0200091a
.L_020008fa:
	cmp	r3, #8
	bgt.n	.L_02000902
	ldr	r0, [pc, #40]
	b.n	.L_0200091a
.L_02000902:
	cmp	r3, #12
	bgt.n	.L_0200090a
	ldr	r0, [pc, #36]
	b.n	.L_0200091a
.L_0200090a:
	ldr	r0, [pc, #36]
	b.n	.L_0200091a
.L_0200090e:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000918
	ldr	r0, [pc, #32]
	b.n	.L_0200091a
.L_02000918:
	ldr	r0, [pc, #32]
.L_0200091a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000e0
	.4byte 0x0200c1a4
	.4byte 0x0200c27c
	.4byte 0x0200c3e4
	.4byte 0x0200c4a4
	.4byte 0x000000e1
	.4byte 0x0200c5c4
	.2byte 0xc03c
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	ldr	r1, [pc, #224]
	lsls	r3, r3, #18
	movs	r2, #240
	lsls	r2, r2, #1
	ldr	r6, [r3, #108]
	adds	r3, #224
	ldr	r0, [r3, #0]
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #208]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_0200098e
	movs	r3, #21
	str	r3, [sp, #4]
	movs	r5, #78
	movs	r0, #98
	movs	r1, #18
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200baa4
	movs	r3, #24
	str	r3, [sp, #4]
	movs	r0, #98
	movs	r1, #24
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200baa4
	ldr	r0, [pc, #168]
	bl 0x0200bc14
	b.n	.L_02000a14
.L_0200098e:
	ldr	r3, [pc, #164]
	cmp	r2, r3
	bne.n	.L_02000a14
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #3
	bgt.n	.L_020009aa
	ldr	r0, [pc, #148]
	bl 0x0200bc14
	b.n	.L_02000a14
.L_020009aa:
	cmp	r3, #7
	bgt.n	.L_020009b6
	ldr	r0, [pc, #140]
	bl 0x0200bc14
	b.n	.L_02000a14
.L_020009b6:
	cmp	r3, #11
	bgt.n	.L_020009d6
	movs	r3, #79
	movs	r2, #36
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #79
	movs	r1, #46
	movs	r2, #2
	movs	r3, #1
	bl 0x0200baa4
	ldr	r0, [pc, #112]
	bl 0x0200bc14
	b.n	.L_02000a14
.L_020009d6:
	ldr	r3, [r0, #20]
	movs	r4, #150
	movs	r2, #18
	ldrsh	r3, [r3, r2]
	lsls	r4, r4, #2
	cmp	r3, r4
	bne.n	.L_020009fa
	movs	r3, #103
	movs	r2, #38
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #104
	movs	r1, #38
	movs	r2, #1
	movs	r3, #1
	bl 0x0200baa4
	b.n	.L_02000a0e
.L_020009fa:
	movs	r3, #99
	movs	r2, #38
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #94
	movs	r1, #44
	movs	r2, #5
	movs	r3, #1
	bl 0x0200baa4
.L_02000a0e:
	ldr	r0, [pc, #52]
	bl 0x0200bc14
.L_02000a14:
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl 0x0200874c
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000df
	.4byte 0x0200bd98
	.4byte 0x000000e0
	.4byte 0x0200bdac
	.4byte 0x0200bdd2
	.4byte 0x0200bde4
	.2byte 0xbde8
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #192
	lsls	r0, r0, #4
	adds	r0, #188
	adds	r3, r3, r0
	ldr	r6, [r3, #0]
	sub	sp, #8
	bl 0x0200baec
	movs	r0, #0
	bl 0x0200bbcc
	bl 0x0200bc1c
	ldr	r1, [pc, #380]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #372]
	cmp	r2, r3
	bne.n	.L_02000aaa
	movs	r3, #21
	str	r3, [sp, #4]
	movs	r5, #78
	movs	r0, #98
	movs	r1, #21
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200baa4
	movs	r3, #24
	str	r3, [sp, #4]
	movs	r0, #98
	movs	r1, #27
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200baa4
	ldr	r0, [pc, #332]
	bl 0x020086cc
	b.n	.L_02000b1e
.L_02000aaa:
	ldr	r3, [pc, #328]
	cmp	r2, r3
	bne.n	.L_02000b1e
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bgt.n	.L_02000ac6
	ldr	r0, [pc, #312]
	bl 0x020086cc
	b.n	.L_02000b1e
.L_02000ac6:
	cmp	r3, #8
	bgt.n	.L_02000ad2
	ldr	r0, [pc, #304]
	bl 0x020086cc
	b.n	.L_02000b1e
.L_02000ad2:
	cmp	r3, #12
	bgt.n	.L_02000af2
	movs	r3, #79
	movs	r2, #36
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #79
	movs	r1, #37
	movs	r2, #2
	movs	r3, #1
	bl 0x0200baa4
	ldr	r0, [pc, #276]
	bl 0x020086cc
	b.n	.L_02000b1e
.L_02000af2:
	movs	r3, #103
	str	r3, [sp, #0]
	movs	r5, #38
	movs	r0, #103
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200baa4
	movs	r3, #99
	str	r3, [sp, #0]
	movs	r0, #99
	movs	r1, #42
	movs	r2, #5
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200baa4
	ldr	r0, [pc, #232]
	bl 0x020086cc
.L_02000b1e:
	ldr	r3, [r6, #8]
	asrs	r2, r3, #20
	ldr	r3, [r6, #16]
	asrs	r1, r3, #20
	ldr	r3, [pc, #192]
	mov	ip, r3
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, ip
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	ldr	r3, [pc, #180]
	cmp	r0, r3
	bne.n	.L_02000b62
	cmp	r2, #15
	bne.n	.L_02000b4e
	cmp	r1, #25
	bne.n	.L_02000b4e
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #214
	bl 0x0200ba44
	b.n	.L_02000be0
.L_02000b4e:
	cmp	r2, #16
	bne.n	.L_02000be0
	cmp	r1, #22
	bne.n	.L_02000be0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #213
	bl 0x0200ba44
	b.n	.L_02000be0
.L_02000b62:
	ldr	r3, [pc, #144]
	cmp	r0, r3
	bne.n	.L_02000be0
	movs	r3, #241
	lsls	r3, r3, #1
	add	r3, ip
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bgt.n	.L_02000b8a
	cmp	r2, #27
	bne.n	.L_02000be0
	cmp	r1, #33
	bne.n	.L_02000be0
	movs	r0, #251
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200ba44
	b.n	.L_02000be0
.L_02000b8a:
	cmp	r3, #8
	bgt.n	.L_02000ba2
	cmp	r2, #55
	bne.n	.L_02000be0
	cmp	r1, #35
	bne.n	.L_02000be0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #216
	bl 0x0200ba44
	b.n	.L_02000be0
.L_02000ba2:
	cmp	r3, #12
	bgt.n	.L_02000bba
	cmp	r2, #16
	bne.n	.L_02000be0
	cmp	r1, #36
	bne.n	.L_02000be0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #217
	bl 0x0200ba44
	b.n	.L_02000be0
.L_02000bba:
	cmp	r2, #40
	bne.n	.L_02000bce
	cmp	r1, #37
	bne.n	.L_02000bce
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #218
	bl 0x0200ba44
	b.n	.L_02000be0
.L_02000bce:
	cmp	r2, #35
	bne.n	.L_02000be0
	cmp	r1, #38
	bne.n	.L_02000be0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #219
	bl 0x0200ba44
.L_02000be0:
	bl 0x0200baf4
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x000000df
	.4byte 0x0200bd98
	.4byte 0x000000e0
	.4byte 0x0200bdac
	.4byte 0x0200bdd2
	.4byte 0x0200bde4
	.2byte 0xbde8
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r1, #1
	movs	r0, #151
	bl 0x0200bbac
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
	bl 0x0200bbb4
	movs	r0, #1
	bl 0x0200bba4
	bl 0x0200bbbc
	bl 0x0200bbc4
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	movs	r1, #100
	adds	r1, r1, r7
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	sub	sp, #56
	ldr	r5, [r7, #104]
	mov	sl, r1
	cmp	r3, #49
	bgt.n	.L_02000cc2
	ldr	r3, [pc, #208]
	ldr	r6, [r3, #0]
	mov	r9, r3
	movs	r3, #3
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02000cba
	add	r1, sp, #16
	mov	r8, r1
	movs	r3, #148
	mov	r2, r8
	adds	r3, #255
	strh	r3, [r2, #24]
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #180]
	movs	r5, #15
	str	r3, [r2, #28]
.L_02000c82:
	bl 0x0200b9e4
	ands	r5, r0
	bl 0x0200b9e4
	ldr	r1, [r7, #12]
	movs	r3, #31
	ands	r3, r0
	lsls	r3, r3, #16
	adds	r1, r1, r3
	ldr	r3, [pc, #160]
	ldr	r0, [r7, #8]
	adds	r1, r1, r3
	movs	r3, #200
	lsls	r3, r3, #14
	adds	r3, #1
	subs	r5, #8
	ldr	r2, [r7, #16]
	lsls	r5, r5, #16
	str	r3, [sp, #8]
	mov	r3, r8
	str	r3, [sp, #12]
	adds	r0, r0, r5
	movs	r3, #0
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200815c
.L_02000cba:
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r2, #4
	b.n	.L_02000cd6
.L_02000cc2:
	cmp	r3, #99
	bgt.n	.L_02000ccc
	ldr	r3, [pc, #104]
	movs	r2, #4
	b.n	.L_02000cd4
.L_02000ccc:
	cmp	r3, #139
	bgt.n	.L_02000cf0
	ldr	r3, [pc, #92]
	movs	r2, #2
.L_02000cd4:
	ldr	r3, [r3, #0]
.L_02000cd6:
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000ce6
	adds	r0, r7, #0
	movs	r1, #10
	bl 0x0200bb34
	b.n	.L_02000d1a
.L_02000ce6:
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bb34
	b.n	.L_02000d1a
.L_02000cf0:
	cmp	r3, #143
	bgt.n	.L_02000cfe
	adds	r0, r7, #0
	movs	r1, #7
	bl 0x0200bb34
	b.n	.L_02000d1a
.L_02000cfe:
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bb34
	movs	r3, #0
	str	r3, [r7, #108]
	ldr	r3, [r7, #8]
	str	r3, [r5, #8]
	ldr	r3, [r7, #12]
	str	r3, [r5, #12]
	ldr	r3, [r7, #16]
	str	r3, [r5, #16]
	ldr	r3, [pc, #36]
	str	r3, [r5, #108]
.L_02000d1a:
	mov	r2, sl
	ldrh	r3, [r2, #0]
	mov	r1, sl
	adds	r3, #1
	strh	r3, [r1, #0]
	add	sp, #56
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.4byte 0x0200bd74
	.4byte 0xfff80000
	.2byte 0x8dc1
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #116]
	sub	sp, #56
	ldr	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	adds	r6, r0, #0
	cmp	r3, #0
	bne.n	.L_02000db4
	movs	r3, #148
	add	r7, sp, #16
	adds	r3, #255
	strh	r3, [r7, #24]
	movs	r3, #2
	str	r3, [r7, #0]
	ldr	r3, [pc, #92]
	str	r3, [r7, #28]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000d7e
	movs	r0, #152
	lsls	r0, r0, #2
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02000d7e
	adds	r0, r6, #0
	movs	r1, #136
	bl 0x0200bc4c
.L_02000d7e:
	bl 0x0200b9e4
	movs	r5, #15
	ands	r5, r0
	bl 0x0200b9e4
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
	movs	r4, #0
	str	r3, [sp, #8]
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #12]
	adds	r2, r2, r5
	movs	r3, #0
	str	r4, [sp, #0]
	str	r7, [sp, #12]
	bl 0x0200815c
.L_02000db4:
	add	sp, #56
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.2byte 0xbd50
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	ldrh	r0, [r6, #6]
	bl 0x0200b9f4
	adds	r3, r6, #0
	movs	r2, #98
	adds	r2, r2, r6
	adds	r3, #102
	adds	r5, r0, #0
.L_02000dda:
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldrb	r3, [r2, #0]
	mov	sl, r2
	adds	r0, r0, r3
	bl 0x0200bafc
	ldr	r3, [pc, #224]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r3, r3, r4
	mov	r8, r0
	ldr	r0, [r3, #0]
	bl 0x0200bafc
	ldr	r3, [r6, #8]
	lsls	r5, r5, #1
	adds	r3, r3, r5
	adds	r7, r0, #0
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl 0x02008d40
	ldr	r1, [pc, #196]
	ldr	r0, [r7, #8]
	movs	r4, #192
	adds	r2, r0, r1
	ldr	r1, [r6, #8]
	lsls	r4, r4, #11
.L_02000e14:
	adds	r3, r1, r4
	cmp	r2, r3
	bge.n	.L_02000e7a
	ldr	r3, [pc, #184]
	movs	r4, #128
	lsls	r4, r4, #12
	adds	r2, r1, r3
	adds	r3, r0, r4
	cmp	r2, r3
	bge.n	.L_02000e7a
	ldr	r1, [pc, #164]
	ldr	r0, [r7, #16]
	movs	r4, #192
	adds	r2, r0, r1
	ldr	r1, [r6, #16]
	lsls	r4, r4, #11
	adds	r3, r1, r4
	cmp	r2, r3
	bge.n	.L_02000e7a
	ldr	r3, [pc, #152]
	movs	r4, #128
	lsls	r4, r4, #12
	adds	r2, r1, r3
	adds	r3, r0, r4
	cmp	r2, r3
	bge.n	.L_02000e7a
	ldr	r2, [r6, #12]
.L_02000e4a:
	ldr	r0, [r7, #12]
	adds	r3, r2, r4
	cmp	r0, r3
	bge.n	.L_02000e7a
	ldr	r1, [pc, #124]
	movs	r4, #240
	lsls	r4, r4, #12
	adds	r2, r2, r1
	adds	r3, r0, r4
	cmp	r2, r3
	bge.n	.L_02000e7a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #181
	lsls	r2, r2, #1
	adds	r1, r3, r2
	movs	r2, #0
	movs	r3, #200
	strh	r3, [r1, #0]
	str	r2, [r6, #108]
	str	r2, [r6, #8]
	str	r2, [r6, #12]
	str	r2, [r6, #16]
.L_02000e7a:
	adds	r5, r6, #0
	adds	r5, #8
	mov	r1, r8
	adds	r1, #8
	adds	r0, r5, #0
	bl 0x020085a0
	cmp	r0, #8
	bgt.n	.L_02000edc
	mov	r4, r8
	ldr	r2, [r6, #12]
	ldr	r3, [r4, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02000edc
	movs	r1, #0
	str	r1, [r5, #0]
	ldr	r2, [pc, #40]
	ldrh	r3, [r6, #6]
	str	r1, [r6, #108]
	eors	r3, r2
	strh	r3, [r6, #6]
	str	r1, [r6, #12]
	str	r1, [r6, #16]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	movs	r2, #1
	eors	r3, r2
	mov	r4, sl
	strb	r3, [r4, #0]
	mov	r3, r8
	adds	r3, #100
	strh	r1, [r3, #0]
	ldr	r3, [pc, #24]
	mov	r1, r8
	str	r3, [r1, #108]
	str	r6, [r1, #104]
	b.n	.L_02000edc
	.4byte 0x00009000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0xfffa0000
	.2byte 0x8c41
	.2byte 0x0200
.L_02000edc:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	asrs	r1, r1, #16
	asrs	r2, r2, #16
	lsrs	r0, r0, #16
	mov	r8, r1
	mov	r9, r2
	bl 0x0200bafc
	adds	r6, r0, #0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	mov	fp, r0
	cmp	r0, #0
	bne.n	.L_02000fbc
	mov	r2, r8
	lsls	r3, r2, #16
	mov	r2, r9
	lsrs	r7, r3, #16
	lsls	r3, r2, #16
	lsrs	r3, r3, #16
	adds	r5, r7, r3
	adds	r0, r5, #0
	mov	sl, r3
	bl 0x0200bafc
	ldr	r3, [r0, #8]
	adds	r0, r5, #0
	str	r3, [r6, #8]
	bl 0x0200bafc
	ldr	r3, [r0, #12]
	adds	r0, r5, #0
	str	r3, [r6, #12]
	bl 0x0200bafc
	ldr	r3, [r0, #16]
	movs	r1, #2
	str	r3, [r6, #16]
	adds	r0, r6, #0
	bl 0x02008038
	adds	r3, r6, #0
	mov	r2, r8
	adds	r3, #102
	strh	r2, [r3, #0]
	movs	r2, #1
	mov	r3, r9
	eors	r3, r2
	adds	r2, r6, #0
	adds	r2, #98
	strb	r3, [r2, #0]
	ldr	r3, [pc, #100]
	movs	r2, #85
	str	r3, [r6, #108]
	adds	r3, r7, #1
	mov	r8, r3
	adds	r2, r2, r6
	mov	r3, sl
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_02000f96
	adds	r0, r7, #0
	bl 0x0200bafc
	adds	r5, r0, #0
	mov	r0, r8
	bl 0x0200bafc
.L_02000f82:
	ldr	r2, [r5, #8]
	ldr	r3, [r0, #8]
	cmp	r2, r3
	bge.n	.L_02000f90
	movs	r3, #128
	lsls	r3, r3, #8
	b.n	.L_02000fb4
.L_02000f90:
	mov	r2, fp
	strh	r2, [r6, #6]
	b.n	.L_02000fb6
.L_02000f96:
	adds	r0, r7, #0
	bl 0x0200bafc
	adds	r5, r0, #0
	mov	r0, r8
	bl 0x0200bafc
	ldr	r2, [r5, #8]
	ldr	r3, [r0, #8]
	cmp	r2, r3
	ble.n	.L_02000fb2
	movs	r3, #128
	lsls	r3, r3, #8
	b.n	.L_02000fb4
.L_02000fb2:
	mov	r3, sl
.L_02000fb4:
	strh	r3, [r6, #6]
.L_02000fb6:
	movs	r3, #0
	mov	r2, r9
	strb	r3, [r2, #0]
.L_02000fbc:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x8dc1
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	adds	r2, r0, #0
	adds	r2, #99
	ldrb	r2, [r2, #0]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000fe6
	movs	r1, #10
	bl 0x0200bb34
	b.n	.L_02000fec
.L_02000fe6:
	movs	r1, #0
	bl 0x0200bb34
.L_02000fec:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r3, #192
	adds	r6, r0, #0
	lsls	r3, r3, #18
	adds	r6, #100
	ldr	r5, [r3, #108]
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	movs	r1, #132
	lsls	r1, r1, #2
	ldrh	r2, [r6, #0]
	cmp	r3, r1
	ble.n	.L_02001012
	movs	r3, #2
	b.n	.L_0200101c
.L_02001012:
	movs	r1, #200
	lsls	r1, r1, #1
	cmp	r3, r1
	ble.n	.L_02001032
	movs	r3, #16
.L_0200101c:
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200102a
	movs	r1, #10
	bl 0x0200bb34
	b.n	.L_0200103a
.L_0200102a:
	movs	r1, #0
	bl 0x0200bb34
	b.n	.L_0200103a
.L_02001032:
	ldr	r3, [pc, #76]
	str	r3, [r0, #108]
	movs	r3, #1
	strh	r3, [r6, #0]
.L_0200103a:
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200107c
	subs	r2, #12
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200107c
	adds	r2, #4
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200107c
	adds	r2, #10
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200107c
	adds	r2, #74
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200107c
	ldrh	r3, [r6, #0]
	subs	r3, #1
	strh	r3, [r6, #0]
.L_0200107c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x9739
	.2byte 0x0200
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
	bl 0x0200862c
	movs	r7, #0
	asrs	r2, r0, #8
	b.n	.L_020010a8
.L_020010a4:
	ldrh	r7, [r5, #0]
	adds	r5, #2
.L_020010a8:
	movs	r1, #255
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	beq.n	.L_020010c8
	adds	r5, #2
	adds	r0, r3, #0
	ldrh	r3, [r5, #0]
	adds	r5, #2
	cmp	r2, r3
	bne.n	.L_020010a4
	bl 0x0200bafc
	ldrh	r7, [r5, #0]
	str	r0, [r6, #20]
.L_020010c8:
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	adds	r3, r0, #0
	adds	r4, r1, #0
	asrs	r3, r3, #20
	asrs	r4, r4, #20
	adds	r5, r2, #0
	adds	r1, r3, #0
	adds	r2, r4, #0
	movs	r0, #0
	bl 0x0200862c
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	movs	r6, #0
	asrs	r0, r0, #8
	b.n	.L_020010fe
.L_020010f0:
	adds	r5, #2
	ldrh	r6, [r5, #0]
	adds	r5, #2
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
.L_020010fe:
	cmp	r3, r2
	beq.n	.L_0200110c
	adds	r5, #2
	ldrh	r3, [r5, #0]
	cmp	r0, r3
	bne.n	.L_020010f0
	ldrh	r6, [r5, #2]
.L_0200110c:
	adds	r0, r6, #0
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	ldr	r1, [pc, #120]
	adds	r6, r0, #0
	lsls	r3, r3, #18
	movs	r0, #240
	adds	r3, #224
	lsls	r0, r0, #1
	ldr	r7, [r3, #0]
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #104]
	ldr	r5, [pc, #108]
	cmp	r2, r3
	bne.n	.L_02001156
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bgt.n	.L_02001142
	ldr	r5, [pc, #92]
	b.n	.L_0200115e
.L_02001142:
	cmp	r3, #8
	bgt.n	.L_0200114a
	ldr	r5, [pc, #88]
	b.n	.L_0200115e
.L_0200114a:
	cmp	r3, #12
	bgt.n	.L_02001152
	ldr	r5, [pc, #84]
	b.n	.L_0200115e
.L_02001152:
	ldr	r5, [pc, #84]
	b.n	.L_0200115e
.L_02001156:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200115e
	ldr	r5, [pc, #80]
.L_0200115e:
	cmp	r6, #2
	bne.n	.L_02001176
	adds	r0, r5, #0
	bl 0x02009084
	ldr	r3, [r7, #20]
	movs	r2, #4
.L_0200116c:
	adds	r1, r3, #0
	adds	r1, #99
	strb	r2, [r1, #0]
.L_02001172:
	ldr	r2, [pc, #64]
	str	r2, [r3, #108]
.L_02001176:
	cmp	r6, #3
	bne.n	.L_0200118e
	adds	r0, r5, #0
	bl 0x02009084
	ldr	r3, [r7, #20]
	movs	r2, #2
	adds	r1, r3, #0
	adds	r1, #99
	strb	r2, [r1, #0]
	ldr	r2, [pc, #40]
	str	r2, [r3, #108]
.L_0200118e:
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x000000e0
	.4byte 0x0200bd9e
	.4byte 0x0200bdb2
	.4byte 0x0200bdc4
	.4byte 0x0200bdd6
	.4byte 0x0200bdee
	.4byte 0x000000e1
	.4byte 0x0200be02
	.4byte 0x02008fcd
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
	mov	r7, sl
.L_020011e8:
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	mov	r8, r0
	movs	r0, #148
	adds	r0, #255
	sub	sp, #68
	bl 0x0200ba64
	mov	r1, r8
	adds	r6, r0, #0
	ldrh	r0, [r1, #6]
	bl 0x0200b9f4
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	lsls	r0, r0, #1
	strb	r3, [r2, #0]
	movs	r1, #1
	mov	sl, r0
	adds	r0, r6, #0
	bl 0x0200ba54
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200bab4
	bl 0x0200baec
	movs	r0, #0
	bl 0x0200bbcc
	ldr	r3, [r6, #8]
	add	r2, sp, #56
	str	r3, [r2, #0]
	ldr	r3, [r6, #12]
	str	r3, [r2, #4]
	ldr	r3, [r6, #16]
	str	r3, [r2, #8]
.L_0200123e:
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
	bl 0x0200862c
	asrs	r0, r0, #8
	cmp	r0, #212
	beq.n	.L_020012d6
	cmp	r0, #222
	beq.n	.L_020012d6
	movs	r2, #8
	mov	r9, r2
.L_02001264:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	mov	r0, r9
	lsls	r3, r0, #2
	adds	r3, #20
	ldr	r5, [r2, r3]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020012ca
	mov	r1, r8
	ldr	r2, [r1, #8]
	ldr	r3, [r5, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_0200129e
	ldr	r2, [r1, #12]
	ldr	r3, [r5, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_0200129e
	ldr	r2, [r1, #16]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	beq.n	.L_020012ca
.L_0200129e:
	adds	r0, r5, #0
	adds	r0, #8
	adds	r1, r7, #0
	bl 0x020085a0
	cmp	r0, #8
	bgt.n	.L_020012ca
	ldr	r2, [r5, #12]
	ldr	r3, [r7, #4]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020012ca
	ldr	r3, [r5, #80]
	movs	r0, #136
	ldr	r3, [r3, #40]
	lsls	r0, r0, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r0
	bne.n	.L_020013ae
	b.n	.L_020012d6
.L_020012ca:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #64
	bne.n	.L_02001264
	b.n	.L_0200123e
.L_020012d6:
	ldr	r3, [pc, #344]
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
	bl 0x0200ba84
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #7
	bl 0x0200bb5c
	ldr	r1, [r7, #4]
	ldr	r2, [r7, #8]
	movs	r3, #1
	ldr	r0, [r7, #0]
	bl 0x0200bb64
	adds	r0, r6, #0
	bl 0x0200ba8c
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	bl 0x0200ba6c
	movs	r3, #0
	mov	r9, r3
.L_0200131a:
	ldr	r3, [pc, #280]
	movs	r2, #1
	ldr	r3, [r3, #0]
	mov	sl, r3
	mov	r0, sl
	ands	r0, r2
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_02001378
	add	r1, sp, #16
	mov	r8, r1
	movs	r3, #148
	mov	r0, r8
	adds	r3, #255
	strh	r3, [r0, #24]
	ldr	r3, [pc, #252]
	str	r2, [r0, #0]
	str	r3, [r0, #36]
	ldr	r3, [pc, #252]
	movs	r6, #31
	str	r3, [r0, #28]
	bl 0x0200b9e4
	adds	r5, r0, #0
	bl 0x0200b9e4
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
.L_02001378:
	movs	r0, #1
	bl 0x0200b9cc
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #16
	bne.n	.L_0200131a
	movs	r0, #24
	bl 0x0200b9cc
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200bb54
	bl 0x0200bb6c
	movs	r0, #10
	bl 0x0200b9cc
	bl 0x0200baf4
	b.n	.L_02001424
.L_020013ae:
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
	bl 0x0200ba84
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #7
	bl 0x0200bb5c
	ldr	r2, [r7, #8]
	ldr	r1, [r7, #4]
	movs	r3, #1
	ldr	r0, [r7, #0]
	bl 0x0200bb64
	adds	r0, r6, #0
	bl 0x0200ba8c
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	bl 0x0200ba6c
	movs	r0, #188
	lsls	r0, r0, #2
	bl 0x0200ba44
	mov	r1, r9
	lsls	r0, r1, #16
	lsrs	r0, r0, #16
	bl 0x02008c08
	movs	r0, #188
	lsls	r0, r0, #2
	bl 0x0200ba4c
	ldr	r3, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200bb54
	bl 0x0200bb6c
	movs	r0, #10
	bl 0x0200b9cc
	bl 0x0200baf4
.L_02001424:
	add	sp, #68
.L_02001426:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02008d41
	.4byte 0x0300122c
	.4byte 0x020091b9
	.4byte 0x0200bd50
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	bl 0x0200baec
	movs	r0, #0
	bl 0x0200bbcc
	movs	r0, #10
	bl 0x0200bae4
	ldr	r0, [r5, #20]
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r1, #7
	bl 0x0200bb34
	movs	r0, #3
	bl 0x0200bae4
	movs	r1, #0
	ldr	r0, [r5, #20]
	bl 0x0200bb34
	movs	r0, #1
	bl 0x0200bae4
	ldr	r0, [r5, #20]
	bl 0x020091e4
	bl 0x0200baf4
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	bl 0x0200baec
	movs	r0, #0
	bl 0x0200bbcc
	ldr	r1, [r5, #20]
	movs	r3, #128
	adds	r2, r1, #0
	adds	r2, #100
	lsls	r3, r3, #3
	strh	r3, [r2, #0]
	ldr	r3, [pc, #8]
	str	r3, [r1, #108]
	bl 0x0200baf4
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x8ff5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r6, r1, #0
	adds	r7, r0, #0
	ldr	r5, [r3, #0]
	cmp	r6, #0
	bne.n	.L_020014d2
	bl 0x0200b8dc
.L_020014d2:
	cmp	r7, #2
	bne.n	.L_020014ea
	ldr	r3, [r5, #20]
	movs	r2, #240
	ldr	r1, [r3, #12]
	lsls	r2, r2, #12
	ldr	r0, [r3, #8]
	adds	r1, r1, r2
	ldr	r2, [r3, #16]
	movs	r3, #1
	bl 0x0200b820
.L_020014ea:
	cmp	r7, #3
	bne.n	.L_0200150c
	ldr	r3, [r5, #20]
	movs	r2, #240
	ldr	r1, [r3, #12]
	lsls	r2, r2, #12
	ldr	r0, [r3, #8]
	adds	r1, r1, r2
	ldr	r2, [r3, #16]
	movs	r3, #8
	bl 0x0200b820
	ldr	r2, [r5, #20]
	movs	r3, #0
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	str	r3, [r2, #16]
.L_0200150c:
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r6, r3
	bne.n	.L_0200151a
	bl 0x0200b998
.L_0200151a:
	cmp	r6, #178
	bne.n	.L_02001526
	ldr	r0, [r5, #20]
	movs	r1, #3
	bl 0x0200ba54
.L_02001526:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	ldr	r5, [r0, #104]
	adds	r4, r5, #0
	adds	r4, #100
	movs	r1, #0
	ldrsh	r2, [r4, r1]
	cmp	r2, #35
	ble.n	.L_0200155c
	adds	r3, r0, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #1
	adds	r3, #36
	cmp	r2, r3
	bge.n	.L_0200155c
	ldr	r2, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r2, r3
	str	r2, [r0, #12]
	ldr	r1, [pc, #140]
	movs	r6, #0
	ldrsh	r3, [r4, r6]
	lsls	r3, r3, #19
	adds	r2, r2, r3
	b.n	.L_020015be
.L_0200155c:
	movs	r2, #98
	adds	r2, r2, r0
	mov	ip, r2
	ldrb	r2, [r2, #0]
	movs	r6, #0
	ldrsh	r1, [r4, r6]
	lsls	r3, r2, #1
	adds	r3, #36
	cmp	r3, r1
	bge.n	.L_02001588
	movs	r3, #166
	lsls	r3, r3, #1
	cmp	r1, r3
	bge.n	.L_02001588
	ldr	r3, [r5, #12]
	lsls	r2, r2, #20
	movs	r6, #128
	adds	r3, r3, r2
	lsls	r6, r6, #14
	adds	r3, r3, r6
	str	r3, [r0, #12]
	b.n	.L_020015de
.L_02001588:
	movs	r2, #0
	ldrsh	r1, [r4, r2]
	movs	r3, #166
	lsls	r3, r3, #1
	cmp	r1, r3
	ble.n	.L_020015c4
	mov	r6, ip
	ldrb	r2, [r6, #0]
	movs	r6, #78
	lsls	r3, r2, #1
	adds	r6, #255
	adds	r3, r3, r6
	cmp	r1, r3
	bge.n	.L_020015c4
	lsls	r3, r2, #20
	ldr	r2, [r5, #12]
	movs	r1, #128
	adds	r2, r2, r3
	lsls	r1, r1, #14
	adds	r2, r2, r1
	str	r2, [r0, #12]
	movs	r1, #166
	movs	r6, #0
	ldrsh	r3, [r4, r6]
	lsls	r1, r1, #20
	lsls	r3, r3, #19
	subs	r2, r2, r3
.L_020015be:
	adds	r2, r2, r1
	str	r2, [r0, #12]
	b.n	.L_020015de
.L_020015c4:
	adds	r3, r0, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	movs	r6, #78
	lsls	r3, r3, #1
	adds	r6, #255
	adds	r2, r3, r6
	movs	r1, #0
	ldrsh	r3, [r4, r1]
	cmp	r2, r3
	bge.n	.L_020015de
	bl 0x0200ba6c
.L_020015de:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xfee8
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	ldr	r0, [r6, #104]
	movs	r1, #224
	mov	r8, r0
	mov	r4, r8
	adds	r4, #100
	ldrh	r2, [r4, #0]
	lsls	r1, r1, #11
	adds	r3, r2, #0
	subs	r3, #35
	lsls	r3, r3, #16
	sub	sp, #56
	cmp	r3, r1
	bhi.n	.L_02001624
	mov	r3, r8
	ldr	r2, [r3, #12]
	movs	r0, #192
	lsls	r0, r0, #13
	adds	r2, r2, r0
	str	r2, [r6, #12]
	movs	r1, #0
	ldrsh	r3, [r4, r1]
	lsls	r3, r3, #19
	adds	r2, r2, r3
	ldr	r3, [pc, #252]
	adds	r2, r2, r3
	str	r2, [r6, #12]
	b.n	.L_020016a4
.L_02001624:
	adds	r3, r2, #0
	subs	r3, #9
	movs	r1, #161
	adds	r0, r6, #0
	lsls	r3, r3, #16
	lsls	r1, r1, #17
	adds	r0, #98
	cmp	r3, r1
	bhi.n	.L_0200164a
	ldrb	r2, [r0, #0]
	mov	r0, r8
	ldr	r3, [r0, #12]
	lsls	r2, r2, #20
	movs	r1, #192
	adds	r3, r3, r2
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r6, #12]
	b.n	.L_020016a4
.L_0200164a:
	lsls	r3, r2, #16
	movs	r2, #166
	asrs	r1, r3, #16
	lsls	r2, r2, #1
	cmp	r1, r2
	ble.n	.L_02001686
	ldrb	r2, [r0, #0]
	movs	r0, #78
	lsls	r3, r2, #1
	adds	r0, #255
	adds	r3, r3, r0
	cmp	r1, r3
	bge.n	.L_02001686
	mov	r1, r8
	lsls	r3, r2, #20
	ldr	r2, [r1, #12]
	movs	r1, #166
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #13
	adds	r2, r2, r3
	str	r2, [r6, #12]
	lsls	r1, r1, #20
	movs	r0, #0
	ldrsh	r3, [r4, r0]
	lsls	r3, r3, #19
	subs	r2, r2, r3
	adds	r2, r2, r1
	str	r2, [r6, #12]
	b.n	.L_020016a4
.L_02001686:
	adds	r3, r6, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	movs	r0, #78
	lsls	r3, r3, #1
	adds	r0, #255
	adds	r2, r3, r0
	movs	r1, #0
	ldrsh	r3, [r4, r1]
	cmp	r2, r3
	bge.n	.L_020016a4
	ldr	r3, [r6, #20]
	str	r3, [r6, #12]
	movs	r3, #0
	str	r3, [r6, #108]
.L_020016a4:
	ldr	r3, [pc, #120]
	movs	r2, #7
	ldr	r7, [r3, #0]
	mov	sl, r2
	ands	r7, r2
	cmp	r7, #0
	bne.n	.L_020016f8
	movs	r3, #148
	add	r5, sp, #16
	adds	r3, #255
	strh	r3, [r5, #24]
	movs	r3, #10
	str	r3, [r5, #4]
	movs	r3, #2
	str	r3, [r5, #0]
	ldr	r3, [pc, #96]
	str	r3, [r5, #36]
	ldr	r3, [pc, #96]
.L_020016c8:
	str	r3, [r5, #28]
	bl 0x0200b9e4
	adds	r3, r0, #0
	mov	r0, sl
	ands	r3, r0
	ldr	r0, [r6, #8]
	ldr	r4, [pc, #84]
	movs	r1, #128
	lsls	r1, r1, #9
	ldr	r2, [pc, #80]
	adds	r0, r0, r1
	ldr	r1, [r6, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #76]
	subs	r3, #3
	adds	r1, r1, r2
	lsls	r3, r3, #14
	ldr	r2, [r6, #16]
	str	r7, [sp, #4]
	str	r4, [sp, #8]
	str	r5, [sp, #12]
	bl 0x0200815c
.L_020016f8:
	mov	r3, r8
	adds	r3, #34
	ldrb	r0, [r3, #0]
	mov	r3, r8
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #16]
	ldr	r3, [r6, #12]
	asrs	r1, r1, #20
	asrs	r3, r3, #19
	asrs	r2, r2, #20
.L_0200170c:
	adds	r3, #1
	bl 0x020085cc
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfef00000
	.4byte 0x0300122c
	.4byte 0x020091b9
	.4byte 0x0200bd50
	.4byte 0xfffe0000
	.4byte 0xfffc0000
	.2byte 0x0000
	.2byte 0x0133
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
	ldr	r5, [pc, #608]
	mov	r9, r3
	ldr	r3, [r5, #0]
	mov	r8, r0
	movs	r0, #15
	ands	r3, r0
	sub	sp, #72
	mov	sl, r0
	cmp	r3, #0
	bne.n	.L_02001768
	mov	r0, r8
	movs	r1, #136
	bl 0x0200bc4c
.L_02001768:
	movs	r1, #10
	mov	r0, r8
	bl 0x0200bb34
	movs	r1, #100
	add	r1, r8
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	fp, r1
	cmp	r3, #31
	bgt.n	.L_020017e6
	ldr	r7, [r5, #0]
	movs	r3, #3
	ands	r7, r3
	cmp	r7, #0
	beq.n	.L_0200178a
	b.n	.L_02001954
.L_0200178a:
	movs	r3, #148
	add	r6, sp, #32
	adds	r3, #255
	strh	r3, [r6, #24]
	movs	r3, #10
	str	r3, [r6, #4]
	movs	r3, #2
	str	r3, [r6, #0]
	ldr	r3, [pc, #536]
	str	r3, [r6, #36]
	ldr	r3, [pc, #536]
	str	r3, [r6, #28]
	bl 0x0200b9e4
	mov	r3, sl
	adds	r5, r0, #0
	ands	r5, r3
	bl 0x0200b9e4
	mov	r4, sl
	mov	r1, r8
	ands	r0, r4
	ldr	r4, [r1, #8]
	ldr	r1, [r1, #12]
	movs	r3, #192
	lsls	r3, r3, #13
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r1, r1, r3
	mov	r3, r8
	adds	r4, r4, r2
	ldr	r2, [r3, #16]
	ldr	r3, [pc, #496]
	subs	r5, #8
	subs	r0, #8
	lsls	r0, r0, #14
	lsls	r5, r5, #14
	str	r0, [sp, #4]
	str	r3, [sp, #8]
	adds	r0, r4, #0
	adds	r3, r5, #0
	str	r7, [sp, #0]
	str	r6, [sp, #12]
	bl 0x0200815c
	b.n	.L_02001954
.L_020017e6:
	cmp	r3, #33
	beq.n	.L_020017ec
	b.n	.L_0200193e
.L_020017ec:
	mov	r0, r8
	movs	r1, #144
	bl 0x0200bc4c
	mov	r4, r8
	ldr	r1, [r4, #8]
	ldr	r2, [r4, #12]
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r3, #128
	adds	r1, r1, r0
	lsls	r3, r3, #14
	movs	r0, #96
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r4, #16]
	bl 0x0200ba64
	add	r7, sp, #16
	str	r0, [r7, #0]
	movs	r4, #1
	mov	sl, r4
.L_02001818:
	ldr	r3, [r7, #0]
	movs	r0, #96
	ldr	r6, [r3, #80]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, #255
	bl 0x0200ba64
	mov	r1, sl
	ldrb	r3, [r6, #16]
	lsls	r5, r1, #2
	ldr	r1, [r0, #80]
	movs	r2, #1
	strb	r3, [r1, #16]
	ldrb	r3, [r1, #17]
	str	r0, [r7, r5]
	orrs	r3, r2
	strb	r3, [r1, #17]
	ldr	r2, [r7, #0]
	movs	r6, #0
	ldr	r3, [r2, #8]
	movs	r1, #0
.L_02001846:
	str	r3, [r0, #8]
	ldr	r3, [r2, #12]
	str	r3, [r0, #12]
	ldr	r3, [r2, #16]
	str	r3, [r0, #16]
	adds	r3, r0, #0
.L_02001852:
	adds	r3, #85
	strb	r6, [r3, #0]
	bl 0x0200bab4
	ldr	r0, [r7, r5]
	movs	r1, #3
	bl 0x0200ba54
	ldr	r0, [r7, r5]
	mov	r2, sl
	movs	r3, #3
	subs	r3, r3, r2
	ldr	r5, [pc, #340]
	adds	r2, r0, #0
	adds	r2, #98
	strb	r3, [r2, #0]
	mov	r3, r8
	str	r3, [r0, #104]
	str	r5, [r0, #108]
	movs	r1, #3
	bl 0x0200bb34
	movs	r4, #1
.L_02001880:
	add	sl, r4
	mov	r0, sl
	cmp	r0, #4
	bne.n	.L_02001818
	ldr	r0, [r7, #0]
	movs	r1, #0
	adds	r3, r0, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	bl 0x0200bab4
	ldr	r0, [r7, #0]
	movs	r1, #3
	bl 0x0200ba54
	ldr	r0, [r7, #0]
	movs	r1, #3
	bl 0x02008038
	ldr	r0, [r7, #0]
	movs	r3, #3
	adds	r2, r0, #0
	adds	r2, #98
	mov	r1, r8
	strb	r3, [r2, #0]
	str	r1, [r0, #104]
	str	r5, [r0, #108]
	movs	r1, #3
	bl 0x0200bb34
	ldr	r1, [pc, #260]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #252]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_020018de
	mov	r1, r8
	ldr	r0, [r1, #8]
	ldr	r2, [pc, #244]
	ldr	r1, [r1, #16]
	bl 0x020090cc
.L_020018dc:
	b.n	.L_0200191e
.L_020018de:
	ldr	r3, [pc, #240]
	cmp	r2, r3
	bne.n	.L_0200191e
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	mov	r1, r8
	ldr	r0, [r1, #8]
	ldr	r1, [r1, #16]
	cmp	r3, #3
	bgt.n	.L_02001900
	ldr	r2, [pc, #216]
	bl 0x020090cc
	b.n	.L_0200191e
.L_02001900:
	cmp	r3, #8
	bgt.n	.L_0200190c
	ldr	r2, [pc, #208]
	bl 0x020090cc
	b.n	.L_0200191e
.L_0200190c:
	cmp	r3, #12
	bgt.n	.L_02001918
	ldr	r2, [pc, #200]
	bl 0x020090cc
	b.n	.L_0200191e
.L_02001918:
	ldr	r2, [pc, #196]
	bl 0x020090cc
.L_0200191e:
	bl 0x0200bafc
	adds	r2, r0, #0
	adds	r2, #98
	movs	r3, #4
	strb	r3, [r2, #0]
	ldr	r3, [pc, #184]
	mov	r2, r8
	str	r2, [r0, #104]
	str	r3, [r0, #108]
	mov	r4, fp
	ldrh	r3, [r4, #0]
	mov	r0, fp
	adds	r3, #1
	strh	r3, [r0, #0]
	b.n	.L_02001954
.L_0200193e:
	movs	r1, #182
	lsls	r1, r1, #1
	cmp	r3, r1
	ble.n	.L_02001954
	mov	r0, r8
	movs	r1, #0
	bl 0x0200bb34
	movs	r3, #0
	mov	r2, r8
	str	r3, [r2, #108]
.L_02001954:
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, r9
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	bne.n	.L_020019a2
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, r9
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L_020019a2
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, r9
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_020019a2
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r9
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020019a2
	movs	r3, #217
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020019a2
	mov	r2, r8
	adds	r2, #100
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_020019a2:
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.4byte 0x020091b9
	.4byte 0x0200bd50
	.4byte 0x01330000
	.4byte 0x02009529
	.4byte 0x02000240
	.4byte 0x000000df
	.4byte 0x0200bd9e
	.4byte 0x000000e0
	.4byte 0x0200bdb2
	.4byte 0x0200bdc4
	.4byte 0x0200bdd6
	.4byte 0x0200bdee
	.2byte 0x95e5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #56]
	adds	r7, r1, #0
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #44]
	adds	r6, r0, #0
	cmp	r2, r3
	bne.n	.L_02001a0c
	movs	r0, #15
	b.n	.L_02001a14
.L_02001a0c:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02001a1a
	movs	r0, #14
.L_02001a14:
	bl 0x0200bafc
	str	r0, [r5, #20]
.L_02001a1a:
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl 0x020094bc
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200ba44
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x000000df
	.2byte 0x00e0
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #60]
	adds	r7, r1, #0
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	adds	r6, r0, #0
	cmp	r2, r3
	bne.n	.L_02001a5c
	movs	r0, #16
	b.n	.L_02001a64
.L_02001a5c:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02001a6a
	movs	r0, #10
.L_02001a64:
	bl 0x0200bafc
	str	r0, [r5, #20]
.L_02001a6a:
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl 0x020094bc
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200ba44
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000df
	.2byte 0x00e0
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r6, r0, #0
	movs	r0, #17
	ldr	r5, [r3, #0]
	mov	r8, r1
	bl 0x0200bafc
	mov	r1, r8
	str	r0, [r5, #20]
	adds	r0, r6, #0
	bl 0x020094bc
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200ba44
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r6, r0, #0
	movs	r0, #18
	ldr	r5, [r3, #0]
	mov	r8, r1
	bl 0x0200bafc
	mov	r1, r8
	str	r0, [r5, #20]
	adds	r0, r6, #0
	bl 0x020094bc
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200ba44
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
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
	bl 0x0200bafc
	mov	r1, r8
	str	r0, [r5, #20]
	adds	r0, r6, #0
	bl 0x020094bc
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200ba44
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	beq.n	.L_02001b3e
	movs	r0, #190
	lsls	r0, r0, #2
	bl 0x0200ba4c
.L_02001b3e:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
.L_02001b46:
	ldr	r5, [pc, #88]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	beq.n	.L_02001b96
	bl 0x0200baec
	movs	r0, #0
	bl 0x0200bbcc
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200bafc
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #132
	movs	r2, #220
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	bl 0x0200bb04
	movs	r0, #5
	bl 0x0200bae4
	movs	r0, #123
	bl 0x0200bc54
	movs	r0, #2
	bl 0x0200bb7c
	bl 0x0200baf4
.L_02001b96:
	movs	r0, #190
	lsls	r0, r0, #2
	bl 0x0200ba44
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x0200bbcc
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200ae30
	pop	{pc}
	.2byte 0x0000
	.2byte 0xbdba
	.2byte 0x0200
	push	{lr}
	movs	r1, #132
	movs	r2, #138
	movs	r0, #0
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	movs	r3, #30
	bl 0x0200bad4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200ba44
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #132
	movs	r2, #138
	movs	r0, #0
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	movs	r3, #0
	bl 0x0200bad4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200ba4c
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r5, [pc, #492]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	sub	sp, #68
	bl 0x0200bafc
	adds	r7, r0, #0
	bl 0x0200baec
	movs	r0, #0
	bl 0x0200bbcc
	movs	r0, #152
	lsls	r0, r0, #2
	bl 0x0200ba44
	movs	r0, #133
	bl 0x0200bc54
	ldr	r0, [r5, #0]
	bl 0x0200bafc
	movs	r1, #6
	bl 0x0200bb34
	ldr	r0, [r5, #0]
	movs	r1, #10
	bl 0x0200bb1c
	ldr	r0, [r5, #0]
	movs	r1, #32
	bl 0x0200bb24
	add	r3, sp, #28
	mov	r9, r3
	mov	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #14
	str	r3, [r2, #4]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_02001c6c:
	bl 0x0200b9e4
	movs	r5, #15
	ldr	r3, [pc, #392]
	ands	r0, r5
	movs	r2, #16
	lsls	r0, r0, #16
	add	r2, sp
	adds	r0, r0, r3
	str	r0, [r2, #0]
	mov	r8, r2
	bl 0x0200b9e4
	ands	r0, r5
	lsls	r0, r0, #16
	mov	r2, r8
	str	r0, [r2, #4]
	ldr	r3, [r2, #0]
	ldr	r4, [r7, #8]
	ldr	r2, [r7, #16]
	adds	r4, r4, r3
	ldr	r3, [pc, #360]
	ldr	r1, [r7, #12]
	str	r3, [sp, #0]
	movs	r3, #176
	lsls	r3, r3, #12
	str	r3, [sp, #8]
	mov	r3, r9
	adds	r1, r1, r0
	str	r3, [sp, #12]
	adds	r0, r4, #0
	movs	r3, #0
	movs	r5, #0
	str	r5, [sp, #4]
	bl 0x0200815c
	movs	r0, #5
	bl 0x0200b9cc
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #8
	bne.n	.L_02001c6c
	movs	r0, #1
	movs	r1, #1
	negs	r0, r0
	negs	r1, r1
	subs	r2, #2
	movs	r3, #0
	bl 0x0200bb64
	ldr	r3, [pc, #288]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
.L_02001cdc:
	adds	r3, r3, r2
	adds	r2, r7, #0
	strb	r5, [r3, #0]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r7, #72]
	str	r5, [r7, #68]
	movs	r0, #1
	bl 0x0200bae4
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	movs	r3, #60
	mov	sl, r3
.L_02001d02:
	ldr	r3, [pc, #256]
	ldr	r6, [r3, #0]
	movs	r3, #1
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02001d50
	bl 0x0200b9e4
	movs	r5, #15
	ldr	r2, [pc, #228]
	ands	r0, r5
	lsls	r0, r0, #16
	adds	r0, r0, r2
	mov	r3, r8
	str	r0, [r3, #0]
	bl 0x0200b9e4
	ands	r0, r5
	lsls	r0, r0, #16
	mov	r2, r8
	str	r0, [r2, #4]
	ldr	r3, [r2, #0]
	ldr	r4, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r4, r4, r3
	ldr	r3, [pc, #200]
	ldr	r2, [r7, #16]
	str	r3, [sp, #0]
	movs	r3, #176
	lsls	r3, r3, #12
	str	r3, [sp, #8]
	mov	r3, r9
	adds	r1, r1, r0
	str	r3, [sp, #12]
	adds	r0, r4, #0
	movs	r3, #0
	str	r6, [sp, #4]
	bl 0x0200815c
.L_02001d50:
	movs	r0, #1
	bl 0x0200b9cc
	ldr	r3, [r7, #40]
	cmp	r3, #0
	beq.n	.L_02001d68
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_02001d02
.L_02001d68:
	movs	r0, #188
	bl 0x0200bc54
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200bab4
	ldr	r5, [pc, #128]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #22
	bl 0x0200bb1c
	movs	r0, #160
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #12
	lsls	r2, r2, #9
	bl 0x0200bac4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200bac4
	bl 0x0200bacc
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200bb74
	bl 0x0200bb6c
	ldr	r0, [r5, #0]
	movs	r1, #4
	bl 0x0200bb1c
	ldr	r0, [r5, #0]
	bl 0x0200bafc
	movs	r1, #0
	bl 0x0200bb34
	ldr	r0, [r5, #0]
	bl 0x0200bb2c
	movs	r0, #152
	lsls	r0, r0, #2
	bl 0x0200ba4c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	bl 0x0200baf4
	add	sp, #68
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0x00013333
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
.L_02001e0e:
	push	{r6, r7}
	ldr	r3, [pc, #192]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #20
	bl 0x0200bafc
	movs	r2, #8
	adds	r1, r0, #0
	add	r2, sp
	ldr	r3, [r1, #8]
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	mov	r2, sl
	str	r3, [r2, #0]
	mov	r0, sl
	ldr	r3, [r1, #12]
	movs	r6, #79
	str	r3, [r2, #4]
	ldr	r3, [r1, #16]
	str	r3, [r2, #8]
	bl 0x02008374
	adds	r5, r0, #0
	bl 0x0200874c
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #20]
	adds	r7, r5, #0
	adds	r7, #34
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	asrs	r3, r3, #19
	ldrb	r0, [r7, #0]
	bl 0x020085cc
	movs	r3, #36
	str	r3, [sp, #4]
	movs	r0, #79
	movs	r1, #46
	movs	r2, #2
	mov	r8, r3
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200baa4
	bl 0x020083b4
	movs	r3, #0
	mov	r1, r8
	strb	r3, [r7, #0]
	movs	r0, #79
	str	r1, [sp, #4]
	movs	r2, #2
	movs	r1, #37
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200baa4
	ldr	r3, [r5, #20]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r3, r3, #19
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	adds	r3, #4
	ldrb	r0, [r7, #0]
	bl 0x020085cc
	ldr	r0, [pc, #52]
	bl 0x020086cc
	ldr	r2, [r5, #8]
.L_02001eaa:
	mov	r3, sl
	asrs	r2, r2, #20
	str	r2, [r3, #0]
	mov	r1, sl
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	str	r3, [r1, #8]
	cmp	r2, #16
	bne.n	.L_02001eca
	cmp	r3, #36
	bne.n	.L_02001eca
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #217
	bl 0x0200ba44
.L_02001eca:
	add	sp, #20
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xbde4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #188]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #20
	bl 0x0200bafc
	add	r2, sp, #8
	adds	r1, r0, #0
	ldr	r3, [r1, #8]
	mov	sl, r2
	ldr	r2, [pc, #168]
	mov	r0, sl
	adds	r3, r3, r2
	mov	r2, sl
	str	r3, [r2, #0]
	movs	r6, #99
	ldr	r3, [r1, #12]
	str	r3, [r2, #4]
	ldr	r3, [r1, #16]
	str	r3, [r2, #8]
	bl 0x02008374
	adds	r5, r0, #0
	bl 0x0200874c
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #20]
	adds	r7, r5, #0
	adds	r7, #34
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	asrs	r3, r3, #19
	ldrb	r0, [r7, #0]
	bl 0x020085cc
	movs	r3, #38
	str	r3, [sp, #4]
	movs	r0, #94
	movs	r1, #44
	movs	r2, #5
	mov	r8, r3
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200baa4
	bl 0x020083b4
	movs	r3, #0
	mov	r1, r8
	strb	r3, [r7, #0]
	movs	r0, #99
	str	r1, [sp, #4]
	movs	r2, #5
	movs	r1, #42
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200baa4
	ldr	r3, [r5, #20]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r3, r3, #19
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	adds	r3, #4
	ldrb	r0, [r7, #0]
	bl 0x020085cc
	ldr	r0, [pc, #56]
	bl 0x020086cc
	ldr	r2, [r5, #8]
	mov	r3, sl
	asrs	r2, r2, #20
	str	r2, [r3, #0]
	mov	r1, sl
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	str	r3, [r1, #8]
	cmp	r2, #35
	bne.n	.L_02001f9a
	cmp	r3, #38
	bne.n	.L_02001f9a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #219
	bl 0x0200ba44
.L_02001f9a:
	add	sp, #20
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfff00000
	.2byte 0xbde8
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #123
	movs	r2, #48
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #48
	movs	r2, #1
	movs	r3, #3
	movs	r0, #121
	bl 0x0200baa4
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200ba44
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #125
	movs	r2, #48
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #48
	movs	r2, #1
	movs	r3, #3
	movs	r0, #121
	bl 0x0200baa4
	movs	r0, #169
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200ba44
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #109
	movs	r2, #56
.L_02002004:
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #56
	movs	r2, #1
	movs	r3, #3
	movs	r0, #108
	bl 0x0200baa4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #82
	bl 0x0200ba44
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	movs	r3, #128
	add	r7, sp, #28
	lsls	r3, r3, #10
	str	r3, [r7, #8]
	str	r3, [r7, #12]
	movs	r3, #148
	adds	r3, #255
	strh	r3, [r7, #24]
	ldr	r3, [pc, #128]
	movs	r2, #1
	str	r2, [r7, #0]
	str	r3, [r7, #28]
	movs	r3, #0
	mov	fp, r0
	mov	r9, r1
	mov	sl, r3
.L_02002054:
	bl 0x0200b9e4
	movs	r3, #63
	ands	r0, r3
	ldr	r6, [pc, #108]
	lsls	r0, r0, #16
	add	r0, fp
	add	r5, sp, #16
	adds	r0, r0, r6
	mov	r8, r3
	str	r0, [r5, #0]
	bl 0x0200b9e4
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #12
	str	r3, [r5, #4]
	bl 0x0200b9e4
	mov	r3, r8
	adds	r2, r0, #0
	ands	r2, r3
	lsls	r2, r2, #16
	ldr	r3, [r5, #4]
	add	r2, r9
	adds	r2, r2, r6
	str	r2, [r5, #8]
	ldr	r0, [r5, #0]
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #232
	lsls	r3, r3, #14
	adds	r3, #1
	str	r3, [sp, #8]
	movs	r1, #0
	movs	r3, #0
	str	r7, [sp, #12]
	bl 0x0200815c
	movs	r3, #1
	add	sl, r3
	mov	r3, sl
	cmp	r3, #4
	bne.n	.L_02002054
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #220
	bl 0x0200ba44
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200bd50
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb500
	ldr	r3, [pc, #32]
	adds	r2, r0, #0
	adds	r2, #99
	ldrb	r2, [r2, #0]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020020ea
	movs	r1, #7
	bl 0x0200bb34
	b.n	.L_020020f0
.L_020020ea:
	movs	r1, #0
	bl 0x0200bb34
.L_020020f0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r7, r1, #0
	sub	sp, #8
	adds	r6, r0, #0
	ldr	r5, [r3, #0]
	cmp	r7, #0
	bne.n	.L_0200211e
	bl 0x0200b8dc
	ldr	r3, [r5, #20]
	movs	r2, #4
	adds	r1, r3, #0
	adds	r1, #99
	strb	r2, [r1, #0]
	ldr	r2, [pc, #196]
	str	r2, [r3, #108]
.L_0200211e:
	cmp	r6, #2
	bne.n	.L_02002140
	ldr	r3, [r5, #20]
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #16]
	bl 0x0200a024
	ldr	r3, [r5, #20]
	movs	r2, #240
	ldr	r1, [r3, #12]
	lsls	r2, r2, #12
	ldr	r0, [r3, #8]
	adds	r1, r1, r2
	ldr	r2, [r3, #16]
	movs	r3, #1
	bl 0x0200b820
.L_02002140:
	cmp	r6, #3
	bne.n	.L_020021ce
	ldr	r3, [r5, #20]
	movs	r6, #8
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #16]
	bl 0x0200a024
	ldr	r3, [r5, #20]
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #16]
	bl 0x0200a024
	ldr	r3, [r5, #20]
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #16]
	ldr	r3, [pc, #128]
	adds	r1, r1, r3
	bl 0x0200a024
	ldr	r3, [r5, #20]
	movs	r2, #240
	ldr	r1, [r3, #12]
	lsls	r2, r2, #12
	ldr	r0, [r3, #8]
	adds	r1, r1, r2
	ldr	r2, [r3, #16]
	movs	r3, #16
	bl 0x0200b820
	ldr	r2, [r5, #20]
	movs	r3, #0
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	movs	r3, #17
	str	r3, [sp, #4]
	movs	r5, #42
	movs	r0, #0
	movs	r1, #32
	movs	r2, #5
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200baa4
	movs	r3, #81
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #96
	movs	r2, #5
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200baa4
	movs	r5, #5
	movs	r0, #0
	movs	r1, #32
	movs	r2, #42
	movs	r3, #17
.L_020021b6:
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ba94
	movs	r0, #0
	movs	r1, #96
	movs	r2, #42
	movs	r3, #81
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ba94
.L_020021ce:
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r7, r3
	bne.n	.L_020021dc
	bl 0x0200b998
.L_020021dc:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x0200a0d1
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb500
.L_020021ea:
	ldr	r1, [pc, #68]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02002222
	movs	r2, #241
.L_020021fe:
	lsls	r2, r2, #1
	adds	r3, r1, r2
.L_02002202:
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bgt.n	.L_0200220e
	ldr	r0, [pc, #44]
	b.n	.L_0200222e
.L_0200220e:
	cmp	r3, #8
	bgt.n	.L_02002216
	ldr	r0, [pc, #40]
	b.n	.L_0200222e
.L_02002216:
	cmp	r3, #12
	bgt.n	.L_0200221e
	ldr	r0, [pc, #36]
	b.n	.L_0200222e
.L_0200221e:
	ldr	r0, [pc, #36]
	b.n	.L_0200222e
.L_02002222:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_0200222c
	ldr	r0, [pc, #32]
.L_0200222a:
	b.n	.L_0200222e
.L_0200222c:
	ldr	r0, [pc, #32]
.L_0200222e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000e0
	.4byte 0x0200c720
	.4byte 0x0200c7e0
	.4byte 0x0200c87c
	.4byte 0x0200c930
	.4byte 0x000000e1
	.4byte 0x0200ca5c
	.2byte 0xc60c
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
	bl 0x0200b9c4
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002284
	adds	r3, #15
.L_02002284:
	asrs	r3, r3, #4
.L_02002286:
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
	ldr	r3, [pc, #384]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200bafc
	adds	r7, r0, #0
	bl 0x0200baec
	movs	r0, #0
	bl 0x0200bbcc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200bb64
	bl 0x0200ba7c
	movs	r0, #1
	bl 0x0200b9cc
	ldr	r3, [r7, #12]
	movs	r4, #130
	lsls	r4, r4, #16
	adds	r3, r3, r4
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
.L_020022fa:
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	bl 0x0200bb94
	bl 0x0200bb9c
	movs	r0, #204
	bl 0x0200bc54
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200bae4
	add	r3, sp, #28
	mov	r8, r3
	mov	r4, r8
	movs	r3, #7
	str	r3, [r4, #4]
	ldr	r3, [pc, #256]
	movs	r2, #0
	str	r3, [r4, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r4, #8]
	str	r3, [r4, #12]
	mov	sl, r2
.L_0200234a:
	mov	r3, sl
	lsls	r5, r3, #12
	adds	r0, r5, #0
	bl 0x0200b9f4
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200b9ec
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200b9e4
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	ldr	r4, [pc, #200]
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r6, #0]
	bl 0x0200b9e4
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r2, [pc, #180]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	ldr	r4, [r6, #4]
	adds	r5, r5, r3
	adds	r5, r5, r2
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #160]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x0200815c
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_0200234a
	movs	r0, #188
	bl 0x0200bc54
	ldr	r5, [pc, #116]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200bb44
	ldr	r0, [r5, #0]
	movs	r1, #22
	bl 0x0200bb1c
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200bac4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200bac4
	bl 0x0200bacc
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200bb44
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
	bl 0x0200baf4
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200a255
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #108]
	adds	r7, r0, #0
	mov	r8, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	mov	sl, r3
	bl 0x0200bafc
	adds	r5, r0, #0
	movs	r0, #10
.L_0200246c:
	adds	r0, #255
	bl 0x0200ba3c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020024bc
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200bab4
	ldr	r3, [r5, #12]
	movs	r2, #128
	adds	r3, r3, r7
	str	r3, [r5, #12]
	lsls	r2, r2, #2
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r2, #18
	str	r3, [r5, #48]
	add	r2, r8
	movs	r3, #2
	str	r6, [r5, #40]
	strb	r3, [r2, #0]
	adds	r2, r5, #0
	adds	r2, #90
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200bb54
	bl 0x0200ba7c
	movs	r0, #1
	bl 0x0200b9cc
.L_020024bc:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x0200bafc
	adds	r5, r0, #0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002510
	movs	r0, #1
	bl 0x0200b9cc
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200ba9c
.L_020024fa:
	movs	r3, #0
	str	r3, [r5, #40]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r0, [r5, #20]
	str	r0, [r5, #12]
	str	r3, [r5, #60]
	movs	r1, #0
	ldr	r0, [r6, #0]
.L_0200250c:
	bl 0x0200bb54
.L_02002510:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	adds	r0, r5, #0
	sub	sp, #16
	bl 0x0200bafc
	ldr	r3, [pc, #224]
	ldr	r2, [r0, #108]
	cmp	r2, r3
	bne.n	.L_02002604
	adds	r0, r5, #0
	bl 0x0200bafc
	ldr	r0, [r0, #104]
.L_0200253c:
	ldr	r1, [r0, #8]
	mov	r8, r0
	mov	r3, r8
	movs	r0, #128
	ldr	r2, [r3, #12]
	lsls	r0, r0, #9
	adds	r1, r1, r0
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r2, r2, r0
	movs	r0, #96
	ldr	r3, [r3, #16]
	adds	r0, #255
	bl 0x0200ba64
	mov	r7, sp
	str	r0, [r7, #0]
	movs	r3, #1
	mov	sl, r3
.L_02002562:
	ldr	r3, [r7, #0]
	movs	r0, #96
	ldr	r6, [r3, #80]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, #255
	bl 0x0200ba64
	mov	r3, sl
	ldr	r1, [r0, #80]
	lsls	r5, r3, #2
	ldrb	r3, [r6, #16]
	movs	r2, #1
	strb	r3, [r1, #16]
	ldrb	r3, [r1, #17]
	str	r0, [r7, r5]
	orrs	r3, r2
	strb	r3, [r1, #17]
	ldr	r2, [r7, #0]
	movs	r6, #0
	ldr	r3, [r2, #8]
	movs	r1, #0
	str	r3, [r0, #8]
	ldr	r3, [r2, #12]
.L_02002594:
	str	r3, [r0, #12]
	ldr	r3, [r2, #16]
	str	r3, [r0, #16]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	bl 0x0200bab4
	ldr	r0, [r7, r5]
	movs	r1, #3
	bl 0x0200ba54
	ldr	r0, [r7, r5]
	mov	r3, sl
	adds	r2, r0, #0
	ldr	r5, [pc, #96]
	adds	r3, #255
	adds	r2, #98
	strb	r3, [r2, #0]
	mov	r3, r8
	str	r3, [r0, #104]
	str	r5, [r0, #108]
	movs	r1, #3
	bl 0x0200bb34
	movs	r0, #1
	add	sl, r0
	mov	r3, sl
	cmp	r3, #4
	bne.n	.L_02002562
	ldr	r0, [r7, #0]
	movs	r1, #0
	adds	r3, r0, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	bl 0x0200bab4
	ldr	r0, [r7, #0]
	movs	r1, #3
	bl 0x0200ba54
	ldr	r0, [r7, #0]
	movs	r1, #3
	bl 0x02008038
	ldr	r0, [r7, #0]
	movs	r3, #3
	adds	r2, r0, #0
	adds	r2, #98
	strb	r3, [r2, #0]
	mov	r3, r8
	str	r3, [r0, #104]
	str	r5, [r0, #108]
	movs	r1, #3
	bl 0x0200bb34
.L_02002604:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x020095e5
	.2byte 0x9529
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
.L_02002624:
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r1, [pc, #56]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r2, #129
	adds	r3, r3, r0
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	mov	r8, r1
	movs	r3, #152
	ldr	r1, [pc, #44]
	lsls	r3, r3, #2
	add	r3, r8
	adds	r2, #94
.L_02002642:
	strh	r1, [r3, #0]
	add	r2, r8
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	sub	sp, #8
	ldr	r7, [pc, #8]
	cmp	r2, r1
	beq.n	.L_0200265e
	b.n	.L_02002780
.L_0200265e:
	b.n	.L_0200266c
	.4byte 0x00000000
	.4byte 0x02000240
	.2byte 0x00df
	.2byte 0x0000
.L_0200266c:
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200bafc
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200bafc
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200bafc
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	ldr	r0, [pc, #860]
	bl 0x02008668
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_020026ba
	movs	r0, #21
	movs	r1, #13
	movs	r2, #0
	bl 0x02008ee4
.L_020026ba:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #213
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_020026d6
	movs	r1, #132
	movs	r2, #180
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200bb0c
.L_020026d6:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #214
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_020026f2
	movs	r1, #248
	movs	r2, #204
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200bb0c
.L_020026f2:
	ldr	r6, [pc, #780]
	movs	r5, #78
	adds	r0, r6, #0
	bl 0x0200bc0c
	movs	r3, #21
	str	r3, [sp, #4]
	movs	r0, #98
	movs	r1, #21
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200baa4
	movs	r3, #24
	str	r3, [sp, #4]
	movs	r0, #98
	movs	r1, #27
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200baa4
	adds	r0, r6, #0
	bl 0x020086cc
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_0200275e
	movs	r0, #20
	bl 0x0200bafc
	ldr	r3, [r0, #8]
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r0, #8]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	adds	r3, #4
	strb	r7, [r3, #0]
	movs	r2, #192
	ldr	r3, [r0, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	movs	r1, #0
	bl 0x0200bab4
.L_0200275e:
	movs	r0, #20
	bl 0x0200a518
	movs	r3, #241
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	cmp	r2, #4
	ble.n	.L_02002774
	b.n	.L_02002c20
.L_02002774:
	cmp	r2, #3
	bge.n	.L_0200277a
	b.n	.L_02002c20
.L_0200277a:
	movs	r0, #128
	lsls	r0, r0, #13
	b.n	.L_02002c1c
.L_02002780:
	ldr	r3, [pc, #640]
	cmp	r2, r3
	beq.n	.L_02002788
	b.n	.L_02002c26
.L_02002788:
	movs	r1, #133
	lsls	r1, r1, #2
	add	r1, r8
	ldr	r0, [r1, #0]
	mov	sl, r1
	bl 0x0200bafc
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r2, #241
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r0, #0]
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	mov	r9, r2
	cmp	r3, #3
	bgt.n	.L_02002848
	ldr	r0, [pc, #596]
	bl 0x0200ad08
	ldr	r0, [pc, #596]
	bl 0x02008668
	bl 0x0200bbec
	movs	r0, #0
	movs	r1, #11
	movs	r2, #12
	bl 0x0200bbf4
	movs	r0, #251
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_020027e6
	movs	r1, #220
	movs	r2, #134
	movs	r0, #15
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200bb0c
.L_020027e6:
	ldr	r0, [pc, #552]
	bl 0x0200bc0c
	mov	r2, r9
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #1
	bne.n	.L_0200281c
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002814
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200bafc
	ldr	r3, [r0, #16]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r0, #16]
.L_02002814:
	ldr	r0, [pc, #508]
	bl 0x0200a44c
	b.n	.L_02002cda
.L_0200281c:
	cmp	r3, #2
	beq.n	.L_02002822
	b.n	.L_02002cda
.L_02002822:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002840
	mov	r2, sl
	ldr	r0, [r2, #0]
	bl 0x0200bafc
	ldr	r3, [r0, #16]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r0, #16]
.L_02002840:
	ldr	r0, [pc, #468]
	bl 0x0200a44c
	b.n	.L_02002cda
.L_02002848:
	cmp	r3, #8
	ble.n	.L_0200284e
	b.n	.L_02002a28
.L_0200284e:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002908
	movs	r0, #8
	bl 0x0200bafc
	adds	r0, #85
	strb	r7, [r0, #0]
	movs	r0, #8
	bl 0x0200bafc
	ldr	r3, [r0, #12]
	movs	r5, #160
	lsls	r5, r5, #15
	adds	r3, r3, r5
	str	r3, [r0, #12]
	movs	r0, #9
	bl 0x0200bafc
	adds	r0, #85
	strb	r7, [r0, #0]
	movs	r0, #9
	bl 0x0200bafc
	ldr	r3, [r0, #12]
	movs	r1, #8
	adds	r3, r3, r5
	movs	r2, #0
	str	r3, [r0, #12]
	movs	r0, #19
	bl 0x02008ee4
	movs	r0, #10
	bl 0x0200bafc
	adds	r0, #85
	strb	r7, [r0, #0]
	movs	r0, #10
	bl 0x0200bafc
	ldr	r3, [r0, #12]
	movs	r5, #160
	lsls	r5, r5, #16
	adds	r3, r3, r5
	str	r3, [r0, #12]
	movs	r0, #11
	bl 0x0200bafc
	adds	r0, #85
	strb	r7, [r0, #0]
	movs	r0, #11
	bl 0x0200bafc
	ldr	r3, [r0, #12]
	movs	r1, #10
	adds	r3, r3, r5
	movs	r2, #1
	str	r3, [r0, #12]
	movs	r0, #20
	bl 0x02008ee4
	movs	r0, #12
	bl 0x0200bafc
	adds	r0, #85
	strb	r7, [r0, #0]
	movs	r0, #12
	bl 0x0200bafc
	ldr	r3, [r0, #12]
	movs	r5, #240
	lsls	r5, r5, #16
	adds	r3, r3, r5
	str	r3, [r0, #12]
	movs	r0, #13
	bl 0x0200bafc
	adds	r0, #85
	strb	r7, [r0, #0]
	movs	r0, #13
	bl 0x0200bafc
	ldr	r3, [r0, #12]
	movs	r1, #12
	adds	r3, r3, r5
	str	r3, [r0, #12]
	movs	r2, #0
	movs	r0, #21
	bl 0x02008ee4
.L_02002908:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #216
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002924
	movs	r1, #222
	movs	r2, #142
	movs	r0, #16
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200bb0c
.L_02002924:
	ldr	r0, [pc, #244]
	bl 0x0200bc0c
	ldr	r0, [pc, #244]
	bl 0x02008668
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_0200298c
	movs	r0, #15
	bl 0x0200bafc
	ldr	r3, [r0, #8]
	movs	r6, #128
	lsls	r6, r6, #9
	adds	r3, r3, r6
	str	r3, [r0, #8]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	adds	r3, #4
	strb	r7, [r3, #0]
	movs	r5, #192
	ldr	r3, [r0, #12]
	lsls	r5, r5, #13
	adds	r3, r3, r5
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	movs	r1, #0
	bl 0x0200bab4
	movs	r0, #18
	bl 0x0200bafc
	ldr	r3, [r0, #8]
	movs	r1, #0
	adds	r3, r3, r6
	str	r3, [r0, #8]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	adds	r3, #4
	strb	r7, [r3, #0]
	ldr	r3, [r0, #12]
	adds	r3, r3, r5
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	bl 0x0200bab4
.L_0200298c:
	movs	r0, #15
	bl 0x0200a518
	movs	r0, #18
	bl 0x0200a518
	mov	r0, r9
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #5
	bne.n	.L_020029ac
	movs	r0, #144
	lsls	r0, r0, #17
.L_020029a6:
	bl 0x0200a44c
	b.n	.L_02002c20
.L_020029ac:
	cmp	r3, #6
	bne.n	.L_020029c4
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_020029be
	b.n	.L_02002c20
.L_020029be:
	bl 0x0200a2ac
	b.n	.L_02002c20
.L_020029c4:
	cmp	r3, #7
	bne.n	.L_020029ee
.L_020029c8:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_020029e6
.L_020029d4:
	mov	r1, sl
	ldr	r0, [r1, #0]
	bl 0x0200bafc
	ldr	r3, [r0, #16]
	movs	r2, #128
.L_020029e0:
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r0, #16]
.L_020029e6:
	ldr	r0, [pc, #60]
.L_020029e8:
	bl 0x0200a44c
	b.n	.L_02002c20
.L_020029ee:
	cmp	r3, #8
	beq.n	.L_020029f4
	b.n	.L_02002c20
.L_020029f4:
	movs	r0, #208
	lsls	r0, r0, #15
	b.n	.L_02002c1c
	.2byte 0x0000
	.4byte 0x0200bd9e
	.4byte 0x0200bd98
	.4byte 0x000000e0
	.4byte 0x0200bdba
	.4byte 0x0200bdb2
	.4byte 0x0200bdac
	.4byte 0xfff00000
	.4byte 0xffa00000
	.4byte 0x0200bdd2
	.4byte 0x0200bdc4
	.2byte 0x0000
	.2byte 0xffd0
.L_02002a28:
	.2byte 0x2b0c
	bgt.n	.L_02002aee
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #217
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002a48
	movs	r1, #132
	movs	r2, #146
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200bb0c
.L_02002a48:
	ldr	r5, [pc, #668]
	adds	r0, r5, #0
	bl 0x0200bc0c
	adds	r0, r5, #0
	bl 0x020086cc
	bl 0x0200bbe4
	movs	r1, #148
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #14
	movs	r3, #13
	bl 0x0200bbfc
	ldr	r0, [pc, #640]
	bl 0x02008668
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002aca
	movs	r0, #10
	bl 0x0200bafc
	ldr	r3, [r0, #8]
	movs	r6, #128
	lsls	r6, r6, #9
	adds	r3, r3, r6
	str	r3, [r0, #8]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	adds	r3, #4
	strb	r7, [r3, #0]
	movs	r5, #192
	ldr	r3, [r0, #12]
	lsls	r5, r5, #13
	adds	r3, r3, r5
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	movs	r1, #0
	bl 0x0200bab4
	movs	r0, #12
	bl 0x0200bafc
	ldr	r3, [r0, #8]
	movs	r1, #0
	adds	r3, r3, r6
	str	r3, [r0, #8]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	adds	r3, #4
	strb	r7, [r3, #0]
	ldr	r3, [r0, #12]
	adds	r3, r3, r5
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	bl 0x0200bab4
.L_02002aca:
	movs	r0, #10
	bl 0x0200a518
	movs	r0, #12
	bl 0x0200a518
	mov	r0, r9
	ldrh	r3, [r0, #0]
	movs	r1, #128
	subs	r3, #9
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	cmp	r3, r1
	bls.n	.L_02002ae8
	b.n	.L_02002c20
.L_02002ae8:
	movs	r0, #192
	lsls	r0, r0, #14
	b.n	.L_02002c1c
.L_02002aee:
	ldr	r0, [pc, #512]
	bl 0x02008668
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002b2c
	movs	r0, #13
	bl 0x0200bafc
	ldr	r3, [r0, #8]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r0, #8]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	adds	r3, #4
	strb	r7, [r3, #0]
	movs	r1, #192
	ldr	r3, [r0, #12]
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	movs	r1, #0
	bl 0x0200bab4
.L_02002b2c:
	movs	r0, #13
	bl 0x0200a518
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #218
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002b4e
	movs	r1, #162
	movs	r2, #150
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200bb0c
.L_02002b4e:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #219
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002b6a
	movs	r1, #142
	movs	r2, #154
	movs	r0, #9
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200bb0c
.L_02002b6a:
	ldr	r0, [pc, #392]
	bl 0x0200879c
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002b90
	movs	r3, #123
	movs	r2, #48
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #121
	movs	r1, #48
	movs	r2, #1
	movs	r3, #3
	bl 0x0200baa4
.L_02002b90:
	movs	r0, #169
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002bb2
	movs	r3, #125
	movs	r2, #48
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #121
	movs	r1, #48
	movs	r2, #1
	movs	r3, #3
	bl 0x0200baa4
.L_02002bb2:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #82
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002bd4
	movs	r3, #109
	movs	r2, #56
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #108
	movs	r1, #56
	movs	r2, #1
	movs	r3, #3
	bl 0x0200baa4
.L_02002bd4:
	mov	r3, r9
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #14
	bne.n	.L_02002bea
	movs	r0, #192
	lsls	r0, r0, #14
	bl 0x0200a44c
	b.n	.L_02002c20
.L_02002bea:
	adds	r3, r2, #0
	subs	r3, #16
	movs	r1, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	cmp	r3, r1
	bhi.n	.L_02002c20
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002c1a
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200bafc
	ldr	r3, [r0, #16]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r0, #16]
.L_02002c1a:
	ldr	r0, [pc, #220]
.L_02002c1c:
	bl 0x0200a44c
.L_02002c20:
	bl 0x0200a4c8
	b.n	.L_02002cda
.L_02002c26:
	ldr	r3, [pc, #212]
	cmp	r2, r3
	bne.n	.L_02002cda
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #220
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002c8e
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bb0c
	movs	r3, #17
	str	r3, [sp, #4]
	movs	r5, #42
	movs	r0, #0
	movs	r1, #32
	movs	r2, #5
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200baa4
	movs	r3, #81
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #96
	movs	r2, #5
	movs	r3, #8
	str	r5, [sp, #0]
	bl 0x0200baa4
	movs	r5, #5
	movs	r6, #8
	movs	r0, #0
	movs	r1, #32
	movs	r2, #42
	movs	r3, #17
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ba94
	movs	r0, #0
	movs	r1, #96
	movs	r2, #42
	movs	r3, #81
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ba94
.L_02002c8e:
	movs	r5, #133
	lsls	r5, r5, #2
	add	r5, r8
	ldr	r0, [r5, #0]
	bl 0x0200bafc
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [pc, #88]
	bl 0x02008668
	movs	r3, #241
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #2
	bgt.n	.L_02002cda
	movs	r0, #10
	adds	r0, #255
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002cd4
	ldr	r0, [r5, #0]
	bl 0x0200bafc
	ldr	r3, [r0, #16]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r0, #16]
.L_02002cd4:
	ldr	r0, [pc, #32]
	bl 0x0200a44c
.L_02002cda:
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0200bde4
	.4byte 0x0200bdd6
	.4byte 0x0200bdee
	.4byte 0x0200bde8
	.4byte 0xffe00000
	.4byte 0x000000e1
	.2byte 0xbe02
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
.L_02002d0a:
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
	beq.n	.L_02002dee
.L_02002d2e:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	str	r3, [sp, #12]
	bl 0x0200bafc
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
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002d7c
	ldr	r0, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bb0c
	b.n	.L_02002ddc
.L_02002d7c:
	adds	r0, r7, #0
	bl 0x0200bc04
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r5, r5, r3
	str	r5, [sp, #4]
	mov	r2, r9
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl 0x0200badc
	mov	r3, r9
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	sl, r0
	ldrb	r0, [r3, #0]
	bl 0x0200ba9c
	adds	r5, r0, #0
	ldr	r0, [sp, #12]
	bl 0x0200bb3c
	ldr	r2, [sp, #4]
	movs	r3, #128
	asrs	r5, r5, #19
	strb	r3, [r2, #3]
	adds	r5, #4
	mov	r3, r9
	adds	r2, r5, #0
	ldrb	r0, [r3, #0]
	mov	r1, sl
	bl 0x0200bc24
	add	r8, r6
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02002ddc
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200ba54
.L_02002ddc:
	movs	r3, #4
	add	fp, r3
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02002d2e
.L_02002dee:
	ldr	r3, [pc, #60]
.L_02002df0:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bafc
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200ba9c
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_02002e16
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_02002e16:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
.L_02002e22:
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	adds	r5, r0, #0
	bl 0x0200bb8c
	cmp	r0, #0
	beq.n	.L_02002e46
	b.n	.L_02002fb6
.L_02002e46:
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bafc
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	bl 0x0200bc2c
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_02002e72
	b.n	.L_02002fb6
.L_02002e72:
	b.n	.L_02002fa8
.L_02002e74:
	ldrh	r7, [r5, #0]
	adds	r0, r7, #0
	bl 0x0200bafc
	cmp	r0, r8
	beq.n	.L_02002e84
	adds	r5, #4
	b.n	.L_02002fa8
.L_02002e84:
	ldrh	r5, [r5, #2]
	bl 0x0200baec
	adds	r0, r5, #0
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002ef2
	movs	r0, #125
	bl 0x0200bc54
	adds	r0, r7, #0
	bl 0x0200bafc
	movs	r1, #7
	bl 0x0200bb34
	movs	r0, #2
	bl 0x0200b9cc
	movs	r1, #0
	mov	r0, r8
	bl 0x0200ba54
	adds	r0, r7, #0
	bl 0x0200bafc
	movs	r1, #0
	bl 0x0200bb34
	movs	r0, #2
	bl 0x0200b9cc
	adds	r0, r7, #0
	bl 0x0200bafc
	movs	r1, #7
	bl 0x0200bb34
	movs	r0, #4
	bl 0x0200b9cc
	adds	r0, r7, #0
	bl 0x0200bafc
	movs	r1, #0
	bl 0x0200bb34
	movs	r0, #0
	bl 0x0200b4ac
	adds	r0, r5, #0
	bl 0x0200ba44
	b.n	.L_02002fa2
.L_02002ef2:
	adds	r5, #1
	mov	sl, r5
	mov	r0, sl
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_02002fa2
	adds	r6, #85
	strb	r0, [r6, #0]
	movs	r0, #185
	bl 0x0200bc54
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200bac4
	movs	r0, #0
	bl 0x0200b4ac
	movs	r5, #2
	movs	r0, #8
	mov	r7, r8
	bl 0x0200b9cc
	negs	r5, r5
.L_02002f2c:
	mov	r0, r8
	movs	r1, #2
	adds	r7, #34
	bl 0x0200ba54
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x0200b784
	movs	r0, #1
	bl 0x0200b4ac
	movs	r0, #16
	bl 0x0200b9cc
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x0200b784
	movs	r0, #4
	bl 0x0200b9cc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200bac4
	movs	r0, #8
	bl 0x0200b9cc
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #5
	bl 0x0200b9cc
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	mov	r0, r8
	bl 0x0200ba74
	movs	r0, #2
	bl 0x0200b9cc
	movs	r0, #188
	bl 0x0200bc54
	bl 0x0200b5e0
.L_02002f96:
	movs	r0, #20
	bl 0x0200b9cc
.L_02002f9c:
	mov	r0, sl
	bl 0x0200ba44
.L_02002fa2:
	bl 0x0200baf4
	b.n	.L_02002fb6
.L_02002fa8:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02002fb6
	b.n	.L_02002e74
.L_02002fb6:
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
.L_02002fd0:
	bl 0x0200bafc
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r7, r0, #0
	cmp	r3, r2
	beq.n	.L_0200309e
.L_02002fe2:
	ldrh	r3, [r5, #0]
	cmp	r3, r6
	beq.n	.L_02002fec
	adds	r5, #4
	b.n	.L_02003092
.L_02002fec:
	ldrh	r5, [r5, #2]
	bl 0x0200baec
	adds	r3, r5, #1
	mov	r8, r3
	mov	r0, r8
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_0200308c
	movs	r0, #185
	bl 0x0200bc54
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200bac4
	movs	r0, #0
	bl 0x0200b4ac
	movs	r0, #8
	bl 0x0200b9cc
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200ba54
	adds	r3, r7, #0
	adds	r3, #34
	movs	r0, #4
	ldrb	r1, [r3, #0]
	adds	r2, r6, #0
	negs	r0, r0
	bl 0x0200b6f8
	movs	r0, #1
	bl 0x0200b4ac
	movs	r0, #16
	bl 0x0200b9cc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200bac4
	movs	r0, #8
	bl 0x0200b9cc
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200ba74
	movs	r0, #2
	bl 0x0200b9cc
	movs	r0, #188
	bl 0x0200bc54
	bl 0x0200b5e0
	movs	r0, #20
	bl 0x0200b9cc
	adds	r0, r5, #0
	bl 0x0200ba44
	mov	r0, r8
	bl 0x0200ba44
.L_0200308c:
	bl 0x0200baf4
	b.n	.L_0200309e
.L_02003092:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02002fe2
.L_0200309e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200bafc
	movs	r3, #3
	adds	r0, #92
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200bb3c
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
.L_020030d4:
	str	r3, [sp, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	fp, r0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200bb14
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200bb14
	ldr	r1, [r5, #0]
	movs	r0, #10
	bl 0x0200bb14
	movs	r0, #1
	bl 0x0200b9cc
	movs	r0, #8
	bl 0x0200b0a4
	movs	r0, #9
	bl 0x0200b0a4
	movs	r0, #10
	bl 0x0200b0a4
	movs	r1, #0
	movs	r0, #9
	bl 0x0200bb1c
	movs	r0, #1
	bl 0x0200b9cc
	movs	r0, #8
	movs	r1, #0
.L_02003120:
	movs	r2, #0
	bl 0x0200bb0c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bb0c
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bb0c
	movs	r0, #1
	bl 0x0200b9cc
	b.n	.L_020031f6
.L_02003142:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	mov	r9, r3
	mov	r0, r9
	bl 0x0200bafc
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r5, r0, #0
	str	r2, [sp, #8]
	adds	r7, r5, #0
	adds	r7, #34
.L_0200315a:
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
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_02003198
	mov	r0, r9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bb0c
	b.n	.L_020031f2
.L_02003198:
	adds	r0, r5, #0
	bl 0x0200bc04
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r6, r6, r3
	str	r6, [sp, #4]
	add	r8, sl
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r7, #0]
	bl 0x0200badc
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	mov	sl, r0
	ldrb	r0, [r7, #0]
	bl 0x0200ba9c
	adds	r5, r0, #0
	mov	r0, r9
	bl 0x0200bb3c
	ldr	r6, [sp, #4]
	asrs	r5, r5, #19
	movs	r3, #128
	adds	r5, #4
	adds	r2, r5, #0
	strb	r3, [r6, #3]
	ldrb	r0, [r7, #0]
	mov	r1, sl
	bl 0x0200bc24
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200ba3c
	cmp	r0, #0
	beq.n	.L_020031f2
	mov	r0, r9
	movs	r1, #9
	bl 0x0200bb4c
.L_020031f2:
	movs	r3, #4
	add	fp, r3
.L_020031f6:
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02003142
	movs	r0, #10
	bl 0x0200b9cc
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
	bl 0x0200ba74
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200ba74
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl 0x0200bb8c
	cmp	r0, #0
	beq.n	.L_02003258
	b.n	.L_0200341a
.L_02003258:
	ldr	r3, [pc, #460]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bafc
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200bafc
	ldr	r3, [r5, #8]
	adds	r7, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl 0x0200bc2c
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_0200328c
	b.n	.L_0200341a
.L_0200328c:
	b.n	.L_0200340c
.L_0200328e:
	ldrh	r3, [r6, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200bafc
	cmp	r0, sl
	beq.n	.L_020032a0
	adds	r6, #4
	b.n	.L_0200340c
.L_020032a0:
	ldrh	r6, [r6, #2]
	bl 0x0200baec
	adds	r0, r6, #0
	bl 0x0200ba3c
	cmp	r0, #0
	bne.n	.L_0200333a
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200ba54
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200b224
	movs	r0, #1
	bl 0x0200b9cc
	movs	r0, #125
	bl 0x0200bc54
	movs	r0, #8
	bl 0x0200bafc
	movs	r1, #7
	bl 0x0200bb34
	movs	r0, #2
	bl 0x0200b9cc
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200ba54
	movs	r1, #9
	mov	r0, r8
	bl 0x0200bb4c
	movs	r0, #8
	bl 0x0200bafc
	movs	r1, #0
	bl 0x0200bb34
	movs	r0, #2
	bl 0x0200b9cc
	movs	r0, #8
	bl 0x0200bafc
	movs	r1, #7
	bl 0x0200bb34
	movs	r0, #4
	bl 0x0200b9cc
	movs	r0, #8
	bl 0x0200bafc
	movs	r1, #0
	bl 0x0200bb34
	movs	r0, #0
	bl 0x0200b4ac
	mov	r0, sl
	adds	r1, r7, #0
	bl 0x0200b224
	movs	r0, #1
	bl 0x0200b9cc
	adds	r0, r6, #0
	bl 0x0200ba44
	b.n	.L_02003406
.L_0200333a:
	adds	r6, #1
	mov	r9, r6
	mov	r0, r9
	bl 0x0200ba3c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02003406
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200ba54
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200b224
	adds	r5, #85
	movs	r0, #1
	bl 0x0200b9cc
	strb	r6, [r5, #0]
	movs	r0, #185
	bl 0x0200bc54
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200bac4
	movs	r0, #0
	bl 0x0200b4ac
	mov	r8, r5
	movs	r0, #8
	movs	r6, #2
	mov	r5, sl
	bl 0x0200b9cc
	negs	r6, r6
	adds	r0, r7, #0
	movs	r1, #2
	adds	r5, #34
	bl 0x0200ba54
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200b784
	movs	r0, #1
	bl 0x0200b4ac
	movs	r0, #16
	bl 0x0200b9cc
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200b784
	movs	r0, #4
	bl 0x0200b9cc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200bac4
	movs	r0, #8
	bl 0x0200b9cc
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r0, #5
	bl 0x0200b9cc
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200ba74
	movs	r0, #2
	bl 0x0200b9cc
	movs	r0, #188
	bl 0x0200bc54
	bl 0x0200b5e0
	movs	r0, #20
	bl 0x0200b9cc
	mov	r0, r9
	bl 0x0200ba44
.L_02003406:
	bl 0x0200baf4
	b.n	.L_0200341a
.L_0200340c:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_0200341a
	b.n	.L_0200328e
.L_0200341a:
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
	bl 0x0200b9c4
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_0200345c
	adds	r3, #15
.L_0200345c:
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
	bl 0x0200bafc
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
	bl 0x0200bafc
	movs	r2, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r2
.L_020034ce:
	bl 0x0200b9e4
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
	bl 0x0200ba64
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020035a2
	mov	r1, r9
	ldr	r0, [r6, #80]
	bl 0x0200bbd4
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r6, #0
	bl 0x0200bab4
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200ba54
	adds	r0, r6, #0
	ldr	r1, [pc, #152]
	bl 0x0200ba5c
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
	beq.n	.L_0200356c
	mov	r3, sl
	lsls	r5, r3, #13
	adds	r0, r5, #0
	bl 0x0200b9f4
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6470
	adds	r0, r5, #0
	bl 0x0200b9ec
	b.n	.L_02003570
.L_0200356c:
	mov	r0, r8
	str	r0, [r6, #68]
.L_02003570:
	str	r0, [r6, #76]
	bl 0x0200b9e4
	movs	r2, #192
	lsls	r0, r0, #14
	lsls	r2, r2, #7
	lsrs	r0, r0, #16
	adds	r0, r0, r2
	negs	r0, r0
	str	r0, [r6, #72]
	bl 0x0200b9e4
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
.L_0200359c:
	str	r3, [r6, #52]
	ldr	r3, [pc, #60]
	str	r3, [r6, #108]
.L_020035a2:
	movs	r0, #1
	add	sl, r0
	mov	r2, sl
	cmp	r2, #7
	bls.n	.L_020034ce
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0xfffe0000
	.4byte 0x0200be0c
	.4byte 0x0300021c
	.4byte 0x00013333
	.4byte 0xffffff00
	.4byte 0xfffff800
	.4byte 0xfffffa00
	.2byte 0xb42d
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
	bl 0x0200bafc
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020036d4
	movs	r3, #0
	mov	r9, r3
	mov	sl, r3
.L_02003604:
	movs	r0, #30
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, #255
	bl 0x0200ba64
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020036ca
	mov	r1, r9
	ldr	r0, [r7, #80]
	bl 0x0200bbd4
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
	bl 0x0200bab4
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200ba54
	ldr	r1, [pc, #160]
	adds	r0, r7, #0
	bl 0x0200ba5c
	mov	r3, sl
	lsls	r5, r3, #12
	adds	r0, r5, #0
	bl 0x0200b9f4
	mov	r4, r8
	str	r4, [r7, #72]
	str	r0, [r7, #68]
	adds	r0, r5, #0
	bl 0x0200b9ec
	ldr	r3, [r7, #68]
	str	r0, [r7, #76]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r7, #68]
	bl 0x0200b9e4
	lsls	r3, r0, #1
	ldr	r2, [r7, #68]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #108]
	adds	r2, r2, r3
	str	r2, [r7, #68]
	bl 0x0200b9e4
	lsls	r3, r0, #1
	ldr	r2, [r7, #76]
	adds	r3, r3, r0
	ldr	r4, [pc, #96]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r7, #76]
	bl 0x0200b9e4
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
.L_020036ca:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02003604
.L_020036d4:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200be3c
	.4byte 0xffffa000
	.4byte 0xffffd000
	.4byte 0xfffff800
	.2byte 0xb42d
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	adds	r0, r2, #0
	adds	r5, r1, #0
	bl 0x0200bafc
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
	bl 0x0200badc
	ldr	r2, [r7, #16]
	mov	r8, r0
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200ba9c
	ldr	r3, [r7, #8]
	asrs	r2, r0, #19
	add	r2, sl
	cmp	r3, #0
	bge.n	.L_02003752
	ldr	r1, [pc, #48]
	adds	r3, r3, r1
.L_02003752:
	ldr	r0, [r7, #16]
	asrs	r1, r3, #20
	cmp	r0, #0
	bge.n	.L_0200375e
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L_0200375e:
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r0, [r5, #0]
	mov	r1, r8
	adds	r6, r6, r3
	bl 0x0200bc24
	strb	r0, [r6, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
.L_02003776:
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
	bl 0x0200b6f8
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
.L_020037b0:
	adds	r5, r7, r2
	lsls	r3, r3, #4
	movs	r2, #63
	adds	r6, r7, r3
	mov	r8, r2
.L_020037ba:
	ldr	r3, [r5, #24]
	cmp	r3, #19
	bhi.n	.L_02003808
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
	bl 0x0200bc3c
.L_020037ea:
	adds	r0, r5, #0
	movs	r1, #63
	ldr	r2, [pc, #20]
	bl 0x0200bc44
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	b.n	.L_02003808
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x8000
	.2byte 0xffff
.L_02003808:
	.2byte 0x2301
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r2, #0
	bge.n	.L_020037ba
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
	ble.n	.L_020038cc
	adds	r7, r2, #0
.L_02003848:
	bl 0x0200b9e4
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
	bl 0x0200b9e4
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r0, r0, #3
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r5, #0
	bl 0x0200b9fc
	mov	r3, r8
	str	r3, [r5, #12]
	movs	r3, #160
	lsls	r3, r3, #11
	mov	r1, r8
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	bl 0x0200b9e4
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
	bl 0x0200b9fc
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #63
	adds	r3, #1
	ands	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	cmp	r7, #0
	bne.n	.L_02003848
.L_020038cc:
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
	bl 0x0200ba04
	adds	r6, r0, #0
	ldr	r0, [pc, #152]
	bl 0x0200ba34
	adds	r1, r6, #0
	bl 0x0200ba14
	bl 0x0200ba2c
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r2, r6, #0
	adds	r5, r0, #0
	bl 0x0200ba24
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
.L_02003934:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl 0x0200bc34
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
	bge.n	.L_02003934
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r2, r6, r1
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200b9d4
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001f0
	.2byte 0xb79d
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl 0x0200b9dc
	movs	r3, #176
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl 0x0200ba1c
	movs	r0, #220
	bl 0x0200ba0c
	pop	{r5, pc}
	.4byte 0x0200b79d
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000129, 0x08000141, 0x08000151, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x08000291, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020179, 0x080201c1, 0x080201e9, 0x08020211, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x080202c1, 0x080202f1, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c80d1, 0x080c80f9, 0x080c8111, 0x080c8119, 0x080c8121, 0x080c8131, 0x080c8171, 0x080c8209, 0x080c8219, 0x080c8221, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8251, 0x080c8279, 0x080c8289, 0x080c82e1, 0x080c83a9, 0x080c83b9, 0x080c8499, 0x080c84a1, 0x080c84a9, 0x080c84b1, 0x080c84b9, 0x080c84e1, 0x080c8519, 0x080c8691, 0x080c86a9, 0x080c86d1, 0x080c86d9, 0x080c86e9, 0x080c8711, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c8731, 0x080c8739, 0x080c87c1, 0x080c87d1, 0x080c87e1, 0x080c88c9, 0x081c0011
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
	.4byte 0x000a0008
	.4byte 0x0009ffff
	.4byte 0x00140032
	.4byte 0x001e000c
	.4byte 0xffffffff
	.4byte 0x000f0008
	.4byte 0x000dffff
	.4byte 0xffff001e
	.4byte 0x0009ffff
	.4byte 0x000a0200
	.4byte 0xffff0202
	.4byte 0x0032000e
	.4byte 0x0011000f
	.4byte 0x00120033
	.4byte 0x0010ffff
	.4byte 0x0009ffff
	.4byte 0x000a0032
	.4byte 0x0033000b
	.4byte 0xffff000c
	.4byte 0xffff0008
	.4byte 0x00090008
	.4byte 0x000bffff
	.4byte 0xffff001e
	.4byte 0x0032000c
	.4byte 0x000f000d
	.4byte 0xffff001f
	.4byte 0x0009ffff
	.4byte 0xffff0032
	.4byte 0x0000ffff
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
	.4byte 0x0200bc5c
	.4byte 0x0200bc98
	.4byte 0x0200bcd4
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
	.4byte 0x014800f0
	.4byte 0x01000160
	.4byte 0x01700158
	.4byte 0x0003ffff
	.4byte 0x014801c0
	.4byte 0x01d00160
	.4byte 0x01700158
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00380110
	.4byte 0x01200240
	.4byte 0x02500048
	.4byte 0x0001ffff
	.4byte 0x00280160
	.4byte 0x01700230
	.4byte 0x02400038
	.4byte 0x0002ffff
	.4byte 0x020002f0
	.4byte 0x03000218
	.4byte 0x02280210
	.4byte 0x0005ffff
	.4byte 0x00500310
	.4byte 0x03200250
	.4byte 0x02600060
	.4byte 0x0007ffff
	.4byte 0x02000380
	.4byte 0x03900210
	.4byte 0x02200210
	.4byte 0x0008ffff
	.4byte 0x00300110
	.4byte 0x01200240
	.4byte 0x02500040
	.4byte 0x0009ffff
	.4byte 0x00200160
	.4byte 0x01700230
	.4byte 0x02400030
	.4byte 0x000affff
	.4byte 0x00380310
	.4byte 0x03200250
	.4byte 0x02600048
	.4byte 0x000effff
	.4byte 0xfea802f0
	.4byte 0x030002a0
	.4byte 0x02b0feb8
	.4byte 0x0010ffff
	.4byte 0xfea803d0
	.4byte 0x03e002a0
	.4byte 0x02b0feb8
	.4byte 0x0011ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc800f0
	.4byte 0x010001c0
	.4byte 0x01d0ffd8
	.4byte 0x0001ffff
	.4byte 0xffc80180
	.4byte 0x019001c0
	.4byte 0x01d0ffd8
	.4byte 0x0002ffff
	.4byte 0xff8001a0
	.4byte 0x01b001c0
	.4byte 0x01d0ff90
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000df
	.4byte 0x00133002
	.4byte 0x0020b0e8
	.4byte 0x003100e0
	.4byte 0x004110e0
	.4byte 0x000000e0
	.4byte 0x001090e0
	.4byte 0x0020a0e0
	.4byte 0x003040e0
	.4byte 0x004030e0
	.4byte 0x005010e1
	.4byte 0x006020e1
	.4byte 0x0070e0e0
	.4byte 0x008020e1
	.4byte 0x009010e0
	.4byte 0x00a020e0
	.4byte 0x00b0d0e0
	.4byte 0x00c0f0e0
	.4byte 0x00d0b0e0
	.4byte 0x00e070e0
	.4byte 0x00f0c0e0
	.4byte 0x010030df
	.4byte 0x011040df
	.4byte 0x000000e1
	.4byte 0x001050e0
	.4byte 0x002080e0
	.4byte 0x003080e3
	.4byte 0x004060e0
	.4byte 0x000001ff
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
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
	.4byte 0x0202c000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
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
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0193
	.4byte 0x00000007
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
	.4byte 0x0202c000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
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
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000008
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
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0193
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00028000
	.4byte 0xffff0193
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00028000
	.4byte 0xffff0193
	.4byte 0x00000007
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
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000012
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
	.4byte 0x00000013
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000014
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0180
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000ca02
	.4byte 0x02f80002
	.4byte 0x02009b45
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02009b25
	.4byte 0x00008f15
	.4byte 0xffff0013
	.4byte 0x00000000
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008941
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a49
	.4byte 0x10008c15
	.4byte 0x08d50008
	.4byte 0x02008941
	.4byte 0x00008c15
	.4byte 0x08d50008
	.4byte 0x02008a49
	.4byte 0x10008c15
	.4byte 0x08d6000a
	.4byte 0x02008941
	.4byte 0x00008c15
	.4byte 0x08d6000a
	.4byte 0x02008a49
	.4byte 0x50009705
	.4byte 0x0200001f
	.4byte 0x020099e9
	.4byte 0x50009705
	.4byte 0x02010020
	.4byte 0x02009a39
	.4byte 0x50009705
	.4byte 0x02020021
	.4byte 0x02009a8d
	.4byte 0x50009705
	.4byte 0x02030022
	.4byte 0x02009ac1
	.4byte 0x50009705
	.4byte 0x02040023
	.4byte 0x02009af5
	.4byte 0x80009705
	.4byte 0xffff001e
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0x02f0001e
	.4byte 0x02009111
	.4byte 0x50009705
	.4byte 0xffff001e
	.4byte 0x020094bd
	.4byte 0x00009705
	.4byte 0xffff001e
	.4byte 0x02009445
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x02009111
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte 0x0200948d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009bb1
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008941
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a49
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008941
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008a49
	.4byte 0x10008c15
	.4byte 0x08d7000f
	.4byte 0x02008941
	.4byte 0x00008c15
	.4byte 0x08d7000f
	.4byte 0x02008a49
	.4byte 0x50009705
	.4byte 0x0200001f
	.4byte 0x020099e9
	.4byte 0x00000002
	.4byte 0x02120032
	.4byte 0x02009bc1
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x02009be1
	.4byte 0x80009705
	.4byte 0xffff001e
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0x02f0001e
	.4byte 0x02009111
	.4byte 0x50009705
	.4byte 0xffff001e
	.4byte 0x020094bd
	.4byte 0x00009705
	.4byte 0xffff001e
	.4byte 0x02009445
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008941
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a49
	.4byte 0x10008c15
	.4byte 0x08d80010
	.4byte 0x02008941
	.4byte 0x00008c15
	.4byte 0x08d80010
	.4byte 0x02008a49
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x02009111
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte 0x0200948d
	.4byte 0x80009705
	.4byte 0xffff0033
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0xffff0033
	.4byte 0x02009111
	.4byte 0x00009705
	.4byte 0xffff0033
	.4byte 0x0200948d
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02009c01
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000602
	.4byte 0x00080028
	.4byte 0x02009e09
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008941
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a49
	.4byte 0x10008c15
	.4byte 0x08d90008
	.4byte 0x02008941
	.4byte 0x00008c15
	.4byte 0x08d90008
	.4byte 0x02008a49
	.4byte 0x00008515
	.4byte 0x0250000e
	.4byte 0x00000000
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x02009111
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte 0x0200948d
	.4byte 0x80009705
	.4byte 0xffff0033
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0xffff0033
	.4byte 0x02009111
	.4byte 0x00009705
	.4byte 0xffff0033
	.4byte 0x0200948d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00008602
	.4byte 0xffff0028
	.4byte 0x02009edd
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008941
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008a49
	.4byte 0x10008c15
	.4byte 0x08da0008
	.4byte 0x02008941
	.4byte 0x00008c15
	.4byte 0x08da0008
	.4byte 0x02008a49
	.4byte 0x10008c15
	.4byte 0x08db0009
	.4byte 0x02008941
	.4byte 0x00008c15
	.4byte 0x08db0009
	.4byte 0x02008a49
	.4byte 0x80009705
	.4byte 0xffff001e
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0x02f0001e
	.4byte 0x02009111
	.4byte 0x50009705
	.4byte 0xffff001e
	.4byte 0x020094bd
	.4byte 0x00009705
	.4byte 0xffff001e
	.4byte 0x02009445
	.4byte 0x80009705
	.4byte 0xffff001f
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0x02f0001f
	.4byte 0x02009111
	.4byte 0x50009705
	.4byte 0xffff001f
	.4byte 0x020094bd
	.4byte 0x00009705
	.4byte 0xffff001f
	.4byte 0x02009445
	.4byte 0x50009705
	.4byte 0x02010020
	.4byte 0x02009a39
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x02009111
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte 0x0200948d
	.4byte 0x00000c15
	.4byte 0x02500010
	.4byte 0x02009fb1
	.4byte 0x00000c15
	.4byte 0x02510011
	.4byte 0x02009fd5
	.4byte 0x00000c15
	.4byte 0x02520012
	.4byte 0x02009ffd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte 0x02009ba5
	.4byte 0x50009705
	.4byte 0x02f00032
	.4byte 0x02009111
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x0200a0f9
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte 0x02009445
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
