.syntax unified
	.thumb
	.global Func_081c1e48
	.thumb_func
Func_081c1e48:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r2, [r1, #24]
	cmp	r2, #0
	bne.n	.L_081c1e58
	movs	r2, #12
	b.n	.L_081c1e5a
.L_081c1e58:
	movs	r2, #3
.L_081c1e5a:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
	.align 2, 0
