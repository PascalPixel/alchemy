.syntax unified
	.thumb
	.set sub_0811a038, 0x0811a038
	.set sub_0811a0b0, 0x0811a0b0
	.set sub_0811a7ac, 0x0811a7ac
	.set sub_0811b180, 0x0811b180
	.set sub_0811be3c, 0x0811be3c
	.global Overlay_0811b37c
Overlay_0811b37c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	sub	sp, #80
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	mov	r2, sp
	adds	r2, #52
	movs	r1, #0
	adds	r0, r2, #0
	mov	sl, r3
	str	r2, [sp, #0]
	mov	r9, r1
	bl	sub_0811a038
	ldr	r3, [sp, #0]
	adds	r5, r0, #0
	lsls	r0, r5, #1
	adds	r0, r3, r0
	bl	sub_0811a0b0
	mov	r3, sl
	adds	r5, r5, r0
	movs	r2, #255
	movs	r7, #13
	adds	r3, #129
.L_0811b3ba:
	subs	r7, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r7, #0
	bge.n	.L_0811b3ba
	mov	r3, sl
	movs	r1, #5
	adds	r3, #129
	movs	r2, #13
.L_0811b3cc:
	subs	r1, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	subs	r2, #1
	cmp	r1, #0
	bge.n	.L_0811b3cc
	cmp	r5, #0
	ble.n	.L_0811b41e
	ldr	r1, [pc, #200]
	ldr	r2, [sp, #0]
	mov	r3, r9
	mov	fp, r1
	mov	r8, r2
	lsls	r6, r3, #1
	adds	r7, r5, #0
.L_0811b3ea:
	mov	r1, r8
	ldrh	r5, [r1, #0]
	movs	r2, #2
	adds	r3, r5, #0
	adds	r3, #116
	mov	r1, sl
	add	r8, r2
	mov	r2, r9
	strb	r2, [r1, r3]
	adds	r0, r5, #0
	bl	sub_0811be3c
	mov	r3, fp
	ldrsb	r2, [r6, r3]
	mov	r1, fp
	adds	r3, r6, #1
	ldrsb	r3, [r3, r1]
	adds	r1, r5, #0
	bl	sub_0811a7ac
	subs	r7, #1
	movs	r2, #1
	adds	r6, #2
	add	r9, r2
	cmp	r7, #0
	bne.n	.L_0811b3ea
.L_0811b41e:
	movs	r5, #2
	add	r5, sl
	movs	r3, #100
	ldrsh	r3, [r5, r3]
	movs	r7, #0
	mov	fp, r5
	cmp	r3, #255
	beq.n	.L_0811b44a
	ldr	r4, [sp, #0]
	movs	r0, #0
	movs	r2, #100
	mov	r1, fp
.L_0811b436:
	ldrh	r3, [r1, r2]
	adds	r7, #1
	strh	r3, [r0, r4]
	adds	r2, #2
	adds	r0, #2
	cmp	r7, #5
	bgt.n	.L_0811b44a
	ldrsh	r3, [r1, r2]
	cmp	r3, #255
	bne.n	.L_0811b436
.L_0811b44a:
	movs	r1, #28
	movs	r2, #4
	add	r1, sp
	add	r2, sp
	adds	r5, r7, #0
	mov	r9, r1
	mov	sl, r2
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	mov	r2, r9
	mov	r3, sl
	bl	sub_0811b180
	cmp	r5, #0
	ble.n	.L_0811b49a
	movs	r3, #0
	lsls	r6, r3, #2
	movs	r3, #100
	mov	r8, r3
.L_0811b470:
	mov	r1, fp
	mov	r3, r8
	ldrsh	r5, [r1, r3]
	cmp	r5, #254
	beq.n	.L_0811b48e
	adds	r0, r5, #0
	bl	sub_0811be3c
	mov	r1, r9
	ldr	r2, [r6, r1]
	mov	r1, sl
	ldr	r3, [r6, r1]
	adds	r1, r5, #0
	bl	sub_0811a7ac
.L_0811b48e:
	movs	r2, #2
	subs	r7, #1
	add	r8, r2
	adds	r6, #4
	cmp	r7, #0
	bne.n	.L_0811b470
.L_0811b49a:
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x8844
	.2byte 0x0812
	push	{lr}
	cmp	r1, #7
	ble.n	.L_0811b4b4
	adds	r1, #120
.L_0811b4b4:
	movs	r2, #0
.L_0811b4b6:
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	adds	r0, #2
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, #255
	beq.n	.L_0811b4d2
	cmp	r3, r1
	bne.n	.L_0811b4cc
	movs	r0, #1
	b.n	.L_0811b4d4
.L_0811b4cc:
	adds	r2, #1
	cmp	r2, #13
	bls.n	.L_0811b4b6
.L_0811b4d2:
	movs	r0, #0
.L_0811b4d4:
	pop	{pc}
