.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
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
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_080156e8, 0x080156e8
	.set sub_08118028, 0x08118028
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b434, 0x0815b434
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_0817136e, 0x0817136e
	.set sub_08171bb0, 0x08171bb0
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08170dac
	.thumb_func
Func_08170dac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #248
	str	r0, [sp, #76]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r6, #0
	str	r0, [sp, #72]
	movs	r0, #1
	ldr	r1, [r3, #96]
	str	r1, [sp, #68]
	ldr	r2, [r3, #48]
	str	r2, [sp, #52]
	ldr	r3, [r3, #100]
	str	r3, [sp, #48]
	bl	sub_081435e0
	ldr	r3, [sp, #72]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #260]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r5, [sp, #72]
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r1, r5, r2
	ldr	r0, [pc, #244]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #48]
	ldr	r0, [pc, #236]
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #192
	ldr	r2, [sp, #48]
	lsls	r1, r1, #2
	adds	r1, #2
.L_08170e12:
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08170e22
	subs	r3, #8
	cmp	r3, #0
	bgt.n	.L_08170e20
	movs	r3, #1
.L_08170e20:
	strb	r3, [r2, #0]
.L_08170e22:
	adds	r6, #1
	adds	r2, #1
	cmp	r6, r1
	bne.n	.L_08170e12
	ldr	r3, [pc, #200]
	movs	r5, #128
	lsls	r5, r5, #5
	adds	r1, r5, r3
	ldr	r0, [pc, #196]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	adds	r1, r5, #0
	ldr	r3, [pc, #188]
	ldr	r0, [pc, #176]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4c2b
	movs	r6, #0
	movs	r7, #0
	movs	r5, #0
	mov	ip, r4
.L_08170e50:
	ldr	r1, [pc, #172]
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
.L_08170e6e:
	ldrb	r3, [r1, #0]
	adds	r4, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r3, [r1, #0]
	adds	r1, #1
	strb	r3, [r2, #0]
	subs	r2, #1
	cmp	r4, #24
	bne.n	.L_08170e6e
	adds	r6, #1
	adds	r7, #2
	adds	r5, #64
	cmp	r6, #48
	bne.n	.L_08170e50
	ldr	r0, [pc, #116]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #112]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2084
	ldr	r4, [sp, #72]
	lsls	r0, r0, #6
	adds	r0, #32
	adds	r5, r4, r0
	movs	r6, #0
.L_08170eac:
	lsls	r0, r6, #9
	bl	sub_08002096
	lsls	r3, r0, #6
	subs	r3, r3, r0
	asrs	r3, r3, #16
	movs	r2, #0
.L_08170eba:
	adds	r2, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	cmp	r2, #32
	bne.n	.L_08170eba
	adds	r6, #1
	cmp	r6, #32
	bne.n	.L_08170eac
	movs	r3, #128
	ldr	r2, [pc, #20]
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	ldr	r1, [sp, #76]
	movs	r6, #4
	ldr	r3, [r1, #4]
	cmp	r3, #0
	beq.n	.L_08170f0c
	movs	r6, #0
	b.n	.L_08170f0c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000178
	.4byte 0x00000141
	.4byte 0x00000134
	.4byte 0x02010000
	.4byte 0x0000013a
	.4byte 0x03000258
	.4byte 0x02010208
	.4byte 0x00000188
	.2byte 0x0730
	.2byte 0x0300
.L_08170f0c:
	movs	r3, #3
	orrs	r3, r6
	movs	r5, #1
	movs	r1, #7
	movs	r2, #7
	movs	r0, #104
	str	r5, [sp, #0]
	bl	sub_08196404
	movs	r3, #11
	orrs	r6, r3
	movs	r1, #7
	movs	r2, #7
	adds	r3, r6, #0
	movs	r0, #188
	str	r5, [sp, #0]
	bl	sub_08196404
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #104]
	adds	r3, #188
	ldr	r4, [sp, #72]
	ldr	r3, [r3, #0]
	movs	r5, #238
	lsls	r5, r5, #7
	movs	r0, #238
	adds	r5, #172
	lsls	r0, r0, #7
	str	r2, [sp, #56]
	str	r3, [sp, #60]
	movs	r2, #0
	adds	r3, r4, r5
	adds	r0, #176
	str	r2, [r3, #0]
	movs	r1, #144
	adds	r3, r4, r0
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #176]
	bl	sub_080145a8
	ldr	r1, [sp, #72]
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r1, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #144]
	bl	sub_080145a8
	movs	r0, #140
	bl	sub_081c0010
	movs	r3, #238
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #72]
	lsls	r3, r3, #7
	mov	r1, sp
	adds	r3, #172
	movs	r5, #0
	adds	r0, #12
	adds	r1, #236
	adds	r3, r2, r3
	str	r5, [sp, #64]
	str	r0, [sp, #16]
	str	r1, [sp, #24]
	str	r3, [sp, #44]
	str	r5, [sp, #12]
.L_08170fa6:
	ldr	r3, [sp, #64]
	subs	r3, #9
	cmp	r3, #54
	bhi.n	.L_08170fb6
	ldr	r3, [pc, #100]
	ldr	r4, [sp, #44]
	str	r3, [r4, #0]
	b.n	.L_08170fd2
.L_08170fb6:
	ldr	r3, [sp, #64]
	subs	r3, #64
	cmp	r3, #15
	bhi.n	.L_08170fcc
	ldr	r5, [sp, #64]
	ldr	r0, [pc, #84]
	ldr	r1, [sp, #44]
	lsls	r3, r5, #4
	adds	r3, r3, r0
	str	r3, [r1, #0]
	b.n	.L_08170fd2
.L_08170fcc:
	ldr	r2, [sp, #44]
	movs	r3, #0
	str	r3, [r2, #0]
.L_08170fd2:
	ldr	r4, [sp, #76]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08170fe2
	ldr	r5, [sp, #44]
	ldr	r3, [r5, #0]
	negs	r3, r3
	str	r3, [r5, #0]
.L_08170fe2:
	ldr	r0, [sp, #64]
	cmp	r0, #79
	ble.n	.L_0817101c
	lsls	r1, r0, #1
	ldr	r3, [pc, #24]
	adds	r2, r1, #0
	subs	r2, #160
	subs	r3, r3, r2
	ldr	r2, [pc, #20]
	str	r1, [sp, #20]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_08171022
	movs	r0, r0
	.4byte 0x00000010
	.4byte 0x00001000
	.4byte 0x0814c929
	.4byte 0x08143001
	.4byte 0xffffff00
	.2byte 0xfb00
	.2byte 0xffff
.L_0817101c:
	ldr	r2, [sp, #64]
	lsls	r2, r2, #1
	str	r2, [sp, #20]
.L_08171022:
	movs	r2, #0
	movs	r3, #100
	movs	r0, #0
	movs	r1, #0
	bl	sub_08118028
	bl	sub_08014de4
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #16]
	bl	sub_080156e8
	ldr	r3, [sp, #76]
	ldr	r0, [r3, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	str	r0, [sp, #40]
	ldr	r3, [r0, #8]
	cmp	r3, #0
	ble.n	.L_08171056
	ldr	r4, [pc, #500]
	ldr	r5, [sp, #24]
	adds	r3, r3, r4
	str	r3, [r5, #0]
	b.n	.L_08171060
.L_08171056:
	movs	r0, #128
	ldr	r1, [sp, #24]
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r1, #0]
.L_08171060:
	ldr	r3, [pc, #484]
	ldr	r2, [sp, #12]
	adds	r3, r2, r3
	str	r3, [sp, #36]
	cmp	r3, #0
	bge.n	.L_0817106e
	b.n	.L_08171176
.L_0817106e:
	mov	r4, sp
	adds	r4, #104
	ldr	r5, [pc, #472]
	str	r4, [sp, #28]
	ldr	r7, [sp, #24]
	mov	r8, r5
	movs	r6, #0
	adds	r5, r4, #0
.L_0817107e:
	adds	r0, r6, #4
	movs	r1, #6
	bl	sub_08002064
	lsls	r0, r0, #1
	mov	r1, r8
	adds	r3, r0, #1
	ldrsb	r3, [r1, r3]
	movs	r2, #240
	lsls	r2, r2, #13
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r4, [sp, #40]
	ldrsb	r3, [r1, r0]
	ldr	r2, [r4, #16]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r7, #8]
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	adds	r6, #1
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	adds	r5, #12
	cmp	r6, #6
	bne.n	.L_0817107e
	movs	r6, #0
.L_081710bc:
	lsls	r3, r6, #3
	ldr	r5, [sp, #36]
	adds	r3, r3, r6
	lsls	r3, r3, #2
	subs	r4, r5, r3
	cmp	r4, #36
	ble.n	.L_081710cc
	movs	r4, #36
.L_081710cc:
	adds	r0, r6, #1
	mov	r9, r0
	cmp	r4, #0
	blt.n	.L_08171170
	movs	r1, #0
	mov	sl, r1
	cmp	r4, #0
	beq.n	.L_0817116c
	lsls	r3, r6, #1
	adds	r3, r3, r6
	lsls	r3, r3, #2
	adds	r2, r0, #0
	mov	r8, r3
	ldr	r7, [sp, #28]
	movs	r3, #4
	str	r2, [sp, #32]
	add	r3, r8
	mov	fp, r3
.L_081710f0:
	ldr	r0, [sp, #32]
	movs	r1, #6
	str	r4, [sp, #8]
	bl	sub_08002064
	lsls	r5, r0, #1
	adds	r5, r5, r0
	lsls	r5, r5, #2
	mov	r0, r8
	ldr	r3, [r7, r0]
	ldr	r2, [r7, r5]
	movs	r1, #224
	subs	r2, r2, r3
	lsls	r1, r1, #3
	mov	r0, sl
	muls	r0, r2
	adds	r1, #28
	ldr	r2, [pc, #316]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x3504
	ldr	r2, [r7, r5]
	mov	r3, r8
	mov	r5, fp
	ldr	r6, [r7, r3]
	ldr	r3, [r7, r5]
	movs	r1, #224
	subs	r2, r2, r3
	lsls	r1, r1, #3
	adds	r6, r6, r0
	adds	r1, #28
	mov	r0, sl
	muls	r0, r2
	ldr	r2, [pc, #284]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x4946
	movs	r2, #8
	ldr	r3, [r7, r5]
	subs	r2, #2
	ldrh	r1, [r1, r2]
	ldr	r5, [sp, #48]
	adds	r3, r3, r0
	movs	r0, #2
	adds	r1, r5, r1
	subs	r6, r6, r0
	movs	r2, #4
	movs	r5, #8
	str	r2, [sp, #0]
	str	r5, [sp, #4]
	subs	r3, #4
	ldr	r0, [sp, #68]
	adds	r2, r6, #0
	ldr	r5, [sp, #56]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9c02
	movs	r0, #1
	add	sl, r0
	cmp	sl, r4
	bne.n	.L_081710f0
	b.n	.L_08171170
.L_0817116c:
	adds	r6, #1
	mov	r9, r6
.L_08171170:
	mov	r6, r9
	cmp	r6, #6
	bne.n	.L_081710bc
.L_08171176:
	ldr	r2, [sp, #64]
	ldr	r1, [sp, #20]
	adds	r3, r1, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	sl, r3
	cmp	r3, #64
	ble.n	.L_0817118c
	movs	r3, #64
	mov	sl, r3
.L_0817118c:
	mov	r4, sl
	movs	r6, #0
	cmp	r4, #0
	beq.n	.L_08171224
	ldr	r5, [sp, #24]
	add	r7, sp, #92
	movs	r0, #8
	mov	r8, r5
	mov	r9, r7
	mov	fp, r0
.L_081711a0:
	lsls	r5, r6, #10
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r1, [sp, #40]
	lsls	r2, r0, #2
	ldr	r3, [r1, #12]
	adds	r2, r2, r0
	lsls	r2, r2, #3
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r3, r3, r2
	mov	r4, r8
	str	r3, [r4, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r5, [sp, #40]
	lsls	r3, r0, #2
	ldr	r2, [r5, #16]
	adds	r3, r3, r0
	lsls	r3, r3, #3
	adds	r2, r2, r3
	mov	r0, r8
	mov	r1, r9
	str	r2, [r0, #8]
	bl	sub_0815e1ec
	mov	r1, r9
	ldr	r3, [r1, #0]
	asrs	r3, r3, #1
	str	r3, [r1, #0]
	ldr	r3, [r1, #8]
	cmp	r3, #159
	bgt.n	.L_081711ec
	movs	r3, #160
	str	r3, [r7, #8]
.L_081711ec:
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	ble.n	.L_081711f8
	str	r2, [r7, #8]
.L_081711f8:
	ldr	r2, [pc, #88]
	mov	r3, fp
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #48]
	movs	r3, #2
	adds	r1, r2, r1
	ldr	r2, [r7, #0]
	movs	r4, #4
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	mov	r5, fp
	str	r4, [sp, #0]
	subs	r3, #4
	str	r5, [sp, #4]
	ldr	r0, [sp, #68]
	ldr	r4, [sp, #60]
	adds	r6, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4556
	bne.n	.L_081711a0
.L_08171224:
	ldr	r5, [sp, #64]
	cmp	r5, #71
	bgt.n	.L_0817122c
	b.n	.L_081713b4
.L_0817122c:
	cmp	r5, #72
	bne.n	.L_081712ae
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #72]
	ldr	r3, [r0, #0]
	str	r3, [r1, #0]
	cmp	r3, #0
	ble.n	.L_0817125c
	ldr	r3, [pc, #24]
	str	r3, [r1, #12]
	b.n	.L_08171264
	movs	r0, r0
	.4byte 0xfff80000
	.4byte 0xfffffeb0
	.4byte 0x08196e9c
	.4byte 0x0300021c
	.4byte 0x08197410
	.2byte 0x0000
	.2byte 0xffe0
.L_0817125c:
	.2byte 0x9a12
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, #12]
.L_08171264:
	ldr	r3, [pc, #16]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [sp, #64]
	cmp	r3, #72
	bne.n	.L_081712ae
	b.n	.L_0817127c
	movs	r0, r0
	.2byte 0x0784
	.2byte 0x0000
.L_0817127c:
	ldr	r4, [sp, #72]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r3, r4, r5
	movs	r5, #8
	str	r5, [r3, #0]
	movs	r0, #145
	bl	sub_081180e8
	ldr	r2, [sp, #76]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #4
	bl	sub_08118088
	ldr	r4, [sp, #76]
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r2, #5
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_0814cd48
.L_081712ae:
	ldr	r0, [sp, #72]
	movs	r1, #132
	ldr	r3, [sp, #72]
	lsls	r1, r1, #6
	movs	r4, #184
	adds	r1, #32
	lsls	r4, r4, #5
	ldr	r7, [sp, #24]
	add	r5, sp, #80
	adds	r0, r0, r1
	movs	r2, #32
	adds	r3, r3, r4
	movs	r6, #0
	mov	r9, r0
	mov	r8, r2
	mov	sl, r3
	mov	fp, r5
.L_081712d0:
	ldr	r0, [sp, #72]
	ldr	r3, [r0, #12]
	adds	r2, r6, #0
	muls	r2, r3
	ldr	r3, [r0, #0]
	adds	r0, r7, #0
	adds	r3, r3, r2
	str	r3, [r7, #0]
	movs	r3, #240
	lsls	r3, r3, #13
	str	r3, [r7, #4]
	ldr	r1, [sp, #40]
	ldr	r3, [r1, #16]
	mov	r1, fp
	str	r3, [r7, #8]
	bl	sub_0815e1ec
	mov	r2, fp
	ldr	r3, [r2, #0]
	asrs	r2, r3, #1
	mov	r3, fp
	str	r2, [r3, #0]
	ldr	r4, [sp, #76]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0817130a
	adds	r3, r2, #0
	subs	r3, #32
	str	r3, [r5, #0]
.L_0817130a:
	cmp	r6, #0
	ble.n	.L_0817133a
	ldr	r3, [r5, #4]
	mov	r0, r8
	ldr	r2, [r5, #0]
	subs	r3, #32
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4640
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	mov	r1, r9
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe019
.L_0817133a:
	ldr	r0, [sp, #64]
	cmp	r0, #81
	bgt.n	sub_0817136e
	ldr	r3, [r5, #4]
	mov	r1, r8
	movs	r4, #36
	ldr	r2, [r5, #0]
	subs	r3, #36
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	mov	r1, sl
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4640
	movs	r1, #36
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r4, [sp, #60]
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #68]
	mov	r1, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x3601
	cmp	r6, #10
	bne.n	.L_081712d0
	ldr	r0, [sp, #72]
	movs	r1, #132
	ldr	r2, [sp, #64]
	lsls	r1, r1, #6
	adds	r1, #32
	adds	r5, r0, r1
	movs	r7, #0
	cmp	r2, #79
	ble.n	.L_0817138c
	ldr	r3, [sp, #12]
	ldr	r4, [pc, #152]
	adds	r7, r3, r4
.L_0817138c:
	movs	r6, #0
.L_0817138e:
	lsls	r0, r6, #9
	bl	sub_08002096
	lsls	r3, r0, #6
	subs	r3, r3, r0
	asrs	r3, r3, #16
	subs	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_081713a2
	movs	r3, #0
.L_081713a2:
	movs	r2, #0
.L_081713a4:
	adds	r2, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	cmp	r2, #32
	bne.n	.L_081713a4
	adds	r6, #1
	cmp	r6, #32
	bne.n	.L_0817138e
.L_081713b4:
	ldr	r5, [sp, #72]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #176
	adds	r2, r5, r0
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_081713c8
	movs	r3, #1
	str	r3, [r2, #0]
.L_081713c8:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r3, #240
	ldr	r1, [sp, #72]
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #12]
	ldr	r5, [sp, #64]
	adds	r4, #8
	adds	r5, #1
	str	r4, [sp, #12]
	str	r5, [sp, #64]
	cmp	r5, #88
	beq.n	.L_081713fa
	b.n	.L_08170fa6
.L_081713fa:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #32]
	bl	sub_08014644
	ldr	r0, [pc, #28]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #248
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffd80
	.4byte 0x08143001
	.2byte 0xc929
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #116
	str	r0, [sp, #68]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	movs	r7, #0
	str	r0, [sp, #64]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #60]
	ldr	r6, [r5, #100]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r2, [sp, #64]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r1, r2, r3
	ldr	r0, [pc, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #64]
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r1, r4, r2
	ldr	r0, [pc, #20]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	adds	r1, r6, #0
	movs	r2, #0
	b.n	.L_081714a0
	.4byte 0x00001010
	.4byte 0x000000c4
	.2byte 0x00c9
	.2byte 0x0000
.L_081714a0:
	movs	r3, #0
	ldr	r0, [pc, #468]
	bl	sub_08157cf4
	ldr	r0, [pc, #464]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #460]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b10
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #64]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #424]
	bl	sub_080145a8
	ldr	r5, [r5, #48]
	ldr	r1, [sp, #68]
	str	r5, [sp, #48]
	movs	r6, #2
	movs	r3, #54
	ldrsh	r2, [r5, r3]
	movs	r5, #0
	str	r2, [sp, #44]
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	bl	sub_08118098
	ldr	r2, [sp, #64]
	ldr	r0, [r0, #0]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, r2, r3
	movs	r4, #0
	str	r0, [sp, #40]
	str	r3, [sp, #36]
	str	r4, [sp, #52]
.L_0817150a:
	ldr	r1, [pc, #380]
	movs	r2, #128
	adds	r0, r7, r1
	lsls	r2, r2, #9
	adds	r1, r6, #0
	bl	sub_0815b434
	adds	r3, r5, #3
	muls	r3, r6
	ldr	r2, [sp, #36]
	adds	r6, #2
	strh	r7, [r5, r2]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r7, r7, r3
	ldr	r3, [sp, #52]
	adds	r5, #2
	adds	r3, #1
	str	r3, [sp, #52]
	cmp	r3, #32
	bne.n	.L_0817150a
	ldr	r0, [sp, #48]
	mov	r1, sp
	mov	r2, sp
	movs	r4, #0
	adds	r0, #12
	adds	r1, #72
	adds	r2, #104
	str	r4, [sp, #56]
	str	r0, [sp, #8]
	str	r1, [sp, #16]
	str	r2, [sp, #28]
.L_0817154c:
	ldr	r3, [sp, #56]
	cmp	r3, #64
	bne.n	.L_08171558
	movs	r0, #134
	bl	sub_081180e8
.L_08171558:
	ldr	r4, [sp, #56]
	cmp	r4, #31
	ble.n	.L_0817156c
	movs	r3, #3
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_0817156c
	ldr	r0, [pc, #292]
	bl	sub_0815f0a0
.L_0817156c:
	ldr	r0, [sp, #56]
	cmp	r0, #0
	bne.n	.L_08171648
	add	r1, sp, #44
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #48]
	strh	r1, [r2, #54]
	ldr	r3, [sp, #64]
	movs	r2, #0
	str	r2, [sp, #52]
	adds	r3, #24
	subs	r2, #1
.L_08171584:
	str	r2, [r3, #0]
	ldr	r4, [sp, #52]
	adds	r3, #28
	adds	r4, #1
	str	r4, [sp, #52]
	cmp	r4, #32
	bne.n	.L_08171584
	movs	r0, #0
	ldr	r1, [pc, #248]
	ldr	r2, [sp, #64]
	movs	r3, #224
	str	r0, [sp, #52]
	lsls	r3, r3, #2
	mov	r8, r1
	adds	r6, r2, r3
.L_081715a2:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r7, r3, #0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r7, #32
	adds	r3, r7, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r6, #0]
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	str	r3, [r6, #8]
	movs	r3, #220
	lsls	r3, r3, #15
	str	r3, [r6, #4]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r0, r5, #0
	adds	r7, r3, #4
	bl	sub_08002096
	ldr	r4, [sp, #40]
	adds	r2, r7, #0
	muls	r2, r0
	ldr	r3, [r4, #8]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r0, [sp, #40]
	mov	r1, r8
	ldr	r5, [r0, #16]
	ldr	r0, [r6, #12]
	adds	r5, r5, r3
	ldr	r3, [r6, #0]
	str	r5, [r6, #20]
	str	r1, [r6, #16]
	subs	r0, r0, r3
	movs	r1, #6
	bl	sub_08002054
	str	r0, [r6, #12]
	ldr	r0, [r6, #4]
	mov	r2, r8
	subs	r0, r2, r0
	movs	r1, #6
	bl	sub_08002054
	ldr	r3, [r6, #8]
	str	r0, [r6, #16]
	subs	r5, r5, r3
	adds	r0, r5, #0
	movs	r1, #6
	bl	sub_08002054
	str	r0, [r6, #20]
	ldr	r4, [sp, #52]
	negs	r3, r4
	lsls	r3, r3, #2
	adds	r4, #1
	str	r3, [r6, #24]
	adds	r6, #28
	str	r4, [sp, #52]
	cmp	r4, #32
	bne.n	.L_081715a2
.L_08171648:
	ldr	r3, [sp, #56]
	subs	r3, #20
	cmp	r3, #59
	bhi.n	.L_0817169e
	ldr	r0, [sp, #56]
	movs	r2, #128
	lsls	r2, r2, #1
	cmp	r0, #71
	ble.n	.L_08171664
	ldr	r1, [sp, #56]
	movs	r3, #160
	lsls	r2, r1, #5
	lsls	r3, r3, #4
	subs	r2, r3, r2
.L_08171664:
	ldr	r4, [sp, #68]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08171694
	ldr	r0, [sp, #48]
	ldrh	r3, [r0, #54]
	adds	r1, r0, #0
	adds	r3, r3, r2
	strh	r3, [r1, #54]
	b.n	.L_0817169e
	.4byte 0x00000134
	.4byte 0x00000149
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0x00000154
	.2byte 0x0000
	.2byte 0xfff8
.L_08171694:
	.2byte 0x9c0c
	ldrh	r3, [r4, #54]
	adds	r0, r4, #0
	subs	r3, r3, r2
	strh	r3, [r0, #54]
.L_0817169e:
	bl	sub_08014de4
	ldr	r1, [sp, #8]
	ldr	r0, [sp, #48]
	bl	sub_080156e8
	movs	r1, #0
	ldr	r2, [sp, #64]
	str	r1, [sp, #52]
	mov	r8, r2
.L_081716b2:
	mov	r3, r8
	ldr	r0, [r3, #24]
	cmp	r0, #15
	bls.n	.L_081716bc
	b.n	.L_081717c2
.L_081716bc:
	lsls	r0, r0, #11
	bl	sub_08002096
	mov	r4, r8
	ldr	r3, [r4, #12]
	muls	r3, r0
	asrs	r3, r3, #16
	lsls	r7, r3, #1
	ldr	r3, [r4, #24]
	subs	r5, r7, #2
	adds	r3, #1
	str	r3, [r4, #24]
	cmp	r5, #61
	bhi.n	.L_081717c2
	ldr	r1, [sp, #28]
	mov	r0, r8
	bl	sub_0815e1ec
	ldr	r0, [sp, #28]
	asrs	r5, r5, #1
	ldr	r3, [r0, #0]
	ldr	r1, [r0, #4]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	str	r1, [sp, #32]
	asrs	r3, r3, #1
	movs	r1, #19
	movs	r0, #188
	mov	fp, r3
	bl	sub_081963ec
	ldr	r2, [sp, #36]
	lsls	r5, r5, #1
	adds	r5, r5, r2
	asrs	r6, r7, #1
	ldr	r2, [sp, #32]
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	ldr	r4, [pc, #936]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	mov	r0, fp
	movs	r3, #192
	subs	r0, r0, r6
	lsls	r3, r3, #18
	mov	sl, r5
	mov	r9, r0
	subs	r5, r2, r7
	adds	r3, #188
	ldr	r0, [sp, #60]
	adds	r1, r1, r4
	mov	r2, r9
	ldr	r4, [r3, #0]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #23
	movs	r0, #188
	bl	sub_081963ec
	mov	r0, sl
	movs	r4, #0
	ldrsh	r1, [r0, r4]
	ldr	r2, [pc, #880]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r4, [r3, #0]
	adds	r1, r1, r2
	ldr	r0, [sp, #60]
	mov	r2, fp
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	ldr	r4, [sp, #52]
	cmp	r4, #15
	ble.n	.L_081717c2
	movs	r1, #27
	movs	r0, #188
	bl	sub_081963ec
	mov	r2, sl
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	ldr	r3, [pc, #828]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r0, #188
	ldr	r4, [r0, #0]
	mov	r2, r9
	adds	r1, r1, r3
	ldr	r0, [sp, #60]
	ldr	r3, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #31
	movs	r0, #188
	bl	sub_081963ec
	mov	r3, sl
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r4, [pc, #784]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r0, #188
	adds	r1, r1, r4
	mov	r2, fp
	ldr	r4, [r0, #0]
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_081717c2:
	ldr	r2, [sp, #52]
	movs	r1, #28
	adds	r2, #1
	add	r8, r1
	str	r2, [sp, #52]
	cmp	r2, #28
	beq.n	.L_081717d2
	b.n	.L_081716b2
.L_081717d2:
	movs	r0, #32
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	movs	r3, #0
	str	r3, [sp, #24]
	ldr	r2, [pc, #720]
	ldr	r3, [sp, #72]
	ldr	r4, [sp, #16]
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #712]
	mov	r1, r8
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	orrs	r3, r2
	str	r3, [sp, #72]
	ldr	r3, [pc, #704]
	str	r4, [r0, #16]
	str	r3, [r0, #8]
	str	r1, [r0, #12]
	ldr	r2, [sp, #24]
	movs	r3, #7
	str	r2, [r0, #20]
	str	r3, [r0, #0]
	ldr	r4, [sp, #64]
	mov	fp, r0
	ldr	r1, [sp, #16]
	movs	r0, #142
	lsls	r0, r0, #7
	adds	r3, r4, r0
	str	r3, [r1, #4]
	movs	r3, #224
	lsls	r3, r3, #1
	mov	r9, r4
	adds	r4, r4, r3
	movs	r2, #0
	mov	sl, r4
	ldr	r4, [sp, #64]
	movs	r0, #224
	str	r2, [sp, #52]
	lsls	r0, r0, #2
	adds	r5, r4, r0
.L_08171832:
	ldr	r2, [r5, #24]
	adds	r3, r2, #1
	str	r3, [r5, #24]
	cmp	r3, #17
	bls.n	.L_0817183e
	b.n	.L_0817199c
.L_0817183e:
	cmp	r3, #11
	ble.n	.L_0817189c
	ldr	r3, [r5, #12]
	subs	r2, #11
	muls	r2, r3
	ldr	r3, [r5, #0]
	add	r6, sp, #92
	adds	r1, r3, r2
	str	r1, [r6, #0]
	ldr	r3, [r5, #24]
	ldr	r2, [r5, #16]
	subs	r3, #12
	muls	r2, r3
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #24]
	ldr	r2, [r5, #20]
	subs	r3, #12
	muls	r2, r3
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r5, #24]
	cmp	r3, #16
	bne.n	.L_08171890
	mov	r2, sl
	str	r1, [r2, #0]
	ldr	r3, [r6, #4]
	str	r3, [r2, #4]
	ldr	r3, [r6, #8]
	str	r3, [r2, #8]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	mov	r4, sl
	str	r3, [r4, #12]
	movs	r3, #0
	str	r3, [r4, #24]
.L_08171890:
	ldr	r7, [sp, #28]
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_0815e1ec
	b.n	.L_081718a8
.L_0817189c:
	ldr	r7, [sp, #28]
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_0815e1ec
	add	r6, sp, #92
.L_081718a8:
	ldr	r3, [r7, #0]
	mov	r0, r8
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r2, r3, #2
	strh	r2, [r0, #0]
	ldr	r2, [r7, #4]
	mov	r1, r8
	adds	r3, #2
	mov	r4, r8
	strh	r2, [r1, #2]
	strh	r3, [r4, #8]
	strh	r2, [r0, #10]
	ldr	r2, [r5, #24]
	cmp	r2, #5
	bgt.n	.L_081718ee
	ldr	r3, [r5, #12]
	muls	r2, r3
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [r5, #24]
	ldr	r2, [r5, #16]
	muls	r2, r3
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #24]
	ldr	r2, [r5, #20]
	muls	r2, r3
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	b.n	.L_08171918
.L_081718ee:
	ldr	r2, [r5, #12]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [r5, #0]
	lsls	r3, r3, #1
	adds	r2, r2, r3
	str	r2, [r6, #0]
	ldr	r2, [r5, #16]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [r5, #4]
	lsls	r3, r3, #1
	adds	r2, r2, r3
	str	r2, [r6, #4]
	ldr	r2, [r5, #20]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [r5, #8]
	lsls	r3, r3, #1
	adds	r2, r2, r3
	str	r2, [r6, #8]
.L_08171918:
	ldr	r3, [r5, #24]
	cmp	r3, #6
	bne.n	.L_08171972
	ldr	r3, [r6, #0]
	mov	r1, r9
	str	r3, [r1, #0]
	ldr	r3, [r6, #4]
	str	r3, [r1, #4]
	ldr	r3, [r6, #8]
	str	r3, [r1, #8]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	mov	r2, r9
	adds	r3, #24
	str	r3, [r2, #12]
	movs	r3, #0
	str	r3, [r2, #24]
	ldr	r3, [sp, #64]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
	ldr	r2, [sp, #68]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #0
	bl	sub_08118088
	ldr	r4, [sp, #68]
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_08171972:
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r7, #0]
	mov	r0, r8
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r2, r3, #2
	strh	r2, [r0, #16]
	ldr	r2, [r7, #4]
	mov	r1, r8
	adds	r3, #2
	mov	r4, r8
	strh	r2, [r1, #18]
	strh	r3, [r4, #24]
	strh	r2, [r0, #26]
	mov	r0, fp
	bl	sub_08196a7c
.L_0817199c:
	ldr	r2, [sp, #52]
	movs	r1, #28
	adds	r2, #1
	add	r9, r1
	add	sl, r1
	adds	r5, #28
	str	r2, [sp, #52]
	cmp	r2, #14
	beq.n	.L_081719b0
	b.n	.L_08171832
.L_081719b0:
	ldr	r3, [sp, #56]
	subs	r3, #16
	cmp	r3, #63
	bls.n	.L_081719ba
	b.n	.L_08171b16
.L_081719ba:
	ldr	r4, [sp, #68]
	mov	r1, sp
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	adds	r1, #80
	str	r1, [sp, #20]
	bl	sub_0815e21c
	ldr	r3, [sp, #16]
	movs	r2, #5
	strb	r2, [r3, #0]
	add	r3, sp, #72
	str	r3, [sp, #16]
	strb	r2, [r3, #1]
	ldr	r4, [sp, #64]
	ldr	r1, [sp, #16]
	movs	r0, #184
	lsls	r0, r0, #5
	adds	r3, r4, r0
	str	r3, [r1, #4]
	mov	r2, fp
	movs	r3, #6
	str	r3, [r2, #0]
	ldr	r3, [pc, #212]
	ldr	r0, [pc, #216]
	str	r3, [r2, #8]
	ldr	r4, [sp, #56]
	movs	r3, #0
	lsls	r4, r4, #8
	str	r3, [sp, #52]
	str	r4, [sp, #12]
	ldr	r6, [pc, #204]
	mov	r9, r0
.L_081719fc:
	ldr	r1, [sp, #12]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r0, r1, r2
	bl	sub_08002096
	ldr	r4, [sp, #48]
	adds	r5, r0, #0
	movs	r3, #54
	ldrsh	r0, [r4, r3]
	bl	sub_08002096
	adds	r1, r0, #0
	ldr	r0, [sp, #20]
	ldr	r4, [sp, #52]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r3, #64
	mov	r2, r9
	lsls	r0, r3, #16
	ldrsb	r3, [r2, r4]
	muls	r3, r5
	cmp	r3, #0
	bge.n	.L_08171a38
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_08171a38:
	asrs	r3, r3, #16
	muls	r3, r1
	ldr	r4, [sp, #48]
	subs	r0, r0, r3
	mov	sl, r0
	movs	r3, #54
	ldrsh	r0, [r4, r3]
	bl	sub_08002090
	ldr	r1, [sp, #52]
	adds	r2, r0, #0
	mov	r0, r9
	ldrsb	r3, [r0, r1]
	adds	r0, r5, #0
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L_08171a62
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_08171a62:
	asrs	r3, r0, #16
	muls	r3, r2
	ldr	r1, [sp, #56]
	negs	r7, r3
	cmp	r1, #47
	bgt.n	.L_08171a7a
	ldr	r2, [pc, #92]
	lsls	r0, r1, #9
	adds	r0, r0, r2
	bl	sub_08002096
	b.n	.L_08171a7e
.L_08171a7a:
	movs	r0, #128
	lsls	r0, r0, #9
.L_08171a7e:
	ldr	r4, [sp, #52]
	ldr	r2, [pc, #76]
	lsls	r3, r4, #2
	ldr	r3, [r2, r3]
	muls	r3, r0
	ldr	r0, [sp, #56]
	lsrs	r5, r3, #16
	cmp	r0, #63
	ble.n	.L_08171aa2
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	lsls	r2, r0, #3
	movs	r1, #128
	subs	r3, r3, r2
	lsls	r1, r1, #2
	adds	r1, r3, r1
	str	r1, [sp, #24]
	b.n	.L_08171ada
.L_08171aa2:
	ldr	r2, [sp, #56]
	cmp	r2, #23
	bgt.n	.L_08171ad4
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	subs	r3, #192
	b.n	.L_08171ad8
	.4byte 0x02010000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199244
	.4byte 0x08198c10
	.4byte 0x08198c0c
	.4byte 0xffffe000
	.2byte 0x8bfc
	.2byte 0x0819
.L_08171ad4:
	movs	r3, #0
	ldrsb	r3, [r6, r3]
.L_08171ad8:
	str	r3, [sp, #24]
.L_08171ada:
	ldr	r4, [sp, #24]
	mov	r0, fp
	str	r4, [r0, #20]
	bl	sub_08014de4
	mov	r0, sl
	adds	r1, r7, #0
	movs	r2, #0
	bl	sub_08015160
	lsls	r1, r5, #1
	adds	r0, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	mov	r1, r8
	ldr	r0, [pc, #112]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, fp
	bl	sub_08196a7c
	ldr	r1, [sp, #52]
	adds	r6, #1
	adds	r1, #1
	str	r1, [sp, #52]
	cmp	r1, #4
	beq.n	.L_08171b16
	b.n	.L_081719fc
.L_08171b16:
	mov	r0, fp
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #64]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #56]
	adds	r0, #1
	str	r0, [sp, #56]
	cmp	r0, #74
	beq.n	.L_08171b4e
	b.n	.L_0817154c
.L_08171b4e:
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081991e0
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_08171bb0
	pop	{pc}
	.2byte 0x0000
