.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_0811be3c, 0x0811be3c
	.set sub_081280fc, 0x081280fc
	.global Overlay_0811b180
Overlay_0811b180:
.L_0811b180:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	mov	fp, r2
	mov	r9, r1
	str	r3, [sp, #0]
	movs	r2, #30
	cmp	r1, #4
	ble.n	.L_0811b19e
	movs	r2, #27
.L_0811b19e:
	mov	r3, r9
	subs	r3, #1
	muls	r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r1, #0
	mov	r2, r9
	asrs	r7, r3, #1
	mov	sl, r1
	cmp	r2, #0
	beq.n	.L_0811b286
	adds	r6, r0, #0
	mov	r8, r1
.L_0811b1b8:
	movs	r3, #80
	negs	r3, r3
	mov	r0, r8
	mov	r1, fp
	str	r3, [r0, r1]
	mov	r3, sl
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L_0811b23a
	ldrh	r3, [r6, #0]
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #2
	adds	r3, r3, r0
	movs	r1, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	movs	r2, #25
	cmp	r3, r1
	bls.n	.L_0811b23a
	ldrh	r0, [r6, #0]
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r5, r0, #0
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	bl	sub_081280fc
	movs	r2, #27
	cmp	r0, #0
	bne.n	.L_0811b1fc
	movs	r2, #38
.L_0811b1fc:
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r3, r5, r0
	ldrh	r0, [r3, #0]
	movs	r1, #102
	adds	r1, #255
	cmp	r0, r1
	beq.n	.L_0811b230
	movs	r3, #174
	lsls	r3, r3, #1
	cmp	r0, r3
	beq.n	.L_0811b230
	cmp	r0, #91
	beq.n	.L_0811b230
	cmp	r0, #92
	beq.n	.L_0811b230
	cmp	r0, #93
	beq.n	.L_0811b230
	cmp	r0, #94
	beq.n	.L_0811b230
	cmp	r0, #95
	beq.n	.L_0811b230
	cmp	r0, #96
	beq.n	.L_0811b230
	cmp	r0, #97
	bne.n	.L_0811b23a
.L_0811b230:
	movs	r3, #50
	negs	r3, r3
	mov	r0, r8
	mov	r1, fp
	str	r3, [r0, r1]
.L_0811b23a:
	lsrs	r3, r2, #1
	subs	r7, r7, r3
	ldr	r3, [sp, #0]
	mov	r2, r8
	str	r7, [r2, r3]
	movs	r0, #255
	ldrh	r3, [r6, #0]
	lsls	r0, r0, #8
	adds	r0, #2
	adds	r3, r3, r0
	movs	r1, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	movs	r2, #25
	cmp	r3, r1
	bls.n	.L_0811b274
	ldrh	r0, [r6, #0]
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	bl	sub_081280fc
	movs	r2, #27
	cmp	r0, #0
	bne.n	.L_0811b274
	movs	r2, #38
.L_0811b274:
	lsrs	r3, r2, #1
	movs	r0, #1
	subs	r7, r7, r3
	add	sl, r0
	movs	r3, #4
	adds	r6, #2
	add	r8, r3
	cmp	sl, r9
	bne.n	.L_0811b1b8
.L_0811b286:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r5, #0
	b.n	.L_0811b29c
.L_0811b29a:
	adds	r5, #1
.L_0811b29c:
	cmp	r5, #5
	bgt.n	.L_0811b2b6
	adds	r6, r5, #0
	adds	r6, #128
	adds	r0, r6, #0
	bl	sub_08016ca4
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811b29a
.L_0811b2b6:
	cmp	r5, #6
	bne.n	.L_0811b2c0
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811b2c2
.L_0811b2c0:
	adds	r0, r6, #0
.L_0811b2c2:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	movs	r3, #100
	adds	r2, #2
	ldrsh	r3, [r2, r3]
	movs	r1, #1
	negs	r1, r1
	sub	sp, #76
	mov	fp, r1
	movs	r0, #0
	movs	r7, #0
	mov	ip, r2
	cmp	r3, #255
	beq.n	.L_0811b322
	add	r1, sp, #48
	mov	sl, r1
	mov	r4, sl
	movs	r1, #100
	adds	r5, r2, #0
.L_0811b2fa:
	ldrsh	r3, [r2, r1]
	cmp	r3, #254
	bne.n	.L_0811b306
	cmp	r0, #0
	bne.n	.L_0811b308
	b.n	.L_0811b310
.L_0811b306:
	mov	fp, r0
.L_0811b308:
	ldrh	r3, [r5, r1]
	adds	r0, #1
	strh	r3, [r4, #0]
	adds	r4, #2
.L_0811b310:
	adds	r7, #1
	adds	r1, #2
	cmp	r7, #5
	bgt.n	.L_0811b326
	mov	r2, ip
	ldrsh	r3, [r2, r1]
	cmp	r3, #255
	bne.n	.L_0811b2fa
	b.n	.L_0811b326
.L_0811b322:
	add	r1, sp, #48
	mov	sl, r1
.L_0811b326:
	mov	r5, fp
	adds	r5, #1
	movs	r2, #24
	mov	r8, sp
	add	r2, sp
	mov	r0, sl
	adds	r1, r5, #0
	mov	r3, r8
	mov	r9, r2
	bl	.L_0811b180
	cmp	r5, #0
	ble.n	.L_0811b36c
	mov	r7, fp
	movs	r6, #0
	mov	r5, sl
	adds	r7, #1
.L_0811b348:
	ldrh	r0, [r5, #0]
	adds	r5, #2
	cmp	r0, #254
	beq.n	.L_0811b364
	bl	sub_0811be3c
	mov	r1, r9
	ldr	r3, [r6, r1]
	mov	r2, r8
	lsls	r3, r3, #16
	str	r3, [r0, #12]
	ldr	r3, [r6, r2]
	lsls	r3, r3, #16
	str	r3, [r0, #16]
.L_0811b364:
	subs	r7, #1
	adds	r6, #4
	cmp	r7, #0
	bne.n	.L_0811b348
.L_0811b36c:
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
