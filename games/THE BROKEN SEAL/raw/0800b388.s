.syntax unified
	.thumb
	.set sub_08003d28, 0x08003d28
	.set sub_08003dec, 0x08003dec
	.set sub_08003f78, 0x08003f78
	.set sub_08005268, 0x08005268
	.set sub_0800aa0c, 0x0800aa0c
	.global Overlay_0800b388
Overlay_0800b388:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #20]
	movs	r1, #1
	str	r2, [sp, #16]
	str	r1, [sp, #0]
	adds	r6, r3, #0
	ldr	r3, [pc, #396]
	ldr	r3, [r3, #0]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #0
	beq.n	.L_0800b3b2
	b.n	.L_0800b658
.L_0800b3b2:
	add	r3, sp, #32
	mov	sl, r3
	mov	r1, sl
	ldr	r0, [sp, #20]
	bl	sub_08005268
	mov	r1, sl
	ldr	r3, [r1, #8]
	adds	r5, r0, #0
	cmp	r3, #0
	bne.n	.L_0800b3ca
	b.n	.L_0800b658
.L_0800b3ca:
	movs	r2, #32
	ldr	r3, [r1, #0]
	negs	r2, r2
	cmp	r3, r2
	bge.n	.L_0800b3d6
	b.n	.L_0800b658
.L_0800b3d6:
	movs	r1, #136
	lsls	r1, r1, #1
	cmp	r3, r1
	ble.n	.L_0800b3e0
	b.n	.L_0800b658
.L_0800b3e0:
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, r2
	bge.n	.L_0800b3ea
	b.n	.L_0800b658
.L_0800b3ea:
	cmp	r3, #208
	ble.n	.L_0800b3f0
	b.n	.L_0800b658
.L_0800b3f0:
	ldrb	r2, [r7, #29]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800b3fe
	ldr	r5, [r7, #24]
	b.n	.L_0800b40a
.L_0800b3fe:
	ldr	r3, [pc, #308]
	adds	r0, r5, #0
	ldr	r1, [r7, #24]
	mov	ip, pc
	bx	r3
	adds	r5, r0, #0
.L_0800b40a:
	adds	r3, r7, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	movs	r2, #33
	adds	r2, r2, r7
	lsrs	r3, r3, #1
	mov	fp, r3
	ldrb	r3, [r2, #0]
	lsls	r1, r6, #16
	lsrs	r3, r3, #1
	str	r3, [sp, #8]
	lsrs	r1, r1, #16
	movs	r3, #8
	adds	r0, r7, #0
	mov	r9, r2
	str	r3, [sp, #4]
	bl	sub_0800aa0c
	movs	r1, #128
	lsls	r1, r1, #3
	ldr	r3, [pc, #260]
	adds	r4, r5, r1
	str	r0, [sp, #12]
	ands	r4, r3
	ldr	r3, [sp, #16]
	ldmia	r3!, {r1}
	adds	r2, r3, #0
	ldr	r6, [pc, #240]
	adds	r0, r4, #0
	str	r2, [sp, #16]
	movs	r0, r0
	mov	ip, pc
	bx	r6
	adds	r5, r0, #0
	ldr	r1, [r3, #0]
	adds	r0, r4, #0
	movs	r0, r0
	mov	ip, pc
	bx	r6
	ldr	r3, [pc, #224]
	mov	lr, r0
	cmp	r5, r3
	ble.n	.L_0800b464
	movs	r5, #252
	lsls	r5, r5, #9
.L_0800b464:
	cmp	lr, r3
	ble.n	.L_0800b46e
	movs	r1, #252
	lsls	r1, r1, #9
	mov	lr, r1
.L_0800b46e:
	adds	r3, r7, #0
	adds	r3, #34
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	adds	r1, r5, #0
	mov	ip, pc
	bx	r6
	mov	r2, r9
	mov	r8, r0
	adds	r3, #1
	ldrb	r0, [r2, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	lsrs	r0, r0, #1
	subs	r0, r0, r3
	mov	r1, lr
	mov	ip, pc
	bx	r6
	movs	r3, #128
	lsls	r3, r3, #9
	negs	r6, r0
	cmp	r5, r3
	bgt.n	.L_0800b4a2
	cmp	lr, r3
	ble.n	.L_0800b4b8
.L_0800b4a2:
	ldr	r2, [sp, #8]
	movs	r3, #3
	mov	r1, fp
	str	r3, [sp, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r3, #16
	mov	fp, r1
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	b.n	.L_0800b4ca
.L_0800b4b8:
	cmp	r5, r3
	bne.n	.L_0800b4ca
	ldrh	r3, [r7, #30]
	cmp	r3, #0
	bne.n	.L_0800b4ca
	cmp	lr, r5
	bne.n	.L_0800b4ca
	movs	r1, #0
	str	r1, [sp, #0]
.L_0800b4ca:
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_0800b520
	add	r0, sp, #24
	ldr	r2, [r0, #4]
	ldr	r1, [pc, #104]
	ldrh	r3, [r7, #30]
	ands	r2, r1
	orrs	r2, r3
	str	r2, [r0, #4]
	ldr	r4, [sp, #24]
	lsls	r3, r5, #8
	lsrs	r3, r3, #16
	ands	r4, r1
	orrs	r4, r3
	ldr	r3, [sp, #12]
	str	r4, [sp, #24]
	cmp	r3, #0
	beq.n	.L_0800b506
	ldrh	r3, [r0, #0]
	negs	r3, r3
	adds	r2, r1, #0
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	ands	r2, r4
	mov	r1, r8
	orrs	r2, r3
	negs	r1, r1
	str	r2, [sp, #24]
	mov	r8, r1
.L_0800b506:
	mov	r2, lr
	lsls	r3, r2, #8
	ldr	r1, [pc, #56]
	ldr	r2, [sp, #24]
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	ands	r2, r1
	orrs	r2, r3
	str	r2, [sp, #24]
	bl	sub_08003d28
	adds	r5, r0, #0
	b.n	.L_0800b54a
.L_0800b520:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L_0800b548
	mov	r1, r8
	negs	r1, r1
	movs	r5, #8
	mov	r8, r1
	b.n	.L_0800b54a
	.4byte 0x03001e68
	.4byte 0x03000118
	.4byte 0xfffff800
	.4byte 0x0001f7ff
	.4byte 0xffff0000
	.2byte 0xffff
	.2byte 0x0000
.L_0800b548:
	movs	r5, #0
.L_0800b54a:
	mov	r3, sl
	ldr	r2, [r3, #0]
	mov	r1, fp
	subs	r2, r2, r1
	ldr	r3, [pc, #60]
	add	r2, r8
	ldrh	r1, [r7, #6]
	ands	r2, r3
	ldr	r3, [pc, #56]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	mov	r2, sl
	ldr	r1, [sp, #8]
	ldr	r3, [r2, #4]
	subs	r3, r3, r1
	adds	r3, r3, r6
	strb	r3, [r7, #4]
	ldrb	r2, [r7, #5]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	ldr	r2, [sp, #0]
	orrs	r3, r2
	strb	r3, [r7, #5]
	adds	r2, r5, #0
	movs	r3, #31
	ands	r2, r3
	ldrb	r1, [r7, #7]
	movs	r3, #63
	negs	r3, r3
	lsls	r2, r2, #1
	ands	r3, r1
	b.n	.L_0800b598
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_0800b598:
	orrs	r3, r2
	strb	r3, [r7, #7]
	ldr	r3, [sp, #88]
	cmp	r3, #0
	bne.n	.L_0800b5c4
	mov	r1, sl
	ldr	r3, [r1, #8]
	movs	r2, #128
	lsls	r2, r2, #2
	subs	r2, r2, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r1, r2, #0
	adds	r1, #128
	cmp	r1, #0
	bgt.n	.L_0800b5bc
	movs	r1, #1
.L_0800b5bc:
	adds	r0, r7, #0
	bl	sub_08003dec
	b.n	.L_0800b5cc
.L_0800b5c4:
	adds	r0, r7, #0
	ldr	r1, [sp, #88]
	bl	sub_08003dec
.L_0800b5cc:
	adds	r3, r7, #0
	adds	r3, #38
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800b670
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	add	r0, sp, #44
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	ldr	r3, [r2, #8]
	mov	r1, sl
	str	r3, [r0, #8]
	bl	sub_08005268
	mov	r3, sl
	ldr	r1, [sp, #4]
	ldr	r2, [r3, #0]
	adds	r0, r7, #0
	ldr	r3, [pc, #56]
	adds	r0, #12
	subs	r2, r2, r1
	ands	r2, r3
	ldrh	r1, [r0, #6]
	ldr	r3, [pc, #52]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r1, [sp, #4]
	mov	r2, sl
	ldr	r3, [r2, #4]
	lsrs	r2, r1, #1
	subs	r3, r3, r2
	adds	r3, #2
	strb	r3, [r0, #4]
	ldrb	r2, [r0, #5]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	ldr	r2, [sp, #0]
	orrs	r3, r2
	strb	r3, [r0, #5]
	movs	r3, #31
	ands	r5, r3
	ldrb	r2, [r0, #7]
	movs	r3, #63
	negs	r3, r3
	lsls	r1, r5, #1
	b.n	.L_0800b63c
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_0800b63c:
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #7]
	ldr	r3, [sp, #88]
	cmp	r3, #0
	bne.n	.L_0800b650
	movs	r1, #0
	bl	sub_08003dec
	b.n	.L_0800b670
.L_0800b650:
	ldr	r1, [sp, #88]
	bl	sub_08003dec
	b.n	.L_0800b670
.L_0800b658:
	ldrb	r2, [r7, #29]
	movs	r5, #1
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0800b670
	ldrb	r0, [r7, #28]
	bl	sub_08003f78
	adds	r3, r7, #0
	adds	r3, #37
	strb	r5, [r3, #0]
.L_0800b670:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
