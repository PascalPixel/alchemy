.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080aec04, 0x080aec04
	.global Overlay_080af1fc
Overlay_080af1fc:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	lsls	r5, r5, #1
	adds	r6, r5, #0
	adds	r7, r0, #0
	adds	r6, #216
	ldrh	r3, [r7, r6]
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_080aec04
	movs	r1, #0
	cmp	r5, #0
	bne.n	.L_080af226
	movs	r0, #8
	b.n	.L_080af240
.L_080af226:
	ldrh	r2, [r7, r6]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080af23e
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080af23e
	movs	r1, #1
.L_080af23e:
	adds	r0, r1, #0
.L_080af240:
	pop	{r5, r6, r7, pc}
