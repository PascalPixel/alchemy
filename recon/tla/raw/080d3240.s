.syntax unified
	.thumb
	.global Object_SetModeById
	.global Func_080d3240
	.thumb_func
Object_SetModeById:
Func_080d3240:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	ObjectTable_Get
	cmp	r0, #0
	beq.n	.L_080d3252
	adds	r1, r5, #0
	bl	Object_SetMode
.L_080d3252:
	pop	{r5, pc}
