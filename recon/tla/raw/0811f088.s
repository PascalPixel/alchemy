.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_080148e8, 0x080148e8
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08038130, 0x08038130
	.set sub_080382a0, 0x080382a0
	.set sub_0811a484, 0x0811a484
	.set sub_0811bdb0, 0x0811bdb0
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811be68, 0x0811be68
	.set sub_0811ddd8, 0x0811ddd8
	.set sub_0811e3ac, 0x0811e3ac
	.set sub_0811e7dc, 0x0811e7dc
	.set sub_0811e830, 0x0811e830
	.set sub_0811f030, 0x0811f030
	.set sub_081201c4, 0x081201c4
	.set sub_08120360, 0x08120360
	.set sub_081234f0, 0x081234f0
	.set sub_08126cfc, 0x08126cfc
	.set sub_08127308, 0x08127308
	.set sub_0812756c, 0x0812756c
	.set sub_08127ba0, 0x08127ba0
	.set sub_08138008, 0x08138008
	.set sub_08138018, 0x08138018
	.set sub_081c0010, 0x081c0010
	.global Func_0811f088
	.thumb_func
Func_0811f088:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	adds	r7, r0, #0
	ldr	r5, [r3, #0]
	ldr	r3, [r7, #88]
	movs	r2, #128
	lsls	r2, r2, #11
	ands	r3, r2
	sub	sp, #92
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L_0811f0c6
	ldrb	r3, [r7, #0]
	cmp	r3, #7
	bhi.n	.L_0811f0ba
	ldr	r3, [pc, #608]
	b.n	.L_0811f0be
.L_0811f0ba:
	movs	r3, #160
	lsls	r3, r3, #7
.L_0811f0be:
	str	r3, [r5, #0]
	movs	r3, #60
	str	r3, [r5, #4]
	b.n	.L_0811f140
.L_0811f0c6:
	ldrb	r0, [r7, #0]
	bl	sub_0811be3c
	ldrb	r1, [r7, #3]
	ldr	r3, [r0, #0]
	mov	r8, r1
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #16]
	ldrb	r6, [r7, #0]
	bl	sub_080148e8
	ldr	r2, [pc, #572]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r1, r0, r2
	cmp	r6, #7
	bls.n	.L_0811f0ee
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r1, r0, r3
.L_0811f0ee:
	lsls	r3, r1, #16
	asrs	r1, r3, #16
	cmp	r6, #7
	bhi.n	.L_0811f0fc
	movs	r3, #128
	lsls	r3, r3, #6
	b.n	.L_0811f0fe
.L_0811f0fc:
	ldr	r3, [pc, #536]
.L_0811f0fe:
	subs	r3, r3, r1
	lsls	r2, r3, #1
	adds	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_0811f10a
	adds	r2, #3
.L_0811f10a:
	asrs	r3, r2, #2
	adds	r1, r1, r3
	mov	r2, r8
	cmp	r2, #7
	bhi.n	.L_0811f122
	movs	r3, #0
	cmp	r6, #7
	bhi.n	.L_0811f11c
	movs	r3, #1
.L_0811f11c:
	cmp	r3, #0
	bne.n	.L_0811f12e
	b.n	.L_0811f138
.L_0811f122:
	movs	r3, #0
	cmp	r6, #7
	bls.n	.L_0811f12a
	movs	r3, #1
.L_0811f12a:
	cmp	r3, #0
	beq.n	.L_0811f138
.L_0811f12e:
	movs	r1, #144
	lsls	r1, r1, #6
	cmp	r6, #7
	bls.n	.L_0811f138
	ldr	r1, [pc, #488]
.L_0811f138:
	ldr	r3, [r5, #0]
	cmp	r3, r1
	beq.n	.L_0811f140
	str	r1, [r5, #0]
.L_0811f140:
	ldr	r3, [r7, #88]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811f160
	ldrb	r3, [r7, #0]
	cmp	r3, #7
	bhi.n	.L_0811f156
	ldr	r3, [pc, #452]
	b.n	.L_0811f15a
.L_0811f156:
	movs	r3, #128
	lsls	r3, r3, #6
.L_0811f15a:
	str	r3, [r5, #0]
	movs	r3, #60
	str	r3, [r5, #4]
.L_0811f160:
	add	r6, sp, #4
	adds	r1, r6, #0
	mov	r8, fp
	adds	r0, r7, #0
	bl	sub_0811ddd8
	mov	r1, r8
	movs	r3, #1
	ands	r1, r3
	mov	r8, r1
	cmp	r1, #0
	beq.n	.L_0811f17a
	str	r3, [r6, #28]
.L_0811f17a:
	movs	r5, #192
	movs	r1, #0
	movs	r0, #0
	lsls	r5, r5, #18
	bl	sub_08126cfc
	ldr	r3, [r5, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	sub_08038130
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_0811e3ac
	mov	sl, r0
	ldr	r0, [r6, #8]
	bl	sub_0811be3c
	ldr	r3, [r5, #36]
	movs	r2, #128
	ldr	r0, [r0, #0]
	lsls	r2, r2, #4
	adds	r2, #105
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	mov	r9, r0
	bl	sub_08020090
	movs	r1, #16
	mov	r0, r9
	bl	sub_08020098
	movs	r0, #154
	bl	sub_081c0010
	movs	r3, #2
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0811f1e0
	ldr	r0, [r6, #8]
	ldr	r1, [r7, #80]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08127308
	b.n	.L_0811f1f2
.L_0811f1e0:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_0811f1f2
	ldr	r0, [r6, #8]
	ldr	r1, [r7, #80]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08127308
.L_0811f1f2:
	ldrb	r3, [r7, #3]
	cmp	r3, #7
	bhi.n	.L_0811f1fc
	movs	r3, #1
	b.n	.L_0811f1fe
.L_0811f1fc:
	movs	r3, #0
.L_0811f1fe:
	str	r3, [r6, #4]
	ldr	r3, [r6, #20]
	movs	r4, #0
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L_0811f252
	movs	r5, #0
.L_0811f20c:
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #0]
	bl	sub_0811be3c
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	sub_0811bdb0
	ldrb	r3, [r0, #27]
	movs	r1, #0
	subs	r3, #1
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L_0811f246
	add	r2, sp, #92
	mov	ip, r3
	adds	r3, r2, r5
	adds	r2, r3, #0
	subs	r2, #34
	adds	r0, #40
.L_0811f238:
	ldmia	r0!, {r3}
	adds	r1, #1
	ldrb	r3, [r3, #5]
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, ip
	bne.n	.L_0811f238
.L_0811f246:
	ldr	r3, [r6, #20]
	adds	r4, #1
	adds	r5, #4
	adds	r2, r6, #0
	cmp	r4, r3
	bne.n	.L_0811f20c
.L_0811f252:
	ldr	r3, [r7, #92]
	cmp	r3, #0
	beq.n	.L_0811f280
	cmp	r3, #1
	bne.n	.L_0811f26e
	ldrb	r1, [r7, #0]
	movs	r0, #0
	bl	sub_08120360
	ldr	r1, [pc, #188]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_0811f276
.L_0811f26e:
	ldr	r1, [pc, #184]
	movs	r0, #4
	bl	sub_08120360
.L_0811f276:
	bl	sub_081201c4
	bl	sub_0812756c
	b.n	.L_0811f306
.L_0811f280:
	ldr	r3, [r7, #100]
	cmp	r3, #0
	bne.n	.L_0811f290
	movs	r1, #144
	ldr	r0, [pc, #160]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_0811f290:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_0811f2be
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_0811f2a2
	mov	r0, sl
	bl	sub_0811e7dc
.L_0811f2a2:
	ldr	r3, [r7, #88]
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811f2b6
	adds	r0, r6, #0
	bl	sub_08138008
	b.n	.L_0811f2c2
.L_0811f2b6:
	adds	r0, r6, #0
	bl	sub_08138018
	b.n	.L_0811f2c2
.L_0811f2be:
	bl	sub_0812756c
.L_0811f2c2:
	mov	r1, sl
	cmp	r1, #0
	beq.n	.L_0811f2d4
	mov	r0, sl
	bl	sub_0811e830
	mov	r0, sl
	bl	sub_08013164
.L_0811f2d4:
	ldr	r3, [r7, #100]
	cmp	r3, #0
	bne.n	.L_0811f2de
	bl	sub_081234f0
.L_0811f2de:
	mov	r0, r9
	movs	r1, #1
	bl	sub_08020090
	ldr	r3, [r6, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_0811f306
	movs	r7, #36
.L_0811f2f0:
	ldrsh	r0, [r6, r7]
	str	r4, [sp, #0]
	bl	sub_0811be68
	adds	r5, r6, #0
	ldr	r4, [sp, #0]
	ldr	r3, [r5, #20]
	adds	r4, #1
	adds	r7, #2
	cmp	r4, r3
	bne.n	.L_0811f2f0
.L_0811f306:
	movs	r0, #0
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffffe000
	.4byte 0xffffe800
	.4byte 0xffffdc00
	.4byte 0x00000cad
	.4byte 0x00000cac
	.2byte 0x2d11
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	sub	sp, #4
	bl	sub_08016ca4
	adds	r0, r7, #0
	bl	sub_0811be3c
	movs	r1, #5
	ldr	r0, [r0, #0]
	bl	sub_08020090
	movs	r3, #1
	mov	r6, sp
	mov	r8, r3
.L_0811f352:
	movs	r3, #255
	adds	r0, r6, #0
	strh	r3, [r6, #2]
	strh	r7, [r6, #0]
	bl	sub_080382a0
	adds	r0, r7, #0
	bl	sub_0811be3c
	movs	r1, #7
	ldr	r0, [r0, #0]
	bl	sub_0811f030
	movs	r0, #2
	bl	sub_08013560
	adds	r0, r6, #0
	strh	r7, [r6, #0]
	bl	sub_080382a0
	adds	r0, r7, #0
	bl	sub_0811be3c
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_0811a484
	adds	r1, r0, #0
	ldr	r0, [r5, #0]
	bl	sub_0811f030
	movs	r0, #2
	bl	sub_08013560
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0811f352
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_08038130
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #36]
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r3, #149
	lsls	r3, r3, #1
	adds	r2, r0, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #88
	ldrsh	r3, [r2, r5]
	movs	r1, #88
	cmp	r3, r6
	beq.n	.L_0811f414
	cmp	r3, #255
	beq.n	.L_0811f3ec
.L_0811f3de:
	adds	r1, #2
	adds	r2, r1, #0
	ldrsh	r3, [r2, r5]
	cmp	r3, r6
	beq.n	.L_0811f414
	cmp	r3, #255
	bne.n	.L_0811f3de
.L_0811f3ec:
	movs	r1, #0
	adds	r0, r5, #2
.L_0811f3f0:
	lsls	r3, r1, #1
	adds	r2, r3, #0
	adds	r2, #100
	ldrsh	r3, [r0, r2]
	cmp	r3, r6
	beq.n	.L_0811f408
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L_0811f3f0
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811f440
.L_0811f408:
	ldr	r3, [pc, #4]
	strh	r3, [r0, r2]
	b.n	.L_0811f418
	movs	r0, r0
	.2byte 0x00fe
	.2byte 0x0000
.L_0811f414:
	ldr	r3, [pc, #36]
	strh	r3, [r5, r2]
.L_0811f418:
	adds	r0, r6, #0
	bl	sub_08127ba0
	movs	r2, #187
	movs	r1, #0
	movs	r0, #255
	lsls	r2, r2, #2
.L_0811f426:
	ldrsh	r3, [r2, r5]
	cmp	r3, r6
	bne.n	.L_0811f42e
	strh	r0, [r2, r5]
.L_0811f42e:
	adds	r1, #1
	adds	r2, #16
	cmp	r1, #19
	bls.n	.L_0811f426
	movs	r0, #0
	b.n	.L_0811f440
	movs	r0, r0
	.2byte 0x00fe
	.2byte 0x0000
.L_0811f440:
	pop	{r5, r6, pc}
	.align 2, 0
