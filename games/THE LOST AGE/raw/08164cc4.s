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
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08020010, 0x08020010
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08020048, 0x08020048
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08118040, 0x08118040
	.set sub_08118078, 0x08118078
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08118118, 0x08118118
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814c97c, 0x0814c97c
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08163c2c, 0x08163c2c
	.set sub_08164a4c, 0x08164a4c
	.set sub_08164c0c, 0x08164c0c
	.set sub_08164e1e, 0x08164e1e
	.set sub_08166000, 0x08166000
	.set sub_08166426, 0x08166426
	.set sub_08166858, 0x08166858
	.set sub_081668ca, 0x081668ca
	.set sub_08166a0a, 0x08166a0a
	.set sub_08166b10, 0x08166b10
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Overlay_08164cc4
Overlay_08164cc4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #280
	str	r1, [sp, #76]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r0, #0
	ldr	r0, [r3, #96]
	mov	r1, sp
	adds	r1, #148
	str	r0, [sp, #72]
	str	r1, [sp, #60]
	movs	r0, #128
	ldr	r3, [r3, #92]
	lsls	r0, r0, #6
	str	r3, [r1, #0]
	bl	sub_081435e0
	movs	r2, #128
	ldr	r3, [pc, #12]
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r2, [sp, #76]
	cmp	r2, #1
	bne.n	.L_08164d46
	b.n	.L_08164d08
	.2byte 0x0100
	.2byte 0x0000
.L_08164d08:
	ldr	r0, [r6, #8]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r3, #160
	lsls	r3, r3, #12
	str	r3, [r2, #40]
	movs	r3, #145
	lsls	r3, r3, #8
	adds	r3, #235
	str	r3, [r2, #72]
	movs	r5, #1
	negs	r5, r5
	movs	r3, #0
	ldr	r0, [r6, #8]
	adds	r1, r5, #0
	str	r3, [sp, #0]
	movs	r2, #2
	adds	r3, r5, #0
	bl	sub_0814cd48
	movs	r0, #145
	bl	sub_081c0010
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #76]
	str	r4, [sp, #64]
	cmp	r3, #1
	beq.n	.L_08164d4c
	str	r5, [sp, #64]
	b.n	.L_08164d4c
.L_08164d46:
	movs	r0, #1
	negs	r0, r0
	str	r0, [sp, #64]
.L_08164d4c:
	bl	sub_0813ba50
	ldr	r2, [pc, #52]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r1, [sp, #60]
	ldr	r5, [pc, #44]
	ldr	r3, [r1, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r3, r2
	movs	r1, #200
	movs	r2, #0
	str	r2, [r3, #0]
	lsls	r1, r1, #4
	adds	r0, r5, #0
	bl	sub_080145a8
	movs	r0, #0
	movs	r1, #0
	bl	sub_08163c2c
	adds	r0, r5, #0
	bl	sub_08014644
	ldr	r3, [sp, #76]
	b.n	.L_08164d90
	.4byte 0x00000000
	.2byte 0x3001
	.2byte 0x0814
.L_08164d90:
	cmp	r3, #1
	bne.n	.L_08164de2
	movs	r5, #238
	movs	r6, #128
	movs	r4, #0
	lsls	r5, r5, #7
	lsls	r6, r6, #2
	mov	r8, r4
	adds	r5, #220
	adds	r6, #126
.L_08164da4:
	adds	r0, r6, #0
	bl	sub_08020040
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #0]
	str	r0, [r3, r5]
	cmp	r0, #0
	beq.n	.L_08164dcc
	movs	r3, #0
	strb	r3, [r0, #26]
	movs	r1, #2
	bl	sub_08020030
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	movs	r2, #12
	ldr	r1, [r3, r5]
	ldrb	r3, [r1, #9]
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_08164dcc:
	movs	r3, #128
	movs	r4, #1
	lsls	r3, r3, #6
	add	r8, r4
	adds	r3, #1
	mov	r0, r8
	adds	r5, #4
	adds	r6, r6, r3
	cmp	r0, #2
	bne.n	.L_08164da4
	b.n	.L_08164df0
.L_08164de2:
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #138
	movs	r0, #1
	movs	r2, #3
	bl	sub_08152404
.L_08164df0:
	ldr	r2, [sp, #60]
	movs	r3, #224
	ldr	r1, [r2, #0]
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r0, [pc, #124]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #76]
	cmp	r4, #1
	bne.n	sub_08164e1e
	ldr	r0, [pc, #112]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #104]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2180
	ldr	r3, [pc, #96]
	lsls	r1, r1, #8
	ldr	r2, [pc, #96]
	ldr	r0, [pc, #100]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b14
	movs	r2, #240
	ldr	r1, [pc, #88]
	lsls	r2, r2, #7
	ldr	r0, [pc, #88]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2001
	bl	sub_08013560
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	subs	r2, #48
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #22
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	ldr	r1, [pc, #56]
	strh	r3, [r2, #0]
	movs	r0, #0
	mov	r8, r0
	movs	r7, #15
	mov	sl, r1
	b.n	.L_08164e98
	.4byte 0x00000000
	.4byte 0x00000100
	.4byte 0x00001f80
	.4byte 0x00002787
	.4byte 0x00000185
	.4byte 0x00000188
	.4byte 0x03000730
	.4byte 0x03000260
	.4byte 0x01010101
	.4byte 0x02010000
	.4byte 0x06008000
	.2byte 0x0100
	.2byte 0x0500
.L_08164e98:
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ands	r5, r7
	ands	r0, r7
	adds	r5, #16
	adds	r0, #16
	ands	r6, r7
	lsls	r0, r0, #10
	lsls	r5, r5, #5
	adds	r6, #16
	orrs	r0, r5
	movs	r4, #1
	orrs	r0, r6
	mov	r2, sl
	add	r8, r4
	strh	r0, [r2, #0]
	movs	r3, #2
	mov	r0, r8
	add	sl, r3
	cmp	r0, #63
	bne.n	.L_08164e98
	movs	r1, #128
	ldr	r3, [pc, #460]
	ldr	r0, [sp, #72]
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2100
	movs	r2, #127
	mov	r8, r1
	mov	sl, r2
	movs	r7, #7
.L_08164ee6:
	bl	sub_08014878
	mov	r3, sl
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	mov	r4, sl
	adds	r5, r0, #0
	ands	r5, r4
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r1, r3, #0
	adds	r1, #64
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08164f0e
	adds	r3, r5, #7
.L_08164f0e:
	asrs	r3, r3, #3
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_08164f18
	adds	r2, r6, #7
.L_08164f18:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ands	r5, r7
	lsls	r3, r3, #3
	adds	r3, r3, r5
	ldr	r0, [sp, #72]
	ands	r6, r7
	lsls	r3, r3, #3
	adds	r3, r3, r6
	strb	r1, [r0, r3]
	movs	r2, #128
	movs	r1, #1
	add	r8, r1
	lsls	r2, r2, #1
	cmp	r8, r2
	bne.n	.L_08164ee6
	movs	r2, #128
	ldr	r1, [sp, #72]
	ldr	r3, [pc, #356]
	lsls	r2, r2, #7
	ldr	r0, [pc, #356]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a58
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r4, [sp, #60]
	movs	r0, #240
	ldr	r3, [r4, #0]
	lsls	r0, r0, #7
	adds	r0, #240
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r1, [sp, #60]
	movs	r4, #238
	ldr	r2, [r1, #0]
	lsls	r4, r4, #7
	movs	r0, #238
	adds	r4, #208
	lsls	r0, r0, #7
	adds	r3, r2, r4
	movs	r1, #0
	adds	r0, #212
	str	r1, [r3, #0]
	subs	r4, #64
	adds	r3, r2, r0
	str	r1, [r3, #0]
	subs	r0, #64
	adds	r3, r2, r4
	str	r1, [r3, #0]
	adds	r1, r2, r0
	movs	r3, #2
	str	r3, [r1, #0]
	ldr	r4, [sp, #64]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #152
	adds	r1, r2, r3
	adds	r0, #8
	lsls	r3, r4, #7
	str	r3, [r1, #0]
	adds	r2, r2, r0
	mov	r1, r8
	str	r1, [r2, #0]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #255
	ldr	r0, [pc, #264]
	bl	sub_080145a8
	movs	r1, #200
	ldr	r0, [pc, #260]
	lsls	r1, r1, #4
	bl	sub_080145a8
	add	r2, sp, #152
	mov	sl, r2
	add	r3, sp, #280
	movs	r6, #63
	mov	r5, sl
	mov	r8, r3
.L_08164fc0:
	bl	sub_08014878
	ands	r0, r6
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r8
	bne.n	.L_08164fc0
	movs	r4, #1
	movs	r6, #0
	mov	r8, r4
	movs	r5, #0
.L_08164fd6:
	mov	r0, r8
	lsrs	r3, r0, #31
	add	r3, r8
	asrs	r3, r3, #1
	movs	r1, #4
	adds	r6, r6, r3
	add	r8, r1
	cmp	r5, r6
	beq.n	.L_08165046
	movs	r2, #127
	movs	r3, #0
	mov	r7, sl
	movs	r4, #7
	mov	lr, r2
	mov	ip, r3
.L_08164ff4:
	movs	r0, #0
.L_08164ff6:
	mov	r1, lr
	adds	r3, r0, #0
	ands	r3, r1
	ldrb	r3, [r7, r3]
	subs	r1, r5, r3
	cmp	r1, #0
	blt.n	.L_08165036
	cmp	r1, #127
	bgt.n	.L_08165036
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_08165010
	adds	r2, r1, #7
.L_08165010:
	asrs	r2, r2, #3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_0816501a
	adds	r3, r0, #7
.L_0816501a:
	asrs	r3, r3, #3
	lsls	r2, r2, #5
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	adds	r3, r0, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #136]
	mov	r1, ip
	adds	r2, r2, r3
	strb	r1, [r2, #0]
.L_08165036:
	movs	r2, #128
	adds	r0, #1
	lsls	r2, r2, #1
	cmp	r0, r2
	bne.n	.L_08164ff6
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_08164ff4
.L_08165046:
	ldr	r4, [sp, #60]
	movs	r0, #240
	ldr	r3, [r4, #0]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r3, r3, r0
	movs	r7, #1
	str	r7, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	cmp	r6, #191
	ble.n	.L_08164fd6
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [pc, #72]
	movs	r5, #192
	ldrh	r1, [r2, #4]
	lsls	r5, r5, #18
	str	r1, [sp, #56]
	movs	r0, #104
	ldrh	r3, [r2, #6]
	movs	r1, #8
	str	r3, [sp, #52]
	adds	r3, r5, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	movs	r6, #3
	str	r3, [sp, #48]
	movs	r3, #0
	strh	r3, [r2, #4]
	movs	r3, #32
	strh	r3, [r2, #6]
	movs	r2, #7
	b.n	.L_081650c0
	.4byte 0x00003f42
	.4byte 0x00001010
	.4byte 0x03000260
	.4byte 0x03000730
	.4byte 0x06004000
	.4byte 0x030011e0
	.4byte 0x0813bb39
	.4byte 0x08143175
	.4byte 0x02010000
	.2byte 0x1120
	.2byte 0x0300
.L_081650c0:
	movs	r3, #3
	str	r7, [sp, #0]
	bl	sub_08196404
	ldr	r5, [r5, #104]
	ldr	r4, [sp, #60]
	str	r5, [sp, #68]
	movs	r0, #239
	ldr	r3, [r4, #0]
	lsls	r0, r0, #7
	movs	r1, #238
	adds	r2, r3, r0
	lsls	r1, r1, #7
	str	r6, [r2, #0]
	adds	r1, #132
	ldr	r2, [pc, #308]
	adds	r3, r3, r1
	movs	r1, #192
	lsls	r1, r1, #4
	str	r2, [r3, #0]
	ldr	r0, [pc, #300]
	adds	r1, #254
	bl	sub_080145a8
	ldr	r4, [sp, #60]
	movs	r2, #0
	ldr	r3, [r4, #0]
	mov	r8, r2
	adds	r3, #24
	subs	r2, #1
.L_081650fc:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #64
	bne.n	.L_081650fc
	ldr	r2, [sp, #48]
	movs	r3, #1
	str	r3, [r2, #16]
	ldr	r4, [sp, #60]
	movs	r0, #238
	ldr	r3, [r4, #0]
	lsls	r0, r0, #7
	adds	r0, #140
	movs	r2, #0
	adds	r3, r3, r0
	str	r2, [r3, #0]
	mov	fp, r2
	mov	r1, sp
	mov	r2, sp
	adds	r1, #132
	adds	r2, #88
	movs	r3, #0
	str	r1, [sp, #24]
	str	r2, [sp, #44]
	str	r4, [sp, #40]
	str	r3, [sp, #16]
.L_08165134:
	ldr	r4, [sp, #60]
	movs	r1, #238
	ldr	r0, [r4, #0]
	lsls	r1, r1, #7
	adds	r1, #140
	adds	r3, r0, r1
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_0816514a
	adds	r3, #3
.L_0816514a:
	asrs	r4, r3, #2
	movs	r2, #236
	ldr	r3, [sp, #76]
	lsls	r2, r2, #7
	adds	r2, #64
	adds	r5, r0, r2
	cmp	r3, #1
	bne.n	.L_0816516e
	ldr	r3, [pc, #192]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08165182
	mov	r0, fp
	cmp	r0, #16
	ble.n	.L_08165182
	b.n	.L_0816553e
.L_0816516e:
	ldr	r3, [pc, #172]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08165182
	mov	r1, fp
	cmp	r1, #4
	ble.n	.L_08165182
	b.n	.L_0816553e
.L_08165182:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_08165192
	movs	r0, #141
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r4, [sp, #8]
.L_08165192:
	movs	r3, #0
	mov	r8, r3
.L_08165196:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r1, #15
	bne.n	.L_08165196
.L_081651a4:
	mov	r1, r8
	subs	r1, #16
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_081651b2
	mov	r3, r8
	subs	r3, #13
.L_081651b2:
	asrs	r3, r3, #2
	adds	r2, r3, r4
	adds	r3, r2, #0
	adds	r1, r2, #0
	subs	r3, #32
	subs	r1, #80
	cmp	r3, #0
	bge.n	.L_081651c4
	movs	r3, #0
.L_081651c4:
	cmp	r3, #31
	ble.n	.L_081651ca
	movs	r3, #31
.L_081651ca:
	cmp	r1, #0
	bge.n	.L_081651d0
	movs	r1, #0
.L_081651d0:
	cmp	r1, #31
	ble.n	.L_081651d6
	movs	r1, #31
.L_081651d6:
	lsls	r2, r1, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	asrs	r2, r1, #1
	orrs	r3, r2
	movs	r2, #1
	add	r8, r2
	strh	r3, [r5, #0]
	mov	r3, r8
	adds	r5, #2
	cmp	r3, #135
	bne.n	.L_081651a4
	ldr	r3, [pc, #32]
.L_081651f0:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r0, #160
	bne.n	.L_081651f0
	ldr	r1, [sp, #64]
	cmp	r1, #1
	bne.n	.L_08165220
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_0816520c
	adds	r3, #3
.L_0816520c:
	asrs	r7, r3, #2
	b.n	.L_0816522e
	.4byte 0x00000000
	.4byte 0x02020202
	.4byte 0x08164bb5
	.2byte 0x1150
	.2byte 0x0300
.L_08165220:
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_08165228
	adds	r2, #3
.L_08165228:
	asrs	r2, r2, #2
	movs	r3, #64
	subs	r7, r3, r2
.L_0816522e:
	ldr	r4, [sp, #24]
	movs	r2, #96
	mov	r3, fp
	subs	r3, r2, r3
	mov	sl, r3
	movs	r3, #0
	str	r3, [r4, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	ldr	r0, [sp, #76]
	cmp	r0, #1
	bne.n	.L_0816529e
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #44]
	movs	r2, #160
	lsls	r2, r2, #8
	adds	r6, r1, r2
	str	r6, [sp, #88]
	str	r6, [r3, #4]
	ldr	r0, [sp, #24]
	movs	r4, #160
	lsls	r4, r4, #15
	lsls	r3, r7, #16
	adds	r3, r3, r4
	str	r3, [r0, #0]
	mov	r1, sl
	movs	r3, #64
	subs	r3, r3, r1
	lsls	r3, r3, #16
	str	r3, [r0, #8]
	ldr	r2, [sp, #40]
	movs	r4, #238
	ldr	r3, [r2, #0]
	lsls	r4, r4, #7
	adds	r4, #220
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	sub_08020010
	ldr	r0, [sp, #40]
	movs	r1, #238
	ldr	r3, [r0, #0]
	lsls	r1, r1, #7
	adds	r1, #224
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	sub_08020010
	b.n	.L_081652d8
.L_0816529e:
	ldr	r3, [sp, #16]
	ldr	r0, [sp, #44]
	movs	r4, #128
	lsls	r4, r4, #9
	adds	r6, r3, r4
	str	r6, [sp, #88]
	str	r6, [r0, #4]
	ldr	r4, [sp, #24]
	movs	r1, #192
	lsls	r1, r1, #15
	lsls	r3, r7, #16
	adds	r3, r3, r1
	mov	r0, sl
	str	r3, [r4, #0]
	subs	r3, r2, r0
	lsls	r3, r3, #16
	str	r3, [r4, #8]
	ldr	r1, [sp, #60]
	movs	r2, #238
	ldr	r3, [r1, #0]
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	sub_08020010
.L_081652d8:
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	movs	r3, #32
	subs	r4, r3, r4
	mov	sl, r4
	movs	r2, #0
.L_081652e6:
	ldr	r0, [sp, #60]
	movs	r1, #1
	ldr	r3, [r0, #0]
	negs	r1, r1
	adds	r5, r3, r2
	ldr	r3, [r5, #24]
	cmp	r3, r1
	bne.n	.L_0816535c
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	movs	r2, #128
	ands	r3, r0
	lsls	r2, r2, #7
	adds	r1, r3, r2
	movs	r3, #0
	str	r3, [r5, #24]
	adds	r0, r1, #0
	str	r1, [sp, #12]
	bl	sub_08002096
	adds	r3, r7, #0
	adds	r3, #96
	lsls	r2, r3, #16
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r3, r3, #1
	ldr	r1, [sp, #12]
	cmp	r3, #0
	bge.n	.L_0816532e
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r3, r3, r4
.L_0816532e:
	asrs	r3, r3, #16
	muls	r3, r6
	adds	r3, r2, r3
	str	r3, [r5, #0]
	adds	r0, r1, #0
	bl	sub_08002090
	lsls	r3, r0, #4
	subs	r3, r3, r0
	mov	r1, sl
	lsls	r3, r3, #1
	lsls	r2, r1, #16
	cmp	r3, #0
	bge.n	.L_08165352
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r3, r3, r4
.L_08165352:
	asrs	r3, r3, #16
	muls	r3, r6
	subs	r3, r2, r3
	str	r3, [r5, #4]
	b.n	.L_08165368
.L_0816535c:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r2, #28
	cmp	r1, #32
	bne.n	.L_081652e6
.L_08165368:
	add	r5, sp, #96
	movs	r3, #0
	str	r3, [r5, #0]
	str	r3, [r5, #4]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r3, [r5, #8]
	bl	sub_08014de4
	adds	r0, r5, #0
	bl	sub_08015128
	movs	r0, #128
	lsls	r0, r0, #4
	bl	sub_080150e4
	ldr	r0, [sp, #16]
	bl	sub_08015068
	ldr	r7, [pc, #216]
	movs	r2, #0
	mov	r8, r2
	add	r6, sp, #120
	add	r5, sp, #108
.L_08165398:
	ldrh	r3, [r7, #0]
	adds	r1, r5, #0
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	movs	r4, #2
	ldrsh	r3, [r7, r4]
	asrs	r2, r2, #1
	add	r3, fp
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	ldrh	r3, [r7, #4]
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	str	r2, [r6, #0]
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	lsls	r2, r2, #16
	str	r2, [r6, #8]
	adds	r0, r6, #0
	bl	sub_0815e1ec
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r0, #153
	adds	r3, r2, #0
	adds	r3, #128
	str	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	lsls	r0, r0, #6
	adds	r1, r3, #0
	adds	r1, #60
	str	r1, [r5, #4]
	ldr	r4, [sp, #60]
	adds	r2, #124
	ldr	r1, [r4, #0]
	adds	r3, #56
	adds	r1, r1, r0
	movs	r0, #8
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [pc, #116]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	adds	r7, #6
	cmp	r1, #7
	bne.n	.L_08165398
	ldr	r2, [sp, #64]
	cmp	r2, #1
	bne.n	.L_0816541c
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_08165414
	adds	r3, #3
.L_08165414:
	asrs	r3, r3, #2
	adds	r7, r3, #0
	subs	r7, #16
	b.n	.L_0816542a
.L_0816541c:
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_08165424
	adds	r2, #3
.L_08165424:
	asrs	r2, r2, #2
	movs	r3, #16
	subs	r7, r3, r2
.L_0816542a:
	movs	r3, #96
	negs	r3, r3
	ldr	r5, [pc, #64]
	add	r3, fp
	movs	r4, #0
	mov	sl, r3
	mov	r8, r4
.L_08165438:
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	add	r3, sl
	cmp	r3, #93
	bgt.n	.L_08165474
	ldr	r2, [sp, #60]
	movs	r4, #142
	ldr	r1, [r2, #0]
	movs	r0, #0
	ldrsh	r2, [r5, r0]
	lsls	r4, r4, #6
	movs	r0, #24
	adds	r2, r2, r7
	adds	r1, r1, r4
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #12
	ldr	r0, [pc, #12]
	ldr	r4, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe012
	movs	r0, r0
	.4byte 0x08198a02
	.4byte 0x02010000
	.2byte 0x8a2c
	.2byte 0x0819
.L_08165474:
	cmp	r3, #95
	bgt.n	.L_0816548c
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	add	r2, sp, #280
	adds	r0, r0, r7
	mov	r9, r2
	lsls	r0, r0, #16
	lsls	r1, r3, #16
	movs	r2, #1
	bl	sub_08164c0c
.L_0816548c:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #4
	cmp	r4, #7
	bne.n	.L_08165438
	ldr	r1, [sp, #64]
	movs	r0, #0
	lsls	r3, r1, #2
	adds	r3, r3, r1
	mov	r8, r0
	lsls	r7, r3, #14
	movs	r6, #0
.L_081654a6:
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	adds	r5, r3, r6
	ldr	r1, [r5, #24]
	cmp	r1, #0
	blt.n	.L_081654f4
	lsls	r1, r1, #10
	adds	r1, r3, r1
	movs	r3, #224
	lsls	r3, r3, #3
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #32
	subs	r3, #16
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [pc, #236]
	ldr	r4, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r0, [pc, #228]
	subs	r3, r3, r7
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r3, r3, r0
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #6
	bne.n	.L_081654f4
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_081654f4:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L_081654a6
	ldr	r4, [sp, #40]
	movs	r0, #240
	ldr	r3, [r4, #0]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r3, r3, r0
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #16]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r1, r1, r2
	str	r1, [sp, #16]
	ldr	r4, [sp, #40]
	movs	r0, #238
	ldr	r2, [r4, #0]
	lsls	r0, r0, #7
	adds	r0, #140
	movs	r3, #1
	adds	r2, r2, r0
	add	fp, r3
	ldr	r3, [r2, #0]
	mov	r1, fp
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r1, #192
	beq.n	.L_0816553e
	b.n	.L_08165134
.L_0816553e:
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #48]
	movs	r5, #0
	str	r5, [r2, #16]
	ldr	r0, [pc, #120]
	bl	sub_08014644
	ldr	r0, [pc, #116]
	bl	sub_08014644
	ldr	r0, [pc, #116]
	bl	sub_08014644
	add	r4, sp, #56
	add	r0, sp, #52
	ldrh	r4, [r4, #0]
	ldr	r3, [pc, #108]
	ldrh	r0, [r0, #0]
	strh	r4, [r3, #4]
	strh	r0, [r3, #6]
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_0814cca8
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	str	r5, [r3, #0]
	ldr	r3, [pc, #76]
	adds	r2, #12
	str	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #38
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	subs	r2, #70
	strh	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r2, [sp, #60]
	str	r3, [sp, #68]
	movs	r3, #224
	b.n	.L_081655d8
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00002784
	.4byte 0x02010000
	.4byte 0xfffb0000
	.4byte 0x0813bb39
	.4byte 0x08164bb5
	.4byte 0x08143175
	.4byte 0x03001120
	.2byte 0xf000
	.2byte 0xffff
.L_081655d8:
	ldr	r1, [r2, #0]
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r0, [pc, #896]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r4, #0
	mov	r8, r4
	movs	r7, #127
	movs	r6, #0
.L_081655f0:
	ldr	r0, [sp, #60]
	ldr	r5, [r0, #0]
	bl	sub_08014878
	adds	r5, r5, r6
	ands	r0, r7
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r7
	add	r8, r1
	adds	r0, #127
	mov	r2, r8
	str	r0, [r5, #4]
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L_081655f0
	ldr	r5, [pc, #844]
	movs	r3, #0
	mov	r8, r3
	movs	r6, #0
	movs	r7, #255
.L_0816561e:
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #127
	movs	r4, #1
	lsls	r0, r0, #12
	add	r8, r4
	str	r0, [r5, #20]
	mov	r0, r8
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r0, #128
	bne.n	.L_0816561e
	ldr	r5, [pc, #784]
	movs	r1, #0
	mov	r8, r1
	movs	r6, #0
	movs	r7, #255
.L_0816565e:
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #13
	add	r8, r2
	lsls	r3, r3, #2
	str	r0, [r5, #20]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_0816565e
	ldr	r4, [sp, #60]
	movs	r0, #239
	ldr	r2, [r4, #0]
	lsls	r0, r0, #7
	adds	r1, r2, r0
	movs	r3, #1
	str	r3, [r1, #0]
	movs	r1, #238
	ldr	r3, [pc, #708]
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r2, r1
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #700]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r3, [sp, #60]
	mov	r4, sp
	movs	r0, #232
	adds	r4, #80
	lsls	r0, r0, #9
	str	r3, [sp, #32]
	str	r4, [sp, #28]
	str	r0, [sp, #20]
	movs	r2, #0
	mov	fp, r2
.L_081656ce:
	movs	r3, #192
	mov	r1, fp
	lsls	r3, r3, #18
	subs	r1, #16
	ldr	r5, [r3, #48]
	str	r1, [sp, #36]
	cmp	r1, #19
	ble.n	.L_081656e8
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164a4c
.L_081656e8:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_081656f4
	movs	r0, #156
	bl	sub_081c0010
.L_081656f4:
	mov	r3, fp
	cmp	r3, #40
	bne.n	.L_08165700
	movs	r0, #145
	bl	sub_081c0010
.L_08165700:
	mov	r4, fp
	cmp	r4, #48
	bne.n	.L_0816573a
	ldr	r0, [sp, #76]
	cmp	r0, #1
	bne.n	.L_08165734
	ldr	r1, [sp, #32]
	movs	r2, #238
	ldr	r3, [r1, #0]
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_08020048
	ldr	r4, [sp, #32]
	movs	r0, #238
	ldr	r3, [r4, #0]
	lsls	r0, r0, #7
	adds	r0, #224
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_08020048
	bl	sub_08118118
.L_08165734:
	movs	r0, #134
	bl	sub_081180e8
.L_0816573a:
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	ldr	r7, [pc, #544]
	bl	sub_080156e8
	movs	r1, #0
	movs	r2, #63
	mov	r8, r1
	mov	sl, r2
.L_08165752:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	blt.n	.L_08165816
	add	r6, sp, #96
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #8]
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	cmp	r2, #159
	bgt.n	.L_08165774
	movs	r3, #160
	str	r3, [r6, #8]
	movs	r2, #160
.L_08165774:
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r2, r3
	ble.n	.L_08165782
	str	r3, [r6, #8]
	adds	r2, r3, #0
.L_08165782:
	adds	r3, r2, #0
	subs	r3, #160
	cmp	r3, #0
	bge.n	.L_0816578c
	adds	r3, #63
.L_0816578c:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	ldr	r2, [pc, #480]
	lsls	r5, r0, #1
	subs	r3, r5, #2
	ldrh	r4, [r2, r3]
	movs	r3, #1
	mov	r2, r8
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	ldr	r2, [sp, #60]
	lsls	r3, r3, #1
	ldr	r1, [r2, #0]
	adds	r4, r4, r3
	movs	r3, #228
	adds	r1, r1, r4
	lsls	r3, r3, #6
	ldr	r2, [r6, #0]
	adds	r1, r1, r3
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #68]
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4a69
	adds	r0, r7, #0
	movs	r1, #64
	bl	sub_08138058
	ldr	r3, [r7, #4]
	movs	r2, #160
	lsls	r2, r2, #13
	cmp	r3, r2
	bgt.n	.L_08165816
	movs	r3, #0
	str	r3, [r7, #0]
	str	r3, [r7, #8]
	str	r2, [r7, #4]
	bl	sub_08014878
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #15
	str	r0, [r7, #12]
	bl	sub_08014878
	mov	r2, sl
	ands	r0, r2
	lsls	r0, r0, #13
	str	r0, [r7, #16]
	bl	sub_08014878
	mov	r3, sl
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #15
	str	r0, [r7, #20]
.L_08165816:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	adds	r7, #28
	cmp	r0, #64
	bne.n	.L_08165752
	ldr	r2, [pc, #336]
	movs	r1, #0
	mov	r8, r1
	mov	sl, r1
	mov	r9, r2
.L_0816582c:
	ldr	r3, [sp, #60]
	mov	r0, r8
	ldr	r1, [r3, #0]
	movs	r5, #7
	ands	r5, r0
	mov	r4, sl
	adds	r7, r1, r4
	adds	r4, r5, #3
	lsls	r6, r4, #1
	subs	r3, r6, #2
	mov	r2, r9
	ldrh	r0, [r2, r3]
	movs	r3, #1
	mov	r2, r8
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	ldr	r2, [r7, #0]
	lsls	r3, r3, #1
	adds	r0, r0, r3
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	adds	r1, r1, r0
	movs	r0, #228
	lsls	r0, r0, #6
	subs	r3, r3, r4
	adds	r1, r1, r0
	str	r4, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x687b
	movs	r0, #10
	subs	r3, r3, r5
	subs	r3, #8
	negs	r0, r0
	str	r3, [r7, #4]
	cmp	r3, r0
	bge.n	.L_08165888
	movs	r3, #128
	str	r3, [r7, #4]
.L_08165888:
	movs	r2, #1
	add	r8, r2
	movs	r1, #28
	mov	r3, r8
	add	sl, r1
	cmp	r3, #64
	bne.n	.L_0816582c
	ldr	r6, [pc, #204]
	movs	r4, #0
	movs	r0, #255
	mov	r8, r4
	mov	sl, r4
	mov	r9, r0
.L_081658a2:
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002054
	ldr	r1, [sp, #36]
	cmp	r0, r1
	bge.n	.L_08165940
	ldr	r3, [r6, #4]
	cmp	r3, #0
	blt.n	.L_08165940
	add	r5, sp, #96
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	asrs	r7, r3, #1
	str	r7, [r5, #0]
	ldr	r3, [r6, #24]
	cmp	r3, #13
	bhi.n	.L_081658fe
	lsrs	r2, r3, #31
	ldr	r4, [sp, #60]
	adds	r2, r3, r2
	ldr	r3, [pc, #168]
	asrs	r2, r2, #1
	lsls	r2, r2, #1
	ldrh	r3, [r3, r2]
	ldr	r1, [r4, #0]
	movs	r0, #224
	adds	r1, r1, r3
	ldr	r3, [pc, #156]
	lsls	r0, r0, #3
	ldrh	r4, [r3, r2]
	ldr	r3, [r5, #4]
	adds	r1, r1, r0
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	subs	r2, r7, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
.L_081658fe:
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #14
	bne.n	.L_08165936
	movs	r3, #160
	lsls	r3, r3, #13
	mov	r0, sl
	str	r3, [r6, #4]
	str	r0, [r6, #0]
	bl	sub_08014878
	mov	r1, r9
	ands	r0, r1
	subs	r0, #127
	lsls	r0, r0, #16
	mov	r2, sl
	str	r0, [r6, #8]
	str	r2, [r6, #12]
	bl	sub_08014878
	mov	r3, r9
	ands	r0, r3
	mov	r4, sl
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	str	r4, [r6, #20]
	str	r4, [r6, #24]
	b.n	.L_08165940
.L_08165936:
	adds	r0, r6, #0
	movs	r1, #64
	movs	r2, #1
	bl	sub_08138058
.L_08165940:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #64
	bne.n	.L_081658a2
	ldr	r2, [sp, #64]
	cmp	r2, #1
	bne.n	.L_08165984
	mov	r4, fp
	lsrs	r3, r4, #31
	add	r3, fp
	asrs	r3, r3, #1
	adds	r1, r3, #0
	adds	r1, #24
	b.n	.L_08165990
	.4byte 0x00000184
	.4byte 0x02010000
	.4byte 0x02010e00
	.4byte 0x10101010
	.4byte 0x08143001
	.4byte 0x08197410
	.4byte 0xffffe000
	.4byte 0x08198a48
	.2byte 0x8a56
	.2byte 0x0819
.L_08165984:
	mov	r0, fp
	lsrs	r3, r0, #31
	add	r3, fp
	asrs	r3, r3, #1
	movs	r2, #56
	subs	r1, r2, r3
.L_08165990:
	mov	r3, fp
	lsls	r2, r3, #1
	mov	r4, fp
	movs	r3, #64
	subs	r0, r3, r2
	lsls	r3, r4, #8
	movs	r4, #128
	lsls	r4, r4, #10
	adds	r2, r3, r4
	ldr	r4, [sp, #24]
	movs	r3, #0
	str	r3, [r4, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	ldr	r3, [sp, #76]
	cmp	r3, #1
	bne.n	.L_08165a02
	ldr	r4, [sp, #20]
	ldr	r2, [sp, #28]
	str	r4, [sp, #80]
	str	r4, [r2, #4]
	lsls	r3, r1, #16
	movs	r4, #192
	ldr	r1, [sp, #24]
	lsls	r4, r4, #15
	adds	r3, r3, r4
	str	r3, [r1, #0]
	movs	r3, #96
	subs	r3, r3, r0
	lsls	r3, r3, #16
	str	r3, [r1, #8]
	ldr	r2, [sp, #32]
	movs	r4, #238
	ldr	r3, [r2, #0]
	lsls	r4, r4, #7
	adds	r4, #220
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	sub_08020010
	ldr	r0, [sp, #32]
	movs	r1, #238
	ldr	r3, [r0, #0]
	lsls	r1, r1, #7
	adds	r1, #224
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	sub_08020010
	b.n	.L_08165a34
.L_08165a02:
	ldr	r3, [sp, #28]
	str	r2, [sp, #80]
	str	r2, [r3, #4]
	movs	r4, #192
	lsls	r3, r1, #16
	ldr	r1, [sp, #24]
	lsls	r4, r4, #15
	adds	r3, r3, r4
	str	r3, [r1, #0]
	movs	r3, #96
	subs	r3, r3, r0
	lsls	r3, r3, #16
	str	r3, [r1, #8]
	ldr	r2, [sp, #60]
	movs	r4, #238
	ldr	r3, [r2, #0]
	lsls	r4, r4, #7
	adds	r4, #220
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	sub_08020010
.L_08165a34:
	ldr	r0, [sp, #32]
	movs	r1, #238
	ldr	r3, [r0, #0]
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r3, r3, r1
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	ldr	r4, [sp, #32]
	movs	r0, #240
	ldr	r3, [r4, #0]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r3, r3, r0
	movs	r1, #1
	str	r1, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #20]
	movs	r3, #128
	movs	r4, #1
	lsls	r3, r3, #1
	add	fp, r4
	adds	r2, r2, r3
	mov	r0, fp
	str	r2, [sp, #20]
	cmp	r0, #54
	beq.n	.L_08165a78
	b.n	.L_081656ce
.L_08165a78:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	ldr	r1, [sp, #76]
	cmp	r1, #0
	bne.n	.L_08165a9c
	ldr	r2, [sp, #60]
	movs	r4, #238
	ldr	r3, [r2, #0]
	lsls	r4, r4, #7
	adds	r4, #220
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	bl	sub_08020048
.L_08165a9c:
	bl	sub_08143bb8
	add	sp, #280
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #96
	str	r0, [sp, #60]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	movs	r6, #240
	str	r0, [sp, #56]
	movs	r0, #0
	ldr	r1, [r3, #92]
	ldr	r3, [r3, #100]
	mov	fp, r1
	str	r3, [sp, #48]
	bl	sub_081435e0
	bl	sub_0813ba50
	ldr	r2, [pc, #60]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #36]
	bl	sub_080145a8
	movs	r1, #0
	movs	r0, #1
	bl	sub_08163c2c
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, fp
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r1, #162
	movs	r2, #2
	lsls	r1, r1, #2
	b.n	.L_08165b28
	.4byte 0x00000000
	.2byte 0x3001
	.2byte 0x0814
.L_08165b28:
	movs	r0, #9
	bl	sub_08152404
	movs	r2, #13
	negs	r2, r2
	movs	r7, #0
	lsls	r6, r6, #7
	mov	r8, r2
.L_08165b38:
	movs	r0, #199
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_08020040
	mov	r3, fp
	adds	r5, r0, #0
	str	r5, [r6, r3]
	cmp	r5, #0
	beq.n	.L_08165b70
	movs	r3, #0
	strb	r3, [r5, #26]
	movs	r1, #3
	adds	r0, r7, #0
	bl	sub_08002064
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_08020030
	mov	r0, fp
	ldr	r1, [r6, r0]
	mov	r2, r8
	ldrb	r3, [r1, #9]
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_08165b70:
	adds	r7, #1
	adds	r6, #4
	cmp	r7, #6
	bne.n	.L_08165b38
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #3
	movs	r0, #188
	str	r3, [sp, #72]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r6, sp
	adds	r6, #72
	str	r6, [sp, #28]
	movs	r2, #128
	str	r3, [r6, #4]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #8
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #6
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #0
	ldr	r1, [pc, #40]
	movs	r0, #1
	bl	sub_08118040
	movs	r0, #1
	movs	r1, #1
	bl	sub_08163c2c
	ldr	r0, [pc, #28]
	ldr	r1, [sp, #48]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_08165bec
	.4byte 0x00002737
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x00000073
	.2byte 0x0134
	.2byte 0x0000
.L_08165bec:
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #80]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r0, #188
	movs	r3, #184
	movs	r1, #160
	b.n	.L_08165c48
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.2byte 0x0184
	.2byte 0x0000
.L_08165c48:
	lsls	r3, r3, #15
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	str	r0, [sp, #40]
	str	r3, [sp, #44]
	str	r1, [sp, #32]
	str	r3, [sp, #36]
	movs	r7, #0
	movs	r6, #0
	movs	r2, #0
	mov	r5, fp
.L_08165c5e:
	str	r2, [sp, #12]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #776]
	adds	r7, #1
	str	r2, [r5, #4]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #24]
	adds	r2, r2, r3
	adds	r5, #28
	cmp	r7, #6
	bne.n	.L_08165c5e
	mov	r3, fp
	movs	r7, #0
	movs	r2, #24
	adds	r3, #192
.L_08165c8a:
	adds	r7, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #58
	bne.n	.L_08165c8a
	ldr	r3, [pc, #740]
	movs	r1, #1
	movs	r2, #128
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #3
.L_08165ca0:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L_08165ca0
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #180
	add	r2, fp
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #184
	movs	r3, #0
	add	r2, fp
	str	r3, [r2, #0]
	str	r3, [sp, #52]
	ldr	r3, [pc, #696]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08165cd2
	b.n	.L_0816617c
.L_08165cd2:
	mov	r6, sp
	mov	r0, sp
	adds	r6, #80
	adds	r0, #64
	str	r6, [sp, #20]
	str	r0, [sp, #24]
.L_08165cde:
	ldr	r1, [sp, #52]
	cmp	r1, #94
	bne.n	.L_08165cea
	movs	r0, #156
	bl	sub_081c0010
.L_08165cea:
	ldr	r2, [sp, #52]
	cmp	r2, #136
	bne.n	.L_08165cf6
	movs	r0, #156
	bl	sub_081c0010
.L_08165cf6:
	ldr	r3, [sp, #52]
	cmp	r3, #178
	bne.n	.L_08165d02
	movs	r0, #156
	bl	sub_081c0010
.L_08165d02:
	ldr	r6, [sp, #52]
	movs	r0, #130
	lsls	r0, r0, #1
	cmp	r6, r0
	bne.n	.L_08165d12
	movs	r0, #145
	bl	sub_081c0010
.L_08165d12:
	ldr	r3, [pc, #624]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #64]
	str	r4, [sp, #68]
	ldr	r3, [sp, #52]
	subs	r3, #96
	cmp	r3, #155
	bls.n	.L_08165d2e
	ldr	r1, [sp, #52]
	ldr	r2, [pc, #608]
	adds	r3, r1, r2
	cmp	r3, #3
	bhi.n	.L_08165d3a
.L_08165d2e:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
.L_08165d3a:
	movs	r3, #0
	movs	r6, #238
	lsls	r6, r6, #7
	str	r3, [sp, #92]
	str	r3, [sp, #84]
	ldr	r5, [sp, #20]
	ldr	r4, [pc, #580]
	adds	r6, #220
	movs	r7, #0
	add	r6, fp
.L_08165d4e:
	ldr	r3, [pc, #576]
	ldr	r0, [sp, #40]
	ldrb	r3, [r3, r7]
	ldr	r1, [sp, #44]
	lsls	r3, r3, #16
	adds	r3, r3, r0
	adds	r3, r3, r4
	str	r3, [r5, #0]
	ldr	r3, [pc, #564]
	ldmia	r6!, {r0}
	ldrb	r3, [r3, r7]
	str	r4, [sp, #8]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r3, r3, r4
	str	r3, [r5, #8]
	adds	r1, r5, #0
	ldr	r2, [sp, #24]
	movs	r3, #0
	bl	sub_08020010
	adds	r7, #1
	ldr	r4, [sp, #8]
	cmp	r7, #7
	bne.n	.L_08165d4e
	ldr	r2, [sp, #52]
	cmp	r2, #90
	bgt.n	.L_08165da8
	lsls	r5, r2, #9
	adds	r0, r5, #0
	bl	sub_08002096
	movs	r3, #156
	lsls	r0, r0, #4
	lsls	r3, r3, #16
	adds	r3, r0, r3
	adds	r0, r5, #0
	str	r3, [sp, #32]
	bl	sub_08002090
	movs	r6, #184
	lsls	r0, r0, #4
	lsls	r6, r6, #15
	adds	r6, r0, r6
	str	r6, [sp, #36]
.L_08165da8:
	ldr	r0, [sp, #52]
	cmp	r0, #196
	bgt.n	.L_08165e3e
	movs	r7, #0
	movs	r6, #91
	mov	r8, fp
.L_08165db4:
	ldr	r1, [sp, #52]
	cmp	r1, r6
	blt.n	.L_08165dca
	adds	r3, r6, #4
	cmp	r1, r3
	bge.n	.L_08165dca
	ldr	r2, [sp, #36]
	movs	r3, #128
	lsls	r3, r3, #12
	adds	r3, r2, r3
	str	r3, [sp, #36]
.L_08165dca:
	ldr	r0, [sp, #52]
	adds	r3, r6, #3
	cmp	r0, r3
	bne.n	.L_08165e1a
	movs	r2, #255
	mov	r5, r8
	movs	r1, #0
	mov	sl, r2
	adds	r5, #168
.L_08165ddc:
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	str	r1, [sp, #16]
	bl	sub_08014878
	mov	r3, sl
	ands	r0, r3
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08014878
	mov	r2, sl
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08014878
	ldr	r1, [sp, #16]
	movs	r3, #15
	ands	r3, r0
	adds	r1, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #4
	bne.n	.L_08165ddc
.L_08165e1a:
	ldr	r0, [sp, #52]
	adds	r3, r6, #0
	adds	r3, #20
	cmp	r0, r3
	blt.n	.L_08165e32
	adds	r3, #16
	cmp	r0, r3
	bge.n	.L_08165e32
	ldr	r1, [sp, #36]
	ldr	r2, [pc, #360]
	adds	r2, r1, r2
	str	r2, [sp, #36]
.L_08165e32:
	movs	r3, #224
	adds	r7, #1
	adds	r6, #40
	add	r8, r3
	cmp	r7, #3
	bne.n	.L_08165db4
.L_08165e3e:
	ldr	r3, [sp, #52]
	subs	r3, #244
	cmp	r3, #7
	bhi.n	.L_08165e4e
	ldr	r6, [sp, #32]
	ldr	r0, [pc, #336]
	adds	r0, r6, r0
	str	r0, [sp, #32]
.L_08165e4e:
	ldr	r3, [sp, #52]
	subs	r3, #252
	cmp	r3, #23
	bhi.n	.L_08165e62
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #32]
	subs	r3, #250
	lsls	r3, r3, #16
	subs	r3, r1, r3
	str	r3, [sp, #32]
.L_08165e62:
	ldr	r2, [sp, #52]
	movs	r3, #4
	adds	r3, #255
	cmp	r2, r3
	bgt.n	.L_08165eb2
	ldr	r0, [sp, #36]
	movs	r3, #255
	lsls	r3, r3, #24
	str	r3, [sp, #84]
	adds	r3, r0, r3
	str	r3, [sp, #88]
	movs	r3, #238
	lsls	r3, r3, #7
	ldr	r6, [sp, #32]
	adds	r3, #248
	add	r2, sp, #80
	add	r3, fp
	ldr	r0, [r3, #0]
	adds	r1, r2, #0
	movs	r3, #0
	ldr	r2, [sp, #24]
	str	r6, [sp, #80]
	bl	sub_08020010
	ldr	r6, [sp, #32]
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r3, r6, r0
	str	r3, [sp, #80]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #252
	add	r3, fp
	add	r2, sp, #80
	ldr	r0, [r3, #0]
	adds	r1, r2, #0
	movs	r3, #0
	ldr	r2, [sp, #24]
	bl	sub_08020010
.L_08165eb2:
	ldr	r6, [sp, #20]
	movs	r3, #0
	str	r3, [r6, #4]
	movs	r7, #0
	mov	r9, r6
	mov	r5, fp
	mov	sl, fp
.L_08165ec0:
	ldr	r3, [r5, #24]
	cmp	r3, #2
	beq.n	.L_08165fae
	ldr	r3, [r5, #0]
	mov	r0, r9
	str	r3, [r0, #0]
	movs	r1, #240
	ldr	r3, [r5, #4]
	lsls	r1, r1, #7
	str	r3, [r0, #8]
	lsls	r3, r7, #2
	adds	r3, r3, r1
	mov	r2, fp
	ldr	r0, [r2, r3]
	mov	r1, r9
	ldr	r2, [sp, #24]
	movs	r3, #0
	bl	sub_08020010
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [sp, #52]
	cmp	r3, #96
	ble.n	.L_08165f04
	movs	r6, #128
	lsls	r6, r6, #7
	adds	r3, r2, r6
	str	r3, [r5, #16]
.L_08165f04:
	ldr	r3, [r5, #4]
	movs	r0, #240
	lsls	r0, r0, #15
	cmp	r3, r0
	ble.n	.L_08165fae
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #1
	bne.n	.L_08165fa0
	ldr	r3, [r5, #16]
	movs	r6, #210
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	movs	r2, #255
	lsls	r6, r6, #2
	movs	r1, #0
	mov	r8, r2
	add	r6, sl
.L_08165f30:
	ldr	r3, [r5, #0]
	ldr	r0, [pc, #88]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	str	r1, [sp, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r0
	str	r3, [r6, #4]
	bl	sub_08014878
	mov	r2, r8
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r6, #12]
	bl	sub_08014878
	mov	r3, r8
	ands	r0, r3
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r6, #16]
	bl	sub_08014878
	ldr	r1, [sp, #16]
	movs	r3, #15
	ands	r3, r0
	adds	r1, #1
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r1, #2
	bne.n	.L_08165f30
	b.n	.L_08165fae
	movs	r0, r0
	.4byte 0xfff00000
	.4byte 0x02010018
	.4byte 0x03001150
	.4byte 0x08196e6c
	.4byte 0xfffffefc
	.4byte 0xffe00000
	.4byte 0x08198a64
	.4byte 0x08198a6d
	.4byte 0xfffe0000
	.2byte 0x0000
	.2byte 0xffff
.L_08165fa0:
	.2byte 0x9e0d
	cmp	r6, #199
	bgt.n	.L_08165fae
	movs	r3, #0
	str	r3, [r5, #4]
	str	r3, [r5, #16]
	str	r3, [r5, #24]
.L_08165fae:
	movs	r0, #56
	adds	r7, #1
	adds	r5, #28
	add	sl, r0
	cmp	r7, #6
	beq.n	.L_08165fbc
	b.n	.L_08165ec0
.L_08165fbc:
	mov	r5, fp
	movs	r7, #0
	adds	r5, #168
.L_08165fc2:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08166010
	cmp	r0, #23
	bhi.n	sub_08166000
	movs	r1, #6
	bl	sub_08002054
	ldr	r3, [pc, #492]
	adds	r0, #3
	lsls	r0, r0, #1
	ldrh	r1, [r3, r0]
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, fp
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #476]
	ldrh	r4, [r3, r0]
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	lsrs	r0, r4, #1
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #56]
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #448]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08166010:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #56
	bne.n	.L_08165fc2
	ldr	r0, [sp, #52]
	movs	r1, #130
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_081660dc
	ldr	r2, [sp, #60]
	movs	r7, #0
	ldr	r3, [r2, #20]
	cmp	r3, #0
	beq.n	.L_08166058
	movs	r5, #36
.L_0816602e:
	ldr	r3, [sp, #60]
	movs	r1, #4
	ldrsh	r0, [r5, r3]
	bl	sub_08118088
	ldr	r1, [sp, #60]
	movs	r3, #8
	ldrsh	r0, [r5, r1]
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	negs	r2, r2
	bl	sub_0814cd48
	ldr	r6, [sp, #60]
	adds	r7, #1
	ldr	r3, [r6, #20]
	adds	r5, #2
	cmp	r7, r3
	bne.n	.L_0816602e
.L_08166058:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r0, [sp, #52]
	movs	r1, #130
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_081660dc
	ldr	r2, [pc, #348]
	movs	r7, #0
.L_08166072:
	str	r2, [sp, #12]
	bl	sub_08014878
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	ldr	r2, [sp, #12]
	adds	r6, r0, #0
	adds	r3, #255
	ands	r6, r3
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, #0]
	movs	r3, #184
	lsls	r3, r3, #15
	str	r3, [r2, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [sp, #12]
	asrs	r3, r3, #7
	str	r3, [r2, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [sp, #12]
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r2, #16]
	bl	sub_08014878
	movs	r3, #15
	ldr	r2, [sp, #12]
	ands	r3, r0
	adds	r3, #32
	str	r3, [r2, #24]
	movs	r3, #128
	adds	r7, #1
	lsls	r3, r3, #2
	adds	r2, #28
	cmp	r7, r3
	bne.n	.L_08166072
.L_081660dc:
	ldr	r0, [pc, #240]
	ldr	r6, [pc, #236]
	movs	r7, #0
	mov	r8, r0
.L_081660e4:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	blt.n	.L_08166136
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r5, r0, #1
	subs	r3, r5, #2
	mov	r2, r8
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #48]
	movs	r4, #1
	adds	r1, r3, r1
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	mov	lr, r2
	movs	r3, #6
	ldrsh	r2, [r6, r3]
	str	r0, [sp, #0]
	subs	r3, r2, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #28]
	ands	r4, r7
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	mov	r2, lr
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r6, #0
	movs	r1, #62
	lsls	r2, r2, #5
	bl	sub_08138086
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_08166136:
	movs	r1, #128
	adds	r7, #1
	lsls	r1, r1, #2
	adds	r6, #28
	cmp	r7, r1
	bne.n	.L_081660e4
	movs	r0, #8
	movs	r1, #8
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
	ldr	r2, [sp, #52]
	movs	r3, #160
	adds	r2, #1
	lsls	r3, r3, #1
	str	r2, [sp, #52]
	cmp	r2, r3
	beq.n	.L_0816617c
	ldr	r3, [pc, #100]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0816617c
	b.n	.L_08165cde
.L_0816617c:
	movs	r0, #134
	bl	sub_081180e8
	bl	sub_0814cca8
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #220
	movs	r7, #0
	add	r5, fp
.L_08166190:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	sub_08020048
	cmp	r7, #15
	bne.n	.L_08166190
	ldr	r0, [pc, #56]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08198a76
	.4byte 0x08198a84
	.4byte 0xffffc000
	.4byte 0x02010000
	.4byte 0x08197410
	.4byte 0x03001150
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r9, r0
	ldr	r0, [r3, #92]
	sub	sp, #88
	str	r0, [sp, #40]
	movs	r0, #0
	ldr	r1, [r3, #96]
	ldr	r5, [pc, #896]
	str	r1, [sp, #36]
	movs	r7, #255
	ldr	r2, [r3, #100]
	str	r2, [sp, #28]
	ldr	r3, [r3, #48]
	str	r3, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [sp, #40]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r3, r2
	ldr	r0, [pc, #872]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	adds	r1, r5, #0
	ldr	r0, [pc, #864]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #28]
	ldr	r0, [pc, #856]
	movs	r3, #0
	bl	sub_08157cf4
	mov	r3, r9
	movs	r2, #36
	ldrsh	r1, [r3, r2]
	ldr	r0, [r3, #8]
	movs	r2, #4
	movs	r3, #0
	bl	sub_08118078
	movs	r0, #1
	bl	sub_08013560
	mov	r1, r9
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r6, [r0, #0]
	ldr	r5, [sp, #40]
	movs	r2, #0
	mov	sl, r2
.L_0816625e:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08014878
	ldr	r3, [r5, #0]
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_08166298
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_08166298:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_081662a0
	adds	r3, #3
.L_081662a0:
	asrs	r3, r3, #2
	lsls	r3, r3, #1
	adds	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_0816625e
	mov	r3, sp
	adds	r3, #64
	mov	r2, r9
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r3, #0
	str	r3, [sp, #20]
	bl	sub_0815e20c
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #704]
	bl	sub_080145a8
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
	movs	r3, #75
	str	r3, [r2, #0]
	movs	r0, #0
	str	r0, [sp, #32]
	ldr	r2, [sp, #24]
	ldr	r0, [pc, #672]
	ldr	r3, [sp, #40]
	mov	r1, sp
	adds	r1, #76
	adds	r2, #12
	adds	r0, r3, r0
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r0, [sp, #8]
.L_08166300:
	ldr	r1, [sp, #32]
	cmp	r1, #8
	bne.n	.L_0816630c
	movs	r0, #134
	bl	sub_081180e8
.L_0816630c:
	mov	r2, r9
	ldr	r3, [r2, #24]
	cmp	r3, #0
	beq.n	.L_08166320
	ldr	r3, [sp, #32]
	cmp	r3, #8
	bne.n	.L_08166320
	movs	r0, #212
	bl	sub_081c0010
.L_08166320:
	mov	r1, r9
	ldr	r0, [r1, #8]
	ldr	r1, [sp, #16]
	bl	sub_0815e20c
	ldr	r3, [sp, #32]
	subs	r3, #6
	cmp	r3, #5
	bhi.n	.L_0816639e
	mov	r2, r9
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08166344
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	b.n	.L_0816634c
.L_08166344:
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
.L_0816634c:
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r0, r9
	ldr	r4, [r3, #104]
	ldr	r3, [r0, #4]
	str	r4, [sp, #44]
	cmp	r3, #0
	bne.n	.L_0816637c
	ldr	r2, [sp, #76]
	movs	r1, #48
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [sp, #80]
	asrs	r2, r2, #1
	str	r1, [sp, #0]
	movs	r1, #72
	str	r1, [sp, #4]
	subs	r2, #24
	subs	r3, #24
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #8]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00d
.L_0816637c:
	ldr	r2, [sp, #76]
	movs	r1, #48
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [sp, #80]
	str	r1, [sp, #0]
	movs	r1, #72
	str	r1, [sp, #4]
	asrs	r2, r2, #1
	subs	r3, #24
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #8]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
.L_0816639e:
	ldr	r2, [sp, #32]
	subs	r2, #16
	cmp	r2, #31
	bhi.n	.L_08166412
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	movs	r0, #104
	movs	r1, #19
	asrs	r5, r3, #1
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	str	r3, [sp, #44]
	cmp	r5, #2
	ble.n	.L_081663c2
	movs	r5, #2
.L_081663c2:
	mov	r1, r9
	ldr	r3, [r1, #24]
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L_081663d0
	movs	r2, #150
	lsls	r2, r2, #6
.L_081663d0:
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #3
	adds	r1, r1, r5
	lsls	r1, r1, #7
	ldr	r3, [sp, #20]
	adds	r1, r2, r1
	ldr	r2, [pc, #412]
	ldr	r0, [sp, #20]
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r4, [sp, #44]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r0, #40
	str	r0, [sp, #0]
	asrs	r2, r2, #1
	movs	r0, #80
	str	r0, [sp, #4]
	subs	r2, #20
	subs	r3, #48
	ldr	r0, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x209c
	lsls	r0, r0, #6
	adds	r0, #16
	bl	sub_0814c97c
	movs	r0, #104
	bl	sub_0801314c
.L_08166412:
	ldr	r1, [sp, #32]
	cmp	r1, #8
	bne.n	sub_08166426
	movs	r1, #128
	ldr	r3, [pc, #376]
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #372]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf6ae
	.2byte 0xfcdd
	.2byte 0x9806
	ldr	r1, [sp, #12]
	bl	sub_080156e8
	ldr	r2, [sp, #32]
	cmp	r2, #3
	ble.n	.L_081664d4
	mov	r3, r9
	movs	r1, #44
	ldr	r0, [r3, #4]
	add	r1, sp
	mov	fp, r1
	bl	sub_08144aac
	movs	r3, #52
	ldr	r7, [sp, #40]
	movs	r2, #0
	add	r3, sp
	mov	sl, r2
	mov	r8, r3
.L_08166452:
	ldr	r5, [r7, #24]
	cmp	r5, #0
	ble.n	.L_081664bc
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_0815e1ec
	mov	r0, r8
	ldr	r2, [r0, #0]
	ldr	r3, [r0, #4]
	asrs	r2, r2, #1
	str	r2, [r0, #0]
	ldr	r0, [sp, #20]
	asrs	r5, r5, #3
	ldr	r1, [r0, #4]
	mov	r0, sl
	adds	r3, r3, r1
	lsrs	r4, r0, #31
	adds	r5, #2
	ldr	r0, [pc, #288]
	subs	r3, #112
	mov	r1, r8
	add	r4, sl
	str	r3, [r1, #4]
	lsls	r6, r5, #1
	movs	r1, #1
	asrs	r4, r4, #1
	ands	r4, r1
	subs	r1, r6, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #28]
	str	r5, [sp, #0]
	adds	r1, r0, r1
	lsrs	r0, r5, #31
	str	r6, [sp, #4]
	adds	r0, r5, r0
	asrs	r0, r0, #1
	subs	r2, r2, r0
	lsls	r4, r4, #2
	mov	r0, fp
	subs	r3, r3, r5
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #60
	ldr	r2, [pc, #236]
	bl	sub_08138058
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_081664bc:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L_08166452
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
.L_081664d4:
	ldr	r3, [sp, #32]
	cmp	r3, #8
	bne.n	.L_081664f4
	mov	r2, r9
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #4
	bl	sub_08118088
	movs	r0, #238
	ldr	r3, [sp, #40]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r3, r0
	movs	r3, #4
	str	r3, [r2, #0]
.L_081664f4:
	ldr	r1, [sp, #32]
	cmp	r1, #6
	bne.n	.L_08166510
	mov	r3, r9
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #10
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_08166510:
	ldr	r0, [sp, #32]
	cmp	r0, #14
	bne.n	.L_0816652c
	mov	r2, r9
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #10
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_0816652c:
	movs	r1, #16
	movs	r0, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r0, #240
	ldr	r3, [sp, #40]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r3, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #32]
	movs	r2, #216
	lsls	r2, r2, #4
	adds	r1, r1, r2
	adds	r3, #1
	str	r1, [sp, #8]
	str	r3, [sp, #32]
	cmp	r3, #64
	beq.n	.L_08166562
	b.n	.L_08166300
.L_08166562:
	ldr	r0, [pc, #40]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x00000159
	.4byte 0x00000125
	.4byte 0x00000134
	.4byte 0x08143001
	.4byte 0xffffb600
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x08197410
	.2byte 0xfc00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	fp, r0
	ldr	r0, [r3, #92]
	sub	sp, #76
	str	r0, [sp, #52]
	mov	r2, fp
	ldr	r1, [r3, #96]
	str	r1, [sp, #48]
	ldr	r3, [r3, #100]
	str	r3, [sp, #40]
	ldr	r0, [r2, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	str	r0, [sp, #36]
	movs	r0, #1
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	mov	r5, sp
	strh	r3, [r2, #0]
	adds	r5, #56
	mov	r3, fp
	ldr	r0, [r3, #4]
	adds	r1, r5, #0
	str	r5, [sp, #28]
	bl	sub_08144aac
	movs	r1, #2
	ldr	r0, [sp, #36]
	bl	sub_08020090
	movs	r1, #48
	ldr	r0, [sp, #36]
	bl	sub_08020098
	mov	r1, fp
	add	r5, sp, #64
	movs	r6, #36
	ldrsh	r0, [r1, r6]
	adds	r1, r5, #0
	bl	sub_0815e21c
	mov	r2, fp
	ldr	r3, [r2, #4]
	b.n	.L_0816661c
	movs	r0, r0
	.2byte 0x1010
	.2byte 0x0000
.L_0816661c:
	cmp	r3, #0
	bne.n	.L_0816662c
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r3, #24
	b.n	.L_08166636
.L_0816662c:
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r3, #92
.L_08166636:
	str	r3, [sp, #32]
	ldr	r3, [sp, #52]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r3, r5
	ldr	r0, [pc, #856]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r6, [sp, #52]
	movs	r2, #156
	lsls	r2, r2, #6
	adds	r1, r6, r2
	ldr	r0, [pc, #844]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [pc, #836]
	ldr	r1, [sp, #40]
	movs	r2, #0
	bl	sub_08157cf4
	movs	r3, #0
	str	r3, [sp, #44]
	str	r3, [sp, #8]
	mov	r8, r3
.L_08166670:
	ldr	r6, [sp, #8]
	ldr	r0, [sp, #52]
	movs	r5, #0
	mov	sl, r5
	adds	r7, r6, r0
.L_0816667a:
	mov	r1, sl
	lsls	r6, r1, #1
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
	mov	r2, sl
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r3, r3, #1
	adds	r3, #25
	str	r3, [r7, #24]
	movs	r3, #1
	add	sl, r3
	mov	r5, sl
	adds	r7, #28
	cmp	r5, #16
	bne.n	.L_0816667a
	mov	r1, r8
	ldr	r0, [pc, #740]
	lsls	r3, r1, #3
	ldr	r2, [pc, #740]
	subs	r3, r3, r1
	movs	r6, #0
	lsls	r3, r3, #2
	mov	sl, r6
	mov	r9, r0
	adds	r7, r3, r2
.L_081666d2:
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r6, r0, #0
	adds	r3, #255
	ands	r6, r3
	mov	r3, fp
	ldr	r2, [r3, #4]
	ldr	r0, [sp, #44]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	mov	r1, r9
	adds	r3, r0, r3
	ldr	r2, [sp, #32]
	ldrb	r3, [r1, r3]
	adds	r0, r6, #0
	adds	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #176
	lsls	r3, r3, #15
	str	r3, [r7, #4]
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
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r5, #170
	movs	r3, #1
	add	sl, r3
	lsls	r5, r5, #1
	adds	r7, #28
	cmp	sl, r5
	bne.n	.L_081666d2
	ldr	r6, [sp, #8]
	ldr	r1, [sp, #44]
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r6, r6, r0
	adds	r1, #1
	add	r8, r5
	str	r6, [sp, #8]
	str	r1, [sp, #44]
	cmp	r1, #3
	bne.n	.L_08166670
	ldr	r3, [sp, #52]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r6, [sp, #52]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r6, r0
	movs	r3, #75
	movs	r1, #200
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #564]
	bl	sub_080145a8
	movs	r1, #0
	mov	r9, r1
.L_08166784:
	mov	r2, r9
	cmp	r2, #4
	bne.n	.L_08166790
	movs	r0, #212
	bl	sub_081c0010
.L_08166790:
	mov	r3, r9
	cmp	r3, #8
	bne.n	.L_081667a4
	ldr	r5, [sp, #52]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	adds	r3, r5, r6
	mov	r0, r9
	str	r0, [r3, #0]
.L_081667a4:
	mov	r1, r9
	cmp	r1, #18
	bne.n	.L_081667b0
	movs	r0, #145
	bl	sub_081c0010
.L_081667b0:
	mov	r2, r9
	cmp	r2, #40
	bne.n	.L_081667bc
	movs	r0, #134
	bl	sub_081180e8
.L_081667bc:
	mov	r3, r9
	cmp	r3, #39
	bgt.n	sub_08166858
	mov	r5, fp
	ldr	r3, [r5, #4]
	movs	r1, #128
	cmp	r3, #1
	bne.n	.L_081667fa
	mov	r6, r9
	cmp	r6, #9
	bgt.n	.L_081667e4
	lsls	r3, r6, #2
	add	r3, r9
	lsls	r3, r3, #1
	adds	r2, r3, #0
	lsls	r3, r6, #4
	adds	r5, r3, #0
	subs	r2, #8
	subs	r5, #128
	b.n	.L_08166828
.L_081667e4:
	mov	r0, r9
	cmp	r0, #20
	ble.n	.L_081667f6
	lsls	r3, r0, #1
	mov	r2, r9
	adds	r5, r3, #0
	adds	r2, #62
	subs	r5, #24
	b.n	.L_08166828
.L_081667f6:
	movs	r2, #82
	b.n	.L_08166826
.L_081667fa:
	mov	r2, r9
	cmp	r2, #9
	bgt.n	.L_08166812
	lsls	r3, r2, #2
	add	r3, r9
	lsls	r3, r3, #1
	mov	r5, r9
	subs	r2, r1, r3
	lsls	r3, r5, #4
	adds	r5, r3, #0
	subs	r5, #128
	b.n	.L_08166828
.L_08166812:
	mov	r6, r9
	cmp	r6, #20
	ble.n	.L_08166824
	movs	r3, #58
	subs	r2, r3, r6
	lsls	r3, r6, #1
	adds	r5, r3, #0
	subs	r5, #24
	b.n	.L_08166828
.L_08166824:
	movs	r2, #38
.L_08166826:
	movs	r5, #16
.L_08166828:
	adds	r3, r5, #0
	adds	r3, #128
	cmp	r3, #104
	ble.n	.L_08166836
	subs	r3, r1, r5
	adds	r1, r3, #0
	subs	r1, #24
.L_08166836:
	cmp	r1, #0
	ble.n	sub_08166858
	ldr	r0, [sp, #32]
	movs	r3, #64
	str	r3, [sp, #0]
	ldr	r3, [sp, #52]
	movs	r6, #224
	adds	r2, r2, r0
	lsls	r6, r6, #3
	str	r1, [sp, #4]
	subs	r2, #32
	adds	r1, r3, r6
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4648
	cmp	r0, #16
	ble.n	.L_08166864
	ldr	r0, [pc, #340]
	bl	sub_0815f0a0
.L_08166864:
	movs	r1, #0
	movs	r2, #22
	movs	r3, #16
	str	r1, [sp, #44]
	str	r2, [sp, #24]
	str	r1, [sp, #20]
	str	r3, [sp, #16]
	str	r1, [sp, #12]
.L_08166874:
	ldr	r5, [sp, #44]
	ldr	r6, [sp, #16]
	lsls	r1, r5, #3
	cmp	r9, r6
	bne.n	.L_0816688c
	ldr	r0, [sp, #52]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r0, r3
	movs	r3, #12
	str	r3, [r2, #0]
.L_0816688c:
	ldr	r5, [sp, #16]
	cmp	r9, r5
	blt.n	.L_0816694c
	adds	r3, r1, #0
	adds	r3, #18
	cmp	r9, r3
	bge.n	sub_081668ca
	mov	r6, fp
	ldr	r2, [r6, #4]
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #260]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, r0, r3
	ldrb	r2, [r1, r3]
	movs	r3, #32
	ldr	r1, [sp, #32]
	str	r3, [sp, #0]
	movs	r3, #64
	str	r3, [sp, #4]
	ldr	r3, [sp, #52]
	movs	r5, #156
	adds	r2, r2, r1
	lsls	r5, r5, #6
	adds	r1, r3, r5
	subs	r2, #16
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	movs	r3, #56
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9903
	ldr	r0, [pc, #232]
	lsls	r3, r1, #3
	ldr	r2, [sp, #52]
	subs	r3, r3, r1
	movs	r6, #0
	lsls	r3, r3, #2
	mov	sl, r6
	mov	r8, r0
	adds	r5, r3, r2
.L_081668de:
	mov	r0, fp
	ldr	r2, [r0, #4]
	movs	r3, #6
	ldrsh	r7, [r5, r3]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #44]
	movs	r6, #2
	ldrsh	r1, [r5, r6]
	ldr	r6, [pc, #180]
	adds	r3, r2, r3
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #32]
	adds	r1, r1, r3
	adds	r6, r1, r0
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_08166932
	movs	r1, #3
	bl	sub_08002054
	mov	r2, r8
	ldrb	r1, [r2, r0]
	ldr	r3, [sp, #52]
	lsls	r1, r1, #11
	movs	r0, #156
	adds	r1, r3, r1
	lsls	r0, r0, #6
	adds	r1, r1, r0
	movs	r0, #32
	str	r0, [sp, #0]
	adds	r2, r6, #0
	movs	r0, #64
	adds	r3, r7, #0
	str	r0, [sp, #4]
	subs	r2, #16
	adds	r3, #56
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69a8
.L_08166932:
	cmp	r0, #0
	ble.n	.L_0816693a
	subs	r3, r0, #1
	b.n	.L_0816693e
.L_0816693a:
	movs	r3, #1
	negs	r3, r3
.L_0816693e:
	str	r3, [r5, #24]
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #28
	cmp	r2, #12
	bne.n	.L_081668de
.L_0816694c:
	ldr	r3, [sp, #16]
	adds	r3, #5
	cmp	r9, r3
	ble.n	.L_08166a16
	ldr	r7, [sp, #20]
	movs	r3, #0
	mov	sl, r3
.L_0816695a:
	lsls	r3, r7, #4
	adds	r3, r7, r3
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r2, r3, #3
	ldr	r5, [pc, #68]
	subs	r2, r2, r3
	lsls	r2, r2, #2
	adds	r6, r2, r5
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	sub_08166a0a
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #64
	lsls	r2, r2, #5
	bl	sub_08138086
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #4]
	movs	r0, #216
	subs	r3, #1
	lsls	r0, r0, #15
	str	r3, [r6, #24]
	cmp	r1, r0
	ble.n	.L_081669bc
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	sub_08166a0a
	.4byte 0x00000117
	.4byte 0x0000013e
	.4byte 0x00000134
	.4byte 0x08198a92
	.4byte 0x02010000
	.4byte 0x08143001
	.4byte 0x00000184
	.2byte 0x8a98
	.2byte 0x0819
.L_081669bc:
	ldr	r0, [r6, #0]
	ldr	r2, [pc, #300]
	cmp	r0, r2
	bhi.n	sub_08166a0a
	cmp	r1, #0
	blt.n	sub_08166a0a
	asrs	r1, r1, #16
	mov	r8, r1
	asrs	r6, r0, #16
	movs	r1, #5
	adds	r0, r3, #0
	bl	sub_08002054
	ldr	r2, [pc, #280]
	adds	r0, #1
	lsls	r5, r0, #1
	mov	r3, sl
	movs	r4, #1
	ands	r4, r3
	subs	r3, r5, #2
	ldrh	r1, [r2, r3]
	lsrs	r3, r0, #31
	ldr	r2, [sp, #40]
	adds	r3, r0, r3
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r5, [sp, #28]
	asrs	r3, r3, #1
	subs	r6, r6, r3
	lsls	r4, r4, #2
	mov	r3, r8
	subs	r3, r3, r0
	adds	r1, r2, r1
	ldr	r4, [r4, r5]
	ldr	r0, [sp, #48]
	adds	r2, r6, #0
	mov	r8, r3
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2601
	movs	r0, #128
	add	sl, r6
	lsls	r0, r0, #1
	cmp	sl, r0
	bne.n	.L_0816695a
.L_08166a16:
	mov	r2, fp
	ldr	r3, [r2, #20]
	movs	r1, #0
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L_08166a6e
	ldr	r3, [sp, #24]
	mov	r5, fp
	mov	r8, r3
	movs	r6, #10
	adds	r5, #36
.L_08166a2c:
	ldr	r0, [sp, #16]
	cmp	r9, r0
	bne.n	.L_08166a42
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	movs	r2, #5
	movs	r1, #7
	mov	r3, sl
	str	r6, [sp, #0]
	bl	sub_0814cd48
.L_08166a42:
	cmp	r9, r8
	bne.n	.L_08166a60
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	movs	r1, #7
	mov	r3, sl
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_0814cd48
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	movs	r1, #4
	bl	sub_08118088
.L_08166a60:
	mov	r1, fp
	ldr	r3, [r1, #20]
	movs	r0, #1
	add	sl, r0
	adds	r5, #2
	cmp	sl, r3
	bne.n	.L_08166a2c
.L_08166a6e:
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #20]
	ldr	r5, [sp, #16]
	ldr	r6, [sp, #12]
	ldr	r0, [sp, #44]
	adds	r2, #8
	adds	r3, #5
	adds	r5, #8
	adds	r6, #16
	adds	r0, #1
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	str	r5, [sp, #16]
	str	r6, [sp, #12]
	str	r0, [sp, #44]
	cmp	r0, #2
	beq.n	.L_08166a92
	b.n	.L_08166874
.L_08166a92:
	movs	r0, #16
	movs	r1, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	ldr	r1, [sp, #52]
	lsls	r2, r2, #7
	adds	r2, #232
	adds	r3, r1, r2
	movs	r5, #1
	str	r5, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	r9, r3
	mov	r5, r9
	cmp	r5, #80
	beq.n	.L_08166abe
	b.n	.L_08166784
.L_08166abe:
	ldr	r0, [sp, #36]
	movs	r1, #16
	bl	sub_08020098
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x007effff
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_08166b10
	pop	{pc}
