.syntax unified
	.thumb
	.global Overlay_0803ccd0
Overlay_0803ccd0:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r7, r1, #0
	ldr	r1, [r3, #60]
	movs	r3, #152
	lsls	r3, r3, #5
	movs	r4, #152
	adds	r3, #76
	lsls	r4, r4, #5
	adds	r6, r0, #0
	movs	r5, #8
	movs	r0, #0
	adds	r2, r1, r3
	adds	r4, #108
.L_0803ccee:
	ldrh	r3, [r4, r1]
	cmp	r3, #0
	bne.n	.L_0803ccfa
	str	r6, [r2, #0]
	strh	r7, [r4, r1]
	b.n	.L_0803cd04
.L_0803ccfa:
	adds	r0, #1
	adds	r2, #4
	adds	r4, #2
	cmp	r0, r5
	bne.n	.L_0803ccee
.L_0803cd04:
	pop	{r5, r6, r7, pc}
