.syntax unified
	.thumb
	.set sub_08038338, 0x08038338
	.set sub_080ad0f8, 0x080ad0f8
	.global Func_080d23f8
	.thumb_func
Func_080d23f8:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	sub_080ad0f8
	adds	r0, r6, #0
	bl	sub_080ad0f8
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_08038338
	pop	{r5, r6, pc}
	.align 2, 0
