.syntax unified
	.thumb
	.set sub_081c0010, 0x081c0010
	.global Func_080d2c64
	.thumb_func
Func_080d2c64:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #184
	adds	r3, r3, r2
	adds	r5, r0, #0
	strh	r5, [r3, #0]
	movs	r2, #1
	lsls	r3, r5, #16
	asrs	r3, r3, #16
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080d2c88
	movs	r5, #195
	lsls	r5, r5, #1
.L_080d2c88:
	movs	r0, #149
	lsls	r0, r0, #1
	bl	sub_081c0010
	adds	r0, r5, #0
	bl	sub_081c0010
	pop	{r5, pc}
	bx	lr
	.2byte 0x0000
