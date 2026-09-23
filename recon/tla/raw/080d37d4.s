.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_08020138, 0x08020138
	.set sub_080cad84, 0x080cad84
	.set sub_080d2240, 0x080d2240
	.set sub_080d2e40, 0x080d2e40
	.global Func_080d37d4
	.thumb_func
Func_080d37d4:
	push	{lr}
	adds	r2, r0, #0
	movs	r0, #0
	cmp	r2, #0
	beq.n	.L_080d3808
	adds	r3, r2, #0
	adds	r3, #100
	ldrh	r3, [r3, #0]
	ldrh	r1, [r2, #6]
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #0
	beq.n	.L_080d3808
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r0, r3
	ble.n	.L_080d37fc
	movs	r0, #128
	lsls	r0, r0, #4
.L_080d37fc:
	ldr	r3, [pc, #12]
	cmp	r0, r3
	bge.n	.L_080d3804
	ldr	r0, [pc, #12]
.L_080d3804:
	adds	r3, r1, r0
	strh	r3, [r2, #6]
.L_080d3808:
	pop	{pc}
	movs	r0, r0
	.4byte 0xfffff000
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, lr}
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d3836
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	str	r3, [r5, #64]
	bl	sub_08020138
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020090
.L_080d3836:
	pop	{r5, pc}
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080d3858
	adds	r3, r0, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	ldr	r1, [pc, #12]
	bl	sub_080200a8
	adds	r0, r6, #0
	bl	sub_080d2240
.L_080d3858:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x3300
	.2byte 0x080f
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r1, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080d3880
	adds	r3, r0, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	ldr	r1, [pc, #12]
	bl	sub_080200a8
	adds	r0, r6, #0
	bl	sub_080d2e40
.L_080d3880:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x3300
	.2byte 0x080f
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080d38d0
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_080d38d0
	ldr	r1, [r6, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r5, r3
	adds	r3, r2, #0
	lsls	r4, r5, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_080d38d0:
	pop	{r5, r6, pc}
	.2byte 0x0000
