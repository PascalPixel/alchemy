.syntax unified
	.thumb
	.global Overlay_081c1e64
Overlay_081c1e64:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
