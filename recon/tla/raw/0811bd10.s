.syntax unified
	.thumb
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_080156f8, 0x080156f8
	.set sub_08015778, 0x08015778
	.set sub_08016ce4, 0x08016ce4
	.global Func_0811bd10
	.thumb_func
Func_0811bd10:
.L_0811bd10:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	bl	sub_08014de4
	movs	r0, #108
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0811bd3c
	ldr	r0, [pc, #28]
	ldr	r3, [pc, #32]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c29
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156f8
	b.n	.L_0811bd46
.L_0811bd3c:
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156e8
.L_0811bd46:
	pop	{r5, pc}
	.4byte 0x08128868
	.2byte 0x03c8
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	adds	r7, r1, #0
	adds	r1, r2, #0
	sub	sp, #12
	adds	r1, #132
	cmp	r0, #7
	ble.n	.L_0811bd66
	subs	r0, #120
.L_0811bd66:
	adds	r0, #116
	ldrb	r3, [r2, r0]
	movs	r5, #0
	cmp	r3, #255
	beq.n	.L_0811bd78
	ldrb	r3, [r2, r0]
	movs	r2, #44
	muls	r3, r2
	adds	r5, r1, r3
.L_0811bd78:
	ldr	r5, [r5, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	.L_0811bdb0
	adds	r6, r0, #0
	bl	.L_0811bd10
	ldr	r3, [r5, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r7, #0
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl	sub_08015778
	ldr	r1, [r6, #12]
	ldr	r3, [pc, #12]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2000
	add	sp, #12
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x021c
	.2byte 0x0300
.L_0811bdb0:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_0811bdd8
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r4, #15
	ands	r4, r3
	cmp	r4, #1
	bne.n	.L_0811bdcc
	cmp	r1, #0
	bne.n	.L_0811bdd8
	ldr	r0, [r0, #80]
	b.n	.L_0811bdda
.L_0811bdcc:
	cmp	r4, #2
	bne.n	.L_0811bdd8
	ldr	r3, [r0, #80]
	lsls	r2, r1, #2
	ldr	r0, [r2, r3]
	b.n	.L_0811bdda
.L_0811bdd8:
	movs	r0, #0
.L_0811bdda:
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	movs	r3, #192
	lsls	r3, r3, #6
	strh	r3, [r5, #54]
	movs	r3, #254
	lsls	r3, r3, #8
	strh	r3, [r5, #52]
	movs	r3, #255
	movs	r6, #0
	lsls	r3, r3, #17
	str	r3, [r5, #32]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #28]
	str	r6, [r5, #24]
	sub	sp, #12
	bl	sub_08014de4
	adds	r0, r5, #0
	adds	r0, #12
	bl	sub_08015128
	movs	r3, #54
	ldrsh	r0, [r5, r3]
	bl	sub_08015068
	movs	r3, #52
	ldrsh	r0, [r5, r3]
	bl	sub_08015024
	mov	r0, sp
	str	r6, [r0, #0]
	str	r6, [r0, #4]
	adds	r1, r5, #0
	ldr	r3, [r5, #32]
	str	r3, [r0, #8]
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb003
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0354
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	adds	r1, r2, #0
	adds	r1, #132
	cmp	r0, #7
	ble.n	.L_0811be4e
	subs	r0, #120
.L_0811be4e:
	adds	r0, #116
	ldrb	r3, [r2, r0]
	cmp	r3, #255
	bne.n	.L_0811be5a
	movs	r0, #0
	b.n	.L_0811be64
.L_0811be5a:
	ldrb	r2, [r2, r0]
	movs	r3, #44
	adds	r0, r2, #0
	muls	r0, r3
	adds	r0, r1, r0
.L_0811be64:
	pop	{pc}
	.align 2, 0
