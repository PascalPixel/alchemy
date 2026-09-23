.syntax unified
	.thumb
	.set sub_08124bfc, 0x08124bfc
	.global Func_08124cac
	.thumb_func
Func_08124cac:
	push	{r5, lr}
	movs	r5, #19
.L_08124cb0:
	subs	r5, #1
	bl	sub_08124bfc
	cmp	r5, #0
	bge.n	.L_08124cb0
	movs	r0, #0
	pop	{r5, pc}
	.align 2, 0
