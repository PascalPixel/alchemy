.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.global Overlay_0811f5a4
Overlay_0811f5a4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	mov	r8, r0
	mov	r9, r3
	bl	sub_08016ca4
	mov	r6, r8
	ldrh	r7, [r0, #60]
	cmp	r6, #7
	bhi.n	.L_0811f610
	movs	r3, #88
	mov	r1, r9
	ldrsh	r5, [r1, r3]
	cmp	r5, #255
	beq.n	.L_0811f65e
	mov	sl, r3
.L_0811f5d0:
	cmp	r5, #254
	beq.n	.L_0811f600
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_0811f600
	cmp	r6, r8
	beq.n	.L_0811f5f6
	cmp	r5, r8
	beq.n	.L_0811f600
	adds	r0, r5, #0
	bl	sub_08016ca4
	ldrh	r3, [r0, #60]
	cmp	r3, r7
	ble.n	.L_0811f600
.L_0811f5f6:
	adds	r6, r5, #0
	adds	r0, r6, #0
	bl	sub_08016ca4
	ldrh	r7, [r0, #60]
.L_0811f600:
	movs	r2, #2
	add	sl, r2
	mov	r3, sl
	mov	r2, r9
	ldrsh	r5, [r3, r2]
	cmp	r5, #255
	bne.n	.L_0811f5d0
	b.n	.L_0811f65e
.L_0811f610:
	movs	r1, #2
	add	r1, r9
	movs	r3, #100
	ldrsh	r5, [r1, r3]
	mov	sl, r1
	cmp	r5, #255
	beq.n	.L_0811f65e
	mov	r9, r3
.L_0811f620:
	cmp	r5, #254
	beq.n	.L_0811f650
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_0811f650
	cmp	r6, r8
	beq.n	.L_0811f646
	cmp	r5, r8
	beq.n	.L_0811f650
	adds	r0, r5, #0
	bl	sub_08016ca4
	ldrh	r3, [r0, #60]
	cmp	r3, r7
	ble.n	.L_0811f650
.L_0811f646:
	adds	r6, r5, #0
	adds	r0, r6, #0
	bl	sub_08016ca4
	ldrh	r7, [r0, #60]
.L_0811f650:
	movs	r2, #2
	add	r9, r2
	mov	r3, sl
	mov	r2, r9
	ldrsh	r5, [r3, r2]
	cmp	r5, #255
	bne.n	.L_0811f620
.L_0811f65e:
	adds	r0, r6, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
