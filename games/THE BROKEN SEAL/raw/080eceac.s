.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080b5078, 0x080b5078
	.set sub_080d4604, 0x080d4604
	.global Overlay_080eceac
Overlay_080eceac:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r3, #36
	ldrsh	r1, [r5, r3]
	ldr	r0, [r5, #8]
	ldr	r3, [pc, #32]
	movs	r2, #24
	bl	sub_080b5078
	movs	r0, #12
	bl	sub_080030f8
	movs	r3, #3
	adds	r0, r5, #0
	movs	r1, #2
	str	r3, [r5, #24]
	bl	sub_080d4604
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00073333
