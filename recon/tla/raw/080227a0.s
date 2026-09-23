.syntax unified
	.thumb
	.global Func_080227a0
	.thumb_func
Func_080227a0:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080227ae
	ldr	r3, [r0, #40]
	strb	r1, [r3, #5]
	movs	r3, #1
	strb	r3, [r0, #25]
.L_080227ae:
	pop	{pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	cmp	r5, #0
	beq.n	.L_080227dc
	ldrb	r4, [r5, #27]
	movs	r0, #0
	cmp	r0, r4
	bge.n	.L_080227d8
	adds	r1, r5, #0
	adds	r1, #40
.L_080227c6:
	ldmia	r1!, {r2}
	ldrb	r3, [r2, #5]
	cmp	r3, #15
	beq.n	.L_080227d2
	strb	r6, [r2, #5]
	ldrb	r4, [r5, #27]
.L_080227d2:
	adds	r0, #1
	cmp	r0, r4
	blt.n	.L_080227c6
.L_080227d8:
	movs	r3, #1
	strb	r3, [r5, #25]
.L_080227dc:
	pop	{r5, r6, pc}
	.align 2, 0
