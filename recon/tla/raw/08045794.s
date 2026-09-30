.syntax unified
	.thumb
	.global Func_08045794
	.thumb_func
Func_08045794:
	push	{lr}
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r3, #0
	adds	r2, #18
	strh	r3, [r2, #0]
	ldr	r2, [pc, #8]
	movs	r0, #2
	movs	r1, #136
	bl	0x08013438
	pop	{pc}
	.2byte 0x5781
	.2byte 0x0804
