.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08020090, 0x08020090
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dd528, 0x080dd528
	.set sub_080dd63c, 0x080dd63c
	.set sub_080dd668, 0x080dd668
	.global Func_080dd4e8
	.thumb_func
Func_080dd4e8:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #12
	ldr	r5, [r3, #16]
	bl	sub_080dc294
	adds	r0, r5, #0
	bl	sub_080dd528
	adds	r5, r0, #0
	bl	sub_080dd63c
	cmp	r5, #0
	beq.n	.L_080dd518
	adds	r0, r5, #0
	movs	r1, #4
	bl	sub_08020090
	movs	r0, #30
	bl	sub_08013560
.L_080dd518:
	bl	sub_080dc384
	adds	r0, r5, #0
	bl	sub_080dd668
	add	sp, #12
	pop	{r5, pc}
	.align 2, 0
