.syntax unified
	.thumb
	.set sub_0803b094, 0x0803b094
	.global Overlay_0803cf6c
Overlay_0803cf6c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #66
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r4, #255
	ldr	r1, [pc, #20]
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r3, r3, r4
	ands	r3, r1
	adds	r3, r3, r4
	ands	r3, r1
	strh	r3, [r2, #0]
	movs	r1, #1
	bl	sub_0803b094
	b.n	.L_0803cf9c
	.2byte 0x01ff
	.2byte 0x0000
.L_0803cf9c:
	pop	{pc}
