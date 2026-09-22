.syntax unified
	.thumb
	.global Overlay_081c1798
Overlay_081c1798:
	ldrb	r2, [r1, #2]
	cmp	r2, #0
	beq.n	.L_081c17aa
	subs	r2, #1
	strb	r2, [r1, #2]
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r3, #68]
	str	r2, [r1, #64]
.L_081c17aa:
	bx	lr
