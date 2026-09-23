.syntax unified
	.thumb
	.global Func_081c3418
	.thumb_func
Func_081c3418:
	ldr	r0, [r1, #64]
	ldrb	r0, [r0, #0]
	adds	r2, r1, #0
	adds	r2, #39
	strb	r0, [r2, #0]
	ldr	r0, [r1, #64]
	adds	r0, #1
	str	r0, [r1, #64]
	bx	lr
	.align 2, 0
