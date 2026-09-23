.syntax unified
	.thumb
	.set sub_080dbb78, 0x080dbb78
	.global Func_080dbdd4
	.thumb_func
Func_080dbdd4:
	push	{r5, lr}
	adds	r5, r3, #0
	bl	sub_080dbb78
	strb	r5, [r0, #2]
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
	adds	r0, r5, #0
	pop	{r5, pc}
	.align 2, 0
