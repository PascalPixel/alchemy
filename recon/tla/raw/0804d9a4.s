.syntax unified
	.thumb
	.global Func_0804d9a4
	.thumb_func
Func_0804d9a4:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	0x0804d0dc
	movs	r0, #25
	bl	0x0804d38c
	movs	r0, #26
	bl	0x0804d38c
	movs	r0, #27
	bl	0x0804d38c
	movs	r0, #28
	bl	0x0804d38c
	movs	r1, #10
	movs	r2, #0
	movs	r0, #17
	bl	0x0804d3e8
	adds	r0, r5, #0
	bl	0x0804d16c
	adds	r5, r0, #0
	bl	0x0804d118
	adds	r0, r5, #0
	pop	{r5, pc}
	.2byte 0x0000
