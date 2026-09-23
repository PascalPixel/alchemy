.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801401c, 0x0801401c
	.set sub_080140d8, 0x080140d8
	.set sub_0801489c, 0x0801489c
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08020090, 0x08020090
	.set sub_080201c0, 0x080201c0
	.set sub_08020218, 0x08020218
	.set sub_080202f0, 0x080202f0
	.set sub_08020310, 0x08020310
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2d84, 0x080d2d84
	.set sub_080ea8a8, 0x080ea8a8
	.global Func_080ea8d4
	.thumb_func
Func_080ea8d4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	sub	sp, #4
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	str	r0, [sp, #0]
	mov	fp, r3
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	mov	sl, r0
	cmp	r3, r2
	beq.n	.L_080ea9ce
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r9, r3
.L_080ea902:
	mov	r2, sl
	ldrh	r0, [r2, #0]
	bl	sub_080d2d84
	mov	r2, sl
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r6, [r2, r3]
	bl	.L_080eab70
	movs	r3, #34
	adds	r3, r3, r7
	ldrb	r2, [r3, #0]
	mov	r8, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	mov	r2, fp
	ldr	r5, [r2, r3]
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r7, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	lsls	r0, r0, #2
	strb	r3, [r1, #0]
	adds	r5, r5, r0
	movs	r1, #0
	adds	r0, r7, #0
	bl	sub_08020218
	ldr	r0, [r7, #8]
	movs	r3, #255
	strb	r3, [r5, #2]
	mov	r3, r8
	ldrb	r2, [r3, #0]
	add	r0, r9
	ldr	r1, [r7, #16]
	ldr	r3, [r7, #20]
	bl	sub_080ea8a8
	ldr	r0, [r7, #8]
	ldr	r2, [pc, #168]
	mov	r3, r8
	adds	r0, r0, r2
	ldr	r1, [r7, #16]
	ldrb	r2, [r3, #0]
	ldr	r3, [r7, #20]
	bl	sub_080ea8a8
	ldr	r1, [r7, #16]
	mov	r3, r8
	ldrb	r2, [r3, #0]
	ldr	r0, [r7, #8]
	add	r1, r9
	ldr	r3, [r7, #20]
	bl	sub_080ea8a8
	ldr	r1, [r7, #16]
	ldr	r2, [pc, #136]
	mov	r3, r8
	lsls	r6, r6, #16
	ldr	r0, [r7, #8]
	adds	r1, r1, r2
	lsrs	r6, r6, #16
	ldrb	r2, [r3, #0]
	ldr	r3, [r7, #20]
	bl	sub_080ea8a8
	adds	r0, r6, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ea9b4
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_08020090
	adds	r0, r6, #0
	bl	sub_08016d18
	b.n	.L_080ea9bc
.L_080ea9b4:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08020090
.L_080ea9bc:
	movs	r2, #4
	add	sl, r2
	mov	r2, sl
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_080ea902
.L_080ea9ce:
	ldr	r0, [sp, #0]
	bl	.L_080eaa14
	bl	sub_080cdf5c
	bl	sub_080d2d84
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #34
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	ldrb	r2, [r6, #0]
	ldr	r3, [r5, #20]
	bl	sub_08020310
	cmp	r0, #0
	beq.n	.L_080eaa00
	ldrb	r0, [r6, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl	sub_080201c0
	str	r0, [r5, #12]
	str	r0, [r5, #20]
.L_080eaa00:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xfff0
.L_080eaa14:
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	str	r3, [sp, #8]
	adds	r2, r3, r1
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [sp, #12]
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_080eaa4a
	b.n	.L_080eab58
.L_080eaa4a:
	ldr	r3, [sp, #12]
	movs	r7, #0
	ldrh	r0, [r3, #0]
	bl	sub_080d2d84
	adds	r6, r0, #0
	ldr	r3, [r6, #80]
	ldr	r2, [sp, #12]
	ldrb	r5, [r3, #24]
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	lsls	r3, r3, #16
	lsrs	r0, r3, #16
	mov	fp, r3
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080eaa74
	cmp	r5, #0
	beq.n	.L_080eaa7a
	b.n	.L_080eaa78
.L_080eaa74:
	cmp	r5, #1
	beq.n	.L_080eaa7a
.L_080eaa78:
	movs	r7, #1
.L_080eaa7a:
	cmp	r7, #0
	beq.n	.L_080eab44
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	movs	r3, #34
	adds	r3, r3, r6
	ldrb	r2, [r3, #0]
	mov	sl, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r1, [r1, r3]
	ldr	r3, [pc, #204]
	mov	r8, r1
	ldr	r1, [pc, #204]
	add	r3, r8
	asrs	r3, r3, #2
	adds	r1, r1, r3
	adds	r0, r6, #0
	mov	r9, r1
	bl	.L_080eab70
	str	r0, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl	sub_080202f0
	str	r0, [sp, #0]
	mov	r3, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl	sub_080201c0
	movs	r2, #192
	ldr	r1, [sp, #8]
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r1, r2
	str	r6, [r3, #0]
	mov	r3, fp
	lsrs	r5, r3, #16
	asrs	r7, r0, #19
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080eaaf0
	adds	r0, r5, #0
	subs	r7, #4
	bl	sub_08016d18
	b.n	.L_080eaaf8
.L_080eaaf0:
	adds	r0, r5, #0
	adds	r7, #4
	bl	sub_08016cfc
.L_080eaaf8:
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #16]
	orrs	r3, r2
	cmp	r3, #0
	beq.n	.L_080eab44
	ldr	r1, [sp, #4]
	mov	r2, sl
	lsls	r3, r1, #2
	add	r9, r1
	ldrb	r0, [r2, #0]
	ldr	r1, [sp, #0]
	adds	r2, r7, #0
	add	r8, r3
	bl	.L_080eab98
	mov	r3, r9
	mov	r1, fp
	strb	r0, [r3, #0]
	lsrs	r0, r1, #16
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080eab34
	movs	r3, #0
	mov	r2, r8
	strb	r3, [r2, #2]
	ldrb	r2, [r2, #3]
	movs	r3, #128
	orrs	r3, r2
	b.n	.L_080eab40
.L_080eab34:
	movs	r3, #255
	mov	r2, r8
	strb	r3, [r2, #2]
	ldrb	r2, [r2, #3]
	movs	r3, #127
	ands	r3, r2
.L_080eab40:
	mov	r1, r8
	strb	r3, [r1, #3]
.L_080eab44:
	ldr	r2, [sp, #12]
	movs	r1, #255
	adds	r2, #4
	str	r2, [sp, #12]
	lsls	r1, r1, #8
	ldrh	r3, [r2, #0]
	adds	r1, #255
	cmp	r3, r1
	beq.n	.L_080eab58
	b.n	.L_080eaa4a
.L_080eab58:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
.L_080eab70:
	push	{lr}
	adds	r3, r0, #0
	ldr	r0, [r3, #8]
	cmp	r0, #0
	bge.n	.L_080eab7e
	ldr	r2, [pc, #24]
	adds	r0, r0, r2
.L_080eab7e:
	asrs	r2, r0, #20
	ldr	r0, [r3, #16]
	cmp	r0, #0
	bge.n	.L_080eab8a
	ldr	r3, [pc, #12]
	adds	r0, r0, r3
.L_080eab8a:
	asrs	r0, r0, #20
	lsls	r0, r0, #7
	adds	r0, r2, r0
	pop	{pc}
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x000f
.L_080eab98:
	push	{r5, lr}
	ldr	r4, [pc, #48]
	lsls	r2, r2, #24
	lsrs	r5, r2, #24
	movs	r0, #0
	asrs	r2, r2, #24
.L_080eaba4:
	ldrb	r3, [r4, #0]
	cmp	r3, r1
	bne.n	.L_080eabb8
	adds	r4, #1
	movs	r3, #0
	ldrsb	r3, [r4, r3]
	subs	r4, #1
	cmp	r3, r2
	bne.n	.L_080eabc2
	b.n	.L_080eabc8
.L_080eabb8:
	cmp	r3, #255
	bne.n	.L_080eabc2
	strb	r1, [r4, #0]
	strb	r5, [r4, #1]
	b.n	.L_080eabc8
.L_080eabc2:
	adds	r4, #4
	adds	r0, #1
	b.n	.L_080eaba4
.L_080eabc8:
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0xc000
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #12
	str	r3, [sp, #8]
	movs	r1, #212
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r3, [r3, #0]
	movs	r2, #255
	str	r3, [sp, #4]
	lsls	r2, r2, #8
	ldrh	r3, [r0, #0]
	adds	r2, #255
	mov	fp, r0
	cmp	r3, r2
	beq.n	.L_080eaca4
.L_080eac00:
	mov	r2, fp
	ldrh	r0, [r2, #0]
	bl	sub_080d2d84
	adds	r6, r0, #0
	adds	r7, r6, #0
	adds	r7, #34
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	ldrb	r0, [r7, #0]
	bl	sub_080202f0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	mov	r9, r0
	ldrb	r0, [r7, #0]
	bl	sub_080201c0
	adds	r4, r0, #0
	asrs	r4, r4, #19
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl	.L_080eab70
	ldrb	r2, [r7, #0]
	movs	r1, #156
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [sp, #8]
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldr	r5, [pc, #152]
	mov	sl, r3
	ldr	r3, [pc, #152]
	add	r5, sl
	asrs	r5, r5, #2
	adds	r2, r6, #0
	adds	r5, r5, r3
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	mov	r8, r0
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_08020218
	ldr	r4, [sp, #0]
	mov	r1, r9
	adds	r4, #4
	adds	r2, r4, #0
	ldrb	r0, [r7, #0]
	bl	.L_080eab98
	mov	r1, r8
	lsls	r1, r1, #2
	add	r5, r8
	mov	r8, r1
	mov	r1, sl
	add	r1, r8
	ldrb	r2, [r1, #3]
	movs	r3, #128
	str	r2, [r6, #76]
	orrs	r2, r3
	strb	r2, [r1, #3]
	strb	r0, [r5, #0]
	ldr	r1, [sp, #4]
	movs	r2, #2
	add	r1, r8
	movs	r3, #255
	add	fp, r2
	strb	r3, [r1, #2]
	mov	r1, fp
	movs	r2, #255
	ldrh	r3, [r1, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_080eac00
.L_080eaca4:
	ldr	r3, [pc, #60]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	sub_080d2d84
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl	sub_080201c0
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_080eaccc
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_080eaccc:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	adds	r3, #224
	str	r1, [sp, #4]
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L_080ead1c
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080ead1c
	ldr	r7, [r0, #20]
	b.n	.L_080ead22
.L_080ead1c:
	bl	.L_080eaeb4
	adds	r7, r0, #0
.L_080ead22:
	ldr	r2, [sp, #4]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	adds	r3, r2, r1
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r2, [sp, #8]
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_080eade6
	adds	r3, r7, #0
	adds	r3, #34
	str	r3, [sp, #0]
.L_080ead44:
	ldr	r1, [sp, #8]
	ldrh	r0, [r1, #0]
	bl	sub_080d2d84
	cmp	r7, r0
	bne.n	.L_080eadd4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	ldr	r3, [sp, #0]
	movs	r1, #156
	ldrb	r0, [r3, #0]
	lsls	r1, r1, #1
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldr	r1, [pc, #136]
	mov	sl, r3
	movs	r3, #212
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r2, [r2, #0]
	add	r1, sl
	mov	r9, r2
	ldr	r2, [pc, #124]
	asrs	r1, r1, #2
	mov	r8, r1
	add	r8, r2
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl	sub_080202f0
	ldr	r3, [sp, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	fp, r0
	ldrb	r0, [r3, #0]
	bl	sub_080201c0
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	.L_080eab70
	ldr	r1, [sp, #0]
	asrs	r5, r5, #19
	subs	r5, #4
	adds	r6, r0, #0
	adds	r2, r5, #0
	ldrb	r0, [r1, #0]
	mov	r1, fp
	bl	.L_080eab98
	add	r8, r6
	ldr	r3, [r7, #76]
	lsls	r6, r6, #2
	add	sl, r6
	mov	r2, r8
	mov	r1, sl
	add	r9, r6
	strb	r3, [r1, #3]
	strb	r0, [r2, #0]
	mov	r3, r9
	movs	r2, #0
	strb	r2, [r3, #2]
	ldr	r1, [sp, #4]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r1, r2
	str	r7, [r3, #0]
.L_080eadd4:
	ldr	r3, [sp, #8]
	movs	r1, #255
	adds	r3, #2
	str	r3, [sp, #8]
	lsls	r1, r1, #8
	ldrh	r3, [r3, #0]
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_080ead44
.L_080eade6:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	adds	r3, r3, r1
	ldr	r7, [r3, #0]
	sub	sp, #4
	cmp	r7, #0
	beq.n	.L_080eae9e
	movs	r3, #34
	adds	r3, r3, r7
	ldrb	r0, [r3, #0]
	mov	fp, r3
	lsls	r3, r0, #3
	ldr	r2, [r2, #32]
	subs	r3, r3, r0
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldr	r6, [pc, #112]
	mov	r8, r3
	movs	r3, #212
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r2, [r2, #0]
	ldr	r1, [pc, #104]
	add	r6, r8
	asrs	r6, r6, #2
	mov	sl, r2
	adds	r6, r6, r1
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #8]
	bl	sub_080202f0
	mov	r2, fp
	ldr	r1, [r7, #8]
	mov	r9, r0
	ldrb	r0, [r2, #0]
	ldr	r2, [r7, #16]
	bl	sub_080201c0
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	.L_080eab70
	asrs	r5, r5, #19
	mov	r3, fp
	adds	r5, #4
	adds	r4, r0, #0
	mov	r1, r9
	ldrb	r0, [r3, #0]
	adds	r2, r5, #0
	adds	r6, r6, r4
	str	r4, [sp, #0]
	bl	.L_080eab98
	ldr	r4, [sp, #0]
	movs	r2, #128
	lsls	r4, r4, #2
	add	r8, r4
	mov	r1, r8
	ldrb	r3, [r1, #3]
	add	sl, r4
	str	r3, [r7, #76]
	orrs	r3, r2
	strb	r3, [r1, #3]
	mov	r2, sl
	movs	r3, #255
	strb	r0, [r6, #0]
	strb	r3, [r2, #2]
.L_080eae9e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
.L_080eaeb4:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #108]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_080d2d84
	adds	r6, r0, #0
	ldrh	r2, [r6, #6]
	movs	r3, #12
	lsrs	r2, r2, #12
	adds	r2, #2
	ands	r2, r3
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	lsls	r7, r2, #12
	ldr	r3, [r6, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	lsls	r0, r0, #13
	ldr	r3, [r6, #16]
	adds	r1, r7, #0
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r5, #0
	movs	r1, #1
	bl	.L_080eaf28
	cmp	r0, #0
	bne.n	.L_080eaf20
	ldr	r3, [r6, #8]
	movs	r0, #128
	str	r3, [r5, #0]
	lsls	r0, r0, #14
	ldr	r3, [r6, #12]
	adds	r1, r7, #0
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r5, #0
	movs	r1, #1
	bl	.L_080eaf28
	cmp	r0, #0
	bne.n	.L_080eaf20
	movs	r0, #0
.L_080eaf20:
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
.L_080eaf28:
	push	{r5, r6, r7, lr}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r4, r0, #0
	ldr	r2, [r2, #108]
	ldr	r3, [r4, #0]
	movs	r7, #255
	mov	ip, r1
	lsls	r7, r7, #8
	adds	r1, r2, #0
	movs	r6, #8
	asrs	r5, r3, #20
	adds	r7, #255
	adds	r1, #52
.L_080eaf44:
	mov	r3, ip
	ldmia	r1!, {r0}
	cmp	r3, #0
	beq.n	.L_080eaf7a
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r5, r3
	bne.n	.L_080eaf8e
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bge.n	.L_080eaf5c
	adds	r3, r3, r7
.L_080eaf5c:
	asrs	r2, r3, #16
	ldr	r3, [r0, #12]
	cmp	r3, #0
	bge.n	.L_080eaf66
	adds	r3, r3, r7
.L_080eaf66:
	asrs	r3, r3, #16
	cmp	r2, r3
	bne.n	.L_080eaf8e
	ldr	r2, [r4, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_080eaf8e
	b.n	.L_080eaf96
.L_080eaf7a:
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r5, r3
	bne.n	.L_080eaf8e
	ldr	r2, [r4, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	beq.n	.L_080eaf96
.L_080eaf8e:
	adds	r6, #1
	cmp	r6, #63
	bls.n	.L_080eaf44
	movs	r0, #0
.L_080eaf96:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	mov	r8, r1
	mov	sl, r2
	adds	r6, r3, #0
	ldr	r7, [sp, #24]
	cmp	r5, #0
	beq.n	.L_080eb002
	ldr	r3, [pc, #96]
	movs	r1, #40
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2380
	lsls	r3, r3, #9
	str	r3, [r5, #20]
	str	r3, [r5, #24]
	movs	r3, #255
	strh	r3, [r5, #30]
	mov	r3, r8
	strh	r3, [r5, #36]
	mov	r3, sl
	strh	r3, [r5, #38]
	lsrs	r2, r6, #12
	lsrs	r3, r6, #30
	orrs	r3, r2
	ldr	r1, [pc, #64]
	movs	r2, #15
	ands	r3, r2
	lsls	r3, r3, #1
	ldrb	r2, [r1, r3]
	adds	r3, #1
	ldrb	r3, [r1, r3]
	strh	r2, [r5, #32]
	strh	r3, [r5, #34]
	adds	r3, r5, #4
	stmia	r3!, {r6}
	movs	r0, #0
	str	r0, [r3, #0]
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r5, #5]
	ldr	r3, [pc, #20]
	ldrh	r2, [r5, #8]
	ands	r7, r3
	ldr	r3, [pc, #28]
	ands	r3, r2
	orrs	r3, r7
	strh	r3, [r5, #8]
.L_080eb002:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x03000260
	.4byte 0x080f3bc4
	.2byte 0xfc00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #12]
	sub	sp, #48
	str	r0, [sp, #28]
	ldr	r1, [r7, #16]
	str	r1, [sp, #24]
	ldr	r3, [r7, #24]
	ldr	r2, [r7, #20]
	str	r3, [sp, #20]
	mov	fp, r2
	ldrh	r0, [r7, #32]
	str	r0, [sp, #16]
	movs	r0, #1
	ldrh	r1, [r7, #34]
	negs	r0, r0
	str	r1, [sp, #12]
	movs	r2, #30
	ldrsh	r3, [r7, r2]
	cmp	r3, r0
	bne.n	.L_080eb054
	b.n	.L_080eb288
.L_080eb054:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_080eb05c
	b.n	.L_080eb288
.L_080eb05c:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L_080eb064
	b.n	.L_080eb288
.L_080eb064:
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	fp, r3
	bne.n	.L_080eb080
	cmp	r2, fp
	bne.n	.L_080eb080
	ldrh	r2, [r7, #28]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_080eb082
	movs	r0, #0
	str	r0, [sp, #32]
	str	r0, [sp, #36]
	b.n	.L_080eb0ba
.L_080eb080:
	ldrh	r2, [r7, #28]
.L_080eb082:
	add	r0, sp, #40
	ldr	r3, [r0, #4]
	ldr	r1, [pc, #216]
	ands	r3, r1
	orrs	r3, r2
	str	r3, [r0, #4]
	mov	r3, fp
	lsls	r2, r3, #8
	ldr	r3, [sp, #40]
	lsrs	r2, r2, #16
	ands	r3, r1
	ldr	r1, [sp, #20]
	orrs	r3, r2
	lsls	r2, r1, #8
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	lsrs	r2, r2, #16
	lsls	r2, r2, #16
	ands	r3, r1
	orrs	r3, r2
	str	r3, [sp, #40]
	bl	sub_0801401c
	movs	r2, #1
	str	r0, [sp, #32]
	str	r2, [sp, #36]
	ldrh	r2, [r7, #28]
.L_080eb0ba:
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_080eb0c2
	negs	r3, r3
.L_080eb0c2:
	movs	r1, #128
	lsls	r1, r1, #9
	cmp	r3, r1
	bgt.n	.L_080eb0d6
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L_080eb0d2
	negs	r3, r3
.L_080eb0d2:
	cmp	r3, r1
	ble.n	.L_080eb0e6
.L_080eb0d6:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	movs	r3, #3
	lsls	r0, r0, #1
	lsls	r1, r1, #1
	str	r3, [sp, #36]
	str	r0, [sp, #16]
	str	r1, [sp, #12]
.L_080eb0e6:
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_080eb164
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	fp, r2
	bne.n	.L_080eb11c
	ldr	r3, [sp, #20]
	cmp	r3, fp
	bne.n	.L_080eb11c
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #16]
	movs	r0, #36
	ldrsh	r2, [r7, r0]
	lsrs	r3, r3, #1
	subs	r2, r2, r3
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	adds	r0, r2, r3
	movs	r3, #38
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r7, #34]
	ldr	r1, [sp, #12]
	lsrs	r3, r3, #1
	subs	r2, r2, r3
	b.n	.L_080eb154
.L_080eb11c:
	movs	r3, #36
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r7, #32]
	movs	r1, #255
	lsrs	r3, r3, #1
	subs	r2, r2, r3
	mov	r0, fp
	muls	r0, r2
	adds	r2, r0, #0
	ldr	r0, [sp, #16]
	lsls	r1, r1, #8
	adds	r1, #255
	lsrs	r3, r0, #31
	adds	r2, r2, r1
	adds	r3, r0, r3
	asrs	r3, r3, #1
	asrs	r2, r2, #16
	adds	r0, r2, r3
	movs	r3, #38
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r7, #34]
	lsrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [sp, #20]
	muls	r2, r3
	adds	r2, r2, r1
	ldr	r1, [sp, #12]
	asrs	r2, r2, #16
.L_080eb154:
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	adds	r2, r2, r3
	b.n	.L_080eb218
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xffff
.L_080eb164:
	.2byte 0x8bb8
	bl	sub_08002096
	str	r0, [sp, #8]
	ldrh	r0, [r7, #28]
	bl	sub_08002090
	str	r0, [sp, #4]
	ldr	r2, [sp, #20]
	ldrh	r3, [r7, #32]
	mov	r5, fp
	muls	r5, r3
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	ldrh	r3, [r7, #34]
	asrs	r5, r5, #1
	adds	r6, r2, #0
	muls	r6, r3
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	ldr	r3, [pc, #236]
	adds	r1, r5, #0
	mov	r8, r3
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c29
	mov	r9, r0
	ldr	r0, [sp, #8]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4649
	asrs	r6, r6, #1
	subs	r1, r1, r0
	str	r1, [sp, #0]
	ldr	r0, [sp, #4]
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	mov	r9, r0
	ldr	r0, [sp, #8]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2224
	ldrsh	r3, [r7, r2]
	add	r9, r0
	mov	r0, fp
	muls	r0, r3
	ldr	r2, [sp, #20]
	movs	r1, #38
	ldrsh	r3, [r7, r1]
	mov	sl, r0
	mov	r1, sl
	ldr	r0, [sp, #4]
	adds	r5, r2, #0
	muls	r5, r3
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c29
	adds	r6, r0, #0
	ldr	r0, [sp, #8]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c29
	subs	r6, r6, r0
	ldr	r0, [sp, #4]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	ldr	r0, [sp, #8]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #0]
	ldr	r2, [sp, #12]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	subs	r6, r6, r1
	asrs	r3, r3, #1
	asrs	r6, r6, #16
	adds	r0, r3, r6
	mov	r1, r9
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	subs	r5, r5, r1
	asrs	r3, r3, #1
	asrs	r5, r5, #16
	adds	r2, r3, r5
.L_080eb218:
	ldr	r1, [sp, #28]
	asrs	r3, r1, #16
	subs	r5, r3, r0
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #16]
	asrs	r3, r0, #16
	subs	r6, r3, r2
	adds	r3, r5, r1
	cmp	r3, #0
	blt.n	.L_080eb288
	cmp	r5, #239
	bgt.n	.L_080eb288
	ldr	r2, [sp, #12]
	adds	r3, r6, r2
	cmp	r3, #0
	ble.n	.L_080eb288
	cmp	r6, #159
	bgt.n	.L_080eb288
	ldr	r3, [pc, #56]
	ldrh	r2, [r7, #6]
	ands	r5, r3
	ldr	r3, [pc, #60]
	strb	r6, [r7, #4]
	ands	r3, r2
	orrs	r3, r5
	strh	r3, [r7, #6]
	ldrb	r2, [r7, #5]
	ldr	r0, [sp, #36]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r7, #5]
	ldr	r1, [sp, #32]
	movs	r3, #31
	ands	r1, r3
	str	r1, [sp, #32]
	ldrb	r2, [r7, #7]
	movs	r3, #63
	negs	r3, r3
	lsls	r1, r1, #1
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r7, #7]
	movs	r2, #30
	ldrsh	r1, [r7, r2]
	adds	r0, r7, #0
	b.n	.L_080eb284
	.4byte 0x000001ff
	.4byte 0x0300021c
	.2byte 0xfe00
	.2byte 0xffff
.L_080eb284:
	bl	sub_080140d8
.L_080eb288:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
