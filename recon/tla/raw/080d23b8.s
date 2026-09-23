.syntax unified
	.thumb
	.set sub_080ca6e8, 0x080ca6e8
	.set sub_080cacc0, 0x080cacc0
	.global Func_080d23b8
	.thumb_func
Func_080d23b8:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_080cacc0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080ca6e8
	pop	{r5, pc}
	movs	r0, r0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	str	r0, [r3, #16]
	bx	lr
	.2byte 0x0000
