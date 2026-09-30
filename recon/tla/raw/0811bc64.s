.syntax unified
	.thumb
	.balign 4
	.global Func_0811bc64
	.thumb_func
Func_0811bc64:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	0x0811a5fc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	adds	r0, r2, #0
	adds	r0, #132
	cmp	r5, #7
	ble.n	.L_0811bc7c
	subs	r5, #120
.L_0811bc7c:
	adds	r5, #116
	ldrb	r3, [r2, r5]
	movs	r1, #0
	cmp	r3, #255
	beq.n	.L_0811bc8e
	ldrb	r3, [r2, r5]
	movs	r2, #44
	muls	r3, r2
	adds	r1, r0, r3
.L_0811bc8e:
	movs	r3, #1
	strh	r3, [r1, #40]
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
