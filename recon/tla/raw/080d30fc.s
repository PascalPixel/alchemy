.syntax unified
	.thumb
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.global Func_080d30fc
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
Func_080d30fc:
	push	{r5, lr}
	bl	ObjectTable_Get
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d3114
	bl	Object_CommitPosition
	adds	r0, r5, #0
	movs	r1, #1
	bl	Object_SetMode
.L_080d3114:
	pop	{r5, pc}
	.2byte 0x0000
