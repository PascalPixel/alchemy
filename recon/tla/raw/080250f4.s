.syntax unified
	.thumb
	.set sub_080148e8, 0x080148e8
	.global Func_080250f4
	.thumb_func
Func_080250f4:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r2, [r5, #104]
	ldr	r3, [r5, #16]
	ldr	r0, [r2, #16]
	ldr	r1, [r2, #8]
	subs	r0, r0, r3
	ldr	r3, [r5, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	ldrh	r3, [r5, #4]
	strh	r0, [r5, #6]
	adds	r3, #1
	strh	r3, [r5, #4]
	movs	r0, #1
	pop	{r5, pc}
	.align 2, 0
