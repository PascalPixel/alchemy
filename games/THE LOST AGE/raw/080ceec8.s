.syntax unified
	.thumb
	.set sub_08016cfc, 0x08016cfc
	.set sub_080cad84, 0x080cad84
	.set sub_080cee80, 0x080cee80
	.set sub_080e70f8, 0x080e70f8
	.global Overlay_080ceec8
Overlay_080ceec8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r7, r0, #0
	adds	r0, r6, #0
	mov	r8, r2
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080cef1c
	cmp	r7, #1
	bne.n	.L_080ceee8
	movs	r3, #0
	str	r3, [r5, #16]
.L_080ceee8:
	cmp	r7, #2
	bne.n	.L_080cef1c
	mov	r0, r8
	bl	sub_08016cfc
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq.n	.L_080ceefe
	adds	r0, r6, #0
	bl	sub_080cee80
.L_080ceefe:
	adds	r3, r5, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r0, r6, #0
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r3, r3, #16
	str	r3, [r5, #16]
	bl	sub_080e70f8
.L_080cef1c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
