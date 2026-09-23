.syntax unified
	.thumb
	.set sub_080dbb78, 0x080dbb78
	.global Func_080dbcc0
	.thumb_func
Func_080dbcc0:
	push	{lr}
	movs	r2, #2
	bl	sub_080dbb78
	ldrb	r3, [r0, #2]
	movs	r2, #0
	cmp	r3, #236
	bne.n	.L_080dbcd2
	movs	r2, #1
.L_080dbcd2:
	adds	r0, r2, #0
	pop	{pc}
	.2byte 0x0000
