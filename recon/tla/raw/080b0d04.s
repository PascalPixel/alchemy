.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080b0298, 0x080b0298
	.set sub_080b0c78, 0x080b0c78
	.global Djinn_Deactivate
	.global Func_080b0d04
	.thumb_func
Djinn_Deactivate:
Func_080b0d04:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	mov	sl, r2
	bl	sub_08016ca4
	adds	r1, r6, #0
	adds	r5, r0, #0
	mov	r2, sl
	adds	r0, r7, #0
	bl	sub_080b0c78
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_080b0d4e
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldrb	r3, [r5, r2]
	lsls	r1, r6, #2
	adds	r3, #255
	strb	r3, [r5, r2]
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r1, r1, r3
	movs	r2, #1
	mov	r3, sl
	lsls	r2, r3
	ldr	r3, [r5, r1]
	adds	r0, r7, #0
	bics	r3, r2
	str	r3, [r5, r1]
	bl	sub_080b0298
.L_080b0d4e:
	mov	r0, r8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
