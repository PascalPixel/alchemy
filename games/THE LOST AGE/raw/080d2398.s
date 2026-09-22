.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080caa4c, 0x080caa4c
	.set sub_080cad84, 0x080cad84
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2394, 0x080d2394
	.global Overlay_080d2398
Overlay_080d2398:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_080d2394
	adds	r0, r5, #0
	bl	sub_080caa4c
	movs	r0, #1
	bl	sub_08013560
	bl	sub_080cdf5c
	bl	sub_080cad84
	pop	{r5, pc}
