.syntax unified
	.thumb
	.set sub_081c1750, 0x081c1750
	.global Overlay_081c1890
Overlay_081c1890:
	mov	ip, lr
	bl	sub_081c1750
	strb	r3, [r1, #27]
	bx	ip
