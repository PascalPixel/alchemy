.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08020010, 0x08020010
	.set sub_08020030, 0x08020030
	.set sub_08020048, 0x08020048
	.set sub_08118040, 0x08118040
	.set sub_08118070, 0x08118070
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180d8, 0x081180d8
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0814d2e8, 0x0814d2e8
	.set sub_0814d344, 0x0814d344
	.set sub_0814d3ac, 0x0814d3ac
	.set sub_0814d9a8, 0x0814d9a8
	.set sub_0814da0a, 0x0814da0a
	.set sub_0814df02, 0x0814df02
	.set sub_0814e19e, 0x0814e19e
	.set sub_0814e436, 0x0814e436
	.set sub_0814ea58, 0x0814ea58
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_08163c2c, 0x08163c2c
	.set sub_0816442c, 0x0816442c
	.set sub_0816467c, 0x0816467c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_0814ce30
Overlay_0814ce30:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #104
	str	r0, [sp, #68]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r1, #195
	str	r0, [sp, #64]
	lsls	r1, r1, #1
	ldr	r3, [r3, #96]
	adds	r1, #255
	movs	r0, #8
	movs	r2, #1
	str	r3, [sp, #60]
	bl	sub_08152404
	movs	r1, #0
	ldr	r2, [pc, #168]
	mov	sl, r1
	movs	r1, #128
	movs	r0, #127
	lsls	r1, r1, #3
.L_0814ce68:
	mov	r3, sl
	ands	r3, r0
	strb	r3, [r2, #0]
	movs	r3, #1
	add	sl, r3
	adds	r2, #1
	cmp	sl, r1
	bne.n	.L_0814ce68
	movs	r4, #0
	movs	r0, #127
	mov	sl, r4
	mov	r8, r0
	mov	r9, r4
.L_0814ce82:
	movs	r7, #0
	mov	r6, r9
.L_0814ce86:
	bl	sub_08014878
	mov	r1, r8
	adds	r5, r0, #0
	ands	r5, r1
	bl	sub_08014878
	ldr	r3, [pc, #112]
	mov	r2, r8
	ands	r0, r2
	adds	r0, r6, r0
	adds	r5, r6, r5
	adds	r0, r0, r3
	adds	r5, r5, r3
	ldrb	r2, [r0, #0]
	ldrb	r3, [r5, #0]
	adds	r7, #1
	strb	r3, [r0, #0]
	strb	r2, [r5, #0]
	cmp	r7, #128
	bne.n	.L_0814ce86
	movs	r0, #1
	add	sl, r0
	movs	r4, #128
	mov	r1, sl
	add	r9, r4
	cmp	r1, #8
	bne.n	.L_0814ce82
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r2, [sp, #64]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #72]
	bl	sub_081963ec
	b.n	.L_0814cf10
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x02010000
	.2byte 0x0176
	.2byte 0x0000
.L_0814cf10:
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r4, sp
	adds	r4, #72
	str	r4, [sp, #36]
	str	r3, [r4, #4]
	ldr	r0, [sp, #64]
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
	ldr	r0, [pc, #784]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #128
	ldr	r3, [pc, #776]
	lsls	r2, r2, #19
	adds	r2, #40
	str	r3, [r2, #0]
	movs	r0, #90
	ldr	r5, [sp, #64]
	movs	r4, #0
	negs	r0, r0
	mov	sl, r4
	movs	r7, #7
	mov	r8, r0
	movs	r6, #0
.L_0814cf58:
	mov	r1, sl
	cmp	r1, #4
	bgt.n	.L_0814cf6a
	str	r6, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #104
	b.n	.L_0814cf76
.L_0814cf6a:
	mov	r2, r8
	str	r2, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #108
.L_0814cf76:
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #4
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r4, #1
	adds	r3, #16
	add	sl, r4
	str	r3, [r5, #24]
	mov	r0, sl
	movs	r3, #20
	add	r8, r3
	adds	r6, #20
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_0814cf58
	ldr	r2, [sp, #64]
	movs	r3, #224
	movs	r1, #0
	lsls	r3, r3, #1
	mov	sl, r1
	adds	r5, r2, r3
.L_0814cfac:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #64
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r4, #1
	negs	r3, r3
	add	sl, r4
	subs	r3, #8
	mov	r0, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_0814cfac
	ldr	r2, [sp, #64]
	movs	r3, #224
	movs	r1, #0
	lsls	r3, r3, #2
	mov	sl, r1
	movs	r6, #0
	adds	r5, r2, r3
.L_0814cfec:
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #200
	movs	r4, #1
	negs	r3, r3
	add	sl, r4
	lsls	r3, r3, #9
	mov	r0, sl
	str	r3, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_0814cfec
	ldr	r1, [sp, #64]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #240
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r3, [pc, #548]
	movs	r4, #0
	str	r3, [sp, #52]
	str	r4, [sp, #48]
	str	r4, [sp, #56]
.L_0814d032:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	ldr	r3, [pc, #536]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0814d064
	ldr	r0, [sp, #56]
	cmp	r0, #190
	ble.n	.L_0814d064
	movs	r1, #30
	adds	r1, #255
	cmp	r0, r1
	bgt.n	.L_0814d064
	movs	r1, #128
	ldr	r3, [pc, #512]
	ldr	r0, [sp, #60]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x228f
	lsls	r2, r2, #1
	str	r2, [sp, #56]
.L_0814d064:
	ldr	r3, [sp, #56]
	cmp	r3, #224
	bne.n	.L_0814d076
	ldr	r4, [sp, #64]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r4, r0
	movs	r3, #0
	str	r3, [r2, #0]
.L_0814d076:
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156e8
	ldr	r1, [sp, #56]
	cmp	r1, #31
	bne.n	.L_0814d0c4
	ldr	r2, [sp, #64]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r3, r2, r4
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r0, #157
	bl	sub_081c0010
	ldr	r1, [sp, #68]
	movs	r0, #0
	ldr	r3, [r1, #20]
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_0814d0c4
	movs	r5, #36
.L_0814d0ac:
	ldr	r2, [sp, #68]
	movs	r1, #6
	ldrsh	r0, [r5, r2]
	bl	sub_08118088
	ldr	r0, [sp, #68]
	movs	r4, #1
	ldr	r3, [r0, #20]
	add	sl, r4
	adds	r5, #2
	cmp	sl, r3
	bne.n	.L_0814d0ac
.L_0814d0c4:
	ldr	r1, [sp, #56]
	cmp	r1, #72
	bne.n	.L_0814d0d0
	movs	r0, #136
	bl	sub_081c0010
.L_0814d0d0:
	ldr	r2, [sp, #56]
	cmp	r2, #140
	bne.n	.L_0814d0dc
	movs	r0, #156
	bl	sub_081c0010
.L_0814d0dc:
	ldr	r3, [sp, #48]
	ldr	r0, [sp, #52]
	movs	r4, #128
	lsls	r4, r4, #7
	adds	r3, r3, r4
	movs	r1, #128
	adds	r0, r0, r3
	lsls	r1, r1, #15
	str	r3, [sp, #48]
	str	r0, [sp, #52]
	cmp	r0, r1
	ble.n	.L_0814d0f6
	str	r1, [sp, #52]
.L_0814d0f6:
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #16
	ldr	r2, [sp, #52]
	bl	sub_0816442c
	ldr	r0, [sp, #56]
	subs	r0, #48
	cmp	r0, #48
	bhi.n	.L_0814d142
	movs	r1, #24
	bl	sub_08002054
	movs	r1, #3
	bl	sub_08002064
	movs	r4, #238
	ldr	r2, [sp, #64]
	ldr	r6, [pc, #320]
	lsls	r4, r4, #7
	adds	r5, r0, #0
	adds	r4, #232
	adds	r3, r2, r4
	lsls	r5, r5, #1
	ldrb	r1, [r6, r5]
	ldr	r0, [r3, #0]
	bl	sub_08020030
	movs	r1, #238
	ldr	r0, [sp, #64]
	lsls	r1, r1, #7
	adds	r1, #236
	adds	r3, r0, r1
	adds	r5, #1
	ldr	r0, [r3, #0]
	ldrb	r1, [r6, r5]
	bl	sub_08020030
.L_0814d142:
	ldr	r3, [sp, #56]
	subs	r3, #72
	cmp	r3, #55
	bhi.n	.L_0814d1c2
	ldr	r3, [sp, #64]
	movs	r4, #224
	movs	r2, #0
	lsls	r4, r4, #2
	mov	sl, r2
	adds	r6, r3, r4
.L_0814d156:
	ldr	r0, [sp, #56]
	mov	r3, sl
	adds	r3, #72
	cmp	r0, r3
	blt.n	.L_0814d1b6
	ldr	r5, [r6, #4]
	ldr	r1, [pc, #252]
	cmp	r5, r1
	bgt.n	.L_0814d1b6
	add	r0, sl
	cmp	r0, #0
	bge.n	.L_0814d170
	adds	r0, #3
.L_0814d170:
	movs	r1, #5
	asrs	r0, r0, #2
	bl	sub_08002064
	ldr	r4, [pc, #232]
	lsls	r1, r0, #1
	ldrh	r1, [r4, r1]
	ldr	r4, [sp, #64]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	adds	r1, r4, r1
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r1, r4
	ldr	r4, [pc, #216]
	asrs	r3, r5, #16
	ldrb	r5, [r4, r0]
	lsrs	r4, r5, #1
	subs	r2, r2, r4
	ldr	r4, [pc, #212]
	ldrb	r4, [r4, r0]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #60]
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r6, #0
	movs	r1, #64
	lsls	r2, r2, #5
	bl	sub_08138086
.L_0814d1b6:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r6, #28
	cmp	r1, #16
	bne.n	.L_0814d156
.L_0814d1c2:
	ldr	r2, [sp, #56]
	cmp	r2, #128
	bne.n	.L_0814d22e
	ldr	r4, [sp, #64]
	movs	r0, #224
	movs	r3, #0
	lsls	r0, r0, #1
	mov	sl, r3
	movs	r6, #255
	adds	r5, r4, r0
.L_0814d1d6:
	bl	sub_08014878
	movs	r1, #96
	bl	sub_0800206c
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #88
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	negs	r0, r0
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r1, #1
	negs	r3, r3
	add	sl, r1
	subs	r3, #16
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #48
	bne.n	.L_0814d1d6
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	movs	r3, #0
	str	r3, [r2, #0]
.L_0814d22e:
	ldr	r3, [sp, #56]
	subs	r3, #128
	str	r3, [sp, #44]
	cmp	r3, #96
	bls.n	.L_0814d23a
	b.n	.L_0814d3ba
.L_0814d23a:
	str	r3, [sp, #40]
	cmp	r3, #80
	ble.n	.L_0814d270
	movs	r4, #80
	str	r4, [sp, #40]
	b.n	.L_0814d27e
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0xffffe000
	.4byte 0xffc00000
	.4byte 0x03001150
	.4byte 0x03000258
	.4byte 0x081981fc
	.4byte 0x0067ffff
	.4byte 0x08198202
	.4byte 0x0819820c
	.2byte 0x8211
	.2byte 0x0819
.L_0814d270:
	ldr	r0, [sp, #64]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
.L_0814d27e:
	ldr	r7, [sp, #64]
	movs	r2, #0
	mov	sl, r2
.L_0814d284:
	ldr	r3, [r7, #24]
	ldr	r4, [sp, #40]
	cmp	r4, r3
	bgt.n	.L_0814d28e
	b.n	sub_0814d3ac
.L_0814d28e:
	ldr	r0, [sp, #64]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r0, r0, r1
	mov	r2, sl
	mov	r9, r0
	cmp	r2, #5
	ble.n	.L_0814d2a8
	ldr	r4, [sp, #64]
	movs	r0, #220
	lsls	r0, r0, #4
	adds	r4, r4, r0
	mov	r9, r4
.L_0814d2a8:
	ldr	r1, [sp, #40]
	subs	r2, r1, r3
	ldr	r3, [r7, #16]
	adds	r4, r3, #0
	muls	r4, r2
	mov	fp, r4
	mov	r6, fp
	cmp	r4, #184
	ble.n	.L_0814d2c0
.L_0814d2ba:
	subs	r6, #64
	cmp	r6, #184
	bgt.n	.L_0814d2ba
.L_0814d2c0:
	cmp	r6, #119
	bgt.n	sub_0814d2e8
	movs	r1, #1
	mov	r0, sl
	ands	r0, r1
	movs	r1, #24
	ldr	r3, [r7, #4]
	ldr	r2, [r7, #0]
	str	r1, [sp, #0]
	movs	r1, #8
	str	r1, [sp, #4]
	ldr	r1, [sp, #36]
	lsls	r0, r0, #2
	subs	r3, r3, r6
	ldr	r4, [r0, r1]
	subs	r3, #8
	ldr	r0, [sp, #60]
	mov	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4653
	movs	r4, #1
	ands	r3, r4
	lsls	r3, r3, #2
	str	r3, [sp, #32]
	movs	r2, #0
	mov	r8, r2
.L_0814d2f6:
	ldr	r1, [r7, #4]
	mov	r0, r8
	lsls	r3, r0, #6
	subs	r2, r1, r6
	adds	r5, r2, r3
	movs	r3, #64
	negs	r3, r3
	movs	r2, #0
	movs	r0, #64
	cmp	r5, r3
	blt.n	sub_0814d344
	cmp	r5, #0
	bge.n	.L_0814d31e
	negs	r2, r5
	lsls	r3, r2, #1
	adds	r0, r5, #0
	adds	r3, r3, r2
	lsls	r2, r3, #3
	adds	r0, #64
	movs	r5, #0
.L_0814d31e:
	adds	r3, r5, r0
	cmp	r3, r1
	ble.n	.L_0814d328
	subs	r3, r3, r1
	subs	r0, r0, r3
.L_0814d328:
	mov	r4, r9
	movs	r3, #24
	adds	r1, r4, r2
	ldr	r2, [r7, #0]
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	ldr	r3, [sp, #36]
	ldr	r0, [sp, #32]
	adds	r1, #192
	ldr	r4, [r0, r3]
	ldr	r0, [sp, #60]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2401
	add	r8, r4
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L_0814d2f6
	mov	r6, sl
	ands	r6, r4
	cmp	r6, #0
	beq.n	sub_0814d3ac
	ldr	r5, [r7, #4]
	mov	r1, fp
	movs	r2, #127
	subs	r3, r5, r1
	ands	r3, r2
	subs	r3, #16
	movs	r1, #3
	mov	r0, sl
	mov	r8, r3
	bl	sub_08002064
	ldr	r3, [pc, #336]
	adds	r1, r0, #0
	ldrb	r4, [r3, r1]
	mov	r2, r8
	adds	r3, r2, r4
	mov	ip, r4
	cmp	r3, r5
	ble.n	.L_0814d380
	subs	r3, r3, r5
	subs	r4, r4, r3
.L_0814d380:
	cmp	r4, #0
	ble.n	sub_0814d3ac
	ldr	r2, [pc, #316]
	lsls	r3, r1, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #64]
	movs	r2, #224
	adds	r1, r3, r1
	lsls	r2, r2, #3
	mov	r3, ip
	adds	r1, r1, r2
	ldr	r2, [r7, #0]
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	ldr	r3, [sp, #36]
	lsls	r0, r6, #2
	ldr	r4, [r0, r3]
	adds	r2, #8
	ldr	r0, [sp, #60]
	mov	r3, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2401
	add	sl, r4
	mov	r0, sl
	adds	r7, #28
	cmp	r0, #10
	beq.n	.L_0814d3ba
	b.n	.L_0814d284
.L_0814d3ba:
	ldr	r1, [sp, #44]
	cmp	r1, #95
	bhi.n	.L_0814d474
	ldr	r3, [sp, #64]
	movs	r4, #224
	movs	r2, #0
	lsls	r4, r4, #2
	mov	sl, r2
	movs	r6, #255
	adds	r5, r3, r4
.L_0814d3ce:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0814d464
	movs	r1, #5
	mov	r0, sl
	bl	sub_08002064
	ldr	r2, [pc, #232]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #64]
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	ldr	r4, [pc, #212]
	ldrb	r4, [r4, r0]
	str	r4, [sp, #0]
	ldr	r4, [pc, #212]
	ldrb	r0, [r4, r0]
	ldr	r4, [sp, #72]
	str	r0, [sp, #4]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	movs	r0, #128
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #7
	movs	r1, #240
	adds	r2, r2, r3
	lsls	r1, r1, #15
	adds	r3, r3, r0
	str	r2, [r5, #4]
	str	r3, [r5, #16]
	cmp	r2, r1
	bls.n	.L_0814d462
	ldr	r2, [sp, #56]
	cmp	r2, #159
	bgt.n	.L_0814d462
	bl	sub_08014878
	movs	r1, #96
	bl	sub_0800206c
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #88
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	negs	r0, r0
	lsls	r0, r0, #11
	str	r0, [r5, #16]
.L_0814d462:
	ldr	r3, [r5, #24]
.L_0814d464:
	adds	r3, #1
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #28
	cmp	r4, #32
	bne.n	.L_0814d3ce
.L_0814d474:
	ldr	r3, [sp, #56]
	subs	r3, #224
	cmp	r3, #23
	bhi.n	.L_0814d508
	ldr	r0, [sp, #56]
	movs	r3, #3
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_0814d508
	ldr	r2, [pc, #52]
	movs	r4, #160
	movs	r1, #0
	lsls	r4, r4, #19
	mov	sl, r1
	mov	r8, r2
.L_0814d492:
	ldrh	r3, [r4, #0]
	movs	r7, #31
	ands	r7, r3
	lsls	r3, r3, #16
	mov	r0, r8
	lsrs	r6, r3, #21
	lsrs	r5, r3, #26
	ands	r6, r0
	ands	r5, r0
	adds	r0, r7, r6
	adds	r0, r0, r5
	movs	r1, #3
	str	r4, [sp, #8]
	bl	sub_08002054
	ldr	r4, [sp, #8]
	cmp	r7, r0
	ble.n	.L_0814d4d4
	subs	r7, #1
	b.n	.L_0814d4d4
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x0819821c
	.4byte 0x08198216
	.4byte 0x08198220
	.4byte 0x0819822a
	.2byte 0x822f
	.2byte 0x0819
.L_0814d4d4:
	cmp	r7, r0
	bge.n	.L_0814d4da
	adds	r7, #1
.L_0814d4da:
	cmp	r6, r0
	ble.n	.L_0814d4e0
	subs	r6, #1
.L_0814d4e0:
	cmp	r6, r0
	bge.n	.L_0814d4e6
	adds	r6, #1
.L_0814d4e6:
	cmp	r5, r0
	ble.n	.L_0814d4ec
	subs	r5, #1
.L_0814d4ec:
	cmp	r5, r0
	bge.n	.L_0814d4f2
	adds	r5, #1
.L_0814d4f2:
	lsls	r2, r6, #5
	lsls	r3, r5, #10
	movs	r1, #1
	orrs	r3, r2
	add	sl, r1
	orrs	r3, r7
	mov	r2, sl
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r2, #64
	bne.n	.L_0814d492
.L_0814d508:
	ldr	r3, [sp, #44]
	cmp	r3, #172
	bhi.n	.L_0814d5e8
	ldr	r0, [sp, #68]
	movs	r4, #0
	ldr	r2, [r0, #20]
	mov	sl, r4
	cmp	r2, #0
	beq.n	.L_0814d5e8
	add	r1, sp, #80
	movs	r2, #36
	mov	r8, r1
	add	r6, sp, #92
	mov	r9, r4
	mov	fp, r2
.L_0814d526:
	ldr	r1, [sp, #68]
	mov	r3, fp
	ldrsh	r0, [r3, r1]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	mov	r1, r8
	ldr	r3, [r2, #8]
	adds	r0, r6, #0
	str	r3, [r6, #0]
	movs	r7, #0
	ldr	r3, [r2, #12]
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	str	r3, [r6, #8]
	bl	sub_0815e1ec
	mov	r2, r9
	lsls	r3, r2, #3
	ldr	r4, [sp, #64]
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #224
	adds	r3, r3, r4
	lsls	r0, r0, #1
	adds	r5, r3, r0
.L_0814d55a:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	bne.n	.L_0814d582
	bl	sub_08014878
	ldr	r3, [sp, #80]
	movs	r1, #15
	ands	r0, r1
	adds	r3, r3, r0
	subs	r3, #8
	str	r3, [r5, #0]
	bl	sub_08014878
	ldr	r3, [sp, #84]
	movs	r2, #15
	ands	r0, r2
	adds	r3, r3, r0
	subs	r3, #40
	str	r3, [r5, #4]
	ldr	r0, [r5, #24]
.L_0814d582:
	cmp	r0, #4
	bhi.n	.L_0814d5b2
	ldr	r2, [pc, #412]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #64]
	ldr	r2, [r5, #0]
	adds	r1, r3, r1
	ldr	r3, [pc, #404]
	movs	r4, #224
	ldrb	r0, [r3, r0]
	ldr	r3, [r5, #4]
	lsls	r4, r4, #3
	adds	r1, r1, r4
	lsrs	r4, r0, #1
	subs	r2, r2, r4
	subs	r3, r3, r4
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #72]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69a8
.L_0814d5b2:
	adds	r3, r0, #1
	str	r3, [r5, #24]
	ldr	r0, [sp, #56]
	cmp	r0, #199
	bgt.n	.L_0814d5cc
	cmp	r3, #5
	bne.n	.L_0814d5cc
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	negs	r3, r3
	str	r3, [r5, #24]
.L_0814d5cc:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #6
	bne.n	.L_0814d55a
	ldr	r4, [sp, #68]
	movs	r2, #2
	add	fp, r2
	ldr	r2, [r4, #20]
	movs	r3, #1
	movs	r1, #6
	add	sl, r3
	add	r9, r1
	cmp	sl, r2
	bne.n	.L_0814d526
.L_0814d5e8:
	ldr	r0, [sp, #56]
	cmp	r0, #232
	ble.n	.L_0814d6b6
	ldr	r1, [pc, #316]
	lsls	r3, r0, #1
	adds	r6, r3, r1
	movs	r2, #0
	movs	r3, #0
	mov	sl, r2
	mov	ip, r3
	adds	r4, r6, #0
.L_0814d5fe:
	movs	r7, #0
.L_0814d600:
	cmp	r4, #127
	bhi.n	.L_0814d642
	movs	r5, #7
	adds	r0, r4, #0
	ands	r0, r5
	lsls	r3, r0, #5
	ldr	r1, [pc, #288]
	add	r3, sl
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r3, r3, r1
	ldrb	r1, [r3, #0]
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_0814d620
	adds	r3, r4, #7
.L_0814d620:
	asrs	r3, r3, #3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_0814d62a
	adds	r2, r1, #7
.L_0814d62a:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r2, [sp, #60]
	ands	r1, r5
	lsls	r3, r3, #3
	adds	r3, r3, r1
	adds	r3, r2, r3
	mov	r0, ip
	strb	r0, [r3, #0]
.L_0814d642:
	adds	r7, #1
	cmp	r7, #4
	bne.n	.L_0814d600
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r4, #1
	cmp	r2, #32
	bne.n	.L_0814d5fe
	movs	r4, #0
	movs	r3, #0
	mov	lr, r4
	mov	sl, r3
	adds	r4, r6, #1
.L_0814d65e:
	movs	r7, #0
	mov	ip, r4
.L_0814d662:
	cmp	r4, #127
	bhi.n	.L_0814d6a4
	movs	r5, #7
	mov	r0, ip
	ands	r0, r5
	lsls	r3, r0, #5
	ldr	r1, [pc, #192]
	add	r3, sl
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r3, r3, r1
	ldrb	r1, [r3, #0]
	mov	r3, ip
	cmp	r3, #0
	bge.n	.L_0814d682
	adds	r3, #7
.L_0814d682:
	asrs	r3, r3, #3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_0814d68c
	adds	r2, r1, #7
.L_0814d68c:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r2, [sp, #60]
	ands	r1, r5
	lsls	r3, r3, #3
	adds	r3, r3, r1
	adds	r3, r2, r3
	mov	r0, lr
	strb	r0, [r3, #0]
.L_0814d6a4:
	adds	r7, #1
	cmp	r7, #4
	bne.n	.L_0814d662
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r4, #1
	cmp	r2, #32
	bne.n	.L_0814d65e
.L_0814d6b6:
	ldr	r3, [sp, #56]
	subs	r3, #161
	cmp	r3, #62
	bhi.n	.L_0814d734
	ldr	r4, [sp, #68]
	movs	r3, #0
	ldr	r2, [r4, #20]
	mov	sl, r3
	cmp	r2, #0
	beq.n	.L_0814d738
	movs	r7, #36
.L_0814d6cc:
	mov	r0, sl
	ldr	r1, [sp, #56]
	lsls	r3, r0, #3
	adds	r3, #160
	cmp	r1, r3
	ble.n	.L_0814d718
	ldr	r2, [sp, #68]
	ldrsh	r0, [r7, r2]
	bl	sub_08118098
	adds	r6, r0, #0
	ldr	r2, [r6, #0]
	movs	r4, #128
	ldr	r3, [r2, #12]
	lsls	r4, r4, #12
	movs	r1, #128
	adds	r3, r3, r4
	lsls	r1, r1, #16
	str	r3, [r2, #12]
	cmp	r3, r1
	ble.n	.L_0814d6f8
	str	r1, [r2, #12]
.L_0814d6f8:
	movs	r3, #0
	movs	r5, #0
	str	r3, [r2, #72]
	b.n	.L_0814d708
.L_0814d700:
	movs	r1, #5
	bl	sub_08020030
	adds	r5, #1
.L_0814d708:
	ldr	r0, [r6, #0]
	adds	r1, r5, #0
	bl	sub_081180d8
	cmp	r0, #0
	bne.n	.L_0814d700
	ldr	r0, [sp, #68]
	ldr	r2, [r0, #20]
.L_0814d718:
	movs	r1, #1
	add	sl, r1
	adds	r7, #2
	cmp	sl, r2
	bne.n	.L_0814d6cc
	b.n	.L_0814d738
	.4byte 0x08198234
	.4byte 0x0819823e
	.4byte 0xfffffe10
	.2byte 0x0000
	.2byte 0x0201
.L_0814d734:
	ldr	r3, [sp, #68]
	ldr	r2, [r3, #20]
.L_0814d738:
	movs	r4, #0
	mov	sl, r4
	cmp	r2, #0
	beq.n	.L_0814d7a4
	movs	r5, #143
	movs	r7, #8
	movs	r6, #36
	lsls	r5, r5, #1
.L_0814d748:
	ldr	r0, [sp, #56]
	cmp	r0, r5
	bne.n	.L_0814d766
	ldr	r1, [sp, #68]
	ldrsh	r0, [r6, r1]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r2, #12]
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r2, #72]
.L_0814d766:
	ldr	r4, [sp, #56]
	adds	r3, r5, #0
	adds	r3, #16
	cmp	r4, r3
	bne.n	.L_0814d794
	ldr	r1, [sp, #68]
	mov	r3, sl
	ldrsh	r0, [r6, r1]
	movs	r2, #1
	movs	r1, #7
	negs	r2, r2
	str	r7, [sp, #0]
	bl	sub_0814cd48
	movs	r0, #134
	bl	sub_081c0010
	movs	r0, #238
	ldr	r4, [sp, #64]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r3, r4, r0
	str	r7, [r3, #0]
.L_0814d794:
	ldr	r3, [sp, #68]
	movs	r1, #1
	ldr	r2, [r3, #20]
	add	sl, r1
	adds	r6, #2
	adds	r5, #5
	cmp	sl, r2
	bne.n	.L_0814d748
.L_0814d7a4:
	ldr	r4, [sp, #56]
	movs	r0, #151
	lsls	r0, r0, #1
	cmp	r4, r0
	beq.n	.L_0814d7b0
	b.n	.L_0814d8e2
.L_0814d7b0:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r2, [sp, #64]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #132]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #64]
	movs	r2, #236
	lsls	r2, r2, #5
	adds	r1, r4, r2
	movs	r3, #1
	movs	r2, #1
	ldr	r0, [pc, #116]
	bl	sub_08157cf4
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #23
	movs	r0, #188
	str	r3, [sp, #72]
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r4, [sp, #36]
	ldr	r3, [r3, #0]
	movs	r2, #128
	str	r3, [r4, #4]
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #48
	strh	r3, [r2, #0]
	adds	r2, #8
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r0, [sp, #64]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	movs	r1, #200
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r0, [pc, #28]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r0, [sp, #68]
	movs	r4, #0
	b.n	.L_0814d858
	movs	r0, r0
	.4byte 0x00003f46
	.4byte 0x00000080
	.4byte 0x0000015b
	.4byte 0x00000184
	.2byte 0x3001
	.2byte 0x0814
.L_0814d858:
	ldr	r2, [r0, #20]
	mov	sl, r4
	cmp	r2, #0
	beq.n	.L_0814d8e2
	movs	r1, #36
	mov	r9, r1
.L_0814d864:
	ldr	r4, [sp, #68]
	mov	r2, r9
	ldrsh	r0, [r2, r4]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r1, [sp, #64]
	mov	r8, r0
	mov	r0, sl
	lsls	r2, r0, #2
	add	r2, sl
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	movs	r7, #0
	adds	r6, r3, r1
.L_0814d884:
	mov	r2, r8
	ldr	r3, [r2, #8]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	str	r3, [r6, #8]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #52
	adds	r5, r7, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r0, r0, #2
	str	r0, [r6, #12]
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	movs	r4, #128
	ands	r3, r0
	lsls	r4, r4, #9
	adds	r3, r3, r4
	str	r3, [r6, #16]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r7, #1
	lsls	r0, r0, #2
	movs	r3, #0
	str	r0, [r6, #20]
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #10
	bne.n	.L_0814d884
	ldr	r3, [sp, #68]
	movs	r1, #1
	ldr	r2, [r3, #20]
	movs	r0, #2
	add	sl, r1
	add	r9, r0
	cmp	sl, r2
	bne.n	.L_0814d864
.L_0814d8e2:
	ldr	r4, [sp, #56]
	movs	r0, #46
	adds	r0, #255
	cmp	r4, r0
	bgt.n	.L_0814d8ee
	b.n	.L_0814da4a
.L_0814d8ee:
	movs	r1, #0
	mov	sl, r1
	cmp	r2, #0
	bne.n	.L_0814d8f8
	b.n	.L_0814da4a
.L_0814d8f8:
	mov	r2, sp
	adds	r2, #92
	mov	r3, sp
	adds	r3, #80
	str	r2, [sp, #28]
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #492]
	movs	r2, #151
	str	r3, [sp, #24]
	movs	r4, #36
	lsls	r2, r2, #1
	movs	r3, #0
	str	r4, [sp, #20]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
	adds	r0, r0, r1
	mov	fp, r0
.L_0814d91a:
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #16]
	cmp	r4, r0
	blt.n	sub_0814d9a8
	ldr	r1, [sp, #12]
	movs	r2, #157
	lsls	r2, r2, #1
	adds	r3, r1, r2
	cmp	r4, r3
	bge.n	sub_0814d9a8
	ldr	r4, [sp, #20]
	ldr	r2, [sp, #68]
	mov	r3, fp
	ldrsh	r0, [r4, r2]
	lsrs	r6, r3, #31
	bl	sub_08118098
	ldr	r2, [r0, #0]
	ldr	r4, [sp, #28]
	ldr	r3, [r2, #8]
	add	r6, fp
	str	r3, [r4, #0]
	movs	r3, #0
	str	r3, [r4, #4]
	asrs	r6, r6, #1
	ldr	r3, [r2, #16]
	lsls	r5, r6, #4
	str	r3, [r4, #8]
	ldr	r0, [sp, #24]
	subs	r5, r5, r6
	mov	r8, r0
	mov	r1, r8
	ldr	r0, [sp, #28]
	bl	sub_0815e1ec
	mov	r1, r8
	ldr	r2, [r1, #0]
	lsls	r5, r5, #5
	asrs	r2, r2, #1
	str	r2, [r1, #0]
	ldr	r3, [sp, #64]
	movs	r4, #224
	adds	r5, r3, r5
	ldr	r3, [r1, #4]
	lsls	r4, r4, #3
	adds	r5, r5, r4
	movs	r0, #20
	movs	r1, #24
	subs	r2, #20
	subs	r3, #24
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #72]
	adds	r1, r5, #0
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4643
	movs	r4, #20
	movs	r0, #24
	ldr	r2, [r3, #0]
	ldr	r1, [sp, #36]
	ldr	r3, [r3, #4]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	subs	r3, #24
	ldr	r4, [r1, #4]
	ldr	r0, [sp, #60]
	adds	r1, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b04
	ldr	r2, [sp, #56]
	adds	r3, #6
	cmp	r2, r3
	blt.n	.L_0814da24
	ldr	r2, [sp, #12]
	add	r3, sp, #80
	add	r2, sl
	mov	r8, r3
	ldr	r4, [sp, #64]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	movs	r0, #12
	movs	r7, #0
	mov	r6, r8
	adds	r5, r3, r4
	mov	r9, r0
.L_0814d9cc:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	asrs	r2, r3, #1
	str	r2, [r6, #0]
	ldr	r3, [r5, #24]
	cmp	r3, #26
	bhi.n	sub_0814da0a
	ldr	r3, [pc, #276]
	mov	r4, r9
	ldrh	r1, [r3, r4]
	ldr	r0, [sp, #64]
	movs	r3, #224
	adds	r1, r0, r1
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #264]
	ldrh	r4, [r3, r4]
	mov	r3, r8
	ldr	r3, [r3, #4]
	lsrs	r0, r4, #1
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #60]
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r7, #1
	adds	r3, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #5
	bne.n	.L_0814d9cc
.L_0814da24:
	ldr	r4, [sp, #20]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	adds	r4, #2
	adds	r1, #4
	adds	r2, #4
	str	r4, [sp, #20]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	ldr	r4, [sp, #68]
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r4, #20]
	movs	r0, #4
	negs	r0, r0
	add	fp, r0
	cmp	sl, r3
	beq.n	.L_0814da4a
	b.n	.L_0814d91a
.L_0814da4a:
	ldr	r0, [sp, #56]
	cmp	r0, #127
	bgt.n	.L_0814da5a
	movs	r0, #4
	movs	r1, #16
	bl	sub_08158ce0
	b.n	.L_0814da76
.L_0814da5a:
	ldr	r1, [sp, #56]
	movs	r2, #46
	adds	r2, #255
	cmp	r1, r2
	bgt.n	.L_0814da6e
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	b.n	.L_0814da76
.L_0814da6e:
	movs	r0, #4
	movs	r1, #8
	bl	sub_08158ce0
.L_0814da76:
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
	movs	r1, #183
	adds	r0, #1
	lsls	r1, r1, #1
	str	r0, [sp, #56]
	cmp	r0, r1
	beq.n	.L_0814daa0
	bl	.L_0814d032
.L_0814daa0:
	ldr	r0, [pc, #92]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #134
	bl	sub_081180e8
	movs	r1, #128
	ldr	r2, [sp, #52]
	movs	r0, #2
	lsls	r1, r1, #16
	bl	sub_0816467c
	movs	r4, #238
	ldr	r3, [sp, #64]
	lsls	r4, r4, #7
	movs	r2, #0
	adds	r4, #220
	mov	sl, r2
	adds	r5, r3, r4
.L_0814dad2:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #8
	bne.n	.L_0814dad2
	bl	sub_08143bb8
	add	sp, #104
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffed2
	.4byte 0x08198244
	.4byte 0x08198252
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #336
	str	r0, [sp, #52]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #96]
	movs	r3, #1
	str	r0, [sp, #48]
	movs	r0, #0
	ldr	r1, [r5, #92]
	movs	r7, #239
	str	r1, [sp, #44]
	lsls	r7, r7, #7
	ldr	r2, [r5, #100]
	str	r3, [sp, #24]
	str	r2, [sp, #28]
	bl	sub_081435e0
	bl	sub_0813ba50
	ldr	r2, [pc, #56]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r4, [sp, #44]
	movs	r3, #0
	adds	r6, r4, r7
	movs	r1, #200
	str	r3, [r6, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #44]
	bl	sub_080145a8
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #0
	bl	sub_08163c2c
	ldr	r3, [pc, #16]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #64
	b.n	.L_0814db84
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00002137
	.4byte 0x0000f0f0
	.2byte 0x3001
	.2byte 0x0814
.L_0814db84:
	strh	r3, [r2, #0]
	ldr	r2, [sp, #44]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #164]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #28]
	movs	r3, #0
	ldr	r0, [pc, #152]
	bl	sub_08157cf4
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r4, [r5, #104]
	movs	r1, #35
	movs	r0, #188
	str	r4, [sp, #36]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r2, [pc, #128]
	ldr	r5, [r5, #0]
	movs	r3, #240
	str	r5, [sp, #32]
	str	r3, [r2, #16]
	ldr	r5, [sp, #44]
	movs	r7, #240
	lsls	r7, r7, #7
	adds	r7, #240
	adds	r3, r5, r7
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #100]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r1, #255
	movs	r0, #9
	movs	r2, #1
	bl	sub_08152404
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	movs	r0, #238
	subs	r2, #2
	lsls	r0, r0, #7
	strh	r3, [r2, #0]
	adds	r0, #132
	movs	r3, #2
	str	r3, [r6, #0]
	adds	r2, r5, r0
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r5, [pc, #36]
	movs	r1, #0
	mov	r8, r1
	b.n	.L_0814dc48
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x0000017d
	.4byte 0x0000017e
	.4byte 0x030011e0
	.4byte 0x00000045
	.2byte 0x0000
	.2byte 0x0201
.L_0814dc48:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #120
	str	r3, [r5, #4]
	movs	r2, #1
	movs	r3, #1
	negs	r3, r3
	add	r8, r2
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_0814dc48
	ldr	r1, [pc, #136]
	movs	r4, #0
	mov	r8, r4
	add	r2, sp, #80
.L_0814dc7a:
	ldrb	r3, [r1, #0]
	movs	r5, #1
	str	r3, [r2, #0]
	ldrb	r3, [r1, #1]
	add	r8, r5
	mov	r6, r8
	str	r3, [r2, #4]
	adds	r1, #2
	adds	r2, #8
	cmp	r6, #32
	bne.n	.L_0814dc7a
	movs	r0, #141
	bl	sub_081c0010
	movs	r7, #0
	str	r7, [sp, #40]
	ldr	r3, [pc, #100]
	mov	r1, sp
	adds	r1, #56
	ldr	r3, [r3, #12]
	str	r1, [sp, #16]
	movs	r0, #16
	mov	sl, r0
.L_0814dca8:
	ldr	r2, [sp, #40]
	cmp	r2, #15
	bhi.n	.L_0814dd10
	ldr	r7, [pc, #84]
	cmp	r2, #1
	bne.n	.L_0814dcdc
	ldr	r5, [pc, #80]
	movs	r3, #63
	adds	r6, r5, #0
	adds	r6, #128
.L_0814dcbc:
	str	r3, [sp, #12]
	bl	sub_08014878
	ldr	r3, [sp, #12]
	ands	r0, r3
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_0814dcbc
	ldr	r3, [pc, #40]
	movs	r1, #200
	strh	r3, [r7, #0]
	ldr	r0, [pc, #52]
	lsls	r1, r1, #4
	bl	sub_080145a8
.L_0814dcdc:
	ldr	r4, [sp, #24]
	ldrh	r3, [r7, #0]
	adds	r3, r3, r4
	strh	r3, [r7, #0]
	ldr	r5, [sp, #40]
	adds	r4, #3
	str	r4, [sp, #24]
	cmp	r5, #15
	bne.n	.L_0814dd10
	ldr	r0, [pc, #28]
	bl	sub_08014644
	b.n	.L_0814dd10
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x08198260
	.4byte 0x03001150
	.4byte 0x02010000
	.4byte 0x02010002
	.2byte 0xcbcd
	.2byte 0x0814
.L_0814dd10:
	ldr	r6, [sp, #40]
	cmp	r6, #103
	ble.n	.L_0814dd1c
	movs	r7, #0
	mov	sl, r7
	b.n	.L_0814dd32
.L_0814dd1c:
	ldr	r0, [sp, #40]
	cmp	r0, #63
	ble.n	.L_0814dd28
	movs	r1, #6
	mov	sl, r1
	b.n	.L_0814dd32
.L_0814dd28:
	ldr	r2, [sp, #40]
	cmp	r2, #31
	ble.n	.L_0814dd32
	movs	r3, #10
	mov	sl, r3
.L_0814dd32:
	ldr	r4, [sp, #40]
	cmp	r4, #167
	bgt.n	.L_0814dd58
	bl	sub_08014878
	movs	r5, #3
	ands	r0, r5
	subs	r0, #1
	mov	r9, r0
	bl	sub_08014878
	ldr	r3, [pc, #816]
	ands	r0, r5
	subs	r7, r0, #1
	mov	r5, r9
	adds	r0, #31
	strh	r5, [r3, #4]
	strh	r0, [r3, #6]
	b.n	.L_0814dd66
.L_0814dd58:
	ldr	r2, [pc, #800]
	movs	r7, #0
	movs	r3, #32
	strh	r7, [r2, #4]
	strh	r3, [r2, #6]
	movs	r6, #0
	mov	r9, r6
.L_0814dd66:
	ldr	r2, [sp, #40]
	subs	r2, #176
	cmp	r2, #3
	bhi.n	.L_0814dd82
	ldr	r3, [pc, #784]
	ldrsb	r3, [r3, r2]
	ldr	r2, [pc, #776]
	negs	r7, r3
	mov	r9, r7
	mov	r0, r9
	adds	r7, r3, #0
	adds	r3, #32
	strh	r0, [r2, #4]
	strh	r3, [r2, #6]
.L_0814dd82:
	movs	r1, #0
	mov	r2, sl
	mov	r8, r1
	cmp	r2, #0
	beq.n	.L_0814ddd4
	ldr	r3, [pc, #756]
	ldr	r6, [pc, #760]
	mov	fp, r3
.L_0814dd92:
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	mov	r4, fp
	lsls	r3, r0, #1
	ldrh	r1, [r4, r3]
	ldr	r5, [sp, #44]
	ldr	r4, [pc, #744]
	movs	r2, #224
	adds	r1, r5, r1
	lsls	r2, r2, #3
	adds	r1, r1, r2
	ldrb	r2, [r6, #0]
	ldrb	r5, [r4, r0]
	ldr	r4, [pc, #732]
	mov	r3, r9
	subs	r2, r2, r3
	ldrb	r3, [r6, #1]
	ldrb	r0, [r4, r0]
	subs	r3, r3, r5
	str	r5, [sp, #4]
	movs	r5, #1
	str	r0, [sp, #0]
	subs	r3, r3, r7
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	add	r8, r5
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x3602
	cmp	r8, sl
	bne.n	.L_0814dd92
.L_0814ddd4:
	ldr	r3, [pc, #700]
	ldr	r6, [sp, #40]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #56]
	str	r4, [sp, #60]
	cmp	r6, #174
	bne.n	.L_0814de0a
	ldr	r1, [sp, #44]
	movs	r2, #238
	lsls	r2, r2, #7
	movs	r0, #0
	movs	r4, #13
	adds	r2, #220
	mov	r8, r0
	negs	r4, r4
	adds	r0, r1, r2
.L_0814ddf6:
	ldmia	r0!, {r1}
	adds	r3, r4, #0
	ldrb	r2, [r1, #9]
	ands	r3, r2
	strb	r3, [r1, #9]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	cmp	r5, #9
	bne.n	.L_0814ddf6
.L_0814de0a:
	ldr	r6, [sp, #40]
	cmp	r6, #208
	ble.n	.L_0814de32
	ldr	r0, [sp, #44]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #228
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #632]
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0814de26
	adds	r3, #3
.L_0814de26:
	asrs	r3, r3, #2
	movs	r2, #3
	ands	r3, r2
	ldrb	r1, [r1, r3]
	bl	sub_08020030
.L_0814de32:
	ldr	r2, [sp, #16]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #56]
	str	r3, [r2, #4]
	add	r2, sp, #64
	movs	r3, #0
	str	r3, [r2, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [sp, #20]
	mov	fp, r3
	movs	r5, #152
	lsls	r3, r7, #16
	negs	r3, r3
	mov	r6, r9
	lsls	r5, r5, #15
	adds	r4, r3, r5
	lsls	r3, r6, #16
	negs	r3, r3
	mov	sl, r2
	mov	r9, r3
.L_0814de62:
	ldr	r1, [sp, #44]
	mov	r0, fp
	movs	r2, #238
	movs	r5, #144
	lsls	r3, r0, #2
	lsls	r2, r2, #7
	lsls	r5, r5, #16
	adds	r3, r3, r1
	adds	r2, #220
	movs	r7, #0
	mov	r8, r4
	add	r5, r9
	adds	r6, r3, r2
.L_0814de7c:
	mov	r3, sl
	mov	r0, r8
	str	r5, [r3, #0]
	str	r0, [r3, #8]
	mov	r1, sl
	ldmia	r6!, {r0}
	ldr	r2, [sp, #16]
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r1, #128
	lsls	r1, r1, #14
	adds	r7, #1
	adds	r5, r5, r1
	ldr	r4, [sp, #8]
	cmp	r7, #3
	bne.n	.L_0814de7c
	ldr	r3, [sp, #20]
	movs	r2, #3
	adds	r3, #1
	add	fp, r2
	adds	r4, r4, r1
	str	r3, [sp, #20]
	cmp	r3, #3
	bne.n	.L_0814de62
	ldr	r1, [sp, #40]
	subs	r1, #160
	cmp	r1, #157
	bhi.n	sub_0814df02
	ldr	r3, [sp, #40]
	ldr	r5, [sp, #40]
	subs	r3, #208
	movs	r2, #80
	movs	r4, #8
	cmp	r5, #175
	bgt.n	.L_0814ded2
	movs	r3, #96
	subs	r2, r3, r1
	lsls	r3, r1, #2
	adds	r4, r3, #0
	subs	r4, #56
	b.n	.L_0814dee6
.L_0814ded2:
	ldr	r6, [sp, #40]
	cmp	r6, #208
	ble.n	.L_0814dee6
	cmp	r3, #0
	bge.n	.L_0814dee0
	adds	r3, r6, #0
	subs	r3, #205
.L_0814dee0:
	asrs	r3, r3, #2
	adds	r4, r3, #0
	adds	r4, #8
.L_0814dee6:
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r3, #48
	str	r3, [sp, #4]
	ldr	r7, [sp, #44]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #70
	adds	r1, r7, r3
	ldr	r0, [sp, #48]
	adds	r3, r4, #0
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9d0a
	cmp	r5, #32
	bne.n	.L_0814df0e
	movs	r0, #134
	bl	sub_081c0010
.L_0814df0e:
	ldr	r6, [sp, #40]
	cmp	r6, #64
	bne.n	.L_0814df1a
	movs	r0, #134
	bl	sub_081c0010
.L_0814df1a:
	ldr	r7, [sp, #40]
	cmp	r7, #104
	bne.n	.L_0814df26
	movs	r0, #134
	bl	sub_081c0010
.L_0814df26:
	ldr	r0, [sp, #40]
	cmp	r0, #176
	bne.n	.L_0814df32
	movs	r0, #134
	bl	sub_081c0010
.L_0814df32:
	ldr	r1, [sp, #40]
	cmp	r1, #226
	bne.n	.L_0814df3e
	movs	r0, #145
	bl	sub_081c0010
.L_0814df3e:
	bl	sub_08014de4
	ldr	r2, [sp, #40]
	cmp	r2, #32
	bne.n	.L_0814df9c
	ldr	r5, [pc, #336]
	movs	r3, #0
	mov	r8, r3
	movs	r7, #31
	movs	r6, #127
.L_0814df52:
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #68
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #8
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #63
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	negs	r0, r0
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #15
	movs	r4, #1
	ands	r3, r0
	add	r8, r4
	adds	r3, #32
	mov	r0, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_0814df52
.L_0814df9c:
	ldr	r1, [sp, #40]
	cmp	r1, #64
	bne.n	.L_0814dffa
	ldr	r5, [pc, #252]
	movs	r2, #0
	mov	r8, r2
	movs	r6, #31
.L_0814dfaa:
	bl	sub_08014878
	movs	r1, #48
	bl	sub_0800206c
	adds	r0, #60
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #52
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #63
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	bl	sub_08014878
	negs	r0, r0
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #32
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #28
	cmp	r4, #32
	bne.n	.L_0814dfaa
.L_0814dffa:
	ldr	r5, [sp, #40]
	cmp	r5, #104
	bne.n	.L_0814e056
	ldr	r5, [pc, #152]
	movs	r6, #0
	mov	r8, r6
	movs	r6, #31
.L_0814e008:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #52
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #72
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #63
	lsls	r3, r3, #11
	str	r3, [r5, #12]
	bl	sub_08014878
	negs	r0, r0
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r7, #1
	movs	r3, #15
	ands	r3, r0
	add	r8, r7
	adds	r3, #32
	mov	r0, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_0814e008
.L_0814e056:
	ldr	r3, [sp, #40]
	subs	r3, #32
	cmp	r3, #175
	bhi.n	.L_0814e0fa
	ldr	r5, [pc, #60]
	movs	r1, #0
	mov	r8, r1
.L_0814e064:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0814e0ee
	ldr	r2, [sp, #40]
	cmp	r2, #191
	ble.n	.L_0814e0a4
	mov	r0, r8
	movs	r1, #7
	bl	sub_08002064
	adds	r4, r0, #4
	b.n	.L_0814e0aa
	.4byte 0x03001120
	.4byte 0x08198280
	.4byte 0x08198284
	.4byte 0x08198260
	.4byte 0x0819828d
	.4byte 0x0819828a
	.4byte 0x08196e14
	.4byte 0x08198290
	.4byte 0x02010000
	.2byte 0x0380
	.2byte 0x0201
.L_0814e0a4:
	movs	r4, #3
	mov	r3, r8
	ands	r4, r3
.L_0814e0aa:
	ldr	r2, [pc, #608]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	ldr	r0, [pc, #600]
	ldr	r6, [sp, #44]
	ldrb	r0, [r0, r4]
	adds	r1, r6, r1
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	ldr	r0, [pc, #592]
	movs	r7, #224
	ldrb	r0, [r0, r4]
	lsls	r7, r7, #3
	adds	r1, r1, r7
	str	r0, [sp, #4]
	ldr	r7, [sp, #32]
	ldr	r0, [sp, #48]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x682a
	ldr	r3, [r5, #12]
	movs	r0, #128
	adds	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	lsls	r0, r0, #6
	adds	r3, r3, r2
	adds	r2, r2, r0
	str	r3, [r5, #4]
	str	r2, [r5, #16]
.L_0814e0ee:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_0814e064
.L_0814e0fa:
	ldr	r3, [sp, #40]
	cmp	r3, #223
	bgt.n	.L_0814e102
	b.n	.L_0814e270
.L_0814e102:
	cmp	r3, #224
	bne.n	.L_0814e146
	ldr	r5, [pc, #528]
	movs	r4, #0
	mov	r8, r4
	movs	r6, #127
.L_0814e10e:
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #224
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	negs	r0, r0
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #16
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r7, #1
	add	r8, r7
	str	r0, [r5, #24]
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #128
	bne.n	.L_0814e10e
.L_0814e146:
	ldr	r5, [pc, #464]
	movs	r1, #0
	mov	r8, r1
.L_0814e14c:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0814e154
	adds	r3, #3
.L_0814e154:
	ldr	r2, [sp, #40]
	asrs	r3, r3, #2
	adds	r3, #224
	cmp	r2, r3
	blt.n	.L_0814e1d0
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	mov	r6, r8
	movs	r3, #1
	ands	r3, r6
	adds	r4, r0, #0
	cmp	r3, #0
	bne.n	sub_0814e19e
	ldr	r2, [pc, #424]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r7, [sp, #44]
	movs	r0, #224
	adds	r1, r7, r1
	lsls	r0, r0, #3
	adds	r1, r1, r0
	ldr	r0, [pc, #412]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	ldrb	r0, [r0, r4]
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	ldr	r0, [pc, #400]
	ldr	r7, [sp, #36]
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #48]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x682a
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #4]
	adds	r2, r2, r3
	ldr	r3, [r5, #16]
	movs	r0, #16
	str	r2, [r5, #0]
	adds	r1, r1, r3
	asrs	r2, r2, #16
	negs	r0, r0
	str	r1, [r5, #4]
	cmp	r2, r0
	blt.n	.L_0814e1be
	asrs	r3, r1, #16
	cmp	r3, #120
	ble.n	.L_0814e1ca
.L_0814e1be:
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #224
	lsls	r3, r3, #14
	str	r3, [r5, #4]
.L_0814e1ca:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0814e1d0:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #128
	bne.n	.L_0814e14c
	ldr	r3, [sp, #40]
	cmp	r3, #228
	bne.n	.L_0814e204
	ldr	r2, [pc, #324]
	ldr	r1, [pc, #304]
	movs	r4, #0
	mov	r8, r4
	movs	r0, #0
.L_0814e1ec:
	ldr	r3, [r1, #0]
	movs	r5, #1
	str	r3, [r2, #0]
	add	r8, r5
	ldr	r3, [r1, #4]
	mov	r6, r8
	str	r3, [r2, #4]
	str	r0, [r2, #24]
	adds	r1, #28
	adds	r2, #28
	cmp	r6, #128
	bne.n	.L_0814e1ec
.L_0814e204:
	ldr	r5, [pc, #288]
	ldr	r6, [pc, #272]
	movs	r7, #0
	mov	r8, r7
.L_0814e20c:
	ldr	r0, [sp, #40]
	mov	r3, r8
	adds	r3, #228
	cmp	r0, r3
	blt.n	.L_0814e262
	ldr	r0, [r5, #24]
	movs	r1, #9
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	bl	sub_08002064
	ldr	r3, [pc, #260]
	ldr	r2, [sp, #28]
	ldrb	r4, [r3, r0]
	ldr	r3, [pc, #260]
	lsls	r0, r0, #1
	ldrh	r1, [r3, r0]
	lsrs	r0, r4, #1
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #18
	bne.n	.L_0814e262
	ldr	r3, [r6, #0]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #24]
.L_0814e262:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	adds	r6, #28
	cmp	r0, #128
	bne.n	.L_0814e20c
.L_0814e270:
	ldr	r1, [sp, #44]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #40]
	movs	r5, #144
	adds	r4, #1
	lsls	r5, r5, #1
	str	r4, [sp, #40]
	cmp	r4, r5
	beq.n	.L_0814e2aa
	ldr	r3, [pc, #160]
	movs	r2, #3
	ldr	r3, [r3, #12]
	movs	r6, #16
	ands	r3, r2
	mov	sl, r6
	cmp	r3, #0
	bne.n	.L_0814e2a4
	b.n	.L_0814dca8
.L_0814e2a4:
	cmp	r4, #16
	bgt.n	.L_0814e2aa
	b.n	.L_0814dca8
.L_0814e2aa:
	ldr	r0, [sp, #44]
	movs	r1, #238
	lsls	r1, r1, #7
	movs	r7, #0
	adds	r1, #220
	mov	r8, r7
	adds	r5, r0, r1
.L_0814e2b8:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #9
	bne.n	.L_0814e2b8
	bl	sub_0814cca8
	ldr	r2, [pc, #56]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	movs	r0, #195
	strh	r2, [r3, #0]
	lsls	r0, r0, #1
	bl	sub_081c0010
	ldr	r5, [sp, #44]
	movs	r4, #0
	mov	r8, r4
	movs	r7, #127
	movs	r6, #63
.L_0814e2e8:
	bl	sub_08014878
	ldr	r3, [pc, #72]
	ands	r0, r7
	adds	r0, #64
	lsls	r0, r0, #16
	str	r3, [r5, #8]
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	negs	r0, r0
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	b.n	.L_0814e33c
	.4byte 0x00001010
	.4byte 0x081982aa
	.4byte 0x08198294
	.4byte 0x0819829f
	.4byte 0x02010000
	.4byte 0x081982c6
	.4byte 0x081982c0
	.4byte 0x081982c3
	.4byte 0x02010e00
	.4byte 0x0819745e
	.4byte 0x0819744c
	.4byte 0x03001150
	.2byte 0x0000
	.2byte 0xfff0
.L_0814e33c:
	.2byte 0xf6c6
	.2byte 0xfa9c
	.2byte 0x4030
	negs	r0, r0
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	movs	r0, #1
	add	r8, r0
	movs	r3, #0
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_0814e2e8
	ldr	r5, [pc, #856]
	mov	r8, r3
.L_0814e36a:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	mov	r4, r8
	lsrs	r3, r4, #31
	add	r3, r8
	movs	r2, #63
	asrs	r3, r3, #1
	ands	r2, r0
	adds	r2, r2, r3
	negs	r3, r4
	str	r2, [r5, #4]
	movs	r6, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	add	r8, r6
	asrs	r3, r3, #1
	mov	r7, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_0814e36a
	movs	r0, #0
	ldr	r1, [pc, #800]
	str	r0, [sp, #40]
	mov	r9, r1
.L_0814e3a6:
	ldr	r3, [sp, #40]
	movs	r2, #0
	mov	sl, r2
	cmp	r3, #96
	bne.n	.L_0814e3b6
	movs	r0, #134
	bl	sub_081180e8
.L_0814e3b6:
	ldr	r4, [sp, #40]
	cmp	r4, #16
	bne.n	.L_0814e3ca
	ldr	r5, [sp, #44]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	adds	r2, r5, r6
	movs	r3, #32
	str	r3, [r2, #0]
.L_0814e3ca:
	ldr	r7, [sp, #40]
	cmp	r7, #16
	ble.n	.L_0814e3e4
	adds	r3, r7, #0
	subs	r3, #16
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	sl, r3
	cmp	r3, #16
	ble.n	.L_0814e3e4
	movs	r0, #16
	mov	sl, r0
.L_0814e3e4:
	ldr	r3, [sp, #40]
	subs	r3, #9
	cmp	r3, #62
	bhi.n	.L_0814e3fc
	ldr	r1, [sp, #40]
	movs	r3, #3
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_0814e3fc
	movs	r0, #132
	bl	sub_081c0010
.L_0814e3fc:
	ldr	r2, [sp, #40]
	cmp	r2, #72
	bne.n	.L_0814e408
	movs	r0, #145
	bl	sub_081c0010
.L_0814e408:
	ldr	r3, [sp, #40]
	cmp	r3, #64
	ble.n	sub_0814e436
	ldr	r4, [sp, #40]
	movs	r3, #64
	subs	r3, r3, r4
	lsls	r2, r3, #3
	ldr	r5, [sp, #44]
	movs	r6, #216
	subs	r2, r2, r3
	lsls	r6, r6, #5
	movs	r3, #40
	str	r3, [sp, #0]
	adds	r6, #249
	movs	r3, #80
	str	r3, [sp, #4]
	adds	r2, #88
	ldr	r0, [sp, #48]
	adds	r1, r5, r6
	mov	r3, r9
	ldr	r7, [sp, #36]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x980a
	cmp	r0, #71
	bgt.n	.L_0814e486
	movs	r1, #0
	mov	r2, sl
	mov	r8, r1
	cmp	r2, #0
	beq.n	.L_0814e486
	ldr	r6, [pc, #640]
	ldr	r7, [pc, #640]
.L_0814e44a:
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	lsls	r3, r0, #1
	ldrh	r1, [r7, r3]
	ldr	r3, [sp, #44]
	movs	r4, #224
	adds	r1, r3, r1
	lsls	r4, r4, #3
	adds	r1, r1, r4
	ldr	r4, [pc, #620]
	ldrb	r2, [r6, #0]
	ldrb	r5, [r4, r0]
	ldr	r4, [pc, #620]
	ldrb	r3, [r6, #1]
	ldrb	r0, [r4, r0]
	subs	r3, r3, r5
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	subs	r2, #56
	ldr	r0, [sp, #48]
	ldr	r5, [sp, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	adds	r6, #2
	cmp	r8, sl
	bne.n	.L_0814e44a
.L_0814e486:
	ldr	r1, [sp, #40]
	cmp	r1, #72
	bne.n	.L_0814e4ea
	ldr	r6, [pc, #568]
	ldr	r5, [sp, #44]
	movs	r2, #0
	mov	r8, r2
.L_0814e494:
	mov	r3, r8
	movs	r2, #15
	ands	r2, r3
	lsls	r2, r2, #1
	ldrb	r3, [r6, r2]
	adds	r2, #1
	subs	r3, #56
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldrb	r3, [r6, r2]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #63
	lsls	r3, r3, #13
	str	r3, [r5, #12]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	negs	r3, r3
	movs	r4, #1
	subs	r3, #16
	add	r8, r4
	lsls	r3, r3, #14
	mov	r7, r8
	str	r3, [r5, #16]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_0814e494
	ldr	r0, [sp, #40]
	cmp	r0, #72
	bne.n	.L_0814e4ea
	ldr	r1, [sp, #44]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #4
	str	r3, [r2, #0]
.L_0814e4ea:
	ldr	r4, [sp, #40]
	cmp	r4, #71
	ble.n	.L_0814e590
	movs	r5, #32
	mov	sl, r5
	cmp	r4, #72
	beq.n	.L_0814e4fc
	movs	r6, #64
	mov	sl, r6
.L_0814e4fc:
	movs	r7, #0
	mov	r0, sl
	mov	r8, r7
	cmp	r0, #0
	beq.n	.L_0814e58a
	ldr	r6, [sp, #44]
.L_0814e508:
	movs	r1, #6
	ldrsh	r7, [r6, r1]
	cmp	r7, #135
	bgt.n	.L_0814e580
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	ldr	r2, [pc, #444]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #44]
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	movs	r4, #2
	ldrsh	r2, [r6, r4]
	adds	r1, r1, r3
	ldr	r4, [pc, #428]
	ldr	r3, [pc, #432]
	ldrb	r5, [r3, r0]
	ldrb	r0, [r4, r0]
	subs	r3, r7, r5
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r5, [sp, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r6, #0
	movs	r1, #64
	lsls	r2, r2, #9
	bl	sub_08138086
	movs	r7, #6
	ldrsh	r3, [r6, r7]
	cmp	r3, #120
	ble.n	.L_0814e580
	ldr	r3, [r6, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	cmp	r3, r0
	ble.n	.L_0814e580
	negs	r3, r3
	cmp	r3, #0
	bge.n	.L_0814e568
	adds	r3, #3
.L_0814e568:
	asrs	r3, r3, #2
	str	r3, [r6, #16]
	movs	r3, #240
	lsls	r3, r3, #15
	str	r3, [r6, #4]
	ldr	r1, [sp, #44]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
.L_0814e580:
	movs	r4, #1
	add	r8, r4
	adds	r6, #28
	cmp	r8, sl
	bne.n	.L_0814e508
.L_0814e58a:
	ldr	r5, [sp, #40]
	cmp	r5, #71
	bgt.n	.L_0814e5f4
.L_0814e590:
	ldr	r5, [sp, #44]
	movs	r6, #0
	mov	r8, r6
.L_0814e596:
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	ldr	r4, [sp, #44]
	movs	r1, #24
	movs	r6, #224
	str	r1, [sp, #0]
	lsls	r6, r6, #3
	movs	r1, #48
	subs	r2, #12
	subs	r3, #24
	str	r1, [sp, #4]
	ldr	r0, [sp, #48]
	adds	r1, r4, r6
	ldr	r7, [sp, #36]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	asrs	r3, r3, #16
	cmp	r3, #120
	ble.n	.L_0814e5e8
	ldr	r0, [sp, #40]
	cmp	r0, #47
	bgt.n	.L_0814e5e8
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [pc, #252]
	str	r3, [r5, #4]
.L_0814e5e8:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_0814e596
.L_0814e5f4:
	ldr	r4, [sp, #52]
	movs	r3, #0
	mov	r8, r3
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq.n	.L_0814e62c
	movs	r6, #36
	movs	r5, #32
.L_0814e604:
	ldr	r7, [sp, #40]
	cmp	r7, r5
	bne.n	.L_0814e620
	ldr	r1, [sp, #52]
	movs	r3, #0
	ldrsh	r0, [r6, r1]
	str	r3, [sp, #0]
	movs	r1, #9
	subs	r3, #1
	movs	r2, #5
	bl	sub_0814cd48
	ldr	r4, [sp, #52]
	ldr	r3, [r4, #20]
.L_0814e620:
	movs	r7, #1
	add	r8, r7
	adds	r6, #2
	adds	r5, #8
	cmp	r8, r3
	bne.n	.L_0814e604
.L_0814e62c:
	ldr	r0, [sp, #40]
	cmp	r0, #72
	ble.n	.L_0814e6ae
	ldr	r5, [pc, #140]
	movs	r1, #0
	adds	r6, r0, #0
	mov	r8, r1
	subs	r6, #54
.L_0814e63c:
	ldr	r3, [r5, #24]
	cmp	r3, #17
	bhi.n	.L_0814e674
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	movs	r1, #9
	asrs	r0, r0, #1
	bl	sub_08002064
	ldr	r3, [pc, #152]
	ldr	r2, [sp, #28]
	ldrb	r4, [r3, r0]
	ldr	r3, [pc, #148]
	lsls	r0, r0, #1
	ldrh	r1, [r3, r0]
	ldr	r3, [r5, #4]
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	subs	r2, r2, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
.L_0814e674:
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #18
	bne.n	.L_0814e6a2
	ldr	r7, [sp, #40]
	cmp	r7, #127
	bgt.n	.L_0814e6a2
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	lsrs	r3, r6, #31
	movs	r2, #63
	adds	r3, r6, r3
	asrs	r3, r3, #1
	ands	r2, r0
	adds	r2, r2, r3
	movs	r3, #0
	str	r2, [r5, #4]
	str	r3, [r5, #24]
.L_0814e6a2:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_0814e63c
.L_0814e6ae:
	ldr	r3, [sp, #40]
	subs	r3, #72
	cmp	r3, #7
	bhi.n	.L_0814e6f0
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	b.n	.L_0814e6f8
	.4byte 0x02010000
	.4byte 0xfffffc20
	.4byte 0x08198260
	.4byte 0x081982cc
	.4byte 0x081982d5
	.4byte 0x081982d2
	.4byte 0x081982de
	.4byte 0x081982d8
	.4byte 0x081982db
	.4byte 0xfff00000
	.4byte 0x0819745e
	.2byte 0x744c
	.2byte 0x0819
.L_0814e6f0:
	movs	r0, #2
	movs	r1, #2
	bl	sub_08158ce0
.L_0814e6f8:
	ldr	r3, [sp, #44]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #40]
	movs	r5, #14
	adds	r6, #1
	add	r9, r5
	str	r6, [sp, #40]
	cmp	r6, #146
	beq.n	.L_0814e71c
	b.n	.L_0814e3a6
.L_0814e71c:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #20]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #336
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	mov	fp, r0
	ldr	r0, [r5, #92]
	sub	sp, #60
	str	r0, [sp, #32]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #28]
	ldr	r2, [r5, #100]
	str	r2, [sp, #16]
	ldr	r6, [r5, #48]
	bl	sub_081435e0
	movs	r2, #0
	ldr	r1, [sp, #16]
	movs	r3, #0
	ldr	r0, [pc, #676]
	bl	sub_08157cf4
	ldr	r0, [pc, #676]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #668]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2113
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	movs	r3, #0
	mov	r8, r3
	str	r5, [sp, #24]
	ldr	r3, [pc, #648]
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #3
.L_0814e7a8:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0814e7a8
	bl	sub_08014de4
	adds	r1, r6, #0
	adds	r1, #12
	adds	r0, r6, #0
	bl	sub_080156e8
	movs	r0, #0
	str	r0, [sp, #20]
	mov	r1, fp
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_0814e8b4
	movs	r3, #36
	movs	r2, #36
	movs	r4, #48
	str	r3, [sp, #12]
	str	r0, [sp, #8]
	add	r2, sp
	add	r4, sp
	mov	sl, r2
	mov	r9, r4
.L_0814e7e0:
	ldr	r1, [sp, #12]
	mov	r3, fp
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r4, [sp, #12]
	mov	r2, fp
	ldr	r5, [r0, #0]
	ldrsh	r0, [r4, r2]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r5, #8]
	mov	r4, r9
	asrs	r0, r0, #1
	str	r0, [r4, #4]
	str	r3, [r4, #0]
	mov	r1, sl
	ldr	r3, [r5, #16]
	mov	r0, r9
	str	r3, [r4, #8]
	bl	sub_0815e1ec
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r1, #0
	asrs	r3, r3, #1
	str	r3, [r0, #0]
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #524]
	mov	r8, r1
	adds	r7, r2, r3
.L_0814e822:
	bl	sub_08014878
	movs	r4, #255
	adds	r6, r0, #0
	ands	r6, r4
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r2, r6, #0
	muls	r2, r0
	mov	r0, sl
	ldr	r3, [r0, #0]
	asrs	r2, r2, #7
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	mov	r1, sl
	ldr	r3, [r1, #4]
	adds	r2, r6, #0
	muls	r2, r0
	lsls	r3, r3, #16
	asrs	r2, r2, #3
	adds	r2, r2, r3
	str	r2, [r7, #4]
	bl	sub_08014878
	movs	r2, #255
	ands	r0, r2
	movs	r3, #128
	subs	r3, r3, r0
	lsls	r3, r3, #9
	str	r3, [r7, #12]
	bl	sub_08014878
	movs	r3, #255
	ands	r0, r3
	negs	r0, r0
	subs	r0, #128
	movs	r4, #1
	lsls	r0, r0, #10
	add	r8, r4
	str	r0, [r7, #16]
	movs	r3, #0
	mov	r0, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r0, #128
	bne.n	.L_0814e822
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	ldr	r4, [sp, #20]
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r2, r2, r3
	adds	r1, #2
	adds	r4, #1
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	str	r4, [sp, #20]
	mov	r0, fp
	ldr	r3, [r0, #20]
	cmp	r4, r3
	bne.n	.L_0814e7e0
.L_0814e8b4:
	ldr	r1, [sp, #32]
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
	ldr	r0, [pc, #348]
	bl	sub_080145a8
	mov	r1, fp
	ldr	r2, [r1, #20]
	movs	r4, #56
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r0, #0
	lsls	r3, r3, #2
	negs	r4, r4
	mov	r9, r0
	cmp	r3, r4
	bne.n	.L_0814e8ee
	b.n	.L_0814e9fc
.L_0814e8ee:
	mov	r0, r9
	cmp	r0, #32
	bne.n	.L_0814e8fe
	movs	r0, #0
	bl	sub_081180e8
	mov	r1, fp
	ldr	r2, [r1, #20]
.L_0814e8fe:
	movs	r3, #0
	str	r3, [sp, #20]
	cmp	r2, #0
	beq.n	.L_0814e9cc
	movs	r4, #36
	mov	sl, r4
	movs	r6, #0
	movs	r7, #0
.L_0814e90e:
	cmp	r9, r6
	bne.n	.L_0814e92e
	movs	r0, #143
	bl	sub_081c0010
	mov	r1, sl
	mov	r3, fp
	ldrsh	r0, [r1, r3]
	movs	r3, #20
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	ldr	r3, [sp, #20]
	bl	sub_0814cd48
.L_0814e92e:
	cmp	r9, r6
	ble.n	.L_0814e9b2
	ldr	r0, [pc, #248]
	movs	r4, #0
	mov	r8, r4
	adds	r5, r7, r0
.L_0814e93a:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0814e9a6
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	ldr	r2, [pc, #232]
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #16]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	mov	lr, r2
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	str	r0, [sp, #0]
	subs	r3, r2, r0
	str	r4, [sp, #4]
	mov	r2, lr
	ldr	r0, [sp, #28]
	ldr	r4, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4a2f
	movs	r0, #3
	mov	r3, r8
	ands	r3, r0
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	adds	r0, r5, #0
	movs	r1, #62
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	ble.n	.L_0814e9a6
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	cmp	r3, #112
	ble.n	.L_0814e9a6
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_0814e9a6:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #128
	bne.n	.L_0814e93a
.L_0814e9b2:
	ldr	r1, [sp, #20]
	mov	r2, fp
	adds	r1, #1
	str	r1, [sp, #20]
	movs	r0, #224
	ldr	r3, [r2, #20]
	movs	r4, #2
	lsls	r0, r0, #4
	add	sl, r4
	adds	r6, #20
	adds	r7, r7, r0
	cmp	r1, r3
	bne.n	.L_0814e90e
.L_0814e9cc:
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #32]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	mov	r1, fp
	ldr	r3, [r1, #20]
	movs	r0, #1
	adds	r2, r3, #0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	add	r9, r0
	adds	r3, #56
	cmp	r9, r3
	beq.n	.L_0814e9fc
	b.n	.L_0814e8ee
.L_0814e9fc:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000134
	.4byte 0x0000017d
	.4byte 0x03000730
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x08143001
	.4byte 0x08197410
	.2byte 0x82e4
	.2byte 0x0819
	push	{lr}
	ldr	r3, [r0, #24]
	negs	r1, r3
	orrs	r1, r3
	lsrs	r1, r1, #31
	bl	sub_0814ea58
	pop	{pc}
	push	{lr}
	movs	r1, #2
	bl	sub_0814ea58
	pop	{pc}
