.syntax unified
	.thumb
	.global Func_080231a4
	.thumb_func
Func_080231a4:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #20]
	movs	r0, #0
	ldr	r3, [r2, #0]
	movs	r1, #0
	b.n	.L_080231be
.L_080231b4:
	adds	r1, #1
	adds	r2, #128
	cmp	r1, #63
	bgt.n	.L_080231c4
	ldr	r3, [r2, #0]
.L_080231be:
	cmp	r3, #0
	bne.n	.L_080231b4
	adds	r0, r2, #0
.L_080231c4:
	pop	{pc}
	.align 2, 0
