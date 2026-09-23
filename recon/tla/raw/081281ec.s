.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0800205c, 0x0800205c
	.set sub_08013164, 0x08013164
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08038118, 0x08038118
	.set sub_08038120, 0x08038120
	.set sub_080381c8, 0x080381c8
	.set sub_080ad028, 0x080ad028
	.set sub_080ad0b0, 0x080ad0b0
	.set sub_080ad140, 0x080ad140
	.set sub_080ad148, 0x080ad148
	.set sub_080ad1d8, 0x080ad1d8
	.set sub_0811a188, 0x0811a188
	.set sub_0811a24c, 0x0811a24c
	.set sub_0811ff08, 0x0811ff08
	.set sub_081281b0, 0x081281b0
	.set sub_081c0010, 0x081c0010
	.global Func_081281ec
	.thumb_func
Func_081281ec:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #36]
	movs	r2, #170
	lsls	r2, r2, #3
	ldr	r3, [pc, #40]
	adds	r1, r0, r2
	movs	r2, #151
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #0
	strh	r2, [r3, #0]
	str	r2, [r1, #0]
	str	r2, [r1, #4]
	str	r2, [r1, #8]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #98
	movs	r3, #3
	adds	r0, r0, r1
.L_08128216:
	subs	r3, #1
	strh	r2, [r0, #0]
	subs	r0, #2
	cmp	r3, #0
	bge.n	.L_08128216
	pop	{pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
.L_08128228:
	push	{lr}
	adds	r2, r0, #0
	adds	r2, #13
.L_0812822e:
	ldrb	r3, [r0, #0]
	movs	r1, #128
	adds	r3, #207
	lsls	r3, r3, #24
	lsls	r1, r1, #20
	cmp	r3, r1
	bhi.n	.L_08128242
	ldrb	r0, [r0, #0]
	subs	r0, #49
	b.n	.L_0812824a
.L_08128242:
	adds	r0, #1
	cmp	r0, r2
	ble.n	.L_0812822e
	movs	r0, #9
.L_0812824a:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	mov	fp, r1
	bl	sub_08016ca4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #36]
	mov	sl, r0
	movs	r0, #170
	lsls	r0, r0, #3
	adds	r7, r6, r0
	cmp	r5, #7
	bhi.n	.L_0812827a
	movs	r0, #1
	negs	r0, r0
	b.n	.L_081284b4
.L_0812827a:
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0812828c
	movs	r0, #2
	negs	r0, r0
	b.n	.L_081284b4
.L_0812828c:
	movs	r5, #26
	ldrsb	r5, [r7, r5]
	movs	r1, #165
	adds	r5, #1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r5, r5, r3
	lsls	r1, r1, #1
	strb	r5, [r7, #26]
	add	r1, sl
	lsls	r5, r5, #24
	ldrh	r2, [r1, #0]
	asrs	r5, r5, #24
	lsls	r3, r5, #1
	adds	r3, #20
	strh	r2, [r7, r3]
	mov	r0, sl
	mov	r8, r1
	bl	.L_08128228
	adds	r5, #24
	strb	r0, [r7, r5]
	ldrb	r2, [r7, #27]
	movs	r3, #27
	ldrsb	r3, [r7, r3]
	cmp	r3, #1
	bgt.n	.L_081282ca
	adds	r3, r2, #1
	strb	r3, [r7, #27]
.L_081282ca:
	mov	r5, r8
	ldrh	r2, [r6, #16]
	ldrh	r3, [r5, #0]
	movs	r4, #0
	movs	r0, #0
	cmp	r2, r3
	beq.n	.L_081282f0
	adds	r1, r6, #0
	mov	ip, r8
	adds	r1, #16
.L_081282de:
	adds	r0, #1
	cmp	r0, #5
	bgt.n	.L_081282f0
	adds	r1, #2
	mov	r5, ip
	ldrh	r2, [r1, #0]
	ldrh	r3, [r5, #0]
	cmp	r2, r3
	bne.n	.L_081282de
.L_081282f0:
	cmp	r0, #6
	beq.n	.L_081282f6
	adds	r4, r0, #0
.L_081282f6:
	ldrh	r3, [r6, #62]
	cmp	r3, #2
	beq.n	.L_0812830e
	ldrh	r3, [r6, #60]
	cmp	r4, r3
	bge.n	.L_08128304
	strh	r4, [r6, #60]
.L_08128304:
	ldr	r3, [r7, #8]
	cmp	r3, #0
	beq.n	.L_08128310
	movs	r3, #1
	strh	r3, [r6, #62]
.L_0812830e:
	ldr	r3, [r7, #8]
.L_08128310:
	adds	r3, #1
	movs	r0, #116
	str	r3, [r7, #8]
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08128322
	b.n	.L_081284b2
.L_08128322:
	movs	r5, #165
	lsls	r5, r5, #1
	add	r5, sl
	ldrh	r3, [r5, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	cmp	r3, r0
	bcs.n	.L_0812833e
	movs	r1, #192
	adds	r0, r3, #0
	lsls	r1, r1, #3
	adds	r0, r0, r1
	bl	sub_08016cfc
.L_0812833e:
	ldrh	r0, [r5, #0]
	bl	sub_080ad140
	mov	r2, fp
	mov	r8, r0
	cmp	r2, #0
	beq.n	.L_081283ee
	movs	r3, #66
	add	r3, r8
	mov	r9, r3
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081283a0
	movs	r6, #0
	movs	r5, #0
	b.n	.L_08128370
.L_0812835e:
	bl	sub_08014878
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	adds	r3, r6, r3
	adds	r6, r3, #1
	adds	r5, #1
.L_08128370:
	mov	r4, sl
	ldrb	r0, [r4, #15]
	movs	r1, #10
	bl	sub_0800205c
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r0, #1
	cmp	r5, r0
	blt.n	.L_0812835e
	mov	r0, r9
	ldrh	r5, [r0, #0]
	movs	r1, #10
	lsls	r0, r5, #1
	adds	r0, r0, r5
	bl	sub_08002054
	cmp	r6, r0
	bge.n	.L_08128398
	adds	r6, r0, #0
.L_08128398:
	ldr	r3, [r7, #0]
	adds	r2, r6, r5
	adds	r3, r3, r2
	str	r3, [r7, #0]
.L_081283a0:
	movs	r1, #72
	add	r1, r8
	ldrh	r3, [r1, #0]
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_08128406
	movs	r6, #0
	movs	r5, #0
	b.n	.L_081283c0
.L_081283b2:
	bl	sub_08014878
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r6, r0
	adds	r6, r0, #1
	adds	r5, #1
.L_081283c0:
	mov	r2, sl
	ldrb	r0, [r2, #15]
	movs	r1, #10
	bl	sub_0800205c
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r0, #1
	cmp	r5, r0
	blt.n	.L_081283b2
	mov	r3, r9
	ldrh	r5, [r3, #0]
	movs	r1, #10
	lsls	r0, r5, #1
	adds	r0, r0, r5
	bl	sub_08002054
	cmp	r6, r0
	bge.n	.L_081283e8
	adds	r6, r0, #0
.L_081283e8:
	ldr	r3, [r7, #4]
	adds	r2, r6, r5
	b.n	.L_08128402
.L_081283ee:
	mov	r3, r8
	adds	r3, #66
	ldrh	r2, [r3, #0]
	ldr	r3, [r7, #0]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	mov	r3, r8
	adds	r3, #72
	ldrh	r2, [r3, #0]
	ldr	r3, [r7, #4]
.L_08128402:
	adds	r3, r3, r2
	str	r3, [r7, #4]
.L_08128406:
	movs	r4, #68
	add	r4, r8
	movs	r5, #0
	ldrsh	r2, [r4, r5]
	mov	r9, r4
	cmp	r2, #0
	beq.n	.L_081284b2
	movs	r0, #70
	add	r0, r8
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	mov	ip, r0
	cmp	r3, #0
	beq.n	.L_081284b2
	ldrh	r3, [r7, #12]
	movs	r5, #0
	cmp	r3, r2
	beq.n	.L_08128442
	adds	r1, r7, #0
	mov	r0, r9
	adds	r1, #12
.L_08128430:
	adds	r5, #1
	cmp	r5, #3
	bgt.n	.L_08128442
	adds	r1, #2
	ldrh	r2, [r1, #0]
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	cmp	r2, r3
	bne.n	.L_08128430
.L_08128442:
	cmp	r5, #4
	bne.n	.L_081284b2
	mov	r1, ip
	mov	r2, fp
	movs	r5, #0
	ldrsh	r0, [r1, r5]
	cmp	r2, #0
	beq.n	.L_08128454
	subs	r0, #2
.L_08128454:
	cmp	r0, #0
	bge.n	.L_0812845a
	movs	r0, #0
.L_0812845a:
	movs	r5, #128
	lsls	r5, r5, #10
	asrs	r5, r0
	bl	sub_080ad148
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r0, r3
	cmp	r5, r0
	ble.n	.L_081284b2
	movs	r3, #1
	movs	r4, #128
	negs	r3, r3
	lsls	r4, r4, #23
	adds	r6, r7, #0
	mov	sl, r3
	mov	r8, r4
	movs	r5, #0
	adds	r6, #12
.L_08128482:
	ldrh	r0, [r6, #0]
	adds	r6, #2
	bl	sub_081281b0
	cmp	r0, r8
	bge.n	.L_08128492
	mov	r8, r0
	mov	sl, r5
.L_08128492:
	adds	r5, #1
	cmp	r5, #3
	ble.n	.L_08128482
	mov	r1, r9
	movs	r5, #0
	ldrsh	r0, [r1, r5]
	bl	sub_081281b0
	cmp	r0, r8
	ble.n	.L_081284b2
	mov	r2, sl
	mov	r4, r9
	lsls	r3, r2, #1
	ldrh	r2, [r4, #0]
	adds	r3, #12
	strh	r2, [r7, r3]
.L_081284b2:
	movs	r0, #0
.L_081284b4:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r0, #170
	lsls	r0, r0, #3
	adds	r0, r3, r0
	sub	sp, #64
	str	r0, [sp, #24]
	ldr	r0, [r0, #4]
	cmp	r0, #0
	beq.n	.L_081284f4
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #676]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_081284f4:
	mov	r2, sp
	adds	r2, #28
	adds	r1, r2, #0
	movs	r0, #1
	str	r2, [sp, #16]
	bl	sub_0811a188
	str	r0, [sp, #20]
	ldr	r1, [sp, #16]
	movs	r0, #1
	bl	sub_0811a24c
	str	r0, [sp, #12]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	sub_08014dac
	mov	fp, r0
	ldr	r0, [sp, #12]
	movs	r3, #0
	str	r3, [sp, #8]
	cmp	r3, r0
	blt.n	.L_08128524
	b.n	.L_081286ec
.L_08128524:
	movs	r2, #48
	str	r3, [sp, #0]
	add	r2, sp
	mov	r8, r2
.L_0812852c:
	ldr	r0, [sp, #16]
	ldr	r3, [sp, #0]
	ldrh	r3, [r3, r0]
	adds	r0, r3, #0
	str	r3, [sp, #4]
	bl	sub_08016ca4
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #20]
	mov	sl, r0
	cmp	r2, r3
	blt.n	.L_08128558
	ldr	r0, [sp, #24]
	movs	r1, #146
	lsls	r1, r1, #1
	add	r1, sl
	ldr	r2, [r0, #4]
	ldr	r3, [r1, #0]
	lsrs	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r1, #0]
	b.n	.L_081286bc
.L_08128558:
	ldr	r0, [sp, #24]
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r2, [r3, #0]
	ldr	r1, [r0, #4]
	adds	r2, r2, r1
	str	r2, [r3, #0]
	b.n	.L_081286bc
.L_0812856a:
	movs	r0, #89
	bl	sub_081c0010
	bl	sub_08038118
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r0, [r3, #0]
	movs	r1, #3
	bl	sub_08038120
	ldr	r2, [sp, #0]
	ldr	r3, [sp, #16]
	movs	r1, #1
	ldrh	r0, [r2, r3]
	bl	sub_08038120
	mov	r2, sl
	ldrb	r0, [r2, #15]
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #504]
	bl	sub_080381c8
	bl	sub_0811ff08
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	mov	r6, sl
	mov	r9, r3
	adds	r6, #88
	movs	r7, #31
.L_081285b0:
	ldrh	r5, [r6, #0]
	mov	r0, r9
	adds	r3, r5, #0
	ands	r3, r0
	adds	r6, #4
	cmp	r3, #0
	beq.n	.L_0812861a
	lsrs	r3, r5, #15
	cmp	r3, #0
	beq.n	.L_0812861a
	movs	r3, #88
	mov	r2, fp
	ldrh	r3, [r2, r3]
	movs	r1, #0
	cmp	r5, r3
	beq.n	.L_081285e0
	adds	r2, #88
.L_081285d2:
	adds	r1, #1
	cmp	r1, #31
	bgt.n	.L_081285e0
	adds	r2, #4
	ldrh	r3, [r2, #0]
	cmp	r5, r3
	bne.n	.L_081285d2
.L_081285e0:
	cmp	r1, #32
	bne.n	.L_0812861a
	bl	sub_08038118
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r0, [r3, #0]
	movs	r1, #3
	bl	sub_08038120
	movs	r1, #1
	ldr	r0, [sp, #4]
	bl	sub_08038120
	mov	r3, r9
	ands	r5, r3
	movs	r1, #4
	adds	r0, r5, #0
	bl	sub_08038120
	movs	r0, #154
	bl	sub_081c0010
	ldr	r0, [pc, #388]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_0812861a:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L_081285b0
	mov	r3, r8
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_0812863a
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #360]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_0812863a:
	mov	r3, r8
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_08128654
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #340]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08128654:
	mov	r3, r8
	movs	r2, #8
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_0812866e
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #316]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_0812866e:
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_08128688
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #296]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08128688:
	mov	r3, r8
	movs	r2, #12
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_081286a2
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #272]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_081286a2:
	mov	r3, r8
	movs	r2, #14
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_081286bc
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #252]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_081286bc:
	movs	r2, #166
	mov	r1, sl
	ldr	r3, [pc, #240]
	mov	r0, fp
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4641
	ldr	r0, [sp, #4]
	bl	sub_080ad0b0
	cmp	r0, #0
	beq.n	.L_081286d8
	b.n	.L_0812856a
.L_081286d8:
	ldr	r0, [sp, #0]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #12]
	adds	r0, #2
	adds	r2, #1
	str	r0, [sp, #0]
	str	r2, [sp, #8]
	cmp	r2, r3
	bge.n	.L_081286ec
	b.n	.L_0812852c
.L_081286ec:
	mov	r0, fp
	bl	sub_08013164
	ldr	r2, [sp, #24]
	ldr	r0, [r2, #0]
	cmp	r0, #0
	beq.n	.L_08128712
	movs	r1, #5
	bl	sub_08038120
	ldr	r0, [pc, #180]
	bl	sub_080381c8
	ldr	r3, [sp, #24]
	ldr	r0, [r3, #0]
	bl	sub_080ad1d8
	bl	sub_0811ff08
.L_08128712:
	ldr	r0, [pc, #168]
	mov	sl, r0
.L_08128716:
	movs	r7, #1
	negs	r7, r7
	mov	r8, r7
	movs	r6, #0
	movs	r5, #12
.L_08128720:
	ldr	r2, [sp, #24]
	ldrh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_08128736
	adds	r0, r3, #0
	bl	sub_081281b0
	cmp	r0, r8
	blt.n	.L_08128736
	mov	r8, r0
	adds	r7, r6, #0
.L_08128736:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #3
	ble.n	.L_08128720
	movs	r6, #1
	negs	r6, r6
	cmp	r7, r6
	beq.n	.L_0812877e
	lsls	r3, r7, #1
	adds	r5, r3, #0
	ldr	r3, [sp, #24]
	adds	r5, #12
	ldrh	r0, [r3, r5]
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #104]
	bl	sub_080381c8
	bl	sub_0811ff08
	ldr	r2, [sp, #24]
	ldrh	r0, [r2, r5]
	bl	sub_080ad028
	cmp	r0, r6
	bne.n	.L_08128776
	ldr	r0, [sp, #24]
	mov	r2, sl
	ldrh	r3, [r0, r5]
	strh	r3, [r2, #0]
	b.n	.L_0812877e
.L_08128776:
	ldr	r3, [pc, #20]
	ldr	r0, [sp, #24]
	strh	r3, [r0, r5]
	b.n	.L_08128716
.L_0812877e:
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000000
	.4byte 0x00000c86
	.4byte 0x00000cfa
	.4byte 0x00000cfb
	.4byte 0x00000cfc
	.4byte 0x00000cfd
	.4byte 0x00000cfe
	.4byte 0x00000cff
	.4byte 0x00000d00
	.4byte 0x00000d01
	.4byte 0x03000730
	.4byte 0x00000c87
	.4byte 0x0200049c
	.4byte 0x00000c88
