.syntax unified
	.thumb
	.global Func_0803972c
	.thumb_func
Func_0803972c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r4, #240
	ldrh	r1, [r0, #22]
	lsls	r4, r4, #4
	adds	r4, #62
	adds	r3, r2, r4
	strh	r1, [r3, #0]
	subs	r4, #2
	ldrh	r1, [r0, #24]
	adds	r3, r2, r4
	strh	r1, [r3, #0]
	movs	r1, #240
	ldrh	r3, [r0, #26]
	lsls	r1, r1, #4
	adds	r1, #56
	adds	r2, r2, r1
	strh	r3, [r2, #0]
	bx	lr
	movs	r3, #2
	strh	r3, [r0, #28]
	bx	lr
	.2byte 0x0000
