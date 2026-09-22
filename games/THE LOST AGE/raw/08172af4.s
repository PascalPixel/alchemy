.syntax unified
	.thumb
	.set sub_08172b00, 0x08172b00
	.global Overlay_08172af4
Overlay_08172af4:
	push	{lr}
	movs	r1, #1
	bl	sub_08172b00
	pop	{pc}
