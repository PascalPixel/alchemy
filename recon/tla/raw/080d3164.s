.syntax unified
	.thumb
	.set sub_080200e8, 0x080200e8
	.set sub_08020138, 0x08020138
	.set sub_080201c0, 0x080201c0
	.set sub_080cad84, 0x080cad84
	.global Func_080d3164
	.thumb_func
Func_080d3164:
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
