.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015128, 0x08015128
	.set sub_08015768, 0x08015768
	.set sub_081b83c4, 0x081b83c4
	.global Func_081b8000
	.thumb_func
Func_081b8000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x8009
	.2byte 0x081b
	push	{lr}
	ldr	r3, [pc, #16]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	bl	sub_081b83c4
	movs	r0, #0
	pop	{pc}
	movs	r0, r0
	.2byte 0x0040
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	movs	r3, #147
	lsls	r3, r3, #8
	strh	r3, [r5, #52]
	movs	r3, #255
	lsls	r3, r3, #17
	str	r3, [r5, #32]
	ldr	r3, [pc, #96]
	movs	r6, #0
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	strh	r6, [r5, #54]
	str	r6, [r5, #28]
	str	r6, [r3, #12]
	str	r6, [r3, #16]
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
	ldr	r3, [pc, #40]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20fa
	movs	r1, #192
	ldr	r3, [pc, #32]
	lsls	r1, r1, #8
	lsls	r0, r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c01
	movs	r0, #250
	lsls	r0, r0, #16
	ldr	r2, [pc, #20]
	bl	sub_08015768
	add	sp, #12
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x030011e0
	.4byte 0x03000354
	.4byte 0x03000230
	.2byte 0x0000
	.2byte 0x7fff
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	adds	r5, r2, #0
	cmp	r3, #0
	ble.n	.L_081b8104
	movs	r1, #31
	mov	r8, r1
	movs	r2, #248
	movs	r1, #248
	lsls	r2, r2, #2
	lsls	r1, r1, #7
	mov	lr, r2
	mov	ip, r1
	adds	r0, r3, #0
.L_081b80ca:
	ldrh	r4, [r7, #0]
	mov	r2, r8
	adds	r3, r4, #0
	ands	r3, r2
	mov	r1, lr
	adds	r2, r4, #0
	ands	r2, r1
	muls	r3, r5
	mov	r1, ip
	muls	r2, r5
	ands	r1, r4
	muls	r1, r5
	lsrs	r4, r3, #16
	mov	r3, r8
	ands	r4, r3
	lsrs	r2, r2, #16
	mov	r3, lr
	ands	r2, r3
	orrs	r4, r2
	lsrs	r1, r1, #16
	mov	r2, ip
	ands	r1, r2
	orrs	r4, r1
	subs	r0, #1
	strh	r4, [r6, #0]
	adds	r7, #2
	adds	r6, #2
	cmp	r0, #0
	bne.n	.L_081b80ca
.L_081b8104:
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r0, r3, #0
	mov	r8, r2
	adds	r5, r1, #0
	bl	sub_08002090
	negs	r0, r0
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	mov	r3, r8
	adds	r0, #16
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_081b8178
	ldr	r7, [pc, #48]
	mov	ip, r7
.L_081b8132:
	ldrh	r3, [r6, #0]
	movs	r2, #31
	ands	r2, r3
	lsls	r3, r3, #16
	mov	r7, ip
	adds	r1, r2, r0
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r7
	ands	r3, r7
	adds	r2, r2, r0
	adds	r3, r3, r0
	cmp	r1, #31
	ble.n	.L_081b8150
	movs	r1, #31
.L_081b8150:
	cmp	r2, #31
	ble.n	.L_081b8156
	movs	r2, #31
.L_081b8156:
	cmp	r3, #31
	ble.n	.L_081b8164
	movs	r3, #31
	b.n	.L_081b8164
	movs	r0, r0
	.2byte 0x001f
	.2byte 0x0000
.L_081b8164:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	adds	r4, #1
	strh	r3, [r5, #0]
	adds	r6, #2
	adds	r5, #2
	cmp	r4, r8
	bne.n	.L_081b8132
.L_081b8178:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
