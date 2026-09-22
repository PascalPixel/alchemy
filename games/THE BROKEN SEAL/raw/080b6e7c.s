.syntax unified
	.thumb
	.global Overlay_080b6e7c
Overlay_080b6e7c:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r4, #0
	ldr	r0, [pc, #40]
.L_080b6e84:
	lsls	r1, r4, #1
	ldrh	r2, [r0, r1]
	ldr	r3, [pc, #36]
	ands	r3, r2
	cmp	r5, r3
	bne.n	.L_080b6e96
	ldrh	r3, [r0, r1]
	lsrs	r0, r3, #9
	b.n	.L_080b6ea6
.L_080b6e96:
	lsls	r3, r2, #16
	movs	r2, #1
	asrs	r3, r3, #16
	negs	r2, r2
	adds	r4, #1
	cmp	r3, r2
	bne.n	.L_080b6e84
	movs	r0, #6
.L_080b6ea6:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x080c593c
	.4byte 0x000001ff
