.syntax unified
	.thumb
	.global Func_081c3388
	.thumb_func
Func_081c3388:
	ldr	r0, [r1, #64]
	ldrb	r2, [r0, #0]
	adds	r0, r1, #0
	adds	r0, #36
	strb	r2, [r0, #0]
	ldr	r0, [r1, #64]
	adds	r0, #1
	str	r0, [r1, #64]
	bx	lr
	.2byte 0x0000
