.syntax unified
	.thumb
	.set sub_081c1750, 0x081c1750
	.global Overlay_081c17e8
Overlay_081c17e8:
	mov	ip, lr
	bl	sub_081c1750
	lsls	r3, r3, #1
	strh	r3, [r0, #28]
	ldrh	r2, [r0, #30]
	muls	r3, r2
	lsrs	r3, r3, #8
	strh	r3, [r0, #32]
	bx	ip
	mov	ip, lr
	bl	sub_081c1750
	strb	r3, [r1, #10]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
