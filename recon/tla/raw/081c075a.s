.syntax unified
	.thumb
	.global Func_081c075a
	.thumb_func
Func_081c075a:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r3, [r1, #24]
	movs	r2, #3
	cmp	r3, #0
	bne.n	.L_081c076a
	movs	r2, #12
.L_081c076a:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
