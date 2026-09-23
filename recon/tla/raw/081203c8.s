.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0800205c, 0x0800205c
	.set sub_08013164, 0x08013164
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038130, 0x08038130
	.set sub_080ad008, 0x080ad008
	.set sub_080ad078, 0x080ad078
	.set sub_080ad0c8, 0x080ad0c8
	.set sub_080ad0d0, 0x080ad0d0
	.set sub_080ad0e8, 0x080ad0e8
	.set sub_080ad120, 0x080ad120
	.set sub_080ad128, 0x080ad128
	.set sub_080ad130, 0x080ad130
	.set sub_080ad138, 0x080ad138
	.set sub_080ad148, 0x080ad148
	.set sub_080ad280, 0x080ad280
	.set sub_0811a12c, 0x0811a12c
	.set sub_0811a44c, 0x0811a44c
	.set sub_0811a490, 0x0811a490
	.set sub_0811a7ac, 0x0811a7ac
	.set sub_0811b294, 0x0811b294
	.set sub_0811b2c4, 0x0811b2c4
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811be68, 0x0811be68
	.set sub_08120360, 0x08120360
	.set sub_08120388, 0x08120388
	.set sub_081203a8, 0x081203a8
	.set sub_08125390, 0x08125390
	.set sub_08127adc, 0x08127adc
	.set sub_08127c2c, 0x08127c2c
	.set sub_08127c80, 0x08127c80
	.global Func_081203c8
	.thumb_func
Func_081203c8:
.L_081203c8:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	movs	r0, #1
	movs	r3, #0
	movs	r4, #0
	sub	sp, #8
	negs	r0, r0
	mov	ip, r1
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	movs	r6, #0
	adds	r1, r0, #0
	movs	r4, #0
	adds	r5, r7, #0
.L_081203e4:
	ldrh	r3, [r5, #0]
	adds	r5, #2
	cmp	r3, #254
	bne.n	.L_08120412
	cmp	r1, #0
	blt.n	.L_0812040e
	cmp	r0, #0
	blt.n	.L_0812040e
	subs	r2, r4, r1
	cmp	r2, #0
	bge.n	.L_081203fc
	subs	r2, r1, r4
.L_081203fc:
	subs	r3, r0, r1
	cmp	r3, #0
	blt.n	.L_08120408
	cmp	r2, r3
	blt.n	.L_0812040e
	b.n	.L_0812042c
.L_08120408:
	subs	r3, r1, r0
	cmp	r2, r3
	bge.n	.L_0812042c
.L_0812040e:
	adds	r0, r4, #0
	b.n	.L_0812042c
.L_08120412:
	cmp	r3, #255
	beq.n	.L_08120432
	cmp	r3, #128
	bne.n	.L_08120420
	movs	r3, #1
	eors	r6, r3
	adds	r1, r4, #0
.L_08120420:
	mov	r3, sp
	lsls	r2, r6, #2
	adds	r2, r2, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0812042c:
	adds	r4, #1
	cmp	r4, #5
	ble.n	.L_081203e4
.L_08120432:
	cmp	r0, #0
	blt.n	.L_0812043e
	lsls	r3, r0, #1
	mov	r2, ip
	strh	r2, [r3, r7]
	b.n	.L_0812044a
.L_0812043e:
	lsls	r3, r4, #1
	adds	r3, r3, r7
	mov	r2, ip
	strh	r2, [r3, #0]
	ldr	r2, [pc, #8]
	strh	r2, [r3, #2]
.L_0812044a:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #112
	str	r0, [sp, #92]
	movs	r0, #0
	str	r1, [sp, #88]
	str	r0, [sp, #68]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	str	r0, [sp, #56]
	str	r0, [sp, #48]
	str	r0, [sp, #44]
	str	r0, [sp, #32]
	str	r0, [sp, #24]
	movs	r0, #166
	lsls	r0, r0, #1
	str	r3, [sp, #64]
	bl	sub_08014d78
	movs	r1, #156
	lsls	r1, r1, #1
	str	r0, [sp, #20]
	cmp	fp, r1
	beq.n	.L_08120498
	movs	r2, #158
	lsls	r2, r2, #1
	cmp	fp, r2
	bne.n	.L_081204a0
.L_08120498:
	ldr	r3, [sp, #92]
	ldrb	r3, [r3, #2]
	str	r3, [sp, #76]
	b.n	.L_081204a6
.L_081204a0:
	ldr	r4, [sp, #92]
	ldrb	r4, [r4, #0]
	str	r4, [sp, #76]
.L_081204a6:
	ldr	r1, [sp, #92]
	ldr	r6, [sp, #92]
	ldr	r1, [r1, #80]
	ldr	r5, [sp, #88]
	ldr	r0, [sp, #92]
	ldr	r3, [sp, #88]
	adds	r6, #3
	ldrb	r5, [r6, r5]
	ldr	r0, [r0, #76]
	str	r1, [sp, #72]
	adds	r3, #28
	ldrsb	r3, [r6, r3]
	mov	sl, r5
	str	r3, [sp, #52]
	ldr	r5, [sp, #92]
	ldr	r3, [sp, #88]
	adds	r5, #1
	adds	r3, #44
	ldrsb	r3, [r5, r3]
	mov	fp, r0
	str	r3, [sp, #36]
	bl	sub_080ad078
	str	r0, [sp, #84]
	ldr	r0, [sp, #76]
	bl	sub_08016ca4
	str	r0, [sp, #80]
	mov	r0, sl
	bl	sub_08016ca4
	movs	r2, #166
	mov	r8, r0
	ldr	r3, [pc, #720]
	lsls	r2, r2, #1
	ldr	r0, [sp, #20]
	mov	r1, r8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23a5
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r2, [sp, #92]
	ldrh	r3, [r3, #0]
	adds	r2, #74
	str	r2, [sp, #4]
	cmp	r3, #101
	blt.n	.L_081205a0
	cmp	r3, #103
	ble.n	.L_0812053a
	cmp	r3, #221
	bne.n	.L_08120582
	adds	r3, r2, #0
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #1
	bne.n	.L_081205a0
	ldr	r3, [sp, #76]
	mov	r0, sl
	eors	r3, r0
	lsrs	r3, r3, #7
	cmp	r3, #0
	beq.n	.L_081205a0
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #648]
	bl	.L_08122326
.L_0812053a:
	movs	r1, #159
	lsls	r1, r1, #1
	adds	r1, #255
	cmp	fp, r1
	beq.n	.L_0812058a
	ldr	r2, [sp, #64]
	movs	r4, #128
	lsls	r4, r4, #4
	adds	r4, #104
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08120592
	ldr	r3, [sp, #76]
	mov	r0, sl
	eors	r3, r0
	lsrs	r3, r3, #7
	cmp	r3, #0
	beq.n	.L_0812059a
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #592]
	movs	r0, #4
	bl	sub_08120360
	ldr	r1, [sp, #92]
	adds	r1, #74
	str	r1, [sp, #4]
	bl	.L_081223ba
.L_08120582:
	ldr	r2, [sp, #92]
	adds	r2, #74
	str	r2, [sp, #4]
	b.n	.L_081205a0
.L_0812058a:
	ldr	r3, [sp, #92]
	adds	r3, #74
	str	r3, [sp, #4]
	b.n	.L_081205a0
.L_08120592:
	ldr	r4, [sp, #92]
	adds	r4, #74
	str	r4, [sp, #4]
	b.n	.L_081205a0
.L_0812059a:
	ldr	r0, [sp, #92]
	adds	r0, #74
	str	r0, [sp, #4]
.L_081205a0:
	movs	r3, #44
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #4
	bne.n	.L_081205cc
	ldr	r3, [sp, #76]
	mov	r1, sl
	eors	r3, r1
	lsrs	r3, r3, #7
	cmp	r3, #0
	beq.n	.L_081205cc
	movs	r0, #11
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #512]
	bl	.L_08122326
.L_081205cc:
	ldr	r2, [sp, #84]
	ldrb	r3, [r2, #8]
	cmp	r3, #255
	beq.n	.L_081205e4
	ldr	r3, [sp, #88]
	adds	r3, #16
	ldrsb	r3, [r5, r3]
	str	r3, [sp, #60]
	cmp	r3, #0
	bge.n	.L_081205e8
	negs	r3, r3
	b.n	.L_081205e6
.L_081205e4:
	movs	r3, #0
.L_081205e6:
	str	r3, [sp, #60]
.L_081205e8:
	ldr	r4, [sp, #72]
	cmp	r4, #4
	beq.n	.L_08120646
	lsls	r3, r4, #2
	mov	r4, r8
	add	r3, r8
	adds	r4, #36
	movs	r5, #38
	ldrsh	r0, [r3, r5]
	movs	r2, #2
	ldrsh	r3, [r4, r2]
	movs	r1, #0
	cmp	r0, r3
	blt.n	.L_08120616
	adds	r2, r4, #0
.L_08120606:
	adds	r1, #1
	adds	r2, #4
	cmp	r1, #3
	bgt.n	.L_08120616
	movs	r5, #2
	ldrsh	r3, [r2, r5]
	cmp	r0, r3
	bge.n	.L_08120606
.L_08120616:
	cmp	r1, #4
	bne.n	.L_08120620
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #24]
.L_08120620:
	movs	r2, #2
	ldrsh	r3, [r4, r2]
	movs	r1, #0
	cmp	r0, r3
	bgt.n	.L_0812063e
	mov	r2, r8
	adds	r2, #36
.L_0812062e:
	adds	r1, #1
	adds	r2, #4
	cmp	r1, #3
	bgt.n	.L_0812063e
	movs	r4, #2
	ldrsh	r3, [r2, r4]
	cmp	r0, r3
	ble.n	.L_0812062e
.L_0812063e:
	cmp	r1, #4
	bne.n	.L_08120646
	movs	r5, #1
	str	r5, [sp, #24]
.L_08120646:
	ldr	r0, [sp, #92]
	ldr	r2, [r0, #80]
	cmp	r2, #3
	bhi.n	.L_08120664
	ldr	r4, [sp, #4]
	movs	r1, #0
	ldrsh	r3, [r4, r1]
	cmp	r3, #2
	beq.n	.L_08120664
	ldr	r5, [sp, #80]
	lsls	r3, r2, #2
	adds	r3, #72
	ldrsh	r5, [r5, r3]
	str	r5, [sp, #16]
	b.n	.L_08120668
.L_08120664:
	movs	r1, #100
	str	r1, [sp, #16]
.L_08120668:
	ldr	r5, [sp, #4]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	cmp	r3, #5
	bne.n	.L_081206ac
	cmp	r2, #3
	bhi.n	.L_081206ac
	ldr	r0, [sp, #24]
	cmp	r0, #0
	ble.n	.L_081206ac
	lsls	r3, r2, #2
	adds	r3, #72
	add	r3, r8
	movs	r1, #2
	ldrsh	r5, [r3, r1]
	ldr	r2, [sp, #16]
	movs	r3, #200
	subs	r5, r2, r5
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r5, #30
	muls	r5, r3
	bl	sub_080ad148
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r0, r3
	cmp	r5, r0
	ble.n	.L_081206ac
	movs	r0, #13
	movs	r1, #5
	bl	sub_08120360
.L_081206ac:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #174
	cmp	fp, r3
	beq.n	.L_081206be
	movs	r4, #102
	adds	r4, #255
	cmp	fp, r4
	bne.n	.L_081206c8
.L_081206be:
	mov	r0, sl
	movs	r1, #0
	movs	r2, #0
	bl	sub_08125390
.L_081206c8:
	ldr	r3, [pc, #256]
	add	r3, fp
	cmp	r3, #1
	bhi.n	.L_081206da
	mov	r0, sl
	movs	r1, #1
	movs	r2, #0
	bl	sub_08125390
.L_081206da:
	movs	r5, #182
	lsls	r5, r5, #2
	cmp	fp, r5
	bne.n	.L_081206ec
	mov	r0, sl
	movs	r1, #1
	movs	r2, #1
	bl	sub_08125390
.L_081206ec:
	ldr	r0, [sp, #84]
	movs	r1, #15
	ldrb	r3, [r0, #1]
	movs	r2, #1
	ands	r1, r3
	ldr	r3, [sp, #88]
	str	r1, [sp, #28]
	adds	r3, #56
	ldrsb	r0, [r6, r3]
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_0812071a
	ldr	r2, [pc, #200]
	ldr	r5, [sp, #60]
	ldr	r4, [sp, #84]
	ldrb	r2, [r2, r5]
	ldrb	r3, [r4, #3]
	ldr	r0, [sp, #76]
	str	r2, [sp, #0]
	mov	r1, sl
	ldr	r2, [sp, #72]
	bl	sub_080ad120
.L_0812071a:
	str	r0, [sp, #40]
	ldr	r0, [sp, #84]
	movs	r2, #128
	ldrb	r1, [r0, #3]
	lsls	r2, r2, #17
	adds	r3, r1, #0
	adds	r3, #206
	lsls	r3, r3, #24
	cmp	r3, r2
	bls.n	.L_0812073a
	adds	r3, r1, #0
	cmp	r3, #86
	beq.n	.L_0812073a
	cmp	r3, #87
	beq.n	.L_0812073a
	b.n	.L_08120964
.L_0812073a:
	ldr	r3, [sp, #80]
	movs	r4, #165
	lsls	r4, r4, #1
	adds	r6, r3, r4
	ldrh	r5, [r6, #0]
	bl	sub_0811b294
	mov	r9, r0
	ldr	r0, [sp, #84]
	movs	r7, #1
	ldrb	r3, [r0, #3]
	negs	r7, r7
	cmp	r3, #51
	bne.n	.L_08120762
	ldr	r1, [sp, #64]
	ldr	r0, [r1, #0]
	bl	sub_08127c80
	adds	r5, r0, #0
	b.n	.L_081207d8
.L_08120762:
	cmp	r3, #86
	bne.n	.L_08120780
	ldrh	r3, [r6, #0]
	cmp	r3, #164
	bne.n	.L_0812077c
	bl	sub_080ad148
	movs	r3, #3
	movs	r2, #189
	ands	r3, r0
	lsls	r2, r2, #1
	adds	r5, r3, r2
	b.n	.L_081207d8
.L_0812077c:
	movs	r5, #81
	b.n	.L_081207d8
.L_08120780:
	cmp	r3, #87
	bne.n	.L_081207d8
	ldr	r4, [sp, #64]
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #107
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_081207d4
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #106
	adds	r3, r4, r1
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r4, #160
	ldr	r0, [sp, #64]
	lsls	r4, r4, #3
	lsls	r3, r2, #1
	adds	r4, #100
	subs	r1, #2
	adds	r3, r3, r4
	adds	r2, r2, r1
	ldrh	r5, [r0, r3]
	ldrb	r7, [r0, r2]
	b.n	.L_081207d8
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x00000cb5
	.4byte 0x00000cb4
	.4byte 0x00000cab
	.4byte 0xfffffd30
	.2byte 0x8898
	.2byte 0x0812
.L_081207d4:
	movs	r2, #0
	str	r2, [sp, #40]
.L_081207d8:
	ldr	r3, [sp, #40]
	cmp	r3, #0
	bne.n	.L_081207e0
	b.n	.L_08120942
.L_081207e0:
	adds	r0, r5, #0
	bl	sub_0811a490
	cmp	r0, #0
	bne.n	.L_081207ec
	b.n	.L_08120942
.L_081207ec:
	mov	r4, r9
	cmp	r4, #0
	bge.n	.L_081207f4
	b.n	.L_08120942
.L_081207f4:
	movs	r0, #1
	negs	r0, r0
	cmp	r7, r0
	bne.n	.L_08120816
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08127adc
	movs	r3, #128
	adds	r7, r0, #0
	lsls	r3, r3, #8
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08120816
	adds	r0, r5, #0
	bl	sub_08127c2c
.L_08120816:
	movs	r2, #254
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r1, r5, #0
	ands	r2, r7
	mov	r0, r9
	bl	sub_080ad0e8
	ldr	r1, [sp, #84]
	ldrb	r3, [r1, #3]
	cmp	r3, #87
	bne.n	.L_0812085a
	ldr	r3, [sp, #64]
	movs	r4, #160
	lsls	r4, r4, #3
	adds	r4, #107
	adds	r2, r3, r4
	ldrb	r3, [r2, #0]
	movs	r0, #160
	subs	r3, #1
	strb	r3, [r2, #0]
	ldr	r5, [sp, #64]
	lsls	r0, r0, #3
	adds	r0, #106
	adds	r1, r5, r0
	movs	r2, #0
	ldrsb	r2, [r1, r2]
	adds	r2, #1
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	strb	r2, [r1, #0]
.L_0812085a:
	ldr	r1, [sp, #80]
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #164
	bne.n	.L_08120874
	ldr	r0, [sp, #64]
	mov	r1, r9
	adds	r0, #102
	bl	.L_081203c8
	b.n	.L_081208b8
.L_08120874:
	ldr	r1, [sp, #64]
	movs	r2, #100
	adds	r1, #2
	ldrsh	r3, [r1, r2]
	movs	r5, #0
	movs	r6, #0
	adds	r7, r1, #0
	movs	r0, #100
	movs	r4, #0
	cmp	r3, #254
	bne.n	.L_0812089c
	mov	r5, r9
	strh	r5, [r1, r2]
	b.n	.L_081208b8
.L_08120890:
	mov	r3, r9
	strh	r3, [r1, r0]
	adds	r3, r6, #0
	adds	r3, #102
	strh	r2, [r1, r3]
	b.n	.L_081208b8
.L_0812089c:
	ldrsh	r2, [r0, r7]
	cmp	r2, #255
	beq.n	.L_08120890
	adds	r5, #1
	adds	r0, #2
	adds	r4, #2
	cmp	r5, #5
	bgt.n	.L_081208b8
	ldrsh	r3, [r0, r1]
	adds	r6, r4, #0
	cmp	r3, #254
	bne.n	.L_0812089c
	mov	r3, r9
	strh	r3, [r0, r1]
.L_081208b8:
	bl	sub_0811b2c4
	mov	r0, r9
	bl	sub_0811be3c
	ldr	r2, [r0, #12]
	cmp	r2, #0
	bge.n	.L_081208d0
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r2, r2, r4
.L_081208d0:
	ldr	r3, [r0, #16]
	asrs	r2, r2, #16
	cmp	r3, #0
	bge.n	.L_081208e0
	movs	r5, #255
	lsls	r5, r5, #8
	adds	r5, #255
	adds	r3, r3, r5
.L_081208e0:
	asrs	r3, r3, #16
	mov	r1, r9
	bl	sub_0811a7ac
	bl	sub_0811a44c
	add	r5, sp, #96
	adds	r0, r5, #0
	bl	sub_0811a12c
	cmp	r0, #0
	ble.n	.L_0812090a
	adds	r6, r5, #0
	adds	r5, r0, #0
.L_081208fc:
	ldrh	r0, [r6, #0]
	subs	r5, #1
	adds	r6, #2
	bl	sub_0811be68
	cmp	r5, #0
	bne.n	.L_081208fc
.L_0812090a:
	movs	r0, #0
	mov	r1, r9
	bl	sub_08120360
	ldr	r0, [sp, #84]
	ldrb	r3, [r0, #3]
	cmp	r3, #87
	bne.n	.L_0812091e
	ldr	r1, [pc, #684]
	b.n	.L_0812094c
.L_0812091e:
	movs	r3, #248
	adds	r3, #255
	cmp	fp, r3
	beq.n	.L_08120934
	ldr	r1, [pc, #676]
	movs	r0, #4
	bl	sub_08120360
	ldr	r4, [sp, #84]
	ldrb	r1, [r4, #3]
	b.n	.L_08120964
.L_08120934:
	ldr	r1, [pc, #664]
	movs	r0, #4
	bl	sub_08120360
	ldr	r5, [sp, #84]
	ldrb	r1, [r5, #3]
	b.n	.L_08120964
.L_08120942:
	movs	r0, #248
	adds	r0, #255
	cmp	fp, r0
	bne.n	.L_08120958
	ldr	r1, [pc, #648]
.L_0812094c:
	movs	r0, #4
	bl	sub_08120360
	ldr	r2, [sp, #84]
	ldrb	r1, [r2, #3]
	b.n	.L_08120964
.L_08120958:
	ldr	r1, [pc, #636]
	movs	r0, #4
	bl	sub_08120360
	ldr	r3, [sp, #84]
	ldrb	r1, [r3, #3]
.L_08120964:
	ldr	r4, [sp, #40]
	cmp	r4, #0
	beq.n	.L_08120a10
	adds	r3, r1, #0
	cmp	r3, #53
	beq.n	.L_08120974
	cmp	r3, #83
	bne.n	.L_081209a8
.L_08120974:
	movs	r5, #0
	str	r5, [sp, #40]
	ldr	r0, [sp, #64]
	movs	r3, #187
	lsls	r3, r3, #2
	ldrsh	r3, [r0, r3]
	movs	r2, #0
	cmp	r3, sl
	bne.n	.L_0812098c
	movs	r5, #1
	str	r5, [sp, #40]
	b.n	.L_08120a10
.L_0812098c:
	adds	r2, #1
	cmp	r2, #19
	bhi.n	.L_08120a10
	movs	r0, #187
	ldr	r4, [sp, #64]
	lsls	r3, r2, #4
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrsh	r3, [r4, r3]
	cmp	r3, sl
	bne.n	.L_0812098c
	movs	r0, #1
	str	r0, [sp, #40]
	b.n	.L_08120a10
.L_081209a8:
	adds	r3, r1, #0
	cmp	r3, #35
	bne.n	.L_081209b4
	movs	r2, #1
	str	r2, [sp, #56]
	b.n	.L_08120a10
.L_081209b4:
	cmp	r3, #34
	bne.n	.L_081209be
	movs	r3, #1
	str	r3, [sp, #44]
	b.n	.L_08120a10
.L_081209be:
	cmp	r3, #27
	bne.n	.L_081209c8
	movs	r4, #1
	str	r4, [sp, #32]
	b.n	.L_08120a10
.L_081209c8:
	cmp	r3, #55
	bne.n	.L_081209e4
	ldr	r0, [sp, #80]
	movs	r5, #56
	ldrsh	r3, [r0, r5]
	cmp	r3, #0
	beq.n	.L_08120a10
	ldr	r1, [sp, #76]
	movs	r0, #12
	bl	sub_08120360
	ldr	r2, [sp, #84]
	ldrb	r1, [r2, #3]
	b.n	.L_08120a10
.L_081209e4:
	cmp	r3, #32
	bne.n	.L_081209fe
	mov	r5, r8
	movs	r4, #58
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_081209f8
	movs	r0, #10
	str	r0, [sp, #28]
	b.n	.L_08120a10
.L_081209f8:
	movs	r2, #0
	str	r2, [sp, #40]
	b.n	.L_08120a10
.L_081209fe:
	cmp	r3, #90
	bne.n	.L_08120a08
	movs	r3, #2
	str	r3, [sp, #56]
	b.n	.L_08120a10
.L_08120a08:
	cmp	r3, #91
	bne.n	.L_08120a10
	movs	r4, #2
	str	r4, [sp, #56]
.L_08120a10:
	adds	r3, r1, #0
	cmp	r3, #75
	bne.n	.L_08120a20
	ldr	r5, [sp, #88]
	cmp	r5, #0
	beq.n	.L_08120a20
	bl	.L_08121480
.L_08120a20:
	ldr	r0, [sp, #32]
	cmp	r0, #0
	beq.n	.L_08120a2a
	bl	.L_08121480
.L_08120a2a:
	mov	r2, r8
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L_08120a44
	ldr	r3, [sp, #84]
	ldrb	r0, [r3, #3]
	bl	sub_080ad280
	cmp	r0, #0
	bne.n	.L_08120a44
	bl	.L_08121480
.L_08120a44:
	ldr	r3, [sp, #28]
	adds	r3, #1
	cmp	r3, #12
	bls.n	.L_08120a50
	bl	.L_08121480
.L_08120a50:
	ldr	r2, [pc, #392]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08120e74
	.4byte 0x08121480
	.4byte 0x08120dce
	.4byte 0x08121304
	.4byte 0x08120a8c
	.4byte 0x08120a8c
	.4byte 0x08120f70
	.4byte 0x08120f70
	.4byte 0x08121480
	.4byte 0x08120f70
	.4byte 0x08121480
	.4byte 0x08120ce2
	.2byte 0x1278
	.2byte 0x0812
	movs	r3, #44
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #4
	bne.n	.L_08120a9c
	bl	.L_08121480
.L_08120a9c:
	mov	r4, r8
	movs	r5, #56
	ldrsh	r4, [r4, r5]
	mov	r5, r8
	ldrh	r5, [r5, #62]
	ldr	r0, [sp, #56]
	mov	r9, r4
	str	r5, [sp, #12]
	cmp	r0, #0
	beq.n	.L_08120ac0
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	str	r3, [sp, #12]
	cmp	r0, #2
	bne.n	.L_08120ac0
	movs	r1, #0
	str	r1, [sp, #12]
.L_08120ac0:
	movs	r2, #1
	str	r2, [sp, #8]
.L_08120ac4:
	ldr	r3, [sp, #72]
	cmp	r3, #4
	beq.n	.L_08120ada
	lsls	r3, r3, #2
	adds	r3, #72
	add	r3, r8
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	ldr	r5, [sp, #16]
	subs	r3, r5, r3
	str	r3, [sp, #68]
.L_08120ada:
	ldr	r0, [sp, #8]
	cmp	r0, #0
	bne.n	.L_08120ae4
	movs	r1, #0
	str	r1, [sp, #68]
.L_08120ae4:
	ldr	r2, [sp, #84]
	ldr	r3, [sp, #28]
	ldrh	r7, [r2, #10]
	cmp	r3, #4
	bne.n	.L_08120b06
	ldr	r4, [sp, #80]
	ldr	r1, [sp, #12]
	ldrh	r0, [r4, #60]
	movs	r2, #0
	ldr	r3, [sp, #68]
	bl	sub_080ad128
	movs	r1, #10
	muls	r0, r7
	bl	sub_08002054
	b.n	.L_08120b42
.L_08120b06:
	ldr	r0, [sp, #80]
	movs	r6, #156
	lsls	r6, r6, #1
	ldrh	r5, [r0, #60]
	cmp	fp, r6
	beq.n	.L_08120b1a
	movs	r1, #158
	lsls	r1, r1, #1
	cmp	fp, r1
	bne.n	.L_08120b36
.L_08120b1a:
	ldr	r2, [sp, #92]
	ldrb	r0, [r2, #0]
	bl	sub_08016ca4
	ldrh	r5, [r0, #60]
	cmp	fp, r6
	bne.n	.L_08120b36
	ldr	r3, [sp, #92]
	ldrb	r0, [r3, #2]
	bl	sub_08016ca4
	ldrh	r3, [r0, #60]
	lsrs	r3, r3, #2
	adds	r5, r5, r3
.L_08120b36:
	adds	r0, r5, #0
	ldr	r1, [sp, #12]
	adds	r2, r7, #0
	ldr	r3, [sp, #68]
	bl	sub_080ad128
.L_08120b42:
	adds	r5, r0, #0
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #36]
	muls	r5, r4
	cmp	r0, #0
	beq.n	.L_08120b9a
	cmp	r0, #1
	bne.n	.L_08120b60
	lsls	r3, r5, #2
	adds	r0, r3, r5
	cmp	r0, #0
	bge.n	.L_08120b5c
	adds	r0, #3
.L_08120b5c:
	asrs	r5, r0, #2
	b.n	.L_08120b6a
.L_08120b60:
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
.L_08120b6a:
	mov	r1, r8
	ldrb	r0, [r1, #15]
	movs	r1, #5
	bl	sub_0800205c
	lsls	r0, r0, #24
	ldr	r2, [sp, #8]
	lsrs	r0, r0, #24
	adds	r0, r5, r0
	adds	r5, r0, #6
	cmp	r2, #0
	bne.n	.L_08120b9a
	movs	r1, #0
	movs	r0, #6
	bl	sub_08120360
	mov	r3, sl
	ldr	r1, [pc, #80]
	cmp	r3, #7
	bhi.n	.L_08120b94
	adds	r1, #1
.L_08120b94:
	movs	r0, #5
	bl	sub_08120360
.L_08120b9a:
	bl	sub_080ad148
	movs	r3, #3
	ands	r3, r0
	adds	r5, r5, r3
	movs	r3, #44
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08120bee
	cmp	r3, #1
	bne.n	.L_08120bbc
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_08120bee
.L_08120bbc:
	cmp	r3, #2
	bne.n	.L_08120be4
	lsls	r0, r5, #1
	movs	r1, #5
	b.n	.L_08120be8
	movs	r0, r0
	.4byte 0x00000d64
	.4byte 0x00000d56
	.4byte 0x00000d54
	.4byte 0x00000d55
	.4byte 0x00000d57
	.4byte 0x08120a58
	.2byte 0x0c6e
	.2byte 0x0000
.L_08120be4:
	adds	r0, r5, #0
	movs	r1, #10
.L_08120be8:
	bl	sub_08002054
	adds	r5, r0, #0
.L_08120bee:
	cmp	r5, #0
	bgt.n	.L_08120bf4
	movs	r5, #1
.L_08120bf4:
	ldr	r4, [sp, #44]
	cmp	r4, #0
	beq.n	.L_08120c0a
	mov	r3, r9
	subs	r3, #1
	cmp	r5, r3
	bge.n	.L_08120c0a
	adds	r5, r3, #0
	cmp	r5, #0
	bgt.n	.L_08120c0a
	movs	r5, #1
.L_08120c0a:
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08120c28
	ldr	r1, [sp, #4]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #5
	bne.n	.L_08120c28
	cmp	r9, r5
	bgt.n	.L_08120c28
	mov	r5, r9
	subs	r5, #1
.L_08120c28:
	ldr	r2, [sp, #8]
	adds	r2, #1
	str	r2, [sp, #8]
	cmp	r2, #1
	bgt.n	.L_08120c34
	b.n	.L_08120ac4
.L_08120c34:
	mov	r3, r9
	subs	r3, r3, r5
	movs	r0, #8
	mov	r1, sl
	mov	r9, r3
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08120360
	mov	r4, sl
	cmp	r4, #7
	bhi.n	.L_08120c60
	ldr	r3, [pc, #736]
	ldr	r5, [sp, #24]
	adds	r1, r5, r3
	b.n	.L_08120c66
.L_08120c60:
	ldr	r3, [pc, #732]
	ldr	r0, [sp, #24]
	adds	r1, r0, r3
.L_08120c66:
	movs	r0, #4
	bl	sub_08120360
	mov	r1, r9
	cmp	r1, #0
	bgt.n	.L_08120cc6
	mov	r0, sl
	bl	sub_081203a8
	cmp	r0, #0
	beq.n	.L_08120c80
	movs	r2, #1
	mov	r9, r2
.L_08120c80:
	mov	r3, r9
	cmp	r3, #0
	bgt.n	.L_08120cc6
	movs	r4, #0
	movs	r0, #9
	mov	r1, sl
	mov	r9, r4
	bl	sub_08120360
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	movs	r5, #114
	adds	r5, #255
	cmp	r3, r5
	beq.n	.L_08120cce
	movs	r0, #118
	adds	r0, #255
	cmp	r3, r0
	beq.n	.L_08120cce
	mov	r1, sl
	movs	r0, #0
	bl	sub_08120360
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L_08120cbc
	ldr	r1, [pc, #648]
	b.n	.L_08120cbe
.L_08120cbc:
	ldr	r1, [pc, #648]
.L_08120cbe:
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08120cce
.L_08120cc6:
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
.L_08120cce:
	mov	r4, r8
	movs	r2, #56
	ldrsh	r3, [r4, r2]
	mov	r5, r9
	mov	r0, r9
	subs	r5, r3, r5
	mov	r1, r8
	str	r5, [sp, #48]
	strh	r0, [r1, #56]
	b.n	.L_081212fc
	movs	r2, #44
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	mov	r9, r2
	cmp	r3, #4
	bne.n	.L_08120cf2
	b.n	.L_08121480
.L_08120cf2:
	ldr	r4, [sp, #84]
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_08120cfc
	b.n	.L_08121480
.L_08120cfc:
	ldr	r1, [sp, #72]
	mov	r0, r8
	movs	r5, #58
	ldrsh	r6, [r0, r5]
	cmp	r1, #4
	beq.n	.L_08120d18
	lsls	r3, r1, #2
	adds	r3, #72
	add	r3, r8
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	ldr	r4, [sp, #16]
	subs	r3, r4, r3
	str	r3, [sp, #68]
.L_08120d18:
	ldr	r5, [sp, #84]
	movs	r2, #128
	ldrh	r7, [r5, #10]
	ldr	r1, [sp, #68]
	lsls	r2, r2, #1
	adds	r0, r7, #0
	bl	sub_080ad130
	adds	r5, r0, #0
	ldr	r0, [sp, #60]
	ldr	r2, [pc, #540]
	lsls	r3, r0, #2
	ldr	r3, [r2, r3]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r5
	bl	sub_08002054
	mov	r2, r9
	ldr	r1, [sp, #52]
	ldrb	r3, [r2, #0]
	adds	r5, r0, #0
	muls	r5, r1
	cmp	r3, #0
	beq.n	.L_08120d6a
	cmp	r3, #1
	bne.n	.L_08120d56
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_08120d6a
.L_08120d56:
	cmp	r3, #2
	bne.n	.L_08120d60
	lsls	r0, r5, #1
	movs	r1, #5
	b.n	.L_08120d64
.L_08120d60:
	adds	r0, r5, #0
	movs	r1, #10
.L_08120d64:
	bl	sub_08002054
	adds	r5, r0, #0
.L_08120d6a:
	ldr	r4, [sp, #84]
	ldrb	r3, [r4, #3]
	cmp	r3, #32
	bne.n	.L_08120d78
	cmp	r5, r6
	ble.n	.L_08120d7e
	adds	r5, r6, #0
.L_08120d78:
	cmp	r5, r6
	ble.n	.L_08120d7e
	adds	r5, r6, #0
.L_08120d7e:
	movs	r0, #8
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	mov	r0, sl
	cmp	r0, #7
	bhi.n	.L_08120da0
	ldr	r1, [pc, #432]
	b.n	.L_08120da2
.L_08120da0:
	ldr	r1, [pc, #432]
.L_08120da2:
	movs	r0, #4
	subs	r6, r6, r5
	bl	sub_08120360
	cmp	r6, #0
	bgt.n	.L_08120db0
	movs	r6, #0
.L_08120db0:
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
	mov	r2, r8
	movs	r1, #58
	ldrsh	r3, [r2, r1]
	mov	r0, sl
	subs	r3, r3, r6
	str	r3, [sp, #48]
	mov	r3, r8
	strh	r6, [r3, #58]
	bl	sub_080ad0d0
	b.n	.L_08121480
	ldr	r4, [sp, #84]
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_08120dd8
	b.n	.L_08121480
.L_08120dd8:
	mov	r0, r8
	movs	r5, #56
	ldrsh	r6, [r0, r5]
	cmp	r6, #0
	bne.n	.L_08120de4
	b.n	.L_08121480
.L_08120de4:
	ldr	r2, [sp, #72]
	adds	r7, r3, #0
	ldr	r1, [sp, #16]
	cmp	r2, #4
	bne.n	.L_08120df0
	movs	r1, #100
.L_08120df0:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r7, #0
	bl	sub_080ad138
	ldr	r4, [sp, #60]
	ldr	r2, [pc, #344]
	lsls	r3, r4, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100
	bl	sub_08002054
	adds	r5, r0, #0
	ldr	r0, [sp, #52]
	muls	r5, r0
	bl	sub_080ad148
	movs	r3, #3
	ands	r3, r0
	mov	r2, r8
	adds	r5, r5, r3
	movs	r1, #52
	ldrsh	r3, [r2, r1]
	adds	r6, r6, r5
	cmp	r6, r3
	ble.n	.L_08120e32
	adds	r6, r3, #0
	movs	r4, #56
	ldrsh	r3, [r2, r4]
	subs	r5, r6, r3
.L_08120e32:
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	mov	r1, r8
	movs	r0, #52
	ldrsh	r3, [r1, r0]
	cmp	r6, r3
	bne.n	.L_08120e4e
	ldr	r1, [pc, #276]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08120e5e
.L_08120e4e:
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08120360
	ldr	r1, [pc, #264]
	movs	r0, #4
	bl	sub_08120360
.L_08120e5e:
	mov	r4, r8
	movs	r2, #56
	ldrsh	r3, [r4, r2]
	mov	r5, r8
	subs	r3, r3, r6
	str	r3, [sp, #48]
	mov	r0, sl
	strh	r6, [r5, #56]
	bl	sub_080ad0d0
	b.n	.L_08121480
	movs	r0, #44
	adds	r0, #255
	add	r0, r8
	ldrb	r3, [r0, #0]
	mov	r9, r0
	cmp	r3, #4
	bne.n	.L_08120e84
	b.n	.L_08121480
.L_08120e84:
	ldr	r1, [sp, #84]
	ldrh	r3, [r1, #10]
	cmp	r3, #0
	bne.n	.L_08120e8e
	b.n	.L_08121480
.L_08120e8e:
	ldr	r4, [sp, #72]
	mov	r3, r8
	movs	r2, #58
	ldrsh	r6, [r3, r2]
	cmp	r4, #4
	beq.n	.L_08120eaa
	lsls	r3, r4, #2
	adds	r3, #72
	add	r3, r8
	movs	r5, #2
	ldrsh	r3, [r3, r5]
	ldr	r0, [sp, #16]
	subs	r3, r0, r3
	str	r3, [sp, #68]
.L_08120eaa:
	ldr	r1, [sp, #84]
	movs	r2, #128
	ldrh	r7, [r1, #10]
	lsls	r2, r2, #1
	ldr	r1, [sp, #68]
	adds	r0, r7, #0
	bl	sub_080ad130
	ldr	r4, [sp, #60]
	ldr	r2, [pc, #164]
	lsls	r3, r4, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100
	bl	sub_08002054
	mov	r1, r9
	adds	r5, r0, #0
	ldrb	r3, [r1, #0]
	ldr	r0, [sp, #52]
	muls	r5, r0
	cmp	r3, #0
	beq.n	.L_08120efc
	cmp	r3, #1
	bne.n	.L_08120ee8
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_08120efc
.L_08120ee8:
	cmp	r3, #2
	bne.n	.L_08120ef2
	lsls	r0, r5, #1
	movs	r1, #5
	b.n	.L_08120ef6
.L_08120ef2:
	adds	r0, r5, #0
	movs	r1, #10
.L_08120ef6:
	bl	sub_08002054
	adds	r5, r0, #0
.L_08120efc:
	movs	r0, #8
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	mov	r2, sl
	cmp	r2, #7
	bhi.n	.L_08120f1e
	ldr	r1, [pc, #76]
	b.n	.L_08120f20
.L_08120f1e:
	ldr	r1, [pc, #76]
.L_08120f20:
	movs	r0, #4
	subs	r6, r6, r5
	bl	sub_08120360
	cmp	r6, #0
	bgt.n	.L_08120f2e
	movs	r6, #0
.L_08120f2e:
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
	mov	r3, r8
	strh	r6, [r3, #58]
	b.n	.L_081212fc
	.4byte 0x00000c80
	.4byte 0x00000c7d
	.4byte 0x00000c71
	.4byte 0x00000c70
	.4byte 0x081288a0
	.4byte 0x00000c76
	.4byte 0x00000c75
	.4byte 0x081288b8
	.4byte 0x00000c6c
	.4byte 0x00000c69
	.4byte 0x081288d0
	.4byte 0x00000c73
	.2byte 0x0c72
	.2byte 0x0000
	movs	r3, #44
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #4
	bne.n	.L_08120f7e
	b.n	.L_08121480
.L_08120f7e:
	ldr	r4, [sp, #84]
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_08120f88
	b.n	.L_08121480
.L_08120f88:
	mov	r0, r8
	movs	r5, #56
	ldrsh	r6, [r0, r5]
	movs	r1, #1
	mov	r9, r1
.L_08120f92:
	ldr	r2, [sp, #72]
	cmp	r2, #4
	beq.n	.L_08120fa8
	lsls	r3, r2, #2
	adds	r3, #72
	add	r3, r8
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	ldr	r5, [sp, #16]
	subs	r3, r5, r3
	str	r3, [sp, #68]
.L_08120fa8:
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_08120fb2
	movs	r1, #0
	str	r1, [sp, #68]
.L_08120fb2:
	ldr	r2, [sp, #84]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r3, #255
	ldrh	r7, [r2, #10]
	cmp	fp, r3
	beq.n	.L_08120fe2
	movs	r4, #209
	lsls	r4, r4, #1
	adds	r4, #255
	cmp	fp, r4
	beq.n	.L_08120fe2
	movs	r5, #181
	lsls	r5, r5, #2
	cmp	fp, r5
	beq.n	.L_08120fe2
	ldr	r1, [sp, #4]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #6
	beq.n	.L_08120fe2
	cmp	r3, #10
	beq.n	.L_08120fe2
	b.n	.L_08121126
.L_08120fe2:
	movs	r3, #199
	lsls	r3, r3, #1
	movs	r2, #0
	cmp	fp, r3
	beq.n	.L_081210ec
	cmp	fp, r3
	bgt.n	.L_08121062
	subs	r3, #10
	cmp	fp, r3
	beq.n	.L_081210dc
	cmp	fp, r3
	bgt.n	.L_0812102c
	subs	r3, #5
	cmp	fp, r3
	beq.n	.L_081210e0
	cmp	fp, r3
	bgt.n	.L_08121016
	subs	r3, #2
	cmp	fp, r3
	beq.n	.L_081210e8
	cmp	fp, r3
	bgt.n	.L_081210ec
	subs	r3, #1
	cmp	fp, r3
	beq.n	.L_081210dc
	b.n	.L_08121106
.L_08121016:
	movs	r3, #130
	adds	r3, #255
	cmp	fp, r3
	beq.n	.L_081210f4
	cmp	fp, r3
	blt.n	.L_081210e8
	movs	r4, #193
	lsls	r4, r4, #1
	cmp	fp, r4
	beq.n	.L_08121100
	b.n	.L_08121106
.L_0812102c:
	movs	r3, #196
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_081210ec
	cmp	fp, r3
	bgt.n	.L_08121044
	subs	r3, #2
	cmp	fp, r3
	beq.n	.L_081210ec
	cmp	fp, r3
	bgt.n	.L_081210e0
	b.n	.L_081210e8
.L_08121044:
	movs	r3, #197
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_081210f8
	cmp	fp, r3
	blt.n	.L_08121106
	movs	r5, #198
	lsls	r5, r5, #1
	cmp	fp, r5
	beq.n	.L_081210dc
	movs	r0, #142
	adds	r0, #255
	cmp	fp, r0
	beq.n	.L_081210e8
	b.n	.L_08121106
.L_08121062:
	movs	r3, #152
	adds	r3, #255
	cmp	fp, r3
	beq.n	.L_081210e8
	cmp	fp, r3
	bgt.n	.L_0812109c
	subs	r3, #5
	cmp	fp, r3
	beq.n	.L_081210f0
	cmp	fp, r3
	bgt.n	.L_08121086
	subs	r3, #2
	cmp	fp, r3
	beq.n	.L_081210e8
	movs	r2, #12
	cmp	fp, r3
	bgt.n	.L_08121106
	b.n	.L_081210e0
.L_08121086:
	movs	r3, #202
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_08121104
	cmp	fp, r3
	blt.n	.L_081210f4
	movs	r1, #203
	lsls	r1, r1, #1
	cmp	fp, r1
	beq.n	.L_081210dc
	b.n	.L_08121106
.L_0812109c:
	movs	r3, #156
	adds	r3, #255
	cmp	fp, r3
	beq.n	.L_081210f4
	cmp	fp, r3
	bgt.n	.L_081210b0
	subs	r3, #2
	cmp	fp, r3
	beq.n	.L_081210e0
	b.n	.L_081210ec
.L_081210b0:
	movs	r3, #209
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	fp, r3
	beq.n	.L_08121100
	cmp	fp, r3
	bgt.n	.L_081210c8
	movs	r3, #206
	lsls	r3, r3, #1
	cmp	fp, r3
	beq.n	.L_081210fc
	b.n	.L_08121106
.L_081210c8:
	movs	r4, #214
	lsls	r4, r4, #1
	adds	r4, #255
	cmp	fp, r4
	beq.n	.L_081210e4
	movs	r5, #181
	lsls	r5, r5, #2
	cmp	fp, r5
	beq.n	.L_08121104
	b.n	.L_08121106
.L_081210dc:
	movs	r2, #3
	b.n	.L_08121106
.L_081210e0:
	movs	r2, #12
	b.n	.L_08121106
.L_081210e4:
	movs	r2, #35
	b.n	.L_08121106
.L_081210e8:
	movs	r2, #6
	b.n	.L_08121106
.L_081210ec:
	movs	r2, #9
	b.n	.L_08121106
.L_081210f0:
	movs	r2, #7
	b.n	.L_08121106
.L_081210f4:
	movs	r2, #15
	b.n	.L_08121106
.L_081210f8:
	movs	r2, #21
	b.n	.L_08121106
.L_081210fc:
	movs	r2, #24
	b.n	.L_08121106
.L_08121100:
	movs	r2, #30
	b.n	.L_08121106
.L_08121104:
	movs	r2, #40
.L_08121106:
	mov	r3, r8
	movs	r4, #156
	movs	r1, #52
	ldrsh	r0, [r3, r1]
	lsls	r4, r4, #6
	adds	r4, #16
	cmp	r0, r4
	ble.n	.L_0812111c
	movs	r0, #156
	lsls	r0, r0, #6
	adds	r0, #16
.L_0812111c:
	muls	r0, r2
	movs	r1, #100
	bl	sub_08002054
	adds	r7, r7, r0
.L_08121126:
	movs	r2, #128
	adds	r0, r7, #0
	ldr	r1, [sp, #68]
	lsls	r2, r2, #1
	bl	sub_080ad130
	adds	r5, r0, #0
	ldr	r3, [sp, #28]
	ldr	r0, [sp, #52]
	muls	r5, r0
	cmp	r3, #6
	beq.n	.L_0812115e
	cmp	r3, #6
	bgt.n	.L_08121148
	cmp	r3, #5
	beq.n	.L_0812114e
	b.n	.L_08121172
.L_08121148:
	cmp	r3, #8
	beq.n	.L_08121156
	b.n	.L_08121172
.L_0812114e:
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #732]
	lsls	r3, r1, #2
	b.n	.L_08121164
.L_08121156:
	ldr	r4, [sp, #60]
	ldr	r2, [pc, #728]
	lsls	r3, r4, #2
	b.n	.L_08121164
.L_0812115e:
	ldr	r0, [sp, #60]
	ldr	r2, [pc, #724]
	lsls	r3, r0, #2
.L_08121164:
	ldr	r3, [r2, r3]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r5
	bl	sub_08002054
	adds	r5, r0, #0
.L_08121172:
	bl	sub_080ad148
	movs	r3, #3
	ands	r3, r0
	adds	r5, r5, r3
	movs	r3, #44
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081211a8
	cmp	r3, #1
	bne.n	.L_08121194
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_081211a8
.L_08121194:
	cmp	r3, #2
	bne.n	.L_0812119e
	lsls	r0, r5, #1
	movs	r1, #5
	b.n	.L_081211a2
.L_0812119e:
	adds	r0, r5, #0
	movs	r1, #10
.L_081211a2:
	bl	sub_08002054
	adds	r5, r0, #0
.L_081211a8:
	ldr	r3, [pc, #656]
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #6
	beq.n	.L_081211ce
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_081211d4
	ldr	r4, [sp, #4]
	movs	r2, #0
	ldrsh	r3, [r4, r2]
	cmp	r3, #6
	bne.n	.L_081211d4
.L_081211ce:
	cmp	r6, r5
	ble.n	.L_081211d4
	adds	r5, r6, #0
.L_081211d4:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	cmp	r1, #1
	bgt.n	.L_081211e0
	b.n	.L_08120f92
.L_081211e0:
	movs	r0, #8
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	mov	r2, sl
	cmp	r2, #7
	bhi.n	.L_08121206
	ldr	r3, [pc, #576]
	ldr	r4, [sp, #24]
	adds	r1, r4, r3
	b.n	.L_0812120c
.L_08121206:
	ldr	r3, [pc, #572]
	ldr	r0, [sp, #24]
	adds	r1, r0, r3
.L_0812120c:
	movs	r0, #4
	subs	r6, r6, r5
	bl	sub_08120360
	cmp	r6, #0
	bgt.n	.L_08121224
	mov	r0, sl
	bl	sub_081203a8
	cmp	r0, #0
	beq.n	.L_08121224
	movs	r6, #1
.L_08121224:
	ldr	r3, [pc, #532]
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #6
	bne.n	.L_08121236
	movs	r6, #0
.L_08121236:
	cmp	r6, #0
	bgt.n	.L_08121260
	movs	r0, #9
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	mov	r2, sl
	movs	r6, #0
	cmp	r2, #7
	bhi.n	.L_08121256
	ldr	r1, [pc, #500]
	b.n	.L_08121258
.L_08121256:
	ldr	r1, [pc, #500]
.L_08121258:
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08121268
.L_08121260:
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
.L_08121268:
	mov	r5, r8
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	mov	r0, r8
	subs	r3, r3, r6
	str	r3, [sp, #48]
	strh	r6, [r0, #56]
	b.n	.L_081212fc
	ldr	r1, [sp, #84]
	ldrh	r3, [r1, #10]
	cmp	r3, #0
	bne.n	.L_08121282
	b.n	.L_08121480
.L_08121282:
	ldr	r4, [sp, #72]
	mov	r3, r8
	ldrh	r7, [r1, #10]
	movs	r2, #58
	ldrsh	r6, [r3, r2]
	ldr	r1, [sp, #16]
	cmp	r4, #4
	bne.n	.L_08121294
	movs	r1, #100
.L_08121294:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r7, #0
	bl	sub_080ad138
	adds	r5, r0, #0
	ldr	r0, [sp, #60]
	ldr	r2, [pc, #428]
	lsls	r3, r0, #2
	ldr	r3, [r2, r3]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r5
	bl	sub_08002054
	ldr	r1, [sp, #52]
	adds	r5, r0, #0
	muls	r5, r1
	mov	r4, r8
	movs	r2, #54
	ldrsh	r3, [r4, r2]
	adds	r6, r6, r5
	cmp	r6, r3
	ble.n	.L_081212cc
	adds	r6, r3, #0
	movs	r5, #58
	ldrsh	r3, [r4, r5]
	subs	r5, r6, r3
.L_081212cc:
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	mov	r1, r8
	movs	r0, #54
	ldrsh	r3, [r1, r0]
	cmp	r6, r3
	bne.n	.L_081212e8
	ldr	r1, [pc, #372]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_081212f8
.L_081212e8:
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08120360
	ldr	r1, [pc, #356]
	movs	r0, #4
	bl	sub_08120360
.L_081212f8:
	mov	r2, r8
	strh	r6, [r2, #58]
.L_081212fc:
	mov	r0, sl
	bl	sub_080ad0d0
	b.n	.L_08121480
	movs	r3, #44
	adds	r3, #255
	add	r3, r8
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #4
	bne.n	.L_08121314
	b.n	.L_08121480
.L_08121314:
	ldr	r4, [sp, #40]
	cmp	r4, #0
	bne.n	.L_0812131c
	b.n	.L_08121468
.L_0812131c:
	ldr	r5, [sp, #84]
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	bne.n	.L_08121326
	b.n	.L_08121480
.L_08121326:
	ldr	r2, [sp, #72]
	mov	r1, r8
	movs	r0, #56
	ldrsh	r6, [r1, r0]
	cmp	r2, #4
	beq.n	.L_08121342
	lsls	r3, r2, #2
	adds	r3, #72
	add	r3, r8
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	ldr	r5, [sp, #16]
	subs	r3, r5, r3
	str	r3, [sp, #68]
.L_08121342:
	ldr	r0, [sp, #84]
	movs	r2, #128
	ldrh	r7, [r0, #10]
	ldr	r1, [sp, #68]
	lsls	r2, r2, #1
	adds	r0, r7, #0
	bl	sub_080ad130
	ldr	r4, [sp, #60]
	ldr	r1, [sp, #52]
	ldr	r2, [pc, #260]
	adds	r5, r0, #0
	lsls	r3, r4, #2
	muls	r5, r1
	ldr	r3, [r2, r3]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r5
	bl	sub_08002054
	adds	r5, r0, #0
	mov	r0, r9
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08121394
	cmp	r3, #1
	bne.n	.L_08121380
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_08121394
.L_08121380:
	cmp	r3, #2
	bne.n	.L_0812138a
	lsls	r0, r5, #1
	movs	r1, #5
	b.n	.L_0812138e
.L_0812138a:
	adds	r0, r5, #0
	movs	r1, #10
.L_0812138e:
	bl	sub_08002054
	adds	r5, r0, #0
.L_08121394:
	movs	r0, #8
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08120360
	mov	r1, sl
	movs	r0, #0
	bl	sub_08120360
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L_081213b6
	ldr	r1, [pc, #172]
	b.n	.L_081213b8
.L_081213b6:
	ldr	r1, [pc, #172]
.L_081213b8:
	movs	r0, #4
	subs	r6, r6, r5
	bl	sub_08120360
	cmp	r6, #0
	bgt.n	.L_08121412
	mov	r0, sl
	bl	sub_081203a8
	cmp	r0, #0
	beq.n	.L_081213d0
	movs	r6, #1
.L_081213d0:
	cmp	r6, #0
	bgt.n	.L_08121412
	movs	r0, #9
	mov	r1, sl
	bl	sub_08120360
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	movs	r2, #114
	adds	r2, #255
	movs	r6, #0
	cmp	r3, r2
	beq.n	.L_0812141a
	movs	r4, #118
	adds	r4, #255
	cmp	r3, r4
	beq.n	.L_0812141a
	movs	r0, #0
	mov	r1, sl
	mov	r5, sl
	bl	sub_08120360
	cmp	r5, #7
	bhi.n	.L_08121408
	ldr	r1, [pc, #64]
	b.n	.L_0812140a
.L_08121408:
	ldr	r1, [pc, #64]
.L_0812140a:
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_0812141a
.L_08121412:
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
.L_0812141a:
	mov	r1, r8
	movs	r0, #56
	ldrsh	r3, [r1, r0]
	mov	r2, r8
	subs	r3, r3, r6
	str	r3, [sp, #48]
	mov	r0, sl
	strh	r6, [r2, #56]
	bl	sub_080ad0d0
	b.n	.L_08121480
	.4byte 0x081288e8
	.4byte 0x08128900
	.4byte 0x08128918
	.4byte 0x02000240
	.4byte 0x00000c80
	.4byte 0x00000c7d
	.4byte 0x00000c71
	.4byte 0x00000c70
	.4byte 0x08128930
	.4byte 0x00000c6d
	.4byte 0x00000c6a
	.4byte 0x08128948
	.4byte 0x00000c73
	.2byte 0x0c72
	.2byte 0x0000
.L_08121468:
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #932]
	movs	r0, #4
	bl	sub_08120360
.L_08121480:
	ldr	r3, [sp, #64]
	movs	r4, #128
	lsls	r4, r4, #4
	adds	r4, #104
	adds	r5, r3, r4
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_081214ae
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	cmp	fp, r0
	bne.n	.L_081214ae
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #896]
	movs	r0, #4
	bl	sub_08120360
	movs	r3, #0
	strb	r3, [r5, #0]
.L_081214ae:
	mov	r1, sl
	movs	r0, #0
	bl	sub_08120360
	ldr	r1, [sp, #84]
	ldrb	r3, [r1, #3]
	cmp	r3, #75
	bne.n	.L_081214c8
	ldr	r2, [sp, #88]
	cmp	r2, #0
	bne.n	.L_081214c8
	bl	.L_081223ba
.L_081214c8:
	ldr	r3, [sp, #84]
	ldrb	r0, [r3, #3]
	bl	sub_080ad280
	cmp	r0, #0
	bne.n	.L_081214ee
	mov	r5, r8
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	bne.n	.L_081214ee
	ldr	r1, [sp, #84]
	ldrb	r0, [r1, #3]
	bl	sub_08120388
	cmp	r0, #0
	bne.n	.L_081214ee
	bl	.L_081223ba
.L_081214ee:
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L_081214f8
	bl	.L_081223ba
.L_081214f8:
	ldr	r4, [sp, #84]
	ldrb	r3, [r4, #3]
	subs	r3, #3
	cmp	r3, #85
	bls.n	.L_08121506
	bl	.L_081223ba
.L_08121506:
	ldr	r2, [pc, #800]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08121d34
	.4byte 0x08121756
	.4byte 0x08121c98
	.4byte 0x08121ada
	.4byte 0x08121a88
	.4byte 0x08121a36
	.4byte 0x081219e4
	.4byte 0x08121c48
	.4byte 0x08121bd0
	.4byte 0x08121b7e
	.4byte 0x08121b2c
	.4byte 0x08121e66
	.4byte 0x08121e08
	.4byte 0x08121dac
	.4byte 0x08121d50
	.4byte 0x08121ec4
	.4byte 0x08121ee2
	.4byte 0x08121f00
	.4byte 0x08121f12
	.4byte 0x08121f24
	.4byte 0x08121f36
	.4byte 0x08121f48
	.4byte 0x08121f5a
	.4byte 0x0812223e
	.4byte 0x08121fa2
	.4byte 0x0812224c
	.4byte 0x08122030
	.4byte 0x08122042
	.4byte 0x08122054
	.4byte 0x081220ba
	.4byte 0x081221b8
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x08122322
	.4byte 0x081222c4
	.4byte 0x081222f2
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081223ba
	.4byte 0x081222b8
	.4byte 0x08122290
	.4byte 0x081223ba
	.4byte 0x08121cb6
	.4byte 0x08121ce4
	.4byte 0x081219b0
	.4byte 0x0812197c
	.4byte 0x08122054
	.4byte 0x08121848
	.4byte 0x08121848
	.4byte 0x081218f2
	.4byte 0x08121668
	.4byte 0x081223ba
	.4byte 0x0812227e
	.4byte 0x08121f7e
	.4byte 0x081223ba
	.4byte 0x08122112
	.4byte 0x08121848
	.4byte 0x08121848
	.4byte 0x081222da
	.4byte 0x08121d0a
	.4byte 0x081223a8
	.4byte 0x08121f00
	.4byte 0x08121848
	.4byte 0x081218f2
	.4byte 0x081218f2
	.4byte 0x0812230a
	.4byte 0x0812224c
	.4byte 0x0812232e
	.4byte 0x0812233e
	.4byte 0x081222b8
	.4byte 0x0812216c
	.4byte 0x08121f36
	.4byte 0x081223ba
	.4byte 0x081223ba
	.2byte 0x22f2
	.2byte 0x0812
	movs	r2, #156
	lsls	r2, r2, #1
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08121688
	movs	r3, #0
	ldr	r1, [pc, #436]
	movs	r0, #4
	strb	r3, [r2, #0]
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
.L_08121688:
	movs	r2, #60
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081216b0
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #388]
	movs	r0, #4
	bl	sub_08120360
.L_081216b0:
	movs	r2, #158
	lsls	r2, r2, #1
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081216d0
	movs	r3, #0
	ldr	r1, [pc, #372]
	movs	r0, #4
	strb	r3, [r2, #0]
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
.L_081216d0:
	movs	r2, #62
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081216f0
	movs	r3, #0
	ldr	r1, [pc, #344]
	movs	r0, #4
	strb	r3, [r2, #0]
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
.L_081216f0:
	movs	r2, #66
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08121710
	movs	r3, #0
	ldr	r1, [pc, #316]
	movs	r0, #4
	strb	r3, [r2, #0]
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
.L_08121710:
	movs	r2, #160
	lsls	r2, r2, #1
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08121730
	movs	r3, #0
	ldr	r1, [pc, #288]
	movs	r0, #4
	strb	r3, [r2, #0]
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
.L_08121730:
	movs	r5, #50
	adds	r5, #255
	add	r5, r8
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_0812174a
	ldr	r1, [pc, #260]
	movs	r0, #4
	bl	sub_08120360
	movs	r3, #0
	strb	r3, [r5, #0]
.L_0812174a:
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	bl	.L_081223ba
	movs	r2, #156
	lsls	r2, r2, #1
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0812177e
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #180]
	movs	r0, #4
	bl	sub_08120360
.L_0812177e:
	movs	r2, #60
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081217a6
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #144]
	movs	r0, #4
	bl	sub_08120360
.L_081217a6:
	movs	r2, #158
	lsls	r2, r2, #1
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081217ce
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #108]
	movs	r0, #4
	bl	sub_08120360
.L_081217ce:
	movs	r2, #62
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081217f6
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #72]
	movs	r0, #4
	bl	sub_08120360
.L_081217f6:
	movs	r2, #66
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_08121806
	bl	.L_081223ba
.L_08121806:
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #0
	mov	r1, sl
	bl	sub_08120360
	ldr	r1, [pc, #32]
	bl	.L_08122326
	lsrs	r3, r5, #18
	movs	r0, r0
	lsrs	r7, r4, #18
	movs	r0, r0
	asrs	r0, r2, #20
	lsrs	r2, r2, #32
	lsrs	r3, r5, #19
	movs	r0, r0
	lsrs	r5, r5, #19
	movs	r0, r0
	lsrs	r3, r4, #19
	movs	r0, r0
	lsrs	r4, r5, #19
	movs	r0, r0
	lsrs	r4, r6, #19
	movs	r0, r0
	lsrs	r7, r5, #19
	movs	r0, r0
	lsrs	r4, r4, #19
	movs	r0, r0
	ldr	r1, [sp, #84]
	mov	r5, r8
	ldrb	r2, [r1, #3]
	ldrh	r7, [r5, #56]
	movs	r0, #56
	ldrsh	r5, [r5, r0]
	cmp	r2, #76
	bne.n	.L_08121866
	mov	r2, r8
	movs	r3, #52
	ldrsh	r0, [r2, r3]
	movs	r1, #5
	lsls	r0, r0, #1
	ldrh	r6, [r2, #52]
	b.n	.L_081218ac
.L_08121866:
	cmp	r2, #71
	bne.n	.L_0812187a
	mov	r4, r8
	movs	r0, #52
	ldrsh	r3, [r4, r0]
	movs	r1, #10
	lsls	r0, r3, #3
	subs	r0, r0, r3
	ldrh	r6, [r4, #52]
	b.n	.L_081218ac
.L_0812187a:
	cmp	r2, #70
	bne.n	.L_08121890
	mov	r1, r8
	ldrh	r6, [r1, #52]
	lsls	r3, r6, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r5, r5, r2
	b.n	.L_081218b2
.L_08121890:
	mov	r3, r8
	ldrh	r6, [r3, #52]
	movs	r4, #52
	ldrsh	r3, [r3, r4]
	cmp	r2, #61
	bne.n	.L_081218a4
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #2
	b.n	.L_081218aa
.L_081218a4:
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #1
.L_081218aa:
	movs	r1, #100
.L_081218ac:
	bl	sub_08002054
	adds	r5, r5, r0
.L_081218b2:
	lsls	r3, r6, #16
	asrs	r2, r3, #16
	cmp	r5, r2
	ble.n	.L_081218bc
	adds	r5, r2, #0
.L_081218bc:
	lsls	r3, r7, #16
	asrs	r3, r3, #16
	subs	r1, r5, r3
	cmp	r1, #0
	bne.n	.L_081218d0
	ldr	r0, [sp, #28]
	cmp	r0, #1
	beq.n	.L_081218d0
	bl	.L_081223ba
.L_081218d0:
	cmp	r5, r2
	bne.n	.L_081218de
	ldr	r1, [pc, #840]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_081218ec
.L_081218de:
	movs	r0, #1
	bl	sub_08120360
	ldr	r1, [pc, #828]
	movs	r0, #4
	bl	sub_08120360
.L_081218ec:
	mov	r1, r8
	strh	r5, [r1, #56]
	b.n	.L_08121fe4
	ldr	r4, [sp, #84]
	mov	r2, r8
	movs	r3, #58
	ldrsh	r5, [r2, r3]
	ldrb	r3, [r4, #3]
	ldrh	r7, [r2, #58]
	cmp	r3, #77
	bne.n	.L_08121914
	movs	r1, #54
	ldrsh	r0, [r2, r1]
	movs	r1, #10
	ldrh	r6, [r2, #54]
	bl	sub_08002054
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	b.n	.L_0812193a
.L_08121914:
	cmp	r3, #78
	bne.n	.L_08121928
	mov	r2, r8
	movs	r4, #54
	ldrsh	r3, [r2, r4]
	movs	r1, #10
	lsls	r0, r3, #1
	adds	r0, r0, r3
	ldrh	r6, [r2, #54]
	b.n	.L_08121936
.L_08121928:
	mov	r0, r8
	movs	r1, #54
	ldrsh	r3, [r0, r1]
	ldrh	r6, [r0, #54]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #100
.L_08121936:
	bl	sub_08002054
.L_0812193a:
	adds	r5, r5, r0
	lsls	r3, r6, #16
	asrs	r2, r3, #16
	cmp	r5, r2
	ble.n	.L_08121946
	adds	r5, r2, #0
.L_08121946:
	lsls	r3, r7, #16
	asrs	r3, r3, #16
	subs	r1, r5, r3
	cmp	r1, #0
	bne.n	.L_0812195a
	ldr	r3, [sp, #28]
	cmp	r3, #11
	beq.n	.L_0812195a
	bl	.L_081223ba
.L_0812195a:
	cmp	r5, r2
	bne.n	.L_08121968
	ldr	r1, [pc, #712]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08121976
.L_08121968:
	movs	r0, #1
	bl	sub_08120360
	ldr	r1, [pc, #700]
	movs	r0, #4
	bl	sub_08120360
.L_08121976:
	mov	r4, r8
	strh	r5, [r4, #58]
	b.n	.L_08121fe4
	movs	r2, #72
	adds	r2, #255
	add	r2, r8
	movs	r3, #8
	strb	r3, [r2, #0]
	movs	r2, #163
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #5
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	sub_080ad008
	mov	r3, r8
	adds	r3, #64
	ldrh	r1, [r3, #0]
	ldr	r3, [sp, #20]
	movs	r0, #1
	adds	r3, #64
	ldrh	r3, [r3, #0]
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #644]
	bl	.L_08122326
	movs	r2, #72
	adds	r2, #255
	add	r2, r8
	movs	r3, #252
	strb	r3, [r2, #0]
	movs	r2, #163
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #5
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	sub_080ad008
	ldr	r3, [sp, #20]
	movs	r0, #1
	adds	r3, #64
	ldrh	r1, [r3, #0]
	mov	r3, r8
	adds	r3, #64
	ldrh	r3, [r3, #0]
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #596]
	bl	.L_08122326
	movs	r2, #52
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r5, #4
	subs	r3, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r5, r5
	cmp	r3, r5
	bge.n	.L_08121a00
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121a00:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_08121a0c
	movs	r3, #4
	strb	r3, [r2, #0]
.L_08121a0c:
	mov	r0, sl
	bl	sub_080ad008
	ldr	r0, [sp, #20]
	mov	r2, r8
	ldrh	r3, [r2, #60]
	ldrh	r1, [r0, #60]
	movs	r0, #1
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #532]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #153
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	bl	.L_081223b8
	movs	r2, #52
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r4, #4
	subs	r3, #2
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_08121a52
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121a52:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_08121a5e
	movs	r3, #4
	strb	r3, [r2, #0]
.L_08121a5e:
	mov	r0, sl
	bl	sub_080ad008
	ldr	r5, [sp, #20]
	mov	r0, r8
	ldrh	r3, [r0, #60]
	ldrh	r1, [r5, #60]
	movs	r0, #1
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #448]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #153
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	bl	.L_081223b8
	movs	r2, #52
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r1, #4
	adds	r3, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_08121aa4
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121aa4:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_08121ab0
	movs	r3, #4
	strb	r3, [r2, #0]
.L_08121ab0:
	mov	r0, sl
	bl	sub_080ad008
	ldr	r4, [sp, #20]
	mov	r2, r8
	ldrh	r1, [r2, #60]
	ldrh	r3, [r4, #60]
	movs	r0, #1
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #372]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #153
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	bl	.L_081223b8
	movs	r2, #52
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r5, #4
	adds	r3, #2
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r5, r5
	cmp	r3, r5
	bge.n	.L_08121af6
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121af6:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_08121b02
	movs	r3, #4
	strb	r3, [r2, #0]
.L_08121b02:
	mov	r0, sl
	bl	sub_080ad008
	ldr	r2, [sp, #20]
	mov	r0, r8
	ldrh	r3, [r2, #60]
	ldrh	r1, [r0, #60]
	movs	r0, #1
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #288]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #153
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	bl	.L_081223b8
	movs	r2, #54
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r4, #4
	subs	r3, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_08121b48
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121b48:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_08121b54
	movs	r3, #4
	strb	r3, [r2, #0]
.L_08121b54:
	mov	r0, sl
	bl	sub_080ad008
	ldr	r5, [sp, #20]
	mov	r0, r8
	ldrh	r3, [r0, #62]
	ldrh	r1, [r5, #62]
	movs	r0, #1
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #212]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #154
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	bl	.L_081223b8
	movs	r2, #54
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r1, #4
	subs	r3, #2
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_08121b9a
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121b9a:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_08121ba6
	movs	r3, #4
	strb	r3, [r2, #0]
.L_08121ba6:
	mov	r0, sl
	bl	sub_080ad008
	ldr	r2, [sp, #20]
	mov	r4, r8
	ldrh	r1, [r2, #62]
	ldrh	r3, [r4, #62]
	movs	r0, #1
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #128]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #154
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	bl	.L_081223b8
	movs	r2, #54
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r5, #4
	adds	r3, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r5, r5
	cmp	r3, r5
	bge.n	.L_08121bec
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121bec:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_08121bf8
	movs	r3, #4
	strb	r3, [r2, #0]
.L_08121bf8:
	mov	r0, sl
	bl	sub_080ad008
	ldr	r2, [sp, #20]
	mov	r0, r8
	ldrh	r3, [r2, #62]
	ldrh	r1, [r0, #62]
	movs	r0, #1
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #52]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #154
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	.4byte 0x00000c6c
	.4byte 0x00000c69
	.4byte 0x00000c6d
	.4byte 0x00000c6a
	.4byte 0x00000cd3
	.4byte 0x00000cd4
	.4byte 0x00000cbc
	.4byte 0x00000cbd
	.4byte 0x00000cbe
	.2byte 0x0cbf
	.2byte 0x0000
	movs	r2, #54
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r4, #4
	adds	r3, #2
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_08121c64
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121c64:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_08121c70
	movs	r3, #4
	strb	r3, [r2, #0]
.L_08121c70:
	mov	r0, sl
	bl	sub_080ad008
	ldr	r0, [sp, #20]
	mov	r5, r8
	ldrh	r3, [r0, #62]
	ldrh	r1, [r5, #62]
	movs	r0, #1
	subs	r1, r1, r3
	bl	sub_08120360
	ldr	r1, [pc, #868]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #154
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	mov	r2, r8
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_08121ca4
	b.n	.L_081223ba
.L_08121ca4:
	movs	r0, #4
	ldr	r1, [pc, #840]
	bl	sub_08120360
	mov	r4, r8
	ldrh	r3, [r4, #52]
	mov	r5, r8
	strh	r3, [r5, #56]
	b.n	.L_08121fe4
	mov	r1, r8
	movs	r0, #56
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	beq.n	.L_08121cc2
	b.n	.L_081223ba
.L_08121cc2:
	movs	r0, #4
	ldr	r1, [pc, #808]
	bl	sub_08120360
	mov	r2, r8
	ldrh	r3, [r2, #52]
	mov	r0, sl
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	mov	r3, r8
	strh	r2, [r3, #56]
	bl	sub_080ad0d0
	b.n	.L_081223ba
	mov	r5, r8
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_08121cf0
	b.n	.L_081223ba
.L_08121cf0:
	ldr	r1, [pc, #764]
	movs	r0, #4
	bl	sub_08120360
	movs	r1, #52
	ldrsh	r0, [r5, r1]
	movs	r1, #10
	lsls	r0, r0, #3
	bl	sub_08002054
	mov	r2, r8
	strh	r0, [r2, #56]
	b.n	.L_08121fe4
	mov	r5, r8
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_08121d16
	b.n	.L_081223ba
.L_08121d16:
	movs	r0, #4
	ldr	r1, [pc, #724]
	bl	sub_08120360
	movs	r0, #52
	ldrsh	r3, [r5, r0]
	movs	r1, #10
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	bl	sub_08002054
	mov	r1, r8
	strh	r0, [r1, #56]
	b.n	.L_08121fe4
	movs	r5, #50
	adds	r5, #255
	add	r5, r8
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_08121d4a
	ldr	r1, [pc, #688]
	movs	r0, #4
	bl	sub_08120360
.L_08121d4a:
	movs	r3, #0
	strb	r3, [r5, #0]
	b.n	.L_081223ba
	movs	r2, #56
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r4, #4
	subs	r3, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_08121d6c
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121d6c:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L_08121d7c
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L_08121d7c:
	ldr	r5, [sp, #20]
	movs	r0, #56
	adds	r0, #255
	adds	r3, r5, r0
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r2, r2, r3
	lsls	r1, r2, #2
	adds	r1, r1, r2
	lsls	r1, r1, #2
	movs	r0, #1
	bl	sub_08120360
	ldr	r1, [pc, #604]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #155
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	movs	r2, #56
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r1, #4
	subs	r3, #2
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_08121dc8
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121dc8:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L_08121dd8
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L_08121dd8:
	ldr	r2, [sp, #20]
	movs	r4, #56
	adds	r4, #255
	adds	r3, r2, r4
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r2, r2, r3
	lsls	r1, r2, #2
	adds	r1, r1, r2
	lsls	r1, r1, #2
	movs	r0, #1
	bl	sub_08120360
	ldr	r1, [pc, #512]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #155
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	movs	r2, #56
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r5, #4
	adds	r3, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r5, r5
	cmp	r3, r5
	bge.n	.L_08121e24
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121e24:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L_08121e34
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L_08121e34:
	ldr	r0, [sp, #20]
	lsls	r3, r1, #24
	movs	r1, #56
	adds	r1, #255
	adds	r2, r0, r1
	ldrb	r2, [r2, #0]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	asrs	r3, r3, #24
	subs	r3, r3, r2
	lsls	r1, r3, #2
	adds	r1, r1, r3
	lsls	r1, r1, #2
	movs	r0, #1
	bl	sub_08120360
	ldr	r1, [pc, #420]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #155
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	movs	r2, #56
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r4, #4
	adds	r3, #2
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_08121e82
	movs	r3, #252
	strb	r3, [r2, #0]
.L_08121e82:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L_08121e92
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L_08121e92:
	ldr	r5, [sp, #20]
	movs	r0, #56
	adds	r0, #255
	adds	r2, r5, r0
	ldrb	r2, [r2, #0]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r3, r3, r2
	lsls	r1, r3, #2
	adds	r1, r1, r3
	lsls	r1, r1, #2
	movs	r0, #1
	bl	sub_08120360
	ldr	r1, [pc, #328]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #155
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	movs	r5, #50
	adds	r5, #255
	add	r5, r8
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_08121ed4
	b.n	.L_081223ba
.L_08121ed4:
	ldr	r1, [pc, #296]
	movs	r0, #4
	bl	sub_08120360
	movs	r3, #1
	strb	r3, [r5, #0]
	b.n	.L_081223ba
	movs	r5, #50
	adds	r5, #255
	add	r5, r8
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #1
	ble.n	.L_08121ef2
	b.n	.L_081223ba
.L_08121ef2:
	ldr	r1, [pc, #272]
	movs	r0, #4
	bl	sub_08120360
	movs	r3, #2
	strb	r3, [r5, #0]
	b.n	.L_081223ba
	ldr	r1, [pc, #260]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #156
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	ldr	r1, [pc, #248]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #58
	adds	r2, #255
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	ldr	r1, [pc, #232]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #157
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	ldr	r1, [pc, #220]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #60
	adds	r2, #255
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	ldr	r1, [pc, #204]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #158
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L_08121f6a
	ldr	r1, [pc, #184]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08121f72
.L_08121f6a:
	ldr	r1, [pc, #180]
	movs	r0, #4
	bl	sub_08120360
.L_08121f72:
	movs	r1, #62
	adds	r1, #255
	add	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #7
	b.n	.L_081222b2
	mov	r2, sl
	cmp	r2, #7
	bhi.n	.L_08121f8e
	ldr	r1, [pc, #148]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08121f96
.L_08121f8e:
	ldr	r1, [pc, #144]
	movs	r0, #4
	bl	sub_08120360
.L_08121f96:
	movs	r1, #62
	adds	r1, #255
	add	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #16
	b.n	.L_081222b2
	mov	r0, sl
	bl	sub_081203a8
	cmp	r0, #0
	beq.n	.L_08121fae
	b.n	.L_081223ba
.L_08121fae:
	movs	r0, #9
	mov	r1, sl
	bl	sub_08120360
	movs	r3, #149
	lsls	r3, r3, #1
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_08121fc6
	ldr	r1, [pc, #96]
	b.n	.L_08121fce
.L_08121fc6:
	mov	r3, fp
	cmp	r3, #219
	bne.n	.L_08121fd6
	ldr	r1, [pc, #88]
.L_08121fce:
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08121fde
.L_08121fd6:
	ldr	r1, [pc, #84]
	movs	r0, #4
	bl	sub_08120360
.L_08121fde:
	movs	r3, #0
	mov	r4, r8
	strh	r3, [r4, #56]
.L_08121fe4:
	mov	r0, sl
	bl	sub_080ad0d0
	b.n	.L_081223ba
	.4byte 0x00000cbf
	.4byte 0x00000cc0
	.4byte 0x00000ce4
	.4byte 0x00000cc1
	.4byte 0x00000cc2
	.4byte 0x00000cc3
	.4byte 0x00000cd0
	.4byte 0x00000cc4
	.4byte 0x00000cc5
	.4byte 0x00000cc6
	.4byte 0x00000cc7
	.4byte 0x00000cc8
	.4byte 0x00000cc9
	.4byte 0x00000cd2
	.4byte 0x00000ca4
	.4byte 0x00000ca5
	.2byte 0x0ca1
	.2byte 0x0000
	ldr	r1, [pc, #796]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #159
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #5
	b.n	.L_081223b8
	ldr	r1, [pc, #784]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #64
	adds	r2, #255
	add	r2, r8
	movs	r3, #7
	b.n	.L_081223b8
	ldr	r1, [sp, #80]
	ldr	r4, [sp, #84]
	movs	r0, #56
	ldrsh	r5, [r1, r0]
	ldrb	r3, [r4, #3]
	adds	r2, r5, #0
	ldr	r7, [sp, #48]
	cmp	r3, #60
	bne.n	.L_0812206e
	adds	r0, r7, #0
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r7, r3, #1
.L_0812206e:
	ldr	r4, [sp, #80]
	adds	r6, r7, #0
	movs	r1, #52
	ldrsh	r3, [r4, r1]
	adds	r5, r5, r6
	cmp	r5, r3
	ble.n	.L_08122080
	adds	r5, r3, #0
	subs	r6, r5, r2
.L_08122080:
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #0
	ldr	r1, [sp, #76]
	bl	sub_08120360
	ldr	r1, [sp, #80]
	movs	r0, #52
	ldrsh	r3, [r1, r0]
	cmp	r5, r3
	bne.n	.L_081220a4
	ldr	r1, [pc, #700]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_081220b4
.L_081220a4:
	movs	r0, #1
	adds	r1, r6, #0
	bl	sub_08120360
	ldr	r1, [pc, #684]
	movs	r0, #4
	bl	sub_08120360
.L_081220b4:
	ldr	r2, [sp, #80]
	strh	r5, [r2, #56]
	b.n	.L_0812210a
	ldr	r4, [sp, #80]
	ldr	r6, [sp, #48]
	movs	r3, #58
	ldrsh	r5, [r4, r3]
	movs	r0, #54
	ldrsh	r3, [r4, r0]
	adds	r2, r5, #0
	adds	r5, r5, r6
	cmp	r5, r3
	ble.n	.L_081220d2
	adds	r5, r3, #0
	subs	r6, r5, r2
.L_081220d2:
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #0
	ldr	r1, [sp, #76]
	bl	sub_08120360
	ldr	r2, [sp, #80]
	movs	r1, #54
	ldrsh	r3, [r2, r1]
	cmp	r5, r3
	bne.n	.L_081220f6
	ldr	r1, [pc, #624]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08122106
.L_081220f6:
	movs	r0, #1
	adds	r1, r6, #0
	bl	sub_08120360
	ldr	r1, [pc, #612]
	movs	r0, #4
	bl	sub_08120360
.L_08122106:
	ldr	r3, [sp, #80]
	strh	r5, [r3, #58]
.L_0812210a:
	ldr	r0, [sp, #76]
	bl	sub_080ad0d0
	b.n	.L_081223ba
	ldr	r0, [sp, #48]
	movs	r1, #10
	bl	sub_08002054
	adds	r5, r0, #0
	mov	r0, r8
	movs	r4, #58
	ldrsh	r3, [r0, r4]
	cmp	r3, r5
	bge.n	.L_08122128
	adds	r5, r3, #0
.L_08122128:
	ldr	r3, [sp, #80]
	ldr	r0, [sp, #80]
	movs	r2, #58
	ldrsh	r1, [r3, r2]
	movs	r4, #54
	ldrsh	r2, [r0, r4]
	adds	r3, r1, r5
	cmp	r3, r2
	ble.n	.L_0812213c
	subs	r5, r2, r1
.L_0812213c:
	cmp	r5, #0
	bne.n	.L_08122142
	b.n	.L_081223ba
.L_08122142:
	adds	r1, r5, #0
	movs	r0, #1
	bl	sub_08120360
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L_0812215a
	ldr	r1, [pc, #532]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08122162
.L_0812215a:
	ldr	r1, [pc, #528]
	movs	r0, #4
	bl	sub_08120360
.L_08122162:
	ldr	r0, [sp, #76]
	adds	r1, r5, #0
	bl	sub_080ad0c8
	b.n	.L_081223ba
	mov	r3, r8
	movs	r2, #54
	ldrsh	r0, [r3, r2]
	movs	r1, #10
	bl	sub_08002054
	lsls	r0, r0, #16
	asrs	r5, r0, #16
	mov	r0, r8
	movs	r4, #58
	ldrsh	r3, [r0, r4]
	cmp	r3, r5
	bge.n	.L_08122188
	adds	r5, r3, #0
.L_08122188:
	cmp	r5, #0
	bne.n	.L_0812218e
	b.n	.L_081223ba
.L_0812218e:
	adds	r1, r5, #0
	movs	r0, #1
	bl	sub_08120360
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L_081221a6
	ldr	r1, [pc, #464]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_081221ae
.L_081221a6:
	ldr	r1, [pc, #460]
	movs	r0, #4
	bl	sub_08120360
.L_081221ae:
	negs	r1, r5
	mov	r0, sl
	bl	sub_080ad0c8
	b.n	.L_081223ba
	movs	r2, #52
	adds	r2, #255
	add	r2, r8
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L_081221d2
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #153
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r2, #0]
.L_081221d2:
	movs	r1, #54
	adds	r1, #255
	add	r1, r8
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	ble.n	.L_081221ec
	movs	r3, #154
	lsls	r3, r3, #1
	movs	r2, #0
	add	r3, r8
	strb	r2, [r1, #0]
	strb	r2, [r3, #0]
.L_081221ec:
	movs	r2, #56
	adds	r2, #255
	add	r2, r8
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L_08122206
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #155
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r2, #0]
.L_08122206:
	movs	r2, #72
	adds	r2, #255
	add	r2, r8
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L_08122218
	movs	r3, #0
	strb	r3, [r2, #0]
.L_08122218:
	movs	r3, #150
	lsls	r3, r3, #1
	movs	r2, #0
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r3, #46
	adds	r3, #255
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r3, #151
	lsls	r3, r3, #1
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r3, #48
	adds	r3, #255
	add	r3, r8
	strb	r2, [r3, #0]
	ldr	r1, [pc, #316]
	b.n	.L_08122326
	ldr	r1, [pc, #316]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #160
	lsls	r2, r2, #1
	b.n	.L_081223b4
	movs	r5, #66
	adds	r5, #255
	add	r5, r8
	ldrb	r3, [r5, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	bne.n	.L_08122268
	ldr	r1, [pc, #292]
	movs	r0, #4
	bl	sub_08120360
	movs	r3, #7
	strb	r3, [r5, #0]
	b.n	.L_081223ba
.L_08122268:
	cmp	r2, #1
	bhi.n	.L_0812226e
	b.n	.L_081223ba
.L_0812226e:
	adds	r3, #255
	strb	r3, [r5, #0]
	movs	r0, #1
	ldrb	r1, [r5, #0]
	bl	sub_08120360
	ldr	r1, [pc, #264]
	b.n	.L_08122326
	ldr	r1, [pc, #264]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #162
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #2
	b.n	.L_081223b8
	ldr	r1, [pc, #248]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #164
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r2, sl
	cmp	r2, #7
	bls.n	.L_081222aa
	b.n	.L_081223ba
.L_081222aa:
	ldr	r1, [sp, #64]
	movs	r2, #2
	adds	r1, #67
	ldrb	r3, [r1, #0]
.L_081222b2:
	orrs	r3, r2
	strb	r3, [r1, #0]
	b.n	.L_081223ba
	ldr	r1, [pc, #212]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #70
	b.n	.L_081223b2
	ldr	r1, [pc, #204]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #44
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_081223ba
	b.n	.L_081223b6
	ldr	r1, [pc, #184]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #44
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #1
	bhi.n	.L_081223ba
	movs	r3, #2
	b.n	.L_081223b8
	ldr	r1, [pc, #164]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #44
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #2
	bhi.n	.L_081223ba
	movs	r3, #3
	b.n	.L_081223b8
	ldr	r1, [pc, #144]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #44
	adds	r2, #255
	add	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #3
	bhi.n	.L_081223ba
	movs	r3, #4
	b.n	.L_081223b8
	movs	r1, #1
	negs	r1, r1
.L_08122326:
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_081223ba
	ldr	r1, [pc, #112]
	movs	r0, #4
	bl	sub_08120360
	ldr	r2, [sp, #64]
	movs	r3, #1
	adds	r2, #71
	b.n	.L_081223b8
	ldr	r1, [pc, #100]
	movs	r0, #4
	bl	sub_08120360
	movs	r0, #15
	mov	r1, sl
	bl	sub_08120360
	b.n	.L_081223ba
	.4byte 0x00000ccb
	.4byte 0x00000ccc
	.4byte 0x00000c6c
	.4byte 0x00000c69
	.4byte 0x00000c6d
	.4byte 0x00000c6a
	.4byte 0x00000cb9
	.4byte 0x00000cb8
	.4byte 0x00000cbb
	.4byte 0x00000cba
	.4byte 0x00000cf6
	.4byte 0x00000cce
	.4byte 0x00000ccf
	.4byte 0x00000cd1
	.4byte 0x00000cd9
	.4byte 0x00000cda
	.4byte 0x00000cdb
	.4byte 0x00000cdd
	.4byte 0x00000cde
	.4byte 0x00000cdf
	.4byte 0x00000c8f
	.2byte 0x0ca6
	.2byte 0x0000
	ldr	r1, [pc, #268]
	movs	r0, #4
	bl	sub_08120360
	movs	r2, #68
.L_081223b2:
	adds	r2, #255
.L_081223b4:
	add	r2, r8
.L_081223b6:
	movs	r3, #1
.L_081223b8:
	strb	r3, [r2, #0]
.L_081223ba:
	movs	r0, #7
	movs	r1, #0
	bl	sub_08120360
	ldr	r5, [sp, #4]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	cmp	r3, #9
	beq.n	.L_08122408
	movs	r3, #68
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08122408
	ldr	r0, [sp, #84]
	ldrb	r3, [r0, #0]
	cmp	r3, #1
	bne.n	.L_08122408
	mov	r2, r8
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_08122446
	ldr	r4, [sp, #64]
	movs	r5, #128
	lsls	r5, r5, #4
	adds	r5, #88
	adds	r3, r4, r5
	mov	r0, sl
	str	r0, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #4
	ldr	r2, [sp, #76]
	adds	r1, #92
	adds	r3, r4, r1
	str	r2, [r3, #0]
.L_08122408:
	mov	r5, r8
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_08122446
	movs	r5, #158
	lsls	r5, r5, #1
	add	r5, r8
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08122446
	cmp	r3, #6
	bhi.n	.L_08122446
	ldr	r0, [sp, #48]
	cmp	r0, #0
	ble.n	.L_08122446
	bl	sub_080ad148
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_08122446
	strb	r0, [r5, #0]
	mov	r1, sl
	movs	r0, #0
	bl	sub_08120360
	ldr	r1, [pc, #124]
	movs	r0, #4
	bl	sub_08120360
.L_08122446:
	ldr	r0, [sp, #20]
	bl	sub_08013164
	mov	r0, sl
	bl	sub_080ad008
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_08038130
	mov	r2, r8
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_08122472
	movs	r0, #11
	mov	r1, sl
	bl	sub_08120360
.L_08122472:
	movs	r3, #148
	adds	r3, #255
	cmp	fp, r3
	beq.n	.L_081224aa
	ldr	r4, [sp, #80]
	movs	r5, #160
	lsls	r5, r5, #1
	adds	r3, r4, r5
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081224aa
	bl	sub_080ad148
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_081224aa
	ldr	r0, [sp, #48]
	cmp	r0, #0
	ble.n	.L_081224aa
	asrs	r0, r0, #2
	cmp	r0, #0
	bne.n	.L_081224a2
	movs	r0, #1
.L_081224a2:
	ldr	r1, [sp, #92]
	ldr	r3, [r1, #96]
	adds	r3, r3, r0
	str	r3, [r1, #96]
.L_081224aa:
	add	sp, #112
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000ce0
	.2byte 0x0ce3
	.2byte 0x0000
	push	{lr}
	cmp	r0, #126
	bne.n	.L_081224ca
	movs	r0, #1
	b.n	.L_081224d6
.L_081224ca:
	bl	sub_080ad078
	ldrb	r3, [r0, #9]
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
.L_081224d6:
	pop	{pc}
	push	{r5, lr}
	mov	r5, r9
	push	{r5}
	mov	r3, r9
	sub	sp, #4
	str	r3, [sp, #0]
	adds	r5, r0, #0
	bl	sub_080ad148
	ldrb	r2, [r5, #0]
	movs	r3, #255
	ands	r0, r3
	movs	r4, #0
	movs	r1, #0
	cmp	r0, r2
	blt.n	.L_08122508
.L_081224f8:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_08122508
	ldrb	r3, [r5, r1]
	adds	r2, r2, r3
	cmp	r0, r2
	bge.n	.L_081224f8
	adds	r4, r1, #0
.L_08122508:
	adds	r0, r4, #0
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, pc}
	.2byte 0x0000
