.syntax unified
	.thumb
	.global Func_080d2398
	.thumb_func
Func_080d2398:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	Func_080d2394
	adds	r0, r5, #0
	bl	0x080caa4c
	movs	r0, #1
	bl	WaitFrames
	bl	0x080cdf5c
	bl	ObjectTable_Get
	pop	{r5, pc}
	.2byte 0x0000
