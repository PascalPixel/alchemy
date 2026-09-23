.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08020090, 0x08020090
	.set sub_08020148, 0x08020148
	.set sub_080201c0, 0x080201c0
	.set sub_08020218, 0x08020218
	.set sub_08020258, 0x08020258
	.set sub_08020348, 0x08020348
	.set sub_080d2240, 0x080d2240
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2350, 0x080d2350
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d2ea4, 0x080d2ea4
	.set sub_080d30fc, 0x080d30fc
	.global Func_080d53b8
	.thumb_func
Func_080d53b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #396]
	sub	sp, #24
	ldr	r0, [r1, #0]
	bl	sub_080d2d84
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r2, [r2, #32]
	movs	r3, #1
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	adds	r7, r0, #0
	movs	r0, #10
	ldrsh	r3, [r7, r0]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #240
	ands	r3, r1
	adds	r3, #8
	mov	fp, r3
	movs	r2, #14
	ldrsh	r3, [r7, r2]
	movs	r2, #16
	negs	r2, r2
	ands	r3, r2
	adds	r5, r3, #0
	movs	r0, #18
	ldrsh	r3, [r7, r0]
	adds	r5, #8
	ands	r3, r1
	mov	r8, r3
	movs	r1, #8
	add	r1, r8
	mov	r9, r1
	bl	sub_080d22a8
	adds	r3, r7, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d541e
	ldr	r3, [r7, #80]
	ldrb	r3, [r3, #26]
	str	r3, [sp, #4]
.L_080d541e:
	ldr	r2, [pc, #312]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080d5428
	b.n	.L_080d5564
.L_080d5428:
	movs	r1, #34
	adds	r1, r1, r7
	asrs	r5, r5, #4
	mov	r0, r9
	mov	ip, r5
	asrs	r5, r0, #4
	ldrb	r0, [r1, #0]
	mov	r3, fp
	asrs	r6, r3, #4
	lsls	r3, r0, #3
	mov	sl, r1
	subs	r3, r3, r0
	ldr	r1, [sp, #8]
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r3, [r1, r3]
	lsls	r2, r5, #7
	adds	r2, r6, r2
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #260]
	movs	r4, #64
	adds	r1, r3, r2
	ldrb	r2, [r3, #3]
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d5466
	b.n	.L_080d5658
.L_080d5466:
	ldrb	r2, [r1, #3]
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d5472
	b.n	.L_080d5658
.L_080d5472:
	ldr	r3, [sp, #8]
	movs	r1, #212
	lsls	r1, r1, #1
	adds	r2, r3, r1
	mov	r1, ip
	subs	r3, r5, r1
	lsls	r3, r3, #7
	ldr	r2, [r2, #0]
	adds	r3, r6, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldr	r3, [pc, #208]
	adds	r1, r2, r3
	ldrb	r3, [r1, #2]
	cmp	r3, #255
	bne.n	.L_080d5494
	b.n	.L_080d5658
.L_080d5494:
	mov	r1, fp
	lsls	r5, r1, #16
	mov	r3, r9
	lsls	r2, r3, #16
	adds	r1, r5, #0
	bl	sub_080201c0
	mov	r2, r8
	mov	r1, sl
	subs	r2, #8
	adds	r6, r0, #0
	lsls	r2, r2, #16
	ldrb	r0, [r1, #0]
	adds	r1, r5, #0
	bl	sub_080201c0
	cmp	r6, r0
	blt.n	.L_080d54ba
	b.n	.L_080d5658
.L_080d54ba:
	ldr	r3, [r7, #8]
	add	r0, sp, #12
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	bl	sub_08020258
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d54d4
	b.n	.L_080d5658
.L_080d54d4:
	adds	r6, r7, #0
	adds	r6, #90
	ldr	r2, [pc, #120]
	strb	r5, [r6, #0]
	mov	r1, fp
	ldr	r0, [r2, #0]
	mov	r2, r9
	bl	sub_080d2ea4
	movs	r1, #6
	adds	r0, r7, #0
	bl	sub_08020090
	movs	r0, #4
	bl	sub_08013560
	movs	r1, #7
	adds	r0, r7, #0
	bl	sub_08020090
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	movs	r0, #4
	bl	sub_08013560
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	ldr	r0, [sp, #4]
	movs	r3, #254
	ands	r0, r3
	str	r0, [sp, #4]
	ldr	r1, [sp, #4]
	adds	r0, r7, #0
	bl	sub_08020218
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r1, #12
	adds	r0, r7, #0
	str	r5, [r7, #40]
	bl	sub_08020090
	movs	r0, #4
	bl	sub_08013560
	ldr	r1, [pc, #32]
	movs	r3, #2
	strb	r3, [r1, #0]
	movs	r3, #1
	strb	r3, [r6, #0]
	movs	r0, #8
	bl	sub_08013560
	ldr	r3, [r7, #16]
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r7, #16]
	b.n	.L_080d5650
	.4byte 0x02000454
	.4byte 0x02000452
	.4byte 0xfffffe00
	.2byte 0x0000
	.2byte 0xfff0
.L_080d5564:
	.2byte 0x8c3b
	add	r4, sp, #12
	subs	r3, #2
	mov	sl, r3
	ldr	r3, [r7, #8]
	movs	r0, #128
	str	r3, [r4, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	ldr	r1, [pc, #200]
	movs	r2, #0
	adds	r3, r3, r1
	str	r3, [r4, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	mov	r8, r2
	adds	r6, r5, #0
	adds	r6, #89
.L_080d5590:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080d55bc
	ldrb	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d55bc
	cmp	r5, r7
	beq.n	.L_080d55bc
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r2, r4, #0
	adds	r0, #8
	subs	r3, #2
	mov	r1, sl
	str	r4, [sp, #0]
	bl	sub_08020348
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bge.n	.L_080d5658
.L_080d55bc:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #128
	adds	r5, #128
	cmp	r1, #63
	ble.n	.L_080d5590
	movs	r2, #0
	mov	r8, r2
	adds	r6, r7, #0
	adds	r6, #85
	mov	r3, r8
	strb	r3, [r6, #0]
	adds	r0, r7, #0
	movs	r1, #11
	bl	sub_08020090
	ldr	r2, [r7, #12]
	mov	r0, fp
	movs	r3, #128
	lsls	r1, r0, #16
	lsls	r3, r3, #12
	mov	r0, r9
	adds	r2, r2, r3
	lsls	r3, r0, #16
	ldr	r0, [pc, #88]
	adds	r3, r3, r0
	adds	r0, r7, #0
	bl	sub_08020148
	ldr	r5, [pc, #80]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl	sub_080d30fc
	movs	r3, #3
	strb	r3, [r6, #0]
	ldr	r3, [r7, #12]
	ldr	r6, [pc, #52]
	str	r3, [r7, #20]
	ldr	r2, [sp, #4]
	adds	r0, r7, #0
	orrs	r2, r6
	adds	r1, r2, #0
	str	r2, [sp, #4]
	bl	sub_08020218
	movs	r0, #4
	bl	sub_080d2240
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r5, r5, r3
	adds	r3, r7, #0
	adds	r3, #90
	strb	r6, [r3, #0]
	mov	r1, r8
	adds	r3, #10
	strh	r1, [r3, #0]
	mov	r0, r8
	adds	r3, #2
	mov	r2, r8
	strb	r0, [r5, #0]
	strh	r2, [r3, #0]
	b.n	.L_080d5650
	.4byte 0x00000001
	.4byte 0xfff00000
	.2byte 0x0240
	.2byte 0x0200
.L_080d5650:
	bl	sub_080d2350
	movs	r0, #0
	b.n	.L_080d5660
.L_080d5658:
	bl	sub_080d2350
	movs	r0, #1
	negs	r0, r0
.L_080d5660:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
