.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.global Overlay_080b6148
Overlay_080b6148:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #92]
	ldr	r1, [r3, #0]
	adds	r3, r1, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	sub	sp, #20
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L_080b615e
	b.n	.L_080b636e
.L_080b615e:
	adds	r3, r1, #0
	adds	r3, #80
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #60]
	lsls	r2, r2, #3
	adds	r5, r2, r3
	adds	r3, r1, #0
	adds	r3, #82
	ldrb	r3, [r3, #0]
	ldr	r6, [pc, #52]
	cmp	r3, #0
	beq.n	.L_080b6180
	b.n	.L_080b634c
.L_080b6180:
	ldr	r3, [pc, #20]
	strh	r3, [r6, #0]
	ldr	r3, [pc, #20]
	strh	r3, [r6, #2]
	ldr	r3, [pc, #20]
	strh	r3, [r6, #8]
	ldr	r3, [pc, #20]
	movs	r0, #1
	strh	r3, [r6, #10]
	bl	sub_080030f8
	b.n	.L_080b61ba
	.4byte 0x00000065
	.4byte 0x00000078
	.4byte 0x00000054
	.4byte 0x00000055
	.4byte 0x03001e74
	.4byte 0x02002024
	.2byte 0x2224
	.2byte 0x0200
.L_080b61b4:
	movs	r0, #1
	bl	sub_080030f8
.L_080b61ba:
	ldr	r3, [pc, #116]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_080b61ce
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_080b61b4
	b.n	.L_080b634c
.L_080b61ce:
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	movs	r7, #0
	cmp	r2, r3
	beq.n	.L_080b61da
	b.n	.L_080b634c
.L_080b61da:
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	beq.n	.L_080b61e4
	b.n	.L_080b634c
.L_080b61e4:
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	cmp	r2, r3
	bne.n	.L_080b61b4
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L_080b61b4
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	cmp	r2, r3
	bne.n	.L_080b61b4
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	bne.n	.L_080b61b4
	ldr	r3, [pc, #32]
	strh	r3, [r6, #12]
	ldr	r3, [pc, #32]
	strh	r3, [r6, #14]
	b.n	.L_080b6214
.L_080b620e:
	movs	r0, #1
	bl	sub_080030f8
.L_080b6214:
	ldr	r3, [pc, #24]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_080b6234
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_080b620e
	b.n	.L_080b634c
	.4byte 0x00000072
	.4byte 0x0000006e
	.2byte 0x1f64
	.2byte 0x0300
.L_080b6234:
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	movs	r7, #0
	cmp	r2, r3
	beq.n	.L_080b6240
	b.n	.L_080b634c
.L_080b6240:
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	beq.n	.L_080b624a
	b.n	.L_080b634c
.L_080b624a:
	ldrh	r2, [r6, #12]
	ldrh	r3, [r5, #12]
	cmp	r2, r3
	bne.n	.L_080b620e
	ldrh	r2, [r6, #14]
	ldrh	r3, [r5, #14]
	cmp	r2, r3
	bne.n	.L_080b620e
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	strh	r3, [r6, #0]
	strh	r3, [r6, #4]
	ldr	r3, [pc, #40]
	strh	r2, [r6, #2]
	strh	r3, [r6, #6]
	b.n	.L_080b6270
.L_080b626a:
	movs	r0, #1
	bl	sub_080030f8
.L_080b6270:
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_080b6294
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_080b626a
	b.n	.L_080b634c
	.4byte 0x00000045
	.4byte 0x00000058
	.4byte 0x00000043
	.2byte 0x1f64
	.2byte 0x0300
.L_080b6294:
	ldrh	r2, [r6, #12]
	ldrh	r3, [r5, #12]
	movs	r7, #0
	cmp	r2, r3
	bne.n	.L_080b634c
	ldrh	r2, [r6, #14]
	ldrh	r3, [r5, #14]
	cmp	r2, r3
	bne.n	.L_080b634c
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	cmp	r2, r3
	bne.n	.L_080b626a
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L_080b626a
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	cmp	r2, r3
	bne.n	.L_080b626a
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	bne.n	.L_080b626a
	ldr	r3, [pc, #36]
	strh	r3, [r6, #8]
	ldr	r3, [pc, #36]
	strh	r3, [r6, #10]
	b.n	.L_080b62d6
.L_080b62d0:
	movs	r0, #1
	bl	sub_080030f8
.L_080b62d6:
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_080b62f8
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_080b62d0
	b.n	.L_080b634c
	movs	r0, r0
	.4byte 0x00000074
	.4byte 0x00000075
	.2byte 0x1f64
	.2byte 0x0300
.L_080b62f8:
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	movs	r7, #0
	cmp	r2, r3
	bne.n	.L_080b634c
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L_080b634c
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	cmp	r2, r3
	bne.n	.L_080b634c
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	bne.n	.L_080b634c
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	cmp	r2, r3
	bne.n	.L_080b62d0
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	bne.n	.L_080b62d0
	ldr	r3, [pc, #40]
	strh	r3, [r6, #12]
	ldr	r3, [pc, #40]
	strh	r3, [r6, #14]
	b.n	.L_080b633a
.L_080b6334:
	movs	r0, #1
	bl	sub_080030f8
.L_080b633a:
	ldr	r3, [pc, #32]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_080b6360
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L_080b6334
.L_080b634c:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080b6370
	movs	r0, r0
	.4byte 0x00000052
	.4byte 0x0000004e
	.2byte 0x1f64
	.2byte 0x0300
.L_080b6360:
	ldrh	r3, [r5, #12]
	movs	r7, #0
	cmp	r3, #114
	bne.n	.L_080b636e
	ldrh	r3, [r5, #14]
	cmp	r3, #110
	beq.n	.L_080b6334
.L_080b636e:
	movs	r0, #0
.L_080b6370:
	add	sp, #20
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
