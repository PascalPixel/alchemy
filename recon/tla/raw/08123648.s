.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08038118, 0x08038118
	.set sub_08038120, 0x08038120
	.set sub_080381c8, 0x080381c8
	.set sub_080ad000, 0x080ad000
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad070, 0x080ad070
	.set sub_080ad078, 0x080ad078
	.set sub_080ad0c0, 0x080ad0c0
	.set sub_080ad0d0, 0x080ad0d0
	.set sub_080ad108, 0x080ad108
	.set sub_080ad118, 0x080ad118
	.set sub_080ad120, 0x080ad120
	.set sub_080ad148, 0x080ad148
	.set sub_080ad158, 0x080ad158
	.set sub_080ad168, 0x080ad168
	.set sub_080ad170, 0x080ad170
	.set sub_080ad188, 0x080ad188
	.set sub_080ad190, 0x080ad190
	.set sub_080ad1b0, 0x080ad1b0
	.set sub_080ad1b8, 0x080ad1b8
	.set sub_080ad280, 0x080ad280
	.set sub_080ad2c0, 0x080ad2c0
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811d720, 0x0811d720
	.set sub_0811d79c, 0x0811d79c
	.set sub_0811f4d4, 0x0811f4d4
	.set sub_0811f5a4, 0x0811f5a4
	.set sub_0811ff08, 0x0811ff08
	.set sub_08120158, 0x08120158
	.set sub_081201c4, 0x081201c4
	.set sub_08120360, 0x08120360
	.set sub_081224c0, 0x081224c0
	.set sub_08122c88, 0x08122c88
	.set sub_081234a4, 0x081234a4
	.set sub_081234f0, 0x081234f0
	.set sub_08123534, 0x08123534
	.set sub_08123574, 0x08123574
	.set sub_08126cfc, 0x08126cfc
	.set sub_08127308, 0x08127308
	.set sub_08128124, 0x08128124
	.set sub_081c0010, 0x081c0010
	.global Func_08123648
	.thumb_func
Func_08123648:
.L_08123648:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	mov	r1, r9
	add	r3, sp, #32
	str	r1, [r3, #0]
	mov	fp, r1
	bl	sub_080ad078
	str	r0, [sp, #28]
	movs	r2, #0
	ldrb	r5, [r0, #0]
	str	r2, [sp, #24]
	ldrb	r0, [r0, #3]
	bl	sub_080ad280
	cmp	r0, #0
	beq.n	.L_0812367a
	movs	r3, #1
	str	r3, [sp, #24]
.L_0812367a:
	cmp	r5, #0
	beq.n	.L_08123696
	cmp	r5, #4
	beq.n	.L_081236ae
	mov	r4, fp
	subs	r4, #12
	str	r4, [sp, #8]
	movs	r2, #0
	ldr	r3, [r4, #0]
	movs	r7, #0
	movs	r1, #12
	ldrsh	r0, [r3, r1]
	str	r2, [sp, #12]
	b.n	.L_081237fe
.L_08123696:
	mov	r3, fp
	subs	r3, #4
	ldr	r3, [r3, #0]
	movs	r1, #1
	strb	r1, [r3, #1]
	strb	r5, [r3, #17]
	mov	r2, fp
	subs	r2, #8
	ldr	r2, [r2, #0]
	strb	r1, [r3, #31]
	strb	r2, [r3, #3]
	b.n	.L_08123806
.L_081236ae:
	mov	r3, fp
	subs	r3, #4
	ldr	r2, [r3, #0]
	movs	r1, #1
	movs	r3, #0
	strb	r3, [r2, #17]
	strb	r1, [r2, #1]
	mov	r3, fp
	subs	r3, #8
	ldr	r3, [r3, #0]
	strb	r1, [r2, #31]
	strb	r3, [r2, #3]
	b.n	.L_08123806
.L_081236c8:
	movs	r3, #16
	negs	r3, r3
	add	r3, fp
	ldr	r2, [r3, #0]
	mov	sl, r3
	movs	r3, #88
	ldrsh	r3, [r2, r3]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L_081236ea
	adds	r2, #88
.L_081236de:
	adds	r2, #2
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	adds	r6, #1
	cmp	r3, #255
	bne.n	.L_081236de
.L_081236ea:
	str	r6, [sp, #20]
	mov	r2, sl
	ldr	r1, [r2, #0]
	movs	r3, #100
	adds	r2, r1, #2
	ldrsh	r3, [r2, r3]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L_0812370a
	adds	r2, #100
.L_081236fe:
	adds	r2, #2
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	adds	r6, #1
	cmp	r3, #255
	bne.n	.L_081236fe
.L_0812370a:
	str	r6, [sp, #16]
	ldr	r2, [sp, #8]
	movs	r4, #15
	ldr	r3, [r2, #0]
	mov	r8, r4
	ldrh	r3, [r3, #10]
	mov	r1, r8
	ands	r1, r3
	subs	r2, r1, r0
	adds	r3, r1, r0
	subs	r3, #1
	adds	r6, r2, #1
	mov	r8, r1
	str	r3, [sp, #4]
	cmp	r6, r3
	bgt.n	.L_081237ac
	movs	r2, #4
	negs	r2, r2
	lsls	r3, r6, #1
	add	r2, fp
	adds	r4, r3, #0
	mov	r9, r2
	adds	r4, #100
.L_08123738:
	cmp	r6, #0
	blt.n	.L_081237a0
	ldr	r0, [sp, #8]
	ldr	r3, [r0, #0]
	ldrh	r2, [r3, #10]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0812375a
	ldr	r1, [sp, #16]
	cmp	r6, r1
	bge.n	.L_081237a0
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, #2
	ldrsh	r5, [r3, r4]
	b.n	.L_0812376a
.L_0812375a:
	ldr	r1, [sp, #20]
	cmp	r6, r1
	bge.n	.L_081237a0
	mov	r3, sl
	ldr	r2, [r3, #0]
	lsls	r3, r6, #1
	adds	r3, #88
	ldrsh	r5, [r2, r3]
.L_0812376a:
	cmp	r5, #254
	beq.n	.L_081237a0
	ldr	r1, [sp, #24]
	cmp	r1, #0
	bne.n	.L_08123786
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L_081237a0
.L_08123786:
	mov	r3, r9
	ldr	r2, [r3, #0]
	adds	r0, r7, #0
	adds	r1, r2, #3
	adds	r0, #28
	movs	r3, #1
	strb	r3, [r1, r0]
	mov	r0, r8
	adds	r2, r2, r7
	subs	r3, r6, r0
	strb	r3, [r2, #17]
	strb	r5, [r1, r7]
	adds	r7, #1
.L_081237a0:
	ldr	r1, [sp, #4]
	adds	r6, #1
	adds	r4, #2
	cmp	r6, r1
	ble.n	.L_08123738
	b.n	.L_081237b4
.L_081237ac:
	movs	r2, #4
	negs	r2, r2
	add	r2, fp
	mov	r9, r2
.L_081237b4:
	mov	r4, r9
	ldr	r3, [r4, #0]
	strb	r7, [r3, #1]
	cmp	r7, #0
	bgt.n	.L_081237ee
	ldr	r0, [sp, #8]
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #72]
	bl	sub_080381c8
	mov	r3, fp
	subs	r3, #20
	ldr	r3, [r3, #0]
	movs	r4, #44
	adds	r4, #255
	adds	r2, r3, r4
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_081237e8
	movs	r3, #1
	strb	r3, [r2, #0]
.L_081237e8:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_08123808
.L_081237ee:
	ldr	r0, [sp, #28]
	ldrb	r3, [r0, #3]
	cmp	r3, #75
	bne.n	.L_08123806
	ldr	r1, [sp, #12]
	movs	r0, #11
	adds	r1, #1
	str	r1, [sp, #12]
.L_081237fe:
	ldr	r2, [sp, #12]
	cmp	r2, #1
	bgt.n	.L_08123806
	b.n	.L_081236c8
.L_08123806:
	movs	r0, #0
.L_08123808:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0c62
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	add	r3, sp, #36
	add	r2, sp, #28
	str	r1, [r3, #0]
	mov	fp, r3
	adds	r3, r0, #0
	str	r0, [r2, #0]
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	mov	sl, r2
	bl	sub_08016ca4
	mov	r1, sp
	mov	r2, sp
	adds	r1, #20
	adds	r2, #24
	str	r1, [sp, #12]
	str	r0, [r1, #0]
	str	r2, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	mov	r4, sl
	str	r3, [r2, #0]
	ldr	r3, [r4, #0]
	movs	r6, #4
	movs	r1, #10
	ldrsh	r0, [r3, r1]
	bl	sub_0811d720
	add	r5, sp, #32
	str	r0, [r5, #0]
	bl	sub_081234a4
	mov	r2, fp
	mov	r4, sl
	ldr	r3, [r2, #0]
	ldr	r2, [r4, #0]
	movs	r1, #0
	ldrh	r2, [r2, #0]
	str	r1, [r3, #100]
	str	r1, [r3, #96]
	strb	r1, [r3, #1]
	str	r1, [r3, #88]
	str	r1, [r3, #92]
	strb	r2, [r3, #0]
	strb	r2, [r3, #2]
	str	r6, [r3, #80]
	bl	sub_08038118
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	movs	r1, #56
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0812389e
	bl	.L_0812417c
.L_0812389e:
	ldr	r3, [pc, #836]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08123918
	movs	r0, #110
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08123918
	ldr	r1, [pc, #820]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08123918
	ldr	r3, [r1, #0]
	movs	r7, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_081238cc
	movs	r7, #0
.L_081238cc:
	movs	r6, #100
	b.n	.L_081238f4
.L_081238d0:
	cmp	r5, #254
	beq.n	.L_081238f2
	movs	r1, #192
	adds	r0, r5, #0
	lsls	r1, r1, #24
	bl	sub_080ad0c0
	cmp	r0, #0
	bne.n	.L_081238f2
	movs	r0, #8
	adds	r1, r5, #0
	bl	sub_08120360
	movs	r0, #9
	adds	r1, r5, #0
	bl	sub_08120360
.L_081238f2:
	adds	r6, #2
.L_081238f4:
	cmp	r7, #0
	beq.n	.L_08123902
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	adds	r3, #2
	ldrsh	r5, [r3, r6]
	b.n	.L_0812390c
.L_08123902:
	ldr	r0, [sp, #8]
	adds	r3, r6, #0
	ldr	r2, [r0, #0]
	subs	r3, #12
	ldrsh	r5, [r2, r3]
.L_0812390c:
	cmp	r5, #255
	bne.n	.L_081238d0
	bl	sub_081201c4
	bl	.L_0812417c
.L_08123918:
	bl	sub_08038118
	mov	r2, sl
	ldr	r0, [r2, #0]
	movs	r3, #6
	ldrsh	r4, [r0, r3]
	cmp	r4, #9
	beq.n	.L_081239c8
	ldr	r1, [sp, #12]
	movs	r3, #70
	ldr	r2, [r1, #0]
	adds	r3, #255
	adds	r1, r2, r3
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08123950
	movs	r3, #0
	strb	r3, [r1, #0]
	movs	r4, #0
	ldrsh	r0, [r0, r4]
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #676]
	bl	sub_080381c8
	bl	.L_081241aa
.L_08123950:
	movs	r1, #158
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08123970
	movs	r2, #0
	ldrsh	r0, [r0, r2]
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #648]
	bl	sub_080381c8
	bl	.L_081241aa
.L_08123970:
	movs	r1, #60
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08123990
	movs	r2, #0
	ldrsh	r0, [r0, r2]
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #620]
	bl	sub_080381c8
	bl	.L_081241aa
.L_08123990:
	movs	r0, #152
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081239c8
	cmp	r4, #3
	beq.n	.L_081239c8
	bl	sub_080ad148
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_081239c8
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08038120
	ldr	r0, [pc, #568]
	bl	sub_080381c8
	bl	.L_081241aa
.L_081239c8:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r0, #6
	ldrsh	r3, [r3, r0]
	cmp	r3, #8
	bne.n	.L_081239d6
	b.n	.L_0812417c
.L_081239d6:
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r7, #1
	movs	r1, #0
	adds	r3, #45
	movs	r2, #13
.L_081239e2:
	subs	r2, #1
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r2, #0
	bge.n	.L_081239e2
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r4, #1
	negs	r4, r4
	adds	r1, r4, #0
	adds	r3, #59
	movs	r2, #13
.L_081239fa:
	subs	r2, #1
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r2, #0
	bge.n	.L_081239fa
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #6
	ldrsh	r3, [r3, r1]
	cmp	r3, #99
	bls.n	.L_08123a14
	bl	.L_08124376
.L_08123a14:
	ldr	r2, [pc, #484]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08123c08
	.4byte 0x08123ca8
	.4byte 0x08123d96
	.4byte 0x08123e7a
	.4byte 0x08123e90
	.4byte 0x08124028
	.4byte 0x0812420a
	.4byte 0x08123e7a
	.4byte 0x0812417c
	.4byte 0x08123c68
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.4byte 0x08124376
	.2byte 0x3bac
	.2byte 0x0812
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldrh	r3, [r3, #0]
	lsls	r0, r3, #16
	movs	r3, #224
	lsls	r3, r3, #11
	cmp	r0, r3
	bhi.n	.L_08123bc4
	ldr	r0, [pc, #64]
	bl	sub_080381c8
	b.n	.L_08123bd2
.L_08123bc4:
	asrs	r0, r0, #16
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #52]
	bl	sub_080381c8
.L_08123bd2:
	bl	sub_0811ff08
	mov	r4, fp
	ldr	r2, [r4, #0]
	movs	r3, #7
	str	r3, [r2, #84]
	bl	.L_081247ea
	movs	r0, r0
	asrs	r0, r7, #8
	lsls	r0, r0, #12
	asrs	r0, r2, #5
	lsls	r0, r0, #12
	lsrs	r4, r3, #19
	movs	r0, r0
	lsrs	r7, r5, #18
	movs	r0, r0
	lsrs	r6, r5, #18
	movs	r0, r0
	lsrs	r0, r6, #18
	movs	r0, r0
	subs	r2, #28
	lsrs	r2, r2, #32
	lsrs	r0, r3, #18
	movs	r0, r0
	lsrs	r3, r3, #18
	movs	r0, r0
	ldr	r1, [sp, #12]
	ldr	r0, [r1, #0]
	bl	sub_080ad108
	add	r2, sp, #40
	mov	r9, r2
	adds	r7, r0, #0
	bl	.L_08123648
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_08123c26
	bl	.L_081247ec
.L_08123c26:
	cmp	r7, #1
	bne.n	.L_08123c2c
	b.n	.L_0812437c
.L_08123c2c:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08038120
	ldr	r2, [sp, #12]
	movs	r1, #1
	ldr	r0, [r2, #0]
	bl	sub_080ad070
	movs	r1, #2
	bl	sub_08038120
	ldr	r5, [pc, #788]
	adds	r0, r5, #0
	bl	sub_080381c8
	adds	r5, #1
	bl	sub_08120158
	adds	r0, r7, #0
	movs	r1, #4
	bl	sub_08038120
	adds	r0, r5, #0
.L_08123c62:
	bl	sub_080381c8
	b.n	.L_08124376
	ldr	r4, [sp, #12]
	movs	r0, #165
	ldr	r3, [r4, #0]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldrh	r3, [r3, #0]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #155
	adds	r3, r3, r1
	movs	r2, #128
	movs	r7, #44
	lsls	r3, r3, #16
	lsls	r2, r2, #10
	adds	r7, #255
	cmp	r3, r2
	bhi.n	.L_08123c8c
	movs	r7, #1
.L_08123c8c:
	ldr	r3, [r5, #0]
	cmp	r3, #255
	bne.n	.L_08123c94
	b.n	.L_0812417c
.L_08123c94:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r1, #1
	movs	r3, #0
	strb	r1, [r2, #1]
	strb	r3, [r2, #17]
	ldr	r3, [r5, #0]
	strb	r1, [r2, #31]
	strb	r3, [r2, #3]
	b.n	.L_08124376
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r5, #1
	movs	r0, #8
	ldrsh	r7, [r3, r0]
	adds	r0, r7, #0
	bl	sub_080ad078
	add	r1, sp, #40
	adds	r6, r0, #0
	mov	r9, r1
	adds	r0, r7, #0
	bl	.L_08123648
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_08123cd0
	bl	.L_081247ec
.L_08123cd0:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r7, #0
	movs	r1, #4
	bl	sub_08038120
	movs	r2, #167
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	r7, r2
	ble.n	.L_08123d1a
	movs	r3, #171
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	r7, r3
	ble.n	.L_08123d12
	movs	r4, #157
	lsls	r4, r4, #2
	cmp	r7, r4
	bge.n	.L_08123d1a
	movs	r0, #150
	lsls	r0, r0, #2
	cmp	r7, r0
	blt.n	.L_08123d1a
	ldr	r0, [pc, #600]
	bl	sub_080381c8
	b.n	.L_08123d20
.L_08123d12:
	ldr	r0, [pc, #596]
	bl	sub_080381c8
	b.n	.L_08123d20
.L_08123d1a:
	ldr	r0, [pc, #592]
	bl	sub_080381c8
.L_08123d20:
	ldr	r2, [sp, #12]
	ldr	r1, [r2, #0]
	movs	r3, #58
	ldrsh	r2, [r1, r3]
	ldrb	r3, [r6, #9]
	cmp	r2, r3
	bge.n	.L_08123d38
	mov	r4, fp
	ldr	r2, [r4, #0]
	movs	r3, #2
	str	r3, [r2, #92]
	movs	r5, #0
.L_08123d38:
	movs	r0, #62
	adds	r0, #255
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08123d4e
	mov	r1, fp
	ldr	r2, [r1, #0]
	movs	r3, #1
	str	r3, [r2, #92]
	movs	r5, #0
.L_08123d4e:
	cmp	r5, #0
	bne.n	.L_08123d54
	b.n	.L_08124376
.L_08123d54:
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r5, #0
	str	r5, [r3, #92]
	ldr	r3, [sp, #12]
	ldrb	r2, [r6, #9]
	ldr	r1, [r3, #0]
	mov	r4, sl
	ldrh	r3, [r1, #58]
	subs	r3, r3, r2
	strh	r3, [r1, #58]
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_080ad0d0
	ldr	r2, [sp, #12]
	ldr	r1, [r2, #0]
	movs	r4, #58
	ldrsh	r3, [r1, r4]
	cmp	r3, #0
	bge.n	.L_08123d82
	strh	r5, [r1, #58]
.L_08123d82:
	movs	r0, #58
	ldrsh	r2, [r1, r0]
	movs	r4, #54
	ldrsh	r3, [r1, r4]
	ldrh	r0, [r1, #54]
	cmp	r2, r3
	bgt.n	.L_08123d92
	b.n	.L_08124376
.L_08123d92:
	strh	r0, [r1, #58]
	b.n	.L_08124376
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #8
	ldrsh	r2, [r3, r1]
	cmp	r2, #0
	bge.n	.L_08123db4
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #448]
	bl	sub_080381c8
	b.n	.L_081241aa
.L_08123db4:
	ldr	r4, [sp, #12]
	lsls	r2, r2, #1
	ldr	r3, [r4, #0]
	adds	r2, #216
	ldrh	r0, [r3, r2]
	bl	sub_080ad010
	adds	r5, r0, #0
	ldrh	r7, [r5, #40]
	cmp	r7, #0
	beq.n	.L_08123de8
	mov	r3, sl
	ldr	r1, [r3, #0]
	ldr	r0, [sp, #12]
	movs	r4, #8
	ldrsh	r3, [r1, r4]
	ldr	r2, [r0, #0]
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r2, [r2, r3]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08123e14
	b.n	.L_08123dec
.L_08123de8:
	mov	r0, sl
	ldr	r1, [r0, #0]
.L_08123dec:
	movs	r2, #0
	ldrsh	r0, [r1, r2]
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #380]
	bl	sub_080381c8
	ldr	r4, [sp, #12]
	movs	r0, #44
	ldr	r3, [r4, #0]
	adds	r0, #255
	adds	r2, r3, r0
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08123e0e
	b.n	.L_081241aa
.L_08123e0e:
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_081241aa
.L_08123e14:
	add	r1, sp, #40
	mov	r9, r1
	adds	r0, r7, #0
	bl	.L_08123648
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_08123e2a
	bl	.L_081247ec
.L_08123e2a:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08038120
	ldr	r3, [sp, #12]
	mov	r4, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r4, #0]
	movs	r1, #2
	movs	r0, #8
	ldrsh	r3, [r3, r0]
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r0, [r2, r3]
	bl	sub_08038120
	ldrb	r3, [r5, #12]
	cmp	r3, #2
	beq.n	.L_08123e5a
	cmp	r3, #0
	bne.n	.L_08123e76
.L_08123e5a:
	ldrb	r0, [r5, #2]
	cmp	r0, #3
	beq.n	.L_08123e72
	cmp	r0, #3
	bgt.n	.L_08123e6a
	cmp	r0, #1
	beq.n	.L_08123e72
	b.n	.L_08123e76
.L_08123e6a:
	cmp	r0, #8
	bgt.n	.L_08123e76
	cmp	r0, #6
	blt.n	.L_08123e76
.L_08123e72:
	ldr	r0, [pc, #260]
	b.n	.L_08123c62
.L_08123e76:
	ldr	r0, [pc, #260]
	b.n	.L_08123c62
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08038120
	ldr	r0, [pc, #232]
	bl	sub_080381c8
	b.n	.L_081241aa
	mov	r4, sl
	ldr	r3, [r4, #0]
	add	r1, sp, #40
	movs	r0, #8
	ldrsh	r7, [r3, r0]
	mov	r9, r1
	adds	r0, r7, #0
	bl	.L_08123648
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_08123eae
	bl	.L_081247ec
.L_08123eae:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r7, #0
	movs	r1, #4
	bl	sub_08038120
	adds	r0, r7, #0
	bl	sub_080ad078
	ldrb	r2, [r0, #1]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #6
	bne.n	.L_08123ed8
	ldr	r0, [pc, #168]
	b.n	.L_08123eda
.L_08123ed8:
	ldr	r0, [pc, #168]
.L_08123eda:
	movs	r3, #246
	lsls	r3, r3, #1
	cmp	r7, r3
	beq.n	.L_08123fd6
	cmp	r7, r3
	bgt.n	.L_08123f30
	movs	r2, #186
	adds	r2, #255
	cmp	r7, r2
	bgt.n	.L_08123f0c
	subs	r3, #56
	cmp	r7, r3
	bgt.n	.L_08123fca
	cmp	r7, #224
	beq.n	.L_08123fb6
	cmp	r7, #224
	bge.n	.L_08123efe
	b.n	.L_08123c62
.L_08123efe:
	movs	r4, #217
	lsls	r4, r4, #1
	cmp	r7, r4
	bgt.n	.L_08123f08
	b.n	.L_08123c62
.L_08123f08:
	ldr	r0, [pc, #124]
	b.n	.L_08123c62
.L_08123f0c:
	movs	r3, #236
	lsls	r3, r3, #1
	cmp	r7, r3
	beq.n	.L_08123fce
	cmp	r7, r3
	bgt.n	.L_08123f26
	movs	r1, #222
	lsls	r1, r1, #1
	cmp	r7, r1
	ble.n	.L_08123f22
	b.n	.L_08123c62
.L_08123f22:
	ldr	r0, [pc, #96]
	b.n	.L_08123c62
.L_08123f26:
	movs	r2, #244
	lsls	r2, r2, #1
	cmp	r7, r2
	beq.n	.L_08123fd2
	b.n	.L_08123c62
.L_08123f30:
	movs	r3, #246
	adds	r3, #255
	cmp	r7, r3
	beq.n	.L_08123fbe
	cmp	r7, r3
	bgt.n	.L_08123f8c
	subs	r3, #6
	cmp	r7, r3
	beq.n	.L_08123fda
	cmp	r7, r3
	bgt.n	.L_08123f4e
	subs	r3, #1
	cmp	r7, r3
	beq.n	.L_08123fc6
	b.n	.L_08123c62
.L_08123f4e:
	movs	r4, #244
	adds	r4, #255
	cmp	r7, r4
	beq.n	.L_08123fc2
	movs	r1, #250
	lsls	r1, r1, #1
	cmp	r7, r1
	beq.n	.L_08123fba
	b.n	.L_08123c62
	.4byte 0x00000c65
	.4byte 0x00000c8b
	.4byte 0x00000c8c
	.4byte 0x00000c8a
	.4byte 0x00000c67
	.4byte 0x00000c62
	.4byte 0x00000c64
	.4byte 0x00000c63
	.4byte 0x00000d52
	.4byte 0x00000d51
	.2byte 0x0d53
	.2byte 0x0000
.L_08123f8c:
	movs	r3, #252
	lsls	r3, r3, #1
	cmp	r7, r3
	beq.n	.L_08123fe2
	cmp	r7, r3
	bgt.n	.L_08123fa2
	movs	r2, #248
	adds	r2, #255
	cmp	r7, r2
	beq.n	.L_08123fde
	b.n	.L_08123c62
.L_08123fa2:
	movs	r3, #254
	lsls	r3, r3, #1
	cmp	r7, r3
	beq.n	.L_08123fe6
	movs	r4, #239
	lsls	r4, r4, #1
	adds	r4, #255
	cmp	r7, r4
	beq.n	.L_08123fea
	b.n	.L_08123c62
.L_08123fb6:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fba:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fbe:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fc2:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fc6:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fca:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fce:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fd2:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fd6:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fda:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fde:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fe2:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fe6:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
.L_08123fea:
	ldr	r0, [pc, #56]
	b.n	.L_08123c62
	movs	r0, r0
	.4byte 0x00000c8a
	.4byte 0x00000d58
	.4byte 0x00000d59
	.4byte 0x00000d5a
	.4byte 0x00000d5b
	.4byte 0x00000d5c
	.4byte 0x00000d5d
	.4byte 0x00000d5e
	.4byte 0x00000d60
	.4byte 0x00000d5f
	.4byte 0x00000d61
	.4byte 0x00000d62
	.4byte 0x00000d63
	.2byte 0x0d65
	.2byte 0x0000
	mov	r0, sl
	ldr	r3, [r0, #0]
	ldr	r6, [pc, #52]
	ldrh	r3, [r3, #8]
	movs	r5, #255
	lsls	r0, r3, #16
	asrs	r0, r0, #24
	adds	r1, r5, #0
	ands	r1, r3
	ands	r0, r6
	bl	sub_080ad190
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r7, r0, #0
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldrh	r3, [r3, #8]
	adds	r2, r5, #0
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	ands	r1, r6
	ands	r2, r3
	bl	sub_080ad1b0
	cmp	r0, #0
	beq.n	.L_08124060
	b.n	.L_081241b0
.L_08124060:
	b.n	.L_08124068
	movs	r0, r0
	.2byte 0x000f
	.2byte 0x0000
.L_08124068:
	mov	r4, sl
	ldr	r3, [r4, #0]
	adds	r2, r5, #0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	ands	r1, r6
	ands	r2, r3
	bl	sub_080ad1b8
	cmp	r0, #0
	bne.n	.L_08124086
	b.n	.L_08124182
.L_08124086:
	adds	r0, r7, #0
	bl	sub_080ad078
	movs	r0, #0
	movs	r1, #0
	bl	sub_08126cfc
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r2, r5, #0
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	ands	r1, r6
	ands	r2, r3
	bl	sub_080ad158
	mov	r0, sl
	ldr	r3, [r0, #0]
	adds	r2, r5, #0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	ands	r1, r6
	ands	r2, r3
	bl	sub_080ad168
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_080ad008
	bl	sub_081234a4
	movs	r0, #30
	bl	sub_08122c88
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r0, #0
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_08120360
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r0, #150
	ldrh	r2, [r3, #8]
	lsls	r0, r0, #1
	lsls	r3, r2, #16
	asrs	r3, r3, #24
	ands	r3, r6
	lsls	r1, r3, #2
	adds	r1, r1, r3
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r1, r1, #2
	adds	r1, r1, r3
	adds	r1, r1, r0
	movs	r0, #3
	bl	sub_08120360
	movs	r0, #14
	movs	r1, #175
	bl	sub_08120360
	movs	r0, #10
	movs	r1, #0
	bl	sub_08120360
	ldr	r1, [pc, #788]
	movs	r0, #4
	bl	sub_08120360
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r0, #11
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_08120360
	movs	r0, #212
	bl	sub_081c0010
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_0811be3c
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	sub_08020090
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_0811be3c
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	sub_08020098
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r2, #3
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldrh	r1, [r3, #8]
	movs	r3, #0
	lsls	r1, r1, #16
	asrs	r1, r1, #24
	ands	r1, r6
	bl	sub_08127308
	bl	sub_081234f0
.L_0812417c:
	movs	r0, #2
	negs	r0, r0
	b.n	.L_081247ec
.L_08124182:
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r1, #1
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_08038120
	movs	r1, #4
	adds	r0, r7, #0
	bl	sub_08038120
	movs	r0, #114
	bl	sub_081c0010
	ldr	r0, [pc, #664]
	bl	sub_080381c8
	movs	r0, #60
	bl	sub_08013560
.L_081241aa:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_081247ec
.L_081241b0:
	add	r0, sp, #40
	mov	r9, r0
	adds	r0, r7, #0
	bl	.L_08123648
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_081241c4
	b.n	.L_081247ec
.L_081241c4:
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r2, r5, #0
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	ldrh	r3, [r3, #8]
	lsls	r1, r3, #16
	asrs	r1, r1, #24
	ands	r2, r3
	ands	r1, r6
	bl	sub_080ad170
	adds	r0, r7, #0
	bl	sub_080ad078
	adds	r5, r0, #0
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r7, #0
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #576]
	bl	sub_080381c8
	mov	r3, fp
	ldr	r2, [r3, #0]
	ldrb	r3, [r5, #2]
	str	r3, [r2, #80]
	b.n	.L_08124376
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #8
	ldrsh	r0, [r3, r1]
	bl	sub_080ad188
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r7, r0, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08124230
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	b.n	.L_08124232
.L_08124230:
	movs	r0, #0
.L_08124232:
	add	r1, sp, #16
	mov	r8, r1
	bl	sub_08123574
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r0, #0
	ldrh	r3, [r3, #0]
	cmp	r3, #7
	bls.n	.L_08124256
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08124256
	movs	r0, #1
.L_08124256:
	bl	sub_080ad000
	adds	r0, #8
	str	r0, [sp, #4]
	mov	r3, r8
	adds	r0, r7, #4
	ldrb	r2, [r3, #0]
	ldrb	r3, [r0, #0]
	movs	r6, #0
	cmp	r2, r3
	bcc.n	.L_0812428c
	movs	r4, #4
	movs	r5, #4
	mov	r1, r8
.L_08124272:
	ldrb	r3, [r7, r4]
	adds	r6, #1
	strb	r3, [r1, #0]
	adds	r5, #1
	adds	r1, #1
	cmp	r6, #3
	bgt.n	.L_0812428c
	adds	r0, #1
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	adds	r4, r5, #0
	cmp	r2, r3
	bcs.n	.L_08124272
.L_0812428c:
	ldrh	r7, [r7, #0]
	add	r4, sp, #40
	mov	r9, r4
	adds	r0, r7, #0
	bl	.L_08123648
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	bne.n	.L_081242a2
	b.n	.L_081247ec
.L_081242a2:
	cmp	r6, #4
	beq.n	.L_081242c6
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r7, #0
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #384]
	bl	sub_080381c8
	adds	r0, r5, #0
	b.n	.L_081247ec
.L_081242c6:
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r1, #1
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_08038120
	movs	r1, #4
	adds	r0, r7, #0
	bl	sub_08038120
	ldr	r0, [pc, #356]
	bl	sub_080381c8
	ldr	r0, [sp, #4]
	movs	r1, #144
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L_08124332
	mov	r9, r5
	adds	r5, r0, #0
.L_081242f6:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, r9
	bne.n	.L_08124320
	ldrb	r0, [r5, #2]
	bl	sub_08123534
	cmp	r0, #0
	beq.n	.L_08124320
	ldrb	r1, [r5, #0]
	mov	r3, r8
	ldrb	r2, [r3, r1]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124320
	movs	r3, #254
	strb	r3, [r5, #3]
	adds	r3, r2, #0
	adds	r3, #255
	mov	r4, r8
	strb	r3, [r4, r1]
.L_08124320:
	ldr	r0, [sp, #4]
	movs	r1, #144
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	adds	r6, #1
	adds	r5, #4
	cmp	r6, r3
	bne.n	.L_081242f6
.L_08124332:
	movs	r2, #201
	lsls	r2, r2, #1
	cmp	r7, r2
	bne.n	.L_08124376
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldrh	r3, [r3, #0]
	cmp	r3, #7
	bls.n	.L_08124364
	ldr	r0, [sp, #8]
	movs	r2, #128
	ldr	r3, [r0, #0]
	movs	r4, #128
	lsls	r2, r2, #4
	lsls	r4, r4, #4
	adds	r2, #98
	adds	r4, #101
	adds	r1, r3, r2
	adds	r3, r3, r4
	ldrb	r2, [r3, #0]
	movs	r3, #1
	lsls	r3, r2
	ldrh	r2, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_08124364:
	ldr	r0, [sp, #8]
	movs	r1, #128
	ldr	r2, [r0, #0]
	lsls	r1, r1, #4
	adds	r1, #101
	adds	r2, r2, r1
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
.L_08124376:
	cmp	r7, #1
	beq.n	.L_0812437c
	b.n	.L_081244e4
.L_0812437c:
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldrb	r0, [r3, #3]
	bl	sub_08016ca4
	mov	r3, fp
	ldr	r2, [r3, #0]
	mov	r4, sl
	movs	r3, #1
	str	r3, [r2, #76]
	ldr	r3, [r4, #0]
	adds	r6, r0, #0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_080ad118
	mov	r2, fp
	ldr	r1, [r2, #0]
	movs	r3, #2
	str	r0, [r1, #80]
	str	r3, [r1, #84]
	ldr	r3, [sp, #12]
	movs	r4, #42
	ldr	r2, [r3, #0]
	adds	r4, #255
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_081243d0
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrh	r0, [r3, #0]
	bl	sub_08128124
	mov	r1, fp
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	orrs	r3, r0
	str	r3, [r2, #88]
	b.n	.L_081243e6
.L_081243d0:
	movs	r3, #0
	str	r3, [r1, #88]
	movs	r4, #165
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrh	r0, [r3, #0]
	bl	sub_0811d79c
	mov	r1, fp
	ldr	r3, [r1, #0]
	str	r0, [r3, #88]
.L_081243e6:
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r1, #1
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_08038120
	ldr	r0, [pc, #80]
	bl	sub_080381c8
	b.n	.L_0812444c
.L_081243fc:
	ldr	r0, [sp, #12]
	movs	r1, #156
	ldr	r3, [r0, #0]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0812441e
	bl	sub_080ad148
	movs	r3, #255
	ands	r0, r3
	cmp	r0, #152
	bgt.n	.L_0812441e
	mov	r2, fp
	ldr	r3, [r2, #0]
	strb	r5, [r3, #31]
.L_0812441e:
	bl	sub_080ad148
	movs	r3, #31
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_08124484
	mov	r4, fp
	ldr	r3, [r4, #0]
	strb	r0, [r3, #31]
	b.n	.L_08124484
	movs	r0, r0
	.4byte 0x00000cf7
	.4byte 0x00000cb2
	.4byte 0x00000c90
	.4byte 0x00000c93
	.4byte 0x00000c92
	.2byte 0x0c60
	.2byte 0x0000
.L_0812444c:
	movs	r0, #56
	ldrsh	r3, [r6, r0]
	cmp	r3, #0
	beq.n	.L_08124484
	movs	r1, #158
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08124484
	movs	r2, #60
	adds	r2, #255
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08124484
	movs	r4, #70
	adds	r4, #255
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08124484
	movs	r0, #157
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_081243fc
.L_08124484:
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08124498
	mov	r1, fp
	ldr	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r2, #31]
.L_08124498:
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bne.n	.L_081244a2
	b.n	.L_081247be
.L_081244a2:
	bl	sub_080ad148
	movs	r3, #31
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_081244b4
	mov	r4, fp
	ldr	r3, [r4, #0]
	b.n	.L_081244dc
.L_081244b4:
	ldr	r1, [sp, #12]
	ldr	r0, [r1, #0]
	bl	sub_080ad2c0
	movs	r1, #200
	lsls	r0, r0, #16
	bl	sub_08002054
	adds	r5, r0, #0
	bl	sub_080ad148
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r0, r3
	cmp	r5, r0
	bgt.n	.L_081244d8
	b.n	.L_081247be
.L_081244d8:
	mov	r2, fp
	ldr	r3, [r2, #0]
.L_081244dc:
	movs	r2, #1
	adds	r3, #45
	strb	r2, [r3, #0]
	b.n	.L_081247be
.L_081244e4:
	adds	r0, r7, #0
	bl	sub_080ad078
	mov	r4, fp
	ldr	r3, [r4, #0]
	ldrb	r2, [r0, #2]
	str	r7, [r3, #76]
	str	r2, [r3, #80]
	movs	r2, #0
	str	r2, [r3, #88]
	ldrb	r2, [r0, #3]
	mov	r8, r0
	adds	r3, r2, #0
	cmp	r3, #65
	beq.n	.L_0812451a
	cmp	r3, #41
	beq.n	.L_0812451a
	cmp	r3, #42
	beq.n	.L_0812451a
	cmp	r3, #43
	beq.n	.L_0812451a
	cmp	r3, #44
	beq.n	.L_0812451a
	cmp	r3, #68
	beq.n	.L_0812451a
	cmp	r3, #89
	bne.n	.L_081245c4
.L_0812451a:
	adds	r3, r2, #0
	cmp	r3, #89
	bne.n	.L_0812455e
	bl	sub_080ad148
	movs	r3, #255
	ands	r0, r3
	cmp	r0, #178
	ble.n	.L_0812452e
	b.n	.L_081246b0
.L_0812452e:
	movs	r5, #1
	cmp	r0, #127
	bgt.n	.L_08124536
	movs	r5, #2
.L_08124536:
	mov	r0, fp
	ldr	r2, [r0, #0]
	movs	r1, #0
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	cmp	r1, r3
	blt.n	.L_08124546
	b.n	.L_081246b0
.L_08124546:
	adds	r0, r2, #0
	adds	r2, #31
.L_0812454a:
	ldrb	r3, [r2, #0]
	adds	r1, #1
	adds	r3, r3, r5
	strb	r3, [r2, #0]
	adds	r2, #1
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	cmp	r1, r3
	blt.n	.L_0812454a
	b.n	.L_081246b0
.L_0812455e:
	lsls	r3, r2, #24
	lsrs	r3, r3, #24
	cmp	r3, #65
	beq.n	.L_0812456a
	cmp	r3, #68
	bne.n	.L_0812456e
.L_0812456a:
	movs	r6, #153
	b.n	.L_0812457a
.L_0812456e:
	cmp	r3, #41
	beq.n	.L_08124578
	movs	r6, #64
	cmp	r3, #43
	bne.n	.L_0812457a
.L_08124578:
	movs	r6, #32
.L_0812457a:
	lsls	r3, r2, #24
	lsrs	r3, r3, #24
	cmp	r3, #65
	beq.n	.L_0812458c
	cmp	r3, #41
	beq.n	.L_0812458c
	movs	r5, #2
	cmp	r3, #42
	bne.n	.L_0812458e
.L_0812458c:
	movs	r5, #1
.L_0812458e:
	bl	sub_080ad148
	movs	r3, #255
	ands	r0, r3
	cmp	r0, r6
	blt.n	.L_0812459c
	b.n	.L_081246b0
.L_0812459c:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r1, #0
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	cmp	r1, r3
	blt.n	.L_081245ac
	b.n	.L_081246b0
.L_081245ac:
	adds	r0, r2, #0
	adds	r2, #31
.L_081245b0:
	ldrb	r3, [r2, #0]
	adds	r1, #1
	adds	r3, r3, r5
	strb	r3, [r2, #0]
	adds	r2, #1
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	cmp	r1, r3
	blt.n	.L_081245b0
	b.n	.L_081246b0
.L_081245c4:
	adds	r3, r2, #0
	adds	r3, #220
	movs	r4, #128
	lsls	r3, r3, #24
	lsls	r4, r4, #19
	cmp	r3, r4
	bhi.n	.L_08124638
	mov	r0, r8
	ldrb	r3, [r0, #3]
	subs	r3, #36
	cmp	r3, #4
	bhi.n	.L_08124608
	ldr	r2, [pc, #540]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x45f8
	lsrs	r2, r2, #32
	.2byte 0x45fc
	lsrs	r2, r2, #32
	.2byte 0x4600
	lsrs	r2, r2, #32
	.2byte 0x4604
	lsrs	r2, r2, #32
	.2byte 0x4608
	lsrs	r2, r2, #32
	movs	r5, #63
	b.n	.L_0812460a
	movs	r5, #31
	b.n	.L_0812460a
	movs	r5, #15
	b.n	.L_0812460a
	movs	r5, #7
	b.n	.L_0812460a
.L_08124608:
	movs	r5, #3
.L_0812460a:
	bl	sub_080ad148
	ands	r0, r5
	cmp	r0, #0
	bne.n	.L_081246b0
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r1, #0
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	cmp	r1, r3
	bge.n	.L_081246b0
	adds	r0, r2, #1
	movs	r4, #2
	adds	r2, #45
.L_08124628:
	strb	r4, [r2, #0]
	adds	r1, #1
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r2, #1
	cmp	r1, r3
	blt.n	.L_08124628
	b.n	.L_081246b0
.L_08124638:
	cmp	r7, #178
	beq.n	.L_0812464e
	movs	r4, #187
	lsls	r4, r4, #1
	cmp	r7, r4
	beq.n	.L_0812464e
	movs	r0, #164
	lsls	r3, r2, #24
	lsls	r0, r0, #23
	cmp	r3, r0
	bne.n	.L_081246b0
.L_0812464e:
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r5, #0
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r5, r3
	bge.n	.L_081246b0
	mov	r6, fp
.L_08124660:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #1
	bne.n	.L_0812467e
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #404]
	adds	r3, r3, r5
	ldrb	r3, [r3, #17]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bge.n	.L_0812467a
	negs	r3, r3
.L_0812467a:
	ldrb	r4, [r1, r3]
	b.n	.L_08124680
.L_0812467e:
	movs	r4, #100
.L_08124680:
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldr	r3, [r6, #0]
	adds	r3, #3
	ldrb	r1, [r3, r5]
	mov	r3, r8
	ldrb	r2, [r3, #2]
	ldrb	r3, [r3, #3]
	str	r4, [sp, #0]
	bl	sub_080ad120
	ldr	r1, [r6, #0]
	adds	r2, r5, #0
	adds	r2, #56
	adds	r3, r1, #3
	strb	r0, [r3, r2]
	adds	r5, #1
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	adds	r2, r6, #0
	cmp	r5, r3
	blt.n	.L_08124660
.L_081246b0:
	movs	r4, #156
	lsls	r4, r4, #1
	cmp	r7, r4
	bne.n	.L_081246ca
	mov	r0, fp
	ldr	r3, [r0, #0]
	ldrb	r0, [r3, #0]
	bl	sub_0811f5a4
	mov	r1, fp
	ldr	r3, [r1, #0]
	strb	r0, [r3, #2]
	b.n	.L_08124708
.L_081246ca:
	movs	r2, #158
	lsls	r2, r2, #1
	cmp	r7, r2
	bne.n	.L_08124708
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r5, #0
	ldrb	r3, [r2, #3]
	strb	r3, [r2, #2]
.L_081246dc:
	mov	r4, fp
	ldr	r3, [r4, #0]
	movs	r0, #0
	ldrb	r3, [r3, #2]
	cmp	r3, #7
	bhi.n	.L_081246ea
	movs	r0, #1
.L_081246ea:
	bl	sub_0811f4d4
	bl	sub_0811d720
	mov	r1, fp
	ldr	r3, [r1, #0]
	strb	r0, [r3, #3]
	ldrb	r3, [r3, #2]
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	cmp	r0, r3
	bne.n	.L_08124708
	adds	r5, #1
	cmp	r5, #9
	ble.n	.L_081246dc
.L_08124708:
	movs	r2, #183
	lsls	r2, r2, #2
	cmp	r7, r2
	bhi.n	.L_0812473c
	mov	r3, fp
	ldr	r1, [r3, #0]
	ldr	r2, [pc, #236]
	lsls	r3, r7, #2
	ldr	r2, [r2, r3]
	movs	r3, #31
	ldrsb	r3, [r1, r3]
	str	r2, [r1, #88]
	cmp	r3, #1
	ble.n	.L_0812472e
	ldr	r4, [pc, #224]
	lsls	r3, r3, #12
	adds	r3, r2, r3
	adds	r3, r3, r4
	str	r3, [r1, #88]
.L_0812472e:
	ldr	r1, [pc, #220]
	ldrb	r3, [r1, r7]
	cmp	r3, #0
	beq.n	.L_0812473c
	mov	r0, fp
	ldr	r2, [r0, #0]
	b.n	.L_08124772
.L_0812473c:
	adds	r0, r7, #0
	bl	sub_081224c0
	cmp	r0, #0
	beq.n	.L_0812474e
	mov	r1, fp
	ldr	r2, [r1, #0]
	movs	r3, #3
	b.n	.L_08124772
.L_0812474e:
	mov	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #88]
	cmp	r3, #0
	beq.n	.L_08124770
	ldr	r4, [sp, #12]
	movs	r0, #42
	ldr	r3, [r4, #0]
	adds	r0, #255
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0812476c
	movs	r3, #8
	b.n	.L_08124772
.L_0812476c:
	movs	r3, #3
	b.n	.L_08124772
.L_08124770:
	movs	r3, #1
.L_08124772:
	str	r3, [r2, #84]
	mov	r1, r8
	ldrb	r0, [r1, #3]
	bl	sub_080ad280
	cmp	r0, #0
	beq.n	.L_0812478e
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r1, #128
	ldr	r2, [r3, #88]
	lsls	r1, r1, #9
	orrs	r2, r1
	str	r2, [r3, #88]
.L_0812478e:
	cmp	r7, #178
	beq.n	.L_081247a2
	movs	r3, #187
	lsls	r3, r3, #1
	cmp	r7, r3
	beq.n	.L_081247a2
	mov	r4, r8
	ldrb	r3, [r4, #3]
	cmp	r3, #82
	bne.n	.L_081247be
.L_081247a2:
	mov	r0, fp
	ldr	r1, [r0, #0]
	adds	r3, r1, #0
	adds	r3, #59
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_081247be
	ldr	r3, [r1, #88]
	movs	r2, #128
	lsls	r2, r2, #5
	orrs	r3, r2
	str	r3, [r1, #88]
.L_081247be:
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r2, #6
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_081247dc
	mov	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #84]
	cmp	r3, #5
	beq.n	.L_081247dc
	cmp	r3, #9
	beq.n	.L_081247dc
	movs	r3, #4
	str	r3, [r2, #84]
.L_081247dc:
	mov	r0, sl
	ldr	r2, [r0, #0]
	mov	r4, fp
	ldr	r3, [r4, #0]
	ldrh	r2, [r2, #6]
	adds	r3, #74
	strh	r2, [r3, #0]
.L_081247ea:
	movs	r0, #0
.L_081247ec:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
