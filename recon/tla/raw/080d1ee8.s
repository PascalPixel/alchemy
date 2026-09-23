.syntax unified
	.thumb
	.global Func_080d1ee8
	.thumb_func
Func_080d1ee8:
.L_080d1ee8:
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r1, #1
	adds	r2, r0, #0
	negs	r1, r1
.L_080d1ef2:
	ldr	r0, [r3, #0]
	cmp	r0, r1
	beq.n	.L_080d1f04
	cmp	r0, r2
	bne.n	.L_080d1f00
	ldr	r0, [r3, #4]
	b.n	.L_080d1f04
.L_080d1f00:
	adds	r3, #8
	b.n	.L_080d1ef2
.L_080d1f04:
	pop	{pc}
	movs	r0, r0
	.2byte 0x3228
	.2byte 0x080f
	push	{lr}
	bl	.L_080d1ee8
	movs	r0, #0
	pop	{pc}
	.align 2, 0
