.syntax unified
	.thumb
	.global Overlay_081269bc
Overlay_081269bc:
	push	{lr}
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	movs	r0, r0
	.4byte 0x0300021c
