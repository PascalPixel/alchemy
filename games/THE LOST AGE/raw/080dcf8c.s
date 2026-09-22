.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200e8, 0x080200e8
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_08020210, 0x08020210
	.set sub_08020278, 0x08020278
	.set sub_080202e8, 0x080202e8
	.set sub_080202f0, 0x080202f0
	.set sub_080ce31c, 0x080ce31c
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080d2d84, 0x080d2d84
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dc7cc, 0x080dc7cc
	.set sub_080dcf54, 0x080dcf54
	.set sub_080dd528, 0x080dd528
	.set sub_080dd63c, 0x080dd63c
	.set sub_080dd668, 0x080dd668
	.set sub_081c0010, 0x081c0010
	.global Overlay_080dcf8c
Overlay_080dcf8c:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #104]
	sub	sp, #12
	cmp	r0, #0
	beq.n	.L_080dcfd6
	ldr	r2, [r0, #8]
	ldr	r3, [r5, #8]
	subs	r1, r2, r3
	ldr	r2, [r0, #16]
	ldr	r3, [r5, #16]
	subs	r0, r2, r3
	cmp	r1, #0
	bne.n	.L_080dcfac
	cmp	r0, #0
	beq.n	.L_080dcfce
.L_080dcfac:
	bl	sub_080148e8
	ldrh	r3, [r5, #6]
	movs	r2, #128
	subs	r0, r0, r3
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	lsls	r2, r2, #5
	cmp	r0, r2
	ble.n	.L_080dcfc2
	adds	r0, r2, #0
.L_080dcfc2:
	ldr	r2, [pc, #132]
	cmp	r0, r2
	bge.n	.L_080dcfca
	adds	r0, r2, #0
.L_080dcfca:
	adds	r3, r3, r0
	strh	r3, [r5, #6]
.L_080dcfce:
	adds	r2, r5, #0
	adds	r2, #90
	movs	r3, #0
	strb	r3, [r2, #0]
.L_080dcfd6:
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	bl	sub_08014878
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #104]
	lsls	r0, r0, #4
	subs	r3, r3, r0
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	bl	sub_08014878
	lsls	r5, r5, #4
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r0, #209
	lsls	r0, r0, #1
	adds	r0, #255
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080dd044
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #72]
	movs	r1, #0
	bl	sub_08020090
	adds	r2, r5, #0
	adds	r2, #94
	movs	r3, #12
	strh	r3, [r2, #0]
	ldr	r1, [pc, #16]
	adds	r0, r5, #0
	bl	sub_080200a8
.L_080dd044:
	add	sp, #12
	pop	{r5, r6, pc}
	.4byte 0xfffff000
	.4byte 0xfff80000
	.2byte 0x0e54
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #68
	str	r3, [sp, #40]
	movs	r5, #128
	ldr	r0, [r3, #16]
	ldr	r1, [r3, #20]
	ldrh	r3, [r3, #0]
	lsls	r5, r5, #8
	mov	sl, r1
	adds	r3, r3, r5
	str	r3, [sp, #28]
	mov	r3, sl
	movs	r2, #0
	adds	r3, #34
	str	r2, [sp, #24]
	str	r3, [sp, #16]
	mov	fp, r0
	ldrb	r4, [r3, #0]
	mov	r0, sl
	str	r4, [sp, #20]
	str	r2, [sp, #8]
	cmp	r0, #0
	bne.n	.L_080dd098
	b.n	.L_080dd414
.L_080dd098:
	bl	sub_080dc294
	mov	r2, fp
	mov	r1, sl
	str	r1, [r2, #104]
	mov	r0, fp
	ldr	r1, [pc, #892]
	bl	sub_080200a8
	mov	r0, fp
	bl	sub_080dd528
	mov	r9, r0
	cmp	r0, #0
	bne.n	.L_080dd0bc
	bl	sub_080dc384
	b.n	.L_080dd414
.L_080dd0bc:
	ldr	r4, [sp, #40]
	movs	r3, #30
	ldrsh	r0, [r4, r3]
	bl	sub_080ce31c
	ldr	r4, [sp, #40]
	adds	r2, r0, #0
	movs	r0, #160
	lsls	r0, r0, #23
	movs	r3, #30
	ldrsh	r1, [r4, r3]
	adds	r0, #5
	bl	sub_080ce458
	str	r0, [sp, #12]
	mov	r0, sl
	ldr	r3, [r0, #8]
	add	r6, sp, #56
	str	r3, [r6, #0]
	mov	r1, r9
	ldr	r3, [r0, #12]
	str	r0, [r1, #104]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r6, #4]
	mov	r2, sl
	ldr	r3, [r2, #16]
	adds	r2, r6, #0
	str	r3, [r6, #8]
	ldr	r1, [sp, #28]
	bl	sub_0801489c
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	mov	r0, r9
	bl	sub_08020148
	mov	r0, r9
	bl	sub_080dd63c
	movs	r3, #128
	mov	r4, r9
	lsls	r3, r3, #11
	str	r3, [r4, #48]
	str	r5, [r4, #52]
	movs	r3, #4
	adds	r4, #85
	str	r4, [sp, #4]
	strb	r3, [r4, #0]
	ldr	r3, [pc, #772]
	mov	r0, sl
	str	r3, [r0, #108]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r0, #48]
	movs	r3, #204
	add	r1, sp, #24
	lsls	r3, r3, #6
	adds	r3, #51
	ldrb	r1, [r1, #0]
	str	r3, [r0, #52]
	mov	r3, sl
	adds	r3, #90
	strb	r1, [r3, #0]
	ldr	r2, [sp, #16]
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [sp, #40]
	adds	r3, #33
	str	r3, [sp, #0]
	b.n	.L_080dd34c
.L_080dd150:
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #255
	cmp	r7, r4
	bne.n	.L_080dd196
	mov	r0, sl
	ldr	r3, [r0, #8]
	mov	r1, sl
	str	r3, [r6, #0]
	adds	r2, r6, #0
	ldr	r3, [r0, #12]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r6, #4]
	ldr	r3, [r1, #16]
	str	r3, [r6, #8]
	ldr	r1, [sp, #28]
	bl	sub_0801489c
	ldr	r2, [r6, #4]
	ldr	r1, [r6, #0]
	ldr	r3, [r6, #8]
	mov	r0, r9
	bl	sub_08020148
	mov	r0, r9
	movs	r1, #1
	bl	sub_08020090
	mov	r2, r9
	str	r5, [r2, #36]
	str	r5, [r2, #40]
	str	r5, [r2, #44]
	b.n	.L_080dd34c
.L_080dd196:
	mov	r4, sl
	ldr	r3, [r4, #8]
	movs	r5, #128
	str	r3, [r6, #0]
	lsls	r5, r5, #13
	ldr	r3, [r4, #12]
	adds	r0, r5, #0
	adds	r3, r3, r5
	str	r3, [r6, #4]
	adds	r2, r6, #0
	ldr	r3, [r4, #16]
	str	r3, [r6, #8]
	ldr	r1, [sp, #28]
	bl	sub_0801489c
	movs	r0, #128
	lsls	r0, r0, #10
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_0801489c
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	mov	r0, r9
	bl	sub_08020148
	mov	r0, r9
	bl	sub_08020150
	mov	r0, sl
	ldr	r3, [r0, #8]
	adds	r1, r7, #0
	str	r3, [r6, #0]
	adds	r2, r6, #0
	ldr	r3, [r0, #12]
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	adds	r0, r5, #0
	str	r3, [r6, #8]
	bl	sub_0801489c
	mov	r2, sl
	ldr	r3, [r2, #8]
	add	r1, sp, #44
	str	r3, [r1, #0]
	mov	r8, r1
	ldr	r3, [r2, #12]
	movs	r0, #128
	str	r3, [r1, #4]
	lsls	r0, r0, #14
	ldr	r3, [r2, #16]
	mov	r2, r8
	str	r3, [r1, #8]
	adds	r1, r7, #0
	bl	sub_0801489c
	mov	r0, sl
	adds	r1, r6, #0
	bl	sub_08020210
	cmp	r0, #0
	bgt.n	.L_080dd266
	mov	r0, sl
	adds	r1, r6, #0
	bl	sub_080202e8
	cmp	r0, #0
	beq.n	.L_080dd286
	cmp	r0, fp
	bne.n	.L_080dd266
	mov	r3, fp
	ldr	r2, [pc, #516]
	ldr	r0, [r3, #8]
	ldr	r4, [r3, #16]
	ldr	r3, [r6, #0]
	ands	r0, r2
	ands	r3, r2
	ands	r4, r2
	cmp	r0, r3
	bne.n	.L_080dd240
	ldr	r3, [r6, #8]
	ands	r3, r2
	cmp	r4, r3
	beq.n	.L_080dd266
.L_080dd240:
	mov	r2, r8
	ldr	r1, [r2, #0]
	ldr	r5, [pc, #484]
	adds	r3, r1, #0
	ands	r3, r5
	cmp	r0, r3
	bne.n	.L_080dd286
	ldr	r2, [r2, #8]
	adds	r3, r2, #0
	ands	r3, r5
	cmp	r4, r3
	bne.n	.L_080dd286
	mov	r3, fp
	adds	r3, #34
	ldrb	r0, [r3, #0]
	bl	sub_080202f0
	cmp	r0, #0
	beq.n	.L_080dd282
.L_080dd266:
	mov	r0, r9
	movs	r1, #4
	bl	sub_08020090
	ldr	r3, [pc, #448]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080dd34c
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_080dd34c
.L_080dd282:
	movs	r3, #1
	str	r3, [sp, #24]
.L_080dd286:
	movs	r0, #175
	bl	sub_081c0010
	movs	r4, #1
	str	r4, [sp, #8]
	ldr	r4, [sp, #28]
	ldr	r0, [r6, #0]
	subs	r3, r4, r7
	str	r0, [sp, #36]
	ldr	r2, [pc, #408]
	ldr	r1, [r6, #8]
	lsls	r3, r3, #16
	str	r1, [sp, #32]
	lsrs	r3, r3, #30
	ldrb	r1, [r2, r3]
	mov	r0, r9
	bl	sub_08020090
	movs	r0, #15
	bl	sub_08013560
	movs	r0, #204
	mov	r3, sl
	lsls	r0, r0, #6
	movs	r5, #0
	adds	r0, #51
	mov	r1, sl
	adds	r3, #91
	strb	r5, [r3, #0]
	str	r0, [r1, #48]
	str	r0, [r1, #52]
	mov	r0, sl
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	sub_08020148
	ldr	r2, [sp, #4]
	movs	r3, #204
	strb	r5, [r2, #0]
	lsls	r3, r3, #6
	movs	r5, #128
	adds	r3, #51
	mov	r4, r9
	lsls	r5, r5, #13
	str	r3, [r4, #48]
	str	r3, [r4, #52]
	adds	r0, r5, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_0801489c
	ldr	r2, [r6, #4]
	mov	r0, r9
	ldr	r1, [r6, #0]
	adds	r2, r2, r5
	ldr	r3, [r6, #8]
	bl	sub_08020148
	ldr	r0, [sp, #24]
	cmp	r0, #1
	bne.n	.L_080dd330
	ldr	r2, [sp, #40]
	movs	r1, #24
	ldrsh	r0, [r2, r1]
	bl	sub_080d2d84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	mov	r4, fp
	str	r3, [r4, #48]
	str	r3, [r4, #52]
	mov	r0, r8
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	ldr	r3, [r0, #8]
	mov	r0, fp
	bl	sub_08020148
.L_080dd330:
	mov	r0, sl
	bl	sub_08020150
	ldr	r1, [sp, #12]
	cmp	r1, #0
	bne.n	.L_080dd396
	ldr	r2, [sp, #36]
	mov	r3, sl
	str	r2, [r3, #8]
	ldr	r4, [sp, #32]
	str	r1, [r3, #36]
	str	r4, [r3, #16]
	str	r1, [r3, #44]
	b.n	.L_080dd396
.L_080dd34c:
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #0]
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bne.n	.L_080dd36e
	ldr	r0, [pc, #216]
	movs	r3, #129
	ldr	r2, [r0, #4]
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080dd396
	b.n	.L_080dd370
.L_080dd36e:
	ldr	r0, [pc, #200]
.L_080dd370:
	ldr	r0, [r0, #0]
	bl	sub_080dcf54
	ldr	r1, [sp, #40]
	lsls	r0, r0, #16
	ldr	r5, [r1, #36]
	lsrs	r7, r0, #16
	cmp	r5, #0
	bne.n	.L_080dd384
	b.n	.L_080dd150
.L_080dd384:
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x22ff
	lsls	r2, r2, #8
	adds	r7, r0, #0
	adds	r2, #255
	cmp	r7, r2
	beq.n	.L_080dd396
	b.n	.L_080dd196
.L_080dd396:
	add	r3, sp, #20
	ldr	r4, [sp, #16]
	ldrb	r3, [r3, #0]
	mov	r0, sl
	strb	r3, [r4, #0]
	ldr	r3, [sp, #40]
	adds	r3, #64
	ldrb	r1, [r3, #0]
	bl	sub_08020278
	ldr	r4, [sp, #40]
	mov	r0, sl
	ldr	r1, [r4, #60]
	bl	sub_080200a8
	ldr	r0, [sp, #40]
	mov	r1, sl
	ldr	r3, [r0, #56]
	str	r3, [r1, #108]
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	.L_080dd3ec
	mov	r4, r9
	movs	r3, #0
	str	r3, [r4, #108]
	mov	r0, r9
	movs	r1, #4
	bl	sub_08020090
	mov	r0, r9
	ldr	r2, [r0, #12]
	ldr	r1, [r0, #8]
	ldr	r3, [r0, #16]
	bl	sub_080200e8
	ldr	r3, [sp, #40]
	ldr	r0, [sp, #12]
	movs	r2, #24
	ldrsh	r1, [r3, r2]
	movs	r4, #26
	ldrsh	r2, [r3, r4]
	bl	sub_080ceafc
.L_080dd3ec:
	bl	sub_080dc7cc
	ldr	r0, [sp, #24]
	cmp	r0, #1
	bne.n	.L_080dd40a
	ldr	r2, [sp, #40]
	movs	r1, #24
	ldrsh	r0, [r2, r1]
	bl	sub_080d2d84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_080dd40a:
	bl	sub_080dc384
	mov	r0, r9
	bl	sub_080dd668
.L_080dd414:
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080f0e60
	.4byte 0x080db91d
	.4byte 0xfff00000
	.4byte 0x0300122c
	.4byte 0x080f0ee4
	.4byte 0x03001150
