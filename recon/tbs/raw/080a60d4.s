.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_08004938, 0x08004938
	.set sub_080150d0, 0x080150d0
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080770d0, 0x080770d0
	.set sub_080a10d0, 0x080a10d0
	.set sub_080a112c, 0x080a112c
	.set sub_080a1804, 0x080a1804
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a23c0, 0x080a23c0
	.set sub_080a33d4, 0x080a33d4
	.set sub_080a6384, 0x080a6384
	.set sub_080a6614, 0x080a6614
	.set sub_080a68ec, 0x080a68ec
	.set sub_080f9010, 0x080f9010
	.global Func_080a60d4
	.thumb_func
Func_080a60d4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r0, [sp, #28]
	ldr	r3, [pc, #152]
	ldr	r7, [r3, #0]
	movs	r1, #30
	ldrsb	r1, [r7, r1]
	movs	r0, #28
	ldrsb	r0, [r7, r0]
	movs	r3, #0
	movs	r2, #1
	str	r1, [sp, #24]
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	str	r3, [sp, #8]
	mov	sl, r0
	add	r1, sp, #16
	movs	r0, #154
	ldrb	r1, [r1, #0]
	lsls	r0, r0, #2
	adds	r3, r7, r0
	strb	r1, [r3, #0]
	ldr	r3, [sp, #28]
	mov	r2, sl
	lsls	r2, r2, #1
	ldrh	r0, [r2, r3]
	bl	sub_08077008
	adds	r5, r7, #0
	movs	r3, #10
	adds	r5, #32
	str	r0, [sp, #12]
	str	r3, [sp, #0]
	movs	r6, #2
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #3
	movs	r3, #17
	str	r6, [sp, #4]
	bl	sub_080a10d0
	cmp	r0, #0
	beq.n	.L_080a613e
	ldr	r1, [r5, #0]
	adds	r0, r7, #0
	bl	sub_080a33d4
.L_080a613e:
	adds	r5, r7, #0
	movs	r3, #4
	adds	r5, #40
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #13
	movs	r3, #17
	str	r6, [sp, #4]
	bl	sub_080a10d0
	cmp	r0, #0
	beq.n	.L_080a6174
	ldr	r0, [sp, #16]
	ldr	r2, [r5, #0]
	movs	r1, #0
	str	r0, [sp, #0]
	movs	r3, #0
	movs	r0, #2
	bl	sub_080150d0
	movs	r1, #135
	lsls	r1, r1, #2
	adds	r3, r7, r1
	str	r0, [r3, #0]
	movs	r3, #13
	strb	r3, [r0, #5]
.L_080a6174:
	movs	r2, #154
	lsls	r2, r2, #2
	adds	r2, r2, r7
	mov	fp, r2
	b.n	.L_080a6338
	movs	r0, r0
	.2byte 0x1f2c
	.2byte 0x0300
.L_080a6184:
	ldr	r3, [sp, #20]
	cmp	r3, #0
	beq.n	.L_080a6230
	movs	r0, #0
	str	r0, [sp, #20]
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #24]
	add	r0, sl
	bl	sub_080022fc
	mov	sl, r0
	mov	r1, sl
	ldr	r5, [sp, #28]
	lsls	r1, r1, #1
	mov	r8, r1
	add	r5, r8
	ldrh	r0, [r5, #0]
	ldr	r6, [r7, #36]
	bl	sub_08077008
	str	r0, [sp, #12]
	ldrh	r0, [r5, #0]
	bl	sub_080a6384
	movs	r2, #0
	movs	r3, #0
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	sub_080a112c
	ldrh	r1, [r5, #0]
	ldr	r0, [r7, #40]
	bl	sub_080a6614
	ldrh	r1, [r5, #0]
	adds	r0, r7, #0
	bl	sub_080a1804
	movs	r0, #165
	lsls	r0, r0, #1
	ldr	r1, [pc, #44]
	mov	r9, r8
	movs	r2, #3
	adds	r3, r7, r0
.L_080a61dc:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080a61dc
	movs	r3, #162
	lsls	r3, r3, #1
	ldr	r2, [pc, #28]
	add	r3, r9
	strh	r2, [r7, r3]
	ldr	r0, [pc, #24]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a6228
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L_080a6228
	b.n	.L_080a6210
	movs	r0, r0
	.4byte 0x0000001e
	.4byte 0x0000001a
	.2byte 0x0151
	.2byte 0x0000
.L_080a6210:
	ldr	r0, [r7, #44]
	bl	sub_08015278
	ldr	r0, [r7, #44]
	bl	sub_08015270
	ldr	r0, [r7, #44]
	bl	sub_080a23c0
	movs	r2, #1
	str	r2, [sp, #8]
	b.n	.L_080a6236
.L_080a6228:
	ldr	r0, [pc, #328]
	bl	sub_080770d0
	b.n	.L_080a6236
.L_080a6230:
	mov	r3, sl
	lsls	r3, r3, #1
	mov	r8, r3
.L_080a6236:
	mov	r0, r8
	add	r0, sl
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #300]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a627a
	movs	r0, #134
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080a6272
	movs	r0, #112
	bl	sub_080f9010
	ldr	r2, [sp, #28]
	mov	r1, r8
	ldrh	r1, [r1, r2]
	str	r1, [sp, #16]
	b.n	.L_080a634c
.L_080a6272:
	movs	r0, #114
	bl	sub_080f9010
	ldr	r1, [pc, #252]
.L_080a627a:
	ldr	r0, [pc, #252]
	ldr	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080a6294
	ldr	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a62ec
.L_080a6294:
	ldr	r3, [sp, #28]
	mov	r2, r8
	ldrh	r2, [r2, r3]
	str	r2, [sp, #16]
	ldr	r3, [r1, #0]
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080a62ac
	movs	r3, #1
	mov	r0, fp
	strb	r3, [r0, #0]
	b.n	.L_080a62b2
.L_080a62ac:
	movs	r3, #2
	mov	r1, fp
	strb	r3, [r1, #0]
.L_080a62b2:
	movs	r0, #64
	bl	sub_08004938
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [sp, #12]
	movs	r2, #1
	bl	sub_080a68ec
	adds	r5, r0, #0
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
	lsls	r5, r5, #24
	adds	r0, r6, #0
	asrs	r5, r5, #24
	bl	sub_08002df0
	cmp	r5, #0
	bne.n	.L_080a62e4
	mov	r2, fp
	strb	r5, [r2, #0]
	movs	r0, #114
	bl	sub_080f9010
	b.n	.L_080a62ec
.L_080a62e4:
	movs	r0, #112
	bl	sub_080f9010
	b.n	.L_080a634c
.L_080a62ec:
	ldr	r0, [pc, #136]
	ldr	r3, [r0, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a6306
	movs	r0, #113
	bl	sub_080f9010
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #16]
	b.n	.L_080a634c
.L_080a6306:
	ldr	r5, [pc, #116]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a6322
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	movs	r3, #1
	negs	r2, r2
	str	r3, [sp, #20]
	add	sl, r2
.L_080a6322:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a6338
	movs	r0, #111
	bl	sub_080f9010
	movs	r0, #1
	str	r0, [sp, #20]
	add	sl, r0
.L_080a6338:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a6346
	b.n	.L_080a6184
.L_080a6346:
	mov	r1, sl
	lsls	r1, r1, #1
	mov	r8, r1
.L_080a634c:
	mov	r2, sl
	strb	r2, [r7, #28]
	ldr	r0, [sp, #28]
	mov	r3, r8
	ldr	r1, [pc, #40]
	ldrh	r2, [r3, r0]
	adds	r3, r7, r1
	str	r2, [r7, #8]
	strb	r2, [r3, #0]
	ldr	r0, [sp, #16]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000151
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000021a
