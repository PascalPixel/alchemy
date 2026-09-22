.syntax unified
	.thumb
	.set sub_081a7a28, 0x081a7a28
	.global Overlay_081a81f4
Overlay_081a81f4:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	adds	r4, r1, #0
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_081a8210
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r2, r1, r3
	adds	r3, r4, #0
	bl	sub_081a7a28
.L_081a8210:
	pop	{pc}
