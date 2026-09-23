.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080142d4, 0x080142d4
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_08014cc0, 0x08014cc0
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c0, 0x080200c0
	.set sub_080200c8, 0x080200c8
	.set sub_080201c0, 0x080201c0
	.set sub_08038248, 0x08038248
	.set sub_080caa2c, 0x080caa2c
	.set sub_080cad84, 0x080cad84
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080dc10c, 0x080dc10c
	.global Func_080ceffc
	.thumb_func
Func_080ceffc:
	push	{lr}
	bl	sub_080cad84
	pop	{pc}
	push	{lr}
	bl	sub_080caa2c
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r2, #0
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080cf04e
	movs	r2, #1
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_080cf02e
	adds	r3, r5, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r6, r3, #16
.L_080cf02e:
	cmp	r7, r2
	bne.n	.L_080cf03c
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r7, r3, #16
.L_080cf03c:
	str	r6, [r5, #8]
	str	r7, [r5, #16]
	movs	r0, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080201c0
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_080cf04e:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	bl	sub_080cdf5c
	bl	sub_080cad84
	ldr	r2, [pc, #100]
	movs	r3, #128
	lsls	r3, r3, #24
	adds	r5, r0, #0
	movs	r7, #0
	mov	r8, r2
	mov	sl, r3
.L_080cf06e:
	adds	r0, r7, #0
	adds	r0, #64
	bl	sub_080cad84
	adds	r4, r0, #0
	ldr	r3, [r5, #8]
	ldr	r1, [r4, #8]
	ldr	r2, [r5, #16]
	subs	r6, r3, r1
	ldr	r3, [r4, #16]
	subs	r0, r2, r3
	ldr	r2, [pc, #68]
	adds	r3, r6, r2
	cmp	r3, r8
	bhi.n	.L_080cf0ba
	adds	r3, r0, r2
	cmp	r3, r8
	bhi.n	.L_080cf0ba
	str	r1, [r5, #8]
	adds	r1, r6, #0
	ldr	r3, [r4, #16]
	str	r3, [r5, #16]
	bl	sub_080148e8
	adds	r1, r0, #0
	lsls	r1, r1, #16
	movs	r0, #160
	adds	r2, r5, #0
	lsrs	r1, r1, #16
	lsls	r0, r0, #13
	adds	r2, #8
	bl	sub_0801489c
	mov	r3, sl
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	str	r3, [r5, #64]
	b.n	.L_080cf0c0
.L_080cf0ba:
	adds	r7, #1
	cmp	r7, #15
	ble.n	.L_080cf06e
.L_080cf0c0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x001ffffe
	.2byte 0xffff
	.2byte 0x000f
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #40]
	movs	r2, #255
	adds	r3, #255
	lsls	r2, r2, #1
	sub	sp, #12
	cmp	r3, r2
	bhi.n	.L_080cf0ea
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_080cf0ea:
	bl	sub_08014878
	movs	r3, #100
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, #9
	bhi.n	.L_080cf150
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r5, r5, #4
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r0, #209
	lsls	r0, r0, #1
	adds	r0, #255
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080cf150
	ldr	r1, [pc, #32]
	bl	sub_080200a8
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_08020090
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_080cf150:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0194
	.2byte 0x080f
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080cf17a
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
.L_080cf17a:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r6, #192
	lsls	r6, r6, #18
	adds	r5, r0, #0
	ldr	r0, [r6, #108]
	lsls	r3, r5, #2
	adds	r3, #20
	ldr	r7, [r0, r3]
	mov	sl, r0
	sub	sp, #12
	mov	r9, r1
	movs	r0, #0
	cmp	r7, #0
	bne.n	.L_080cf1a2
	b.n	.L_080cf32e
.L_080cf1a2:
	ldr	r3, [r7, #8]
	mov	r8, sp
	str	r3, [sp, #0]
	ldr	r3, [r7, #12]
	str	r3, [sp, #4]
	ldr	r3, [r7, #16]
	str	r3, [sp, #8]
	bl	sub_080cdf5c
	cmp	r5, r0
	bne.n	.L_080cf1de
	movs	r0, #128
	ldrh	r1, [r7, #6]
	mov	r2, r8
	lsls	r0, r0, #13
	bl	sub_0801489c
	mov	r2, r8
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #280]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	mov	r4, r8
	adds	r3, r3, r2
	str	r3, [r4, #0]
	ldr	r3, [r4, #8]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r4, #8]
.L_080cf1de:
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #164
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080cf21e
	ldr	r5, [r6, #20]
	movs	r6, #63
.L_080cf1f4:
	ldr	r1, [r5, #0]
	cmp	r1, #0
	beq.n	.L_080cf216
	ldr	r2, [r5, #108]
	ldr	r3, [pc, #232]
	cmp	r2, r3
	bne.n	.L_080cf20a
	adds	r0, r5, #0
	bl	sub_080200c8
	ldr	r1, [r5, #0]
.L_080cf20a:
	ldr	r3, [pc, #224]
	cmp	r1, r3
	bne.n	.L_080cf216
	adds	r0, r5, #0
	bl	sub_080200c8
.L_080cf216:
	subs	r6, #1
	adds	r5, #128
	cmp	r6, #0
	bge.n	.L_080cf1f4
.L_080cf21e:
	movs	r0, #3
	bl	sub_08013560
	mov	r2, r8
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #4]
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r4, r8
	movs	r0, #234
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r4, #8]
	bl	sub_080200c0
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r7, #0
	beq.n	.L_080cf32e
	ldr	r1, [pc, #168]
	adds	r0, r7, #0
	bl	sub_080200a8
	ldr	r6, [r7, #80]
	movs	r3, #0
	ldrb	r2, [r6, #5]
	strb	r3, [r6, #26]
	strb	r3, [r6, #27]
	subs	r3, #33
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	adds	r2, r7, #0
	strb	r3, [r6, #9]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	mov	r0, r8
	ldr	r3, [r0, #4]
	movs	r1, #193
	str	r3, [r7, #20]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #40]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #72]
	lsls	r1, r1, #3
	movs	r0, #68
	bl	sub_08014cc0
	adds	r5, r0, #0
	mov	r0, r9
	bl	sub_08038248
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r5, r5, r2
	movs	r1, #128
	adds	r2, r5, #0
	ldrb	r0, [r6, #16]
	bl	sub_080142d4
	ldr	r3, [pc, #52]
	ldrh	r2, [r6, #8]
	ands	r0, r3
	ldr	r3, [pc, #64]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r6, #8]
	movs	r0, #68
	bl	sub_0801314c
	ldr	r3, [pc, #56]
	str	r3, [r7, #108]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #164
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080cf32c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #20]
	movs	r1, #0
	b.n	.L_080cf300
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfff00000
	.4byte 0x080cf4e1
	.4byte 0x080f0194
	.4byte 0x080effd8
	.4byte 0xfffffc00
	.2byte 0xf0d1
	.2byte 0x080c
.L_080cf2fc:
	adds	r1, #1
	adds	r0, #128
.L_080cf300:
	cmp	r1, #63
	bgt.n	.L_080cf32c
	ldr	r2, [r0, #0]
	cmp	r2, #0
	beq.n	.L_080cf2fc
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_080cf2fc
	ldr	r2, [r0, #8]
	ldr	r3, [r7, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_080cf2fc
	ldr	r2, [r0, #16]
	ldr	r3, [r7, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_080cf2fc
	bl	sub_080200c8
.L_080cf32c:
	adds	r0, r7, #0
.L_080cf32e:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x01b8
	.2byte 0x080f
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080cf34a
	bl	sub_080200c8
.L_080cf34a:
	pop	{pc}
