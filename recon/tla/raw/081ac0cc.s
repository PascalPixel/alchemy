.syntax unified
	.thumb
	.global Func_081ac0cc
	.thumb_func
Func_081ac0cc:
	push	{lr}
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	asrs	r0, r0, #8
	bl	Math_Div
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	lsls	r1, r1, #16
	movs	r0, #128
	asrs	r1, r1, #16
	lsls	r0, r0, #9
	bl	Math_Div
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	pop	{pc}
	.2byte 0x0000
