.syntax unified
	.thumb
	.set sub_08001f7c, 0x08001f7c
	.set sub_08001f9c, 0x08001f9c
	.set sub_08013300, 0x08013300
	.global Func_081b203c
	.thumb_func
Func_081b203c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	adds	r5, r2, #0
	cmp	r3, #0
	ble.n	.L_081b2098
	movs	r1, #31
	mov	r8, r1
	movs	r2, #248
	movs	r1, #248
	lsls	r2, r2, #2
	lsls	r1, r1, #7
	mov	lr, r2
	mov	ip, r1
	adds	r0, r3, #0
.L_081b205e:
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
	bne.n	.L_081b205e
.L_081b2098:
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r5, [r0, #92]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_081b2132
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	ldr	r4, [r0, #96]
	cmp	r3, #1
	beq.n	.L_081b20ca
	cmp	r3, #2
	beq.n	.L_081b20f2
	b.n	.L_081b211a
.L_081b20ca:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r1, [pc, #112]
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r5, r2
	movs	r1, #128
	ldr	r2, [r3, #0]
	adds	r0, r4, #0
	ldr	r3, [pc, #96]
	lsls	r1, r1, #8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe013
.L_081b20f2:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #50
	bne.n	.L_081b210e
	movs	r2, #128
	ldr	r1, [pc, #64]
	lsls	r2, r2, #8
	adds	r0, r4, #0
	bl	sub_08001f9c
	b.n	.L_081b211a
.L_081b210e:
	movs	r2, #128
	ldr	r1, [pc, #48]
	lsls	r2, r2, #8
	adds	r0, r4, #0
	bl	sub_08001f7c
.L_081b211a:
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r5, r3
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #228
	adds	r2, r5, r3
	movs	r3, #1
	b.n	.L_081b213e
.L_081b2132:
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #228
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
.L_081b213e:
	str	r3, [r2, #0]
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x06003500
	.4byte 0x84002000
	.2byte 0x0260
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	movs	r6, #0
.L_081b2158:
	ldrh	r2, [r5, #0]
	movs	r4, #31
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	ands	r1, r7
	ands	r4, r2
	subs	r0, #1
	subs	r1, #1
	subs	r4, #1
	cmp	r0, #0
	bge.n	.L_081b2174
	movs	r0, #0
.L_081b2174:
	cmp	r1, #0
	bge.n	.L_081b217a
	movs	r1, #0
.L_081b217a:
	cmp	r4, #0
	bge.n	.L_081b218c
	movs	r4, #0
	b.n	.L_081b218c
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x0140
	.2byte 0x0500
.L_081b218c:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #16
	bne.n	.L_081b2158
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	movs	r6, #0
.L_081b21a4:
	ldrh	r2, [r5, #0]
	movs	r4, #31
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	ands	r1, r7
	ands	r4, r2
	subs	r0, #1
	subs	r1, #1
	subs	r4, #1
	cmp	r0, #0
	bge.n	.L_081b21c0
	movs	r0, #0
.L_081b21c0:
	cmp	r1, #0
	bge.n	.L_081b21c6
	movs	r1, #0
.L_081b21c6:
	cmp	r4, #0
	bge.n	.L_081b21d8
	movs	r4, #0
	b.n	.L_081b21d8
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x0202
	.2byte 0x0500
.L_081b21d8:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #239
	bne.n	.L_081b21a4
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #160
	lsls	r1, r1, #19
	sub	sp, #128
	mov	r8, r1
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r1, sp
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r7, [pc, #16]
	movs	r2, #0
	movs	r3, #31
	mov	lr, r2
	mov	sl, r3
	mov	ip, sp
	movs	r6, #0
	b.n	.L_081b2228
	movs	r0, r0
	.2byte 0x001f
	.2byte 0x0000
.L_081b2228:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	mov	r5, sl
	mov	r1, ip
	ands	r5, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r0, r3, #26
	ldrh	r3, [r6, r1]
	mov	r1, sl
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r4, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r7
	ands	r0, r7
	ands	r4, r7
	ands	r3, r7
	cmp	r5, r1
	bge.n	.L_081b2254
	adds	r5, #1
	b.n	.L_081b225a
.L_081b2254:
	cmp	r5, r1
	ble.n	.L_081b225a
	subs	r5, #1
.L_081b225a:
	cmp	r2, r4
	bge.n	.L_081b2262
	adds	r2, #1
	b.n	.L_081b2268
.L_081b2262:
	cmp	r2, r4
	ble.n	.L_081b2268
	subs	r2, #1
.L_081b2268:
	cmp	r0, r3
	bge.n	.L_081b2270
	adds	r0, #1
	b.n	.L_081b2276
.L_081b2270:
	cmp	r0, r3
	ble.n	.L_081b2276
	subs	r0, #1
.L_081b2276:
	lsls	r2, r2, #5
	lsls	r3, r0, #10
	orrs	r3, r2
	orrs	r3, r5
	mov	r2, ip
	strh	r3, [r6, r2]
	movs	r3, #1
	add	lr, r3
	movs	r1, #2
	mov	r2, lr
	adds	r6, #2
	add	r8, r1
	cmp	r2, #64
	bne.n	.L_081b2228
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	mov	r0, sp
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, #2
	adds	r1, #2
	adds	r2, #63
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #128
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
