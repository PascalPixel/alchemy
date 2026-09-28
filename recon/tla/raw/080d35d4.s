.syntax unified
	.thumb
	.global Func_080d35d4
	.thumb_func
Func_080d35d4:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r2, #0
	bl	ObjectTable_Get
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	ObjectTable_Get
	adds	r1, r0, #0
	cmp	r6, #0
	beq.n	.L_080d35fc
	cmp	r1, #0
	beq.n	.L_080d35fc
	adds	r0, r6, #0
	bl	Func_080d3600
	adds	r0, r7, #0
	bl	Battle_WaitMode0
.L_080d35fc:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
