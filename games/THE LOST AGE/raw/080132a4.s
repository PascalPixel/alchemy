.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.global Overlay_080132a4
Overlay_080132a4:
	push	{r5, lr}
	ldr	r5, [pc, #12]
.L_080132a8:
	movs	r0, #1
	ldr	r3, [r5, #4]
	bl	sub_08013560
	b.n	.L_080132a8
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
	bx	lr
