.syntax unified
	.thumb
	.set sub_0814fab8, 0x0814fab8
	.global Func_0814fa98
	.thumb_func
Func_0814fa98:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #184
	adds	r3, r3, r2
	movs	r2, #0
	str	r2, [r3, #0]
	movs	r1, #10
	str	r2, [r0, #28]
	bl	sub_0814fab8
	pop	{pc}
	.align 2, 0
