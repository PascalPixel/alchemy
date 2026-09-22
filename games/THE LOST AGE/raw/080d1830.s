.syntax unified
	.thumb
	.global Overlay_080d1830
Overlay_080d1830:
	push	{lr}
	movs	r3, #248
	lsls	r3, r3, #7
	cmp	r0, r3
	ble.n	.L_080d183c
	adds	r0, r3, #0
.L_080d183c:
	pop	{pc}
