.syntax unified
	.thumb
	.set sub_08028ef0, 0x08028ef0
	.global Overlay_08029094
Overlay_08029094:
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #100]
	adds	r7, r0, #0
	adds	r0, r3, #0
	ldr	r3, [r6, #0]
	adds	r4, r2, #0
	movs	r2, #1
	lsls	r1, r1, #16
	ands	r3, r2
	asrs	r5, r1, #16
	cmp	r3, #0
	beq.n	.L_080290b2
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080291dc
.L_080290b2:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080290c2
	movs	r0, #2
	negs	r0, r0
	b.n	.L_080291dc
.L_080290c2:
	ldr	r3, [r6, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080290d6
	ldr	r1, [r6, #0]
	movs	r3, #64
	ands	r1, r3
	cmp	r1, #0
	beq.n	.L_080290e0
.L_080290d6:
	ldrh	r3, [r0, #0]
	ldr	r2, [pc, #28]
	eors	r3, r2
	strh	r3, [r0, #0]
	b.n	.L_080291da
.L_080290e0:
	ldr	r3, [r6, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029114
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_08029100
	adds	r3, r5, #1
	b.n	.L_08029160
	movs	r0, r0
	.4byte 0x00000001
	.2byte 0x1b04
	.2byte 0x0300
.L_08029100:
	ldrh	r3, [r4, #0]
	movs	r2, #198
	adds	r3, #1
	strh	r3, [r4, #0]
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_08029184
	strh	r1, [r4, #0]
	b.n	.L_08029184
.L_08029114:
	ldr	r3, [r6, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029146
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_0802912e
	subs	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L_0802913e
.L_0802912e:
	ldrh	r3, [r4, #0]
	subs	r3, #1
	strh	r3, [r4, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_0802913e
	ldr	r3, [pc, #44]
	strh	r3, [r4, #0]
.L_0802913e:
	cmp	r5, #0
	bge.n	.L_0802918a
	movs	r5, #200
	b.n	.L_0802918a
.L_08029146:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029196
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_0802916c
	strh	r3, [r4, #0]
	adds	r3, r5, #0
	adds	r3, #10
.L_08029160:
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L_08029184
	movs	r0, r0
	.2byte 0x0063
	.2byte 0x0000
.L_0802916c:
	ldrh	r2, [r4, #0]
	adds	r3, r2, #0
	adds	r3, #10
	movs	r1, #198
	strh	r3, [r4, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	ble.n	.L_08029184
	adds	r3, r2, #0
	subs	r3, #89
	strh	r3, [r4, #0]
.L_08029184:
	cmp	r5, #200
	ble.n	.L_0802918a
	movs	r5, #0
.L_0802918a:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl	sub_08028ef0
	b.n	.L_080291da
.L_08029196:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080291da
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_080291b6
	strh	r3, [r4, #0]
	adds	r3, r5, #0
	subs	r3, #10
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L_080291ca
.L_080291b6:
	ldrh	r2, [r4, #0]
	adds	r3, r2, #0
	subs	r3, #10
	strh	r3, [r4, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_080291ca
	adds	r3, r2, #0
	adds	r3, #89
	strh	r3, [r4, #0]
.L_080291ca:
	cmp	r5, #0
	bge.n	.L_080291d0
	movs	r5, #200
.L_080291d0:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl	sub_08028ef0
.L_080291da:
	adds	r0, r5, #0
.L_080291dc:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
