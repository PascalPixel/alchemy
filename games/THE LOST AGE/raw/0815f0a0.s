.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.global Overlay_0815f0a0
Overlay_0815f0a0:
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
	mov	r2, sp
	movs	r3, #0
	ldr	r7, [pc, #16]
	strh	r3, [r2, #0]
	mov	ip, r3
	movs	r3, #31
	mov	sl, r3
	mov	lr, r2
	movs	r6, #0
	b.n	.L_0815f0e0
	movs	r0, r0
	.2byte 0x001f
	.2byte 0x0000
.L_0815f0e0:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	mov	r5, sl
	mov	r1, lr
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
	bge.n	.L_0815f10c
	adds	r5, #1
	b.n	.L_0815f112
.L_0815f10c:
	cmp	r5, r1
	ble.n	.L_0815f112
	subs	r5, #1
.L_0815f112:
	cmp	r2, r4
	bge.n	.L_0815f11a
	adds	r2, #1
	b.n	.L_0815f120
.L_0815f11a:
	cmp	r2, r4
	ble.n	.L_0815f120
	subs	r2, #1
.L_0815f120:
	cmp	r0, r3
	bge.n	.L_0815f128
	adds	r0, #1
	b.n	.L_0815f12e
.L_0815f128:
	cmp	r0, r3
	ble.n	.L_0815f12e
	subs	r0, #1
.L_0815f12e:
	lsls	r2, r2, #5
	lsls	r3, r0, #10
	orrs	r3, r2
	orrs	r3, r5
	mov	r2, lr
	strh	r3, [r6, r2]
	movs	r3, #1
	add	ip, r3
	movs	r1, #2
	mov	r2, ip
	adds	r6, #2
	add	r8, r1
	cmp	r2, #64
	bne.n	.L_0815f0e0
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r0, sp
	lsls	r1, r1, #19
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #128
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
