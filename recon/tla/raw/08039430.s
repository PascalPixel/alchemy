.syntax unified
	.thumb
	.set sub_08039510, 0x08039510
	.global Func_08039430
	.thumb_func
Func_08039430:
	push	{r5, lr}
	adds	r3, r0, #0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_08039450
	ldr	r0, [r3, #0]
	str	r3, [r3, #4]
	str	r5, [r3, #0]
	cmp	r0, #0
	beq.n	.L_08039450
.L_08039444:
	ldr	r5, [r0, #0]
	bl	sub_08039510
	adds	r0, r5, #0
	cmp	r0, #0
	bne.n	.L_08039444
.L_08039450:
	pop	{r5, pc}
	.2byte 0x0000
