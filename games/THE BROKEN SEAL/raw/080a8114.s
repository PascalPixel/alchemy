.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08015068, 0x08015068
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_08077290, 0x08077290
	.set sub_080a10d0, 0x080a10d0
	.set sub_080a1804, 0x080a1804
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1ac0, 0x080a1ac0
	.set sub_080a847c, 0x080a847c
	.set sub_080a8508, 0x080a8508
	.set sub_080a8578, 0x080a8578
	.set sub_080a8604, 0x080a8604
	.set sub_080a8b10, 0x080a8b10
	.set sub_080a9d84, 0x080a9d84
	.set sub_080f9010, 0x080f9010
	.global Overlay_080a8114
Overlay_080a8114:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #100]
	sub	sp, #40
	movs	r0, #0
	ldr	r7, [r3, #0]
	mov	sl, r0
	str	r0, [sp, #28]
	mov	r8, r0
	subs	r0, #1
	bl	sub_08077290
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	str	r3, [sp, #12]
	movs	r3, #5
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #30
	adds	r0, #44
	bl	sub_080a10d0
	ldr	r0, [pc, #52]
	bl	sub_08004278
	ldr	r0, [pc, #52]
	ldr	r1, [pc, #36]
	movs	r2, #3
	adds	r3, r7, r0
.L_080a8162:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080a8162
	movs	r1, #0
	movs	r0, #10
	str	r1, [sp, #16]
	negs	r0, r0
	movs	r1, #88
	bl	sub_080a1ac0
	ldr	r2, [pc, #24]
	adds	r2, r7, r2
	str	r2, [sp, #8]
	b.n	.L_080a8406
	movs	r0, r0
	.4byte 0x00000068
	.4byte 0x03001f2c
	.4byte 0x080a19a1
	.4byte 0x00000242
	.2byte 0x021a
	.2byte 0x0000
.L_080a8198:
	ldr	r3, [sp, #8]
	ldrb	r0, [r3, #0]
	bl	sub_08077008
	ldr	r2, [sp, #8]
	ldr	r0, [r7, #36]
	ldrb	r1, [r2, #0]
	movs	r2, #1
	bl	sub_080a8604
	movs	r3, #32
	ldr	r0, [sp, #8]
	add	r3, sp
	mov	fp, r3
	ldrb	r2, [r0, #0]
	movs	r1, #1
	mov	r0, fp
	bl	sub_080a8b10
	lsls	r0, r0, #24
	movs	r1, #0
	lsrs	r2, r0, #24
	str	r1, [sp, #20]
	str	r2, [sp, #24]
	cmp	r0, #0
	bne.n	.L_080a81f2
	movs	r3, #1
	str	r3, [sp, #24]
	b.n	.L_080a81f6
.L_080a81d2:
	movs	r0, #112
	bl	sub_080f9010
	movs	r0, #1
	str	r0, [sp, #16]
	str	r0, [sp, #28]
	b.n	.L_080a8406
.L_080a81e0:
	movs	r0, #113
	bl	sub_080f9010
	movs	r2, #1
	movs	r1, #1
	negs	r2, r2
	str	r1, [sp, #16]
	str	r2, [sp, #28]
	b.n	.L_080a8406
.L_080a81f2:
	movs	r3, #1
	str	r3, [sp, #20]
.L_080a81f6:
	movs	r0, #1
	mov	r9, r0
	b.n	.L_080a83f8
.L_080a81fc:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_080a82ba
	movs	r2, #0
	ldr	r0, [sp, #24]
	mov	r9, r2
	mov	r2, sl
	adds	r2, #2
	lsls	r3, r0, #24
	asrs	r1, r3, #24
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	mov	sl, r2
	cmp	r2, #0
	bne.n	.L_080a8256
	mov	r2, r8
	adds	r0, r2, r1
	bl	sub_080022fc
	mov	r8, r0
	ldr	r0, [r7, #44]
	bl	sub_08015270
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne.n	.L_080a8286
	ldr	r5, [pc, #432]
	movs	r6, #24
	negs	r6, r6
	ldr	r1, [r7, #36]
	adds	r0, r5, #0
	movs	r2, #80
	adds	r3, r6, #0
	bl	sub_08015080
	ldr	r1, [r7, #36]
	adds	r0, r5, #1
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_08015080
	b.n	.L_080a8286
.L_080a8256:
	ldr	r0, [r7, #44]
	bl	sub_08015270
	ldr	r0, [sp, #12]
	cmp	r0, #0
	beq.n	.L_080a827a
	mov	r2, r8
	adds	r2, #8
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080a8270
	mov	r3, r8
	adds	r3, #15
.L_080a8270:
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r2, r2, r3
	mov	r8, r2
	b.n	.L_080a8286
.L_080a827a:
	mov	r0, r8
	adds	r0, #7
	movs	r1, #7
	bl	sub_080022fc
	mov	r8, r0
.L_080a8286:
	mov	r1, r8
	mov	r2, fp
	movs	r3, #0
	mov	r0, sl
	bl	sub_080a847c
	ldr	r0, [r7, #44]
	bl	sub_08015278
	movs	r0, #1
	bl	sub_080030f8
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_080a82b0
	ldr	r0, [r7, #44]
	mov	r1, r8
	mov	r2, fp
	bl	sub_080a8508
	b.n	.L_080a82ba
.L_080a82b0:
	ldr	r0, [r7, #44]
	mov	r1, r8
	ldr	r2, [sp, #12]
	bl	sub_080a8578
.L_080a82ba:
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080a82d6
	mov	r3, r8
	lsls	r1, r3, #4
	movs	r0, #10
	adds	r1, #88
	negs	r0, r0
	bl	sub_080a1a40
	b.n	.L_080a82f4
.L_080a82d6:
	mov	r0, r8
	cmp	r0, #3
	bgt.n	.L_080a82e8
	lsls	r1, r0, #3
	adds	r1, #48
	movs	r0, #24
	bl	sub_080a1a40
	b.n	.L_080a82f4
.L_080a82e8:
	mov	r2, r8
	lsls	r1, r2, #3
	adds	r1, #80
	movs	r0, #48
	bl	sub_080a1a40
.L_080a82f4:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r5, [pc, #240]
	ldr	r2, [r5, #0]
	movs	r3, #240
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a8312
	mov	r0, sl
	mov	r1, r8
	mov	r2, fp
	movs	r3, #1
	bl	sub_080a847c
.L_080a8312:
	ldr	r1, [pc, #220]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a8320
	b.n	.L_080a81d2
.L_080a8320:
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a832c
	b.n	.L_080a81e0
.L_080a832c:
	ldr	r2, [r5, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a8346
	movs	r0, #111
	bl	sub_080f9010
	movs	r0, #1
	movs	r3, #1
	negs	r0, r0
	mov	r9, r3
	add	r8, r0
.L_080a8346:
	ldr	r2, [r5, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a835c
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	mov	r9, r1
	add	r8, r1
.L_080a835c:
	ldr	r2, [r5, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a8372
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	mov	r9, r2
	add	sl, r2
.L_080a8372:
	ldr	r2, [r5, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a838c
	movs	r0, #111
	bl	sub_080f9010
	movs	r0, #1
	movs	r3, #1
	negs	r0, r0
	mov	r9, r3
	add	sl, r0
.L_080a838c:
	ldr	r3, [r5, #0]
	movs	r6, #128
	lsls	r6, r6, #1
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080a83a4
	ldr	r2, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a83f8
.L_080a83a4:
	movs	r0, #111
	bl	sub_080f9010
	ldr	r3, [r5, #0]
	ands	r3, r6
	movs	r0, #28
	ldrsb	r0, [r7, r0]
	cmp	r3, #0
	beq.n	.L_080a83ba
	adds	r0, #1
	b.n	.L_080a83bc
.L_080a83ba:
	subs	r0, #1
.L_080a83bc:
	ldr	r1, [pc, #52]
	adds	r3, r7, r1
	ldrb	r1, [r3, #0]
	adds	r0, r0, r1
	bl	sub_080022fc
	movs	r3, #130
	lsls	r2, r0, #1
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrh	r3, [r7, r2]
	str	r3, [r7, #8]
	ldr	r1, [sp, #8]
	ldrh	r3, [r7, r2]
	strb	r3, [r1, #0]
	strb	r0, [r7, #28]
	adds	r0, r7, #0
	ldrh	r1, [r7, r2]
	bl	sub_080a1804
	b.n	.L_080a8406
	movs	r0, r0
	.4byte 0x00000b06
	.4byte 0x03001b04
	.4byte 0x03001c94
	.2byte 0x0219
	.2byte 0x0000
.L_080a83f8:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a8406
	b.n	.L_080a81fc
.L_080a8406:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L_080a841a
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a841a
	b.n	.L_080a8198
.L_080a841a:
	ldr	r0, [r7, #44]
	bl	sub_08015278
	ldr	r0, [r7, #44]
	bl	sub_08015270
	movs	r3, #96
	ldr	r0, [r7, #36]
	movs	r2, #56
	str	r3, [sp, #0]
	movs	r1, #64
	movs	r3, #224
	bl	sub_08015068
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	bl	sub_080041d8
	ldr	r1, [pc, #16]
	ldr	r3, [pc, #8]
	movs	r2, #3
	adds	r0, r7, r1
	b.n	.L_080a8458
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x080a19a1
	.2byte 0x0242
	.2byte 0x0000
.L_080a8458:
	subs	r2, #1
	strh	r3, [r0, #0]
	subs	r0, #2
	cmp	r2, #0
	bge.n	.L_080a8458
	bl	sub_080a9d84
	ldr	r0, [sp, #28]
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
