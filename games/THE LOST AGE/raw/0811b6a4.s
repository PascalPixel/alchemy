.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020048, 0x08020048
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_0811bdb0, 0x0811bdb0
	.set sub_0811be3c, 0x0811be3c
	.global Overlay_0811b6a4
Overlay_0811b6a4:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_08016ca4
	adds	r2, r0, #0
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	movs	r5, #1
	cmp	r3, #0
	beq.n	.L_0811b6ee
	movs	r1, #158
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811b6d8
	subs	r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811b6d8
	adds	r1, #10
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811b702
.L_0811b6d8:
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r3, r2
	negs	r2, r3
	orrs	r2, r3
	lsrs	r5, r2, #31
	lsls	r5, r5, #2
	b.n	.L_0811b702
.L_0811b6ee:
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	eors	r3, r5
	negs	r2, r3
	orrs	r2, r3
	lsrs	r5, r2, #31
	movs	r3, #5
	subs	r5, r3, r5
.L_0811b702:
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	sub_08020090
	adds	r0, r6, #0
	bl	sub_0811be3c
	movs	r1, #3
	ands	r1, r6
	ldr	r0, [r0, #0]
	adds	r1, #14
	bl	sub_08020098
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	bl	sub_0811be3c
	cmp	r0, #0
	beq.n	.L_0811b758
	ldr	r5, [r0, #0]
	cmp	r5, #0
	beq.n	.L_0811b758
	movs	r3, #0
	movs	r6, #0
	str	r3, [r0, #32]
	str	r3, [r0, #36]
	b.n	.L_0811b744
.L_0811b73e:
	bl	sub_08020048
	adds	r6, #1
.L_0811b744:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_0811bdb0
	cmp	r0, #0
	bne.n	.L_0811b73e
	adds	r3, r5, #0
	adds	r3, #84
	strb	r0, [r3, #0]
	str	r0, [r5, #80]
.L_0811b758:
	pop	{r5, r6, pc}
