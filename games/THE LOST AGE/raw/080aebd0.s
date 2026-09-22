.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080aea30, 0x080aea30
	.global Overlay_080aebd0
Overlay_080aebd0:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r0, #0
	bl	sub_08016ca4
	adds	r6, r0, #0
	movs	r1, #58
	ldrsh	r3, [r6, r1]
	movs	r1, #54
	ldrsh	r2, [r6, r1]
	adds	r3, r3, r5
	adds	r1, r2, #0
	cmp	r3, r2
	bgt.n	.L_080aebf4
	movs	r1, #0
	cmp	r3, #0
	blt.n	.L_080aebf4
	adds	r1, r3, #0
.L_080aebf4:
	adds	r0, r7, #0
	strh	r1, [r6, #58]
	bl	sub_080aea30
	movs	r2, #58
	ldrsh	r0, [r6, r2]
	pop	{r5, r6, r7, pc}
