.syntax unified
	.thumb
	.set sub_081c0010, 0x081c0010
	.global Func_080d2c9c
	.thumb_func
Func_080d2c9c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	cmp	r3, #0
	beq.n	.L_080d2cc0
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #184
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080d2cc0
	bl	sub_081c0010
.L_080d2cc0:
	pop	{pc}
	.align 2, 0
