.syntax unified
	.thumb
	.set sub_080200c8, 0x080200c8
	.global Func_0811bc98
	.thumb_func
Func_0811bc98:
	push	{r5, r6, lr}
	movs	r6, #0
.L_0811bc9c:
	adds	r3, r6, #0
	adds	r3, #120
	cmp	r6, #7
	bgt.n	.L_0811bca6
	adds	r3, r6, #0
.L_0811bca6:
	adds	r2, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #36]
	adds	r0, r1, #0
	adds	r0, #132
	cmp	r2, #7
	ble.n	.L_0811bcb8
	subs	r2, #120
.L_0811bcb8:
	adds	r2, #116
	ldrb	r3, [r1, r2]
	movs	r5, #0
	cmp	r3, #255
	beq.n	.L_0811bcca
	ldrb	r3, [r1, r2]
	movs	r2, #44
	muls	r3, r2
	adds	r5, r0, r3
.L_0811bcca:
	cmp	r5, #0
	beq.n	.L_0811bce2
	movs	r2, #40
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_0811bce2
	ldr	r0, [r5, #0]
	bl	sub_080200c8
	movs	r3, #0
	str	r3, [r5, #0]
	strh	r3, [r5, #40]
.L_0811bce2:
	adds	r6, #1
	cmp	r6, #13
	ble.n	.L_0811bc9c
	pop	{r5, r6, pc}
	movs	r0, r0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #36]
	movs	r3, #44
	adds	r2, r0, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r3, #144
	ldr	r3, [r4, r3]
	adds	r2, #148
	str	r3, [r1, #0]
	movs	r3, #0
	str	r3, [r1, #4]
	movs	r0, #0
	ldr	r3, [r4, r2]
	str	r3, [r1, #8]
	bx	lr
	.2byte 0x0000
