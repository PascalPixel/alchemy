.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.global Func_080ccd48
	.thumb_func
Func_080ccd48:
	push	{lr}
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080ccd56
	movs	r0, #1
	b.n	.L_080ccd74
.L_080ccd56:
	movs	r3, #128
	lsls	r3, r3, #5
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ccd66
	bl	sub_08016ce4
	b.n	.L_080ccd74
.L_080ccd66:
	bl	sub_08016ce4
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	movs	r0, #1
	subs	r0, r0, r3
.L_080ccd74:
	pop	{pc}
	.2byte 0x0000
