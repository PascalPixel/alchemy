.syntax unified
	.thumb
	.global Overlay_080fb768
Overlay_080fb768:
	ldr	r0, [r1, #64]
	ldrb	r0, [r0, #0]
	adds	r2, r1, #0
	adds	r2, #38
	strb	r0, [r2, #0]
	ldr	r0, [r1, #64]
	adds	r0, #1
	str	r0, [r1, #64]
	bx	lr
