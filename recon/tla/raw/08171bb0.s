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
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_080156e8, 0x080156e8
	.set sub_08016ca4, 0x08016ca4
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_08171e1c, 0x08171e1c
	.set sub_08172010, 0x08172010
	.set sub_081722ea, 0x081722ea
	.set sub_08172b00, 0x08172b00
	.set sub_08178680, 0x08178680
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969ac, 0x081969ac
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08171bb0
	.thumb_func
Func_08171bb0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #176
	str	r0, [sp, #92]
	str	r1, [sp, #88]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	ldr	r4, [sp, #92]
	str	r0, [sp, #84]
	ldr	r1, [r3, #96]
	str	r1, [sp, #80]
	ldr	r2, [r3, #48]
	str	r2, [sp, #64]
	ldr	r3, [r3, #100]
	str	r3, [sp, #60]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08016ca4
	str	r0, [sp, #52]
	movs	r0, #0
	bl	sub_081435e0
	ldr	r5, [sp, #88]
	cmp	r5, #1
	bhi.n	.L_08171c1c
	movs	r0, #38
	str	r0, [sp, #56]
	ldr	r2, [sp, #92]
	movs	r3, #163
	ldr	r1, [r2, #4]
	add	r7, sp, #164
	lsls	r1, r1, #4
	orrs	r1, r3
	add	r2, sp, #152
	adds	r3, r7, #0
	ldr	r0, [sp, #92]
	bl	sub_0815585c
	movs	r2, #128
	ldr	r3, [pc, #8]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	b.n	.L_08171cec
	movs	r0, r0
	.2byte 0x0785
	.2byte 0x0000
.L_08171c1c:
	ldr	r3, [sp, #88]
	cmp	r3, #2
	beq.n	.L_08171c26
	cmp	r3, #4
	bne.n	.L_08171cde
.L_08171c26:
	ldr	r4, [sp, #92]
	ldr	r0, [r4, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r5, [sp, #88]
	mov	r8, r0
	cmp	r5, #2
	bne.n	.L_08171cb0
	ldr	r2, [pc, #200]
	movs	r5, #128
	lsls	r5, r5, #5
	adds	r1, r5, r2
	ldr	r0, [pc, #196]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [pc, #192]
	adds	r1, r5, #0
	ldr	r0, [pc, #180]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b2b
	movs	r6, #0
	movs	r7, #0
	movs	r5, #0
	mov	ip, r3
.L_08171c5e:
	ldr	r1, [pc, #176]
	mov	r0, ip
	adds	r3, r7, r6
	adds	r2, r5, r0
	adds	r0, r5, r1
	lsls	r1, r3, #3
	movs	r3, #128
	lsls	r3, r3, #5
	add	r1, ip
	adds	r1, r1, r3
	movs	r3, #156
	lsls	r3, r3, #1
	adds	r3, #255
	movs	r4, #0
	adds	r2, r2, r3
.L_08171c7c:
	ldrb	r3, [r1, #0]
	adds	r4, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r3, [r1, #0]
	adds	r1, #1
	strb	r3, [r2, #0]
	subs	r2, #1
	cmp	r4, #24
	bne.n	.L_08171c7c
	adds	r6, #1
	adds	r7, #2
	adds	r5, #64
	cmp	r6, #48
	bne.n	.L_08171c5e
	ldr	r0, [pc, #120]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #112]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe005
.L_08171cb0:
	ldr	r0, [pc, #104]
	ldr	r1, [pc, #80]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	mov	r4, r8
	ldr	r3, [r4, #8]
	add	r2, sp, #140
	str	r3, [r2, #0]
	ldr	r0, [pc, #88]
	ldr	r3, [r4, #12]
	ldr	r1, [pc, #88]
	str	r3, [r2, #4]
	movs	r5, #52
	ldr	r3, [r4, #16]
	str	r3, [r2, #8]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	str	r5, [sp, #56]
	b.n	.L_08171cec
.L_08171cde:
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #19
	adds	r2, #12
	movs	r0, #52
	strh	r3, [r2, #0]
	str	r0, [sp, #56]
.L_08171cec:
	ldr	r1, [sp, #92]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08171d28
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_08171d30
	movs	r0, r0
	.4byte 0x00000785
	.4byte 0x02014000
	.4byte 0x0000013a
	.4byte 0x03000258
	.4byte 0x02014208
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x000000b4
	.4byte 0x0000013e
	.2byte 0x5000
	.2byte 0x0201
.L_08171d28:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_08171d30:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #128
	str	r3, [sp, #72]
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [sp, #84]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r5, [sp, #84]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r5, r0
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	ldr	r2, [sp, #92]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r3, #192
	str	r0, [sp, #48]
	b.n	.L_08171d80
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_08171d80:
	lsls	r3, r3, #3
	adds	r3, #228
	adds	r2, r5, r3
	ldr	r3, [r0, #8]
	movs	r4, #221
	str	r3, [r2, #0]
	lsls	r4, r4, #3
	ldr	r3, [r0, #12]
	adds	r2, r5, r4
	str	r3, [r2, #0]
	ldr	r1, [sp, #48]
	movs	r0, #192
	ldr	r3, [r1, #16]
	lsls	r0, r0, #3
	adds	r0, #236
	adds	r2, r5, r0
	str	r3, [r2, #0]
	movs	r3, #222
	lsls	r3, r3, #3
	adds	r2, r5, r3
	ldr	r3, [r1, #24]
	adds	r4, #12
	str	r3, [r2, #0]
	adds	r2, r5, r4
	ldr	r3, [r1, #28]
	str	r3, [r2, #0]
	ldr	r5, [sp, #88]
	cmp	r5, #2
	beq.n	.L_08171dbe
	cmp	r5, #4
	bne.n	.L_08171dd2
.L_08171dbe:
	ldr	r2, [sp, #84]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #836]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_08171de4
.L_08171dd2:
	ldr	r4, [sp, #84]
	movs	r5, #224
	lsls	r5, r5, #3
	ldr	r0, [pc, #820]
	adds	r1, r4, r5
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_08171de4:
	ldr	r1, [sp, #60]
	ldr	r0, [pc, #812]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #84]
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #796]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #88]
	cmp	r4, #3
	bne.n	sub_08171e1c
	ldr	r0, [pc, #784]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #780]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d16
	cmp	r5, #4
	beq.n	.L_08171e26
	cmp	r5, #2
	bne.n	.L_08171e40
.L_08171e26:
	ldr	r0, [pc, #764]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #748]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9816
	cmp	r0, #2
	beq.n	.L_08171e48
.L_08171e40:
	ldr	r2, [sp, #88]
	movs	r1, #4
	cmp	r2, #4
	bne.n	.L_08171e4a
.L_08171e48:
	movs	r1, #8
.L_08171e4a:
	movs	r3, #32
	ldr	r0, [pc, #728]
	movs	r2, #32
	bl	sub_08178680
	ldr	r4, [sp, #56]
	movs	r3, #0
	str	r3, [sp, #68]
	cmp	r4, #0
	bne.n	.L_08171e62
	bl	.L_08172a5c
.L_08171e62:
	ldr	r5, [sp, #64]
	mov	r0, sp
	adds	r5, #12
	adds	r0, #96
	str	r5, [sp, #36]
	str	r0, [sp, #40]
	str	r3, [sp, #20]
.L_08171e70:
	ldr	r1, [sp, #68]
	cmp	r1, #0
	bne.n	.L_08171e7c
	movs	r0, #199
	bl	sub_081c0010
.L_08171e7c:
	ldr	r2, [sp, #68]
	cmp	r2, #32
	bne.n	.L_08171e88
	movs	r0, #206
	bl	sub_081c0010
.L_08171e88:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L_08171f78
	ldr	r4, [sp, #84]
	movs	r5, #192
	lsls	r5, r5, #3
	adds	r5, #228
	adds	r3, r4, r5
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #48]
	movs	r1, #221
	str	r3, [r0, #8]
	lsls	r1, r1, #3
	adds	r3, r4, r1
	ldr	r3, [r3, #0]
	movs	r2, #192
	str	r3, [r0, #12]
	lsls	r2, r2, #3
	adds	r2, #236
	adds	r3, r4, r2
	ldr	r3, [r3, #0]
	adds	r5, #12
	str	r3, [r0, #16]
	adds	r3, r4, r5
	ldr	r3, [r3, #0]
	str	r3, [r0, #24]
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #244
	adds	r3, r4, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #48]
	str	r3, [r1, #28]
	ldr	r2, [sp, #88]
	cmp	r2, #0
	bne.n	.L_08171f1c
	ldr	r6, [sp, #84]
	movs	r3, #0
	mov	r8, r3
	movs	r7, #127
.L_08171ed8:
	ldr	r4, [sp, #92]
	adds	r5, r6, #0
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08171ee8
	movs	r3, #200
	lsls	r3, r3, #16
	b.n	.L_08171eea
.L_08171ee8:
	ldr	r3, [pc, #576]
.L_08171eea:
	str	r3, [r6, #0]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ldr	r3, [r5, #0]
	ands	r0, r7
	str	r3, [r5, #12]
	ldr	r3, [r5, #4]
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #8]
	str	r0, [r5, #20]
	str	r3, [r5, #16]
	movs	r5, #1
	add	r8, r5
	mov	r0, r8
	adds	r6, #28
	cmp	r0, #63
	bne.n	.L_08171ed8
	b.n	.L_08171f78
.L_08171f1c:
	ldr	r1, [sp, #88]
	cmp	r1, #1
	bne.n	.L_08171f78
	ldr	r5, [sp, #84]
	movs	r2, #0
	mov	r8, r2
	add	r7, sp, #164
	movs	r6, #0
.L_08171f2c:
	ldr	r3, [r7, #0]
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	movs	r4, #128
	lsls	r4, r4, #15
	lsls	r3, r3, #16
	adds	r3, r3, r4
	str	r3, [r5, #4]
	str	r6, [r5, #8]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #64
	lsls	r2, r3, #13
	ldr	r3, [r5, #0]
	str	r2, [r5, #12]
	cmp	r3, #0
	ble.n	.L_08171f5c
	negs	r3, r2
	str	r3, [r5, #12]
.L_08171f5c:
	bl	sub_08014878
	movs	r3, #128
	negs	r3, r3
	orrs	r3, r0
	movs	r0, #1
	add	r8, r0
	lsls	r3, r3, #11
	mov	r1, r8
	str	r3, [r5, #16]
	str	r6, [r5, #20]
	adds	r5, #28
	cmp	r1, #63
	bne.n	.L_08171f2c
.L_08171f78:
	bl	sub_08014de4
	ldr	r0, [sp, #64]
	ldr	r1, [sp, #36]
	bl	sub_080156e8
	ldr	r2, [sp, #88]
	cmp	r2, #1
	bhi.n	sub_08172010
	ldr	r3, [sp, #68]
	movs	r5, #4
	lsls	r3, r3, #11
	mov	r9, r3
	mov	r0, r9
	bl	sub_08002096
	add	r7, sp, #164
	ldr	r3, [r7, #0]
	lsls	r0, r0, #4
	add	r6, sp, #140
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldr	r4, [sp, #68]
	cmp	r4, #39
	ble.n	.L_08171fae
	adds	r5, r4, #0
	subs	r5, #36
.L_08171fae:
	ldr	r1, [sp, #68]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #232
	adds	r0, r1, #0
	muls	r0, r3
	bl	sub_08002090
	ldr	r3, [r7, #4]
	lsls	r2, r5, #16
	adds	r3, r3, r2
	adds	r2, r5, #0
	muls	r2, r0
	subs	r2, r3, r2
	str	r2, [r6, #4]
	ldr	r3, [sp, #68]
	cmp	r3, #27
	ble.n	.L_08171fdc
	ldr	r4, [pc, #348]
	lsls	r3, r3, #18
	adds	r3, r2, r3
	adds	r3, r3, r4
	str	r3, [r6, #4]
.L_08171fdc:
	ldr	r3, [r7, #8]
	add	r5, sp, #152
	adds	r1, r5, #0
	str	r3, [r6, #8]
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r2, [r5, #0]
	movs	r1, #20
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	ldr	r5, [sp, #84]
	movs	r4, #156
	asrs	r2, r2, #1
	str	r1, [sp, #0]
	lsls	r4, r4, #6
	movs	r1, #40
	str	r1, [sp, #4]
	subs	r2, #10
	adds	r1, r5, r4
	subs	r3, #20
	ldr	r0, [sp, #80]
	ldr	r5, [sp, #72]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9811
	cmp	r0, #7
	bgt.n	.L_08172018
	b.n	.L_0817223e
.L_08172018:
	ldr	r1, [sp, #88]
	cmp	r1, #1
	bhi.n	.L_08172030
	cmp	r0, #47
	bgt.n	.L_08172030
	ldr	r3, [sp, #84]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #3
	str	r3, [r2, #0]
.L_08172030:
	ldr	r5, [sp, #88]
	cmp	r5, #0
	beq.n	.L_08172038
	b.n	.L_081721e8
.L_08172038:
	ldr	r0, [sp, #68]
	cmp	r0, #32
	bne.n	.L_0817209e
	ldr	r1, [sp, #92]
	ldr	r3, [r1, #24]
	cmp	r3, #1
	bne.n	.L_08172050
	ldr	r2, [sp, #48]
	movs	r3, #248
	lsls	r3, r3, #15
	str	r3, [r2, #12]
	b.n	.L_0817223e
.L_08172050:
	ldr	r3, [sp, #84]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081180e8
	ldr	r5, [sp, #84]
	movs	r0, #222
	lsls	r0, r0, #3
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #48]
	movs	r2, #192
	str	r3, [r1, #24]
	lsls	r2, r2, #3
	adds	r2, #244
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	movs	r2, #128
	str	r3, [r1, #28]
	ldr	r4, [sp, #92]
	movs	r1, #1
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [sp, #0]
	movs	r3, #150
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r2, r2, #10
	lsls	r3, r3, #12
	bl	sub_0815f000
	b.n	.L_0817223e
.L_0817209e:
	ldr	r5, [sp, #68]
	cmp	r5, #19
	bgt.n	.L_08172134
	ldr	r1, [sp, #48]
	ldr	r0, [sp, #88]
	movs	r4, #192
	str	r0, [r1, #72]
	ldr	r2, [sp, #84]
	lsls	r4, r4, #3
	adds	r4, #228
	adds	r3, r2, r4
	add	r1, sp, #140
	ldr	r2, [r1, #0]
	ldr	r3, [r3, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081720c2
	adds	r2, #127
.L_081720c2:
	ldr	r5, [sp, #48]
	asrs	r2, r2, #7
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r0, [sp, #84]
	movs	r2, #221
	lsls	r2, r2, #3
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r1, #4]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081720e0
	adds	r2, #127
.L_081720e0:
	ldr	r4, [sp, #48]
	asrs	r2, r2, #7
	ldr	r3, [r4, #12]
	movs	r0, #192
	adds	r3, r3, r2
	str	r3, [r4, #12]
	ldr	r5, [sp, #84]
	lsls	r0, r0, #3
	adds	r0, #236
	adds	r3, r5, r0
	ldr	r2, [r1, #8]
	ldr	r3, [r3, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_08172100
	adds	r2, #127
.L_08172100:
	ldr	r1, [sp, #48]
	asrs	r2, r2, #7
	ldr	r3, [r1, #16]
	adds	r3, r3, r2
	str	r3, [r1, #16]
	b.n	.L_0817223e
	.4byte 0x000000d2
	.4byte 0x000000d3
	.4byte 0x00000134
	.4byte 0x00000153
	.4byte 0x00000150
	.4byte 0x03000730
	.4byte 0x0000012f
	.4byte 0x02012000
	.4byte 0xff380000
	.2byte 0x0000
	.2byte 0xff90
.L_08172134:
	.2byte 0x9a11
	cmp	r2, #31
	ble.n	.L_0817213c
	b.n	.L_0817223e
.L_0817213c:
	ldr	r4, [sp, #92]
	ldr	r3, [r4, #24]
	cmp	r3, #1
	beq.n	.L_08172148
	cmp	r2, #23
	bgt.n	.L_0817223e
.L_08172148:
	ldr	r5, [sp, #84]
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #228
	add	r1, sp, #140
	adds	r3, r5, r0
	ldr	r2, [r1, #0]
	ldr	r3, [r3, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_08172160
	adds	r2, #15
.L_08172160:
	ldr	r4, [sp, #48]
	asrs	r2, r2, #4
	ldr	r3, [r4, #8]
	movs	r0, #221
	adds	r3, r3, r2
	str	r3, [r4, #8]
	ldr	r5, [sp, #84]
	lsls	r0, r0, #3
	adds	r3, r5, r0
	ldr	r2, [r1, #4]
	ldr	r3, [r3, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_0817217e
	adds	r2, #15
.L_0817217e:
	ldr	r4, [sp, #48]
	asrs	r2, r2, #4
	ldr	r3, [r4, #12]
	movs	r0, #192
	adds	r3, r3, r2
	str	r3, [r4, #12]
	ldr	r5, [sp, #84]
	lsls	r0, r0, #3
	adds	r0, #236
	adds	r3, r5, r0
	ldr	r2, [r1, #8]
	ldr	r3, [r3, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_0817219e
	adds	r2, #15
.L_0817219e:
	ldr	r1, [sp, #48]
	asrs	r2, r2, #4
	ldr	r3, [r1, #16]
	movs	r4, #222
	adds	r3, r3, r2
	str	r3, [r1, #16]
	ldr	r2, [sp, #84]
	lsls	r4, r4, #3
	adds	r3, r2, r4
	ldr	r3, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #7
	subs	r2, r1, r3
	cmp	r2, #0
	bge.n	.L_081721be
	adds	r2, #15
.L_081721be:
	ldr	r5, [sp, #48]
	asrs	r2, r2, #4
	ldr	r3, [r5, #24]
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r0, [sp, #84]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #244
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	subs	r2, r1, r3
	cmp	r2, #0
	bge.n	.L_081721dc
	adds	r2, #15
.L_081721dc:
	ldr	r4, [sp, #48]
	asrs	r2, r2, #4
	ldr	r3, [r4, #28]
	adds	r3, r3, r2
	str	r3, [r4, #28]
	b.n	.L_0817223e
.L_081721e8:
	ldr	r5, [sp, #88]
	cmp	r5, #1
	bne.n	.L_0817223e
	ldr	r1, [sp, #48]
	movs	r0, #0
	str	r0, [r1, #72]
	ldr	r2, [sp, #68]
	cmp	r2, #55
	ble.n	.L_08172202
	movs	r3, #144
	lsls	r3, r3, #16
	str	r3, [r1, #12]
	b.n	.L_0817223e
.L_08172202:
	ldr	r3, [sp, #68]
	cmp	r3, #31
	bgt.n	.L_08172224
	ldr	r4, [sp, #48]
	ldr	r3, [r4, #8]
	cmp	r3, #0
	ble.n	.L_0817221a
	movs	r5, #128
	lsls	r5, r5, #8
	adds	r3, r3, r5
	str	r3, [r4, #8]
	b.n	.L_0817223e
.L_0817221a:
	ldr	r0, [pc, #696]
	ldr	r1, [sp, #48]
	adds	r3, r3, r0
	str	r3, [r1, #8]
	b.n	.L_0817223e
.L_08172224:
	ldr	r2, [sp, #48]
	ldr	r3, [r2, #8]
	cmp	r3, #0
	ble.n	.L_08172236
	movs	r4, #128
	lsls	r4, r4, #13
	adds	r3, r3, r4
	str	r3, [r2, #8]
	b.n	.L_0817223e
.L_08172236:
	ldr	r5, [pc, #672]
	ldr	r0, [sp, #48]
	adds	r3, r3, r5
	str	r3, [r0, #8]
.L_0817223e:
	ldr	r1, [sp, #68]
	cmp	r1, #0
	ble.n	.L_081722f6
	ldr	r2, [sp, #88]
	cmp	r2, #1
	bhi.n	.L_081722f6
	ldr	r5, [sp, #84]
	movs	r3, #0
	mov	r8, r3
	add	r7, sp, #128
.L_08172252:
	ldr	r4, [sp, #68]
	cmp	r4, r8
	blt.n	sub_081722ea
	mov	r3, r8
	adds	r3, #30
	cmp	r4, r3
	bge.n	sub_081722ea
	ldr	r1, [sp, #88]
	movs	r3, #3
	mov	r0, r8
	ands	r3, r0
	adds	r6, r3, #1
	cmp	r1, #0
	bne.n	.L_081722ae
	add	r1, sp, #140
	ldr	r2, [r1, #0]
	ldr	r3, [r5, #12]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_0817227c
	adds	r2, #31
.L_0817227c:
	ldr	r3, [r5, #0]
	asrs	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r5, #16]
	ldr	r2, [r1, #4]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_08172290
	adds	r2, #31
.L_08172290:
	ldr	r3, [r5, #4]
	asrs	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #20]
	ldr	r2, [r1, #8]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081722a4
	adds	r2, #31
.L_081722a4:
	ldr	r3, [r5, #8]
	asrs	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r5, #8]
	b.n	.L_081722ba
.L_081722ae:
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #3
	bl	sub_08138058
.L_081722ba:
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_0815e1ec
	ldr	r2, [pc, #536]
	lsls	r0, r6, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #60]
	ldr	r4, [sp, #72]
	adds	r1, r2, r1
	ldr	r2, [r7, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	lsrs	r3, r6, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r0, [sp, #4]
	subs	r3, r3, r6
	str	r6, [sp, #0]
	ldr	r0, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_08172252
.L_081722f6:
	ldr	r2, [sp, #88]
	cmp	r2, #2
	beq.n	.L_08172300
	cmp	r2, #4
	bne.n	.L_0817236a
.L_08172300:
	ldr	r3, [sp, #68]
	movs	r1, #0
	lsls	r3, r3, #11
	movs	r4, #64
	mov	r9, r3
	mov	fp, r4
	mov	sl, r1
.L_0817230e:
	adds	r7, r1, #0
	cmp	r1, #28
	ble.n	.L_08172316
	movs	r7, #28
.L_08172316:
	movs	r5, #0
	mov	r8, r5
	ldr	r5, [pc, #452]
	lsls	r4, r1, #10
	mov	r2, fp
	movs	r6, #0
	add	r5, sl
.L_08172324:
	mov	r3, r9
	strb	r2, [r5, #0]
	subs	r0, r4, r3
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, r3, r6
	subs	r3, #24
	strb	r3, [r5, #1]
	movs	r3, #1
	movs	r0, #0
	add	r8, r3
	strb	r0, [r5, #2]
	mov	r0, r8
	adds	r6, #48
	adds	r5, #4
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #8]
	cmp	r0, #2
	bne.n	.L_08172324
	movs	r2, #4
	negs	r2, r2
	movs	r3, #8
	adds	r1, #1
	add	fp, r2
	add	sl, r3
	cmp	r1, #33
	bne.n	.L_0817230e
	b.n	.L_08172462
.L_0817236a:
	ldr	r4, [sp, #88]
	cmp	r4, #3
	bne.n	.L_081723e4
	ldr	r5, [sp, #68]
	movs	r1, #0
	str	r1, [sp, #28]
	lsls	r5, r5, #11
	mov	r9, r5
	mov	fp, r1
.L_0817237c:
	mov	r3, fp
	lsls	r6, r1, #2
	adds	r7, r3, #4
	cmp	r6, #64
	ble.n	.L_08172388
	movs	r6, #64
.L_08172388:
	movs	r0, #0
	ldr	r2, [sp, #28]
	mov	r8, r0
	ldr	r0, [pc, #336]
	lsls	r3, r3, #2
	mov	sl, r2
	lsls	r4, r1, #10
	movs	r2, #0
	adds	r5, r3, r0
.L_0817239a:
	mov	r3, sl
	strb	r3, [r5, #0]
	mov	r3, r9
	subs	r0, r4, r3
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r2, [sp, #12]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	subs	r3, r3, r7
	strb	r3, [r5, #1]
	lsls	r3, r7, #1
	adds	r2, r2, r3
	movs	r3, #1
	movs	r0, #0
	add	r8, r3
	strb	r0, [r5, #2]
	mov	r0, r8
	adds	r5, #4
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #8]
	cmp	r0, #2
	bne.n	.L_0817239a
	ldr	r2, [sp, #28]
	movs	r3, #2
	subs	r2, #3
	adds	r1, #1
	str	r2, [sp, #28]
	add	fp, r3
	cmp	r1, #33
	bne.n	.L_0817237c
	b.n	.L_08172462
.L_081723e4:
	ldr	r4, [sp, #68]
	movs	r1, #0
	mov	sl, r4
	movs	r4, #0
	str	r1, [sp, #32]
	str	r4, [sp, #24]
.L_081723f0:
	adds	r7, r4, r1
	adds	r6, r4, #0
	cmp	r7, #96
	ble.n	.L_081723fa
	movs	r7, #96
.L_081723fa:
	cmp	r4, #40
	ble.n	.L_08172400
	movs	r6, #40
.L_08172400:
	ldr	r0, [sp, #32]
	ldr	r3, [sp, #24]
	mov	fp, r0
	ldr	r0, [pc, #216]
	movs	r5, #0
	mov	r8, r5
	mov	r9, sl
	movs	r2, #0
	adds	r5, r3, r0
.L_08172412:
	mov	r3, fp
	strb	r3, [r5, #0]
	mov	r3, r9
	lsls	r0, r3, #10
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r2, [sp, #12]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	subs	r3, r3, r6
	strb	r3, [r5, #1]
	lsls	r3, r6, #1
	adds	r2, r2, r3
	movs	r3, #1
	movs	r0, #0
	add	r8, r3
	strb	r0, [r5, #2]
	mov	r0, r8
	adds	r5, #4
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #8]
	cmp	r0, #2
	bne.n	.L_08172412
	ldr	r3, [sp, #32]
	ldr	r5, [sp, #24]
	movs	r2, #2
	subs	r3, #4
	adds	r5, #8
	adds	r1, #1
	add	sl, r2
	str	r3, [sp, #32]
	str	r5, [sp, #24]
	adds	r4, #2
	cmp	r1, #33
	bne.n	.L_081723f0
.L_08172462:
	movs	r0, #1
	bl	sub_081969f8
	mov	fp, r0
	ldr	r0, [sp, #88]
	cmp	r0, #4
	bne.n	.L_08172494
	ldr	r1, [sp, #68]
	cmp	r1, #45
	bgt.n	.L_0817247e
	lsls	r3, r1, #2
	subs	r3, #64
	mov	sl, r3
	b.n	.L_08172488
.L_0817247e:
	ldr	r2, [sp, #68]
	ldr	r4, [pc, #96]
	lsls	r3, r2, #3
	adds	r4, r4, r3
	mov	sl, r4
.L_08172488:
	movs	r5, #8
	negs	r5, r5
	cmp	sl, r5
	ble.n	.L_081724b6
	mov	sl, r5
	b.n	.L_081724b6
.L_08172494:
	ldr	r0, [sp, #68]
	cmp	r0, #45
	bgt.n	.L_081724a0
	lsls	r3, r0, #2
	subs	r3, #64
	b.n	.L_081724aa
.L_081724a0:
	ldr	r1, [sp, #68]
	movs	r3, #174
	lsls	r2, r1, #3
	lsls	r3, r3, #1
	subs	r3, r3, r2
.L_081724aa:
	mov	sl, r3
	movs	r2, #20
	negs	r2, r2
	cmp	sl, r2
	ble.n	.L_081724b6
	mov	sl, r2
.L_081724b6:
	movs	r3, #64
	negs	r3, r3
	cmp	sl, r3
	bge.n	.L_081724c0
	mov	sl, r3
.L_081724c0:
	ldr	r4, [sp, #88]
	cmp	r4, #2
	beq.n	.L_081724ca
	cmp	r4, #4
	bne.n	.L_081724ec
.L_081724ca:
	ldr	r3, [pc, #28]
	ldr	r2, [sp, #96]
	ands	r2, r3
	movs	r3, #5
	b.n	.L_081724f4
	.4byte 0xffff8000
	.4byte 0xfff00000
	.4byte 0x08197410
	.4byte 0x02010000
	.4byte 0xfffffe88
	.2byte 0xff00
	.2byte 0xffff
.L_081724ec:
	ldr	r3, [pc, #712]
	ldr	r2, [sp, #96]
	ands	r2, r3
	movs	r3, #7
.L_081724f4:
	orrs	r2, r3
	str	r2, [sp, #96]
	ldr	r3, [sp, #96]
	ldr	r2, [pc, #704]
	ldr	r5, [sp, #84]
	ands	r3, r2
	movs	r2, #160
	ldr	r1, [sp, #40]
	lsls	r2, r2, #3
	movs	r0, #224
	orrs	r3, r2
	lsls	r0, r0, #3
	str	r3, [sp, #96]
	adds	r3, r5, r0
	mov	r2, fp
	str	r3, [r1, #4]
	movs	r3, #7
	str	r3, [r2, #0]
	ldr	r4, [pc, #676]
	ldr	r3, [pc, #680]
	mov	r5, sl
	str	r1, [r2, #16]
	str	r3, [r2, #8]
	str	r4, [r2, #12]
	str	r5, [r2, #20]
	ldr	r0, [sp, #88]
	cmp	r0, #2
	beq.n	.L_08172532
	cmp	r0, #4
	beq.n	.L_08172532
	b.n	.L_081727e8
.L_08172532:
	ldr	r2, [sp, #92]
	mov	r3, sp
	adds	r3, #116
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r3, #0
	str	r3, [sp, #44]
	bl	sub_0815e21c
	ldr	r4, [sp, #68]
	cmp	r4, #16
	bne.n	.L_0817258a
	movs	r0, #144
	bl	sub_081180e8
	ldr	r1, [sp, #92]
	movs	r3, #224
	lsls	r3, r3, #11
	movs	r5, #36
	ldrsh	r0, [r1, r5]
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	bl	sub_0815f000
	movs	r4, #238
	ldr	r2, [sp, #84]
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r3, r2, r4
	movs	r2, #8
	str	r2, [r3, #0]
	ldr	r1, [sp, #92]
	movs	r3, #0
	movs	r5, #36
	ldrsh	r0, [r1, r5]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	bl	sub_0814cd48
.L_0817258a:
	ldr	r2, [sp, #68]
	cmp	r2, #0
	bne.n	.L_081725fc
	ldr	r4, [sp, #44]
	ldr	r7, [sp, #84]
	movs	r3, #0
	mov	r8, r3
	mov	r9, r4
.L_0817259a:
	bl	sub_08014878
	movs	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r6, r6, r5
	movs	r5, #254
	lsls	r5, r5, #7
	adds	r5, #255
	ands	r5, r0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r5, r5, r0
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	mov	r1, r9
	ldr	r3, [r1, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r2, #0
	ldr	r3, [r1, #4]
	str	r2, [r7, #24]
	subs	r3, #8
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #28
	cmp	r4, #63
	bne.n	.L_0817259a
.L_081725fc:
	ldr	r5, [sp, #68]
	cmp	r5, #15
	ble.n	.L_08172654
	ldr	r5, [sp, #84]
	movs	r0, #0
	mov	r8, r0
.L_08172608:
	ldr	r1, [r5, #24]
	cmp	r1, #23
	bgt.n	.L_08172648
	cmp	r1, #0
	bge.n	.L_08172614
	adds	r1, #3
.L_08172614:
	ldr	r2, [pc, #432]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #396]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08172648:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #8
	bne.n	.L_08172608
.L_08172654:
	ldr	r3, [sp, #68]
	mov	r4, fp
	lsls	r2, r3, #3
	movs	r3, #127
	bics	r3, r2
	movs	r5, #0
	strb	r3, [r4, #24]
	strb	r5, [r4, #25]
	ldr	r1, [sp, #68]
	ldr	r6, [sp, #20]
	movs	r0, #0
	lsls	r1, r1, #11
	mov	r8, r0
	mov	r9, r1
.L_08172670:
	lsls	r0, r6, #1
	bl	sub_08002096
	lsls	r0, r0, #6
	asrs	r0, r0, #16
	movs	r5, #64
	subs	r5, r5, r0
	bl	sub_08014e38
	mov	r2, r8
	lsls	r3, r2, #3
	subs	r5, r5, r3
	movs	r3, #192
	lsls	r3, r3, #14
	lsls	r5, r5, #16
	adds	r5, r5, r3
	movs	r0, #0
	adds	r1, r5, #0
	movs	r2, #0
	bl	sub_08015160
	ldr	r4, [sp, #92]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_081726aa
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_081726aa:
	mov	r5, r8
	movs	r1, #128
	lsls	r1, r1, #1
	lsls	r0, r5, #10
	adds	r0, r0, r1
	bl	sub_080150e4
	mov	r2, r9
	lsls	r0, r5, #13
	subs	r0, r0, r2
	bl	sub_08015024
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r1, r2, #0
	ldr	r0, [pc, #260]
	bl	sub_080151e4
	ldr	r1, [pc, #240]
	movs	r2, #66
	ldr	r0, [pc, #256]
	bl	sub_08196958
	mov	r0, fp
	bl	sub_08196a7c
	bl	sub_08014ea8
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #3
	bne.n	.L_08172670
	ldr	r5, [sp, #40]
	movs	r3, #6
	add	r2, sp, #96
	strb	r3, [r5, #0]
	str	r2, [sp, #40]
	strb	r3, [r2, #1]
	ldr	r3, [pc, #220]
	ldr	r1, [pc, #196]
	mov	r0, fp
	str	r3, [r0, #8]
	mov	r2, sl
	movs	r3, #0
	str	r1, [r0, #12]
	str	r2, [r0, #20]
	strb	r3, [r0, #24]
	strb	r3, [r0, #25]
	ldr	r5, [sp, #40]
	ldr	r4, [pc, #204]
	movs	r0, #0
	str	r4, [r5, #4]
	ldr	r5, [sp, #44]
	movs	r7, #128
	mov	r8, r0
	lsls	r7, r7, #8
.L_0817271c:
	ldr	r2, [sp, #68]
	mov	r1, r8
	lsls	r3, r1, #2
	subs	r3, r2, r3
	adds	r2, r3, #0
	subs	r2, #16
	cmp	r2, #0
	blt.n	.L_081727aa
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r6, r3, #12
	movs	r3, #0
	mov	sl, r3
	cmp	r2, #7
	ble.n	.L_08172742
	movs	r3, #8
	subs	r3, r3, r2
	lsls	r3, r3, #3
	mov	sl, r3
.L_08172742:
	movs	r1, #64
	mov	r4, sl
	mov	r0, fp
	negs	r1, r1
	str	r4, [r0, #20]
	cmp	sl, r1
	ble.n	.L_081727aa
	bl	sub_08014de4
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #64
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r0, r7, #0
	adds	r2, r7, #0
	bl	sub_080151e4
	mov	r2, r8
	movs	r3, #128
	lsls	r0, r2, #14
	lsls	r3, r3, #6
	adds	r0, r0, r3
	bl	sub_080150e4
	ldr	r0, [pc, #88]
	bl	sub_08015024
	ldr	r4, [sp, #68]
	lsls	r0, r4, #12
	bl	sub_08015068
	adds	r0, r6, #0
	bl	sub_0801521c
	ldr	r0, [pc, #72]
	ldr	r1, [pc, #32]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, fp
	bl	sub_08196a7c
.L_081727aa:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #2
	bne.n	.L_0817271c
	b.n	.L_08172a08
	movs	r0, r0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02011000
	.4byte 0x02012000
	.4byte 0x02015000
	.4byte 0xffffe000
	.4byte 0x00021770
	.4byte 0x02010000
	.4byte 0x08199340
	.4byte 0x02014000
	.4byte 0xfffff000
	.2byte 0x9210
	.2byte 0x0819
.L_081727e8:
	ldr	r2, [sp, #88]
	cmp	r2, #3
	beq.n	.L_081727f0
	b.n	.L_0817290c
.L_081727f0:
	ldr	r3, [sp, #92]
	add	r4, sp, #104
	mov	sl, r4
	ldr	r0, [r3, #8]
	mov	r1, sl
	bl	sub_0815e20c
	ldr	r5, [sp, #68]
	cmp	r5, #8
	bne.n	.L_0817280a
	movs	r0, #144
	bl	sub_081c0010
.L_0817280a:
	ldr	r0, [sp, #68]
	cmp	r0, #16
	bne.n	.L_08172816
	movs	r0, #134
	bl	sub_081180e8
.L_08172816:
	ldr	r2, [sp, #92]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_08172878
	movs	r7, #8
	movs	r5, #36
	movs	r6, #16
.L_08172828:
	ldr	r4, [sp, #68]
	cmp	r4, r6
	bne.n	.L_0817286c
	movs	r0, #134
	bl	sub_081c0010
	ldr	r1, [sp, #92]
	movs	r3, #224
	lsls	r3, r3, #11
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	bl	sub_0815f000
	ldr	r3, [sp, #92]
	movs	r1, #7
	ldrsh	r0, [r5, r3]
	movs	r2, #5
	mov	r3, r8
	str	r7, [sp, #0]
	bl	sub_0814cd48
	movs	r1, #238
	ldr	r0, [sp, #84]
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r3, r0, r1
	str	r7, [r3, #0]
	ldr	r2, [sp, #92]
	ldr	r3, [r2, #20]
.L_0817286c:
	movs	r4, #1
	add	r8, r4
	adds	r5, #2
	adds	r6, #4
	cmp	r8, r3
	bne.n	.L_08172828
.L_08172878:
	movs	r5, #0
	mov	r0, fp
	strb	r5, [r0, #25]
	ldr	r2, [sp, #68]
	movs	r1, #0
	mov	r8, r1
	mov	r7, sl
	lsls	r6, r2, #11
	lsls	r5, r2, #3
.L_0817288a:
	movs	r3, #127
	mov	r4, fp
	bics	r3, r5
	strb	r3, [r4, #24]
	bl	sub_08014de4
	ldr	r0, [r7, #0]
	ldr	r1, [r7, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r2, [pc, #564]
	asrs	r0, r0, #1
	subs	r0, #64
	lsls	r1, r1, #16
	adds	r1, r1, r2
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	ldr	r4, [sp, #92]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_081728c0
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_081728c0:
	mov	r1, r8
	lsls	r0, r1, #15
	bl	sub_08015024
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r0, r0, #11
	asrs	r0, r0, #16
	bl	sub_080150e4
	movs	r0, #167
	lsls	r0, r0, #9
	movs	r1, #128
	movs	r2, #128
	adds	r0, #32
	lsls	r1, r1, #8
	lsls	r2, r2, #9
	bl	sub_080151e4
	movs	r2, #66
	ldr	r0, [pc, #492]
	ldr	r1, [pc, #492]
	bl	sub_08196958
	mov	r0, fp
	bl	sub_08196a7c
	movs	r3, #1
	movs	r2, #128
	add	r8, r3
	lsls	r2, r2, #7
	mov	r4, r8
	adds	r6, r6, r2
	adds	r5, #48
	cmp	r4, #1
	bne.n	.L_0817288a
	b.n	.L_08172a08
.L_0817290c:
	ldr	r5, [sp, #68]
	cmp	r5, #29
	bgt.n	.L_08172918
	lsls	r3, r5, #2
	subs	r3, #64
	b.n	.L_08172920
.L_08172918:
	ldr	r0, [sp, #68]
	movs	r3, #220
	lsls	r2, r0, #3
	subs	r3, r3, r2
.L_08172920:
	mov	sl, r3
	movs	r1, #20
	negs	r1, r1
	cmp	sl, r1
	ble.n	.L_0817292c
	mov	sl, r1
.L_0817292c:
	movs	r2, #64
	negs	r2, r2
	cmp	sl, r2
	ble.n	.L_08172a02
	movs	r2, #128
	mov	r3, sl
	lsls	r2, r2, #9
	mov	r4, fp
	movs	r0, #128
	str	r3, [r4, #20]
	adds	r1, r2, #0
	lsls	r0, r0, #8
	bl	sub_080151e4
	ldr	r2, [pc, #404]
	movs	r3, #60
	str	r3, [r2, #12]
	ldr	r5, [sp, #88]
	cmp	r5, #0
	bne.n	.L_08172962
	ldr	r0, [sp, #68]
	movs	r2, #127
	lsls	r3, r0, #3
	ands	r3, r2
	mov	r1, fp
	strb	r3, [r1, #24]
	b.n	.L_0817296e
.L_08172962:
	ldr	r3, [sp, #68]
	mov	r4, fp
	lsls	r2, r3, #3
	movs	r3, #127
	bics	r3, r2
	strb	r3, [r4, #24]
.L_0817296e:
	movs	r5, #0
	mov	r0, fp
	strb	r5, [r0, #25]
	add	r0, sp, #140
	bl	sub_08015128
	movs	r1, #128
	lsls	r1, r1, #13
	movs	r2, #0
	movs	r0, #0
	bl	sub_08015160
	ldr	r2, [sp, #68]
	movs	r1, #0
	movs	r6, #128
	mov	r8, r1
	lsls	r6, r6, #9
	lsls	r5, r2, #11
.L_08172992:
	bl	sub_08014e38
	ldr	r4, [sp, #92]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_081729a6
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_081729a6:
	movs	r2, #192
	mov	r1, r8
	lsls	r2, r2, #4
	lsls	r0, r1, #10
	adds	r2, #24
	adds	r0, r0, r2
	bl	sub_080150e4
	ldr	r3, [sp, #88]
	cmp	r3, #1
	beq.n	.L_081729c4
	adds	r0, r5, #0
	bl	sub_08015024
	b.n	.L_081729d2
.L_081729c4:
	ldr	r1, [sp, #68]
	mov	r4, r8
	lsls	r0, r4, #13
	lsls	r3, r1, #11
	subs	r0, r0, r3
	bl	sub_08015024
.L_081729d2:
	movs	r0, #128
	lsls	r0, r0, #11
	adds	r1, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	movs	r2, #66
	ldr	r1, [pc, #248]
	ldr	r0, [pc, #244]
	bl	sub_081969ac
	mov	r0, fp
	bl	sub_08196a7c
	bl	sub_08014ea8
	movs	r3, #1
	movs	r2, #128
	add	r8, r3
	lsls	r2, r2, #6
	mov	r4, r8
	adds	r5, r5, r2
	cmp	r4, #3
	bne.n	.L_08172992
.L_08172a02:
	ldr	r2, [pc, #220]
	movs	r3, #120
	str	r3, [r2, #12]
.L_08172a08:
	mov	r0, fp
	bl	sub_08013164
	ldr	r5, [sp, #88]
	cmp	r5, #2
	beq.n	.L_08172a18
	cmp	r5, #4
	bne.n	.L_08172a22
.L_08172a18:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	b.n	.L_08172a2a
.L_08172a22:
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
.L_08172a2a:
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #84]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #20]
	ldr	r4, [sp, #68]
	movs	r3, #60
	ldr	r5, [sp, #56]
	adds	r3, #255
	adds	r2, r2, r3
	adds	r4, #1
	str	r2, [sp, #20]
	str	r4, [sp, #68]
	cmp	r4, r5
	beq.n	.L_08172a5c
	bl	.L_08171e70
.L_08172a5c:
	ldr	r0, [sp, #52]
	movs	r1, #42
	adds	r1, #255
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08172ab4
	ldr	r2, [sp, #88]
	cmp	r2, #0
	bne.n	.L_08172ab4
	ldr	r4, [sp, #92]
	ldr	r3, [r4, #24]
	cmp	r3, #1
	bne.n	.L_08172ab4
	ldr	r5, [sp, #84]
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #228
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #48]
	movs	r2, #221
	str	r3, [r1, #8]
	lsls	r2, r2, #3
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r1, #12]
	lsls	r4, r4, #3
	adds	r4, #236
	adds	r3, r5, r4
	ldr	r3, [r3, #0]
	adds	r0, #12
	str	r3, [r1, #16]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	str	r3, [r1, #24]
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #244
	adds	r3, r5, r1
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #48]
	str	r3, [r2, #28]
.L_08172ab4:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #40]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #176
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffcc0000
	.4byte 0x02010000
	.4byte 0x02011000
	.4byte 0x030011e0
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_08172b00
	pop	{pc}
	.2byte 0x0000
