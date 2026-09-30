.syntax unified
	.thumb
	.global Func_080237c8
	.thumb_func
Func_080237c8:
	push	{lr}
	ldr	r3, [r0, #80]
	cmp	r0, #0
	beq.n	.L_080237e4
	cmp	r3, #0
	beq.n	.L_080237e4
	movs	r2, #40
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080237e4
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	b.n	.L_080237e6
.L_080237e4:
	movs	r0, #0
.L_080237e6:
	pop	{pc}
	.global Func_080237e8
	.thumb_func
Func_080237e8:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080237f4
	ldr	r0, [r0, #80]
	bl	Func_08022f24
.L_080237f4:
	pop	{pc}
	.2byte 0x0000
