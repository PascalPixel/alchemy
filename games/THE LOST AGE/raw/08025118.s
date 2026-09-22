.syntax unified
	.thumb
	.set sub_08024738, 0x08024738
	.global Overlay_08025118
Overlay_08025118:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #104]
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #12]
	ldr	r3, [r3, #16]
	bl	sub_08024738
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r5, #4]
	pop	{r5, pc}
