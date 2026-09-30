.syntax unified
	.thumb
	.balign 4
	.global Func_080d2a3c
	.thumb_func
Func_080d2a3c:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r5, r2
	adds	r2, #8
	ldr	r0, [r3, #0]
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	bl	Func_080d01cc
	movs	r3, #217
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	pop	{r5, pc}
	.2byte 0x0000