.syntax unified
	.thumb
	.set sub_081c0010, 0x081c0010
	.global Overlay_080f93b8
Overlay_080f93b8:
	push	{lr}
	bl	sub_081c0010
	movs	r0, #1
	pop	{pc}
