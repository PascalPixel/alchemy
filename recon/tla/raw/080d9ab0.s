.syntax unified
	.thumb
	.global Func_080d9ab0
	.thumb_func
Func_080d9ab0:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r2, [r3, #0]
	cmp	r2, #0
	bne.n	.L_080d9ac4
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080d9acc
.L_080d9ac4:
	lsls	r3, r0, #5
	adds	r3, r2, r3
	movs	r2, #22
	ldrsh	r0, [r3, r2]
.L_080d9acc:
	pop	{pc}
	.align 2, 0
