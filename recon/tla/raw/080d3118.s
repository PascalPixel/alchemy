.syntax unified
	.thumb
	.set sub_080200e8, 0x080200e8
	.set sub_08020138, 0x08020138
	.set sub_080201c0, 0x080201c0
	.set sub_080cad84, 0x080cad84
	.global Func_080d3118
	.thumb_func
Func_080d3118:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r2, #0
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d3160
	bl	sub_08020138
	ldr	r2, [r5, #12]
	adds	r3, r7, #0
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080200e8
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d3160
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
.L_080d3160:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
