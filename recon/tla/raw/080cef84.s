.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08020090, 0x08020090
	.set sub_08020218, 0x08020218
	.set sub_080cad84, 0x080cad84
	.set sub_081c0010, 0x081c0010
	.global Func_080cef84
	.thumb_func
Func_080cef84:
	push	{r5, lr}
	bl	sub_080cad84
	adds	r5, r0, #0
	movs	r0, #18
	bl	sub_08013560
	adds	r0, r5, #0
	movs	r1, #7
	bl	sub_08020090
	movs	r0, #146
	bl	sub_081c0010
	cmp	r5, #0
	beq.n	.L_080cefb2
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #40]
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020218
.L_080cefb2:
	pop	{r5, pc}
	push	{lr}
	bl	sub_080cad84
	movs	r1, #4
	bl	sub_08020090
	movs	r0, #124
	bl	sub_081c0010
	movs	r0, #12
	bl	sub_08013560
	pop	{pc}
	.2byte 0x0000
