.syntax unified
	.thumb
	.set sub_08016d18, 0x08016d18
	.set sub_08038040, 0x08038040
	.set sub_080ad2e8, 0x080ad2e8
	.set sub_080cded4, 0x080cded4
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2350, 0x080d2350
	.global Overlay_080d2cc4
Overlay_080d2cc4:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_080d22a8
	movs	r0, #0
	bl	sub_080cded4
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08038040
	movs	r0, #161
	lsls	r0, r0, #1
	bl	sub_08016d18
	bl	sub_080d2350
	movs	r0, #136
	bl	sub_080ad2e8
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080d2d04
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
.L_080d2d04:
	pop	{r5, pc}
