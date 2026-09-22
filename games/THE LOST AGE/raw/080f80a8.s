.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080146d4, 0x080146d4
	.set sub_0801479c, 0x0801479c
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_080202d8, 0x080202d8
	.set sub_080202e0, 0x080202e0
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038068, 0x08038068
	.set sub_08038080, 0x08038080
	.set sub_08038090, 0x08038090
	.set sub_080380a8, 0x080380a8
	.set sub_080380b0, 0x080380b0
	.set sub_080380d8, 0x080380d8
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038270, 0x08038270
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad048, 0x080ad048
	.set sub_080ad1c0, 0x080ad1c0
	.set sub_080ad298, 0x080ad298
	.set sub_080f8658, 0x080f8658
	.set sub_080f8708, 0x080f8708
	.set sub_08105580, 0x08105580
	.global Overlay_080f80a8
Overlay_080f80a8:
	push	{lr}
	bl	sub_0801479c
	bl	sub_080202e0
	movs	r0, #179
	lsls	r0, r0, #1
	bl	sub_08016d18
	movs	r0, #169
	lsls	r0, r0, #1
	bl	sub_08016d18
	pop	{pc}
	push	{lr}
	movs	r0, #169
	lsls	r0, r0, #1
	bl	sub_08016cfc
	movs	r0, #179
	lsls	r0, r0, #1
	bl	sub_08016cfc
	bl	sub_080202d8
	bl	sub_080146d4
	pop	{pc}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #4
	adds	r3, #220
	ldr	r4, [r3, #0]
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r4, #0
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #255
	strb	r3, [r4, #28]
	adds	r2, r4, #0
	movs	r3, #1
	strb	r3, [r4, #30]
	strb	r3, [r4, #31]
	adds	r2, #246
	adds	r4, #247
	strb	r3, [r2, #0]
	add	sp, #4
	strb	r3, [r4, #0]
	bx	lr
	movs	r0, r0
	.2byte 0x033b
	.2byte 0x8500
.L_080f811c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r0, [r6, #0]
	sub	sp, #4
	adds	r5, r3, #0
	ldr	r4, [sp, #20]
	cmp	r0, #0
	beq.n	.L_080f8142
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080f813a
	movs	r0, #0
	b.n	.L_080f8158
.L_080f813a:
	bl	sub_08038260
	movs	r0, #0
	b.n	.L_080f8158
.L_080f8142:
	movs	r3, #255
	ands	r4, r3
	adds	r0, r1, #0
	ldr	r3, [sp, #16]
	adds	r1, r2, #0
	adds	r2, r5, #0
	str	r4, [sp, #0]
	bl	sub_08038010
	str	r0, [r6, #0]
	movs	r0, #1
.L_080f8158:
	add	sp, #4
	pop	{r5, r6, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_080f816e
	bl	sub_08038018
	movs	r3, #0
	str	r3, [r5, #0]
.L_080f816e:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r2, [sp, #24]
	movs	r2, #0
	str	r3, [sp, #20]
	str	r2, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	fp, r1
	mov	r0, fp
	mov	r9, r3
	bl	sub_08016ca4
	ldr	r3, [sp, #24]
	mov	sl, r0
	lsls	r3, r3, #1
	str	r3, [sp, #8]
	adds	r3, #216
	ldrh	r3, [r0, r3]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	mov	r8, r3
	bl	sub_080ad010
	ldr	r2, [sp, #20]
	movs	r5, #128
	lsls	r5, r5, #1
	ands	r5, r2
	str	r0, [sp, #12]
	cmp	r5, #0
	bne.n	.L_080f81da
	movs	r3, #12
	str	r3, [sp, #0]
	mov	r0, r9
	adds	r3, #246
	str	r3, [sp, #4]
	adds	r0, #40
	movs	r1, #0
	movs	r2, #5
	movs	r3, #13
	bl	.L_080f811c
	str	r0, [sp, #16]
.L_080f81da:
	mov	r3, r9
	ldr	r7, [r3, #40]
	cmp	r5, #0
	bne.n	.L_080f8294
	ldr	r5, [sp, #16]
	cmp	r5, #0
	bne.n	.L_080f8200
	movs	r0, #1
	bl	sub_08013560
	mov	r2, r9
	movs	r3, #32
	ldr	r0, [r2, #40]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #88
	bl	sub_08038068
.L_080f8200:
	movs	r3, #0
	mov	r0, sl
	adds	r1, r7, #0
	movs	r2, #32
	bl	sub_08038090
	add	r6, sp, #28
	adds	r0, r6, #0
	movs	r1, #1
	mov	r2, fp
	bl	sub_08105580
	ldrb	r3, [r6, #1]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080f822e
	ldr	r0, [pc, #800]
	adds	r1, r7, #0
	movs	r2, #32
	movs	r3, #8
	bl	sub_08038080
	movs	r5, #1
.L_080f822e:
	ldrb	r3, [r6, #2]
	cmp	r3, #0
	beq.n	.L_080f8244
	lsls	r3, r5, #3
	ldr	r0, [pc, #784]
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #32
	bl	sub_08038080
	adds	r5, #1
.L_080f8244:
	ldrb	r3, [r6, #3]
	cmp	r3, #0
	beq.n	.L_080f825a
	lsls	r3, r5, #3
	ldr	r0, [pc, #764]
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #32
	bl	sub_08038080
	adds	r5, #1
.L_080f825a:
	ldrb	r3, [r6, #4]
	cmp	r3, #0
	beq.n	.L_080f8270
	lsls	r3, r5, #3
	ldr	r0, [pc, #748]
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #32
	bl	sub_08038080
	adds	r5, #1
.L_080f8270:
	cmp	r5, #1
	bgt.n	.L_080f8294
	mov	r3, sl
	ldrb	r6, [r3, #15]
	ldr	r0, [pc, #728]
	adds	r1, r7, #0
	movs	r2, #40
	movs	r3, #16
	bl	sub_08038090
	movs	r3, #16
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #4
	adds	r2, r7, #0
	movs	r3, #56
	bl	sub_080380b0
.L_080f8294:
	ldr	r5, [sp, #16]
	cmp	r5, #0
	bne.n	.L_080f82b2
	movs	r0, #1
	bl	sub_08013560
	mov	r2, r9
	movs	r3, #80
	ldr	r0, [r2, #40]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #32
	movs	r3, #88
	bl	sub_08038068
.L_080f82b2:
	adds	r0, r7, #0
	bl	sub_08038268
	ldr	r5, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_080f82dc
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, fp
	adds	r3, r7, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080380d8
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, r9
	str	r0, [r3, #0]
.L_080f82dc:
	ldr	r2, [sp, #20]
	movs	r3, #255
	ands	r3, r2
	cmp	r3, #9
	bls.n	.L_080f82e8
	b.n	.L_080f85fe
.L_080f82e8:
	ldr	r2, [pc, #620]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080f83f0
	.4byte 0x080f85fe
	.4byte 0x080f8458
	.4byte 0x080f8458
	.4byte 0x080f84dc
	.4byte 0x080f85fe
	.4byte 0x080f8436
	.4byte 0x080f85fe
	.4byte 0x080f8578
	.2byte 0x8318
	.2byte 0x080f
	movs	r3, #128
	lsls	r3, r3, #2
	mov	r5, r8
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080f8336
	movs	r3, #152
	lsls	r3, r3, #2
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080f83f0
	b.n	.L_080f8346
.L_080f8336:
	movs	r3, #152
	lsls	r3, r3, #2
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080f83f0
.L_080f8346:
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #524]
	adds	r1, r7, #0
	adds	r6, r3, r2
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #32
	bl	sub_08038080
	mov	r0, fp
	mov	r1, r8
	bl	sub_080ad1c0
	cmp	r0, #0
	bne.n	.L_080f8372
	ldr	r0, [pc, #500]
	adds	r1, r7, #0
	movs	r2, #16
	b.n	.L_080f846a
.L_080f8372:
	movs	r5, #166
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	sub_08014d78
	ldr	r3, [pc, #484]
	mov	r1, sl
	adds	r2, r5, #0
	mov	r8, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2398
	lsls	r3, r3, #2
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080f83b4
	ldr	r1, [sp, #8]
	mov	r3, sl
	adds	r1, #216
	ldrh	r2, [r3, r1]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	mov	r5, sl
	strh	r3, [r5, r1]
	mov	r0, fp
	bl	sub_080ad298
	b.n	.L_080f83bc
.L_080f83b4:
	mov	r0, fp
	ldr	r1, [sp, #24]
	bl	sub_080ad048
.L_080f83bc:
	mov	r0, fp
	bl	sub_080ad008
	movs	r1, #241
	movs	r3, #0
	lsls	r1, r1, #8
	str	r3, [sp, #0]
	adds	r1, #42
	adds	r0, r7, #0
	movs	r2, #3
	movs	r3, #5
	bl	sub_08038270
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #380]
	adds	r1, r7, #0
	adds	r6, r2, r3
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08038080
	b.n	.L_080f84c6
.L_080f83f0:
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #352]
	adds	r1, r7, #0
	adds	r6, r2, r3
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #32
	bl	sub_08038080
	mov	r0, sl
	adds	r1, r7, #0
	bl	sub_080f8658
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r6, [r3, #0]
	ldr	r0, [pc, #332]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #64
	bl	sub_08038080
	movs	r3, #72
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #8
	adds	r2, r7, #0
	movs	r3, #24
	bl	sub_080380b0
	b.n	.L_080f85fe
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #284]
	adds	r1, r7, #0
	adds	r6, r2, r3
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #32
	bl	sub_08038080
	mov	r0, sl
	adds	r1, r7, #0
	bl	sub_080f8658
	b.n	.L_080f85fe
	mov	r0, fp
	mov	r1, r8
	bl	sub_080ad1c0
	cmp	r0, #0
	bne.n	.L_080f8472
	ldr	r0, [pc, #248]
	adds	r1, r7, #0
	movs	r2, #0
.L_080f846a:
	movs	r3, #48
	bl	sub_08038080
	b.n	.L_080f85fe
.L_080f8472:
	movs	r5, #166
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	sub_08014d78
	ldr	r3, [pc, #228]
	mov	r1, sl
	adds	r2, r5, #0
	mov	r8, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2398
	lsls	r3, r3, #2
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080f84ae
	ldr	r1, [sp, #8]
	mov	r3, sl
	adds	r1, #216
	ldrh	r2, [r3, r1]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	mov	r5, sl
	strh	r3, [r5, r1]
	b.n	.L_080f84b6
.L_080f84ae:
	mov	r0, fp
	ldr	r1, [sp, #24]
	bl	sub_080ad048
.L_080f84b6:
	mov	r0, fp
	bl	sub_080ad008
	mov	r0, sl
	mov	r1, r8
	adds	r2, r7, #0
	bl	sub_080f8708
.L_080f84c6:
	movs	r2, #166
	ldr	r3, [pc, #152]
	mov	r0, sl
	mov	r1, r8
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4640
	bl	sub_08013164
	b.n	.L_080f85fe
	ldr	r2, [sp, #12]
	movs	r3, #88
	mov	r5, sl
	ldrh	r4, [r2, #40]
	ldrh	r2, [r5, r3]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	ands	r3, r2
	movs	r0, #0
	movs	r1, #0
	b.n	.L_080f850a
.L_080f84f4:
	adds	r1, #1
	cmp	r1, #31
	bgt.n	.L_080f8510
	lsls	r3, r1, #2
	adds	r3, #88
	mov	r5, sl
	ldrh	r2, [r5, r3]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	ands	r3, r2
.L_080f850a:
	cmp	r3, r4
	bne.n	.L_080f84f4
	movs	r0, #1
.L_080f8510:
	cmp	r0, #0
	beq.n	.L_080f8526
	ldr	r0, [pc, #84]
	adds	r1, r7, #0
	adds	r0, r4, r0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08038080
	ldr	r0, [pc, #76]
	b.n	.L_080f8536
.L_080f8526:
	ldr	r0, [pc, #68]
	adds	r1, r7, #0
	adds	r0, r4, r0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08038080
	ldr	r0, [pc, #60]
.L_080f8536:
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #56
	bl	sub_08038080
	b.n	.L_080f85fe
	movs	r0, r0
	.4byte 0x00001107
	.4byte 0x00001108
	.4byte 0x00001109
	.4byte 0x0000110a
	.4byte 0x08105938
	.4byte 0x080f82f0
	.4byte 0x00000b63
	.4byte 0x00001050
	.4byte 0x03000730
	.4byte 0x0000103d
	.4byte 0x000005a7
	.4byte 0x00001052
	.2byte 0x1051
	.2byte 0x0000
	ldr	r5, [pc, #144]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #40
	bl	sub_08038080
	mov	r2, sl
	ldrh	r6, [r2, #60]
	movs	r3, #40
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #3
	adds	r2, r7, #0
	movs	r3, #64
	bl	sub_080380b0
	adds	r0, r5, #1
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08038080
	mov	r3, sl
	ldrh	r6, [r3, #62]
	movs	r3, #48
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #3
	adds	r2, r7, #0
	movs	r3, #64
	bl	sub_080380b0
	adds	r0, r5, #4
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #56
	bl	sub_08038080
	mov	r3, sl
	adds	r3, #64
	ldrh	r6, [r3, #0]
	movs	r3, #56
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #3
	adds	r2, r7, #0
	movs	r3, #64
	adds	r5, #3
	bl	sub_080380b0
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #64
	bl	sub_08038080
	mov	r3, sl
	adds	r3, #66
	ldrb	r6, [r3, #0]
	movs	r1, #3
	movs	r3, #64
	adds	r0, r6, #0
	adds	r2, r7, #0
	str	r3, [sp, #0]
	bl	sub_080380b0
.L_080f85fe:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x104b
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	adds	r7, r0, #0
	mov	sl, r1
	adds	r6, r2, #0
	mov	r8, r3
	movs	r5, #1
	b.n	.L_080f8628
.L_080f8626:
	adds	r5, #1
.L_080f8628:
	cmp	r5, #15
	bgt.n	.L_080f8636
	movs	r1, #10
	bl	sub_08002054
	cmp	r0, #9
	bgt.n	.L_080f8626
.L_080f8636:
	adds	r5, #1
	lsls	r3, r5, #3
	subs	r6, r6, r3
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	mov	r2, sl
	adds	r3, r6, #0
	bl	sub_080380a8
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
