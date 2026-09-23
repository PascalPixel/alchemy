.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08020098, 0x08020098
	.set sub_080cad84, 0x080cad84
	.global Object_SetActionById
	.global Func_080d3254
	.thumb_func
Object_SetActionById:
Func_080d3254:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080d3266
	adds	r1, r5, #0
	bl	sub_08020098
.L_080d3266:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	sub	sp, #4
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080d32b2
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d32b2
	ldr	r5, [r0, #80]
	mov	r6, sp
	ldrb	r3, [r5, #24]
	movs	r7, #0
	str	r3, [r6, #0]
	b.n	.L_080d328c
.L_080d328a:
	adds	r7, #1
.L_080d328c:
	cmp	r7, #89
	bgt.n	.L_080d32b2
	movs	r0, #1
	bl	sub_08013560
	ldrb	r2, [r5, #24]
	ldr	r3, [r6, #0]
	cmp	r3, r2
	bne.n	.L_080d32b2
	ldr	r3, [r5, #40]
	cmp	r3, #0
	beq.n	.L_080d328a
	ldr	r2, [r3, #16]
	cmp	r2, #0
	beq.n	.L_080d328a
	ldrb	r3, [r3, #20]
	ldrb	r3, [r2, r3]
	cmp	r3, #241
	bne.n	.L_080d328a
.L_080d32b2:
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.align 2, 0
