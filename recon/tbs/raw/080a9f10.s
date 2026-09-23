.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08004458, 0x08004458
	.set sub_08015120, 0x08015120
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077080, 0x08077080
	.set sub_08077128, 0x08077128
	.set sub_08077190, 0x08077190
	.global BattleEffect_ApplyToTargets
	.global Func_080a9f10
	.thumb_func
BattleEffect_ApplyToTargets:
Func_080a9f10:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r1, [sp, #12]
	mov	sl, r2
	str	r3, [sp, #8]
	str	r0, [sp, #16]
	bl	sub_08077080
	ldr	r3, [pc, #864]
	ldr	r3, [r3, #0]
	mov	fp, r0
	movs	r1, #0
	movs	r0, #0
	mov	r2, sl
	str	r3, [sp, #4]
	mov	r9, r0
	movs	r7, #0
	mov	r8, r1
	cmp	r2, #9
	beq.n	.L_080a9f48
	mov	r0, sl
	b.n	.L_080a9f4a
.L_080a9f48:
	movs	r0, #0
.L_080a9f4a:
	bl	sub_08077008
	adds	r5, r0, #0
	mov	r3, fp
	movs	r4, #0
	ldr	r0, [sp, #4]
	ldr	r1, [pc, #828]
	ldrb	r2, [r3, #8]
	str	r4, [sp, #0]
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L_080a9f66
	b.n	.L_080aa3d6
.L_080a9f66:
	adds	r3, r2, #0
	cmp	r3, #255
	bne.n	.L_080a9f84
	ldr	r2, [sp, #0]
	movs	r4, #130
	ldr	r0, [sp, #4]
	lsls	r3, r2, #1
	lsls	r4, r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r0, r3]
	mov	sl, r3
	mov	r0, sl
	bl	sub_08077008
	adds	r5, r0, #0
.L_080a9f84:
	mov	r1, fp
	ldrb	r2, [r1, #1]
	movs	r3, #15
	ands	r3, r2
	subs	r3, #1
	ldrh	r6, [r1, #10]
	cmp	r3, #10
	bls.n	.L_080a9f96
	b.n	.L_080aa178
.L_080a9f96:
	ldr	r2, [pc, #768]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080a9fcc
	.4byte 0x080aa178
	.4byte 0x080aa178
	.4byte 0x080aa178
	.4byte 0x080aa178
	.4byte 0x080aa178
	.4byte 0x080aa178
	.4byte 0x080aa178
	.4byte 0x080aa066
	.4byte 0x080aa178
	.2byte 0xa122
	.2byte 0x080a
	ldr	r2, [sp, #8]
	cmp	r2, #0
	bne.n	.L_080a9ffa
	mov	r4, fp
	ldrb	r3, [r4, #2]
	cmp	r3, #4
	beq.n	.L_080a9fec
	ldr	r0, [sp, #12]
	bl	sub_08077008
	mov	r1, fp
	ldrb	r3, [r1, #2]
	lsls	r3, r3, #2
	adds	r3, #72
	ldrsh	r1, [r0, r3]
	b.n	.L_080a9fee
.L_080a9fec:
	movs	r1, #100
.L_080a9fee:
	movs	r2, #128
	adds	r0, r6, #0
	lsls	r2, r2, #1
	bl	sub_08077190
	adds	r6, r0, #0
.L_080a9ffa:
	movs	r4, #56
	ldrsh	r1, [r5, r4]
	ldrh	r3, [r5, #56]
	cmp	r1, #0
	bgt.n	.L_080aa010
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L_080aa00c
	b.n	.L_080aa178
.L_080aa00c:
	movs	r7, #2
	b.n	.L_080aa178
.L_080aa010:
	movs	r4, #52
	ldrsh	r2, [r5, r4]
	ldrh	r0, [r5, #52]
	cmp	r1, r2
	bne.n	.L_080aa026
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L_080aa022
	b.n	.L_080aa178
.L_080aa022:
	movs	r7, #4
	b.n	.L_080aa178
.L_080aa026:
	adds	r3, r3, r6
	strh	r3, [r5, #56]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_080aa042
	subs	r3, r3, r2
	mov	r1, r8
	subs	r6, r6, r3
	strh	r0, [r5, #56]
	cmp	r1, #0
	bne.n	.L_080aa04a
	movs	r7, #0
	b.n	.L_080aa04a
.L_080aa042:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080aa04a
	movs	r7, #1
.L_080aa04a:
	mov	r0, sl
	bl	sub_08077128
	movs	r3, #1
	mov	r4, fp
	mov	r9, r3
	ldrb	r3, [r4, #8]
	cmp	r3, #255
	beq.n	.L_080aa05e
	b.n	.L_080aa178
.L_080aa05e:
	movs	r0, #1
	mov	r8, r0
	movs	r7, #3
	b.n	.L_080aa178
	bl	sub_08004458
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_080aa078
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080aa086
.L_080aa078:
	movs	r1, #1
	adds	r2, r1, #0
	eors	r2, r0
	negs	r3, r2
	orrs	r3, r2
	lsrs	r0, r3, #31
	subs	r0, r1, r0
.L_080aa086:
	ldr	r3, [pc, #532]
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #532]
	ands	r3, r1
	adds	r3, r3, r2
	cmp	r3, #5
	bhi.n	.L_080aa178
	ldr	r2, [pc, #524]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080aa0b4
	.4byte 0x080aa0c4
	.4byte 0x080aa0f2
	.4byte 0x080aa10a
	.4byte 0x080aa0d4
	.2byte 0xa0e4
	.2byte 0x080a
	ldrh	r3, [r5, #16]
	adds	r2, r6, r0
	adds	r3, r3, r2
	strh	r3, [r5, #16]
	movs	r3, #1
	movs	r7, #16
	mov	r9, r3
	b.n	.L_080aa178
	ldrh	r3, [r5, #18]
	adds	r2, r6, r0
	adds	r3, r3, r2
	movs	r4, #1
	strh	r3, [r5, #18]
	movs	r7, #17
	mov	r9, r4
	b.n	.L_080aa178
	ldrh	r3, [r5, #28]
	adds	r2, r6, r0
	adds	r3, r3, r2
	movs	r0, #1
	strh	r3, [r5, #28]
	movs	r7, #18
	mov	r9, r0
	b.n	.L_080aa178
	ldrb	r3, [r5, #30]
	movs	r1, #1
	adds	r3, r3, r6
	strb	r3, [r5, #30]
	movs	r7, #19
	mov	r9, r1
	b.n	.L_080aa178
	ldrh	r3, [r5, #24]
	adds	r2, r6, r0
	adds	r3, r3, r2
	strh	r3, [r5, #24]
	movs	r0, #3
	movs	r1, #5
	bl	sub_08015120
	movs	r2, #1
	movs	r7, #20
	mov	r9, r2
	b.n	.L_080aa178
	ldrh	r3, [r5, #26]
	adds	r2, r6, r0
	adds	r3, r3, r2
	strh	r3, [r5, #26]
	movs	r0, #4
	movs	r1, #5
	bl	sub_08015120
	movs	r3, #1
	movs	r7, #21
	mov	r9, r3
	b.n	.L_080aa178
	movs	r4, #58
	ldrsh	r3, [r5, r4]
	movs	r4, #54
	ldrsh	r2, [r5, r4]
	ldrh	r1, [r5, #58]
	ldrh	r0, [r5, #54]
	cmp	r3, r2
	bne.n	.L_080aa13c
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_080aa178
	movs	r7, #7
	b.n	.L_080aa178
.L_080aa13c:
	adds	r3, r1, r6
	strh	r3, [r5, #58]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_080aa158
	subs	r3, r3, r2
	mov	r1, r8
	subs	r6, r6, r3
	strh	r0, [r5, #58]
	cmp	r1, #0
	bne.n	.L_080aa160
	movs	r7, #5
	b.n	.L_080aa160
.L_080aa158:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080aa160
	movs	r7, #6
.L_080aa160:
	mov	r0, sl
	bl	sub_08077128
	movs	r3, #1
	mov	r4, fp
	mov	r9, r3
	ldrb	r3, [r4, #8]
	cmp	r3, #255
	bne.n	.L_080aa178
	movs	r0, #1
	mov	r8, r0
	movs	r7, #8
.L_080aa178:
	mov	r1, fp
	ldrb	r3, [r1, #3]
	subs	r3, #1
	cmp	r3, #56
	bls.n	.L_080aa184
	b.n	.L_080aa3b0
.L_080aa184:
	ldr	r2, [pc, #288]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080aa270
	.4byte 0x080aa2ce
	.4byte 0x080aa38a
	.4byte 0x080aa3b0
	.4byte 0x080aa314
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa3b0
	.4byte 0x080aa33e
	.2byte 0xa358
	.2byte 0x080a
	movs	r4, #56
	ldrsh	r2, [r5, r4]
	ldrh	r3, [r5, #56]
	cmp	r2, #0
	ble.n	.L_080aa284
	movs	r4, #52
	ldrsh	r1, [r5, r4]
	ldrh	r0, [r5, #52]
	cmp	r2, r1
	bne.n	.L_080aa2ac
.L_080aa284:
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L_080aa28c
	b.n	.L_080aa3b0
.L_080aa28c:
	movs	r7, #2
	b.n	.L_080aa3b0
	.4byte 0x03001f2c
	.4byte 0x00000219
	.4byte 0x080a9fa0
	.4byte 0x00003fff
	.4byte 0xfffffefc
	.4byte 0x080aa09c
	.2byte 0xa18c
	.2byte 0x080a
.L_080aa2ac:
	adds	r3, r3, r6
	strh	r3, [r5, #56]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r1
	ble.n	.L_080aa2c4
	mov	r1, r8
	strh	r0, [r5, #56]
	cmp	r1, #0
	bne.n	.L_080aa308
	movs	r7, #0
	b.n	.L_080aa308
.L_080aa2c4:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080aa308
	movs	r7, #1
	b.n	.L_080aa308
	movs	r4, #58
	ldrsh	r3, [r5, r4]
	movs	r4, #54
	ldrsh	r2, [r5, r4]
	ldrh	r1, [r5, #58]
	ldrh	r0, [r5, #54]
	cmp	r3, r2
	bne.n	.L_080aa2e8
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_080aa3b0
	movs	r7, #7
	b.n	.L_080aa3b0
.L_080aa2e8:
	adds	r3, r1, r6
	strh	r3, [r5, #58]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_080aa300
	mov	r1, r8
	strh	r0, [r5, #58]
	cmp	r1, #0
	bne.n	.L_080aa308
	movs	r7, #5
	b.n	.L_080aa308
.L_080aa300:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080aa308
	movs	r7, #6
.L_080aa308:
	mov	r0, sl
	bl	sub_08077128
	movs	r3, #1
	mov	r9, r3
	b.n	.L_080aa3b0
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	bne.n	.L_080aa334
	ldrh	r3, [r5, #52]
	mov	r0, sl
	strh	r3, [r5, #56]
	bl	sub_08077128
	movs	r0, #1
	mov	r1, r8
	mov	r9, r0
	cmp	r1, #0
	bne.n	.L_080aa3b0
	movs	r7, #12
	b.n	.L_080aa3b0
.L_080aa334:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080aa3b0
	movs	r7, #13
	b.n	.L_080aa3b0
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	bne.n	.L_080aa380
	ldrh	r3, [r5, #52]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	mov	r0, sl
	strh	r2, [r5, #56]
	b.n	.L_080aa372
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_080aa380
	movs	r4, #52
	ldrsh	r3, [r5, r4]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	sub_080022ec
	strh	r0, [r5, #56]
	mov	r0, sl
.L_080aa372:
	bl	sub_08077128
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_080aa3b0
	movs	r7, #12
	b.n	.L_080aa3b0
.L_080aa380:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_080aa3b0
	movs	r7, #13
	b.n	.L_080aa3b0
	ldr	r3, [pc, #176]
	adds	r2, r5, r3
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080aa3a8
	movs	r3, #0
	movs	r4, #1
	mov	r0, r8
	strb	r3, [r2, #0]
	mov	r9, r4
	cmp	r0, #0
	bne.n	.L_080aa3b0
	movs	r7, #10
	b.n	.L_080aa3b0
.L_080aa3a8:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_080aa3b0
	movs	r7, #11
.L_080aa3b0:
	mov	r2, fp
	ldrb	r3, [r2, #8]
	adds	r2, r3, #0
	adds	r3, r2, #0
	cmp	r3, #255
	bne.n	.L_080aa3d6
	ldr	r3, [sp, #0]
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	ldr	r4, [sp, #4]
	ldr	r0, [pc, #120]
	str	r3, [sp, #0]
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	ldr	r1, [sp, #0]
	cmp	r1, r3
	bcs.n	.L_080aa3d6
	b.n	.L_080a9f66
.L_080aa3d6:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_080aa3ea
	ldr	r4, [sp, #4]
	ldr	r0, [pc, #100]
	adds	r3, r4, r0
	movs	r0, #1
	strh	r7, [r3, #0]
	negs	r0, r0
	b.n	.L_080aa42a
.L_080aa3ea:
	movs	r1, #0
	ldr	r2, [sp, #4]
	ldr	r4, [pc, #80]
	str	r1, [sp, #0]
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	cmp	r1, r3
	bcs.n	.L_080aa420
	adds	r5, r2, r4
.L_080aa3fc:
	ldr	r0, [sp, #0]
	movs	r1, #130
	lsls	r3, r0, #1
	lsls	r1, r1, #2
	ldr	r2, [sp, #4]
	adds	r3, r3, r1
	ldrh	r0, [r2, r3]
	bl	sub_08077010
	ldr	r3, [sp, #0]
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	str	r3, [sp, #0]
	ldr	r4, [sp, #0]
	ldrb	r3, [r5, #0]
	cmp	r4, r3
	bcc.n	.L_080aa3fc
.L_080aa420:
	ldr	r0, [sp, #4]
	ldr	r1, [pc, #32]
	adds	r3, r0, r1
	strh	r7, [r3, #0]
	movs	r0, #0
.L_080aa42a:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000131
	.4byte 0x00000219
	.4byte 0x0000025a
