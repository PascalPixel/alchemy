.syntax unified
	.thumb
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.global Func_0803f624
	.thumb_func
Func_0803f624:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #84]
	adds	r5, r0, #0
	mov	r8, r3
	adds	r6, r1, #0
	bl	sub_080143ac
	mov	r2, r8
	str	r0, [r6, #0]
	movs	r1, #128
	bl	sub_080142d4
	ldr	r3, [pc, #56]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #60]
	ldrb	r1, [r5, #7]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	movs	r0, #13
	ldrb	r3, [r5, #5]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r2, r3
	movs	r3, #17
	negs	r3, r3
	ands	r2, r3
	movs	r3, #32
	orrs	r2, r3
	movs	r3, #4
	negs	r3, r3
	ands	r2, r3
	subs	r3, #59
	ands	r3, r1
	movs	r1, #63
	ands	r3, r1
	strb	r3, [r5, #7]
	ands	r2, r1
	movs	r3, #128
	orrs	r2, r3
	b.n	.L_0803f688
	.4byte 0x000003ff
	.4byte 0x0805c5c4
	.2byte 0xfc00
	.2byte 0xffff
.L_0803f688:
	ldrb	r3, [r5, #9]
	strb	r2, [r5, #5]
	ands	r0, r3
	strb	r0, [r5, #9]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.align 2, 0
