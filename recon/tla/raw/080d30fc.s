.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_08020150, 0x08020150
	.set sub_080cad84, 0x080cad84
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.global Func_080d30fc
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
Func_080d30fc:
	push	{r5, lr}
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d3114
	bl	sub_08020150
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020090
.L_080d3114:
	pop	{r5, pc}
	.2byte 0x0000
