.syntax unified
	.thumb
	.set sub_0804d0dc, 0x0804d0dc
	.set sub_0804d118, 0x0804d118
	.set sub_0804d16c, 0x0804d16c
	.set sub_0804d38c, 0x0804d38c
	.set sub_0804d3e8, 0x0804d3e8
	.global Func_0804d9a4
	.thumb_func
Func_0804d9a4:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_0804d0dc
	movs	r0, #25
	bl	sub_0804d38c
	movs	r0, #26
	bl	sub_0804d38c
	movs	r0, #27
	bl	sub_0804d38c
	movs	r0, #28
	bl	sub_0804d38c
	movs	r1, #10
	movs	r2, #0
	movs	r0, #17
	bl	sub_0804d3e8
	adds	r0, r5, #0
	bl	sub_0804d16c
	adds	r5, r0, #0
	bl	sub_0804d118
	adds	r0, r5, #0
	pop	{r5, pc}
	.align 2, 0
