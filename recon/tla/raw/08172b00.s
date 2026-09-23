.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
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
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_080156e8, 0x080156e8
	.set sub_08020098, 0x08020098
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_08173228, 0x08173228
	.set sub_08173588, 0x08173588
	.set sub_0818de3c, 0x0818de3c
	.set sub_081963ec, 0x081963ec
	.set sub_081969ac, 0x081969ac
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08172b00
	.thumb_func
Func_08172b00:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #144
	str	r0, [sp, #96]
	str	r1, [sp, #92]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	ldr	r0, [r3, #92]
	str	r1, [sp, #88]
	mov	fp, r0
	ldr	r2, [r3, #48]
	movs	r0, #0
	str	r2, [sp, #72]
	ldr	r3, [r3, #100]
	str	r3, [sp, #68]
	bl	sub_081435e0
	ldr	r3, [sp, #96]
	ldr	r4, [sp, #92]
	ldr	r1, [r3, #4]
	ldr	r3, [pc, #64]
	mov	r0, sp
	ldrb	r3, [r3, r4]
	mov	r2, sp
	lsls	r3, r3, #5
	adds	r0, #120
	adds	r2, #132
	lsls	r1, r1, #4
	str	r0, [sp, #64]
	str	r2, [sp, #60]
	orrs	r1, r3
	movs	r3, #2
	orrs	r1, r3
	ldr	r2, [sp, #64]
	ldr	r3, [sp, #60]
	ldr	r0, [sp, #96]
	bl	sub_0815585c
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r4, [sp, #96]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08172b7c
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	b.n	.L_08172b84
	.4byte 0x00000785
	.2byte 0x8c14
	.2byte 0x0819
.L_08172b7c:
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
.L_08172b84:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #128
	str	r3, [sp, #80]
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #50
	add	r2, fp
	movs	r1, #200
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #28]
	bl	sub_080145a8
	ldr	r2, [sp, #96]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r3, [sp, #72]
	str	r0, [sp, #56]
	movs	r4, #54
	ldrsh	r3, [r3, r4]
	b.n	.L_08172bd4
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_08172bd4:
	ldr	r4, [sp, #96]
	str	r3, [sp, #52]
	ldr	r0, [r4, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r2, #217
	str	r0, [sp, #48]
	ldr	r0, [sp, #60]
	lsls	r2, r2, #3
	ldr	r3, [r0, #0]
	add	r2, fp
	str	r3, [r2, #0]
	movs	r2, #192
	ldr	r3, [r0, #4]
	lsls	r2, r2, #3
	adds	r2, #204
	add	r2, fp
	str	r3, [r2, #0]
	movs	r2, #218
	ldr	r3, [r0, #8]
	lsls	r2, r2, #3
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r1, [sp, #68]
	ldr	r0, [pc, #88]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #184
	lsls	r1, r1, #5
	add	r1, fp
	ldr	r0, [pc, #76]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r1, [sp, #92]
	cmp	r1, #0
	bne.n	.L_08172c78
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #60]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #156
	lsls	r1, r1, #6
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #40]
	bl	sub_08157cf4
	ldr	r0, [pc, #40]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #32]
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2266
	str	r2, [sp, #44]
	b.n	.L_08172c98
	.4byte 0x00000134
	.4byte 0x00000151
	.4byte 0x000000cd
	.4byte 0x0000017b
	.4byte 0x00000148
	.2byte 0x0730
	.2byte 0x0300
.L_08172c78:
	ldr	r0, [pc, #68]
	ldr	r1, [pc, #72]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #156
	lsls	r1, r1, #6
	movs	r3, #0
	ldr	r0, [pc, #60]
	add	r1, fp
	movs	r2, #1
	bl	sub_08157cf4
	movs	r3, #50
	str	r3, [sp, #44]
.L_08172c98:
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [sp, #48]
	movs	r1, #240
	bl	sub_08020098
	ldr	r0, [sp, #44]
	movs	r4, #0
	str	r4, [sp, #76]
	cmp	r0, #0
	bne.n	.L_08172cb8
	bl	.L_0817352c
.L_08172cb8:
	b.n	.L_08172ccc
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x000000c5
	.4byte 0x02010000
	.2byte 0x013e
	.2byte 0x0000
.L_08172ccc:
	ldr	r1, [sp, #72]
	movs	r3, #192
	adds	r1, #12
	str	r1, [sp, #16]
	movs	r0, #192
	movs	r1, #192
	ldr	r2, [sp, #60]
	lsls	r3, r3, #3
	movs	r4, #221
	lsls	r0, r0, #3
	lsls	r1, r1, #3
	adds	r3, #228
	lsls	r4, r4, #3
	adds	r0, #236
	adds	r1, #244
	add	r3, fp
	add	r4, fp
	add	r0, fp
	add	r1, fp
	str	r2, [sp, #40]
	str	r3, [sp, #36]
	str	r4, [sp, #32]
	str	r0, [sp, #28]
	str	r1, [sp, #24]
.L_08172cfc:
	ldr	r2, [sp, #76]
	cmp	r2, #0
	bne.n	.L_08172da6
	add	r3, sp, #52
	ldr	r4, [sp, #72]
	ldrh	r3, [r3, #0]
	strh	r3, [r4, #54]
	movs	r3, #217
	lsls	r3, r3, #3
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r4, [sp, #40]
	str	r3, [r4, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #204
	add	r3, fp
	ldr	r3, [r3, #0]
	str	r3, [r4, #4]
	movs	r3, #218
	lsls	r3, r3, #3
	add	r3, fp
	ldr	r3, [r3, #0]
	str	r3, [r4, #8]
	ldr	r0, [sp, #36]
	str	r2, [r0, #0]
	ldr	r1, [sp, #32]
	str	r2, [r1, #0]
	ldr	r3, [sp, #28]
	movs	r1, #222
	str	r2, [r3, #0]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #40]
	ldr	r3, [r4, #8]
	ldr	r2, [r0, #0]
	lsls	r1, r1, #3
	add	r1, fp
	subs	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r1, [sp, #92]
	cmp	r1, #0
	bne.n	.L_08172d5a
	ldr	r3, [r4, #12]
	ldr	r4, [sp, #60]
	movs	r0, #240
	ldr	r2, [r4, #4]
	b.n	.L_08172d64
.L_08172d5a:
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #60]
	ldr	r3, [r2, #12]
	ldr	r2, [r4, #4]
	movs	r0, #240
.L_08172d64:
	ldr	r1, [sp, #24]
	subs	r3, r3, r2
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r1, #0]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #60]
	ldr	r2, [r4, #16]
	ldr	r1, [r0, #8]
	movs	r3, #223
	lsls	r3, r3, #3
	subs	r2, r2, r1
	add	r3, fp
	str	r2, [r3, #0]
	movs	r1, #0
	movs	r2, #1
	mov	r3, fp
	mov	r8, r1
	negs	r2, r2
	adds	r3, #24
.L_08172d8c:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r0, #62
	bne.n	.L_08172d8c
	ldr	r1, [sp, #92]
	cmp	r1, #1
	bne.n	.L_08172da6
	movs	r0, #145
	bl	sub_081c0010
.L_08172da6:
	bl	sub_08014de4
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #16]
	bl	sub_080156e8
	ldr	r2, [sp, #92]
	cmp	r2, #0
	beq.n	.L_08172dba
	b.n	.L_08172ffa
.L_08172dba:
	ldr	r3, [sp, #76]
	cmp	r3, #15
	bhi.n	.L_08172df0
	movs	r2, #128
	lsls	r2, r2, #1
	cmp	r3, #71
	ble.n	.L_08172dd2
	ldr	r4, [sp, #76]
	movs	r3, #160
	lsls	r2, r4, #5
	lsls	r3, r3, #4
	subs	r2, r3, r2
.L_08172dd2:
	ldr	r0, [sp, #96]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_08172de6
	ldr	r1, [sp, #72]
	ldrh	r3, [r1, #54]
	subs	r3, r3, r2
	adds	r2, r1, #0
	strh	r3, [r2, #54]
	b.n	.L_08172df0
.L_08172de6:
	ldr	r4, [sp, #72]
	ldrh	r3, [r4, #54]
	adds	r0, r4, #0
	adds	r3, r3, r2
	strh	r3, [r0, #54]
.L_08172df0:
	ldr	r1, [sp, #76]
	cmp	r1, #31
	bgt.n	.L_08172ea4
	movs	r1, #192
	ldr	r2, [sp, #40]
	lsls	r1, r1, #3
	adds	r1, #228
	add	r1, fp
	ldr	r3, [r2, #0]
	ldr	r2, [r1, #0]
	ldr	r4, [sp, #40]
	subs	r3, r3, r2
	str	r3, [r4, #0]
	movs	r0, #221
	lsls	r0, r0, #3
	add	r0, fp
	ldr	r3, [r4, #4]
	ldr	r2, [r0, #0]
	subs	r3, r3, r2
	str	r3, [r4, #4]
	movs	r4, #192
	ldr	r2, [sp, #40]
	lsls	r4, r4, #3
	adds	r4, #236
	add	r4, fp
	ldr	r3, [r2, #8]
	ldr	r2, [r4, #0]
	subs	r3, r3, r2
	ldr	r2, [sp, #40]
	str	r3, [r2, #8]
	ldr	r3, [sp, #76]
	cmp	r3, #15
	bgt.n	.L_08172e5e
	movs	r3, #222
	lsls	r3, r3, #3
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r1, #0]
	asrs	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r3, [sp, #24]
	ldr	r2, [r3, #0]
	ldr	r3, [r0, #0]
	asrs	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r0, #0]
	movs	r3, #223
	lsls	r3, r3, #3
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r4, #0]
	asrs	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r4, #0]
.L_08172e5e:
	ldr	r2, [r1, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_08172e6c
	adds	r3, #63
.L_08172e6c:
	asrs	r3, r3, #6
	str	r3, [r1, #0]
	ldr	r2, [r0, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_08172e7e
	adds	r3, #63
.L_08172e7e:
	asrs	r3, r3, #6
	str	r3, [r0, #0]
	ldr	r2, [r4, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_08172e90
	adds	r3, #63
.L_08172e90:
	asrs	r3, r3, #6
	str	r3, [r4, #0]
	ldr	r4, [sp, #76]
	cmp	r4, #31
	beq.n	.L_08172e9c
	b.n	.L_08173118
.L_08172e9c:
	movs	r0, #138
	bl	sub_081c0010
	b.n	.L_08173118
.L_08172ea4:
	ldr	r0, [sp, #76]
	cmp	r0, #45
	bgt.n	.L_08172efa
	ldr	r1, [sp, #40]
	ldr	r4, [sp, #36]
	ldr	r3, [r1, #0]
	ldr	r2, [r4, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r0, [sp, #32]
	ldr	r3, [r1, #4]
	ldr	r2, [r0, #0]
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r1, #8]
	ldr	r1, [sp, #28]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	ldr	r2, [sp, #40]
	str	r3, [r2, #8]
	movs	r3, #222
	lsls	r3, r3, #3
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r4, #0]
	asrs	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r4, #0]
	ldr	r3, [sp, #24]
	ldr	r2, [r3, #0]
	ldr	r3, [r0, #0]
	asrs	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r0, #0]
	movs	r3, #223
	lsls	r3, r3, #3
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r1, #0]
	asrs	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r1, #0]
	b.n	.L_08173118
.L_08172efa:
	ldr	r4, [sp, #76]
	cmp	r4, #72
	ble.n	.L_08172f02
	b.n	.L_08173118
.L_08172f02:
	movs	r3, #3
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08172f0c
	b.n	.L_08173118
.L_08172f0c:
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #40]
	ldr	r2, [r0, #0]
	movs	r7, #0
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r1, #0]
	asrs	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r3, [sp, #32]
	ldr	r2, [r3, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r1, #4]
	asrs	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r4, [sp, #28]
	ldr	r2, [r4, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r1, #8]
	asrs	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r1, #8]
	ldr	r0, [sp, #76]
	cmp	r0, #72
	bne.n	.L_08172f58
	movs	r0, #134
	bl	sub_081180e8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #8
	b.n	.L_08172f68
.L_08172f58:
	movs	r0, #138
	bl	sub_081c0010
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #2
.L_08172f68:
	str	r3, [r2, #0]
	ldr	r2, [sp, #96]
	movs	r3, #6
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	movs	r2, #5
	bl	sub_0814cd48
	movs	r3, #0
	mov	r8, r3
	mov	r5, fp
.L_08172f84:
	ldr	r3, [r5, #24]
	movs	r4, #1
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_08172fec
	ldr	r0, [sp, #56]
	movs	r1, #160
	ldr	r3, [r0, #8]
	lsls	r1, r1, #11
	str	r3, [r5, #0]
	movs	r6, #128
	ldr	r3, [r0, #12]
	lsls	r6, r6, #1
	adds	r3, r3, r1
	str	r3, [r5, #4]
	adds	r6, #255
	ldr	r3, [r0, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r2, #128
	ands	r0, r6
	lsls	r2, r2, #1
	adds	r0, r0, r2
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	ldr	r4, [sp, #96]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08172fc4
	negs	r3, r0
	str	r3, [r5, #12]
.L_08172fc4:
	bl	sub_08014878
	movs	r1, #128
	lsls	r1, r1, #1
	ands	r0, r6
	adds	r0, r0, r1
	movs	r3, #0
	lsls	r0, r0, #9
	str	r3, [r5, #20]
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	adds	r7, #1
	str	r3, [r5, #24]
	cmp	r7, #16
	bne.n	.L_08172fec
	b.n	.L_08173118
.L_08172fec:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #63
	bne.n	.L_08172f84
	b.n	.L_08173118
.L_08172ffa:
	ldr	r4, [sp, #76]
	cmp	r4, #22
	bhi.n	.L_08173054
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #36]
	ldr	r3, [r0, #0]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r4, [sp, #32]
	ldr	r3, [r0, #4]
	ldr	r2, [r4, #0]
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r3, [r0, #8]
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #40]
	ldr	r2, [r0, #0]
	adds	r3, r3, r2
	str	r3, [r1, #8]
	movs	r3, #222
	ldr	r4, [sp, #36]
	lsls	r3, r3, #3
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r4, #0]
	asrs	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r4, #0]
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #32]
	ldr	r2, [r0, #0]
	ldr	r3, [r1, #0]
	asrs	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r1, #0]
	movs	r3, #223
	ldr	r4, [sp, #28]
	lsls	r3, r3, #3
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r4, #0]
	asrs	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r4, #0]
.L_08173054:
	ldr	r0, [sp, #76]
	cmp	r0, #15
	bne.n	.L_08173118
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	movs	r5, #6
	str	r5, [r3, #0]
	movs	r0, #134
	bl	sub_081180e8
	ldr	r2, [sp, #96]
	movs	r3, #0
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #7
	movs	r2, #5
	str	r5, [sp, #0]
	bl	sub_0814cd48
	ldr	r4, [sp, #96]
	movs	r1, #0
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118088
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #0
	adds	r1, #255
	mov	r8, r0
	mov	sl, r1
	mov	r6, fp
.L_08173098:
	ldr	r2, [sp, #56]
	movs	r4, #160
	ldr	r3, [r2, #8]
	lsls	r4, r4, #12
	str	r3, [r6, #0]
	ldr	r3, [r2, #12]
	adds	r3, r3, r4
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r0
	ldr	r0, [pc, #656]
	mov	r1, r8
	adds	r7, r3, r0
	cmp	r1, #53
	ble.n	.L_081730d2
	bl	sub_08014878
	mov	r2, sl
	movs	r3, #128
	ands	r0, r2
	lsls	r3, r3, #1
	adds	r5, r0, r3
	b.n	.L_081730e0
.L_081730d2:
	bl	sub_08014878
	mov	r4, sl
	movs	r1, #128
	ands	r0, r4
	lsls	r1, r1, #1
	adds	r5, r0, r1
.L_081730e0:
	adds	r0, r7, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r6, #12]
	adds	r0, r7, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r6, #16]
	movs	r3, #0
	str	r3, [r6, #20]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r2, #1
	adds	r3, #16
	add	r8, r2
	str	r3, [r6, #24]
	mov	r3, r8
	adds	r6, #28
	cmp	r3, #63
	bne.n	.L_08173098
.L_08173118:
	ldr	r0, [sp, #60]
	ldr	r1, [sp, #64]
	bl	sub_0815e1ec
	ldr	r4, [sp, #92]
	cmp	r4, #0
	bne.n	.L_0817319a
	ldr	r0, [sp, #76]
	cmp	r0, #73
	bgt.n	.L_08173154
	ldr	r1, [sp, #64]
	ldr	r0, [sp, #88]
	ldr	r2, [r1, #0]
	ldr	r4, [sp, #80]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r1, #4]
	movs	r1, #20
	str	r1, [sp, #0]
	movs	r1, #40
	str	r1, [sp, #4]
	movs	r1, #184
	asrs	r2, r2, #1
	lsls	r1, r1, #5
	subs	r2, #10
	subs	r3, #20
	add	r1, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe069
.L_08173154:
	ldr	r0, [sp, #76]
	cmp	r0, #93
	bgt.n	sub_08173228
	adds	r3, r0, #0
	subs	r3, #74
	cmp	r3, #0
	bge.n	.L_08173164
	adds	r3, #3
.L_08173164:
	asrs	r3, r3, #2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	adds	r1, r1, r3
	ldr	r3, [sp, #64]
	lsls	r1, r1, #6
	movs	r2, #156
	lsls	r2, r2, #6
	add	r1, fp
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r4, [sp, #64]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r4, #4]
	movs	r0, #40
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #20
	subs	r3, #20
	ldr	r0, [sp, #88]
	ldr	r4, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe046
.L_0817319a:
	ldr	r0, [sp, #76]
	cmp	r0, #22
	bgt.n	sub_08173228
	ldr	r1, [sp, #64]
	ldr	r0, [sp, #88]
	ldr	r2, [r1, #0]
	ldr	r4, [sp, #80]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r1, #4]
	movs	r1, #20
	str	r1, [sp, #0]
	movs	r1, #40
	str	r1, [sp, #4]
	movs	r1, #184
	asrs	r2, r2, #1
	lsls	r1, r1, #5
	subs	r2, #10
	subs	r3, #20
	add	r1, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9817
	cmp	r0, #1
	bne.n	sub_08173228
	ldr	r1, [sp, #76]
	cmp	r1, #1
	ble.n	sub_08173228
	cmp	r1, #0
	bge.n	.L_081731d8
	adds	r1, #3
.L_081731d8:
	ldr	r2, [sp, #96]
	movs	r3, #3
	asrs	r1, r1, #2
	ands	r1, r3
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_081731f6
	ldr	r4, [sp, #64]
	ldr	r3, [r4, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r4, r3, #0
	subs	r4, #14
	b.n	.L_08173204
.L_081731f6:
	ldr	r0, [sp, #64]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r4, r3, #0
	subs	r4, #46
.L_08173204:
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #112
	ldr	r0, [sp, #64]
	muls	r1, r3
	ldr	r2, [pc, #320]
	ldr	r3, [r0, #4]
	adds	r1, r1, r2
	movs	r2, #60
	str	r2, [sp, #0]
	movs	r2, #100
	str	r2, [sp, #4]
	subs	r3, #50
	adds	r2, r4, #0
	ldr	r0, [sp, #88]
	ldr	r4, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9917
	movs	r0, #62
	mov	sl, r0
	cmp	r1, #0
	beq.n	.L_08173236
	movs	r2, #54
	mov	sl, r2
.L_08173236:
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	cmp	r4, #0
	beq.n	.L_081732bc
	ldr	r0, [pc, #272]
	add	r7, sp, #108
	mov	r6, fp
	mov	r9, r0
.L_08173248:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	blt.n	.L_081732b2
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	asrs	r5, r5, #3
	adds	r5, #2
	ldr	r2, [pc, #252]
	lsls	r0, r5, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #68]
	ldr	r4, [sp, #80]
	adds	r1, r2, r1
	ldr	r2, [r7, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	asrs	r2, r2, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r0, [sp, #4]
	subs	r3, r3, r5
	str	r5, [sp, #0]
	ldr	r0, [sp, #88]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4640
	movs	r3, #3
	ands	r3, r0
	lsls	r3, r3, #2
	mov	r1, r9
	ldr	r2, [r1, r3]
	adds	r0, r6, #0
	movs	r1, #62
	bl	sub_08138058
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_081732ac
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
.L_081732ac:
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_081732b2:
	movs	r2, #1
	add	r8, r2
	adds	r6, #28
	cmp	r8, sl
	bne.n	.L_08173248
.L_081732bc:
	ldr	r3, [sp, #92]
	cmp	r3, #1
	bne.n	.L_0817332c
	movs	r6, #189
	movs	r4, #0
	lsls	r6, r6, #3
	mov	r8, r4
	add	r7, sp, #108
	add	r6, fp
.L_081732ce:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	blt.n	.L_08173320
	movs	r1, #6
	bl	sub_08002054
	movs	r5, #5
	subs	r5, r5, r0
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r2, [r7, #0]
	lsls	r5, r5, #11
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	movs	r0, #156
	ldr	r3, [r7, #4]
	lsls	r0, r0, #6
	movs	r1, #32
	add	r5, fp
	str	r1, [sp, #0]
	adds	r5, r5, r0
	movs	r1, #64
	asrs	r2, r2, #1
	subs	r3, #32
	str	r1, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #88]
	adds	r1, r5, #0
	ldr	r4, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #62
	ldr	r2, [pc, #68]
	bl	sub_08138058
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_08173320:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #8
	bne.n	.L_081732ce
.L_0817332c:
	ldr	r2, [sp, #92]
	cmp	r2, #0
	bne.n	.L_08173360
	ldr	r3, [sp, #76]
	cmp	r3, #24
	bne.n	.L_08173378
	ldr	r4, [sp, #96]
	movs	r1, #1
	ldr	r0, [r4, #8]
	negs	r1, r1
	movs	r2, #2
	movs	r3, #0
	str	r1, [sp, #0]
	bl	sub_0814cd48
	b.n	.L_08173378
	.4byte 0xffffc000
	.4byte 0x02010000
	.4byte 0x08198c18
	.4byte 0x08197410
	.2byte 0xfc00
	.2byte 0xffff
.L_08173360:
	ldr	r0, [sp, #76]
	cmp	r0, #0
	bne.n	.L_08173378
	ldr	r1, [sp, #96]
	movs	r2, #2
	ldr	r0, [r1, #8]
	movs	r1, #1
	negs	r1, r1
	movs	r3, #0
	str	r1, [sp, #0]
	bl	sub_0814cd48
.L_08173378:
	ldr	r2, [sp, #92]
	cmp	r2, #0
	beq.n	.L_08173380
	b.n	.L_081734fe
.L_08173380:
	ldr	r3, [sp, #76]
	cmp	r3, #89
	ble.n	.L_08173388
	b.n	.L_081734fe
.L_08173388:
	movs	r0, #1
	bl	sub_081969f8
	ldr	r4, [sp, #76]
	mov	r8, r0
	cmp	r4, #0
	bne.n	.L_0817341c
	movs	r3, #8
	ldr	r0, [pc, #440]
	movs	r1, #8
	movs	r2, #8
	str	r3, [sp, #0]
	bl	sub_0818de3c
	movs	r0, #0
	str	r0, [sp, #20]
	str	r0, [sp, #12]
	mov	r9, r0
.L_081733ac:
	mov	r0, r9
	bl	sub_08002090
	negs	r0, r0
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r3, r3, #16
	mov	r0, r9
	mov	sl, r3
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r2, r3, #16
	ldr	r1, [sp, #12]
	ldr	r3, [pc, #392]
	movs	r7, #0
	adds	r6, r1, r3
.L_081733d4:
	lsls	r5, r7, #13
	adds	r0, r5, #0
	str	r2, [sp, #8]
	bl	sub_08002096
	ldr	r2, [sp, #8]
	mov	r4, sl
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #0]
	strb	r4, [r6, #1]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [sp, #8]
	adds	r7, #1
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #2]
	adds	r6, #4
	cmp	r7, #8
	bne.n	.L_081733d4
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #20]
	movs	r0, #144
	lsls	r0, r0, #5
	adds	r0, #73
	adds	r1, #32
	adds	r2, #1
	add	r9, r0
	str	r1, [sp, #12]
	str	r2, [sp, #20]
	cmp	r2, #8
	bne.n	.L_081733ac
.L_0817341c:
	ldr	r3, [sp, #100]
	ldr	r2, [pc, #316]
	add	r5, sp, #100
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #308]
	movs	r1, #128
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #100]
	movs	r3, #224
	lsls	r3, r3, #3
	movs	r2, #128
	add	r3, fp
	lsls	r2, r2, #8
	adds	r0, r2, #0
	lsls	r1, r1, #9
	str	r3, [r5, #4]
	bl	sub_080151e4
	ldr	r2, [pc, #280]
	movs	r3, #60
	str	r3, [r2, #12]
	ldr	r0, [sp, #60]
	bl	sub_08015128
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #13
	movs	r2, #0
	bl	sub_08015160
	ldr	r4, [sp, #96]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08173476
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #96
	bl	sub_080150e4
	b.n	.L_08173480
.L_08173476:
	movs	r0, #175
	lsls	r0, r0, #8
	adds	r0, #160
	bl	sub_080150e4
.L_08173480:
	ldr	r1, [sp, #76]
	movs	r0, #160
	lsls	r0, r0, #8
	cmp	r1, #31
	ble.n	.L_08173492
	movs	r2, #128
	lsls	r3, r1, #10
	lsls	r2, r2, #6
	adds	r0, r3, r2
.L_08173492:
	bl	sub_0801521c
	ldr	r3, [sp, #76]
	cmp	r3, #3
	ble.n	.L_081734f2
	ldr	r4, [sp, #76]
	lsls	r3, r3, #2
	adds	r1, r3, #0
	subs	r1, #80
	cmp	r4, #73
	ble.n	.L_081734ae
	movs	r3, #74
	subs	r3, r3, r4
	lsls	r1, r3, #2
.L_081734ae:
	cmp	r1, #0
	ble.n	.L_081734b4
	movs	r1, #0
.L_081734b4:
	mov	r0, r8
	movs	r3, #6
	str	r3, [r0, #0]
	ldr	r2, [pc, #152]
	ldr	r3, [pc, #168]
	str	r5, [r0, #16]
	str	r2, [r0, #8]
	str	r3, [r0, #12]
	ldr	r3, [sp, #76]
	cmp	r3, #0
	bge.n	.L_081734cc
	adds	r3, #3
.L_081734cc:
	movs	r2, #63
	asrs	r3, r3, #2
	mov	r4, r8
	ands	r3, r2
	strb	r3, [r4, #24]
	ldr	r3, [sp, #76]
	ldr	r0, [pc, #124]
	ands	r3, r2
	strb	r3, [r4, #25]
	movs	r3, #3
	str	r1, [r4, #20]
	str	r3, [r4, #4]
	ldr	r1, [pc, #128]
	movs	r2, #64
	bl	sub_081969ac
	mov	r0, r8
	bl	sub_08196a7c
.L_081734f2:
	ldr	r2, [pc, #112]
	movs	r3, #120
	str	r3, [r2, #12]
	mov	r0, r8
	bl	sub_08013164
.L_081734fe:
	movs	r1, #8
	movs	r0, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #76]
	ldr	r1, [sp, #44]
	adds	r0, #1
	str	r0, [sp, #76]
	cmp	r0, r1
	beq.n	.L_0817352c
	bl	.L_08172cfc
.L_0817352c:
	ldr	r0, [sp, #48]
	movs	r1, #16
	bl	sub_08020098
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #48]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #144
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02014000
	.4byte 0x02010000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x030011e0
	.4byte 0x02012000
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_08173588
	pop	{pc}
	.align 2, 0
