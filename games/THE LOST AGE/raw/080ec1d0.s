.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080140d8, 0x080140d8
	.set sub_080144c0, 0x080144c0
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_080149e0, 0x080149e0
	.set sub_0801587c, 0x0801587c
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08020270, 0x08020270
	.set sub_08038000, 0x08038000
	.set sub_08038040, 0x08038040
	.set sub_08038060, 0x08038060
	.set sub_08038078, 0x08038078
	.set sub_080383a0, 0x080383a0
	.set sub_080ca280, 0x080ca280
	.set sub_080ca368, 0x080ca368
	.set sub_080cad84, 0x080cad84
	.set sub_080cad9c, 0x080cad9c
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2260, 0x080d2260
	.set sub_080d2a3c, 0x080d2a3c
	.set sub_080d2a64, 0x080d2a64
	.set sub_080d2a8c, 0x080d2a8c
	.set sub_080d46a4, 0x080d46a4
	.set sub_080ebf94, 0x080ebf94
	.set sub_080ec14c, 0x080ec14c
	.global Overlay_080ec1d0
Overlay_080ec1d0:
.L_080ec1d0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r1
	mov	r8, r0
	bl	sub_080cdf5c
	movs	r7, #143
	movs	r3, #192
	lsls	r3, r3, #18
	lsls	r7, r7, #1
	ldr	r5, [r3, #108]
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_08016d18
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080ec27a
	adds	r0, r6, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080ec288
	movs	r3, #10
	ldrsh	r6, [r0, r3]
	movs	r2, #18
	ldrsh	r5, [r0, r2]
	movs	r3, #197
	lsls	r3, r3, #3
	adds	r3, #255
	cmp	r5, r3
	bgt.n	.L_080ec244
	movs	r2, #184
	lsls	r2, r2, #6
	adds	r2, #223
	cmp	r6, r2
	bgt.n	.L_080ec238
	adds	r0, r7, #0
	bl	sub_08016cfc
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #174
	adds	r6, #65
	b.n	.L_080ec242
.L_080ec238:
	ldr	r2, [pc, #84]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #196
	adds	r6, r6, r2
.L_080ec242:
	adds	r5, r5, r3
.L_080ec244:
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #85
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_080ec258
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r2, #255
	adds	r3, r3, r2
.L_080ec258:
	ldr	r2, [pc, #56]
	asrs	r3, r3, #14
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r2, #0]
	lsls	r3, r5, #2
	adds	r3, r3, r5
	lsls	r0, r3, #7
	cmp	r0, #0
	bge.n	.L_080ec274
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	adds	r0, r0, r3
.L_080ec274:
	asrs	r3, r0, #14
	subs	r3, #112
	b.n	.L_080ec284
.L_080ec27a:
	movs	r3, #132
	lsls	r3, r3, #1
	mov	r2, r8
	str	r3, [r2, #0]
	movs	r3, #16
.L_080ec284:
	mov	r2, sl
	str	r3, [r2, #0]
.L_080ec288:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffd8d
	.2byte 0xfef3
	.2byte 0xffff
.L_080ec298:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080ec2aa
	ldrb	r0, [r0, #1]
	lsrs	r0, r0, #4
	b.n	.L_080ec2b0
.L_080ec2aa:
	ldrb	r3, [r0, #1]
	movs	r0, #15
	ands	r0, r3
.L_080ec2b0:
	pop	{pc}
	movs	r0, r0
	.2byte 0xa642
	.2byte 0x0202
.L_080ec2b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r0, [sp, #20]
	bl	sub_080cdf5c
	bl	.L_080ed804
	mov	fp, r0
	movs	r0, #128
	lsls	r0, r0, #19
	ldr	r5, [pc, #408]
	str	r0, [sp, #8]
	movs	r4, #1
	movs	r2, #6
	ldrsh	r1, [r5, r2]
	movs	r0, #10
	ldrsh	r3, [r5, r0]
	mov	r9, r1
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #68
	adds	r5, r5, r1
	ldr	r5, [r5, #0]
	negs	r4, r4
	mov	sl, r3
	cmp	r5, #0
	beq.n	.L_080ec30a
	ldr	r3, [pc, #380]
	asrs	r5, r5, #16
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	mov	r9, r5
	mov	sl, r2
.L_080ec30a:
	mov	r7, fp
	ldrb	r3, [r7, #0]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_080ec3aa
	adds	r5, r7, #0
.L_080ec318:
	ldrb	r3, [r7, #0]
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r0, r7, #0
	str	r4, [sp, #0]
	adds	r6, r3, r2
	bl	.L_080ec298
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080ec396
	cmp	r0, #15
	beq.n	.L_080ec396
	adds	r0, r6, #0
	bl	sub_08016ce4
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080ec396
	movs	r3, #4
	ldrsh	r2, [r7, r3]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #85
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L_080ec356
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	adds	r2, r2, r0
.L_080ec356:
	asrs	r3, r2, #14
	ldr	r2, [pc, #292]
	adds	r1, r3, r2
	movs	r3, #6
	ldrsh	r2, [r7, r3]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #7
	cmp	r0, #0
	bge.n	.L_080ec372
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r2, #255
	adds	r0, r0, r2
.L_080ec372:
	asrs	r3, r0, #14
	mov	r0, r9
	subs	r2, r1, r0
	subs	r3, #112
	mov	r1, sl
	subs	r3, r3, r1
	adds	r0, r2, #0
	muls	r0, r2
	adds	r1, r3, #0
	muls	r1, r3
	adds	r2, r0, #0
	adds	r3, r1, #0
	adds	r0, r2, r3
	ldr	r2, [sp, #8]
	cmp	r2, r0
	ble.n	.L_080ec396
	str	r0, [sp, #8]
	mov	r4, r8
.L_080ec396:
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r5, #20
	cmp	r0, #127
	bgt.n	.L_080ec3aa
	adds	r7, r5, #0
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_080ec318
.L_080ec3aa:
	movs	r1, #1
	negs	r1, r1
	cmp	r4, r1
	bne.n	.L_080ec3c0
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #16]
	bl	.L_080ec1d0
	b.n	.L_080ec468
.L_080ec3bc:
	adds	r7, r5, #0
	b.n	.L_080ec428
.L_080ec3c0:
	lsls	r3, r4, #2
	adds	r3, r3, r4
	lsls	r3, r3, #2
	mov	r2, fp
	adds	r7, r2, r3
	ldr	r3, [sp, #8]
	cmp	r3, #4
	bgt.n	.L_080ec428
	movs	r2, #0
.L_080ec3d2:
	ldr	r0, [sp, #12]
	adds	r4, r4, r0
	cmp	r4, #0
	bge.n	.L_080ec3e2
	mov	r4, r8
	subs	r4, #1
	adds	r2, #1
	b.n	.L_080ec424
.L_080ec3e2:
	lsls	r3, r4, #2
	adds	r3, r3, r4
	lsls	r3, r3, #2
	mov	r1, fp
	adds	r5, r1, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080ec3f8
	movs	r4, #0
	adds	r2, #1
	b.n	.L_080ec424
.L_080ec3f8:
	ldrb	r3, [r5, #0]
	movs	r0, #192
	lsls	r0, r0, #1
	adds	r6, r3, r0
	adds	r0, r5, #0
	str	r2, [sp, #4]
	str	r4, [sp, #0]
	bl	.L_080ec298
	ldr	r2, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080ec424
	cmp	r0, #15
	beq.n	.L_080ec424
	adds	r0, r6, #0
	bl	sub_08016ce4
	ldr	r2, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080ec3bc
.L_080ec424:
	cmp	r2, #1
	ble.n	.L_080ec3d2
.L_080ec428:
	movs	r1, #4
	ldrsh	r2, [r7, r1]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #85
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L_080ec440
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	adds	r2, r2, r3
.L_080ec440:
	ldr	r0, [pc, #60]
	ldr	r1, [sp, #20]
	asrs	r3, r2, #14
	adds	r3, r3, r0
	str	r3, [r1, #0]
	movs	r3, #6
	ldrsh	r2, [r7, r3]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #7
	cmp	r0, #0
	bge.n	.L_080ec460
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #255
	adds	r0, r0, r1
.L_080ec460:
	ldr	r2, [sp, #16]
	asrs	r3, r0, #14
	subs	r3, #112
	str	r3, [r2, #0]
.L_080ec468:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0202a000
	.4byte 0x0202a648
	.2byte 0xfef3
	.2byte 0xffff
.L_080ec484:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r5, r2, #0
	cmp	r0, #0
	beq.n	.L_080ec4ce
	ldr	r4, [r0, #0]
	cmp	r4, #0
	beq.n	.L_080ec4ce
	movs	r3, #128
	lsls	r3, r3, #1
	ldr	r7, [pc, #52]
	adds	r3, #255
	mov	ip, r3
.L_080ec49e:
	ldrh	r3, [r4, #6]
	adds	r1, r4, #0
	adds	r3, r3, r6
	strh	r3, [r4, #6]
	ldrh	r3, [r4, #8]
	adds	r1, #16
	adds	r3, r3, r5
	strh	r3, [r4, #8]
	mov	r3, ip
	ldrh	r0, [r1, #6]
	ldr	r4, [r4, #0]
	lsls	r2, r0, #23
	lsrs	r2, r2, #23
	adds	r2, r2, r6
	ands	r2, r3
	adds	r3, r7, #0
	ands	r3, r0
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldrb	r3, [r1, #4]
	adds	r3, r3, r5
	strb	r3, [r1, #4]
	cmp	r4, #0
	bne.n	.L_080ec49e
.L_080ec4ce:
	pop	{r5, r6, r7, pc}
	.2byte 0xfe00
	.2byte 0xffff
.L_080ec4d4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	bl	sub_080cdf5c
	ldr	r0, [pc, #680]
	ldr	r1, [pc, #680]
	adds	r0, #32
	str	r0, [sp, #52]
	ldr	r2, [pc, #676]
	ldrh	r3, [r1, #0]
	movs	r4, #0
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	movs	r2, #1
	lsrs	r3, r3, #5
	negs	r2, r2
	str	r3, [sp, #48]
	movs	r3, #64
	str	r2, [sp, #40]
	str	r3, [sp, #36]
	str	r4, [sp, #32]
	str	r4, [sp, #28]
	bl	.L_080ed804
	ldr	r5, [pc, #640]
	str	r0, [sp, #8]
	movs	r0, #198
	lsls	r0, r0, #3
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #636]
	movs	r1, #192
	ands	r3, r0
	str	r3, [sp, #4]
	lsls	r1, r1, #3
	adds	r1, #52
	adds	r3, r5, r1
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #624]
	ands	r3, r0
	str	r3, [sp, #0]
	ldr	r3, [pc, #620]
	movs	r2, #31
	ldr	r3, [r3, #0]
	lsrs	r3, r3, #1
	ands	r3, r2
	ldrb	r3, [r1, r3]
	str	r3, [sp, #44]
	ldr	r3, [pc, #612]
	ldr	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L_080ec630
	ldr	r3, [r5, #4]
	ldr	r4, [pc, #604]
	subs	r0, r2, r3
	mov	fp, r2
	ldr	r6, [r4, #0]
	cmp	r0, #0
	bge.n	.L_080ec560
	movs	r5, #255
	lsls	r5, r5, #8
	adds	r5, #255
	adds	r0, r0, r5
.L_080ec560:
	asrs	r0, r0, #16
	mov	sl, r0
	ldr	r0, [pc, #556]
	ldr	r3, [r0, #8]
	subs	r0, r6, r3
	cmp	r0, #0
	bge.n	.L_080ec576
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_080ec576:
	asrs	r0, r0, #16
	mov	r8, r0
	mov	r2, sl
	mov	r4, r8
	mov	r3, r8
	muls	r3, r4
	mov	r0, sl
	muls	r0, r2
	adds	r0, r0, r3
	ldr	r3, [pc, #548]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4d81
	lsls	r7, r0, #16
	ldr	r3, [r5, #4]
	mov	r0, fp
	subs	r0, r0, r3
	ldr	r3, [r5, #8]
	movs	r1, #128
	subs	r3, r6, r3
	lsls	r1, r1, #15
	mov	sl, r0
	mov	r8, r3
	cmp	r7, r1
	bge.n	.L_080ec5c4
	ldr	r7, [pc, #520]
	mov	r1, sl
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4641
	adds	r5, r0, #0
	mov	r0, r8
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	adds	r7, r0, #0
.L_080ec5c4:
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L_080ec5cc
	adds	r1, r7, #3
.L_080ec5cc:
	asrs	r1, r1, #2
	movs	r3, #128
	mov	r9, r1
	lsls	r3, r3, #12
	cmp	r9, r3
	ble.n	.L_080ec5da
	mov	r9, r3
.L_080ec5da:
	movs	r2, #128
	lsls	r2, r2, #8
	cmp	r7, r2
	bge.n	.L_080ec5f6
	ldr	r4, [pc, #432]
	mov	r3, fp
	str	r3, [r4, #4]
	str	r6, [r4, #8]
	ldr	r5, [sp, #28]
	ldr	r0, [pc, #440]
	ldr	r1, [pc, #444]
	str	r5, [r0, #0]
	str	r5, [r1, #0]
	b.n	.L_080ec6b2
.L_080ec5f6:
	cmp	r7, r9
	ble.n	.L_080ec620
	ldr	r2, [pc, #444]
	mov	r1, sl
	mov	fp, r2
	adds	r0, r7, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4d6b
	mov	r1, r9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4641
	mov	sl, r0
	adds	r0, r7, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4649
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4680
.L_080ec620:
	ldr	r4, [pc, #368]
	ldr	r3, [r4, #4]
	add	r3, sl
	str	r3, [r4, #4]
	ldr	r3, [r4, #8]
	add	r3, r8
	str	r3, [r4, #8]
	b.n	.L_080ec6b2
.L_080ec630:
	ldr	r3, [pc, #392]
	ldr	r1, [pc, #396]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r1, [r1, r3]
	lsrs	r3, r0, #16
	cmp	r1, r3
	beq.n	.L_080ec6b2
	ldr	r0, [pc, #332]
	add	r5, sp, #72
	ldr	r3, [r0, #4]
	str	r3, [r5, #0]
	ldr	r2, [sp, #28]
	str	r2, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r0, #8]
	str	r3, [r5, #8]
	ldr	r0, [r0, #24]
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	movs	r4, #128
	lsls	r4, r4, #13
	cmp	r3, r4
	bge.n	.L_080ec66e
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r5, #0]
.L_080ec66e:
	ldr	r2, [pc, #340]
	cmp	r3, r2
	ble.n	.L_080ec676
	str	r2, [r5, #0]
.L_080ec676:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bge.n	.L_080ec682
	ldr	r0, [sp, #28]
	movs	r3, #0
	str	r0, [r5, #8]
.L_080ec682:
	ldr	r2, [pc, #324]
	cmp	r3, r2
	ble.n	.L_080ec68a
	str	r2, [r5, #8]
.L_080ec68a:
	ldr	r3, [r5, #0]
	ldr	r1, [pc, #260]
	ldr	r2, [pc, #316]
	str	r3, [r1, #4]
	ldr	r3, [r5, #8]
	str	r3, [r1, #8]
	ldr	r3, [r5, #0]
	str	r3, [r2, #0]
	adds	r2, #4
	ldr	r3, [r5, #8]
	str	r3, [r2, #0]
	movs	r2, #192
	ldr	r3, [r1, #24]
	lsls	r2, r2, #11
	cmp	r3, r2
	bge.n	.L_080ec6b2
	movs	r4, #128
	lsls	r4, r4, #6
	adds	r3, r3, r4
	str	r3, [r1, #24]
.L_080ec6b2:
	ldr	r5, [pc, #224]
	ldr	r7, [sp, #8]
	movs	r0, #6
	ldrsh	r5, [r5, r0]
	ldr	r0, [pc, #216]
	mov	sl, r5
	movs	r1, #10
	ldrsh	r0, [r0, r1]
	movs	r1, #0
	mov	r8, r0
	mov	r9, r1
.L_080ec6c8:
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_080ec6fa
	ldr	r3, [pc, #256]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080ec6da
	b.n	.L_080ec95e
.L_080ec6da:
	ldr	r3, [sp, #8]
	movs	r0, #1
	negs	r0, r0
	movs	r4, #160
	movs	r5, #128
	lsls	r4, r4, #4
	str	r0, [sp, #24]
	add	r1, sp, #64
	add	r0, sp, #68
	mov	r9, r5
	adds	r7, r3, r4
	movs	r6, #0
	movs	r5, #0
	bl	.L_080ec1d0
	b.n	.L_080ec826
.L_080ec6fa:
	movs	r2, #12
	ldrsh	r1, [r7, r2]
	movs	r0, #192
	str	r1, [sp, #24]
	lsls	r0, r0, #1
	movs	r4, #14
	ldrsh	r3, [r7, r4]
	str	r3, [sp, #12]
	ldrb	r3, [r7, #0]
	adds	r5, r3, r0
	adds	r0, r7, #0
	bl	.L_080ec298
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080ec71c
	b.n	.L_080ec950
.L_080ec71c:
	cmp	r6, #15
	bne.n	.L_080ec7dc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #222
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ec730
	b.n	.L_080ec950
.L_080ec730:
	ldr	r1, [pc, #160]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_080ec744
	b.n	.L_080ec950
.L_080ec744:
	movs	r5, #2
	negs	r5, r5
	str	r5, [sp, #24]
	movs	r0, #158
	lsls	r0, r0, #2
	adds	r3, r1, r0
	movs	r4, #2
	ldrsh	r2, [r3, r4]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #85
	muls	r2, r3
	movs	r6, #2
	movs	r5, #0
	cmp	r2, #0
	bge.n	.L_080ec76c
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	adds	r2, r2, r0
.L_080ec76c:
	asrs	r3, r2, #14
	ldr	r2, [pc, #104]
	movs	r4, #159
	adds	r3, r3, r2
	str	r3, [sp, #68]
	lsls	r4, r4, #2
	adds	r3, r1, r4
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #7
	cmp	r3, #0
	bge.n	.L_080ec820
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #255
	adds	r3, r3, r1
	b.n	.L_080ec820
	movs	r0, r0
	.4byte 0x0202a000
	.4byte 0x020036e0
	.4byte 0xffff0000
	.4byte 0x080f1100
	.4byte 0x0300122c
	.4byte 0x0202a644
	.4byte 0x0202a648
	.4byte 0x030002d4
	.4byte 0x0300021c
	.4byte 0x03000230
	.4byte 0x03001150
	.4byte 0x080f1120
	.4byte 0x01ff0000
	.4byte 0x016d0000
	.4byte 0x0202a64c
	.4byte 0x0202a642
	.4byte 0x02000240
	.2byte 0xfef3
	.2byte 0xffff
.L_080ec7dc:
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ec7e8
	b.n	.L_080ec950
.L_080ec7e8:
	movs	r3, #4
	ldrsh	r2, [r7, r3]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #85
	muls	r2, r3
	movs	r5, #1
	cmp	r2, #0
	bge.n	.L_080ec802
	movs	r4, #252
	lsls	r4, r4, #6
	adds	r4, #255
	adds	r2, r2, r4
.L_080ec802:
	ldr	r0, [pc, #92]
	asrs	r3, r2, #14
	movs	r1, #6
	ldrsh	r2, [r7, r1]
	adds	r3, r3, r0
	str	r3, [sp, #68]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #7
	cmp	r3, #0
	bge.n	.L_080ec820
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r2, #255
	adds	r3, r3, r2
.L_080ec820:
	asrs	r3, r3, #14
	subs	r3, #112
	str	r3, [sp, #64]
.L_080ec826:
	ldr	r4, [sp, #52]
	movs	r0, #13
	ldrb	r3, [r4, #5]
	negs	r0, r0
	adds	r2, r0, #0
	lsls	r1, r5, #2
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r4, #5]
	ldr	r2, [sp, #48]
	ldr	r3, [pc, #28]
	adds	r1, r2, r6
	ands	r1, r3
	ldr	r2, [pc, #24]
	ldrh	r3, [r4, #8]
	ldr	r4, [sp, #52]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r4, #8]
	ldr	r3, [pc, #20]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L_080ec8b0
	b.n	.L_080ec868
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0xfffffef3
	.2byte 0xa640
	.2byte 0x0202
.L_080ec868:
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #68]
	asrs	r3, r1, #16
	subs	r2, r2, r3
	subs	r3, r2, #1
	ldr	r4, [sp, #0]
	mov	fp, r3
	ldr	r3, [sp, #64]
	asrs	r1, r4, #16
	subs	r3, r3, r1
	adds	r2, #3
	subs	r6, r3, #1
	cmp	r2, #247
	bhi.n	.L_080ec950
	movs	r0, #4
	negs	r0, r0
	cmp	r6, r0
	blt.n	.L_080ec950
	cmp	r6, #163
	bgt.n	.L_080ec950
	ldr	r4, [sp, #52]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, fp
	adds	r3, #255
	ldrh	r2, [r4, #6]
	ands	r3, r1
	ldr	r1, [pc, #12]
	adds	r0, r4, #0
	ands	r2, r1
	orrs	r2, r3
	strh	r2, [r0, #6]
	strb	r6, [r0, #4]
	b.n	.L_080ec8fc
	.2byte 0xfe00
	.2byte 0xffff
.L_080ec8b0:
	ldr	r2, [sp, #68]
	movs	r1, #160
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r1, r1, #2
	lsls	r0, r0, #2
	bl	sub_08002054
	ldr	r3, [pc, #44]
	ldr	r1, [sp, #52]
	subs	r0, #1
	ands	r0, r3
	ldr	r2, [pc, #40]
	ldrh	r3, [r1, #6]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #6]
	ldr	r2, [sp, #64]
	movs	r1, #160
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #2
	lsls	r1, r1, #2
	bl	sub_08002054
	ldr	r3, [sp, #52]
	subs	r0, #1
	strb	r0, [r3, #4]
	b.n	.L_080ec8fc
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_080ec8fc:
	ldr	r0, [sp, #68]
	ldr	r1, [sp, #64]
	mov	r4, sl
	subs	r2, r0, r4
	mov	r4, r8
	subs	r3, r1, r4
	adds	r4, r2, #0
	muls	r4, r2
	str	r2, [sp, #60]
	adds	r2, r4, #0
	adds	r4, r3, #0
	muls	r4, r3
	str	r3, [sp, #56]
	adds	r3, r4, #0
	adds	r2, r2, r3
	ldr	r3, [sp, #36]
	cmp	r2, r3
	bge.n	.L_080ec932
	mov	r4, r9
	str	r4, [sp, #40]
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #12]
	str	r3, [sp, #20]
	str	r4, [sp, #16]
	str	r2, [sp, #36]
	str	r0, [sp, #32]
	str	r1, [sp, #28]
.L_080ec932:
	cmp	r5, #0
	bne.n	.L_080ec942
	ldr	r3, [pc, #156]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #7
	bhi.n	.L_080ec950
.L_080ec942:
	ldr	r0, [sp, #52]
	movs	r1, #246
	adds	r5, r0, #0
	adds	r5, #12
	str	r5, [sp, #52]
	bl	sub_080140d8
.L_080ec950:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r7, #20
	cmp	r1, #127
	bgt.n	.L_080ec95e
	b.n	.L_080ec6c8
.L_080ec95e:
	ldr	r3, [pc, #120]
	ldr	r6, [r3, #0]
	cmp	r6, #0
	bne.n	.L_080ec9e8
	ldr	r2, [sp, #40]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080ec9e0
	ldr	r4, [sp, #36]
	cmp	r4, #2
	bgt.n	.L_080ec98c
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #96]
	lsls	r3, r5, #16
	str	r3, [r0, #4]
	ldr	r1, [sp, #28]
	lsls	r3, r1, #16
	str	r3, [r0, #8]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	b.n	.L_080ec9c2
.L_080ec98c:
	ldr	r2, [sp, #28]
	ldr	r4, [sp, #32]
	mov	r3, r8
	mov	r5, sl
	subs	r1, r4, r5
	subs	r0, r2, r3
	bl	sub_080148e8
	adds	r1, r0, #0
	ldr	r0, [pc, #60]
	add	r5, sp, #72
	ldr	r3, [r0, #4]
	str	r6, [r5, #4]
	str	r3, [r5, #0]
	lsls	r1, r1, #16
	ldr	r3, [r0, #8]
	lsrs	r1, r1, #16
	str	r3, [r5, #8]
	adds	r2, r5, #0
	ldr	r0, [r0, #24]
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	ldr	r1, [pc, #32]
	str	r3, [r1, #4]
	ldr	r3, [r5, #8]
	str	r3, [r1, #8]
.L_080ec9c2:
	ldr	r2, [pc, #24]
	movs	r3, #6
	ldrsh	r2, [r2, r3]
	ldr	r3, [pc, #16]
	mov	sl, r2
	movs	r4, #10
	ldrsh	r3, [r3, r4]
	mov	r8, r3
	b.n	.L_080ec9e8
	.4byte 0x0300122c
	.4byte 0x03001150
	.2byte 0xa000
	.2byte 0x0202
.L_080ec9e0:
	ldr	r4, [pc, #100]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r4, #24]
.L_080ec9e8:
	ldr	r5, [sp, #40]
	movs	r0, #1
	negs	r0, r0
	cmp	r5, r0
	beq.n	.L_080ecaba
	ldr	r3, [pc, #88]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #7
	bhi.n	.L_080ecaba
	ldr	r1, [sp, #52]
	movs	r3, #13
	ldrb	r2, [r1, #5]
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #5]
	ldr	r2, [sp, #48]
	ldr	r3, [pc, #48]
	adds	r2, #3
	ands	r2, r3
	ldr	r3, [sp, #52]
	ldr	r4, [sp, #52]
	ldrh	r1, [r3, #8]
	ldr	r3, [pc, #52]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #8]
	ldr	r3, [pc, #48]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #0
	bne.n	.L_080eca70
	ldr	r0, [sp, #4]
	ldr	r1, [sp, #32]
	ldr	r3, [pc, #20]
	asrs	r2, r0, #16
	subs	r2, r1, r2
	subs	r2, #2
	ldrh	r1, [r4, #6]
	ands	r2, r3
	ldr	r3, [pc, #28]
	ands	r3, r1
	b.n	.L_080eca5c
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x0202a000
	.4byte 0x0300122c
	.4byte 0xfffffc00
	.4byte 0x0202a640
	.2byte 0xfe00
	.2byte 0xffff
.L_080eca5c:
	orrs	r3, r2
	ldr	r2, [sp, #52]
	strh	r3, [r2, #6]
	ldr	r4, [sp, #0]
	ldr	r5, [sp, #28]
	asrs	r3, r4, #16
	subs	r3, r5, r3
	subs	r3, #2
	strb	r3, [r2, #4]
	b.n	.L_080ecab2
.L_080eca70:
	ldr	r0, [sp, #32]
	movs	r5, #160
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r5, r5, #2
	adds	r1, r5, #0
	lsls	r0, r0, #2
	bl	sub_08002054
	ldr	r3, [pc, #60]
	ldr	r1, [sp, #52]
	subs	r0, #2
	ldrh	r2, [r1, #6]
	ands	r0, r3
	ldr	r3, [pc, #52]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #6]
	ldr	r4, [sp, #28]
	adds	r1, r5, #0
	lsls	r3, r4, #2
	adds	r3, r3, r4
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #2
	bl	sub_08002054
	ldr	r5, [sp, #52]
	subs	r0, #2
	strb	r0, [r5, #4]
.L_080ecab2:
	ldr	r0, [sp, #52]
	movs	r1, #246
	bl	sub_080140d8
.L_080ecaba:
	ldr	r0, [pc, #16]
	ldr	r3, [pc, #16]
	str	r0, [sp, #52]
	b.n	.L_080ecad4
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0202a620
	.2byte 0xa640
	.2byte 0x0202
.L_080ecad4:
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_080ecb18
	ldrb	r2, [r0, #7]
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r0, #7]
	ldr	r3, [sp, #4]
	mov	r4, sl
	asrs	r2, r3, #16
	ldr	r3, [pc, #32]
	subs	r2, r4, r2
	subs	r2, #17
	ldrh	r1, [r0, #6]
	ands	r2, r3
	ldr	r3, [pc, #28]
	ldr	r5, [sp, #52]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r0, [sp, #0]
	mov	r1, r8
	asrs	r3, r0, #16
	subs	r3, r1, r3
	adds	r3, #1
	strb	r3, [r5, #4]
	b.n	.L_080ecb5a
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_080ecb18:
	mov	r2, sl
	lsls	r3, r2, #2
	add	r3, sl
	lsls	r0, r3, #4
	movs	r5, #160
	subs	r0, r0, r3
	lsls	r5, r5, #2
	adds	r1, r5, #0
	lsls	r0, r0, #2
	bl	sub_08002054
	ldr	r3, [pc, #56]
	subs	r0, #17
	ands	r0, r3
	ldr	r3, [sp, #52]
	ldr	r4, [sp, #52]
	ldrh	r2, [r3, #6]
	ldr	r3, [pc, #48]
	adds	r1, r5, #0
	ands	r3, r2
	orrs	r3, r0
	mov	r0, r8
	strh	r3, [r4, #6]
	lsls	r3, r0, #2
	add	r3, r8
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #2
	bl	sub_08002054
	ldr	r1, [sp, #52]
	adds	r0, #1
	strb	r0, [r1, #4]
.L_080ecb5a:
	ldr	r0, [sp, #52]
	movs	r1, #246
	bl	sub_080140d8
	ldr	r4, [pc, #12]
	ldr	r5, [sp, #40]
	b.n	.L_080ecb74
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0xa000
	.2byte 0x0202
.L_080ecb74:
	movs	r2, #18
	ldrsh	r3, [r4, r2]
	cmp	r3, r5
	bne.n	.L_080ecb7e
	b.n	.L_080ecc8e
.L_080ecb7e:
	ldr	r5, [pc, #620]
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	cmp	r3, #0
	bne.n	.L_080ecb9a
	ldr	r0, [r4, #28]
	bl	sub_08038060
	ldr	r2, [pc, #608]
	movs	r3, #1
	strh	r3, [r5, #0]
	movs	r3, #6
	strh	r3, [r2, #0]
	b.n	.L_080ecd44
.L_080ecb9a:
	ldr	r2, [pc, #596]
	ldrh	r3, [r2, #0]
	subs	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	ble.n	.L_080ecbaa
	b.n	.L_080ecd44
.L_080ecbaa:
	add	r1, sp, #40
	ldr	r2, [pc, #580]
	ldrh	r1, [r1, #0]
	movs	r3, #0
	strh	r3, [r5, #0]
	strh	r1, [r2, #18]
	ldr	r2, [sp, #40]
	subs	r3, #1
	cmp	r2, r3
	bne.n	.L_080ecbc0
	b.n	.L_080ecd44
.L_080ecbc0:
	ldr	r4, [sp, #20]
	cmp	r4, r3
	bne.n	.L_080ecbcc
	ldr	r5, [pc, #560]
	str	r5, [sp, #20]
	b.n	.L_080ecbea
.L_080ecbcc:
	ldr	r0, [sp, #20]
	movs	r1, #2
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080ecbdc
	ldr	r2, [pc, #548]
	str	r2, [sp, #20]
	b.n	.L_080ecbea
.L_080ecbdc:
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #16]
	bl	sub_080ca280
	ldr	r3, [pc, #536]
	adds	r0, r0, r3
	str	r0, [sp, #20]
.L_080ecbea:
	add	r1, sp, #60
	add	r2, sp, #56
	ldr	r0, [sp, #20]
	bl	sub_080383a0
	ldr	r3, [pc, #524]
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	bne.n	.L_080ecc16
	ldr	r5, [sp, #32]
	ldr	r1, [sp, #4]
	ldr	r0, [sp, #28]
	ldr	r2, [sp, #0]
	asrs	r3, r1, #16
	subs	r5, #1
	subs	r5, r5, r3
	subs	r0, #11
	asrs	r3, r2, #16
	subs	r0, r0, r3
	mov	sl, r5
	b.n	.L_080ecc44
.L_080ecc16:
	ldr	r4, [sp, #32]
	movs	r5, #160
	lsls	r3, r4, #2
	adds	r3, r3, r4
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r5, r5, #2
	adds	r1, r5, #0
	lsls	r0, r0, #2
	bl	sub_08002054
	subs	r0, #1
	mov	sl, r0
	ldr	r0, [sp, #28]
	adds	r1, r5, #0
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #2
	bl	sub_08002054
	subs	r0, #11
.L_080ecc44:
	mov	r8, r0
	ldr	r4, [sp, #60]
	mov	r1, sl
	adds	r3, r1, r4
	cmp	r3, #239
	ble.n	.L_080ecc5c
	movs	r3, #232
	movs	r2, #10
	subs	r3, r3, r4
	negs	r2, r2
	mov	sl, r3
	add	r8, r2
.L_080ecc5c:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080ecc66
	movs	r4, #0
	mov	r8, r4
.L_080ecc66:
	ldr	r5, [pc, #396]
	ldr	r0, [sp, #20]
	ldr	r1, [r5, #28]
	mov	r2, sl
	mov	r3, r8
	bl	sub_08038078
	ldr	r3, [pc, #400]
	ldr	r2, [sp, #60]
	mov	r0, sl
	strh	r2, [r3, #0]
	ldr	r2, [sp, #56]
	adds	r3, #2
	strh	r2, [r3, #0]
	subs	r3, #6
	strh	r0, [r3, #0]
	mov	r1, r8
	adds	r3, #2
	strh	r1, [r3, #0]
	b.n	.L_080ecd44
.L_080ecc8e:
	ldr	r2, [sp, #40]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080ecd44
	ldr	r3, [pc, #364]
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	str	r3, [sp, #60]
	ldr	r3, [pc, #360]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	str	r3, [sp, #56]
	ldr	r3, [pc, #344]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L_080ecccc
	ldr	r1, [sp, #32]
	ldr	r4, [sp, #4]
	ldr	r2, [sp, #28]
	ldr	r5, [sp, #0]
	asrs	r3, r4, #16
	subs	r1, #1
	subs	r1, r1, r3
	subs	r2, #11
	asrs	r3, r5, #16
	subs	r2, r2, r3
	mov	sl, r1
	mov	r8, r2
	b.n	.L_080eccfc
.L_080ecccc:
	ldr	r0, [sp, #32]
	movs	r5, #160
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r5, r5, #2
	adds	r1, r5, #0
	lsls	r0, r0, #2
	bl	sub_08002054
	ldr	r1, [sp, #28]
	subs	r0, #1
	lsls	r3, r1, #2
	adds	r3, r3, r1
	mov	sl, r0
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #2
	adds	r1, r5, #0
	bl	sub_08002054
	subs	r0, #11
	mov	r8, r0
.L_080eccfc:
	ldr	r4, [sp, #60]
	mov	r2, sl
	adds	r3, r2, r4
	cmp	r3, #239
	ble.n	.L_080ecd12
	movs	r3, #232
	subs	r3, r3, r4
	mov	sl, r3
	movs	r3, #10
	negs	r3, r3
	add	r8, r3
.L_080ecd12:
	mov	r4, r8
	cmp	r4, #0
	bge.n	.L_080ecd1c
	movs	r5, #0
	mov	r8, r5
.L_080ecd1c:
	ldr	r7, [pc, #240]
	mov	r2, sl
	movs	r0, #0
	ldrsh	r1, [r7, r0]
	ldr	r5, [pc, #236]
	subs	r1, r2, r1
	str	r1, [sp, #60]
	mov	r4, r8
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #192]
	subs	r2, r4, r2
	ldr	r0, [r3, #28]
	str	r2, [sp, #56]
	bl	.L_080ec484
	mov	r4, sl
	mov	r0, r8
	strh	r4, [r7, #0]
	strh	r0, [r5, #0]
.L_080ecd44:
	ldr	r0, [pc, #208]
	ldr	r1, [pc, #212]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_080ecd76
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, #4
	lsls	r2, r2, #6
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080ecd76:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_080ecdac
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r0, #0]
	ldr	r5, [sp, #44]
	movs	r3, #16
	lsls	r2, r2, #2
	subs	r3, r3, r5
	adds	r2, r2, r0
	lsls	r3, r3, #8
	adds	r2, #4
	orrs	r3, r5
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_080ecdac:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_080ecdda
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	adds	r3, #4
	strh	r2, [r0, #0]
	movs	r2, #0
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #84
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080ecdda:
	strh	r4, [r1, #0]
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0202a654
	.4byte 0x0202a656
	.4byte 0x0202a000
	.4byte 0x00000e29
	.4byte 0x00000e2b
	.4byte 0x00000e58
	.4byte 0x0202a640
	.4byte 0x0202a63c
	.4byte 0x0202a63e
	.4byte 0x0202a638
	.4byte 0x0202a63a
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_080ece20:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #128]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #44
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r4, [pc, #120]
	ldmia	r3!, {r2}
	sub	sp, #8
	adds	r5, r2, r4
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #112]
	ldr	r0, [pc, #116]
	adds	r6, r3, r2
	movs	r3, #200
	lsls	r3, r3, #3
	adds	r1, r1, r3
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	cmp	r3, #0
	beq.n	.L_080ece5e
	ldr	r0, [pc, #100]
	movs	r5, #0
	movs	r6, #0
.L_080ece5e:
	bl	sub_08013300
	mov	r9, r0
	ldr	r0, [r0, #0]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	add	r0, r9
	lsls	r1, r1, #19
	adds	r2, #120
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #44]
	movs	r2, #160
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r3, #136
	movs	r2, #224
	lsls	r3, r3, #17
	lsls	r2, r2, #16
	cmp	r5, #0
	bge.n	.L_080ece92
	movs	r5, #0
.L_080ece92:
	cmp	r5, r3
	ble.n	.L_080ece98
	adds	r5, r3, #0
.L_080ece98:
	cmp	r6, #0
	bge.n	.L_080ece9e
	movs	r6, #0
.L_080ece9e:
	cmp	r6, r2
	ble.n	.L_080ecea4
	adds	r6, r2, #0
.L_080ecea4:
	ldr	r3, [pc, #28]
	asrs	r2, r5, #16
	b.n	.L_080ecec8
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0202a000
	.4byte 0xff880000
	.4byte 0xffb00000
	.4byte 0x00000024
	.4byte 0x00000023
	.2byte 0x1120
	.2byte 0x0300
.L_080ecec8:
	lsrs	r5, r5, #19
	strh	r2, [r3, #4]
	mov	r8, r5
	asrs	r2, r6, #16
	lsrs	r6, r6, #19
	strh	r2, [r3, #6]
	movs	r2, #31
	mov	r3, r8
	str	r6, [sp, #4]
	movs	r1, #0
	ands	r3, r2
	mov	sl, r1
	mov	fp, r3
.L_080ecee2:
	ldr	r7, [sp, #4]
	add	r7, sl
	adds	r2, r7, #0
	cmp	r7, #23
	ble.n	.L_080eceee
	subs	r7, #24
.L_080eceee:
	lsls	r3, r2, #5
	mov	r4, fp
	adds	r6, r4, r3
	adds	r0, r6, #0
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r0, #128
	str	r2, [sp, #0]
	bl	sub_08002064
	ldr	r2, [sp, #0]
	movs	r1, #128
	lsls	r3, r2, #2
	add	r3, r9
	lsls	r1, r1, #1
	adds	r6, r0, r1
	ldr	r2, [pc, #96]
	ldr	r0, [r3, #4]
	lsls	r1, r7, #12
	add	r0, r9
	adds	r1, r1, r2
	bl	sub_0801587c
	lsls	r5, r7, #6
	ldr	r3, [pc, #80]
	add	r5, r8
	movs	r7, #192
	lsls	r5, r5, #6
	movs	r4, #0
	lsls	r7, r7, #19
	adds	r5, r5, r3
.L_080ecf2c:
	movs	r3, #128
	movs	r2, #132
	lsls	r1, r6, #6
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	adds	r1, r1, r7
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r1, r8
	adds	r3, r4, r1
	adds	r5, #64
	adds	r6, #1
	cmp	r3, #31
	bne.n	.L_080ecf50
	subs	r6, #32
.L_080ecf50:
	adds	r4, #1
	cmp	r4, #30
	bls.n	.L_080ecf2c
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #20
	bls.n	.L_080ecee2
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0201
.L_080ecf74:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #160]
	mov	r8, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r6, r1, #0
	ldr	r0, [pc, #152]
	cmp	r3, #0
	beq.n	.L_080ecf90
	ldr	r0, [pc, #152]
.L_080ecf90:
	bl	sub_08013300
	mov	sl, r6
	adds	r7, r0, #0
	cmp	r6, #23
	ble.n	.L_080ecf9e
	subs	r6, #24
.L_080ecf9e:
	lsls	r3, r6, #6
	add	r3, r8
	mov	r2, r8
	mov	r9, r3
	movs	r3, #31
	ands	r3, r2
	lsls	r2, r6, #5
	adds	r5, r3, r2
	adds	r0, r5, #0
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r0, #128
	bl	sub_08002064
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r2, sl
	adds	r5, r0, r3
	lsls	r3, r2, #2
	adds	r3, r3, r7
	ldr	r0, [r3, #4]
	lsls	r1, r6, #12
	ldr	r6, [pc, #96]
	adds	r0, r7, r0
	adds	r1, r1, r6
	bl	sub_0801587c
	mov	r3, r8
	movs	r7, #0
	cmp	r3, #63
	bhi.n	.L_080ed016
	movs	r2, #192
	lsls	r2, r2, #19
	mov	ip, r2
	mov	r2, r9
	lsls	r3, r2, #6
	mov	r4, r8
	adds	r6, r3, r6
.L_080ecfea:
	movs	r3, #128
	movs	r2, #132
	lsls	r1, r5, #6
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r6, #0
	add	r1, ip
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #64
	adds	r5, #1
	cmp	r4, #31
	bne.n	.L_080ed00a
	subs	r5, #32
.L_080ed00a:
	adds	r7, #1
	adds	r4, #1
	cmp	r7, #30
	bhi.n	.L_080ed016
	cmp	r4, #63
	bls.n	.L_080ecfea
.L_080ed016:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0202a640
	.4byte 0x00000024
	.4byte 0x00000023
	.2byte 0x0000
	.2byte 0x0201
.L_080ed030:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #192
	adds	r7, r0, #0
	movs	r4, #31
	lsls	r2, r2, #2
	sub	sp, #4
	mov	r8, r1
	movs	r6, #0
	ands	r4, r7
	mov	sl, r2
.L_080ed04a:
	mov	r3, r8
	adds	r0, r3, r6
	adds	r5, r0, #0
	cmp	r0, #23
	ble.n	.L_080ed056
	subs	r5, #24
.L_080ed056:
	lsls	r0, r0, #5
	adds	r0, r4, r0
	mov	r1, sl
	adds	r0, #128
	str	r4, [sp, #0]
	bl	sub_08002064
	lsls	r5, r5, #6
	ldr	r2, [pc, #48]
	adds	r5, r7, r5
	lsls	r5, r5, #6
	adds	r1, r0, #0
	adds	r0, r5, r2
	ldr	r2, [pc, #40]
	lsls	r1, r1, #6
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #1
	ldr	r4, [sp, #0]
	cmp	r6, #20
	bls.n	.L_080ed04a
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.2byte 0x4000
	.2byte 0x0600
.L_080ed0a0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #460]
	ldr	r1, [pc, #460]
	ldr	r3, [r3, #0]
	sub	sp, #4
	ldmia	r3!, {r2}
	adds	r1, r1, r2
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #452]
	mov	sl, r1
	adds	r2, r2, r3
	movs	r3, #224
	mov	r8, r2
	lsls	r3, r3, #16
	cmp	r1, #0
	bge.n	.L_080ed0d0
	movs	r1, #0
	mov	sl, r1
.L_080ed0d0:
	movs	r2, #136
	lsls	r2, r2, #17
	cmp	sl, r2
	ble.n	.L_080ed0da
	mov	sl, r2
.L_080ed0da:
	mov	r1, r8
	cmp	r1, #0
	bge.n	.L_080ed0e4
	movs	r2, #0
	mov	r8, r2
.L_080ed0e4:
	cmp	r8, r3
	ble.n	.L_080ed0ea
	mov	r8, r3
.L_080ed0ea:
	ldr	r3, [pc, #412]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080ed0f6
	b.n	.L_080ed262
.L_080ed0f6:
	ldr	r2, [pc, #404]
	mov	r1, sl
	ldr	r3, [r2, #0]
	subs	r0, r1, r3
	cmp	r0, #0
	bge.n	.L_080ed10a
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r0, r0, r2
.L_080ed10a:
	ldr	r1, [pc, #388]
	asrs	r0, r0, #16
	ldr	r3, [r1, #0]
	mov	r2, r8
	mov	r9, r0
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080ed122
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080ed122:
	mov	r1, r9
	asrs	r6, r0, #16
	adds	r3, r6, #0
	muls	r3, r6
	mov	r0, r9
	muls	r0, r1
	adds	r0, r0, r3
	ldr	r3, [pc, #352]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a55
	mov	r1, sl
	ldr	r3, [r2, #0]
	adds	r2, #4
	subs	r1, r1, r3
	ldr	r3, [r2, #0]
	movs	r2, #128
	mov	r9, r1
	lsls	r7, r0, #16
	mov	r1, r8
	lsls	r2, r2, #15
	subs	r6, r1, r3
	cmp	r7, r2
	bge.n	.L_080ed174
	ldr	r3, [pc, #324]
	mov	r1, r9
	mov	fp, r3
	mov	r0, r9
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c31
	adds	r7, r0, #0
	adds	r0, r6, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x183f
	adds	r0, r7, #0
	str	r7, [sp, #0]
	bl	sub_080149e0
	adds	r7, r0, #0
.L_080ed174:
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L_080ed17c
	adds	r1, r7, #3
.L_080ed17c:
	movs	r3, #128
	asrs	r5, r1, #2
	lsls	r3, r3, #12
	cmp	r5, r3
	ble.n	.L_080ed188
	adds	r5, r3, #0
.L_080ed188:
	movs	r1, #128
	lsls	r1, r1, #7
	cmp	r7, r1
	blt.n	.L_080ed1f4
	cmp	r7, r5
	ble.n	.L_080ed1bc
	ldr	r2, [pc, #260]
	mov	r1, r9
	mov	sl, r2
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x4b3d
	adds	r1, r5, #0
	mov	r8, r3
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	mov	r9, r0
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1c29
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c06
.L_080ed1bc:
	ldr	r1, [pc, #204]
	ldr	r2, [pc, #208]
	ldr	r3, [r1, #0]
	add	r3, r9
	mov	sl, r3
	ldr	r3, [r2, #0]
	adds	r3, r3, r6
	mov	r8, r3
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080ed1d6
	movs	r1, #0
	mov	sl, r1
.L_080ed1d6:
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_080ed1e0
	movs	r3, #0
	mov	r8, r3
.L_080ed1e0:
	movs	r1, #136
	lsls	r1, r1, #17
	cmp	sl, r1
	ble.n	.L_080ed1ea
	mov	sl, r1
.L_080ed1ea:
	movs	r2, #136
	lsls	r2, r2, #17
	cmp	r8, r2
	ble.n	.L_080ed1f4
	mov	r8, r2
.L_080ed1f4:
	mov	r3, sl
	lsrs	r6, r3, #19
	ldr	r3, [pc, #144]
	mov	r1, r8
	lsrs	r5, r1, #19
	ldr	r1, [r3, #0]
	mov	r2, sl
	adds	r3, r1, #0
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed228
	cmp	r1, sl
	bge.n	.L_080ed220
	adds	r0, r6, #0
	adds	r0, #30
	adds	r1, r5, #0
	bl	.L_080ed030
	b.n	.L_080ed228
.L_080ed220:
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	.L_080ed030
.L_080ed228:
	ldr	r3, [pc, #100]
	mov	r2, r8
	ldr	r1, [r3, #0]
	adds	r3, r1, #0
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed254
	cmp	r1, r8
	bge.n	.L_080ed24c
	adds	r1, r5, #0
	adds	r1, #20
	adds	r0, r6, #0
	bl	.L_080ecf74
	b.n	.L_080ed254
.L_080ed24c:
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	.L_080ecf74
.L_080ed254:
	ldr	r3, [pc, #72]
	mov	r1, sl
	asrs	r2, r1, #16
	mov	r1, r8
	strh	r2, [r3, #4]
	asrs	r2, r1, #16
	strh	r2, [r3, #6]
.L_080ed262:
	ldr	r3, [pc, #40]
	mov	r2, sl
	str	r2, [r3, #0]
	mov	r1, r8
	adds	r3, #4
	str	r1, [r3, #0]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0202a62c
	.4byte 0xff880000
	.4byte 0xffb00000
	.4byte 0x0202a640
	.4byte 0x0202a630
	.4byte 0x0202a634
	.4byte 0x030002d4
	.4byte 0x0300021c
	.4byte 0x03000230
	.2byte 0x1120
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r0, [sp, #28]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r4, [pc, #172]
	str	r1, [sp, #20]
	add	r6, sp, #28
	ldr	r2, [r3, #108]
	mov	fp, r4
	str	r2, [sp, #16]
	ldr	r3, [r3, #24]
	str	r3, [sp, #12]
	movs	r3, #0
	str	r3, [sp, #4]
	str	r3, [sp, #0]
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #3
	ldrh	r6, [r6, #0]
	adds	r3, #66
	add	r3, fp
	strh	r6, [r3, #0]
	ldr	r1, [sp, #28]
	cmp	r1, #0
	bne.n	.L_080ed2f6
	movs	r4, #197
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_080ed30c
.L_080ed2f6:
	bl	sub_080cad9c
	ldr	r6, [sp, #16]
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080ed370
.L_080ed30c:
	ldr	r3, [sp, #16]
	movs	r4, #218
	lsls	r4, r4, #1
	adds	r2, r3, r4
	ldr	r6, [r2, #0]
	movs	r3, #6
	str	r6, [sp, #24]
	str	r3, [r2, #0]
	bl	sub_080d2a64
	bl	sub_080d2a8c
	ldr	r1, [sp, #20]
	mov	r0, sp
	adds	r1, #24
	movs	r4, #1
	add	r2, sp, #40
	adds	r0, #55
.L_080ed330:
	ldrh	r3, [r1, #10]
	strh	r4, [r1, #10]
	strb	r3, [r2, #0]
	adds	r2, #1
	adds	r1, #12
	cmp	r2, r0
	ble.n	.L_080ed330
	ldr	r1, [sp, #12]
	movs	r3, #1
	strh	r3, [r1, #4]
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #144
	ldr	r2, [sp, #20]
	ldr	r5, [pc, #24]
	lsls	r4, r4, #4
	adds	r4, #114
	adds	r3, r2, r4
	strb	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #8]
	b.n	.L_080ed436
	.4byte 0x00000001
	.2byte 0xa000
	.2byte 0x0202
.L_080ed370:
	ldr	r6, [sp, #16]
	movs	r1, #230
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	movs	r5, #1
	adds	r3, #91
	strb	r5, [r3, #0]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldr	r4, [r2, #0]
	subs	r1, #32
	movs	r3, #6
	str	r4, [sp, #24]
	str	r3, [r2, #0]
	adds	r2, r6, r1
	movs	r3, #0
	str	r3, [r2, #0]
	bl	sub_080d2a64
	bl	sub_080d2a8c
	bl	sub_080144c0
	bl	sub_08038000
	ldr	r2, [sp, #12]
	movs	r0, #1
	strh	r5, [r2, #4]
	bl	sub_08013560
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	ldrh	r3, [r3, #0]
	movs	r4, #128
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #8]
	ldr	r0, [pc, #720]
	ldr	r2, [pc, #724]
	lsls	r4, r4, #10
	movs	r1, #0
	adds	r4, #2
.L_080ed3ca:
	movs	r3, #15
.L_080ed3cc:
	subs	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L_080ed3cc
	adds	r1, #1
	cmp	r1, #19
	ble.n	.L_080ed3ca
	movs	r4, #128
	ldr	r2, [pc, #700]
	lsls	r4, r4, #10
	movs	r1, #0
	adds	r4, #2
.L_080ed3e6:
	movs	r3, #15
.L_080ed3e8:
	subs	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L_080ed3e8
	adds	r1, #1
	cmp	r1, #23
	ble.n	.L_080ed3e6
	movs	r4, #128
	ldr	r2, [pc, #672]
	lsls	r4, r4, #10
	movs	r1, #0
	adds	r4, #2
.L_080ed402:
	movs	r3, #15
.L_080ed404:
	subs	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L_080ed404
	adds	r1, #1
	cmp	r1, #3
	ble.n	.L_080ed402
	ldr	r4, [sp, #20]
	ldr	r2, [pc, #648]
	movs	r3, #0
	str	r3, [r4, #0]
	strh	r3, [r2, #4]
	strh	r3, [r2, #6]
	movs	r2, #192
	movs	r3, #128
	lsls	r2, r2, #3
	lsls	r3, r3, #19
	adds	r2, #130
	adds	r3, #10
	strh	r2, [r3, #0]
	movs	r2, #154
	lsls	r2, r2, #5
	subs	r3, #10
	strh	r2, [r3, #0]
.L_080ed436:
	bl	sub_080ebf94
.L_080ed43a:
	mov	r6, sp
	ldr	r3, [pc, #612]
	ldrh	r6, [r6, #0]
	strh	r6, [r3, #0]
	bl	.L_080ece20
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #134
	movs	r3, #128
	lsls	r2, r2, #8
	lsls	r3, r3, #19
	adds	r2, #130
	adds	r3, #10
	strh	r2, [r3, #0]
	movs	r2, #154
	lsls	r2, r2, #5
	subs	r3, #10
	strh	r2, [r3, #0]
	movs	r0, #142
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ed47a
	movs	r0, #143
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ed48c
.L_080ed47a:
	ldr	r1, [sp, #4]
	cmp	r1, #0
	bne.n	.L_080ed48c
	ldr	r0, [pc, #548]
	movs	r1, #1
	bl	sub_08038040
	movs	r2, #1
	str	r2, [sp, #4]
.L_080ed48c:
	ldr	r3, [pc, #540]
	ldr	r4, [pc, #544]
	ldr	r6, [pc, #544]
	ldr	r7, [pc, #548]
	mov	sl, r3
	mov	r8, r4
.L_080ed498:
	bl	.L_080ed0a0
	mov	r5, sl
	bl	.L_080ec4d4
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [r5, #12]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ed4d2
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_080ed4d2
	add	r0, sp, #36
	add	r1, sp, #32
	bl	.L_080ec1d0
	ldr	r3, [sp, #36]
	ldr	r5, [pc, #488]
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [sp, #32]
	movs	r2, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	mov	r9, r2
.L_080ed4d2:
	mov	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080ed4f6
	mov	r4, r9
	cmp	r4, #1
	bne.n	.L_080ed4f6
	mov	r1, r8
	ldr	r3, [r1, #0]
	ldr	r5, [pc, #448]
	str	r3, [r6, #0]
	ldr	r3, [pc, #460]
	movs	r2, #0
	ldr	r3, [r3, #0]
	mov	r9, r2
	str	r3, [r7, #0]
.L_080ed4f6:
	mov	r3, sl
	ldr	r2, [r3, #12]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ed526
	movs	r2, #1
	add	r0, sp, #36
	add	r1, sp, #32
	bl	.L_080ec2b8
	ldr	r3, [sp, #36]
	mov	r4, r8
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [sp, #32]
	ldr	r2, [pc, #416]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r6, #0]
	str	r3, [r4, #0]
	ldr	r3, [r7, #0]
	str	r3, [r2, #0]
.L_080ed526:
	ldr	r2, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ed556
	movs	r2, #1
	add	r1, sp, #32
	negs	r2, r2
	add	r0, sp, #36
	bl	.L_080ec2b8
	ldr	r3, [sp, #36]
	mov	r1, r8
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [sp, #32]
	ldr	r2, [pc, #368]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r6, #0]
	str	r3, [r1, #0]
	ldr	r3, [r7, #0]
	str	r3, [r2, #0]
.L_080ed556:
	ldr	r2, [r5, #12]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ed5d2
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r2, fp
	adds	r3, #255
	strh	r3, [r2, #18]
	ldr	r0, [r2, #28]
	bl	sub_08038060
	movs	r2, #64
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	movs	r3, #1
	ldr	r4, [sp, #0]
	eors	r4, r3
	str	r4, [sp, #0]
	ldr	r2, [r6, #0]
	cmp	r2, #0
	bne.n	.L_080ed588
	b.n	.L_080ed43a
.L_080ed588:
	mov	r6, fp
	str	r2, [r6, #4]
	ldr	r3, [r7, #0]
	ldr	r1, [pc, #304]
	str	r3, [r6, #8]
	ldr	r6, [pc, #304]
	movs	r0, #136
	adds	r3, r2, r6
	str	r3, [r1, #0]
	mov	r6, fp
	ldr	r3, [r6, #8]
	ldr	r6, [pc, #296]
	ldr	r2, [pc, #296]
	adds	r3, r3, r6
	str	r3, [r2, #0]
	movs	r5, #224
	ldr	r3, [r1, #0]
	movs	r4, #0
	lsls	r0, r0, #17
	lsls	r5, r5, #16
	cmp	r3, #0
	bge.n	.L_080ed5b8
	str	r4, [r1, #0]
	movs	r3, #0
.L_080ed5b8:
	cmp	r3, r0
	ble.n	.L_080ed5be
	str	r0, [r1, #0]
.L_080ed5be:
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_080ed5c8
	str	r4, [r2, #0]
	movs	r3, #0
.L_080ed5c8:
	cmp	r3, r5
	bgt.n	.L_080ed5ce
	b.n	.L_080ed43a
.L_080ed5ce:
	str	r5, [r2, #0]
	b.n	.L_080ed43a
.L_080ed5d2:
	ldr	r2, [r5, #12]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ed628
	ldr	r1, [sp, #28]
	cmp	r1, #0
	beq.n	.L_080ed654
	mov	r4, fp
	movs	r2, #18
	ldrsh	r3, [r4, r2]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_080ed5f2
	b.n	.L_080ed498
.L_080ed5f2:
	bl	.L_080ed804
	mov	r4, fp
	movs	r3, #18
	ldrsh	r2, [r4, r3]
	ldr	r6, [pc, #208]
	lsls	r3, r2, #2
	movs	r1, #128
	adds	r3, r3, r2
	lsls	r1, r1, #2
	ldr	r2, [pc, #204]
	lsls	r3, r3, #2
	adds	r1, #190
	adds	r0, r0, r3
	adds	r3, r6, r1
	strh	r2, [r3, #0]
	ldrb	r3, [r0, #0]
	ldr	r2, [pc, #192]
	movs	r6, #18
	ldrsh	r1, [r0, r6]
	strh	r3, [r2, #0]
	movs	r4, #16
	ldrsh	r3, [r0, r4]
	adds	r0, r3, #0
	bl	sub_080ca368
	b.n	.L_080ed654
.L_080ed628:
	ldr	r2, [r5, #12]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080ed634
	b.n	.L_080ed498
.L_080ed634:
	ldr	r1, [sp, #28]
	cmp	r1, #0
	beq.n	.L_080ed654
	ldr	r2, [pc, #148]
	movs	r4, #128
	lsls	r4, r4, #2
	adds	r4, #190
	adds	r3, r2, r4
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	ldr	r6, [pc, #140]
	strh	r2, [r3, #0]
	movs	r3, #1
	negs	r3, r3
	strh	r3, [r6, #0]
.L_080ed654:
	bl	sub_080ec14c
	movs	r2, #64
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	ldr	r1, [sp, #28]
	cmp	r1, #0
	bne.n	.L_080ed678
	ldr	r2, [sp, #16]
	movs	r4, #197
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_080ed6dc
.L_080ed678:
	ldr	r6, [sp, #16]
	movs	r1, #172
	movs	r2, #186
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	adds	r3, r6, r1
	adds	r2, #255
	movs	r0, #10
	strh	r2, [r3, #0]
	adds	r0, #255
	bl	sub_08016cfc
	b.n	.L_080ed778
	movs	r0, r0
	.4byte 0x06003000
	.4byte 0x01810180
	.4byte 0x01010100
	.4byte 0x03001120
	.4byte 0x0202a640
	.4byte 0x00000e2a
	.4byte 0x03001150
	.4byte 0x0202a64c
	.4byte 0x0202a644
	.4byte 0x0202a648
	.4byte 0x0202a650
	.4byte 0x0202a630
	.4byte 0xff880000
	.4byte 0xffb00000
	.4byte 0x0202a634
	.4byte 0x02000240
	.4byte 0x00000002
	.2byte 0x0500
	.2byte 0x0200
.L_080ed6dc:
	bl	sub_08020270
	ldr	r1, [pc, #136]
	ldr	r0, [pc, #140]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_080ed714
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	ldr	r6, [sp, #8]
	lsls	r2, r2, #2
	adds	r2, r2, r1
	lsls	r3, r6, #16
	adds	r2, #4
	lsrs	r3, r3, #16
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_080ed714:
	strh	r4, [r0, #0]
	ldr	r1, [sp, #20]
	mov	r0, sp
	adds	r1, #24
	add	r2, sp, #40
	adds	r0, #55
.L_080ed720:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strh	r3, [r1, #10]
	adds	r1, #12
	cmp	r2, r0
	ble.n	.L_080ed720
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #68]
	movs	r3, #0
	strh	r3, [r1, #4]
	strh	r3, [r2, #4]
	strh	r3, [r2, #6]
	ldr	r2, [sp, #20]
	movs	r4, #144
	ldr	r5, [pc, #40]
	lsls	r4, r4, #4
	adds	r4, #114
	adds	r3, r2, r4
	strb	r5, [r3, #0]
	bl	sub_080d2a3c
	bl	sub_080d2a8c
	ldr	r6, [sp, #16]
	ldr	r2, [sp, #24]
	movs	r1, #218
	lsls	r1, r1, #1
	movs	r4, #230
	adds	r3, r6, r1
	lsls	r4, r4, #1
	str	r2, [r3, #0]
	adds	r3, r6, r4
	ldr	r3, [r3, #0]
	adds	r3, #91
	strb	r5, [r3, #0]
	b.n	.L_080ed778
	.4byte 0x00000000
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x1120
	.2byte 0x0300
.L_080ed778:
	movs	r0, #0
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080ed7f8
	bl	sub_080d2260
	movs	r0, #157
	lsls	r0, r0, #8
	movs	r1, #6
	adds	r0, #137
	bl	sub_080d46a4
	ldr	r1, [pc, #72]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed7d0
	adds	r6, r1, #0
	adds	r5, r2, #0
.L_080ed7c2:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_080ed7c2
.L_080ed7d0:
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080ed7ee
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #6
	bl	sub_080d46a4
	b.n	.L_080ed7f8
.L_080ed7ee:
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #6
	bl	sub_080d46a4
.L_080ed7f8:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x03001150
	.2byte 0x0240
	.2byte 0x0200
.L_080ed804:
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x080f1140
