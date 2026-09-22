.syntax unified
	.thumb
	.set sub_080231c8, 0x080231c8
	.global Overlay_08024ec8
Overlay_08024ec8:
	push	{lr}
	bl	sub_080231c8
	movs	r0, #0
	pop	{pc}
