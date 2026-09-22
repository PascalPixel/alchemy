.syntax unified
	.thumb
	.global Overlay_08015ec8
Overlay_08015ec8:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r3, [r3, #0]
	movs	r4, #188
	lsls	r4, r4, #6
	adds	r2, r3, #0
	movs	r0, #0
	movs	r1, #0
	adds	r4, #239
	adds	r2, #76
.L_08015ee0:
	ldrb	r3, [r2, #0]
	adds	r1, #8
	adds	r0, r0, r3
	ldrb	r3, [r2, #1]
	adds	r0, r0, r3
	ldrb	r3, [r2, #2]
	adds	r0, r0, r3
	ldrb	r3, [r2, #3]
	adds	r0, r0, r3
	ldrb	r3, [r2, #4]
	adds	r0, r0, r3
	ldrb	r3, [r2, #5]
	adds	r0, r0, r3
	ldrb	r3, [r2, #6]
	adds	r0, r0, r3
	ldrb	r3, [r2, #7]
	adds	r2, #8
	adds	r0, r0, r3
	cmp	r1, r4
	bls.n	.L_08015ee0
	pop	{pc}
