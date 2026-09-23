.syntax unified
	.thumb
	.set sub_08020218, 0x08020218
	.set sub_080d2d84, 0x080d2d84
	.global Func_080d38d4
	.thumb_func
Func_080d38d4:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r0
	adds	r0, r1, #0
	bl	sub_080d2d84
	ldr	r0, [r0, #80]
	ldrb	r6, [r0, #16]
	ldrh	r5, [r0, #8]
	mov	r0, r8
	bl	sub_080d2d84
	ldr	r0, [r0, #80]
	ldr	r3, [pc, #20]
	ldrh	r2, [r0, #8]
	lsls	r5, r5, #22
	lsrs	r5, r5, #22
	ands	r3, r2
	orrs	r3, r5
	strb	r6, [r0, #16]
	strh	r3, [r0, #8]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0xffff
	push	{lr}
	bl	sub_080d2d84
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	bl	sub_08020218
	pop	{pc}
	.2byte 0x0000
