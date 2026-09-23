.syntax unified
	.thumb
	.set sub_080aec04, 0x080aec04
	.global Func_080aec68
	.thumb_func
Func_080aec68:
	push	{lr}
	bl	sub_080aec04
	ldrb	r1, [r0, #2]
	movs	r0, #1
	cmp	r1, #1
	beq.n	.L_080aeca0
	movs	r0, #2
	cmp	r1, #2
	beq.n	.L_080aeca0
	cmp	r1, #3
	beq.n	.L_080aeca0
	cmp	r1, #4
	beq.n	.L_080aeca0
	cmp	r1, #5
	beq.n	.L_080aeca0
	cmp	r1, #9
	beq.n	.L_080aeca0
	movs	r0, #1
	cmp	r1, #7
	beq.n	.L_080aeca0
	movs	r3, #10
	eors	r3, r1
	negs	r2, r3
	orrs	r2, r3
	lsrs	r0, r2, #31
	movs	r3, #1
	subs	r0, r3, r0
.L_080aeca0:
	pop	{pc}
	.2byte 0x0000
