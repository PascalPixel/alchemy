.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038330, 0x08038330
	.set sub_080ad0f8, 0x080ad0f8
	.global Overlay_080d23d8
Overlay_080d23d8:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	sub_08016ca4
	adds	r0, r5, #0
	bl	sub_080ad0f8
	cmp	r6, #0
	beq.n	.L_080d23f4
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_08038330
.L_080d23f4:
	pop	{r5, r6, pc}
