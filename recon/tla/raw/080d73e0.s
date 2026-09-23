.syntax unified
	.thumb
	.set sub_08014d00, 0x08014d00
	.global Func_080d73e0
	.thumb_func
Func_080d73e0:
	push	{lr}
	movs	r1, #248
	lsls	r1, r1, #5
	adds	r1, #136
	movs	r0, #120
	bl	sub_08014d00
	movs	r3, #252
	lsls	r3, r3, #5
	adds	r2, r0, r3
	movs	r3, #12
	strh	r3, [r2, #0]
	movs	r3, #248
	lsls	r3, r3, #5
	adds	r3, #130
	movs	r1, #0
	adds	r0, r0, r3
	strh	r1, [r0, #0]
	pop	{pc}
	.2byte 0x0000
