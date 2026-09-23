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
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118078, 0x08118078
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b434, 0x0815b434
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815e22c, 0x0815e22c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_0818ca42, 0x0818ca42
	.set sub_0818cd62, 0x0818cd62
	.set sub_0818d5a8, 0x0818d5a8
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0818c7b8
	.thumb_func
Func_0818c7b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	str	r0, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #92]
	ldr	r3, [r3, #96]
	mov	fp, r1
	movs	r0, #0
	mov	r8, r3
	mov	sl, r2
	bl	sub_08143a88
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_0818c7f2
	ldr	r2, [sp, #8]
	movs	r4, #3
	ldr	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_0818c7fe
	movs	r4, #7
	b.n	.L_0818c7fe
.L_0818c7f2:
	ldr	r2, [sp, #8]
	movs	r4, #11
	ldr	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_0818c7fe
	movs	r4, #15
.L_0818c7fe:
	movs	r3, #1
	str	r3, [sp, #0]
	adds	r3, r4, #0
	movs	r1, #7
	movs	r2, #7
	movs	r0, #104
	bl	sub_08196404
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #104]
	ldr	r0, [pc, #88]
	ldr	r1, [pc, #92]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	movs	r3, #0
	ldr	r0, [pc, #80]
	add	r1, sl
	movs	r2, #1
	bl	sub_08157cf4
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_0818c884
	ldr	r0, [pc, #68]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b08
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	b.n	.L_0818c8a2
	movs	r0, r0
	.4byte 0x00000810
	.4byte 0x0000010b
	.4byte 0x02010000
	.4byte 0x0000010c
	.4byte 0x00000150
	.2byte 0x0730
	.2byte 0x0300
.L_0818c884:
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #75
.L_0818c8a2:
	str	r3, [r2, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	bl	sub_080145a8
	ldr	r2, [sp, #8]
	movs	r3, #36
	ldrsh	r1, [r2, r3]
	movs	r3, #128
	ldr	r0, [r2, #8]
	lsls	r3, r3, #11
	movs	r2, #8
	bl	sub_08118078
	b.n	.L_0818c8cc
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_0818c8cc:
	movs	r2, #0
	mov	r9, r2
.L_0818c8d0:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_0818c8e2
	ldr	r3, [sp, #8]
	add	r1, sp, #12
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_0815e21c
.L_0818c8e2:
	mov	r2, r9
	cmp	r2, #8
	bne.n	.L_0818c8ee
	movs	r0, #212
	bl	sub_081c0010
.L_0818c8ee:
	mov	r3, r9
	cmp	r3, #12
	bne.n	.L_0818c92e
	mov	r2, fp
	cmp	r2, #1
	bne.n	.L_0818c902
	movs	r0, #144
	bl	sub_081180e8
	b.n	.L_0818c908
.L_0818c902:
	movs	r0, #134
	bl	sub_081180e8
.L_0818c908:
	ldr	r2, [sp, #8]
	movs	r1, #250
	movs	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #11
	lsls	r3, r3, #13
	str	r1, [sp, #4]
	movs	r1, #1
	str	r2, [sp, #0]
	bl	sub_0815f000
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #4
	str	r3, [r2, #0]
.L_0818c92e:
	mov	r3, r9
	cmp	r3, #7
	bgt.n	.L_0818c936
	b.n	sub_0818ca42
.L_0818c936:
	mov	r0, r9
	subs	r0, #8
	movs	r1, #3
	bl	sub_08002054
	mov	r2, fp
	negs	r3, r2
	orrs	r3, r2
	ldr	r2, [sp, #8]
	lsrs	r5, r3, #31
	ldr	r3, [r2, #4]
	lsls	r5, r5, #4
	cmp	r3, #0
	bne.n	.L_0818c960
	ldr	r3, [sp, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r4, r3, #0
	subs	r4, #48
	b.n	.L_0818c96c
.L_0818c960:
	ldr	r3, [sp, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r4, r3, #0
	subs	r4, #80
.L_0818c96c:
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_0818c9b8
	cmp	r0, #4
	bhi.n	sub_0818ca42
	ldr	r2, [pc, #276]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0818c9e8
	.4byte 0x0818c9d8
	.4byte 0x0818c994
	.4byte 0x0818c9a2
	.2byte 0xc9b0
	.2byte 0x0818
	movs	r3, #128
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	subs	r2, r4, #4
	mov	r0, r8
	ldr	r1, [pc, #240]
	b.n	.L_0818c9f8
	movs	r3, #128
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r1, [pc, #232]
	subs	r2, r4, #4
	mov	r0, r8
	b.n	.L_0818c9f8
	movs	r1, #253
	movs	r3, #120
	lsls	r1, r1, #6
	b.n	.L_0818c9ee
.L_0818c9b8:
	cmp	r0, #4
	bhi.n	sub_0818ca42
	ldr	r2, [pc, #216]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0818c9e8
	.4byte 0x0818c9d8
	.4byte 0x0818ca00
	.4byte 0x0818ca16
	.2byte 0xca2c
	.2byte 0x0818
	movs	r1, #128
	ldr	r3, [pc, #192]
	mov	r0, r8
	lsls	r1, r1, #7
	ldr	r2, [pc, #188]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe02c
	movs	r1, #224
	movs	r3, #120
	lsls	r1, r1, #3
.L_0818c9ee:
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	add	r1, sl
	mov	r0, r8
	adds	r2, r4, #0
.L_0818c9f8:
	adds	r3, r5, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe020
	movs	r1, #128
	adds	r3, r5, #0
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, r4, #4
	subs	r3, #8
	mov	r0, r8
	ldr	r1, [pc, #128]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe015
	movs	r0, #128
	adds	r3, r5, #0
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r1, [pc, #116]
	subs	r2, r4, #4
	subs	r3, #8
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe00a
	movs	r1, #253
	movs	r3, #120
	lsls	r1, r1, #6
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	add	r1, sl
	mov	r0, r8
	adds	r2, r4, #0
	adds	r3, r5, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2008
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #32
	beq.n	.L_0818ca6c
	b.n	.L_0818c8d0
.L_0818ca6c:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0818c980
	.4byte 0x02010000
	.4byte 0x02014000
	.4byte 0x0818c9c4
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0x3001
	.2byte 0x0814
.L_0818caa8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #12]
	str	r2, [sp, #8]
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r8, r3
	ldr	r3, [r3, #96]
	adds	r7, r1, #0
	movs	r0, #188
	movs	r1, #19
	mov	sl, r3
	bl	sub_081963ec
	lsrs	r5, r6, #31
	adds	r5, r6, r5
	asrs	r5, r5, #1
	movs	r3, #188
	add	r8, r3
	subs	r3, r7, r5
	mov	r9, r3
	ldr	r3, [sp, #8]
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	subs	r3, r3, r6
	mov	fp, r3
	mov	r3, r8
	ldr	r1, [sp, #12]
	ldr	r4, [r3, #0]
	mov	r2, r9
	mov	r3, fp
	mov	r0, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #23
	movs	r0, #188
	bl	sub_081963ec
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	mov	r3, r8
	ldr	r1, [sp, #12]
	ldr	r4, [r3, #0]
	adds	r2, r7, #0
	mov	r3, fp
	mov	r0, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #27
	movs	r0, #188
	bl	sub_081963ec
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	mov	r3, r8
	ldr	r1, [sp, #12]
	ldr	r4, [r3, #0]
	mov	r2, r9
	ldr	r3, [sp, #8]
	mov	r0, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #31
	movs	r0, #188
	bl	sub_081963ec
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	mov	r3, r8
	ldr	r4, [r3, #0]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #8]
	adds	r2, r7, #0
	mov	r0, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	add	sp, #16
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
	sub	sp, #104
	str	r0, [sp, #56]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	movs	r7, #240
	str	r0, [sp, #52]
	movs	r0, #0
	ldr	r1, [r5, #96]
	lsls	r7, r7, #7
	str	r1, [sp, #48]
	adds	r7, #240
	ldr	r2, [r5, #100]
	movs	r6, #2
	str	r2, [sp, #36]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #52]
	adds	r3, r4, r7
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r1, #166
	lsls	r1, r1, #2
	movs	r0, #4
	movs	r2, #1
	bl	sub_08152404
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #16]
	subs	r2, #6
	b.n	.L_0818cbec
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00002737
	.4byte 0x00003f21
	.2byte 0x107c
	.2byte 0x0000
.L_0818cbec:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r0, [sp, #52]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	ldr	r5, [r5, #104]
	adds	r3, #132
	adds	r2, r0, r3
	movs	r1, #200
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	str	r5, [sp, #40]
	bl	sub_080145a8
	movs	r7, #166
	ldr	r4, [sp, #52]
	lsls	r7, r7, #7
	movs	r2, #224
	b.n	.L_0818cc34
	.4byte 0x00001088
	.2byte 0x3001
	.2byte 0x0814
.L_0818cc34:
	lsls	r2, r2, #3
	adds	r7, #86
	adds	r7, r4, r7
	adds	r1, r4, r2
	ldr	r0, [pc, #800]
	movs	r2, #1
	movs	r3, #0
	str	r7, [sp, #28]
	bl	sub_08157cf4
	movs	r4, #152
	ldr	r3, [sp, #52]
	lsls	r4, r4, #5
	adds	r4, #86
	adds	r1, r3, r4
	ldr	r0, [pc, #784]
	movs	r2, #0
	movs	r3, #0
	ldr	r7, [pc, #780]
	bl	sub_08157cf4
	movs	r2, #160
	lsls	r2, r2, #7
	adds	r1, r7, r2
	movs	r3, #0
	ldr	r0, [pc, #772]
	movs	r2, #1
	bl	sub_08157cf4
	movs	r7, #144
	movs	r3, #0
	lsls	r7, r7, #6
	mov	r9, r3
	movs	r5, #0
.L_0818cc78:
	ldr	r4, [pc, #748]
	movs	r2, #128
	adds	r0, r7, r4
	adds	r1, r6, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	adds	r3, r5, #3
	muls	r3, r6
	ldr	r0, [sp, #28]
	lsrs	r2, r3, #31
	movs	r1, #1
	adds	r3, r3, r2
	add	r9, r1
	asrs	r3, r3, #1
	mov	r2, r9
	strh	r7, [r5, r0]
	adds	r6, #2
	adds	r7, r7, r3
	adds	r5, #2
	cmp	r2, #12
	bne.n	.L_0818cc78
	ldr	r0, [pc, #712]
	ldr	r1, [pc, #704]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #704]
	ldr	r1, [pc, #708]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #700]
	ldr	r1, [pc, #704]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #36]
	movs	r3, #0
	ldr	r0, [pc, #692]
	bl	sub_08157cf4
	ldr	r0, [pc, #688]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #684]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9f0d
	movs	r3, #0
	mov	r9, r3
.L_0818ccee:
	mov	r4, r9
	lsls	r6, r4, #1
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	mov	r0, r9
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r0, #31
	add	r3, r9
	movs	r1, #1
	asrs	r3, r3, #1
	add	r9, r1
	adds	r3, #25
	mov	r2, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #32
	bne.n	.L_0818ccee
	add	r5, sp, #92
	adds	r0, r5, #0
	bl	sub_0815e22c
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #32]
	movs	r3, #0
	mov	fp, r3
.L_0818cd48:
	mov	r4, fp
	cmp	r4, #0
	bne.n	sub_0818cd62
	ldr	r0, [pc, #568]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #560]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465f
	cmp	r7, #76
	bgt.n	.L_0818cd7c
	mov	r0, fp
	cmp	r0, #64
	ble.n	.L_0818cd76
	ldr	r0, [pc, #544]
	bl	sub_0815f0a0
	b.n	.L_0818cd90
.L_0818cd76:
	mov	r1, fp
	cmp	r1, #23
	ble.n	.L_0818cd84
.L_0818cd7c:
	ldr	r0, [pc, #520]
	bl	sub_0815f0a0
	b.n	.L_0818cd90
.L_0818cd84:
	mov	r2, fp
	cmp	r2, #8
	ble.n	.L_0818cd90
	ldr	r0, [pc, #516]
	bl	sub_0815f0a0
.L_0818cd90:
	mov	r3, fp
	cmp	r3, #8
	bne.n	.L_0818cdd2
	ldr	r4, [sp, #52]
	movs	r7, #238
	lsls	r7, r7, #7
	adds	r7, #168
	adds	r3, r4, r7
	mov	r0, fp
	str	r0, [r3, #0]
	ldr	r2, [sp, #56]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_0818cdd2
	movs	r6, #16
	movs	r5, #36
.L_0818cdb4:
	ldr	r3, [sp, #56]
	movs	r1, #7
	ldrsh	r0, [r5, r3]
	movs	r2, #5
	mov	r3, r9
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r0, [sp, #56]
	movs	r7, #1
	ldr	r3, [r0, #20]
	add	r9, r7
	adds	r5, #2
	cmp	r9, r3
	bne.n	.L_0818cdb4
.L_0818cdd2:
	mov	r1, fp
	cmp	r1, #48
	bne.n	.L_0818cde6
	ldr	r3, [sp, #52]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
.L_0818cde6:
	mov	r7, fp
	cmp	r7, #64
	bne.n	.L_0818ce36
	ldr	r0, [sp, #52]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r3, r0, r1
	movs	r2, #16
	str	r2, [r3, #0]
	movs	r0, #144
	bl	sub_081180e8
	ldr	r4, [sp, #56]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_0818ce36
	movs	r6, #128
	lsls	r6, r6, #12
	movs	r5, #36
	movs	r7, #120
.L_0818ce14:
	ldr	r1, [sp, #56]
	adds	r3, r6, #0
	ldrsh	r0, [r5, r1]
	movs	r2, #128
	movs	r1, #1
	lsls	r2, r2, #10
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	bl	sub_0815f000
	ldr	r4, [sp, #56]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r9, r3
	bne.n	.L_0818ce14
.L_0818ce36:
	mov	r7, fp
	cmp	r7, #0
	bne.n	.L_0818ce42
	movs	r0, #212
	bl	sub_081c0010
.L_0818ce42:
	mov	r0, fp
	cmp	r0, #4
	bne.n	.L_0818ce4e
	movs	r0, #144
	bl	sub_081c0010
.L_0818ce4e:
	mov	r1, fp
	cmp	r1, #16
	bne.n	.L_0818ce5a
	movs	r0, #164
	bl	sub_081c0010
.L_0818ce5a:
	mov	r2, fp
	cmp	r2, #64
	bne.n	.L_0818ce66
	movs	r0, #145
	bl	sub_081c0010
.L_0818ce66:
	mov	r3, fp
	cmp	r3, #63
	bgt.n	.L_0818ceee
	ldr	r0, [sp, #32]
	movs	r2, #128
	subs	r0, #8
	cmp	r3, #9
	bgt.n	.L_0818ce7e
	lsls	r3, r3, #4
	adds	r1, r3, #0
	subs	r1, #144
	b.n	.L_0818ce80
.L_0818ce7e:
	movs	r1, #0
.L_0818ce80:
	adds	r3, r1, #0
	adds	r3, #128
	cmp	r3, #108
	ble.n	.L_0818ce8e
	subs	r3, r2, r1
	adds	r2, r3, #0
	subs	r2, #20
.L_0818ce8e:
	cmp	r2, #0
	ble.n	.L_0818ceee
	ldr	r3, [pc, #256]
	add	r2, sp, #76
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	lsls	r0, r0, #17
	str	r3, [sp, #68]
	str	r4, [sp, #72]
	movs	r3, #255
	movs	r4, #0
	lsls	r3, r3, #16
	str	r4, [r2, #12]
	str	r3, [r2, #4]
	mov	r8, r0
	adds	r7, r2, #0
	ldr	r0, [sp, #52]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r6, r0, r2
	movs	r0, #160
	lsls	r3, r1, #16
	lsls	r0, r0, #14
	mov	r9, r4
	adds	r5, r3, r0
	add	r4, sp, #68
.L_0818cec4:
	movs	r3, #128
	lsls	r3, r3, #13
	add	r3, r8
	adds	r2, r4, #0
	str	r3, [r7, #0]
	str	r5, [r7, #8]
	adds	r1, r7, #0
	movs	r3, #0
	ldmia	r6!, {r0}
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r2, #1
	movs	r1, #128
	add	r9, r2
	lsls	r1, r1, #14
	mov	r3, r9
	adds	r5, r5, r1
	ldr	r4, [sp, #8]
	cmp	r3, #4
	bne.n	.L_0818cec4
.L_0818ceee:
	mov	r3, fp
	subs	r3, #12
	cmp	r3, #52
	bls.n	.L_0818cef8
	b.n	.L_0818d078
.L_0818cef8:
	mov	r4, fp
	cmp	r4, #12
	bne.n	.L_0818cf18
	ldr	r3, [sp, #52]
	movs	r7, #0
	mov	r9, r7
	adds	r3, #24
	movs	r2, #0
.L_0818cf08:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	str	r2, [r3, #0]
	adds	r3, #28
	subs	r2, #2
	cmp	r1, #32
	bne.n	.L_0818cf08
.L_0818cf18:
	mov	r4, fp
	lsls	r3, r4, #2
	ldr	r7, [sp, #52]
	movs	r2, #0
	subs	r3, #156
	mov	r9, r2
	mov	sl, r3
.L_0818cf26:
	ldr	r3, [r7, #24]
	mov	r0, r9
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r0, #0
	bne.n	.L_0818cf98
	mov	r1, fp
	cmp	r1, #39
	bgt.n	.L_0818cf3a
	b.n	.L_0818d06a
.L_0818cf3a:
	ldr	r2, [sp, #52]
	movs	r3, #166
	lsls	r3, r3, #7
	adds	r3, #134
	adds	r5, r2, r3
	movs	r2, #128
	adds	r0, r5, #0
	mov	r1, sl
	lsls	r2, r2, #9
	bl	sub_0815b434
	adds	r0, r5, #0
	ldr	r1, [sp, #32]
	movs	r2, #100
	mov	r3, sl
	bl	.L_0818caa8
	b.n	.L_0818d06a
	movs	r0, r0
	.4byte 0x00000192
	.4byte 0x000000f8
	.4byte 0x02010000
	.4byte 0x0000013e
	.4byte 0x000000c1
	.4byte 0x000000fa
	.4byte 0x02011000
	.4byte 0x000000da
	.4byte 0x02011400
	.4byte 0x00000134
	.4byte 0x00000149
	.4byte 0x03000730
	.4byte 0x00000148
	.2byte 0x6f18
	.2byte 0x0819
.L_0818cf98:
	cmp	r3, #0
	bne.n	.L_0818d020
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	adds	r6, r0, #0
	movs	r4, #128
	lsls	r4, r4, #7
	ands	r6, r3
	adds	r2, r6, r4
	str	r2, [sp, #16]
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	ldr	r2, [sp, #16]
	adds	r5, #255
	ands	r5, r0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r5, r5, r0
	adds	r0, r2, #0
	bl	sub_08002096
	ldr	r1, [sp, #32]
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [sp, #16]
	lsls	r1, r1, #16
	mov	r8, r1
	asrs	r3, r3, #4
	add	r3, r8
	str	r3, [r7, #0]
	adds	r0, r2, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #208
	lsls	r2, r2, #15
	asrs	r3, r3, #3
	adds	r3, r3, r2
	str	r3, [r7, #4]
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r6, r6, r3
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r4, #7
	ands	r0, r4
	adds	r0, #4
	str	r0, [r7, #20]
.L_0818d020:
	ldr	r0, [r7, #24]
	cmp	r0, #31
	bhi.n	.L_0818d06a
	lsls	r0, r0, #10
	bl	sub_08002096
	ldr	r3, [r7, #20]
	muls	r3, r0
	asrs	r3, r3, #16
	lsls	r4, r3, #1
	subs	r5, r4, #2
	cmp	r5, #61
	bhi.n	.L_0818d06a
	movs	r1, #6
	ldrsh	r3, [r7, r1]
	movs	r0, #2
	ldrsh	r6, [r7, r0]
	movs	r1, #64
	adds	r0, r7, #0
	movs	r2, #0
	str	r3, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08138086
	ldr	r2, [sp, #28]
	asrs	r5, r5, #1
	lsls	r5, r5, #1
	ldrsh	r0, [r5, r2]
	ldr	r3, [sp, #12]
	ldr	r2, [pc, #652]
	ldr	r4, [sp, #8]
	adds	r0, r0, r2
	adds	r1, r6, #0
	adds	r2, r3, #0
	adds	r3, r4, #0
	bl	.L_0818caa8
.L_0818d06a:
	movs	r3, #1
	add	r9, r3
	mov	r4, r9
	adds	r7, #28
	cmp	r4, #14
	beq.n	.L_0818d078
	b.n	.L_0818cf26
.L_0818d078:
	mov	r7, fp
	cmp	r7, #7
	bgt.n	.L_0818d080
	b.n	.L_0818d27c
.L_0818d080:
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014dac
	str	r0, [sp, #24]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #600]
	ldr	r3, [sp, #60]
	adds	r7, r0, #0
	ands	r3, r2
	ldr	r2, [pc, #596]
	movs	r0, #7
	orrs	r3, r0
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #60]
	movs	r3, #0
	str	r3, [r7, #4]
	ldr	r2, [pc, #568]
	ldr	r3, [pc, #580]
	add	r1, sp, #60
	str	r2, [r1, #4]
	str	r0, [r7, #0]
	str	r1, [r7, #16]
	str	r3, [r7, #8]
	ldr	r4, [sp, #24]
	movs	r0, #0
	str	r4, [r7, #12]
	strb	r0, [r7, #24]
	strb	r0, [r7, #25]
	ldr	r2, [sp, #32]
	mov	r8, r1
	subs	r2, #64
	movs	r1, #0
	mov	r9, r1
	mov	r6, fp
	mov	sl, r2
.L_0818d0d2:
	ldr	r3, [pc, #548]
	mov	r4, r9
	ldrb	r1, [r3, r4]
	cmp	fp, r1
	ble.n	.L_0818d14e
	ldr	r3, [pc, #540]
	mov	r0, fp
	ldrb	r3, [r3, r4]
	subs	r2, r0, r1
	muls	r2, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #220
	muls	r3, r2
	movs	r2, #131
	lsls	r2, r2, #7
	adds	r5, r3, r2
	subs	r3, r1, r0
	lsls	r3, r3, #3
	adds	r3, #56
	cmp	r3, #0
	ble.n	.L_0818d100
	movs	r3, #0
.L_0818d100:
	movs	r4, #64
	negs	r4, r4
	cmp	r3, r4
	ble.n	.L_0818d14e
	str	r3, [r7, #20]
	bl	sub_08014de4
	ldr	r3, [pc, #496]
	mov	r0, r9
	ldrsb	r1, [r3, r0]
	mov	r2, sl
	lsls	r0, r2, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	lsls	r1, r5, #1
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r4, #7
	adds	r3, r6, #0
	ands	r3, r4
	movs	r0, #176
	lsls	r3, r3, #4
	lsls	r0, r0, #4
	strb	r3, [r7, #24]
	adds	r0, #184
	bl	sub_08015024
	ldr	r0, [pc, #452]
	ldr	r1, [sp, #24]
	movs	r2, #32
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818d14e:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r6, #5
	cmp	r1, #8
	bne.n	.L_0818d0d2
	mov	r2, fp
	cmp	r2, #71
	ble.n	.L_0818d162
	b.n	.L_0818d270
.L_0818d162:
	movs	r3, #5
	mov	r4, r8
	strb	r3, [r4, #0]
	ldr	r3, [pc, #412]
	mov	r0, sp
	adds	r0, #60
	movs	r1, #7
	movs	r2, #7
	str	r0, [sp, #20]
	strb	r1, [r0, #1]
	str	r0, [r7, #16]
	str	r2, [r7, #0]
	str	r3, [r7, #8]
	ldr	r3, [sp, #24]
	movs	r4, #0
	mov	r0, fp
	str	r3, [r7, #12]
	strb	r4, [r7, #24]
	strb	r4, [r7, #25]
	cmp	r0, #8
	bne.n	.L_0818d1bc
	ldr	r2, [sp, #52]
	movs	r3, #224
	ldr	r6, [pc, #376]
	movs	r1, #0
	lsls	r3, r3, #2
	mov	r9, r1
	adds	r5, r2, r3
.L_0818d19a:
	ldr	r3, [r6, #0]
	mov	r0, r9
	str	r3, [r5, #8]
	movs	r1, #6
	ldr	r3, [r6, #4]
	adds	r6, #8
	str	r3, [r5, #20]
	bl	sub_08002064
	movs	r4, #1
	lsls	r0, r0, #2
	add	r9, r4
	str	r0, [r5, #24]
	mov	r0, r9
	adds	r5, #28
	cmp	r0, #8
	bne.n	.L_0818d19a
.L_0818d1bc:
	ldr	r3, [sp, #52]
	movs	r4, #224
	movs	r1, #0
	movs	r2, #18
	lsls	r4, r4, #2
	mov	r9, r1
	mov	sl, r2
	adds	r6, r3, r4
.L_0818d1cc:
	mov	r0, r9
	lsls	r5, r0, #2
	cmp	fp, sl
	blt.n	.L_0818d260
	adds	r3, r5, #0
	adds	r3, #42
	cmp	fp, r3
	bge.n	.L_0818d260
	ldr	r0, [r6, #24]
	cmp	r0, #0
	bge.n	.L_0818d1e4
	adds	r0, #7
.L_0818d1e4:
	asrs	r0, r0, #3
	movs	r1, #3
	bl	sub_08002064
	mov	r8, r0
	movs	r3, #16
	negs	r3, r3
	str	r3, [r7, #20]
	ldr	r2, [sp, #52]
	mov	r1, r8
	movs	r4, #152
	ldr	r0, [sp, #20]
	lsls	r3, r1, #12
	lsls	r4, r4, #5
	adds	r4, #86
	adds	r3, r2, r3
	adds	r3, r3, r4
	str	r3, [r0, #4]
	bl	sub_08014de4
	ldr	r0, [sp, #32]
	movs	r1, #128
	subs	r0, #64
	lsls	r1, r1, #14
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [r6, #8]
	bl	sub_080150e4
	movs	r3, #1
	mov	r1, r9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0818d234
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_0818d234:
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #20]
	movs	r0, #128
	adds	r3, r3, r2
	movs	r2, #128
	str	r3, [r6, #8]
	lsls	r0, r0, #10
	ldr	r1, [pc, #204]
	lsls	r2, r2, #9
	bl	sub_080151e4
	ldr	r3, [pc, #200]
	mov	r2, r8
	lsls	r0, r2, #4
	adds	r0, r0, r3
	ldr	r1, [sp, #24]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0818d260:
	movs	r4, #1
	add	r9, r4
	movs	r3, #4
	mov	r0, r9
	add	sl, r3
	adds	r6, #28
	cmp	r0, #7
	bne.n	.L_0818d1cc
.L_0818d270:
	adds	r0, r7, #0
	bl	sub_08013164
	ldr	r0, [sp, #24]
	bl	sub_08013164
.L_0818d27c:
	mov	r1, fp
	cmp	r1, #63
	bgt.n	.L_0818d284
	b.n	.L_0818d458
.L_0818d284:
	cmp	r1, #64
	beq.n	.L_0818d28a
	b.n	.L_0818d3ac
.L_0818d28a:
	ldr	r3, [sp, #32]
	ldr	r7, [sp, #52]
	movs	r2, #0
	lsls	r3, r3, #16
	mov	r9, r2
	mov	r8, r3
.L_0818d296:
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	movs	r4, #128
	lsls	r4, r4, #2
	ands	r5, r0
	adds	r0, r6, #0
	adds	r5, r5, r4
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	mov	r0, r9
	asrs	r2, r3, #7
	str	r2, [r7, #16]
	cmp	r0, #15
	ble.n	.L_0818d318
	ldr	r3, [r7, #12]
	movs	r1, #200
	lsls	r3, r3, #2
	add	r3, r8
	str	r3, [r7, #0]
	lsls	r1, r1, #15
	lsls	r3, r2, #2
	adds	r3, r3, r1
	str	r3, [r7, #4]
	movs	r3, #16
	subs	r3, r3, r0
	b.n	.L_0818d32e
	.4byte 0x02010000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08198ec4
	.4byte 0x08199dcb
	.4byte 0x08199dd3
	.4byte 0x08199ddb
	.4byte 0x08198cac
	.4byte 0x08199268
	.4byte 0x08199de4
	.4byte 0x00012710
	.2byte 0x9e24
	.2byte 0x0819
.L_0818d318:
	mov	r2, r8
	str	r2, [r7, #0]
	bl	sub_08014878
	movs	r1, #104
	bl	sub_0800206c
	mov	r4, r9
	lsls	r0, r0, #16
	negs	r3, r4
	str	r0, [r7, #4]
.L_0818d32e:
	cmp	r3, #0
	bge.n	.L_0818d334
	adds	r3, #3
.L_0818d334:
	asrs	r3, r3, #2
	str	r3, [r7, #24]
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r7, #28
	cmp	r1, #32
	bne.n	.L_0818d296
	ldr	r7, [pc, #548]
	movs	r2, #0
	mov	r9, r2
.L_0818d34a:
	bl	sub_08014878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r6, r0
	adds	r6, r6, r3
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	movs	r3, #200
	lsls	r3, r3, #15
	ands	r5, r0
	movs	r4, #128
	mov	r0, r8
	lsls	r4, r4, #2
	str	r0, [r7, #0]
	str	r3, [r7, #4]
	adds	r0, r6, #0
	adds	r5, r5, r4
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	r9, r1
	adds	r3, #16
	mov	r2, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #128
	bne.n	.L_0818d34a
.L_0818d3ac:
	ldr	r5, [sp, #52]
	movs	r3, #0
	mov	r9, r3
.L_0818d3b2:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_0818d3f0
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_0818d3c0
	adds	r1, r3, #3
.L_0818d3c0:
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	ldr	r4, [pc, #420]
	movs	r0, #32
	asrs	r1, r1, #2
	str	r0, [sp, #0]
	lsls	r1, r1, #11
	movs	r0, #64
	adds	r1, r1, r4
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #392]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_0818d3f0:
	movs	r7, #1
	add	r9, r7
	adds	r3, #1
	mov	r0, r9
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_0818d3b2
	ldr	r6, [pc, #372]
	ldr	r5, [pc, #360]
	movs	r1, #0
	mov	r9, r1
.L_0818d408:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_0818d44c
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #36]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #40]
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
.L_0818d44c:
	movs	r7, #1
	add	r9, r7
	mov	r0, r9
	adds	r5, #28
	cmp	r0, #128
	bne.n	.L_0818d408
.L_0818d458:
	mov	r3, fp
	subs	r3, #24
	cmp	r3, #39
	bhi.n	.L_0818d504
	movs	r1, #0
	mov	r9, r1
	movs	r7, #3
.L_0818d466:
	mov	r4, r9
	ands	r4, r7
	str	r4, [sp, #8]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08002096
	ldr	r3, [pc, #260]
	ldr	r4, [sp, #8]
	ldr	r2, [sp, #32]
	adds	r5, r0, #0
	mov	sl, r3
	ldrb	r3, [r3, r4]
	lsls	r5, r5, #3
	asrs	r5, r5, #16
	adds	r5, r2, r5
	lsrs	r3, r3, #1
	adds	r0, r6, #0
	subs	r5, r5, r3
	bl	sub_08002090
	ldr	r2, [pc, #236]
	ldr	r4, [sp, #8]
	mov	r1, r9
	ldrb	r3, [r2, r4]
	lsls	r0, r0, #4
	lsls	r6, r1, #5
	asrs	r0, r0, #16
	lsrs	r3, r3, #1
	adds	r6, r6, r0
	mov	r8, r2
	subs	r6, r6, r3
	bl	sub_08014878
	ldr	r3, [pc, #212]
	ands	r0, r7
	ldrb	r2, [r3, r0]
	adds	r3, r7, #0
	orrs	r3, r2
	movs	r2, #1
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #192]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #52]
	mov	r2, sl
	adds	r1, r3, r1
	ldrb	r3, [r2, r4]
	movs	r0, #224
	str	r3, [sp, #0]
	lsls	r0, r0, #3
	adds	r1, r1, r0
	mov	r0, r8
	ldrb	r3, [r0, r4]
	movs	r2, #192
	str	r3, [sp, #4]
	lsls	r2, r2, #18
	adds	r2, #188
	ldr	r4, [r2, #0]
	adds	r3, r6, #0
	ldr	r0, [sp, #48]
	adds	r2, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r3, #1
	add	r9, r3
	mov	r4, r9
	cmp	r4, #4
	bne.n	.L_0818d466
.L_0818d504:
	movs	r1, #16
	movs	r0, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r0, #240
	ldr	r7, [sp, #52]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r7, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #104
	beq.n	.L_0818d532
	bl	.L_0818cd48
.L_0818d532:
	ldr	r0, [pc, #88]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	movs	r4, #238
	lsls	r4, r4, #7
	movs	r3, #0
	adds	r4, #220
	mov	r9, r3
	adds	r5, r7, r4
.L_0818d54a:
	movs	r7, #1
	ldmia	r5!, {r0}
	add	r9, r7
	bl	sub_08020048
	mov	r0, r9
	cmp	r0, #4
	bne.n	.L_0818d54a
	bl	sub_08143bb8
	add	sp, #104
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02012964
	.4byte 0x02015000
	.4byte 0xffffc000
	.4byte 0x08197410
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x08199e54
	.4byte 0x08197486
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_0818d5a8
	pop	{pc}
	.2byte 0x0000
