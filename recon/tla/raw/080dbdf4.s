.syntax unified
	.thumb
	.set sub_080dbb78, 0x080dbb78
	.global Func_080dbdf4
	.thumb_func
Func_080dbdf4:
	push	{r5, lr}
	adds	r5, r3, #0
	bl	sub_080dbb78
	strb	r5, [r0, #3]
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
	adds	r0, r5, #0
	pop	{r5, pc}
	.2byte 0x0000
