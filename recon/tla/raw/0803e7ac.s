.syntax unified
	.thumb
	.set sub_0803e7c8, 0x0803e7c8
	.global Func_0803e7ac
	.thumb_func
Func_0803e7ac:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #72]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_0803e7c8
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_0803e7c8
	pop	{r5, pc}
	.2byte 0x0000
