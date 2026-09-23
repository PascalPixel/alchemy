.syntax unified
	.thumb
	.set sub_080f8888, 0x080f8888
	.global Func_08100700
	.thumb_func
Func_08100700:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #248
	adds	r5, r3, #0
	mov	r8, r2
	adds	r5, #76
	movs	r7, #168
	movs	r6, #31
.L_0810071a:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L_0810072a
	mov	r3, r8
	strh	r3, [r0, #6]
	strh	r7, [r0, #8]
	bl	sub_080f8888
.L_0810072a:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_0810071a
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
