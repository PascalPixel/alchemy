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
	bl 0x0200d1e0
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
	bl 0x0200d140
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
	bl 0x0200d130
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200d138
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200d188
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
	bl 0x0200d278
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
	bl 0x0200d0b8
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
	bl 0x0200d0b8
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x0200d0b8
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
	bl 0x0200d130
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200d138
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
	.4byte 0x0200d508
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #11
	movs	r1, #27
	bl 0x0200d308
	pop	{pc}
	.4byte 0x88033066
	.4byte 0x80033b01
	.4byte 0x141b041b
	.4byte 0x43184258
	.2byte 0x0fc0
	.2byte 0x4770
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r5, #100
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020002b4
	subs	r3, r2, #1
	b.n	.L_020002d4
.L_020002b4:
	adds	r3, r0, #0
	adds	r3, #102
	movs	r1, #6
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	adds	r1, #255
	movs	r2, #0
	bl 0x0200d2c8
	bl 0x0200d0e0
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	adds	r3, #240
.L_020002d4:
	strh	r3, [r5, #0]
	movs	r0, #1
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r5, #100
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #2
	beq.n	.L_0200031a
	cmp	r3, #2
	bgt.n	.L_020002f4
	cmp	r3, #0
	beq.n	.L_0200032e
	b.n	.L_02000344
.L_020002f4:
	cmp	r3, #4
	beq.n	.L_0200030c
	cmp	r3, #6
	bne.n	.L_02000344
	ldr	r3, [r0, #24]
	ldr	r2, [pc, #80]
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r3, [r0, #28]
	movs	r2, #128
	lsls	r2, r2, #6
	b.n	.L_02000328
.L_0200030c:
	ldr	r3, [r0, #24]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r2, [pc, #60]
	b.n	.L_02000326
.L_0200031a:
	ldr	r3, [r0, #24]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r2, [pc, #48]
.L_02000326:
	ldr	r3, [r0, #28]
.L_02000328:
	adds	r3, r3, r2
	str	r3, [r0, #28]
	b.n	.L_02000344
.L_0200032e:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	bl 0x0200d0e0
	movs	r1, #90
	bl 0x0200d0c0
	adds	r0, #60
	strh	r0, [r5, #0]
.L_02000344:
	ldrh	r3, [r5, #0]
	movs	r0, #1
	subs	r3, #1
	strh	r3, [r5, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0xfffff000
	.2byte 0xf800
	.2byte 0xffff
	.2byte 0xb500
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	adds	r1, r0, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #8
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	bl 0x0200d188
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.global Func_02000388
	.thumb_func
Func_02000388:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd670
	.2byte 0x0200
	.global Func_02000390
	.thumb_func
Func_02000390:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020003a8
	ldr	r0, [pc, #20]
	b.n	.L_020003b2
.L_020003a8:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_020003b2
	ldr	r0, [pc, #16]
.L_020003b2:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000010
	.4byte 0x0200d6a0
	.4byte 0x00000012
	.2byte 0xd6c0
	.2byte 0x0200
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xd6e0
	.2byte 0x0200
	.global Func_020003d0
	.thumb_func
Func_020003d0:
	push	{lr}
	ldr	r3, [pc, #152]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #144]
	cmp	r2, r3
	bne.n	.L_020003e8
	ldr	r0, [pc, #140]
	b.n	.L_02000468
.L_020003e8:
	ldr	r3, [pc, #140]
	cmp	r2, r3
	bne.n	.L_020003f2
	ldr	r0, [pc, #140]
	b.n	.L_02000468
.L_020003f2:
	ldr	r3, [pc, #140]
	cmp	r2, r3
	bne.n	.L_020003fc
	ldr	r0, [pc, #136]
	b.n	.L_02000468
.L_020003fc:
	ldr	r3, [pc, #136]
	cmp	r2, r3
	bne.n	.L_02000406
	ldr	r0, [pc, #136]
	b.n	.L_02000468
.L_02000406:
	ldr	r3, [pc, #136]
	cmp	r2, r3
	bne.n	.L_02000430
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_0200042c
	ldr	r2, [pc, #120]
	movs	r3, #0
	adds	r1, r2, #0
	adds	r1, #94
	strb	r3, [r1, #0]
	adds	r2, #142
	adds	r1, #24
	strb	r3, [r1, #0]
	strb	r3, [r2, #0]
.L_0200042c:
	ldr	r0, [pc, #100]
	b.n	.L_02000468
.L_02000430:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_02000466
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02000462
	ldr	r1, [pc, #84]
	movs	r3, #3
	adds	r2, r1, #0
	adds	r2, #142
	strb	r3, [r2, #0]
	movs	r3, #184
	adds	r2, #10
	lsls	r3, r3, #17
	str	r3, [r2, #0]
	ldr	r3, [pc, #72]
	adds	r2, #8
	str	r3, [r2, #0]
	adds	r2, #30
	movs	r3, #1
	strb	r3, [r2, #0]
.L_02000462:
	ldr	r0, [pc, #56]
	b.n	.L_02000468
.L_02000466:
	ldr	r0, [pc, #60]
.L_02000468:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000000e
	.4byte 0x0200d7c0
	.4byte 0x00000010
	.4byte 0x0200d8b0
	.4byte 0x00000011
	.4byte 0x0200d958
	.4byte 0x00000014
	.4byte 0x0200daa8
	.4byte 0x00000015
	.4byte 0x0200db20
	.4byte 0x00000016
	.4byte 0x0200dbe0
	.4byte 0x02b60000
	.2byte 0xd7a8
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #66
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02000574
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x0200d118
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #4
	movs	r1, #104
	movs	r2, #252
	bl 0x0200d220
	movs	r1, #160
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #16
	movs	r2, #16
	movs	r3, #192
	lsls	r3, r3, #8
	negs	r2, r2
	negs	r1, r1
	movs	r0, #14
	bl 0x0200d378
	movs	r0, #14
	bl 0x0200d228
	movs	r0, #20
	bl 0x0200d1b8
	ldr	r0, [pc, #120]
	bl 0x0200d280
	movs	r0, #14
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #14
	bl 0x0200d2b0
	movs	r0, #30
	bl 0x0200d1b8
	movs	r0, #14
	movs	r1, #3
	bl 0x0200d248
	movs	r1, #3
	movs	r0, #4
	bl 0x0200d250
	movs	r0, #20
	bl 0x0200d1b8
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #64]
	adds	r2, #153
	bl 0x0200d1e8
	movs	r0, #14
	movs	r1, #2
	bl 0x0200d248
	movs	r0, #4
	bl 0x0200d1e0
	cmp	r0, #0
	beq.n	.L_02000560
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #14
	bl 0x0200d208
.L_02000560:
	movs	r0, #14
	bl 0x0200d228
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	bl 0x0200d1c8
.L_02000574:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000017b9
	.2byte 0x3333
	.2byte 0x0001
	.global Func_02000580
	.thumb_func
Func_02000580:
	push	{lr}
	ldr	r3, [pc, #104]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_02000598
	ldr	r0, [pc, #92]
	b.n	.L_020005ea
.L_02000598:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_020005a2
	ldr	r0, [pc, #92]
	b.n	.L_020005ea
.L_020005a2:
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_020005ac
	ldr	r0, [pc, #88]
	b.n	.L_020005ea
.L_020005ac:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020005b6
	ldr	r0, [pc, #88]
	b.n	.L_020005ea
.L_020005b6:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020005c0
	ldr	r0, [pc, #84]
	b.n	.L_020005ea
.L_020005c0:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_020005ca
	ldr	r0, [pc, #84]
	b.n	.L_020005ea
.L_020005ca:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_020005d4
	ldr	r0, [pc, #80]
	b.n	.L_020005ea
.L_020005d4:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_020005de
	ldr	r0, [pc, #80]
	b.n	.L_020005ea
.L_020005de:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_020005e8
	ldr	r0, [pc, #76]
	b.n	.L_020005ea
.L_020005e8:
	ldr	r0, [pc, #76]
.L_020005ea:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000000e
	.4byte 0x0200dd54
	.4byte 0x0000000f
	.4byte 0x0200dde4
	.4byte 0x00000010
	.4byte 0x0200de20
	.4byte 0x00000011
	.4byte 0x0200de8c
	.4byte 0x00000012
	.4byte 0x0200def8
	.4byte 0x00000013
	.4byte 0x0200df10
	.4byte 0x00000014
	.4byte 0x0200df40
	.4byte 0x00000015
	.4byte 0x0200dfc4
	.4byte 0x00000016
	.4byte 0x0200e15c
	.2byte 0xdd48
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #36]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02008668
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [r0, #44]
	ldr	r3, [r0, #36]
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r2, [pc, #56]
	cmp	r3, #0
	bge.n	.L_0200067a
	negs	r3, r3
.L_0200067a:
	movs	r4, #187
	lsls	r4, r4, #8
	adds	r4, #128
	cmp	r3, r4
	bls.n	.L_0200068a
	ldr	r3, [r2, #0]
	adds	r3, #4
	b.n	.L_02000692
.L_0200068a:
	cmp	r3, #0
	beq.n	.L_02000694
	ldr	r3, [r2, #0]
	adds	r3, #2
.L_02000692:
	str	r3, [r2, #0]
.L_02000694:
	ldr	r5, [pc, #20]
	ldr	r3, [r5, #0]
	adds	r3, r3, r1
	str	r3, [r5, #0]
	cmp	r3, #40
	ble.n	.L_020006a8
	bl 0x020086b0
	movs	r3, #0
	str	r3, [r5, #0]
.L_020006a8:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xe294
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #14
	adds	r0, #255
	bl 0x0200d140
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000708
	ldr	r1, [pc, #68]
	ldr	r6, [r5, #80]
	bl 0x0200d138
	adds	r3, r5, #0
	adds	r3, #85
	movs	r7, #0
	adds	r2, r5, #0
	strb	r7, [r3, #0]
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	movs	r3, #2
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_02000708
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200d128
	ldrb	r2, [r6, #5]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #5]
	ldrb	r3, [r6, #9]
	movs	r2, #12
	orrs	r3, r2
	strb	r7, [r6, #26]
	strb	r3, [r6, #9]
.L_02000708:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xd4e4
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	ldr	r3, [pc, #76]
	movs	r2, #7
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200075c
	ldr	r2, [r6, #12]
	movs	r3, #192
	lsls	r3, r3, #11
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	movs	r0, #14
	bl 0x0200d140
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200075c
	movs	r1, #0
	bl 0x0200d188
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200d130
	ldr	r1, [pc, #28]
	adds	r0, r5, #0
	bl 0x0200d138
.L_0200075c:
	movs	r1, #217
	lsls	r1, r1, #8
	adds	r1, #153
	adds	r0, r6, #0
	bl 0x02008778
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0300122c
	.2byte 0xd4f0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #230
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	mov	r8, r3
	ldr	r3, [r6, #8]
	sub	sp, #12
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	adds	r7, r1, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r1, r5, #0
	adds	r3, r3, r7
	str	r3, [r5, #8]
	bl 0x0200d180
	ldr	r3, [r6, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	lsls	r2, r2, #12
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	adds	r3, r3, r2
	adds	r1, r5, #0
	str	r3, [r5, #8]
	bl 0x0200d180
	cmp	r0, #0
	bgt.n	.L_02000814
	ldr	r2, [pc, #80]
	ldr	r3, [r6, #8]
	adds	r0, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	adds	r1, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	bl 0x0200d180
	cmp	r0, #0
	bgt.n	.L_02000814
	ldr	r2, [pc, #56]
	ldr	r3, [r6, #8]
	adds	r0, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #40]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r1, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #8]
	bl 0x0200d180
	cmp	r0, #0
	bgt.n	.L_02000814
	mov	r2, r8
	ldr	r3, [r2, #16]
	adds	r3, r3, r7
	str	r3, [r2, #16]
	ldr	r3, [r6, #16]
	adds	r3, r3, r7
	str	r3, [r6, #16]
.L_02000814:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0005b333
	.2byte 0x4ccd
	.2byte 0xfffa
	.2byte 0xb500
	ldr	r3, [r1, #8]
	ldr	r4, [pc, #48]
	adds	r2, r3, r4
	movs	r4, #192
	lsls	r4, r4, #12
	adds	r3, r3, r4
	ldr	r4, [r0, #8]
	cmp	r4, r2
	ble.n	.L_02000856
	cmp	r4, r3
	bge.n	.L_02000856
	ldr	r1, [r1, #16]
	ldr	r2, [pc, #32]
	ldr	r0, [r0, #16]
	adds	r3, r1, r2
	cmp	r0, r3
	ble.n	.L_02000856
	movs	r4, #128
	lsls	r4, r4, #12
	adds	r3, r1, r4
	cmp	r0, r3
	bge.n	.L_02000856
	movs	r0, #1
	b.n	.L_02000858
.L_02000856:
	movs	r0, #0
.L_02000858:
	pop	{pc}
	.2byte 0x0000
	.4byte 0xfff40000
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #188]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	mov	r8, r0
	movs	r0, #8
	bl 0x0200d1e0
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
	mov	sl, r0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200d0b8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020008b2
	adds	r3, #15
.L_020008b2:
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
	mov	r3, r8
	ldr	r5, [r3, #12]
	cmp	r5, #0
	bne.n	.L_02000922
	adds	r0, r6, #0
	mov	r1, r8
	bl 0x02008824
	cmp	r0, #0
	beq.n	.L_020008fc
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r5, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
.L_020008fc:
	mov	r2, r8
	ldr	r5, [r2, #12]
	cmp	r5, #0
	bne.n	.L_02000922
	adds	r0, r6, #0
	mov	r1, sl
	bl 0x02008824
	cmp	r0, #0
	beq.n	.L_02000922
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r5, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
.L_02000922:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r2, r5, #0
	adds	r3, #4
	strb	r6, [r3, #0]
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #3
	ldr	r0, [r5, #80]
	ands	r1, r3
	ldrb	r2, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r1, r1, #2
	orrs	r3, r1
	strb	r3, [r0, #9]
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200d188
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200d130
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl 0x0200d138
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x0200d278
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0xd494
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #4]
	adds	r6, r1, #0
	ldr	r1, [r0, #0]
	ldr	r0, [pc, #104]
	adds	r3, r3, r0
	movs	r0, #30
	adds	r0, #255
	bl 0x0200d140
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020009fe
	bl 0x0200d0e0
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #12
	movs	r3, #192
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200d0f0
	str	r0, [r5, #68]
	bl 0x0200d0e0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [r5, #72]
	bl 0x0200d0e0
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r0, r0, r6
	str	r0, [r5, #76]
	bl 0x0200d0e0
	ldr	r3, [pc, #36]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x02008930
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #1
	bl 0x0200d190
.L_020009fe:
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0xffff8000
	.2byte 0x8865
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	mov	r8, r0
	movs	r0, #12
	bl 0x0200d1e0
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
	mov	sl, r0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200d0b8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02000a5a
	adds	r3, #15
.L_02000a5a:
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
	adds	r0, r6, #0
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	movs	r5, #0
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	mov	r1, r8
	bl 0x02008824
	cmp	r0, #0
	beq.n	.L_02000aa8
	ldr	r1, [r6, #80]
	movs	r2, #12
	ldrb	r3, [r1, #9]
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r5, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
.L_02000aa8:
	adds	r0, r6, #0
	mov	r1, sl
	bl 0x02008824
	cmp	r0, #0
	beq.n	.L_02000ad0
	ldr	r1, [r6, #80]
	movs	r2, #12
	ldrb	r3, [r1, #9]
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r5, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r6, #72]
.L_02000ad0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #30
	movs	r1, #174
	movs	r2, #224
	movs	r3, #156
	adds	r0, #255
	lsls	r1, r1, #18
	lsls	r2, r2, #13
	lsls	r3, r3, #16
	bl 0x0200d140
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000b52
	bl 0x0200d0e0
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #12
	movs	r3, #192
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200d0f0
	str	r0, [r5, #68]
	bl 0x0200d0e0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [r5, #72]
	bl 0x0200d0e0
	movs	r3, #176
	lsls	r0, r0, #17
	lsls	r3, r3, #11
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	str	r0, [r5, #76]
	bl 0x0200d0e0
	ldr	r3, [pc, #32]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x02008930
	ldr	r3, [pc, #16]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #1
	bl 0x0200d190
.L_02000b52:
	pop	{r5, pc}
	.4byte 0xffff8000
	.2byte 0x8a0d
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	movs	r1, #1
	bl 0x0200d188
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d1e0
	adds	r2, r0, #0
	adds	r2, #98
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	adds	r0, r5, #0
	.2byte 0xf004
	.2byte 0xfb57
	adds	r0, r5, #0
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xfb8b
	adds	r0, r5, #0
	.2byte 0xf004
	.2byte 0xfb8c
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200d1e0
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r1, #2
	adds	r0, r5, #0
	.2byte 0xf004
	.2byte 0xfb7b
	adds	r0, r5, #0
	.2byte 0xf004
	.2byte 0xfb7c
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #133
	movs	r1, #1
	.2byte 0xf004
	.2byte 0xfbb9
	movs	r1, #10
	movs	r0, #12
	.2byte 0xf004
	.2byte 0xfbb9
	bl 0x0200d368
	movs	r0, #1
	.2byte 0xf004
	.2byte 0xfbac
	.2byte 0xf004
	.2byte 0xfbb6
	.2byte 0xf004
	.2byte 0xfbb8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #52]
	sub	sp, #12
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000c28
	ldr	r3, [pc, #44]
	movs	r1, #9
	ldr	r0, [r3, #0]
	bl 0x0200d0c0
	adds	r2, r0, #0
	cmp	r2, #0
	bne.n	.L_02000c28
	movs	r3, #148
	mov	r0, sp
	lsls	r3, r3, #17
	str	r3, [r0, #0]
	movs	r3, #176
	lsls	r3, r3, #16
	movs	r1, #128
	str	r2, [r0, #4]
	str	r3, [r0, #8]
	lsls	r1, r1, #10
	bl 0x0200898c
.L_02000c28:
	add	sp, #12
	pop	{pc}
	.4byte 0x0200e298
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #52]
	sub	sp, #12
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000c68
	ldr	r3, [pc, #44]
	movs	r1, #5
	ldr	r0, [r3, #0]
	bl 0x0200d0c0
	adds	r2, r0, #0
	cmp	r2, #0
	bne.n	.L_02000c68
	movs	r3, #172
	mov	r0, sp
	lsls	r3, r3, #17
	str	r3, [r0, #0]
	movs	r3, #160
	lsls	r3, r3, #16
	movs	r1, #128
	str	r2, [r0, #4]
	str	r3, [r0, #8]
	lsls	r1, r1, #11
	bl 0x0200898c
.L_02000c68:
	add	sp, #12
	pop	{pc}
	.4byte 0x0200e29c
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r1, #5
	ldr	r0, [r3, #0]
	bl 0x0200d0c0
	cmp	r0, #0
	bne.n	.L_02000c88
	bl 0x02008adc
.L_02000c88:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	.2byte 0xf004
	.2byte 0xfaa4
	movs	r3, #128
	lsls	r3, r3, #4
	str	r3, [r0, #28]
	adds	r0, r5, #0
	bl 0x0200d1e0
	movs	r1, #216
	movs	r3, #216
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #16
	.2byte 0xf004
	.2byte 0xfa4f
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	.2byte 0xf004
	.2byte 0xfa92
	movs	r3, #128
	lsls	r3, r3, #4
	str	r3, [r0, #28]
	adds	r0, r5, #0
	.2byte 0xf004
	.2byte 0xfa8c
.L_02000cc8:
	movs	r1, #188
	movs	r3, #248
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #16
	.2byte 0xf004
	.2byte 0xfa3d
	pop	{r5, pc}
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r2, #0
	.2byte 0xf004
	.2byte 0xfa7f
	ldr	r3, [r0, #28]
	adds	r3, r3, r5
	str	r3, [r0, #28]
	ldr	r3, [r0, #12]
	adds	r3, r3, r6
	str	r3, [r0, #12]
	ldr	r3, [r0, #16]
	subs	r3, r3, r6
	str	r3, [r0, #16]
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r2, #0
	.2byte 0xf004
	.2byte 0xfa6f
	ldr	r3, [r0, #28]
	subs	r3, r3, r5
	str	r3, [r0, #28]
	ldr	r3, [r0, #12]
	subs	r3, r3, r6
	str	r3, [r0, #12]
	ldr	r3, [r0, #16]
	adds	r3, r3, r6
	str	r3, [r0, #16]
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #10
	.2byte 0xf004
	.2byte 0xf9f6
	cmp	r0, #0
	bne.n	.L_02000d34
	movs	r0, #192
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xf9f0
	cmp	r0, #0
	beq.n	.L_02000d38
.L_02000d34:
	movs	r0, #1
	b.n	.L_02000d3a
.L_02000d38:
	movs	r0, #0
.L_02000d3a:
	pop	{pc}
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #10
	.2byte 0xf004
	.2byte 0xf9e4
	cmp	r0, #0
	bne.n	.L_02000d5a
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
.L_02000d52:
	.2byte 0xf004
	.2byte 0xf9dd
	cmp	r0, #0
	beq.n	.L_02000d5e
.L_02000d5a:
	movs	r0, #1
	b.n	.L_02000d60
.L_02000d5e:
	movs	r0, #0
.L_02000d60:
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #11
	movs	r1, #2
	bl 0x0200d248
	movs	r0, #13
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xfa69
	movs	r0, #14
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xfa65
	movs	r0, #15
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #16
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xfa5d
	movs	r0, #9
	movs	r1, #0
	.2byte 0xf004
	.2byte 0xfa59
	movs	r1, #2
	movs	r0, #9
	.2byte 0xf004
	.2byte 0xfa8d
	movs	r0, #9
	.2byte 0xf004
	.2byte 0xfa1e
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	.2byte 0xf004
	.2byte 0xfa16
	movs	r1, #216
	movs	r2, #128
	movs	r3, #216
	lsls	r1, r1, #16
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	.2byte 0xf004
	.2byte 0xf9c6
	movs	r0, #130
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xf9a2
	cmp	r0, #0
	beq.n	.L_02000e02
	movs	r1, #4
	movs	r0, #9
	.2byte 0xf004
	.2byte 0xfa38
	ldr	r5, [pc, #124]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	.2byte 0xf004
	.2byte 0xf9fd
	movs	r1, #216
	movs	r2, #192
	movs	r3, #216
	lsls	r1, r1, #16
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	.2byte 0xf004
	.2byte 0xf9ad
	ldr	r0, [r5, #0]
	.2byte 0xf004
	.2byte 0xf9f2
	movs	r1, #0
	.2byte 0xf004
	.2byte 0xf9c3
.L_02000e02:
	movs	r0, #11
	.2byte 0xf004
	.2byte 0xf9ec
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #28]
	movs	r0, #11
	.2byte 0xf004
	.2byte 0xf9e6
	movs	r1, #216
	movs	r3, #216
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #16
	.2byte 0xf004
	.2byte 0xf997
	movs	r0, #13
	bl 0x02008c90
	movs	r0, #14
	bl 0x02008c90
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	.2byte 0xf004
	.2byte 0xf96c
	cmp	r0, #0
	beq.n	.L_02000e48
	movs	r0, #15
	bl 0x02008c90
	movs	r0, #16
	bl 0x02008c90
.L_02000e48:
	ldr	r2, [pc, #16]
	movs	r3, #1
	movs	r0, #131
	str	r3, [r2, #0]
	lsls	r0, r0, #2
	bl 0x0200d118
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0xe298
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #12
	movs	r1, #2
	.2byte 0xf004
	.2byte 0xf9ef
	movs	r0, #17
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xf9eb
	movs	r0, #18
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xf9e7
	movs	r0, #19
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xf9e3
	movs	r0, #20
	movs	r1, #3
	.2byte 0xf004
	.2byte 0xf9df
	movs	r0, #10
	movs	r1, #0
	.2byte 0xf004
	.2byte 0xf9db
	movs	r1, #2
	movs	r0, #10
	.2byte 0xf004
	.2byte 0xfa0f
	movs	r0, #10
	.2byte 0xf004
	.2byte 0xf9a0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	.2byte 0xf004
	.2byte 0xf998
	movs	r1, #188
	movs	r2, #128
	movs	r3, #248
	lsls	r1, r1, #17
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	.2byte 0xf004
	.2byte 0xf948
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02000f00
	movs	r1, #4
	movs	r0, #10
	bl 0x0200d248
	ldr	r5, [pc, #128]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
.L_02000ede:
	ldr	r0, [r5, #0]
	.2byte 0xf004
	.2byte 0xf97e
	movs	r1, #188
	movs	r2, #192
	movs	r3, #248
	lsls	r1, r1, #17
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	.2byte 0xf004
	.2byte 0xf92e
	ldr	r0, [r5, #0]
	.2byte 0xf004
	.2byte 0xf973
	movs	r1, #0
	.2byte 0xf004
	.2byte 0xf944
.L_02000f00:
	movs	r0, #12
	.2byte 0xf004
	.2byte 0xf96d
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #28]
	movs	r0, #12
	.2byte 0xf004
	.2byte 0xf967
	movs	r1, #188
	movs	r3, #248
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #16
	.2byte 0xf004
	.2byte 0xf918
	movs	r0, #17
	bl 0x02008cb4
	movs	r0, #18
	bl 0x02008cb4
	movs	r0, #192
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xf8ee
	cmp	r0, #0
	beq.n	.L_02000f44
	movs	r0, #19
	bl 0x02008cb4
	movs	r0, #20
	bl 0x02008cb4
.L_02000f44:
	ldr	r2, [pc, #20]
	movs	r0, #135
	movs	r3, #1
	lsls	r0, r0, #1
	str	r3, [r2, #0]
	adds	r0, #255
	.2byte 0xf004
	.2byte 0xf8e2
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xe29c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #828]
	movs	r2, #150
	ldr	r3, [r7, #0]
	lsls	r2, r2, #1
	sub	sp, #8
	cmp	r3, r2
	bge.n	.L_02000f80
	bl 0x02008d18
	cmp	r0, #0
	bne.n	.L_02000f7e
	b.n	.L_02001294
.L_02000f7e:
	b.n	.L_0200129c
.L_02000f80:
	movs	r2, #150
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_02000fb2
	movs	r5, #13
	movs	r0, #15
	movs	r1, #15
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	.2byte 0xf004
	.2byte 0xf8ef
	movs	r3, #63
	str	r3, [sp, #0]
	movs	r0, #65
	movs	r1, #15
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	.2byte 0xf004
	.2byte 0xf8e6
	bl 0x02008d64
	b.n	.L_02001294
.L_02000fb2:
	movs	r2, #176
	lsls	r2, r2, #4
	adds	r2, #183
	mov	r8, r2
	cmp	r3, r8
	ble.n	.L_02000fc0
	b.n	.L_0200119c
.L_02000fc0:
	movs	r0, #9
	.2byte 0xf004
	.2byte 0xf90d
	adds	r6, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	.2byte 0xf004
	.2byte 0xf89f
	ldr	r3, [r7, #0]
	cmp	r0, #0
	beq.n	.L_020010ae
	movs	r2, #158
	lsls	r2, r2, #1
	cmp	r3, r2
	bgt.n	.L_02001022
	ldr	r3, [r6, #12]
	movs	r2, #144
	lsls	r2, r2, #11
	movs	r5, #128
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #128
	str	r3, [r6, #12]
	lsls	r1, r1, #10
	movs	r0, #11
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #192
	lsls	r1, r1, #9
	movs	r0, #13
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r0, #14
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #15
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r0, #16
	b.n	.L_020010dc
.L_02001022:
	movs	r2, #150
	lsls	r2, r2, #2
	cmp	r3, r2
	bge.n	.L_0200105a
	movs	r0, #130
	lsls	r0, r0, #2
	.2byte 0xf004
	.2byte 0xf86f
	cmp	r0, #0
	beq.n	.L_020010ee
	ldr	r3, [r7, #0]
	movs	r2, #200
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_020010ee
	movs	r3, #13
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #22
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	.2byte 0xf004
	.2byte 0xf893
	ldr	r0, [pc, #596]
	.2byte 0xf004
	.2byte 0xf840
	b.n	.L_020010ee
.L_0200105a:
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	cmp	r3, r1
	ble.n	.L_020010fc
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #580]
	movs	r5, #192
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #192
	str	r3, [r6, #12]
	lsls	r1, r1, #10
	movs	r0, #11
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #144
	lsls	r1, r1, #10
	movs	r0, #13
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	lsls	r1, r1, #9
	movs	r0, #14
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #15
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r0, #16
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r0, #16
	b.n	.L_0200115c
.L_020010ae:
	movs	r2, #158
	lsls	r2, r2, #1
	cmp	r3, r2
	bgt.n	.L_020010e6
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #500]
	movs	r5, #128
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #128
	str	r3, [r6, #12]
	lsls	r1, r1, #9
	movs	r0, #11
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #13
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r0, #14
.L_020010dc:
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x02008cd8
	b.n	.L_0200116a
.L_020010e6:
	movs	r2, #150
	lsls	r2, r2, #2
	cmp	r3, r2
	bge.n	.L_020010f2
.L_020010ee:
	ldr	r5, [pc, #452]
	b.n	.L_02001112
.L_020010f2:
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	cmp	r3, r1
	bgt.n	.L_0200112c
.L_020010fc:
	ldr	r5, [pc, #436]
	ldrh	r2, [r5, #0]
	cmp	r2, #0
	bne.n	.L_02001112
	ldr	r3, [pc, #432]
	movs	r0, #131
.L_02001108:
	str	r2, [r3, #0]
	lsls	r0, r0, #2
	str	r1, [r7, #0]
	bl 0x0200d120
.L_02001112:
	ldrh	r0, [r5, #0]
	.2byte 0xf003
	.2byte 0xffe8
	ldr	r3, [r6, #12]
	asrs	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r6, #12]
	ldrh	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r5, #0]
	b.n	.L_0200116a
.L_0200112c:
	ldr	r3, [r6, #12]
.L_0200112e:
	ldr	r2, [pc, #396]
	movs	r5, #192
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #192
	str	r3, [r6, #12]
	lsls	r1, r1, #9
	movs	r0, #11
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #13
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r0, #14
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r0, #14
.L_0200115c:
	bl 0x0200d1e0
	ldr	r3, [r0, #28]
	cmp	r3, r5
	bgt.n	.L_0200116a
	mov	r3, r8
	str	r3, [r7, #0]
.L_0200116a:
	movs	r0, #130
	lsls	r0, r0, #2
	.2byte 0xf003
	.2byte 0xffcf
	cmp	r0, #0
	bne.n	.L_02001178
	b.n	.L_02001294
.L_02001178:
	ldr	r3, [pc, #324]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r0, #12]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r0, #60]
	movs	r3, #0
	str	r3, [r0, #40]
	b.n	.L_02001294
.L_0200119c:
	movs	r2, #176
	lsls	r2, r2, #4
	adds	r2, #184
	cmp	r3, r2
	bne.n	.L_02001262
	movs	r5, #13
	movs	r0, #8
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d178
	movs	r3, #63
	str	r3, [sp, #0]
	movs	r0, #58
	movs	r3, #1
	movs	r1, #0
	movs	r2, #1
	str	r5, [sp, #4]
	bl 0x0200d178
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r2, #0
	movs	r0, #16
	movs	r1, #0
	bl 0x0200d230
	movs	r1, #1
	movs	r0, #9
	bl 0x0200d248
	movs	r0, #9
	bl 0x0200d1e0
	movs	r1, #216
	movs	r3, #216
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #16
	bl 0x0200d150
	movs	r0, #9
	bl 0x0200d1e0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #9
	bl 0x0200d2b8
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02001294
	ldr	r5, [pc, #128]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r2, #0
	ldr	r1, [r0, #8]
	ldr	r3, [r0, #16]
	bl 0x0200d150
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #1
	bl 0x0200d188
	b.n	.L_02001294
.L_02001262:
	bl 0x02008d18
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0200129c
	bl 0x0200d0e0
	ldr	r3, [r7, #0]
	lsls	r0, r0, #7
	lsrs	r0, r0, #16
	adds	r3, r3, r0
	str	r3, [r7, #0]
	bl 0x0200d0e0
	movs	r2, #156
	lsls	r2, r2, #6
	adds	r2, #16
	adds	r3, r0, #0
	muls	r3, r2
	lsrs	r3, r3, #16
	adds	r3, r3, r2
	ldr	r2, [r7, #0]
	cmp	r2, r3
	bls.n	.L_02001294
	str	r5, [r7, #0]
.L_02001294:
	ldr	r2, [pc, #12]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0200129c:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200e2a0
	.4byte 0x02008f61
	.4byte 0xfff94000
	.4byte 0x00027999
	.4byte 0x0200e2a8
	.4byte 0x0200e298
	.4byte 0xfffc499a
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #840]
	movs	r2, #150
	ldr	r3, [r7, #0]
	lsls	r2, r2, #1
.L_020012d2:
	sub	sp, #8
	cmp	r3, r2
	bge.n	.L_020012e4
	bl 0x02008d3c
	cmp	r0, #0
	bne.n	.L_020012e2
	b.n	.L_02001604
.L_020012e2:
	b.n	.L_0200160c
.L_020012e4:
	movs	r2, #150
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_02001318
	movs	r3, #23
	str	r3, [sp, #0]
	movs	r5, #15
	movs	r0, #15
	movs	r1, #15
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200d178
	movs	r3, #73
	str	r3, [sp, #0]
	movs	r0, #65
	movs	r1, #15
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200d178
	bl 0x02008e60
	b.n	.L_02001604
.L_02001318:
	movs	r2, #176
	lsls	r2, r2, #4
	adds	r2, #183
	mov	r8, r2
.L_02001320:
	cmp	r3, r8
	ble.n	.L_02001326
	b.n	.L_02001508
.L_02001326:
	movs	r0, #10
	bl 0x0200d1e0
	adds	r6, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200d110
	ldr	r3, [r7, #0]
	cmp	r0, #0
	beq.n	.L_02001416
	movs	r2, #158
	lsls	r2, r2, #1
	cmp	r3, r2
	bgt.n	.L_02001386
	ldr	r3, [r6, #12]
	movs	r2, #144
	lsls	r2, r2, #11
	movs	r5, #128
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #128
	str	r3, [r6, #12]
	lsls	r1, r1, #10
	movs	r0, #12
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #192
	lsls	r1, r1, #9
	movs	r0, #17
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	lsls	r1, r1, #9
	movs	r0, #18
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #19
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r0, #20
	b.n	.L_02001444
.L_02001386:
	movs	r2, #150
	lsls	r2, r2, #2
	cmp	r3, r2
	bge.n	.L_020013c2
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02001456
	ldr	r3, [r7, #0]
	movs	r2, #200
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_02001456
	movs	r3, #23
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #23
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d178
	ldr	r0, [pc, #604]
	bl 0x0200d0d8
	b.n	.L_02001456
.L_020013c2:
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	cmp	r3, r1
	ble.n	.L_02001464
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #588]
	movs	r5, #192
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #192
	str	r3, [r6, #12]
	lsls	r1, r1, #10
	movs	r0, #12
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #144
	lsls	r1, r1, #10
	movs	r0, #17
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	lsls	r1, r1, #9
	movs	r0, #18
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #19
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r0, #20
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r0, #20
	b.n	.L_020014c6
.L_02001416:
	movs	r2, #158
	lsls	r2, r2, #1
	cmp	r3, r2
	bgt.n	.L_0200144e
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #508]
	movs	r5, #128
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #128
	str	r3, [r6, #12]
	lsls	r1, r1, #9
	movs	r0, #12
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #17
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r0, #18
.L_02001444:
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x02008cd8
	b.n	.L_020014d4
.L_0200144e:
	movs	r2, #150
	lsls	r2, r2, #2
	cmp	r3, r2
	bge.n	.L_0200145a
.L_02001456:
	ldr	r5, [pc, #460]
	b.n	.L_0200147c
.L_0200145a:
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	cmp	r3, r1
	bgt.n	.L_02001496
.L_02001464:
	ldr	r5, [pc, #444]
	ldrh	r2, [r5, #0]
	cmp	r2, #0
	bne.n	.L_0200147c
	ldr	r3, [pc, #440]
	movs	r0, #135
	lsls	r0, r0, #1
	str	r2, [r3, #0]
.L_02001474:
	adds	r0, #255
	str	r1, [r7, #0]
	bl 0x0200d120
.L_0200147c:
	ldrh	r0, [r5, #0]
	bl 0x0200d0e8
	ldr	r3, [r6, #12]
	asrs	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r6, #12]
	ldrh	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r5, #0]
	b.n	.L_020014d4
.L_02001496:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #400]
	movs	r5, #192
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #192
	str	r3, [r6, #12]
	lsls	r1, r1, #9
	movs	r0, #12
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #17
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r0, #18
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r0, #18
.L_020014c6:
	bl 0x0200d1e0
	ldr	r3, [r0, #28]
	cmp	r3, r5
	bgt.n	.L_020014d4
	mov	r3, r8
	str	r3, [r7, #0]
.L_020014d4:
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_020014e4
	b.n	.L_02001604
.L_020014e4:
	ldr	r3, [pc, #328]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r0, #12]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r0, #60]
	movs	r3, #0
	str	r3, [r0, #40]
	b.n	.L_02001604
.L_02001508:
	movs	r2, #176
	lsls	r2, r2, #4
	adds	r2, #184
	cmp	r3, r2
	bne.n	.L_020015d2
	movs	r3, #23
	str	r3, [sp, #0]
	movs	r5, #15
	movs	r0, #8
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200d178
	movs	r3, #73
	str	r3, [sp, #0]
	movs	r0, #58
	movs	r3, #1
	movs	r1, #0
	movs	r2, #1
	str	r5, [sp, #4]
	bl 0x0200d178
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r2, #0
	movs	r0, #20
	movs	r1, #0
	bl 0x0200d230
	movs	r1, #1
	movs	r0, #10
	bl 0x0200d248
	movs	r0, #10
	bl 0x0200d1e0
	movs	r1, #188
	movs	r3, #248
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #16
	bl 0x0200d150
	movs	r0, #10
	bl 0x0200d1e0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #10
	bl 0x0200d2b8
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02001604
	ldr	r5, [pc, #128]
	movs	r3, #133
	lsls	r3, r3, #2
.L_020015b2:
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r2, #0
	ldr	r1, [r0, #8]
	ldr	r3, [r0, #16]
	bl 0x0200d150
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #1
	bl 0x0200d188
	b.n	.L_02001604
.L_020015d2:
	bl 0x02008d3c
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0200160c
	bl 0x0200d0e0
	ldr	r3, [r7, #0]
	lsls	r0, r0, #7
	lsrs	r0, r0, #16
	adds	r3, r3, r0
	str	r3, [r7, #0]
	bl 0x0200d0e0
	movs	r2, #156
	lsls	r2, r2, #6
.L_020015f2:
	adds	r2, #16
	adds	r3, r0, #0
	muls	r3, r2
	lsrs	r3, r3, #16
	adds	r3, r3, r2
	ldr	r2, [r7, #0]
	cmp	r2, r3
	bls.n	.L_02001604
	str	r5, [r7, #0]
.L_02001604:
	ldr	r2, [pc, #12]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0200160c:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200e2a4
	.4byte 0x020092c5
	.4byte 0xfff94000
	.4byte 0x00027999
	.4byte 0x0200e2aa
	.4byte 0x0200e29c
	.4byte 0xfffc499a
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #13
	sub	sp, #8
	bl 0x0200d1e0
	adds	r5, r0, #0
	movs	r0, #15
	bl 0x0200d1e0
	ldr	r7, [pc, #416]
	movs	r1, #150
	ldr	r3, [r7, #0]
	lsls	r1, r1, #1
	adds	r6, r0, #0
	cmp	r3, r1
	bne.n	.L_020016b6
	movs	r0, #162
	bl 0x0200d388
	movs	r0, #13
	bl 0x0200d1e0
	movs	r1, #220
	movs	r3, #204
	lsls	r1, r1, #17
	ldr	r2, [pc, #388]
	lsls	r3, r3, #16
	bl 0x0200d150
	movs	r0, #15
	bl 0x0200d1e0
	movs	r1, #220
	movs	r3, #200
	ldr	r2, [pc, #368]
	lsls	r3, r3, #16
	lsls	r1, r1, #17
	bl 0x0200d150
	movs	r0, #13
	movs	r1, #3
	bl 0x0200d2b8
	movs	r0, #15
	movs	r1, #3
	bl 0x0200d2b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	movs	r0, #13
	movs	r1, #0
	str	r3, [r6, #28]
	bl 0x0200d248
	movs	r0, #15
	movs	r1, #2
	bl 0x0200d248
	b.n	.L_020017da
.L_020016b6:
	ldr	r1, [pc, #312]
	adds	r2, r3, r1
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #162
	cmp	r2, r1
	bhi.n	.L_0200170c
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [r5, #12]
	adds	r1, r2, #0
	movs	r0, #15
	bl 0x02008cd8
	ldr	r3, [r5, #12]
	ldr	r1, [pc, #276]
	cmp	r3, r1
	ble.n	.L_020016ec
	movs	r0, #13
	movs	r1, #2
	bl 0x0200d2b8
.L_020016ec:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	blt.n	.L_020017da
	movs	r3, #27
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	bl 0x0200d178
	movs	r3, #250
	lsls	r3, r3, #3
	b.n	.L_020017d8
.L_0200170c:
	movs	r2, #175
	lsls	r2, r2, #4
	cmp	r3, r2
	bge.n	.L_0200178e
	ldr	r6, [pc, #224]
	ldrh	r0, [r6, #0]
	bl 0x0200d0e8
	ldr	r3, [r5, #12]
	asrs	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldrh	r3, [r6, #0]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	strh	r3, [r6, #0]
	movs	r2, #128
	ldr	r3, [r7, #0]
	lsls	r2, r2, #4
	adds	r2, #232
	cmp	r3, r2
	bne.n	.L_0200174e
	movs	r3, #27
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #27
	movs	r1, #10
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d178
.L_0200174e:
	movs	r1, #128
	ldr	r3, [r7, #0]
	lsls	r1, r1, #4
	adds	r1, #252
	cmp	r3, r1
	ble.n	.L_020017da
	ldrh	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_020017da
	movs	r0, #130
.L_02001762:
	lsls	r0, r0, #2
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02001788
	movs	r3, #27
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #21
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d178
	ldr	r0, [pc, #120]
	bl 0x0200d0d8
	b.n	.L_020017da
.L_02001788:
	movs	r3, #175
	lsls	r3, r3, #4
	b.n	.L_020017d8
.L_0200178e:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #108]
	movs	r0, #15
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [r5, #12]
	adds	r1, r2, #0
	bl 0x02008cf8
	ldr	r3, [r5, #12]
	ldr	r1, [pc, #76]
	cmp	r3, r1
	bge.n	.L_020017b4
	movs	r0, #13
	movs	r1, #3
	bl 0x0200d2b8
.L_020017b4:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #52]
	cmp	r3, r2
	bgt.n	.L_020017da
	movs	r0, #245
	bl 0x0200d388
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r3, #0
.L_020017d8:
	str	r3, [r7, #0]
.L_020017da:
	ldr	r2, [pc, #12]
	add	sp, #8
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200e2a0
	.4byte 0xffec0000
	.4byte 0xfffffed3
	.4byte 0xfffb0000
	.4byte 0x0200e2a8
	.4byte 0x02009635
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb5e0
	movs	r0, #14
	sub	sp, #8
	bl 0x0200d1e0
	adds	r5, r0, #0
	movs	r0, #16
	bl 0x0200d1e0
	ldr	r7, [pc, #416]
	movs	r1, #150
	ldr	r3, [r7, #0]
	lsls	r1, r1, #1
	adds	r6, r0, #0
	cmp	r3, r1
	bne.n	.L_02001886
	movs	r0, #162
	bl 0x0200d388
	movs	r0, #14
	bl 0x0200d1e0
	movs	r1, #150
	movs	r3, #236
	lsls	r1, r1, #18
	ldr	r2, [pc, #388]
	lsls	r3, r3, #16
	bl 0x0200d150
	movs	r0, #16
	bl 0x0200d1e0
	movs	r1, #150
	movs	r3, #232
	ldr	r2, [pc, #368]
	lsls	r3, r3, #16
	lsls	r1, r1, #18
	bl 0x0200d150
	movs	r0, #14
	movs	r1, #3
	bl 0x0200d2b8
	movs	r0, #16
	movs	r1, #3
	bl 0x0200d2b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	movs	r0, #14
	movs	r1, #0
	str	r3, [r6, #28]
	bl 0x0200d248
	movs	r0, #16
	movs	r1, #2
	bl 0x0200d248
	b.n	.L_020019ac
.L_02001886:
	ldr	r1, [pc, #312]
	adds	r2, r3, r1
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #162
	cmp	r2, r1
	bhi.n	.L_020018dc
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [r5, #12]
	adds	r1, r2, #0
	movs	r0, #16
	bl 0x02008cd8
	ldr	r3, [r5, #12]
	ldr	r1, [pc, #276]
	cmp	r3, r1
	ble.n	.L_020018bc
	movs	r0, #14
	movs	r1, #2
	bl 0x0200d2b8
.L_020018bc:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	blt.n	.L_020019ac
	movs	r3, #37
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	bl 0x0200d178
	movs	r3, #250
	lsls	r3, r3, #3
	b.n	.L_020019aa
.L_020018dc:
	movs	r2, #175
	lsls	r2, r2, #4
	cmp	r3, r2
	bge.n	.L_02001960
	ldr	r6, [pc, #224]
	ldrh	r0, [r6, #0]
	bl 0x0200d0e8
	ldr	r3, [r5, #12]
	asrs	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldrh	r3, [r6, #0]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	strh	r3, [r6, #0]
	movs	r2, #128
	ldr	r3, [r7, #0]
	lsls	r2, r2, #4
	adds	r2, #232
	cmp	r3, r2
	bne.n	.L_0200191e
	movs	r3, #37
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #37
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d178
.L_0200191e:
	movs	r1, #128
	ldr	r3, [r7, #0]
	lsls	r1, r1, #4
	adds	r1, #252
	cmp	r3, r1
	ble.n	.L_020019ac
	ldrh	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_020019ac
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_0200195a
	movs	r3, #37
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #22
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d178
	ldr	r0, [pc, #120]
	bl 0x0200d0d8
	b.n	.L_020019ac
.L_0200195a:
	movs	r3, #175
	lsls	r3, r3, #4
	b.n	.L_020019aa
.L_02001960:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #108]
	movs	r0, #16
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [r5, #12]
	adds	r1, r2, #0
	bl 0x02008cf8
	ldr	r3, [r5, #12]
	ldr	r1, [pc, #72]
	cmp	r3, r1
	bge.n	.L_02001986
	movs	r0, #14
	movs	r1, #3
	bl 0x0200d2b8
.L_02001986:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #48]
	cmp	r3, r2
	bgt.n	.L_020019ac
	movs	r0, #245
	bl 0x0200d388
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r3, #0
.L_020019aa:
	str	r3, [r7, #0]
.L_020019ac:
	ldr	r2, [pc, #8]
	add	sp, #8
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r5, r6, r7, pc}
	.4byte 0x0200e2a4
	.4byte 0xffec0000
	.4byte 0xfffffed3
	.4byte 0xfffb0000
	.4byte 0x0200e2aa
	.4byte 0x02009805
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #348]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r6, #0
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r5, #16]
	asrs	r7, r3, #20
	mov	r3, r8
	cmp	r3, #13
	bne.n	.L_02001a0c
	cmp	r7, #13
	bne.n	.L_02001a0c
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200d118
	movs	r6, #1
	b.n	.L_02001a14
.L_02001a0c:
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200d120
.L_02001a14:
	mov	r2, r8
	cmp	r2, #23
	bne.n	.L_02001a2c
	cmp	r7, #15
	bne.n	.L_02001a2c
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d118
	movs	r6, #1
	b.n	.L_02001a36
.L_02001a2c:
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d120
.L_02001a36:
	cmp	r6, #0
	beq.n	.L_02001a44
	movs	r0, #126
	adds	r0, #255
	bl 0x0200d118
	b.n	.L_02001a4c
.L_02001a44:
	movs	r0, #126
	adds	r0, #255
	bl 0x0200d120
.L_02001a4c:
	ldr	r3, [pc, #236]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02001a8c
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bne.n	.L_02001a8c
	ldr	r3, [r5, #8]
	movs	r2, #142
	lsls	r2, r2, #17
	cmp	r3, r2
	ble.n	.L_02001a8c
	movs	r2, #154
	lsls	r2, r2, #17
	cmp	r3, r2
	bge.n	.L_02001a8c
	ldr	r3, [r5, #16]
	movs	r2, #200
	lsls	r2, r2, #16
	cmp	r3, r2
	bge.n	.L_02001a8c
	movs	r0, #134
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02001a8c
	ldr	r1, [pc, #184]
	adds	r0, r5, #0
	bl 0x02008778
.L_02001a8c:
	ldr	r3, [pc, #180]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02001aca
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bne.n	.L_02001aca
	ldr	r3, [r5, #8]
	movs	r2, #165
	lsls	r2, r2, #17
	cmp	r3, r2
	ble.n	.L_02001aca
	ldr	r2, [pc, #160]
	cmp	r3, r2
	bgt.n	.L_02001aca
	ldr	r3, [r5, #16]
	movs	r2, #216
	lsls	r2, r2, #16
	cmp	r3, r2
	bge.n	.L_02001aca
	movs	r0, #134
.L_02001ab6:
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02001aca
	ldr	r1, [pc, #136]
	adds	r0, r5, #0
	bl 0x02008778
.L_02001aca:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #14
.L_02001ad0:
	bl 0x0200d110
	cmp	r0, #0
.L_02001ad6:
	beq.n	.L_02001b06
	movs	r0, #131
	lsls	r0, r0, #2
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02001b30
	movs	r0, #135
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02001b30
	movs	r0, #1
	negs	r0, r0
	bl 0x0200d388
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #14
	bl 0x0200d120
	b.n	.L_02001b30
.L_02001b06:
	movs	r0, #131
	lsls	r0, r0, #2
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02001b20
	movs	r0, #135
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02001b30
.L_02001b20:
	movs	r0, #162
	bl 0x0200d388
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #14
	bl 0x0200d118
.L_02001b30:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200e298
	.4byte 0x00026666
	.4byte 0x0200e29c
	.4byte 0x0165ffff
	.2byte 0x9999
	.2byte 0x0003
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #292]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r0, r0, r3
	mov	r9, r0
	ldr	r0, [r0, #0]
	bl 0x0200d1e0
	adds	r6, r0, #0
	ldr	r2, [r6, #8]
	ldr	r1, [r6, #16]
	asrs	r3, r2, #20
	asrs	r0, r1, #20
	mov	sl, r3
	ldr	r3, [r6, #12]
	mov	r8, r0
	movs	r0, #128
	lsls	r0, r0, #13
	movs	r7, #0
	cmp	r3, r0
	bgt.n	.L_02001c76
	ldr	r0, [pc, #252]
	adds	r3, r2, r0
	movs	r2, #240
	lsls	r2, r2, #16
	cmp	r3, r2
	bhi.n	.L_02001c76
	ldr	r0, [pc, #244]
	movs	r2, #200
	adds	r3, r1, r0
	lsls	r2, r2, #15
	cmp	r3, r2
	bhi.n	.L_02001c76
	mov	r3, sl
	cmp	r3, #27
	bne.n	.L_02001bda
	mov	r0, r8
	cmp	r0, #12
	bne.n	.L_02001bda
	movs	r0, #13
	bl 0x0200d1e0
	mov	r2, r9
	adds	r5, r0, #0
	ldr	r0, [r2, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	movs	r0, #13
	movs	r1, #4
	bl 0x0200d248
	adds	r3, r6, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r0, #130
	ldr	r3, [r5, #12]
	lsls	r0, r0, #2
	str	r3, [r6, #12]
	bl 0x0200d118
.L_02001bd8:
	movs	r7, #1
.L_02001bda:
	mov	r3, sl
	cmp	r3, #37
.L_02001bde:
	bne.n	.L_02001c22
	mov	r0, r8
	cmp	r0, #14
	bne.n	.L_02001c22
	movs	r0, #14
	bl 0x0200d1e0
	ldr	r3, [pc, #144]
	movs	r2, #133
.L_02001bf0:
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
.L_02001bf6:
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	movs	r0, #14
	movs	r1, #4
	bl 0x0200d248
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #133
	ldr	r3, [r5, #12]
	lsls	r0, r0, #1
	str	r3, [r6, #12]
	adds	r0, #255
	bl 0x0200d118
	movs	r7, #1
.L_02001c22:
	cmp	r7, #0
	beq.n	.L_02001c30
	movs	r0, #126
	adds	r0, #255
	bl 0x0200d118
	b.n	.L_02001c76
.L_02001c30:
	movs	r0, #13
	movs	r1, #0
	bl 0x0200d248
	movs	r1, #0
	movs	r0, #14
	bl 0x0200d248
	ldr	r3, [pc, #60]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	movs	r1, #1
	bl 0x0200d188
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #3
	movs	r0, #130
	strb	r3, [r2, #0]
	lsls	r0, r0, #2
	bl 0x0200d120
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d120
	movs	r0, #126
	adds	r0, #255
	bl 0x0200d120
.L_02001c76:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfe700000
	.2byte 0x0000
	.2byte 0xff4c
	.2byte 0xb520
.L_02001c8e:
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	ldr	r3, [r0, #80]
	movs	r0, #13
	ldrb	r5, [r3, #9]
	lsls	r5, r5, #28
	lsrs	r5, r5, #30
	adds	r1, r5, #0
	bl 0x0200d2b8
	adds	r1, r5, #0
	movs	r0, #14
	bl 0x0200d2b8
	movs	r0, #15
	adds	r1, r5, #0
	bl 0x0200d2b8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
.L_02001cd0:
	bl 0x0200d1e0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	adds	r6, r0, #0
.L_02001cdc:
	bl 0x0200d3b0
	movs	r3, #184
	lsls	r3, r3, #1
	adds	r5, r5, r3
	ldr	r3, [r5, #0]
.L_02001ce8:
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrb	r3, [r3, #2]
	cmp	r3, #0
	beq.n	.L_02001cf8
	movs	r3, #0
.L_02001cf4:
	str	r3, [r6, #12]
	str	r3, [r6, #20]
.L_02001cf8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200d1a8
	bl 0x0200d1c8
	pop	{pc}
	.2byte 0x0000
	.2byte 0x17b7
	.2byte 0x0000
	push	{lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x0200d1a8
	bl 0x0200d1c8
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0dfa
	.2byte 0x0000
	push	{lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #9
	bl 0x0200d1f8
	movs	r0, #1
	bl 0x0200d0c8
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #9
	bl 0x0200d268
	ldr	r0, [pc, #64]
	bl 0x0200d280
	movs	r1, #0
.L_02001d72:
	movs	r0, #9
	bl 0x0200d2a0
	movs	r0, #9
	bl 0x0200d1e0
.L_02001d7e:
	movs	r3, #192
	lsls	r3, r3, #6
	strh	r3, [r0, #6]
	movs	r0, #1
	bl 0x0200d0c8
.L_02001d8a:
	ldr	r1, [pc, #36]
	movs	r0, #9
	bl 0x0200d1f0
	movs	r0, #1
	bl 0x0200d0c8
	bl 0x0200d1c8
	movs	r0, #128
	lsls	r0, r0, #4
.L_02001da0:
	adds	r0, #69
	bl 0x0200d118
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00001759
	.2byte 0xd514
	.2byte 0x0200
	push	{lr}
	bl 0x0200d1c0
.L_02001dba:
	movs	r0, #0
	bl 0x0200d370
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d268
	ldr	r0, [pc, #52]
	bl 0x0200d280
	movs	r1, #0
	movs	r0, #10
	bl 0x0200d298
	movs	r0, #10
	bl 0x0200d1e0
	movs	r3, #160
	lsls	r3, r3, #7
	strh	r3, [r0, #6]
	movs	r0, #1
	bl 0x0200d0c8
	bl 0x0200d1c8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #70
	bl 0x0200d118
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x175c
	.2byte 0x0000
	push	{lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	ldr	r0, [pc, #16]
	bl 0x0200d280
	movs	r1, #0
	movs	r0, #13
	bl 0x0200d2a0
	bl 0x0200d1c8
	pop	{pc}
	.2byte 0x17a9
	.2byte 0x0000
	push	{lr}
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #9
.L_02001e3a:
	bl 0x0200d2c8
	bl 0x02009d40
	pop	{pc}
	push	{lr}
.L_02001e46:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200d2c8
	bl 0x02009db4
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #212]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r1, r3, #20
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	cmp	r3, #5
	bne.n	.L_02001e98
	cmp	r1, #9
	bne.n	.L_02001e86
	ldr	r3, [pc, #184]
	movs	r2, #16
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001f2e
.L_02001e86:
	cmp	r1, #11
	bne.n	.L_02001eac
	ldr	r3, [pc, #168]
	movs	r2, #32
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001eac
	b.n	.L_02001f2e
.L_02001e98:
	cmp	r1, #10
	bne.n	.L_02001eac
	cmp	r3, #6
	bne.n	.L_02001eac
	ldr	r3, [pc, #144]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001f2e
.L_02001eac:
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	ldr	r5, [pc, #112]
	strb	r3, [r2, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d1e8
	movs	r2, #80
	ldr	r0, [r5, #0]
	movs	r1, #168
	bl 0x0200d220
	movs	r1, #192
	lsls	r1, r1, #8
	ldr	r0, [r5, #0]
	bl 0x0200d2b0
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	ldr	r0, [r5, #0]
	movs	r1, #13
	bl 0x0200d248
	movs	r1, #168
	movs	r3, #160
	ldr	r2, [pc, #52]
	lsls	r3, r3, #15
	lsls	r1, r1, #16
	adds	r0, r6, #0
	bl 0x0200d160
	adds	r0, r6, #0
	bl 0x0200d168
	movs	r1, #10
	ldr	r0, [r5, #0]
	bl 0x0200d248
	movs	r0, #123
	bl 0x0200d3d8
	bl 0x0200d320
	bl 0x0200d328
	movs	r0, #8
	bl 0x0200d2f8
.L_02001f2e:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x03001150
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0xb520
	sub	sp, #8
	movs	r5, #2
	movs	r1, #39
	movs	r2, #24
	movs	r3, #38
	movs	r0, #39
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d170
	movs	r0, #10
	bl 0x0200d0c8
	movs	r1, #41
	movs	r2, #24
	movs	r3, #38
	movs	r0, #39
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d170
	movs	r0, #20
	bl 0x0200d0c8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d118
	add	sp, #8
	pop	{r5, pc}
.L_02001f7c:
	push	{r5, lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200d1e8
	movs	r1, #200
	movs	r2, #162
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200d2b0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02001fce
	movs	r0, #188
	bl 0x0200d3d8
	bl 0x02009f3c
.L_02001fce:
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200d248
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x0200d1e8
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200d2b8
	movs	r1, #200
	movs	r2, #160
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	bl 0x0200d218
	movs	r0, #123
	bl 0x0200d3d8
	bl 0x0200d320
	bl 0x0200d328
	movs	r0, #9
	bl 0x0200d2f8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
.L_02002016:
	ldr	r5, [pc, #168]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	adds	r6, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r1, #208
	movs	r2, #144
.L_02002034:
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #128
.L_02002040:
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #10
	bl 0x0200d2a8
	movs	r1, #192
.L_0200204c:
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d2a8
	adds	r2, r6, #0
.L_02002058:
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d2b8
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	movs	r5, #0
.L_02002074:
	ldr	r3, [r6, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r6, #16]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d0c8
	cmp	r5, #7
	bls.n	.L_02002074
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #13
	bl 0x0200d248
	movs	r5, #0
.L_0200209c:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #36]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #12]
	adds	r5, #1
	bl 0x0200d0c8
	cmp	r5, #7
	bls.n	.L_0200209c
	bl 0x0200d320
	bl 0x0200d328
	movs	r0, #4
	bl 0x0200d2f8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	ldr	r0, [pc, #8]
	bl 0x0200d3c0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd4dc
	.2byte 0x0200
	push	{r5, lr}
.L_020020da:
	movs	r0, #12
	sub	sp, #8
	bl 0x0200d1e0
	ldr	r3, [r0, #8]
	asrs	r5, r3, #20
.L_020020e6:
	bl 0x0200d3c8
	cmp	r5, #43
	bne.n	.L_020020fa
	movs	r0, #137
	lsls	r0, r0, #1
.L_020020f2:
	adds	r0, #255
	bl 0x0200d118
	b.n	.L_02002104
.L_020020fa:
	movs	r0, #137
	lsls	r0, r0, #1
.L_020020fe:
	adds	r0, #255
	bl 0x0200d120
.L_02002104:
	cmp	r5, #39
	beq.n	.L_0200211c
.L_02002108:
	movs	r3, #39
	movs	r2, #10
.L_0200210c:
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #39
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
.L_02002118:
	bl 0x0200d178
.L_0200211c:
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
.L_02002122:
	movs	r0, #8
	bl 0x0200d1e0
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	subs	r3, #15
	cmp	r3, #6
	bls.n	.L_0200213c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x0200d118
.L_0200213c:
	movs	r0, #134
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d118
	ldr	r0, [pc, #8]
	bl 0x0200d3c0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd4e0
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #8
	bl 0x0200d1e0
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	asrs	r5, r3, #20
	bl 0x0200d3c8
	cmp	r6, #13
	bne.n	.L_0200217a
	cmp	r5, #13
	bne.n	.L_0200217a
	movs	r0, #192
	lsls	r0, r0, #2
.L_02002174:
	bl 0x0200d118
	b.n	.L_02002182
.L_0200217a:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200d120
.L_02002182:
	cmp	r6, #23
	bne.n	.L_02002196
	cmp	r5, #15
	bne.n	.L_02002196
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200d118
	b.n	.L_020021a0
.L_02002196:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200d120
.L_020021a0:
	movs	r0, #128
	lsls	r0, r0, #2
.L_020021a4:
	adds	r0, #10
	bl 0x0200d120
	movs	r0, #134
	lsls	r0, r0, #1
	adds	r0, #255
.L_020021b0:
	bl 0x0200d120
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
.L_020021ba:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200d120
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d120
	ldr	r0, [pc, #8]
	bl 0x0200d3c0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xd4e0
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x0200d1e0
	ldr	r3, [r0, #8]
	asrs	r5, r3, #20
	bl 0x0200d3c8
	cmp	r5, #19
	bne.n	.L_02002218
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200d118
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200d118
	movs	r3, #28
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #3
	movs	r3, #3
	str	r5, [sp, #0]
	bl 0x0200d178
	b.n	.L_02002242
.L_02002218:
	cmp	r5, #21
	bne.n	.L_02002242
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d118
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200d118
	movs	r3, #19
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #3
	movs	r3, #3
	bl 0x0200d178
.L_02002242:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	adds	r6, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d2c8
	movs	r0, #132
	bl 0x0200d3d8
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #10
	bl 0x0200d278
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	movs	r0, #20
	bl 0x0200d0c8
	ldr	r3, [r6, #40]
	cmp	r3, #0
	beq.n	.L_020022a0
.L_02002294:
	movs	r0, #1
	bl 0x0200d0c8
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02002294
.L_020022a0:
	ldr	r3, [pc, #24]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d278
	bl 0x0200d1c8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #188
	bl 0x0200d3d8
	movs	r3, #2
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #77
	movs	r1, #33
	movs	r2, #77
	movs	r3, #24
	bl 0x0200d170
	movs	r3, #17
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #30
	movs	r2, #2
	movs	r3, #1
	movs	r0, #32
	bl 0x0200d178
	movs	r0, #10
	bl 0x0200d0c8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d118
	bl 0x0200d1c8
	add	sp, #8
.L_02002310:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #5
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r1, #0
	movs	r2, #1
	movs	r0, #0
	bl 0x0200d178
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #66
	bl 0x0200d118
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #4
	movs	r1, #89
	movs	r2, #237
	bl 0x0200d220
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d2a8
	movs	r3, #160
	lsls	r3, r3, #8
	movs	r2, #16
	movs	r1, #16
	movs	r0, #14
	bl 0x0200d378
	movs	r0, #14
	bl 0x0200d228
	movs	r0, #20
	bl 0x0200d1b8
	ldr	r0, [pc, #108]
	bl 0x0200d280
	movs	r1, #0
	movs	r0, #14
	bl 0x0200d298
	movs	r0, #30
	bl 0x0200d1b8
	movs	r0, #14
	movs	r1, #3
	bl 0x0200d248
	movs	r1, #3
	movs	r0, #4
	bl 0x0200d250
	movs	r0, #20
	bl 0x0200d1b8
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #64]
	adds	r2, #153
	bl 0x0200d1e8
	movs	r0, #14
	movs	r1, #2
	bl 0x0200d248
	movs	r0, #4
.L_020023b2:
	bl 0x0200d1e0
	cmp	r0, #0
	beq.n	.L_020023c8
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #14
	bl 0x0200d208
.L_020023c8:
	movs	r0, #14
	bl 0x0200d228
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	bl 0x0200d1c8
	add	sp, #8
	pop	{pc}
	.4byte 0x000017ba
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #432]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r2, r2, r3
	ldr	r0, [r2, #0]
	mov	r8, r2
	bl 0x0200d1e0
	movs	r3, #6
	ldrsh	r5, [r0, r3]
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	bl 0x0200d2f0
	movs	r2, #0
	mov	sl, r2
	mov	r3, sl
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r0, #12
	bl 0x0200d2a8
	ldr	r0, [pc, #384]
	bl 0x0200d280
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	mov	r2, r8
	ldr	r0, [r2, #0]
	movs	r1, #12
	movs	r2, #0
	bl 0x0200d268
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #12
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl 0x0200d250
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d250
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	lsls	r5, r5, #16
.L_0200249a:
	bl 0x0200d298
	lsrs	r5, r5, #16
	mov	r2, r8
	ldr	r0, [r2, #0]
	adds	r1, r5, #0
	movs	r2, #10
	bl 0x0200d2a8
	ldr	r5, [pc, #252]
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_020024ba
	movs	r1, #3
	bl 0x0200d330
.L_020024ba:
	movs	r0, #198
	movs	r1, #0
	bl 0x0200d1d0
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_020024cc
	bl 0x0200d148
.L_020024cc:
	movs	r0, #12
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200d2d8
	movs	r0, #212
	movs	r2, #200
	movs	r3, #1
	movs	r1, #0
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200d2e0
	movs	r0, #12
	bl 0x0200d1e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r5, r0, #0
	adds	r1, #204
	movs	r0, #12
	adds	r2, #102
	ldr	r6, [r5, #80]
	bl 0x0200d1e8
	movs	r1, #212
	movs	r2, #188
	lsls	r2, r2, #1
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200d220
	movs	r0, #12
	movs	r1, #1
	bl 0x0200d2b8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #116]
	adds	r2, #204
	bl 0x0200d1e8
	movs	r0, #12
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d258
	movs	r1, #222
	movs	r2, #188
	lsls	r2, r2, #1
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200d210
	movs	r1, #7
	movs	r0, #12
	bl 0x0200d248
	ldr	r3, [pc, #80]
	ldr	r2, [pc, #60]
	str	r3, [r5, #24]
	adds	r7, r5, #0
	movs	r3, #224
	adds	r7, #85
	lsls	r3, r3, #8
	strb	r2, [r7, #0]
	strh	r3, [r6, #18]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r5, #12]
	movs	r0, #12
	bl 0x0200d1e0
.L_02002582:
	movs	r1, #0
	bl 0x0200d188
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #12
	adds	r1, #102
	adds	r2, #51
	bl 0x0200d1e8
	movs	r1, #246
	movs	r2, #128
	b.n	.L_020025b8
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x000017a1
	.4byte 0x0200e2ac
	.4byte 0x00019999
	.2byte 0x0000
	.2byte 0xffff
.L_020025b8:
	.2byte 0x23bc
	adds	r0, r5, #0
	lsls	r1, r1, #17
	lsls	r2, r2, #12
	lsls	r3, r3, #17
	bl 0x0200d160
	movs	r0, #12
	bl 0x0200d228
	movs	r3, #3
	strb	r3, [r7, #0]
	mov	r3, sl
	str	r3, [r5, #20]
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r2, sl
	str	r3, [r5, #24]
	movs	r1, #1
	strh	r2, [r6, #18]
	movs	r0, #12
	bl 0x0200d248
	movs	r0, #12
	bl 0x0200d1e0
	movs	r1, #1
	bl 0x0200d188
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #116]
	adds	r2, #204
	bl 0x0200d1e8
	movs	r0, #12
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d258
	movs	r1, #252
	movs	r2, #188
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d210
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #12
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1e8
	movs	r1, #248
	movs	r2, #197
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #216
	movs	r2, #197
	lsls	r2, r2, #1
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200d220
	mov	r3, r8
	ldr	r1, [r3, #0]
	movs	r0, #12
	bl 0x0200d380
	movs	r1, #2
	movs	r0, #12
	bl 0x0200d2b8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #74
	bl 0x0200d118
	bl 0x0200d1c8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
.L_0200266c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	ldr	r5, [pc, #32]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d2b8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #40]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #223
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #164
	movs	r1, #1
	movs	r2, #174
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r0, #20
	bl 0x0200d1b8
	ldr	r6, [pc, #176]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r2, #204
	adds	r5, r6, r3
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #168]
	adds	r2, #204
	bl 0x0200d1e8
	movs	r1, #164
	movs	r2, #174
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d220
	movs	r1, #2
	movs	r0, #11
	bl 0x0200d260
	movs	r0, #146
	bl 0x0200d3d8
	movs	r1, #2
	movs	r0, #11
	bl 0x0200d248
	movs	r0, #11
	bl 0x0200d1e0
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r0, #10
	bl 0x0200d1b8
	movs	r0, #146
	bl 0x0200d3d8
	movs	r0, #12
	movs	r1, #2
	bl 0x0200d248
	movs	r1, #2
	movs	r0, #13
	bl 0x0200d248
	movs	r0, #12
	bl 0x0200d1e0
	movs	r5, #192
	lsls	r5, r5, #11
	str	r5, [r0, #40]
	movs	r0, #13
	bl 0x0200d1e0
	movs	r1, #150
	str	r5, [r0, #40]
	lsls	r1, r1, #1
	movs	r0, #12
	movs	r2, #164
	bl 0x0200d208
	movs	r1, #178
	lsls	r1, r1, #1
	movs	r2, #164
	movs	r0, #13
	bl 0x0200d210
	movs	r0, #40
	bl 0x0200d1b8
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r6, r6, r3
	movs	r3, #2
	strb	r3, [r6, #0]
	ldr	r0, [pc, #28]
	movs	r1, #3
	bl 0x0200d310
	movs	r0, #9
	movs	r1, #4
	bl 0x0200d300
	bl 0x0200d1c8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x00019999
	.2byte 0x0010
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #73
	movs	r2, #14
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #76
	movs	r1, #0
	movs	r2, #2
	movs	r3, #2
	bl 0x0200d3d0
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #75
	movs	r2, #16
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #76
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d3d0
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #74
	movs	r2, #18
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #76
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d3d0
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #80
	movs	r2, #18
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #76
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d3d0
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #87
	movs	r2, #15
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #76
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d3d0
	add	sp, #12
	pop	{pc}
	.global Func_02002854
	.thumb_func
Func_02002854:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	ldr	r3, [pc, #116]
	subs	r2, #36
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #112]
	cmp	r2, r3
	bne.n	.L_0200287e
	bl 0x0200a90c
	b.n	.L_020028dc
.L_0200287e:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_0200288a
	bl 0x0200aa5c
	b.n	.L_020028dc
.L_0200288a:
	ldr	r3, [pc, #96]
	cmp	r2, r3
	bne.n	.L_02002896
	bl 0x0200aa84
	b.n	.L_020028dc
.L_02002896:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020028a2
	bl 0x0200ac1c
	b.n	.L_020028dc
.L_020028a2:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_020028ae
	bl 0x0200ba14
	b.n	.L_020028dc
.L_020028ae:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020028ba
	bl 0x0200ba5c
	b.n	.L_020028dc
.L_020028ba:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020028c6
	bl 0x0200ba8c
	b.n	.L_020028dc
.L_020028c6:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020028d2
	bl 0x0200bb2c
	b.n	.L_020028dc
.L_020028d2:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020028dc
	bl 0x0200bc8c
.L_020028dc:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000000e
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x00000011
	.4byte 0x00000012
	.4byte 0x00000013
	.4byte 0x00000014
	.4byte 0x00000015
	.2byte 0x0016
	.2byte 0x0000
	.global Func_02002908
	.thumb_func
Func_02002908:
	movs	r0, #0
	bx	lr
	push	{lr}
	bl 0x0200d398
	movs	r1, #8
	movs	r2, #9
	movs	r0, #0
	bl 0x0200d3a0
	movs	r2, #11
	movs	r1, #10
	movs	r0, #1
	bl 0x0200d3a0
	movs	r0, #8
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #9
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #10
	movs	r1, #4
	bl 0x0200d248
	movs	r1, #4
	movs	r0, #11
	bl 0x0200d248
	ldr	r0, [pc, #72]
	bl 0x0200d3b8
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r0, #245
	bl 0x0200d388
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	bl 0x0200d0d0
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	b.n	.L_02002998
	.4byte 0x00000c08
	.4byte 0x00003f10
	.4byte 0x0200d4dc
	.2byte 0x8c75
	.2byte 0x0200
.L_02002998:
	bl 0x0200d230
	movs	r2, #0
	movs	r1, #0
	movs	r0, #16
	bl 0x0200d230
	movs	r0, #13
	bl 0x02008b78
	movs	r0, #14
	bl 0x02008b78
	movs	r0, #15
	bl 0x02008bac
	movs	r0, #16
	bl 0x02008bac
	ldr	r3, [pc, #124]
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r3, [pc, #120]
	movs	r0, #10
	str	r2, [r3, #0]
	ldr	r3, [pc, #120]
	adds	r0, #255
	strh	r2, [r3, #0]
	ldr	r3, [pc, #116]
	strh	r2, [r3, #0]
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_020029fa
	ldr	r3, [pc, #108]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_020029f2
	.2byte 0xf000
	.2byte 0xfe4a
	b.n	.L_020029fa
.L_020029f2:
	cmp	r3, #6
	bne.n	.L_020029fa
	.2byte 0xf000
	.2byte 0xfef3
.L_020029fa:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02002a14
	movs	r1, #144
	ldr	r0, [pc, #68]
	lsls	r1, r1, #3
	bl 0x0200d0d0
	b.n	.L_02002a2a
.L_02002a14:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02002a2a
	movs	r1, #144
	ldr	r0, [pc, #48]
	lsls	r1, r1, #3
	bl 0x0200d0d0
.L_02002a2a:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200d0d0
	movs	r0, #0
	bl 0x0200d338
	pop	{pc}
	.4byte 0x0200e2a0
	.4byte 0x0200e2a4
	.4byte 0x0200e2a8
	.4byte 0x0200e2aa
	.4byte 0x02000240
	.4byte 0x02009635
	.4byte 0x02009805
	.2byte 0x9b51
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x0200d338
	ldr	r3, [pc, #24]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_02002a7c
	movs	r0, #48
	adds	r0, #255
	bl 0x0200d120
.L_02002a7c:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x0200d1e0
	adds	r6, r0, #0
	movs	r0, #9
	bl 0x0200d2c0
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #9
	movs	r1, #2
	bl 0x0200d248
	movs	r0, #8
	bl 0x0200d1e0
	adds	r6, r0, #0
	adds	r3, r6, #0
	movs	r0, #192
	movs	r5, #0
	adds	r3, #85
	lsls	r0, r0, #2
	strb	r5, [r3, #0]
	adds	r0, #2
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02002af4
	movs	r0, #8
	bl 0x0200d1e0
	movs	r1, #156
	movs	r2, #128
	movs	r3, #236
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	lsls	r3, r3, #17
	bl 0x0200d150
	movs	r3, #19
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #3
	movs	r3, #3
	bl 0x0200d178
	b.n	.L_02002b2c
.L_02002af4:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02002b2c
	movs	r0, #8
	bl 0x0200d1e0
	movs	r1, #172
	movs	r2, #128
	movs	r3, #236
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	lsls	r3, r3, #17
	bl 0x0200d150
	movs	r3, #19
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #3
	movs	r3, #3
	bl 0x0200d178
.L_02002b2c:
	ldr	r0, [pc, #228]
	bl 0x0200d3b8
	movs	r0, #10
	bl 0x0200d1e0
	movs	r3, #179
	adds	r6, r0, #0
	lsls	r3, r3, #8
	adds	r3, #51
	adds	r2, r6, #0
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	adds	r2, #89
	movs	r3, #0
	movs	r0, #0
	strb	r3, [r2, #0]
	bl 0x0200d338
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #45
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02002b92
	movs	r3, #1
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #0
	movs	r0, #41
	movs	r1, #0
	movs	r2, #20
	bl 0x0200d170
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
.L_02002b92:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02002bf6
	ldr	r2, [pc, #120]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #2
	bne.n	.L_02002bc2
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r0, #12]
	b.n	.L_02002bf6
.L_02002bc2:
	cmp	r3, #3
	bne.n	.L_02002bf6
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #45
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02002bf6
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	bl 0x0200d028
.L_02002bf6:
	ldr	r3, [pc, #32]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #2
	bne.n	.L_02002c0e
	movs	r0, #48
	adds	r0, #255
	bl 0x0200d120
.L_02002c0e:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200d4e0
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02002cd2
	movs	r0, #9
	bl 0x0200d1e0
	movs	r1, #216
	movs	r3, #216
	lsls	r3, r3, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl 0x0200d150
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r1, #0
	movs	r2, #0
	movs	r0, #16
	bl 0x0200d230
	movs	r0, #10
	bl 0x0200d1e0
	movs	r1, #188
	movs	r3, #248
	lsls	r3, r3, #16
	lsls	r1, r1, #17
	movs	r2, #0
	bl 0x0200d150
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r1, #0
	movs	r2, #0
	movs	r0, #20
	bl 0x0200d230
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #14
	bl 0x0200d120
	movs	r0, #131
	lsls	r0, r0, #2
	bl 0x0200d120
	movs	r0, #135
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d120
.L_02002cd2:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02002cee
	movs	r1, #216
	movs	r2, #216
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200d230
	b.n	.L_02002d0a
.L_02002cee:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02002d0a
	movs	r1, #188
	movs	r2, #248
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200d230
.L_02002d0a:
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r3, [pc, #48]
	movs	r1, #144
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	bl 0x0200d0d0
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200d0d0
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #32]
	bl 0x0200d0d0
	movs	r0, #9
	b.n	.L_02002d64
	.2byte 0x0000
	.4byte 0x00000c08
	.4byte 0x00003f10
	.4byte 0x0200e298
	.4byte 0x0200e29c
	.4byte 0x02008bf5
	.4byte 0x02008c35
	.2byte 0x9cc5
	.2byte 0x0200
.L_02002d64:
	bl 0x02008b78
	movs	r0, #10
	bl 0x02008b78
	movs	r0, #11
	bl 0x02008bac
	movs	r0, #13
	bl 0x02008bac
	movs	r0, #14
	bl 0x02008bac
	movs	r0, #15
	bl 0x02008bac
	movs	r0, #16
	bl 0x02008bac
	movs	r0, #12
	bl 0x02008bac
	movs	r0, #17
	bl 0x02008bac
	movs	r0, #18
	bl 0x02008bac
	movs	r0, #19
	bl 0x02008bac
	movs	r0, #20
	bl 0x02008bac
	ldr	r0, [pc, #156]
	bl 0x0200d3b8
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02002dda
	ldr	r3, [pc, #140]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_02002dd2
	.2byte 0xf000
	.2byte 0xf94e
	b.n	.L_02002dda
.L_02002dd2:
	cmp	r3, #4
	bne.n	.L_02002dda
	.2byte 0xf000
	.2byte 0xfa77
.L_02002dda:
	ldr	r3, [pc, #116]
	movs	r2, #0
	ldr	r1, [pc, #116]
	str	r2, [r3, #0]
	movs	r3, #250
	lsls	r3, r3, #4
	str	r3, [r1, #0]
	ldr	r3, [pc, #108]
	movs	r1, #144
	strh	r2, [r3, #0]
	ldr	r3, [pc, #108]
	lsls	r1, r1, #3
	strh	r2, [r3, #0]
	ldr	r0, [pc, #104]
	bl 0x0200d0d0
	movs	r1, #144
	ldr	r0, [pc, #100]
	lsls	r1, r1, #3
	bl 0x0200d0d0
	movs	r0, #10
	adds	r0, #255
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02002e24
	ldr	r3, [pc, #56]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_02002e24
	bl 0x0200aec4
.L_02002e24:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #60]
	bl 0x0200d0d0
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl 0x0200d2b8
	movs	r0, #0
	bl 0x0200d338
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200d4e0
	.4byte 0x02000240
	.4byte 0x0200e2a0
	.4byte 0x0200e2a4
	.4byte 0x0200e2a8
	.4byte 0x0200e2aa
	.4byte 0x02008f61
	.4byte 0x020092c5
	.2byte 0x99d5
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
	bl 0x0200d0b8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002e9c
	adds	r3, #15
.L_02002e9c:
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
	bl 0x0200d1e0
	adds	r7, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2e0
	bl 0x0200d158
	movs	r0, #1
	bl 0x0200d0c8
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
	bl 0x0200d318
	bl 0x0200d328
	movs	r0, #204
	bl 0x0200d3d8
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200d1b8
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #272]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_02002f5e:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200d0f0
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200d0e8
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200d0e0
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #204]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200d0e0
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
	bl 0x020080b8
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02002f5e
	movs	r0, #188
	bl 0x0200d3d8
	ldr	r5, [pc, #128]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200d2d0
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200d248
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200d198
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200d198
	bl 0x0200d1a0
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d2d0
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200d1b8
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200d248
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200d1c8
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200ae6d
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #576]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
.L_0200307c:
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	adds	r6, r0, #0
	movs	r0, #9
	bl 0x0200d1e0
	mov	r8, r0
	bl 0x0200d2f0
	adds	r7, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2e0
	bl 0x0200d2f0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	adds	r0, #85
	strb	r3, [r0, #0]
	strb	r3, [r2, #0]
	movs	r0, #1
	bl 0x0200d0c8
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200d118
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d120
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200d118
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200d120
	movs	r0, #162
	bl 0x0200d3d8
	bl 0x02008d64
	movs	r2, #0
	mov	sl, r2
.L_020030f6:
	ldr	r3, [r6, #12]
	movs	r2, #144
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r6, #12]
	mov	r1, r8
	ldr	r3, [r1, #12]
	movs	r5, #128
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r3, [r7, #12]
	movs	r2, #230
	lsls	r2, r2, #9
	adds	r2, #204
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #128
	str	r3, [r7, #12]
	movs	r0, #11
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #8
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #0
	adds	r2, r5, #0
	movs	r0, #16
	bl 0x02008cd8
	mov	r1, sl
	movs	r2, #128
	lsls	r3, r1, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	movs	r2, #240
	asrs	r1, r3, #16
	lsls	r2, r2, #12
	mov	sl, r1
	cmp	r3, r2
	bls.n	.L_020030f6
	bl 0x0200d158
	movs	r0, #1
	bl 0x0200d0c8
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
	bl 0x0200d318
	bl 0x0200d328
	movs	r0, #10
	bl 0x0200d1b8
.L_02003194:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #292]
	mov	r1, r8
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r5, #192
	ldr	r3, [r1, #12]
	lsls	r5, r5, #5
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #276]
	movs	r1, #192
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r0, #11
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #144
	movs	r0, #13
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	movs	r0, #14
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	movs	r0, #15
	lsls	r1, r1, #8
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #0
	adds	r2, r5, #0
	movs	r0, #16
	bl 0x02008cf8
	movs	r0, #1
	bl 0x0200d0c8
	movs	r0, #16
	bl 0x0200d1e0
	ldr	r3, [r0, #28]
	cmp	r3, r5
	bgt.n	.L_02003194
	ldr	r2, [pc, #196]
	movs	r3, #0
	movs	r0, #131
	str	r3, [r2, #0]
	lsls	r0, r0, #2
	bl 0x0200d120
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d3d8
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r2, #0
	movs	r0, #16
	movs	r1, #0
	bl 0x0200d230
	movs	r1, #1
	movs	r0, #9
	bl 0x0200d248
	movs	r0, #9
	bl 0x0200d1e0
	movs	r1, #216
	movs	r3, #216
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #16
	bl 0x0200d150
	mov	r1, r8
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #9
	movs	r1, #3
	bl 0x0200d2b8
	movs	r2, #0
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	bl 0x0200d150
	ldr	r3, [pc, #52]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	movs	r1, #1
	bl 0x0200d188
	movs	r0, #1
	bl 0x0200d0c8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200d1c8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff94000
	.4byte 0xfffd4ccd
	.2byte 0xe298
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #576]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200d1e0
	mov	r8, r0
	bl 0x0200d2f0
	adds	r7, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2e0
	bl 0x0200d2f0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	adds	r0, #85
	strb	r3, [r0, #0]
	strb	r3, [r2, #0]
	movs	r0, #1
	bl 0x0200d0c8
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x0200d120
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200d118
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200d120
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200d118
	movs	r0, #162
	bl 0x0200d3d8
	bl 0x02008e60
	movs	r2, #0
	mov	sl, r2
.L_02003352:
	ldr	r3, [r6, #12]
	movs	r2, #144
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r6, #12]
	mov	r1, r8
	ldr	r3, [r1, #12]
	movs	r5, #128
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r3, [r7, #12]
	movs	r2, #230
	lsls	r2, r2, #9
	adds	r2, #204
	adds	r3, r3, r2
	lsls	r5, r5, #5
	movs	r1, #128
	str	r3, [r7, #12]
	movs	r0, #12
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #128
	movs	r0, #19
	lsls	r1, r1, #8
	adds	r2, r5, #0
	bl 0x02008cd8
	movs	r1, #0
	adds	r2, r5, #0
	movs	r0, #20
	bl 0x02008cd8
	mov	r1, sl
	movs	r2, #128
	lsls	r3, r1, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	movs	r2, #240
	asrs	r1, r3, #16
	lsls	r2, r2, #12
	mov	sl, r1
	cmp	r3, r2
	bls.n	.L_02003352
	bl 0x0200d158
	movs	r0, #1
	bl 0x0200d0c8
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
	bl 0x0200d318
	bl 0x0200d328
	movs	r0, #10
	bl 0x0200d1b8
.L_020033f0:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #292]
	mov	r1, r8
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r5, #192
	ldr	r3, [r1, #12]
	lsls	r5, r5, #5
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #276]
	movs	r1, #192
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r0, #12
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #144
	movs	r0, #17
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	movs	r0, #18
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #192
	movs	r0, #19
	lsls	r1, r1, #8
	adds	r2, r5, #0
	bl 0x02008cf8
	movs	r1, #0
	adds	r2, r5, #0
	movs	r0, #20
	bl 0x02008cf8
	movs	r0, #1
	bl 0x0200d0c8
	movs	r0, #20
	bl 0x0200d1e0
	ldr	r3, [r0, #28]
	cmp	r3, r5
	bgt.n	.L_020033f0
	ldr	r2, [pc, #196]
	movs	r0, #135
	movs	r3, #0
	lsls	r0, r0, #1
	str	r3, [r2, #0]
	adds	r0, #255
	bl 0x0200d120
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200d3d8
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r2, #0
	movs	r0, #20
	movs	r1, #0
	bl 0x0200d230
	movs	r1, #1
	movs	r0, #10
	bl 0x0200d248
	movs	r0, #10
	bl 0x0200d1e0
	movs	r1, #188
	movs	r3, #248
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #16
	bl 0x0200d150
	mov	r1, r8
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #10
	movs	r1, #3
	bl 0x0200d2b8
	movs	r2, #0
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	bl 0x0200d150
	ldr	r3, [pc, #52]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	movs	r1, #1
	bl 0x0200d188
	movs	r0, #1
	bl 0x0200d0c8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200d1c8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfff94000
	.4byte 0xfffd4ccd
	.2byte 0xe29c
	.2byte 0x0200
	push	{lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200d1e0
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200d320
	bl 0x0200d328
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200b524
	movs	r0, #3
	bl 0x0200d2f8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200b524
	movs	r0, #4
	bl 0x0200d2f8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #240]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r5, r0
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	adds	r7, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	adds	r3, r7, #0
	adds	r3, #85
	movs	r6, #0
	strb	r6, [r3, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200d2b8
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	strh	r6, [r7, #6]
	movs	r1, #16
	ldr	r0, [r5, #0]
	bl 0x0200d248
	movs	r5, #0
.L_020035c8:
	ldr	r3, [r7, #16]
	movs	r0, #192
	lsls	r0, r0, #10
	adds	r3, r3, r0
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	bl 0x0200d150
	adds	r5, #1
	movs	r0, #1
	bl 0x0200d0c8
	cmp	r5, #7
	bls.n	.L_020035c8
	movs	r0, #20
	bl 0x0200d1b8
	movs	r3, #128
	lsls	r3, r3, #7
	ldr	r5, [pc, #132]
	strh	r3, [r7, #6]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d2d0
	ldr	r0, [r5, #0]
	movs	r1, #27
	bl 0x0200d248
	movs	r0, #30
	bl 0x0200d1b8
	ldr	r0, [r5, #0]
	movs	r1, #28
	bl 0x0200d248
	movs	r0, #10
	bl 0x0200d1b8
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200d2b8
	movs	r5, #0
.L_0200362a:
	ldr	r3, [r7, #24]
	ldr	r2, [pc, #76]
	ldr	r0, [pc, #80]
	adds	r3, r3, r2
	str	r3, [r7, #24]
	ldr	r3, [r7, #28]
	adds	r3, r3, r2
	str	r3, [r7, #28]
	ldr	r3, [r7, #12]
	adds	r3, r3, r0
	str	r3, [r7, #12]
	movs	r0, #1
	bl 0x0200d0c8
	cmp	r5, #10
	bne.n	.L_02003650
	movs	r0, #204
	bl 0x0200d3d8
.L_02003650:
	cmp	r5, #20
	bne.n	.L_02003668
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200d320
.L_02003668:
	adds	r5, #1
	cmp	r5, #39
	bls.n	.L_0200362a
	movs	r0, #5
	bl 0x0200d2f8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfffffc00
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #328]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	ldr	r0, [r5, #0]
	sub	sp, #8
	bl 0x0200d1e0
	mov	r8, r0
	movs	r0, #13
	bl 0x0200d1e0
	adds	r7, r0, #0
	movs	r0, #15
	bl 0x0200d1e0
	adds	r6, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #162
	bl 0x0200d388
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2e0
	movs	r0, #1
	bl 0x0200d0c8
	mov	r2, r8
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #220
	movs	r3, #200
	ldr	r2, [pc, #240]
	lsls	r3, r3, #16
	lsls	r1, r1, #17
	bl 0x0200d150
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	movs	r0, #13
	bl 0x0200d1e0
	movs	r1, #220
	movs	r3, #204
	lsls	r1, r1, #17
	ldr	r2, [pc, #204]
	lsls	r3, r3, #16
	bl 0x0200d150
	movs	r0, #15
	bl 0x0200d1e0
	movs	r1, #220
	movs	r3, #200
	ldr	r2, [pc, #188]
	lsls	r3, r3, #16
	lsls	r1, r1, #17
	bl 0x0200d150
	movs	r0, #13
	movs	r1, #2
	bl 0x0200d2b8
	movs	r0, #15
	movs	r1, #3
	bl 0x0200d2b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r6, #28]
	movs	r0, #13
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #15
	movs	r1, #2
	bl 0x0200d248
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200d318
	bl 0x0200d328
.L_0200376e:
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r7, #12]
	mov	r1, r8
	ldr	r3, [r1, #12]
	movs	r0, #15
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [r1, #12]
	adds	r1, r2, #0
	bl 0x02008cd8
	movs	r0, #1
	bl 0x0200d0c8
	ldr	r3, [r7, #12]
	cmp	r3, #0
	blt.n	.L_0200376e
	movs	r3, #27
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d178
	ldr	r2, [pc, #44]
	movs	r3, #250
	lsls	r3, r3, #3
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200d1c8
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffec0000
	.2byte 0xe2a0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #328]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	ldr	r0, [r5, #0]
	sub	sp, #8
	bl 0x0200d1e0
	mov	r8, r0
	movs	r0, #14
	bl 0x0200d1e0
	adds	r7, r0, #0
	movs	r0, #16
	bl 0x0200d1e0
	adds	r6, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #162
	bl 0x0200d388
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2e0
	movs	r0, #1
	bl 0x0200d0c8
	mov	r2, r8
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #150
	movs	r3, #232
	ldr	r2, [pc, #240]
	lsls	r3, r3, #16
	lsls	r1, r1, #18
	bl 0x0200d150
	ldr	r0, [r5, #0]
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	movs	r0, #14
	bl 0x0200d1e0
	movs	r1, #150
	movs	r3, #236
	lsls	r1, r1, #18
	ldr	r2, [pc, #204]
	lsls	r3, r3, #16
	bl 0x0200d150
	movs	r0, #16
	bl 0x0200d1e0
	movs	r1, #150
	movs	r3, #232
	ldr	r2, [pc, #188]
	lsls	r3, r3, #16
	lsls	r1, r1, #18
	bl 0x0200d150
	movs	r0, #14
	movs	r1, #2
	bl 0x0200d2b8
	movs	r0, #16
	movs	r1, #3
	bl 0x0200d2b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r6, #28]
	movs	r0, #14
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #16
	movs	r1, #2
	bl 0x0200d248
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200d318
	bl 0x0200d328
.L_020038ca:
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r7, #12]
	mov	r1, r8
	ldr	r3, [r1, #12]
	movs	r0, #16
	adds	r3, r3, r2
	movs	r2, #152
.L_020038de:
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [r1, #12]
.L_020038e4:
	adds	r1, r2, #0
	bl 0x02008cd8
	movs	r0, #1
	bl 0x0200d0c8
	ldr	r3, [r7, #12]
	cmp	r3, #0
	blt.n	.L_020038ca
	movs	r3, #37
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d178
	ldr	r2, [pc, #44]
	movs	r3, #250
	lsls	r3, r3, #3
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
.L_0200391e:
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200d1c8
	add	sp, #8
	pop	{r3}
.L_0200392a:
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffec0000
	.2byte 0xe2a4
	.2byte 0x0200
.L_0200393c:
	push	{r5, lr}
	movs	r0, #13
	bl 0x0200d1e0
	adds	r5, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
.L_02003950:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #76]
.L_02003954:
	movs	r0, #15
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [r5, #12]
	adds	r1, r2, #0
	bl 0x02008cf8
	movs	r0, #1
	bl 0x0200d0c8
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #52]
	cmp	r3, r2
	bgt.n	.L_02003950
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200d320
	bl 0x0200d328
	movs	r0, #6
	bl 0x0200d2f8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0xfffc0000
	.2byte 0x0000
	.2byte 0xffec
	.2byte 0xb520
	movs	r0, #14
	bl 0x0200d1e0
	adds	r5, r0, #0
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
.L_020039bc:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #76]
	movs	r0, #16
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [r5, #12]
	adds	r1, r2, #0
	bl 0x02008cf8
	movs	r0, #1
	bl 0x0200d0c8
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #52]
	cmp	r3, r2
	bgt.n	.L_020039bc
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200d320
	bl 0x0200d328
	movs	r0, #7
	bl 0x0200d2f8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0xfffc0000
	.2byte 0x0000
	.2byte 0xffec
	.2byte 0xb500
	ldr	r2, [pc, #60]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	movs	r3, #0
	str	r3, [r2, #0]
	subs	r3, #13
	ldrb	r2, [r1, #23]
	movs	r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #23]
	bl 0x0200d338
	ldr	r3, [pc, #32]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #1
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02003a52
	movs	r0, #48
	adds	r0, #255
	bl 0x0200d120
.L_02003a52:
	pop	{pc}
	.4byte 0x0200e294
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	ldr	r2, [pc, #36]
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r0, #170
	bl 0x0200d388
	ldrb	r2, [r5, #23]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #23]
	movs	r0, #0
	bl 0x0200d338
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xe294
	.2byte 0x0200
	push	{lr}
	bl 0x0200d390
	movs	r1, #136
	lsls	r1, r1, #1
	movs	r0, #0
	adds	r1, #255
	movs	r2, #8
	movs	r3, #9
	bl 0x0200d3a8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02003abc
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	b.n	.L_02003ad8
.L_02003abc:
	movs	r0, #10
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	movs	r0, #10
	movs	r1, #6
	bl 0x0200d248
	ldr	r1, [pc, #12]
	movs	r0, #10
	bl 0x0200d1f0
.L_02003ad8:
	movs	r0, #0
	bl 0x0200d338
	pop	{pc}
	.2byte 0xd550
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r0
	adds	r6, r1, #0
	bl 0x0200d1e0
	movs	r1, #5
	adds	r5, r0, #0
	mov	r0, r8
	bl 0x0200d248
	bl 0x0200d0e0
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	adds	r6, r6, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	adds	r5, #102
	mov	r3, r8
	strh	r3, [r5, #0]
	ldr	r1, [pc, #12]
	mov	r0, r8
	bl 0x0200d1f0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0xd548
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #128
	adds	r3, r3, r2
	lsls	r0, r0, #4
	subs	r2, #172
	str	r2, [r3, #0]
	adds	r0, #66
	sub	sp, #8
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02003b6e
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r3, #5
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200d178
	b.n	.L_02003b74
.L_02003b6e:
	movs	r0, #8
	bl 0x0200d2c0
.L_02003b74:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
.L_02003b80:
	beq.n	.L_02003be0
	movs	r3, #17
	movs	r2, #24
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #3
	movs	r0, #2
	movs	r1, #0
	movs	r2, #2
	bl 0x0200d178
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r1, #187
	movs	r2, #141
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x0200d238
	movs	r3, #208
	movs	r1, #136
	movs	r2, #148
	lsls	r3, r3, #8
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d238
	movs	r3, #192
	movs	r1, #216
	movs	r2, #132
	lsls	r3, r3, #6
	movs	r0, #13
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200d238
	b.n	.L_02003bfa
.L_02003be0:
	movs	r0, #11
	movs	r1, #0
	bl 0x0200bae4
	movs	r0, #12
	movs	r1, #240
	bl 0x0200bae4
	movs	r1, #240
	lsls	r1, r1, #1
	movs	r0, #13
	bl 0x0200bae4
.L_02003bfa:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02003c2e
.L_02003c08:
	movs	r3, #2
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #77
	movs	r1, #33
	movs	r2, #77
	movs	r3, #24
	bl 0x0200d170
	movs	r3, #17
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r1, #30
	movs	r2, #2
	movs	r3, #1
	bl 0x0200d178
.L_02003c2e:
	ldr	r3, [pc, #32]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #3
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02003c4c
	movs	r0, #48
	adds	r0, #255
	bl 0x0200d120
.L_02003c4c:
	add	sp, #8
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02003c78
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x0200d118
	movs	r1, #132
	movs	r0, #1
	lsls	r1, r1, #2
	movs	r2, #10
	movs	r3, #11
	bl 0x0200d3a8
.L_02003c78:
	movs	r3, #192
	movs	r1, #196
	movs	r2, #180
	lsls	r3, r3, #6
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d238
	pop	{pc}
	push	{lr}
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
	bl 0x0200d390
	movs	r1, #136
	lsls	r1, r1, #1
	movs	r0, #0
	adds	r1, #255
	movs	r2, #8
	movs	r3, #9
	bl 0x0200d3a8
	movs	r1, #132
	movs	r0, #1
	lsls	r1, r1, #2
	movs	r2, #10
	movs	r3, #11
	bl 0x0200d3a8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02003ce2
	movs	r0, #13
	movs	r1, #5
	bl 0x0200d248
	movs	r0, #15
	movs	r1, #5
	bl 0x0200d248
.L_02003ce2:
	movs	r0, #14
	movs	r1, #5
	bl 0x0200d248
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #316]
	bl 0x0200d0d0
	ldr	r3, [pc, #312]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	movs	r1, #128
	subs	r3, r2, #4
	lsls	r3, r3, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	bls.n	.L_02003d14
	lsls	r3, r2, #16
	movs	r2, #224
	lsls	r2, r2, #12
	cmp	r3, r2
	bne.n	.L_02003d4c
.L_02003d14:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #74
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02003d36
	movs	r3, #160
	movs	r1, #216
	movs	r2, #197
	lsls	r3, r3, #7
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d238
	b.n	.L_02003d4c
.L_02003d36:
	bl 0x0200cfb4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02003d4c
	bl 0x0200bc54
.L_02003d4c:
	ldr	r3, [pc, #224]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r2, [r3, #0]
	movs	r1, #128
	subs	r3, r2, #1
	lsls	r3, r3, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	bls.n	.L_02003d6a
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, #9
	bne.n	.L_02003dd2
.L_02003d6a:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02003d7c
	bl 0x02009f3c
.L_02003d7c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02003da8
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r3, #176
	movs	r1, #204
	movs	r2, #166
	lsls	r3, r3, #8
	movs	r0, #15
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200d238
	b.n	.L_02003e0a
.L_02003da8:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #72
	bl 0x0200d110
	cmp	r0, #0
	beq.n	.L_02003dcc
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	b.n	.L_02003e0a
.L_02003dcc:
	bl 0x0200be8c
	b.n	.L_02003e0a
.L_02003dd2:
	cmp	r3, #6
	bne.n	.L_02003dea
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02003e0a
	bl 0x0200c4d4
	b.n	.L_02003e0a
.L_02003dea:
	cmp	r3, #13
	bne.n	.L_02003df4
	bl 0x0200c6d8
	b.n	.L_02003e0a
.L_02003df4:
	cmp	r3, #14
	bne.n	.L_02003e0a
	movs	r0, #128
.L_02003dfa:
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d110
	cmp	r0, #0
	bne.n	.L_02003e0a
.L_02003e06:
	bl 0x0200c82c
.L_02003e0a:
	ldr	r3, [pc, #36]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
.L_02003e12:
	ldrh	r3, [r3, #0]
	movs	r1, #128
	subs	r3, #8
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	cmp	r3, r1
.L_02003e1e:
	bhi.n	.L_02003e28
	movs	r0, #48
	adds	r0, #255
	bl 0x0200d120
.L_02003e28:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02009c8d
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	movs	r5, #0
	adds	r7, r0, #0
	mov	r8, r2
	cmp	r5, r6
	bcs.n	.L_02003e58
.L_02003e46:
	ldrh	r3, [r7, #18]
	movs	r0, #1
	add	r3, r8
	strh	r3, [r7, #18]
	adds	r5, #1
	bl 0x0200d1b8
.L_02003e54:
	cmp	r5, r6
	bcc.n	.L_02003e46
.L_02003e58:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	movs	r5, #0
	adds	r7, r0, #0
	mov	r8, r2
	cmp	r5, r6
	bcs.n	.L_02003e84
.L_02003e72:
	ldr	r3, [r7, #8]
	movs	r0, #1
	add	r3, r8
	str	r3, [r7, #8]
	adds	r5, #1
	bl 0x0200d1b8
	cmp	r5, r6
	bcc.n	.L_02003e72
.L_02003e84:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2e0
	bl 0x0200d2f0
	movs	r2, #0
	mov	sl, r2
	mov	r3, sl
	adds	r0, #85
	strb	r3, [r0, #0]
.L_02003ebe:
	movs	r0, #1
	bl 0x0200d0c8
	ldr	r5, [pc, #928]
	movs	r2, #133
	lsls	r2, r2, #2
.L_02003eca:
	adds	r5, r5, r2
	movs	r1, #200
	movs	r2, #199
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	ldr	r0, [r5, #0]
.L_02003ed6:
	bl 0x0200d230
	movs	r0, #1
	bl 0x0200d0c8
.L_02003ee0:
	bl 0x0200d318
	movs	r1, #200
	movs	r2, #188
	ldr	r0, [r5, #0]
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	bl 0x0200d220
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #884]
	adds	r1, #153
	bl 0x0200d2d8
	movs	r0, #218
	movs	r1, #144
	movs	r2, #166
	movs	r3, #1
	lsls	r0, r0, #17
.L_02003f08:
	lsls	r1, r1, #15
	lsls	r2, r2, #18
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r2, #0
	movs	r1, #4
	movs	r0, #12
	bl 0x0200d258
	ldr	r0, [pc, #848]
	bl 0x0200d280
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #228
	movs	r2, #172
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #17
	movs	r1, #0
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r1, #2
	movs	r0, #13
	bl 0x0200d260
	movs	r0, #131
	bl 0x0200d3d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #13
	bl 0x0200d270
	movs	r0, #20
	bl 0x0200d1b8
	movs	r1, #0
	movs	r0, #13
	bl 0x0200d270
	movs	r0, #20
	bl 0x0200d1b8
	movs	r0, #13
	bl 0x0200d1e0
	movs	r3, #85
	adds	r7, r0, #0
	adds	r3, r3, r7
	mov	r2, sl
	ldr	r6, [r7, #80]
	strb	r2, [r3, #0]
	mov	r8, r3
	movs	r5, #0
.L_02003f82:
	ldr	r3, [r7, #12]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #204
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d1b8
	cmp	r5, #63
	bls.n	.L_02003f82
	movs	r2, #32
	movs	r1, #32
	adds	r0, r6, #0
	bl 0x0200be34
	movs	r0, #20
	bl 0x0200d1b8
	movs	r1, #129
	movs	r0, #13
	lsls	r1, r1, #1
	bl 0x0200d2d0
	movs	r2, #64
	negs	r2, r2
	adds	r0, r6, #0
	movs	r1, #32
	bl 0x0200be34
	adds	r0, r6, #0
	movs	r1, #32
	movs	r2, #64
	bl 0x0200be34
	movs	r2, #32
	adds	r0, r6, #0
	movs	r1, #64
	bl 0x0200be34
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #13
	bl 0x0200d2d0
	movs	r0, #60
	bl 0x0200d1b8
	mov	r2, r8
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #5
	bl 0x0200d1b8
	movs	r0, #159
	bl 0x0200d3d8
	movs	r3, #192
	lsls	r3, r3, #10
	movs	r5, #0
	str	r3, [r7, #40]
	movs	r0, #60
	strh	r5, [r6, #18]
	mov	r8, r3
	bl 0x0200d1b8
	movs	r0, #13
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #218
	movs	r1, #144
	movs	r2, #166
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #15
	lsls	r2, r2, #18
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #12
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1e8
	movs	r1, #238
	movs	r2, #166
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r1, #192
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #6
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #4
.L_02004056:
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #12
	movs	r1, #0
.L_02004062:
	bl 0x0200d298
	movs	r1, #237
	movs	r2, #166
	movs	r0, #12
	lsls	r1, r1, #1
.L_0200406e:
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r1, #236
	movs	r2, #162
	movs	r0, #12
.L_0200407a:
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r2, #0
.L_02004084:
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d230
	movs	r1, #152
	lsls	r1, r1, #6
	ldr	r0, [pc, #480]
	adds	r1, #102
	bl 0x0200d2d8
	movs	r0, #182
	movs	r1, #144
	movs	r2, #166
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #15
	lsls	r2, r2, #18
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r1, #164
	movs	r2, #162
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	movs	r0, #12
	bl 0x0200d230
	movs	r0, #1
	bl 0x0200d0c8
	movs	r1, #163
	movs	r2, #166
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r1, #160
	movs	r2, #166
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r1, #160
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #7
	bl 0x0200d2a8
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #368]
	adds	r1, #153
	bl 0x0200d2d8
	movs	r0, #171
	movs	r2, #172
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #17
	movs	r1, #0
.L_0200410e:
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r1, #2
	movs	r0, #14
.L_0200411a:
	bl 0x0200d260
	movs	r0, #131
	bl 0x0200d3d8
	movs	r1, #128
.L_02004126:
	lsls	r1, r1, #1
	movs	r0, #14
	bl 0x0200d270
	movs	r0, #20
	bl 0x0200d1b8
	movs	r1, #0
	movs	r0, #14
	bl 0x0200d270
.L_0200413c:
	movs	r0, #20
	bl 0x0200d1b8
	movs	r1, #2
	movs	r0, #14
	adds	r1, #255
	bl 0x0200d2d0
	movs	r0, #14
	bl 0x0200d1e0
	movs	r2, #85
	adds	r7, r0, #0
	adds	r2, r2, r7
	mov	r3, r8
	strb	r3, [r2, #0]
	mov	sl, r2
.L_0200415e:
	ldr	r3, [r7, #12]
	movs	r2, #152
	lsls	r2, r2, #6
	adds	r2, #102
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d1b8
	cmp	r5, #63
	bls.n	.L_0200415e
	movs	r3, #128
	lsls	r3, r3, #5
	ldr	r5, [pc, #252]
	mov	r8, r3
	adds	r0, r7, #0
	movs	r1, #64
	mov	r2, r8
	bl 0x0200be60
	adds	r0, r7, #0
	adds	r2, r5, #0
	movs	r1, #128
	bl 0x0200be60
	adds	r0, r7, #0
	movs	r1, #128
	mov	r2, r8
	bl 0x0200be60
	adds	r2, r5, #0
	adds	r0, r7, #0
	movs	r1, #64
	bl 0x0200be60
	movs	r0, #20
	bl 0x0200d1b8
.L_020041ac:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #14
	bl 0x0200d2d0
	mov	r2, sl
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #5
	bl 0x0200d1b8
	movs	r0, #159
	bl 0x0200d3d8
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r7, #40]
	movs	r0, #60
	bl 0x0200d1b8
	movs	r0, #14
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #182
	movs	r1, #144
	movs	r2, #166
	movs	r3, #1
	lsls	r2, r2, #18
	lsls	r0, r0, #17
	lsls	r1, r1, #15
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #192
	movs	r2, #10
	movs	r0, #12
	lsls	r1, r1, #6
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #200
	movs	r2, #172
	movs	r3, #1
	lsls	r2, r2, #18
	lsls	r0, r0, #17
	movs	r1, #0
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r1, #2
	movs	r0, #15
	bl 0x0200d260
	movs	r0, #131
	bl 0x0200d3d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200d270
	movs	r0, #20
	bl 0x0200d1b8
	movs	r1, #0
	movs	r0, #15
	bl 0x0200d270
	movs	r0, #20
	bl 0x0200d1b8
	movs	r0, #15
	bl 0x0200d1e0
	adds	r7, r0, #0
	adds	r3, r7, #0
	movs	r5, #0
	adds	r3, #85
	ldr	r6, [r7, #80]
	strb	r5, [r3, #0]
	b.n	.L_0200427c
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0004cccc
	.4byte 0x00001765
	.4byte 0x00013333
	.2byte 0xf000
	.2byte 0xffff
.L_0200427c:
	ldr	r3, [r7, #12]
	movs	r2, #204
	lsls	r2, r2, #6
	adds	r2, #51
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d1b8
	cmp	r5, #63
	bls.n	.L_0200427c
	movs	r2, #64
	adds	r0, r6, #0
	movs	r1, #32
	movs	r5, #0
	bl 0x0200be34
	movs	r1, #1
	movs	r0, #15
	strh	r5, [r6, #18]
	bl 0x0200d260
	movs	r0, #60
	bl 0x0200d1b8
	movs	r2, #64
	negs	r2, r2
	adds	r0, r6, #0
	movs	r1, #32
	bl 0x0200be34
	movs	r0, #15
	strh	r5, [r6, #18]
	movs	r1, #1
	bl 0x0200d260
	movs	r0, #60
	bl 0x0200d1b8
.L_020042cc:
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #508]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	adds	r5, #1
	bl 0x0200d1b8
	cmp	r5, #63
	bls.n	.L_020042cc
	movs	r0, #20
	bl 0x0200d1b8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #15
	bl 0x0200d2c8
	movs	r2, #20
	movs	r0, #15
	movs	r1, #0
	bl 0x0200d290
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #15
	bl 0x0200d2c8
	movs	r0, #15
	movs	r1, #1
	bl 0x0200d248
	movs	r0, #15
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d258
	movs	r1, #176
	movs	r2, #20
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200d2a8
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #15
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #182
	movs	r1, #144
	movs	r2, #166
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #15
	lsls	r2, r2, #18
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r1, #129
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x0200d2c8
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #15
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #176
	movs	r2, #40
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #15
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #6
	movs	r2, #80
	adds	r1, #255
	movs	r0, #12
	bl 0x0200d2c8
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #192
	movs	r2, #20
	movs	r0, #12
	lsls	r1, r1, #6
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #163
	movs	r2, #166
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r1, #164
	movs	r2, #162
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x0200d220
	movs	r0, #12
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	movs	r0, #12
	bl 0x0200d1e0
	movs	r1, #15
	bl 0x0200d278
	movs	r1, #220
	movs	r2, #150
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200d230
	movs	r0, #200
	movs	r2, #172
	movs	r3, #1
	lsls	r0, r0, #17
	movs	r1, #0
	lsls	r2, r2, #18
	bl 0x0200d2e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #15
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1e8
	movs	r1, #200
	movs	r2, #161
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200d220
	movs	r0, #20
	bl 0x0200d1b8
	movs	r0, #188
	bl 0x0200d3d8
	bl 0x02009f3c
	movs	r1, #0
	movs	r0, #12
	bl 0x0200d298
	ldr	r5, [pc, #112]
	adds	r0, r5, #0
	bl 0x0200d0d8
	movs	r0, #15
	movs	r1, #3
	bl 0x0200d250
	movs	r0, #15
	movs	r1, #3
	bl 0x0200d2b8
	movs	r1, #200
	movs	r2, #160
	movs	r0, #15
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #200
	movs	r2, #188
	lsls	r2, r2, #18
	movs	r3, #1
	lsls	r0, r0, #17
	movs	r1, #0
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200d0d0
	bl 0x0200d1c8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #72
	bl 0x0200d118
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xffffcccd
	.2byte 0x9c8d
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r1, r1
	negs	r2, r2
	negs	r0, r0
	bl 0x0200d2e0
	movs	r0, #1
	bl 0x0200d0c8
	ldr	r5, [pc, #464]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #164
	movs	r2, #180
	ldr	r0, [r5, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200d230
	movs	r3, #160
	movs	r1, #236
	movs	r2, #204
	lsls	r3, r3, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #12
	bl 0x0200d238
	movs	r0, #16
	bl 0x0200d1e0
	movs	r6, #192
	lsls	r6, r6, #8
	strh	r6, [r0, #6]
	movs	r0, #1
	bl 0x0200d0c8
	bl 0x0200d318
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1e8
	movs	r1, #164
	movs	r2, #196
	ldr	r0, [r5, #0]
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	bl 0x0200d220
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200d2d8
	movs	r0, #204
	movs	r1, #1
	movs	r2, #200
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200d2e0
	movs	r1, #188
	movs	r2, #204
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200d220
	movs	r0, #20
	bl 0x0200d1b8
	movs	r1, #1
	movs	r0, #12
	bl 0x0200d260
	ldr	r0, [pc, #312]
	bl 0x0200d280
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d2a8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #12
	bl 0x0200d2c8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
.L_020045c0:
	ldr	r1, [pc, #272]
	adds	r2, #204
	bl 0x0200d1e8
	movs	r1, #228
	movs	r2, #204
	lsls	r2, r2, #1
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200d220
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	ldr	r1, [r5, #0]
	movs	r0, #16
	bl 0x0200d240
	movs	r0, #1
	bl 0x0200d0c8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #16
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1e8
	movs	r1, #188
	movs	r2, #196
	movs	r0, #16
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	ldr	r0, [r5, #0]
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200d2a8
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r2, #20
	movs	r0, #16
	movs	r1, #0
	bl 0x0200d2a8
	movs	r1, #129
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200d2d0
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #10
	adds	r1, #255
	movs	r2, #0
	movs	r0, #16
	bl 0x0200d2c8
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #0
.L_0200465c:
	bl 0x0200d2a8
	movs	r2, #20
	ldr	r0, [r5, #0]
	adds	r1, r6, #0
	bl 0x0200d2a8
	movs	r0, #16
	movs	r1, #0
	bl 0x0200d298
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200d250
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #234
	movs	r2, #214
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #236
	movs	r2, #227
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #86
	str	r2, [r3, #0]
	bl 0x0200d320
	bl 0x0200d328
	movs	r0, #13
	bl 0x0200d2f8
.L_020046c8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000177c
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	ldr	r3, [pc, #252]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d230
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2e0
	bl 0x0200d2f0
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #1
	bl 0x0200d0c8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #86
	str	r2, [r3, #0]
	bl 0x0200d318
	bl 0x0200d328
	movs	r1, #236
	movs	r2, #162
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	movs	r0, #12
	bl 0x0200d230
	movs	r0, #1
	bl 0x0200d0c8
	movs	r1, #236
	movs	r2, #166
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	movs	r1, #192
	movs	r0, #12
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200d2a8
	movs	r1, #160
	movs	r0, #12
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200d2a8
	movs	r1, #192
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #6
	bl 0x0200d2a8
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #88]
	adds	r1, #153
	bl 0x0200d2d8
	movs	r0, #228
	movs	r1, #160
	movs	r2, #166
	movs	r3, #1
	lsls	r2, r2, #18
	lsls	r1, r1, #14
	lsls	r0, r0, #17
	bl 0x0200d2e0
	bl 0x0200d2e8
	movs	r0, #40
	bl 0x0200d1b8
	ldr	r0, [pc, #56]
	bl 0x0200d280
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #236
	movs	r2, #162
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200d220
	bl 0x0200d320
	bl 0x0200d328
	movs	r0, #14
	bl 0x0200d2f8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0004cccc
	.2byte 0x1780
	.2byte 0x0000
	push	{lr}
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #44]
	adds	r2, #204
	bl 0x0200d1e8
	movs	r1, #248
	movs	r2, #196
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d2a8
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	pop	{pc}
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200d2e0
	bl 0x0200d2f0
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #1
	bl 0x0200d0c8
	movs	r1, #188
	movs	r2, #196
	movs	r0, #16
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x0200d238
	ldr	r6, [pc, #732]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r6, r6, r1
	movs	r2, #204
	movs	r1, #188
	ldr	r0, [r6, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x0200d238
	movs	r7, #192
	movs	r3, #176
	movs	r1, #236
	movs	r2, #227
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r3, r3, #8
	lsls	r2, r2, #17
	lsls	r7, r7, #18
	bl 0x0200d238
	ldr	r2, [r7, #108]
	movs	r3, #128
	movs	r5, #214
	lsls	r3, r3, #2
	adds	r3, #2
	lsls	r5, r5, #1
	str	r3, [r2, r5]
	bl 0x0200d318
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #668]
	adds	r2, #153
	bl 0x0200d1e8
	movs	r1, #232
	movs	r0, #12
	lsls	r1, r1, #1
	adds	r2, r5, #0
	bl 0x0200d220
	movs	r1, #216
	movs	r2, #204
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #128
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x0200d2a8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x0200d2d0
	ldr	r0, [pc, #616]
	bl 0x0200d280
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #12
	bl 0x0200d2c8
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r2, #40
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #16
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #12
	movs	r1, #2
	bl 0x0200d260
	movs	r1, #128
	movs	r2, #20
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #10
	bl 0x0200d2a8
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #0
	movs	r0, #16
	bl 0x0200d288
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200d1d8
	cmp	r0, #0
	bne.n	.L_0200499a
	movs	r0, #12
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	ldr	r2, [r7, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020049b8
.L_0200499a:
	ldr	r2, [r7, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	movs	r0, #12
	adds	r3, #1
	movs	r1, #3
	strh	r3, [r2, #0]
	bl 0x0200d248
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
.L_020049b8:
	ldr	r5, [pc, #404]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #16
	bl 0x0200d2c8
	movs	r0, #16
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #129
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200d2d0
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #16
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #16
	movs	r1, #0
	movs	r2, #40
	bl 0x0200d290
	movs	r1, #160
	movs	r2, #40
	movs	r0, #12
	lsls	r1, r1, #7
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200d2a8
	movs	r1, #128
	movs	r2, #20
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #16
	bl 0x0200d2c8
	movs	r0, #16
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200d2d8
	movs	r0, #212
	movs	r1, #128
	movs	r2, #192
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	bl 0x0200d2e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1e8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #16
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1e8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #12
	adds	r1, #204
	bl 0x0200d1e8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #116]
	bl 0x0200d1f0
	ldr	r1, [pc, #112]
	movs	r0, #16
	bl 0x0200d1f0
	ldr	r1, [pc, #108]
	movs	r0, #12
	bl 0x0200d200
	movs	r0, #40
	bl 0x0200d1b8
	bl 0x02008bcc
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200d2a8
	movs	r0, #160
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #16
	bl 0x0200d288
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200d1d8
	cmp	r0, #0
	bne.n	.L_02004b68
	bl 0x0200c7f0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02004b7e
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00013333
	.4byte 0x00001781
	.4byte 0x0200d564
	.4byte 0x0200d5a0
	.2byte 0xd5e4
	.2byte 0x0200
.L_02004b68:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200c7f0
.L_02004b7e:
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	ldr	r5, [pc, #228]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r2, #0
	movs	r1, #0
	bl 0x0200d2a8
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #16
	movs	r1, #3
	bl 0x0200d250
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #16
	bl 0x0200d2c8
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #16
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200d2a8
	movs	r0, #160
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #16
	bl 0x0200d288
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200d1d8
	cmp	r0, #0
	bne.n	.L_02004c74
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #16
	bl 0x0200d2c8
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #16
	movs	r1, #0
	bl 0x0200d298
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_02004cbc
	.2byte 0x0240
	.2byte 0x0200
.L_02004c74:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	movs	r0, #12
	adds	r3, #2
	strh	r3, [r2, #0]
	movs	r1, #4
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #8
	adds	r1, #255
	movs	r0, #16
	movs	r2, #20
	bl 0x0200d2c8
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #16
	movs	r1, #0
	bl 0x0200d298
.L_02004cbc:
	ldr	r6, [pc, #408]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200d2a8
	movs	r0, #212
	movs	r2, #200
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	movs	r1, #0
	bl 0x0200d2e0
	movs	r0, #153
	movs	r1, #152
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
	bl 0x0200d2d8
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #204
	movs	r0, #12
	adds	r1, #153
	bl 0x0200d1e8
	ldr	r0, [r6, #0]
	movs	r1, #12
	bl 0x0200d380
	movs	r0, #16
	movs	r1, #12
	bl 0x0200d380
	movs	r1, #244
	movs	r2, #208
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #236
	movs	r2, #216
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #160
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #7
	bl 0x0200d2a8
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #16
	movs	r1, #0
	movs	r2, #80
	bl 0x0200d290
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #12
	bl 0x0200d2c8
	movs	r1, #176
	movs	r2, #20
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x0200d2a8
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #12
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x0200d298
	movs	r0, #212
	movs	r1, #128
	movs	r2, #192
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	bl 0x0200d2e0
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #184]
	adds	r2, #153
	bl 0x0200d1e8
	movs	r1, #244
	movs	r2, #208
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #252
	movs	r2, #188
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #8
	movs	r0, #12
	bl 0x0200d2a8
	movs	r0, #12
	bl 0x0200d1e0
	adds	r5, r0, #0
	ldr	r3, [r5, #80]
	movs	r0, #12
	movs	r1, #1
	mov	sl, r3
	bl 0x0200d2b8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #112]
	adds	r2, #204
	bl 0x0200d1e8
	movs	r0, #12
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d258
	movs	r1, #246
	movs	r2, #188
	lsls	r2, r2, #1
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x0200d210
	movs	r0, #12
	movs	r1, #7
	bl 0x0200d248
	ldr	r3, [pc, #76]
	ldr	r2, [pc, #60]
	str	r3, [r5, #24]
	movs	r1, #0
	movs	r3, #224
	adds	r7, r5, #0
	mov	r8, r1
	lsls	r3, r3, #8
	mov	r1, sl
	adds	r7, #85
	strh	r3, [r1, #18]
	strb	r2, [r7, #0]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r5, #12]
	movs	r0, #12
	bl 0x0200d1e0
	movs	r1, #0
	bl 0x0200d188
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #12
	adds	r1, #102
	adds	r2, #51
	b.n	.L_02004e68
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x00013333
	.4byte 0x00019999
	.2byte 0x0000
	.2byte 0xffff
.L_02004e68:
	.2byte 0xf000
	.2byte 0xf9be
	.2byte 0x21de
	movs	r2, #160
	movs	r3, #188
	lsls	r2, r2, #14
	lsls	r1, r1, #17
	lsls	r3, r3, #17
	adds	r0, r5, #0
	bl 0x0200d160
	movs	r0, #12
	bl 0x0200d228
	movs	r3, #3
	strb	r3, [r7, #0]
	movs	r0, #12
	bl 0x0200d1e0
	movs	r1, #1
	bl 0x0200d188
	mov	r3, r8
	mov	r1, sl
	strh	r3, [r1, #18]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r5, #20]
	movs	r0, #12
	movs	r1, #1
	bl 0x0200d248
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #248]
	adds	r2, #204
	bl 0x0200d1e8
	movs	r0, #12
	movs	r1, #4
	movs	r2, #0
	bl 0x0200d258
	movs	r1, #212
	movs	r2, #188
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d210
	movs	r0, #204
	movs	r1, #128
	movs	r2, #200
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	bl 0x0200d2e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #12
	adds	r1, #204
	adds	r2, #102
	bl 0x0200d1e8
	movs	r1, #196
	movs	r2, #180
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200d220
	movs	r1, #192
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #6
	bl 0x0200d2a8
	movs	r1, #2
	movs	r0, #12
	bl 0x0200d2b8
	movs	r0, #16
	bl 0x0200d1f8
	ldr	r0, [r6, #0]
	bl 0x0200d1f8
	movs	r0, #1
	bl 0x0200d0c8
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200d2a8
	movs	r1, #160
	movs	r2, #10
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200d2a8
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200d248
	movs	r0, #16
	movs	r1, #3
	bl 0x0200d250
	movs	r0, #16
	movs	r1, #2
	bl 0x0200d248
	ldr	r0, [r6, #0]
	bl 0x0200d1e0
	cmp	r0, #0
	beq.n	.L_02004f74
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #16
	bl 0x0200d208
.L_02004f74:
	movs	r0, #16
	bl 0x0200d228
	movs	r1, #0
	movs	r2, #0
	movs	r0, #16
	bl 0x0200d230
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200d118
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
	bl 0x0200d1c8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	movs	r0, #234
	movs	r1, #204
	movs	r2, #160
	movs	r3, #172
	adds	r0, #255
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	lsls	r3, r3, #17
	ldr	r5, [pc, #92]
	bl 0x0200d140
	movs	r7, #0
	str	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_02005022
	ldr	r6, [r0, #80]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r6, #9]
	adds	r3, r0, #0
	adds	r3, #85
	adds	r2, r0, #0
	adds	r2, #92
	strb	r7, [r3, #0]
	movs	r1, #193
	movs	r3, #1
	strb	r3, [r2, #0]
	lsls	r1, r1, #3
	strb	r7, [r6, #26]
	strb	r7, [r6, #27]
	movs	r0, #68
	bl 0x0200d0f8
	adds	r5, r0, #0
	movs	r0, #198
	bl 0x0200d1b0
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x0200d108
	movs	r0, #68
	bl 0x0200d100
.L_02005022:
	pop	{r5, r6, r7, pc}
	.2byte 0xe2ac
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200d1c0
	movs	r0, #0
	bl 0x0200d370
	bl 0x0200d318
	bl 0x0200d328
	movs	r0, #20
	bl 0x0200d1b8
	movs	r0, #121
	bl 0x0200d3d8
	movs	r5, #1
	movs	r1, #0
	movs	r2, #20
	movs	r3, #0
	movs	r0, #41
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d170
	movs	r0, #10
	bl 0x0200d1b8
	movs	r1, #1
	movs	r2, #20
	movs	r3, #1
	movs	r0, #41
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d170
	movs	r0, #10
	bl 0x0200d1b8
	movs	r1, #2
	movs	r2, #20
	movs	r3, #2
	movs	r0, #41
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d170
	movs	r0, #10
	bl 0x0200d1b8
	movs	r1, #3
	movs	r2, #20
	movs	r3, #3
	movs	r0, #41
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200d170
	movs	r0, #10
	bl 0x0200d1b8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #45
	bl 0x0200d118
	bl 0x0200d1c8
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.section .rodata.x0200d3e0,"a",%progbits
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
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x0000002e
	.4byte 0x0200828d
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0xffff000c
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00000018
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x0200d3e0
	.4byte 0x0200d41c
	.4byte 0x0200d458
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x020082a1
	.4byte 0x0000002e
	.4byte 0x020082dd
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200835d
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00580000
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200835d
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x002c0000
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200835d
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
	.4byte 0x004a0140
	.4byte 0x01500060
	.4byte 0x0070005a
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x002a0200
	.4byte 0x02100110
	.4byte 0x0120003a
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000e
	.4byte 0x00101014
	.4byte 0x00203014
	.4byte 0x00301013
	.4byte 0x00402011
	.4byte 0x00505011
	.4byte 0x00603011
	.4byte 0x00704011
	.4byte 0x0000000f
	.4byte 0x00109016
	.4byte 0x00204014
	.4byte 0x00301010
	.4byte 0x00402014
	.4byte 0x00000010
	.4byte 0x0010300f
	.4byte 0x00208016
	.4byte 0x00000011
	.4byte 0x00102013
	.4byte 0x0020400e
	.4byte 0x0030500e
	.4byte 0x0040600e
	.4byte 0x00000012
	.4byte 0x00103015
	.4byte 0x00204015
	.4byte 0x00000013
	.4byte 0x0010300e
	.4byte 0x00201011
	.4byte 0x00000014
	.4byte 0x0010100e
	.4byte 0x0020400f
	.4byte 0x0030200e
	.4byte 0x0040200f
	.4byte 0x00000015
	.4byte 0x00104002
	.4byte 0x00201016
	.4byte 0x00301012
	.4byte 0x00402012
	.4byte 0x00000016
	.4byte 0x00102015
	.4byte 0x00204016
	.4byte 0x00305016
	.4byte 0x00402016
	.4byte 0x00503016
	.4byte 0x00607016
	.4byte 0x00706016
	.4byte 0x00802010
	.4byte 0x0090100f
	.4byte 0x00d0d016
	.4byte 0x00e0e016
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
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
	.4byte 0xffff0170
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0170
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff00b8
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff00b8
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff00b8
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
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
	.4byte 0x02024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
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
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01540000
	.4byte 0x00024000
	.4byte 0x004b00f4
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00024000
	.4byte 0xffff005b
	.4byte 0x0200d514
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00013000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00015000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x014e0000
	.4byte 0x00000000
	.4byte 0x013d0000
	.4byte 0x00023000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x011c0000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00023000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x013e0000
	.4byte 0x00025000
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
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02a40000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02a40000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00034000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x0200d634
	.4byte 0x01b70000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x0200d634
	.4byte 0x01670000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x0200d64c
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x01440000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x0200d664
	.4byte 0x005a0000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x0200d664
	.4byte 0x00ae0000
	.4byte 0x00000000
	.4byte 0x00480000
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
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x0200a0c9
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200a0d9
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte 0x0200a0c9
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x0200a0d9
	.4byte 0x00000002
	.2byte 0x000a
	.2byte 0x0211
	push	{r0, r2, r7, lr}
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0xb93d
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0xb9a9
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
.L_02005de0:
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
	movs	r1, r4
	movs	r0, r0
.L_02005e00:
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
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r6
	movs	r0, r0
.L_02005e24:
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
	movs	r0, r1
.L_02005e32:
	lsls	r4, r0, #12
	add	r1, pc, #740
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
.L_02005e3a:
	movs	r0, r0
	movs	r0, r1
.L_02005e3e:
	lsls	r4, r0, #12
	add	r1, pc, #884
	lsls	r0, r0, #8
	lsls	r2, r0, #24
	movs	r0, r0
	movs	r2, r1
	lsls	r4, r0, #12
	add	r2, pc, #292
	lsls	r0, r0, #8
	stmia	r6!, {r1}
	movs	r0, r0
	movs	r3, r1
.L_02005e56:
	lsls	r4, r0, #12
.L_02005e58:
	add	r2, pc, #292
	lsls	r0, r0, #8
	strh	r2, [r0, #48]
	movs	r0, r0
	movs	r4, r1
	lsls	r4, r0, #12
.L_02005e64:
	add	r2, pc, #292
.L_02005e66:
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x9d01
	lsls	r0, r0, #8
	movs	r2, r0
.L_02005e76:
	movs	r0, r0
.L_02005e78:
	movs	r6, r0
	lsrs	r5, r5, #4
	add	r6, pc, #820
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
.L_02005e86:
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
.L_02005e9a:
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r0, r1
.L_02005ea6:
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xa121
	lsls	r0, r0, #8
	movs	r1, r1
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xa155
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	asrs	r0, r0, #32
.L_02005ec0:
	movs	r0, r1
	.2byte 0xffff
	.2byte 0xa121
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0xa155
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r4, r2
.L_02005eda:
	.2byte 0xffff
	.2byte 0xb565
	lsls	r0, r0, #8
.L_02005ee0:
	movs	r2, r0
	movs	r0, r0
	movs	r5, r2
	.2byte 0xffff
	.2byte 0xb575
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r4
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r4
.L_02005f12:
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
.L_02005f1c:
	movs	r1, r0
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
.L_02005f26:
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
.L_02005f2c:
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x8711
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r0
.L_02005f42:
	movs	r0, r0
.L_02005f44:
	movs	r1, r0
.L_02005f46:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r4
.L_02005f4e:
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
	movs	r1, r4
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
.L_02005f70:
	strh	r5, [r2, #40]
	movs	r0, r0
	movs	r0, r1
	lsls	r7, r1, #8
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r3, r1
.L_02005f82:
	.2byte 0xffff
	.2byte 0x8281
	lsls	r0, r0, #8
.L_02005f88:
	movs	r2, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0xa675
.L_02005f92:
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r3, r1
	.2byte 0xffff
	.2byte 0xa69d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x177a
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x177b
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02005fbc:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02005fc4:
	movs	r1, r0
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
.L_02005fd4:
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
.L_02005fda:
	movs	r0, r0
.L_02005fdc:
	movs	r1, r6
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
.L_02005fe6:
	movs	r0, r0
	.2byte 0x4602
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0xa015
.L_02005ff2:
	lsls	r0, r0, #8
	lsls	r3, r6, #7
	movs	r0, r0
	lsls	r0, r1, #3
	.2byte 0xffff
	.2byte 0x303d
	lsls	r0, r0, #1
	movs	r2, r0
	movs	r0, r0
	movs	r4, r2
	lsls	r0, r6, #8
	strh	r1, [r5, #36]
	lsls	r0, r0, #8
	.2byte 0x4602
.L_0200600e:
	movs	r0, r0
	movs	r4, r0
	lsls	r2, r2, #8
	add	r2, pc, #772
	lsls	r0, r0, #8
	stmia	r6!, {r1}
	movs	r0, r0
	movs	r5, r0
.L_0200601e:
	lsls	r2, r2, #8
	add	r2, pc, #772
	lsls	r0, r0, #8
	movs	r3, r0
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x9d21
	lsls	r0, r0, #8
	ldr	r6, [pc, #84]
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0xa315
	lsls	r0, r0, #8
	ldrh	r5, [r0, #8]
	str	r0, [r0, r0]
	movs	r6, r3
.L_02006042:
	.2byte 0xffff
	.2byte 0xa7b5
	lsls	r0, r0, #8
	ldrh	r5, [r0, #8]
	str	r0, [r0, r0]
	movs	r7, r3
	.2byte 0xffff
	.2byte 0xa7d5
	lsls	r0, r0, #8
.L_02006054:
	ldrh	r5, [r0, #8]
	str	r0, [r0, r0]
	movs	r0, r4
	.2byte 0xffff
	.2byte 0xa7f5
	lsls	r0, r0, #8
	ldrh	r5, [r0, #8]
	str	r0, [r0, r0]
	movs	r1, r4
	.2byte 0xffff
	.2byte 0xa815
.L_0200606a:
	lsls	r0, r0, #8
	ldrh	r5, [r0, #8]
.L_0200606e:
	str	r0, [r0, r0]
	movs	r2, r4
.L_02006072:
	.2byte 0xffff
	.2byte 0xa835
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r1, r1
.L_0200607e:
	.2byte 0xffff
	.2byte 0x9d41
	lsls	r0, r0, #8
	movs	r0, r0
.L_02006086:
	movs	r0, r0
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x9db5
.L_0200608e:
	lsls	r0, r0, #8
	ldrh	r5, [r2, #40]
	movs	r0, r0
	lsls	r1, r1, #16
	lsrs	r5, r0, #1
	ldr	r6, [sp, #196]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #40]
	movs	r0, r0
.L_020060a0:
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x175d
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	lsls	r2, r1, #16
	lsrs	r6, r0, #1
.L_020060b0:
	ldr	r6, [sp, #276]
	lsls	r0, r0, #8
	ldrh	r5, [r2, #40]
	movs	r0, r0
.L_020060b8:
	movs	r2, r1
	.2byte 0xffff
	.2byte 0x175e
	movs	r0, r0
.L_020060c0:
	movs	r0, r0
.L_020060c2:
	movs	r0, r0
	movs	r3, r1
	lsrs	r1, r1, #1
.L_020060c8:
	asrs	r7, r3, #29
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_020060d0:
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x17b1
	movs	r0, r0
.L_020060d8:
	movs	r0, r0
	movs	r0, r0
	movs	r4, r1
	lsrs	r1, r1, #1
.L_020060e0:
	asrs	r0, r4, #29
	movs	r0, r0
.L_020060e4:
	movs	r0, r0
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x17b2
	movs	r0, r0
.L_020060f0:
	movs	r0, r0
	movs	r0, r0
	movs	r5, r1
	lsrs	r1, r1, #1
	asrs	r1, r4, #29
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x17b3
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r3, r1
	lsrs	r1, r1, #1
.L_02006110:
	asrs	r2, r4, #29
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r3, r1
	.2byte 0xffff
	.2byte 0x17b4
	movs	r0, r0
	ldrh	r5, [r2, #40]
.L_02006122:
	movs	r0, r0
	movs	r4, r1
	lsrs	r1, r1, #1
	asrs	r3, r4, #29
	movs	r0, r0
	ldrh	r5, [r2, #40]
.L_0200612e:
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x17b5
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r5, r1
	lsrs	r1, r1, #1
.L_02006140:
	asrs	r4, r4, #29
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x17b6
	movs	r0, r0
.L_02006150:
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
.L_020061a4:
	movs	r1, r0
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0007
	movs	r0, r0
	lsls	r2, r0, #8
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x9e59
	lsls	r0, r0, #8
	stmia	r6!, {r1}
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x9f7d
	lsls	r0, r0, #8
.L_020061c8:
	strh	r5, [r2, #40]
	movs	r0, r0
	movs	r0, r1
	lsls	r7, r1, #8
	movs	r0, r0
	movs	r0, r0
	strh	r5, [r2, #40]
	movs	r0, r0
.L_020061d8:
	movs	r2, r1
	lsls	r0, r2, #8
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r5, r1
	lsrs	r1, r1, #1
	asrs	r6, r6, #29
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x9e0d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r6, r1
.L_020061fe:
	lsrs	r1, r1, #1
	asrs	r7, r6, #29
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r6, r1
	.2byte 0xffff
	.2byte 0x17ac
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r7, r1
	.2byte 0xffff
	.2byte 0x17ad
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r5, r1
	lsrs	r1, r1, #1
	asrs	r0, r7, #29
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x17ae
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r6, r1
	lsrs	r1, r1, #1
	asrs	r1, r7, #29
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r6, r1
	.2byte 0xffff
	.2byte 0x17af
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r7, r1
	.2byte 0xffff
	.2byte 0x17b0
	movs	r0, r0
	movs	r3, r0
	movs	r0, r0
	movs	r0, r2
	lsrs	r2, r1, #1
	add	r3, pc, #932
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r4, r1
	lsrs	r2, r1, #1
	asrs	r0, r4, #30
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x17a7
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
.L_02006280:
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x17a8
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
