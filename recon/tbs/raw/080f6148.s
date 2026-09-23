.syntax unified
	.thumb
	.global Unnamed_080f6148
	.global Func_080f6148
	.thumb_func
Unnamed_080f6148:
Func_080f6148:
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	movs	r6, #0
.L_080f6150:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r7
	ands	r4, r2
	subs	r0, #1
	subs	r1, #1
	subs	r4, #1
	cmp	r0, #0
	bge.n	.L_080f616c
	movs	r0, #0
.L_080f616c:
	cmp	r1, #0
	bge.n	.L_080f6172
	movs	r1, #0
.L_080f6172:
	cmp	r4, #0
	bge.n	.L_080f6184
	movs	r4, #0
	b.n	.L_080f6184
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x0140
	.2byte 0x0500
.L_080f6184:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #16
	bne.n	.L_080f6150
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	movs	r6, #0
.L_080f619c:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r7
	ands	r4, r2
	subs	r0, #1
	subs	r1, #1
	subs	r4, #1
	cmp	r0, #0
	bge.n	.L_080f61b8
	movs	r0, #0
.L_080f61b8:
	cmp	r1, #0
	bge.n	.L_080f61be
	movs	r1, #0
.L_080f61be:
	cmp	r4, #0
	bge.n	.L_080f61d0
	movs	r4, #0
	b.n	.L_080f61d0
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x0202
	.2byte 0x0500
.L_080f61d0:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #239
	bne.n	.L_080f619c
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
