.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_0803df14, 0x0803df14
	.set sub_0803e8ec, 0x0803e8ec
	.set sub_0803f758, 0x0803f758
	.set sub_08042010, 0x08042010
	.global Func_0803e5a8
	.thumb_func
Func_0803e5a8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #72]
	mov	sl, r0
	adds	r0, r6, #0
	sub	sp, #4
	adds	r5, r1, #0
	bl	sub_0803e8ec
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r2, r2, r6
	mov	r9, r0
	ldr	r0, [r2, #0]
	mov	r8, r2
	cmp	r0, #0
	bne.n	.L_0803e644
	mov	r3, sl
	cmp	r3, #6
	bne.n	.L_0803e61c
	movs	r2, #238
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803e5f2
	mov	r3, sl
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r0, #17
	movs	r1, #17
	b.n	.L_0803e5fc
.L_0803e5f2:
	mov	r3, sl
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r0, #17
	movs	r1, #0
.L_0803e5fc:
	movs	r3, #3
	bl	sub_08039260
	mov	r2, r8
	str	r0, [r2, #0]
	movs	r3, #232
	lsls	r3, r3, #2
	adds	r2, r6, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #2
	adds	r2, r6, r3
	adds	r3, #47
	strh	r3, [r2, #0]
	b.n	.L_0803e636
.L_0803e61c:
	movs	r0, #9
	subs	r0, r0, r5
	movs	r3, #6
	lsrs	r0, r0, #1
	adds	r2, r5, #2
	str	r3, [sp, #0]
	adds	r0, #19
	movs	r1, #17
	movs	r3, #3
	bl	sub_08039260
	mov	r2, r8
	str	r0, [r2, #0]
.L_0803e636:
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl	sub_080393fc
	b.n	.L_0803e67c
.L_0803e644:
	cmp	r5, #0
	beq.n	.L_0803e670
	ldrh	r3, [r0, #8]
	adds	r7, r5, #2
	cmp	r3, r7
	beq.n	.L_0803e670
	movs	r1, #2
	bl	sub_0803939c
	movs	r0, #9
	subs	r0, r0, r5
	movs	r3, #6
	lsrs	r0, r0, #1
	str	r3, [sp, #0]
	adds	r0, #19
	movs	r3, #3
	movs	r1, #17
	adds	r2, r7, #0
	bl	sub_08039260
	mov	r3, r8
	str	r0, [r3, #0]
.L_0803e670:
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl	sub_080393fc
.L_0803e67c:
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803e694
	mov	r3, r9
	subs	r2, #68
	ldrh	r0, [r3, #32]
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	b.n	.L_0803e6a8
.L_0803e694:
	mov	r3, sl
	cmp	r3, #2
	beq.n	.L_0803e6b2
	cmp	r3, #4
	bne.n	.L_0803e6c4
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #40]
.L_0803e6a8:
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042010
	b.n	.L_0803e6c4
.L_0803e6b2:
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #24]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042010
.L_0803e6c4:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0000006e
	.2byte 0x006d
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #72]
	bl	sub_0803df14
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_0803e71a
	movs	r7, #0
.L_0803e706:
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	beq.n	.L_0803e714
	ldrh	r0, [r5, #12]
	bl	sub_08014274
	strh	r7, [r5, #10]
.L_0803e714:
	ldr	r5, [r5, #4]
	cmp	r5, #0
	bne.n	.L_0803e706
.L_0803e71a:
	movs	r2, #211
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_0803e73c
	movs	r7, #0
.L_0803e728:
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	beq.n	.L_0803e736
	ldrh	r0, [r5, #12]
	bl	sub_08014274
	strh	r7, [r5, #10]
.L_0803e736:
	ldr	r5, [r5, #4]
	cmp	r5, #0
	bne.n	.L_0803e728
.L_0803e73c:
	bl	sub_0803f758
	movs	r2, #18
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_0803e760
	ldrh	r0, [r6, #12]
	bl	sub_08014274
	movs	r2, #18
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_0803e760
	adds	r3, r6, #0
	adds	r3, #64
	ldrh	r0, [r3, #0]
	bl	sub_08014274
.L_0803e760:
	movs	r2, #185
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	sub_08014274
	movs	r0, #72
	bl	sub_0801314c
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #72]
	movs	r4, #192
	lsls	r4, r4, #2
	adds	r4, #150
	adds	r2, r3, r4
	adds	r4, #2
	strh	r0, [r2, #0]
	adds	r2, r3, r4
	strh	r1, [r2, #0]
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803e7a8
.L_0803e798:
	strh	r0, [r3, #16]
	strh	r0, [r3, #24]
	strh	r1, [r3, #18]
	strh	r1, [r3, #26]
	ldr	r3, [r3, #4]
	adds	r0, #16
	cmp	r3, #0
	bne.n	.L_0803e798
.L_0803e7a8:
	pop	{pc}
	.align 2, 0
