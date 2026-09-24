.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002322, 0x08002322
	.set sub_080030f8, 0x080030f8
	.set sub_08003d28, 0x08003d28
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_080040b4, 0x080040b4
	.set sub_080040d0, 0x080040d0
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_0801671c, 0x0801671c
	.set sub_08017a64, 0x08017a64
	.set sub_08017aa4, 0x08017aa4
	.set sub_0801965c, 0x0801965c
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801e8b0, 0x0801e8b0
	.set sub_0801ea08, 0x0801ea08
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080b50b8, 0x080b50b8
	.set sub_080b50e0, 0x080b50e0
	.set sub_080f9010, 0x080f9010
	.global BattleTarget_RunSelection
	.global Func_08026080
	.thumb_func
BattleTarget_RunSelection:
Func_08026080:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #324
	str	r2, [sp, #84]
	str	r3, [sp, #80]
	ldr	r5, [pc, #272]
	mov	sl, r0
	ldr	r0, [r5, #0]
	ldr	r2, [pc, #272]
	str	r0, [sp, #76]
	movs	r0, #128
	mov	r8, r1
	lsls	r0, r0, #1
	movs	r1, #0
	str	r1, [sp, #64]
	str	r2, [sp, #56]
	bl	sub_080040b4
	ldr	r4, [sp, #84]
	movs	r3, #0
	str	r0, [sp, #52]
	mov	r9, r3
	cmp	r4, #0
	bne.n	.L_080260be
	movs	r6, #1
	str	r6, [sp, #84]
.L_080260be:
	mov	r7, r8
	cmp	r7, #2
	beq.n	.L_080260c8
	cmp	r7, #4
	bne.n	.L_080260d4
.L_080260c8:
	adds	r3, r5, #0
	adds	r3, #192
	ldr	r2, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	b.n	.L_080260dc
.L_080260d4:
	adds	r3, r5, #0
	adds	r3, #192
	ldr	r2, [r3, #0]
	movs	r3, #16
.L_080260dc:
	str	r3, [r2, #40]
	mov	r0, sp
	adds	r0, #212
	mov	r3, sp
	str	r0, [sp, #36]
	movs	r2, #0
	movs	r7, #5
	adds	r3, #234
.L_080260ec:
	subs	r7, #1
	strb	r2, [r3, #0]
	subs	r3, #4
	cmp	r7, #0
	bge.n	.L_080260ec
	movs	r1, #1
	negs	r1, r1
	mov	r2, r8
	str	r1, [sp, #68]
	cmp	r2, #2
	bne.n	.L_0802613e
	ldr	r4, [sp, #76]
	movs	r3, #88
	ldrsh	r3, [r4, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L_08026194
	movs	r6, #154
	lsls	r6, r6, #1
	ldr	r0, [sp, #64]
	add	r6, sp
	adds	r2, r4, #0
	lsls	r3, r0, #1
	str	r6, [sp, #28]
	adds	r2, #88
	adds	r1, r3, r6
.L_08026120:
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldr	r3, [sp, #64]
	adds	r7, #1
	adds	r3, #1
	adds	r1, #2
	str	r3, [sp, #64]
	adds	r2, #2
	cmp	r7, #5
	bgt.n	.L_080261b8
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #255
	bne.n	.L_08026120
	b.n	.L_080261b8
.L_0802613e:
	mov	r5, r8
	cmp	r5, #4
	bne.n	.L_08026158
	movs	r6, #154
	lsls	r6, r6, #1
	add	r6, sp
	mov	r7, sl
	adds	r0, r6, #0
	movs	r1, #1
	str	r6, [sp, #28]
	strh	r7, [r0, #0]
	str	r1, [sp, #64]
	b.n	.L_080261b8
.L_08026158:
	ldr	r1, [sp, #76]
	movs	r3, #100
	adds	r1, #2
	ldrsh	r3, [r1, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L_080261b0
	movs	r3, #154
	lsls	r3, r3, #1
	add	r3, sp
	str	r3, [sp, #28]
	ldr	r4, [sp, #64]
	ldr	r5, [sp, #28]
	lsls	r3, r4, #1
	movs	r0, #100
	adds	r2, r3, r5
.L_08026178:
	ldrh	r3, [r1, r0]
	strh	r3, [r2, #0]
	ldr	r6, [sp, #64]
	adds	r7, #1
	adds	r6, #1
	adds	r2, #2
	str	r6, [sp, #64]
	adds	r0, #2
	cmp	r7, #5
	bgt.n	.L_080261b8
	ldrsh	r3, [r1, r0]
	cmp	r3, #255
	bne.n	.L_08026178
	b.n	.L_080261b8
.L_08026194:
	movs	r5, #154
	lsls	r5, r5, #1
	add	r5, sp
	str	r5, [sp, #28]
	b.n	.L_080261b8
.L_0802619e:
	ldr	r6, [sp, #28]
	mov	r7, fp
	ldrh	r6, [r6, r7]
	mov	sl, r6
	b.n	.L_080262b6
	.4byte 0x03001e74
	.2byte 0xffff
	.2byte 0x0000
.L_080261b0:
	movs	r0, #154
	lsls	r0, r0, #1
	add	r0, sp
	str	r0, [sp, #28]
.L_080261b8:
	ldr	r1, [sp, #64]
	ldr	r3, [pc, #44]
	ldr	r4, [sp, #28]
	lsls	r2, r1, #1
	mov	r5, r8
	strh	r3, [r4, r2]
	str	r1, [sp, #60]
	cmp	r5, #2
	beq.n	.L_080261cc
	b.n	.L_080262e0
.L_080261cc:
	ldr	r6, [sp, #84]
	cmp	r6, #255
	beq.n	.L_080262b6
	ldr	r7, [sp, #80]
	cmp	r7, #0
	beq.n	.L_080262b6
	movs	r5, #0
	movs	r7, #0
	cmp	r5, r1
	bge.n	.L_080262b6
	ldr	r4, [pc, #8]
	movs	r6, #0
	b.n	.L_080261f0
	movs	r0, r0
	.4byte 0x000000ff
	.2byte 0xffff
	.2byte 0x0000
.L_080261f0:
	ldr	r0, [sp, #28]
	ldrh	r3, [r6, r0]
	mov	fp, r6
	cmp	r3, #254
	beq.n	.L_080262ac
	adds	r0, r3, #0
	str	r4, [sp, #8]
	bl	sub_08077008
	ldr	r2, [sp, #80]
	adds	r1, r0, #0
	ldr	r4, [sp, #8]
	cmp	r2, #4
	beq.n	.L_08026240
	cmp	r2, #4
	bhi.n	.L_08026216
	cmp	r2, #3
	beq.n	.L_08026234
	b.n	.L_080262a6
.L_08026216:
	ldr	r3, [sp, #80]
	cmp	r3, #5
	beq.n	.L_08026222
	cmp	r3, #6
	beq.n	.L_08026264
	b.n	.L_080262a6
.L_08026222:
	movs	r0, #56
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_080262a6
	ldr	r1, [sp, #28]
	ldrh	r1, [r6, r1]
	movs	r7, #1
	mov	sl, r1
	b.n	.L_080262a6
.L_08026234:
	ldr	r2, [pc, #324]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	b.n	.L_080262a0
.L_08026240:
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #308]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080262a4
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080262a4
	adds	r0, #9
	adds	r3, r1, r0
	b.n	.L_0802629e
.L_08026264:
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #272]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080262a4
	movs	r0, #158
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldrh	r3, [r3, #0]
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080262a4
	ldr	r2, [pc, #256]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080262a4
	subs	r0, #11
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080262a4
	subs	r2, #1
	adds	r3, r1, r2
.L_0802629e:
	ldrb	r3, [r3, #0]
.L_080262a0:
	cmp	r3, #0
	beq.n	.L_080262a6
.L_080262a4:
	movs	r7, #1
.L_080262a6:
	cmp	r7, #0
	beq.n	.L_080262ac
	b.n	.L_0802619e
.L_080262ac:
	ldr	r3, [sp, #64]
	adds	r5, #1
	adds	r6, #2
	cmp	r5, r3
	blt.n	.L_080261f0
.L_080262b6:
	ldr	r4, [sp, #64]
	movs	r5, #0
	cmp	r5, r4
	bge.n	.L_080262d8
	ldr	r6, [sp, #28]
	ldrh	r3, [r6, #0]
	cmp	r3, sl
	beq.n	.L_080262d8
	adds	r2, r6, #0
.L_080262c8:
	ldr	r7, [sp, #64]
	adds	r5, #1
	cmp	r5, r7
	bge.n	.L_080262d8
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, sl
	bne.n	.L_080262c8
.L_080262d8:
	ldr	r0, [sp, #64]
	cmp	r5, r0
	beq.n	.L_080262e0
	str	r5, [sp, #68]
.L_080262e0:
	ldr	r1, [sp, #68]
	cmp	r1, #0
	bge.n	.L_08026308
	ldr	r3, [sp, #64]
	subs	r3, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #68]
	b.n	.L_08026308
.L_080262f4:
	ldr	r2, [sp, #68]
	ldr	r4, [sp, #64]
	adds	r3, r2, r4
	subs	r3, #1
	adds	r0, r3, #0
	adds	r1, r4, #0
	str	r3, [sp, #68]
	bl	sub_080022fc
	str	r0, [sp, #68]
.L_08026308:
	ldr	r5, [sp, #68]
	lsls	r5, r5, #1
	str	r5, [sp, #24]
	ldr	r6, [sp, #28]
	ldrh	r3, [r6, r5]
	cmp	r3, #254
	beq.n	.L_080262f4
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_08026336
	mov	r7, r8
	cmp	r7, #1
	bne.n	.L_08026336
	ldrh	r0, [r6, r5]
	bl	sub_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_080262f4
.L_08026336:
	mov	r2, r8
	cmp	r2, #2
	beq.n	.L_08026354
	add	r5, sp, #200
	mov	r0, sl
	adds	r1, r5, #0
	bl	sub_080b50b8
	ldr	r4, [sp, #36]
	movs	r3, #8
	strb	r3, [r4, #2]
	ldr	r3, [r5, #0]
	strb	r3, [r4, #0]
	movs	r3, #128
	strb	r3, [r4, #1]
.L_08026354:
	movs	r3, #74
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #12
	movs	r2, #30
	movs	r3, #4
	bl	sub_080162d4
	movs	r6, #152
	mov	r5, sp
	mov	r7, sp
	adds	r5, #236
	add	r6, sp
	adds	r7, #88
	str	r0, [sp, #72]
	str	r5, [sp, #32]
	mov	fp, r6
	str	r7, [sp, #20]
	b.n	.L_0802638e
	movs	r0, r0
	.4byte 0x00000131
	.4byte 0xff0000ff
	.2byte 0x0141
	.2byte 0x0000
.L_08026388:
	ldr	r0, [sp, #68]
	lsls	r0, r0, #1
	str	r0, [sp, #24]
.L_0802638e:
	movs	r1, #0
	str	r1, [sp, #48]
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #28]
	mov	r1, fp
	ldrh	r0, [r2, r3]
	bl	sub_080b50b8
	ldr	r3, [pc, #72]
	ldr	r4, [sp, #32]
	str	r3, [r4, #4]
	ldr	r5, [sp, #48]
	str	r5, [r4, #8]
	ldr	r5, [pc, #64]
	ldr	r1, [r5, #0]
	movs	r3, #31
	lsrs	r1, r1, #2
	ands	r1, r3
	ldr	r3, [pc, #60]
	lsls	r1, r1, #8
	adds	r1, r1, r3
	ldr	r0, [sp, #52]
	bl	sub_080040d0
	ldr	r3, [pc, #36]
	ldr	r6, [sp, #32]
	ands	r0, r3
	ldrh	r2, [r6, #8]
	ldr	r3, [pc, #44]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [r5, #0]
	adds	r7, r6, #0
	strh	r3, [r7, #8]
	lsls	r0, r0, #12
	bl	sub_08002322
	cmp	r0, #0
	bge.n	.L_080263fc
	ldr	r1, [pc, #24]
	adds	r0, r0, r1
	b.n	.L_080263fc
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x40002000
	.4byte 0x03001e40
	.4byte 0x080346f8
	.4byte 0xfffffc00
	.2byte 0x7fff
	.2byte 0x0000
.L_080263fc:
	mov	r4, fp
	ldr	r3, [r4, #4]
	asrs	r2, r0, #15
	adds	r0, r3, r2
	str	r0, [r4, #4]
	ldr	r5, [sp, #36]
	movs	r1, #1
	ldrb	r2, [r5, #2]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802644e
	ldr	r4, [r4, #0]
	ldrb	r3, [r5, #0]
	adds	r3, r4, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r3, #1
	ldrb	r3, [r5, #1]
	adds	r3, r0, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r0, r3, #1
	subs	r3, r4, r1
	cmp	r3, #0
	blt.n	.L_08026436
	cmp	r3, #7
	ble.n	.L_0802643c
	b.n	.L_08026440
.L_08026436:
	subs	r3, r1, r4
	cmp	r3, #7
	bgt.n	.L_08026440
.L_0802643c:
	movs	r6, #1
	str	r6, [sp, #48]
.L_08026440:
	mov	r7, fp
	str	r1, [r7, #0]
	ldr	r2, [sp, #36]
	str	r0, [r7, #4]
	strb	r1, [r2, #0]
	strb	r0, [r2, #1]
	b.n	.L_08026486
.L_0802644e:
	movs	r4, #192
	lsls	r3, r2, #24
	lsls	r4, r4, #18
	cmp	r3, r4
	bhi.n	.L_08026468
	mov	r5, fp
	ldr	r6, [sp, #36]
	ldr	r3, [r5, #0]
	str	r0, [r5, #4]
	strb	r3, [r6, #0]
	strb	r0, [r6, #1]
	strb	r1, [r6, #2]
	b.n	.L_08026486
.L_08026468:
	ldr	r7, [sp, #36]
	ldrb	r3, [r7, #0]
	mov	r0, fp
	str	r3, [r0, #0]
	ldrb	r3, [r7, #1]
	str	r3, [r0, #4]
	adds	r3, r2, #0
	adds	r3, #252
	movs	r2, #192
	strb	r3, [r7, #2]
	lsls	r2, r2, #18
	lsls	r3, r3, #24
	cmp	r3, r2
	bhi.n	.L_08026486
	strb	r1, [r7, #2]
.L_08026486:
	mov	r3, fp
	ldr	r2, [r3, #0]
	ldr	r4, [sp, #32]
	ldr	r3, [pc, #48]
	subs	r2, #8
	ldrh	r1, [r4, #6]
	ands	r2, r3
	ldr	r3, [pc, #44]
	ands	r3, r1
	orrs	r3, r2
	adds	r5, r4, #0
	mov	r6, fp
	strh	r3, [r5, #6]
	ldr	r3, [r6, #4]
	subs	r3, #16
	strb	r3, [r5, #4]
	ldr	r0, [sp, #32]
	movs	r1, #240
	bl	sub_08003dec
	ldr	r7, [sp, #84]
	cmp	r7, #255
	bne.n	.L_080264d8
	ldr	r2, [pc, #16]
	ldr	r3, [sp, #88]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #1
	b.n	.L_080264cc
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0000
	.2byte 0xffff
.L_080264cc:
	.2byte 0x4313
	ldr	r2, [pc, #284]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #17
	b.n	.L_080264ea
.L_080264d8:
	ldr	r2, [pc, #276]
	ldr	r3, [sp, #88]
	ands	r3, r2
	movs	r2, #176
	orrs	r3, r2
	ldr	r2, [pc, #264]
	ands	r3, r2
	movs	r2, #176
	lsls	r2, r2, #16
.L_080264ea:
	orrs	r3, r2
	str	r3, [sp, #88]
	ldr	r0, [sp, #20]
	ldr	r3, [pc, #252]
	ldr	r2, [r0, #4]
	ands	r2, r3
	str	r2, [r0, #4]
	ldr	r0, [sp, #20]
	bl	sub_08003d28
	ldr	r1, [sp, #56]
	movs	r3, #1
	ands	r3, r1
	str	r0, [sp, #44]
	cmp	r3, #0
	bne.n	.L_0802650c
	b.n	.L_08026b96
.L_0802650c:
	movs	r2, #0
	str	r2, [sp, #64]
	ldr	r1, [sp, #36]
	movs	r0, #253
	movs	r7, #5
.L_08026516:
	ldrb	r2, [r1, #2]
	adds	r3, r0, #0
	ands	r3, r2
	subs	r7, #1
	strb	r3, [r1, #2]
	adds	r1, #4
	cmp	r7, #0
	bge.n	.L_08026516
	ldr	r3, [sp, #84]
	movs	r7, #0
	cmp	r7, r3
	bcs.n	.L_080265f4
	ldr	r6, [sp, #64]
	add	r4, sp, #172
	add	r0, sp, #324
	ldr	r1, [sp, #64]
	adds	r3, r6, r0
	mov	sl, r4
	ldr	r5, [sp, #28]
	ldr	r4, [sp, #36]
	adds	r6, r3, #0
	mov	r2, sl
	lsls	r3, r1, #1
	mov	lr, r5
	adds	r0, r4, #0
	adds	r5, r3, r2
	movs	r3, #254
	subs	r6, #160
	mov	r8, r3
	adds	r0, #24
.L_08026552:
	ldr	r1, [sp, #68]
	ldr	r2, [sp, #60]
	adds	r3, r1, r7
	cmp	r3, r2
	bge.n	.L_08026592
	lsls	r3, r3, #1
	mov	r1, lr
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #254
	beq.n	.L_08026592
	ldrb	r3, [r4, #2]
	strh	r2, [r5, #0]
	movs	r2, #2
	orrs	r2, r3
	movs	r3, #0
	orrs	r2, r3
	movs	r3, #3
	ldrsb	r3, [r4, r3]
	strb	r2, [r4, #2]
	cmp	r3, r7
	beq.n	.L_08026586
	mov	r1, r8
	ands	r2, r1
	strb	r2, [r4, #2]
	strb	r7, [r4, #3]
.L_08026586:
	strb	r7, [r6, #0]
	ldr	r2, [sp, #64]
	adds	r2, #1
	str	r2, [sp, #64]
	adds	r6, #1
	adds	r5, #2
.L_08026592:
	cmp	r7, #0
	beq.n	.L_080265de
	ldr	r1, [sp, #68]
	subs	r3, r1, r7
	cmp	r3, #0
	blt.n	.L_080265de
	lsls	r3, r3, #1
	mov	r1, lr
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #254
	beq.n	.L_080265de
	strh	r2, [r5, #0]
	movs	r2, #6
	ldrb	r3, [r0, #2]
	subs	r2, r2, r7
	mov	ip, r2
	movs	r2, #2
	orrs	r2, r3
	movs	r3, #0
	orrs	r2, r3
	movs	r3, #3
	ldrsb	r3, [r0, r3]
	negs	r1, r7
	strb	r2, [r0, #2]
	cmp	r3, r1
	beq.n	.L_080265d0
	mov	r3, r8
	ands	r2, r3
	strb	r2, [r0, #2]
	strb	r1, [r0, #3]
.L_080265d0:
	mov	r1, ip
	strb	r1, [r6, #0]
	ldr	r2, [sp, #64]
	adds	r2, #1
	str	r2, [sp, #64]
	adds	r6, #1
	adds	r5, #2
.L_080265de:
	ldr	r3, [sp, #84]
	adds	r7, #1
	adds	r4, #4
	subs	r0, #4
	cmp	r7, r3
	bcc.n	.L_08026552
	b.n	.L_080265f8
	.4byte 0x0000ffff
	.2byte 0x0000
	.2byte 0xffff
.L_080265f4:
	.2byte 0xac2b
	mov	sl, r4
.L_080265f8:
	ldr	r1, [sp, #36]
	movs	r4, #2
	movs	r0, #6
	movs	r7, #5
.L_08026600:
	ldrb	r2, [r1, #2]
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802660c
	strb	r0, [r1, #3]
.L_0802660c:
	subs	r7, #1
	adds	r1, #4
	cmp	r7, #0
	bge.n	.L_08026600
	ldr	r5, [sp, #64]
	ldr	r2, [pc, #44]
	lsls	r3, r5, #1
	mov	r6, sl
	strh	r2, [r6, r3]
	mov	r0, sl
	movs	r1, #1
	bl	sub_080b50e0
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #24]
	ldrh	r3, [r7, r0]
	cmp	r3, #7
	bls.n	.L_08026632
	b.n	.L_08026a84
.L_08026632:
	ldr	r1, [sp, #84]
	cmp	r1, #255
	bne.n	.L_0802663a
	b.n	.L_08026b8c
.L_0802663a:
	ldr	r2, [sp, #80]
	cmp	r2, #0
	bne.n	.L_08026642
	b.n	.L_08026b8c
.L_08026642:
	b.n	.L_08026648
	.2byte 0x00ff
	.2byte 0x0000
.L_08026648:
	adds	r0, r3, #0
	bl	sub_08077008
	ldr	r3, [sp, #24]
	adds	r6, r0, #0
	mov	r1, fp
	ldrh	r0, [r7, r3]
	bl	sub_080b50b8
	mov	r4, r9
	cmp	r4, #0
	beq.n	.L_08026668
	mov	r0, r9
	movs	r1, #1
	bl	sub_08016418
.L_08026668:
	ldr	r3, [sp, #80]
	subs	r3, #1
	cmp	r3, #6
	bls.n	.L_08026672
	b.n	.L_08026b8c
.L_08026672:
	ldr	r2, [pc, #616]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080266c8
	.4byte 0x0802671e
	.4byte 0x080267b4
	.4byte 0x080267f8
	.4byte 0x08026780
	.4byte 0x0802691c
	.2byte 0x6698
	.2byte 0x0802
	mov	r5, fp
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_080266a2
	adds	r3, #7
.L_080266a2:
	asrs	r3, r3, #3
	subs	r0, r3, #4
	adds	r3, #4
	cmp	r3, #29
	ble.n	.L_080266ae
	movs	r0, #22
.L_080266ae:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #9
	movs	r3, #3
	bl	sub_080162d4
	mov	r9, r0
	movs	r0, #2
	bl	sub_0801e71c
	ldr	r0, [pc, #536]
	b.n	.L_08026a72
	mov	r7, fp
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_080266d2
	adds	r3, #7
.L_080266d2:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #6
	cmp	r3, #29
	ble.n	.L_080266de
	movs	r0, #17
.L_080266de:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #13
	movs	r3, #3
	bl	sub_080162d4
	mov	r9, r0
	mov	r1, r9
	ldr	r0, [pc, #496]
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e8b0
	movs	r5, #0
	movs	r1, #56
	ldrsh	r0, [r6, r1]
	mov	r2, r9
	movs	r1, #4
	movs	r3, #16
	str	r5, [sp, #0]
	bl	sub_0801ea08
	movs	r2, #48
	ldr	r0, [pc, #472]
	mov	r1, r9
	movs	r3, #0
	bl	sub_0801e8b0
	movs	r2, #52
	ldrsh	r0, [r6, r2]
	b.n	.L_08026772
	mov	r4, fp
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_08026728
	adds	r3, #7
.L_08026728:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #6
	cmp	r3, #29
	ble.n	.L_08026734
	movs	r0, #17
.L_08026734:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #13
	movs	r3, #3
	bl	sub_080162d4
	mov	r9, r0
	mov	r1, r9
	ldr	r0, [pc, #420]
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e8b0
	movs	r5, #58
	ldrsh	r0, [r6, r5]
	movs	r1, #4
	movs	r5, #0
	mov	r2, r9
	movs	r3, #16
	str	r5, [sp, #0]
	bl	sub_0801ea08
	ldr	r0, [pc, #388]
	mov	r1, r9
	movs	r2, #48
	movs	r3, #0
	bl	sub_0801e8b0
	movs	r7, #54
	ldrsh	r0, [r6, r7]
.L_08026772:
	movs	r1, #4
	mov	r2, r9
	movs	r3, #56
	str	r5, [sp, #0]
	bl	sub_0801ea08
	b.n	.L_08026b8c
	mov	r0, fp
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bge.n	.L_0802678a
	adds	r3, #7
.L_0802678a:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #5
	cmp	r3, #29
	ble.n	.L_08026796
	movs	r0, #18
.L_08026796:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r3, #3
	movs	r2, #12
	bl	sub_080162d4
	movs	r1, #56
	ldrsh	r3, [r6, r1]
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_080267b0
	b.n	.L_08026a6a
.L_080267b0:
	ldr	r0, [pc, #316]
	b.n	.L_080267ec
	mov	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_080267be
	adds	r3, #7
.L_080267be:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #5
	cmp	r3, #29
	ble.n	.L_080267ca
	movs	r0, #18
.L_080267ca:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r3, #3
	movs	r2, #12
	bl	sub_080162d4
	ldr	r4, [pc, #280]
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r9, r0
	cmp	r3, #0
	bne.n	.L_080267ea
	b.n	.L_08026a6a
.L_080267ea:
	ldr	r0, [pc, #268]
.L_080267ec:
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e7c0
	b.n	.L_08026b8c
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r7, r6, r0
	ldrb	r3, [r7, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_08026808
	movs	r5, #1
.L_08026808:
	ldr	r1, [pc, #240]
	adds	r1, r1, r6
	ldrb	r3, [r1, #0]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_08026816
	adds	r5, #1
.L_08026816:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r2, r2, r6
	ldrb	r3, [r2, #0]
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_08026826
	adds	r5, #1
.L_08026826:
	ldr	r3, [pc, #216]
	adds	r3, r6, r3
	str	r3, [sp, #40]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026834
	adds	r5, #1
.L_08026834:
	ldr	r4, [pc, #204]
	adds	r6, r6, r4
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_08026840
	adds	r5, #1
.L_08026840:
	cmp	r5, #0
	bne.n	.L_08026846
	movs	r5, #1
.L_08026846:
	movs	r3, #9
	subs	r1, r3, r5
	cmp	r1, #3
	bgt.n	.L_08026850
	movs	r1, #4
.L_08026850:
	mov	r0, fp
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bge.n	.L_0802685a
	adds	r3, #7
.L_0802685a:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #9
	cmp	r3, #29
	ble.n	.L_08026866
	movs	r0, #14
.L_08026866:
	movs	r2, #6
	adds	r3, r5, #2
	str	r2, [sp, #0]
	movs	r2, #16
	bl	sub_080162d4
	ldrb	r3, [r7, #0]
	mov	r9, r0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0802688a
	ldr	r0, [pc, #136]
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e7c0
	movs	r5, #1
.L_0802688a:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080268a0
	lsls	r3, r5, #3
	ldr	r0, [pc, #116]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_080268a0:
	mov	r2, sl
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080268b6
	lsls	r3, r5, #3
	ldr	r0, [pc, #100]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_080268b6:
	ldr	r4, [sp, #40]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_080268cc
	lsls	r3, r5, #3
	ldr	r0, [pc, #80]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_080268cc:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_080268d4
	b.n	.L_08026a64
.L_080268d4:
	lsls	r3, r5, #3
	ldr	r0, [pc, #64]
	b.n	.L_08026a5a
	movs	r0, r0
	.4byte 0x0802667c
	.4byte 0x000008ac
	.4byte 0x080373dc
	.4byte 0x080373e0
	.4byte 0x080373e4
	.4byte 0x000008ab
	.4byte 0x00000131
	.4byte 0x000008a4
	.4byte 0x0000013b
	.4byte 0x0000013d
	.4byte 0x00000141
	.4byte 0x000008a5
	.4byte 0x000008a6
	.4byte 0x000008a7
	.4byte 0x000008a8
	.2byte 0x08a9
	.2byte 0x0000
	ldr	r7, [pc, #488]
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0802692e
	movs	r5, #1
.L_0802692e:
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802693c
	adds	r5, #1
.L_0802693c:
	ldr	r1, [pc, #460]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026948
	adds	r5, #1
.L_08026948:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026956
	adds	r5, #1
.L_08026956:
	ldr	r4, [pc, #440]
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026962
	adds	r5, #1
.L_08026962:
	ldr	r7, [pc, #432]
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802696e
	adds	r5, #1
.L_0802696e:
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802697c
	adds	r5, #1
.L_0802697c:
	cmp	r5, #0
	bne.n	.L_08026982
	movs	r5, #1
.L_08026982:
	movs	r3, #9
	subs	r1, r3, r5
	cmp	r1, #3
	bgt.n	.L_0802698c
	movs	r1, #4
.L_0802698c:
	mov	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_08026996
	adds	r3, #7
.L_08026996:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #9
	cmp	r3, #29
	ble.n	.L_080269a2
	movs	r0, #14
.L_080269a2:
	movs	r2, #6
	adds	r3, r5, #2
	str	r2, [sp, #0]
	movs	r2, #16
	bl	sub_080162d4
	ldr	r4, [pc, #344]
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r9, r0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080269ce
	ldr	r0, [pc, #340]
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e7c0
	movs	r5, #1
.L_080269ce:
	movs	r7, #156
	lsls	r7, r7, #1
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080269e8
	lsls	r3, r5, #3
	ldr	r0, [pc, #316]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_080269e8:
	ldr	r0, [pc, #288]
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026a00
	lsls	r3, r5, #3
	ldr	r0, [pc, #296]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_08026a00:
	movs	r1, #158
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026a1a
	lsls	r3, r5, #3
	ldr	r0, [pc, #276]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_08026a1a:
	ldr	r2, [pc, #244]
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026a32
	lsls	r3, r5, #3
	ldr	r0, [pc, #256]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_08026a32:
	ldr	r4, [pc, #224]
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026a4a
	lsls	r3, r5, #3
	ldr	r0, [pc, #236]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_08026a4a:
	movs	r7, #160
	lsls	r7, r7, #1
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026a64
	lsls	r3, r5, #3
	ldr	r0, [pc, #212]
.L_08026a5a:
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_08026a64:
	cmp	r5, #0
	beq.n	.L_08026a6a
	b.n	.L_08026b8c
.L_08026a6a:
	movs	r0, #2
	bl	sub_0801e71c
	ldr	r0, [pc, #192]
.L_08026a72:
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e7c0
	movs	r0, #15
	bl	sub_0801e71c
	b.n	.L_08026b8c
.L_08026a84:
	ldr	r0, [sp, #84]
	cmp	r0, #255
	bne.n	.L_08026a8c
	b.n	.L_08026b8c
.L_08026a8c:
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	ldrh	r0, [r1, r2]
	bl	sub_08077008
	ldr	r3, [sp, #28]
	ldr	r4, [sp, #24]
	add	r5, sp, #108
	mov	r8, r0
	adds	r1, r5, #0
	ldrh	r0, [r3, r4]
	bl	sub_080b50b8
	ldr	r3, [pc, #144]
	ldr	r0, [r3, #0]
	lsls	r0, r0, #12
	bl	sub_08002322
	cmp	r0, #0
	bge.n	.L_08026ab8
	ldr	r6, [pc, #132]
	adds	r0, r0, r6
.L_08026ab8:
	ldr	r2, [r5, #4]
	asrs	r3, r0, #15
	adds	r2, r2, r3
	movs	r3, #148
	str	r2, [r5, #4]
	lsls	r3, r3, #1
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #125
	beq.n	.L_08026ad6
	cmp	r3, #122
	beq.n	.L_08026ad6
	movs	r7, #0
	add	r6, sp, #120
	b.n	.L_08026af2
.L_08026ad6:
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, r8
	ldrb	r3, [r3, #0]
	ldr	r0, [pc, #96]
	cmp	r3, #125
	bne.n	.L_08026ae6
	adds	r0, #1
.L_08026ae6:
	add	r6, sp, #120
	adds	r1, r6, #0
	movs	r2, #14
	bl	sub_0801965c
	b.n	.L_08026b4a
.L_08026af2:
	cmp	r7, #13
	bgt.n	.L_08026b44
	mov	r0, r8
	ldrb	r3, [r0, r7]
	lsls	r2, r7, #1
	strh	r3, [r6, r2]
	adds	r7, #1
	cmp	r3, #0
	bne.n	.L_08026af2
	b.n	.L_08026b46
	movs	r0, r0
	.4byte 0x00000131
	.4byte 0x0000013b
	.4byte 0x0000013d
	.4byte 0x00000141
	.4byte 0x000008a4
	.4byte 0x000008a5
	.4byte 0x000008a6
	.4byte 0x000008a7
	.4byte 0x000008a8
	.4byte 0x000008a9
	.4byte 0x000008aa
	.4byte 0x000008a3
	.4byte 0x03001e40
	.4byte 0x00007fff
	.2byte 0x080e
	.2byte 0x0000
.L_08026b44:
	lsls	r2, r7, #1
.L_08026b46:
	ldr	r3, [pc, #48]
	strh	r3, [r6, r2]
.L_08026b4a:
	adds	r0, r6, #0
	bl	sub_08017a64
	lsrs	r2, r0, #31
	ldr	r3, [r5, #0]
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r3, r3, r2
	subs	r3, #8
	str	r3, [r5, #0]
	adds	r3, r3, r0
	cmp	r3, #224
	ble.n	.L_08026b6a
	movs	r3, #224
	subs	r3, r3, r0
	str	r3, [r5, #0]
.L_08026b6a:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_08026b7c
	movs	r3, #0
	str	r3, [r5, #0]
	b.n	.L_08026b7c
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_08026b7c:
	bl	sub_0801671c
	ldr	r2, [r5, #0]
	adds	r0, r6, #0
	ldr	r1, [sp, #72]
	movs	r3, #4
	bl	sub_08017aa4
.L_08026b8c:
	ldr	r1, [sp, #56]
	movs	r3, #2
	negs	r3, r3
	ands	r1, r3
	str	r1, [sp, #56]
.L_08026b96:
	ldr	r2, [sp, #48]
	cmp	r2, #0
	bne.n	.L_08026b9e
	b.n	.L_08026cdc
.L_08026b9e:
	ldr	r3, [sp, #64]
	movs	r7, #1
	cmp	r7, r3
	blt.n	.L_08026ba8
	b.n	.L_08026cdc
.L_08026ba8:
	mov	r5, sp
	adds	r5, #164
	movs	r6, #96
	movs	r4, #172
	str	r5, [sp, #16]
	add	r6, sp
	ldr	r5, [sp, #32]
	movs	r0, #2
	add	r4, sp
	mov	r8, r6
	str	r0, [sp, #12]
	mov	sl, r4
	adds	r5, #12
	mov	r4, r8
.L_08026bc4:
	ldr	r1, [sp, #16]
	ldrb	r3, [r1, r7]
	ldr	r2, [sp, #36]
	lsls	r3, r3, #2
	adds	r3, r2, r3
	str	r3, [sp, #4]
	ldr	r3, [sp, #12]
	mov	r6, sl
	adds	r1, r4, #0
	ldrh	r0, [r3, r6]
	str	r4, [sp, #8]
	bl	sub_080b50b8
	ldr	r3, [pc, #180]
	ldr	r0, [r3, #0]
	lsls	r0, r0, #12
	bl	sub_08002322
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bge.n	.L_08026bf2
	ldr	r1, [pc, #168]
	adds	r0, r0, r1
.L_08026bf2:
	ldr	r3, [r4, #4]
	asrs	r2, r0, #15
	adds	r3, r3, r2
	str	r3, [r4, #4]
	ldr	r2, [sp, #32]
	mov	lr, r5
	mov	ip, r2
	mov	r3, lr
	mov	r6, ip
	ldmia	r6!, {r0, r1, r2}
	stmia	r3!, {r0, r1, r2}
	ldr	r3, [sp, #4]
	movs	r1, #1
	ldrb	r2, [r3, #2]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026c3a
	ldr	r6, [sp, #4]
	ldr	r1, [r4, #0]
	ldrb	r3, [r6, #0]
	adds	r1, r1, r3
	lsrs	r3, r1, #31
	ldrb	r2, [r6, #1]
	adds	r1, r1, r3
	ldr	r3, [r4, #4]
	adds	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r1, #1
	asrs	r3, r3, #1
	str	r1, [r4, #0]
	strb	r1, [r6, #0]
	str	r3, [r4, #4]
	strb	r3, [r6, #1]
	b.n	.L_08026c50
.L_08026c3a:
	ldrh	r3, [r5, #6]
	ldrb	r2, [r5, #4]
	ldr	r0, [sp, #4]
	lsls	r3, r3, #23
	lsrs	r3, r3, #23
	adds	r2, #8
	strb	r1, [r0, #2]
	str	r3, [r4, #0]
	strb	r3, [r0, #0]
	str	r2, [r4, #4]
	strb	r2, [r0, #1]
.L_08026c50:
	ldrb	r2, [r5, #5]
	movs	r1, #13
	negs	r1, r1
	adds	r3, r1, #0
	adds	r0, r2, #0
	mov	r2, r8
	ands	r0, r3
	ldr	r1, [r2, #0]
	movs	r3, #4
	orrs	r0, r3
	ldr	r3, [pc, #36]
	subs	r1, #8
	ands	r1, r3
	ldr	r2, [pc, #36]
	ldrh	r3, [r5, #6]
	ands	r3, r2
	orrs	r3, r1
	mov	r6, r8
	strh	r3, [r5, #6]
	ldr	r3, [r6, #4]
	subs	r3, #12
	strb	r0, [r5, #5]
	strb	r3, [r5, #4]
	ldr	r1, [sp, #84]
	cmp	r1, #255
	bne.n	.L_08026c9c
	movs	r2, #4
	negs	r2, r2
	ands	r0, r2
	b.n	.L_08026ca6
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001e40
	.2byte 0x7fff
	.2byte 0x0000
.L_08026c9c:
	movs	r3, #4
	negs	r3, r3
	ands	r0, r3
	movs	r3, #1
	orrs	r0, r3
.L_08026ca6:
	strb	r0, [r5, #5]
	ldr	r2, [sp, #44]
	movs	r3, #31
	movs	r6, #63
	ands	r2, r3
	negs	r6, r6
	ldrb	r3, [r5, #7]
	adds	r1, r6, #0
	lsls	r2, r2, #1
	ands	r3, r1
	orrs	r3, r2
	adds	r0, r5, #0
	strb	r3, [r5, #7]
	movs	r1, #240
	str	r4, [sp, #8]
	bl	sub_08003dec
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #64]
	adds	r0, #2
	adds	r7, #1
	adds	r5, #12
	str	r0, [sp, #12]
	ldr	r4, [sp, #8]
	cmp	r7, r1
	bge.n	.L_08026cdc
	b.n	.L_08026bc4
.L_08026cdc:
	ldr	r3, [pc, #404]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #404]
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #404]
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026d0c
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r5, #0
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_08026d08
	movs	r3, #60
	str	r3, [r2, #0]
	movs	r5, #1
	movs	r6, #1
	b.n	.L_08026d0c
.L_08026d08:
	subs	r3, #1
	str	r3, [r2, #0]
.L_08026d0c:
	movs	r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08026d8e
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #24]
	movs	r5, #0
	ldrh	r4, [r2, r3]
	ldr	r7, [sp, #76]
	str	r5, [sp, #56]
	movs	r3, #88
	ldrsh	r3, [r7, r3]
	movs	r0, #1
	negs	r0, r0
	movs	r1, #0
	cmp	r3, #255
	beq.n	.L_08026d54
	cmp	r3, r4
	bne.n	.L_08026d38
	movs	r0, #128
	lsls	r0, r0, #1
	b.n	.L_08026d54
.L_08026d38:
	adds	r1, #1
	cmp	r1, #5
	bgt.n	.L_08026d54
	lsls	r3, r1, #1
	ldr	r5, [sp, #76]
	adds	r3, #88
	ldrsh	r3, [r5, r3]
	cmp	r3, #255
	beq.n	.L_08026d54
	cmp	r3, r4
	bne.n	.L_08026d38
	movs	r0, #128
	lsls	r0, r0, #1
	orrs	r0, r1
.L_08026d54:
	cmp	r0, #0
	bge.n	.L_08026d8a
	ldr	r2, [sp, #76]
	adds	r2, #102
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	movs	r5, #192
	movs	r1, #0
	lsls	r5, r5, #1
	cmp	r3, #255
	beq.n	.L_08026d8a
	cmp	r3, r4
	bne.n	.L_08026d72
	adds	r0, r5, #0
	b.n	.L_08026d8a
.L_08026d72:
	adds	r1, #1
	adds	r2, #2
	cmp	r1, #5
	bgt.n	.L_08026d8a
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	cmp	r3, #255
	beq.n	.L_08026d8a
	cmp	r3, r4
	bne.n	.L_08026d72
	adds	r0, r5, #0
	orrs	r0, r1
.L_08026d8a:
	str	r0, [sp, #68]
	b.n	.L_08026df6
.L_08026d8e:
	ldr	r0, [sp, #84]
	cmp	r0, #255
	beq.n	.L_08026df6
	movs	r3, #144
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08026dc4
	movs	r0, #111
	bl	sub_080f9010
.L_08026da2:
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	adds	r0, r1, #0
	ldr	r1, [sp, #60]
	bl	sub_080022fc
	str	r0, [sp, #68]
	ldr	r4, [sp, #28]
	lsls	r2, r0, #1
	ldrh	r3, [r4, r2]
	cmp	r3, #254
	beq.n	.L_08026da2
	ldr	r7, [sp, #56]
	movs	r3, #1
	orrs	r7, r3
	str	r7, [sp, #56]
.L_08026dc4:
	movs	r3, #96
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08026df6
	movs	r0, #111
	bl	sub_080f9010
.L_08026dd2:
	ldr	r0, [sp, #68]
	ldr	r1, [sp, #60]
	adds	r3, r0, r1
	subs	r3, #1
	adds	r0, r3, #0
	str	r3, [sp, #68]
	bl	sub_080022fc
	str	r0, [sp, #68]
	ldr	r2, [sp, #28]
	lsls	r3, r0, #1
	ldrh	r3, [r2, r3]
	cmp	r3, #254
	beq.n	.L_08026dd2
	ldr	r4, [sp, #56]
	movs	r3, #1
	orrs	r4, r3
	str	r4, [sp, #56]
.L_08026df6:
	ldr	r3, [pc, #132]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L_08026e08
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08026e16
.L_08026e08:
	movs	r0, #113
	bl	sub_080f9010
	movs	r5, #1
	negs	r5, r5
	str	r5, [sp, #68]
	b.n	.L_08026e26
.L_08026e16:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r6, [sp, #56]
	cmp	r6, #0
	beq.n	.L_08026e26
	bl	.L_08026388
.L_08026e26:
	movs	r0, #1
	bl	sub_080030f8
	mov	r7, r9
	ldr	r0, [sp, #52]
	bl	sub_08003f3c
	cmp	r7, #0
	beq.n	.L_08026e40
	mov	r0, r9
	movs	r1, #1
	bl	sub_08016418
.L_08026e40:
	ldr	r0, [sp, #72]
	movs	r1, #1
	bl	sub_08016418
	movs	r1, #0
	ldr	r0, [sp, #28]
	bl	sub_080b50e0
	ldr	r3, [pc, #40]
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #40]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #68]
	add	sp, #324
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x03001f34
