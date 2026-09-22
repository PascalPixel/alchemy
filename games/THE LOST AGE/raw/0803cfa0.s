.syntax unified
	.thumb
	.global Overlay_0803cfa0
Overlay_0803cfa0:
	push	{r5, r6, lr}
	movs	r6, #4
	movs	r5, #0
	negs	r6, r6
.L_0803cfa8:
	ldrh	r3, [r0, #0]
	adds	r4, r1, #0
	ands	r3, r6
	lsls	r3, r3, #16
	adds	r0, #2
	cmp	r3, #0
	beq.n	.L_0803cfc4
.L_0803cfb6:
	cmp	r3, #0
	bge.n	.L_0803cfbc
	strb	r2, [r4, #0]
.L_0803cfbc:
	lsls	r3, r3, #1
	adds	r4, #1
	cmp	r3, #0
	bne.n	.L_0803cfb6
.L_0803cfc4:
	adds	r5, #1
	adds	r1, #192
	cmp	r5, #14
	bne.n	.L_0803cfa8
	pop	{r5, r6, pc}
