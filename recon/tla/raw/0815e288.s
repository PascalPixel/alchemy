.syntax unified
	.thumb
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08015778, 0x08015778
	.set sub_08118070, 0x08118070
	.set sub_08118098, 0x08118098
	.set sub_081180d8, 0x081180d8
	.global Func_0815e288
	.thumb_func
Func_0815e288:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #48]
	mov	r8, r1
	mov	r9, r0
	bl	sub_08118098
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_081180d8
	mov	sl, r0
	bl	sub_08014de4
	adds	r1, r6, #0
	adds	r0, r6, #0
	adds	r1, #12
	adds	r5, #8
	bl	sub_080156e8
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08015778
	mov	r2, sl
	ldr	r1, [r2, #12]
	ldr	r6, [pc, #44]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c05
	mov	r0, r9
	bl	sub_08118070
	adds	r1, r0, #0
	asrs	r1, r1, #17
	adds	r0, r5, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	ldr	r3, [r2, #4]
	subs	r3, r3, r0
	str	r3, [r2, #4]
	movs	r0, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0300021c
