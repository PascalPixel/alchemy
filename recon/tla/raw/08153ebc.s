.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014c4c, 0x08014c4c
	.set sub_08014de4, 0x08014de4
	.set sub_08014e74, 0x08014e74
	.set sub_08014e90, 0x08014e90
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815456a, 0x0815456a
	.set sub_081547ee, 0x081547ee
	.set sub_081548d0, 0x081548d0
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b290, 0x0815b290
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_08163c2c, 0x08163c2c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_08153ebc
	.thumb_func
Func_08153ebc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #128
	movs	r5, #192
	str	r0, [sp, #64]
	lsls	r5, r5, #18
	adds	r3, r5, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #96]
	str	r3, [sp, #60]
	ldr	r3, [pc, #96]
	ldr	r0, [r5, #96]
	str	r0, [sp, #56]
	movs	r0, #128
	ldr	r2, [r5, #100]
	ldr	r1, [r5, #92]
	str	r2, [sp, #40]
	lsls	r0, r0, #6
	ldrh	r3, [r3, #4]
	mov	r9, r1
	str	r3, [sp, #36]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	bl	sub_0813ba50
	ldr	r2, [pc, #48]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #36]
	bl	sub_080145a8
	movs	r1, #0
	movs	r0, #0
	bl	sub_08163c2c
	movs	r3, #240
	lsls	r3, r3, #7
	b.n	.L_08153f48
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x03001120
	.2byte 0x3001
	.2byte 0x0814
.L_08153f48:
	adds	r3, #240
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r3, [r5, #36]
	movs	r0, #237
	lsls	r0, r0, #3
	adds	r0, #255
	adds	r3, r3, r0
	movs	r2, #1
	strb	r2, [r3, #0]
	ldr	r0, [pc, #368]
	bl	sub_08013300
	movs	r2, #128
	adds	r7, r0, #0
	ldr	r5, [pc, #364]
	adds	r1, r7, #0
	lsls	r2, r2, #2
	ldr	r0, [pc, #360]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #2
	adds	r7, r7, r1
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_0801587c
	movs	r7, #238
	movs	r2, #0
	ldr	r3, [pc, #340]
	lsls	r7, r7, #7
	movs	r0, #13
	str	r2, [sp, #48]
	adds	r7, #220
	negs	r0, r0
	mov	r8, r5
	mov	sl, r3
	add	r7, r9
	adds	r5, r0, #0
.L_08153f9c:
	movs	r1, #32
	ldr	r2, [pc, #324]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	movs	r2, #4
	ands	r3, r5
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r7!, {r0}
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r3, #2]
	ldr	r1, [pc, #296]
	movs	r2, #128
	adds	r0, r0, r1
	lsls	r2, r2, #3
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9b0c
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r3, #1
	adds	r6, r6, r2
	str	r3, [sp, #48]
	cmp	r3, #11
	bne.n	.L_08153f9c
	movs	r1, #224
	lsls	r1, r1, #3
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #260]
	bl	sub_08157cf4
	ldr	r0, [pc, #260]
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	adds	r1, r7, #0
	ldr	r3, [pc, #224]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	ldr	r1, [sp, #40]
	movs	r3, #0
	ldr	r0, [pc, #236]
	bl	sub_08157cf4
	ldr	r0, [pc, #232]
	bl	sub_08013300
	movs	r1, #19
	str	r0, [sp, #52]
	movs	r0, #104
	bl	sub_081963ec
	movs	r1, #3
	movs	r0, #188
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #104]
	adds	r3, #188
	str	r2, [sp, #68]
	mov	r0, sp
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #200]
	adds	r0, #68
	str	r0, [sp, #24]
	str	r3, [r0, #4]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #184]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r2, #238
	adds	r3, #144
	lsls	r2, r2, #7
	movs	r1, #0
	add	r3, r9
	adds	r2, #148
	str	r1, [r3, #0]
	add	r2, r9
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #152
	add	r2, r9
	subs	r3, #5
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #156
	add	r3, r9
	str	r1, [r3, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #128]
	bl	sub_080145a8
	ldr	r1, [sp, #60]
	movs	r3, #1
	str	r3, [r1, #16]
	movs	r0, #0
	movs	r1, #1
	bl	sub_08163c2c
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r2, #0
	movs	r3, #255
	lsls	r3, r3, #8
	str	r2, [sp, #32]
	str	r2, [sp, #28]
	str	r2, [sp, #48]
	adds	r3, #255
	mov	r8, r3
	mov	r7, r9
	b.n	.L_08154108
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x000000a6
	.4byte 0x03000730
	.4byte 0x05000200
	.4byte 0x020036e0
	.4byte 0x80002000
	.4byte 0x06010000
	.4byte 0x0000012b
	.4byte 0x00000163
	.4byte 0x00000134
	.4byte 0x00000195
	.4byte 0x030011e0
	.4byte 0x00000045
	.2byte 0xbaed
	.2byte 0x0813
.L_08154108:
	bl	sub_08014878
	movs	r1, #96
	bl	sub_0800206c
	adds	r0, #12
	lsls	r0, r0, #16
	str	r0, [r7, #0]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #12]
	str	r3, [r7, #16]
	str	r3, [r7, #24]
	ldr	r0, [sp, #48]
	mov	fp, r3
	lsls	r2, r0, #1
	adds	r2, r2, r0
	lsls	r3, r2, #1
	ldr	r1, [pc, #160]
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r6, r3, r1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #152]
	lsls	r3, r3, #5
	adds	r5, r3, r2
.L_0815414a:
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #48
	str	r3, [r5, #0]
	bl	sub_08014de4
	bl	sub_08014878
	mov	r3, r8
	ands	r0, r3
	bl	sub_080150e4
	bl	sub_08014878
	mov	r1, r8
	ands	r0, r1
	bl	sub_08015024
	bl	sub_08014878
	mov	r2, r8
	ands	r0, r2
	bl	sub_08015068
	adds	r0, r6, #0
	bl	sub_08014e74
	movs	r3, #1
	add	fp, r3
	mov	r0, fp
	adds	r5, #28
	adds	r6, #48
	cmp	r0, #24
	bne.n	.L_0815414a
	ldr	r1, [sp, #48]
	adds	r7, #28
	adds	r1, #1
	str	r1, [sp, #48]
	cmp	r1, #16
	bne.n	.L_08154108
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	movs	r2, #0
	str	r2, [sp, #44]
	ldr	r3, [pc, #32]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081541d0
	b.n	.L_081545ae
.L_081541d0:
	mov	r3, sp
	adds	r3, #116
	str	r3, [sp, #20]
	b.n	.L_081541e8
	.4byte 0x00000784
	.4byte 0x02013800
	.4byte 0x02010000
	.2byte 0x1150
	.2byte 0x0300
.L_081541e8:
	ldr	r0, [sp, #44]
	cmp	r0, #209
	bgt.n	.L_081542b8
	cmp	r0, #0
	bne.n	.L_08154212
	ldr	r1, [sp, #52]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r2, [r1, #1]
	lsls	r3, r3, #8
	adds	r3, r3, r2
	str	r3, [sp, #32]
	movs	r3, #2
	ldrsb	r3, [r1, r3]
	ldrb	r2, [r1, #3]
	lsls	r3, r3, #8
	adds	r3, r3, r2
	adds	r1, #4
	str	r3, [sp, #28]
	str	r1, [sp, #52]
	b.n	.L_0815422c
.L_08154212:
	ldr	r2, [sp, #52]
	ldr	r0, [sp, #32]
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldr	r1, [sp, #28]
	adds	r0, r0, r3
	str	r0, [sp, #32]
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	adds	r2, #2
	adds	r1, r1, r3
	str	r1, [sp, #28]
	str	r2, [sp, #52]
.L_0815422c:
	add	r7, sp, #88
	movs	r3, #0
	str	r3, [r7, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	ldr	r3, [sp, #28]
	ldr	r0, [sp, #32]
	movs	r2, #0
	mov	sl, r2
	str	r2, [sp, #12]
	lsls	r0, r0, #16
	lsls	r2, r3, #16
	movs	r3, #128
	lsls	r3, r3, #15
	str	r0, [sp, #16]
	subs	r2, r3, r2
	mov	r8, r2
.L_08154250:
	ldr	r1, [sp, #16]
	movs	r2, #160
	ldr	r0, [sp, #12]
	lsls	r2, r2, #15
	adds	r5, r1, r2
	movs	r1, #238
	lsls	r3, r0, #2
	lsls	r1, r1, #7
	add	r3, r9
	adds	r1, #220
	movs	r4, #0
	mov	fp, r8
	adds	r6, r3, r1
.L_0815426a:
	mov	r2, sl
	cmp	r2, #3
	bne.n	.L_08154274
	cmp	r4, #2
	beq.n	.L_08154298
.L_08154274:
	mov	r3, fp
	mov	r0, sl
	str	r5, [sp, #88]
	str	r3, [sp, #96]
	cmp	r0, #3
	bne.n	.L_08154288
	movs	r1, #128
	lsls	r1, r1, #14
	adds	r3, r5, r1
	str	r3, [r7, #0]
.L_08154288:
	ldr	r0, [r6, #0]
	adds	r1, r7, #0
	ldr	r2, [pc, #604]
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	ldr	r4, [sp, #8]
.L_08154298:
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r4, #1
	adds	r5, r5, r2
	adds	r6, #4
	cmp	r4, #3
	bne.n	.L_0815426a
	ldr	r3, [sp, #12]
	movs	r0, #1
	add	sl, r0
	adds	r3, #3
	mov	r1, sl
	str	r3, [sp, #12]
	add	r8, r2
	cmp	r1, #4
	bne.n	.L_08154250
.L_081542b8:
	ldr	r2, [sp, #20]
	movs	r1, #0
	str	r1, [r2, #4]
	str	r1, [r2, #8]
	ldr	r3, [sp, #44]
	cmp	r3, #48
	bne.n	.L_081542dc
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #180
	add	r3, r9
	movs	r2, #24
	str	r2, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #184
	add	r3, r9
	str	r1, [r3, #0]
.L_081542dc:
	movs	r0, #0
	str	r0, [sp, #48]
.L_081542e0:
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #44]
	lsls	r6, r1, #3
	adds	r7, r6, #0
	adds	r7, #64
	cmp	r2, r7
	bge.n	.L_081542f0
	b.n	.L_08154578
.L_081542f0:
	subs	r3, r6, r1
	lsls	r3, r3, #2
	mov	r0, r9
	adds	r5, r0, r3
	movs	r2, #2
	ldrsh	r1, [r5, r2]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	mov	r8, r1
	ldr	r1, [sp, #44]
	mov	sl, r3
	adds	r3, r6, #0
	adds	r3, #84
	cmp	r1, r3
	bne.n	.L_08154314
	movs	r0, #212
	bl	sub_081c0010
.L_08154314:
	ldr	r2, [sp, #44]
	adds	r3, r6, #0
	adds	r3, #85
	cmp	r2, r3
	blt.n	.L_08154394
	ldr	r1, [r5, #12]
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	movs	r0, #128
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [pc, #444]
	lsls	r0, r0, #10
	adds	r1, r1, r3
	str	r1, [r5, #12]
	movs	r1, #232
	adds	r2, r2, r0
	lsls	r1, r1, #5
	str	r2, [r5, #16]
	movs	r0, #16
	movs	r5, #21
	adds	r1, #172
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	ldr	r4, [sp, #68]
	add	r1, r9
	adds	r2, #4
	subs	r3, #40
	str	r5, [sp, #4]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x21f0
	movs	r0, #29
	lsls	r1, r1, #5
	str	r0, [sp, #0]
	adds	r1, #252
	movs	r0, #35
	mov	r2, r8
	mov	r3, sl
	add	r1, r9
	subs	r2, #16
	subs	r3, #19
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2188
	lsls	r1, r1, #6
	movs	r0, #24
	adds	r1, #243
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #20
	adds	r3, #16
	str	r5, [sp, #0]
	b.n	.L_081544e0
.L_08154394:
	ldr	r1, [sp, #44]
	adds	r3, r6, #0
	adds	r3, #80
	cmp	r1, r3
	bge.n	.L_081543a0
	b.n	.L_081544f8
.L_081543a0:
	subs	r3, r1, r7
	subs	r3, #16
	cmp	r3, #4
	bls.n	.L_081543aa
	b.n	.L_08154578
.L_081543aa:
	ldr	r2, [pc, #328]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x081543c8
	.4byte 0x081543e0
	.4byte 0x081543fa
	.4byte 0x08154434
	.2byte 0x448c
	.2byte 0x0815
	movs	r0, #14
	movs	r1, #224
	lsls	r1, r1, #3
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	movs	r0, #28
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #7
	subs	r3, #14
	b.n	.L_081544e0
	movs	r1, #128
	movs	r0, #23
	lsls	r1, r1, #4
	adds	r1, #136
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	movs	r0, #44
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #11
	subs	r3, #22
	b.n	.L_081544e0
	movs	r1, #192
	movs	r0, #20
	lsls	r1, r1, #4
	str	r0, [sp, #0]
	adds	r1, #124
	movs	r0, #30
	mov	r2, r8
	mov	r3, sl
	add	r1, r9
	subs	r2, #4
	subs	r3, #31
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x21e0
	movs	r0, #22
	lsls	r1, r1, #4
	adds	r1, #212
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	movs	r0, #33
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #16
	subs	r3, #1
	b.n	.L_081544e0
	movs	r1, #136
	movs	r0, #18
	lsls	r1, r1, #5
	str	r0, [sp, #0]
	adds	r1, #170
	movs	r0, #27
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	add	r1, r9
	adds	r2, #1
	subs	r3, #38
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2198
	lsls	r1, r1, #5
	movs	r0, #22
	adds	r1, #144
	mov	r2, r8
	mov	r3, sl
	add	r1, r9
	subs	r2, #11
	subs	r3, #11
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x21a8
	movs	r0, #19
	lsls	r1, r1, #5
	adds	r1, #116
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	movs	r0, #28
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #19
	adds	r3, #11
	b.n	.L_081544e0
	movs	r1, #184
	lsls	r1, r1, #5
	movs	r5, #23
	movs	r0, #16
	adds	r1, #136
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #68]
	add	r1, r9
	adds	r2, #4
	subs	r3, #40
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x21c0
	lsls	r1, r1, #5
	str	r5, [sp, #0]
	adds	r1, #248
	movs	r5, #28
	mov	r2, r8
	mov	r3, sl
	add	r1, r9
	subs	r2, #10
	subs	r3, #17
	ldr	r4, [sp, #68]
	str	r5, [sp, #4]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x21d8
	lsls	r1, r1, #5
	movs	r0, #20
	adds	r1, #124
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	add	r1, r9
	subs	r2, #20
	adds	r3, #11
	str	r5, [sp, #4]
.L_081544e0:
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe046
	movs	r0, r0
	.4byte 0x081983cc
	.4byte 0xffff0000
	.2byte 0x43b4
	.2byte 0x0815
.L_081544f8:
	ldr	r3, [sp, #48]
	movs	r2, #0
	mov	fp, r2
	lsls	r2, r3, #1
	adds	r3, r2, r3
	lsls	r2, r3, #1
	ldr	r0, [pc, #308]
	adds	r2, r2, r3
	lsls	r2, r2, #7
	adds	r7, r2, r0
	ldr	r1, [pc, #304]
	lsls	r2, r3, #3
	subs	r2, r2, r3
	lsls	r2, r2, #5
	add	r6, sp, #104
	adds	r5, r2, r1
.L_08154518:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	ble.n	sub_0815456a
	adds	r0, r7, #0
	bl	sub_08014e90
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #20]
	adds	r1, r6, #0
	str	r3, [r2, #0]
	ldr	r0, [sp, #20]
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	movs	r0, #5
	asrs	r3, r3, #1
	add	r3, r8
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	add	r3, sl
	adds	r3, #16
	str	r3, [r6, #4]
	ldr	r3, [r5, #0]
	subs	r3, #4
	str	r3, [r5, #0]
	ldr	r3, [pc, #248]
	ldr	r2, [r6, #0]
	ldrh	r1, [r3, #8]
	ldr	r3, [sp, #40]
	subs	r2, #2
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	movs	r0, #10
	str	r0, [sp, #4]
	ldr	r0, [sp, #24]
	subs	r3, #5
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	add	fp, r1
	mov	r2, fp
	adds	r7, #48
	adds	r5, #28
	cmp	r2, #24
	bne.n	.L_08154518
.L_08154578:
	ldr	r3, [sp, #48]
	adds	r3, #1
	str	r3, [sp, #48]
	cmp	r3, #16
	beq.n	.L_08154584
	b.n	.L_081542e0
.L_08154584:
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #44]
	adds	r0, #1
	str	r0, [sp, #44]
	cmp	r0, #220
	beq.n	.L_081545ae
	ldr	r3, [pc, #164]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081545ae
	b.n	.L_081541e8
.L_081545ae:
	ldr	r0, [pc, #156]
	bl	sub_08014644
	ldr	r1, [sp, #60]
	add	r2, sp, #36
	movs	r3, #0
	str	r3, [r1, #16]
	ldrh	r2, [r2, #0]
	ldr	r3, [pc, #144]
	strh	r2, [r3, #4]
	bl	sub_0814cca8
	bl	sub_08014c4c
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #120]
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #116]
	movs	r2, #160
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #19
	adds	r2, #188
	strh	r3, [r2, #0]
	movs	r5, #238
	movs	r3, #0
	lsls	r5, r5, #7
	str	r3, [sp, #48]
	adds	r5, #220
	add	r5, r9
.L_081545fa:
	ldmia	r5!, {r0}
	bl	sub_08020048
	ldr	r0, [sp, #48]
	adds	r0, #1
	str	r0, [sp, #48]
	cmp	r0, #11
	bne.n	.L_081545fa
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	subs	r2, #32
	strh	r3, [r2, #0]
	ldr	r1, [pc, #36]
	ldr	r0, [pc, #60]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #0
	str	r1, [sp, #48]
	add	r6, sp, #76
	mov	r5, r9
	movs	r7, #31
	b.n	.L_08154660
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00007741
	.4byte 0x02013800
	.4byte 0x02010000
	.4byte 0x08197410
	.4byte 0x03001150
	.4byte 0x0813baed
	.4byte 0x03001120
	.4byte 0x05000200
	.4byte 0x050001e8
	.2byte 0x0178
	.2byte 0x0000
.L_08154660:
	ldr	r0, [sp, #48]
	movs	r1, #6
	bl	sub_08002064
	ldr	r2, [sp, #64]
	ldr	r3, [r2, #20]
	cmp	r0, r3
	bge.n	.L_081546a0
	lsls	r3, r0, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	adds	r1, r6, #0
	bl	sub_0815e20c
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #40
	negs	r0, r0
	str	r0, [r5, #4]
	ldr	r1, [r6, #0]
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	movs	r3, #80
	subs	r3, r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r1, #1
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r5, #0]
	b.n	.L_081546b8
.L_081546a0:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #80
	str	r3, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #40
	negs	r0, r0
	str	r0, [r5, #4]
.L_081546b8:
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
	ldr	r2, [sp, #48]
	adds	r5, #28
	adds	r2, #1
	str	r2, [sp, #48]
	cmp	r2, #32
	bne.n	.L_08154660
	movs	r3, #0
	str	r3, [sp, #44]
.L_081546ce:
	movs	r0, #0
	str	r0, [sp, #48]
	mov	r7, r9
.L_081546d4:
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #44]
	lsls	r3, r1, #1
	cmp	r2, r3
	bge.n	.L_081546e4
	cmp	r2, #40
	bgt.n	.L_081546e4
	b.n	.L_08154844
.L_081546e4:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_08154780
	cmp	r3, #23
	bgt.n	.L_0815477a
	adds	r6, r3, #0
	cmp	r3, #0
	bge.n	.L_081546f6
	adds	r6, r3, #3
.L_081546f6:
	ldr	r3, [sp, #48]
	ldr	r2, [pc, #420]
	asrs	r6, r6, #2
	movs	r4, #1
	ands	r4, r3
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #412]
	ldr	r2, [r7, #0]
	ldrb	r5, [r3, r6]
	ldr	r0, [pc, #412]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #408]
	adds	r1, r1, r0
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #400]
	lsls	r4, r4, #2
	ldrb	r0, [r0, r6]
	subs	r3, #40
	str	r0, [sp, #4]
	ldr	r0, [sp, #24]
	subs	r2, #8
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69bb
	cmp	r3, #11
	bgt.n	.L_0815477a
	movs	r1, #16
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #21
	str	r1, [sp, #4]
	movs	r1, #232
	lsls	r1, r1, #5
	adds	r1, #172
	ldr	r4, [sp, #68]
	adds	r2, #4
	subs	r3, #40
	ldr	r0, [sp, #56]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x211d
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #35
	str	r1, [sp, #4]
	movs	r1, #240
	lsls	r1, r1, #5
	adds	r1, #252
	subs	r3, #19
	subs	r2, #16
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #56]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69bb
.L_0815477a:
	adds	r3, #1
	str	r3, [r7, #24]
	b.n	.L_08154844
.L_08154780:
	ldr	r1, [r7, #4]
	movs	r5, #24
	cmp	r1, #56
	ble.n	.L_0815478e
	subs	r3, r5, r1
	adds	r5, r3, #0
	adds	r5, #56
.L_0815478e:
	adds	r3, r1, #0
	movs	r1, #16
	ldr	r2, [r7, #0]
	str	r1, [sp, #0]
	movs	r1, #232
	lsls	r1, r1, #5
	movs	r6, #21
	adds	r1, #172
	adds	r2, #4
	subs	r3, #40
	ldr	r4, [sp, #68]
	add	r1, r9
	str	r6, [sp, #4]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x211d
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #35
	str	r1, [sp, #4]
	movs	r1, #240
	lsls	r1, r1, #5
	adds	r1, #252
	subs	r2, #16
	subs	r3, #19
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #56]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2d00
	ble.n	sub_081547ee
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r1, #136
	lsls	r1, r1, #6
	adds	r1, #243
	subs	r2, #20
	adds	r3, #16
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #56]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x683b
	subs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #12
	str	r3, [r7, #4]
	cmp	r3, #79
	ble.n	.L_08154844
	movs	r2, #238
	lsls	r2, r2, #7
	movs	r3, #0
	adds	r2, #168
	str	r3, [r7, #24]
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081c0010
	movs	r1, #6
	ldr	r0, [sp, #48]
	bl	sub_08002064
	ldr	r1, [sp, #64]
	adds	r4, r0, #0
	ldr	r3, [r1, #20]
	cmp	r4, r3
	bge.n	.L_08154844
	lsls	r5, r4, #1
	adds	r5, #36
	movs	r3, #8
	ldrsh	r0, [r1, r5]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	adds	r3, r4, #0
	bl	sub_0814cd48
	ldr	r3, [sp, #64]
	ldrsh	r0, [r3, r5]
	movs	r1, #1
	bl	sub_08118088
.L_08154844:
	ldr	r2, [sp, #48]
	adds	r7, #28
	adds	r2, #1
	str	r2, [sp, #48]
	cmp	r2, #24
	beq.n	.L_08154852
	b.n	.L_081546d4
.L_08154852:
	movs	r0, #4
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #44]
	adds	r3, #1
	str	r3, [sp, #44]
	cmp	r3, #88
	beq.n	.L_0815487c
	b.n	.L_081546ce
.L_0815487c:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #128
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0819747a
	.4byte 0x08197467
	.4byte 0x02010000
	.4byte 0x08197473
	.4byte 0x0819746d
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_081548d0
	pop	{pc}
	.2byte 0x0000
