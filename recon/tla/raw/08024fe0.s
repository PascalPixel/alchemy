.syntax unified
	.thumb
	.set sub_0802471c, 0x0802471c
	.set sub_0802d45c, 0x0802d45c
	.global Func_08024fe0
	.thumb_func
Func_08024fe0:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r3, #4
	ldrsh	r2, [r6, r3]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r1}
	ldmia	r3!, {r5}
	ldr	r3, [r3, #0]
	adds	r2, r5, #0
	bl	sub_0802471c
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_0802502c
	adds	r3, r6, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802502c
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r2, [r6, #16]
	ldr	r1, [r6, #8]
	bl	sub_0802d45c
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #20]
	str	r0, [r6, #20]
	subs	r3, r3, r2
	adds	r3, r3, r0
	str	r3, [r6, #12]
.L_0802502c:
	ldrh	r3, [r6, #4]
	movs	r0, #1
	adds	r3, #4
	strh	r3, [r6, #4]
	pop	{r5, r6, pc}
	.2byte 0x0000
