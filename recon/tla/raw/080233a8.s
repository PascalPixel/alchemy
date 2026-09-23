.syntax unified
	.thumb
	.global ObjectDispatch_Initialize
	.global Func_080233a8
	.thumb_func
ObjectDispatch_Initialize:
Func_080233a8:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080233cc
	ldr	r3, [pc, #24]
	movs	r2, #0
	strh	r2, [r0, #4]
	adds	r2, r0, #0
	adds	r2, #91
	str	r1, [r0, #0]
	strb	r3, [r2, #0]
	adds	r2, #2
	strb	r3, [r2, #0]
	subs	r2, #6
	strb	r3, [r2, #0]
	b.n	.L_080233cc
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080233cc:
	pop	{pc}
	.2byte 0x0000
