.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_08014e38, 0x08014e38
	.set sub_08014ea8, 0x08014ea8
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_08118040, 0x08118040
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e21c, 0x0815e21c
	.set sub_08164a4c, 0x08164a4c
	.set sub_08178680, 0x08178680
	.set sub_0817d6c4, 0x0817d6c4
	.set sub_0817d9a6, 0x0817d9a6
	.set sub_0817da44, 0x0817da44
	.set sub_0817db04, 0x0817db04
	.set sub_0817dea6, 0x0817dea6
	.set sub_0817e0c0, 0x0817e0c0
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0817d724
	.thumb_func
Func_0817d724:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #116
	str	r0, [sp, #80]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	movs	r7, #239
	str	r0, [sp, #76]
	lsls	r7, r7, #7
	ldr	r1, [r5, #96]
	str	r1, [sp, #72]
	ldr	r2, [r5, #100]
	str	r2, [sp, #60]
	bl	sub_0813ba50
	movs	r0, #1
	bl	sub_081435e0
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r2, #128
	str	r3, [sp, #64]
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r2, r4, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r4, r0
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #32]
	bl	sub_080145a8
	ldr	r2, [sp, #80]
	movs	r7, #0
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	str	r0, [sp, #56]
	ldr	r1, [sp, #56]
	ldr	r5, [r5, #48]
	ldr	r2, [sp, #56]
	b.n	.L_0817d7a8
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_0817d7a8:
	str	r5, [sp, #52]
	movs	r4, #54
	ldrsh	r3, [r5, r4]
	str	r7, [sp, #44]
	str	r3, [sp, #48]
	ldr	r4, [sp, #80]
	ldr	r0, [r0, #72]
	mov	r7, sp
	str	r0, [sp, #40]
	adds	r7, #104
	ldr	r1, [r1, #8]
	str	r1, [sp, #36]
	adds	r1, r7, #0
	ldr	r2, [r2, #16]
	str	r2, [sp, #32]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	str	r7, [sp, #28]
	bl	sub_0815e21c
	ldr	r2, [sp, #80]
	mov	r3, sp
	adds	r3, #92
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r3, #0
	str	r3, [sp, #24]
	bl	sub_0815e21c
	ldr	r4, [sp, #76]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r4, r7
	ldr	r0, [pc, #120]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r2, [sp, #76]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r1, r2, r3
	ldr	r0, [pc, #104]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #76]
	movs	r7, #142
	lsls	r7, r7, #7
	adds	r1, r4, r7
	ldr	r0, [pc, #92]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #84]
	ldr	r1, [sp, #60]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [pc, #68]
	movs	r1, #4
	movs	r2, #32
	movs	r3, #32
	bl	sub_08178680
	movs	r0, #0
	mov	fp, r0
.L_0817d83e:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_0817d91a
	ldr	r0, [pc, #48]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b13
	movs	r2, #0
	b.n	.L_0817d880
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x000000f4
	.4byte 0x0000013e
	.4byte 0x000000e8
	.4byte 0x00000134
	.4byte 0x02012000
	.4byte 0x00000148
	.2byte 0x0730
	.2byte 0x0300
.L_0817d880:
	mov	r8, r2
	adds	r3, #24
	subs	r2, #1
.L_0817d886:
	movs	r4, #1
	add	r8, r4
	mov	r7, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_0817d886
	ldr	r3, [pc, #236]
	movs	r0, #0
	movs	r2, #1
	mov	r8, r0
	negs	r2, r2
.L_0817d89e:
	movs	r1, #1
	add	r8, r1
	mov	r4, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r4, #128
	bne.n	.L_0817d89e
	movs	r7, #0
	mov	r8, r7
	ldr	r7, [pc, #212]
.L_0817d8b2:
	ldr	r0, [sp, #28]
	movs	r6, #254
	ldr	r3, [r0, #0]
	lsls	r6, r6, #7
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #240
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, #255
	movs	r1, #128
	lsls	r1, r1, #7
	ands	r6, r0
	adds	r6, r6, r1
	bl	sub_08014878
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #255
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r2, #1
	adds	r3, #32
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #128
	bne.n	.L_0817d8b2
.L_0817d91a:
	mov	r4, fp
	cmp	r4, #4
	bne.n	.L_0817d948
	ldr	r1, [sp, #80]
	movs	r3, #2
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	movs	r4, #238
	ldr	r3, [sp, #76]
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
.L_0817d948:
	mov	r7, fp
	cmp	r7, #163
	bne.n	.L_0817d958
	movs	r2, #128
	ldr	r3, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
.L_0817d958:
	mov	r0, fp
	cmp	r0, #164
	bne.n	.L_0817d974
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r1, #206
	lsls	r1, r1, #3
	adds	r3, r3, r1
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #24
	bl	sub_08118040
.L_0817d974:
	mov	r2, fp
	cmp	r2, #165
	bne.n	sub_0817d9a6
	ldr	r2, [pc, #16]
	movs	r3, #120
	b.n	.L_0817d990
	.4byte 0x00000784
	.4byte 0x02014e18
	.4byte 0x02014000
	.2byte 0x11e0
	.2byte 0x0300
.L_0817d990:
	str	r3, [r2, #16]
	ldr	r0, [pc, #140]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #132]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465b
	cmp	r3, #166
	bne.n	.L_0817d9e2
	add	r4, sp, #48
	ldr	r7, [sp, #52]
	ldrh	r4, [r4, #0]
	movs	r3, #200
	strh	r4, [r7, #54]
	ldr	r0, [sp, #56]
	ldr	r7, [sp, #40]
	lsls	r3, r3, #15
	str	r7, [r0, #72]
	ldr	r1, [sp, #36]
	str	r3, [r0, #12]
	str	r1, [r0, #8]
	ldr	r2, [sp, #32]
	movs	r3, #32
	str	r2, [r0, #16]
	ldr	r2, [pc, #92]
	movs	r1, #7
	strh	r3, [r2, #6]
	ldr	r4, [sp, #80]
	movs	r2, #5
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r3, #0
	bl	sub_0814cd48
.L_0817d9e2:
	mov	r7, fp
	cmp	r7, #179
	bne.n	.L_0817d9fc
	movs	r0, #134
	bl	sub_081180e8
	movs	r1, #238
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #8
	str	r3, [r2, #0]
.L_0817d9fc:
	mov	r2, fp
	cmp	r2, #165
	ble.n	.L_0817da30
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r7, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	ldr	r3, [pc, #20]
	adds	r0, #132
	adds	r2, r7, r0
	str	r3, [r2, #0]
	b.n	.L_0817e020
	movs	r0, r0
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x03001120
	.2byte 0x0404
	.2byte 0x0404
.L_0817da30:
	mov	r1, fp
	cmp	r1, #59
	bne.n	sub_0817da44
	movs	r1, #128
	ldr	r3, [pc, #68]
	ldr	r0, [sp, #72]
	lsls	r1, r1, #7
	ldr	r2, [pc, #68]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465a
	cmp	r2, #60
	bne.n	.L_0817daea
	movs	r0, #1
	ldr	r1, [pc, #56]
	movs	r2, #0
	bl	sub_08118040
	ldr	r5, [pc, #36]
	movs	r4, #160
	lsls	r4, r4, #19
	movs	r3, #0
	adds	r4, #192
	mov	r8, r3
	movs	r6, #31
.L_0817da62:
	ldrh	r3, [r4, #0]
	adds	r2, r6, #0
	ands	r2, r3
	lsls	r3, r3, #16
	lsrs	r1, r3, #26
	ands	r1, r5
	cmp	r2, #31
	ble.n	.L_0817da74
	movs	r2, #31
.L_0817da74:
	cmp	r1, #31
	ble.n	.L_0817da8c
	movs	r1, #31
	b.n	.L_0817da8c
	.4byte 0x0000001f
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0x0072
	.2byte 0x0000
.L_0817da8c:
	lsls	r3, r2, #10
	movs	r7, #1
	lsls	r2, r2, #5
	orrs	r3, r2
	add	r8, r7
	orrs	r3, r1
	mov	r0, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r0, #128
	bne.n	.L_0817da62
	bl	sub_0817d6c4
	ldr	r2, [pc, #216]
	movs	r3, #104
	strh	r3, [r2, #6]
	ldr	r2, [pc, #212]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r1, [sp, #80]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0817dac4
	ldr	r2, [sp, #52]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r2, #54]
	b.n	.L_0817dacc
.L_0817dac4:
	ldr	r4, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r4, #54]
.L_0817dacc:
	ldr	r3, [pc, #184]
	movs	r7, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r7
	negs	r1, r1
	lsls	r2, r2, #1
.L_0817dada:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0817dada
	movs	r1, #220
	str	r1, [sp, #44]
.L_0817daea:
	mov	r2, fp
	cmp	r2, #134
	bne.n	sub_0817db04
	movs	r0, #145
	bl	sub_081c0010
	movs	r1, #128
	ldr	r3, [pc, #144]
	ldr	r0, [sp, #72]
	lsls	r1, r1, #7
	ldr	r2, [pc, #144]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465b
	cmp	r3, #59
	ble.n	.L_0817db4c
	ldr	r2, [pc, #116]
	ldrh	r3, [r2, #6]
	cmp	r3, #104
	ble.n	.L_0817db16
	ldr	r4, [pc, #128]
	adds	r3, r3, r4
.L_0817db16:
	movs	r7, #16
	subs	r3, #8
	negs	r7, r7
	cmp	r3, r7
	bge.n	.L_0817db22
	adds	r3, #120
.L_0817db22:
	strh	r3, [r2, #6]
	mov	r2, fp
	subs	r2, #60
	cmp	r2, #0
	bge.n	.L_0817db2e
	adds	r2, #7
.L_0817db2e:
	asrs	r2, r2, #3
	movs	r3, #4
	subs	r2, r3, r2
	cmp	r2, #0
	bge.n	.L_0817db3a
	movs	r2, #0
.L_0817db3a:
	mov	r0, fp
	cmp	r0, #119
	ble.n	.L_0817db44
	mov	r2, fp
	subs	r2, #120
.L_0817db44:
	ldr	r1, [sp, #44]
	subs	r1, r1, r2
	str	r1, [sp, #44]
	b.n	.L_0817db56
.L_0817db4c:
	mov	r3, fp
	lsls	r2, r3, #3
	movs	r3, #200
	subs	r3, r3, r2
	str	r3, [sp, #44]
.L_0817db56:
	ldr	r4, [sp, #44]
	cmp	r4, #55
	bgt.n	.L_0817db60
	movs	r7, #56
	str	r7, [sp, #44]
.L_0817db60:
	ldr	r0, [sp, #56]
	mov	r3, fp
	movs	r2, #0
	subs	r3, #4
	str	r2, [r0, #72]
	cmp	r3, #55
	bhi.n	.L_0817db9c
	mov	r1, fp
	lsls	r3, r1, #3
	ldr	r2, [pc, #36]
	add	r3, fp
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r0, #12]
	b.n	.L_0817dbe2
	movs	r0, r0
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0x02014018
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0xffff0000
	.2byte 0x0000
	.2byte 0xffee
.L_0817db9c:
	.2byte 0x465b
	cmp	r3, #59
	ble.n	.L_0817dbe2
	ldr	r4, [sp, #56]
	movs	r3, #147
	str	r2, [r4, #8]
	ldr	r7, [sp, #44]
	lsls	r3, r3, #1
	subs	r3, r3, r7
	mov	r0, fp
	lsls	r3, r3, #16
	lsls	r7, r0, #10
	str	r3, [r4, #12]
	adds	r0, r7, #0
	bl	sub_08002096
	ldr	r1, [sp, #56]
	lsls	r0, r0, #3
	negs	r0, r0
	mov	r2, fp
	str	r0, [r1, #16]
	cmp	r2, #60
	bne.n	.L_0817dbd6
	ldr	r4, [sp, #80]
	ldr	r1, [sp, #28]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_0815e21c
.L_0817dbd6:
	ldr	r1, [sp, #80]
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	ldr	r1, [sp, #24]
	bl	sub_0815e21c
.L_0817dbe2:
	mov	r3, fp
	subs	r3, #152
	cmp	r3, #15
	bhi.n	.L_0817dc18
	ldr	r2, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r3, r2, r4
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r7, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	ldr	r3, [pc, #916]
	adds	r0, #132
	adds	r2, r7, r0
	mov	r1, fp
	str	r3, [r2, #0]
	cmp	r1, #162
	ble.n	.L_0817dc0e
	ldr	r3, [pc, #908]
	str	r3, [r2, #0]
.L_0817dc0e:
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164a4c
.L_0817dc18:
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_0817dc20
	b.n	.L_0817e020
.L_0817dc20:
	movs	r4, #64
	negs	r4, r4
	movs	r3, #0
	mov	sl, r4
	mov	r9, r3
	lsls	r7, r2, #10
	movs	r4, #0
	movs	r1, #0
.L_0817dc30:
	movs	r0, #0
	mov	r8, r0
	ldr	r0, [pc, #868]
	adds	r2, r4, #0
	mov	r3, sl
	movs	r6, #0
	adds	r5, r1, r0
.L_0817dc3e:
	adds	r0, r2, r7
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002096
	lsls	r0, r0, #1
	asrs	r0, r0, #16
	adds	r0, r6, r0
	subs	r0, #10
	strb	r0, [r5, #0]
	movs	r0, #0
	strb	r0, [r5, #2]
	ldr	r3, [sp, #12]
	movs	r0, #1
	add	r8, r0
	mov	r0, r8
	strb	r3, [r5, #1]
	adds	r6, #20
	adds	r5, #4
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #16]
	ldr	r4, [sp, #8]
	cmp	r0, #2
	bne.n	.L_0817dc3e
	movs	r3, #224
	movs	r0, #1
	movs	r2, #2
	lsls	r3, r3, #3
	add	r9, r0
	add	sl, r2
	adds	r3, #226
	mov	r2, r9
	adds	r1, #8
	adds	r4, r4, r3
	cmp	r2, #33
	bne.n	.L_0817dc30
	movs	r3, #0
	mov	sl, r3
	bl	sub_081969f8
	movs	r4, #6
	adds	r5, r0, #0
	ldr	r0, [pc, #776]
	mov	r8, r4
	add	r6, sp, #84
	mov	r7, sl
	mov	r1, r8
	movs	r3, #5
	str	r4, [r5, #0]
	str	r7, [r5, #20]
	strb	r7, [r5, #25]
	str	r0, [r5, #12]
	strb	r1, [r6, #0]
	strb	r3, [r6, #1]
	str	r6, [r5, #16]
	ldr	r2, [sp, #76]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r4
	mov	r0, fp
	ldr	r7, [pc, #744]
	lsls	r2, r0, #2
	str	r3, [r6, #4]
	movs	r3, #127
	bics	r3, r2
	strb	r3, [r5, #24]
	str	r7, [r5, #8]
	movs	r7, #128
	bl	sub_08014de4
	lsls	r7, r7, #8
	movs	r1, #128
	adds	r0, r7, #0
	lsls	r1, r1, #9
	adds	r2, r7, #0
	bl	sub_080151e4
	bl	sub_08014e38
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #44]
	ldr	r0, [r1, #0]
	lsls	r1, r2, #16
	subs	r0, #128
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	ldr	r4, [sp, #80]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_0817dd00
	adds	r0, r7, #0
	bl	sub_08015068
.L_0817dd00:
	ldr	r0, [pc, #676]
	bl	sub_0801521c
	ldr	r1, [pc, #664]
	movs	r2, #66
	ldr	r0, [pc, #656]
	bl	sub_08196958
	mov	r7, fp
	adds	r0, r5, #0
	bl	sub_08196a7c
	bl	sub_08014ea8
	cmp	r7, #133
	bgt.n	.L_0817dd80
	mov	r3, fp
	subs	r3, #17
	cmp	r3, #42
	bls.n	.L_0817dd80
	ldr	r3, [pc, #640]
	mov	r0, r8
	mov	r1, sl
	strb	r0, [r6, #0]
	strb	r0, [r6, #1]
	str	r3, [r5, #8]
	strb	r1, [r5, #24]
	ldr	r2, [sp, #76]
	movs	r4, #142
	lsls	r4, r4, #7
	adds	r3, r2, r4
	str	r3, [r6, #4]
	ldr	r7, [sp, #24]
	ldr	r1, [sp, #44]
	ldr	r0, [r7, #0]
	subs	r1, #144
	subs	r0, #124
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r0, r0, #16
	bl	sub_08015160
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_080150e4
	movs	r0, #206
	movs	r1, #167
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	movs	r2, #128
	adds	r0, #64
	adds	r1, #32
	lsls	r2, r2, #9
	bl	sub_080151e4
	ldr	r0, [pc, #572]
	ldr	r1, [pc, #556]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
.L_0817dd80:
	adds	r0, r5, #0
	bl	sub_08013164
	mov	r0, fp
	cmp	r0, #134
	bne.n	.L_0817ddf4
	ldr	r7, [pc, #548]
	movs	r1, #0
	mov	r8, r1
.L_0817dd92:
	ldr	r2, [sp, #28]
	movs	r6, #254
	ldr	r3, [r2, #0]
	lsls	r6, r6, #7
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #0
	str	r3, [r7, #4]
	bl	sub_08014878
	ldr	r3, [pc, #520]
	adds	r6, #255
	ands	r6, r0
	adds	r6, r6, r3
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r4, #1
	movs	r0, #128
	adds	r3, #32
	add	r8, r4
	lsls	r0, r0, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L_0817dd92
.L_0817ddf4:
	mov	r3, fp
	subs	r3, #134
	cmp	r3, #7
	bhi.n	.L_0817de66
	lsls	r2, r3, #2
	lsls	r3, r3, #5
	subs	r3, r3, r2
	ldr	r2, [sp, #76]
	movs	r1, #0
	lsls	r3, r3, #2
	mov	r8, r1
	mov	sl, r1
	adds	r7, r3, r2
.L_0817de0e:
	ldr	r4, [sp, #28]
	mov	r0, sl
	ldr	r3, [r4, #0]
	str	r0, [r7, #4]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	bl	sub_08014878
	movs	r6, #254
	ldr	r1, [pc, #400]
	lsls	r6, r6, #7
	adds	r6, #255
	ands	r6, r0
	adds	r6, r6, r1
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	movs	r3, #1
	add	r8, r3
	mov	r2, sl
	mov	r4, r8
	str	r2, [r7, #24]
	adds	r7, #28
	cmp	r4, #4
	bne.n	.L_0817de0e
.L_0817de66:
	ldr	r5, [sp, #76]
	movs	r7, #0
	mov	r8, r7
.L_0817de6c:
	ldr	r1, [r5, #24]
	cmp	r1, #23
	bhi.n	.L_0817deb6
	cmp	r1, #1
	ble.n	sub_0817dea6
	cmp	r1, #0
	bge.n	.L_0817de7c
	adds	r1, #3
.L_0817de7c:
	ldr	r0, [sp, #76]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r2, #240
	adds	r1, r0, r1
	lsls	r2, r2, #4
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r0, [sp, #72]
	ldr	r7, [sp, #64]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #272]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0817deb6:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L_0817de6c
	mov	r2, fp
	cmp	r2, #133
	bhi.n	.L_0817df6c
	mov	r1, fp
	movs	r3, #63
	subs	r1, #16
	ands	r1, r3
	movs	r3, #120
	cmp	r2, #59
	ble.n	.L_0817deda
	ldr	r3, [sp, #44]
	subs	r3, #80
.L_0817deda:
	lsls	r3, r3, #16
	lsls	r2, r1, #1
	mov	sl, r3
	ldr	r7, [pc, #220]
	lsls	r3, r1, #4
	subs	r3, r3, r2
	movs	r4, #0
	lsls	r3, r3, #2
	mov	r8, r4
	adds	r6, r3, r7
.L_0817deee:
	ldr	r0, [sp, #28]
	mov	r1, sl
	ldr	r3, [r0, #0]
	str	r1, [r6, #4]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	movs	r2, #128
	ands	r0, r3
	lsls	r2, r2, #7
	mov	r3, fp
	adds	r7, r0, r2
	cmp	r3, #59
	ble.n	.L_0817df1e
	movs	r4, #192
	lsls	r4, r4, #8
	adds	r7, r0, r4
.L_0817df1e:
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	adds	r0, r7, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r6, #12]
	adds	r0, r7, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	mov	r7, fp
	asrs	r3, r3, #6
	str	r3, [r6, #16]
	cmp	r7, #59
	ble.n	.L_0817df54
	ldr	r3, [r6, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #12]
.L_0817df54:
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r0, #1
	add	r8, r0
	adds	r3, #32
	mov	r1, r8
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r1, #2
	bne.n	.L_0817deee
.L_0817df6c:
	mov	r2, fp
	ldr	r6, [pc, #76]
	cmp	r2, #59
	ble.n	.L_0817df78
	movs	r6, #128
	lsls	r6, r6, #7
.L_0817df78:
	ldr	r4, [pc, #72]
	ldr	r5, [pc, #56]
	movs	r3, #0
	mov	r8, r3
	mov	sl, r4
.L_0817df82:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0817e012
	adds	r0, r3, #0
	subs	r0, #8
	cmp	r0, #0
	bge.n	.L_0817dfc8
	subs	r0, r3, #1
	b.n	.L_0817dfc8
	.4byte 0x20202020
	.4byte 0x3f3f3f3f
	.4byte 0x02010000
	.4byte 0x02011000
	.4byte 0x02012000
	.4byte 0x00021770
	.4byte 0x08199340
	.4byte 0x081991e0
	.4byte 0x02014000
	.4byte 0xffffc000
	.4byte 0xfffff000
	.4byte 0x02014e00
	.2byte 0x7410
	.2byte 0x0819
.L_0817dfc8:
	mov	r7, fp
	asrs	r4, r0, #3
	cmp	r7, #59
	ble.n	.L_0817dfd2
	adds	r4, #2
.L_0817dfd2:
	cmp	r4, #0
	bgt.n	.L_0817dfd8
	movs	r4, #1
.L_0817dfd8:
	lsls	r0, r4, #1
	subs	r3, r0, #2
	mov	r2, sl
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #60]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r1, r3, r1
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #64]
	ldr	r0, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	adds	r2, r6, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0817e012:
	movs	r7, #1
	movs	r0, #128
	add	r8, r7
	lsls	r0, r0, #1
	adds	r5, #28
	cmp	r8, r0
	bne.n	.L_0817df82
.L_0817e020:
	bl	sub_081434f8
	mov	r3, fp
	subs	r3, #32
	cmp	r3, #133
	bls.n	.L_0817e034
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
.L_0817e034:
	ldr	r1, [sp, #76]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #1
	add	fp, r4
	mov	r7, fp
	cmp	r7, #186
	beq.n	.L_0817e056
	bl	.L_0817d83e
.L_0817e056:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #20]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #4
	bl	sub_0817e0c0
	pop	{pc}
	.2byte 0x0000
