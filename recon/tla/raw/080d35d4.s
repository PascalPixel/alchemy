.syntax unified
	.thumb
	.set sub_080cad84, 0x080cad84
	.set sub_080d2240, 0x080d2240
	.set sub_080d3600, 0x080d3600
	.global Func_080d35d4
	.thumb_func
Func_080d35d4:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r2, #0
	bl	sub_080cad84
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080cad84
	adds	r1, r0, #0
	cmp	r6, #0
	beq.n	.L_080d35fc
	cmp	r1, #0
	beq.n	.L_080d35fc
	adds	r0, r6, #0
	bl	sub_080d3600
	adds	r0, r7, #0
	bl	sub_080d2240
.L_080d35fc:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
