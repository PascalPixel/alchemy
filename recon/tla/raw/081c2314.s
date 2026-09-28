.syntax unified
	.thumb
	.global Func_081c2314
	.thumb_func
Func_081c2314:
	push	{lr}
	ldr	r1, [pc, #12]
	ldr	r1, [r1, #0]
	bl	_call_via_r1
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02006888
