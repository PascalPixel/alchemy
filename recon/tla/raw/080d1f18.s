.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014dac, 0x08014dac
	.set sub_0801591c, 0x0801591c
	.set sub_080d170c, 0x080d170c
	.set sub_080d172c, 0x080d172c
	.set sub_080d17ac, 0x080d17ac
	.set sub_080d1ee8, 0x080d1ee8
	.set sub_080d2a8c, 0x080d2a8c
	.global Func_080d1f18
	.thumb_func
Func_080d1f18:
	push	{lr}
	bl	sub_080d1ee8
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	ldr	r0, [pc, #248]
	sub	sp, #8
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #16
	mov	sl, r0
	bl	sub_080d2a8c
	movs	r0, #130
	lsls	r0, r0, #5
	bl	sub_08014dac
	lsls	r5, r5, #2
	ldr	r3, [r5, r7]
	ldr	r6, [pc, #220]
	mov	r8, r0
	cmp	r3, #0
	bge.n	.L_080d1f52
	adds	r3, #3
.L_080d1f52:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	adds	r7, r7, r3
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_0801591c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	mov	r0, r8
	ldr	r1, [pc, #192]
	ldr	r2, [pc, #196]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	bl	sub_08013164
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #254
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r4, #240
	movs	r3, #128
	add	r0, sp, #4
	lsls	r4, r4, #8
	lsls	r3, r3, #19
	str	r4, [r0, #0]
	adds	r3, #212
	adds	r1, r6, #0
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r6, [pc, #152]
	movs	r1, #0
.L_080d1fa0:
	adds	r6, #4
	movs	r5, #0
.L_080d1fa4:
	mov	r2, sl
	movs	r0, #128
	lsls	r3, r2, #16
	lsls	r0, r0, #9
	lsls	r2, r2, #16
	adds	r3, r3, r0
	lsrs	r2, r2, #16
	asrs	r3, r3, #16
	orrs	r2, r4
	adds	r5, #1
	strh	r2, [r6, #0]
	mov	sl, r3
	adds	r6, #2
	cmp	r5, #25
	bls.n	.L_080d1fa4
	adds	r1, #1
	adds	r6, #8
	cmp	r1, #4
	bls.n	.L_080d1fa0
	movs	r5, #128
	lsls	r5, r5, #19
	ldrh	r2, [r5, #0]
	movs	r3, #248
	lsls	r3, r3, #5
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r5, #0]
	ldr	r2, [pc, #52]
	ldrh	r3, [r5, #0]
	movs	r6, #128
	orrs	r3, r2
	strh	r3, [r5, #0]
	movs	r2, #128
	ldr	r3, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	lsls	r6, r6, #19
	adds	r6, #64
	strh	r3, [r6, #0]
	ldr	r3, [pc, #32]
	subs	r2, #6
	strh	r3, [r2, #0]
	movs	r1, #0
	bl	sub_080d172c
	movs	r0, #128
	lsls	r0, r0, #9
	b.n	.L_080d203c
	movs	r0, r0
	.4byte 0x00002000
	.4byte 0x0000003f
	.4byte 0x0000003e
	.4byte 0x00007878
	.4byte 0x00001848
	.4byte 0x00000026
	.4byte 0x06002000
	.4byte 0x06000200
	.4byte 0x84000410
	.4byte 0x85000200
	.2byte 0x20c0
	.2byte 0x0600
.L_080d203c:
	adds	r0, #8
	movs	r1, #0
	bl	sub_080d170c
	movs	r0, #24
	bl	sub_080d17ac
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #12]
	movs	r7, #120
	eors	r3, r2
	strh	r3, [r5, #0]
	movs	r2, #120
	movs	r5, #0
	b.n	.L_080d2060
	movs	r0, r0
	.2byte 0x0100
	.2byte 0x0000
.L_080d2060:
	subs	r2, #5
	lsls	r3, r2, #8
	adds	r7, #5
	orrs	r3, r7
	strh	r3, [r6, #0]
	movs	r0, #1
	str	r2, [sp, #0]
	bl	sub_08013560
	adds	r5, #1
	ldr	r2, [sp, #0]
	cmp	r5, #23
	bls.n	.L_080d2060
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	ldr	r6, [pc, #24]
	movs	r5, #0
	b.n	.L_080d208e
.L_080d208c:
	adds	r5, #1
.L_080d208e:
	cmp	r5, #119
	bhi.n	.L_080d209e
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r6, #4]
	cmp	r3, #0
	beq.n	.L_080d208c
.L_080d209e:
	pop	{r5, r6, pc}
	.2byte 0x1150
	.2byte 0x0300
.L_080d20a4:
	push	{lr}
	movs	r3, #26
	muls	r3, r1
	adds	r3, r3, r0
	ldr	r0, [pc, #72]
	lsls	r3, r3, #5
	adds	r1, r3, r0
	lsrs	r3, r2, #2
	lsls	r3, r3, #1
	adds	r1, r1, r3
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #1
	beq.n	.L_080d20da
	cmp	r3, #1
	bcc.n	.L_080d20ce
	cmp	r3, #2
	beq.n	.L_080d20e4
	cmp	r3, #3
	beq.n	.L_080d20ee
	b.n	.L_080d20f2
.L_080d20ce:
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #240
	ands	r3, r2
	b.n	.L_080d20f0
.L_080d20da:
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	ands	r3, r2
	b.n	.L_080d20f0
.L_080d20e4:
	ldrh	r2, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #8
	ands	r3, r2
	b.n	.L_080d20f0
.L_080d20ee:
	ldr	r3, [pc, #4]
.L_080d20f0:
	strh	r3, [r1, #0]
.L_080d20f2:
	pop	{pc}
	.4byte 0x00000000
	.2byte 0x0200
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #0
	mov	sl, r0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	sub	sp, #8
	bl	sub_080d170c
	movs	r0, #24
	bl	sub_080d17ac
	ldr	r2, [pc, #96]
	movs	r1, #9
	negs	r1, r1
	movs	r3, #0
	mov	fp, r1
	mov	r8, r3
	mov	r9, r2
.L_080d212e:
	mov	r4, r8
	ldrsh	r3, [r2, r4]
	cmp	r3, #0
	blt.n	.L_080d216a
	ldr	r4, [pc, #76]
	mov	r1, sl
	lsls	r6, r1, #1
.L_080d213c:
	movs	r7, #0
.L_080d213e:
	movs	r5, #0
.L_080d2140:
	ldrsh	r2, [r6, r4]
	adds	r1, r5, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	.L_080d20a4
	adds	r5, #1
	ldr	r4, [sp, #0]
	cmp	r5, #4
	bls.n	.L_080d2140
	adds	r7, #1
	cmp	r7, #25
	bls.n	.L_080d213e
	adds	r6, #2
	ldrsh	r3, [r6, r4]
	movs	r0, #2
	movs	r1, #1
	add	r8, r0
	add	sl, r1
	cmp	r3, #0
	bge.n	.L_080d213c
.L_080d216a:
	movs	r0, #2
	bl	sub_08013560
	mov	r2, r9
	mov	r0, r8
	ldrsh	r3, [r2, r0]
	cmp	r3, fp
	beq.n	.L_080d2188
	movs	r1, #2
	movs	r3, #1
	add	r8, r1
	add	sl, r3
	b.n	.L_080d212e
	.2byte 0x3230
	.2byte 0x080f
.L_080d2188:
	movs	r5, #128
	lsls	r5, r5, #19
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #52]
	add	r0, sp, #4
	eors	r3, r2
	strh	r3, [r5, #0]
	movs	r3, #240
	lsls	r3, r3, #8
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #36]
	ldr	r2, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #36]
	bl	sub_08013300
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	lsls	r1, r1, #19
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_080d21d8
	movs	r0, r0
	.4byte 0x00002100
	.4byte 0x06002000
	.4byte 0x85000200
	.4byte 0x00000013
	.2byte 0x0800
	.2byte 0x8400
.L_080d21d8:
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #20]
	add	sp, #8
	eors	r3, r2
	strh	r3, [r5, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0100
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	ldr	r3, [pc, #56]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080d2234
	ldr	r0, [pc, #52]
	movs	r2, #128
	ldr	r3, [r0, #4]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d221c
	movs	r3, #220
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #0
	str	r3, [r2, #0]
.L_080d221c:
	ldr	r3, [r0, #4]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d2234
	movs	r3, #220
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #1
	negs	r3, r3
	str	r3, [r2, #0]
.L_080d2234:
	pop	{pc}
	movs	r0, r0
	.4byte 0x03001238
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #220
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080d225c
	cmp	r0, #0
	beq.n	.L_080d225c
	bl	sub_08013560
.L_080d225c:
	pop	{pc}
	.align 2, 0
