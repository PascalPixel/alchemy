.syntax unified
	.thumb
	.global Func_08041b68
	.thumb_func
Func_08041b68:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #60]
	movs	r3, #30
	ldrb	r7, [r5, #2]
	adds	r4, r5, #0
	adds	r4, #8
	mov	r9, r3
	movs	r6, #20
.L_08041b84:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_08041bde
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	mov	sl, r3
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r8, r3
	adds	r3, #128
	mov	lr, r3
	movs	r3, #255
	mov	r0, r9
	mov	ip, r3
.L_08041ba4:
	ldrh	r3, [r4, #0]
	mov	r1, sl
	ands	r1, r3
	adds	r3, r1, #0
	subs	r3, #128
	adds	r4, #2
	cmp	r3, #127
	bls.n	.L_08041bc0
	cmp	r7, #0
	beq.n	.L_08041bd8
	cmp	r1, r8
	bls.n	.L_08041bd8
	cmp	r1, lr
	bhi.n	.L_08041bd8
.L_08041bc0:
	mov	r3, ip
	ands	r1, r3
	movs	r3, #128
	eors	r1, r3
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r3, #56
	adds	r1, r1, r3
	ldrb	r3, [r5, r1]
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r5, r1]
.L_08041bd8:
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L_08041ba4
.L_08041bde:
	subs	r6, #1
	cmp	r6, #0
	bne.n	.L_08041b84
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r3, #56
	movs	r1, #0
	movs	r6, #255
	adds	r2, r5, r3
.L_08041bf0:
	ldrb	r3, [r2, #0]
	cmp	r3, #1
	bne.n	.L_08041bf8
	strb	r1, [r2, #0]
.L_08041bf8:
	subs	r6, #1
	adds	r2, #1
	cmp	r6, #0
	bge.n	.L_08041bf0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.align 2, 0
