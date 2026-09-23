.syntax unified
	.thumb
	.global Func_080d174c
	.thumb_func
Func_080d174c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080d175c
	strh	r0, [r3, #0]
.L_080d175c:
	pop	{pc}
	.align 2, 0
