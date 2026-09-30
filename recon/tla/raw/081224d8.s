.syntax unified
	.thumb
	.balign 4
	.global Func_081224d8
	.thumb_func
Func_081224d8:
	push	{r5, lr}
	mov	r5, r9
	push	{r5}
	mov	r3, r9
	sub	sp, #4
	str	r3, [sp, #0]
	adds	r5, r0, #0
	bl	0x080ad148
	ldrb	r2, [r5, #0]
	movs	r3, #255
	ands	r0, r3
	movs	r4, #0
	movs	r1, #0
	cmp	r0, r2
	blt.n	.L_08122508
.L_081224f8:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_08122508
	ldrb	r3, [r5, r1]
	adds	r2, r2, r3
	cmp	r0, r2
	bge.n	.L_081224f8
	adds	r4, r1, #0
.L_08122508:
	adds	r0, r4, #0
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, pc}
	.2byte 0x0000
