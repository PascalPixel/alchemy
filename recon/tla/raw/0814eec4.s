.syntax unified
	.thumb
	.set sub_0814ef44, 0x0814ef44
	.global Func_0814eec4
	.thumb_func
Func_0814eec4:
	push	{lr}
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_0814eed4
	movs	r1, #3
	bl	sub_0814ef44
	b.n	.L_0814eeda
.L_0814eed4:
	movs	r1, #4
	bl	sub_0814ef44
.L_0814eeda:
	pop	{pc}
	push	{lr}
	movs	r1, #4
	bl	sub_0814ef44
	pop	{pc}
	.2byte 0x0000
