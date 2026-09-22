.syntax unified
	.thumb
	.global Overlay_080fb700
Overlay_080fb700:
	ldr	r0, [r1, #64]
	ldrb	r2, [r0, #0]
	adds	r0, r1, #0
	adds	r0, #44
	strb	r2, [r0, #0]
	ldr	r0, [r1, #64]
	adds	r0, #1
	str	r0, [r1, #64]
	bx	lr
