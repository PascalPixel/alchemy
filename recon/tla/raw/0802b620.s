.syntax unified
	.thumb
	.global Func_0802b620
	.thumb_func
Func_0802b620:
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	movs r3, #241
	ldrh r1, [r4, #20]
	movs r2, #224
	lsls r3, r3, #8
	lsls r2, r2, #4
	adds r3, #255
	ands r2, r0
	ands r3, r1
	orrs r3, r2
	strh r3, [r4, #20]
	bx lr
