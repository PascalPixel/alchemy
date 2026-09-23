.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_08015778, 0x08015778
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020090, 0x08020090
	.set sub_08038040, 0x08038040
	.set sub_08038120, 0x08038120
	.set sub_080ad078, 0x080ad078
	.set sub_080cad84, 0x080cad84
	.set sub_080dc0b8, 0x080dc0b8
	.set sub_080dc164, 0x080dc164
	.set sub_080e1420, 0x080e1420
	.set sub_080e15fc, 0x080e15fc
	.set sub_080ebc30, 0x080ebc30
	.set sub_081c0010, 0x081c0010
	.global Func_080dc390
	.thumb_func
Func_080dc390:
	push	{r5, r6, lr}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #12
	adds	r6, r0, #0
	cmp	r3, #3
	bne.n	.L_080dc3c0
	mov	r5, sp
	adds	r1, r5, #0
	bl	sub_08015778
	ldr	r3, [r5, #0]
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [r5, #4]
	lsls	r3, r3, #16
	b.n	.L_080dc3e0
.L_080dc3c0:
	ldr	r2, [r2, #32]
	adds	r3, r2, #0
	adds	r3, #228
	adds	r2, #232
	ldr	r1, [r3, #0]
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #28]
	ldr	r2, [r6, #4]
	ands	r1, r3
	ands	r0, r3
	ldr	r3, [r6, #0]
	subs	r3, r3, r1
	str	r3, [r6, #0]
	ldr	r3, [r6, #8]
	subs	r3, r3, r2
	subs	r3, r3, r0
.L_080dc3e0:
	str	r3, [r6, #8]
	movs	r3, #0
	str	r3, [r6, #4]
	add	sp, #12
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r6, #23
	adds	r5, r3, #0
	adds	r5, #80
.L_080dc400:
	adds	r0, r5, #0
	subs	r6, #1
	bl	sub_080ebc30
	adds	r5, #72
	cmp	r6, #0
	bge.n	.L_080dc400
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	mov	fp, r1
	ldr	r3, [r3, #32]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #182
	mov	r8, r3
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	sl, r0
	cmp	r3, #0
	bne.n	.L_080dc460
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r5, #244
	adds	r1, r5, #0
	movs	r0, #224
	bl	sub_08014cc0
	adds	r6, r0, #0
	ldr	r3, [pc, #12]
	movs	r2, #0
	adds	r1, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe006
	movs	r0, r0
	.2byte 0x0260
	.2byte 0x0300
.L_080dc460:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r6, [r3, #0]
	ldr	r2, [pc, #52]
	mov	r3, sl
	strh	r3, [r6, #28]
	mov	r0, sl
	mov	r9, r2
	bl	sub_080ad078
	movs	r1, #192
	ldrb	r3, [r0, #6]
	lsls	r1, r1, #4
	adds	r1, #182
	strh	r3, [r6, #30]
	adds	r3, r7, r1
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	cmp	r5, #0
	beq.n	.L_080dc48c
	b.n	.L_080dc5fc
.L_080dc48c:
	bl	sub_080dc0b8
	ldr	r3, [pc, #16]
	adds	r2, r6, #0
	subs	r3, r3, r0
	adds	r2, #66
	strh	r3, [r2, #0]
	adds	r3, r6, #0
	adds	r3, #33
	b.n	.L_080dc4a8
	.4byte 0x00000000
	.2byte 0x0200
	.2byte 0x0000
.L_080dc4a8:
	mov	r2, fp
	strb	r2, [r3, #0]
	movs	r4, #1
	adds	r3, #1
	movs	r1, #224
	strb	r4, [r3, #0]
	lsls	r1, r1, #3
	subs	r3, #2
	movs	r2, #224
	strb	r4, [r3, #0]
	adds	r1, #18
	adds	r3, #3
	lsls	r2, r2, #3
	strb	r4, [r3, #0]
	adds	r2, #19
	adds	r3, r6, r1
	strb	r4, [r3, #0]
	mov	r1, r9
	adds	r3, r6, r2
	adds	r2, #177
	strb	r1, [r3, #0]
	adds	r3, r6, r2
	strh	r5, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #198
	adds	r2, r6, r3
	movs	r1, #224
	movs	r3, #255
	lsls	r3, r3, #8
	lsls	r1, r1, #3
	adds	r3, #255
	adds	r1, #242
	strh	r3, [r2, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	mov	r2, r8
	ldr	r3, [r2, #4]
	movs	r0, #253
	str	r3, [r6, #68]
	lsls	r0, r0, #1
	ldr	r3, [r2, #8]
	str	r3, [r6, #72]
	ldr	r3, [r2, #12]
	str	r3, [r6, #76]
	ldr	r3, [pc, #260]
	mov	ip, r3
	add	r0, ip
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [pc, #252]
	ldrh	r1, [r0, #0]
	cmp	r2, r3
	bne.n	.L_080dc51a
	adds	r3, r6, #0
	adds	r3, #65
	strb	r4, [r3, #0]
.L_080dc51a:
	ldr	r2, [pc, #244]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_080dc52c
	adds	r3, r6, #0
	adds	r3, #65
	strb	r4, [r3, #0]
	ldrh	r1, [r0, #0]
.L_080dc52c:
	ldr	r2, [pc, #228]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_080dc53e
	adds	r3, r6, #0
	adds	r3, #65
	strb	r4, [r3, #0]
	ldrh	r1, [r0, #0]
.L_080dc53e:
	ldr	r2, [pc, #216]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_080dc550
	adds	r3, r6, #0
	adds	r3, #65
	strb	r4, [r3, #0]
	ldrh	r1, [r0, #0]
.L_080dc550:
	ldr	r2, [pc, #200]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_080dc562
	adds	r3, r6, #0
	adds	r3, #65
	strb	r4, [r3, #0]
	ldrh	r1, [r0, #0]
.L_080dc562:
	ldr	r2, [pc, #188]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_080dc574
	adds	r3, r6, #0
	adds	r3, #65
	strb	r4, [r3, #0]
	ldrh	r1, [r0, #0]
.L_080dc574:
	ldr	r2, [pc, #172]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_080dc584
	adds	r3, r6, #0
	adds	r3, #65
	strb	r4, [r3, #0]
.L_080dc584:
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, ip
	movs	r1, #1
	ldr	r0, [r3, #0]
	negs	r1, r1
	bl	.L_080dc62c
	movs	r1, #30
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #30]
	cmp	r3, #8
	beq.n	.L_080dc5aa
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #165
	adds	r3, r7, r1
	mov	r1, r9
	strb	r1, [r3, #0]
.L_080dc5aa:
	lsls	r3, r2, #16
	movs	r2, #224
	lsls	r2, r2, #13
	cmp	r3, r2
	beq.n	.L_080dc5c0
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #173
	adds	r3, r7, r1
	mov	r2, r9
	strb	r2, [r3, #0]
.L_080dc5c0:
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #49
	adds	r3, r7, r1
	mov	r2, r9
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #51
	mov	r1, r9
	adds	r3, r7, r2
	adds	r2, #5
	strb	r1, [r3, #0]
	adds	r3, r7, r2
	strb	r1, [r3, #0]
	movs	r3, #211
	lsls	r3, r3, #4
	adds	r2, r7, r3
	movs	r3, #255
	strb	r3, [r2, #0]
	bl	sub_080e15fc
	movs	r1, #144
	ldr	r0, [pc, #48]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_080dc5fc:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00000082
	.4byte 0x000000ce
	.4byte 0x000000cf
	.4byte 0x000000d4
	.4byte 0x00000108
	.4byte 0x00000109
	.4byte 0x0000010a
	.2byte 0xc3f1
	.2byte 0x080d
.L_080dc62c:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r6, [r3, #0]
	adds	r5, r1, #0
	strh	r0, [r6, #24]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl	sub_080cad84
	strh	r5, [r6, #26]
	lsls	r5, r5, #16
	adds	r7, r0, #0
	asrs	r5, r5, #16
	str	r7, [r6, #16]
	adds	r0, r5, #0
	bl	sub_080cad84
	ldrh	r1, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r1, r2
	ldr	r2, [pc, #52]
	str	r0, [r6, #20]
	ands	r3, r2
	strh	r3, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r1, r1, r3
	ldr	r3, [pc, #40]
	ands	r1, r3
	strh	r1, [r6, #2]
	cmp	r7, #0
	beq.n	.L_080dc698
	adds	r3, r7, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r1, #227
	lsls	r1, r1, #3
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldr	r3, [r7, #80]
	adds	r1, #1
	ldrb	r3, [r3, #9]
	adds	r2, r6, r1
	lsls	r3, r3, #28
	lsrs	r3, r3, #30
	strb	r3, [r2, #0]
	b.n	.L_080dc698
	.4byte 0xffffc000
	.2byte 0xe000
	.2byte 0xffff
.L_080dc698:
	.2byte 0x2800
	beq.n	.L_080dc6be
	ldr	r3, [r0, #108]
	str	r3, [r6, #56]
	ldr	r3, [r0, #0]
	str	r3, [r6, #60]
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	ldrb	r2, [r3, #5]
	adds	r3, r6, #0
	adds	r3, #64
	strb	r2, [r3, #0]
	ldr	r3, [r0, #8]
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	str	r3, [r6, #12]
	ldr	r3, [r0, #12]
	str	r3, [r6, #8]
	b.n	.L_080dc6d6
.L_080dc6be:
	ldr	r3, [r7, #8]
	movs	r0, #128
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #13
	str	r3, [r6, #12]
	ldr	r3, [r7, #12]
	ldrh	r1, [r6, #0]
	str	r3, [r6, #8]
	adds	r2, r6, #4
	bl	sub_0801489c
.L_080dc6d6:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r7, [r3, #0]
	ldr	r2, [r2, #108]
	ldr	r6, [r7, #16]
	mov	fp, r2
	movs	r3, #28
	ldrsh	r2, [r7, r3]
	adds	r0, r6, #0
	movs	r1, #20
	mov	r9, r2
	bl	sub_08020090
	ldr	r3, [r6, #8]
	movs	r2, #0
	str	r3, [r6, #56]
	ldr	r3, [r6, #12]
	str	r2, [r6, #36]
	str	r3, [r6, #60]
	ldr	r3, [r6, #16]
	str	r2, [r6, #40]
	str	r3, [r6, #64]
	str	r2, [r6, #44]
	movs	r3, #34
	adds	r3, r3, r7
	mov	sl, r3
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_080dc738
	movs	r0, #212
	bl	sub_081c0010
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080e1420
.L_080dc738:
	adds	r3, r7, #0
	adds	r3, #35
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080dc784
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #19
	movs	r3, #1
	adds	r5, r7, r2
	strb	r3, [r5, #0]
	movs	r2, #0
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080dc164
	mov	r0, r9
	movs	r1, #4
	bl	sub_08038120
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #18
	adds	r3, r7, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	ldr	r0, [pc, #84]
	bl	sub_08038040
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #16
	bl	sub_080dc164
	mov	r3, r8
	strb	r3, [r5, #0]
.L_080dc784:
	movs	r0, #160
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc7ac
	mov	r2, sl
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080dc7a2
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_080e1420
.L_080dc7a2:
	adds	r0, r6, #0
	movs	r1, #21
	bl	sub_08020090
	b.n	.L_080dc7b0
.L_080dc7ac:
	bl	.L_080dc7cc
.L_080dc7b0:
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #183
	add	r2, fp
	movs	r3, #1
	strb	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0dc2
	.2byte 0x0000
.L_080dc7cc:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldr	r0, [r3, #16]
	bl	sub_080e1420
	movs	r0, #1
	bl	sub_08013560
	pop	{pc}
	.2byte 0x0000
