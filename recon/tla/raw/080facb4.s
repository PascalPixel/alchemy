.syntax unified
	.thumb
	.set sub_08038080, 0x08038080
	.set sub_08038260, 0x08038260
	.global Func_080facb4
	.thumb_func
Func_080facb4:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	adds	r6, r1, #0
	adds	r5, #240
	ldr	r0, [r5, #0]
	bl	sub_08038260
	ldr	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	pop	{r5, r6, pc}
	.align 2, 0
