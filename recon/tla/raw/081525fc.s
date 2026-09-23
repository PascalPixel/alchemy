.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08118098, 0x08118098
	.set sub_0815265c, 0x0815265c
	.global Func_081525fc
	.thumb_func
Func_081525fc:
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
	movs	r1, #10
	bl	sub_0815265c
	adds	r0, r5, #0
	movs	r1, #16
	bl	sub_08020098
	pop	{r5, r6, pc}
	.align 2, 0
