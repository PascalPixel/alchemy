.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_080200e8, 0x080200e8
	.set sub_08020138, 0x08020138
	.set sub_080201c0, 0x080201c0
	.set sub_080cad84, 0x080cad84
	.global Overlay_080d3164
Overlay_080d3164:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r6, r1, #0
	adds	r7, r2, #0
	mov	r8, r3
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d31ba
	bl	sub_08020138
	ldr	r2, [r5, #12]
	adds	r3, r7, #0
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080200e8
	mov	r3, r8
	strh	r3, [r5, #6]
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d31ba
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r2, [r5, #16]
	ldr	r1, [r5, #8]
	bl	sub_080201c0
	ldr	r3, [r5, #12]
	ldr	r2, [r5, #20]
	str	r0, [r5, #20]
	subs	r3, r3, r2
	adds	r3, r3, r0
	str	r3, [r5, #12]
.L_080d31ba:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r7, r2, #0
	mov	r8, r3
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d320e
	bl	sub_08020138
	adds	r2, r7, #0
	mov	r3, r8
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080200e8
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d320e
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r2, [r5, #16]
	ldr	r1, [r5, #8]
	bl	sub_080201c0
	ldr	r3, [r5, #12]
	ldr	r2, [r5, #20]
	str	r0, [r5, #20]
	subs	r3, r3, r2
	adds	r3, r3, r0
	str	r3, [r5, #12]
.L_080d320e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	sub_080cad84
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r6, #0
	beq.n	.L_080d323e
	cmp	r5, #0
	beq.n	.L_080d323e
	ldr	r3, [r5, #16]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	adds	r0, r6, #0
	bl	sub_080200e8
	ldr	r3, [r5, #20]
	str	r3, [r6, #20]
.L_080d323e:
	pop	{r5, r6, pc}
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080d3252
	adds	r1, r5, #0
	bl	sub_08020090
.L_080d3252:
	pop	{r5, pc}
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
