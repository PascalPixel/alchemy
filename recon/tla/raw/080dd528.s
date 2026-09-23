.syntax unified
	.thumb
	.set sub_08014878, 0x08014878
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_08020150, 0x08020150
	.set sub_080db974, 0x080db974
	.set sub_080dc10c, 0x080dc10c
	.set sub_081c0010, 0x081c0010
	.global Func_080dd528
	.thumb_func
Func_080dd528:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldrh	r3, [r0, #6]
	mov	r8, r0
	mov	r2, r8
	movs	r0, #128
	lsls	r0, r0, #6
	ldr	r1, [r2, #8]
	ldr	r2, [r2, #12]
	adds	r5, r3, r0
	movs	r6, #128
	movs	r3, #192
	lsls	r3, r3, #8
	mov	r4, r8
	movs	r0, #139
	lsls	r6, r6, #13
	ands	r5, r3
	lsls	r0, r0, #1
	adds	r2, r2, r6
	ldr	r3, [r4, #16]
	bl	sub_080dc10c
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_080dd564
	movs	r0, #0
	b.n	.L_080dd628
.L_080dd564:
	movs	r3, #128
	mov	r0, sl
	lsls	r3, r3, #7
	str	r3, [r0, #28]
	str	r3, [r0, #24]
	ldr	r3, [pc, #196]
	mov	r2, sl
	str	r3, [r0, #108]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r0, #48]
	str	r3, [r0, #52]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #3
	bl	sub_08020090
	adds	r2, r5, #0
	mov	r0, sl
	adds	r1, r6, #0
	bl	sub_080db974
	movs	r2, #7
	mov	r9, r2
.L_080dd596:
	mov	r3, r8
	ldr	r2, [r3, #12]
	movs	r4, #128
	movs	r0, #209
	lsls	r4, r4, #13
	lsls	r0, r0, #1
	ldr	r1, [r3, #8]
	adds	r2, r2, r4
	ldr	r3, [r3, #16]
	adds	r0, #255
	bl	sub_080dc10c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_080dd614
	ldr	r1, [pc, #128]
	bl	sub_080200a8
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r2, r7, #0
	adds	r2, #85
	adds	r0, r0, r3
	str	r3, [r7, #52]
	movs	r3, #2
	str	r0, [r7, #48]
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #30
	str	r3, [r7, #72]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	subs	r5, r5, r0
	str	r5, [r7, #40]
	bl	sub_08014878
	lsls	r6, r0, #1
	adds	r6, r6, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r6, r6, #3
	adds	r6, r6, r0
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	mov	r2, r8
	ldrh	r3, [r2, #6]
	subs	r5, r5, r0
	lsrs	r5, r5, #3
	adds	r5, r5, r3
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_080db974
.L_080dd614:
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r4, r9
	cmp	r4, #0
	bge.n	.L_080dd596
	movs	r0, #138
	bl	sub_081c0010
	mov	r0, sl
.L_080dd628:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080dcf8d
	.2byte 0x0e78
	.2byte 0x080f
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080dd664
	movs	r3, #255
	ldr	r2, [r0, #24]
	lsls	r3, r3, #8
	adds	r3, #255
	cmp	r2, r3
	bgt.n	.L_080dd660
	movs	r1, #128
	lsls	r1, r1, #5
	mov	ip, r3
.L_080dd654:
	adds	r3, r2, r1
	adds	r2, r3, #0
	cmp	r3, ip
	ble.n	.L_080dd654
	str	r3, [r0, #24]
	str	r3, [r0, #28]
.L_080dd660:
	bl	sub_08020150
.L_080dd664:
	pop	{pc}
	.2byte 0x0000
