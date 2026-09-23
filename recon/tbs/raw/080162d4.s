.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08016230, 0x08016230
	.set sub_080163ec, 0x080163ec
	.set sub_080173ac, 0x080173ac
	.global Func_080162d4
	.thumb_func
Func_080162d4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	lr, r3
	ldr	r3, [pc, #24]
	mov	ip, r2
	ldr	r3, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r4, r3, r2
	ldrh	r2, [r4, #22]
	movs	r3, #1
	ands	r3, r2
	adds	r7, r1, #0
	ldr	r6, [sp, #20]
	movs	r5, #0
	movs	r1, #0
	b.n	.L_0801630a
	.2byte 0x1e8c
	.2byte 0x0300
.L_080162fc:
	adds	r1, #1
	adds	r4, #36
	cmp	r1, #8
	beq.n	.L_08016318
	ldrh	r2, [r4, #22]
	movs	r3, #1
	ands	r3, r2
.L_0801630a:
	cmp	r3, #0
	bne.n	.L_080162fc
	movs	r2, #26
	ldrsh	r3, [r4, r2]
	cmp	r3, #0
	bne.n	.L_080162fc
	adds	r5, r4, #0
.L_08016318:
	cmp	r5, #0
	beq.n	.L_080163e0
	movs	r3, #0
	mov	r8, r3
	mov	r2, ip
	mov	r3, lr
	strh	r7, [r5, #14]
	strh	r2, [r5, #8]
	strh	r3, [r5, #10]
	mov	r2, r8
	mov	r3, r8
	movs	r7, #1
	strh	r0, [r5, #12]
	strh	r3, [r5, #20]
	str	r2, [r5, #0]
	str	r4, [r5, #4]
	strh	r7, [r5, #16]
	strh	r7, [r5, #22]
	bl	sub_080173ac
	movs	r0, #8
	adds	r3, r6, #0
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08016352
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #36]
	orrs	r3, r2
	strh	r3, [r5, #22]
.L_08016352:
	movs	r3, #32
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08016362
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #24]
	orrs	r3, r2
	strh	r3, [r5, #22]
.L_08016362:
	movs	r3, #64
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08016380
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #12]
	orrs	r3, r2
	strh	r3, [r5, #22]
	b.n	.L_08016380
	.4byte 0x00000008
	.4byte 0x00000020
	.2byte 0x0040
	.2byte 0x0000
.L_08016380:
	movs	r3, #128
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08016390
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #48]
	orrs	r3, r2
	strh	r3, [r5, #22]
.L_08016390:
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r6, #0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080163a4
	ldrh	r2, [r5, #22]
	adds	r3, r1, #0
	orrs	r3, r2
	strh	r3, [r5, #22]
.L_080163a4:
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080163ce
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #16]
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r5, #22]
	strh	r2, [r5, #24]
	b.n	.L_080163c4
	movs	r0, r0
	.4byte 0x00000080
	.2byte 0x0002
	.2byte 0x0000
.L_080163c4:
	strh	r7, [r5, #26]
	adds	r0, r5, #0
	bl	sub_08016230
	b.n	.L_080163e0
.L_080163ce:
	movs	r3, #7
	strh	r0, [r5, #26]
	strh	r3, [r5, #24]
	adds	r0, r5, #0
	bl	sub_080163ec
	movs	r0, #1
	bl	sub_080030f8
.L_080163e0:
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
