.syntax unified
	.thumb
	.set sub_081c1750, 0x081c1750
	.global Overlay_081c17dc
Overlay_081c17dc:
	mov	ip, lr
	bl	sub_081c1750
	strb	r3, [r1, #29]
	bx	ip
