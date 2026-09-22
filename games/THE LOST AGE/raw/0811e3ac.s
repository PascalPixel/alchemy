.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08013ba4, 0x08013ba4
	.set sub_08014dac, 0x08014dac
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_080202b8, 0x080202b8
	.set sub_0811a31c, 0x0811a31c
	.set sub_0811a5fc, 0x0811a5fc
	.set sub_0811a720, 0x0811a720
	.set sub_0811b724, 0x0811b724
	.set sub_0811b75c, 0x0811b75c
	.set sub_0811be3c, 0x0811be3c
	.set sub_08126ae4, 0x08126ae4
	.set sub_081280bc, 0x081280bc
	.set sub_081280d8, 0x081280d8
	.set sub_081c0010, 0x081c0010
	.global Overlay_0811e3ac
Overlay_0811e3ac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r0, [sp, #8]
	adds	r7, r1, #0
	ldr	r3, [r0, #84]
	subs	r3, #15
	cmp	r3, #2
	bhi.n	.L_0811e3ce
	ldr	r5, [r0, #92]
	cmp	r5, #0
	beq.n	.L_0811e3d2
.L_0811e3ce:
	movs	r0, #0
	b.n	.L_0811e7cc
.L_0811e3d2:
	ldr	r1, [sp, #8]
	movs	r0, #56
	ldrb	r2, [r1, #3]
	ldrb	r3, [r1, #0]
	eors	r3, r2
	movs	r2, #128
	ands	r3, r2
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	str	r2, [sp, #4]
	bl	sub_08014dac
	add	r2, sp, #4
	mov	r9, r0
	ldrb	r2, [r2, #0]
	mov	r3, r9
	mov	r6, r9
	adds	r3, #49
	adds	r6, #50
	strb	r2, [r3, #0]
	strb	r5, [r6, #0]
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #452]
	ldr	r3, [r1, #76]
	adds	r3, r3, r2
	cmp	r3, #31
	bls.n	.L_0811e410
	b.n	.L_0811e528
.L_0811e410:
	ldr	r2, [pc, #444]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0811e498
	.4byte 0x0811e49c
	.4byte 0x0811e4a0
	.4byte 0x0811e4a4
	.4byte 0x0811e4aa
	.4byte 0x0811e4b0
	.4byte 0x0811e4b6
	.4byte 0x0811e4bc
	.4byte 0x0811e4c2
	.4byte 0x0811e4c6
	.4byte 0x0811e4ca
	.4byte 0x0811e4ce
	.4byte 0x0811e4d4
	.4byte 0x0811e4d8
	.4byte 0x0811e4de
	.4byte 0x0811e4e2
	.4byte 0x0811e4e6
	.4byte 0x0811e4ea
	.4byte 0x0811e4ee
	.4byte 0x0811e4f4
	.4byte 0x0811e4f8
	.4byte 0x0811e4fc
	.4byte 0x0811e500
	.4byte 0x0811e504
	.4byte 0x0811e508
	.4byte 0x0811e50e
	.4byte 0x0811e512
	.4byte 0x0811e518
	.4byte 0x0811e528
	.4byte 0x0811e51c
	.4byte 0x0811e520
	.2byte 0xe524
	.2byte 0x0811
	movs	r3, #117
	b.n	.L_0811e52a
	movs	r3, #137
	b.n	.L_0811e52a
	movs	r3, #182
	b.n	.L_0811e52a
	movs	r3, #44
	adds	r3, #255
	b.n	.L_0811e52a
	movs	r3, #148
	lsls	r3, r3, #1
	b.n	.L_0811e52a
	movs	r3, #139
	lsls	r3, r3, #1
	b.n	.L_0811e52a
	movs	r3, #158
	lsls	r3, r3, #1
	b.n	.L_0811e52a
	movs	r3, #62
	adds	r3, #255
	b.n	.L_0811e52a
	movs	r3, #185
	b.n	.L_0811e52a
	movs	r3, #158
	b.n	.L_0811e52a
	movs	r3, #150
	b.n	.L_0811e52a
	movs	r3, #56
	adds	r3, #255
	b.n	.L_0811e52a
	movs	r3, #212
	b.n	.L_0811e52a
	movs	r3, #68
	adds	r3, #255
	b.n	.L_0811e52a
	movs	r3, #191
	b.n	.L_0811e52a
	movs	r3, #159
	b.n	.L_0811e52a
	movs	r3, #162
	b.n	.L_0811e52a
	movs	r3, #155
	b.n	.L_0811e52a
	movs	r3, #8
	adds	r3, #255
	b.n	.L_0811e52a
	movs	r3, #197
	b.n	.L_0811e52a
	movs	r3, #215
	b.n	.L_0811e52a
	movs	r3, #99
	b.n	.L_0811e52a
	movs	r3, #196
	b.n	.L_0811e52a
	movs	r3, #160
	b.n	.L_0811e52a
	movs	r3, #26
	adds	r3, #255
	b.n	.L_0811e52a
	movs	r3, #163
	b.n	.L_0811e52a
	movs	r3, #151
	lsls	r3, r3, #1
	b.n	.L_0811e52a
	movs	r3, #188
	b.n	.L_0811e52a
	movs	r3, #242
	b.n	.L_0811e52a
	movs	r3, #203
	b.n	.L_0811e52a
	movs	r3, #218
	b.n	.L_0811e52a
.L_0811e528:
	movs	r3, #9
.L_0811e52a:
	mov	r1, r9
	strh	r3, [r1, #52]
	ldr	r2, [sp, #4]
	cmp	r2, #0
	bne.n	.L_0811e570
	ldr	r1, [sp, #8]
	movs	r2, #128
	ldrb	r3, [r1, #0]
	eors	r3, r2
	ands	r3, r2
	movs	r2, #48
	add	r2, r9
	strb	r3, [r2, #0]
	mov	r8, r2
	ldrb	r0, [r2, #0]
	bl	sub_0811be3c
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0811e57c
	movs	r3, #1
	strb	r3, [r6, #0]
	mov	r3, r8
	ldrb	r0, [r3, #0]
	bl	sub_0811be3c
	bl	sub_0811a720
	mov	r1, r8
	ldrb	r0, [r1, #0]
	bl	sub_0811be3c
	ldr	r3, [pc, #104]
	str	r3, [r0, #24]
	b.n	.L_0811e57c
.L_0811e570:
	ldr	r2, [sp, #8]
	movs	r1, #48
	ldrb	r3, [r2, #0]
	add	r1, r9
	strb	r3, [r1, #0]
	mov	r8, r1
.L_0811e57c:
	mov	r2, r8
	ldrb	r3, [r2, #0]
	str	r3, [r7, #8]
	ldr	r3, [sp, #8]
	ldrb	r0, [r3, #0]
	bl	sub_0811be3c
	ldr	r5, [r0, #0]
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #16
	bl	sub_08020098
	movs	r0, #128
	lsls	r0, r0, #19
	movs	r1, #253
	lsls	r1, r1, #6
	adds	r0, #80
	bl	sub_08013ba4
	movs	r0, #154
	bl	sub_081c0010
	movs	r0, #10
	bl	sub_08013560
	ldr	r1, [sp, #4]
	cmp	r1, #0
	beq.n	.L_0811e5d8
	mov	r2, r8
	ldrb	r0, [r2, #0]
	movs	r1, #1
	bl	sub_08126ae4
	add	r3, sp, #12
	mov	fp, r3
	b.n	.L_0811e608
	.4byte 0xfffffda8
	.4byte 0x0811e418
	.2byte 0x3333
	.2byte 0x0001
.L_0811e5d8:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	movs	r2, #12
	negs	r0, r3
	orrs	r0, r3
	add	r2, sp
	lsrs	r0, r0, #31
	mov	fp, r2
	adds	r0, #1
	mov	r1, fp
	bl	sub_0811a31c
	cmp	r0, #0
	ble.n	.L_0811e608
	mov	r6, fp
	adds	r5, r0, #0
.L_0811e5f8:
	ldrh	r0, [r6, #0]
	movs	r1, #1
	subs	r5, #1
	adds	r6, #2
	bl	sub_08126ae4
	cmp	r5, #0
	bne.n	.L_0811e5f8
.L_0811e608:
	movs	r7, #128
	ldr	r6, [pc, #8]
	lsls	r7, r7, #19
	movs	r5, #0
	adds	r7, #82
	b.n	.L_0811e618
	.2byte 0x0010
	.2byte 0x0000
.L_0811e618:
	subs	r2, r6, r5
	lsls	r3, r5, #8
	orrs	r3, r2
	strh	r3, [r7, #0]
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #16
	bne.n	.L_0811e618
	mov	r3, r8
	ldrb	r0, [r3, #0]
	bl	sub_0811be3c
	str	r0, [sp, #0]
	ldr	r2, [sp, #4]
	ldr	r1, [r0, #0]
	mov	sl, r1
	cmp	r2, #0
	beq.n	.L_0811e64a
	mov	r3, r8
	ldrb	r0, [r3, #0]
	bl	sub_0811b724
	b.n	.L_0811e67a
.L_0811e64a:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	mov	r1, fp
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	adds	r0, #1
	bl	sub_0811a31c
	cmp	r0, #0
	ble.n	.L_0811e67a
	mov	r7, fp
	movs	r6, #0
	adds	r5, r0, #0
.L_0811e666:
	ldrh	r0, [r6, r7]
	bl	sub_0811b724
	subs	r5, #1
	ldrh	r0, [r6, r7]
	bl	sub_0811a5fc
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L_0811e666
.L_0811e67a:
	mov	r2, r8
	ldrb	r0, [r2, #0]
	bl	sub_0811be3c
	movs	r2, #44
	adds	r1, r0, #0
	ldr	r3, [pc, #128]
	mov	r0, r9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4643
	ldrb	r0, [r3, #0]
	bl	sub_0811be3c
	ldr	r1, [sp, #4]
	bl	sub_0811a720
	mov	r1, r8
	ldrb	r0, [r1, #0]
	bl	sub_0811be3c
	ldr	r3, [pc, #100]
	str	r3, [r0, #24]
	ldr	r2, [sp, #4]
	cmp	r2, #0
	beq.n	.L_0811e6bc
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08014dac
	mov	r3, r9
	str	r0, [r3, #44]
	b.n	.L_0811e6c2
.L_0811e6bc:
	ldr	r3, [pc, #80]
	mov	r1, r9
	str	r3, [r1, #44]
.L_0811e6c2:
	mov	r2, r9
	ldrh	r0, [r2, #52]
	bl	sub_081280bc
	mov	r3, r9
	adds	r5, r0, #0
	ldrh	r0, [r3, #52]
	bl	sub_081280d8
	mov	r2, r9
	adds	r3, r0, #0
	ldr	r1, [r2, #44]
	movs	r0, #7
	adds	r2, r5, #0
	bl	sub_080202b8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0811e714
	mov	r3, r9
	ldrh	r0, [r3, #52]
	bl	sub_081280bc
	movs	r1, #224
	ldr	r2, [sp, #0]
	lsls	r1, r1, #7
	adds	r0, r0, r1
	strh	r0, [r2, #4]
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L_0811e732
	mov	r1, r9
	str	r3, [r1, #44]
	b.n	.L_0811e732
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x00013333
	.2byte 0x8000
	.2byte 0x0201
.L_0811e714:
	mov	r2, r9
	ldrh	r0, [r2, #52]
	bl	sub_081280bc
	ldr	r3, [sp, #0]
	strh	r0, [r3, #4]
	ldr	r1, [sp, #4]
	cmp	r1, #0
	beq.n	.L_0811e72e
	mov	r2, r9
	ldr	r0, [r2, #44]
	bl	sub_08013164
.L_0811e72e:
	mov	r3, r9
	str	r5, [r3, #44]
.L_0811e732:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	mov	r2, fp
	strh	r3, [r2, #0]
	mov	r1, fp
	movs	r3, #255
	strh	r3, [r1, #2]
	movs	r2, #0
	movs	r1, #1
	mov	r0, fp
	bl	sub_0811b75c
	movs	r3, #160
	lsls	r3, r3, #14
	mov	r2, sl
	str	r3, [r2, #12]
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #32]
	ldrb	r3, [r1, #3]
	cmp	r3, #7
	bhi.n	.L_0811e75e
	ldr	r2, [pc, #28]
.L_0811e75e:
	mov	r3, sl
	mov	r1, r8
	ldrb	r0, [r1, #0]
	strh	r2, [r3, #6]
	movs	r1, #1
	bl	sub_08126ae4
	movs	r6, #128
	lsls	r6, r6, #19
	movs	r5, #0
	adds	r6, #82
	b.n	.L_0811e780
	movs	r0, r0
	.4byte 0x00008000
	.2byte 0x0000
	.2byte 0x0000
.L_0811e780:
	movs	r2, #29
	subs	r2, r2, r5
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #16
	mov	r2, sl
	str	r3, [r2, #12]
	movs	r3, #0
	str	r3, [r2, #40]
	movs	r1, #136
	ldrh	r3, [r2, #6]
	lsls	r1, r1, #6
	adds	r1, #34
	adds	r3, r3, r1
	strh	r3, [r2, #6]
	cmp	r5, #15
	bgt.n	.L_0811e7ac
	ldr	r3, [pc, #36]
	subs	r3, r3, r5
	lsls	r3, r3, #8
	orrs	r3, r5
	strh	r3, [r6, #0]
.L_0811e7ac:
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #29
	ble.n	.L_0811e780
	mov	r3, r8
	ldrb	r0, [r3, #0]
	movs	r1, #0
	bl	sub_08126ae4
	mov	r0, r9
	b.n	.L_0811e7cc
	movs	r0, r0
	.2byte 0x0010
	.2byte 0x0000
.L_0811e7cc:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
