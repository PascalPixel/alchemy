.syntax unified
	.thumb
	.set sub_080d0e1c, 0x080d0e1c
	.global Overlay_080d172c
Overlay_080d172c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	adds	r4, r1, #0
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_080d1748
	movs	r3, #224
	lsls	r3, r3, #2
	adds	r2, r1, r3
	adds	r3, r4, #0
	bl	sub_080d0e1c
.L_080d1748:
	pop	{pc}
