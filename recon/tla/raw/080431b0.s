.syntax unified
	.thumb
	.set sub_0800205c, 0x0800205c
	.set sub_0800206c, 0x0800206c
	.set sub_0803ae14, 0x0803ae14
	.global Func_080431b0
	.thumb_func
Func_080431b0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	movs	r1, #225
	lsls	r1, r1, #4
	sub	sp, #16
	bl	sub_0800205c
	movs	r3, #234
	lsls	r3, r3, #8
	adds	r6, r0, #0
	adds	r3, #95
	cmp	r6, r3
	bls.n	.L_080431d0
	adds	r6, r3, #0
.L_080431d0:
	adds	r0, r6, #0
	movs	r1, #60
	bl	sub_0800205c
	movs	r1, #60
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_0800206c
	mov	r8, sp
	adds	r1, r5, #0
	adds	r6, r0, #0
	movs	r2, #3
	mov	r0, r8
	bl	sub_0803ae14
	ldrb	r3, [r0, #0]
	adds	r0, #1
	strb	r3, [r7, #0]
	adds	r5, r7, #1
	ldrb	r3, [r0, #0]
	adds	r6, #100
	strb	r3, [r5, #0]
	adds	r5, #1
	ldrb	r3, [r0, #1]
	adds	r1, r6, #0
	strb	r3, [r5, #0]
	movs	r3, #58
	adds	r5, #1
	strb	r3, [r5, #0]
	mov	r0, r8
	movs	r2, #2
	bl	sub_0803ae14
	ldrb	r3, [r0, #0]
	adds	r5, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	ldrb	r3, [r0, #1]
	add	sp, #16
	strb	r3, [r5, #0]
	movs	r3, #0
	adds	r0, r7, #0
	strb	r3, [r5, #1]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
