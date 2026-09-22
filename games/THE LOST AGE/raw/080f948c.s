.syntax unified
	.thumb
	.set sub_080380b8, 0x080380b8
	.global Overlay_080f948c
Overlay_080f948c:
	push	{lr}
	movs	r0, #2
	bl	sub_080380b8
	pop	{pc}
