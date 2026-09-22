.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0803e5a8, 0x0803e5a8
	.set sub_0803ef48, 0x0803ef48
	.set sub_0803ef8c, 0x0803ef8c
	.set sub_0803f004, 0x0803f004
	.global Overlay_0803eb2c
Overlay_0803eb2c:
	push	{r5, r6, r7, lr}
	movs	r1, #231
	adds	r6, r0, #0
	lsls	r1, r1, #2
	adds	r7, r6, r1
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_0803ebd8
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #158
	adds	r5, r6, r2
	ldrh	r1, [r5, #0]
	bl	sub_0803ef48
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #162
	adds	r2, r6, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldrh	r5, [r5, #0]
	cmp	r5, #1
	bne.n	.L_0803eb8c
	ldrh	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_0803eb8c
	movs	r3, #8
	strh	r3, [r6, #8]
	ldrh	r3, [r7, #0]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	strh	r3, [r7, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_0803f004
	ldrh	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_0803eb88
	strh	r3, [r6, #10]
.L_0803eb88:
	strh	r5, [r6, #62]
	b.n	.L_0803eba0
.L_0803eb8c:
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #158
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	strh	r3, [r2, #0]
.L_0803eba0:
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #162
	adds	r2, r6, r3
	movs	r1, #192
	movs	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #2
	adds	r1, #158
	adds	r3, r6, r1
	ldrh	r1, [r3, #0]
	adds	r0, r6, #0
	bl	sub_0803ef8c
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	sub_0803e5a8
	movs	r0, #1
	bl	sub_08013560
.L_0803ebd8:
	pop	{r5, r6, r7, pc}
