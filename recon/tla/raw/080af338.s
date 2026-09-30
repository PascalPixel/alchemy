.syntax unified
	.thumb
	.global Func_080af338
	.thumb_func
Func_080af338:
	push	{lr}
	bl	Item_GetDirect
	ldrh	r0, [r0, #40]
	bl	Func_080af43c
	ldrb	r0, [r0, #0]
	pop	{pc}
	.global ItemCounter_Adjust
	.thumb_func
ItemCounter_Adjust:
.L_080af348:
	push	{lr}
	movs	r3, #150
	adds	r2, r0, #0
	lsls	r3, r3, #1
	ldr	r4, [pc, #32]
	movs	r0, #0
	cmp	r2, r3
	bge.n	.L_080af372
	ldrb	r3, [r4, r2]
	adds	r3, r3, r1
	cmp	r3, #0
	bge.n	.L_080af364
	movs	r3, #0
	b.n	.L_080af370
.L_080af364:
	cmp	r3, #99
	ble.n	.L_080af36e
	movs	r3, #99
	movs	r0, #99
	b.n	.L_080af370
.L_080af36e:
	adds	r0, r3, #0
.L_080af370:
	strb	r3, [r4, r2]
.L_080af372:
	pop	{pc}
	.2byte 0x208c
	.2byte 0x0200
	.global Item_AdjustCounter
	.thumb_func
Item_AdjustCounter:
	push	{lr}
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ldr	r2, [pc, #24]
	ands	r3, r0
	lsls	r3, r3, #1
	ldrh	r0, [r2, r3]
	movs	r4, #0
	cmp	r0, #0
	beq.n	.L_080af396
	subs	r0, #1
	bl	ItemCounter_Adjust
	adds	r4, r0, #0
.L_080af396:
	adds	r0, r4, #0
	pop	{pc}
	movs	r0, r0
	.2byte 0x1f40
	.2byte 0x080b
