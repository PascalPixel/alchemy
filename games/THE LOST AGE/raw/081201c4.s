.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038118, 0x08038118
	.set sub_08038120, 0x08038120
	.set sub_08038130, 0x08038130
	.set sub_080381c8, 0x080381c8
	.set sub_0811b4d8, 0x0811b4d8
	.set sub_0811b6a4, 0x0811b6a4
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811f330, 0x0811f330
	.set sub_0811f3b8, 0x0811f3b8
	.set sub_0811f444, 0x0811f444
	.set sub_0811fe3c, 0x0811fe3c
	.set sub_0811ff08, 0x0811ff08
	.set sub_08120178, 0x08120178
	.set sub_081201b4, 0x081201b4
	.set sub_081234a4, 0x081234a4
	.set sub_0812824c, 0x0812824c
	.set sub_081c0010, 0x081c0010
	.global Overlay_081201c4
Overlay_081201c4:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #228
	adds	r6, r3, r2
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #40
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r7, #0
	cmp	r7, r3
	blt.n	.L_081201e6
	b.n	.L_08120356
.L_081201e6:
	ldrb	r3, [r6, r7]
	cmp	r3, #15
	bls.n	.L_081201ee
	b.n	.L_08120346
.L_081201ee:
	ldr	r2, [pc, #364]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08120252
	.4byte 0x08120260
	.4byte 0x0812026e
	.4byte 0x08120284
	.4byte 0x081202a8
	.4byte 0x081202c0
	.4byte 0x0812029a
	.4byte 0x081202ba
	.4byte 0x081202d0
	.4byte 0x081202f2
	.4byte 0x08120312
	.4byte 0x08120322
	.4byte 0x08120246
	.4byte 0x08120238
	.4byte 0x08120346
	.2byte 0x033c
	.2byte 0x0812
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r1, [r6, r3]
	adds	r0, r6, #0
	bl	sub_081201b4
	b.n	.L_08120346
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	bl	sub_08120178
	b.n	.L_08120346
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	movs	r1, #1
	bl	sub_08038120
	b.n	.L_08120346
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	movs	r1, #5
	bl	sub_08038120
	b.n	.L_08120346
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r1, #2
	bl	sub_08038120
	b.n	.L_08120346
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	ands	r0, r3
	movs	r1, #4
	bl	sub_08038120
	b.n	.L_08120346
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #148
	ldr	r2, [r3, #0]
	movs	r3, #1
	str	r3, [r2, #8]
	b.n	.L_08120346
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	cmp	r0, #0
	blt.n	.L_081202b6
	bl	sub_080381c8
.L_081202b6:
	bl	sub_0811ff08
.L_081202ba:
	bl	sub_08038118
	b.n	.L_08120346
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	cmp	r0, #0
	blt.n	.L_081202ba
	bl	sub_080381c8
	b.n	.L_081202ba
	movs	r2, #180
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	ble.n	.L_081202e2
	bl	sub_081c0010
.L_081202e2:
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	movs	r1, #0
	movs	r2, #0
	bl	sub_0811f330
	b.n	.L_08120346
	movs	r2, #182
	lsls	r2, r2, #1
	lsls	r5, r7, #2
	adds	r5, #64
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	ldr	r0, [r6, r5]
	bl	sub_0812824c
	ldr	r0, [r6, r5]
	bl	sub_0811fe3c
	ldr	r0, [r6, r5]
	bl	sub_0811f444
	b.n	.L_08120346
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_08038130
	b.n	.L_08120346
	lsls	r5, r7, #2
	adds	r5, #64
	ldr	r0, [r6, r5]
	bl	sub_0811be3c
	adds	r1, r0, #0
	ldr	r0, [r6, r5]
	bl	sub_0811b4d8
	ldr	r0, [r6, r5]
	bl	sub_0811b6a4
	b.n	.L_08120346
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	bl	sub_0811f3b8
.L_08120346:
	movs	r2, #162
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	adds	r7, #1
	cmp	r7, r3
	bge.n	.L_08120356
	b.n	.L_081201e6
.L_08120356:
	bl	sub_081234a4
	pop	{r5, r6, r7, pc}
	.2byte 0x01f8
	.2byte 0x0812
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #36]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #228
	adds	r5, r4, r3
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #40
	adds	r4, r4, r3
	ldr	r2, [r4, #0]
	lsls	r3, r2, #2
	strb	r0, [r5, r2]
	adds	r3, #64
	adds	r2, #1
	str	r1, [r5, r3]
	str	r2, [r4, #0]
	pop	{r5, pc}
	push	{lr}
	cmp	r0, #60
	beq.n	.L_081203a0
	cmp	r0, #60
	bgt.n	.L_0812039c
	cmp	r0, #32
	bgt.n	.L_081203a4
	cmp	r0, #31
	blt.n	.L_081203a4
	b.n	.L_081203a0
.L_0812039c:
	cmp	r0, #69
	bne.n	.L_081203a4
.L_081203a0:
	movs	r0, #1
	b.n	.L_081203a6
.L_081203a4:
	movs	r0, #0
.L_081203a6:
	pop	{pc}
	push	{lr}
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	cmp	r0, #110
	beq.n	.L_081203be
	cmp	r0, #113
	bne.n	.L_081203c2
.L_081203be:
	movs	r0, #1
	b.n	.L_081203c4
.L_081203c2:
	movs	r0, #0
.L_081203c4:
	pop	{pc}
