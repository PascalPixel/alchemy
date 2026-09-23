.syntax unified
	.thumb
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005268, 0x08005268
	.set sub_080b5070, 0x080b5070
	.set sub_080b5098, 0x080b5098
	.set sub_080b50d8, 0x080b50d8
	.global Func_080e3994
	.thumb_func
Func_080e3994:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #104]
	sub	sp, #12
	ldr	r6, [r3, #0]
	mov	r8, r1
	mov	r9, r0
	bl	sub_080b5098
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080b50d8
	mov	sl, r0
	bl	sub_080049ac
	adds	r1, r6, #0
	adds	r0, r6, #0
	adds	r1, #12
	adds	r5, #8
	bl	sub_080051d8
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08005268
	mov	r2, sl
	ldr	r5, [pc, #56]
	ldr	r1, [r2, #24]
	mov	ip, pc
	bx	r5
	adds	r6, r0, #0
	mov	r0, r9
	bl	sub_080b5070
	adds	r1, r0, #0
	asrs	r1, r1, #17
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r5
	mov	r2, r8
	ldr	r3, [r2, #4]
	subs	r3, r3, r0
	str	r3, [r2, #4]
	movs	r0, #0
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e80
	.4byte 0x03000118
