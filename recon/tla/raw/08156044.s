.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08118098, 0x08118098
	.set sub_08156140, 0x08156140
	.global Func_08156044
	.thumb_func
Func_08156044:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r0, [r6, #8]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	movs	r1, #2
	adds	r0, r5, #0
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #48
	bl	sub_08020098
	adds	r0, r6, #0
	movs	r1, #9
	bl	sub_08156140
	adds	r0, r5, #0
	movs	r1, #16
	bl	sub_08020098
	pop	{r5, r6, pc}
	.2byte 0x0000
