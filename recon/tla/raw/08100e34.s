.syntax unified
	.thumb
	.global Func_08100e34
	.thumb_func
Func_08100e34:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r0, #61
	movs	r1, #32
	movs	r2, #3
.L_08100e4a:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #16]
	adds	r1, #56
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L_08100e4a
	pop	{pc}
	.2byte 0x0000
