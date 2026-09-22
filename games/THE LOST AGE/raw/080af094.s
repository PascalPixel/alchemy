.syntax unified
	.thumb
	.set sub_080aec04, 0x080aec04
	.global Overlay_080af094
Overlay_080af094:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #1
	lsls	r2, r2, #2
	adds	r3, #255
	adds	r5, r0, #0
	adds	r7, r1, #0
	movs	r6, #0
	mov	r8, r2
	mov	sl, r3
	adds	r5, #216
.L_080af0b2:
	ldrh	r3, [r5, #0]
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080af0d0
	ldrh	r0, [r5, #0]
	bl	sub_080aec04
	ldrb	r3, [r0, #2]
	cmp	r3, r7
	bne.n	.L_080af0d0
	ldrh	r3, [r5, #0]
	mov	r0, sl
	ands	r0, r3
	b.n	.L_080af0da
.L_080af0d0:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #14
	ble.n	.L_080af0b2
	movs	r0, #0
.L_080af0da:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
