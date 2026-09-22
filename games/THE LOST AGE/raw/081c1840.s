.syntax unified
	.thumb
	.set sub_081c1750, 0x081c1750
	.global Overlay_081c1840
Overlay_081c1840:
	mov	ip, lr
	bl	sub_081c1750
	strb	r3, [r1, #18]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
