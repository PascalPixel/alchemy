.syntax unified
	.thumb
	.global Func_08039500
	.thumb_func
Func_08039500:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_0803950c
	ldr	r3, [r0, #4]
	str	r1, [r3, #0]
	str	r1, [r0, #4]
.L_0803950c:
	pop	{pc}
	.align 2, 0
