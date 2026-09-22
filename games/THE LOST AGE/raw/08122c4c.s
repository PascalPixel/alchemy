.syntax unified
	.thumb
	.global Overlay_08122c4c
Overlay_08122c4c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #44
	adds	r1, r2, r3
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_08122c84
	movs	r3, #1
	str	r3, [r1, #0]
	cmp	r0, #0
	beq.n	.L_08122c84
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #78
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_08122c84
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #76
	adds	r3, r2, r1
	strh	r0, [r3, #0]
.L_08122c84:
	pop	{pc}
