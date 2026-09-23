.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_08014878, 0x08014878
	.set sub_080200a8, 0x080200a8
	.set sub_080db974, 0x080db974
	.set sub_080dc10c, 0x080dc10c
	.global Func_080dbf94
	.thumb_func
Func_080dbf94:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	mov	sl, r1
	ldr	r1, [r3, #16]
	sub	sp, #12
	ldr	r3, [r1, #8]
	mov	r8, r2
	mov	r2, sp
	str	r3, [r2, #0]
	adds	r5, r0, #0
	ldr	r3, [r1, #12]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r2, #4]
	movs	r6, #0
	ldr	r3, [r1, #16]
	mov	r9, r2
	str	r3, [r2, #8]
	mov	fp, r6
.L_080dbfd0:
	mov	r2, r9
	ldr	r3, [r2, #0]
	mov	r0, sl
	subs	r3, r3, r0
	adds	r0, r6, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_08002054
	add	r0, sl
	str	r0, [r5, #8]
	mov	r2, r9
	ldr	r3, [r2, #4]
	mov	r0, r8
	subs	r3, r3, r0
	adds	r0, r6, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_08002054
	add	r0, r8
	str	r0, [r5, #12]
	mov	r2, r9
	ldr	r3, [r2, #8]
	movs	r1, #10
	subs	r3, r3, r7
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r0, r7, r0
	str	r0, [r5, #16]
	movs	r1, #10
	mov	r0, fp
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r0, r3
	str	r0, [r5, #24]
	str	r0, [r5, #28]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [pc, #20]
	adds	r6, #1
	add	fp, r0
	cmp	r6, #10
	ble.n	.L_080dbfd0
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x4000
	.2byte 0xffff
	.2byte 0xb560
	adds	r4, r0, #0
	movs	r0, #209
	adds	r5, r1, #0
	lsls	r0, r0, #1
	adds	r3, r2, #0
	adds	r0, #255
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl	sub_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080dc0b0
	ldr	r1, [pc, #80]
	bl	sub_080200a8
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r2, r6, #0
	adds	r2, #85
	adds	r0, r0, r3
	str	r3, [r6, #52]
	movs	r3, #2
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #61
	str	r3, [r6, #72]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	subs	r5, r5, r0
	str	r5, [r6, #40]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #3
	adds	r5, r5, r3
	bl	sub_08014878
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	sub_080db974
.L_080dc0b0:
	adds	r0, r6, #0
	pop	{r5, r6, pc}
	.2byte 0x0e78
	.2byte 0x080f
	push	{lr}
	ldr	r1, [pc, #24]
	movs	r2, #128
	movs	r0, #0
	lsls	r2, r2, #2
.L_080dc0c2:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L_080dc0cc
	adds	r0, #1
.L_080dc0cc:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_080dc0c2
	pop	{pc}
	.2byte 0x3410
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_080dc108
	cmp	r6, #0
	bne.n	.L_080dc0f2
	ldrb	r3, [r5, #17]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r5, #17]
	b.n	.L_080dc106
.L_080dc0f2:
	ldrb	r0, [r5, #16]
	bl	sub_08014274
	ldrb	r3, [r6, #16]
	movs	r2, #1
	strb	r3, [r5, #16]
	ldrb	r3, [r5, #17]
	orrs	r3, r2
	strb	r3, [r5, #17]
	adds	r5, r6, #0
.L_080dc106:
	adds	r0, r5, #0
.L_080dc108:
	pop	{r5, r6, pc}
	.align 2, 0
