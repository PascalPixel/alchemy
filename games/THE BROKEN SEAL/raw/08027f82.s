.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08003d28, 0x08003d28
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016738, 0x08016738
	.set sub_080270d8, 0x080270d8
	.set sub_080272a8, 0x080272a8
	.set sub_0802739e, 0x0802739e
	.set sub_080b50d0, 0x080b50d0
	.set sub_080f9010, 0x080f9010
	.global Overlay_08027f82
Overlay_08027f82:
	movs	r0, #110
	bl	sub_080f9010
	add	r1, sp, #64
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #76]
	strh	r1, [r2, #0]
	ldr	r5, [sp, #72]
	adds	r5, #64
	ldrh	r6, [r5, #0]
	cmp	r6, #0
	beq.n	.L_08027fa6
	bl	sub_08004458
	ldrh	r3, [r5, #0]
	muls	r3, r0
	lsrs	r3, r3, #20
	adds	r6, r6, r3
.L_08027fa6:
	ldr	r2, [sp, #76]
	strh	r6, [r2, #4]
	ldr	r3, [sp, #44]
	cmp	r3, #0
	beq.n	.L_08027fce
	ldr	r1, [sp, #84]
	ldr	r0, [sp, #32]
	adds	r3, r0, r1
	subs	r2, r3, #2
	ldrh	r1, [r3, #0]
	ldrh	r3, [r2, #0]
	cmp	r1, r3
	bne.n	.L_08027fce
	lsls	r2, r6, #16
	asrs	r3, r2, #16
	lsrs	r2, r2, #31
	adds	r3, r3, r2
	ldr	r2, [sp, #76]
	asrs	r3, r3, #1
	strh	r3, [r2, #4]
.L_08027fce:
	ldr	r1, [sp, #76]
	movs	r0, #4
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bge.n	.L_08027fe0
	movs	r3, #250
	lsls	r3, r3, #3
	adds	r2, r1, #0
	strh	r3, [r2, #4]
.L_08027fe0:
	add	r3, sp, #60
	ldrh	r3, [r3, #0]
	ldr	r0, [sp, #76]
	strh	r3, [r0, #6]
	add	r0, sp, #56
	ldrh	r0, [r0, #0]
	ldr	r1, [sp, #76]
	strh	r0, [r1, #8]
	add	r1, sp, #68
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #76]
	strh	r1, [r2, #10]
	ldr	r2, [sp, #44]
	adds	r2, #1
	lsls	r3, r2, #2
	str	r3, [sp, #28]
	ldr	r3, [sp, #80]
	lsls	r0, r2, #4
	lsls	r1, r2, #1
	str	r2, [sp, #44]
	str	r0, [sp, #24]
	str	r1, [sp, #32]
	cmp	r2, r3
	bge.n	.L_08028014
	bl	sub_0802739e
.L_08028014:
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #80]
	cmp	r0, r1
	bge.n	.L_08028020
	bl	sub_080272a8
.L_08028020:
	ldr	r2, [sp, #36]
	ldr	r0, [r2, #0]
	ldr	r3, [r0, #80]
	cmp	r3, #0
	beq.n	.L_0802803c
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #8]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #10]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #12]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #14]
.L_0802803c:
	ldr	r0, [r0, #68]
	cmp	r0, #0
	beq.n	.L_08028048
	movs	r1, #1
	bl	sub_08016418
.L_08028048:
	ldr	r0, [sp, #48]
	bl	sub_08003f3c
	ldr	r0, [sp, #52]
	bl	sub_08003f3c
	ldr	r0, [pc, #28]
	bl	sub_08004278
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	b.n	.L_08028078
	.4byte 0x00000045
	.4byte 0x00000044
	.4byte 0x00000056
	.4byte 0x00000053
	.4byte 0x02002224
	.2byte 0x6e81
	.2byte 0x0802
.L_08028078:
	ldr	r3, [r2, #80]
	cmp	r3, #0
	beq.n	.L_08028166
	ldr	r3, [pc, #264]
	ldr	r5, [r3, #0]
	ldr	r3, [r2, #68]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_080280b8
	adds	r7, r5, #0
	adds	r7, #82
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_080280bc
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r1, #16
	movs	r3, #4
	movs	r0, #0
	bl	sub_080162d4
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	str	r0, [r3, #68]
	bl	sub_08016738
	add	r2, sp, #100
	mov	r9, r2
	bl	sub_080270d8
	b.n	.L_080280c4
.L_080280b8:
	adds	r7, r5, #0
	adds	r7, #82
.L_080280bc:
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #68]
.L_080280c4:
	adds	r5, #80
	ldrb	r2, [r5, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #184]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_080280e4
	movs	r0, #1
	negs	r0, r0
	str	r0, [sp, #80]
	b.n	.L_08028156
.L_080280e4:
	ldr	r3, [pc, #168]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_080280fe
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L_08028134
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #80]
	b.n	.L_08028156
.L_080280fe:
	ldrh	r2, [r1, #8]
	adds	r3, r2, #0
	movs	r6, #0
	cmp	r3, #69
	bne.n	.L_0802811a
	ldrh	r3, [r1, #10]
	cmp	r3, #68
	bne.n	.L_0802811a
	ldrh	r3, [r1, #12]
	cmp	r3, #86
	bne.n	.L_0802811a
	ldrh	r3, [r1, #14]
	cmp	r3, #83
	beq.n	.L_08028156
.L_0802811a:
	adds	r3, r2, #0
	cmp	r3, #86
	bne.n	.L_08028132
	ldrh	r3, [r1, #10]
	cmp	r3, #83
	bne.n	.L_08028132
	ldrh	r3, [r1, #12]
	cmp	r3, #83
	bne.n	.L_08028132
	ldrh	r3, [r1, #14]
	cmp	r3, #84
	beq.n	.L_08028134
.L_08028132:
	movs	r6, #1
.L_08028134:
	movs	r0, #1
	bl	sub_080030f8
	ldrb	r2, [r5, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #68]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_080280e4
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #80]
.L_08028156:
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	ldr	r0, [r3, #68]
	cmp	r0, #0
	beq.n	.L_08028166
	movs	r1, #1
	bl	sub_08016418
.L_08028166:
	movs	r0, #0
	bl	sub_080b50d0
	movs	r0, #57
	bl	sub_08002dd8
	ldr	r0, [sp, #80]
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x02002024
	.2byte 0x1f64
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #760]
	ldr	r3, [r3, #0]
	mov	r8, r3
	ldr	r3, [pc, #756]
	ldr	r3, [r3, #0]
	movs	r2, #31
	lsls	r3, r3, #1
	ldr	r1, [pc, #752]
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r7, [r1, r3]
	ldr	r0, [pc, #748]
	adds	r3, r7, r0
	sub	sp, #12
	mov	r6, r8
	cmp	r3, #0
	bge.n	.L_080281c4
	adds	r3, r7, #0
	subs	r3, #253
.L_080281c4:
	movs	r1, #152
	asrs	r3, r3, #2
	lsls	r1, r1, #1
	ldr	r4, [pc, #732]
	adds	r7, r3, r1
	ldr	r3, [sp, #4]
	lsls	r1, r7, #16
	ands	r3, r4
	ldr	r2, [pc, #724]
	lsrs	r1, r1, #16
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #4]
	add	r0, sp, #4
	ldr	r3, [r0, #4]
	ands	r3, r4
	str	r3, [r0, #4]
	bl	sub_08003d28
	movs	r2, #142
	add	r2, r8
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	movs	r5, #0
	mov	r9, r0
	mov	sl, r2
	cmp	r5, r3
	bcs.n	.L_080282bc
	ldr	r4, [pc, #684]
.L_08028202:
	movs	r0, #12
	ldrsh	r2, [r6, r0]
	cmp	r2, #0
	beq.n	.L_080282ae
	mov	r3, r8
	adds	r3, #140
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r5, r3
	bne.n	.L_08028282
	lsls	r3, r7, #3
	subs	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_08028222
	ldr	r0, [pc, #660]
	adds	r3, r3, r0
.L_08028222:
	asrs	r3, r3, #9
	adds	r3, r2, r3
	movs	r2, #14
	ldrsh	r1, [r6, r2]
	subs	r3, #20
	mov	ip, r3
	cmp	r1, #0
	beq.n	.L_08028246
	lsls	r3, r7, #1
	adds	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_0802823c
	adds	r3, #255
.L_0802823c:
	asrs	r3, r3, #8
	adds	r3, r1, r3
	adds	r1, r3, #0
	subs	r1, #20
	b.n	.L_0802825a
.L_08028246:
	lsls	r3, r7, #4
	subs	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_08028250
	adds	r3, #255
.L_08028250:
	asrs	r3, r3, #8
	adds	r1, r3, #0
	subs	r1, #30
	movs	r3, #255
	ands	r1, r3
.L_0802825a:
	adds	r0, r6, #0
	movs	r3, #0
	mov	r2, r9
	stmia	r0!, {r3}
	lsls	r3, r2, #25
	orrs	r3, r1
	mov	r1, ip
	lsls	r2, r1, #16
	orrs	r3, r2
	ldr	r2, [pc, #584]
	orrs	r3, r2
	stmia	r0!, {r3}
	ldrh	r3, [r6, #18]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	str	r3, [r0, #0]
	movs	r1, #246
	b.n	.L_080282a4
.L_08028282:
	adds	r0, r6, #0
	movs	r3, #14
	ldrsh	r1, [r6, r3]
	movs	r3, #0
	stmia	r0!, {r3}
	lsls	r3, r2, #16
	orrs	r1, r3
	ldr	r3, [pc, #552]
	orrs	r1, r3
	ldrh	r3, [r6, #18]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #2]
	stmia	r0!, {r1}
	lsrs	r3, r3, #5
	str	r3, [r0, #0]
	movs	r1, #245
.L_080282a4:
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl	sub_08003dec
	ldr	r4, [sp, #0]
.L_080282ae:
	mov	r1, sl
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	adds	r5, #1
	adds	r6, #20
	cmp	r5, r3
	bcc.n	.L_08028202
.L_080282bc:
	mov	r3, r8
	adds	r3, #148
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_080283ae
	ldr	r3, [pc, #500]
	ldr	r1, [r3, #0]
	cmp	r1, #0
	bne.n	.L_080282d2
	b.n	.L_08028488
.L_080282d2:
	mov	r0, sl
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	bne.n	.L_080282de
	b.n	.L_08028488
.L_080282de:
	ldr	r2, [pc, #484]
	adds	r3, r1, r2
	ldrb	r2, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	movs	r3, #140
	add	r3, r8
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	mov	lr, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	mov	sl, r1
	adds	r1, r3, #0
	lsls	r4, r7, #1
	mov	r2, r8
	adds	r1, #12
	ldrsh	r0, [r2, r1]
	mov	ip, r4
	adds	r3, r4, r7
	ldr	r4, [pc, #436]
	lsls	r3, r3, #2
	adds	r2, r3, r4
	cmp	r2, #0
	bge.n	.L_0802831e
	adds	r4, #255
	adds	r2, r3, r4
.L_0802831e:
	asrs	r2, r2, #8
	subs	r3, r0, r2
	lsls	r3, r3, #8
	adds	r2, r0, r2
	adds	r3, r3, r2
	mov	r2, r8
	adds	r0, r3, #0
	adds	r3, r2, r1
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	ldr	r1, [pc, #408]
	lsls	r2, r7, #5
	adds	r4, r3, #0
	adds	r3, r2, r1
	adds	r0, #23
	adds	r4, #24
	cmp	r3, #0
	bge.n	.L_08028346
	ldr	r1, [pc, #396]
	adds	r3, r2, r1
.L_08028346:
	asrs	r3, r3, #9
	adds	r3, r4, r3
	mov	r1, sl
	adds	r4, r3, #1
	movs	r5, #24
	adds	r1, #102
	cmp	r5, r4
	bcs.n	.L_0802836a
	movs	r6, #255
.L_08028358:
	ldrh	r2, [r1, #0]
	adds	r3, r6, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, r4
	bcc.n	.L_08028358
.L_0802836a:
	mov	r3, r8
	mov	r1, lr
	movs	r2, #12
	ldrsh	r0, [r3, r2]
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L_08028390
	mov	r2, ip
	adds	r3, r2, r7
	ldr	r4, [pc, #328]
	lsls	r1, r3, #2
	adds	r3, r1, r4
	cmp	r3, #0
	bge.n	.L_0802838c
	ldr	r2, [pc, #328]
	adds	r3, r1, r2
.L_0802838c:
	asrs	r3, r3, #8
	subs	r0, r0, r3
.L_08028390:
	mov	r1, sl
	lsls	r0, r0, #8
	adds	r1, #6
	movs	r5, #0
	movs	r4, #255
.L_0802839a:
	ldrh	r2, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #23
	bls.n	.L_0802839a
	b.n	.L_08028488
.L_080283ae:
	ldr	r3, [pc, #272]
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_08028488
	mov	r0, sl
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	.L_08028488
	ldr	r2, [pc, #256]
	adds	r3, r1, r2
	ldrb	r2, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	movs	r3, #140
	add	r3, r8
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	mov	ip, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	mov	lr, r1
	adds	r1, r3, #0
	mov	r2, r8
	adds	r1, #12
	lsls	r6, r7, #1
	ldrsh	r0, [r2, r1]
	ldr	r4, [pc, #216]
	adds	r3, r6, r7
	lsls	r2, r3, #2
	adds	r3, r2, r4
	cmp	r3, #0
	bge.n	.L_080283fe
	adds	r4, #255
	adds	r3, r2, r4
.L_080283fe:
	asrs	r3, r3, #8
	subs	r2, r0, r3
	lsls	r2, r2, #8
	adds	r3, r0, r3
	adds	r2, r2, r3
	adds	r0, r2, #0
	mov	r2, r8
	adds	r3, r2, r1
	movs	r4, #2
	ldrsh	r2, [r3, r4]
	ldr	r4, [pc, #184]
	lsls	r1, r7, #5
	adds	r3, r1, r4
	adds	r0, #23
	cmp	r3, #0
	bge.n	.L_08028422
	ldr	r4, [pc, #176]
	adds	r3, r1, r4
.L_08028422:
	asrs	r3, r3, #9
	subs	r3, r2, r3
	subs	r2, r3, #1
	lsls	r3, r2, #2
	add	r3, lr
	adds	r5, r2, #0
	adds	r1, r3, #6
	cmp	r5, #135
	bhi.n	.L_08028448
	movs	r4, #255
.L_08028436:
	ldrh	r2, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #135
	bls.n	.L_08028436
.L_08028448:
	mov	r2, r8
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	mov	r1, ip
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L_0802846c
	adds	r3, r6, r7
	ldr	r2, [pc, #108]
	lsls	r1, r3, #2
	adds	r3, r1, r2
	cmp	r3, #0
	bge.n	.L_08028468
	ldr	r4, [pc, #108]
	adds	r3, r1, r4
.L_08028468:
	asrs	r3, r3, #8
	subs	r0, r0, r3
.L_0802846c:
	ldr	r1, [pc, #104]
	lsls	r0, r0, #8
	add	r1, lr
	movs	r5, #136
	movs	r4, #255
.L_08028476:
	ldrh	r2, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #159
	bls.n	.L_08028476
.L_08028488:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f38
	.4byte 0x03001e40
	.4byte 0x080366f8
	.4byte 0xffffff00
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.4byte 0x03001b10
	.4byte 0x000001ff
	.4byte 0x80002300
	.4byte 0x80002000
	.4byte 0x03001ecc
	.4byte 0x00000539
	.4byte 0xfffff4ff
	.4byte 0xffffe0ff
	.4byte 0xffffe2fe
	.4byte 0xfffff5fe
	.4byte 0x00000226
