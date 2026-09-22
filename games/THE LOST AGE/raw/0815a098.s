.syntax unified
	.thumb
	.set sub_0815a110, 0x0815a110
	.global Overlay_0815a098
Overlay_0815a098:
	push	{lr}
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_0815a0a8
	movs	r1, #3
	bl	sub_0815a110
	b.n	.L_0815a0ba
.L_0815a0a8:
	cmp	r3, #1
	bne.n	.L_0815a0b4
	movs	r1, #4
	bl	sub_0815a110
	b.n	.L_0815a0ba
.L_0815a0b4:
	movs	r1, #5
	bl	sub_0815a110
.L_0815a0ba:
	pop	{pc}
	push	{lr}
	movs	r1, #9
	bl	sub_0815a110
	pop	{pc}
