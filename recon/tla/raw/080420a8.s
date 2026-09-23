.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014dac, 0x08014dac
	.set sub_0803aae4, 0x0803aae4
	.global UiText_DrawString
	.global Func_080420a8
	.thumb_func
UiText_DrawString:
Func_080420a8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	mov	r8, r2
	mov	sl, r3
	adds	r7, r1, #0
	bl	sub_08014dac
	ldrb	r3, [r5, #0]
	adds	r6, r0, #0
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L_080420d8
.L_080420ca:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	strh	r3, [r2, #0]
	adds	r2, #2
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080420ca
.L_080420d8:
	ldr	r3, [pc, #20]
	adds	r0, r6, #0
	strh	r3, [r2, #0]
	adds	r1, r7, #0
	mov	r2, r8
	mov	r3, sl
	bl	sub_0803aae4
	adds	r0, r6, #0
	bl	sub_08013164
	b.n	.L_080420f4
	.2byte 0x0000
	.2byte 0x0000
.L_080420f4:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
