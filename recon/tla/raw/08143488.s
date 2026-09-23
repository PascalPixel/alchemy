.syntax unified
	.thumb
	.set sub_081180f0, 0x081180f0
	.global Func_08143488
	.thumb_func
Func_08143488:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #92]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #180
	adds	r5, r2, r1
	ldr	r0, [r3, #36]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	ble.n	.L_081434d4
	adds	r1, #4
	adds	r3, r2, r1
	ldr	r2, [r3, #0]
	movs	r1, #160
	adds	r2, #1
	str	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #108
	adds	r0, r0, r3
	lsls	r3, r2, #4
	adds	r3, r3, r2
	lsls	r3, r3, #4
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r3, r3, #2
	lsls	r1, r1, #19
	lsls	r2, r2, #9
	subs	r2, r2, r3
	adds	r1, #192
	movs	r3, #128
	bl	sub_081180f0
	ldr	r3, [r5, #0]
	subs	r3, #1
	str	r3, [r5, #0]
.L_081434d4:
	pop	{r5, pc}
	.align 2, 0
