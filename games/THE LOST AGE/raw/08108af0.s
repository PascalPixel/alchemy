.syntax unified
	.thumb
	.set sub_08108a88, 0x08108a88
	.global Overlay_08108af0
Overlay_08108af0:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r4, [r3, #0]
	cmp	r0, #0
	beq.n	.L_08108b16
	movs	r5, #12
	ldrsh	r3, [r0, r5]
	lsls	r3, r3, #3
	adds	r3, r1, r3
	adds	r1, r3, #0
	movs	r5, #14
	ldrsh	r3, [r0, r5]
	adds	r1, #8
	lsls	r3, r3, #3
	adds	r3, r2, r3
	adds	r2, r3, #0
	adds	r2, #8
.L_08108b16:
	movs	r3, #128
	movs	r5, #160
	lsls	r3, r3, #3
	lsls	r5, r5, #3
	adds	r3, #220
	adds	r5, #5
	adds	r0, r4, r3
	adds	r3, r4, r5
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	bl	sub_08108a88
	pop	{r5, pc}
