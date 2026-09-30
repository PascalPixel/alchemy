.syntax unified
	.thumb
	.global Func_08045528
	.thumb_func
Func_08045528:
	push	{lr}
	sub	sp, #16
	str	r1, [sp, #12]
	cmp	r0, #7
	bls.n	.L_08045534
	movs	r0, #0
.L_08045534:
	bl	Func_0803d2f0
	movs	r1, #14
	str	r1, [sp, #0]
	movs	r1, #1
	add	r2, sp, #12
	add	r3, sp, #8
	str	r1, [sp, #4]
	movs	r1, #0
	bl	0x0803dab0
	ldr	r0, [sp, #8]
	add	sp, #16
	pop	{pc}