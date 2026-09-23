.syntax unified
	.thumb
	.global Func_081a8278
	.thumb_func
Func_081a8278:
	push	{lr}
	movs	r3, #248
	lsls	r3, r3, #7
	cmp	r0, r3
	ble.n	.L_081a8284
	adds	r0, r3, #0
.L_081a8284:
	pop	{pc}
	.align 2, 0
