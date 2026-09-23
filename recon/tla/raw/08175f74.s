.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
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
	.set sub_08014c4c, 0x08014c4c
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118010, 0x08118010
	.set sub_08118040, 0x08118040
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081180f0, 0x081180f0
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158d68, 0x08158d68
	.set sub_0815b290, 0x0815b290
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815b410, 0x0815b410
	.set sub_0815b434, 0x0815b434
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815f000, 0x0815f000
	.set sub_08163c2c, 0x08163c2c
	.set sub_08164b2c, 0x08164b2c
	.set sub_08176642, 0x08176642
	.set sub_08176664, 0x08176664
	.set sub_08176c8c, 0x08176c8c
	.set sub_081771e0, 0x081771e0
	.set sub_081773b6, 0x081773b6
	.set sub_081776e4, 0x081776e4
	.set sub_081776f8, 0x081776f8
	.set sub_081784a6, 0x081784a6
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08175f74
	.thumb_func
Func_08175f74:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #324
	str	r0, [sp, #100]
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r0, [r2, #96]
	adds	r3, r2, #0
	str	r0, [sp, #96]
	adds	r3, #176
	ldr	r1, [r2, #92]
	movs	r0, #0
	str	r1, [sp, #92]
	ldr	r6, [pc, #100]
	ldr	r3, [r3, #0]
	mov	fp, r6
	str	r3, [sp, #88]
	ldr	r3, [pc, #96]
	movs	r6, #1
	ldrh	r4, [r3, #4]
	mov	sl, r3
	str	r4, [sp, #76]
	ldr	r5, [r2, #100]
	str	r5, [sp, #72]
	movs	r5, #240
	ldr	r2, [r2, #36]
	str	r2, [sp, #68]
	bl	sub_081435e0
	bl	sub_0813ba50
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r7, [sp, #92]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r3, r7, r0
	movs	r1, #0
	str	r1, [r3, #0]
	mov	r8, r1
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #32]
	bl	sub_080145a8
	movs	r0, #1
	movs	r1, #0
	bl	sub_08163c2c
	ldr	r3, [pc, #24]
	mov	r4, sl
	b.n	.L_08176010
	.4byte 0x00000784
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x03001120
	.4byte 0x08143001
	.2byte 0x11e0
	.2byte 0x0300
.L_08176010:
	str	r5, [r3, #16]
	ldr	r2, [sp, #88]
	mov	r3, r8
	add	r0, sp, #264
	strh	r3, [r4, #4]
	movs	r3, #255
	movs	r1, #0
	str	r6, [r2, #16]
	strh	r3, [r0, #0]
	bl	sub_08118010
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r5, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08014c4c
	movs	r7, #237
	ldr	r5, [sp, #68]
	lsls	r7, r7, #3
	adds	r7, #255
	adds	r3, r5, r7
	strb	r6, [r3, #0]
	ldr	r1, [pc, #24]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	movs	r2, #20
	negs	r2, r2
	adds	r1, r2, #0
	movs	r0, #0
	b.n	.L_0817606c
	movs	r0, r0
	.4byte 0x00002737
	.2byte 0x0075
	.2byte 0x0000
.L_0817606c:
	bl	sub_08164b2c
	movs	r0, #1
	movs	r1, #1
	bl	sub_08163c2c
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #48
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	mov	r1, sp
	adds	r3, #40
	mov	r0, r8
	str	r0, [r3, #0]
	adds	r1, #176
	str	r1, [sp, #64]
	bl	sub_08144aac
	ldr	r0, [pc, #24]
	bl	sub_08013300
	movs	r2, #160
	adds	r7, r0, #0
	b.n	.L_081760c0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x00000080
	.2byte 0x009c
	.2byte 0x0000
.L_081760c0:
	adds	r1, r7, #0
	ldr	r3, [pc, #540]
	lsls	r2, r2, #1
	ldr	r0, [pc, #540]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b17
	movs	r2, #160
	movs	r4, #224
	lsls	r2, r2, #1
	lsls	r4, r4, #3
	adds	r7, r7, r2
	adds	r5, r3, r4
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	sub_0801587c
	movs	r6, #0
	movs	r1, #128
	mov	r9, r6
	lsls	r1, r1, #5
.L_081760ea:
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081760f6
	adds	r3, #32
	strb	r3, [r5, #0]
.L_081760f6:
	movs	r7, #1
	add	r9, r7
	adds	r5, #1
	cmp	r9, r1
	bne.n	.L_081760ea
	ldr	r2, [pc, #484]
	movs	r1, #64
	movs	r3, #0
	movs	r0, #64
	bl	sub_0815b290
	movs	r2, #238
	ldr	r1, [sp, #92]
	ldrb	r3, [r0, #9]
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r1, r1, r2
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldr	r3, [pc, #460]
	str	r0, [r1, #0]
	mov	sl, r3
	ldrb	r3, [r0, #16]
	ldr	r5, [sp, #92]
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r4, [pc, #448]
	ldrh	r0, [r3, #2]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r6, r5, r7
	movs	r2, #128
	adds	r0, r0, r4
	ldr	r5, [pc, #420]
	mov	r8, r1
	lsls	r2, r2, #5
	adds	r1, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4640
	ldr	r2, [r0, #0]
	movs	r3, #13
	ldrb	r1, [r2, #5]
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r2, #5]
	movs	r3, #32
	strb	r3, [r2, #23]
	ldr	r0, [pc, #408]
	bl	sub_08013300
	adds	r7, r0, #0
	adds	r1, r7, #0
	movs	r2, #32
	adds	r7, #32
	ldr	r0, [pc, #400]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c38
	adds	r1, r6, #0
	bl	sub_0801587c
	movs	r2, #146
	lsls	r2, r2, #1
	add	r2, sp
	str	r2, [sp, #24]
	ldr	r7, [pc, #380]
	movs	r1, #0
	mov	r8, r5
	mov	r9, r1
	adds	r5, r2, #0
.L_08176186:
	movs	r3, #240
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	movs	r0, #16
	bl	sub_0815b290
	movs	r4, #13
	ldrb	r2, [r0, #9]
	negs	r4, r4
	adds	r3, r4, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldrb	r3, [r0, #16]
	str	r0, [r7, #0]
	lsls	r3, r3, #2
	add	r3, sl
	strb	r2, [r0, #9]
	ldr	r1, [pc, #320]
	ldrh	r0, [r3, #2]
	movs	r2, #32
	adds	r0, r0, r1
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0xcf08
	movs	r2, #1
	ldrh	r3, [r3, #8]
	add	r9, r2
	lsls	r3, r3, #22
	lsrs	r3, r3, #22
	strh	r3, [r5, #0]
	mov	r3, r9
	adds	r5, #2
	adds	r6, #32
	cmp	r3, #16
	bne.n	.L_08176186
	ldr	r6, [pc, #268]
	ldr	r5, [pc, #296]
	movs	r4, #0
	mov	r9, r4
.L_081761da:
	movs	r3, #240
	movs	r1, #8
	movs	r2, #0
	lsls	r3, r3, #8
	movs	r0, #8
	bl	sub_0815b3b0
	mov	r7, fp
	stmia	r5!, {r0}
	ldr	r1, [r7, #0]
	movs	r2, #24
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2001
	add	r9, r0
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_081761da
	ldr	r5, [pc, #260]
	ldr	r1, [sp, #72]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #92]
	movs	r6, #224
	lsls	r6, r6, #3
	adds	r0, r5, #0
	adds	r1, r2, r6
	movs	r3, #0
	movs	r2, #0
	bl	sub_08157cf4
	ldr	r4, [pc, #232]
	movs	r3, #0
	str	r3, [sp, #84]
	movs	r5, #32
	mov	ip, r4
	movs	r7, #6
	mov	lr, r5
	movs	r4, #0
	movs	r0, #0
.L_08176230:
	ldr	r3, [sp, #72]
	movs	r1, #0
	lsls	r2, r4, #1
	mov	r9, r1
	adds	r2, r2, r3
	adds	r1, r0, #0
.L_0817623c:
	mov	r5, ip
	ldrh	r3, [r5, r7]
	ldr	r5, [sp, #92]
	add	r3, r9
	adds	r3, r3, r6
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_08176254
	subs	r3, r3, r1
	cmp	r3, #0
	bgt.n	.L_08176254
	movs	r3, #1
.L_08176254:
	strb	r3, [r2, #0]
	movs	r3, #1
	add	r9, r3
	adds	r2, #1
	cmp	r9, lr
	bne.n	.L_0817623c
	ldr	r5, [sp, #84]
	adds	r4, #16
	adds	r5, #1
	adds	r0, #7
	str	r5, [sp, #84]
	cmp	r5, #10
	bne.n	.L_08176230
	ldr	r6, [sp, #92]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r6, r7
	ldr	r0, [pc, #148]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r1, r6, r2
	movs	r3, #1
	movs	r2, #1
	ldr	r0, [pc, #132]
	bl	sub_08157cf4
	ldr	r0, [pc, #128]
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r5, [pc, #64]
	lsls	r0, r0, #19
	adds	r1, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4b0d
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	movs	r3, #128
	adds	r2, #28
	lsls	r3, r3, #4
	str	r3, [r2, #0]
	movs	r3, #0
	str	r3, [sp, #60]
	str	r3, [sp, #56]
	str	r3, [sp, #80]
	ldr	r3, [pc, #84]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081762d0
	bl	.L_08177932
.L_081762d0:
	ldr	r5, [pc, #72]
	movs	r4, #0
	str	r4, [sp, #20]
	str	r5, [sp, #16]
	b.n	.L_08176320
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03000730
	.4byte 0x05000240
	.4byte 0xc0002000
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x0000009e
	.4byte 0x050003e0
	.4byte 0x02010000
	.4byte 0x02010040
	.4byte 0x00000134
	.4byte 0x08197410
	.4byte 0x000000b4
	.4byte 0x000000b6
	.4byte 0x000000bd
	.4byte 0x03001150
	.2byte 0x3e18
	.2byte 0x0201
.L_08176320:
	ldr	r0, [sp, #80]
	subs	r0, #32
	cmp	r0, #21
	bhi.n	.L_08176336
	ldr	r2, [sp, #80]
	movs	r1, #20
	negs	r0, r0
	subs	r2, #52
	negs	r1, r1
	bl	sub_08164b2c
.L_08176336:
	ldr	r6, [sp, #80]
	cmp	r6, #55
	bne.n	.L_0817634a
	movs	r1, #10
	movs	r2, #20
	movs	r0, #0
	negs	r1, r1
	negs	r2, r2
	bl	sub_08164b2c
.L_0817634a:
	ldr	r7, [sp, #80]
	cmp	r7, #4
	bne.n	.L_08176356
	movs	r0, #107
	bl	sub_081c0010
.L_08176356:
	ldr	r0, [sp, #80]
	cmp	r0, #55
	bne.n	.L_08176362
	movs	r0, #208
	bl	sub_081c0010
.L_08176362:
	ldr	r1, [sp, #80]
	cmp	r1, #78
	bne.n	.L_0817636e
	movs	r0, #219
	bl	sub_081c0010
.L_0817636e:
	ldr	r2, [sp, #80]
	cmp	r2, #127
	bne.n	.L_0817637a
	movs	r0, #212
	bl	sub_081c0010
.L_0817637a:
	ldr	r3, [sp, #80]
	cmp	r3, #131
	bne.n	.L_08176386
	movs	r0, #149
	bl	sub_081c0010
.L_08176386:
	ldr	r4, [sp, #80]
	cmp	r4, #206
	bne.n	.L_08176392
	movs	r0, #142
	bl	sub_081c0010
.L_08176392:
	ldr	r5, [sp, #80]
	cmp	r5, #244
	bne.n	.L_0817639e
	movs	r0, #212
	bl	sub_081c0010
.L_0817639e:
	ldr	r6, [sp, #80]
	movs	r7, #151
	lsls	r7, r7, #1
	cmp	r6, r7
	bne.n	.L_081763ae
	movs	r0, #212
	bl	sub_081c0010
.L_081763ae:
	ldr	r0, [sp, #80]
	movs	r1, #154
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_081763be
	movs	r0, #212
	bl	sub_081c0010
.L_081763be:
	ldr	r2, [sp, #80]
	movs	r3, #156
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_081763ce
	movs	r0, #212
	bl	sub_081c0010
.L_081763ce:
	ldr	r4, [sp, #80]
	movs	r5, #175
	lsls	r5, r5, #1
	cmp	r4, r5
	bne.n	.L_081763de
	movs	r0, #104
	bl	sub_081c0010
.L_081763de:
	ldr	r6, [sp, #80]
	cmp	r6, #0
	bne.n	.L_08176456
	ldr	r7, [sp, #92]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #75
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [pc, #40]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r2, #0
	mov	r9, r2
	movs	r5, #240
	ldr	r4, [pc, #28]
	ldr	r0, [pc, #32]
	ldr	r1, [pc, #32]
	ldr	r2, [pc, #36]
	lsls	r5, r5, #16
	adds	r5, #160
.L_08176418:
	str	r5, [r2, #0]
	ldr	r3, [r0, #0]
	adds	r0, #28
	cmp	r3, #0
	bge.n	.L_0817643c
	adds	r3, #15
	b.n	.L_0817643c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000f000
	.4byte 0x02014218
	.4byte 0x02014004
	.2byte 0x4000
	.2byte 0x0201
.L_0817643c:
	ldr	r6, [sp, #24]
	asrs	r3, r3, #4
	lsls	r3, r3, #1
	ldrh	r3, [r6, r3]
	movs	r7, #1
	orrs	r3, r4
	add	r9, r7
	str	r3, [r1, #0]
	mov	r3, r9
	adds	r1, #8
	adds	r2, #8
	cmp	r3, #64
	bne.n	.L_08176418
.L_08176456:
	ldr	r4, [sp, #80]
	subs	r4, #132
	mov	fp, r4
	cmp	r4, #107
	bls.n	.L_08176462
	b.n	.L_081765b2
.L_08176462:
	ldr	r3, [pc, #244]
	movs	r5, #168
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	add	r5, sp
	str	r3, [sp, #168]
	str	r4, [sp, #172]
	movs	r3, #160
	lsls	r3, r3, #9
	str	r3, [r5, #4]
	ldr	r6, [sp, #80]
	mov	r8, r5
	str	r3, [sp, #168]
	cmp	r6, #132
	bne.n	.L_081764d4
	ldr	r5, [pc, #216]
	movs	r7, #0
	mov	r9, r7
	movs	r6, #255
.L_08176488:
	bl	sub_08014878
	movs	r1, #160
	bl	sub_0800206c
	adds	r0, #40
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #24]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #9
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	negs	r0, r0
	subs	r0, #128
	lsls	r0, r0, #8
	str	r0, [r5, #16]
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r5, #28
	cmp	r1, #128
	bne.n	.L_08176488
.L_081764d4:
	ldr	r7, [pc, #136]
	ldr	r6, [pc, #140]
	movs	r2, #0
	movs	r4, #128
	mov	r9, r2
	add	r5, sp, #248
	lsls	r4, r4, #9
.L_081764e2:
	mov	r3, r9
	cmp	r3, #10
	bne.n	.L_081764ee
	mov	r0, r8
	str	r4, [r0, #4]
	str	r4, [sp, #168]
.L_081764ee:
	movs	r3, #0
	str	r3, [r5, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	movs	r1, #128
	ldr	r3, [r6, #0]
	lsls	r1, r1, #14
	str	r3, [r5, #0]
	ldr	r0, [r7, #0]
	ldr	r3, [r6, #4]
	mov	r2, r8
	adds	r3, r3, r1
	str	r3, [r5, #8]
	adds	r1, r5, #0
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	ldr	r3, [r6, #24]
	ldr	r0, [r7, #0]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bge.n	.L_08176520
	adds	r3, #15
.L_08176520:
	ldr	r2, [sp, #24]
	asrs	r3, r3, #4
	lsls	r3, r3, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r0, #8]
	adds	r7, #4
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	adds	r6, #28
	cmp	r0, #48
	bne.n	.L_081764e2
	movs	r1, #0
	mov	r9, r1
	ldr	r4, [pc, #28]
	ldr	r1, [pc, #16]
	b.n	.L_0817656c
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x08196ea8
	.4byte 0x02014200
	.4byte 0x02010000
	.4byte 0x02014900
	.2byte 0x0000
	.2byte 0xfff0
.L_0817656c:
	.2byte 0x6848
	cmp	r0, r4
	ble.n	.L_0817658a
	ldr	r2, [r1, #0]
	cmp	r2, r4
	ble.n	.L_0817658a
	ldr	r3, [pc, #120]
	cmp	r2, r3
	bgt.n	.L_0817658a
	ldr	r3, [r1, #12]
	adds	r3, r2, r3
	str	r3, [r1, #0]
	ldr	r3, [r1, #16]
	adds	r3, r0, r3
	str	r3, [r1, #4]
.L_0817658a:
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L_08176592
	adds	r2, #15
.L_08176592:
	ldr	r3, [r1, #24]
	asrs	r2, r2, #4
	adds	r2, r3, r2
	adds	r3, r2, #2
	str	r3, [r1, #24]
	cmp	r3, #255
	ble.n	.L_081765a6
	adds	r3, r2, #0
	subs	r3, #254
	str	r3, [r1, #24]
.L_081765a6:
	movs	r5, #1
	add	r9, r5
	mov	r6, r9
	adds	r1, #28
	cmp	r6, #128
	bne.n	.L_0817656c
.L_081765b2:
	ldr	r7, [sp, #80]
	subs	r7, #56
	str	r7, [sp, #52]
	cmp	r7, #75
	bhi.n	.L_08176614
	ldr	r3, [pc, #56]
	lsls	r0, r7, #7
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #160]
	str	r4, [sp, #164]
	bl	sub_08002096
	movs	r1, #128
	lsls	r1, r1, #7
	lsls	r0, r0, #1
	adds	r0, r0, r1
	movs	r3, #0
	add	r1, sp, #232
	add	r2, sp, #160
	str	r0, [r2, #4]
	str	r0, [sp, #160]
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	movs	r3, #228
	lsls	r3, r3, #15
	str	r3, [r1, #0]
	cmp	r7, #0
	blt.n	.L_081765fc
	movs	r3, #156
	b.n	.L_081765fe
	.4byte 0x00ffffff
	.2byte 0x6eb0
	.2byte 0x0819
.L_081765fc:
	movs	r3, #200
.L_081765fe:
	lsls	r3, r3, #15
	str	r3, [r1, #8]
	ldr	r4, [sp, #92]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #220
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	movs	r3, #0
	bl	sub_08020010
.L_08176614:
	ldr	r6, [sp, #80]
	cmp	r6, #131
	bne.n	sub_08176642
	movs	r0, #1
	ldr	r1, [pc, #124]
	movs	r2, #0
	bl	sub_08118040
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [pc, #112]
	adds	r0, #192
	ldr	r7, [pc, #112]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	ldr	r2, [pc, #104]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4658
	cmp	r0, #14
	bhi.n	sub_08176664
	ldr	r1, [sp, #80]
	ldr	r0, [sp, #96]
	lsls	r3, r1, #8
	adds	r3, r3, r1
	lsls	r2, r3, #16
	adds	r3, r3, r2
	ldr	r2, [pc, #84]
	lsls	r3, r3, #2
	movs	r1, #240
	subs	r2, r2, r3
	lsls	r1, r1, #6
	ldr	r3, [pc, #68]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c14
	movs	r3, #3
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08176670
	b.n	.L_08176794
.L_08176670:
	mov	r5, fp
	cmp	r5, #107
	bls.n	.L_08176678
	b.n	.L_08176816
.L_08176678:
	ldr	r0, [pc, #32]
	bl	sub_08013300
	movs	r6, #160
	mov	lr, r0
	ldr	r0, [pc, #20]
	lsls	r6, r6, #19
	adds	r6, #192
	movs	r7, #0
	movs	r1, #31
	mov	r8, r6
	mov	r9, r7
	mov	ip, r0
	mov	sl, r1
	b.n	.L_081766b0
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x00000075
	.4byte 0x7fff7fff
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0x514f
	.2byte 0x5151
.L_081766b0:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r5, [sp, #80]
	lsls	r3, r2, #16
	mov	r4, ip
	lsrs	r0, r3, #26
	lsrs	r6, r3, #21
	mov	r7, sl
	ands	r0, r4
	ands	r6, r4
	ands	r7, r2
	cmp	r5, #167
	bgt.n	.L_081766e0
	mov	r1, lr
	ldrh	r2, [r1, #0]
	lsls	r3, r2, #16
	lsrs	r5, r3, #26
	ands	r5, r4
	lsrs	r4, r3, #21
	mov	r3, ip
	ands	r4, r3
	mov	r3, sl
	ands	r3, r2
	b.n	.L_08176700
.L_081766e0:
	mov	r4, lr
	ldrh	r1, [r4, #0]
	mov	r5, ip
	lsls	r2, r1, #16
	lsrs	r3, r2, #26
	ands	r3, r5
	adds	r5, r3, #0
	lsrs	r2, r2, #21
	mov	r3, ip
	ands	r2, r3
	mov	r3, sl
	adds	r4, r2, #0
	ands	r3, r1
	subs	r5, #16
	subs	r4, #16
	adds	r3, #8
.L_08176700:
	cmp	r5, #0
	bge.n	.L_08176706
	movs	r5, #0
.L_08176706:
	cmp	r4, #0
	bge.n	.L_0817670c
	movs	r4, #0
.L_0817670c:
	cmp	r3, #31
	ble.n	.L_08176712
	movs	r3, #31
.L_08176712:
	cmp	r0, r5
	ble.n	.L_08176718
	subs	r0, #1
.L_08176718:
	cmp	r6, r4
	ble.n	.L_0817671e
	subs	r6, #1
.L_0817671e:
	cmp	r7, r3
	ble.n	.L_08176724
	subs	r7, #1
.L_08176724:
	lsls	r2, r6, #5
	lsls	r3, r0, #10
	movs	r5, #1
	orrs	r3, r2
	add	r9, r5
	orrs	r3, r7
	mov	r4, r8
	movs	r6, #2
	mov	r7, r9
	strh	r3, [r4, #0]
	add	lr, r6
	add	r8, r6
	cmp	r7, #128
	bne.n	.L_081766b0
	movs	r0, #0
	mov	r9, r0
	ldr	r0, [pc, #112]
	movs	r4, #31
.L_08176748:
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_08176752
	ldr	r3, [sp, #80]
	subs	r3, #129
.L_08176752:
	asrs	r3, r3, #2
	subs	r1, r4, r3
	adds	r2, r1, #0
	adds	r3, r1, #0
	cmp	r1, #31
	ble.n	.L_08176760
	movs	r1, #31
.L_08176760:
	cmp	r1, #11
	bgt.n	.L_08176766
	movs	r1, #12
.L_08176766:
	cmp	r2, #31
	ble.n	.L_0817676c
	movs	r2, #31
.L_0817676c:
	cmp	r2, #3
	bgt.n	.L_08176772
	movs	r2, #4
.L_08176772:
	cmp	r3, #31
	ble.n	.L_08176778
	movs	r3, #31
.L_08176778:
	cmp	r3, #3
	bgt.n	.L_0817677e
	movs	r3, #4
.L_0817677e:
	lsls	r2, r2, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	orrs	r3, r1
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r2, #15
	bne.n	.L_08176748
.L_08176794:
	mov	r3, fp
	cmp	r3, #107
	bhi.n	.L_08176816
	ldr	r0, [pc, #32]
	bl	sub_08013300
	ldr	r6, [pc, #16]
	ldr	r4, [pc, #28]
	movs	r5, #0
	movs	r7, #31
	mov	lr, r4
	mov	r9, r5
	mov	ip, r6
	mov	r8, r7
	b.n	.L_081767c4
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x050003c2
	.4byte 0x0000009b
	.2byte 0x0200
	.2byte 0x0500
.L_081767c4:
	mov	r1, lr
	ldrh	r2, [r1, #0]
	ldrh	r1, [r0, #0]
	lsls	r3, r2, #16
	lsrs	r7, r3, #26
	lsrs	r5, r3, #21
	mov	r6, r8
	lsls	r3, r1, #16
	mov	r4, ip
	ands	r6, r2
	lsrs	r2, r3, #26
	lsrs	r3, r3, #21
	ands	r7, r4
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	mov	r4, r8
	ands	r4, r1
	cmp	r7, r2
	ble.n	.L_081767ee
	subs	r7, #1
.L_081767ee:
	cmp	r5, r3
	ble.n	.L_081767f4
	subs	r5, #1
.L_081767f4:
	cmp	r6, r4
	ble.n	.L_081767fa
	subs	r6, #1
.L_081767fa:
	lsls	r3, r7, #10
	lsls	r2, r5, #5
	orrs	r3, r2
	orrs	r3, r6
	movs	r6, #1
	add	r9, r6
	mov	r5, lr
	movs	r7, #2
	mov	r1, r9
	strh	r3, [r5, #0]
	adds	r0, #2
	add	lr, r7
	cmp	r1, #192
	bne.n	.L_081767c4
.L_08176816:
	ldr	r2, [sp, #80]
	movs	r1, #0
	subs	r2, #214
	cmp	r2, #31
	bhi.n	.L_0817683c
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_0817682a
	ldr	r3, [sp, #80]
	subs	r3, #211
.L_0817682a:
	asrs	r2, r3, #2
	lsrs	r3, r3, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	str	r3, [sp, #48]
	movs	r3, #0
	str	r3, [sp, #44]
	str	r2, [sp, #40]
	movs	r1, #1
.L_0817683c:
	ldr	r2, [sp, #80]
	subs	r2, #251
	cmp	r2, #31
	bhi.n	.L_0817686c
	lsrs	r3, r2, #31
	adds	r0, r2, r3
	asrs	r1, r0, #1
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_08176852
	adds	r2, r1, #3
.L_08176852:
	asrs	r2, r2, #2
	movs	r3, #4
	subs	r3, r3, r2
	str	r3, [sp, #48]
	lsrs	r3, r0, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	movs	r2, #8
	negs	r4, r3
	subs	r2, r2, r3
	str	r4, [sp, #44]
	str	r2, [sp, #40]
	movs	r1, #1
.L_0817686c:
	ldr	r5, [sp, #80]
	ldr	r6, [pc, #108]
	adds	r3, r5, r6
	cmp	r3, #63
	bhi.n	.L_0817689c
	cmp	r3, #0
	bge.n	.L_0817687e
	ldr	r7, [pc, #100]
	adds	r3, r5, r7
.L_0817687e:
	asrs	r0, r3, #2
	lsrs	r3, r3, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r3, #8
	str	r3, [sp, #44]
	str	r0, [sp, #48]
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_08176894
	adds	r3, #7
.L_08176894:
	asrs	r3, r3, #3
	subs	r3, #4
	str	r3, [sp, #40]
	movs	r1, #1
.L_0817689c:
	cmp	r1, #1
	bne.n	.L_0817691c
	ldr	r0, [pc, #64]
	bl	sub_08013300
	ldr	r5, [pc, #48]
	ldr	r4, [pc, #60]
	movs	r1, #0
	mov	r9, r1
.L_081768ae:
	ldrh	r3, [r0, #0]
	ldr	r6, [sp, #48]
	movs	r2, #31
	ands	r2, r3
	adds	r1, r2, r6
	lsls	r3, r3, #16
	ldr	r7, [sp, #44]
	ldr	r6, [sp, #40]
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r5
	ands	r3, r5
	adds	r2, r2, r7
	adds	r3, r3, r6
	cmp	r1, #31
	ble.n	.L_081768d0
	movs	r1, #31
.L_081768d0:
	cmp	r2, #31
	ble.n	.L_081768ec
	movs	r2, #31
	b.n	.L_081768ec
	.4byte 0x0000001f
	.4byte 0xfffffea4
	.4byte 0xfffffea7
	.4byte 0x0000009b
	.2byte 0x0200
	.2byte 0x0500
.L_081768ec:
	cmp	r3, #31
	ble.n	.L_081768f2
	movs	r3, #31
.L_081768f2:
	cmp	r1, #0
	bge.n	.L_081768f8
	movs	r1, #0
.L_081768f8:
	cmp	r2, #0
	bge.n	.L_081768fe
	movs	r2, #0
.L_081768fe:
	cmp	r3, #0
	bge.n	.L_08176904
	movs	r3, #0
.L_08176904:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	movs	r7, #1
	orrs	r3, r2
	add	r9, r7
	orrs	r3, r1
	mov	r1, r9
	strh	r3, [r4, #0]
	adds	r0, #2
	adds	r4, #2
	cmp	r1, #192
	bne.n	.L_081768ae
.L_0817691c:
	movs	r2, #236
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	fp, r2
	bls.n	.L_0817692a
	bl	.L_0817749a
.L_0817692a:
	ldr	r3, [sp, #80]
	cmp	r3, #132
	beq.n	.L_08176932
	b.n	.L_08176ac0
.L_08176932:
	ldr	r4, [sp, #92]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #220
	adds	r5, r4, r6
	ldr	r0, [r5, #0]
	bl	sub_08020048
	ldr	r0, [pc, #584]
	bl	sub_08013300
	movs	r1, #192
	ldr	r2, [pc, #580]
	adds	r7, r0, #0
	lsls	r1, r1, #1
	ldr	r3, [pc, #576]
	ldr	r0, [pc, #580]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24c0
	lsls	r4, r4, #1
	adds	r7, r7, r4
	ldr	r6, [sp, #92]
	adds	r0, r7, #0
	movs	r7, #172
	lsls	r7, r7, #6
	adds	r1, r6, r7
	bl	sub_0801587c
	ldr	r1, [pc, #556]
	ldr	r6, [sp, #92]
	movs	r2, #172
	ldr	r7, [pc, #556]
	movs	r0, #0
	lsls	r2, r2, #6
	mov	r9, r0
	mov	sl, r1
	adds	r6, r6, r2
.L_0817697e:
	movs	r1, #32
	ldr	r2, [pc, #544]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	movs	r4, #12
	ldrb	r3, [r0, #9]
	mov	r8, r4
	mov	r1, r8
	orrs	r3, r1
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r5!, {r0}
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r3, #2]
	ldr	r2, [pc, #516]
	adds	r1, r6, #0
	adds	r0, r0, r2
	movs	r2, #128
	lsls	r2, r2, #3
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2401
	movs	r3, #128
	add	r9, r4
	lsls	r3, r3, #3
	mov	r0, r9
	adds	r6, r6, r3
	cmp	r0, #12
	bne.n	.L_0817697e
	ldr	r0, [pc, #492]
	bl	sub_08013300
	adds	r7, r0, #0
	ldr	r3, [pc, #472]
	adds	r1, r7, #0
	movs	r2, #32
	ldr	r0, [pc, #480]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4c70
	ldr	r2, [pc, #440]
	movs	r1, #32
	ldr	r0, [pc, #468]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9e17
	movs	r0, #172
	lsls	r0, r0, #6
	adds	r5, r6, r0
	adds	r7, #32
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	sub_0801587c
	movs	r2, #192
	movs	r3, #224
	movs	r1, #64
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #64
	bl	sub_0815b290
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #12
	adds	r3, r6, r1
	str	r0, [r3, #0]
	ldrb	r3, [r0, #9]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	ldr	r6, [pc, #388]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r0, [r3, #2]
	ldr	r3, [pc, #392]
	movs	r2, #128
	adds	r1, r5, #0
	ldr	r4, [pc, #380]
	adds	r0, r0, r3
	lsls	r2, r2, #4
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2140
	ldr	r2, [pc, #388]
	movs	r3, #0
	movs	r0, #64
	bl	sub_0815b290
	movs	r1, #240
	ldr	r7, [sp, #92]
	lsls	r1, r1, #7
	ldrb	r2, [r0, #9]
	adds	r1, #16
	movs	r4, #13
	negs	r4, r4
	adds	r3, r7, r1
	str	r0, [r3, #0]
	adds	r3, r4, #0
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	movs	r3, #32
	strb	r3, [r0, #23]
	ldrb	r3, [r0, #16]
	strb	r2, [r0, #9]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r0, [r3, #2]
	ldr	r6, [pc, #324]
	movs	r1, #128
	lsls	r1, r1, #5
	movs	r2, #0
	adds	r0, r0, r6
	ldr	r7, [pc, #296]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4851
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	adds	r1, r7, #0
	movs	r2, #128
	ldr	r3, [pc, #288]
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c29
	movs	r3, #0
	ldr	r0, [pc, #304]
	movs	r2, #0
	bl	sub_08157cf4
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r1, r5, #0
	ldr	r0, [sp, #72]
	adds	r2, #2
	ldr	r4, [pc, #256]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9d17
	movs	r6, #184
	lsls	r6, r6, #5
	ldr	r0, [pc, #276]
	adds	r1, r5, r6
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r7, #144
	movs	r0, #224
	lsls	r7, r7, #15
	lsls	r0, r0, #14
	str	r7, [sp, #60]
	str	r0, [sp, #56]
.L_08176ac0:
	ldr	r1, [sp, #80]
	cmp	r1, #243
	bne.n	.L_08176b46
	ldr	r0, [pc, #252]
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r1, #64
	movs	r2, #0
	ldr	r3, [pc, #192]
	ldr	r0, [pc, #240]
	adds	r7, #64
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x493b
	adds	r0, r7, #0
	bl	sub_0801587c
	ldr	r1, [pc, #228]
	movs	r4, #0
	movs	r0, #128
	mov	r9, r4
	lsls	r0, r0, #5
.L_08176aee:
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08176afa
	adds	r3, #192
	strb	r3, [r1, #0]
.L_08176afa:
	movs	r5, #1
	add	r9, r5
	adds	r1, #1
	cmp	r9, r0
	bne.n	.L_08176aee
	ldr	r3, [pc, #200]
	movs	r6, #0
	mov	r9, r6
.L_08176b0a:
	movs	r0, #1
	mov	r7, r9
	add	r9, r0
	mov	r1, r9
	strb	r7, [r3, #0]
	adds	r3, #1
	cmp	r1, #32
	bne.n	.L_08176b0a
	ldr	r5, [pc, #180]
	movs	r2, #0
	movs	r6, #128
	mov	r9, r2
	movs	r7, #31
	lsls	r6, r6, #5
.L_08176b26:
	bl	sub_08014878
	ldr	r3, [pc, #160]
	ands	r0, r7
	adds	r0, r0, r3
	adds	r0, r0, r6
	ldrb	r2, [r5, #0]
	ldrb	r3, [r0, #0]
	movs	r4, #1
	add	r9, r4
	strb	r3, [r5, #0]
	strb	r2, [r0, #0]
	mov	r0, r9
	adds	r5, #1
	cmp	r0, #32
	bne.n	.L_08176b26
.L_08176b46:
	add	r5, sp, #216
	movs	r3, #0
	str	r3, [r5, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	ldr	r1, [sp, #80]
	cmp	r1, #159
	bgt.n	.L_08176b6c
	ldr	r2, [sp, #60]
	ldr	r4, [sp, #56]
	ldr	r6, [pc, #116]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r2, r2, r3
	adds	r4, r4, r6
	str	r2, [sp, #60]
	str	r4, [sp, #56]
	b.n	.L_08176be2
.L_08176b6c:
	ldr	r7, [sp, #80]
	movs	r0, #76
	adds	r0, #255
	cmp	r7, r0
	bgt.n	.L_08176bd8
	ldr	r1, [sp, #60]
	ldr	r3, [sp, #56]
	movs	r2, #128
	movs	r4, #128
	lsls	r2, r2, #6
	lsls	r4, r4, #5
	adds	r1, r1, r2
	adds	r3, r3, r4
	str	r1, [sp, #60]
	str	r3, [sp, #56]
	b.n	.L_08176be2
	.4byte 0x0000009b
	.4byte 0x7fff7fff
	.4byte 0x03000260
	.4byte 0x05000200
	.4byte 0x020036e0
	.4byte 0x03000730
	.4byte 0x80002000
	.4byte 0x06010000
	.4byte 0x0000009f
	.4byte 0x050003c0
	.4byte 0xc0002000
	.4byte 0x000000b8
	.4byte 0x00000137
	.4byte 0x000000be
	.4byte 0x0000009d
	.4byte 0x05000380
	.4byte 0x02014000
	.4byte 0x02015000
	.2byte 0xc000
	.2byte 0xffff
.L_08176bd8:
	.2byte 0x9e0f
	movs	r7, #128
	lsls	r7, r7, #6
	adds	r6, r6, r7
	str	r6, [sp, #60]
.L_08176be2:
	movs	r3, #160
	add	r7, sp, #152
	lsls	r3, r3, #9
	str	r3, [r7, #4]
	ldr	r0, [sp, #56]
	movs	r1, #48
	str	r3, [sp, #152]
	bl	sub_08002054
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r0, r0, r1
	str	r0, [r7, #4]
	ldr	r2, [sp, #60]
	movs	r4, #128
	lsls	r4, r4, #15
	adds	r3, r2, r4
	str	r0, [sp, #152]
	str	r3, [r5, #0]
	movs	r3, #144
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	ldr	r6, [sp, #92]
	movs	r0, #240
	lsls	r0, r0, #7
	adds	r0, #12
	adds	r3, r6, r0
	ldr	r0, [r3, #0]
	adds	r1, r5, #0
	adds	r2, r7, #0
	movs	r3, #0
	bl	sub_08020010
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #4]
	ldr	r2, [sp, #92]
	str	r3, [sp, #152]
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r1, #0
	adds	r3, #220
	mov	r9, r1
	adds	r6, r2, r3
.L_08176c3a:
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_08176c42
	adds	r3, #3
.L_08176c42:
	asrs	r3, r3, #2
	ldr	r0, [sp, #60]
	mov	r4, r9
	lsls	r2, r3, #2
	subs	r2, r4, r2
	lsls	r2, r2, #21
	adds	r2, r2, r0
	str	r2, [r5, #0]
	ldr	r1, [sp, #56]
	lsls	r3, r3, #21
	adds	r3, r3, r1
	str	r3, [r5, #8]
	adds	r2, r7, #0
	movs	r3, #0
	ldmia	r6!, {r0}
	adds	r1, r5, #0
	bl	sub_08020010
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #12
	bne.n	.L_08176c3a
	ldr	r4, [sp, #80]
	cmp	r4, #200
	bne.n	sub_08176c8c
	ldr	r0, [pc, #660]
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	lsls	r0, r0, #19
	adds	r1, r7, #0
	movs	r2, #128
	ldr	r5, [pc, #648]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9d14
	subs	r5, #200
	cmp	r5, #101
	bls.n	.L_08176c96
	b.n	.L_08176e16
.L_08176c96:
	movs	r0, #32
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	adds	r7, r5, #0
	lsls	r3, r7, #2
	adds	r4, r3, #0
	lsls	r3, r7, #1
	adds	r3, r3, r7
	adds	r6, r0, #0
	lsls	r2, r3, #7
	movs	r0, #195
	ldr	r1, [sp, #80]
	subs	r2, r2, r3
	lsls	r0, r0, #9
	adds	r0, #160
	lsls	r2, r2, #2
	subs	r4, #64
	mov	fp, r0
	subs	r5, r0, r2
	cmp	r1, #253
	ble.n	.L_08176cce
	movs	r3, #254
	subs	r3, r3, r1
	lsls	r4, r3, #3
.L_08176cce:
	movs	r2, #212
	lsls	r2, r2, #6
	adds	r2, #231
	cmp	r5, r2
	bgt.n	.L_08176cde
	movs	r5, #212
	lsls	r5, r5, #6
	adds	r5, #232
.L_08176cde:
	cmp	r4, #0
	ble.n	.L_08176ce4
	movs	r4, #0
.L_08176ce4:
	str	r4, [r6, #20]
	ldr	r3, [sp, #92]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #92]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r3, [sp, #144]
	ldr	r2, [pc, #528]
	movs	r4, #184
	ands	r3, r2
	movs	r2, #7
	mov	r9, r2
	orrs	r3, r2
	ldr	r2, [pc, #520]
	lsls	r4, r4, #5
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #144]
	adds	r3, r0, r4
	add	r0, sp, #144
	str	r3, [r0, #4]
	ldr	r3, [pc, #504]
	mov	r1, r9
	mov	r2, sl
	str	r1, [r6, #0]
	str	r0, [r6, #16]
	str	r3, [r6, #8]
	str	r2, [r6, #12]
	ldr	r3, [sp, #80]
	movs	r4, #141
	lsls	r4, r4, #1
	mov	r8, r0
	cmp	r3, r4
	bgt.n	.L_08176d80
	bl	sub_08014de4
	lsls	r2, r5, #1
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_080151e4
	ldr	r1, [pc, #468]
	movs	r2, #0
	ldr	r0, [pc, #468]
	bl	sub_08015160
	movs	r0, #224
	lsls	r0, r0, #8
	bl	sub_08015024
	movs	r0, #128
	lsls	r3, r7, #8
	lsls	r0, r0, #7
	subs	r0, r0, r3
	bl	sub_080150e4
	negs	r0, r7
	lsls	r0, r0, #10
	bl	sub_08015068
	ldr	r0, [pc, #436]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_08176d80:
	ldr	r5, [sp, #80]
	cmp	r5, #231
	ble.n	.L_08176e0a
	adds	r7, r5, #0
	subs	r7, #232
	lsls	r2, r7, #2
	lsls	r3, r7, #1
	adds	r2, r2, r7
	adds	r4, r3, #0
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r1, #64
	lsls	r3, r3, #4
	mov	r0, fp
	subs	r4, #64
	negs	r1, r1
	subs	r5, r0, r3
	cmp	r4, r1
	bge.n	.L_08176daa
	movs	r4, #64
	negs	r4, r4
.L_08176daa:
	cmp	r4, #0
	ble.n	.L_08176db0
	movs	r4, #0
.L_08176db0:
	mov	r2, r8
	movs	r3, #6
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	str	r4, [r6, #20]
	ldr	r4, [sp, #92]
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r3, r4, r0
	str	r3, [r2, #4]
	ldr	r3, [pc, #356]
	mov	r1, r9
	str	r2, [r6, #16]
	mov	r2, sl
	str	r3, [r6, #8]
	str	r1, [r6, #0]
	str	r2, [r6, #12]
	bl	sub_08014de4
	ldr	r1, [pc, #328]
	movs	r2, #0
	adds	r0, r1, #0
	bl	sub_08015160
	adds	r1, r5, #0
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #192
	lsls	r0, r0, #8
	bl	sub_08015024
	negs	r0, r7
	lsls	r0, r0, #10
	bl	sub_08015068
	ldr	r0, [pc, #300]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_08176e0a:
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
.L_08176e16:
	ldr	r3, [sp, #80]
	subs	r3, #244
	cmp	r3, #65
	bls.n	.L_08176e20
	b.n	.L_08176f9a
.L_08176e20:
	ldr	r3, [pc, #268]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #136]
	str	r4, [sp, #140]
	ldr	r3, [sp, #80]
	movs	r4, #4
	adds	r4, #255
	cmp	r3, r4
	bgt.n	.L_08176eb4
	ldr	r6, [sp, #16]
	ldr	r0, [sp, #92]
	movs	r1, #240
	movs	r7, #128
	lsls	r1, r1, #7
	lsls	r7, r7, #5
	adds	r1, #16
	movs	r5, #0
	mov	ip, r6
	adds	r3, r0, r1
	adds	r6, r6, r7
	mov	r9, r5
	mov	lr, r6
	ldr	r4, [r3, #0]
	ldr	r6, [pc, #224]
	ldr	r5, [pc, #228]
.L_08176e54:
	mov	r2, lr
	ldrb	r0, [r2, #0]
	mov	r3, r9
	ldr	r7, [pc, #224]
	lsls	r1, r3, #6
	lsls	r0, r0, #1
	adds	r3, r1, r0
	adds	r2, r3, r7
	adds	r7, #1
	adds	r3, r3, r7
	ldrb	r3, [r3, #0]
	ldrb	r2, [r2, #0]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r4, #16]
	subs	r7, #1
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r3, [r3, #2]
	adds	r3, r1, r3
	adds	r3, r3, r0
	strh	r2, [r3, r5]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #1
	add	r3, ip
	ldrb	r0, [r3, #0]
	lsls	r0, r0, #1
	adds	r3, r1, r0
	adds	r2, r3, r7
	adds	r7, #1
	adds	r3, r3, r7
	ldrb	r3, [r3, #0]
	ldrb	r2, [r2, #0]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r4, #16]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r3, [r3, #2]
	adds	r1, r1, r3
	adds	r1, r1, r0
	movs	r0, #1
	add	r9, r0
	strh	r2, [r1, r5]
	mov	r1, r9
	cmp	r1, #64
	bne.n	.L_08176e54
.L_08176eb4:
	ldr	r3, [sp, #80]
	add	r1, sp, #200
	lsls	r2, r3, #11
	movs	r3, #155
	lsls	r3, r3, #12
	subs	r3, r3, r2
	add	r2, sp, #136
	str	r3, [sp, #136]
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	movs	r3, #184
	lsls	r3, r3, #15
	str	r3, [r1, #0]
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r1, #8]
	ldr	r4, [sp, #92]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #16
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	movs	r3, #0
	bl	sub_08020010
	ldr	r0, [pc, #80]
	bl	sub_08013300
	ldr	r6, [pc, #76]
	ldr	r1, [pc, #16]
	movs	r7, #0
	movs	r2, #31
	mov	lr, r6
	mov	r9, r7
	mov	ip, r1
	mov	r8, r2
	b.n	.L_08176f48
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x000000bd
	.4byte 0x03000730
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0xfff80000
	.4byte 0xfff00000
	.4byte 0x08199210
	.4byte 0x08199340
	.4byte 0x08196eb8
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x02014000
	.4byte 0x0000009d
	.2byte 0x0380
	.2byte 0x0500
.L_08176f48:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	ldrh	r1, [r0, #0]
	lsls	r3, r2, #16
	lsrs	r7, r3, #26
	lsrs	r5, r3, #21
	mov	r6, r8
	lsls	r3, r1, #16
	mov	r4, ip
	ands	r6, r2
	lsrs	r2, r3, #26
	lsrs	r3, r3, #21
	ands	r7, r4
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	mov	r4, r8
	ands	r4, r1
	cmp	r7, r2
	bge.n	.L_08176f72
	adds	r7, #1
.L_08176f72:
	cmp	r5, r3
	bge.n	.L_08176f78
	adds	r5, #1
.L_08176f78:
	cmp	r6, r4
	bge.n	.L_08176f7e
	adds	r6, #1
.L_08176f7e:
	lsls	r3, r7, #10
	lsls	r2, r5, #5
	orrs	r3, r2
	orrs	r3, r6
	movs	r6, #1
	add	r9, r6
	mov	r5, lr
	movs	r7, #2
	mov	r1, r9
	strh	r3, [r5, #0]
	adds	r0, #2
	add	lr, r7
	cmp	r1, #32
	bne.n	.L_08176f48
.L_08176f9a:
	ldr	r2, [sp, #80]
	movs	r3, #46
	adds	r3, #255
	cmp	r2, r3
	bgt.n	.L_08176fa6
	b.n	.L_081771ca
.L_08176fa6:
	movs	r4, #151
	lsls	r4, r4, #1
	cmp	r2, r4
	bne.n	.L_08177008
	movs	r1, #240
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	ldr	r2, [pc, #124]
	ldr	r5, [pc, #128]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2140
	ldr	r2, [pc, #124]
	ldr	r0, [pc, #124]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x481f
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r2, #128
	lsls	r2, r2, #1
	ldr	r6, [pc, #112]
	adds	r0, #192
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x481b
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	adds	r1, r7, #0
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9f17
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r1, r7, r2
	ldr	r0, [pc, #84]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	bl	sub_0815b410
.L_08177008:
	ldr	r3, [sp, #80]
	movs	r4, #48
	adds	r4, #255
	cmp	r3, r4
	ble.n	.L_081770c6
	movs	r5, #160
	ldr	r7, [pc, #24]
	ldr	r0, [pc, #44]
	lsls	r5, r5, #19
	bl	sub_08013300
	adds	r5, #192
	movs	r6, #0
	movs	r1, #31
	mov	lr, r5
	mov	r9, r6
	mov	ip, r7
	mov	r8, r1
	b.n	.L_08177054
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x7fff7fff
	.4byte 0x05000380
	.4byte 0x00000075
	.4byte 0x03000730
	.4byte 0x000000bd
	.2byte 0x00c2
	.2byte 0x0000
.L_08177054:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	mov	r4, ip
	lsls	r3, r2, #16
	lsrs	r6, r3, #26
	ldrh	r1, [r0, #0]
	mov	r5, ip
	ands	r6, r4
	lsrs	r4, r3, #21
	ands	r4, r5
	mov	r5, r8
	ands	r5, r2
	lsls	r2, r1, #16
	lsrs	r3, r2, #26
	mov	r7, ip
	ands	r3, r7
	adds	r7, r3, #0
	lsrs	r2, r2, #21
	mov	r3, ip
	ands	r2, r3
	mov	r3, r8
	ands	r3, r1
	subs	r7, #20
	subs	r2, #20
	adds	r3, #8
	cmp	r7, #0
	bge.n	.L_0817708c
	movs	r7, #0
.L_0817708c:
	cmp	r2, #0
	bge.n	.L_08177092
	movs	r2, #0
.L_08177092:
	cmp	r3, #31
	ble.n	.L_08177098
	movs	r3, #31
.L_08177098:
	cmp	r6, r7
	ble.n	.L_0817709e
	subs	r6, #1
.L_0817709e:
	cmp	r4, r2
	ble.n	.L_081770a4
	subs	r4, #1
.L_081770a4:
	cmp	r5, r3
	ble.n	.L_081770aa
	subs	r5, #1
.L_081770aa:
	lsls	r3, r6, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	orrs	r3, r5
	movs	r5, #1
	add	r9, r5
	mov	r4, lr
	movs	r6, #2
	mov	r7, r9
	strh	r3, [r4, #0]
	adds	r0, #2
	add	lr, r6
	cmp	r7, #128
	bne.n	.L_08177054
.L_081770c6:
	ldr	r0, [sp, #80]
	movs	r1, #46
	adds	r1, #255
	cmp	r0, r1
	ble.n	.L_081771ca
	movs	r0, #32
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #564]
	ldr	r3, [sp, #128]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #560]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #92]
	movs	r4, #184
	lsls	r4, r4, #5
	str	r3, [sp, #128]
	adds	r3, r2, r4
	add	r2, sp, #128
	str	r3, [r2, #4]
	ldr	r3, [pc, #536]
	adds	r6, r0, #0
	mov	r5, fp
	str	r1, [r6, #0]
	str	r2, [r6, #16]
	str	r3, [r6, #8]
	str	r5, [r6, #12]
	ldr	r0, [sp, #80]
	ldr	r1, [pc, #524]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r2, r3, #7
	adds	r3, r3, r2
	ldr	r2, [pc, #520]
	lsls	r3, r3, #4
	adds	r1, r1, r3
	lsls	r3, r0, #2
	adds	r2, r2, r3
	ldr	r3, [pc, #512]
	movs	r7, #0
	mov	r9, r7
	mov	sl, r1
	mov	r8, r2
	adds	r7, r0, r3
.L_08177130:
	cmp	r7, #0
	blt.n	.L_081771a8
	ldr	r3, [pc, #500]
	mov	r4, sl
	mov	r0, r8
	subs	r5, r3, r4
	subs	r0, #16
	cmp	r5, #0
	bge.n	.L_08177144
	movs	r5, #0
.L_08177144:
	cmp	r0, #0
	ble.n	.L_0817714a
	movs	r0, #0
.L_0817714a:
	str	r0, [r6, #20]
	bl	sub_08014de4
	ldr	r1, [pc, #476]
	movs	r2, #0
	adds	r0, r1, #0
	bl	sub_08015160
	lsrs	r2, r5, #31
	adds	r2, r5, r2
	asrs	r2, r2, #1
	adds	r1, r5, #0
	adds	r0, r2, #0
	mov	r5, r9
	bl	sub_080151e4
	cmp	r5, #0
	bne.n	.L_08177178
	movs	r0, #240
	lsls	r0, r0, #8
	bl	sub_080150e4
	b.n	.L_08177190
.L_08177178:
	mov	r0, r9
	cmp	r0, #1
	bne.n	.L_08177188
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_080150e4
	b.n	.L_08177190
.L_08177188:
	movs	r0, #224
	lsls	r0, r0, #8
	bl	sub_080150e4
.L_08177190:
	movs	r0, #128
	lsls	r0, r0, #4
	bl	sub_08015024
	ldr	r0, [pc, #408]
	mov	r1, fp
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_081771a8:
	ldr	r1, [pc, #396]
	movs	r3, #1
	movs	r2, #32
	add	r9, r3
	negs	r2, r2
	mov	r4, r9
	add	sl, r1
	add	r8, r2
	subs	r7, #8
	cmp	r4, #3
	bne.n	.L_08177130
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
.L_081771ca:
	ldr	r5, [sp, #80]
	movs	r6, #165
	lsls	r6, r6, #1
	cmp	r5, r6
	bne.n	sub_081771e0
	ldr	r0, [pc, #356]
	movs	r1, #64
	ldr	r2, [pc, #356]
	ldr	r7, [pc, #360]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9814
	ldr	r1, [pc, #356]
	adds	r3, r0, r1
	cmp	r3, #20
	bhi.n	.L_08177254
	ldr	r3, [pc, #352]
	ldr	r2, [sp, #80]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r0, #0
	str	r3, [sp, #120]
	str	r4, [sp, #124]
	movs	r3, #173
	lsls	r3, r3, #1
	movs	r4, #0
	cmp	r2, r3
	ble.n	.L_08177214
	ldr	r5, [pc, #332]
	adds	r3, #1
	subs	r4, r3, r2
	adds	r3, r2, r5
	cmp	r3, #0
	bge.n	.L_08177212
	ldr	r6, [pc, #324]
	adds	r3, r2, r6
.L_08177212:
	asrs	r0, r3, #2
.L_08177214:
	ldr	r7, [sp, #80]
	ldr	r1, [pc, #320]
	lsls	r3, r7, #11
	adds	r3, r3, r1
	add	r2, sp, #120
	str	r3, [r2, #4]
	add	r1, sp, #184
	str	r3, [sp, #120]
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	lsls	r3, r4, #16
	movs	r4, #184
	lsls	r4, r4, #15
	adds	r3, r3, r4
	movs	r5, #144
	str	r3, [r1, #0]
	lsls	r5, r5, #15
	lsls	r3, r0, #16
	adds	r3, r3, r5
	str	r3, [r1, #8]
	ldr	r6, [sp, #92]
	movs	r7, #240
	lsls	r7, r7, #7
	adds	r7, #16
	adds	r3, r6, r7
	ldr	r0, [r3, #0]
	movs	r3, #0
	bl	sub_08020010
.L_08177254:
	ldr	r0, [sp, #80]
	movs	r1, #92
	adds	r1, #255
	cmp	r0, r1
	bne.n	.L_081772ae
	ldr	r3, [sp, #92]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r5, [sp, #92]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #132
	movs	r2, #0
	adds	r3, r5, r6
	str	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	movs	r1, #160
	strh	r2, [r3, #0]
	lsls	r1, r1, #19
	adds	r1, #2
	mov	r9, r2
	movs	r0, #31
.L_0817728a:
	mov	r7, r9
	subs	r2, r0, r7
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08177296
	movs	r2, #0
.L_08177296:
	cmp	r3, #0
	bge.n	.L_0817729c
	movs	r3, #0
.L_0817729c:
	lsls	r3, r3, #10
	orrs	r3, r2
	movs	r2, #1
	add	r9, r2
	strh	r3, [r1, #0]
	mov	r3, r9
	adds	r1, #2
	cmp	r3, #63
	bne.n	.L_0817728a
.L_081772ae:
	ldr	r4, [sp, #80]
	movs	r5, #173
	lsls	r5, r5, #1
	cmp	r4, r5
	ble.n	.L_08177392
	ldr	r6, [pc, #160]
	movs	r7, #94
	lsls	r3, r4, #2
	adds	r7, #255
	adds	r5, r3, r6
	movs	r6, #54
	cmp	r4, r7
	ble.n	.L_081772de
	ldr	r1, [pc, #148]
	lsls	r0, r4, #10
	adds	r0, r0, r1
	bl	sub_08002096
	adds	r3, r0, #0
	muls	r3, r6
	negs	r3, r3
	asrs	r3, r3, #16
	adds	r6, r3, #0
	adds	r6, #54
.L_081772de:
	ldr	r2, [sp, #80]
	movs	r3, #180
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_081772f4
	ldr	r4, [sp, #92]
	movs	r7, #239
	lsls	r7, r7, #7
	adds	r2, r4, r7
	movs	r3, #0
	str	r3, [r2, #0]
.L_081772f4:
	ldr	r0, [sp, #80]
	movs	r1, #116
	adds	r1, #255
	cmp	r0, r1
	bgt.n	.L_08177364
	ldr	r2, [sp, #92]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r0, r2, r3
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	b.n	.L_08177366
	movs	r0, r0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0xffe37760
	.4byte 0xfffffb48
	.4byte 0xfffffed2
	.4byte 0x000222e0
	.4byte 0xfff80000
	.4byte 0x08199210
	.4byte 0xffff3e80
	.4byte 0x05000380
	.4byte 0x7c1f7c1f
	.4byte 0x03000260
	.4byte 0xfffffeb6
	.4byte 0x08196ec0
	.4byte 0xfffffea5
	.4byte 0xfffffea8
	.4byte 0xfff5b000
	.4byte 0xfffffa9c
	.2byte 0x8800
	.2byte 0xfffa
.L_08177364:
	.2byte 0x2568
.L_08177366:
	movs	r0, #188
	bl	sub_0801314c
	ldr	r4, [sp, #92]
	movs	r7, #184
	lsls	r7, r7, #5
	adds	r0, r4, r7
	adds	r3, r5, #0
	adds	r1, r6, #0
	movs	r2, #58
	bl	sub_0818caa8
	movs	r0, #188
	movs	r1, #3
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #64]
	str	r3, [r0, #4]
.L_08177392:
	ldr	r2, [sp, #80]
	ldr	r3, [pc, #72]
	movs	r1, #0
	str	r1, [sp, #84]
	adds	r2, r2, r3
	mov	r8, r2
.L_0817739e:
	mov	r4, r8
	cmp	r4, #0
	bne.n	sub_081773b6
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #128
	adds	r0, #192
	lsls	r1, r1, #1
	ldr	r2, [pc, #52]
	ldr	r5, [pc, #52]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4646
	cmp	r6, #159
	bhi.n	.L_08177488
	ldr	r0, [pc, #44]
	bl	sub_08013300
	movs	r7, #160
	ldr	r1, [pc, #20]
	lsls	r7, r7, #19
	mov	ip, r0
	adds	r7, #192
	movs	r0, #0
	movs	r2, #31
	mov	sl, r7
	mov	r9, r0
	mov	lr, r1
	mov	fp, r2
	b.n	.L_081773f0
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0xfffffe9b
	.4byte 0x7fff7fff
	.4byte 0x03000260
	.2byte 0x0075
	.2byte 0x0000
.L_081773f0:
	mov	r3, sl
	ldrh	r2, [r3, #0]
	mov	r4, lr
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	lsrs	r6, r3, #21
	mov	r7, fp
	mov	r5, r8
	ands	r0, r4
	ands	r6, r4
	ands	r7, r2
	cmp	r5, #7
	bgt.n	.L_08177420
	mov	r1, ip
	ldrh	r2, [r1, #0]
	mov	r1, fp
	lsls	r3, r2, #16
	lsrs	r5, r3, #26
	ands	r5, r4
	lsrs	r4, r3, #21
	mov	r3, lr
	ands	r4, r3
	ands	r1, r2
	b.n	.L_08177442
.L_08177420:
	mov	r4, ip
	ldrh	r1, [r4, #0]
	mov	r5, lr
	lsls	r2, r1, #16
	lsrs	r3, r2, #26
	ands	r3, r5
	adds	r5, r3, #0
	lsrs	r2, r2, #21
	mov	r3, lr
	ands	r2, r3
	mov	r3, fp
	ands	r3, r1
	adds	r4, r2, #0
	adds	r1, r3, #0
	subs	r5, #16
	subs	r4, #16
	adds	r1, #8
.L_08177442:
	cmp	r5, #0
	bge.n	.L_08177448
	movs	r5, #0
.L_08177448:
	cmp	r4, #0
	bge.n	.L_0817744e
	movs	r4, #0
.L_0817744e:
	cmp	r1, #31
	ble.n	.L_08177454
	movs	r1, #31
.L_08177454:
	adds	r3, r5, #1
	cmp	r0, r3
	ble.n	.L_0817745c
	subs	r0, #2
.L_0817745c:
	adds	r3, r4, #1
	cmp	r6, r3
	ble.n	.L_08177464
	subs	r6, #2
.L_08177464:
	adds	r3, r1, #1
	cmp	r7, r3
	ble.n	.L_0817746c
	subs	r7, #2
.L_0817746c:
	lsls	r2, r6, #5
	lsls	r3, r0, #10
	movs	r5, #1
	orrs	r3, r2
	add	r9, r5
	orrs	r3, r7
	mov	r4, sl
	movs	r6, #2
	mov	r7, r9
	strh	r3, [r4, #0]
	add	ip, r6
	add	sl, r6
	cmp	r7, #128
	bne.n	.L_081773f0
.L_08177488:
	ldr	r1, [sp, #84]
	movs	r0, #19
	negs	r0, r0
	adds	r1, #1
	add	r8, r0
	str	r1, [sp, #84]
	cmp	r1, #2
	beq.n	.L_0817749a
	b.n	.L_0817739e
.L_0817749a:
	ldr	r2, [sp, #52]
	cmp	r2, #23
	bhi.n	.L_0817750c
	ldr	r0, [sp, #20]
	ldr	r7, [sp, #20]
	subs	r0, #114
	movs	r1, #6
	subs	r7, #104
	bl	sub_08002054
	cmp	r0, #0
	bge.n	.L_081774b4
	movs	r0, #0
.L_081774b4:
	movs	r3, #0
	movs	r4, #8
	lsls	r0, r0, #5
	movs	r5, #2
	mov	r9, r3
	mov	fp, r4
	mov	r8, r0
	mov	sl, r5
.L_081774c4:
	mov	r0, r9
	lsls	r6, r0, #8
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, r7, #0
	muls	r5, r0
	adds	r0, r6, #0
	bl	sub_08002090
	lsls	r5, r5, #1
	asrs	r5, r5, #16
	adds	r3, r7, #0
	muls	r3, r0
	mov	r1, sl
	adds	r5, #66
	subs	r5, r5, r1
	ldr	r1, [sp, #72]
	movs	r2, #4
	str	r2, [sp, #0]
	mov	r4, fp
	adds	r2, r5, #0
	asrs	r3, r3, #16
	movs	r5, #1
	movs	r6, #128
	str	r4, [sp, #4]
	adds	r3, #68
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #96]
	add	r1, r8
	add	r9, r5
	lsls	r6, r6, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x45b1
	bne.n	.L_081774c4
.L_0817750c:
	ldr	r3, [sp, #80]
	subs	r3, #108
	cmp	r3, #23
	bhi.n	.L_0817757c
	movs	r0, #160
	ldr	r4, [pc, #40]
	movs	r7, #0
	lsls	r0, r0, #19
	mov	r9, r7
	adds	r0, #192
.L_08177520:
	ldrh	r3, [r0, #0]
	movs	r1, #31
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	ands	r2, r4
	lsrs	r3, r3, #26
	subs	r2, #1
	ands	r3, r4
	cmp	r2, #0
	bge.n	.L_08177538
	subs	r1, #1
.L_08177538:
	cmp	r1, #31
	ble.n	.L_08177544
	movs	r1, #31
	b.n	.L_0817754a
	.2byte 0x001f
	.2byte 0x0000
.L_08177544:
	cmp	r1, #0
	bge.n	.L_0817754a
	movs	r1, #0
.L_0817754a:
	cmp	r2, #31
	ble.n	.L_08177552
	movs	r2, #31
	b.n	.L_08177558
.L_08177552:
	cmp	r2, #0
	bge.n	.L_08177558
	movs	r2, #0
.L_08177558:
	cmp	r3, #31
	ble.n	.L_08177560
	movs	r3, #31
	b.n	.L_08177566
.L_08177560:
	cmp	r3, #0
	bge.n	.L_08177566
	movs	r3, #0
.L_08177566:
	lsls	r2, r2, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	orrs	r3, r1
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r2, #128
	bne.n	.L_08177520
.L_0817757c:
	ldr	r3, [sp, #52]
	cmp	r3, #35
	bls.n	.L_08177584
	b.n	.L_081776d0
.L_08177584:
	ldr	r4, [sp, #80]
	cmp	r4, #56
	bne.n	.L_0817764e
	ldr	r5, [sp, #92]
	movs	r6, #239
	lsls	r6, r6, #7
	adds	r2, r5, r6
	movs	r3, #3
	movs	r7, #238
	str	r3, [r2, #0]
	lsls	r7, r7, #7
	ldr	r3, [pc, #256]
	adds	r7, #132
	adds	r2, r5, r7
	str	r3, [r2, #0]
	ldr	r0, [pc, #252]
	bl	sub_08013300
	movs	r2, #172
	adds	r7, r0, #0
	adds	r7, #32
	lsls	r2, r2, #6
	adds	r0, r7, #0
	adds	r1, r5, r2
	bl	sub_0801587c
	movs	r3, #7
	movs	r4, #0
	movs	r7, #0
	mov	sl, r3
	mov	lr, r4
.L_081775c2:
	ldr	r3, [sp, #92]
	movs	r5, #172
	lsls	r5, r5, #6
	add	r3, lr
	adds	r5, #1
	adds	r5, r5, r3
	movs	r6, #0
	mov	r8, lr
	mov	ip, r5
.L_081775d4:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_081775dc
	adds	r3, r7, #7
.L_081775dc:
	asrs	r3, r3, #3
	lsls	r3, r3, #5
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_081775e8
	adds	r2, r6, #7
.L_081775e8:
	asrs	r2, r2, #3
	mov	r0, sl
	adds	r2, r3, r2
	adds	r3, r7, #0
	ands	r3, r0
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldr	r1, [pc, #172]
	adds	r3, r6, #0
	ands	r3, r0
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	lsrs	r5, r3, #8
	lsls	r3, r3, #24
	lsrs	r4, r3, #24
	mov	r3, r8
	adds	r0, r3, r6
	mov	r3, ip
	ldrb	r1, [r3, #0]
	adds	r3, r1, #0
	cmp	r3, #0
	beq.n	.L_0817761e
	adds	r3, #224
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
.L_0817761e:
	movs	r1, #172
	lsls	r1, r1, #6
	adds	r3, r0, r1
	ldr	r0, [sp, #92]
	ldrb	r1, [r0, r3]
	adds	r3, r1, #0
	cmp	r3, #0
	beq.n	.L_08177634
	adds	r3, #224
	lsls	r3, r3, #24
	lsrs	r4, r3, #24
.L_08177634:
	lsls	r3, r5, #8
	orrs	r3, r4
	movs	r1, #2
	adds	r6, #2
	strh	r3, [r2, #0]
	add	ip, r1
	cmp	r6, #184
	bne.n	.L_081775d4
	movs	r2, #184
	adds	r7, #1
	add	lr, r2
	cmp	r7, #80
	bne.n	.L_081775c2
.L_0817764e:
	ldr	r3, [sp, #80]
	cmp	r3, #87
	bgt.n	.L_081776d0
	ldr	r0, [pc, #72]
	bl	sub_08013300
	ldr	r7, [sp, #52]
	ldr	r6, [pc, #72]
	lsls	r7, r7, #9
	mov	r8, r7
	ldr	r7, [pc, #52]
	movs	r4, #0
	adds	r5, r0, #0
	mov	r9, r4
.L_0817766a:
	mov	r0, r8
	bl	sub_08002090
	lsls	r2, r0, #1
	adds	r2, r2, r0
	ldrh	r1, [r5, #0]
	lsls	r2, r2, #2
	asrs	r2, r2, #16
	movs	r3, #31
	adds	r2, #16
	ands	r3, r1
	lsls	r1, r1, #16
	adds	r4, r3, r2
	lsrs	r3, r1, #21
	lsrs	r1, r1, #26
	ands	r3, r7
	ands	r1, r7
	adds	r0, r3, r2
	adds	r1, r1, r2
	cmp	r4, #31
	ble.n	.L_081776ac
	movs	r4, #31
	b.n	.L_081776ac
	.4byte 0x0000001f
	.4byte 0x04040404
	.4byte 0x000000b5
	.4byte 0x0600a900
	.2byte 0x01c0
	.2byte 0x0500
.L_081776ac:
	cmp	r0, #31
	ble.n	.L_081776b2
	movs	r0, #31
.L_081776b2:
	cmp	r1, #31
	ble.n	.L_081776b8
	movs	r1, #31
.L_081776b8:
	lsls	r3, r1, #10
	lsls	r2, r0, #5
	movs	r0, #1
	orrs	r3, r2
	add	r9, r0
	orrs	r3, r4
	mov	r1, r9
	strh	r3, [r6, #0]
	adds	r5, #2
	adds	r6, #2
	cmp	r1, #16
	bne.n	.L_0817766a
.L_081776d0:
	ldr	r2, [sp, #80]
	cmp	r2, #54
	bne.n	sub_081776e4
	movs	r1, #240
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	ldr	r2, [pc, #624]
	ldr	r3, [pc, #628]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c14
	cmp	r4, #55
	bne.n	sub_081776f8
	movs	r1, #240
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	ldr	r2, [pc, #612]
	ldr	r5, [pc, #608]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9e14
	cmp	r6, #131
	ble.n	.L_08177700
	b.n	.L_081778f4
.L_08177700:
	movs	r0, #32
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	adds	r7, r0, #0
	ldr	r0, [sp, #92]
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
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #80]
	adds	r2, r4, r6
	lsls	r3, r2, #7
	subs	r3, r3, r2
	adds	r5, r4, #0
	lsls	r4, r3, #2
	ldr	r3, [pc, #548]
	subs	r5, #64
	subs	r6, r3, r4
	cmp	r0, #55
	ble.n	.L_08177744
	ldr	r1, [pc, #540]
	adds	r6, r4, r1
.L_08177744:
	movs	r2, #8
	negs	r2, r2
	cmp	r5, r2
	blt.n	.L_08177750
	movs	r5, #8
	negs	r5, r5
.L_08177750:
	ldr	r3, [sp, #80]
	cmp	r3, #95
	ble.n	.L_0817775e
	ldr	r4, [sp, #20]
	movs	r3, #184
	subs	r5, r3, r4
	str	r5, [r7, #20]
.L_0817775e:
	ldr	r3, [sp, #112]
	ldr	r2, [pc, #512]
	ldr	r0, [sp, #92]
	ands	r3, r2
	movs	r2, #6
	orrs	r3, r2
	ldr	r2, [pc, #508]
	movs	r1, #224
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	lsls	r1, r1, #3
	add	r2, sp, #112
	str	r3, [sp, #112]
	adds	r3, r0, r1
	str	r3, [r2, #4]
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r3, [pc, #484]
	movs	r4, #48
	str	r3, [r7, #8]
	negs	r4, r4
	mov	r3, sl
	mov	r8, r2
	str	r2, [r7, #16]
	str	r3, [r7, #12]
	cmp	r5, r4
	ble.n	.L_081777e4
	str	r5, [r7, #20]
	bl	sub_08014de4
	movs	r0, #128
	lsls	r0, r0, #11
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
	adds	r0, r6, #0
	adds	r1, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r6, [sp, #80]
	cmp	r6, #55
	ble.n	.L_081777ca
	lsls	r0, r6, #10
	bl	sub_08015068
	b.n	.L_081777d4
.L_081777ca:
	ldr	r1, [sp, #80]
	negs	r0, r1
	lsls	r0, r0, #10
	bl	sub_08015068
.L_081777d4:
	ldr	r0, [pc, #408]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081777e4:
	ldr	r2, [sp, #80]
	cmp	r2, #95
	ble.n	.L_081777f8
	ldr	r4, [sp, #20]
	adds	r3, r4, r2
	movs	r2, #142
	lsls	r3, r3, #1
	lsls	r2, r2, #2
	subs	r5, r2, r3
	str	r5, [r7, #20]
.L_081777f8:
	movs	r6, #48
	negs	r6, r6
	cmp	r5, r6
	ble.n	.L_08177838
	bl	sub_08014de4
	movs	r0, #128
	ldr	r1, [pc, #364]
	movs	r2, #0
	lsls	r0, r0, #11
	bl	sub_08015160
	ldr	r0, [pc, #356]
	bl	sub_0801521c
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r5, [sp, #80]
	negs	r0, r5
	lsls	r0, r0, #9
	bl	sub_08015068
	ldr	r0, [pc, #324]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08177838:
	ldr	r0, [sp, #80]
	cmp	r0, #55
	ble.n	.L_081778d6
	ldr	r2, [sp, #20]
	ldr	r1, [sp, #92]
	movs	r4, #184
	lsls	r4, r4, #5
	movs	r0, #32
	adds	r3, r1, r4
	mov	r5, r8
	subs	r2, #208
	negs	r0, r0
	str	r3, [r5, #4]
	cmp	r2, r0
	blt.n	.L_0817785a
	movs	r2, #32
	negs	r2, r2
.L_0817785a:
	str	r2, [r7, #20]
	bl	sub_08014de4
	movs	r0, #128
	movs	r2, #0
	ldr	r1, [pc, #276]
	lsls	r0, r0, #10
	bl	sub_08015160
	movs	r0, #176
	lsls	r0, r0, #7
	adds	r0, #240
	bl	sub_0801521c
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r1, [sp, #80]
	lsls	r0, r1, #10
	bl	sub_08015068
	ldr	r5, [pc, #232]
	mov	r1, sl
	adds	r0, r5, #0
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	ldr	r4, [sp, #80]
	cmp	r4, r6
	ble.n	.L_081778d6
	bl	sub_08014de4
	movs	r0, #128
	ldr	r1, [pc, #216]
	movs	r2, #0
	lsls	r0, r0, #10
	bl	sub_08015160
	ldr	r0, [pc, #212]
	bl	sub_0801521c
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r6, [sp, #80]
	negs	r0, r6
	lsls	r0, r0, #10
	bl	sub_08015068
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081778d6:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
	movs	r0, #240
	ldr	r7, [sp, #92]
	lsls	r0, r0, #7
	adds	r0, #228
	adds	r2, r7, r0
.L_081778ec:
	ldr	r3, [r2, #0]
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bls.n	.L_081778ec
.L_081778f4:
	ldr	r1, [sp, #92]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #20]
	ldr	r5, [sp, #16]
	ldr	r6, [sp, #80]
	movs	r7, #197
	adds	r4, #2
	adds	r5, #2
	adds	r6, #1
	lsls	r7, r7, #1
	str	r4, [sp, #20]
	str	r5, [sp, #16]
	str	r6, [sp, #80]
	cmp	r6, r7
	beq.n	.L_08177932
	ldr	r3, [pc, #100]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08177932
	bl	.L_08176320
.L_08177932:
	movs	r0, #162
	bl	sub_081c0010
	ldr	r0, [sp, #80]
	cmp	r0, #131
	bgt.n	.L_0817798c
	ldr	r1, [sp, #92]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	sub_08020048
	b.n	.L_081779aa
	.4byte 0x20202020
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x0001c350
	.4byte 0xffff32a0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x08199210
	.4byte 0xfff00000
	.4byte 0xffff58f0
	.4byte 0xffe00000
	.4byte 0xffd00000
	.4byte 0xffffce20
	.2byte 0x1150
	.2byte 0x0300
.L_0817798c:
	ldr	r4, [sp, #92]
	movs	r6, #238
	lsls	r6, r6, #7
	movs	r3, #0
	adds	r6, #220
	mov	r9, r3
	adds	r5, r4, r6
.L_0817799a:
	movs	r7, #1
	ldmia	r5!, {r0}
	add	r9, r7
	bl	sub_08020048
	mov	r0, r9
	cmp	r0, #14
	bne.n	.L_0817799a
.L_081779aa:
	ldr	r5, [pc, #96]
	movs	r1, #0
	mov	r9, r1
.L_081779b0:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #48
	bne.n	.L_081779b0
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08014c4c
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r5, #237
	ldr	r4, [sp, #68]
	lsls	r5, r5, #3
	adds	r5, #255
	adds	r2, r4, r5
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r6, #206
	lsls	r6, r6, #3
	adds	r3, r4, r6
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #24
	bl	sub_08118040
	ldr	r5, [pc, #16]
	movs	r1, #128
	ldr	r0, [sp, #96]
	lsls	r1, r1, #7
	movs	r2, #0
	b.n	.L_08177a14
	.4byte 0x00000141
	.4byte 0x02010000
	.2byte 0x0260
	.2byte 0x0300
.L_08177a14:
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x481d
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4a1b
	movs	r3, #120
	str	r3, [r2, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	movs	r7, #0
	str	r3, [sp, #36]
	str	r7, [sp, #32]
	add	r2, sp, #32
	movs	r1, #54
	ldrsh	r0, [r3, r1]
	movs	r4, #239
	str	r0, [sp, #28]
	lsls	r4, r4, #7
	ldrh	r2, [r2, #0]
	movs	r6, #238
	strh	r2, [r3, #54]
	ldr	r3, [sp, #92]
	lsls	r6, r6, #7
	adds	r1, r3, r4
	movs	r3, #1
	str	r3, [r1, #0]
	ldr	r5, [sp, #92]
	adds	r6, #132
	adds	r2, r5, r6
	movs	r3, #2
	str	r7, [r2, #0]
	str	r3, [r1, #0]
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	str	r7, [r3, #0]
	ldr	r3, [pc, #20]
	subs	r2, #70
	movs	r5, #160
	strh	r3, [r2, #0]
	movs	r7, #1
	lsls	r5, r5, #19
	mov	r9, r7
	adds	r5, #2
	b.n	.L_08177a98
	movs	r0, r0
	.4byte 0x00000410
	.4byte 0x00000786
	.4byte 0x06004000
	.2byte 0x11e0
	.2byte 0x0300
.L_08177a98:
	mov	r1, r9
	lsls	r0, r1, #1
	movs	r1, #3
	bl	sub_08002054
	movs	r3, #31
	mov	r2, r9
	subs	r1, r3, r0
	lsrs	r3, r2, #31
	add	r3, r9
	asrs	r3, r3, #1
	movs	r2, #8
	subs	r2, r2, r3
	movs	r3, #27
	subs	r3, r3, r0
	cmp	r1, #0
	bge.n	.L_08177abc
	movs	r1, #0
.L_08177abc:
	cmp	r2, #0
	bge.n	.L_08177ac2
	movs	r2, #0
.L_08177ac2:
	cmp	r3, #0
	bge.n	.L_08177ac8
	movs	r3, #0
.L_08177ac8:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #0]
	movs	r3, #1
	add	r9, r3
	mov	r4, r9
	adds	r5, #2
	cmp	r4, #64
	bne.n	.L_08177a98
	ldr	r5, [sp, #100]
	ldr	r2, [pc, #60]
	ldr	r3, [r5, #20]
	adds	r6, r5, #0
	lsls	r3, r3, #1
	adds	r3, #36
	adds	r0, r6, #0
	strh	r2, [r6, r3]
	adds	r0, #36
	movs	r1, #0
	bl	sub_08118010
	ldr	r7, [sp, #92]
	movs	r1, #224
	movs	r5, #128
	lsls	r5, r5, #9
	lsls	r1, r1, #3
	adds	r0, r7, r1
	adds	r2, r5, #0
	movs	r1, #56
	bl	sub_0815b434
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r0, r7, r2
	movs	r1, #16
	adds	r2, r5, #0
	bl	sub_0815b434
	movs	r3, #156
	lsls	r3, r3, #6
	b.n	.L_08177b24
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
.L_08177b24:
	movs	r1, #128
	adds	r0, r7, r3
	lsls	r1, r1, #5
	ldr	r3, [pc, #808]
	ldr	r2, [pc, #808]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24dc
	lsls	r4, r4, #6
	adds	r1, r7, r4
	ldr	r0, [pc, #800]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [pc, #792]
	ldr	r1, [sp, #72]
	movs	r2, #0
	bl	sub_08157cf4
	movs	r3, #192
	movs	r5, #0
	lsls	r3, r3, #2
	mov	r9, r5
	adds	r3, #2
.L_08177b58:
	movs	r6, #35
	add	r9, r6
	cmp	r9, r3
	bne.n	.L_08177b58
	ldr	r3, [sp, #92]
	movs	r4, #220
	movs	r7, #0
	movs	r1, #128
	lsls	r4, r4, #6
	mov	r9, r7
	movs	r0, #63
	lsls	r1, r1, #5
	adds	r2, r3, r4
.L_08177b72:
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08177b7a
	strb	r0, [r2, #0]
.L_08177b7a:
	movs	r5, #1
	add	r9, r5
	adds	r2, #1
	cmp	r9, r1
	bne.n	.L_08177b72
	ldr	r1, [pc, #732]
	ldr	r0, [pc, #736]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08177bb6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #238
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08177bb6:
	strh	r4, [r0, #0]
	movs	r6, #0
	ldr	r2, [sp, #92]
	mov	r9, r6
	adds	r2, #24
.L_08177bc0:
	movs	r0, #1
	mov	r7, r9
	add	r9, r0
	negs	r3, r7
	mov	r1, r9
	str	r3, [r2, #0]
	adds	r2, #28
	cmp	r1, #16
	bne.n	.L_08177bc0
	ldr	r3, [pc, #664]
	movs	r2, #0
	mov	r9, r2
	subs	r2, #1
.L_08177bda:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r5, #128
	bne.n	.L_08177bda
	ldr	r7, [sp, #100]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_08177c24
	ldr	r0, [sp, #92]
	movs	r1, #224
	lsls	r1, r1, #2
	adds	r5, r0, r1
	movs	r6, #36
.L_08177bfe:
	ldr	r2, [sp, #100]
	ldrsh	r0, [r6, r2]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r4, #1
	ldr	r3, [r2, #8]
	add	r9, r4
	str	r3, [r5, #0]
	adds	r6, #2
	ldr	r3, [r2, #12]
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	ldr	r7, [sp, #100]
	adds	r5, #28
	ldr	r3, [r7, #20]
	cmp	r9, r3
	bne.n	.L_08177bfe
.L_08177c24:
	movs	r0, #0
	str	r0, [sp, #80]
.L_08177c28:
	ldr	r3, [pc, #580]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08177c3e
	ldr	r1, [sp, #80]
	cmp	r1, #119
	bgt.n	.L_08177c3e
	movs	r2, #120
	str	r2, [sp, #80]
.L_08177c3e:
	ldr	r3, [sp, #80]
	cmp	r3, #0
	bne.n	.L_08177cd2
	movs	r0, #162
	bl	sub_081c0010
	movs	r5, #192
	ldr	r4, [sp, #92]
	lsls	r5, r5, #3
	adds	r5, #252
	movs	r3, #160
	adds	r2, r4, r5
	str	r3, [r2, #0]
	ldr	r7, [sp, #100]
	movs	r6, #0
	str	r6, [sp, #32]
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_08177c80
	movs	r0, #236
	movs	r1, #1
	lsls	r0, r0, #1
	negs	r1, r1
	adds	r2, r4, r0
.L_08177c70:
	str	r1, [r2, #0]
	ldr	r4, [sp, #100]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r4, #20]
	adds	r2, #28
	cmp	r9, r3
	bne.n	.L_08177c70
.L_08177c80:
	movs	r5, #0
	mov	r9, r5
	cmp	r3, #0
	beq.n	.L_08177cd2
	movs	r6, #1
	negs	r6, r6
	movs	r7, #254
	mov	sl, r6
	mov	r8, r5
	lsls	r7, r7, #24
	movs	r6, #36
.L_08177c96:
	ldr	r1, [sp, #100]
	ldrsh	r0, [r6, r1]
	bl	sub_08118098
	ldr	r3, [sp, #100]
	mov	r1, sl
	ldr	r5, [r0, #0]
	movs	r2, #5
	ldrsh	r0, [r6, r3]
	str	r1, [sp, #0]
	mov	r3, sl
	movs	r1, #0
	bl	sub_0814cd48
	movs	r3, #160
	mov	r2, r8
	lsls	r3, r3, #13
	str	r7, [r5, #8]
	str	r3, [r5, #12]
	str	r2, [r5, #16]
	str	r2, [r5, #72]
	ldr	r3, [pc, #432]
	ldr	r5, [sp, #100]
	adds	r7, r7, r3
	ldr	r3, [r5, #20]
	movs	r4, #1
	add	r9, r4
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08177c96
.L_08177cd2:
	ldr	r6, [sp, #80]
	cmp	r6, #120
	bne.n	.L_08177d3c
	ldr	r7, [sp, #92]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r3, r7, r1
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r3, [sp, #68]
	movs	r4, #160
	lsls	r4, r4, #3
	movs	r1, #160
	movs	r2, #153
	adds	r4, #108
	lsls	r1, r1, #19
	lsls	r2, r2, #8
	adds	r0, r3, r4
	adds	r1, #192
	movs	r3, #128
	adds	r2, #160
	bl	sub_081180f0
	ldr	r6, [sp, #100]
	movs	r5, #0
	ldr	r3, [r6, #20]
	mov	r9, r5
	cmp	r3, #0
	beq.n	.L_08177d3c
	movs	r6, #36
.L_08177d1a:
	ldr	r7, [sp, #100]
	ldrsh	r0, [r6, r7]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r3, #248
	lsls	r3, r3, #15
	str	r5, [r2, #8]
	str	r3, [r2, #12]
	str	r5, [r2, #16]
	str	r5, [r2, #72]
	ldr	r3, [r7, #20]
	movs	r2, #1
	add	r9, r2
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08177d1a
.L_08177d3c:
	ldr	r3, [sp, #80]
	cmp	r3, #160
	bne.n	.L_08177d48
	movs	r0, #134
	bl	sub_081180e8
.L_08177d48:
	ldr	r4, [sp, #80]
	cmp	r4, #139
	bgt.n	.L_08177d50
	b.n	.L_08177ffc
.L_08177d50:
	cmp	r4, #140
	beq.n	.L_08177d56
	b.n	.L_08177f16
.L_08177d56:
	movs	r6, #240
	ldr	r7, [pc, #284]
	movs	r5, #0
	lsls	r6, r6, #14
	mov	r9, r5
	mov	r8, r6
.L_08177d62:
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	adds	r6, r0, #0
	mov	r0, r8
	str	r0, [r7, #0]
	str	r0, [r7, #4]
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
	movs	r2, #1
	asrs	r3, r3, #5
	add	r9, r2
	str	r3, [r7, #16]
	movs	r1, #0
	mov	r3, r9
	str	r1, [r7, #24]
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L_08177d62
	ldr	r7, [pc, #208]
	mov	r9, r1
.L_08177dac:
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r7, #0]
	str	r3, [r7, #4]
	adds	r6, r0, #0
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
	movs	r3, #15
	movs	r4, #1
	ands	r3, r0
	add	r9, r4
	adds	r3, #16
	mov	r5, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r5, #128
	bne.n	.L_08177dac
	ldr	r1, [pc, #136]
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #132]
	bl	sub_08157cf4
	movs	r0, #144
	bl	sub_081c0010
	movs	r1, #240
	ldr	r3, [pc, #72]
	lsls	r1, r1, #6
	ldr	r2, [pc, #72]
	ldr	r0, [sp, #96]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0e
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r6, [sp, #92]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r6, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	adds	r2, r6, r0
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r2, [sp, #100]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_08177eb0
	movs	r3, #226
	lsls	r3, r3, #2
	adds	r5, r6, r3
	movs	r6, #36
	b.n	.L_08177e88
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x000000c3
	.4byte 0x00000134
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x02014218
	.4byte 0x03001150
	.4byte 0xff800000
	.4byte 0x02014c80
	.4byte 0x02015000
	.4byte 0x02010000
	.2byte 0x013e
	.2byte 0x0000
.L_08177e88:
	ldr	r4, [sp, #100]
	ldrsh	r0, [r6, r4]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	movs	r0, #0
	str	r0, [r2, #8]
	str	r3, [r2, #12]
	movs	r1, #1
	ldr	r3, [r5, #0]
	add	r9, r1
	str	r3, [r2, #16]
	ldr	r2, [sp, #100]
	adds	r5, #28
	ldr	r3, [r2, #20]
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08177e88
.L_08177eb0:
	movs	r4, #0
	mov	r9, r4
	cmp	r3, #0
	beq.n	.L_08177f08
	movs	r6, #36
.L_08177eba:
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	movs	r3, #31
	ldr	r1, [sp, #100]
	ands	r3, r0
	adds	r3, #120
	movs	r7, #128
	ldrsh	r0, [r6, r1]
	lsls	r7, r7, #10
	str	r3, [sp, #4]
	lsls	r5, r5, #1
	movs	r2, #128
	movs	r3, #128
	adds	r5, r5, r7
	movs	r1, #1
	lsls	r2, r2, #11
	lsls	r3, r3, #12
	str	r5, [sp, #0]
	bl	sub_0815f000
	ldr	r3, [sp, #100]
	movs	r1, #7
	ldrsh	r0, [r6, r3]
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r9
	bl	sub_0814cd48
	ldr	r7, [sp, #100]
	movs	r5, #1
	ldr	r3, [r7, #20]
	add	r9, r5
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08177eba
.L_08177f08:
	ldr	r0, [sp, #92]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #3
	str	r3, [r2, #0]
.L_08177f16:
	movs	r0, #104
	movs	r1, #31
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r5, [pc, #316]
	str	r3, [sp, #176]
	movs	r2, #0
	mov	r9, r2
.L_08177f2c:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_08177f6a
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_08177f3a
	adds	r1, r3, #3
.L_08177f3a:
	ldr	r3, [pc, #300]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #264]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_08177f6a:
	movs	r7, #1
	add	r9, r7
	adds	r3, #1
	mov	r0, r9
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_08177f2c
	ldr	r6, [pc, #244]
	movs	r1, #0
	mov	r9, r1
.L_08177f80:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_08177fea
	asrs	r3, r3, #3
	adds	r5, r3, #2
	ldr	r3, [pc, #232]
	movs	r2, #0
	mov	fp, r3
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	mov	r8, r2
	lsls	r7, r5, #1
	mov	sl, r3
.L_08177f9c:
	subs	r3, r7, #2
	mov	r4, fp
	ldrh	r1, [r4, r3]
	ldr	r0, [sp, #72]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	adds	r1, r0, r1
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	mov	r4, sl
	subs	r2, r2, r4
	subs	r3, r3, r5
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x213f
	adds	r0, r6, #0
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r6, #4]
	movs	r1, #224
	lsls	r1, r1, #15
	cmp	r3, r1
	ble.n	.L_08177fda
	ldr	r3, [r6, #16]
	negs	r3, r3
	str	r3, [r6, #16]
.L_08177fda:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #4
	bne.n	.L_08177f9c
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_08177fea:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	adds	r6, #28
	cmp	r5, #128
	bne.n	.L_08177f80
	movs	r0, #104
	bl	sub_0801314c
.L_08177ffc:
	ldr	r6, [sp, #80]
	cmp	r6, #119
	ble.n	.L_08178004
	b.n	.L_0817810e
.L_08178004:
	movs	r7, #0
	mov	r9, r7
.L_08178008:
	ldr	r0, [pc, #108]
	mov	r1, r9
	ldrb	r2, [r0, r1]
	ldr	r3, [sp, #80]
	cmp	r3, r2
	bne.n	.L_0817802c
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #128
	ldr	r2, [pc, #96]
	ldr	r3, [pc, #96]
	adds	r0, #192
	lsls	r1, r1, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4c14
	mov	r5, r9
	ldrb	r2, [r4, r5]
.L_0817802c:
	ldr	r6, [sp, #80]
	adds	r3, r2, #1
	cmp	r6, r3
	blt.n	.L_08178102
	adds	r3, #15
	cmp	r6, r3
	bge.n	.L_08178102
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r7, #160
	movs	r0, #0
	lsls	r7, r7, #3
	ldr	r1, [pc, #24]
	adds	r7, #108
	str	r0, [sp, #84]
	adds	r7, r7, r3
	movs	r2, #31
	movs	r3, #10
	mov	fp, r7
	mov	lr, r1
	mov	sl, r2
	mov	r8, r3
	mov	ip, r0
	b.n	.L_08178084
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x02014c80
	.4byte 0x02010000
	.4byte 0xffffe000
	.4byte 0x02015000
	.4byte 0x08197410
	.4byte 0x08198c67
	.4byte 0x7fff7fff
	.2byte 0x0260
	.2byte 0x0300
.L_08178084:
	mov	r4, ip
	mov	r5, fp
	ldrh	r2, [r4, r5]
	mov	r3, sl
	ands	r3, r2
	mov	r7, r8
	lsls	r2, r2, #16
	mov	r0, lr
	subs	r6, r3, r7
	lsrs	r3, r2, #21
	lsrs	r2, r2, #26
	ands	r2, r0
	subs	r5, r2, r7
	movs	r7, #160
	lsls	r7, r7, #19
	adds	r7, #192
	ands	r3, r0
	add	r7, ip
	adds	r4, r3, #0
	ldrh	r3, [r7, #0]
	mov	r0, sl
	ands	r0, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	mov	r1, lr
	ands	r2, r1
	lsrs	r1, r3, #26
	mov	r3, lr
	subs	r4, #20
	ands	r1, r3
	cmp	r6, #0
	bge.n	.L_081780c6
	movs	r6, #0
.L_081780c6:
	cmp	r4, #0
	bge.n	.L_081780cc
	movs	r4, #0
.L_081780cc:
	cmp	r5, #0
	bge.n	.L_081780d2
	movs	r5, #0
.L_081780d2:
	subs	r3, r0, #1
	cmp	r6, r3
	bge.n	.L_081780da
	subs	r0, #2
.L_081780da:
	subs	r3, r2, #1
	cmp	r4, r3
	bge.n	.L_081780e2
	subs	r2, #2
.L_081780e2:
	subs	r3, r1, #1
	cmp	r5, r3
	bge.n	.L_081780ea
	subs	r1, #2
.L_081780ea:
	lsls	r3, r1, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #0]
	ldr	r5, [sp, #84]
	movs	r4, #2
	adds	r5, #1
	add	ip, r4
	str	r5, [sp, #84]
	cmp	r5, #128
	bne.n	.L_08178084
.L_08178102:
	movs	r6, #1
	add	r9, r6
	mov	r7, r9
	cmp	r7, #3
	beq.n	.L_0817810e
	b.n	.L_08178008
.L_0817810e:
	ldr	r0, [sp, #80]
	cmp	r0, #120
	ble.n	.L_0817817c
	cmp	r0, #121
	bne.n	.L_0817812e
	add	r1, sp, #76
	ldr	r3, [pc, #388]
	ldrh	r1, [r1, #0]
	movs	r2, #0
	strh	r1, [r3, #4]
	ldr	r3, [sp, #88]
	add	r4, sp, #28
	str	r2, [r3, #16]
	ldrh	r4, [r4, #0]
	ldr	r5, [sp, #36]
	strh	r4, [r5, #54]
.L_0817812e:
	ldr	r5, [sp, #80]
	cmp	r5, #147
	ble.n	.L_08178136
	b.n	.L_081785f6
.L_08178136:
	adds	r0, r5, #0
	subs	r0, #120
	movs	r1, #6
	bl	sub_08002054
	movs	r7, #192
	ldr	r6, [sp, #92]
	lsls	r7, r7, #3
	adds	r7, #252
	adds	r2, r6, r7
	ldr	r6, [r2, #0]
	movs	r3, #8
	subs	r3, r3, r0
	subs	r3, r6, r3
	str	r3, [r2, #0]
	cmp	r6, #0
	bgt.n	.L_0817815a
	b.n	.L_081785f6
.L_0817815a:
	ldr	r0, [sp, #92]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r5, r0, r1
	movs	r2, #128
	adds	r0, r5, #0
	adds	r1, r6, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #60
	adds	r3, r6, #0
	bl	sub_0818caa8
	b.n	.L_081785f6
.L_0817817c:
	ldr	r3, [sp, #80]
	subs	r3, #4
	cmp	r3, #21
	bhi.n	.L_0817818a
	ldr	r2, [sp, #32]
	adds	r2, #4
	str	r2, [sp, #32]
.L_0817818a:
	ldr	r3, [sp, #80]
	cmp	r3, #107
	ble.n	.L_0817819e
	ldr	r4, [sp, #32]
	subs	r4, #8
	str	r4, [sp, #32]
	cmp	r4, #0
	bge.n	.L_0817819e
	movs	r5, #0
	str	r5, [sp, #32]
.L_0817819e:
	ldr	r7, [sp, #100]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	bne.n	.L_081781ac
	b.n	.L_08178320
.L_081781ac:
	ldr	r0, [sp, #92]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r2, r7, #0
	str	r6, [sp, #12]
	adds	r0, r0, r1
	adds	r2, #36
	mov	r8, r0
	mov	fp, r2
.L_081781be:
	mov	r4, fp
	movs	r3, #0
	ldrsh	r0, [r4, r3]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r5, [pc, #216]
	ldr	r3, [r0, #12]
	mov	sl, r0
	movs	r1, #0
	cmp	r3, r5
	bgt.n	.L_081782ac
	mov	r6, r8
	ldr	r3, [r6, #24]
	movs	r7, #1
	negs	r7, r7
	cmp	r3, r7
	bne.n	.L_081781f0
	ldr	r3, [r0, #8]
	movs	r0, #160
	lsls	r0, r0, #12
	adds	r3, r3, r0
	mov	r2, sl
	str	r3, [r2, #8]
	b.n	.L_081781fc
.L_081781f0:
	mov	r4, sl
	ldr	r3, [r4, #8]
	movs	r5, #128
	lsls	r5, r5, #10
	adds	r3, r3, r5
	str	r3, [r4, #8]
.L_081781fc:
	mov	r6, r8
	ldr	r2, [r6, #24]
	movs	r7, #1
	negs	r7, r7
	cmp	r2, r7
	bne.n	.L_081782b0
	mov	r0, sl
	ldr	r3, [r0, #8]
	movs	r4, #192
	lsls	r4, r4, #14
	cmp	r3, r4
	ble.n	.L_08178216
	movs	r1, #1
.L_08178216:
	cmp	r1, #1
	bne.n	.L_081782b0
	movs	r5, #0
	mov	r6, r8
	str	r5, [r6, #24]
	mov	r1, fp
	movs	r7, #0
	ldrsh	r0, [r1, r7]
	mov	r1, r8
	bl	sub_0815e20c
	movs	r0, #103
	bl	sub_081c0010
	ldr	r3, [pc, #116]
	ldr	r2, [sp, #12]
	str	r5, [sp, #84]
	adds	r7, r2, r3
.L_0817823a:
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	mov	r0, r8
	ldr	r3, [r0, #0]
	movs	r4, #128
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	lsls	r4, r4, #1
	ldr	r3, [r0, #4]
	adds	r0, r6, #0
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	adds	r5, r5, r4
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r7, #24]
	ldr	r1, [sp, #84]
	adds	r7, #28
	adds	r1, #1
	str	r1, [sp, #84]
	cmp	r1, #16
	bne.n	.L_0817823a
	mov	r3, r8
	ldr	r2, [r3, #24]
	b.n	.L_081782b0
	.4byte 0x03001120
	.4byte 0x0063ffff
	.2byte 0x4200
	.2byte 0x0201
.L_081782ac:
	mov	r4, r8
	ldr	r2, [r4, #24]
.L_081782b0:
	cmp	r2, #2
	beq.n	.L_081782b8
	cmp	r2, #7
	bne.n	.L_081782d0
.L_081782b8:
	movs	r3, #32
	mov	r6, fp
	movs	r5, #0
	ldrsh	r0, [r6, r5]
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	mov	r3, r9
	mov	r7, r8
	bl	sub_0814cd48
	ldr	r2, [r7, #24]
.L_081782d0:
	cmp	r2, #6
	bne.n	.L_081782ee
	mov	r2, fp
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	movs	r3, #32
	movs	r2, #1
	str	r3, [sp, #0]
	negs	r2, r2
	mov	r3, r9
	movs	r1, #15
	bl	sub_0814cd48
	mov	r3, r8
	ldr	r2, [r3, #24]
.L_081782ee:
	cmp	r2, #8
	bne.n	.L_08178300
	movs	r3, #148
	mov	r5, sl
	movs	r4, #0
	lsls	r3, r3, #16
	str	r4, [r5, #8]
	str	r3, [r5, #12]
	str	r4, [r5, #16]
.L_08178300:
	ldr	r0, [sp, #12]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r0, r0, r1
	str	r0, [sp, #12]
	ldr	r4, [sp, #100]
	movs	r2, #1
	ldr	r3, [r4, #20]
	movs	r6, #2
	movs	r7, #28
	add	r9, r2
	add	fp, r6
	add	r8, r7
	cmp	r9, r3
	beq.n	.L_08178320
	b.n	.L_081781be
.L_08178320:
	ldr	r5, [sp, #80]
	cmp	r5, #0
	bge.n	.L_08178328
	b.n	.L_08178524
.L_08178328:
	ldr	r2, [pc, #812]
	movs	r6, #255
	ldrh	r3, [r2, #4]
	lsls	r6, r6, #8
	adds	r6, #244
	adds	r3, r3, r6
	strh	r3, [r2, #4]
	movs	r1, #27
	movs	r0, #104
	bl	sub_081963ec
	movs	r7, #0
	ldr	r2, [sp, #92]
	mov	r9, r7
	movs	r0, #26
	movs	r3, #168
	ldr	r7, [sp, #32]
	negs	r0, r0
	movs	r1, #0
	lsls	r3, r3, #3
	movs	r4, #28
	lsls	r6, r5, #10
	mov	sl, r0
	mov	r8, r1
	adds	r5, r2, r3
	mov	fp, r4
.L_0817835c:
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r3, r7, r0
	mov	r1, r9
	mov	r0, r8
	str	r3, [r5, #0]
	str	r0, [r5, #4]
	cmp	r1, #3
	bne.n	.L_0817838a
	subs	r3, #8
	mov	r2, sl
	movs	r0, #104
	str	r3, [r5, #0]
	str	r2, [r5, #4]
	bl	sub_0801314c
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
.L_0817838a:
	ldr	r2, [r5, #0]
	mov	r0, fp
	movs	r1, #56
	ldr	r4, [r5, #4]
	mov	r3, fp
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	subs	r3, r2, r3
	movs	r2, #192
	mov	ip, r3
	lsls	r2, r2, #18
	ldr	r3, [sp, #92]
	mov	lr, r4
	ldr	r4, [r2, #104]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r3, r2
	ldr	r0, [sp, #96]
	mov	r3, lr
	mov	r2, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	movs	r4, #128
	add	r9, r0
	movs	r3, #30
	lsls	r4, r4, #6
	mov	r1, r9
	add	sl, r3
	add	r8, r3
	adds	r7, #8
	adds	r6, r6, r4
	adds	r5, #28
	cmp	r1, #4
	bne.n	.L_0817835c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r5, [sp, #92]
	movs	r2, #0
	movs	r6, #160
	mov	r9, r2
	lsls	r6, r6, #15
.L_081783e0:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #4
	bne.n	.L_081783ee
	movs	r3, #0
	str	r3, [r5, #24]
.L_081783ee:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_08178428
	bl	sub_08014878
	movs	r1, #24
	bl	sub_0800206c
	ldr	r4, [sp, #32]
	mov	r7, r9
	lsls	r3, r7, #1
	adds	r0, r0, r4
	subs	r0, r0, r3
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	str	r6, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	ldr	r0, [pc, #576]
	negs	r3, r3
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [pc, #572]
	str	r3, [r5, #16]
	ldr	r3, [r5, #24]
.L_08178428:
	cmp	r3, #0
	blt.n	.L_08178452
	ldr	r4, [sp, #92]
	movs	r7, #184
	lsls	r7, r7, #5
	movs	r2, #2
	ldrsh	r1, [r5, r2]
	adds	r0, r4, r7
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	movs	r3, #16
	bl	sub_0818caa8
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
.L_08178452:
	movs	r1, #1
	movs	r0, #192
	add	r9, r1
	lsls	r0, r0, #10
	mov	r2, r9
	adds	r6, r6, r0
	adds	r5, #28
	cmp	r2, #12
	bne.n	.L_081783e0
	movs	r0, #104
	movs	r1, #31
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r4, [sp, #92]
	movs	r7, #168
	str	r3, [sp, #176]
	lsls	r7, r7, #3
	movs	r3, #0
	mov	r9, r3
	movs	r6, #0
	adds	r5, r4, r7
.L_08178482:
	ldr	r2, [r5, #0]
	movs	r3, #120
	subs	r3, r3, r2
	adds	r5, #28
	cmp	r3, #0
	ble.n	sub_081784a6
	str	r3, [sp, #0]
	movs	r3, #30
	str	r3, [sp, #4]
	ldr	r3, [sp, #92]
	movs	r7, #156
	lsls	r7, r7, #6
	adds	r1, r3, r7
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #96]
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r9, r0
	mov	r1, r9
	adds	r6, #30
	cmp	r1, #4
	bne.n	.L_08178482
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #104
	movs	r1, #47
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r6, [pc, #412]
	str	r3, [sp, #176]
	ldr	r5, [pc, #412]
	movs	r2, #0
	mov	r9, r2
.L_081784d0:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08178512
	asrs	r0, r0, #3
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r3, [sp, #72]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #96]
	ldr	r4, [sp, #176]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08178512:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r5, #28
	cmp	r1, #96
	bne.n	.L_081784d0
	movs	r0, #104
	bl	sub_0801314c
.L_08178524:
	movs	r0, #32
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #312]
	ldr	r3, [sp, #104]
	movs	r4, #220
	ands	r3, r2
	movs	r2, #6
	orrs	r3, r2
	ldr	r2, [pc, #304]
	lsls	r4, r4, #6
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #92]
	str	r3, [sp, #104]
	adds	r3, r2, r4
	add	r2, sp, #104
	adds	r7, r0, #0
	str	r3, [r2, #4]
	movs	r3, #9
	str	r3, [r7, #0]
	ldr	r3, [pc, #280]
	mov	r5, r8
	str	r3, [r7, #8]
	str	r2, [r7, #16]
	str	r5, [r7, #12]
	ldr	r0, [sp, #100]
	movs	r6, #0
	ldr	r3, [r0, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_081785f6
	ldr	r1, [sp, #92]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r6, r1, r2
.L_08178578:
	ldr	r1, [r6, #24]
	cmp	r1, #17
	bhi.n	.L_081785dc
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r5, r3, #7
	adds	r2, r3, #0
	subs	r5, r5, r3
	movs	r3, #128
	lsls	r3, r3, #4
	lsls	r5, r5, #3
	adds	r5, r5, r3
	subs	r2, #64
	adds	r3, r1, #1
	str	r3, [r6, #24]
	str	r2, [r7, #20]
	bl	sub_08014de4
	ldr	r0, [r6, #0]
	ldr	r1, [r6, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #56
	subs	r1, #56
	movs	r2, #0
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	bl	sub_08015160
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_08002054
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	movs	r0, #192
	lsls	r0, r0, #8
	bl	sub_08015024
	ldr	r0, [pc, #168]
	mov	r1, r8
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081785dc:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	ldr	r5, [sp, #100]
	movs	r4, #1
	ldr	r3, [r5, #20]
	add	r9, r4
	adds	r6, #28
	cmp	r9, r3
	bne.n	.L_08178578
.L_081785f6:
	movs	r0, #8
	bl	sub_08158d68
	bl	sub_081434f8
	movs	r7, #240
	ldr	r6, [sp, #92]
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r6, r7
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #80]
	adds	r0, #1
	str	r0, [sp, #80]
	cmp	r0, #180
	beq.n	.L_08178622
	bl	.L_08177c28
.L_08178622:
	ldr	r2, [sp, #100]
	movs	r1, #0
	ldr	r0, [r2, #20]
	mov	r9, r1
	cmp	r0, #0
	beq.n	.L_08178638
	mov	ip, r0
.L_08178630:
	movs	r3, #1
	add	r9, r3
	cmp	r9, ip
	bne.n	.L_08178630
.L_08178638:
	movs	r0, #188
	bl	sub_0801314c
	ldr	r0, [pc, #60]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #324
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001120
	.4byte 0xffffc000
	.4byte 0xffff0000
	.4byte 0x08197410
	.4byte 0x02014200
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x08199210
	.4byte 0x08143001
