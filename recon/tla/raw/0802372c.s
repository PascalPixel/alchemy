.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080142d4, 0x080142d4
	.set sub_08014cc0, 0x08014cc0
	.set sub_08038248, 0x08038248
	.global Func_0802372c
	.thumb_func
Func_0802372c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	cmp	r0, #0
	beq.n	.L_080237c0
	movs	r3, #0
	adds	r2, r0, #0
	adds	r2, #92
	mov	r8, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r3, r0, #0
	adds	r3, #98
	strb	r7, [r3, #0]
	ldr	r6, [r0, #80]
	cmp	r6, #0
	beq.n	.L_080237c0
	movs	r1, #193
	lsls	r1, r1, #3
	movs	r0, #68
	bl	sub_08014cc0
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_08038248
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	adds	r2, r5, #0
	movs	r1, #128
	ldrb	r0, [r6, #16]
	bl	sub_080142d4
	adds	r5, r0, #0
	movs	r0, #68
	bl	sub_0801314c
	mov	r3, r8
	strb	r3, [r6, #27]
	strb	r3, [r6, #25]
	movs	r3, #16
	strb	r3, [r6, #20]
	strb	r3, [r6, #21]
	mov	r3, r8
	strb	r3, [r6, #22]
	movs	r3, #4
	strb	r3, [r6, #23]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #12]
	ldr	r3, [pc, #32]
	ldrh	r2, [r6, #8]
	ands	r5, r3
	ldr	r3, [pc, #32]
	ands	r3, r2
	orrs	r3, r5
	strh	r3, [r6, #8]
	ldrb	r2, [r6, #5]
	movs	r3, #33
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r6, #9]
	b.n	.L_080237c0
	movs	r0, r0
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080237c0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
