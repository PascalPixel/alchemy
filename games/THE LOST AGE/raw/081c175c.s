.syntax unified
	.thumb
	.set sub_081c1734, 0x081c1734
	.global Overlay_081c175c
Overlay_081c175c:
	push	{lr}
	ldr	r2, [r1, #64]
	ldrb	r0, [r2, #3]
	lsls	r0, r0, #8
	ldrb	r3, [r2, #2]
	orrs	r0, r3
	lsls	r0, r0, #8
	ldrb	r3, [r2, #1]
	orrs	r0, r3
	lsls	r0, r0, #8
	bl	sub_081c1734
	orrs	r0, r3
	str	r0, [r1, #64]
	pop	{r0}
	bx	r0
