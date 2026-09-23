.syntax unified
	.thumb
	.set sub_0811a31c, 0x0811a31c
	.set sub_0811b9d4, 0x0811b9d4
	.global Func_0811a468
	.thumb_func
Func_0811a468:
	push	{r5, lr}
	sub	sp, #28
	mov	r5, sp
	adds	r1, r5, #0
	movs	r0, #3
	bl	sub_0811a31c
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0811b9d4
	add	sp, #28
	pop	{r5, pc}
	.2byte 0x0000
