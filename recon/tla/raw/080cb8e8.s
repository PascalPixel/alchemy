.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080144c0, 0x080144c0
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08020088, 0x08020088
	.set sub_08020090, 0x08020090
	.set sub_080200b0, 0x080200b0
	.set sub_08020108, 0x08020108
	.set sub_08020110, 0x08020110
	.set sub_08020128, 0x08020128
	.set sub_08020130, 0x08020130
	.set sub_08020138, 0x08020138
	.set sub_08020180, 0x08020180
	.set sub_08020218, 0x08020218
	.set sub_08038000, 0x08038000
	.set sub_08038040, 0x08038040
	.set sub_08038120, 0x08038120
	.set sub_080381e8, 0x080381e8
	.set sub_08038200, 0x08038200
	.set sub_08038208, 0x08038208
	.set sub_08038278, 0x08038278
	.set sub_08038280, 0x08038280
	.set sub_08038358, 0x08038358
	.set sub_080ad2b8, 0x080ad2b8
	.set sub_080c9934, 0x080c9934
	.set sub_080c9e1c, 0x080c9e1c
	.set sub_080c9e48, 0x080c9e48
	.set sub_080ca2fc, 0x080ca2fc
	.set sub_080ca3f4, 0x080ca3f4
	.set sub_080cad9c, 0x080cad9c
	.set sub_080cae5c, 0x080cae5c
	.set sub_080caf70, 0x080caf70
	.set sub_080cafc4, 0x080cafc4
	.set sub_080cb05c, 0x080cb05c
	.set sub_080cb2b8, 0x080cb2b8
	.set sub_080cb82c, 0x080cb82c
	.set sub_080cb8a4, 0x080cb8a4
	.set sub_080cc54c, 0x080cc54c
	.set sub_080cc9c8, 0x080cc9c8
	.set sub_080cc9fc, 0x080cc9fc
	.set sub_080cce94, 0x080cce94
	.set sub_080ccec8, 0x080ccec8
	.set sub_080cd584, 0x080cd584
	.set sub_080cd5bc, 0x080cd5bc
	.set sub_080cd680, 0x080cd680
	.set sub_080cd808, 0x080cd808
	.set sub_080cd91c, 0x080cd91c
	.set sub_080cdec8, 0x080cdec8
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080ce0ac, 0x080ce0ac
	.set sub_080ce61c, 0x080ce61c
	.set sub_080d01cc, 0x080d01cc
	.set sub_080d0520, 0x080d0520
	.set sub_080d1684, 0x080d1684
	.set sub_080d170c, 0x080d170c
	.set sub_080d2260, 0x080d2260
	.set sub_080d25c8, 0x080d25c8
	.set sub_080d260c, 0x080d260c
	.set sub_080d2aa4, 0x080d2aa4
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d5720, 0x080d5720
	.set sub_080d57e4, 0x080d57e4
	.set sub_080d59dc, 0x080d59dc
	.set sub_080d5aa0, 0x080d5aa0
	.set sub_080d5c70, 0x080d5c70
	.set sub_080d5f18, 0x080d5f18
	.set sub_080d784c, 0x080d784c
	.set sub_080d793c, 0x080d793c
	.set sub_080dec8c, 0x080dec8c
	.set sub_080e2718, 0x080e2718
	.set sub_080e7238, 0x080e7238
	.set sub_080e7804, 0x080e7804
	.set sub_080ed2a4, 0x080ed2a4
	.set sub_080ed788, 0x080ed788
	.set sub_081c0010, 0x081c0010
	.set sub_081c0070, 0x081c0070
	.global Func_080cb8e8
	.thumb_func
Func_080cb8e8:
.L_080cb8e8:
	push	{lr}
	ldr	r2, [pc, #44]
	movs	r1, #128
	movs	r3, #192
	lsls	r1, r1, #2
	lsls	r3, r3, #18
	adds	r1, #118
	ldr	r0, [r3, #108]
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_080cb912
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	lsls	r3, r3, #2
	adds	r3, #20
	ldr	r0, [r0, r3]
	b.n	.L_080cb914
.L_080cb912:
	ldr	r0, [r0, #52]
.L_080cb914:
	pop	{pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #213
	lsls	r5, r5, #4
	adds	r1, r5, #0
	movs	r0, #108
	sub	sp, #12
	bl	sub_08014d00
	adds	r1, r5, #0
	ldr	r3, [pc, #128]
	mov	r8, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2004
	adds	r0, #255
	bl	sub_08016d18
	ldr	r5, [pc, #116]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r5, r0
	ldrh	r2, [r3, #0]
	movs	r1, #244
	lsls	r1, r1, #1
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r4, #245
	lsls	r4, r4, #1
	adds	r2, r5, r4
	strh	r3, [r2, #0]
	movs	r3, #255
	adds	r0, #14
	lsls	r3, r3, #8
	adds	r2, r5, r0
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #248
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	negs	r3, r3
	adds	r4, #8
	strh	r3, [r2, #0]
	adds	r0, #6
	adds	r2, r5, r4
	strh	r3, [r2, #0]
	adds	r2, r5, r0
	strh	r3, [r2, #0]
	movs	r2, #226
	ldr	r1, [pc, #36]
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	strb	r1, [r3, #0]
	bl	sub_080144c0
	movs	r0, #0
	bl	sub_080d793c
	movs	r4, #253
	lsls	r4, r4, #1
	adds	r3, r5, r4
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r0, r3
	bne.n	.L_080cb9e0
	b.n	.L_080cb9c8
	.4byte 0x00000000
	.4byte 0x03000258
	.4byte 0x02000240
	.2byte 0x0001
	.2byte 0x0000
.L_080cb9c8:
	bl	sub_08020110
	movs	r3, #145
	lsls	r3, r3, #2
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r7, #3
	bl	sub_080c9e48
	b.n	.L_080cb9ec
.L_080cb9e0:
	bl	sub_08020108
	movs	r0, #1
	movs	r7, #2
	bl	sub_080c9e48
.L_080cb9ec:
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #10
	strb	r7, [r3, #0]
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080cba0e
	ldr	r3, [pc, #140]
	movs	r4, #200
	lsls	r4, r4, #5
	adds	r4, #76
	adds	r3, r3, r4
	strb	r0, [r3, #0]
	b.n	.L_080cba12
.L_080cba0e:
	bl	sub_080e2718
.L_080cba12:
	bl	sub_08038000
	adds	r0, r7, #0
	bl	sub_08020088
	bl	sub_080caf70
	ldr	r6, [pc, #112]
	ldr	r0, [r6, #36]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0x4641
	str	r0, [r1, #16]
	bl	sub_080cc9c8
	ldr	r0, [r6, #28]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0xf7ff
	.2byte 0xf809
	.2byte 0xf003
	.2byte 0xf8fb
	.2byte 0x200a
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cba4e
	bl	sub_080cae5c
.L_080cba4e:
	ldr	r5, [pc, #72]
	movs	r2, #149
	lsls	r2, r2, #2
	adds	r3, r5, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_080cba62
	bl	sub_080d784c
.L_080cba62:
	movs	r2, #192
	lsls	r2, r2, #4
	movs	r3, #255
	adds	r2, #184
	lsls	r3, r3, #8
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
	cmp	r7, #3
	bne.n	.L_080cba9c
	bl	sub_08020128
	movs	r0, #254
	lsls	r0, r0, #1
	movs	r1, #129
	adds	r3, r5, r0
	lsls	r1, r1, #2
	ldr	r0, [r3, #0]
	adds	r3, r5, r1
	ldr	r1, [r3, #0]
	bl	sub_08020130
	b.n	.L_080cbaaa
	.4byte 0x02001000
	.4byte 0x02008000
	.2byte 0x0240
	.2byte 0x0200
.L_080cba9c:
	bl	sub_08020180
	ldr	r0, [r6, #52]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0xf754
	.2byte 0xfb3b
.L_080cbaaa:
	.2byte 0x2020
	adds	r0, #255
	bl	sub_08016d18
	bl	sub_080d1684
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_080d170c
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r3, #128
	add	r2, r8
	lsls	r3, r3, #1
	movs	r4, #217
	str	r3, [r2, #0]
	lsls	r4, r4, #1
	adds	r3, #180
	add	r3, r8
	add	r4, r8
	mov	fp, r2
	movs	r5, #16
	movs	r2, #206
	str	r5, [r3, #0]
	mov	sl, r4
	mov	r9, r3
	lsls	r2, r2, #1
	movs	r3, #154
	movs	r7, #0
	adds	r3, #255
	mov	r0, sl
	add	r2, r8
	strh	r7, [r0, #0]
	str	r3, [r2, #0]
	adds	r3, #7
	add	r3, r8
	ldr	r6, [pc, #112]
	movs	r1, #128
	str	r7, [r3, #0]
	lsls	r1, r1, #2
	adds	r1, #94
	adds	r3, r6, r1
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #2
	bne.n	.L_080cbb14
	movs	r0, #162
	str	r7, [r2, #0]
	lsls	r0, r0, #1
	bl	sub_08016cfc
.L_080cbb14:
	movs	r3, #212
	ldr	r2, [pc, #84]
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #10
	str	r2, [r3, #0]
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080cbb70
	bl	sub_080ca2fc
	movs	r0, #147
	movs	r1, #128
	lsls	r0, r0, #2
	lsls	r1, r1, #2
	adds	r3, r6, r0
	adds	r1, #78
	strh	r5, [r3, #0]
	adds	r3, r6, r1
	strh	r7, [r3, #0]
	movs	r3, #148
	lsls	r3, r3, #2
	adds	r2, r6, r3
	movs	r4, #128
	movs	r3, #1
	strh	r3, [r2, #0]
	lsls	r4, r4, #2
	movs	r3, #255
	adds	r4, #106
	lsls	r3, r3, #8
	adds	r2, r6, r4
	adds	r3, #255
	strh	r3, [r2, #0]
	ldr	r3, [pc, #8]
	adds	r0, #33
	adds	r2, r6, r0
	strb	r3, [r2, #0]
	b.n	.L_080cbb70
	.4byte 0x00000000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x0201
.L_080cbb70:
	bl	sub_080dec8c
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_080c9934
	movs	r2, #208
	adds	r5, r0, #0
	lsls	r2, r2, #4
	lsrs	r3, r5, #4
	movs	r1, #15
	adds	r2, #55
	ands	r3, r1
	add	r2, r8
	strb	r3, [r2, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #54
	add	r3, r8
	ands	r5, r1
	strb	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #194
	adds	r6, r6, r3
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #2
	bne.n	.L_080cbbc8
	bl	sub_080cdf5c
	bl	sub_080d2d84
	str	r7, [r0, #24]
	bl	sub_080cdf5c
	bl	sub_080d2d84
	movs	r1, #0
	bl	sub_08020218
.L_080cbbc8:
	bl	sub_080cb05c
	movs	r0, #2
	bl	sub_08013560
	ldr	r3, [pc, #520]
	ldr	r0, [r3, #4]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0x23ac
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	cmp	r0, #0
	beq.n	.L_080cbbf0
	adds	r6, r0, #0
	strh	r7, [r3, #0]
	bl	.L_080cc4fa
.L_080cbbf0:
	movs	r0, #10
	adds	r0, #255
	bl	sub_08016d18
	bl	sub_080cafc4
	cmp	r0, #0
	beq.n	.L_080cbc02
	b.n	.L_080cbd7c
.L_080cbc02:
	mov	r0, sl
	ldrh	r5, [r0, #0]
	cmp	r5, #0
	bne.n	.L_080cbc7a
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #1
	bne.n	.L_080cbc2a
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #184
	movs	r0, #92
	bl	sub_08014cc0
	movs	r0, #20
	bl	sub_08013560
	movs	r0, #92
	bl	sub_0801314c
.L_080cbc2a:
	mov	r1, fp
	mov	r2, r9
	ldr	r0, [r1, #0]
	ldr	r1, [r2, #0]
	bl	sub_080d01cc
	movs	r3, #1
	mov	r4, sl
	strh	r3, [r4, #0]
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #1
	bne.n	.L_080cbc5c
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #184
	movs	r0, #92
	bl	sub_08014cc0
	movs	r0, #20
	bl	sub_08013560
	movs	r0, #92
	bl	sub_0801314c
.L_080cbc5c:
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r5, [r3, #0]
	bl	sub_080d5f18
	cmp	r0, #0
	bne.n	.L_080cbc7a
	mov	r1, r9
	ldr	r0, [r1, #0]
	adds	r0, #1
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	bl	sub_08013560
.L_080cbc7a:
	ldr	r7, [pc, #356]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #194
	adds	r2, r7, r3
	movs	r6, #0
	ldrsb	r6, [r2, r6]
	cmp	r6, #1
	bne.n	.L_080cbd08
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r4, #0
	mov	sl, r4
	bl	sub_080e7804
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #190
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #316]
	cmp	r2, r3
	bne.n	.L_080cbcc2
	movs	r0, #176
	lsls	r0, r0, #2
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_080c9e1c
	movs	r3, #16
	adds	r5, r0, #0
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080cbcd0
.L_080cbcc2:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r7, r2
	mov	r4, sl
	strh	r4, [r3, #0]
	b.n	.L_080cbce2
.L_080cbcd0:
	movs	r3, #32
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080cbce2
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #118
	adds	r3, r7, r0
	strh	r6, [r3, #0]
.L_080cbce2:
	ldr	r3, [pc, #252]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #190
	adds	r2, r3, r1
	adds	r1, #2
	adds	r3, r3, r1
	movs	r4, #0
	ldrsh	r0, [r2, r4]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_080d2aa4
	b.n	.L_080cbd14
.L_080cbcfe:
	movs	r3, #0
	adds	r6, r0, #0
	strh	r3, [r1, #0]
	bl	.L_080cc4fa
.L_080cbd08:
	cmp	r6, #2
	bne.n	.L_080cbd14
	movs	r3, #0
	strb	r3, [r2, #0]
	bl	sub_080e7238
.L_080cbd14:
	movs	r5, #48
	adds	r5, #255
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cbd38
	adds	r0, r5, #0
	bl	sub_08016d18
	ldr	r3, [pc, #180]
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r3, r4
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08038200
.L_080cbd38:
	ldr	r5, [pc, #164]
	movs	r2, #149
	lsls	r2, r2, #2
	adds	r6, r5, r2
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	cmp	r0, #0
	beq.n	.L_080cbd52
	movs	r1, #1
	bl	sub_080d793c
	movs	r3, #0
	strh	r3, [r6, #0]
.L_080cbd52:
	movs	r4, #151
	lsls	r4, r4, #2
	adds	r5, r5, r4
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080cbd7c
	bl	sub_080cb82c
	ldrh	r0, [r5, #0]
	bl	sub_080d25c8
	cmp	r0, #0
	bne.n	.L_080cbd74
	ldrh	r0, [r5, #0]
	movs	r1, #0
	bl	sub_080d260c
.L_080cbd74:
	bl	sub_080cb8a4
	movs	r3, #0
	strh	r3, [r5, #0]
.L_080cbd7c:
	ldr	r0, [pc, #96]
	movs	r2, #173
	lsls	r2, r2, #1
	str	r0, [sp, #8]
	movs	r1, #0
	add	r2, r8
	mov	sl, r1
	mov	fp, r2
.L_080cbd8c:
	movs	r0, #130
	lsls	r0, r0, #1
	bl	sub_08016cfc
	bl	.L_080cb8e8
	mov	r9, r0
	bl	sub_08020138
	ldr	r3, [sp, #8]
	movs	r4, #254
	lsls	r4, r4, #1
	mov	r0, r9
	adds	r2, r3, r4
	ldr	r3, [r0, #8]
	mov	r4, sl
	str	r3, [r2, #0]
	ldr	r1, [sp, #8]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r3, r1, r2
	str	r4, [r3, #0]
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r2, r1, r0
	mov	r1, r9
	ldr	r3, [r1, #16]
	movs	r4, #130
	str	r3, [r2, #0]
	ldr	r3, [sp, #8]
	lsls	r4, r4, #2
	adds	r2, r3, r4
	ldrh	r3, [r1, #6]
	ldr	r0, [pc, #24]
	str	r3, [r2, #0]
	mov	r3, r9
	adds	r3, #34
	ldrb	r3, [r3, #0]
	strh	r3, [r0, #0]
	b.n	.L_080cc47e
	.4byte 0x02008000
	.4byte 0x02000240
	.4byte 0x00000002
	.2byte 0x044c
	.2byte 0x0200
.L_080cbdec:
	movs	r1, #172
	lsls	r1, r1, #1
	add	r1, r8
	movs	r2, #0
	ldrsh	r0, [r1, r2]
	cmp	r0, #0
	beq.n	.L_080cbdfc
	b.n	.L_080cbcfe
.L_080cbdfc:
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, r8
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #0
	bne.n	.L_080cbe0c
	b.n	.L_080cc0c6
.L_080cbe0c:
	movs	r3, #192
	lsls	r3, r3, #4
	ldr	r0, [pc, #52]
	adds	r3, #162
	add	r3, r8
	strh	r0, [r3, #0]
	movs	r5, #1
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	negs	r5, r5
	ldr	r6, [pc, #36]
	cmp	r3, r5
	beq.n	.L_080cbe28
	b.n	.L_080cbf84
.L_080cbe28:
	bl	sub_080d2260
	bl	sub_080cb82c
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L_080cbe78
	movs	r6, #184
	lsls	r6, r6, #1
	b.n	.L_080cbe4c
	movs	r0, r0
	.2byte 0x0001
	.2byte 0x0000
.L_080cbe4c:
	mov	r0, r9
	movs	r1, #22
	bl	sub_08020090
	mov	r3, r8
	ldrsh	r0, [r6, r3]
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #60]
	movs	r1, #1
	bl	sub_08038040
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	adds	r5, #1
	adds	r6, #2
	cmp	r5, r3
	bcc.n	.L_080cbe4c
.L_080cbe78:
	movs	r3, #183
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080cbe88
	b.n	.L_080cbf7e
.L_080cbe88:
	ldr	r2, [pc, #20]
	ldr	r3, [r2, #0]
	cmp	r3, #4
	bne.n	.L_080cbea4
	mov	r0, r9
	movs	r1, #36
	bl	sub_08020090
	b.n	.L_080cbeac
	movs	r0, r0
	.4byte 0x00000d92
	.2byte 0x0454
	.2byte 0x0200
.L_080cbea4:
	mov	r0, r9
	movs	r1, #19
	bl	sub_08020090
.L_080cbeac:
	movs	r0, #59
	bl	sub_081c0010
	movs	r1, #1
	ldr	r0, [pc, #52]
	bl	sub_08038040
	ldr	r3, [pc, #52]
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	ldr	r4, [pc, #36]
	adds	r6, r0, #0
	strh	r4, [r6, #56]
	movs	r0, #1
	lsls	r5, r0, #14
	movs	r2, #52
	ldrsh	r1, [r6, r2]
	adds	r0, r5, #0
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080cbef4
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L_080cbef4
	adds	r5, r0, #0
	b.n	.L_080cbef4
	.4byte 0x00000001
	.4byte 0x00000d93
	.2byte 0x0454
	.2byte 0x0200
.L_080cbef4:
	lsls	r3, r5, #16
	strh	r5, [r6, #20]
	cmp	r3, #0
	bne.n	.L_080cbf08
	movs	r4, #56
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	beq.n	.L_080cbf08
	ldr	r0, [pc, #32]
	strh	r0, [r6, #20]
.L_080cbf08:
	movs	r1, #58
	ldrsh	r0, [r6, r1]
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080cbf2c
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080cbf2c
	adds	r3, r0, #0
	b.n	.L_080cbf2c
	.2byte 0x0001
	.2byte 0x0000
.L_080cbf2c:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080cbf40
	movs	r4, #58
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	beq.n	.L_080cbf40
	ldr	r0, [pc, #52]
	strh	r0, [r6, #22]
.L_080cbf40:
	ldr	r1, [sp, #8]
	movs	r2, #242
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r0, [r3, #0]
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r1, r4
	strh	r0, [r3, #0]
	adds	r2, #2
	adds	r3, r1, r2
	ldrh	r1, [r3, #0]
	ldr	r3, [pc, #28]
	lsls	r0, r0, #16
	strh	r1, [r3, #0]
	movs	r6, #186
	lsls	r1, r1, #16
	asrs	r0, r0, #16
	asrs	r1, r1, #16
	lsls	r6, r6, #2
	bl	sub_080ca3f4
	adds	r6, #255
	bl	sub_080cb8a4
	b.n	.L_080cbf7c
	.4byte 0x00000001
	.2byte 0x0422
	.2byte 0x0200
.L_080cbf7c:
	b.n	.L_080cc4fa
.L_080cbf7e:
	bl	sub_080cb8a4
	b.n	.L_080cc0b6
.L_080cbf84:
	ldr	r4, [pc, #392]
	cmp	r3, r4
	bne.n	.L_080cbfb4
	bl	sub_080d2260
	movs	r0, #1
	bl	sub_080ed2a4
	ldr	r2, [pc, #380]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #190
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, r5
	bne.n	.L_080cbfa8
	b.n	.L_080cc0b6
.L_080cbfa8:
	movs	r4, #128
	lsls	r4, r4, #2
	adds	r4, #194
	adds	r3, r2, r4
	strb	r6, [r3, #0]
	b.n	.L_080cc0b6
.L_080cbfb4:
	ldr	r0, [pc, #352]
	cmp	r3, r0
	bne.n	.L_080cbfd0
	bl	sub_080d2260
	movs	r5, #0
	movs	r6, #1
.L_080cbfc2:
	adds	r0, r5, #0
	bl	sub_080ed2a4
	eors	r5, r6
	cmp	r0, #0
	bne.n	.L_080cbfc2
	b.n	.L_080cc0b6
.L_080cbfd0:
	movs	r3, #181
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldr	r2, [pc, #320]
	cmp	r0, r2
	bne.n	.L_080cbfea
	bl	sub_080d2260
	bl	sub_080ed788
	b.n	.L_080cc0b6
.L_080cbfea:
	ldr	r3, [pc, #308]
	cmp	r0, r3
	bne.n	.L_080cc006
	movs	r3, #195
	ldr	r2, [pc, #304]
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L_080cc0b6
	bl	sub_080d5720
	b.n	.L_080cc03c
.L_080cc006:
	ldr	r1, [pc, #288]
	cmp	r0, r1
	bne.n	.L_080cc022
	movs	r3, #195
	ldr	r2, [pc, #276]
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L_080cc0b6
	bl	sub_080d57e4
	b.n	.L_080cc084
.L_080cc022:
	ldr	r3, [pc, #264]
	cmp	r0, r3
	bne.n	.L_080cc06a
	movs	r3, #195
	ldr	r2, [pc, #248]
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L_080cc0b6
	bl	sub_080d59dc
.L_080cc03c:
	movs	r3, #175
	lsls	r3, r3, #1
	mov	r4, sl
	mov	r0, fp
	add	r3, r8
	mov	r1, sl
	strh	r4, [r0, #0]
	strh	r1, [r3, #0]
	movs	r3, #174
	lsls	r3, r3, #1
	add	r3, r8
	mov	r2, sl
	strh	r2, [r3, #0]
	movs	r3, #178
	lsls	r3, r3, #1
	add	r3, r8
	strh	r4, [r3, #0]
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, r8
	mov	r0, sl
	strh	r0, [r3, #0]
	b.n	.L_080cc0b6
.L_080cc06a:
	ldr	r1, [pc, #196]
	cmp	r0, r1
	bne.n	.L_080cc0b2
	movs	r3, #195
	ldr	r2, [pc, #176]
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r3, [r3, #0]
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L_080cc0b6
	bl	sub_080d5aa0
.L_080cc084:
	mov	r2, sl
	mov	r3, fp
	strh	r2, [r3, #0]
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, r8
	mov	r4, sl
	strh	r4, [r3, #0]
	movs	r3, #174
	lsls	r3, r3, #1
	add	r3, r8
	mov	r0, sl
	strh	r0, [r3, #0]
	movs	r3, #178
	lsls	r3, r3, #1
	add	r3, r8
	mov	r1, sl
	strh	r1, [r3, #0]
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, r8
	strh	r2, [r3, #0]
	b.n	.L_080cc0b6
.L_080cc0b2:
	bl	sub_080cd584
.L_080cc0b6:
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #162
	add	r3, r8
	mov	r4, sl
	strh	r4, [r3, #0]
	movs	r3, #181
	b.n	.L_080cc476
.L_080cc0c6:
	movs	r6, #170
	lsls	r6, r6, #1
	add	r6, r8
	movs	r2, #0
	ldrsh	r7, [r6, r2]
	cmp	r7, #0
	beq.n	.L_080cc0f2
	movs	r5, #192
	ldr	r4, [pc, #52]
	lsls	r5, r5, #4
	adds	r5, #162
	add	r5, r8
	strh	r4, [r5, #0]
	str	r3, [sp, #0]
	movs	r1, #0
	ldrsh	r0, [r6, r1]
	bl	sub_080cd5bc
	ldr	r3, [sp, #0]
	strh	r3, [r5, #0]
	strh	r3, [r6, #0]
	b.n	.L_080cc47e
.L_080cc0f2:
	movs	r5, #171
	lsls	r5, r5, #1
	add	r5, r8
	movs	r2, #0
	ldrsh	r6, [r5, r2]
	cmp	r6, #0
	beq.n	.L_080cc134
	adds	r0, r6, #0
	bl	sub_080cd680
.L_080cc106:
	strh	r7, [r5, #0]
	b.n	.L_080cc47e
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0xfffffc89
	.4byte 0x02000240
	.4byte 0xfffffc88
	.4byte 0xfffffc87
	.4byte 0xfffffc85
	.4byte 0x03001150
	.4byte 0xfffffc86
	.4byte 0xfffffc83
	.2byte 0xfc84
	.2byte 0xffff
.L_080cc134:
	movs	r5, #178
	lsls	r5, r5, #1
	add	r5, r8
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_080cc192
	str	r1, [sp, #4]
	bl	sub_080d2260
	bl	sub_080cad9c
	movs	r3, #128
	ldr	r0, [sp, #8]
	lsls	r3, r3, #2
	adds	r3, #62
	adds	r2, r0, r3
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	movs	r4, #240
	strh	r3, [r2, #0]
	lsls	r4, r4, #1
	movs	r3, #255
	adds	r2, r0, r4
	lsls	r3, r3, #1
	strh	r3, [r2, #0]
	ldr	r0, [pc, #172]
	ldrh	r3, [r5, #0]
	ldr	r1, [sp, #4]
	strh	r3, [r0, #0]
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	strh	r3, [r1, #0]
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	bl	sub_080d5c70
	movs	r3, #202
	ldr	r2, [pc, #148]
	lsls	r3, r3, #1
	add	r3, r8
	str	r6, [r3, #0]
	strh	r6, [r5, #0]
	str	r6, [r2, #0]
	b.n	.L_080cc47e
.L_080cc192:
	movs	r3, #174
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_080cc238
	bl	sub_08038208
	ldr	r3, [pc, #120]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #118
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_080cc1be
	ldr	r2, [pc, #104]
	ldr	r0, [r2, #0]
	b.n	.L_080cc1c0
.L_080cc1be:
	movs	r0, #8
.L_080cc1c0:
	bl	sub_080cd91c
	adds	r5, r0, #0
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L_080cc1e2
	movs	r6, #1
	cmp	r5, #63
	bgt.n	.L_080cc1e2
	adds	r0, r5, #0
	bl	sub_080cce94
	cmp	r0, #0
	beq.n	.L_080cc1e0
	movs	r0, #1
.L_080cc1e0:
	adds	r6, r0, #0
.L_080cc1e2:
	cmp	r6, #0
	beq.n	.L_080cc1fa
	ldr	r3, [pc, #44]
	mov	r4, sl
	orrs	r5, r3
	movs	r3, #176
	lsls	r3, r3, #1
	add	r3, r8
	mov	r0, fp
	strh	r5, [r3, #0]
	strh	r4, [r0, #0]
	b.n	.L_080cc22e
.L_080cc1fa:
	bl	sub_080cc54c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080cc228
	movs	r3, #177
	lsls	r3, r3, #1
	add	r3, r8
	mov	r1, sl
	mov	r2, fp
	strh	r6, [r3, #0]
	strh	r1, [r2, #0]
	b.n	.L_080cc22e
	.4byte 0x00004000
	.4byte 0x02000422
	.4byte 0x02000498
	.4byte 0x02000240
	.2byte 0x0454
	.2byte 0x0200
.L_080cc228:
	ldr	r3, [pc, #8]
	mov	r4, fp
	strh	r3, [r4, #0]
.L_080cc22e:
	movs	r3, #174
	b.n	.L_080cc476
	movs	r0, r0
	.2byte 0x0001
	.2byte 0x0000
.L_080cc238:
	mov	r2, fp
	movs	r1, #0
	ldrsh	r7, [r2, r1]
	cmp	r7, #0
	beq.n	.L_080cc2c6
	bl	sub_08038208
	bl	sub_080d2260
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080cb82c
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_08016cfc
	ldr	r3, [pc, #168]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080cc280
	ldr	r1, [pc, #164]
	movs	r2, #2
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cc280
	ldr	r3, [r1, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cc280
	bl	sub_08038280
	b.n	.L_080cc2ae
.L_080cc280:
	movs	r0, #8
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cc298
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L_080cc2ae
.L_080cc298:
	movs	r5, #192
	lsls	r5, r5, #4
	adds	r5, #165
	add	r5, r8
	strb	r0, [r5, #0]
	bl	sub_080381e8
	movs	r3, #1
	strb	r3, [r5, #0]
	bl	sub_080cb05c
.L_080cc2ae:
	bl	sub_080cb8a4
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_08016d18
	bl	sub_080ad2b8
	mov	r4, sl
	mov	r0, fp
	strh	r4, [r0, #0]
	b.n	.L_080cc47e
.L_080cc2c6:
	movs	r5, #176
	lsls	r5, r5, #1
	add	r5, r8
	movs	r1, #0
	ldrsh	r6, [r5, r1]
	cmp	r6, #0
	beq.n	.L_080cc2e8
	bl	sub_080cb82c
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #40]
	ands	r0, r3
	bl	sub_080ccec8
	bl	sub_080cb8a4
	b.n	.L_080cc106
.L_080cc2e8:
	movs	r5, #177
	lsls	r5, r5, #1
	add	r5, r8
	movs	r2, #0
	ldrsh	r7, [r5, r2]
	cmp	r7, #0
	beq.n	.L_080cc310
	bl	sub_080cb82c
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_080cd808
	b.n	.L_080cc378
	.4byte 0x00003fff
	.4byte 0x03001238
	.2byte 0x1150
	.2byte 0x0300
.L_080cc310:
	movs	r5, #179
	lsls	r5, r5, #1
	add	r5, r8
	movs	r4, #0
	ldrsh	r6, [r5, r4]
	ldrh	r2, [r5, #0]
	cmp	r6, #0
	beq.n	.L_080cc354
	ldr	r3, [pc, #44]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r6, r3, #16
	bl	sub_08038208
	cmp	r6, #0
	bne.n	.L_080cc33a
	bl	sub_080cb82c
	movs	r0, #1
	bl	sub_08013560
.L_080cc33a:
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	bl	sub_080ce61c
	cmp	r6, #0
	beq.n	.L_080cc348
	b.n	.L_080cc106
.L_080cc348:
	bl	sub_080cb8a4
	b.n	.L_080cc106
	movs	r0, r0
	.2byte 0x2000
	.2byte 0x0000
.L_080cc354:
	movs	r5, #180
	lsls	r5, r5, #1
	add	r5, r8
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_080cc380
	bl	sub_080cb82c
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	movs	r3, #193
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	bl	sub_080ce0ac
.L_080cc378:
	bl	sub_080cb8a4
	strh	r6, [r5, #0]
	b.n	.L_080cc47e
.L_080cc380:
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, r8
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_080cc47e
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080d2260
	bl	sub_080cb82c
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_08016cfc
	ldr	r1, [pc, #172]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080cc3be
	ldr	r3, [pc, #168]
	movs	r2, #2
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cc3be
	bl	sub_08038278
	b.n	.L_080cc468
.L_080cc3be:
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080cc3d8
	ldr	r3, [pc, #144]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cc3d8
	bl	sub_080cc9fc
	b.n	.L_080cc468
.L_080cc3d8:
	movs	r0, #8
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cc3f0
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L_080cc468
.L_080cc3f0:
	bl	sub_080cdec8
	bl	sub_08038208
	bl	sub_081c0070
	movs	r2, #128
	ldr	r1, [sp, #8]
	lsls	r2, r2, #2
	adds	r2, #62
	adds	r3, r1, r2
	strh	r0, [r3, #0]
	movs	r0, #191
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080cc460
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #24]
	ldr	r1, [pc, #64]
	movs	r2, #128
	movs	r0, #0
	lsls	r2, r2, #2
.L_080cc422:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L_080cc42c
	adds	r0, #1
.L_080cc42c:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_080cc422
	adds	r3, r0, #0
	subs	r3, #136
	cmp	r3, #0
	bge.n	.L_080cc444
	ldr	r3, [pc, #20]
	movs	r0, #1
	strh	r3, [r5, #4]
	bl	sub_08013560
.L_080cc444:
	movs	r0, #0
	bl	sub_08038358
	mov	r4, sl
	strh	r4, [r5, #4]
	b.n	.L_080cc468
	.4byte 0x00000001
	.4byte 0x03001238
	.4byte 0x03001150
	.2byte 0x3410
	.2byte 0x0200
.L_080cc460:
	ldr	r0, [pc, #220]
	movs	r1, #1
	bl	sub_08038040
.L_080cc468:
	bl	sub_080cb8a4
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r3, #175
.L_080cc476:
	lsls	r3, r3, #1
	add	r3, r8
	mov	r0, sl
	strh	r0, [r3, #0]
.L_080cc47e:
	bl	sub_080cafc4
	cmp	r0, #0
	beq.n	.L_080cc488
	b.n	.L_080cbdec
.L_080cc488:
	movs	r0, #130
	lsls	r0, r0, #1
	bl	sub_08016d18
	bl	.L_080cb8e8
	cmp	r0, #0
	beq.n	.L_080cc49c
	bl	sub_080200b0
.L_080cc49c:
	ldr	r5, [pc, #164]
.L_080cc49e:
	movs	r0, #1
	bl	sub_08013560
	bl	.L_080cb8e8
	ldr	r3, [pc, #156]
	mov	r9, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080cc4be
	movs	r0, #100
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080cc4f0
.L_080cc4be:
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_080cc4de
	mov	r3, r9
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_080cc4f0
	mov	r3, r9
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_080cc4f0
.L_080cc4de:
	mov	r3, r9
	adds	r3, #34
	ldrb	r0, [r3, #0]
	mov	r3, r9
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #12]
	ldr	r3, [r3, #16]
	bl	sub_080cb2b8
.L_080cc4f0:
	bl	sub_080cafc4
	cmp	r0, #0
	beq.n	.L_080cc49e
	b.n	.L_080cbd8c
.L_080cc4fa:
	movs	r7, #217
	lsls	r7, r7, #1
	add	r7, r8
	ldrh	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_080cc524
	movs	r3, #214
	movs	r5, #218
	lsls	r3, r3, #1
	lsls	r5, r5, #1
	add	r3, r8
	add	r5, r8
	ldr	r0, [r3, #0]
	ldr	r1, [r5, #0]
	bl	sub_080d0520
	movs	r3, #0
	strh	r3, [r7, #0]
	ldr	r0, [r5, #0]
	bl	sub_08013560
.L_080cc524:
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #108
	bl	sub_0801314c
	adds	r0, r6, #0
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
