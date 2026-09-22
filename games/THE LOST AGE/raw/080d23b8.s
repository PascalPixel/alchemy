.syntax unified
	.thumb
	.set sub_080ca6e8, 0x080ca6e8
	.set sub_080cacc0, 0x080cacc0
	.global Overlay_080d23b8
Overlay_080d23b8:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_080cacc0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080ca6e8
	pop	{r5, pc}
