.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_080200c8, 0x080200c8
	.set sub_08020278, 0x08020278
	.set sub_08020280, 0x08020280
	.set sub_080cad84, 0x080cad84
	.set sub_080d1ed8, 0x080d1ed8
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080ebe70, 0x080ebe70
	.set sub_080ebeb4, 0x080ebeb4
	.set sub_080ebec8, 0x080ebec8
	.set sub_080ebf68, 0x080ebf68
	.set sub_081c0010, 0x081c0010
	.global Func_080e0020
	.thumb_func
Func_080e0020:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	movs	r2, #64
	adds	r2, r2, r7
	sub	sp, #12
	mov	r8, r3
	mov	sl, r2
.L_080e003c:
	mov	r3, sl
	movs	r6, #0
	ldrsb	r6, [r3, r6]
	cmp	r6, #0
	bne.n	.L_080e0094
	ldr	r3, [r7, #20]
	mov	r8, sp
	str	r3, [sp, #0]
	ldr	r3, [r7, #24]
	str	r3, [sp, #8]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r5, r5, #16
	adds	r3, r0, #0
	lsls	r0, r3, #4
	asrs	r5, r5, #16
	subs	r0, r0, r3
	movs	r2, #160
	lsls	r5, r5, #16
	lsls	r2, r2, #14
	lsls	r0, r0, #1
	lsrs	r5, r5, #16
	adds	r0, r0, r2
	adds	r1, r5, #0
	mov	r2, r8
	bl	sub_0801489c
	mov	r2, r8
	ldr	r3, [r2, #0]
	str	r3, [r7, #12]
	ldr	r3, [r2, #8]
	mov	r2, sl
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #36]
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r6, [r3, #0]
	b.n	.L_080e00f6
.L_080e0094:
	cmp	r6, #1
	bne.n	.L_080e00ac
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080e0112
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080e003c
.L_080e00ac:
	cmp	r6, #2
	bne.n	.L_080e00fe
	mov	r2, r8
	ldr	r3, [r2, #4]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r2, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r2, r8
	ldr	r3, [r2, #12]
	str	r3, [r5, #8]
	bl	sub_080dc390
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #11
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	adds	r2, r7, #0
	str	r3, [r7, #12]
	adds	r2, #66
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #5
	strh	r3, [r7, #50]
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r2, sl
.L_080e00f6:
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080e0112
.L_080e00fe:
	cmp	r6, #3
	bne.n	.L_080e0112
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080e0112
	adds	r0, r7, #0
	bl	sub_080ebf68
.L_080e0112:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	adds	r3, #91
	strb	r2, [r3, #0]
	bl	.L_080e0134
	pop	{pc}
.L_080e0134:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldr	r5, [r3, #16]
	mov	r9, r3
	ldr	r3, [r5, #12]
	mov	r0, r9
	str	r3, [r0, #8]
	movs	r0, #54
	adds	r0, #255
	movs	r2, #0
	movs	r3, #0
	sub	sp, #36
	bl	sub_080dc10c
	movs	r1, #0
	adds	r6, r0, #0
	movs	r7, #0
	bl	sub_08020090
	cmp	r6, #0
	bne.n	.L_080e0170
	b.n	.L_080e02dc
.L_080e0170:
	bl	sub_080dc294
	ldr	r3, [r5, #8]
	add	r1, sp, #12
	str	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r1, #4]
	mov	r0, r9
	ldr	r3, [r5, #16]
	mov	r2, sp
	str	r3, [r1, #8]
	mov	sl, r1
	ldr	r3, [r0, #4]
	mov	r8, r2
	str	r3, [r2, #0]
	ldr	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r2, #4]
	mov	r0, r9
	ldr	r3, [r0, #12]
	str	r3, [r2, #8]
.L_080e01a4:
	mov	r2, r8
	mov	r0, sl
	ldr	r3, [r2, #0]
	ldr	r5, [r0, #0]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r6, #8]
	mov	r2, r8
	mov	r0, sl
	ldr	r3, [r2, #4]
	ldr	r5, [r0, #4]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r6, #12]
	mov	r2, r8
	mov	r0, sl
	ldr	r3, [r2, #8]
	ldr	r5, [r0, #8]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r7, #0
	muls	r0, r3
	str	r5, [r6, #16]
	bl	sub_08002054
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r0, r0, r2
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	adds	r7, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r7, #11
	blt.n	.L_080e01a4
	movs	r0, #5
	bl	sub_08013560
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_08020090
	movs	r0, #108
	bl	sub_081c0010
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #108
	bl	sub_081c0010
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #108
	bl	sub_081c0010
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #109
	bl	sub_081c0010
	add	r3, sp, #24
	mov	r5, r9
	mov	r8, r3
	movs	r0, #15
	adds	r5, #80
	mov	r7, r8
	mov	sl, r0
.L_080e0256:
	ldr	r3, [r6, #8]
	movs	r2, #128
	str	r3, [r7, #0]
	lsls	r2, r2, #12
	ldr	r3, [r6, #12]
	adds	r0, r7, #0
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	str	r3, [r7, #8]
	bl	sub_080dc390
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #11
	adds	r2, r7, #0
	bl	sub_0801489c
	movs	r1, #209
	lsls	r1, r1, #1
	ldr	r3, [r7, #8]
	ldr	r2, [r7, #0]
	adds	r0, r5, #0
	adds	r1, #255
	bl	sub_080ebec8
	adds	r0, r5, #0
	ldr	r1, [pc, #84]
	bl	sub_080ebeb4
	ldr	r0, [r5, #0]
	movs	r1, #7
	bl	sub_08020280
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r0, sl
	adds	r5, #72
	cmp	r0, #0
	bge.n	.L_080e0256
	ldr	r3, [r6, #8]
	mov	r2, r8
	str	r3, [r2, #0]
	movs	r0, #128
	ldr	r3, [r6, #12]
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r2, #4]
	movs	r0, #8
	ldr	r3, [r6, #16]
	str	r3, [r2, #8]
	bl	sub_08013560
	adds	r0, r6, #0
	bl	sub_080200c8
	movs	r0, #4
	bl	sub_08013560
	movs	r0, #30
	bl	sub_08013560
	bl	sub_080dc384
.L_080e02dc:
	add	sp, #36
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0021
	.2byte 0x080e
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r5, [pc, #16]
	bl	sub_08014878
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	ldrsb	r1, [r5, r0]
	adds	r0, r6, #0
	bl	sub_08020278
	pop	{r5, r6, pc}
	.2byte 0x0f2c
	.2byte 0x080f
	push	{lr}
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080e0352
	ldr	r1, [pc, #64]
	movs	r3, #156
	lsls	r3, r3, #2
	adds	r2, r1, r3
	ldr	r3, [r0, #108]
	str	r3, [r2, #0]
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r1, r1, r3
	movs	r3, #0
	strb	r3, [r1, #0]
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080e0340
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	cmp	r3, #0
	beq.n	.L_080e0340
	ldrb	r3, [r3, #5]
	strb	r3, [r1, #0]
.L_080e0340:
	ldr	r3, [pc, #20]
	adds	r2, r0, #0
	str	r3, [r0, #108]
	adds	r2, #91
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r1, #0
	bl	sub_08020098
.L_080e0352:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x02ed
	.2byte 0x080e
	push	{r5, lr}
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080e03a0
	ldr	r2, [r5, #108]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_080e0390
	ldr	r2, [pc, #52]
	movs	r3, #156
	lsls	r3, r3, #2
	adds	r1, r2, r3
	ldr	r3, [r1, #0]
	str	r3, [r5, #108]
	movs	r3, #0
	str	r3, [r1, #0]
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r3
	movs	r1, #0
	ldrsb	r1, [r2, r1]
	bl	sub_08020278
.L_080e0390:
	adds	r2, r5, #0
	adds	r2, #91
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #16
	bl	sub_08020098
.L_080e03a0:
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x080e02ed
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_080d1ed8
	cmp	r0, #255
	bne.n	.L_080e03be
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080e03c0
.L_080e03be:
	adds	r0, r5, #0
.L_080e03c0:
	pop	{r5, pc}
	.2byte 0x0000
