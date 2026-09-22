.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_08015778, 0x08015778
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08020090, 0x08020090
	.set sub_08020138, 0x08020138
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08163c2c, 0x08163c2c
	.set sub_08164abc, 0x08164abc
	.set sub_0818d776, 0x0818d776
	.set sub_0818dd52, 0x0818dd52
	.set sub_0818e6de, 0x0818e6de
	.set sub_0818e6f8, 0x0818e6f8
	.set sub_0818f620, 0x0818f620
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0818d5a8
Overlay_0818d5a8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	str	r0, [sp, #68]
	str	r1, [sp, #64]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #60]
	ldr	r1, [r3, #96]
	str	r1, [sp, #56]
	ldr	r3, [r3, #100]
	str	r3, [sp, #44]
	bl	sub_0813ba50
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r1, [sp, #44]
	ldr	r0, [pc, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #60]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #24]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #64]
	cmp	r4, #0
	bne.n	.L_0818d628
	b.n	.L_0818d614
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000134
	.2byte 0x013e
	.2byte 0x0000
.L_0818d614:
	ldr	r5, [sp, #60]
	movs	r2, #220
	lsls	r2, r2, #6
	adds	r1, r5, r2
	ldr	r0, [pc, #864]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_0818d65c
.L_0818d628:
	ldr	r3, [sp, #60]
	movs	r4, #220
	lsls	r4, r4, #6
	adds	r1, r3, r4
	ldr	r0, [pc, #848]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r5, [sp, #60]
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r1, r5, r2
	ldr	r0, [pc, #836]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #174
	lsls	r3, r3, #7
	adds	r1, r5, r3
	ldr	r0, [pc, #824]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_0818d65c:
	ldr	r0, [pc, #816]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #812]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c11
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0818d682
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_0818d68a
.L_0818d682:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_0818d68a:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r5, [sp, #60]
	movs	r0, #239
	lsls	r0, r0, #7
	str	r3, [sp, #48]
	adds	r2, r5, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #64]
	cmp	r1, #0
	bne.n	.L_0818d6b0
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r5, r3
	movs	r3, #75
	b.n	.L_0818d6bc
.L_0818d6b0:
	ldr	r4, [sp, #60]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #132
	adds	r2, r4, r5
	movs	r3, #50
.L_0818d6bc:
	str	r3, [r2, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #724]
	bl	sub_080145a8
	ldr	r3, [pc, #720]
	movs	r0, #0
	movs	r2, #128
	mov	r8, r0
	movs	r1, #0
	lsls	r2, r2, #1
.L_0818d6d4:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0818d6d4
	ldr	r5, [sp, #68]
	ldr	r0, [r5, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	mov	r9, r0
	movs	r1, #36
	ldrsh	r0, [r5, r1]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r2, #0
	str	r0, [sp, #40]
	str	r2, [sp, #36]
	str	r2, [sp, #32]
	mov	r3, r9
	ldr	r3, [r3, #8]
	mov	r4, r9
	str	r3, [sp, #28]
	mov	r5, sp
	ldr	r4, [r4, #16]
	adds	r5, #80
	str	r4, [sp, #24]
	str	r2, [sp, #20]
	str	r5, [sp, #16]
	mov	fp, r2
.L_0818d714:
	mov	r0, fp
	cmp	r0, #0
	bne.n	.L_0818d72a
	ldr	r1, [sp, #28]
	mov	r2, r9
	str	r1, [r2, #8]
	str	r0, [r2, #12]
	ldr	r3, [sp, #24]
	movs	r4, #0
	str	r3, [r2, #16]
	str	r4, [sp, #20]
.L_0818d72a:
	mov	r5, fp
	cmp	r5, #17
	bgt.n	.L_0818d734
	cmp	r5, #0
	bne.n	.L_0818d74c
.L_0818d734:
	ldr	r1, [sp, #68]
	ldr	r0, [r1, #8]
	ldr	r1, [sp, #16]
	bl	sub_0815e21c
	ldr	r2, [sp, #16]
	ldr	r4, [sp, #16]
	ldr	r3, [r2, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r4, #0]
.L_0818d74c:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	sub_0818d776
	ldr	r5, [sp, #16]
	ldr	r4, [sp, #60]
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	movs	r1, #32
	movs	r5, #224
	str	r1, [sp, #0]
	lsls	r5, r5, #3
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #16
	adds	r1, r4, r5
	subs	r3, #64
	ldr	r0, [sp, #56]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465a
	subs	r2, #4
	cmp	r2, #11
	bhi.n	.L_0818d7e0
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	ldr	r0, [sp, #60]
	asrs	r3, r3, #1
	lsls	r3, r3, #11
	movs	r5, #0
	adds	r0, r0, r3
	mov	r8, r5
	add	r7, sp, #80
	mov	sl, r0
.L_0818d792:
	mov	r1, r8
	lsls	r6, r1, #12
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r3, fp
	muls	r3, r0
	ldr	r5, [r7, #0]
	asrs	r3, r3, #16
	adds	r0, r6, #0
	adds	r5, r5, r3
	bl	sub_08002090
	mov	r2, fp
	muls	r2, r0
	ldr	r3, [r7, #4]
	asrs	r2, r2, #16
	adds	r3, r3, r2
	mov	r2, fp
	subs	r3, r3, r2
	movs	r2, #32
	subs	r5, #16
	str	r2, [sp, #0]
	movs	r1, #224
	movs	r2, #64
	str	r2, [sp, #4]
	lsls	r1, r1, #3
	adds	r2, r5, #0
	movs	r5, #1
	subs	r3, #64
	ldr	r0, [sp, #56]
	add	r1, sl
	ldr	r4, [sp, #48]
	add	r8, r5
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4640
	cmp	r0, #16
	bne.n	.L_0818d792
.L_0818d7e0:
	mov	r1, fp
	cmp	r1, #4
	bne.n	.L_0818d83c
	mov	r2, r9
	adds	r2, #90
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #153
	lsls	r3, r3, #8
	mov	r2, r9
	adds	r3, #153
	str	r3, [r2, #72]
	movs	r3, #240
	lsls	r3, r3, #12
	str	r3, [r2, #40]
	ldr	r3, [r2, #8]
	cmp	r3, #0
	bge.n	.L_0818d80a
	ldr	r3, [pc, #408]
	str	r3, [r2, #36]
	b.n	.L_0818d812
.L_0818d80a:
	movs	r3, #128
	lsls	r3, r3, #12
	mov	r4, r9
	str	r3, [r4, #36]
.L_0818d812:
	ldr	r5, [sp, #64]
	cmp	r5, #1
	bne.n	.L_0818d820
	mov	r0, r9
	ldr	r3, [r0, #36]
	negs	r3, r3
	str	r3, [r0, #36]
.L_0818d820:
	mov	r0, r9
	movs	r1, #2
	bl	sub_08020090
	movs	r3, #238
	ldr	r1, [sp, #60]
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #136
	bl	sub_081c0010
.L_0818d83c:
	mov	r4, fp
	cmp	r4, #16
	bne.n	.L_0818d89a
	movs	r3, #0
	mov	r5, r9
	str	r3, [r5, #72]
	ldr	r0, [sp, #64]
	cmp	r0, #0
	bne.n	.L_0818d856
	ldr	r1, [sp, #40]
	ldr	r3, [r1, #16]
	str	r3, [r5, #16]
	b.n	.L_0818d85a
.L_0818d856:
	mov	r2, r9
	str	r3, [r2, #16]
.L_0818d85a:
	ldr	r4, [sp, #40]
	mov	r5, r9
	ldr	r3, [r4, #16]
	movs	r1, #100
	str	r3, [r5, #16]
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	ldr	r3, [r4, #8]
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #2
	bl	sub_08002054
	ldr	r3, [r5, #8]
	movs	r1, #16
	subs	r0, r0, r3
	bl	sub_08002054
	str	r0, [sp, #36]
	ldr	r1, [sp, #40]
	ldr	r3, [r5, #12]
	ldr	r0, [r1, #12]
	movs	r1, #16
	subs	r0, r0, r3
	bl	sub_08002054
	str	r0, [sp, #32]
	mov	r0, r9
	bl	sub_08020138
.L_0818d89a:
	mov	r2, fp
	cmp	r2, #17
	bgt.n	.L_0818d8a2
	b.n	.L_0818dc96
.L_0818d8a2:
	mov	r3, r9
	ldr	r2, [r3, #12]
	cmp	r2, #0
	ble.n	.L_0818d8c0
	ldr	r3, [r3, #8]
	ldr	r4, [sp, #36]
	mov	r5, r9
	adds	r3, r3, r4
	str	r3, [r5, #8]
	ldr	r0, [sp, #32]
	adds	r3, r2, r0
	str	r3, [r5, #12]
	cmp	r3, #0
	ble.n	.L_0818d8c0
	b.n	.L_0818d9e6
.L_0818d8c0:
	ldr	r1, [sp, #20]
	cmp	r1, #0
	beq.n	.L_0818d8c8
	b.n	.L_0818d9e6
.L_0818d8c8:
	movs	r3, #0
	movs	r2, #1
	mov	r4, r9
	str	r2, [sp, #20]
	movs	r5, #80
	str	r3, [r4, #12]
	ldr	r7, [pc, #204]
	add	r5, sp
	mov	r8, r3
	mov	sl, r5
.L_0818d8dc:
	bl	sub_08014878
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r6, r0, #0
	adds	r3, #255
	mov	r0, sl
	ands	r6, r3
	ldr	r3, [r0, #0]
	adds	r5, #32
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	adds	r0, r6, #0
	subs	r3, #24
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r1, #1
	movs	r2, #128
	adds	r3, #32
	add	r8, r1
	lsls	r2, r2, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_0818d8dc
	ldr	r3, [sp, #60]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #4
	str	r3, [r2, #0]
	ldr	r5, [sp, #64]
	cmp	r5, #0
	bne.n	.L_0818d9a8
	movs	r0, #145
	bl	sub_081180e8
	ldr	r2, [sp, #68]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #4
	bl	sub_08118088
	ldr	r4, [sp, #68]
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	b.n	.L_0818d9e6
	movs	r0, r0
	.4byte 0x0000014c
	.4byte 0x000000e8
	.4byte 0x000000da
	.4byte 0x000000c1
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02016018
	.4byte 0xfff80000
	.2byte 0x6000
	.2byte 0x0201
.L_0818d9a8:
	movs	r0, #145
	bl	sub_081c0010
	ldr	r0, [sp, #68]
	movs	r5, #0
	ldr	r3, [r0, #20]
	mov	r8, r5
	cmp	r3, #0
	beq.n	.L_0818d9e6
	movs	r5, #36
.L_0818d9bc:
	ldr	r1, [sp, #68]
	ldrsh	r0, [r5, r1]
	movs	r1, #3
	bl	sub_08118088
	ldr	r3, [sp, #68]
	movs	r1, #7
	ldrsh	r0, [r5, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	bl	sub_0814cd48
	ldr	r1, [sp, #68]
	movs	r0, #1
	ldr	r3, [r1, #20]
	add	r8, r0
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0818d9bc
.L_0818d9e6:
	ldr	r2, [sp, #64]
	cmp	r2, #0
	bne.n	.L_0818da1a
	mov	r4, r9
	ldr	r3, [r4, #12]
	cmp	r3, #0
	bgt.n	.L_0818d9f6
	b.n	.L_0818dc96
.L_0818d9f6:
	ldr	r5, [sp, #16]
	ldr	r4, [sp, #60]
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	movs	r1, #40
	movs	r5, #220
	str	r1, [sp, #0]
	lsls	r5, r5, #6
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #20
	adds	r1, r4, r5
	subs	r3, #52
	ldr	r0, [sp, #56]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe13d
.L_0818da1a:
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014dac
	str	r0, [sp, #12]
	movs	r0, #1
	bl	sub_081969f8
	mov	r5, fp
	adds	r7, r0, #0
	movs	r1, #0
	cmp	r5, #40
	bne.n	.L_0818da42
	ldr	r0, [sp, #60]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r0, r3
	movs	r3, #14
	str	r3, [r2, #0]
.L_0818da42:
	ldr	r3, [sp, #72]
	ldr	r2, [pc, #868]
	movs	r4, #6
	ands	r3, r2
	ldr	r2, [pc, #868]
	orrs	r3, r4
	ldr	r5, [sp, #60]
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	movs	r0, #220
	orrs	r3, r2
	lsls	r0, r0, #6
	str	r3, [sp, #72]
	add	r6, sp, #72
	adds	r3, r5, r0
	str	r3, [r6, #4]
	ldr	r3, [pc, #844]
	str	r4, [r7, #0]
	str	r3, [r7, #8]
	str	r6, [r7, #16]
	ldr	r2, [sp, #12]
	mov	r3, fp
	str	r2, [r7, #12]
	str	r1, [r7, #20]
	cmp	r3, #45
	bgt.n	.L_0818dad2
	bl	sub_08014de4
	ldr	r4, [sp, #16]
	movs	r2, #0
	ldr	r0, [r4, #0]
	ldr	r1, [r4, #4]
	subs	r0, #60
	subs	r1, #80
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r5, #128
	bl	sub_08015160
	lsls	r5, r5, #8
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [sp, #68]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0818dab0
	ldr	r0, [pc, #780]
	bl	sub_080150e4
	b.n	.L_0818dabc
.L_0818dab0:
	adds	r0, r5, #0
	bl	sub_08015068
	ldr	r0, [pc, #768]
	bl	sub_080150e4
.L_0818dabc:
	ldr	r0, [pc, #764]
	bl	sub_0801521c
	ldr	r0, [pc, #764]
	ldr	r1, [sp, #12]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818dad2:
	mov	r2, sp
	movs	r1, #6
	adds	r2, #72
	strb	r1, [r6, #0]
	str	r2, [sp, #8]
	strb	r1, [r2, #1]
	ldr	r4, [sp, #60]
	movs	r5, #142
	lsls	r5, r5, #7
	adds	r3, r4, r5
	str	r3, [r2, #4]
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r3, [pc, #724]
	mov	r1, fp
	str	r3, [r7, #8]
	movs	r0, #0
	lsls	r1, r1, #3
	mov	r8, r0
	mov	sl, r1
.L_0818dafa:
	ldr	r3, [pc, #716]
	mov	r2, r8
	ldrb	r3, [r3, r2]
	adds	r2, r3, #0
	adds	r2, #40
	cmp	fp, r2
	ble.n	.L_0818dbb2
	mov	r4, fp
	subs	r3, r2, r4
	lsls	r3, r3, #3
	adds	r1, r3, #0
	adds	r1, #56
	cmp	r1, #0
	ble.n	.L_0818db18
	movs	r1, #0
.L_0818db18:
	movs	r5, #64
	negs	r5, r5
	cmp	r1, r5
	ble.n	.L_0818dbb2
	ldr	r3, [pc, #680]
	mov	r4, r8
	ldrb	r3, [r3, r4]
	mov	r0, fp
	subs	r2, r0, r2
	muls	r2, r3
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r2
	movs	r0, #128
	lsls	r6, r3, #4
	lsls	r0, r0, #8
	str	r1, [r7, #20]
	adds	r5, r6, r0
	bl	sub_08014de4
	movs	r2, #63
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r7, #25]
	ldr	r1, [sp, #16]
	ldr	r3, [pc, #640]
	ldr	r0, [r1, #0]
	mov	r2, r8
	ldrsb	r1, [r3, r2]
	subs	r0, #60
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	asrs	r0, r5, #1
	lsls	r1, r5, #2
	cmp	r5, #0
	bge.n	.L_0818db70
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r3, #3
	adds	r5, r6, r3
.L_0818db70:
	asrs	r2, r5, #2
	bl	sub_080151e4
	movs	r0, #250
	lsls	r0, r0, #2
	bl	sub_08015024
	movs	r3, #1
	mov	r4, r8
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0818db94
	lsls	r0, r4, #2
	add	r0, fp
	lsls	r0, r0, #11
	bl	sub_08015068
	b.n	.L_0818dba2
.L_0818db94:
	mov	r5, r8
	lsls	r0, r5, #2
	mov	r1, fp
	subs	r0, r0, r1
	lsls	r0, r0, #11
	bl	sub_08015068
.L_0818dba2:
	ldr	r0, [pc, #560]
	ldr	r1, [sp, #12]
	movs	r2, #32
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818dbb2:
	movs	r3, #1
	add	r8, r3
	movs	r2, #16
	mov	r4, r8
	add	sl, r2
	cmp	r4, #3
	bne.n	.L_0818dafa
	movs	r3, #0
	strb	r3, [r7, #25]
	ldr	r3, [pc, #528]
	movs	r0, #174
	str	r3, [r7, #8]
	ldr	r5, [sp, #60]
	ldr	r1, [sp, #8]
	lsls	r0, r0, #7
	adds	r3, r5, r0
	str	r3, [r1, #4]
	movs	r2, #7
	add	r3, sp, #72
	strb	r2, [r3, #0]
	strb	r2, [r1, #1]
	ldr	r6, [sp, #60]
	movs	r2, #0
	mov	r8, r2
	mov	sl, fp
.L_0818dbe4:
	ldr	r3, [pc, #500]
	mov	r4, r8
	ldrb	r3, [r3, r4]
	adds	r2, r3, #0
	adds	r2, #34
	cmp	fp, r2
	bne.n	.L_0818dbfc
	ldr	r5, [sp, #16]
	ldr	r3, [r5, #0]
	str	r3, [r6, #0]
	ldr	r3, [r5, #4]
	str	r3, [r6, #4]
.L_0818dbfc:
	cmp	fp, r2
	ble.n	.L_0818dc7a
	mov	r0, fp
	subs	r3, r2, r0
	lsls	r3, r3, #3
	adds	r1, r3, #0
	adds	r1, #40
	cmp	r1, #0
	ble.n	.L_0818dc10
	movs	r1, #0
.L_0818dc10:
	movs	r3, #64
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_0818dc7a
	ldr	r3, [pc, #452]
	mov	r5, r8
	ldrb	r3, [r3, r5]
	mov	r4, fp
	subs	r2, r4, r2
	muls	r2, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #220
	adds	r5, r2, #0
	muls	r5, r3
	str	r1, [r7, #20]
	movs	r3, #7
	mov	r1, sl
	ands	r3, r1
	lsls	r3, r3, #4
	movs	r0, #131
	lsls	r0, r0, #7
	strb	r3, [r7, #24]
	adds	r5, r5, r0
	bl	sub_08014de4
	ldr	r3, [pc, #412]
	ldr	r0, [r6, #0]
	mov	r2, r8
	ldrsb	r1, [r3, r2]
	subs	r0, #60
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	lsls	r1, r5, #1
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #250
	lsls	r0, r0, #3
	bl	sub_08015024
	ldr	r0, [pc, #380]
	ldr	r1, [sp, #12]
	movs	r2, #32
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818dc7a:
	movs	r4, #1
	add	r8, r4
	movs	r3, #5
	mov	r5, r8
	add	sl, r3
	adds	r6, #28
	cmp	r5, #2
	bne.n	.L_0818dbe4
	adds	r0, r7, #0
	bl	sub_08013164
	ldr	r0, [sp, #12]
	bl	sub_08013164
.L_0818dc96:
	ldr	r0, [sp, #64]
	cmp	r0, #1
	bne.n	.L_0818dce0
	mov	r1, fp
	cmp	r1, #50
	bne.n	.L_0818dce0
	movs	r0, #134
	bl	sub_081180e8
	ldr	r4, [sp, #68]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0818dce0
	movs	r5, #36
.L_0818dcb6:
	ldr	r1, [sp, #68]
	movs	r3, #128
	lsls	r3, r3, #9
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r3, #250
	str	r3, [sp, #4]
	movs	r2, #128
	movs	r3, #128
	lsls	r3, r3, #13
	movs	r1, #1
	lsls	r2, r2, #11
	bl	sub_0815f000
	ldr	r4, [sp, #68]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0818dcb6
.L_0818dce0:
	ldr	r7, [pc, #264]
	movs	r5, #0
	mov	r8, r5
.L_0818dce6:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	ble.n	sub_0818dd52
	subs	r3, #1
	movs	r2, #128
	adds	r0, r7, #0
	str	r3, [r7, #24]
	movs	r1, #56
	lsls	r2, r2, #6
	bl	sub_08138086
	ldr	r6, [r7, #4]
	movs	r0, #224
	lsls	r0, r0, #15
	cmp	r6, r0
	ble.n	.L_0818dd14
	ldr	r3, [r7, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #16]
	b.n	sub_0818dd52
.L_0818dd14:
	ldr	r5, [r7, #0]
	ldr	r1, [pc, #216]
	cmp	r5, r1
	bhi.n	sub_0818dd52
	cmp	r6, #0
	blt.n	sub_0818dd52
	ldr	r0, [r7, #24]
	cmp	r0, #0
	bge.n	.L_0818dd28
	adds	r0, #7
.L_0818dd28:
	asrs	r0, r0, #3
	adds	r0, #1
	ldr	r2, [pc, #196]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #44]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r1, r2, r1
	asrs	r2, r5, #16
	subs	r2, r2, r3
	asrs	r3, r6, #16
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #56]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2501
	movs	r0, #128
	add	r8, r5
	lsls	r0, r0, #1
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L_0818dce6
	movs	r0, #12
	movs	r1, #12
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r3, #240
	ldr	r1, [sp, #60]
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #1
	add	fp, r4
	mov	r5, fp
	cmp	r5, #88
	beq.n	.L_0818dd8c
	b.n	.L_0818d714
.L_0818dd8c:
	ldr	r0, [pc, #104]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0xffffcc78
	.4byte 0x000286a0
	.4byte 0x081991e0
	.4byte 0x081990d0
	.4byte 0x08199e58
	.4byte 0x08199e5c
	.4byte 0x08199e60
	.4byte 0x08199090
	.4byte 0x08198ec4
	.4byte 0x08199e64
	.4byte 0x08199e68
	.4byte 0x08199e6c
	.4byte 0x08198cac
	.4byte 0x02016000
	.4byte 0x007effff
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r1, #160
	ldrh	r2, [r3, #10]
	movs	r2, #236
	lsls	r2, r2, #7
	adds	r2, #64
	adds	r0, r0, r2
	lsls	r1, r1, #19
	ldr	r2, [pc, #8]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.2byte 0x0001
	.2byte 0xa260
.L_0818de3c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r0, [sp, #40]
	movs	r0, #0
	mov	r9, r1
	str	r2, [sp, #36]
	str	r3, [sp, #32]
	str	r0, [sp, #28]
	cmp	r2, #0
	beq.n	.L_0818df32
	ldr	r1, [sp, #76]
	str	r0, [sp, #12]
	subs	r1, #1
	str	r1, [sp, #16]
	str	r0, [sp, #8]
.L_0818de66:
	movs	r2, #0
	mov	r3, r9
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_0818df12
	ldr	r0, [sp, #36]
	ldr	r2, [sp, #12]
	subs	r0, #1
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #16]
	str	r0, [sp, #20]
	str	r2, [sp, #24]
	mov	r8, r1
	mov	fp, r3
.L_0818de82:
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #20]
	cmp	r0, r1
	bge.n	.L_0818df08
	mov	r3, r8
	add	r3, sl
	lsls	r5, r3, #1
	ldr	r2, [sp, #40]
	add	r0, sp, #24
	adds	r5, r5, r3
	ldr	r3, [sp, #32]
	ldrb	r0, [r0, #0]
	lsls	r5, r5, #3
	adds	r5, r5, r2
	mov	r1, sl
	muls	r1, r3
	strb	r0, [r5, #5]
	add	r0, sp, #24
	mov	r2, fp
	ldrb	r0, [r0, #0]
	strb	r2, [r5, #7]
	strb	r2, [r5, #9]
	adds	r3, r1, r3
	adds	r2, r5, #0
	adds	r2, #12
	subs	r3, #1
	mov	r7, sl
	strb	r3, [r5, #4]
	strb	r1, [r5, #6]
	strb	r3, [r5, #8]
	adds	r7, #1
	strb	r1, [r2, #4]
	strb	r1, [r2, #6]
	strb	r3, [r2, #8]
	mov	r1, fp
	adds	r3, r0, #0
	strb	r0, [r2, #5]
	strb	r1, [r2, #7]
	strb	r3, [r2, #9]
	mov	r1, r9
	adds	r0, r7, #0
	str	r2, [sp, #4]
	bl	sub_08002064
	adds	r6, r0, #0
	mov	r0, r8
	adds	r4, r6, r0
	strb	r4, [r5, #0]
	mov	r1, r9
	mov	r0, sl
	str	r4, [sp, #0]
	bl	sub_08002064
	mov	r1, r9
	ldr	r2, [sp, #4]
	ldr	r4, [sp, #0]
	adds	r3, r0, r1
	add	r6, r9
	add	r3, r8
	add	r6, r8
	add	r0, r8
	strb	r3, [r5, #1]
	strb	r6, [r5, #2]
	strb	r0, [r2, #0]
	strb	r3, [r2, #1]
	strb	r4, [r2, #2]
	b.n	.L_0818df0c
.L_0818df08:
	mov	r7, sl
	adds	r7, #1
.L_0818df0c:
	mov	sl, r7
	cmp	sl, r9
	bne.n	.L_0818de82
.L_0818df12:
	ldr	r3, [sp, #76]
	ldr	r2, [sp, #16]
	ldr	r0, [sp, #12]
	adds	r2, r2, r3
	ldr	r1, [sp, #8]
	str	r2, [sp, #16]
	ldr	r2, [sp, #28]
	adds	r0, r0, r3
	ldr	r3, [sp, #36]
	add	r1, r9
	adds	r2, #1
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #28]
	cmp	r2, r3
	bne.n	.L_0818de66
.L_0818df32:
	ldr	r0, [sp, #36]
	movs	r2, #0
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	subs	r3, #24
	mov	r1, r9
	muls	r1, r3
	ldr	r0, [sp, #40]
	adds	r3, r1, #0
	strb	r2, [r0, r3]
	adds	r3, r0, r3
	strb	r2, [r3, #1]
	strb	r2, [r3, #2]
	add	sp, #44
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
	sub	sp, #208
	str	r0, [sp, #104]
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r1, [r0, #96]
	movs	r6, #240
	str	r1, [sp, #100]
	mov	r8, r0
	ldr	r3, [r0, #92]
	lsls	r6, r6, #7
	str	r3, [sp, #96]
	adds	r6, #240
	ldr	r4, [r0, #100]
	ldr	r0, [sp, #104]
	adds	r7, r3, r6
	ldr	r2, [pc, #96]
	str	r4, [sp, #84]
	str	r0, [r7, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	mov	fp, r2
	bl	sub_081435e0
	movs	r2, #128
	movs	r1, #128
	lsls	r2, r2, #19
	lsls	r1, r1, #1
	adds	r2, #32
	strh	r1, [r2, #0]
	bl	sub_0813ba50
	ldr	r2, [pc, #60]
	movs	r3, #160
	movs	r6, #160
	lsls	r3, r3, #19
	lsls	r6, r6, #19
	adds	r3, #2
	strh	r2, [r6, #0]
	strh	r2, [r3, #0]
	ldr	r4, [sp, #96]
	movs	r0, #239
	ldr	r5, [pc, #48]
	lsls	r0, r0, #7
	adds	r3, r4, r0
	movs	r1, #0
	str	r1, [r3, #0]
	mov	r9, r1
	movs	r1, #200
	adds	r0, r5, #0
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r1, #0
	movs	r0, #0
	bl	sub_08163c2c
	adds	r0, r5, #0
	bl	sub_08014644
	ldr	r2, [sp, #96]
	movs	r3, #224
	b.n	.L_0818dff4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02010000
	.2byte 0x3001
	.2byte 0x0814
.L_0818dff4:
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #196]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #96]
	movs	r2, #156
	lsls	r2, r2, #6
	adds	r1, r4, r2
	ldr	r0, [pc, #184]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r4, #128
	ldr	r3, [sp, #96]
	lsls	r4, r4, #4
	adds	r4, #194
	adds	r1, r3, r4
	ldr	r0, [sp, #84]
	ldr	r5, [pc, #164]
	movs	r2, #30
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4828
	bl	sub_08013300
	movs	r2, #128
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0xf7cd
	.2byte 0xf9ea
	.2byte 0x4a24
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [pc, #136]
	movs	r1, #128
	mov	sl, r6
	mov	r0, fp
	lsls	r1, r1, #8
	ldr	r2, [pc, #132]
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x4820
	movs	r2, #240
	mov	r1, fp
	lsls	r2, r2, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6838
	bl	sub_0814cc4c
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #138
	movs	r0, #1
	movs	r2, #1
	bl	sub_08152404
	movs	r6, #128
	movs	r2, #128
	ldr	r3, [pc, #56]
	lsls	r6, r6, #19
	movs	r1, #128
	lsls	r2, r2, #19
	adds	r6, #80
	mov	r0, r9
	lsls	r1, r1, #1
	adds	r2, #32
	strh	r0, [r6, #0]
	strh	r1, [r2, #0]
	subs	r2, #22
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [sp, #100]
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x480e
	movs	r2, #128
	ldr	r1, [sp, #100]
	lsls	r2, r2, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21c8
	b.n	.L_0818e0e4
	movs	r0, r0
	.4byte 0x00001f81
	.4byte 0x00002787
	.4byte 0x000000af
	.4byte 0x000000cd
	.4byte 0x03000730
	.4byte 0x00000148
	.4byte 0x030011e0
	.4byte 0x03000260
	.4byte 0x01010101
	.4byte 0x06008000
	.2byte 0x4000
	.2byte 0x0600
.L_0818e0e4:
	lsls	r1, r1, #4
	ldr	r0, [pc, #72]
	bl	sub_080145a8
	ldr	r3, [pc, #56]
	movs	r2, #128
	strh	r3, [r6, #0]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [sp, #96]
	movs	r4, #188
	movs	r6, #204
	movs	r0, #212
	lsls	r4, r4, #7
	lsls	r6, r6, #7
	lsls	r0, r0, #7
	adds	r4, #64
	adds	r6, #64
	adds	r0, #64
	adds	r4, r3, r4
	adds	r6, r3, r6
	adds	r0, r3, r0
	str	r0, [sp, #72]
	str	r4, [sp, #80]
	str	r6, [sp, #76]
	movs	r1, #176
	add	r8, r1
	mov	r2, r8
	movs	r4, #224
	ldr	r2, [r2, #0]
	lsls	r4, r4, #4
	b.n	.L_0818e134
	.4byte 0x00003f46
	.4byte 0x00001010
	.2byte 0x3175
	.2byte 0x0814
.L_0818e134:
	adds	r4, #164
	movs	r0, #128
	adds	r4, r3, r4
	lsls	r0, r0, #9
	movs	r3, #2
	movs	r6, #0
	str	r2, [sp, #68]
	str	r0, [sp, #40]
	str	r3, [sp, #0]
	ldr	r0, [sp, #72]
	movs	r1, #2
	movs	r2, #16
	movs	r3, #32
	str	r4, [sp, #64]
	str	r6, [sp, #60]
	str	r6, [sp, #56]
	str	r6, [sp, #52]
	str	r6, [sp, #48]
	str	r6, [sp, #44]
	bl	.L_0818de3c
	mov	fp, r6
	mov	sl, r6
.L_0818e162:
	mov	r0, sl
	bl	sub_08002090
	negs	r0, r0
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r3, r3, #16
	mov	r0, sl
	mov	r8, r3
	bl	sub_08002096
	lsls	r3, r0, #1
	ldr	r6, [sp, #76]
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r7, r3, #16
	movs	r4, #0
	add	r6, r9
.L_0818e188:
	lsls	r5, r4, #13
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	mov	r1, r8
	asrs	r3, r3, #16
	strb	r3, [r6, #0]
	strb	r1, [r6, #1]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r4, [sp, #8]
	asrs	r3, r3, #16
	adds	r4, #1
	strb	r3, [r6, #2]
	adds	r6, #4
	cmp	r4, #2
	bne.n	.L_0818e188
	movs	r2, #128
	movs	r6, #1
	lsls	r2, r2, #3
	add	fp, r6
	adds	r2, #68
	movs	r3, #8
	mov	r0, fp
	add	sl, r2
	add	r9, r3
	cmp	r0, #16
	bne.n	.L_0818e162
	movs	r1, #8
	movs	r2, #7
	movs	r3, #3
	movs	r0, #104
	str	r6, [sp, #0]
	str	r4, [sp, #8]
	bl	sub_08196404
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r1, [sp, #96]
	str	r3, [sp, #88]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r5, #3
	str	r5, [r2, #0]
	ldr	r0, [sp, #96]
	adds	r3, #4
	adds	r1, r0, r3
	ldr	r3, [pc, #152]
	ldr	r4, [sp, #8]
	str	r3, [r1, #0]
	movs	r3, #50
	str	r4, [r2, #0]
	str	r3, [r1, #0]
	movs	r1, #192
	lsls	r1, r1, #4
	ldr	r0, [pc, #140]
	adds	r1, #254
	bl	sub_080145a8
	ldr	r2, [pc, #136]
	movs	r3, #0
	strh	r3, [r2, #4]
	ldr	r4, [sp, #68]
	movs	r7, #0
	str	r6, [r4, #16]
	ldr	r6, [sp, #64]
	ldr	r0, [sp, #64]
	adds	r6, #12
	mov	sl, r6
	mov	r8, r0
.L_0818e224:
	movs	r5, #255
	ands	r5, r7
	lsls	r5, r5, #7
	adds	r0, r5, #0
	bl	sub_08002096
	movs	r6, #160
	adds	r3, r0, #0
	muls	r3, r6
	mov	r1, r8
	str	r3, [r1, #4]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r0, #0
	muls	r3, r6
	asrs	r6, r3, #16
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	mov	r2, r8
	str	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	mov	r4, r8
	str	r3, [r4, #8]
	movs	r3, #31
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_0818e29c
	adds	r0, r7, #0
	cmp	r7, #0
	bge.n	.L_0818e280
	adds	r0, #31
.L_0818e280:
	asrs	r0, r0, #5
	movs	r1, #7
	bl	sub_08002064
	mov	r6, sl
	str	r0, [r6, #0]
	b.n	.L_0818e2a2
	movs	r0, r0
	.4byte 0x04040404
	.4byte 0x0818ddfd
	.2byte 0x1120
	.2byte 0x0300
.L_0818e29c:
	movs	r3, #7
	mov	r0, sl
	str	r3, [r0, #0]
.L_0818e2a2:
	movs	r2, #128
	movs	r1, #16
	adds	r7, #1
	lsls	r2, r2, #1
	add	sl, r1
	add	r8, r1
	cmp	r7, r2
	bne.n	.L_0818e224
	ldr	r1, [pc, #176]
	movs	r3, #0
	mov	fp, r3
	ldr	r3, [r1, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0818e2c4
	b.n	.L_0818e7fe
.L_0818e2c4:
	ldr	r3, [r1, #12]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0818e2d0
	b.n	.L_0818e7fe
.L_0818e2d0:
	mov	r4, sp
	mov	r6, sp
	adds	r4, #124
	adds	r6, #156
	str	r4, [sp, #20]
	str	r6, [sp, #16]
.L_0818e2dc:
	mov	r0, fp
	cmp	r0, #0
	bne.n	.L_0818e314
	movs	r1, #136
	lsls	r1, r1, #17
	ldr	r0, [sp, #96]
	str	r1, [sp, #52]
	movs	r1, #238
	movs	r2, #176
	movs	r3, #128
	lsls	r1, r1, #7
	lsls	r2, r2, #15
	lsls	r3, r3, #3
	movs	r4, #208
	movs	r6, #148
	adds	r1, #140
	str	r2, [sp, #48]
	str	r3, [sp, #44]
	adds	r2, r0, r1
	lsls	r4, r4, #4
	lsls	r6, r6, #6
	movs	r3, #8
	str	r4, [sp, #60]
	str	r6, [sp, #56]
	movs	r0, #162
	str	r3, [r2, #0]
	bl	sub_081c0010
.L_0818e314:
	mov	r2, fp
	cmp	r2, #120
	bne.n	.L_0818e320
	movs	r0, #163
	bl	sub_081c0010
.L_0818e320:
	mov	r1, fp
	subs	r1, #96
	cmp	r1, #16
	bhi.n	.L_0818e338
	ldr	r2, [pc, #44]
	movs	r3, #128
	subs	r2, r2, r1
	ldr	r1, [pc, #44]
	lsls	r3, r3, #19
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_0818e338:
	mov	r3, fp
	cmp	r3, #115
	bne.n	.L_0818e348
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
.L_0818e348:
	mov	r4, fp
	cmp	r4, #116
	bne.n	.L_0818e3e6
	ldr	r0, [pc, #24]
	bl	sub_08014644
	movs	r1, #200
	b.n	.L_0818e36c
	.4byte 0x00000010
	.4byte 0x00001000
	.4byte 0x00001010
	.4byte 0x03001150
	.2byte 0x3175
	.2byte 0x0814
.L_0818e36c:
	lsls	r1, r1, #4
	ldr	r0, [pc, #256]
	bl	sub_080145a8
	ldr	r1, [pc, #252]
	ldr	r6, [pc, #256]
	ldrh	r3, [r6, #0]
	adds	r0, r3, #0
	movs	r2, #130
	ldr	r3, [pc, #248]
	lsls	r2, r2, #2
	strh	r2, [r3, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0818e3a8
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r2, #128
	stmia	r3!, {r2}
	lsls	r2, r2, #19
	adds	r2, #32
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0818e3a8:
	ldr	r4, [pc, #204]
	strh	r0, [r4, #0]
	ldrh	r3, [r4, #0]
	adds	r0, r3, #0
	movs	r6, #130
	ldr	r2, [pc, #196]
	lsls	r6, r6, #2
	strh	r6, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0818e3e2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #224
	adds	r3, r3, r1
	lsls	r2, r2, #3
	adds	r3, #4
	adds	r2, #132
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0818e3e2:
	ldr	r3, [pc, #148]
	strh	r0, [r3, #0]
.L_0818e3e6:
	mov	r4, fp
	cmp	r4, #128
	ble.n	.L_0818e3f2
	ldr	r0, [pc, #140]
	bl	sub_0815f0a0
.L_0818e3f2:
	mov	r6, fp
	cmp	r6, #117
	bne.n	.L_0818e420
	ldr	r0, [sp, #96]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #140
	adds	r2, r0, r1
	movs	r3, #45
	str	r3, [r2, #0]
	movs	r2, #1
	adds	r3, r0, #0
	movs	r7, #0
	negs	r2, r2
	adds	r3, #24
.L_0818e410:
	adds	r7, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_0818e410
	movs	r2, #128
	lsls	r2, r2, #9
	str	r2, [sp, #40]
.L_0818e420:
	movs	r3, #168
	lsls	r3, r3, #3
	adds	r3, #255
	cmp	fp, r3
	bgt.n	.L_0818e4d2
	ldr	r4, [sp, #96]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #140
	adds	r1, r4, r2
	ldr	r2, [r1, #0]
	movs	r6, #236
	lsrs	r3, r2, #31
	lsls	r6, r6, #7
	adds	r3, r2, r3
	asrs	r3, r3, #1
	adds	r6, #64
	adds	r0, r4, r6
	adds	r4, r3, #0
	mov	r3, fp
	adds	r4, #16
	cmp	r3, #79
	ble.n	.L_0818e45a
	adds	r3, r2, #1
	str	r3, [r1, #0]
	cmp	r3, #236
	ble.n	.L_0818e45a
	movs	r3, #236
	str	r3, [r1, #0]
.L_0818e45a:
	ldr	r3, [pc, #16]
	movs	r7, #0
.L_0818e45e:
	adds	r7, #1
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r7, #15
	bne.n	.L_0818e45e
	b.n	.L_0818e480
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x08143001
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x0148
	.2byte 0x0000
.L_0818e480:
	adds	r1, r7, #0
	subs	r1, #16
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_0818e48e
	adds	r3, r7, #0
	subs	r3, #13
.L_0818e48e:
	asrs	r3, r3, #2
	adds	r2, r3, r4
	adds	r3, r2, #0
	adds	r1, r2, #0
	subs	r3, #32
	subs	r1, #80
	cmp	r3, #0
	bge.n	.L_0818e4a0
	movs	r3, #0
.L_0818e4a0:
	cmp	r3, #31
	ble.n	.L_0818e4a6
	movs	r3, #31
.L_0818e4a6:
	cmp	r1, #0
	bge.n	.L_0818e4ac
	movs	r1, #0
.L_0818e4ac:
	cmp	r1, #31
	ble.n	.L_0818e4b2
	movs	r1, #31
.L_0818e4b2:
	lsls	r2, r1, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	asrs	r2, r1, #1
	orrs	r3, r2
	adds	r7, #1
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r7, #135
	bne.n	.L_0818e480
	ldr	r3, [pc, #48]
.L_0818e4c8:
	adds	r7, #1
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r7, #160
	bne.n	.L_0818e4c8
.L_0818e4d2:
	mov	r4, fp
	cmp	r4, #115
	ble.n	.L_0818e5c8
	movs	r0, #1
	bl	sub_081969f8
	adds	r5, r0, #0
	ldr	r0, [pc, #24]
	mov	r6, fp
	lsls	r3, r6, #2
	adds	r1, r3, r0
	cmp	r1, #0
	ble.n	.L_0818e4ee
	movs	r1, #0
.L_0818e4ee:
	ldr	r3, [sp, #132]
	ldr	r2, [pc, #12]
	movs	r4, #156
	ands	r3, r2
	b.n	.L_0818e504
	.4byte 0x00000000
	.4byte 0xfffffdf0
	.2byte 0xff00
	.2byte 0xffff
.L_0818e504:
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #788]
	lsls	r4, r4, #6
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #96]
	str	r3, [sp, #132]
	adds	r3, r2, r4
	add	r2, sp, #132
	str	r3, [r2, #4]
	movs	r3, #6
	str	r3, [r5, #0]
	str	r2, [r5, #16]
	ldr	r6, [sp, #72]
	movs	r3, #0
	str	r6, [r5, #8]
	ldr	r0, [sp, #80]
	str	r1, [r5, #20]
	mov	r1, fp
	str	r0, [r5, #12]
	strb	r3, [r5, #24]
	cmp	r1, #127
	ble.n	.L_0818e55c
	ldr	r2, [pc, #744]
	movs	r4, #192
	lsls	r3, r1, #6
	lsls	r4, r4, #2
	adds	r3, r3, r2
	adds	r4, #255
	cmp	r3, r4
	ble.n	.L_0818e54c
	movs	r3, #128
	lsls	r3, r3, #3
.L_0818e54c:
	ldr	r6, [sp, #40]
	adds	r6, r6, r3
	adds	r0, r6, #0
	adds	r1, r0, #0
	adds	r2, r1, #0
	str	r6, [sp, #40]
	bl	sub_080151e4
.L_0818e55c:
	ldr	r0, [pc, #712]
	mov	r1, fp
	movs	r7, #0
	mov	r8, r0
	lsls	r6, r1, #1
.L_0818e566:
	movs	r3, #127
	bics	r3, r6
	strb	r3, [r5, #25]
	bl	sub_08014de4
	ldr	r0, [pc, #696]
	ldr	r1, [pc, #700]
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #164
	lsls	r0, r0, #8
	movs	r2, #128
	adds	r0, #16
	ldr	r1, [pc, #688]
	lsls	r2, r2, #9
	bl	sub_080151e4
	ldr	r0, [sp, #40]
	adds	r7, #1
	adds	r1, r0, #0
	adds	r2, r1, #0
	bl	sub_080151e4
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_080150e4
	mov	r0, r8
	bl	sub_08015068
	movs	r2, #32
	ldr	r0, [sp, #76]
	ldr	r1, [sp, #80]
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	movs	r2, #192
	lsls	r2, r2, #5
	add	r8, r2
	adds	r6, #20
	cmp	r7, #5
	bne.n	.L_0818e566
	adds	r0, r5, #0
	bl	sub_08013164
	b.n	sub_0818e6f8
.L_0818e5c8:
	mov	r3, fp
	cmp	r3, #159
	bgt.n	.L_0818e5da
	ldr	r4, [sp, #60]
	movs	r6, #128
	lsls	r3, r4, #16
	lsls	r6, r6, #17
	adds	r3, r3, r6
	b.n	.L_0818e5f2
.L_0818e5da:
	mov	r0, fp
	cmp	r0, #191
	bgt.n	.L_0818e5f6
	ldr	r1, [sp, #60]
	lsls	r2, r0, #3
	lsls	r3, r1, #16
	lsrs	r3, r3, #16
	subs	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #3
	adds	r3, r3, r2
	lsls	r3, r3, #16
.L_0818e5f2:
	asrs	r3, r3, #16
	str	r3, [sp, #60]
.L_0818e5f6:
	mov	r3, fp
	cmp	r3, #47
	bgt.n	.L_0818e608
	ldr	r4, [sp, #56]
	movs	r6, #128
	lsls	r3, r4, #16
	lsls	r6, r6, #17
	adds	r3, r3, r6
	b.n	.L_0818e620
.L_0818e608:
	mov	r0, fp
	cmp	r0, #111
	bgt.n	.L_0818e624
	ldr	r1, [sp, #56]
	lsls	r2, r0, #2
	lsls	r3, r1, #16
	lsrs	r3, r3, #16
	subs	r3, r3, r2
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #16
.L_0818e620:
	asrs	r3, r3, #16
	str	r3, [sp, #56]
.L_0818e624:
	add	r5, sp, #196
	movs	r3, #120
	str	r3, [r5, #0]
	movs	r3, #64
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	bl	sub_08014de4
	adds	r0, r5, #0
	bl	sub_08015128
	ldr	r3, [sp, #60]
	mov	r6, fp
	lsls	r0, r3, #16
	lsrs	r0, r0, #16
	bl	sub_08015024
	ldr	r4, [sp, #56]
	lsls	r0, r4, #16
	lsrs	r0, r0, #16
	bl	sub_08015068
	cmp	r6, #112
	bgt.n	.L_0818e6ec
	add	r0, sp, #184
	movs	r1, #172
	ldr	r6, [sp, #64]
	mov	r8, r0
	add	r1, sp
	movs	r7, #0
	mov	sl, r1
	mov	r9, r8
.L_0818e666:
	movs	r2, #2
	ldrsh	r3, [r6, r2]
	mov	r4, sl
	ldr	r5, [r6, #12]
	str	r3, [r4, #0]
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	mov	r0, sl
	str	r3, [r4, #4]
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	mov	r1, r9
	str	r3, [r4, #8]
	bl	sub_08015778
	mov	r2, r9
	ldr	r3, [r2, #8]
	cmp	r3, #0
	ble.n	sub_0818e6de
	cmp	r5, #7
	bne.n	.L_0818e6b4
	movs	r1, #30
	adds	r0, r7, #0
	bl	sub_08002064
	ldr	r3, [sp, #84]
	mov	r4, r8
	adds	r1, r0, #0
	movs	r0, #1
	ldr	r2, [r4, #0]
	adds	r1, r3, r1
	ldr	r3, [r4, #4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #88]
	ldr	r0, [pc, #392]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe014
.L_0818e6b4:
	ldr	r2, [pc, #388]
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	ldr	r0, [sp, #96]
	movs	r2, #224
	adds	r1, r0, r1
	ldr	r0, [pc, #380]
	lsls	r2, r2, #3
	ldrb	r0, [r0, r5]
	mov	r3, r8
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #368]
	ldr	r4, [sp, #88]
	ldrb	r0, [r0, r5]
	str	r0, [sp, #4]
	ldr	r0, [pc, #348]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2080
	adds	r7, #1
	lsls	r0, r0, #1
	adds	r6, #16
	cmp	r7, r0
	bne.n	.L_0818e666
	b.n	sub_0818e6f8
.L_0818e6ec:
	movs	r1, #240
	ldr	r3, [pc, #344]
	ldr	r0, [pc, #324]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x990b
	ldr	r2, [sp, #20]
	movs	r3, #0
	str	r1, [r2, #4]
	ldr	r4, [sp, #16]
	str	r1, [sp, #124]
	str	r3, [r4, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	ldr	r6, [sp, #52]
	movs	r2, #238
	str	r6, [r4, #0]
	ldr	r0, [sp, #48]
	lsls	r2, r2, #7
	str	r0, [r4, #8]
	ldr	r1, [sp, #96]
	adds	r2, #220
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #16]
	movs	r3, #0
	ldr	r2, [sp, #20]
	bl	sub_08020010
	movs	r5, #128
	movs	r6, #192
	mov	r3, fp
	lsls	r5, r5, #4
	lsls	r6, r6, #7
	cmp	r3, #23
	ble.n	.L_0818e752
	lsls	r3, r3, #3
	add	r3, fp
	lsls	r3, r3, #4
	mov	r4, fp
	subs	r3, r3, r4
	lsls	r3, r3, #1
	movs	r0, #143
	subs	r3, r6, r3
	lsls	r0, r0, #5
	adds	r6, r3, r0
	cmp	r6, #0
	bge.n	.L_0818e752
	movs	r6, #0
.L_0818e752:
	mov	r1, fp
	cmp	r1, #87
	ble.n	.L_0818e768
	lsls	r3, r1, #6
	movs	r2, #176
	subs	r3, r5, r3
	lsls	r2, r2, #5
	adds	r5, r3, r2
	cmp	r5, #0
	bge.n	.L_0818e768
	movs	r5, #0
.L_0818e768:
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r4, [sp, #52]
	asrs	r3, r3, #10
	subs	r4, r4, r3
	adds	r0, r6, #0
	str	r4, [sp, #52]
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r6, [sp, #48]
	ldr	r0, [sp, #44]
	movs	r1, #128
	asrs	r3, r3, #10
	lsls	r1, r1, #2
	adds	r6, r6, r3
	adds	r0, r0, r1
	mov	r2, fp
	str	r6, [sp, #48]
	str	r0, [sp, #44]
	cmp	r2, #127
	ble.n	.L_0818e7be
	ldr	r4, [pc, #172]
	ldr	r6, [pc, #176]
	lsls	r3, r2, #13
	adds	r5, r3, r4
	cmp	r5, r6
	ble.n	.L_0818e7ac
	movs	r5, #128
	lsls	r5, r5, #10
.L_0818e7ac:
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #44]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r0, r0, r5
	adds	r1, r1, r2
	str	r0, [sp, #48]
	str	r1, [sp, #44]
	b.n	.L_0818e7ca
.L_0818e7be:
	ldr	r3, [sp, #44]
	movs	r4, #128
	lsls	r4, r4, #9
	cmp	r3, r4
	ble.n	.L_0818e7ca
	str	r4, [sp, #44]
.L_0818e7ca:
	ldr	r6, [sp, #96]
	movs	r0, #240
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r3, r6, r0
	movs	r5, #1
	str	r5, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #160
	beq.n	.L_0818e7fe
	ldr	r2, [pc, #104]
	ldr	r3, [r2, #12]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_0818e7fe
	ldr	r3, [r2, #12]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0818e7fe
	b.n	.L_0818e2dc
.L_0818e7fe:
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #68]
	movs	r3, #0
	str	r3, [r4, #16]
	ldr	r0, [pc, #76]
	mov	r6, fp
	bl	sub_08014644
	cmp	r6, #115
	bgt.n	.L_0818e860
	ldr	r0, [pc, #68]
	bl	sub_08014644
	b.n	.L_0818e866
	movs	r0, r0
	.4byte 0xffff00ff
	.4byte 0xffffe000
	.4byte 0xffffc000
	.4byte 0xfffe0000
	.4byte 0xffc00000
	.4byte 0x00024650
	.4byte 0x02010000
	.4byte 0x08199e7e
	.4byte 0x08199e70
	.4byte 0x08199e77
	.4byte 0x03000258
	.4byte 0xfff00000
	.4byte 0x0001ffff
	.4byte 0x03001150
	.4byte 0x0818ddfd
	.2byte 0x3175
	.2byte 0x0814
.L_0818e860:
	ldr	r0, [pc, #88]
	bl	sub_08014644
.L_0818e866:
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_0814cca8
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #0
	movs	r0, #200
	adds	r2, #32
	str	r0, [sp, #36]
	str	r1, [sp, #32]
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	str	r1, [r3, #0]
	ldr	r3, [pc, #52]
	adds	r2, #12
	str	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #38
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	subs	r2, #70
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #60
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	b.n	.L_0818e8c4
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00000784
	.4byte 0x00002737
	.4byte 0x00000721
	.4byte 0x08143001
	.2byte 0xf000
	.2byte 0xffff
.L_0818e8c4:
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #6
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r4, #239
	str	r3, [sp, #88]
	ldr	r3, [sp, #96]
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r6, [sp, #96]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r6, r0
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	b.n	.L_0818e90c
	movs	r0, r0
	.4byte 0x0000107c
	.2byte 0x0088
	.2byte 0x0000
.L_0818e90c:
	lsls	r1, r1, #4
	ldr	r0, [pc, #668]
	bl	sub_080145a8
	ldr	r0, [pc, #664]
	ldr	r1, [sp, #84]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r6, r2
	ldr	r0, [pc, #652]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #142
	lsls	r3, r3, #7
	adds	r1, r6, r3
	ldr	r0, [pc, #640]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r4, #174
	lsls	r4, r4, #7
	adds	r1, r6, r4
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #624]
	bl	sub_08157cf4
	ldr	r0, [pc, #620]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #616]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b98
	movs	r2, #128
	movs	r7, #0
	movs	r1, #0
	lsls	r2, r2, #1
.L_0818e96e:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L_0818e96e
	movs	r1, #4
	movs	r2, #12
	movs	r0, #5
	negs	r1, r1
	negs	r2, r2
	bl	sub_08164abc
	movs	r0, #108
	movs	r6, #0
	add	r0, sp
	mov	fp, r6
	mov	r9, r0
.L_0818e990:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_0818e99e
	movs	r2, #144
	movs	r3, #0
	str	r2, [sp, #36]
	str	r3, [sp, #32]
.L_0818e99e:
	mov	r4, fp
	cmp	r4, #33
	bgt.n	.L_0818e9f0
	ldr	r3, [pc, #548]
	add	r1, sp, #140
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r2, fp
	str	r3, [sp, #116]
	str	r4, [sp, #120]
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	ldr	r6, [sp, #36]
	lsls	r3, r6, #16
	str	r3, [r1, #0]
	ldr	r0, [sp, #32]
	lsls	r3, r0, #16
	str	r3, [r1, #8]
	cmp	r2, #15
	bgt.n	.L_0818e9d6
	subs	r6, #1
	adds	r0, #9
	str	r6, [sp, #36]
	str	r0, [sp, #32]
	b.n	.L_0818e9dc
.L_0818e9d6:
	ldr	r3, [sp, #32]
	adds	r3, #2
	str	r3, [sp, #32]
.L_0818e9dc:
	ldr	r4, [sp, #96]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #220
	adds	r3, r4, r6
	ldr	r0, [r3, #0]
	add	r2, sp, #116
	movs	r3, #0
	bl	sub_08020010
.L_0818e9f0:
	mov	r0, fp
	cmp	r0, #34
	bne.n	.L_0818e9fc
	movs	r0, #134
	bl	sub_081180e8
.L_0818e9fc:
	mov	r1, fp
	cmp	r1, #16
	bne.n	.L_0818eac6
	ldr	r4, [sp, #36]
	movs	r2, #128
	lsrs	r3, r4, #31
	lsls	r2, r2, #1
	adds	r3, r4, r3
	mov	r8, r2
	asrs	r3, r3, #1
	ldr	r2, [pc, #444]
	lsls	r3, r3, #16
	movs	r7, #0
	mov	sl, r3
.L_0818ea18:
	str	r2, [sp, #12]
	bl	sub_08014878
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r5, #254
	lsls	r5, r5, #7
	adds	r5, #255
	ldr	r2, [sp, #12]
	ands	r5, r0
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r3, #228
	mov	r1, sl
	lsls	r3, r3, #15
	adds	r5, r5, r0
	str	r1, [r2, #0]
	str	r3, [r2, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	add	r6, r8
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r2, [sp, #12]
	asrs	r3, r3, #6
	str	r3, [r2, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r2, [sp, #12]
	lsls	r3, r3, #1
	asrs	r3, r3, #6
	str	r3, [r2, #16]
	bl	sub_08014878
	movs	r3, #7
	ldr	r2, [sp, #12]
	ands	r0, r3
	adds	r0, #24
	adds	r7, #1
	str	r0, [r2, #24]
	adds	r2, #28
	cmp	r7, r8
	bne.n	.L_0818ea18
	ldr	r4, [sp, #96]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	adds	r3, r4, r6
	movs	r2, #4
	str	r2, [r3, #0]
	movs	r0, #145
	bl	sub_081c0010
	ldr	r0, [sp, #104]
	movs	r7, #0
	ldr	r3, [r0, #20]
	cmp	r3, #0
	beq.n	.L_0818eac6
	movs	r5, #36
.L_0818ea9e:
	ldr	r1, [sp, #104]
	ldrsh	r0, [r5, r1]
	movs	r1, #4
	bl	sub_08118088
	ldr	r3, [sp, #104]
	movs	r1, #7
	ldrsh	r0, [r5, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	adds	r3, r7, #0
	bl	sub_0814cd48
	ldr	r6, [sp, #104]
	adds	r7, #1
	ldr	r3, [r6, #20]
	adds	r5, #2
	cmp	r7, r3
	bne.n	.L_0818ea9e
.L_0818eac6:
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014dac
	str	r0, [sp, #28]
	movs	r0, #1
	bl	sub_081969f8
	mov	r2, r9
	str	r2, [r0, #16]
	ldr	r3, [sp, #28]
	movs	r1, #0
	str	r3, [r0, #12]
	add	r4, sp, #108
	movs	r3, #6
	str	r1, [r0, #20]
	strb	r3, [r4, #0]
	strb	r3, [r2, #1]
	ldr	r6, [sp, #96]
	mov	r8, r0
	movs	r0, #142
	lsls	r0, r0, #7
	adds	r3, r6, r0
	str	r3, [r2, #4]
	ldr	r3, [pc, #220]
	mov	r2, r8
	movs	r1, #7
	str	r3, [r2, #8]
	str	r1, [r2, #0]
	ldr	r4, [sp, #36]
	mov	r6, fp
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r3, #60
	str	r3, [sp, #24]
	lsls	r3, r6, #11
	negs	r0, r3
	movs	r7, #0
	mov	r9, r0
	mov	sl, r3
.L_0818eb18:
	ldr	r3, [pc, #188]
	ldrb	r3, [r3, r7]
	adds	r2, r3, #0
	adds	r2, #16
	cmp	fp, r2
	ble.n	.L_0818ebfa
	mov	r1, fp
	subs	r3, r2, r1
	lsls	r3, r3, #3
	adds	r1, r3, #0
	movs	r3, #16
	adds	r1, #56
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_0818eb3a
	movs	r1, #16
	negs	r1, r1
.L_0818eb3a:
	movs	r4, #64
	negs	r4, r4
	cmp	r1, r4
	ble.n	.L_0818ebfa
	ldr	r3, [pc, #152]
	mov	r6, fp
	ldrb	r3, [r3, r7]
	subs	r2, r6, r2
	muls	r2, r3
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #167
	adds	r3, r3, r2
	lsls	r0, r0, #8
	mov	r2, r8
	lsls	r6, r3, #4
	adds	r0, #16
	str	r1, [r2, #20]
	adds	r5, r6, r0
	bl	sub_08014de4
	ldr	r3, [sp, #24]
	movs	r1, #160
	lsls	r0, r3, #16
	lsls	r1, r1, #14
	movs	r2, #0
	bl	sub_08015160
	ldr	r3, [pc, #104]
	adds	r2, r5, #0
	ldrb	r3, [r3, r7]
	adds	r1, r3, #0
	muls	r1, r5
	cmp	r5, #0
	bge.n	.L_0818eb8a
	movs	r4, #167
	lsls	r4, r4, #8
	adds	r4, #19
	adds	r2, r6, r4
.L_0818eb8a:
	asrs	r2, r2, #2
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #250
	lsls	r0, r0, #2
	bl	sub_08015024
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_0818ebe4
	mov	r0, sl
	bl	sub_08015068
	b.n	.L_0818ebea
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x00000134
	.4byte 0x0000013e
	.4byte 0x000000da
	.4byte 0x000000c1
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x02010018
	.4byte 0x08196f20
	.4byte 0x02010000
	.4byte 0x081990d0
	.4byte 0x08199e8c
	.4byte 0x08199e90
	.2byte 0x9e94
	.2byte 0x0819
.L_0818ebe4:
	mov	r0, r9
	bl	sub_08015068
.L_0818ebea:
	ldr	r0, [pc, #404]
	ldr	r1, [sp, #28]
	movs	r2, #32
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_0818ebfa:
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r7, #1
	add	r9, r3
	add	sl, r3
	cmp	r7, #4
	bne.n	.L_0818eb18
	ldr	r3, [pc, #376]
	mov	r6, r8
	str	r3, [r6, #8]
	ldr	r0, [sp, #96]
	movs	r1, #174
	lsls	r1, r1, #7
	adds	r3, r0, r1
	str	r3, [sp, #112]
	movs	r2, #7
	add	r3, sp, #108
	strb	r2, [r3, #0]
	mov	r9, r3
	movs	r3, #109
	add	r3, sp
	strb	r2, [r3, #0]
	ldr	r4, [sp, #36]
	movs	r7, #0
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r3, #64
	mov	sl, r3
	mov	r6, fp
.L_0818ec36:
	ldr	r3, [pc, #336]
	ldrb	r3, [r3, r7]
	adds	r2, r3, #0
	adds	r2, #8
	cmp	fp, r2
	ble.n	.L_0818ecb8
	mov	r0, fp
	subs	r3, r2, r0
	lsls	r3, r3, #3
	adds	r1, r3, #0
	adds	r1, #40
	cmp	r1, #0
	ble.n	.L_0818ec52
	movs	r1, #0
.L_0818ec52:
	movs	r3, #64
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_0818ecb8
	ldr	r3, [pc, #304]
	mov	r4, fp
	ldrb	r3, [r3, r7]
	subs	r2, r4, r2
	muls	r2, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #220
	movs	r4, #7
	adds	r5, r2, #0
	muls	r5, r3
	adds	r3, r6, #0
	ands	r3, r4
	mov	r2, r8
	lsls	r3, r3, #4
	movs	r0, #131
	lsls	r0, r0, #7
	str	r1, [r2, #20]
	strb	r3, [r2, #24]
	adds	r5, r5, r0
	bl	sub_08014de4
	ldr	r3, [pc, #264]
	mov	r2, sl
	ldrsb	r1, [r3, r7]
	lsls	r0, r2, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	lsls	r1, r5, #1
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #250
	lsls	r0, r0, #3
	bl	sub_08015024
	ldr	r0, [pc, #232]
	ldr	r1, [sp, #28]
	movs	r2, #32
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_0818ecb8:
	adds	r7, #1
	adds	r6, #5
	cmp	r7, #3
	bne.n	.L_0818ec36
	mov	r0, r8
	bl	sub_08013164
	ldr	r0, [sp, #28]
	bl	sub_08013164
	ldr	r3, [pc, #200]
	ldr	r5, [pc, #204]
	movs	r7, #0
	mov	r8, r3
.L_0818ecd4:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_0818ed1a
	asrs	r0, r0, #2
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	mov	r6, r8
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #84]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #100]
	ldr	r4, [sp, #88]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
	movs	r2, #128
	subs	r3, #1
	str	r3, [r5, #24]
	adds	r0, r5, #0
	movs	r1, #56
	lsls	r2, r2, #6
	bl	sub_08138086
.L_0818ed1a:
	movs	r6, #128
	adds	r7, #1
	lsls	r6, r6, #1
	adds	r5, #28
	cmp	r7, r6
	bne.n	.L_0818ecd4
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #96]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #50
	beq.n	.L_0818ed52
	b.n	.L_0818e990
.L_0818ed52:
	ldr	r0, [pc, #76]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	movs	r6, #238
	ldr	r4, [sp, #96]
	lsls	r6, r6, #7
	adds	r6, #220
	adds	r3, r4, r6
	ldr	r0, [r3, #0]
	bl	sub_08020048
	bl	sub_08143bb8
	add	sp, #208
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08199090
	.4byte 0x08198ec4
	.4byte 0x08199e98
	.4byte 0x08199e9c
	.4byte 0x08199ea0
	.4byte 0x08198cac
	.4byte 0x08197410
	.4byte 0x02010000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	str	r0, [sp, #48]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	str	r0, [sp, #44]
	movs	r0, #0
	ldr	r1, [r3, #92]
	str	r1, [sp, #40]
	ldr	r3, [r3, #100]
	str	r3, [sp, #32]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #60
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #28]
	bl	sub_08144aac
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #44]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #40]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r3, r2
	ldr	r0, [pc, #28]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #40]
	movs	r2, #184
	lsls	r2, r2, #5
	b.n	.L_0818ee1c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000013e
	.4byte 0x02014000
	.2byte 0x00cb
	.2byte 0x0000
.L_0818ee1c:
	adds	r1, r3, r2
	ldr	r0, [pc, #632]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #624]
	ldr	r1, [pc, #628]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #32]
	movs	r3, #0
	ldr	r0, [pc, #616]
	bl	sub_08157cf4
	ldr	r0, [pc, #612]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #608]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b0a
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r3, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #40]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #572]
	bl	sub_080145a8
	ldr	r1, [sp, #48]
	mov	r2, sp
	adds	r2, #80
	ldr	r0, [r1, #8]
	adds	r1, r2, #0
	str	r2, [sp, #24]
	bl	sub_0815e21c
	ldr	r1, [sp, #48]
	mov	r2, sp
	adds	r2, #68
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	adds	r1, r2, #0
	str	r2, [sp, #20]
	bl	sub_0815e21c
	mov	r0, sp
	movs	r3, #0
	adds	r0, #52
	str	r3, [sp, #36]
	str	r0, [sp, #8]
.L_0818eea4:
	ldr	r1, [sp, #36]
	cmp	r1, #0
	bne.n	.L_0818ef28
	movs	r2, #0
	ldr	r3, [pc, #516]
	mov	r8, r2
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #2
.L_0818eeb8:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0818eeb8
	ldr	r2, [sp, #40]
	movs	r3, #224
	ldr	r6, [sp, #20]
	movs	r1, #0
	lsls	r3, r3, #2
	mov	r8, r1
	adds	r5, r2, r3
.L_0818eed2:
	bl	sub_08014878
	ldr	r3, [r6, #0]
	movs	r2, #127
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #64
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #248
	lsls	r3, r3, #5
	adds	r3, #255
	ands	r3, r0
	movs	r1, #1
	movs	r0, #224
	lsls	r0, r0, #7
	add	r8, r1
	adds	r3, r3, r0
	mov	r2, r8
	str	r3, [r5, #8]
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_0818eed2
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne.n	.L_0818ef28
	ldr	r0, [sp, #40]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	movs	r3, #50
	str	r3, [r2, #0]
.L_0818ef28:
	ldr	r0, [sp, #36]
	cmp	r0, #12
	bne.n	.L_0818ef46
	ldr	r1, [sp, #40]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r0, #238
	movs	r3, #3
	str	r3, [r2, #0]
	lsls	r0, r0, #7
	ldr	r3, [pc, #376]
	adds	r0, #132
	adds	r2, r1, r0
	str	r3, [r2, #0]
.L_0818ef46:
	ldr	r1, [sp, #36]
	cmp	r1, #54
	bne.n	.L_0818ef66
	ldr	r3, [sp, #40]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r3, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #40]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #50
	str	r3, [r2, #0]
.L_0818ef66:
	ldr	r0, [sp, #36]
	cmp	r0, #0
	bne.n	.L_0818ef72
	movs	r0, #139
	bl	sub_081c0010
.L_0818ef72:
	ldr	r1, [sp, #36]
	cmp	r1, #16
	bne.n	.L_0818ef7e
	movs	r0, #134
	bl	sub_081c0010
.L_0818ef7e:
	ldr	r2, [sp, #36]
	cmp	r2, #15
	bgt.n	.L_0818ef86
	b.n	.L_0818f0c0
.L_0818ef86:
	cmp	r2, #16
	bne.n	.L_0818f00a
	ldr	r0, [sp, #24]
	ldr	r7, [sp, #40]
	movs	r3, #0
	mov	r8, r3
	mov	sl, r0
.L_0818ef94:
	bl	sub_08014878
	movs	r1, #190
	lsls	r1, r1, #7
	adds	r1, #255
	bl	sub_0800206c
	movs	r1, #160
	lsls	r1, r1, #7
	adds	r6, r0, #0
	adds	r6, r6, r1
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r5, r0
	adds	r0, r6, #0
	adds	r5, r5, r2
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	mov	r0, sl
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	bl	sub_08014878
	mov	r1, sl
	ldr	r3, [r1, #4]
	movs	r2, #15
	ands	r2, r0
	subs	r3, r3, r2
	subs	r3, #8
	lsls	r3, r3, #16
	movs	r2, #1
	str	r3, [r7, #4]
	add	r8, r2
	movs	r3, #0
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L_0818ef94
.L_0818f00a:
	ldr	r1, [sp, #40]
	movs	r0, #0
	mov	r8, r0
	mov	sl, r1
.L_0818f012:
	mov	r6, sl
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_0818f086
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0818f022
	adds	r3, #3
.L_0818f022:
	ldr	r2, [sp, #36]
	asrs	r3, r3, #2
	cmp	r2, r3
	blt.n	.L_0818f086
	mov	r3, r8
	movs	r0, #1
	ands	r3, r0
	adds	r5, r3, #5
	lsls	r3, r3, #2
	str	r3, [sp, #16]
	movs	r1, #0
	lsrs	r2, r5, #1
	mov	r9, r1
	lsls	r7, r5, #1
	mov	fp, r2
.L_0818f040:
	ldr	r0, [pc, #120]
	subs	r3, r7, #2
	ldrh	r1, [r0, r3]
	ldr	r2, [sp, #32]
	mov	r0, fp
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	subs	r2, r2, r0
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	ldr	r0, [sp, #28]
	subs	r3, r3, r5
	mov	ip, r3
	str	r5, [sp, #0]
	ldr	r3, [sp, #16]
	str	r7, [sp, #4]
	ldr	r4, [r3, r0]
	ldr	r0, [sp, #44]
	mov	r3, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2140
	movs	r2, #0
	adds	r0, r6, #0
	bl	sub_08138086
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #2
	bne.n	.L_0818f040
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
.L_0818f086:
	movs	r0, #1
	add	r8, r0
	movs	r3, #28
	mov	r1, r8
	add	sl, r3
	cmp	r1, #32
	bne.n	.L_0818f012
	b.n	.L_0818f1c4
	movs	r0, r0
	.4byte 0x000000c9
	.4byte 0x000000c2
	.4byte 0x02014000
	.4byte 0x00000134
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x04040404
	.2byte 0x7410
	.2byte 0x0819
.L_0818f0c0:
	ldr	r2, [sp, #36]
	cmp	r2, #0
	bne.n	.L_0818f150
	ldr	r0, [sp, #24]
	ldr	r7, [sp, #40]
	movs	r3, #0
	mov	r8, r3
	mov	sl, r0
.L_0818f0d0:
	bl	sub_08014878
	movs	r6, #254
	ldr	r1, [pc, #884]
	lsls	r6, r6, #7
	adds	r6, #255
	ands	r6, r0
	adds	r6, r6, r1
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r5, r0
	adds	r0, r6, #0
	adds	r5, r5, r2
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #10
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #16]
	mov	r0, sl
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	bl	sub_08014878
	mov	r2, sl
	ldr	r1, [r2, #4]
	movs	r3, #15
	ands	r3, r0
	ldr	r2, [r7, #12]
	subs	r1, r1, r3
	ldr	r3, [r7, #0]
	lsls	r2, r2, #6
	subs	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r3, [r7, #16]
	subs	r1, #8
	lsls	r3, r3, #6
	lsls	r1, r1, #16
	subs	r1, r1, r3
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	str	r1, [r7, #4]
	adds	r7, #28
	cmp	r0, #32
	bne.n	.L_0818f0d0
.L_0818f150:
	ldr	r5, [sp, #40]
	movs	r1, #0
	mov	r8, r1
.L_0818f156:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0818f1b8
	mov	r3, r8
	movs	r2, #1
	ands	r3, r2
	adds	r6, r3, #2
	movs	r0, #0
	lsls	r3, r3, #2
	lsrs	r1, r6, #1
	mov	sl, r0
	mov	fp, r3
	lsls	r7, r6, #1
	mov	r9, r1
.L_0818f172:
	ldr	r2, [pc, #732]
	subs	r3, r7, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #32]
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	adds	r1, r3, r1
	mov	r3, r9
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	ldr	r0, [sp, #28]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	subs	r3, r3, r6
	mov	ip, r3
	mov	r3, fp
	ldr	r4, [r3, r0]
	ldr	r0, [sp, #44]
	mov	r3, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2140
	movs	r2, #0
	adds	r0, r5, #0
	bl	sub_08138086
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #4
	bne.n	.L_0818f172
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0818f1b8:
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_0818f156
.L_0818f1c4:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #12]
	movs	r0, #1
	bl	sub_081969f8
	movs	r3, #6
	adds	r7, r0, #0
	str	r3, [r7, #0]
	ldr	r2, [sp, #8]
	mov	r0, sp
	str	r2, [r7, #16]
	ldr	r1, [sp, #12]
	adds	r0, #52
	strb	r3, [r0, #0]
	str	r1, [r7, #12]
	strb	r3, [r2, #1]
	ldr	r1, [sp, #40]
	mov	r9, r0
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r3, r1, r0
	str	r3, [r2, #4]
	ldr	r3, [pc, #604]
	movs	r2, #128
	str	r3, [r7, #8]
	ldr	r6, [sp, #24]
	movs	r1, #0
	lsls	r2, r2, #8
	mov	r8, r1
	mov	sl, r2
.L_0818f204:
	ldr	r3, [pc, #592]
	mov	r0, r8
	ldrb	r2, [r3, r0]
	ldr	r1, [sp, #36]
	cmp	r1, r2
	blt.n	.L_0818f286
	adds	r3, r2, #0
	adds	r3, #16
	cmp	r1, r3
	bge.n	.L_0818f286
	movs	r3, #0
	str	r3, [r7, #20]
	subs	r5, r1, r2
	bl	sub_08014de4
	movs	r1, #128
	mov	r0, sl
	lsls	r1, r1, #9
	mov	r2, sl
	bl	sub_080151e4
	ldr	r0, [r6, #0]
	ldr	r1, [r6, #4]
	subs	r0, #128
	subs	r1, #74
	lsls	r1, r1, #16
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	ldr	r3, [pc, #536]
	mov	r2, r8
	lsls	r0, r2, #12
	adds	r0, r0, r3
	lsls	r5, r5, #11
	bl	sub_080150e4
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r3, [pc, #520]
	mov	r1, r8
	ldrb	r3, [r3, r1]
	adds	r1, r3, #0
	muls	r1, r0
	cmp	r1, #0
	bge.n	.L_0818f264
	adds	r1, #63
.L_0818f264:
	movs	r0, #170
	asrs	r1, r1, #6
	lsls	r0, r0, #7
	movs	r2, #128
	lsls	r1, r1, #1
	adds	r0, #85
	lsls	r2, r2, #8
	bl	sub_080151e4
	ldr	r0, [pc, #492]
	ldr	r1, [sp, #12]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818f286:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #8
	bne.n	.L_0818f204
	mov	r0, r9
	movs	r3, #5
	movs	r1, #53
	strb	r3, [r0, #0]
	add	r1, sp
	movs	r3, #2
	strb	r3, [r1, #0]
	ldr	r2, [sp, #40]
	movs	r0, #184
	lsls	r0, r0, #5
	adds	r3, r2, r0
	str	r3, [sp, #56]
	ldr	r3, [pc, #444]
	movs	r1, #0
	str	r3, [r7, #8]
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r0, [sp, #36]
	mov	r8, r1
	ldr	r1, [pc, #436]
	lsls	r3, r0, #13
	adds	r1, r1, r3
	movs	r2, #128
	ldr	r3, [sp, #40]
	lsls	r2, r2, #8
	movs	r0, #224
	mov	fp, r2
	lsls	r0, r0, #2
	movs	r2, #54
	mov	sl, r1
	mov	r9, r2
	adds	r6, r3, r0
.L_0818f2d0:
	ldr	r2, [sp, #36]
	mov	r1, r8
	lsls	r3, r1, #1
	cmp	r2, r9
	blt.n	.L_0818f330
	adds	r3, #70
	cmp	r2, r3
	bge.n	.L_0818f330
	movs	r0, #0
	movs	r5, #128
	mov	r3, sl
	str	r0, [r7, #20]
	lsls	r5, r5, #10
	subs	r5, r5, r3
	bl	sub_08014de4
	movs	r1, #128
	mov	r0, fp
	lsls	r1, r1, #9
	mov	r2, fp
	bl	sub_080151e4
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #368]
	ldr	r2, [pc, #368]
	adds	r0, r0, r1
	ldr	r1, [r6, #4]
	adds	r1, r1, r2
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [r6, #8]
	bl	sub_080150e4
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #9
	mov	r2, fp
	bl	sub_080151e4
	ldr	r0, [pc, #340]
	ldr	r1, [sp, #12]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818f330:
	ldr	r3, [pc, #280]
	movs	r1, #1
	add	r8, r1
	movs	r0, #2
	mov	r2, r8
	add	sl, r3
	add	r9, r0
	adds	r6, #28
	cmp	r2, #16
	bne.n	.L_0818f2d0
	movs	r2, #7
	add	r3, sp, #52
	strb	r2, [r3, #0]
	str	r3, [sp, #8]
	strb	r2, [r3, #1]
	ldr	r1, [sp, #8]
	ldr	r0, [pc, #296]
	ldr	r3, [pc, #300]
	str	r0, [r1, #4]
	str	r3, [r7, #8]
	str	r2, [r7, #0]
	ldr	r0, [sp, #36]
	movs	r2, #0
	ldr	r1, [pc, #292]
	mov	r8, r2
	ldr	r2, [pc, #292]
	lsls	r3, r0, #3
	adds	r1, r1, r3
	lsls	r3, r0, #14
	adds	r5, r3, r2
	negs	r3, r0
	adds	r3, #54
	subs	r0, #54
	mov	r9, r3
	ldr	r3, [sp, #40]
	mov	sl, r0
	movs	r0, #224
	lsls	r0, r0, #2
	mov	fp, r1
	adds	r6, r3, r0
.L_0818f380:
	mov	r1, r8
	lsls	r3, r1, #1
	ldr	r0, [sp, #36]
	adds	r2, r3, #0
	adds	r2, #54
	cmp	r0, r2
	blt.n	.L_0818f3ee
	adds	r3, #70
	cmp	r0, r3
	bge.n	.L_0818f3ee
	mov	r1, r9
	mov	r2, sl
	lsls	r3, r1, #2
	cmp	r2, #7
	ble.n	.L_0818f3a4
	mov	r0, fp
	subs	r3, r3, r0
	adds	r3, #64
.L_0818f3a4:
	str	r3, [r7, #20]
	bl	sub_08014de4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl	sub_080151e4
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #176]
	ldr	r2, [pc, #180]
	adds	r0, r0, r1
	ldr	r1, [r6, #4]
	adds	r1, r1, r2
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #240
	lsls	r0, r0, #6
	bl	sub_08015024
	adds	r0, r5, #0
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #172]
	ldr	r1, [sp, #12]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818f3ee:
	movs	r3, #16
	negs	r3, r3
	ldr	r0, [pc, #156]
	add	fp, r3
	movs	r3, #1
	movs	r2, #2
	add	r8, r3
	adds	r5, r5, r0
	movs	r1, #2
	negs	r2, r2
	mov	r0, r8
	add	r9, r1
	add	sl, r2
	adds	r6, #28
	cmp	r0, #16
	bne.n	.L_0818f380
	movs	r1, #0
	ldr	r3, [sp, #36]
	mov	r8, r1
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #24]
	movs	r2, #128
	lsls	r2, r2, #8
	lsls	r3, r3, #13
	mov	fp, r2
	mov	sl, r3
	lsls	r6, r0, #3
	mov	r9, r1
.L_0818f426:
	ldr	r0, [sp, #36]
	mov	r2, r8
	lsls	r3, r2, #2
	cmp	r0, r3
	blt.n	.L_0818f4e8
	adds	r3, #12
	cmp	r0, r3
	bge.n	.L_0818f4e8
	adds	r0, r6, #0
	movs	r3, #192
	lsls	r3, r3, #9
	mov	r1, sl
	subs	r0, #64
	subs	r5, r3, r1
	cmp	r0, #0
	ble.n	.L_0818f494
	movs	r0, #0
	b.n	.L_0818f494
	movs	r0, r0
	.4byte 0xffffc000
	.4byte 0x08197410
	.4byte 0x081992b0
	.4byte 0x08199eb4
	.4byte 0xffffc800
	.4byte 0x08199ebc
	.4byte 0x08199ea4
	.4byte 0x08199220
	.4byte 0xfff94000
	.4byte 0xff800000
	.4byte 0xffc00000
	.4byte 0x081991c0
	.4byte 0x02014000
	.4byte 0x08199364
	.4byte 0xfffffe50
	.4byte 0xfff28000
	.4byte 0x081991e0
	.2byte 0x8000
	.2byte 0xffff
.L_0818f494:
	.2byte 0x6178
	bl	sub_08014de4
	movs	r1, #128
	mov	r0, fp
	lsls	r1, r1, #9
	mov	r2, fp
	bl	sub_080151e4
	mov	r2, r9
	ldr	r0, [r2, #0]
	ldr	r1, [r2, #4]
	subs	r0, #128
	subs	r1, #80
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	lsls	r1, r5, #1
	adds	r0, r1, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	mov	r0, r8
	lsls	r3, r0, #14
	movs	r0, #192
	lsls	r0, r0, #7
	subs	r0, r0, r3
	bl	sub_080150e4
	ldr	r0, [pc, #232]
	bl	sub_08015024
	ldr	r0, [pc, #228]
	ldr	r1, [sp, #12]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818f4e8:
	ldr	r1, [pc, #216]
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	add	sl, r1
	subs	r6, #32
	cmp	r3, #2
	bne.n	.L_0818f426
	movs	r0, #0
	mov	r8, r0
.L_0818f4fc:
	mov	r1, r8
	ldr	r2, [sp, #36]
	lsls	r3, r1, #2
	adds	r3, #54
	cmp	r2, r3
	bne.n	.L_0818f554
	cmp	r1, #7
	bne.n	.L_0818f514
	movs	r0, #144
	bl	sub_081180e8
	b.n	.L_0818f51a
.L_0818f514:
	movs	r0, #144
	bl	sub_081c0010
.L_0818f51a:
	ldr	r1, [sp, #48]
	movs	r2, #0
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r3, #0
	bl	sub_0815f000
	ldr	r3, [sp, #48]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r1, #238
	ldr	r0, [sp, #40]
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #4
	str	r3, [r2, #0]
.L_0818f554:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #8
	bne.n	.L_0818f4fc
	adds	r0, r7, #0
	bl	sub_08013164
	ldr	r0, [sp, #12]
	bl	sub_08013164
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #40]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #36]
	adds	r2, #1
	str	r2, [sp, #36]
	cmp	r2, #111
	beq.n	.L_0818f596
	b.n	.L_0818eea4
.L_0818f596:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0x08199210
	.4byte 0xffff8000
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	ldr	r3, [r0, #0]
	cmp	r3, #199
	ble.n	.L_0818f5dc
	movs	r1, #2
	bl	sub_0818f620
	b.n	.L_0818f5e2
.L_0818f5dc:
	movs	r1, #1
	bl	sub_0818f620
.L_0818f5e2:
	pop	{pc}
	push	{lr}
	movs	r1, #0
	bl	sub_0818f620
	pop	{pc}
