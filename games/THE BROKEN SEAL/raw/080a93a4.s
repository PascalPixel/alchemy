.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_08015280, 0x08015280
	.set sub_08077018, 0x08077018
	.set sub_080a2268, 0x080a2268
	.global Overlay_080a93a4
Overlay_080a93a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #472]
	mov	sl, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #8]
	mov	r8, r3
	lsls	r3, r2, #2
	mov	r1, sl
	adds	r3, r3, r2
	mov	r9, r3
	ldr	r3, [r1, #16]
	mov	r2, r8
	add	r3, r9
	str	r3, [r1, #24]
	adds	r6, r0, #0
	ldr	r0, [r2, #44]
	sub	sp, #8
	bl	sub_08015270
	movs	r0, #1
	bl	sub_080030f8
	mov	r1, sl
	ldr	r3, [r1, #24]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	mov	r1, r8
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080a9404
	ldr	r0, [pc, #412]
	ldr	r3, [pc, #412]
	ands	r0, r2
	adds	r0, r0, r3
	ldr	r1, [r1, #44]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
.L_080a9404:
	movs	r2, #1
	mov	r1, r9
	mov	fp, r2
	lsls	r3, r1, #1
	movs	r2, #228
	add	r3, r8
	lsls	r2, r2, #1
	adds	r2, r2, r3
	movs	r7, #0
	movs	r5, #1
	mov	r9, r2
.L_080a941a:
	mov	r1, sl
	ldr	r3, [r1, #16]
	cmp	r7, r3
	bne.n	.L_080a9468
	ldr	r3, [r1, #24]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	mov	r1, r8
	ldrh	r3, [r1, r3]
	ldr	r0, [pc, #348]
	ands	r0, r3
	bl	sub_08077018
	ldr	r0, [r0, #20]
	cmp	r0, #4
	beq.n	.L_080a9454
	movs	r3, #0
	adds	r1, r0, #1
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r2, #27
	adds	r3, r5, #0
	bl	sub_08015280
	mov	r2, fp
	movs	r3, #14
	b.n	.L_080a948e
.L_080a9454:
	mov	r3, fp
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r3, #14
	movs	r1, #14
	adds	r2, r5, #0
	str	r3, [sp, #4]
	bl	sub_080a2268
	b.n	.L_080a94b4
.L_080a9468:
	mov	r1, r9
	ldrh	r3, [r1, #0]
	ldr	r0, [pc, #288]
	ands	r0, r3
	bl	sub_08077018
	ldr	r0, [r0, #20]
	cmp	r0, #4
	beq.n	.L_080a94a0
	movs	r3, #4
	adds	r1, r0, #1
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r2, #27
	adds	r3, r5, #0
	bl	sub_08015280
	mov	r2, fp
	movs	r3, #15
.L_080a948e:
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #14
	adds	r2, r5, #0
	movs	r3, #13
	bl	sub_080a2268
	b.n	.L_080a94b4
.L_080a94a0:
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #14
	adds	r2, r5, #0
	movs	r3, #14
	bl	sub_080a2268
.L_080a94b4:
	movs	r1, #2
	adds	r7, #1
	adds	r5, #2
	add	r9, r1
	cmp	r7, #4
	ble.n	.L_080a941a
	movs	r2, #1
	mov	r9, r2
	movs	r5, #1
	movs	r6, #15
	movs	r7, #3
.L_080a94ca:
	mov	r3, r8
	mov	r1, r9
	ldr	r0, [r3, #48]
	adds	r2, r5, #0
	str	r1, [sp, #0]
	movs	r3, #12
	movs	r1, #1
	subs	r7, #1
	str	r6, [sp, #4]
	adds	r5, #2
	bl	sub_080a2268
	cmp	r7, #0
	bge.n	.L_080a94ca
	mov	r2, sl
	ldr	r3, [r2, #24]
	movs	r1, #228
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	r1, r8
	ldrh	r2, [r1, r3]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a9572
	ldr	r0, [pc, #140]
	ands	r0, r2
	bl	sub_08077018
	ldrb	r1, [r0, #2]
	cmp	r1, #2
	beq.n	.L_080a955c
	cmp	r1, #2
	bgt.n	.L_080a9518
	cmp	r1, #1
	beq.n	.L_080a9522
	b.n	.L_080a9572
.L_080a9518:
	cmp	r1, #3
	beq.n	.L_080a9544
	cmp	r1, #4
	beq.n	.L_080a9532
	b.n	.L_080a9572
.L_080a9522:
	mov	r2, r8
	movs	r3, #14
	ldr	r0, [r2, #48]
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #1
	b.n	.L_080a9554
.L_080a9532:
	mov	r3, r8
	ldr	r0, [r3, #48]
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #3
	b.n	.L_080a9554
.L_080a9544:
	mov	r1, r8
	movs	r3, #1
	ldr	r0, [r1, #48]
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #5
.L_080a9554:
	movs	r3, #12
	bl	sub_080a2268
	b.n	.L_080a9572
.L_080a955c:
	mov	r2, r8
	movs	r3, #1
	ldr	r0, [r2, #48]
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #7
	movs	r3, #12
	bl	sub_080a2268
.L_080a9572:
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00000075
