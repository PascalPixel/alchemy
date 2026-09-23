.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.global Func_080af4b8
	.thumb_func
Func_080af4b8:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	movs	r1, #252
	lsls	r1, r1, #6
	movs	r2, #0
	adds	r1, #255
	adds	r0, #88
.L_080af4ca:
	ldrh	r3, [r0, #0]
	adds	r0, #4
	ands	r3, r1
	cmp	r3, r5
	bne.n	.L_080af4d8
	movs	r0, #1
	b.n	.L_080af4e0
.L_080af4d8:
	adds	r2, #1
	cmp	r2, #31
	ble.n	.L_080af4ca
	movs	r0, #0
.L_080af4e0:
	pop	{r5, pc}
	.2byte 0x0000
