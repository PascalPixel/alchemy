.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08118078, 0x08118078
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814b2f0, 0x0814b2f0
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e22c, 0x0815e22c
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Overlay_0814a814
Overlay_0814a814:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #120
	str	r0, [sp, #72]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r6, #224
	ldr	r3, [r3, #96]
	mov	fp, r0
	lsls	r6, r6, #3
	mov	sl, r1
	movs	r0, #0
	movs	r1, #16
	add	r6, fp
	str	r3, [sp, #68]
	str	r1, [sp, #52]
	bl	sub_081435e0
	movs	r2, #1
	ldr	r0, [pc, #492]
	adds	r1, r6, #0
	movs	r3, #1
	bl	sub_08157cf4
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_0814a85a
	ldr	r0, [pc, #480]
	b.n	.L_0814a8ba
.L_0814a85a:
	mov	r3, sl
	cmp	r3, #1
	bne.n	.L_0814a864
	ldr	r0, [pc, #472]
	b.n	.L_0814a8ba
.L_0814a864:
	mov	r4, sl
	cmp	r4, #2
	bne.n	.L_0814a86e
	ldr	r0, [pc, #468]
	b.n	.L_0814a8ba
.L_0814a86e:
	mov	r0, sl
	cmp	r0, #3
	bne.n	.L_0814a878
	ldr	r0, [pc, #460]
	b.n	.L_0814a8ba
.L_0814a878:
	mov	r1, sl
	cmp	r1, #4
	bne.n	.L_0814a882
	ldr	r0, [pc, #456]
	b.n	.L_0814a8ba
.L_0814a882:
	mov	r2, sl
	cmp	r2, #5
	bne.n	.L_0814a88c
	ldr	r5, [pc, #448]
	b.n	.L_0814a8ac
.L_0814a88c:
	mov	r3, sl
	cmp	r3, #7
	bne.n	.L_0814a8a6
	movs	r4, #24
	adds	r1, r6, #0
	ldr	r0, [pc, #436]
	movs	r2, #1
	movs	r3, #0
	str	r4, [sp, #52]
	bl	sub_08157cf4
	ldr	r0, [pc, #408]
	b.n	.L_0814a8ba
.L_0814a8a6:
	ldr	r5, [pc, #424]
	movs	r0, #32
	str	r0, [sp, #52]
.L_0814a8ac:
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	adds	r0, r5, #0
.L_0814a8ba:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #400]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4651
	cmp	r1, #4
	bne.n	.L_0814a8e2
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #384]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_0814a8e2:
	mov	r2, sl
	cmp	r2, #3
	bne.n	.L_0814a8f8
	movs	r1, #178
	lsls	r1, r1, #6
	ldr	r0, [pc, #364]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
.L_0814a8f8:
	ldr	r5, [pc, #356]
	movs	r3, #0
	mov	r9, r3
	movs	r6, #255
.L_0814a900:
	mov	r4, sl
	cmp	r4, #1
	bls.n	.L_0814a916
	cmp	r4, #4
	beq.n	.L_0814a916
	cmp	r4, #5
	beq.n	.L_0814a916
	cmp	r4, #6
	beq.n	.L_0814a916
	cmp	r4, #7
	bne.n	.L_0814a93a
.L_0814a916:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #14
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	b.n	.L_0814a95c
.L_0814a93a:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #13
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #255
	lsls	r0, r0, #13
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #13
.L_0814a95c:
	str	r0, [r5, #8]
	movs	r0, #1
	movs	r1, #128
	movs	r3, #0
	add	r9, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r9, r1
	bne.n	.L_0814a900
	mov	r2, sl
	cmp	r2, #1
	bls.n	.L_0814a986
	cmp	r2, #4
	beq.n	.L_0814a986
	cmp	r2, #5
	beq.n	.L_0814a986
	cmp	r2, #6
	beq.n	.L_0814a986
	cmp	r2, #7
	bne.n	.L_0814a990
.L_0814a986:
	ldr	r4, [sp, #72]
	ldr	r3, [r4, #20]
	lsls	r3, r3, #3
	adds	r3, #64
	b.n	.L_0814a998
.L_0814a990:
	ldr	r0, [sp, #72]
	ldr	r3, [r0, #20]
	lsls	r3, r3, #3
	adds	r3, #32
.L_0814a998:
	str	r3, [sp, #60]
	mov	r1, sl
	cmp	r1, #1
	bls.n	.L_0814a9ae
	cmp	r1, #3
	beq.n	.L_0814a9ae
	movs	r1, #200
	ldr	r0, [pc, #188]
	lsls	r1, r1, #4
	bl	sub_080145a8
.L_0814a9ae:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #156]
	bl	sub_080145a8
	movs	r0, #142
	bl	sub_081c0010
	ldr	r3, [sp, #60]
	movs	r2, #0
	str	r2, [sp, #64]
	cmp	r3, #0
	bne.n	.L_0814a9e0
	b.n	.L_0814ae74
.L_0814a9e0:
	subs	r3, #32
	str	r3, [sp, #32]
.L_0814a9e4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	mov	r4, sl
	str	r3, [sp, #48]
	cmp	r4, #7
	bne.n	.L_0814aa6c
	ldr	r3, [sp, #60]
	ldr	r0, [sp, #64]
	subs	r3, #46
	cmp	r0, r3
	bne.n	.L_0814aa0c
	ldr	r1, [sp, #72]
	movs	r3, #0
	ldr	r0, [r1, #8]
	movs	r2, #36
	ldrsh	r1, [r1, r2]
	movs	r2, #16
	bl	sub_08118078
.L_0814aa0c:
	ldr	r3, [sp, #64]
	ldr	r4, [sp, #32]
	cmp	r3, r4
	bne.n	.L_0814aa7a
	movs	r0, #134
	bl	sub_081180e8
	ldr	r2, [sp, #72]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #4
	bl	sub_08118088
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #8
	str	r3, [r2, #0]
	b.n	.L_0814aa7a
	.4byte 0x00000161
	.4byte 0x00000163
	.4byte 0x00000150
	.4byte 0x00000166
	.4byte 0x00000184
	.4byte 0x0000017f
	.4byte 0x0000017b
	.4byte 0x00000191
	.4byte 0x03000730
	.4byte 0x0000016e
	.4byte 0x00000192
	.4byte 0x02010000
	.4byte 0x08152475
	.2byte 0x3001
	.2byte 0x0814
.L_0814aa6c:
	ldr	r3, [sp, #64]
	ldr	r4, [sp, #32]
	cmp	r3, r4
	bne.n	.L_0814aa7a
	movs	r0, #133
	bl	sub_081180e8
.L_0814aa7a:
	ldr	r1, [sp, #64]
	movs	r6, #225
	lsls	r6, r6, #7
	movs	r0, #0
	movs	r7, #128
	add	r6, fp
	mov	r9, r0
	lsls	r7, r7, #11
	lsls	r5, r1, #12
.L_0814aa8c:
	adds	r0, r5, #0
	bl	sub_08002096
	movs	r3, #1
	lsls	r0, r0, #2
	subs	r0, r7, r0
	movs	r2, #128
	add	r9, r3
	asrs	r0, r0, #10
	lsls	r2, r2, #4
	mov	r4, r9
	stmia	r6!, {r0}
	adds	r5, r5, r2
	cmp	r4, #160
	bne.n	.L_0814aa8c
	movs	r0, #0
	str	r0, [sp, #56]
	ldr	r1, [sp, #72]
	ldr	r3, [r1, #20]
	cmp	r3, #0
	bne.n	.L_0814aab8
	b.n	.L_0814ae48
.L_0814aab8:
	ldr	r3, [sp, #48]
	mov	r2, sp
	mov	r4, sp
	adds	r2, #76
	adds	r3, #12
	adds	r4, #96
	movs	r0, #36
	movs	r1, #0
	str	r2, [sp, #40]
	str	r3, [sp, #24]
	str	r4, [sp, #36]
	str	r0, [sp, #20]
	str	r1, [sp, #16]
.L_0814aad2:
	ldr	r2, [sp, #20]
	ldr	r4, [sp, #72]
	ldrsh	r0, [r2, r4]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	mov	r1, sl
	str	r0, [sp, #44]
	ldr	r0, [sp, #56]
	lsls	r0, r0, #3
	str	r0, [sp, #28]
	cmp	r1, #3
	bne.n	.L_0814abcc
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #64]
	str	r2, [sp, #28]
	cmp	r3, r2
	ble.n	.L_0814abcc
	ldr	r4, [sp, #64]
	adds	r3, r2, #0
	adds	r3, #32
	cmp	r4, r3
	bge.n	.L_0814abcc
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #72]
	add	r5, sp, #84
	ldrsh	r0, [r1, r3]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r7, [sp, #64]
	movs	r4, #0
	mov	r0, sl
	mov	r9, r4
	ands	r7, r0
.L_0814ab18:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r0, #0
	ands	r2, r3
	str	r2, [sp, #12]
	bl	sub_08014878
	ldr	r2, [sp, #12]
	movs	r5, #31
	ands	r5, r0
	adds	r0, r2, #0
	bl	sub_08002096
	ldr	r6, [sp, #84]
	adds	r5, #4
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r1, [pc, #852]
	asrs	r3, r3, #17
	asrs	r6, r6, #1
	ldr	r2, [sp, #12]
	adds	r6, r6, r3
	ldrb	r3, [r1, r7]
	adds	r0, r2, #0
	lsrs	r3, r3, #1
	mov	r8, r1
	subs	r6, r6, r3
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r4, [pc, #828]
	ldr	r5, [sp, #88]
	asrs	r3, r3, #16
	subs	r5, r5, r3
	ldrb	r3, [r4, r7]
	str	r4, [sp, #8]
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	bl	sub_08014878
	ldr	r3, [pc, #812]
	movs	r2, #3
	ands	r0, r2
	ldrb	r2, [r3, r0]
	movs	r3, #3
	orrs	r3, r2
	movs	r2, #1
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r2, [pc, #792]
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	movs	r3, #178
	lsls	r3, r3, #6
	mov	r0, r8
	add	r1, fp
	adds	r1, r1, r3
	ldrb	r3, [r0, r7]
	ldr	r4, [sp, #8]
	str	r3, [sp, #0]
	movs	r2, #192
	ldrb	r3, [r4, r7]
	lsls	r2, r2, #18
	str	r3, [sp, #4]
	adds	r2, #188
	adds	r5, #16
	ldr	r4, [r2, #0]
	adds	r3, r5, #0
	ldr	r0, [sp, #68]
	adds	r2, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r3, #1
	add	r9, r3
	mov	r4, r9
	cmp	r4, #2
	bne.n	.L_0814ab18
.L_0814abcc:
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #19
	movs	r0, #188
	str	r3, [sp, #76]
	bl	sub_081963ec
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r0, #188
	ldr	r3, [r0, #0]
	ldr	r1, [sp, #40]
	str	r3, [r1, #4]
	bl	sub_08014de4
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #24]
	bl	sub_080156e8
	ldr	r2, [sp, #44]
	ldr	r4, [sp, #36]
	ldr	r3, [r2, #8]
	str	r3, [r4, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r4, #4]
	ldr	r3, [r2, #16]
	str	r3, [r4, #8]
	ldr	r0, [sp, #36]
	bl	sub_08015128
	ldr	r0, [sp, #64]
	ldr	r1, [sp, #16]
	cmp	r0, r1
	bgt.n	.L_0814ac1e
	b.n	.L_0814adfa
.L_0814ac1e:
	lsls	r5, r0, #9
	adds	r0, r5, #0
	bl	sub_08015068
	mov	r2, sl
	cmp	r2, #1
	bls.n	.L_0814ac30
	cmp	r2, #4
	bne.n	.L_0814ac36
.L_0814ac30:
	adds	r0, r5, #0
	bl	sub_08015024
.L_0814ac36:
	ldr	r4, [sp, #52]
	movs	r3, #0
	mov	r9, r3
	cmp	r4, #0
	bne.n	.L_0814ac42
	b.n	.L_0814adfa
.L_0814ac42:
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #612]
	lsls	r2, r0, #6
	lsls	r3, r0, #9
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r7, r3, r1
.L_0814ac50:
	ldr	r3, [sp, #28]
	ldr	r2, [sp, #64]
	add	r3, r9
	cmp	r2, r3
	bgt.n	.L_0814ac5c
	b.n	.L_0814adec
.L_0814ac5c:
	ldr	r3, [r7, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r7, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r7, #8]
	adds	r0, r0, r2
	asrs	r3, r3, #8
	adds	r4, r3, #0
	muls	r4, r3
	adds	r3, r4, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #564]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1240
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0814ac8a
	b.n	.L_0814adec
.L_0814ac8a:
	ldr	r3, [r7, #24]
	cmp	r3, #23
	ble.n	.L_0814ac92
	b.n	.L_0814adec
.L_0814ac92:
	adds	r1, r3, #0
	cmp	r1, #0
	bge.n	.L_0814ac9a
	adds	r1, #3
.L_0814ac9a:
	add	r5, sp, #108
	asrs	r6, r1, #2
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	mov	r0, sl
	asrs	r2, r3, #1
	str	r2, [r5, #0]
	cmp	r0, #5
	beq.n	.L_0814acb6
	cmp	r0, #7
	bne.n	.L_0814ace0
.L_0814acb6:
	lsls	r1, r6, #1
	adds	r1, r1, r6
	lsls	r1, r1, #3
	adds	r1, r1, r6
	lsls	r1, r1, #6
	movs	r3, #224
	movs	r0, #40
	lsls	r3, r3, #3
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #40]
	subs	r2, #20
	ldr	r4, [r0, #4]
	subs	r3, #20
	ldr	r0, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe03b
.L_0814ace0:
	mov	r1, sl
	cmp	r1, #6
	bne.n	.L_0814acfe
	movs	r1, #12
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #24
	str	r1, [sp, #4]
	ldr	r0, [sp, #40]
	movs	r1, #152
	lsls	r1, r1, #5
	ldr	r4, [r0, #4]
	subs	r2, #6
	subs	r3, #12
	b.n	.L_0814ad1a
.L_0814acfe:
	mov	r1, sl
	cmp	r1, #4
	bne.n	.L_0814ad24
	movs	r1, #22
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #42
	str	r1, [sp, #4]
	ldr	r0, [sp, #40]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r4, [r0, #4]
	subs	r2, #11
	subs	r3, #21
.L_0814ad1a:
	ldr	r0, [sp, #68]
	add	r1, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe019
.L_0814ad24:
	mov	r1, r9
	movs	r3, #3
	ands	r3, r1
	lsls	r1, r6, #3
	negs	r4, r3
	adds	r1, r1, r6
	orrs	r4, r3
	lsls	r1, r1, #7
	movs	r3, #224
	lsls	r3, r3, #3
	movs	r0, #24
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	ldr	r0, [sp, #40]
	lsrs	r4, r4, #31
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	subs	r2, #12
	subs	r3, #24
	ldr	r0, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4651
	cmp	r1, #1
	bls.n	.L_0814ad6a
	cmp	r1, #4
	beq.n	.L_0814ad6a
	cmp	r1, #5
	beq.n	.L_0814ad6a
	cmp	r1, #6
	bne.n	.L_0814ad96
.L_0814ad6a:
	ldr	r5, [r7, #0]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r7, #0]
	ldr	r5, [r7, #4]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r7, #4]
	ldr	r5, [r7, #8]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r7, #8]
	b.n	.L_0814ada0
.L_0814ad96:
	ldr	r3, [r7, #4]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r7, #4]
.L_0814ada0:
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #24
	bne.n	.L_0814adec
	mov	r3, sl
	cmp	r3, #1
	bls.n	.L_0814adbc
	cmp	r3, #4
	beq.n	.L_0814adbc
	cmp	r3, #5
	beq.n	.L_0814adbc
	cmp	r3, #6
	bne.n	.L_0814adc2
.L_0814adbc:
	movs	r3, #0
	str	r3, [r7, #24]
	b.n	.L_0814adec
.L_0814adc2:
	bl	sub_08014878
	movs	r4, #255
	ands	r0, r4
	subs	r0, #127
	lsls	r0, r0, #13
	str	r0, [r7, #0]
	bl	sub_08014878
	movs	r1, #255
	ands	r0, r1
	subs	r0, #255
	lsls	r0, r0, #12
	str	r0, [r7, #4]
	bl	sub_08014878
	movs	r2, #255
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #13
	str	r0, [r7, #8]
.L_0814adec:
	ldr	r4, [sp, #52]
	movs	r3, #1
	add	r9, r3
	adds	r7, #28
	cmp	r9, r4
	beq.n	.L_0814adfa
	b.n	.L_0814ac50
.L_0814adfa:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r3, [sp, #16]
	ldr	r0, [sp, #64]
	adds	r3, #16
	cmp	r0, r3
	bne.n	.L_0814ae2c
	ldr	r1, [sp, #60]
	subs	r3, r1, r0
	cmp	r3, #31
	ble.n	.L_0814ae1a
	movs	r3, #31
.L_0814ae1a:
	ldr	r2, [sp, #20]
	ldr	r1, [sp, #72]
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #56]
	bl	sub_0814cd48
.L_0814ae2c:
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	ldr	r4, [sp, #56]
	adds	r3, #8
	adds	r2, #2
	adds	r4, #1
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	str	r4, [sp, #56]
	ldr	r0, [sp, #72]
	ldr	r3, [r0, #20]
	cmp	r4, r3
	beq.n	.L_0814ae48
	b.n	.L_0814aad2
.L_0814ae48:
	movs	r1, #16
	movs	r0, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #60]
	adds	r1, #1
	str	r1, [sp, #64]
	cmp	r1, r2
	beq.n	.L_0814ae74
	b.n	.L_0814a9e4
.L_0814ae74:
	ldr	r0, [pc, #60]
	bl	sub_08014644
	mov	r3, sl
	cmp	r3, #1
	bls.n	.L_0814ae8a
	cmp	r3, #3
	beq.n	.L_0814ae8a
	ldr	r0, [pc, #48]
	bl	sub_08014644
.L_0814ae8a:
	bl	sub_08143bb8
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x08197a34
	.4byte 0x08197486
	.4byte 0x02010000
	.4byte 0x030002d4
	.4byte 0x08143001
	.2byte 0x2475
	.2byte 0x0815
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r0, [sp, #28]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	movs	r2, #0
	str	r0, [sp, #24]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r2, [sp, #8]
	str	r1, [sp, #20]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	add	r4, sp, #8
	adds	r2, #32
	strh	r3, [r2, #0]
	ldrh	r4, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r4, [r3, #0]
	ldr	r2, [sp, #24]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #32]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	ldr	r4, [r5, #104]
	movs	r0, #188
	movs	r1, #47
	str	r4, [sp, #12]
	b.n	.L_0814af28
	movs	r0, r0
	.4byte 0x00000100
	.2byte 0x00e3
	.2byte 0x0000
.L_0814af28:
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	movs	r7, #0
	str	r5, [sp, #16]
	ldr	r5, [pc, #532]
.L_0814af36:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [pc, #520]
	movs	r6, #0
	str	r3, [r5, #4]
	bl	sub_08014878
	str	r6, [r5, #16]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L_0814af36
	add	r5, sp, #44
	adds	r0, r5, #0
	bl	sub_0815e22c
	ldr	r2, [r5, #0]
	adds	r3, r2, #0
	subs	r3, #64
	str	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_0814af86
	str	r3, [sp, #8]
	str	r6, [r5, #0]
	b.n	.L_0814af92
.L_0814af86:
	cmp	r3, #112
	ble.n	.L_0814af92
	subs	r2, #176
	movs	r3, #112
	str	r2, [sp, #8]
	str	r3, [r5, #0]
.L_0814af92:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	negs	r3, r3
	adds	r2, #40
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r0, [sp, #24]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	movs	r1, #200
	movs	r3, #0
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #404]
	bl	sub_080145a8
	movs	r0, #142
	bl	sub_081c0010
	movs	r4, #0
	mov	fp, r4
.L_0814afcc:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	mov	r0, fp
	mov	sl, r3
	cmp	r0, #80
	bne.n	.L_0814afe0
	movs	r0, #0
	bl	sub_081180e8
.L_0814afe0:
	ldr	r2, [sp, #28]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_0814b04a
	movs	r3, #12
	add	r3, sl
	mov	r9, r3
	add	r6, sp, #32
	movs	r7, #36
.L_0814aff6:
	ldr	r4, [sp, #28]
	ldrsh	r0, [r7, r4]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	bl	sub_08014de4
	mov	r0, sl
	mov	r1, r9
	bl	sub_080156e8
	ldr	r3, [r5, #8]
	adds	r0, r6, #0
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	sub_08015128
	mov	r2, r8
	lsls	r3, r2, #4
	adds	r3, #64
	cmp	fp, r3
	bne.n	.L_0814b03c
	ldr	r3, [sp, #28]
	movs	r1, #0
	ldrsh	r0, [r7, r3]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r2, #5
	subs	r3, #1
	bl	sub_0814cd48
.L_0814b03c:
	ldr	r1, [sp, #28]
	movs	r0, #1
	ldr	r3, [r1, #20]
	add	r8, r0
	adds	r7, #2
	cmp	r8, r3
	bne.n	.L_0814aff6
.L_0814b04a:
	ldr	r6, [pc, #256]
	movs	r2, #32
	movs	r7, #0
	mov	r8, r2
.L_0814b052:
	lsls	r3, r7, #2
	cmp	fp, r3
	ble.n	.L_0814b0fe
	ldr	r0, [r6, #4]
	ldr	r3, [pc, #252]
	cmp	r0, r3
	bgt.n	.L_0814b0fe
	ldr	r1, [r6, #24]
	cmp	r1, #0
	bge.n	.L_0814b068
	adds	r1, #15
.L_0814b068:
	asrs	r1, r1, #4
	movs	r3, #7
	ands	r1, r3
	cmp	r1, #3
	bgt.n	.L_0814b09c
	ldr	r4, [sp, #24]
	lsls	r1, r1, #10
	movs	r2, #240
	adds	r1, r4, r1
	lsls	r2, r2, #4
	ldr	r4, [sp, #8]
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	asrs	r3, r0, #16
	adds	r2, r4, r2
	mov	r0, r8
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #20]
	ldr	r4, [sp, #12]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe012
.L_0814b09c:
	ldr	r2, [sp, #24]
	ldr	r3, [pc, #188]
	lsls	r1, r1, #10
	adds	r1, r2, r1
	adds	r1, r1, r3
	movs	r4, #2
	ldrsh	r2, [r6, r4]
	ldr	r3, [sp, #8]
	mov	r4, r8
	adds	r2, r3, r2
	asrs	r3, r0, #16
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #20]
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6934
	movs	r0, #128
	lsls	r0, r0, #6
	ldr	r2, [r6, #4]
	adds	r1, r4, r0
	ldr	r3, [r6, #24]
	ldr	r0, [r6, #8]
	movs	r5, #184
	adds	r2, r2, r4
	adds	r3, r3, r0
	lsls	r5, r5, #15
	str	r2, [r6, #4]
	str	r1, [r6, #16]
	str	r3, [r6, #24]
	cmp	r2, r5
	ble.n	.L_0814b0fe
	cmp	r1, #0
	bne.n	.L_0814b0fe
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r1, #1
	adds	r3, r4, r1
	adds	r2, r0, #4
	negs	r3, r3
	str	r2, [r6, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r5, [r6, #4]
	str	r3, [r6, #16]
.L_0814b0fe:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #12
	bne.n	.L_0814b052
	ldr	r3, [sp, #24]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	cmp	r1, #98
	beq.n	.L_0814b126
	b.n	.L_0814afcc
.L_0814b126:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xffe00000
	.4byte 0x08143001
	.4byte 0x007fffff
	.2byte 0xff00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	mov	sl, r0
	ldr	r0, [r5, #96]
	sub	sp, #40
	str	r0, [sp, #24]
	movs	r0, #1
	ldr	r1, [r5, #92]
	str	r1, [sp, #20]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r2, [sp, #20]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #36]
	movs	r3, #1
	movs	r2, #1
	bl	sub_08157cf4
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_0814b1cc
	movs	r2, #128
	ldr	r3, [pc, #20]
	lsls	r2, r2, #19
	adds	r2, #40
	str	r3, [r2, #0]
	b.n	.L_0814b1cc
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x0000013b
	.2byte 0x9000
	.2byte 0xffff
.L_0814b1cc:
	.2byte 0x2123
	movs	r0, #104
	bl	sub_081963ec
	ldr	r0, [sp, #20]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	ldr	r5, [r5, #104]
	adds	r3, #132
	adds	r2, r0, r3
	movs	r1, #200
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r0, [pc, #344]
	lsls	r1, r1, #4
	str	r5, [sp, #12]
	bl	sub_080145a8
	mov	r0, sl
	ldr	r2, [r0, #20]
	movs	r4, #0
	mov	r9, r4
	cmp	r2, #0
	beq.n	.L_0814b222
	ldr	r5, [sp, #20]
	movs	r6, #63
	adds	r5, #24
.L_0814b20c:
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #0]
	mov	r3, sl
	ldr	r2, [r3, #20]
	movs	r1, #1
	add	r9, r1
	adds	r5, #28
	cmp	r9, r2
	bne.n	.L_0814b20c
.L_0814b222:
	movs	r0, #32
	movs	r4, #0
	lsls	r3, r2, #5
	negs	r0, r0
	mov	r8, r4
	cmp	r3, r0
	beq.n	.L_0814b32c
.L_0814b230:
	mov	r1, r8
	cmp	r1, #32
	bne.n	.L_0814b240
	movs	r0, #0
	bl	sub_081180e8
	mov	r3, sl
	ldr	r2, [r3, #20]
.L_0814b240:
	movs	r4, #0
	mov	r9, r4
	cmp	r2, #0
	beq.n	.L_0814b306
	movs	r0, #36
	ldr	r1, [sp, #20]
	str	r0, [sp, #8]
	mov	fp, r1
.L_0814b250:
	mov	r2, r9
	lsls	r6, r2, #4
	cmp	r8, r6
	bne.n	.L_0814b25e
	movs	r0, #143
	bl	sub_081c0010
.L_0814b25e:
	cmp	r8, r6
	blt.n	sub_0814b2f0
	adds	r3, r6, #0
	adds	r3, #72
	cmp	r8, r3
	bge.n	sub_0814b2f0
	ldr	r3, [sp, #8]
	mov	r1, sl
	add	r5, sp, #28
	ldrsh	r0, [r3, r1]
	adds	r1, r5, #0
	bl	sub_0815e20c
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_0814b286
	ldr	r3, [r5, #0]
	subs	r3, #112
	str	r3, [r5, #0]
.L_0814b286:
	ldr	r3, [r5, #4]
	movs	r1, #20
	adds	r2, r3, #0
	subs	r2, #16
	str	r2, [r5, #4]
	subs	r3, #20
	ldr	r2, [r5, #0]
	mov	ip, r3
	ldr	r4, [sp, #20]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r3, #220
	lsls	r3, r3, #4
	str	r1, [sp, #4]
	subs	r2, #8
	adds	r1, r4, r3
	ldr	r7, [sp, #12]
	ldr	r0, [sp, #24]
	mov	r3, ip
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x45b0
	blt.n	sub_0814b2f0
	mov	r1, fp
	ldr	r3, [r1, #24]
	mov	r4, r8
	subs	r0, r4, r6
	adds	r0, r0, r3
	movs	r1, #6
	bl	sub_08002054
	movs	r1, #9
	bl	sub_08002064
	lsls	r1, r0, #1
	ldr	r2, [sp, #20]
	adds	r1, r1, r0
	lsls	r1, r1, #6
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	ldr	r2, [r5, #0]
	adds	r1, r1, r3
	ldr	r3, [r5, #4]
	movs	r0, #12
	movs	r4, #16
	str	r0, [sp, #4]
	subs	r2, #8
	subs	r3, #16
	str	r4, [sp, #0]
	ldr	r0, [sp, #24]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9802
	mov	r4, sl
	adds	r0, #2
	str	r0, [sp, #8]
	movs	r2, #1
	ldr	r3, [r4, #20]
	movs	r1, #28
	add	r9, r2
	add	fp, r1
	cmp	r9, r3
	bne.n	.L_0814b250
.L_0814b306:
	ldr	r0, [sp, #20]
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	mov	r4, sl
	ldr	r3, [r4, #20]
	movs	r2, #1
	add	r8, r2
	adds	r2, r3, #0
	lsls	r3, r2, #5
	adds	r3, #32
	cmp	r8, r3
	bne.n	.L_0814b230
.L_0814b32c:
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08143001
