.syntax unified
	.thumb
	.set sub_08015218, 0x08015218
	.set sub_08077008, 0x08077008
	.set sub_080770c8, 0x080770c8
	.set sub_080770d0, 0x080770d0
	.set sub_080b60a0, 0x080b60a0
	.set sub_080b7f9c, 0x080b7f9c
	.set sub_080b8fd4, 0x080b8fd4
	.set sub_080b90ac, 0x080b90ac
	.set sub_080b90f8, 0x080b90f8
	.set sub_080b920c, 0x080b920c
	.set sub_080b9324, 0x080b9324
	.set sub_080b9470, 0x080b9470
	.set sub_080b9724, 0x080b9724
	.set sub_080b98b4, 0x080b98b4
	.global Overlay_080b9934
Overlay_080b9934:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #252]
	movs	r4, #187
	ldr	r5, [r3, #0]
	movs	r1, #128
	lsls	r4, r4, #2
	adds	r7, r0, #0
	movs	r2, #0
	movs	r0, #255
	lsls	r1, r1, #8
	adds	r3, r5, r4
.L_080b994a:
	adds	r2, #1
	strh	r0, [r3, #0]
	strh	r1, [r3, #4]
	adds	r3, #16
	cmp	r2, #19
	bls.n	.L_080b994a
	bl	sub_080b90ac
	movs	r0, #8
	bl	sub_080b98b4
	ldr	r0, [pc, #212]
	bl	sub_080770c8
	adds	r5, #69
	movs	r0, #0
	bl	sub_080b8fd4
	bl	sub_08015218
	ldrb	r3, [r5, #0]
	cmp	r3, #2
	beq.n	.L_080b999e
	adds	r0, r7, #0
	bl	sub_080b920c
	adds	r6, r0, #0
	cmp	r6, #0
	blt.n	.L_080b9a16
	cmp	r6, #0
	beq.n	.L_080b99a0
	movs	r1, #6
	ldrsh	r3, [r7, r1]
	cmp	r3, #99
	bne.n	.L_080b99a0
	bl	sub_080b90f8
	cmp	r0, #0
	bne.n	.L_080b99a0
	movs	r3, #2
	strb	r3, [r5, #0]
	b.n	.L_080b99a0
.L_080b999e:
	movs	r6, #0
.L_080b99a0:
	ldr	r3, [pc, #144]
	ldr	r3, [r3, #0]
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080b99ca
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_080b9724
	adds	r5, r0, #0
	bl	sub_080b60a0
	cmp	r0, #0
	blt.n	.L_080b99c4
	adds	r6, r6, r5
	cmp	r5, #0
	bge.n	.L_080b99d4
.L_080b99c4:
	movs	r6, #1
	negs	r6, r6
	b.n	.L_080b9a16
.L_080b99ca:
	lsls	r0, r6, #4
	adds	r0, r7, r0
	bl	sub_080b9324
	adds	r6, r6, r0
.L_080b99d4:
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_080b9470
	cmp	r6, #0
	ble.n	.L_080b9a16
	adds	r5, r7, #0
	adds	r7, r6, #0
.L_080b99e4:
	movs	r2, #6
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	beq.n	.L_080b99f0
	cmp	r3, #7
	bne.n	.L_080b9a0e
.L_080b99f0:
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_08077008
	movs	r4, #6
	ldrsh	r2, [r5, r4]
	movs	r3, #3
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	ldr	r1, [pc, #52]
	lsrs	r3, r3, #31
	adds	r3, #1
	adds	r2, r0, r1
	strb	r3, [r2, #0]
.L_080b9a0e:
	subs	r7, #1
	adds	r5, #16
	cmp	r7, #0
	bne.n	.L_080b99e4
.L_080b9a16:
	ldr	r0, [pc, #32]
	bl	sub_080770d0
	bl	sub_080b7f9c
	ldr	r3, [pc, #28]
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r2, #0]
	adds	r0, r6, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x0000016b
	.4byte 0x0000012b
	.4byte 0x03001f00
