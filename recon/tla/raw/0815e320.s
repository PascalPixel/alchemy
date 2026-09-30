.syntax unified
	.thumb
	.global Func_0815e320
	.thumb_func
Func_0815e320:
	push	{r5, lr}
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r5, r0, #0
	adds	r1, #14
	movs	r0, #100
	bl	Runtime_AllocateBlock
	movs	r1, #246
	lsls	r1, r1, #7
	adds	r1, #124
	movs	r0, #92
	bl	Func_08014cc0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #96
	bl	Func_08014cc0
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
	bl	Func_0815f16c
	b.n	.L_0815e396
.L_0815e374:
	cmp	r3, #210
	beq.n	.L_0815e37c
	cmp	r3, #10
	bne.n	.L_0815e384
.L_0815e37c:
	adds	r0, r5, #0
	bl	0x0818f5cc
	b.n	.L_0815e396
.L_0815e384:
	cmp	r3, #199
	ble.n	.L_0815e390
	adds	r0, r5, #0
	bl	0x0815e9f4
	b.n	.L_0815e396
.L_0815e390:
	adds	r0, r5, #0
	bl	Func_0815e3ac
.L_0815e396:
	movs	r0, #96
	bl	Runtime_ReleaseHeapBlock
	movs	r0, #92
	bl	Runtime_ReleaseHeapBlock
	movs	r0, #100
	bl	Runtime_ReleaseHeapBlock
	pop	{r5, pc}
	.2byte 0x0000
