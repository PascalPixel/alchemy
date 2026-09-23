.syntax unified
	.thumb
	.set sub_08038390, 0x08038390
	.set sub_080f8028, 0x080f8028
	.global Func_08108660
	.thumb_func
Func_08108660:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	adds	r5, r5, r3
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	ldrb	r6, [r0, #5]
	bl	sub_080f8028
	adds	r2, r7, #0
	movs	r1, #7
	movs	r0, #7
	bl	sub_08038390
	ldr	r3, [r5, #0]
	adds	r7, r0, #0
	strb	r6, [r3, #5]
	pop	{r5, r6, r7, pc}
	.align 2, 0
