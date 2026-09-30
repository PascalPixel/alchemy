.syntax unified
	.thumb
	.balign 4
	.global Func_080aff28
	.thumb_func
Func_080aff28:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r7, [pc, #68]
	movs	r3, #28
	adds	r3, r3, r7
	mov	sl, r3
	movs	r3, #1
	sub	sp, #4
	adds	r5, r0, #0
	movs	r1, #0
	mov	r8, r3
.L_080aff42:
	movs	r0, #0
	ldrb	r6, [r7, #0]
	str	r1, [sp, #0]
	bl	Trade_GetOfferState
	ldr	r3, [r0, #0]
	mov	r2, r8
	lsls	r2, r6
	ands	r3, r2
	adds	r7, #1
	ldr	r1, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080aff62
	strb	r6, [r5, #0]
	adds	r1, #1
	adds	r5, #1
.L_080aff62:
	cmp	r7, sl
	bls.n	.L_080aff42
	movs	r3, #32
	adds	r0, r1, #0
	strb	r3, [r5, #0]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x14ec
	.2byte 0x080c