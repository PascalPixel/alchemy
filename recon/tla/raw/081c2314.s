.syntax unified
	.thumb
	.set sub_0801787c, 0x0801787c
	.global Func_081c2314
	.thumb_func
Func_081c2314:
	push	{lr}
	ldr	r1, [pc, #12]
	ldr	r1, [r1, #0]
	bl	sub_0801787c
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02006888
