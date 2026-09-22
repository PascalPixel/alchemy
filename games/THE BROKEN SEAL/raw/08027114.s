.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_080040b4, 0x080040b4
	.set sub_080041d8, 0x080041d8
	.set sub_080048f4, 0x080048f4
	.set sub_08004938, 0x08004938
	.set sub_08021848, 0x08021848
	.set sub_080219c8, 0x080219c8
	.set sub_08021a18, 0x08021a18
	.set sub_08021b80, 0x08021b80
	.set sub_08021c64, 0x08021c64
	.set sub_08021e6c, 0x08021e6c
	.set sub_08023178, 0x08023178
	.set sub_08026fa8, 0x08026fa8
	.set sub_080270ac, 0x080270ac
	.set sub_08028020, 0x08028020
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080b5090, 0x080b5090
	.set sub_080b5130, 0x080b5130
	.set sub_080f9010, 0x080f9010
	.global Overlay_08027114
Overlay_08027114:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r0, [sp, #88]
	movs	r0, #128
	lsls	r0, r0, #1
	str	r0, [sp, #68]
	movs	r0, #128
	str	r1, [sp, #84]
	lsls	r0, r0, #3
	movs	r1, #0
	str	r2, [sp, #80]
	str	r1, [sp, #60]
	str	r1, [sp, #56]
	bl	sub_080040b4
	str	r0, [sp, #52]
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_080040b4
	movs	r1, #130
	str	r0, [sp, #48]
	lsls	r1, r1, #1
	movs	r0, #57
	bl	sub_080048f4
	mov	r2, sp
	adds	r2, #92
	str	r2, [sp, #36]
	str	r0, [r2, #0]
	movs	r1, #0
	ldr	r0, [sp, #52]
	bl	sub_08021c64
	ldr	r0, [pc, #136]
	bl	sub_08021a18
	ldr	r0, [pc, #136]
	bl	sub_080219c8
	bl	sub_08021848
	ldr	r3, [pc, #128]
	ldr	r2, [sp, #36]
	ldr	r0, [r3, #0]
	ldr	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #24
	adds	r3, #228
	movs	r2, #7
.L_08027184:
	subs	r2, #1
	stmia	r3!, {r1}
	cmp	r2, #0
	bge.n	.L_08027184
	ldr	r2, [sp, #36]
	ldr	r3, [r2, #0]
	movs	r1, #0
	adds	r3, #36
	movs	r2, #2
.L_08027196:
	subs	r2, #1
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r2, #0
	bge.n	.L_08027196
	ldr	r2, [sp, #36]
	ldr	r1, [r2, #0]
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	str	r3, [r1, #40]
	str	r3, [r1, #44]
	str	r3, [r1, #60]
	str	r3, [r1, #64]
	str	r3, [r1, #80]
	str	r3, [r1, #72]
	str	r3, [r1, #68]
	str	r2, [r1, #76]
	adds	r3, r0, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802729e
	movs	r3, #1
	ldr	r2, [pc, #52]
	str	r3, [r1, #80]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #8]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #10]
	strh	r3, [r2, #12]
	ldr	r3, [pc, #20]
	movs	r5, #0
	strh	r3, [r2, #14]
	adds	r7, r0, #0
	adds	r6, r0, #0
	adds	r7, #80
	adds	r6, #82
	b.n	.L_0802727a
	.4byte 0x00000056
	.4byte 0x00000053
	.4byte 0x00000054
	.4byte 0x06006000
	.4byte 0x06006680
	.4byte 0x03001e74
	.2byte 0x2224
	.2byte 0x0200
.L_08027200:
	ldr	r3, [pc, #432]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_08027220
	adds	r5, #1
	cmp	r5, #24
	ble.n	.L_08027274
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #80]
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	str	r0, [r3, #80]
	b.n	.L_0802729e
.L_08027220:
	ldrh	r2, [r1, #8]
	adds	r3, r2, #0
	movs	r5, #0
	cmp	r3, #86
	bne.n	.L_0802723c
	ldrh	r3, [r1, #10]
	cmp	r3, #83
	bne.n	.L_0802723c
	ldrh	r3, [r1, #12]
	cmp	r3, #83
	bne.n	.L_0802723c
	ldrh	r3, [r1, #14]
	cmp	r3, #84
	beq.n	.L_0802729e
.L_0802723c:
	adds	r3, r2, #0
	cmp	r3, #69
	bne.n	.L_08027254
	ldrh	r3, [r1, #10]
	cmp	r3, #68
	bne.n	.L_08027254
	ldrh	r3, [r1, #12]
	cmp	r3, #86
	bne.n	.L_08027254
	ldrh	r3, [r1, #14]
	cmp	r3, #83
	beq.n	.L_0802729e
.L_08027254:
	ldrh	r3, [r1, #0]
	cmp	r3, #69
	bne.n	.L_0802726c
	ldrh	r3, [r1, #2]
	cmp	r3, #88
	bne.n	.L_0802726c
	ldrh	r3, [r1, #4]
	cmp	r3, #69
	bne.n	.L_0802726c
	ldrh	r3, [r1, #6]
	cmp	r3, #67
	beq.n	.L_08027274
.L_0802726c:
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #80]
	b.n	.L_08027296
.L_08027274:
	movs	r0, #1
	bl	sub_080030f8
.L_0802727a:
	ldrb	r2, [r7, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldrb	r0, [r6, #0]
	ldr	r3, [pc, #304]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	cmp	r0, #0
	beq.n	.L_08027200
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #80]
.L_08027296:
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	movs	r2, #0
	str	r2, [r3, #80]
.L_0802729e:
	movs	r1, #200
	ldr	r0, [pc, #280]
	lsls	r1, r1, #4
	bl	sub_080041d8
.L_080272a8:
	add	r1, sp, #100
	mov	r9, r1
	bl	sub_080270ac
	ldr	r2, [sp, #36]
	ldr	r0, [r2, #0]
	adds	r3, r0, #0
	adds	r3, #38
	movs	r1, #0
	strb	r1, [r3, #0]
	adds	r2, r0, #0
	movs	r3, #1
	negs	r3, r3
	adds	r2, #224
	str	r3, [r2, #0]
	adds	r3, r0, #0
	adds	r3, #216
	movs	r0, #183
	str	r1, [r3, #0]
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080272ea
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	adds	r1, r2, #0
	movs	r3, #1
	adds	r1, #216
	str	r3, [r1, #0]
	adds	r2, #220
	movs	r3, #60
	str	r3, [r2, #0]
.L_080272ea:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	ble.n	.L_080272fa
	movs	r0, #0
	bl	sub_08021e6c
	adds	r6, r0, #0
	b.n	.L_080272fc
.L_080272fa:
	movs	r6, #14
.L_080272fc:
	cmp	r6, #7
	bne.n	.L_08027340
	movs	r0, #12
	bl	sub_08004938
	ldr	r3, [pc, #184]
	ldrb	r3, [r3, #0]
	adds	r6, r0, #0
	cmp	r3, #0
	beq.n	.L_0802731e
	ldr	r3, [pc, #176]
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	movs	r0, #2
	cmp	r2, #0
	bne.n	.L_08027320
.L_0802731e:
	movs	r0, #1
.L_08027320:
	adds	r1, r6, #0
	bl	sub_080b5090
	adds	r5, r0, #0
	movs	r0, #1
	bl	sub_080030f8
	ldrh	r2, [r6, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_08023178
	adds	r0, r6, #0
	bl	sub_08002df0
	b.n	.L_080272a8
.L_08027340:
	cmp	r6, #4
	bne.n	.L_08027376
	bl	sub_08026fa8
	cmp	r0, #0
	bne.n	.L_080272a8
	ldr	r2, [sp, #88]
	movs	r1, #1
	str	r2, [sp, #76]
	str	r1, [sp, #80]
	ldr	r1, [sp, #84]
	ldrh	r3, [r1, #0]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #76]
	ldr	r3, [pc, #104]
	strh	r3, [r1, #4]
	ldr	r2, [sp, #76]
	movs	r3, #99
	strh	r3, [r2, #6]
	ldr	r3, [sp, #76]
	strh	r0, [r3, #8]
	ldr	r0, [sp, #76]
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r3, [r0, #10]
	bl	sub_08028020
.L_08027376:
	cmp	r6, #14
	beq.n	.L_0802737e
	bl	sub_08028020
.L_0802737e:
	movs	r0, #154
	bl	sub_080f9010
	ldr	r2, [sp, #80]
	movs	r1, #0
	str	r1, [sp, #44]
	cmp	r1, r2
	blt.n	.L_08027392
	bl	sub_08028020
.L_08027392:
	mov	r3, sp
	adds	r3, #96
	str	r1, [sp, #28]
	str	r1, [sp, #24]
	str	r1, [sp, #32]
	str	r3, [sp, #20]
	ldr	r0, [sp, #44]
	cmp	r0, #0
	bne.n	.L_080273cc
	ldr	r2, [sp, #36]
	ldr	r1, [r2, #0]
	movs	r0, #0
	adds	r1, #84
	bl	sub_080b5130
	b.n	.L_080273e6
	movs	r0, r0
	.4byte 0x03001f64
	.4byte 0x02002024
	.4byte 0x08026e81
	.4byte 0x03001f54
	.4byte 0x03001ae8
	.2byte 0x7ffe
	.2byte 0x0000
.L_080273cc:
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #28]
	ldr	r3, [r0, #0]
	adds	r3, r1, r3
	adds	r2, r3, #0
	adds	r2, #80
	movs	r1, #3
.L_080273da:
	ldrb	r3, [r2, #0]
	subs	r1, #1
	strb	r3, [r2, #4]
	adds	r2, #1
	cmp	r1, #0
	bge.n	.L_080273da
.L_080273e6:
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #88]
	adds	r2, r2, r3
	str	r2, [sp, #76]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #84]
	ldrh	r0, [r0, r1]
	str	r0, [sp, #64]
	bl	sub_08077008
	ldr	r3, [pc, #104]
	str	r0, [sp, #72]
	ldr	r5, [r3, #0]
	ldr	r2, [sp, #64]
	adds	r3, r5, #0
	adds	r3, #224
	str	r2, [r3, #0]
	movs	r2, #0
	ldr	r3, [pc, #92]
	str	r2, [r5, #64]
	adds	r5, #24
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	ldr	r0, [sp, #72]
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r0, [r3, #0]
	ldr	r1, [sp, #48]
	bl	sub_08021b80
	ldr	r3, [pc, #56]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #64]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldrh	r2, [r5, #6]
	ldr	r3, [pc, #56]
	ands	r3, r2
	strh	r3, [r5, #6]
	ldrb	r2, [r5, #9]
	movs	r3, #128
	strb	r3, [r5, #4]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r2
	strb	r3, [r5, #9]
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #1
	adds	r2, #38
	strb	r3, [r2, #0]
