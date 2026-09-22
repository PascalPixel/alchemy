.syntax unified
	.thumb
	.set sub_080ad010, 0x080ad010
	.set sub_08100d58, 0x08100d58
	.global Overlay_08100d40
Overlay_08100d40:
	push	{lr}
	bl	sub_080ad010
	ldrh	r3, [r0, #40]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_08100d58
	pop	{pc}
