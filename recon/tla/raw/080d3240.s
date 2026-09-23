.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_080cad84, 0x080cad84
	.global Object_SetModeById
	.global Func_080d3240
	.thumb_func
Object_SetModeById:
Func_080d3240:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080d3252
	adds	r1, r5, #0
	bl	sub_08020090
.L_080d3252:
	pop	{r5, pc}
