.syntax unified
	.thumb
	.global Func_080cc9ac
	.thumb_func
Func_080cc9ac:
	adds	r1, r0, #0
	ldr	r2, [r1, #0]
	ldrb	r0, [r2, #0]
	adds	r2, #1
	str	r2, [r1, #0]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	lsls	r3, r3, #8
	orrs	r0, r3
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	str	r2, [r1, #0]
	bx	lr
	.align 2, 0
