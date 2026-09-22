.syntax unified
	.thumb
	.set sub_080cad84, 0x080cad84
	.global Overlay_080ceeac
Overlay_080ceeac:
	push	{r5, lr}
	movs	r5, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080ceec2
	ldr	r0, [r0, #80]
	cmp	r0, #0
	beq.n	.L_080ceec2
	ldrb	r3, [r0, #5]
	lsrs	r5, r3, #6
.L_080ceec2:
	adds	r0, r5, #0
	pop	{r5, pc}
