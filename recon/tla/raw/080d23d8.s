.syntax unified
	.thumb
	.global Func_080d23d8
	.thumb_func
Func_080d23d8:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	Owner_GetState
	adds	r0, r5, #0
	bl	0x080ad0f8
	cmp	r6, #0
	beq.n	.L_080d23f4
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	0x08038330
.L_080d23f4:
	pop	{r5, r6, pc}
	.2byte 0x0000
