.syntax unified
	.thumb
	.set sub_080cded4, 0x080cded4
	.global Overlay_080cdec8
Overlay_080cdec8:
	push	{lr}
	movs	r0, #0
	bl	sub_080cded4
	pop	{pc}
