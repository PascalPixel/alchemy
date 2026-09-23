.syntax unified
	.thumb
	.set sub_0800307c, 0x0800307c
	.global Func_08021e28
	.thumb_func
Func_08021e28:
	push	{lr}
	ldr	r2, [pc, #20]
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r2, [pc, #16]
	movs	r0, #2
	movs	r1, #136
	bl	sub_0800307c
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x04000012
	.4byte 0x08021e15
