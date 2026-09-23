.syntax unified
	.thumb
	.global Func_081c11ac
	.thumb_func
Func_081c11ac:
	movs	r3, #7
	ands	r3, r0
	movs	r2, #1
	lsls	r2, r3
	movs	r3, #192
	lsls	r3, r3, #2
	ldr	r1, [pc, #12]
	adds	r3, #255
	ands	r3, r0
	asrs	r0, r3, #3
	ldrb	r3, [r1, r0]
	orrs	r2, r3
	strb	r2, [r1, r0]
	bx	lr
	.4byte 0x02002f20
