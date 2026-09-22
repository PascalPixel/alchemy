.syntax unified
	.thumb
	.set sub_081c1750, 0x081c1750
	.global Overlay_081c1854
Overlay_081c1854:
	mov	ip, lr
	bl	sub_081c1750
	subs	r3, #64
	strb	r3, [r1, #20]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	mov	ip, lr
	bl	sub_081c1750
	subs	r3, #64
	strb	r3, [r1, #14]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	mov	ip, lr
	bl	sub_081c1750
	strb	r3, [r1, #15]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
