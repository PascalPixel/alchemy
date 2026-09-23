.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_0815e3ac, 0x0815e3ac
	.set sub_0815e9f4, 0x0815e9f4
	.set sub_0815f16c, 0x0815f16c
	.set sub_0818f5cc, 0x0818f5cc
	.global Func_0815e320
	.thumb_func
Func_0815e320:
	push	{r5, lr}
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r5, r0, #0
	adds	r1, #14
	movs	r0, #100
	bl	sub_08014d00
	movs	r1, #246
	lsls	r1, r1, #7
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014cc0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #96
	bl	sub_08014cc0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #240
	adds	r2, r3, r1
	str	r5, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	adds	r3, r3, r2
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r3, [r5, #0]
	adds	r1, r3, #0
	subs	r1, #100
	cmp	r1, #100
	bhi.n	.L_0815e374
	adds	r0, r5, #0
	bl	sub_0815f16c
	b.n	.L_0815e396
.L_0815e374:
	cmp	r3, #210
	beq.n	.L_0815e37c
	cmp	r3, #10
	bne.n	.L_0815e384
.L_0815e37c:
	adds	r0, r5, #0
	bl	sub_0818f5cc
	b.n	.L_0815e396
.L_0815e384:
	cmp	r3, #199
	ble.n	.L_0815e390
	adds	r0, r5, #0
	bl	sub_0815e9f4
	b.n	.L_0815e396
.L_0815e390:
	adds	r0, r5, #0
	bl	sub_0815e3ac
.L_0815e396:
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	movs	r0, #100
	bl	sub_0801314c
	pop	{r5, pc}
	.align 2, 0
