.syntax unified
	.thumb
	.global Overlay_080db9a8
Overlay_080db9a8:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #8
	cmp	r1, r3
	bne.n	.L_080db9bc
	ldr	r3, [r0, #80]
	ldrb	r0, [r3, #9]
	lsls	r0, r0, #28
	lsrs	r0, r0, #30
	b.n	.L_080db9be
.L_080db9bc:
	movs	r0, #0
.L_080db9be:
	pop	{pc}
	ldr	r3, [r0, #80]
	ldrb	r0, [r3, #9]
	lsls	r0, r0, #28
	lsrs	r0, r0, #30
	bx	lr
